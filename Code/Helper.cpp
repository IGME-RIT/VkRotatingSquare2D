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

#include "Helper.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <signal.h>
#include <vector>

void Helper::DbgMsg(char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vprintf(fmt, va);
	va_end(va);
	fflush(stdout);
}

// This is a common helper function that is used in many Vulkan
// tutorials. From one tutorial to the next, it does not require
// a lot of changes

// memory_properties is a structure of VkPhysicalDeviceMemoryProperties
// which holds arrays of memory types (VkMemoryType), and arrays of memory heaps (VkMemoryHeaps)
// memoryTypeIndex is an index identifying a memory type from the memoryTypes array

// typeBits are gotten from memory requirements (you'll see that in the buffer classes)
// requirements_mask tells us if the buffer is on CPU or GPU, and read/write access permissions

bool Helper::memory_type_from_properties(VkPhysicalDeviceMemoryProperties memory_properties, uint32_t requiredBits, VkFlags requirements_mask, uint32_t *typeIndex)
{
	// If students have trouble understanding this function, 
	// don't worry it will never need to be changed

	// Loop through all memory types that are available to us
	// Search memtypes to find first index of the memoryType array
	// that has the properties that we need

	// There are usually around 11 types of memory
	// Here is a list of them
	// https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkMemoryPropertyFlagBits.html
	// Notice a pattern? The list of types look like this:
	// 0001 = 1
	// 0010 = 2
	// 0100 = 4
	// 1000 = 8
	// etc

	for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
	{
		// Check if this type is needed from VkMemoryRequirements
		// this checks if the last bit in the byte is 1
		if ((requiredBits & 1) == 1)
		{
			// If we need this type of daa
			// check to see if this memoryType supports the required
			// mask (device, local, coherent, etc)
			if ((memory_properties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask)
			{
				// if it does, then record this data
				*typeIndex = i;
				
				// we found it, so return true
				return true;
			}
		}

		// this shifts all bits to the right,
		// notice the pattern of FlagBits
		requiredBits >>= 1;
	}

	// No memory types matched, return failure
	// There is no index in the memoryType array
	// that supports the memory that we want to allocate
	return false;
}

// This function simply reads a file,
// records every byte into an array of bytes,
// and records the size. This can be used
// for textures and shaders
void Helper::ReadFile(const char* path, char** data, int* size)
{
	// open the file 
	FILE *fp = fopen(path, "rb");

	// Go to the end of the file
	fseek(fp, 0L, SEEK_END);

	// see how far away the end of 
	// the file was, from the beginning,
	// and record that number as size
	*size = ftell(fp);

	// go back to the beginning
	rewind(fp);

	// allocate memory that is big enough
	// for all bytes in the file
	*data = (char*)calloc(1, *size + 1);

	// dump every byte from the file
	// into our array of bytes
	fread(*data, *size, 1, fp);

	// close the file
	fclose(fp);
}