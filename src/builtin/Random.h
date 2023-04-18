#pragma once

#include <random>

#include "Time.h"

static double Random(double min, double max)
{
	static std::default_random_engine generator = std::default_random_engine(unsigned int(CurrentTime()));
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);

	return min + (max - min) * distribution(generator);
}