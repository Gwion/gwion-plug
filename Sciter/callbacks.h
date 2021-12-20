static bool sciter_needs_drawing = true;

ANN static void error_callback(int error, const char* description) {
  gw_err("{R+}Error{0+]:{0} %s\n", description);
}

static MOUSE_BUTTONS mbutton = MOUSE_BUTTONS(0);

static void mouse_button_callback(GLFWwindow * window, int button, int action, int modifiers) {
  MOUSE_EVENTS  me = action == GLFW_PRESS ? MOUSE_DOWN : MOUSE_UP;
  switch (button) {
    default:
    case GLFW_MOUSE_BUTTON_1: mbutton = MAIN_MOUSE_BUTTON; break;
    case GLFW_MOUSE_BUTTON_2: mbutton = PROP_MOUSE_BUTTON; break;
    case GLFW_MOUSE_BUTTON_3: mbutton = MIDDLE_MOUSE_BUTTON; break;
  }
  //KEYBOARD_STATES
  UINT ks = 0;
  if (modifiers & GLFW_MOD_SHIFT)   ks |= SHIFT_KEY_PRESSED;
  if (modifiers & GLFW_MOD_CONTROL) ks |= CONTROL_KEY_PRESSED;
  if (modifiers & GLFW_MOD_ALT)     ks |= ALT_KEY_PRESSED;
  double x, y; glfwGetCursorPos(window, &x, &y);
  POINT pos = {int(x),int(y)};
  SciterProcX(window, SCITER_X_MSG_MOUSE(me, mbutton, KEYBOARD_STATES(ks),pos));
  if (me == MOUSE_UP)
    mbutton = MOUSE_BUTTONS(0);
}

static void mouse_move_callback(GLFWwindow * window, double x, double y) {
  MOUSE_EVENTS  me = MOUSE_MOVE;
  POINT pos = { int(x),int(y) };
  SciterProcX(window, SCITER_X_MSG_MOUSE(me, mbutton, KEYBOARD_STATES(0), pos));
}

static void mouse_enter_leave_callback(GLFWwindow * window, int enter) {
  MOUSE_EVENTS  me = enter ? MOUSE_ENTER : MOUSE_LEAVE;
  UINT ks = 0;
  double x, y; glfwGetCursorPos(window, &x, &y);
  POINT pos = { int(x),int(y) };
  SciterProcX(window, SCITER_X_MSG_MOUSE(me, MOUSE_BUTTONS(0), KEYBOARD_STATES(ks), pos));
}

static void mouse_wheel_callback(GLFWwindow * window, double dx, double dy) {
  MOUSE_EVENTS  me = MOUSE_WHEEL;
  double x, y; glfwGetCursorPos(window, &x, &y);
  POINT pos = { int(x),int(y) };
  dx *= 120;
  dy *= 120;
  UINT  deltas = ((uint16_t)(int16_t)dx) << 16 | (uint16_t)(int16_t)dy;
  SciterProcX(window, SCITER_X_MSG_MOUSE(me, MOUSE_BUTTONS(deltas), KEYBOARD_STATES(0), pos));
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  KEY_EVENTS me = GLFW_RELEASE == action ? KEY_UP : KEY_DOWN;
  UINT ks = 0;
  if (mods & GLFW_MOD_SHIFT)   ks |= SHIFT_KEY_PRESSED;
  if (mods & GLFW_MOD_CONTROL) ks |= CONTROL_KEY_PRESSED;
  if (mods & GLFW_MOD_ALT)     ks |= ALT_KEY_PRESSED;
  SciterProcX(window, SCITER_X_MSG_KEY(me, UINT(key), KEYBOARD_STATES(ks)));
}

static void char_callback(GLFWwindow* window, unsigned int codepoint) {
  KEY_EVENTS me = KEY_CHAR;
  SciterProcX(window, SCITER_X_MSG_KEY(me, codepoint, KEYBOARD_STATES(0)));
}

static void focus_callback(GLFWwindow* window, int got_focus) {
  SciterProcX(window, SCITER_X_MSG_FOCUS(!!got_focus));
}

static UINT on_load_data(LPSCN_LOAD_DATA pnmld) {
  // your custom loader is here
  LPCBYTE pb = 0; UINT cb = 0;
  aux::wchars wu = aux::chars_of(pnmld->uri);
  if (wu.like(WSTR("this://app/*"))) { /**/
    // try to get them from archive first
    aux::bytes adata = sciter::archive::instance().get(wu.start + 11);
    if (adata.length)
      ::SciterDataReady(pnmld->hwnd, pnmld->uri, adata.start, adata.length);
  }
  return LOAD_OK;
}

static UINT on_data_loaded(LPSCN_DATA_LOADED pnm) { return 0; }

static UINT attach_behavior(LPSCN_ATTACH_BEHAVIOR lpab) {
  // attach native behaviors (if we have any)
  sciter::event_handler *pb = sciter::behavior_factory::create(lpab->behaviorName, lpab->element);
  if (pb) {
    lpab->elementTag  = pb;
    lpab->elementProc = sciter::event_handler::element_proc;
    return 1;
  }
  return 0;
}

static UINT on_invalidate_rect(LPSCN_INVALIDATE_RECT pnm) {
  sciter_needs_drawing = true;
  //glfwPostEmptyEvent();
  return 0;
}

// notification cracker
static UINT SC_CALLBACK handle_notification(LPSCITER_CALLBACK_NOTIFICATION pnm, LPVOID callbackParam) {
  // Crack and call appropriate method
  // here are all notifiactions
  switch (pnm->code) {
    case SC_LOAD_DATA: return on_load_data((LPSCN_LOAD_DATA)pnm);
    case SC_DATA_LOADED: return on_data_loaded((LPSCN_DATA_LOADED)pnm);
    case SC_ATTACH_BEHAVIOR: return attach_behavior((LPSCN_ATTACH_BEHAVIOR)pnm);
    case SC_INVALIDATE_RECT: return on_invalidate_rect((LPSCN_INVALIDATE_RECT)pnm);
    case SC_ENGINE_DESTROYED: break;
  }
  return 0;
}
