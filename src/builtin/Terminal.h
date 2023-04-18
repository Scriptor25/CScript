#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <algorithm>
#include <iostream>

#include <Windows.h>

static void ClearTerminal()
{
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		// TODO: Handle failure!
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}

static int StringToVK(std::string key)
{
	std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return std::tolower(c); });

	if (key == "lctrl")
	{
		return VK_LCONTROL;
	}
	else if (key == "rctrl")
	{
		return VK_RCONTROL;
	}
	else if (key == "lshift")
	{
		return VK_LSHIFT;
	}
	else if (key == "rshift")
	{
		return VK_RSHIFT;
	}
	else if (key == "up")
	{
		return VK_UP;
	}
	else if (key == "down")
	{
		return VK_DOWN;
	}
	else if (key == "left")
	{
		return VK_LEFT;
	}
	else  if (key == "right")
	{
		return VK_RIGHT;
	}
	else if (key == "esc")
	{
		return VK_ESCAPE;
	}
	else if (key == "shift")
	{
		return VK_SHIFT;
	}
	else if (key == "ctrl")
	{
		return VK_CONTROL;
	}
	else if (key == "tab")
	{
		return VK_TAB;
	}
	else if (key == "f1")
	{
		return VK_F1;
	}
	else if (key == "f2")
	{
		return VK_F2;
	}
	else if (key == "f3")
	{
		return VK_F3;
	}
	else if (key == "f4")
	{
		return VK_F4;
	}
	else if (key == "f5")
	{
		return VK_F5;
	}
	else if (key == "f6")
	{
		return VK_F6;
	}
	else if (key == "f7")
	{
		return VK_F7;
	}
	else if (key == "f8")
	{
		return VK_F8;
	}
	else if (key == "f9")
	{
		return VK_F9;
	}
	else if (key == "f10")
	{
		return VK_F10;
	}
	else if (key == "f11")
	{
		return VK_F11;
	}
	else if (key == "f12")
	{
		return VK_F12;
	}
	else if (key == "f13")
	{
		return VK_F13;
	}
	else if (key == "f14")
	{
		return VK_F14;
	}
	else if (key == "f15")
	{
		return VK_F15;
	}
	else if (key == "f16")
	{
		return VK_F16;
	}
	else if (key == "f17")
	{
		return VK_F17;
	}
	else if (key == "f18")
	{
		return VK_F18;
	}
	else if (key == "f19")
	{
		return VK_F19;
	}
	else if (key == "f20")
	{
		return VK_F20;
	}
	else if (key == "f21")
	{
		return VK_F21;
	}
	else if (key == "f22")
	{
		return VK_F22;
	}
	else if (key == "f23")
	{
		return VK_F23;
	}
	else if (key == "f24")
	{
		return VK_F24;
	}
	else if (key == "del")
	{
		return VK_DELETE;
	}

	throw RuntimeException{ "unknown key code '" + key + "'" };
}

struct KeyState
{
	bool down;
	bool toggle;
};

static KeyState GetKey(int key)
{
	SHORT state = GetKeyState(key);

	bool down = state < 0;
	bool toggle = (state & 1) != 0;

	return { down, toggle };
}

static KeyState GetKey(std::string key)
{
	int vk;
	if (key.length() > 1) vk = StringToVK(key);
	else vk = std::toupper(key[0]);

	return GetKey(vk);
}