#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "plug.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "array.h"
#include "gwi.h"


#include "buffer.h"
#include "window.h"
#include "windows.h"
#include "widgets/row.h"
#include "screen.h"

#define FPS_30 33333
#define FPS_60 16667
#define FPS FPS_60

#define CONV_MTOU(x) ((x) * 1000)
#define CONV_UTOM(x) ((x) / 1000)

struct TUIMeta {
  TUIBuffer buffer;
  TUIWindows wstack;
  THREAD_TYPE thread;
  TUIConfig old_config;
  MemPool mp;
  VM *vm;
  volatile bool running;
};

#include <signal.h>
static volatile int grun = 1;
void ctrlc(int _NUSED) {
  grun = 0;
//  pthread_exit(NULL);
//  return;
  THREAD_RETURN();
}
#include "shreduler_private.h"
void* tui_func(void* arg) {
signal(SIGINT, ctrlc);
  struct TUIMeta* meta = (struct TUIMeta*)arg;
  const int timeout_msecs = 12;
  TUIEvent event = {};
  while (grun && meta->running) {
    vm_run(meta->vm);
    for(short i = 0; i < meta->wstack.count; i++)
      tui_window_draw(&meta->buffer, &meta->wstack.windows[i]);
    tui_buffer_draw(&meta->buffer);
    usleep(FPS - CONV_MTOU(timeout_msecs));
    if (tui_screen_input(&event, timeout_msecs)) {
        tui_window_receive_primitive_event(&meta->wstack.windows[meta->wstack.current], event);
    }
    tui_screen_update();
  }
  tui_screen_deconfigure(&meta->old_config);
  tui_buffer_destroy(&meta->buffer);
  return NULL;
}

#define GET_META(o) (&*(struct TUIMeta*)(o->type_ref->info->value->from->owner_class->nspc->info->class_data))

#define WINDOW(a) (*(TUIWindow**)(a->data + SZ_INT*3))
#define WIDGET(a) (*(TUIWidget*)(a->data + SZ_INT))

static CTOR(win_ctor) {
  struct TUIMeta *meta = GET_META(o);
  meta->wstack.count++;
  TUIWindow *win = WINDOW(o) = &meta->wstack.windows[0];
  if(!meta->running) {
    meta->mp = shred->info->vm->gwion->mp;
    meta->vm = new_vm(meta->mp, 0);
    meta->vm->gwion = shred->info->vm->gwion;
    if(tui_screen_configure(&meta->buffer, &meta->old_config) == EXIT_FAILURE)
      exit(3);//TODO
    THREAD_CREATE(meta->thread, tui_func, meta);
  }
  tui_window_init(win, TUIMenubarMake("A Few TUI Controls", (TUITools){}), TUIRectMake(0,0,0,0), (TUIWidgets){0, 0, NULL});
  tui_window_fullscreen(win, &meta->buffer);
  ++meta->running;
  vector_init(&*(struct Vector_*)(o->data + SZ_INT));
  *(TUIWidget**)(o->data + SZ_INT*2) = _mp_malloc(meta->mp, 64 * sizeof(TUIWidget));
}

static DTOR(win_dtor) {
  struct TUIMeta *meta = GET_META(o);
  TUIWindow *win = WINDOW(o);
  if(!--meta->running) {
    pthread_cancel(meta->thread);
    pthread_detach(meta->thread);
    free_vm(meta->vm);
  }
  win->widgets = (TUIWidgets){ 0, 0, NULL };
  struct Vector_ v = *(struct Vector_*)(o->data + SZ_INT);
  for(m_uint i = 0; i < vector_size(&v); ++i)
    _release((M_Object)vector_at(&v, i), shred);
  vector_release(&v);
  _mp_free(shred->info->vm->gwion->mp, 64 * sizeof(TUIWidget), *(TUIWidget**)(o->data + SZ_INT*2));
  tui_window_destroy(win);
}

static MFUN(Window_title_set) {
  TUIWindow *win = WINDOW(o);
  win->menu.title = STRING(*(M_Object*)MEM(SZ_INT));
}

static MFUN(Window_title_get) {
  TUIWindow *win = WINDOW(o);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion->mp, shred, (m_str)win->menu.title);
}

static MFUN(window_position) {
  TUIWindow *win = WINDOW(o);
  win->total= TUIRectMake(0, 0, *(m_uint*)MEM(SZ_INT), *(m_uint*)MEM(SZ_INT*2));
}

bool tui_window_select(TUIWindow* window, ssize_t (*array_func)(TUIWidgets*), TUIEvent event);

static INSTR(window_append) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  const M_Object w = *(M_Object*)REG(0);
  TUIWindow *win = WINDOW(o);
  const Vector v = &*(struct Vector_*)(o->data+ SZ_INT);
  const m_uint sz = vector_size(v);
  TUIWidget *e = *(TUIWidget**)(o->data + SZ_INT*2);
  e[sz] = WIDGET(w);
  ++w->ref;
  vector_add(v, (m_uint)w);
  win->widgets = (TUIWidgets){ sz + 1, win->widgets.selection, e };
  tui_window_select(win, array_next, (TUIEvent){});
}

