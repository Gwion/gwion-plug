ANN static inline GLFWwindow* window_ini(void) {
  GLFWmonitor* const monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  return window = glfwCreateWindow(mode->width, mode->height, s_title, NULL, NULL);
}

ANN static void window_setup(GLFWwindow *const window) {
  glfwMakeContextCurrent( window );
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glfwSwapInterval(1);
  SciterProcX(window, SCITER_X_MSG_CREATE(GFX_LAYER_SKIA_OPENGL,FALSE));
  SciterSetCallback(window, handle_notification, NULL);
  sciter::archive::instance().open(aux::elements_of(resources)); // bind resources[] (defined in "facade-resources.cpp") with the archive
}

ANN static inline void window_resolution(GLFWwindow *const window) {
  float xscale, yscale;
  glfwGetWindowContentScale(window, &xscale, &yscale);
  SciterProcX(window, SCITER_X_MSG_RESOLUTION(UINT(96 * xscale)));
}

ANN static void window_redraw(GLFWwindow *const window) {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  SciterProcX(window, SCITER_X_MSG_PAINT());
  glfwSwapBuffers(window);
}

ANN static inline void window_end(GLFWwindow *const window) {
  SciterProcX(window, SCITER_X_MSG_DESTROY());
  glfwDestroyWindow(window);
}

#include <signal.h>
static void sig(int unused NUSED) {
  if(window)  glfwSetWindowShouldClose(window, 1);
}

ANN static void* sciter_loop(void *data) {
  window_ini();
  if (!window) {
    glfwTerminate();
    gw_err((m_str)"{+}[Sciter]{+} {R}error{0}: can't create window");
    return NULL;
  }
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  window_setup(window);
  SciterLoadFile(window, s_url);

  sciter::dom::element _root = sciter::dom::element::root_element(window);
  root = &_root;

  window_resolution(window);
  int width = 0, height = 0;
  while (!glfwWindowShouldClose(window)) {
    const UINT ticks = UINT(glfwGetTime() * 1000); // in milliseconds
    SciterProcX(window, SCITER_X_MSG_HEARTBIT(ticks));

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    if (w != width || h != height)
      SciterProcX(window, SCITER_X_MSG_SIZE(width = w, height = h));

    if (sciter_needs_drawing) {
      sciter_needs_drawing = false;
      glViewport(0, 0, w, h);
      window_redraw(window);
    }

    glfwWaitEventsTimeout(0.002); // 60 FPS
    glfwPollEvents();
  }
  window_end(window);
  window = NULL;
  glfwTerminate();
  return NULL;
}
