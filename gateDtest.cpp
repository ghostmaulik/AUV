#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/video/tracking.hpp"

#include <iostream>
#include <unistd.h>

using namespace cv;
using namespace std;

void help()
{
 cout << "\nThis program demonstrates line finding with the Hough transform.\n"
         "Usage:\n"
         "./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}

int thresh=100;
int j=1;
Mat Roi1,Roi2;

void thresh_callback1(int, void*);
void thresh_callback2(int, void*);


int main(int argc, char** argv)
{

Mat imgThresholded1,imgThresholded2;

//namedWindow("Control1", CV_WINDOW_AUTOSIZE); 		//create a window called "Control"
	
	int iLowH = 42;  //130
	int iHighH = 80;

	int iLowS = 56; //152
	int iHighS = 255;

	int iLowV = 111;
	int iHighV = 255;

								// Create trackbars in "Control" window

//	cvCreateTrackbar("LowH", "Control1", &iLowH, 179); 	//Hue (0 - 179)
//	cvCreateTrackbar("HighH", "Control1", &iHighH, 179);


//	cvCreateTrackbar("LowS", "Control1", &iLowS, 255); 	//Saturation (0 - 255)
//	cvCreateTrackbar("HighS", "Control1", &iHighS, 255);

//	cvCreateTrackbar("LowV", "Control1", &iLowV, 255); 	//Value (0 - 255)
//	cvCreateTrackbar("HighV", "Control1", &iHighV, 255);



//namedWindow("Control2", CV_WINDOW_AUTOSIZE); 		//create a window called "Control"
	
	int iLowH2 = 35;  //85
	int iHighH2 = 179;

	int iLowS2 = 94; //152
	int iHighS2 = 255;

	int iLowV2 = 140;
	int iHighV2 = 255;

								// Create trackbars in "Control" window
//	cvCreateTrackbar("LowH", "Control2", &iLowH2, 179); 	//Hue (0 - 179)
//	cvCreateTrackbar("HighH", "Control2", &iHighH2, 179);


//	cvCreateTrackbar("LowS", "Control2", &iLowS2, 255); 	//Saturation (0 - 255)
//	cvCreateTrackbar("HighS", "Control2", &iHighS2, 255);

//	cvCreateTrackbar("LowV", "Control2", &iLowV2, 255); 	//Value (0 - 255)
//	cvCreateTrackbar("HighV", "Control2", &iHighV2, 255);




Mat image1,image2;

image1=imread("/home/maulik/Downloads/gr.png" , CV_LOAD_IMAGE_COLOR);
image2=imread("/home/maulik/Downloads/gr.png", CV_LOAD_IMAGE_COLOR);

/*while (true)
	{
		
		bool bSuccess = cap.read(imgOriginal); 		// read a new frame from video

		 if (!bSuccess) 				//if not success, break loop
		{
		     cout << "Cannot read a frame from video stream" << endl;
		     break;
		}

*/


//sleep(1);


Mat imgHSV1,imgHSV2;


		cvtColor(image1, imgHSV1, COLOR_BGR2HSV); 		//Convert the captured frame from BGR to HSV
		cvtColor(image1, imgHSV2, COLOR_BGR2HSV); 		//Convert the captured frame from BGR to HSV 
		  
		inRange(imgHSV1, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded1); //Threshold the image
		inRange(imgHSV2, Scalar(iLowH2, iLowS2, iLowV2), Scalar(iHighH2, iHighS2, iHighV2), imgThresholded2); //Threshold the image

	//	erode(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)) ); 
	//	dilate( imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
		
	//	erode(imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)) ); 
	//	dilate( imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

		imshow("thresholded1",imgThresholded1);
	//	imshow("thresholded2",imgThresholded2);


Rect Rec1(0, 0, 100, 100);		//0,0,319,351
Rect Rec2(0, 0, 100, 100);		//problamatic at 140th iteration

//Draw the rectangle into LoadedImage
//Parameters are (into Mat, Rec describe position where to draw rectangle
// Scalar is Color, 1 is thickness, 8 is line type and 0 shift position
rectangle(imgThresholded1, Rec1, Scalar(200), 1, 8, 0);
rectangle(imgThresholded2, Rec2, Scalar(255), 1, 8, 0);		

Roi1 = imgThresholded1(Rec1);
  //namedWindow("Step 3 Draw selected Roi1", WINDOW_AUTOSIZE);
  imshow("Step 3 Draw selected Roi1", Roi1);

Roi2 = imgThresholded2(Rec2);
  //namedWindow("Step 3 Draw selected Roi2", WINDOW_AUTOSIZE);
  imshow("Saep 3 Draw selected Roi2", Roi2);

thresh_callback1(0,0);			//Calling function for contours
thresh_callback2(0,0);




 //Canny(imgOriginal, dst, 50, 150, 3);

//cvtColor(imgOriginal, gray, COLOR_BGR2GRAY);
//threshold( gray, thr, 100,150,THRESH_BINARY );

//Moments m = moments(imgThresholded,true);
//Point p(m.m10/m.m00, m.m01/m.m00);

//cvtColor(dst, cdst, CV_GRAY2BGR);

/* Mat cdst2=cdst;

  vector<Vec2f> lines2;
  HoughLines(dst, lines2, 1, CV_PI/180, 100, 0, 0 );

  for( size_t i = 0; i < lines2.size(); i++ )
  {
     float rho = lines2[i][0], theta = lines2[i][1];
     Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;
     pt1.x = cvRound(x0 + 1000*(-b));
     pt1.y = cvRound(y0 + 1000*(a));
     pt2.x = cvRound(x0 - 1000*(-b));
     pt2.y = cvRound(y0 - 1000*(a));
 
     double angle = atan2(y0, x0) * 180.0 / CV_PI;
	cout<<angle;	
     line( cdst2, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
  }
 */

 /* vector<Vec4i> lines;
  HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 20 );
  for( size_t i = 0; i < lines.size(); i++ )
  {
    Vec4i l = lines[i];
    line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
  }
*/ 

//circle(imgThresholded, p, 10, Scalar(128,0,0), -1);


namedWindow("source1", CV_WINDOW_AUTOSIZE);
imshow("source1", image1);

waitKey(0);
// imshow("detected lines", cdst);
//imshow("aha!", cdst2);
//imshow("Image with center",imgThresholded);



 return 0;
}


void thresh_callback1(int, void*)
{
Mat canny_output;
	vector< vector<Point> >contours;
	vector<Vec4i>hierarchy;

	Canny(Roi1,canny_output,thresh,thresh*2, 3);

	findContours(canny_output,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));

	Roi1=Mat::zeros(canny_output.size(),CV_8UC3);
	for(int i=1; i<contours.size()-1; i++)
	{
		Scalar color=Scalar(0,255,0);
		drawContours(Roi1,contours,i,color,2,8,hierarchy,0,Point());
	
	}
	
	if(contours.size()>3)
	{
		cout<<"move towards left "<<j++<<endl;
	}
namedWindow("Contours1",CV_WINDOW_AUTOSIZE);
	imshow("Contours1", Roi1);

}


void thresh_callback2(int, void*)
{
Mat canny_output;
	vector< vector<Point> >contours;
	vector<Vec4i>hierarchy;

	Canny(Roi2,canny_output,thresh,thresh*2, 3);

	findContours(canny_output,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));

	Roi2=Mat::zeros(canny_output.size(),CV_8UC3);
	for(int i=1; i<contours.size()-1; i++)
	{
		Scalar color=Scalar(0,255,0);
		drawContours(Roi1,contours,i,color,2,8,hierarchy,0,Point());
	
	}
	
	if(contours.size()>3)
	{
		cout<<"move towards right "<<j++<<endl;
	}
namedWindow("Contours2",CV_WINDOW_AUTOSIZE);
	imshow("Contours2", Roi2);

}