/* TUIUser */
typedef struct TUIUser {
  TUIBuffer *buffer;
  TUIRect rect;
  TUIRect region;
  M_Object o;
  VM *vm;
  VM_Shred draw_shred;
  VM_Shred activate_shred;
  VM_Shred select_shred;
} TUIUser;

#define USER_WIDGET(o) ((TUIUser*)WIDGET((o)).user_data)

static TUI_INIT(TUIUser) {
  TUIUser *user = tui_alloc(sizeof(TUIUser));
  user->rect = TUISizeMake(1,1);
  widget->user_data = user;
}

static TUI_SIZE(TUIUser) {
  TUIUser *user = widget->user_data;
  return user->rect;
}

// set arguments in memory
static TUI_DRAW(TUIUser) {
  TUIUser* user = widget->user_data;
  VM_Shred shred = user->draw_shred;
  user->region = region;
  *(M_Object*)MEM(0) = user->o;
  shred->pc = 0;
  ++shred->info->me->ref;
  vmcode_addref(shred->code);
  shreduler_remove(user->vm->shreduler, shred, 0);
  shredule(user->vm->shreduler, shred, 0);
  vm_run(user->vm);
}

static TUI_SELECT(TUIUser) {
  TUIUser* user = widget->user_data;
  VM_Shred shred = user->select_shred;
//  user->region = region;
  *(M_Object*)MEM(0) = user->o;
  *(m_uint*)MEM(SZ_INT) = event.keycode;
  *(m_uint*)MEM(SZ_INT*2) = event.intent;
  shred->pc = 0;
  ++shred->info->me->ref;
  vmcode_addref(shred->code);
  shreduler_remove(user->vm->shreduler, shred, 0);
  shredule(user->vm->shreduler, shred, 0);
  vm_run(user->vm);
}

static MFUN(WidgetSetIsSelected) {
  *(m_uint*)RETURN = WIDGET(o).is_selected = *(m_uint*)MEM(SZ_INT);
}

static MFUN(WidgetGetIsSelected) {
  *(m_uint*)RETURN = WIDGET(o).is_selected;
}

static TUI_ACTIVATE(TUIUser) {
  TUIUser* user = widget->user_data;
  VM_Shred shred = user->activate_shred;
  *(M_Object*)MEM(0) = user->o;
  *(m_uint*)MEM(SZ_INT) = event.keycode;
  *(m_uint*)MEM(SZ_INT*2) = event.intent;
  shred->pc = 0;
  ++shred->info->me->ref;
  vmcode_addref(shred->code);
  shreduler_remove(user->vm->shreduler, shred, 0);
  shredule(user->vm->shreduler, shred, 0);
  vm_run(user->vm);
}

static TUI_DESTROY(TUIUser) {
  // ignore this
}

static CTOR(UserCtor) {
  WIDGET(o).init = TUIUser_init;
  WIDGET(o).size = TUIUser_size;
  WIDGET(o).draw = TUIUser_draw;
  WIDGET(o).select = TUIUser_select; // could be set to 1
  WIDGET(o).activate = TUIUser_activate;
  WIDGET(o).destroy = TUIUser_destroy;
  WIDGET(o).is_selected = false;
  WIDGET(o).init(&WIDGET(o));
  // construct the widget
  const Gwion gwion = shred->info->vm->gwion;
  const Type tui = str2type(gwion, "TUI", (loc_t){});
  struct TUIMeta* meta = (&*(struct TUIMeta*)(tui->nspc->info->class_data));
  USER_WIDGET(o)->vm = meta->vm;
  USER_WIDGET(o)->buffer = &meta->buffer;
  // catch the *draw* function
  const Func draw_func = (Func)vector_at(&o->vtable, 5);
  VM_Code draw_code = vmcode_callback(gwion->mp, draw_func->code);
  USER_WIDGET(o)->o = o;
  USER_WIDGET(o)->draw_shred = new_vm_shred(gwion->mp, draw_code);
  USER_WIDGET(o)->draw_shred->base = shred->base;
  vm_add_shred(USER_WIDGET(o)->vm, USER_WIDGET(o)->draw_shred);
  shreduler_remove(USER_WIDGET(o)->vm->shreduler, USER_WIDGET(o)->draw_shred, 0);
  const Func on_func = (Func)vector_at(&o->vtable, 6);
  VM_Code on_code = vmcode_callback(gwion->mp, on_func->code);
  USER_WIDGET(o)->activate_shred = new_vm_shred(gwion->mp, on_code);
  USER_WIDGET(o)->activate_shred->base = shred->base;
  vm_add_shred(USER_WIDGET(o)->vm, USER_WIDGET(o)->activate_shred);
  shreduler_remove(USER_WIDGET(o)->vm->shreduler, USER_WIDGET(o)->activate_shred, 0);
  const Func select_func = (Func)vector_at(&o->vtable, 7);
  VM_Code select_code = vmcode_callback(gwion->mp, select_func->code);
  USER_WIDGET(o)->select_shred = new_vm_shred(gwion->mp, select_code);
  USER_WIDGET(o)->select_shred->base = shred->base;
  vm_add_shred(USER_WIDGET(o)->vm, USER_WIDGET(o)->select_shred);
  shreduler_remove(USER_WIDGET(o)->vm->shreduler, USER_WIDGET(o)->select_shred, 0);
}

