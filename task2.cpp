#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

void help()
{
 cout << "\nThis program demonstrates line finding with the Hough transform.\n"
         "Usage:\n"
         "./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}

int main(int argc, char** argv)
{

Mat imgOriginal;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
 
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


 Mat dst, cdst;
 Canny(imgOriginal, dst, 50, 150, 3);
 findContours( dst, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
 cvtColor(dst, cdst, CV_GRAY2BGR);

 /*#if 0
  vector<Vec2f> lines;
  HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );

  for( size_t i = 0; i < lines.size(); i++ )
  {
     float rho = lines[i][0], theta = lines[i][1];
     Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;
     pt1.x = cvRound(x0 + 1000*(-b));
     pt1.y = cvRound(y0 + 1000*(a));
     pt2.x = cvRound(x0 - 1000*(-b));
     pt2.y = cvRound(y0 - 1000*(a));
     line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
  }
 #else */
  vector<Vec4i> lines;
  HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 20 );
  for( size_t i = 0; i < lines.size(); i++ )
  {
    Vec4i l = lines[i];
    line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
  }
 //#endif

 	
  // get the moments
vector<Moments> mu(contours.size());
for( int i = 0; i<contours.size(); i++ )
{ mu[i] = moments( contours[i], false ); }
 
// get the centroid of figures.
vector<Point2f> mc(contours.size());
for( int i = 0; i<contours.size(); i++)
{ mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }
 
 
// draw contours
Mat drawing(dst.size(), CV_8UC3, Scalar(255,255,255));
for( int i = 0; i<contours.size(); i++ )
{
Scalar color = Scalar(167,151,0); // B G R values
drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
circle( drawing, mc[i], 8, color, -1, 8, 0 );
}



 imshow("source", imgOriginal);
 imshow("detected lines", cdst);
 namedWindow( "Contours", WINDOW_AUTOSIZE );
imshow( "Contours", drawing );

 if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop	
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}

waitKey();

}
 return 0;
}



