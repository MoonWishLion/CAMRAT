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

//#include <opencvimagereader.h>

using namespace cv;
using namespace std;

#define SHARED_LIB_SOURCE

#include "main.hpp"
#include <windows.h>
#include <iostream>

extern "C" BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            std::cout << "DLL loaded" << std::endl;
            break;

        case DLL_PROCESS_DETACH:
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }
	
    return true;
}

bool shouldRun = true;

float yLower = -1, yHigher = 1, xLower = -1, xHigher = 1;
float posLR = 0;
float posFB = 0;

void convexHulls(vector<vector<Point> > &inputContours, vector<vector<Point> > &outputContours) {
	vector<vector<Point> > hull (inputContours.size());
	outputContours.clear();
	for(size_t i = 0; i < inputContours.size(); i++)
	{
		convexHull(Mat((inputContours)[i]), hull[i], false);
	}
	outputContours = hull;
}

void filterContours(vector<vector<Point> > &inputContours, double minArea, double minPerimeter, double minWidth, double maxWidth, double minHeight, double maxHeight, double minSolidity, double maxSolidity, double maxVertexCount, double minVertexCount, double minRatio, double maxRatio, vector<vector<Point> > &output) {
	vector<Point> hull;
	output.clear();
	for(vector<Point> contour: inputContours)
	{
		Rect bb = boundingRect(contour);
		if(bb.width < minWidth || bb.width > maxWidth) continue;
		if(bb.height < minHeight || bb.height > maxHeight) continue;
		double area = contourArea(contour);
		if(area < minArea) continue;
		if(arcLength(contour, true) < minPerimeter) continue;
		convexHull(Mat(contour, true), hull);
		double solid = 100 * area / contourArea(hull);
		if(solid < minSolidity || solid > maxSolidity) continue;
		if(contour.size() < minVertexCount || contour.size() > maxVertexCount)	continue;
		double ratio = (double) bb.width / (double) bb.height;
		if(ratio < minRatio || ratio > maxRatio) continue;
		output.push_back(contour);
	}
}

void findContours(Mat &input, bool externalOnly, vector<vector<Point> > &contours)
{
	vector<Vec4i> hierarchy;
	contours.clear();
	int mode = externalOnly ? RETR_EXTERNAL : RETR_LIST;
	findContours(input, contours, hierarchy, mode, CHAIN_APPROX_SIMPLE);
}

void mask(Mat &input, Mat &mask, Mat &output)
{
	mask.convertTo(mask, CV_8UC1);
	bitwise_xor(output, output, output);
	input.copyTo(output, mask);
}

void thresholdMoving(Mat &input, Mat &lastImage, Mat &output)
{
	Size lastSize = lastImage.size();
	Size inputSize = input.size();
	if(!lastImage.empty() && lastSize.height == inputSize.height && lastSize.width == inputSize.width)
	{
		absdiff(input, lastImage, output);
	}
	input.copyTo(lastImage);
}

void drawAllContours(Mat &img, vector<vector<cv::Point> > &contours, Scalar color)
{
	//bitwise_xor(img, img, img);
	
	for(int i = 0; i < contours.size(); i++)
	{
		drawContours(img, contours, i, color);
	}
}

extern "C" PUBLIC_FUNCTION void calibrate()
{
	if(edgeContours.size() >= 2)
	{
		Moments m1 = moments(edgeContours[0]);
		Moments m2 = moments(edgeContours[1]);
		
		float x1 = m1.m10 / m1.m00;
		float y1 = m1.m01 / m1.m00;
		float x2 = m2.m10 / m2.m00;
		float y2 = m2.m01 / m2.m00;
		
		if(x1 < x2)
		{
			xLower = x1;
			xHigher = x2;
		}
		else
		{
			xLower = x2;
			xHigher = x1;
		}
		
		if(y1 < y2)
		{
			yLower = y1;
			yHigher = y2;
		}
		else
		{
			yLower = y2;
			yHigher = y1;
		}
	}
}

extern "C" PUBLIC_FUNCTION float getPosFB()
{
	return posFB;
}

extern "C" PUBLIC_FUNCTION float getPosLR()
{
	return posLR;
}

extern "C" PUBLIC_FUNCTION void stopThread()
{
	shouldRun = false;
}

extern "C" PUBLIC_FUNCTION void startThread(bool showWindow)
{
	pthread_create("CamInputRun", NULL, &runLoop, showWindow); 
}

