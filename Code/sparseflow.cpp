#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdexcept>
#include <fstream>

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

    if(argc < 3)
    {
        cout<<"Please specify empty Image file as well Video file name in the format : ./sparseflow $(filename) $(Video Filename) or"<<endl;
        cout<<"To Compile and Execute Type Command : make -B sparseflow empty=$(filename) video=$(Video Filename)\nTo Compile Type Command : make -B sparseflow_compile"<<endl;
        throw std::invalid_argument( "Wrong Command Line Argument");
        return -1;
    }

    if(argc > 3)
    {
        cout<<"Too Many Arguments. Enter only a Empty Image Filename and Video Filename"<<endl;
        cout<<"Please specify empty Image file as well Video file name in the format : ./sparseflow $(filename) $(Video Filename) or"<<endl;
        cout<<"To Compile and Execute Type Command : make -B sparseflow empty=$(filename) video=$(Video Filename)\nTo Compile Type Command : make -B sparseflow_compile"<<endl;
        throw std::invalid_argument( "Wrong Command Line Argument");
        return -1;
    }
    time_t bas,bae;
    // Reading the Video
    VideoCapture cap(argv[2]); 

    // if not success, exit program
    if (cap.isOpened() == false)  
    {
        cout << "Cannot open the video file" << endl;
        return -1;
    }

    // Creating Matrix for Empty(Background) Image according to points chosen by user
    Mat emptyimg;
    empty_image(argv[1], emptyimg);

    time(&bas);

    // Storing Previous Frames
    Mat cropped_frame_prev;

    // Counting Number of frames
    int l = 1;

    // Generating random color pixels
    vector<Scalar> colors;
    RNG rng;
    for(int i = 0; i < 100; i++)
    {
        int r = rng.uniform(0, 256);
        int g = rng.uniform(0, 256);
        int b = rng.uniform(0, 256);
        colors.push_back(Scalar(r,g,b));
    }

    // Setting up parameter values for good features to track function and sparse optical flow function
    Mat first,frame_prev,mask;
    
    vector<uchar> status;
    vector<float> err;
    TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
    vector<Point2f> corners;
    int maxCorners = 100,
        minDistance = 7,
        blockSize = 7;
    double qualityLevel = 0.3;
    
    // Reading first frame
    cap.read(first);
    cvtColor(first,frame_prev,COLOR_BGR2GRAY);
    homography_of_frames(frame_prev,cropped_frame_prev);
    
    mask = Mat::zeros(cropped_frame_prev.size(), first.type());
    //prevPts = corners;

    // File output
    std::ofstream myfile;
    myfile.open ("./csvfiles/sparseflow.csv");
    myfile << "Time(in seconds),Dynamic Density,\n";
    myfile<<1<<","<<0<<","<<0<<",\n";

    destroyAllWindows();
    
    //Iterating Frame by Frame
    while (true)
    {
        Mat frame;
        vector<Point2f> prevPts, nextPts;
        bool done = cap.read(frame); // read a new frame from video 

        int total_pixels = 0; 
        int dynamic_pixels = 0; // Counting Number of pixels changed in the last 3 frames
        int e_dynamic = 0;  // error for estimating dynamic density (Found Experimentally)

        //Breaking the while loop at the end of the video
        if (done == false) {
            cout << "Found the end of the video" << endl;
            break;
        }

        //Converting Video Frame to grayscale
        Mat temp_gray;
        cvtColor(frame, temp_gray, COLOR_BGR2GRAY);
        frame = temp_gray;
        Mat cropped_frame;

        // Applying Homography to the current frame
        homography_of_frames(frame,cropped_frame);
        
        // Detecting the corner points of a moving object
        goodFeaturesToTrack(cropped_frame_prev, corners, maxCorners, qualityLevel, minDistance, Mat(), blockSize,false,0.04);
    
        // Initializing the output matrix
        mask = Mat::zeros(cropped_frame_prev.size(), first.type());
        prevPts = corners;

        // Detecting the movement of corner points
        calcOpticalFlowPyrLK(cropped_frame_prev, cropped_frame, prevPts, nextPts, status, err, Size(15,15), 2, criteria);
        vector<Point2f> good_new;

        for(uint i = 0; i < prevPts.size(); i++)
        {
            // Select good points
            if(status[i] == 1) {
                good_new.push_back(nextPts[i]);
                // Draw the tracks
                line(mask,nextPts[i], prevPts[i], colors[i], 2);
            }
        }

        // Converting the mask to grayscasle
        Mat a1,img;
        a1 = mask.clone();
        cvtColor(a1,img,COLOR_BGR2GRAY);

        // Calculating dynamic pixels
        for(int i=0;i<emptyimg.rows;i++) {
            for (int j=0;j<emptyimg.cols;j++){  
                total_pixels++;
                int a = img.at<uchar>(i,j);
                if (abs(a) > e_dynamic){
                    dynamic_pixels++;
                }
            }
        }

        // Calculating dynamic density
        float dynamic_density = 50*((float)dynamic_pixels)/((float)total_pixels);

        // Updating States
        l++;
        prevPts = good_new;
        cropped_frame_prev = cropped_frame.clone();
        cout<<"Frame no: "<<l<<"    sparse density: "<<dynamic_density<<endl;
        myfile<<l<<","<<0<<","<<dynamic_density<<",\n";

    }
    
    time(&bae);
    cout<<double(bae-bas)<<endl; 

    return 0;
    
}
