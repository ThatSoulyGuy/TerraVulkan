#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <vector>
#include <glm/glm.hpp>

#define GLFW_INCLUDE_VULKAN
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GetMember(type, member) ((::size_t)&reinterpret_cast<char const volatile&>((((type*)0)->member)))

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 textureCoordinates;

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription = {};

		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {};

		attributeDescriptions.resize(3);

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = GetMember(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = GetMember(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = GetMember(Vertex, textureCoordinates);

		return attributeDescriptions;
	}

	static Vertex Register(const glm::vec3& position, const glm::vec2& textureCoordinates)
	{
		Vertex vertex = {};

		vertex.position = position;
		vertex.color = {1.0f, 1.0f, 1.0f};
		vertex.textureCoordinates = textureCoordinates;

		return vertex;
	}

	static Vertex Register(const glm::vec3& position, const glm::vec3& color, const glm::vec2& textureCoordinates)
	{
		Vertex vertex = {};

		vertex.position = position;
		vertex.color = color;
		vertex.textureCoordinates = textureCoordinates;

		return vertex;
	}
};

#endif // !VERTEX_HPP