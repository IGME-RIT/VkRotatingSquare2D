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

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#if defined(NDEBUG) && defined(__GNUC__)
#define U_ASSERT_ONLY __attribute__((unused))
#else
#define U_ASSERT_ONLY
#endif

#if defined(__GNUC__)
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

#define ERR_EXIT(err_msg, err_class)                                             \
    do {                                                                         \
        MessageBox(NULL, err_msg, err_class, MB_OK); \
        exit(1);                                                                 \
    } while (0)

class Helper
{
public:
	static void DbgMsg(char *fmt, ...);

	static bool memory_type_from_properties(
		VkPhysicalDeviceMemoryProperties memory_properties,
		uint32_t typeBits,
		VkFlags requirements_mask,
		uint32_t *typeIndex);
	
	static void ReadFile(const char* path, char** data, int* size);
};

