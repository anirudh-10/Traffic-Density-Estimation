# COP290-Assignment1

### Queue Density and Dynamic Density Estimation and Optimization

The code takes a background image and a video file and outputs the estimated traffic queue and traffic dynamic density at various instances.

### Pre-requisites:
1) C++ Compiler(Preferably the latest version) (If not refer https://www.tutorialspoint.com/cplusplus/cpp_environment_setup.htm)
2) OpenCV
3) python3

To install OpenCV refer:

i. ubuntu: http://techawarey.com/programming/install-opencv-c-c-in-ubuntu-18-04-lts-step-by-step-guide/

ii. windows: https://cv-tricks.com/how-to/installation-of-opencv-4-1-0-in-windows-10-from-source/

iii. MacOS: https://docs.opencv.org/master/d0/db2/tutorial_macos_install.html

### Executing the Code

1) Extract the .tar file in your local machine

2) On Terminal Run the Command (On Linux OS):

i. Compiling all files :$ make compile

#### For Frame Skip Method
	To compile:$ make method1_compile
	To execute and compile:$ make -B method1 empty=$(empty image file) video=$(video file) x=$(frame skip)
	To execute:$ ./method1 $(empty) $(video) $(x)
#### For Reducing the Resolution Method
	To compile:$ make method2_compile
	To execute and compile:$ make -B method2 empty=$(empty image file) video=$(video file) x=$(X) y=$(Y)
	To execute:$ ./method2 $(empty) $(video) $(x) $(y)
#### Multithreading based on dividing the frame into multiple threads method
	To compile:$ make method3_compile
	To execute and compile:$ make -B method3 empty=$(empty image file) video=$(video file) num=$(num)
	To execute:$ ./method3 $(empty) $(video) $(num)
#### Multithreading based on executing different frames on different thread method
	To compile:$ make method4_compile
	To execute and compile:$ make -B method4 empty=$(empty image file) video=$(video file) num=$(num)
	To execute:$ ./method4 $(empty) $(video) $(num)	
#### For Dense Optical method
	To compile:$ make denserflow_compile
	To execute and compile:$ make -B denserflow empty=$(empty image file) video=$(video file)
	To execute:$ ./denserflow $(empty) $(video)
#### For Sparse optical Flow method
	To compile:$ make sparseflow_compile
	To execute and compile:$ make -B sparseflow empty=$(empty image file) video=$(video file)
	To execute:$ ./sparseflow $(empty) $(video)
#### For static density only method
	To compile:$ make sub2queue_compile
	To execute and compile:$ make -B sub2queue empty=$(empty image file) video=$(video file)
	To execute:$ ./sub2queue $(empty) $(video)
#### For frame skip and multithreading method
	To compile:$ make method1_static_compile
	To execute and compile:$ make -B method1_static empty=$(empty file) video=$(video file) num=$(num)x=$(x) 
	To execute:$ ./method1_static $(empty) $(video) $(num) $(x)
#### For reducing the resolution and multithreading method
	To compile:$ make method2_static_compile
	To execute and compile:$ make -B method2_static empty=$(emptyfile) video=$(video) num=$(num) x=$(X) y=$(Y) 
	To execute:$ ./method2_static $(empty) $(video) $(num) $(x) $(y)
#### subtask2:
	To compile:$ make subtask2_compile
	To execute and compile:$ make -B subtask2 empty=$(empty image file) video=$(video file)
	To execute:$ ./subtask2 $(empty) $(video)
#### subtask1:
	To compile:$ make subtask1_compile
	To execute and compile:$ make -B subtask1 empty=$(empty image file)
	To execute:$ ./subtask1 $(empty)
#### Utility:
	To compile:$ make utility_compile
	To execute and compile: make -B utility file1=$(file) file2=$(file)
	To execute:$ ./utility $(file1) $(file2)
#### Plot Graph:
	To execute:$ python3 plotgraph.py
	After running the above command the terminal will ask for following inputs:
	Give filename: $(filename)
	x-axis: $(Column number in the csv file to be shown as x-axis)
	y-axis: $(Column number in the csv file to be shown as y-axis)	

where filename is the name of the background image file with extension and Video Filename is the name of the video with extension and num referes to number of threads  and X,Y refers to the new dimensions of the image

