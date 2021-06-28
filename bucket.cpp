#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace cv;
using namespace std;

Mat imgOriginal;
Mat imgThresholded;

int thresh=100;
int max_thresh=255;

void thresh_callback(int, void*);

int j=0;

int main()
{
	VideoCapture cap(0); 				//capture the video from web cam

	if ( !cap.isOpened() )  				// if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	namedWindow("Control", CV_WINDOW_AUTOSIZE); 		//create a window called "Control"
	
	int iLowH = 85;  //130
	int iHighH = 179;

	int iLowS = 145; //152
	int iHighS = 255;

	int iLowV = 84;
	int iHighV = 255;

								// Create trackbars in "Control" window

	cvCreateTrackbar("LowH", "Control", &iLowH, 179); 	//Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);


	cvCreateTrackbar("LowS", "Control", &iLowS, 255); 	//Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); 	//Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);


	while (true)
	{
		sleep(2);
		bool bSuccess = cap.read(imgOriginal); 		// read a new frame from video

		 if (!bSuccess) 				//if not success, break loop
		{
		     cout << "Cannot read a frame from video stream" << endl;
		     break;
		}


		Mat imgHSV;

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); 		//Convert the captured frame from BGR to HSV
		 
		  
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

		//morphological opening (remove small objects from the foreground)
		      
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) ); 
		dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) ); 


		imshow("Thresholded Image", imgThresholded); 		//show the thresholded image

		imshow("Original", imgOriginal); 			//show the original image



		thresh_callback(0,0);					//Calling function for contours

	

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop	
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}

	
	}
	
	return 0;

}


void thresh_callback(int, void*)
{
	Mat canny_output;
	vector< vector<Point> >contours;
	vector<Vec4i>hierarchy;

	Canny(imgThresholded,canny_output,thresh,thresh*2, 3);

	findContours(canny_output,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));

	imgOriginal=Mat::zeros(canny_output.size(),CV_8UC3);
	for(int i=0; i<contours.size(); i++)
	{
		Scalar color=Scalar(0,255,0);
		drawContours(imgOriginal,contours,i,color,2,8,hierarchy,0,Point());
	
	}
	
	if(contours.size()>0)
	{
		cout<<"blue object detected "<<j++<<endl;
	}


	namedWindow("Contours",CV_WINDOW_AUTOSIZE);
	imshow("Contours", imgOriginal);
	
}
