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


cv::VideoCapture capVideo;



capVideo.open("/home/maulik/Videos/Video1.mp4");

if(!capVideo.isOpened())
{
	std::cout<<"\n error loading video file"<<std::endl<<std::endl;
	getch();
	return(0);
}



capVideo.read(imgOriginal);

char chCheckForEscKey = 0;

while (capVideo.isOpened() &&chCheckForEscKey !=27)
{


cv::imshow("imgFrame", imgFrame);

if((capVideo.get(CV_CAP_PROP_POS_FRAMES)+1)<capVideo.get(CV_CAP_PROP_FRAME_COUNT))
{

	capVideo.read(imgFrame);
}

else
{
cout<<"end of video"<<endl;
break;
}





/*VideoCapture cap(0); 				//capture the video from web cam

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
*/

 Mat dst, cdst;

 Canny(imgOriginal, dst, 50, 150, 3);


cvtColor(dst, cdst, CV_GRAY2BGR);

 Mat cdst2=cdst;

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
 

  vector<Vec4i> lines;
  HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 20 );
  for( size_t i = 0; i < lines.size(); i++ )
  {
    Vec4i l = lines[i];
    line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
  }
 //#endif


imshow("source", imgOriginal);
imshow("detected lines", cdst);
imshow("aha!", cdst2);

if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop	
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}


}

 return 0;
}

