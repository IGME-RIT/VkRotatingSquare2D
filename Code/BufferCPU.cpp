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

#include "BufferCPU.h"
#include "Helper.h"

// When we create a CPU buffer, we need the Device (lets us give commands to GPU),
// Even though this is a CPU buffer, we still need information from the GPU
// we need the Memory Properties from the PhysicalDevice (same GPU, but the properties of teh GPU),
// and we need the BufferCreateInfo, to tell us what type of buffer this is (uniform, vertex, index, etc)
BufferCPU::BufferCPU(VkDevice d, VkPhysicalDeviceMemoryProperties memory_properties, VkBufferCreateInfo info)
{
	// save device, so that
	// we can use it to store
	// data, and delete data
	device = d;

	// create buffer with the device,
	// and the VkBufferCreateInfo
	vkCreateBuffer(device, &info, NULL, &buffer);

	// get memory requirements, so that we know
	// what we need in order to allocate the memory
	VkMemoryRequirements mem_reqs;
	vkGetBufferMemoryRequirements(device, buffer, &mem_reqs);

	// create allocation information
	// this uses the memory requirements
	// to tell the device how to allocate the buffer
	VkMemoryAllocateInfo memAllocInfo = {};
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllocInfo.allocationSize = mem_reqs.size;

	// There are only a few different combinations of 
	// memory properties that Vulkan supports. The
	// combination we want to use here is
	// HOST_VISIBLE and HOST_COHERENT
	
	// Here is a list of every possible combination
	// https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkPhysicalDeviceMemoryProperties.html

	// find memory on the CPU's RAM
	// HOST_VISIBLE says we are in CPU's RAM
	// HOST_COHERENT says we can use vkMapMemory

	// This function writes to the memoryTypeIndex variable
	// that is inside our VkMemoryAllocationInfo.

	// memory_properties is a structure of VkPhysicalDeviceMemoryProperties
	// which holds arrays of memory types (VkMemoryType), and arrays of memory heaps (VkMemoryHeaps)
	// memoryTypeIndex is an index identifying a memory type from the memoryTypes array
	Helper::memory_type_from_properties(
		memory_properties,
		mem_reqs.memoryTypeBits,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&memAllocInfo.memoryTypeIndex);

	// allocate memory
	// our memoryAllocationInfo got the required size
	// from the VkMemoryRequirements, and it got
	// the type of memory from memory_type_from_properties
	vkAllocateMemory(device, &memAllocInfo, NULL, &memory);

	// After our memory is allocated, we have
	// to bind the buffer to the memory, so that
	// we can use the memory
	vkBindBufferMemory(device, buffer, memory, 0);
}

BufferCPU::~BufferCPU()
{
	// when we want to delete this CPU memory
	// we delete the buffer, which is what we
	// used to access the memory
	vkDestroyBuffer(device, buffer, NULL);

	// after deleting the buffer, there is no
	// way to access the memory, so we delete
	// the memory
	vkFreeMemory(device, memory, NULL);
}

void BufferCPU::Store(void* d, int size)
{
	// create a pointer that does not go anywhere
	uint8_t *pData = nullptr;

	// Basically, this gives us a pointer to where
	// the buffer's memory is stored in RAM. 
	vkMapMemory(device, memory, 0, size, 0, (void **)&pData);

	// pData now points to the location in RAM
	// where the buffer's memory is,
	// so we use memcpy to transfer data into
	// the buffer's memory
	memcpy(pData, d, size);
	
	// we unmap the memory, because we don't need
	// to write to it, so we leave it alone
	vkUnmapMemory(device, memory);
}
