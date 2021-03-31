#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <pthread.h>
#include <thread>
#include <stdlib.h>
#include <unistd.h>

using namespace std;
using namespace cv;
// Comparator Function for sorting of Source points( As Clicked By User)
bool comp(pair<int,int> x,pair<int,int> y)
{
    if(x.second<y.second)
        return true;
    if(x.second == y.second)
    {
        return x.first<y.first;
    }
    return false;
}

vector<double> file_output;
// Global Vectors for storing points
vector<Point2f> source_pts,destination_pts;
vector<pair<int,int>> source_pts_temp,destination_pts_temp;

Mat homography;

// Global Variable for storing number of times left mouse button is clicked on the displayed image
int number_of_mouse_clicks = 0;

// Processing Clicked Points
void CLICKDETECTION(int event, int x, int y, int flags, void* userdata)
{
    if( event == EVENT_LBUTTONDOWN and number_of_mouse_clicks < 4 )
    {
        source_pts_temp.push_back(make_pair(x,y));
        number_of_mouse_clicks++;
        if(number_of_mouse_clicks == 4)
        {
            destroyWindow("Original Frame");
        }
    }
     
}

// AutoScaling Destinantion Points according to size of image
/*
    We used Riju Ma'am destination point coordinates. But if original image has small size then there could be a issue of 
    going out of boundary. So to accomodate that we scaled the destination points using the normal image size as 1920x1080. 
*/
void AutoScaling (int a , int b) {
    for (int i = 0; i<4 ; i++){
        destination_pts_temp[i].first = destination_pts_temp[i].first*a/1920;
        destination_pts_temp[i].second = destination_pts_temp[i].second*b/1080;
        int x = destination_pts_temp[i].first;
        int y = destination_pts_temp[i].second;
        destination_pts.push_back(Point2f(x,y));
    } 
}

void empty_image(string s, Mat&crop)
{
    // Reading Image
    Mat img = imread(s, IMREAD_GRAYSCALE);

    // Error Reading Image
    if ( !img.data ) 
    { 
        throw std::invalid_argument( "Image File not found or unable to open");
        return; 
    }

    // Creating a Window Frame
    namedWindow("Original Frame",  WINDOW_AUTOSIZE);
    
    // Detecting a Mouse Click
    setMouseCallback("Original Frame", CLICKDETECTION, NULL);

    // Showing the Image Frame
    imshow("Original Frame", img);

    // Storing size of image for Autoscaling
    int width_of_inputimage = img.cols; // x coordinate
    int height_of_inputimage=img.rows;  // y coordinate

    // Storing Points in the order (Top Left,Top Right,Bottom Left,Bottom Right)
    destination_pts_temp = {make_pair(472,52),make_pair(800,52),make_pair(472,830),make_pair(800,830)};

    /*
        Automatially Closing the image frame when user clicks 4 points(handled in CLICKDETECTION function).
        If 4 clicks not detected on the frame and the frame is closed or a key is pressed then raising an exception.

    */
    waitKey(0);
    if(cv::getWindowProperty("Original Frame",cv::WND_PROP_VISIBLE)!=1 and number_of_mouse_clicks < 4)
    {
        throw std::invalid_argument( "Image Closed before selecting 4 points");
        return;
    }
    
    // Hardcoding the points
    source_pts_temp[0]=make_pair(464,1008);
    source_pts_temp[1]=make_pair(997,213);
    source_pts_temp[2]=make_pair(1265,197);
    source_pts_temp[3]=make_pair(1513,1012);
    
    // Ordering the Points Clicked by the user according to (Top Left,Top Right,Bottom Left,Bottom Right)
    sort(source_pts_temp.begin(),source_pts_temp.end(),comp);
    sort(source_pts_temp.begin(),source_pts_temp.begin()+2);
    sort(source_pts_temp.begin()+2,source_pts_temp.end());
    
    // Storing Source points in the OpenCV coordinates format
    for(int i = 0; i< 4; i++)
    {
        source_pts.push_back(Point2f(source_pts_temp[i].first,source_pts_temp[i].second));
    }

    //Calling the autoscaling Function
    AutoScaling(width_of_inputimage,height_of_inputimage);
     
    // Finding the homography matrix for given 2 set of points
    homography = findHomography(source_pts,destination_pts);

    // Defining Matrix for the projected image
    Mat projected_image;

    // Correcting the Camera Angle based on the found Homography Matrix
    warpPerspective(img,projected_image,homography,img.size());

    
    // Showing the Projected Image 
    // Defining the Cropped Coordinates for Cropping the Image
    Rect cropped_coordinates;

    //Setting the Top Left Coordinate from where the Image will be Cropped
    cropped_coordinates.x = destination_pts_temp[0].first;
    cropped_coordinates.y = destination_pts_temp[0].second;

    // Size of the Cropped Image
    cropped_coordinates.width = destination_pts_temp[1].first - destination_pts_temp[0].first;
    cropped_coordinates.height = destination_pts_temp[2].second - destination_pts_temp[0].second;

    // Forming the Matrix for Cropped Image 
    crop = projected_image(cropped_coordinates);

    
    return;
}
void homography_of_frames(Mat img,Mat&crop)
{

    // Defining Matrix for the projected image
    Mat projected_image;

    // Correcting the Camera Angle based on the found Homography Matrix
    warpPerspective(img,projected_image,homography,img.size());

    
    // Showing the Projected Image 
    // Defining the Cropped Coordinates for Cropping the Image
    Rect cropped_coordinates;

    //Setting the Top Left Coordinate from where the Image will be Cropped
    cropped_coordinates.x = destination_pts_temp[0].first;
    cropped_coordinates.y = destination_pts_temp[0].second;

    // Size of the Cropped Image
    cropped_coordinates.width = destination_pts_temp[1].first - destination_pts_temp[0].first;
    cropped_coordinates.height = destination_pts_temp[2].second - destination_pts_temp[0].second;

    // Forming the Matrix for Cropped Image 
    crop = projected_image(cropped_coordinates);

}

