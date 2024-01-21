#ifndef MESH_HPP
#define MESH_HPP

#include "render/Vertex.hpp"
#include "util/MeshHelper.hpp"

class Mesh
{

public:

    void GenerateTestTriangle()
    {
        vertices =
        {
            Vertex::Register({-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}),
            Vertex::Register({0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}),
            Vertex::Register({0.0f, 0.5f, 0.0f}, {0.5f, 1.0f})
        };

		indices = { 0, 1, 2 };
	    
        Generate();
    }

	void Generate()
	{
        GenerateVertexBuffer();
        GenerateIndexBuffer();
	}

	void ReRegister(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		this->vertices = vertices;
		this->indices = indices;
	}

    void CleanUp()
    {
        vkDestroyBuffer(VulkanManager::device, vertexBuffer, nullptr);
        vkFreeMemory(VulkanManager::device, vertexBufferMemory, nullptr);
        vkDestroyBuffer(VulkanManager::device, indexBuffer, nullptr);
        vkFreeMemory(VulkanManager::device, indexBufferMemory, nullptr);
    }

	static Mesh Register(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		Mesh mesh = {};

		mesh.name = name;
		mesh.vertices = vertices;
		mesh.indices = indices;

		return mesh;
	}
	
	std::string	name;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

private:

	VkDeviceMemory vertexBufferMemory;
	VkDeviceMemory indexBufferMemory;
    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;

    void GenerateVertexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        MeshHelper::GenerateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(VulkanManager::device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(VulkanManager::device, stagingBufferMemory);

        MeshHelper::GenerateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

        MeshHelper::CopyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(VulkanManager::device, stagingBuffer, nullptr);
        vkFreeMemory(VulkanManager::device, stagingBufferMemory, nullptr);
    }

    void GenerateIndexBuffer() 
    {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        MeshHelper::GenerateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(VulkanManager::device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(VulkanManager::device, stagingBufferMemory);

        MeshHelper::GenerateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

        MeshHelper::CopyBuffer(stagingBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(VulkanManager::device, stagingBuffer, nullptr);
        vkFreeMemory(VulkanManager::device, stagingBufferMemory, nullptr);
    }
};

#endif // !MESH_HPP