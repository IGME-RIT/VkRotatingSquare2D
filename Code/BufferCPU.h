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
#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>

class BufferCPU
{
private:
	VkDeviceMemory memory;
	VkDevice device;

public:
	VkBuffer buffer;

	BufferCPU(
		VkDevice d, 
		VkPhysicalDeviceMemoryProperties memory_properties, 
		VkBufferCreateInfo info);

	~BufferCPU();

	void Store(void* d, int size);
};
