#include "core/Logger.hpp"
#include "core/VulkanManager.hpp"
#include "core/Window.hpp"
#include "render/Mesh.hpp"
#include "render/ShaderManager.hpp"

Mesh mesh = {};

int main()
{
	Logger_Initialize();

	Logger_WriteConsole("Hello, TerraVulkan!", LogLevel::INFO);

	Window::Initialize({750, 450}, "TerraVulkan");

	VulkanManager::PreInitialize();
	
	ShaderManager::Register(Shader::Register("shaders/default", "default"));
	ShaderManager::Generate();

	mesh = Mesh::Register("test", {}, {}, "default");
	mesh.GenerateTestTriangle();

	VulkanManager::RequestRenderCall([](VkCommandBuffer buffer)
	{
		mesh.Render(buffer);
	});

	VulkanManager::PostInitialize();

	while (!Window::ShouldClose())
	{
		VulkanManager::Render();

		Window::Update();
	}

	ShaderManager::CleanUp();
	mesh.CleanUp();
	VulkanManager::CleanUp();
	Window::CleanUp();
	Logger_CleanUp();

	return 0;
}