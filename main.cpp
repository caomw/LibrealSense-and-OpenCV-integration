#define _CRT_SECURE_NO_WARNINGS
#include <librealsense/rs.hpp>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <cstdio>
#include <stdint.h>
#include <vector>
#include <limits>
#include <iostream>
#include <fstream>


using namespace std;
using namespace cv;




int main() try
{
	// Turn on logging. We can separately enable logging to console or to file, and use different severity filters for each.
	rs::log_to_console(rs::log_severity::warn);
	//rs::log_to_file(rs::log_severity::debug, "librealsense.log");

	// Create a context object. This object owns the handles to all connected realsense devices.
	rs::context ctx;
	printf("There are %d connected RealSense devices.\n", ctx.get_device_count());
	if (ctx.get_device_count() == 0) return EXIT_FAILURE;

	// This tutorial will access only a single device, but it is trivial to extend to multiple devices
	rs::device * dev = ctx.get_device(0);
	printf("\nUsing device 0, an %s\n", dev->get_name());
	printf("    Serial number: %s\n", dev->get_serial());
	printf("    Firmware version: %s\n", dev->get_firmware_version());

	cout << endl << endl;

	// Configure depth and color to run with the device's preferred settings
	dev->enable_stream(rs::stream::depth, 640, 480, rs::format::z16, 60);
	dev->enable_stream(rs::stream::color, 640, 480, rs::format::rgb8, 60);
	dev->enable_stream(rs::stream::infrared, 640, 480, rs::format::y8, 60);
	
	// Infrared 2 will only work with R200 model.
	bool r200_model = false;
	try{
		dev->enable_stream(rs::stream::infrared2, 640, 480, rs::format::y8, 60);
	r200_model = true;
	}
	catch (...) {
		printf("Camera model is not R200, second IR image not enabled.\n\n");
	}

	printf("Device is warming up... \n\n");
	// Start the device
	dev->start();
	// Capture first 500 frames to allow camera to stabilize
	for (int i = 0; i < 50; ++i) dev->wait_for_frames();

	bool acquire = true;

	while (acquire)
	{

		// Wait for new frame data
		dev->wait_for_frames();

		Mat rgb(480, 640, CV_8UC3, (uchar *) dev->get_frame_data(rs::stream::color));
		cvtColor(rgb, rgb, CV_BGR2RGB);

		uchar *depth_image = (uchar *) dev->get_frame_data(rs::stream::depth);
		// Saved for PCloud save purposes
		const uint16_t * depth_image_pointCloud = (const uint16_t *) depth_image;
		Mat depth16(480, 640, CV_16U, depth_image);

		Mat ir1(480, 640, CV_8UC1, (uchar *) dev->get_frame_data(rs::stream::infrared));
		Mat ir2;
		if (r200_model){
			Mat ir2(480, 640, CV_8UC1, (uchar *) dev->get_frame_data(rs::stream::infrared2));
		}


		// Retrieve camera parameters for mapping between depth and color
		rs::intrinsics depth_intrin = dev->get_stream_intrinsics(rs::stream::depth);
		rs::extrinsics depth_to_color = dev->get_extrinsics(rs::stream::depth, rs::stream::color);
		rs::intrinsics color_intrin = dev->get_stream_intrinsics(rs::stream::color);
		// Get scale depth parameter
		float scale = dev->get_depth_scale();

		imshow("RGB", rgb);

		int key = waitKey(1);
		

		// If 'S' key is pressed, save the frames
		if (key == 115){
			ofstream myfile;
			myfile.open("PCloud.txt");
			
			for (int dy = 0; dy<depth_intrin.height; ++dy)
			{
				for (int dx = 0; dx<depth_intrin.width; ++dx)
				{
					// Retrieve the 16-bit depth value and map it into a depth in meters
					uint16_t depth_value = depth_image_pointCloud[dy * depth_intrin.width + dx];
					float depth_in_meters = depth_value * scale;

					// Skip over pixels with a depth value of zero, which is used to indicate no data
					if (depth_value == 0) continue;

					// Map from pixel coordinates in the depth image to pixel coordinates in the color image
					rs::float2 depth_pixel = { (float) dx, (float) dy };
					rs::float3 depth_point = depth_intrin.deproject(depth_pixel, depth_in_meters);
					rs::float3 color_point = depth_to_color.transform(depth_point);
					rs::float2 color_pixel = color_intrin.project(color_point);

					myfile << depth_point.x << '\t' << depth_point.y << '\t' << depth_point.z << endl;

				}
			}

			imwrite("rgb.png", rgb);
			imwrite("depth.png", depth16);
			imwrite("ir.png", ir1);
			if (r200_model){
				imwrite("ir2.png", ir2);
			}

			printf("Frames and Point Cloud saved in project directory. \n\n");
		}
		else if (key == 27){ // If 'Esc' key is pressed, end program
			acquire = false;
			printf("Program ended due to user interferance. \n\n");
		}

		
	}

	return 0;
}
catch (const rs::error & e)
{
	// Method calls against librealsense objects may throw exceptions of type rs::error
	printf("rs::error was thrown when calling %s(%s):\n", e.get_failed_function().c_str(), e.get_failed_args().c_str());
	printf("    %s\n", e.what());
	return EXIT_FAILURE;
}
