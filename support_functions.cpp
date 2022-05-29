#include "support_functions.h"

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

std::vector<float> normalizeLevels(oneapi::tbb::concurrent_vector<float>& levels) { // normalize levels in range (0,256)

    float maxLevel = levels[0];
    float minLevel = levels[0];

    for (auto& el : levels) {
        if (el > maxLevel) {
            maxLevel = el;
        }
        if (el < minLevel) {
            minLevel = el;
        }
    }
    float maxDiff = maxLevel - minLevel;

    std::vector<float> normalizedLevels(256, 0.0);
    for (int i = 0; i < 256; i++) {
        normalizedLevels[i] = ((levels[i] - minLevel) * 256) / maxDiff;
    }
    return normalizedLevels;
}

Scalar hueToWeightedBgr(int hue, double hueAmount) {
    // Converts hue and its amount to rgb value of the hue. Its luminance will increase accordingly to the hueAmount
    // partially taken from https://www.codespeedy.com/hsv-to-rgb-in-cpp/
    
    // Opencv hue is in range 0-256, while the standart range is 0-360
    // therefore we will multiply it by 1.4 to reach standart hue values
    float hueCorrected = hue * 1.406;



    float saturation = 1;
    float value = hueAmount / 255;
    float C = saturation * value;
    float X = C * (1 - fabs(fmod(hueCorrected / 60.0, 2) - 1));
    float m = value - C;
    float r, g, b;
    if (hueCorrected >= 0 && hueCorrected < 60) {
        r = C, g = X, b = 0;
    }
    else if (hueCorrected >= 60 && hueCorrected < 120) {
        r = X, g = C, b = 0;
    }
    else if (hueCorrected >= 120 && hueCorrected < 180) {
        r = 0, g = C, b = X;
    }
    else if (hueCorrected >= 180 && hueCorrected < 240) {
        r = 0, g = X, b = C;
    }
    else if (hueCorrected >= 240 && hueCorrected < 300) {
        r = X, g = 0, b = C;
    }
    else {
        r = C, g = 0, b = X;
    }

    int R = (r + m) * 255;
    int G = (g + m) * 255;
    int B = (b + m) * 255;
    return Scalar(B, G, R);
}