static CTOR(UserDtor) {
  TUIUser *user = USER_WIDGET(o);
  shreduler_remove(USER_WIDGET(o)->vm->shreduler, USER_WIDGET(o)->draw_shred, 0);
  vmcode_remref(user->draw_shred->code, user->vm->gwion);
  free_vm_shred(user->draw_shred);
  shreduler_remove(USER_WIDGET(o)->vm->shreduler, USER_WIDGET(o)->activate_shred, 0);
  vmcode_remref(user->activate_shred->code, user->vm->gwion);
  free_vm_shred(user->activate_shred);
  shreduler_remove(USER_WIDGET(o)->vm->shreduler, USER_WIDGET(o)->select_shred, 0);
  vmcode_remref(user->select_shred->code, user->vm->gwion);
  free_vm_shred(user->select_shred);
}

static MFUN(user_position) {
  TUIUser *user = USER_WIDGET(o);
  user->rect = TUISizeMake(*(m_uint*)MEM(SZ_INT), *(m_uint*)MEM(SZ_INT*2));
}

static MFUN(user_write) {
  TUIUser *user = USER_WIDGET(o);
  tui_buffer_write(user->buffer, STRING(*(M_Object*)MEM(SZ_INT)),
    user->region.x + *(size_t*)MEM(SZ_INT*2), user->region.y + *(size_t*)MEM(SZ_INT*3));
}

static MFUN(user_markup) {
  TUIUser *user = USER_WIDGET(o);
  M_Object attr = *(M_Object*)MEM(SZ_INT);
  tui_buffer_markup(user->buffer, *(TUIAttribute*)attr->data, user->region.x + *(m_uint*)MEM(SZ_INT*2),
    user->region.y + *(size_t*)MEM(SZ_INT*3), *(size_t*)MEM(SZ_INT*4));
}

static MFUN(user_set_no_select) {
  WIDGET(o).select = *(m_uint*)MEM(SZ_INT) ? (void*)1 : TUIUser_select;
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
}

static MFUN(user_get_no_select) {
  *(m_uint*)RETURN = WIDGET(o).select == (void*)1;
}

static CTOR(WidgetCtor) {
  tui_widget_init(&WIDGET(o), *(m_str*)o->type_ref->nspc->info->class_data);
  WIDGET(o).init(&WIDGET(o));
}

static CTOR(RowCtor) {
  vector_init(&*(struct Vector_*)(o->data + SZ_INT+sizeof(TUIWidget)));
  *(void**)(o->data + SZ_INT*2+sizeof(TUIWidget)) = _mp_malloc(shred->info->vm->gwion->mp, 64 * sizeof(TUIWidget));
  TUIRow *row = (TUIRow*)WIDGET(o).user_data;
  row->widgets = (TUIWidgets){};
}

static INSTR(row_append) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  const M_Object w = *(M_Object*)REG(0);
  TUIRow *row = (TUIRow*)WIDGET(o).user_data;
  struct Vector_ v = *(struct Vector_*)(o->data+ SZ_INT+sizeof(TUIWidget));
  const m_uint sz = vector_size(&v);
  TUIWidget *e = *(TUIWidget**)(o->data + SZ_INT*2+sizeof(TUIWidget));
  e[sz] = WIDGET(w);
  ++w->ref;
  vector_add(&v, (m_uint)w);
  row->widgets = (TUIWidgets){ sz + 1, row->widgets.selection, e };
  tui_row_select(row, array_next, (TUIEvent){});
}

static MFUN(Label_text_set) {
  WIDGET(o).user_data = STRING(*(M_Object*)MEM(SZ_INT));
}

static MFUN(Label_text_get) {                                             \
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion->mp, shred, WIDGET(o).user_data); \
}

struct TUIClosure_ {
  M_Object o;
  VM_Shred shred;
  VM_Code code;
};

ANN static void widget_cb(void* data, TUIEvent event) {
  struct TUIClosure_ *closure = data;
  const VM_Shred shred = new_vm_shred(closure->shred->info->vm->gwion->mp, closure->code);
  vmcode_addref(closure->code);
  shred->base = closure->shred->base;
  *(M_Object*)shred->mem = closure->o;
  vm_add_shred(closure->shred->info->vm, shred);
}

