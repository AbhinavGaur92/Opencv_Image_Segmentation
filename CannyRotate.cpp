#include <opencv2/opencv.hpp> 
#include <iostream> 

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // Read the image file 
    Mat image = imread("<img_path");
    // Check for failure 
    if (image.empty())
    {
        cout << "Image Not Found!!!" << endl;
        cin.get(); //wait for any key press 
        return -1;
    }
    // Show our image inside a window. 
    imshow("image_window", image);

    /////////////////
    Mat image_gray,canny_gray;
    cvtColor(image, image_gray, COLOR_BGR2GRAY);
    Canny(image_gray, canny_gray, 10, 25, 3);
    imshow("canny_window", canny_gray);
    /////////////////
    // 
    // //////////////
    cv::Point2f center((image.cols - 1) / 2.0, (image.rows - 1) / 2.0);
    float angle = 10.0;
    cv::Mat matRotation = cv::getRotationMatrix2D(center, angle, 1.0);

    cv::Mat rotated_image;

    cv::warpAffine(canny_gray, rotated_image, matRotation, image.size());

    cv::imshow("rotated_window", rotated_image);

    // //////////////
    // Wait for any keystroke in the window 
    waitKey(0);
    return 0;
}
