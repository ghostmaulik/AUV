#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/video/tracking.hpp"

#include <iostream>
#include <unistd.h>

using namespace cv;
using namespace std;

double identify_g(int, void*);
double identify_r(int, void*);
double verify_g(int, void*);
double verify_r(int, void*);

Mat imgOriginal,imgThresholded1,imgThresholded2;
int thresh=100;
int j=1,k=1;
Mat Roi1,Roi2;

int main(int argc, char** argv)
{
	
namedWindow("Control1", CV_WINDOW_AUTOSIZE); 		//create a window called "Control"
	
	int iLowH = 52;  //130
	int iHighH = 100;

	int iLowS = 90; //152
	int iHighS = 255;

	int iLowV = 118;
	int iHighV = 255;

								// Create trackbars in "Control" window

	cvCreateTrackbar("LowH", "Control1", &iLowH, 179); 	//Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control1", &iHighH, 179);


	cvCreateTrackbar("LowS", "Control1", &iLowS, 255); 	//Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control1", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control1", &iLowV, 255); 	//Value (0 - 255)
	cvCreateTrackbar("HighV", "Control1", &iHighV, 255);



namedWindow("Control2", CV_WINDOW_AUTOSIZE); 		//create a window called "Control"
	
	int iLowH2 = 56;  //85
	int iHighH2 = 179;

	int iLowS2 = 199; //152
	int iHighS2 = 255;

	int iLowV2 = 140;
	int iHighV2 = 255;

								// Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control2", &iLowH2, 179); 	//Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control2", &iHighH2, 179);


	cvCreateTrackbar("LowS", "Control2", &iLowS2, 255); 	//Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control2", &iHighS2, 255);

	cvCreateTrackbar("LowV", "Control2", &iLowV2, 255); 	//Value (0 - 255)
	cvCreateTrackbar("HighV", "Control2", &iHighV2, 255);




cv::VideoCapture capVideo;

VideoCapture cap(0); 				//capture the video from web cam

	if ( !cap.isOpened() )  				// if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}


while (true)
{
		
		bool bSuccess = cap.read(imgOriginal); 		// read a new frame from video

		 if (!bSuccess) 				//if not success, break loop
		{
		     cout << "Cannot read a frame from video stream" << endl;
		     break;
		}



	Mat imgHSV1,imgHSV2;


		cvtColor(imgOriginal, imgHSV1, COLOR_BGR2HSV); 		//Convert the captured frame from BGR to HSV
		cvtColor(imgOriginal, imgHSV2, COLOR_BGR2HSV); 		//Convert the captured frame from BGR to HSV 
		  
		inRange(imgHSV1, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded1); //Threshold the image
		inRange(imgHSV2, Scalar(iLowH2, iLowS2, iLowV2), Scalar(iHighH2, iHighS2, iHighV2), imgThresholded2); //Threshold the image

		erode(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) ); 
		dilate( imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
		
		erode(imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) ); 
		dilate( imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );



	
	imshow("1st", imgThresholded1);


	
	imshow("2nd", imgThresholded2);


imshow("source", imgOriginal);	

double x=0;
double y=0;

x=identify_g(0,0);
y=identify_r(0,0);


if(x<3&&y>5)
{
	cout<<"offset right";
}
else if(x>5&&y<3)
{
	cout<<"offset left";
}
else if(x<3&&y<3)
{
	cout<<"err";
}
else if(x>7&&y>7)
{
	cout<<"both visible, using roi";

	Rect Rec1(0, 0, 210, 470);		//0,0,319,351
	Rect Rec2(420, 0, 210, 470);		//problamatic at 140th iteration

	rectangle(imgThresholded1, Rec1, Scalar(255), 1, 8, 0);
	rectangle(imgThresholded2, Rec2, Scalar(255), 1, 8, 0);	


	Roi1 = imgThresholded1(Rec1);


	Roi2 = imgThresholded2(Rec2);

	double m,k;

	m=verify_r(0,0);
	k=verify_g(0,0);

	if(m==k&&m==1)
	{
		cout<<"surge";
	}
	else
	{
		continue;
	}

}


else 
{
	continue;
}



if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop	
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}
sleep(0.4);
}


return 0;
}

