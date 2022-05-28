#pragma once

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "tbb/concurrent_queue.h"
#include "support_functions.h"
#include "oneapi/tbb/concurrent_hash_map.h"

#include <iostream>
#include <vector>
#include <map>

void ovrework_frame(oneapi::tbb::concurrent_bounded_queue<cv::Mat>& frames, oneapi::tbb::concurrent_bounded_queue<std::map<int, float>>& levels);

void merge_levels(oneapi::tbb::concurrent_bounded_queue<std::map<int, float>>& levels_queue, oneapi::tbb::concurrent_hash_map<int, float> global_levels);
