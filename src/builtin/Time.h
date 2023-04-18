#pragma once

#include <chrono>

static double CurrentTime()
{
	return double(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}