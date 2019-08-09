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
 
#include "Demo.h"
#include "Main.h"
#include <stdio.h>

// Make this global, so it can be initialized in WinMain
// and used in WndProc
Demo* demo;

// keyboard keys
bool keys[255];

// WndProc is the default function that Windows uses to handle
// handle a window. We do not need to call this function ourselves,
// we connect it to the window, and then, the Win32 API calls it 
// automatically. It gives us window size, what keys are pressed while 
// using the window, etc
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	// when the window tries to close
	// this sets msg.message to WM_QUIT
	// in the main loop (in WinMain)
	if (uMsg == WM_CLOSE)
		PostQuitMessage(0);

	// When the window is opened, resized,
	// minimized, or maximized, then rebuild
	// all assets that depend on window size
	else if (uMsg == WM_SIZE && (demo != nullptr))
	{
		demo->width = LOWORD(lParam);
		demo->height = HIWORD(lParam);
		demo->resize();
	}

	// when a key is hit
	// set a member of the "keys" array to true
	else if (uMsg == WM_KEYDOWN)
		keys[(char)wParam] = true;

	// when a key is released
	// set a member of the "keys" array to false
	else if (uMsg == WM_KEYUP)
		keys[(char)wParam] = false;

	// this will be the return statement every time
	// WndProc is called. Keep in mind, this function is 
	// called by the Win32 API, it is not called by us
	// in any of our files of code
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) 
{
	// First we create demo, the demo's constructor will
	// do all the initialization for the whole program.
	// Go to Demo.cpp and look for Demo::Demo to learn
	// about how this works
	demo = new Demo();

	// The main loop of our program.
	// This will repeat infinitely until we tell it to stop
	while (true)
	{
		// MSG is a message that the Window sends to us,
		// it tell us things like "has the X button in the corner
		// of the window been it?"
		MSG msg = {};

		// This function gets a message from the window
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

		// if the windoww sent us a message that the
		// X button in the corner of the window has been hit,
		// or if someone hit the Escape key in the window,
		// (which is determined in Wnd_Proc), then "break" 
		// the loop. "break" quites the loop
		if (msg.message == WM_QUIT || keys[VK_ESCAPE]) break;

		// We are done with the message from this frame,
		// so now dispatch the message, and allow Win32 to 
		// look for a new message in the next frame
		DispatchMessage(&msg);

		// This is the demo's main update function,
		// It will update everything related to the demo.
		// Go to Demo.cpp and look for Demo::run() to learn
		// about how this works
		demo->run();
	}

	// After the loop is finished, it is time to quit the demo.
	// This will call demo's deconstructor, and delete everything
	// that we created in the demo. If we do not delete demo, we 
	// will have memory leaks. Go to Demo.cpp and look for
	// Demo::~Demo() to learn about how this works
	delete demo;

	// This is just a helpful reminder that checks for bugs
	// when it is time to release software, comment this out
	printf("\n\nYou just tried to exit the program\n");
	printf("If any errors occurred while trying to quit,\n");
	printf("They will show up here\n\n");
	printf("Click on this black window, and press Spacebar to close it\n\n");
	system("pause");

	return 0;
}
