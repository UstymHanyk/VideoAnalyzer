#pragma once

#include "oneapi/tbb/concurrent_vector.h"
#include <vector>
#include "opencv2/imgproc.hpp"
using namespace cv;

float chiSquare(float x, float expected);

float colorWeight(float s, float l);

Scalar hueToWeightedBgr(int hue, double hueAmount);

std::vector<float> normalizeLevels(oneapi::tbb::concurrent_vector<float>& levels); // normalize levels in range (0,256)