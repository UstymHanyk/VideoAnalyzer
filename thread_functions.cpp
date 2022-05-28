#include "thread_functions.h"


void ovrework_frame(oneapi::tbb::concurrent_bounded_queue<cv::Mat>& frames, oneapi::tbb::concurrent_bounded_queue<std::map<int, float>>& levels_queue) {

    cv::Mat frame;
    cv::Mat hslFrame;
    std::map<int, float> levels;

    frames.pop(frame);

    resize(frame, frame, cv::Size(256, 144), cv::InterpolationFlags::INTER_LINEAR);
    cvtColor(frame, hslFrame, cv::ColorConversionCodes::COLOR_BGR2HLS_FULL);


    // Iterate over all pixels of the image
    for (int row = 0; row < hslFrame.rows; row++) {
        // get pointer to the beginning of the row
        cv::Vec3b* ptr = hslFrame.ptr<cv::Vec3b>(row);

        for (int col = 0; col < hslFrame.cols; col++) {
            float colWei = colorWeight(((float)ptr[col][2]) / 100, ((float)ptr[col][1]) / 100);
            if (levels.find((int)ptr[col][0]) != levels.end()){
                levels.find((int)ptr[col][0])->second += colWei;
            }
            else {
                levels.insert({ (int)ptr[col][0], colWei });
            }

        }
    }

    levels_queue.push(levels);
}

void merge_levels(oneapi::tbb::concurrent_bounded_queue<std::map<int, float>>& levels_queue, oneapi::tbb::concurrent_hash_map<int, float> global_levels) {
    
    std::map<int, float> levels;
    levels_queue.pop(levels);
    oneapi::tbb::concurrent_hash_map<int, float>::accessor a;

    for (auto &i: levels) {
        global_levels.insert(a, i.first);
        a->second += i.second;
        a.release();
    }
    

}

