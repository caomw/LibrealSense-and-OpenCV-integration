## Synopsis

This will be a project explaining how to integrate the OpenCV and Librealsense in Visual Studio 2013. The Sample Code will allow to display and save the acquired images in a openCV window and store them in a Mat file. 

The depth data has 16 bytes and it is converted to 8bits to visualization purposes.

## Installation

Assuming that you have already installed OpenCV and librealsense, you should create a new project in VS2013. 

So the first thing to do after creating the project is to define the target machine (x64 in my case) in the configuration manager. 
On the Project properties->Linker->Advanced confirm that you have target machine as MachineX64. 


I have my instalation folder of librealsense in C:\librealsense\ and the opencv 3.0 in C:\opencv\opencv (you should adapt the following paths to your paths):
Project Properties -> C/C++ -> General -> Additional Include Directories: C:\librealsense\include; C:\librealsense\examples; C:\opencv\opencv\build\include; C:\opencv\opencv\build\include\opencv
Linker -> General -> Additional Library Directories: C:\librealsense\bin\x64; C:\opencv\opencv\build\x64\vc12\lib

You can also do this with property sheets. 

Assuming that you have already build librealsense projects:

Linker -> Input -> Additional Dependencies: realsense-d.lib; opencv_ts300d.lib; opencv_world300d.lib

Problems you can find:

- "The Application was unable to start correctly (0xc000007b)". This was only resolved after going to Configuration Properties -> Debugging -> Working Directory: C:\opencv\opencv\build\x64\vc12\bin. From what I read, it this error ir resolved by changind the working directory to a x64 folder (in my case it is x64 machine).
- An error message saying that you are missing realsense-d.dll. Go to your environment variables and add C:\librealsense\bin\x64 to your PATH variable (adapt to your folder installation). 



## Future Developments

In the next few days I'll include PointCloudSave in a Mat file and in a txt file in the source code.


