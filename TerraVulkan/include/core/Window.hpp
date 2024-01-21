#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>
#include <glm/glm.hpp>
#include "util/GL.hpp"

#define WINDOW_SIZE_CALLBACK \
    \
	GL_WINDOW_SET_FRAMEBUFFER_SIZE_CALLBACK(Window::window, [](GLFWwindow* window, int width, int height) \
	{ \
		VulkanManager::Rebuild(); \
	}); \

namespace Window
{
	GLFWwindow* window;

	void Initialize(const glm::ivec2& size, const std::string& title)
	{
		GL_INIT();

		GL_WINDOW_DATA(GLFW_CLIENT_API, GLFW_NO_API);
		window = GL_WINDOW_CREATE(size.x, size.y, title.c_str(), nullptr, nullptr);

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	}

	void GetSurface(GLFWwindow* window, VkSurfaceKHR& surface, VkInstance& instance) 
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) 
			Logger_ThrowError("VK_FAILURE", "Failed to create window surface!", true);
	}

	void Update()
	{
		GL_POLL_EVENTS();
	}

	void SetTitle(const std::string& title)
	{
		GL_WINDOW_SET_TITLE(window, title.c_str());
	}

	VkExtent2D GetSize()
	{
		glm::ivec2 size = {};

		GL_WINDOW_GET_SIZE(window, &size.x, &size.y);

		return {(unsigned int)size.x, (unsigned int)size.y};
	}

	bool ShouldClose()
	{
		return GL_WINDOW_SHOULD_CLOSE(window);
	}

	void CleanUp()
	{
		GL_WINDOW_DESTROY(window);
		GL_TERMINATE();
	}
}

#endif // !WINDOW_HPP