// Storing Each threads local data for computation
struct thread_data{
    int start;
    int end;
    int total_pixels;
    int static_pixels;
    Mat emptyimg;
    int e_static;
    Mat cropped_frame_prev;
    Mat cropped_frame;
    int l;
};

// The function each thread executes
void *frame_iterate(void *input){

    struct thread_data *my_data;
    my_data = (struct thread_data *) input;

    int start = my_data->start;
    int end = my_data->end;
    int total_pixels = my_data->total_pixels;
    int static_pixels = my_data->static_pixels;
    Mat emptyimg = my_data->emptyimg;
    int e_static = my_data->e_static;
    Mat cropped_frame_prev = my_data->cropped_frame_prev;
    Mat cropped_frame = my_data->cropped_frame;
    int l = my_data->l;
    
    for(int i=start;i<end;i++) {
        for (int j=0;j<emptyimg.cols;j++){  
            total_pixels++;
            int a = emptyimg.at<uchar>(i,j) - cropped_frame.at<uchar>(i,j);
            if (abs(a) > e_static){
                static_pixels++;
            }
        }
    }

    my_data->total_pixels = total_pixels;
    my_data->static_pixels = static_pixels;
}

int main(int argc, char** argv)
{

    if(argc < 5)
    {
        cout<<"Please specify empty Image file,Video file name, number of threads and frame skip value in the format : ./method1_static $(filename) $(Video Filename) $(num) $(Frame skip) or"<<endl;
        cout<<"To Compile and Execute Type Command : make -B method1_static empty=$(filename) video=$(Video Filename) num=$(num) x=$(Frame Skip)\nTo Compile Type Command : make -B method1_static_compile"<<endl;
        throw std::invalid_argument( "Wrong Command Line Argument");
        return -1;
    }

    if(argc > 5)
    {
        cout<<"Too Many Arguments. Please specify empty Image file,Video file name, number of threads and frame skip value in the format : ./method1_static $(filename) $(Video Filename) $(num) $(Frame skip) or"<<endl;
        cout<<"To Compile and Execute Type Command : make -B method1_static empty=$(filename) video=$(Video Filename) num=$(num) x=$(Frame Skip)\nTo Compile Type Command : make -B method1_static_compile"<<endl;
        throw std::invalid_argument( "Wrong Command Line Argument");
        return -1;
    }
    
    int num = stoi(argv[3]);
    int x = stoi(argv[4]);

    // Wrong Values
    if(x<=0 || num<=0)
    {
        cout<< "value of frame skip and number of threads has to be greater than 0"<<endl;
        throw std::invalid_argument( "Wrong Command Line Argument");
    }

    // Reading the Video
    VideoCapture cap(argv[2]); 
    
    // if not success, exit program
    if (cap.isOpened() == false)  
    {
        cout << "Cannot open the video file" << endl;
        return -1;
    }
    
    time_t method3_start,method3_end;

    // Creating Matrix for Empty(Background) Image according to points chosen by user
    Mat emptyimg;
    empty_image(argv[1], emptyimg);
    time(&method3_start);
    
    // Storing Previous Frames
    Mat cropped_frame_prev;

    // Counting Number of frames
    int l = 0;
    
    // File output
    std::ofstream myfile;
    myfile.open ("./csvfiles/method1_static.csv");
    myfile << "Time(in seconds),Queue Density,Dynamic Density,\n";
    
    //Iterating Frame by Frame
    while (true)
    {
        Mat frame;
        bool done = cap.read(frame); // read a new frame from video 
        //Breaking the while loop at the end of the video
        if (done == false) 
        {
            cout << "Found the end of the video" << endl;
            break;
        }    
        if(l%x==0)
        {
            pthread_t threads[num];
            struct thread_data input[num];
            int e_static = 35; // Error for estimating queue density (Found Experimentally)
            int total_pixels = 0; 
            int static_pixels = 0; // Counting Number of pixels changed in the current frame relative to backgroun image
            

            

            //Converting Video Frame to grayscale
            Mat temp_gray;
            cvtColor(frame, temp_gray, COLOR_BGR2GRAY);
            frame = temp_gray;
            Mat cropped_frame;

            // Applying Homography to the current frame
            homography_of_frames(frame,cropped_frame);

            // Dividing the matrix to each thread and setting up local space for each thread
            int start = 0;
            int end = emptyimg.rows/num;
            for(int i=0;i<num;i++){
                input[i].emptyimg = emptyimg;
                input[i].e_static = e_static;
                input[i].cropped_frame_prev = cropped_frame_prev;
                input[i].cropped_frame = cropped_frame;
                input[i].l = l;
                input[i].total_pixels = total_pixels;
                input[i].static_pixels = static_pixels;
                input[i].start = start;
                input[i].end = end;
                start = end;
                end += emptyimg.rows/num;
                if (i==num-2){
                    end = emptyimg.rows;
                }            
            }
            
            // Creating Threads
            for (int i = 0;i<num;i++){
                pthread_create(&threads[i], NULL, frame_iterate, (void *)&input[i]);
            }

            // Joining each thread to the main threads
            for (int i = 0;i<num;i++){

                pthread_join(threads[i], NULL);    
            }

            // Calculating pixel values
            for (int i = 0;i<num;i++){
                total_pixels+=input[i].total_pixels;
                static_pixels+=input[i].static_pixels;
            }

            // calculating queue and dynamic density
            float output_static = ((float)static_pixels)/((float)total_pixels);

            
            cout<<fixed<<setprecision(3);
            cropped_frame_prev = cropped_frame;

            // Outputting the Values on the terminal
            cout<<"Frame no: "<<l<<"    Queue density: "<<output_static<<endl;
            
            file_output.push_back(output_static);
        }

        // Updating States 
        l++;
    }

    time(&method3_end);
    double method3 = double(method3_end - method3_start);
    cout<<method3<<endl;

    vector<double> file_output_final;
    for(int i = 0 ; i < file_output.size();i++)
    {
        for(int j = i ; j < i+x ; j++)
        {
            file_output_final.push_back((double)file_output[i]);
        }
    }
    for(int i = 0 ; i < l;i++)
    {
       
        myfile<<i+1<<","<<file_output_final[i]<<","<<0<<",\n";
    }
    return 0;
    
}


