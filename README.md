## Synopsis

This will be a simple that integrates OpenCV and Librealsense in Visual Studio 2013. The Sample Code will allow to display and save the acquired images. 

When the "s" key is pressed the program saves the RGB, IR, Depth and the PointCloud. The point cloud is saved in a text file. 

The Escape Key ends the program.

Be sure to edit the save path in main.cpp to edit the directory where the frames are saved.

## Installation

Assuming that you have already installed OpenCV and librealsense, you should create a new project in VS2013. 

So the first thing to do after creating the project is to define the target machine (x64 in my case) in the configuration manager. 
On the Project properties->Linker->Advanced confirm that you have target machine as MachineX64. 


I have my instalation folder of librealsense in C:\librealsense\ and the opencv 3.1 in C:\opencv\opencv (you should adapt the following paths to your paths):
Project Properties -> C/C++ -> General -> Additional Include Directories: C:\librealsense\include; C:\librealsense\examples; C:\opencv\opencv\build\include; C:\opencv\opencv\build\include\opencv
Linker -> General -> Additional Library Directories: C:\librealsense\bin\x64; C:\opencv\opencv\build\x64\vc12\lib

You can also do this with property sheets (available in the repository -> Be sure you adapth those to your openCV and librealsense directories. The property sheet provided is for OPENCV 3.1 with all the extra_modules available in debug mode. Take that into account and if you use it adapth to your lib files)

Assuming that you have already build librealsense projects:

Linker -> Input -> Additional Dependencies: realsense-d.lib; opencv_ts300d.lib; opencv_world300d.lib (this libs are for OpenCV 3.0 In the descriptions there are property sheets for openCV 3.1 with the extra_modules, be sure to adapt them)

Problems you can find:

- "The Application was unable to start correctly (0xc000007b)". This was only resolved after going to Configuration Properties -> Debugging -> Working Directory: C:\opencv\opencv\build\x64\vc12\bin. From what I read, it this error ir resolved by changind the working directory to a x64 folder (in my case it is x64 machine).
- An error message saying that you are missing realsense-d.dll. Go to your environment variables and add C:\librealsense\bin\x64 to your PATH variable (adapt to your folder installation). 



