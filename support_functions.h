#pragma once

#include "oneapi/tbb/concurrent_vector.h"
#include <vector>

float chiSquare(float x, float expected);

float colorWeight(float s, float l);

std::vector<float> normalizeLevels(oneapi::tbb::concurrent_vector<float>& levels); // normalize levels in range (0,256)