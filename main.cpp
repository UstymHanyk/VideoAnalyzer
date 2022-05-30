
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "tbb/concurrent_queue.h"
#include "oneapi/tbb/concurrent_hash_map.h"

#include "thread_functions.h"
#include "frame_divider.h"

#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    int num_of_frame_threads;
    int num_of_merging_threads;
    string path;
    bool do_write;
    bool show;

    if (argc < 2) {
        num_of_frame_threads = 14;
        num_of_merging_threads = 10;
        path = "joker_trailer.mp4";
        do_write = false;
        show = true;
    }
    else {
        num_of_frame_threads = stoi(argv[1]);
        num_of_merging_threads = stoi(argv[2]);
        path = argv[3];
        do_write = stoi(argv[4]);
        show = stoi(argv[5]);
    }


    int hueRange = 256;
    int levelsHeight = 256;


    oneapi::tbb::concurrent_bounded_queue<cv::Mat> frame_queue;
    vector<thread> frame_threads;
    frame_threads.reserve(num_of_frame_threads);

    oneapi::tbb::concurrent_bounded_queue<std::map<int, float>> levels_queue;
    vector<thread> levels_threads;
    levels_threads.reserve(num_of_merging_threads);

    oneapi::tbb::concurrent_vector<float> global_levels(hueRange, 0.0);

    int num_of_working_threads = num_of_frame_threads;
    std::mutex working_threads_mutex;

    auto time_start = std::chrono::high_resolution_clock::now();


    VideoCapture cap(path);

    if (!cap.isOpened()) {
        std::cout << "cannot read video!\n";
        exit(1);
    }


    int totalFrameCount = cap.get(CAP_PROP_FRAME_COUNT);
    int inBetweenFrameTime = totalFrameCount / 500;  // in total 20 frames will be analyzed
    
    std::thread frame_read_thread(divide_on_frames, std::ref(cap), inBetweenFrameTime, std::ref(frame_queue));

    for (size_t i = 0; i < num_of_frame_threads; i++) {
        frame_threads.emplace_back(ovrework_frame, std::ref(frame_queue), std::ref(levels_queue), std::ref(num_of_working_threads), std::ref(working_threads_mutex));
    }

    for (size_t i = 0; i < num_of_merging_threads; i++) {
        levels_threads.emplace_back(merge_levels, std::ref(levels_queue), std::ref(global_levels));
    }



    try {
        if (frame_read_thread.joinable()) {
            frame_read_thread.join();
        }

        for (auto& thread : frame_threads) {
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
    
    // value which the default image will be upscaled by. Original size is 256x128 px. 
    int upscaleFactor = 4;  

    Mat colorSpectrum(128 * upscaleFactor, hueRange * upscaleFactor, CV_8UC3, Scalar(0, 0, 0));
    vector<float> normalizedlevels = normalizeLevels(global_levels);
    for (int i = 1; i < hueRange; i++)
    {

        line(colorSpectrum, Point(i * 4 * upscaleFactor, 0),
            Point(i * 4 * upscaleFactor, levelsHeight * 2 * upscaleFactor),
            hueToWeightedBgr(i, normalizedlevels[i]), 3, 8, 2);

        line(colorSpectrum, Point((i - 1) * 4 * upscaleFactor, (levelsHeight - normalizedlevels[i - 1]) * 2 * upscaleFactor),
            Point(i * 4 * upscaleFactor, (levelsHeight - normalizedlevels[i]) * 2 * upscaleFactor),
            Scalar(255, 255, 255), 1, LINE_AA, 2);
    }

    auto time_stop = std::chrono::high_resolution_clock::now();

    long long total_time = std::chrono::duration_cast<std::chrono::microseconds>( time_stop - time_start ).count();
    std::string name = "Color Spectrum_" + path;

    std::cout << "Total time: " << total_time << std::endl;

    if (show) {
        imshow(name, colorSpectrum);
    }
    
    if (do_write) {
        name += num_of_frame_threads;
        name += ".png";
        imwrite(name, colorSpectrum);
        
    }
    waitKey();
    cap.release();



    return EXIT_SUCCESS;
}



