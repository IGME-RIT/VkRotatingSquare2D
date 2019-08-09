/*
Copyright 2019
Original authors: Niko Procopi
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
<http://www.gnu.org/licenses/>.

Special Thanks to Exzap from Team Cemu,
he gave me advice on how to optimize Vulkan
graphics, he is working on a Wii U emulator
that utilizes Vulkan, see more at http://cemu.info
*/

#pragma once

#define APP_NAME_STR_LEN 80
#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>
#include "BufferCPU.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Allow a maximum of two outstanding presentation operations.
#define FRAME_LAG 2

typedef struct {
	VkImage image;
	VkImageView view;

	VkCommandBuffer cmd;
	VkFramebuffer framebuffer;
} SwapchainImageResources;

class Demo
{
public:
	char name[APP_NAME_STR_LEN];  // Name to put on the window/icon
	HWND window;                  // hWnd - window handle
	POINT minsize;                // minimum window size

	VkSurfaceKHR surface;
	bool prepared;
	bool is_minimized;

	bool syncd_with_actual_presents;
	uint64_t refresh_duration;
	uint64_t refresh_duration_multiplier;
	uint64_t target_IPD;  // image present duration (inverse of frame rate)
	uint64_t prev_desired_present_time;
	uint32_t next_present_id;
	uint32_t last_early_id;  // 0 if no early images
	uint32_t last_late_id;   // 0 if no late images

	VkInstance inst;
	VkPhysicalDevice gpu;
	VkDevice device;
	VkQueue queue;
	VkSemaphore image_acquired_semaphores[FRAME_LAG];
	VkSemaphore draw_complete_semaphores[FRAME_LAG];
	VkPhysicalDeviceMemoryProperties memory_properties;

	uint32_t enabled_extension_count;
	uint32_t enabled_layer_count;
	char *extension_names[64];
	char *enabled_layers[64];

	int width, height;
	VkFormat format;
	VkColorSpaceKHR color_space;

	// Function pointers that we get from the instance
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR;
	PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
	PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr;


	// Function pointers that we get from the device
	PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
	PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
	PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;
	PFN_vkQueuePresentKHR fpQueuePresentKHR;

	// swapchain, and the swapchain images
	VkSwapchainKHR swapchain;
	uint32_t swapchainImageCount;
	SwapchainImageResources *swapchain_image_resources;

	// current mode of the swapchain
	VkPresentModeKHR currentPresentMode;

	// fences that are used for drawing
	VkFence drawFences[FRAME_LAG];
	int frame_index;

	BufferCPU* vertexDataCPU;
	BufferCPU* indexDataCPU;

	VkCommandPool cmd_pool;
	VkPipelineLayout pipeline_layout;
	VkDescriptorSetLayout desc_layout;
	VkPipelineCache pipelineCache;
	VkRenderPass render_pass;
	VkPipeline pipeline;

	glm::mat4x4 projection_matrix;
	glm::mat4x4 view_matrix;
	glm::mat4x4 model_matrix;

	BufferCPU* matrixBufferCPU;
	VkDescriptorSet descriptor_set;
	VkDescriptorPool desc_pool;

	bool validate;

	VkShaderModule vert_shader_module;
	VkShaderModule frag_shader_module;

	uint32_t current_buffer;

	void prepare_console();
	void prepare_window();
	void prepare_instance();
	void prepare_physical_device();
	void prepare_instance_functionPointers();
	void prepare_surface();
	void prepare_device_queue();
	void prepare_device_functionPointers();
	void prepare_synchronization();
	void prepare_swapchain();
	void prepare_uniform_buffer();
	void prepare_descriptor_layout();
	void prepare_descriptor_pool();
	void prepare_descriptor_set();
	void prepare_vb_ib();
	void prepare_render_pass();
	void prepare_pipeline();
	void prepare_framebuffers();
	void build_swapchain_cmds();
	void prepare();


	void delete_resolution_dependencies();
	void resize();
	void update_uniform_buffer();
	void draw();
	void run();

	Demo();
	~Demo();
};