// C defined func won't work atm
static MFUN(WidgetCallback) {
  struct TUIClosure_ *closure = mp_malloc(shred->info->vm->gwion->mp, TUIClosure);
  const m_int key = *(m_int*)MEM(SZ_INT);
  const VM_Code code = *(VM_Code*)MEM(SZ_INT*2);
  closure->shred = shred;
  closure->code = vmcode_callback(shred->info->vm->gwion->mp, code);
  closure->o = o;
  if(!WIDGET(o).callbacks)
    WIDGET(o).callbacks = tui_alloc(sizeof(TUIWidgetCallback));
  WIDGET(o).callbacks->func[key] = widget_cb;
  WIDGET(o).callbacks->user_data = closure;
}

#define WIDGET_SET_INT(type, ctype, name)    \
static MFUN(type##_##name##_set) {           \
  TUIWidget w = WIDGET(o);                   \
  TUI##type *a = (TUI##type*)(w.user_data);  \
  a->name = (ctype)*(m_int*)MEM(SZ_INT);     \
}
#define WIDGET_GET_INT(type, name)           \
static MFUN(type##_##name##_get) {           \
  TUIWidget w = WIDGET(o);                   \
  TUI##type *a = (TUI##type*)(w.user_data);  \
  *(m_int*)RETURN = (m_int)a->name;          \
}

#define WIDGET_SET_FLOAT(type, name)          \
static MFUN(type##_##name##_set) {            \
  TUIWidget w = WIDGET(o);                    \
  TUI##type *a = (TUI##type*)(w.user_data);   \
  a->name = (TUIFloat)*(m_float*)MEM(SZ_INT); \
}
#define WIDGET_GET_FLOAT(type, name)        \
static MFUN(type##_##name##_get) {          \
  TUIWidget w = WIDGET(o);                  \
  TUI##type *a = (TUI##type*)(w.user_data); \
  *(m_float*)RETURN = (m_float)a->name;     \
}

#define WIDGET_SET_STRING(type, name)        \
static MFUN(type##_##name##_set) {           \
  TUIWidget w = WIDGET(o);                  \
  TUI##type *a = (TUI##type*)(w.user_data);  \
  a->name = STRING(*(M_Object*)MEM(SZ_INT)); \
}
#define WIDGET_GET_STRING(type, name)                                          \
static MFUN(type##_##name##_get) {                                             \
  TUIWidget w = WIDGET(o);                                                    \
  TUI##type *a = (TUI##type*)(w.user_data);                                    \
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion->mp, shred, a->name); \
}

#define WIDGET_SET_STRING_ARRAY(type, name)                           \
static MFUN(type##_##name##_set) {                                    \
  TUIWidget w = WIDGET(o);                                           \
  TUI##type *a = (TUI##type*)(w.user_data);                           \
  M_Object array_obj = *(M_Object*)MEM(SZ_INT);                       \
  M_Vector array = ARRAY(array_obj);                                  \
  /* TODO free/realloc */                                             \
  const m_uint n = a->length = m_vector_size(array);                  \
  m_str *strs = _mp_malloc(shred->info->vm->gwion->mp, n * SZ_INT);   \
  for(m_uint i = 0; i < n; ++i) {                                     \
    const M_Object str = *(M_Object*)(ARRAY_PTR(array) + i * SZ_INT); \
    strs[i] = mstrdup(shred->info->vm->gwion->mp, STRING(str));       \
  }                                                                   \
  a->name = strs;                                                     \
}
#define WIDGET_GET_STRING_ARRAY(TYPE, name)                                          \
static MFUN(TYPE##_##name##_get) {                               \
  TUIWidget w = WIDGET(o);                                      \
  TUI##TYPE *a = (TUI##TYPE*)(w.user_data);                      \
  const Gwion gwion = shred->info->vm->gwion;                    \
  const Type t = array_type(gwion->env, gwion->type[et_string], 1); \
  *(M_Object*)RETURN = new_array(gwion->mp, t, a->length);   \
  for(m_uint i = 0; i < a->length; ++i)                                       \
    m_vector_set(ARRAY(*(M_Object*)RETURN), i, mstrdup(gwion->mp, a->names[i])); \
}

#define WIDGET_INT(type, ctype, name) \
WIDGET_SET_INT(type, ctype, name)     \
WIDGET_GET_INT(type, name)            \

#define WIDGET_FLOAT(type, name) \
WIDGET_SET_FLOAT(type, name)     \
WIDGET_GET_FLOAT(type, name)     \

#define WIDGET_STRING(type, name) \
WIDGET_SET_STRING(type, name)     \
WIDGET_GET_STRING(type, name)     \

#define WIDGET_STRING_ARRAY(type, name) \
WIDGET_SET_STRING_ARRAY(type, name)     \
WIDGET_GET_STRING_ARRAY(type, name)     \

WIDGET_STRING(Button, text)
WIDGET_INT(Button, uint16_t, timeout)

WIDGET_STRING(Toggle, text)
WIDGET_INT(Toggle, bool, state)

WIDGET_INT(Check, bool, state)

WIDGET_FLOAT(Slider, value)
WIDGET_FLOAT(Slider, step)
WIDGET_FLOAT(Slider, min)
WIDGET_FLOAT(Slider, max)
WIDGET_INT(Slider, uint16_t, size)
WIDGET_INT(Slider, uint16_t, precision)
WIDGET_INT(Slider, int, pos) // enum
WIDGET_INT(Slider, int, dis) // enum

