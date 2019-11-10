/*
Copyright 2019 HackRats

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencvimagereader.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	// Read the image file
	/*Mat image = imread("./hand.jpg");
	
	// Check for failure
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}*/
	
	VideoCapture cap;
	cap.open(1, CAP_ANY);
	
	if(!cap.isOpened())
	{
		cerr << "Failed to open video stream" << endl;
		return -1;
	}
	
	String windowName = "CAMRAT Hand Detection Output"; //Name of the window
	namedWindow(windowName); // Create a window
	//imshow(windowName, image); // Show our image inside the created window.
	
	Mat image;
	while(1)
	{
		cap.read(image);
		
		if(image.empty())
		{
			cerr << "Failed to read image" << endl;
			break;
		}
		
		if(waitKey(5))
		{
			break;
		}
		
		imshow(windowName, image);
	}
	
	//waitKey(0); // Wait for any keystroke in the window
	destroyWindow(windowName); //destroy the created window
	return 0;
}