//int main(int argc, char** argv)
extern "C" PUBLIC_FUNCTION void runLoop(bool showWindow)
{
	//OpenCVImageReader reader = OpenCVImageReader();
	
	VideoCapture cap;
	cap.open(1, CAP_ANY);
	
	if(!cap.isOpened())
	{
		cerr << "Failed to open video stream" << endl;
		return -1;
	}
	
	cv::String windowName = "CAMRAT Hand Detection Output";
	if(showWindow)
	{
		namedWindow(windowName);
	}
	
	Point anchor = Point(-1, -1);
	Mat erodeDilateKernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	Mat closeKernel = getStructuringElement(MORPH_ELLIPSE, Size(50, 50));
	
	// For both parts
	Mat image;
	Mat drawnContours;
	
	// For the glove tracker
	Mat hsvImage;
	Mat hsvThresh;
	Mat eroded;
	Mat dilated;
	Mat prevDilated;
	Mat threshMoving;
	Mat threshStill;
	Mat stillMask;
	vector<vector<Point> > contours;
	vector<vector<Point> > gloveContours;
	
	// For the box edge tracker
	Mat hsvImage2;
	Mat hsvThresh2;
	Mat eroded2;
	Mat prevEroded2;
	Mat threshMoving2;
	Mat threshStill2;
	Mat stillMask2;
	Mat dilated2;
	Mat prevDilated2;
	Mat threshMoving3;
	Mat threshStill3;
	Mat stillMask3;
	Mat eroded3;
	vector<vector<Point> > contours2;
	vector<vector<Point> > contoursFiltered;
	vector<vector<Point> > edgeContours;
	
	while(shouldRun)
	{
		cap.read(image);
		
		if(image.empty())
		{
			cerr << "Failed to read image" << endl;
			break;
		}
		
		cvtColor(image, hsvImage, COLOR_BGR2HSV);
		hsvImage2 = hsvImage.clone();
		
		/////////// GLOVE TRACKING //////////
		inRange(hsvImage, Scalar(138, 39, 187), Scalar(180, 98, 255), hsvThresh);
		erode(hsvThresh, eroded, erodeDilateKernel, anchor, 4);
		dilate(eroded, dilated, erodeDilateKernel, anchor, 11);
		
		if(prevDilated.empty())
		{
			prevDilated = dilated.clone();
			continue;
		}
		
		thresholdMoving(dilated, prevDilated, threshMoving);
		prevDilated = dilated.clone();
		bitwise_not(threshMoving, threshStill);
		mask(dilated, threshStill, stillMask);
		findContours(stillMask, true, contours);
		filterContours(contours, 100, 200, 100, 1000, 100, 1000, 0, 100, 1000000, 0, 0, 1000, gloveContours);
		////////// GLOVE TRACKING //////////
		
		////////// WALL TRACKING //////////
		inRange(hsvImage2, Scalar(0, 0, 92), Scalar(29, 83, 144), hsvThresh2);
		erode(hsvThresh2, eroded2, erodeDilateKernel, anchor, 6);
		
		if(prevEroded2.empty())
		{
			prevEroded2 = eroded2.clone();
			continue;
		}
		
		thresholdMoving(eroded2, prevEroded2, threshMoving2);
		prevEroded2 = eroded2.clone();
		bitwise_not(threshMoving2, threshStill2);
		mask(eroded2, threshStill2, stillMask2);
		dilate(stillMask2, dilated2, erodeDilateKernel, anchor, 10);
		
		if(prevDilated2.empty())
		{
			prevDilated2 = dilated2.clone();
			continue;
		}
		
		thresholdMoving(dilated2, prevDilated2, threshMoving3);
		prevDilated2 = dilated2.clone();
		bitwise_not(threshMoving3, threshStill3);
		mask(dilated2, threshStill3, stillMask3);
		erode(stillMask3, eroded3, erodeDilateKernel, anchor, 4);
		findContours(eroded3, true, contours2);
		filterContours(contours2, 100, 100, 50, 1000, 50, 1000, 81, 100, 1000000, 0, 0, 1000, contoursFiltered);
		convexHulls(contoursFiltered, edgeContours);
		////////// WALL TRACKING //////////
		
		drawnContours = image.clone();
		bitwise_xor(drawnContours, drawnContours, drawnContours);
		
		drawAllContours(drawnContours, gloveContours, Scalar(255, 0, 0));
		drawAllContours(drawnContours, edgeContours, Scalar(0, 0, 255));
		imshow(windowName, drawnContours);
		
		if(gloveContours.size() > 0)
		{
			float xDif = xHigher - xLower;
			float yDif = yHigher - yLower;
			
			Moments m = moments(gloveContours[0]);
			lr = (m.m10 / m.m00) / xDif;
			fb = (m.m91 / m.m00) / yDif;
		}
		
		if(showWindow && waitKey(5) > 0)
		{
			break;
		}
	}
	
	if(showWindow)
	{
		destroyWindow(windowName);
	}
	//return 0;
}