#ifndef VULKAN_MANAGER_HPP
#define VULKAN_MANAGER_HPP

#include <queue>
#include <functional>
#include <set>
#include "util/VulkanHelper.hpp"
#include "core/Logger.hpp"
#include "core/Window.hpp"

#define MAX_FRAMES_IN_FLIGHT 2

namespace VulkanManager
{
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkSurfaceKHR surface;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkRenderPass renderPass;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::queue<std::function<void()>> renderFunctions;
    size_t currentFrame = 0;

    void RequestRenderCall(std::function<void()> function)
	{
		renderFunctions.push(function);
	}

    void SetupDebugMessenger() 
    {
#ifndef _DEBUG
        return;
#endif // !_DEBUG


        VkDebugUtilsMessengerCreateInfoEXT creationInformation = {};

        creationInformation.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        creationInformation.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        creationInformation.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        creationInformation.pfnUserCallback = VulkanHelper::DebugCallback;
        creationInformation.pUserData = nullptr;

        if (VulkanHelper::CreateDebugUtilsMessenger(instance, &creationInformation, nullptr, &debugMessenger) != VK_SUCCESS)
            Logger_ThrowError("VK_FAILURE", "Failed to set up debug messenger!", true);
    }

    void CreateLogicalDeviceAndQueue()
	{
        QueueFamilyIndices indices = VulkanHelper::FindQueueFamilies(physicalDevice, surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) 
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;

        std::vector<const char*> deviceExtensions = 
        {   
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

#ifdef _DEBUG

        auto validationLayers = VulkanHelper::GetAllValidationLayers();

        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
#else
        createInfo.enabledLayerCount = 0;
#endif

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) 
            Logger_ThrowError("VK_FAILURE", "Failed to create logical device!", true);

        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    }

