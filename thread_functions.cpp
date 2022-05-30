#include "thread_functions.h"


void ovrework_frame(oneapi::tbb::concurrent_bounded_queue<cv::Mat>& frames, oneapi::tbb::concurrent_bounded_queue<std::map<int, float>>& levels_queue, int &num_of_working_threads, std::mutex &working_threads_mutex) {

    cv::Mat frame;
    cv::Mat hslFrame;
    std::map<int, float> levels;

    frames.pop(frame);
     
    
    // Iterate over all pixels of the image
    while (!frame.empty())
    {
        cvtColor(frame, hslFrame, cv::ColorConversionCodes::COLOR_BGR2HLS_FULL);
        std::map<int, float> levels;
        for (int row = 0; row < hslFrame.rows; row++) {
            // get pointer to the beginning of the row
            cv::Vec3b* ptr = hslFrame.ptr<cv::Vec3b>(row);

            for (int col = 0; col < hslFrame.cols; col++) {
                float colWei = colorWeight(((float)ptr[col][2]) / 100, ((float)ptr[col][1]) / 100);
                int key = ptr[col][0];
                if (levels.find(key) != levels.end()) {
                    levels.find(key)->second += colWei;
                }
                else {
                    levels.insert({ key, colWei });
                }

            }
        }
        levels_queue.push(levels);
        frames.pop(frame);
    }

    frames.push(frame);
    if (num_of_working_threads == 1){
        levels.clear();
        levels_queue.push(levels);
    }
    else {
        working_threads_mutex.lock();
        num_of_working_threads--;
        working_threads_mutex.unlock();
    }

}

void merge_levels(oneapi::tbb::concurrent_bounded_queue<std::map<int, float>>& levels_queue, oneapi::tbb::concurrent_vector<float>& global_levels) {
    
    std::map<int, float> levels;
    levels_queue.pop(levels);
    while (!levels.empty())
    {
        for (auto& i : levels) {
            global_levels[i.first] += i.second;
        }
        levels.clear();
        levels_queue.pop(levels);
    }

    levels.clear();
    levels_queue.push(levels);
    

}

