
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "tbb/concurrent_queue.h"
#include "oneapi/tbb/concurrent_hash_map.h"

#include "thread_functions.h"

#include <iostream>
#include <thread>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    int hueRange = 256;
    int levelsHeight = 256;
    vector<float> levels(hueRange, 0.0);



    string path = "./test.mp4";
    VideoCapture cap(path);

    if (!cap.isOpened()) {
        std::cout << "cannot read video!\n";
        exit(1);
    }

    Mat frame;
    Mat hslFrame;
    int frameCount = 0;
    int totalFrameCount = cap.get(CAP_PROP_FRAME_COUNT);
    int inBetweenFrameTime = totalFrameCount / 20;  // in total 20 frames will be analyzed
    while (true) {
        cap.read(frame);

        if (!frame.empty()) {
            frameCount += inBetweenFrameTime; // analyze every n-th frame
            cap.set(CAP_PROP_POS_FRAMES, frameCount);
            resize(frame, frame, Size(256, 144), INTER_LINEAR);
            cvtColor(frame, hslFrame, COLOR_BGR2HLS_FULL);


            // Iterate over all pixels of the image
            for (int row = 0; row < hslFrame.rows; row++) {
                // get pointer to the beginning of the row
                Vec3b* ptr = hslFrame.ptr<Vec3b>(row);

                for (int col = 0; col < hslFrame.cols; col++) {
                    levels[(int)ptr[col][0]] += colorWeight(((float)ptr[col][2]) / 100, ((float)ptr[col][1]) / 100);

                }
            }

            imshow("Source video", frame);
            waitKey(10);
        }







        else {
            Mat colorSpectre(144, hueRange, CV_8UC3, Scalar(0, 0, 0));
            vector<float> normalizedlevels = normalizeLevels(levels);
            for (int i = 1; i < hueRange; i++)
            {
                line(colorSpectre, Point((i - 1) * 4, levelsHeight * 2 - normalizedlevels[i - 1]),
                    Point(i * 4, levelsHeight * 2 - normalizedlevels[i]),
                    Scalar(255, 255, 255), 1, 8, 2);
            }

            imshow("Color Scpectre", colorSpectre);
            waitKey();
            cap.release();
            break;
        }

    }
    return EXIT_SUCCESS;
}



