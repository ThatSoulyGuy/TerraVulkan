#ifndef GL_HPP
#define GL_HPP

#define GLFW_INCLUDE_VULKAN

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GL_INIT() glfwInit()
#define GL_TERMINATE() glfwTerminate()

#define GL_WINDOW_DATA(type, value) glfwWindowHint(type, value)

#define GL_WINDOW_CREATE(width, height, title, monitor, share) glfwCreateWindow(width, height, title, monitor, share)

#define GL_WINDOW_DESTROY(window) glfwDestroyWindow(window)

#define GL_WINDOW_SHOULD_CLOSE(window) glfwWindowShouldClose(window)

#define GL_WINDOW_SET_SHOULD_CLOSE(window, value) glfwSetWindowShouldClose(window, value)

#define GL_WINDOW_SET_TITLE(window, title) glfwSetWindowTitle(window, title)

#define GL_WINDOW_SET_SIZE_LIMITS(window, minwidth, minheight, maxwidth, maxheight) glfwSetWindowSizeLimits(window, minwidth, minheight, maxwidth, maxheight)

#define GL_WINDOW_SET_ASPECT_RATIO(window, numer, denom) glfwSetWindowAspectRatio(window, numer, denom)

#define GL_WINDOW_SET_SIZE(window, width, height) glfwSetWindowSize(window, width, height)

#define GL_WINDOW_SET_POSITION(window, x, y) glfwSetWindowPos(window, x, y)

#define GL_WINDOW_SET_ICON(window, count, images) glfwSetWindowIcon(window, count, images)

#define GL_WINDOW_SET_FOCUS_CALLBACK(window, callback) glfwSetWindowFocusCallback(window, callback)

#define GL_WINDOW_SET_ICONIFY_CALLBACK(window, callback) glfwSetWindowIconifyCallback(window, callback)

#define GL_WINDOW_SET_MAXIMIZE_CALLBACK(window, callback) glfwSetWindowMaximizeCallback(window, callback)

#define GL_WINDOW_SET_REFRESH_CALLBACK(window, callback) glfwSetWindowRefreshCallback(window, callback)

#define GL_WINDOW_SET_CONTENT_SCALE_CALLBACK(window, callback) glfwSetWindowContentScaleCallback(window, callback)

#define GL_WINDOW_SET_CLOSE_CALLBACK(window, callback) glfwSetWindowCloseCallback(window, callback)

#define GL_WINDOW_SET_POS_CALLBACK(window, callback) glfwSetWindowPosCallback(window, callback)

#define GL_WINDOW_SET_SIZE_CALLBACK(window, callback) glfwSetWindowSizeCallback(window, callback)

#define GL_WINDOW_SET_FRAMEBUFFER_SIZE_CALLBACK(window, callback) glfwSetFramebufferSizeCallback(window, callback)

#define GL_WINDOW_SET_KEY_CALLBACK(window, callback) glfwSetKeyCallback(window, callback)

#define GL_WINDOW_SET_CHAR_CALLBACK(window, callback) glfwSetCharCallback(window, callback)

#define GL_WINDOW_SET_CHAR_MODS_CALLBACK(window, callback) glfwSetCharModsCallback(window, callback)

#define GL_WINDOW_SET_MOUSE_BUTTON_CALLBACK(window, callback) glfwSetMouseButtonCallback(window, callback)

#define GL_WINDOW_SET_CURSOR_POS_CALLBACK(window, callback) glfwSetCursorPosCallback(window, callback)

#define GL_WINDOW_SET_CURSOR_ENTER_CALLBACK(window, callback) glfwSetCursorEnterCallback(window, callback)

#define GL_WINDOW_SET_SCROLL_CALLBACK(window, callback) glfwSetScrollCallback(window, callback)

#define GL_WINDOW_SET_DROP_CALLBACK(window, callback) glfwSetDropCallback(window, callback)

#define GL_WINDOW_GET_SIZE(window, width, height) glfwGetWindowSize(window, width, height)

#define GL_WINDOW_GET_FRAME_BUFFER_SIZE(window, width, height) glfwGetFramebufferSize(window, width, height)

#define GL_WINDOW_GET_POSITION(window, x, y) glfwGetWindowPos(window, x, y)

#define GL_WINDOW_GET_CONTENT_SCALE(window, xscale, yscale) glfwGetWindowContentScale(window, xscale, yscale)

#define GL_WINDOW_GET_FRAME_BUFFER_SIZE(window, width, height) glfwGetFramebufferSize(window, width, height)

#define GL_WINDOW_GET_OPAQUE_WINDOW(window) glfwGetWin32Window(window)

#define GL_WINDOW_GET_MONITOR(window) glfwGetWindowMonitor(window)

#define GL_WINDOW_GET_ATTRIBUTE(window, attrib) glfwGetWindowAttrib(window, attrib)

#define GL_WINDOW_SET_USER_POINTER(window, pointer) glfwSetWindowUserPointer(window, pointer)

#define GL_WINDOW_GET_USER_POINTER(window) glfwGetWindowUserPointer(window)

#define GL_WINDOW_SET_INPUT_MODE(window, mode, value) glfwSetInputMode(window, mode, value)

