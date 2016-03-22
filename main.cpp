// First include the librealsense C++ header file

#define _CRT_SECURE_NO_WARNINGS
#include <librealsense/rs.hpp>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cstdio>
#include <stdint.h>
#include <vector>
#include <limits>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third_party/stb_image_write.h"

using namespace std;
using namespace cv;

int main()
{

	cout << "OpenCV version being used: " << CV_VERSION << endl << endl;


	//Create a context object. This object owns the handles to all connected realsense devices.
	rs::context ctx;
	printf("There are %d connected RealSense devices.\n", ctx.get_device_count());
	if (ctx.get_device_count() == 0) return EXIT_FAILURE;

	// This tutorial will access only a single device, but it is trivial to extend to multiple devices
	rs::device * dev = ctx.get_device(0);
	printf("\nUsing device 0, an %s\n", dev->get_name());
	printf("    Serial number: %s\n", dev->get_serial());
	printf("    Firmware version: %s\n", dev->get_firmware_version());
	model_r200 = false;

	// Configure all streams to run at VGA resolution at 60 frames per second. Capture IR image 2 if needed.
	dev->enable_stream(rs::stream::depth, 640, 480, rs::format::z16, 60);
	dev->enable_stream(rs::stream::color, 640, 480, rs::format::rgb8, 60);
	dev->enable_stream(rs::stream::infrared, 640, 480, rs::format::y8, 60);
	try { dev->enable_stream(rs::stream::infrared2, 640, 480, rs::format::y8, 60);
	model_r200 = true; }
	catch (...) { printf("Device does not provide infrared2 stream.\n"); }

	const int width = 640;
	const int height = 480;

	dev->start();

	// Capture 50 frames to give autoexposure, etc. a chance to settle
	for (int i = 0; i < 50; ++i) dev->wait_for_frames();


	bool continue_acquisition = true;
	namedWindow("RGB window", WINDOW_FULLSCREEN); // Window for display.

	while (continue_acquisition)
	{
		dev->wait_for_frames();
		Mat rgb(height, width, CV_8UC3, (uchar *) dev->get_frame_data(rs::stream::color));
		// Remember that in openCV it is assumed that the pixel are stored in BGR and not in RGB. So we need to change that.
		cvtColor(rgb, rgb, CV_BGR2RGB);
		imshow("RGB Window", rgb);
		Mat depth16(height, width, CV_16U, (uchar *) dev->get_frame_data(rs::stream::depth));
		// Depth comes in 16 bits so we need to convert it to 8bits
		depth16.convertTo(depth16, CV_8UC1);
		imshow("Depth Image", depth16);
		Mat ir1(height, width, CV_8UC1, (uchar *) dev->get_frame_data(rs::stream::infrared));
		imshow("IR Image 1", ir1);
		// If we use R200 model we also plot the second IR image.
		if (model_r200){
			Mat ir2(height, width, CV_8UC1, (uchar *) dev->get_frame_data(rs::stream::infrared2));
			imshow()"IR Image 2", ir2);
		}

		int key = waitKey(1);

		// If the Esc key is detected the acquisition is stopped.
		if (key == 27){
			continue_acquisition = false;
		}
	}

	cout << endl << "Program ended due to user interference." << endl << endl;
	system("PAUSE");

	return 0;
}
