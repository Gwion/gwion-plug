#include "sciter-x.h"
#include "sciter-x-behavior.h"
#include "sciter-x-dom.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>
#include "aux-fs.h"

#include "../facade-resources.cpp"

#include "Gwion.hh"

#include "callbacks.h"

static GLFWwindow *window;
sciter::dom::element *root;
static char s_title[1024];
static char16_t s_url[1024] = { 'f', 'i', 'l', 'e', ':' , '/' , '/' };

#include "window.h"

static void sciter_init_hints(void) {
#if defined(OSX)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
  glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, GLFW_TRUE);
  glfwSwapInterval(1);
#elif DEVICE==IOT
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
}

static SFUN(sciter_end) {
  if(!window) return;
  glfwSetWindowShouldClose(window, 1);
}

ANN static void convert(const char* src, char16_t* dest, size_t size) {
  for (size_t i = 0; i < size; i++)
    dest[i] = src[i];
  dest[size] = '\0';
}

static SFUN(sciter_run) {
  if(window) {
    handle(shred, (m_str)"SciterAlreadyExists");
    return;
  }
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    handle(shred, "SciterInitException");
  sciter_init_hints();
  const m_str title = STRING(*(M_Object*)MEM(0));
  strncpy(s_title, title, strlen(title));
  const m_str file  = STRING(*(M_Object*)MEM(SZ_INT));
  convert(file, s_url + 7, strlen(file));
  pthread_t thread;
  pthread_create(&thread, NULL, sciter_loop, shred->info->vm->bbq);
  pthread_detach(thread);
}

static SFUN(sciter_rt_options) {
  *(m_int*)RETURN = SciterSetOption(NULL, SCITER_SET_SCRIPT_RUNTIME_FEATURES, *(m_int*)MEM(0));
}

static SFUN(sciter_debug) {
  *(m_int*)RETURN = SciterSetOption(NULL, SCITER_SET_DEBUG_MODE, *(m_int*)MEM(0));
}

static SFUN(sciter_data) {
  if(!window) return;
  const std::string data_key(STRING(*(M_Object*)MEM(0)));
  const std::string data_value(STRING(*(M_Object*)MEM(SZ_INT)));
  root->call_function("data", data_key, data_value);
}

#define callback_fn(fn_name, fn)              \
static SFUN(sciter_##fn) {                    \
  if(!window) return;                         \
  if(*(m_int*)MEM(0))                         \
    glfwSet##fn_name##Callback(window, fn);   \
  else                                        \
    glfwSet##fn_name##Callback(window, NULL); \
}
callback_fn(MouseButton, mouse_button_callback);
callback_fn(CursorPos,   mouse_move_callback);
callback_fn(CursorEnter, mouse_enter_leave_callback);
callback_fn(Scroll, mouse_wheel_callback);
callback_fn(Key, key_callback);
callback_fn(Char, char_callback);
callback_fn(WindowFocus, focus_callback);

extern "C" {
  GWION_IMPORT(Sciter) {
    DECL_OB(const Type, t_sciter, = gwi_class_ini(gwi, "Sciter", "Object"));
      GWI_BB(gwi_enum_ini(gwi, (m_str)"Runtime"));
      GWI_BB(gwi_enum_add(gwi, (m_str)"fileIO", ALLOW_FILE_IO));
      GWI_BB(gwi_enum_add(gwi, (m_str)"socketIO", ALLOW_SOCKET_IO));
      GWI_BB(gwi_enum_add(gwi, (m_str)"eval", ALLOW_EVAL));
      GWI_BB(gwi_enum_add(gwi, (m_str)"sysinfo", ALLOW_SYSINFO));
      GWI_OB(gwi_enum_end(gwi));

    GWI_BB(gwi_func_ini(gwi, "int", "rtOption"));
//    GWI_BB(gwi_func_arg(gwi, "Runtime", "arg"));
    GWI_BB(gwi_func_arg(gwi, "int", "arg"));
    GWI_BB(gwi_func_end(gwi, sciter_rt_options, ae_flag_static));

    GWI_BB(gwi_func_ini(gwi, "int", "debug"));
    GWI_BB(gwi_func_arg(gwi, "bool", "arg"));
    GWI_BB(gwi_func_end(gwi, sciter_debug, ae_flag_static));

    #define import_callback(name, fn)                         \
      GWI_BB(gwi_func_ini(gwi, "void", name "_cb"));          \
      GWI_BB(gwi_func_arg(gwi, "bool", "arg"));               \
      GWI_BB(gwi_func_end(gwi, sciter_##fn, ae_flag_static)); \

    import_callback("button", mouse_button_callback);
    import_callback("mouse", mouse_move_callback);
    import_callback("enter", mouse_enter_leave_callback);
    import_callback("scroll", mouse_wheel_callback);
    import_callback("key", key_callback);
    import_callback("char", char_callback);
    import_callback("focus", focus_callback);
    #undef import_callback

    GWI_BB(gwi_func_ini(gwi, "void", "run"));
    GWI_BB(gwi_func_arg(gwi, "string", "title"));
    GWI_BB(gwi_func_arg(gwi, "string", "path"));
    GWI_BB(gwi_func_end(gwi, sciter_run, ae_flag_static));

    GWI_BB(gwi_func_ini(gwi, "void", "end"));
    GWI_BB(gwi_func_end(gwi, sciter_end, ae_flag_static));

    GWI_BB(gwi_func_ini(gwi, "void", "data"));
    GWI_BB(gwi_func_arg(gwi, "string", "key"));
    GWI_BB(gwi_func_arg(gwi, "string", "value"));
    GWI_BB(gwi_func_end(gwi, sciter_data, ae_flag_static));

    GWI_BB(gwi_class_end(gwi));

    return GW_OK;
  }
}
