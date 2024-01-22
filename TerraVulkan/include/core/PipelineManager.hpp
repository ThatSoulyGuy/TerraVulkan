#ifndef PIPELINE_MANAGER_HPP
#define PIPELINE_MANAGER_HPP

#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Logger.hpp"

namespace PipelineManager
{
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
    VkDevice device;

    void GenerateLayout() 
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            Logger_ThrowError("VK_FAILURE", "Failed to create pipeline layout!", true);
    }

    VkPipeline GenerateGraphics(VkRenderPass renderPass, VkShaderModule vertexShader, VkShaderModule fragmentShader) 
    {
        GenerateLayout();

        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};

        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertexShader;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};

        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragmentShader;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };



        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;

        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) 
            Logger_ThrowError("VK_FAILURE", "Failed to create graphics pipeline!", true);
        
        return graphicsPipeline;
    }

    void PreInitialize(VkDevice device)
    {
        PipelineManager::device = device;
    }

    void Initialize(VkRenderPass renderPass, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule) 
    {
        graphicsPipeline = GenerateGraphics(renderPass, vertShaderModule, fragShaderModule);
    }

    void RegisterShader(VkRenderPass renderPass, const VkShaderModule& vertexShader, const VkShaderModule& fragmentShader) 
    {
        Initialize(renderPass, vertexShader, fragmentShader);
    }

    void CleanUp() 
    {
        vkDestroyPipeline(device, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    }
}

#endif // !PIPELINE_MANAGER_HPP