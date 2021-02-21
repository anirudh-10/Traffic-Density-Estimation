#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;
bool comp(pair<int,int> x,pair<int,int> y)
{
     if(x.second>y.second)
          return true;
     if(x.second == y.second)
     {
          return x.first>y.first;
     }
     return false;
}
vector<Point2f> source_pts,destination_pts;
vector<pair<int,int>> source_pts_temp,destination_pts_temp;
int number_of_mouse_clicks = 0;
void CLICKDETECTION(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          source_pts.push_back(make_pair(x,y));
          number_of_mouse_clicks++;
     }
     
}
void destination_points (int a , int b) {
     for (int i = 0; i<4 ; i++){
          int x = destination_pts_temp[i].first*a/1920;
          int y = destination_pts_temp[i].second*b/1080;
          destination_pts.push_back(Point2f(x,y));
     }
     

}
int main(int argc, char** argv)
{
     // Read image from file 
     Mat img = imread("test.jpg", IMREAD_GRAYSCALE);

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
     int width_of_inputimage = img.cols; // x coordinate
     int height_of_inputimage=img.rows;  // y coordinate
     destination_pts_temp = {make_pair(472,52),make_pair(472,830),make_pair(800,830),make_pair(800,52)};


     // Wait until user press some key
     while(1){
          waitKey(1);
          if(number_of_mouse_clicks == 4) break;
     }
     cout<<source_pts.size()<<endl;
     
     cout<<"DONE 4 times"<<endl;
     for( auto [a,b]:source_pts)
     {
          cout<<a<<" "<<b<<endl;
     }

     sort(source_pts_temp.begin(),source_pts_temp.end(),comp);
     sort(source_pts_temp.begin(),source_pts_temp.begin()+2);
     sort(source_pts_temp.begin()+2,source_pts_temp.end());
     for(int i = 0; i< 4; i++)
     {
     	source_pts.push_back(Point2f(source_pts_temp[i].first,source_pts_temp[i].second));

     }


     /*
          x*y
          (472*x/1920,52*y/1080)
     */
     // Setting destination points 
     destination_points(width_of_inputimage,height_of_inputimage);

     Mat homography = findHomography(source_pts,destination_pts);
     Mat output_image;
     warpPerspective(img,output_image,homography,img.size());
     imwrite("output.jpeg",output_image);
     Mat outputimg = imread("output.jpeg", IMREAD_GRAYSCALE);

     namedWindow("OUTPUT IMAGE", WINDOW_AUTOSIZE);
     imshow("OUTPUT IMAGE",outputimg);
     //waitKey(0);
     return 0;

}