    void CreatePhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
            Logger_ThrowError("nullptr", "Failed to find GPUs with Vulkan support!", true);

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto& device : devices)
        {
            if (VulkanHelper::IsDeviceSuitable(device, surface))
            {
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE)
            Logger_ThrowError("VK_NULL_HANDLE", "Failed to find a suitable GPU!", true);
    }

    void CreateSwapChain()
    {
        SwapChainSupportDetails swapChainSupport = VulkanHelper::GetSwapChainSupport(physicalDevice, surface);

        VkSurfaceFormatKHR surfaceFormat = VulkanHelper::GetSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = VulkanHelper::GetSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = VulkanHelper::GetSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
            imageCount = swapChainSupport.capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = VulkanHelper::FindQueueFamilies(physicalDevice, surface);

        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value() };

        std::vector<uint32_t> uniqueQueueFamilies;

        if (indices.graphicsFamily.has_value()) 
            uniqueQueueFamilies.push_back(indices.graphicsFamily.value());

        for (uint32_t presentFamily : indices.presentFamily) 
        {
            if (std::find(uniqueQueueFamilies.begin(), uniqueQueueFamilies.end(), presentFamily) == uniqueQueueFamilies.end()) 
                uniqueQueueFamilies.push_back(presentFamily);
        }

        if (uniqueQueueFamilies.size() > 1) 
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = static_cast<uint32_t>(uniqueQueueFamilies.size());
            createInfo.pQueueFamilyIndices = uniqueQueueFamilies.data();
        }
        else 
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
            Logger_ThrowError("VK_FAILURE", "Failed to create swap chain!", true);

        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }

    void CreateImageViews()
    {
        swapChainImageViews.resize(swapChainImages.size());

        for (size_t i = 0; i < swapChainImages.size(); i++) 
        {
            VkImageViewCreateInfo creationInformation = {};

            creationInformation.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            creationInformation.image = swapChainImages[i];
            creationInformation.viewType = VK_IMAGE_VIEW_TYPE_2D;
            creationInformation.format = swapChainImageFormat;

            creationInformation.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            creationInformation.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            creationInformation.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            creationInformation.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            creationInformation.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            creationInformation.subresourceRange.baseMipLevel = 0;
            creationInformation.subresourceRange.levelCount = 1;
            creationInformation.subresourceRange.baseArrayLayer = 0;
            creationInformation.subresourceRange.layerCount = 1;

            if (vkCreateImageView(device, &creationInformation, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) 
                Logger_ThrowError("VK_FAILURE", "Failed to create image views!", true);
        }
	}

    void CreateRenderPass()
    {
        VkAttachmentDescription colorAttachment{};

        colorAttachment.format = swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentReference{};

        colorAttachmentReference.attachment = 0;
        colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};

        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentReference;

        VkRenderPassCreateInfo renderPassInfo{};

        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) 
            Logger_ThrowError("VK_FAILURE", "Failed to create render pass!", true);
	}

    void CreateFramebuffers()
    {
        swapChainFramebuffers.resize(swapChainImageViews.size());

        for (size_t i = 0; i < swapChainImageViews.size(); i++) 
        {
            VkImageView attachments[] = 
            {
                swapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo = {};

            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) 
                Logger_ThrowError("VK_FAILURE", "Failed to create framebuffer!", true);
        }
    }

    void CreateCommandPool()
    {
        QueueFamilyIndices queueFamilyIndices = VulkanHelper::FindQueueFamilies(physicalDevice, surface);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) 
            Logger_ThrowError("VK_FAILURE", "Failed to create command pool!", true);
	}

    void CreateCommandBuffers()
    {
        commandBuffers.resize(swapChainFramebuffers.size());

        VkCommandBufferAllocateInfo allocationInformation{};

        allocationInformation.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocationInformation.commandPool = commandPool;
        allocationInformation.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocationInformation.commandBufferCount = (uint32_t)commandBuffers.size();

        if (vkAllocateCommandBuffers(device, &allocationInformation, commandBuffers.data()) != VK_SUCCESS) 
            Logger_ThrowError("VK_FAILURE", "Failed to allocate command buffers!", true);

        for (size_t i = 0; i < commandBuffers.size(); i++) 
        {
            VkCommandBufferBeginInfo recordingInformation = {};
            recordingInformation.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &recordingInformation) != VK_SUCCESS) 
                Logger_ThrowError("VK_FAILURE", "Failed to begin recording command buffer!", true);


            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass;
            renderPassInfo.framebuffer = swapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = swapChainExtent;

            VkClearValue clearColor = { {{0.0f, 0.4f, 0.7f, 1.0f}} };
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            while (!renderFunctions.empty())
            {
				renderFunctions.front()();
				renderFunctions.pop();
			}

            vkCmdEndRenderPass(commandBuffers[i]);


            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) 
                Logger_ThrowError("VK_FAILURE", "Failed to record command buffer!", true);
        }
	}

    void CreateSyncObjects()
    {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
        {
            if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) 
                Logger_ThrowError("VK_FAILURE", "Failed to create synchronization objects for a frame!", true);
        }
	}

    void CleanUpSwapChain() 
    {
        for (auto framebuffer : swapChainFramebuffers) 
            vkDestroyFramebuffer(device, framebuffer, nullptr);

        vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

        for (auto imageView : swapChainImageViews) 
            vkDestroyImageView(device, imageView, nullptr);

        vkDestroyRenderPass(device, renderPass, nullptr);
        vkDestroySwapchainKHR(device, swapChain, nullptr);
    }

    void Rebuild()
    {
        vkDeviceWaitIdle(device);

        CleanUpSwapChain();

        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateFramebuffers();
        CreateCommandBuffers();
    }

    void Initialize()
    {
        WINDOW_SIZE_CALLBACK

        VkApplicationInfo applicationInformation = {};
        applicationInformation.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInformation.pApplicationName = "TerraVulkan";
        applicationInformation.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInformation.pEngineName = "Terracraft";
        applicationInformation.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInformation.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo creationInformation = {};
        creationInformation.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        creationInformation.pApplicationInfo = &applicationInformation;

        unsigned int glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef _DEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        auto validationLayers = VulkanHelper::GetAllValidationLayers();
        creationInformation.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        creationInformation.ppEnabledLayerNames = validationLayers.data();
#else
        creationInformation.enabledLayerCount = 0;
#endif
        creationInformation.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        creationInformation.ppEnabledExtensionNames = extensions.data();

        VkResult result = vkCreateInstance(&creationInformation, nullptr, &instance);

        if (result != VK_SUCCESS) 
        {
            std::string errorMessage = "Failed to create Vulkan instance! Code: " + std::to_string(result);

            if (result == VK_ERROR_EXTENSION_NOT_PRESENT) 
            {
                std::string extensionError = "\nRequired extensions are not present. Available extensions:\n";

                uint32_t extensionCount = 0;

                vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
                std::vector<VkExtensionProperties> availableExts(extensionCount);
                vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExts.data());

                for (const auto& ext : availableExts) 
                    extensionError += "\t" + std::string(ext.extensionName) + "\n";
                
                errorMessage += extensionError;
            }

            Logger_ThrowError("VK_FAILURE", errorMessage, true);
        }

#ifdef _DEBUG
        SetupDebugMessenger();
#endif

        Window::GetSurface(Window::window, surface, instance);

        if (!surface)
            Logger_ThrowError("VK_FAILURE", "Window surface is null, stopping now...", true);

        CreatePhysicalDevice();

        CreateLogicalDeviceAndQueue();

        CreateSwapChain();

        CreateImageViews();

        CreateRenderPass();

        CreateFramebuffers();

        CreateCommandPool();

        CreateCommandBuffers();

        CreateSyncObjects();
    }

    void Render() 
    {
        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
        vkResetFences(device, 1, &inFlightFences[currentFrame]);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) 
            Logger_ThrowError("VK_FAILURE", "Failed to submit draw command buffer!", true);

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        vkQueuePresentKHR(graphicsQueue, &presentInfo);

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void CleanUp()
    {
        vkDeviceWaitIdle(device);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }

        vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

        for (auto framebuffer : swapChainFramebuffers)
            vkDestroyFramebuffer(device, framebuffer, nullptr);

        vkDestroyRenderPass(device, renderPass, nullptr);

        for (auto imageView : swapChainImageViews)
            vkDestroyImageView(device, imageView, nullptr);

        vkDestroySwapchainKHR(device, swapChain, nullptr);
        
        vkDestroySurfaceKHR(instance, surface, nullptr);

        vkDestroyCommandPool(device, commandPool, nullptr);

        vkDestroyDevice(device, nullptr);

#ifdef _DEBUG
        VulkanHelper::DestroyDebugUtilsMessenger(instance, debugMessenger, nullptr);
#endif

        vkDestroyInstance(instance, nullptr);
    }
}

#endif // !VULKAN_MANAGER_HPP