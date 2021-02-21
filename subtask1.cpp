#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;
vector<Point2f> pnt_src,pnt_dst;
void CLICKDETECTION(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          pnt_src.push_back(Point2f(x,y));
     }
     
}

int main(int argc, char** argv)
{
     // Read image from file 
     Mat img = imread("opencv_testimage.jpeg");

     //if fail to read the image
     if ( !img.data ) 
     { 
          cout <<  "Image not found or unable to open" << endl;
          return -1; 
     }
     cout<<pnt_src.size()<<endl;
     //Create a window
     namedWindow("Original Image", WINDOW_AUTOSIZE);

     //set the callback function for any mouse event
     setMouseCallback("Original Image", CLICKDETECTION, NULL);

     //show the image
     imshow("Original Image", img);

     // Wait until user press some key
     	waitKey(0);
     cout<<pnt_src.size()<<endl;
     
     cout<<"DONE 4 times"<<endl;

     return 0;

}