double identify_g(int, void*)
{
Mat canny_output;
double perimeter_i;
	vector< vector<Point> >contours;
	vector<Vec4i>hierarchy;

	Canny(imgThresholded2,canny_output,thresh,thresh*2, 3);

	findContours(canny_output,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));

	//Roi1=Mat::zeros(canny_output.size(),CV_8UC3);
	//for(int i=1; i<contours.size()-1; i++)
	//{
	//	Scalar color=Scalar(0,255,0);
	//	drawContours(Roi1,contours,i,color,2,8,hierarchy,0,Point());
	
//if(contours.size()>3)
    //{
        // The first contour (and probably the only one)
        // is the one you're looking for

        // Compute the perimeter
        // perimeter_i = perimeter_i + contours[0].size();
    //}


	//}
	
	if(contours.size()>7)
	{
		cout<<"move towards left "<<j++<<endl; //<<" perimeter : "<<perimeter_i<<endl;
		
	}
//namedWindow("Contours1",CV_WINDOW_AUTOSIZE);
//	imshow("Contours1", Roi1);

	
	return contours.size();//perimeter_i;
}


double identify_r(int, void*)
{
Mat canny_output; double perimeter_i;

	vector< vector<Point> >contours;
	vector<Vec4i>hierarchy;

	Canny(imgThresholded1,canny_output,thresh,thresh*2, 3);

	findContours(canny_output,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));

	//Roi2=Mat::zeros(canny_output.size(),CV_8UC3);
	//for(int i=1; i<contours.size()-1; i++)
	//{
	//	Scalar color=Scalar(0,255,0);
	//	drawContours(Roi2,contours,i,color,2,8,hierarchy,0,Point());


//if(contours.size()>3)
  //  {
        // The first contour (and probably the only one)
        // is the one you're looking for

        // Compute the perimeter
    //     perimeter_i = perimeter_i + contours[0].size();
    //}

	
	//}
	
	if(contours.size()>7)
	{
		cout<<"move towards right "<<k++<<endl;//" Perimeter : "<<perimeter_i<<endl;
	}
//namedWindow("Contours2",CV_WINDOW_AUTOSIZE);
//	imshow("Contours2", Roi2);

	return contours.size();
}




double verify_g(int, void*)
{
Mat canny_output;
double perimeter_i;
	vector< vector<Point> >contours;
	vector<Vec4i>hierarchy;

	Canny(Roi2,canny_output,thresh,thresh*2, 3);

	findContours(canny_output,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));

	//Roi1=Mat::zeros(canny_output.size(),CV_8UC3);
	//for(int i=1; i<contours.size()-1; i++)
	//{
	//	Scalar color=Scalar(0,255,0);
	//	drawContours(Roi1,contours,i,color,2,8,hierarchy,0,Point());
	
//if(contours.size()>3)
    //{
        // The first contour (and probably the only one)
        // is the one you're looking for

        // Compute the perimeter
        // perimeter_i = perimeter_i + contours[0].size();
    //}


	//}
double flag=0;	
	if(contours.size()>5)
	{
		flag=1;			//<<" perimeter : "<<perimeter_i<<endl;
		
	}
//namedWindow("Contours1",CV_WINDOW_AUTOSIZE);
//	imshow("Contours1", Roi1);

	
	return flag;//perimeter_i;
}


double verify_r(int, void*)
{
Mat canny_output; double perimeter_i;

	vector< vector<Point> >contours;
	vector<Vec4i>hierarchy;

	Canny(Roi1,canny_output,thresh,thresh*2, 3);

	findContours(canny_output,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));

	//Roi2=Mat::zeros(canny_output.size(),CV_8UC3);
	//for(int i=1; i<contours.size()-1; i++)
	//{
	//	Scalar color=Scalar(0,255,0);
	//	drawContours(Roi2,contours,i,color,2,8,hierarchy,0,Point());


//if(contours.size()>3)
  //  {
        // The first contour (and probably the only one)
        // is the one you're looking for

        // Compute the perimeter
    //     perimeter_i = perimeter_i + contours[0].size();
    //}

	
	//}
double flag=0;	
	if(contours.size()>5)
	{
		flag=1;		
	}
//namedWindow("Contours2",CV_WINDOW_AUTOSIZE);
//	imshow("Contours2", Roi2);

	return flag;
}
