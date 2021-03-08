# COP290-Assignment1

### Queue Density and Dynamic Density Estimation

The code takes a background image and a video file and outputs the estimated traffic queue and traffic dynamic density at various instances.

### Pre-requisites:
1) C++ Compiler(Preferably the latest version) (If not refer https://www.tutorialspoint.com/cplusplus/cpp_environment_setup.htm)
2) OpenCV

To install OpenCV refer:

i. ubuntu: http://techawarey.com/programming/install-opencv-c-c-in-ubuntu-18-04-lts-step-by-step-guide/

ii. windows: https://cv-tricks.com/how-to/installation-of-opencv-4-1-0-in-windows-10-from-source/

iii. MacOS: https://docs.opencv.org/master/d0/db2/tutorial_macos_install.html

### Executing the Code

1) Extract the .tar file in your local machine

2) On Terminal Run the Command (On Linux OS):

i. Compiling and Executing :$ make all empty=$(filename) video=$(Video Filename) 

where filename is the name of the background image file with extension and Video Filename is the name of the video with extension

If having trouble in previous step run the following 2 commands in order:

1) For Compiling :$ make compile

2) For Executing :$ make run empty=$(filename) video=$(Video Filename) 

				or
2) For Executing :$ ./a.out $(filename) $(Video Filename)

Where filename and Video Filename are same as before
