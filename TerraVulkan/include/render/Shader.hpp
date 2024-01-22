#ifndef SHADER_HPP
#define SHADER_HPP

#include <fstream>
#include "core/Settings.hpp"
#include "core/VulkanManager.hpp"

struct ShaderStage
{

};

struct Shader
{

public:

    void Generate()
    {
        auto vertexShaderCode = ReadShaderFile(vertexPath);
        auto fragmentShaderCode = ReadShaderFile(fragmentPath);

        vertexShaderModule = CreateShaderModule(vertexShaderCode);
        fragmentShaderModule = CreateShaderModule(fragmentShaderCode);
    }

    void CleanUp()
    {
        vkDestroyShaderModule(VulkanManager::device, vertexShaderModule, nullptr);
        vkDestroyShaderModule(VulkanManager::device, fragmentShaderModule, nullptr);
    }

	static Shader Register(const std::string& path, const std::string& name, const std::string& domain = Settings::domain)
	{
		Shader shader = {};

		shader.name = name;
		shader.vertexPath = "assets/" + domain + "/" + path + "Vertex.spv";
		shader.fragmentPath = "assets/" + domain + "/" + path + "Fragment.spv";

		return shader;
	}
	
	std::string name = "";
	std::string vertexPath = "";
	std::string fragmentPath = "";

private:

    std::vector<char> ReadShaderFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
            throw std::runtime_error("failed to open file: " + filename);

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    VkShaderModule CreateShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(VulkanManager::device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
            Logger_ThrowError("VK_FAILURE", "Failed to create shader module!", true);

        return shaderModule;
    }

    VkShaderModule vertexShaderModule = {};
	VkShaderModule fragmentShaderModule = {};
};

#endif // !SHADER_HPP