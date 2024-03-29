#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdexcept>
#include <fstream>
#define pb push_back
using namespace std;
using namespace cv;
vector<bool> is_homo;
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

int main(int argc, char** argv)
{

    if(argc < 4)
    {
        cout<<"Please specify empty Image file,Video file name and frame skip value in the format : ./method1 $(filename) $(Video Filename) $(Frame skip) or"<<endl;
        cout<<"To Compile and Execute Type Command : make -B method1 empty=$(filename) video=$(Video Filename) x=$(Frame Skip)\nTo Compile Type Command : make -B method1_compile"<<endl;
        throw std::invalid_argument( "Wrong Command Line Argument");
        return -1;
    }

    if(argc > 4)
    {
        cout<<"Too Many Arguments. Enter 4 arguments :  Empty Image Filename,Video Filename and Frame skip value"<<endl;
        cout<<"To Execute Type Command : ./method1 $(filename) $(Video Filename) $(Frame skip) or"<<endl;
        cout<<"To Compile and Execute Type Command : make -B method1 empty=$(filename) video=$(Video Filename) x=$(Frame Skip)\nTo Compile Type Command : make -B method1_compile"<<endl;
        throw std::invalid_argument( "Wrong Command Line Argument");
        return -1;
    }

    is_homo.reserve(10000);
    int x = stoi(argv[3]);

    if(x<=0)
    {
        cout<< "value of frame skip has to be greater than 0"<<endl;
        throw std::invalid_argument( "Wrong Command Line Argument");
    }

    // for file output
    vector<float> method1_queue,method1_dynamic,method1_queue_final,method1_dynamic_final;
    time_t method1_start,method1_end;

    // Creating Matrix for Empty(Background) Image according to points chosen by user
    Mat emptyimg;
    empty_image(argv[1], emptyimg);

    time(&method1_start);

    // Reading the Video
    VideoCapture cap(argv[2]); 

    // if not success, exit program
    if (cap.isOpened() == false)  
    {
        cout << "Cannot open the video file" << endl;
        return -1;
    }


    // Storing Previous Frames
    Mat cropped_frame_prev,cropped_frame_2ndlast,cropped_frame_3rdlast;

    // Counting Number of frames
    int l = 3;
    
    // Reading first 3 frames
    Mat first,frame_prev;
    cap.read(first);
    cvtColor(first,frame_prev,COLOR_BGR2GRAY);
    homography_of_frames(frame_prev,cropped_frame_3rdlast);
    cap.read(first);
    cvtColor(first,frame_prev,COLOR_BGR2GRAY);
    homography_of_frames(frame_prev,cropped_frame_2ndlast);
    cap.read(first);
    cvtColor(first,frame_prev,COLOR_BGR2GRAY);
    homography_of_frames(frame_prev,cropped_frame_prev);

    //is_homo matrix implies if homography is done for the previous frames. 1 means done 0 means not done
    is_homo.pb(1);
    is_homo.pb(1);
    is_homo.pb(1);

    // file output
    std::ofstream myfile;
    myfile.open ("./csvfiles/method1.csv");
    myfile << "Time(in seconds),Queue Density,Dynamic Density,\n";
    cout<<"Frame no: "<<1<<"    Queue density: "<<0<<"    dynamic density: "<<0<<endl;
    cout<<"Frame no: "<<2<<"    Queue density: "<<0<<"    dynamic density: "<<0<<endl;
    cout<<"Frame no: "<<3<<"    Queue density: "<<0<<"    dynamic density: "<<0<<endl;

    method1_queue.push_back(0);
    method1_dynamic.push_back(0);
    
    //Iterating Frame by Frame
    while (true)
    {
        l++;
        bool is_homo_done = false;
        Mat frame;
        bool done = cap.read(frame); // read a new frame from video 

        int total_pixels = 0; 
        int static_pixels = 0; // Counting Number of pixels changed in the current frame relative to backgroun image
        int dynamic_pixels = 0; // Counting Number of pixels changed in the last 3 frames
        int e_static = 35; // Error for estimating queue density (Found Experimentally)
        int e_dynamic = 0;  // error for estimating dynamic density (Found Experimentally)

        //Breaking the while loop at the end of the video
        if (done == false) 
        {
            cout << "Found the end of the video" << endl;
            break;
        }

        //Converting Video Frame to grayscale
        Mat temp_gray;
        cvtColor(frame, temp_gray, COLOR_BGR2GRAY);
        frame = temp_gray;
        Mat cropped_frame;

        // Applying Homography to the current frame
          
        if (l%x==0){
            // Estimating Pixels changed in static and dynamic matrix
            is_homo_done=1;
            // Checking if homography is to be done for the previous frames( we require last 3 frames for calculating dynamic density)
            homography_of_frames(frame,cropped_frame);
            for(int tteemmpp = is_homo.size()-1;tteemmpp>=0;tteemmpp--)
            {
                int jjj = is_homo.size()-tteemmpp;
                if(is_homo[tteemmpp]==0)
                {
                    if(jjj==1)
                    {
                        Mat temp_frame;
                        homography_of_frames(cropped_frame_prev,temp_frame);
                        cropped_frame_prev=temp_frame;
                        is_homo[tteemmpp]=1;
            
                    }
                    else if(jjj==2)
                    {
                        Mat temp_frame;
                        homography_of_frames(cropped_frame_2ndlast,temp_frame);
                        cropped_frame_2ndlast=temp_frame; 
                        is_homo[tteemmpp]=1;  
                    }
                    else if(jjj==3)
                    {
                        Mat temp_frame;
                        homography_of_frames(cropped_frame_3rdlast,temp_frame);
                        cropped_frame_3rdlast=temp_frame;   
                        is_homo[tteemmpp]=1;
                    }
                    else
                    {
                        break;
                    }
                }
            }

            is_homo.pb(1);

            // counting pixels to calculate dynamic and queue density
            for(int i=0;i<emptyimg.rows;i++) {
                for (int j=0;j<emptyimg.cols;j++){  
                    total_pixels++;
                    int a = emptyimg.at<uchar>(i,j) - cropped_frame.at<uchar>(i,j);
                    if (abs(a) > e_static){
                        static_pixels++;
                    }
                        
                    if (l>2){
                        int b = cropped_frame_prev.at<uchar>(i,j) - cropped_frame.at<uchar>(i,j);
                        int c = cropped_frame_2ndlast.at<uchar>(i,j) - cropped_frame_prev.at<uchar>(i,j);
                        int d = cropped_frame_3rdlast.at<uchar>(i,j) -cropped_frame_2ndlast.at<uchar>(i,j);
                        if (abs(b) > e_dynamic and abs(c) > e_dynamic and abs(d) > e_dynamic ){
                            dynamic_pixels++;
                        }
                    }
                }
            }

            // calculating queue and dynamic density
            float output_static = ((float)static_pixels)/((float)total_pixels);
            float output_dynamic = ((float)dynamic_pixels)/((float)total_pixels);

            cout<<fixed<<setprecision(3);

            // Improving Queue Density
            while(output_dynamic>=output_static or (output_static-output_dynamic<0.001 and output_dynamic<=output_static))
            {
                e_static/=1.15;
                if(e_static<1.5)
                {
                    output_static=output_dynamic+0.05;
                    break;
                }
                static_pixels=0;
                total_pixels=0;
                for(int i=0;i<emptyimg.rows;i++) {
                    for (int j=0;j<emptyimg.cols;j++){  
                        total_pixels++;
                        int a = emptyimg.at<uchar>(i,j) - cropped_frame.at<uchar>(i,j);
                        if (abs(a) > e_static){
                            static_pixels++;
                        }
                    }
                }
                output_static = ((float)static_pixels)/((float)total_pixels);
            }

            // Outputting the Values on the terminal
            cout<<"Frame no: "<<l<<"    Queue density: "<<output_static<<"    dynamic density: "<<output_dynamic<<endl;

            method1_queue.push_back(output_static);
            method1_dynamic.push_back(output_dynamic);
        }

        if(is_homo_done==0)
        {
            is_homo.pb(0);
        }
        // Updating States 
        
        cropped_frame_3rdlast = cropped_frame_2ndlast;
        cropped_frame_2ndlast = cropped_frame_prev;
    
        if(is_homo_done)
            cropped_frame_prev = cropped_frame;
        else
            cropped_frame_prev=frame;
    }
    time(&method1_end);
    double method1 = double(method1_end - method1_start);
    cout<<method1<<endl;

    // Outputting the values in file
    int atleast = max(x,3);
    for(int i = 0; i < atleast ; i++)
    {
        method1_dynamic_final.pb(0);
        method1_queue_final.pb(0);
    }
    if(x==3)
    {
        method1_dynamic_final.pb(0);
        method1_queue_final.pb(0);
    }
    for(int i = 1 ; i < method1_dynamic.size();i++)
    {
        for(int j = i ; j < i+x ; j++)
        {
            method1_dynamic_final.push_back(method1_dynamic[i]);            
        }
    }
    for(int i = 1 ; i < method1_queue.size();i++)
    {
        for(int j = i ; j < i+x ; j++)
        {
            method1_queue_final.push_back(method1_queue[i]);
               
        }
    }
    for(int i = 1 ; i < l;i++)
    {
        myfile<<i<<","<<method1_queue_final[i-1]<<","<<method1_dynamic_final[i-1]<<",\n";
            
    }

    return 0;
}
