#pragma once
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "tbb/concurrent_queue.h"

#include <iostream>


void divide_on_frames(cv::VideoCapture& cap, const int inBetweenFrameTime, oneapi::tbb::concurrent_bounded_queue<cv::Mat>& frames);