WIDGET_INT(Sep, char, c)
WIDGET_INT(Sep, uint16_t, size)

WIDGET_INT(Options, uint64_t, selections)
WIDGET_INT(Options, TUIOptionsSelectionType, type)
WIDGET_STRING_ARRAY(Options, names)

WIDGET_INT(Row, uint16_t, spacing)
WIDGET_INT(Row, TUIRowPositioning, positioning)

//#define USER_BUFFER(o)    *(void**)((o)->datat + sizeof(TUIWidget))
//#define USER_SHRED(o)    *(void**)((o)->datat + sizeof(TUIWidget) + SZ_INT)
//  tui_buffer_markup(buffer, attr, *(size_t*)MEM(SZ_INT*2),
//                       *(size_t*)MEM(SZ_INT*3), *(size_t)MEM(SZ_INT*4))
//static MFUN(user_write) {
//  tui_buffer_write(*(TUIBuffer**)MEM(SZ_INT), STRING(*(M_Object*)MEM(SZ_INT*2)),
//    *(size_t*)MEM(SZ_INT*3), *(size_t*)MEM(SZ_INT*4));
//}

#define TUI_INI(name, parent)                                         \
  DECL_OB(const Type, t_##name, = gwi_class_ini(gwi, #name, #parent)) \
  GWI_BB(gwi_item_ini(gwi, "@internal", "@class name"))               \
  CHECK_BB(gwi_item_end(gwi, ae_flag_static, num, 0))

#define TUI_FUNC(class, type, name)                             \
  GWI_BB(gwi_func_ini(gwi, #type, #name))                       \
  GWI_BB(gwi_func_end(gwi, class##_##name##_get, ae_flag_none)) \
  GWI_BB(gwi_func_ini(gwi, #type, #name))                       \
  GWI_BB(gwi_func_arg(gwi, #type, "a"))                         \
  GWI_BB(gwi_func_end(gwi, class##_##name##_set, ae_flag_none)) \


#define TUI_END(name, tuiclass)                          \
  GWI_BB(gwi_class_end(gwi))                             \
  *(m_str*)t_##name->nspc->info->class_data = #tuiclass; \

ANN static m_bool attr_object(const Gwi gwi, const Type t, const m_str name, const TUIAttribute attr) {
  const M_Object o = new_object(gwi->gwion->mp, NULL, t);
  *(TUIAttribute*)o->data = attr;
  GWI_BB(gwi_item_ini(gwi, "Attribute", name))
  GWI_BB(gwi_item_end(gwi, ae_flag_static | ae_flag_const | ae_flag_late, obj, o))
}

static SFUN(tui_attr0) {
  const M_Object attr = new_object_str(shred->info->vm->gwion, shred, "TUI.Attribute");
  *(TUIAttribute*)attr->data = (TUIAttribute) {
      .foreground=*(TUIColor*)MEM(0)
  };
  *(M_Object*)RETURN = attr;
}

static SFUN(tui_attr1) {
  const M_Object attr = new_object_str(shred->info->vm->gwion, shred, "TUI.Attribute");
  *(TUIAttribute*)attr->data = (TUIAttribute) {
      .foreground=*(TUIColor*)MEM(0),
      .background=*(TUIColor*)MEM(SZ_INT),
  };
  *(M_Object*)RETURN = attr;
}

static SFUN(tui_attr2) {
  const M_Object attr = new_object_str(shred->info->vm->gwion, shred, "TUI.Attribute");
  *(TUIAttribute*)attr->data = (TUIAttribute) {
      .foreground=*(TUIColor*)MEM(0),
      .background=*(TUIColor*)MEM(SZ_INT),
      .font_mode=*(TUIFontMode*)MEM(SZ_INT*2),
  };
  *(M_Object*)RETURN = attr;
}

static SFUN(tui_attr3) {
  const M_Object attr = new_object_str(shred->info->vm->gwion, shred, "TUI.Attribute");
  *(TUIAttribute*)attr->data = (TUIAttribute) {
      .foreground=*(TUIColor*)MEM(0),
      .background=*(TUIColor*)MEM(SZ_INT),
      .font_mode=*(TUIFontMode*)MEM(SZ_INT*2),
      .formatting=*(TUIFormatting*)MEM(SZ_INT*3),
  };
  *(M_Object*)RETURN = attr;
}

GWION_IMPORT(TUI) {
  DECL_OB(const Type, t_tui, = gwi_class_ini(gwi, "TUI", "Object"))
  t_tui->nspc->info->class_data_size += sizeof(struct TUIMeta);

    DECL_OB(const Type, t_attrs, = gwi_class_ini(gwi, "Attribute", NULL))
    t_attrs->nspc->info->offset += sizeof(TUIAttribute);
    CHECK_BB(attr_object(gwi, t_attrs, "reset", TUIResetAttribute))
    CHECK_BB(attr_object(gwi, t_attrs, "normal", TUINormalAttribute))
    CHECK_BB(attr_object(gwi, t_attrs, "selected", TUISelectedAttribute))
    CHECK_BB(attr_object(gwi, t_attrs, "active", TUIActivatedAttribute))
    CHECK_BB(attr_object(gwi, t_attrs, "backgroundActive", TUIBackgroundActivatedAttribute))

    GWI_BB(gwi_class_end(gwi))

    GWI_OB(gwi_struct_ini(gwi, "FontMode"))
    GWI_BB(gwi_enum_ini(gwi, "fontmode"))
    GWI_BB(gwi_enum_add(gwi, "normal", TUIFontModeNormal))
    GWI_BB(gwi_enum_add(gwi, "bold", TUIFontModeBold))
    GWI_BB(gwi_enum_add(gwi, "dim", TUIFontModeDim))
    GWI_BB(gwi_enum_add(gwi, "reset", TUI_RESETATTRIBUTES))
    GWI_BB(gwi_enum_end(gwi))
    GWI_BB(gwi_struct_end(gwi))

    GWI_OB(gwi_struct_ini(gwi, "Formatting"))
    GWI_BB(gwi_enum_ini(gwi, "formatting"))
    GWI_BB(gwi_enum_add(gwi, "none", TUIFormattingNone))
    GWI_BB(gwi_enum_add(gwi, "underline", TUIFormattingUnderline))
    GWI_BB(gwi_enum_add(gwi, "strikethrough", TUIFormattingStrikethrough))
    GWI_BB(gwi_enum_add(gwi, "understrike", TUIFormattingUnderStrike))
    GWI_BB(gwi_enum_end(gwi))
    GWI_BB(gwi_struct_end(gwi))

    GWI_OB(gwi_struct_ini(gwi, "Color"))
    GWI_BB(gwi_enum_ini(gwi, "color"))
    GWI_BB(gwi_enum_add(gwi, "black", TUIColorBlack))
    GWI_BB(gwi_enum_add(gwi, "red", TUIColorRed))
    GWI_BB(gwi_enum_add(gwi, "green", TUIColorGreen))
    GWI_BB(gwi_enum_add(gwi, "yellow", TUIColorYellow))
    GWI_BB(gwi_enum_add(gwi, "blue", TUIColorBlue))
    GWI_BB(gwi_enum_add(gwi, "magenta", TUIColorMagenta))
    GWI_BB(gwi_enum_add(gwi, "cyan", TUIColorCyan))
    GWI_BB(gwi_enum_add(gwi, "white", TUIColorWhite))
    GWI_BB(gwi_enum_add(gwi, "reset", TUIColorReset))
    GWI_BB(gwi_enum_end(gwi))
    GWI_BB(gwi_struct_end(gwi))

  //  GWI_OB(gwi_struct_ini(gwi, "Intent"))
    GWI_BB(gwi_enum_ini(gwi, "Intent"))
    GWI_BB(gwi_enum_add(gwi, "None", TUIEventIntentNone))
    GWI_BB(gwi_enum_add(gwi, "Meta", TUIEventIntentMeta))
    GWI_BB(gwi_enum_add(gwi, "Enter", TUIEventIntentEnter))
    GWI_BB(gwi_enum_add(gwi, "Leave", TUIEventIntentLeave))
    GWI_BB(gwi_enum_add(gwi, "Activate", TUIEventIntentActivate))
    GWI_BB(gwi_enum_add(gwi, "Left", TUIEventIntentLeft))
    GWI_BB(gwi_enum_add(gwi, "Right", TUIEventIntentRight))
    GWI_BB(gwi_enum_add(gwi, "Back", TUIEventIntentBack))
    GWI_BB(gwi_enum_add(gwi, "Forward", TUIEventIntentForward))
    GWI_BB(gwi_enum_add(gwi, "Previous", TUIEventIntentPrevious))
    GWI_BB(gwi_enum_add(gwi, "Next", TUIEventIntentNext))
    GWI_BB(gwi_enum_add(gwi, "First", TUIEventIntentFirst))
    GWI_BB(gwi_enum_add(gwi, "Last", TUIEventIntentLast))
    GWI_BB(gwi_enum_end(gwi))
  //  GWI_BB(gwi_struct_end(gwi))

    GWI_BB(gwi_func_ini(gwi, "Attribute", "attribute"))
    GWI_BB(gwi_func_arg(gwi, "Color.color", "foreground"))
    GWI_BB(gwi_func_end(gwi, tui_attr0, ae_flag_static))

    GWI_BB(gwi_func_ini(gwi, "Attribute", "attribute"))
    GWI_BB(gwi_func_arg(gwi, "Color.color", "foreground"))
    GWI_BB(gwi_func_arg(gwi, "Color.color", "background"))
    GWI_BB(gwi_func_end(gwi, tui_attr1, ae_flag_static))

    GWI_BB(gwi_func_ini(gwi, "Attribute", "attribute"))
    GWI_BB(gwi_func_arg(gwi, "Color.color", "foreground"))
    GWI_BB(gwi_func_arg(gwi, "Color.color", "background"))
    GWI_BB(gwi_func_arg(gwi, "FontMode", "fontmode"))
    GWI_BB(gwi_func_end(gwi, tui_attr2, ae_flag_static))

    GWI_BB(gwi_func_ini(gwi, "Attribute", "attribute"))
    GWI_BB(gwi_func_arg(gwi, "Color.color", "foreground"))
    GWI_BB(gwi_func_arg(gwi, "Color.color", "background"))
    GWI_BB(gwi_func_arg(gwi, "FontMode", "fontmode"))
    GWI_BB(gwi_func_arg(gwi, "Formatting", "format"))
    GWI_BB(gwi_func_end(gwi, tui_attr3, ae_flag_static))

    DECL_OB(const Type, t_widget, = gwi_class_ini(gwi, "Widget", "Event"))
    SET_FLAG(t_widget, abstract);
    t_widget->nspc->info->offset += sizeof(TUIWidget);
    GWI_BB(gwi_fptr_ini(gwi, "void", "FunType"))
    GWI_BB(gwi_func_arg(gwi, "TUI.Widget", "widget"))
    GWI_BB(gwi_fptr_end(gwi, ae_flag_global))

    GWI_BB(gwi_func_ini(gwi, "void", "callback"))
    GWI_BB(gwi_func_arg(gwi, "int", "key"))
    GWI_BB(gwi_func_arg(gwi, "FunType", "func"))
    GWI_BB(gwi_func_end(gwi, WidgetCallback, ae_flag_none))

    GWI_BB(gwi_func_ini(gwi, "bool", "is_selected"))
    GWI_BB(gwi_func_arg(gwi, "bool", "_"))
    GWI_BB(gwi_func_end(gwi, WidgetSetIsSelected, ae_flag_none))

    GWI_BB(gwi_func_ini(gwi, "bool", "is_selected"))
    GWI_BB(gwi_func_end(gwi, WidgetGetIsSelected, ae_flag_none))

    GWI_BB(gwi_class_end(gwi))

    DECL_OB(const Type, t_twidget, = gwi_class_ini(gwi, "TuiWidget", "Widget"))
    gwi_class_xtor(gwi, WidgetCtor, NULL);
    GWI_BB(gwi_class_end(gwi))

    TUI_INI(Label, TuiWidget)
    TUI_FUNC(Label, string, text)
    TUI_END(Label, label)

    TUI_INI(Button, TuiWidget)
    TUI_FUNC(Button, string, text)
    TUI_FUNC(Button, int, timeout)
    TUI_END(Button, button)

    TUI_INI(Toggle, TuiWidget)
    TUI_FUNC(Toggle, bool, state)
    TUI_FUNC(Toggle, string, text)
    TUI_END(Toggle, toggle)

    TUI_INI(Check, TuiWidget)
    TUI_FUNC(Check, bool, state)
    TUI_END(Check, check)

    TUI_INI(Sep, TuiWidget)
    TUI_FUNC(Sep, char, c)
    TUI_END(Sep, sep)

    TUI_INI(Slider, TuiWidget)
    TUI_FUNC(Slider, float, value)
    TUI_FUNC(Slider, float, min)
    TUI_FUNC(Slider, float, max)
    TUI_FUNC(Slider, float, step)
    TUI_FUNC(Slider, int, size)
    TUI_FUNC(Slider, int, precision)
    GWI_BB(gwi_enum_ini(gwi, "Position"))
    GWI_BB(gwi_enum_add(gwi, "Left", TUISliderValuePositionLeft))
    GWI_BB(gwi_enum_add(gwi, "Right",TUISliderValuePositionRight))
    GWI_BB(gwi_enum_end(gwi))
    TUI_FUNC(Slider, Position, pos) // should be enum
    GWI_BB(gwi_enum_ini(gwi, "Display"))
    GWI_BB(gwi_enum_add(gwi, "Percent", TUISliderValueDisplayPercent))
    GWI_BB(gwi_enum_add(gwi, "Actual",TUISliderValueDisplayActual))
    GWI_BB(gwi_enum_end(gwi))
    TUI_FUNC(Slider, Display, dis) // should be enum
    TUI_END(Slider, slider)

    TUI_INI(Options, TuiWidget)
    GWI_BB(gwi_enum_ini(gwi, "SelectionType"))
    GWI_BB(gwi_enum_add(gwi, "Multiple", TUIOptionsSelectionTypeMultiple))
    GWI_BB(gwi_enum_add(gwi, "Singular", TUIOptionsSelectionTypeSingular))
    GWI_BB(gwi_enum_end(gwi))
    TUI_FUNC(Options, string[], names)
    TUI_FUNC(Options, int, selections)
    TUI_FUNC(Options, SelectionType, selections)
    TUI_END(Options, options)

    TUI_INI(Row, TuiWidget)
    t_Row->nspc->info->offset += SZ_INT*2;
    gwi_class_xtor(gwi, RowCtor, NULL);
    TUI_FUNC(Row, int, spacing)
    GWI_BB(gwi_enum_ini(gwi, "Positioning"))
    GWI_BB(gwi_enum_add(gwi, "Top", TUIRowPositioningTop))
    GWI_BB(gwi_enum_add(gwi, "Center", TUIRowPositioningCenter))
    GWI_BB(gwi_enum_add(gwi, "Bottom", TUIRowPositioningBottom))
    GWI_BB(gwi_enum_end(gwi))
      TUI_FUNC(Row, Positioning, positioning) // should be enum
    TUI_END(Row, row)

    DECL_OB(const Type, t_window, = gwi_class_ini(gwi, "Window", "Event"))
    t_window->nspc->info->offset += SZ_INT*3;
    TUI_FUNC(Window, string, title)
    GWI_BB(gwi_func_ini(gwi, "void", "size"))
    GWI_BB(gwi_func_arg(gwi, "int", "x"))
    GWI_BB(gwi_func_arg(gwi, "int", "y"))
    GWI_BB(gwi_func_end(gwi, window_position, ae_flag_none))

    gwi_class_xtor(gwi, win_ctor, win_dtor);
    GWI_BB(gwi_class_end(gwi))

//    TUI_INI(User, Widget)
    DECL_OB(const Type, t_user, = gwi_class_ini(gwi, "User", "Widget"))
    gwi_class_xtor(gwi, UserCtor, UserDtor);
    SET_FLAG(t_user, abstract);
       GWI_BB(gwi_func_ini(gwi, "void", "draw"))
//       GWI_BB(gwi_func_arg(gwi, "int", "y"))
//       GWI_BB(gwi_func_arg(gwi, "int", "x"))
       GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_abstract))

       GWI_BB(gwi_func_ini(gwi, "void", "activate"))
       GWI_BB(gwi_func_arg(gwi, "int", "keycode"))
       GWI_BB(gwi_func_arg(gwi, "TUI.Intent", "_"))
       GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_abstract))

       GWI_BB(gwi_func_ini(gwi, "void", "select"))
       GWI_BB(gwi_func_arg(gwi, "int", "keycode"))
       GWI_BB(gwi_func_arg(gwi, "TUI.Intent", "_"))
       GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_abstract))

       GWI_BB(gwi_func_ini(gwi, "void", "size"))
       GWI_BB(gwi_func_arg(gwi, "int", "x"))
       GWI_BB(gwi_func_arg(gwi, "int", "y"))
       GWI_BB(gwi_func_end(gwi, user_position, ae_flag_none))

       GWI_BB(gwi_func_ini(gwi, "bool", "no_select"))
       GWI_BB(gwi_func_arg(gwi, "bool", "state"))
       GWI_BB(gwi_func_end(gwi, user_set_no_select, ae_flag_none))

       GWI_BB(gwi_func_ini(gwi, "bool", "no_select"))
       GWI_BB(gwi_func_end(gwi, user_get_no_select, ae_flag_none))
/*
         DECL_OB(const Type, t_buffer, = gwi_mk_type(gwi, "Buffer", SZ_INT, NULL))
         SET_FLAG(t_buffer, abstract);
         gwi_add_type(gwi, t_buffer);
*/

//         DECL_OB(const Type, t_attribute, = gwi_mk_type(gwi, "Attribute", SZ_INT, NULL))
//         gwi_add_type(gwi, t_attribute);


       GWI_BB(gwi_func_ini(gwi, "void", "write"))
       GWI_BB(gwi_func_arg(gwi, "string", "content"))
       GWI_BB(gwi_func_arg(gwi, "int", "y"))
       GWI_BB(gwi_func_arg(gwi, "int", "x"))
       GWI_BB(gwi_func_end(gwi, user_write, ae_flag_protect))

       GWI_BB(gwi_func_ini(gwi, "void", "markup"))
       GWI_BB(gwi_func_arg(gwi, "Attribute", "attr"))
       GWI_BB(gwi_func_arg(gwi, "int", "y"))
       GWI_BB(gwi_func_arg(gwi, "int", "x"))
       GWI_BB(gwi_func_arg(gwi, "int", "height"))
       GWI_BB(gwi_func_end(gwi, user_markup, ae_flag_protect))

//    t_user->nspc->info->offset += SZ_INT*3;
//    TUI_FUNC(Window, string, title)
//    gwi_class_xtor(gwi, win_ctor, win_dtor);
//    set_tflag(t_user, tflag_abstract);
    GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_class_end(gwi))


  GWI_BB(gwi_oper_ini(gwi, "TUI.Window", "TUI.Widget", "TUI.Window"))
  GWI_BB(gwi_oper_end(gwi, "<<", window_append))
  GWI_BB(gwi_oper_ini(gwi, "TUI.Row", "TUI.Widget", "TUI.Row"))
  GWI_BB(gwi_oper_end(gwi, "<<", row_append))
// remove
// insert
  return GW_OK;
}
