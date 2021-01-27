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


#include "buffer.h"
#include "window.h"
#include "widgets/row.h"
#include "screen.h"

#define FPS_30 33333
#define FPS_60 16667
#define FPS FPS_60

#define CONV_MTOU(x) ((x) * 1000)
#define CONV_UTOM(x) ((x) / 1000)

struct TUIMeta {
  TUIBuffer buffer;
  THREAD_TYPE thread;
  struct Vector_ windows;
  TUIConfig old_config;
  MemPool mp;
  volatile bool running;
};

#include <signal.h>
static int grun = 1;
void ctrlc(int _NUSED) {
  grun = 0;
  THREAD_RETURN();
}

void* tui_func(void* arg) {
signal(SIGINT, ctrlc);
  struct TUIMeta* meta = (struct TUIMeta*)arg;
  const int timeout_msecs = 12;

  TUIEvent event = {};
  while (meta->running && grun) {
    for(m_uint i = 0; i < vector_size(&meta->windows); ++i)
      tui_window_draw(&meta->buffer, (TUIWindow*)vector_at(&meta->windows, i));

    tui_buffer_draw(&meta->buffer);
    usleep(FPS - CONV_MTOU(timeout_msecs));
    if (tui_screen_input(&event, timeout_msecs)) {
        tui_window_receive_primitive_event((TUIWindow*)vector_front(&meta->windows), event);
    }
    tui_screen_update();
  }
  vector_release(&meta->windows);
  tui_screen_deconfigure(&meta->old_config);
  tui_buffer_destroy(&meta->buffer);

  // for some reason the terminal is messed up at this point
  system("tput reset");

  return NULL;
}

#define GET_META(o) (&*(struct TUIMeta*)(o->type_ref->info->owner_class->nspc->info->class_data))

#define WINDOW(a) (*(TUIWindow*)(a->data + SZ_INT*3))
#define WIDGET(a) (*(TUIWidget*)(a->data + SZ_INT))

static CTOR(win_ctor) {
  struct TUIMeta *meta = GET_META(o);
  if(!meta->running) {
    meta->mp = shred->info->vm->gwion->mp;
    if(tui_screen_configure(&meta->buffer, &meta->old_config) == EXIT_FAILURE)
      exit(3);//TODO
    vector_init(&meta->windows);
    THREAD_CREATE(meta->thread, tui_func, meta);
  }
  TUIWindow *win = &WINDOW(o);
  tui_window_init(win, TUIMenubarMake("A Few TUI Controls", (TUITools){}), TUIRectMake(0,0,0,0), (TUIWidgets){0, 0, NULL});
  tui_window_fullscreen(win, &meta->buffer);
  vector_add(&meta->windows, (m_uint)win);
  ++meta->running;
  vector_init(&*(struct Vector_*)(o->data + SZ_INT));
  *(TUIWidget**)(o->data + SZ_INT*2) = _mp_malloc(shred->info->vm->gwion->mp, 64 * sizeof(TUIWidget));
}

static DTOR(win_dtor) {
  struct TUIMeta *meta = GET_META(o);
  TUIWindow *win = &WINDOW(o);
  --meta->running;
  win->widgets = (TUIWidgets){ 0, 0, NULL };
  vector_rem(&meta->windows, (m_uint)win);
  struct Vector_ v = *(struct Vector_*)(o->data + SZ_INT);
  for(m_uint i = 0; i < vector_size(&v); ++i)
    _release((M_Object)vector_at(&v, i), shred);
  vector_release(&v);
  _mp_free(shred->info->vm->gwion->mp, 64 * sizeof(TUIWidget), *(TUIWidget**)(o->data + SZ_INT*2));
  tui_window_destroy(win);
}

static MFUN(Window_title_set) {
  TUIWindow *win = &WINDOW(o);
  win->menu.title = STRING(*(M_Object*)MEM(SZ_INT));
}

static MFUN(Window_title_get) {
  TUIWindow *win = &WINDOW(o);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion->mp, shred, (m_str)win->menu.title);
}

bool tui_window_select(TUIWindow* window, ssize_t (*array_func)(TUIWidgets*), TUIEvent event);

static INSTR(window_append) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  const M_Object w = *(M_Object*)REG(0);
  TUIWindow *win = &WINDOW(o);
  const Vector v = &*(struct Vector_*)(o->data+ SZ_INT);
  const m_uint sz = vector_size(v);
  TUIWidget *e = *(TUIWidget**)(o->data + SZ_INT*2);
  e[sz] = WIDGET(w);
  ++w->ref;
  vector_add(v, (m_uint)w);
  win->widgets = (TUIWidgets){ sz + 1, win->widgets.selection, e };
  tui_window_select(win, array_next, (TUIEvent){});
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

ANN static void button_clicked(TUIButton *button) {
  broadcast((M_Object)button->user_data);
}

static CTOR(ButtonCtor) {
  TUIButton *button = (TUIButton*)WIDGET(o).user_data;
  button->clicked = button_clicked;
  button->user_data = o;
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

GWION_IMPORT(TUI) {
  DECL_OB(const Type, t_tui, = gwi_class_ini(gwi, "TUI", "Object"))
  t_tui->nspc->info->class_data_size += sizeof(struct TUIMeta);

    DECL_OB(const Type, t_widget, = gwi_class_ini(gwi, "Widget", "Event"))
    t_widget->nspc->info->offset += sizeof(TUIWidget);
    GWI_BB(gwi_item_ini(gwi, "@internal", "@data"))
    CHECK_BB((gwi_item_end(gwi, ae_flag_none, num, 0)))
    gwi_class_xtor(gwi, WidgetCtor, NULL);
    GWI_BB(gwi_class_end(gwi))

    TUI_INI(Label, Widget)
    TUI_FUNC(Label, string, text)
    TUI_END(Label, label)

    TUI_INI(Button, Widget)
      gwi_class_xtor(gwi, ButtonCtor, NULL);
    TUI_FUNC(Button, string, text)
    TUI_FUNC(Button, int, timeout)
    TUI_END(Button, button)

    TUI_INI(Toggle, Widget)
    TUI_FUNC(Toggle, bool, state)
    TUI_FUNC(Toggle, string, text)
    TUI_END(Toggle, toggle)

    TUI_INI(Check, Widget)
    TUI_FUNC(Check, bool, state)
    TUI_END(Check, check)

    TUI_INI(Sep, Widget)
    TUI_FUNC(Sep, char, c)
    TUI_END(Sep, sep)

    TUI_INI(Slider, Widget)
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

    TUI_INI(Options, Widget)
    GWI_BB(gwi_enum_ini(gwi, "SelectionType"))
    GWI_BB(gwi_enum_add(gwi, "Multiple", TUIOptionsSelectionTypeMultiple))
    GWI_BB(gwi_enum_add(gwi, "Singular", TUIOptionsSelectionTypeSingular))
    GWI_BB(gwi_enum_end(gwi))
    TUI_FUNC(Options, string[], names)
    TUI_FUNC(Options, int, selections)
    TUI_FUNC(Options, SelectionType, selections)
    TUI_END(Options, options)

    TUI_INI(Row, Widget)
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
    t_window->nspc->info->offset += SZ_INT*2 + sizeof(TUIWindow);
    TUI_FUNC(Window, string, title)
    gwi_class_xtor(gwi, win_ctor, win_dtor);
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
