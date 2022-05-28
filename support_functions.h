#pragma once

#include <vector>

float chiSquare(float x, float expected) {
	float expectedSquare = pow(expected, 2);
	return (expectedSquare - pow(expected - x, 2)) / expectedSquare;
}


float colorWeight(float s, float l) {
	float median = 1.27;
	float hueValueRange = 256;
	float weight = s * chiSquare(l, median);
	return weight;
}

std::vector<float> normalizeLevels(std::vector<float> levels) { // normalize levels in range (0,256)
    float maxLevel = *max_element(levels.begin(), levels.end());
    float minLevel = *min_element(levels.begin(), levels.end());
    float maxDiff = maxLevel - minLevel;

    std::vector<float> normalizedLevels(256, 0.0);
    for (int i = 0; i < 256; i++) {
        normalizedLevels[i] = ((levels[i] - minLevel) * 256) / maxDiff;
    }
    return normalizedLevels;
}