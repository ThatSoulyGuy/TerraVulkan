#include "core/Logger.hpp"
#include "core/VulkanManager.hpp"
#include "core/Window.hpp"

int main()
{
	Logger_Initialize();

	Logger_WriteConsole("Hello, TerraVulkan!", LogLevel::INFO);

	Window::Initialize({750, 450}, "TerraVulkan");
	VulkanManager::Initialize();

	while (!Window::ShouldClose())
	{
		VulkanManager::Render();

		Window::Update();
	}

	VulkanManager::CleanUp();
	Window::CleanUp();
	Logger_CleanUp();

	return 0;
}