#define GL_WINDOW_GET_KEY(window, key) glfwGetKey(window, key)

#define GL_WINDOW_GET_MOUSE_BUTTON(window, button) glfwGetMouseButton(window, button)

#define GL_WINDOW_GET_CURSOR_POS(window, xpos, ypos) glfwGetCursorPos(window, xpos, ypos)

#define GL_WINDOW_SET_CURSOR(window, cursor) glfwSetCursor(window, cursor)

#define GL_WINDOW_CREATE_CURSOR(image, xhot, yhot) glfwCreateCursor(image, xhot, yhot)

#define GL_WINDOW_CREATE_STANDARD_CURSOR(shape) glfwCreateStandardCursor(shape)

#define GL_WINDOW_DESTROY_CURSOR(cursor) glfwDestroyCursor(cursor)

#define GL_WINDOW_SET_KEY_CALLBACK(window, callback) glfwSetKeyCallback(window, callback)

#define GL_WINDOW_SET_CHAR_CALLBACK(window, callback) glfwSetCharCallback(window, callback)

#define GL_WINDOW_SET_CHAR_MODS_CALLBACK(window, callback) glfwSetCharModsCallback(window, callback)

#define GL_WINDOW_SET_MOUSE_BUTTON_CALLBACK(window, callback) glfwSetMouseButtonCallback(window, callback)

#define GL_WINDOW_SET_CURSOR_POS_CALLBACK(window, callback) glfwSetCursorPosCallback(window, callback)

#define GL_WINDOW_SET_CURSOR_ENTER_CALLBACK(window, callback) glfwSetCursorEnterCallback(window, callback)

#define GL_WINDOW_SET_SCROLL_CALLBACK(window, callback) glfwSetScrollCallback(window, callback)

#define GL_WINDOW_SET_DROP_CALLBACK(window, callback) glfwSetDropCallback(window, callback)

#define GL_WINDOW_GET_CLIPBOARD_STRING(window) glfwGetClipboardString(window)

#define GL_WINDOW_SET_CLIPBOARD_STRING(window, string) glfwSetClipboardString(window, string)

#define GL_WINDOW_GET_TIME() glfwGetTime()

#define GL_WINDOW_SET_TIME(time) glfwSetTime(time)

#define GL_WINDOW_SWAP_BUFFERS(window) glfwSwapBuffers(window)

#define GL_WINDOW_MAKE_CONTEXT_CURRENT(window) glfwMakeContextCurrent(window)

#define GL_WINDOW_GET_CURRENT_CONTEXT() glfwGetCurrentContext()

#define GL_WINDOW_SWAP_INTERVAL(interval) glfwSwapInterval(interval)

#define GL_WINDOW_EXTENSION_SUPPORTED(extension) glfwExtensionSupported(extension)

#define GL_WINDOW_GET_PROC_ADDRESS(procname) glfwGetProcAddress(procname)

#define GL_WINDOW_VULKAN_SUPPORTED() glfwVulkanSupported()

#define GL_WINDOW_GET_REQUIRED_INSTANCE_EXTENSIONS(count) glfwGetRequiredInstanceExtensions(count)

#define GL_WINDOW_GET_PHYSICAL_DEVICE_PRESENTATION_SUPPORT(instance, device, queuefamily) glfwGetPhysicalDevicePresentationSupport(instance, device, queuefamily)

#define GL_WINDOW_CREATE_WINDOW_SURFACE(instance, window, allocator, surface) glfwCreateWindowSurface(instance, window, allocator, surface)

#define GL_WINDOW_SET_JOYSTICK_CALLBACK(callback) glfwSetJoystickCallback(callback)

#define GL_WINDOW_JOYSTICK_PRESENT(joystick) glfwJoystickPresent(joystick)

#define GL_WINDOW_GET_JOYSTICK_AXES(joystick, count) glfwGetJoystickAxes(joystick, count)

#define GL_WINDOW_GET_JOYSTICK_BUTTONS(joystick, count) glfwGetJoystickButtons(joystick, count)

#define GL_WINDOW_GET_JOYSTICK_HATS(joystick, count) glfwGetJoystickHats(joystick, count)

#define GL_WINDOW_GET_JOYSTICK_NAME(joystick) glfwGetJoystickName(joystick)

#define GL_WINDOW_SET_JOYSTICK_USER_POINTER(joystick, pointer) glfwSetJoystickUserPointer(joystick, pointer)

#define GL_WINDOW_JOYSTICK_IS_GAMEPAD(joystick) glfwJoystickIsGamepad(joystick)

#define GL_WINDOW_GET_GAMEPAD_NAME(joystick) glfwGetGamepadName(joystick)

#define GL_WINDOW_GET_GAMEPAD_STATE(joystick, state) glfwGetGamepadState(joystick, state)

#define GL_WINDOW_SET_CLIPBOARD_STRING(window, string) glfwSetClipboardString(window, string)

#define GL_WINDOW_GET_CLIPBOARD_STRING(window) glfwGetClipboardString(window)

#define GL_POLL_EVENTS() glfwPollEvents()

#endif // !GL_HPP