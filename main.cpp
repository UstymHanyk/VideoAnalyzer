
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "tbb/concurrent_queue.h"
#include "oneapi/tbb/concurrent_hash_map.h"

#include "thread_functions.h"
#include "frame_divider.h"

#include <iostream>
#include <thread>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    int hueRange = 256;
    int levelsHeight = 256;

    int num_of_frame_threads = 3;
    int num_of_merging_threads = 2;

    oneapi::tbb::concurrent_bounded_queue<cv::Mat> frame_queue;
    vector<thread> frame_threads;
    frame_threads.reserve(num_of_frame_threads);

    oneapi::tbb::concurrent_bounded_queue<std::map<int, float>> levels_queue;
    vector<thread> levels_threads;
    levels_threads.reserve(num_of_merging_threads);

    oneapi::tbb::concurrent_vector<float> global_levels(hueRange, 0.0);

    int num_of_working_threads = num_of_frame_threads;
    std::mutex working_threads_mutex;


    string path = "./test.mp4";
    VideoCapture cap(path);

    if (!cap.isOpened()) {
        std::cout << "cannot read video!\n";
        exit(1);
    }


    int totalFrameCount = cap.get(CAP_PROP_FRAME_COUNT);
    int inBetweenFrameTime = totalFrameCount / 100;  // in total 20 frames will be analyzed
    
    std::thread frame_read_thread(divide_on_frames, std::ref(cap), inBetweenFrameTime, std::ref(frame_queue));

    for (size_t i = 0; i < num_of_frame_threads; i++) {
        frame_threads.emplace_back(ovrework_frame, std::ref(frame_queue), std::ref(levels_queue), std::ref(num_of_working_threads), std::ref(working_threads_mutex));
    }

    for (size_t i = 0; i < num_of_merging_threads; i++) {
        levels_threads.emplace_back(merge_levels, std::ref(levels_queue), std::ref(global_levels));
    }


    std::cout << "All threads started \n";

    try {
        if (frame_read_thread.joinable()) {
            frame_read_thread.join();
        }

        for (auto& thread : frame_threads) {
            std::cout << "trying to finish threads\n";
            if (thread.joinable()) {
                thread.join();
            }
        }

        
        for (auto& thread : levels_threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
    catch (std::error_code e) {
        std::cout << "Error while loining threads " << e << std::endl;
    }
    
    
    Mat colorSpectre(144, hueRange, CV_8UC3, Scalar(0, 0, 0));
    vector<float> normalizedlevels = normalizeLevels(global_levels);
    for (int i = 1; i < hueRange; i++)
    {
        line(colorSpectre, Point((i - 1) * 4, levelsHeight * 2 - normalizedlevels[i - 1]),
            Point(i * 4, levelsHeight * 2 - normalizedlevels[i]),
            Scalar(255, 255, 255), 1, 8, 2);
    }

    imshow("Color Scpectre", colorSpectre);
    waitKey();
    cap.release();



    return EXIT_SUCCESS;
}



