#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;
vector<Point2f> source_pts,destination_pts;
int number_of_mouse_clicks = 0;
void CLICKDETECTION(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          source_pts.push_back(Point2f(x,y));
          number_of_mouse_clicks++;
     }
     
}

int main(int argc, char** argv)
{
     // Read image from file 
     Mat img = imread("opencv_testimage.jpeg", IMREAD_GRAYSCALE);

     //if fail to read the image
     if ( !img.data ) 
     { 
          cout <<  "Image not found or unable to open" << endl;
          return -1; 
     }
     cout<<source_pts.size()<<endl;
     //Create a window
     namedWindow("Original Image", WINDOW_AUTOSIZE);

     //set the callback function for any mouse event
     setMouseCallback("Original Image", CLICKDETECTION, NULL);

     //show the image
     imshow("Original Image", img);

     // Wait until user press some key
     while(1){
          waitKey(1);
          if(number_of_mouse_clicks == 4) break;
     }
     cout<<source_pts.size()<<endl;
     
     cout<<"DONE 4 times"<<endl;

     // Setting destination points 
     destination_pts= {Point2f(472,52),Point2f(472,830),Point2f(800,830),Point2f(800,52)};
     Mat homography = findHomography(source_pts,destination_pts);
     Mat output_image;
     warpPerspective(img,output_image,homography,img.size());
     imwrite("output.jpeg",output_image);
     namedWindow("OUTPUT IMAGE", WINDOW_AUTOSIZE);
     imshow("OUTPUT IMAGE",output_image);
     return 0;

}