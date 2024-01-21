#ifndef MESH_HELPER_HPP
#define MESH_HELPER_HPP

#include "core/VulkanManager.hpp"

namespace MeshHelper
{
    uint32_t GetMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) 
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(VulkanManager::physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
                return i;
        }

        Logger_ThrowError("VK_FAILURE", "Failed to find suitable memory type!", true);
    }

    void GenerateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInformation = {};

        bufferInformation.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInformation.size = size;
        bufferInformation.usage = usage;
        bufferInformation.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(VulkanManager::device, &bufferInformation, nullptr, &buffer) != VK_SUCCESS) 
            Logger_ThrowError("VK_FAILURE", "Failed to create buffer!", true);

        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(VulkanManager::device, buffer, &memoryRequirements);

        VkMemoryAllocateInfo allocationInformation = {};

        allocationInformation.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocationInformation.allocationSize = memoryRequirements.size;
        allocationInformation.memoryTypeIndex = GetMemoryType(memoryRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(VulkanManager::device, &allocationInformation, nullptr, &bufferMemory) != VK_SUCCESS)
            Logger_ThrowError("VK_FAILURE", "Failed to allocate buffer memory!", true);

        vkBindBufferMemory(VulkanManager::device, buffer, bufferMemory, 0);
    }

    void CopyBuffer(VkBuffer from, VkBuffer to, VkDeviceSize size) 
    {
        VkCommandBufferAllocateInfo allocationInformation{};

        allocationInformation.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocationInformation.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocationInformation.commandPool = VulkanManager::commandPool;
        allocationInformation.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(VulkanManager::device, &allocationInformation, &commandBuffer);

        VkCommandBufferBeginInfo beginInformation{};

        beginInformation.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInformation.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInformation);

        VkBufferCopy copyRegion{};

        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, from, to, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submissionInformation{};

        submissionInformation.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submissionInformation.commandBufferCount = 1;
        submissionInformation.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(VulkanManager::graphicsQueue, 1, &submissionInformation, VK_NULL_HANDLE);
        vkQueueWaitIdle(VulkanManager::graphicsQueue);

        vkFreeCommandBuffers(VulkanManager::device, VulkanManager::commandPool, 1, &commandBuffer);
    }
}

#endif // !MESH_HELPER_HPP