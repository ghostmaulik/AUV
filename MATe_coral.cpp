#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "opencv2/video/background_segm.hpp"
#include "opencv2/video/tracking.hpp"

using namespace cv;
using namespace std;

/**
 * Simple shape detector program.
 * It loads an image and tries to find simple shapes (rectangle, triangle, circle, etc) in it.
 * This program is a modified version of `squares.cpp` found in the OpenCV sample dir.
 */

/**
 * Helper function to find a cosine of angle between vectors
 * from pt0->pt1 and pt0->pt2
 */
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
 
/**
 * Helper function to display text in the center of a contour
 
void setLabel(cv::Mat& im, const std::string label, std::vector& contour)
{
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.4;
    int thickness = 1;
    int baseline = 0;
 
    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::Rect r = cv::boundingRect(contour);
 
    cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
    cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
    cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}
*/

int check_yellow(cv::Mat& Roi1)
{
    Mat imgHSV;
    Mat imgThresholded;
	int thresh=100;
    int n=0;
    int max_thresh=255;


    int iLowH = 85;  //130
	int iHighH = 179;

	int iLowS = 145; //152
	int iHighS = 255;

	int iLowV = 84;
	int iHighV = 255;


        cvtColor(Roi1, imgHSV, COLOR_BGR2HSV); 		//Convert the captured frame from BGR to HSV
		 
		  
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

		//morphological opening (remove small objects from the foreground)
		      
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) ); 
		dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) ); 

    Mat canny_output;
	vector< vector<Point> >contours;
	vector<Vec4i>hierarchy;

	Canny(imgThresholded,canny_output,thresh,thresh*2, 3);

	findContours(canny_output,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));

	Roi1=Mat::zeros(canny_output.size(),CV_8UC3);
	//for(int i=0; i<contours.size(); i++)
	//{
	//	Scalar color=Scalar(0,255,0);
	//	drawContours(Roi1,contours,i,color,2,8,hierarchy,0,Point());
	//
	//}
	
	if(contours.size()>2)
	{
		n=1;
	}

    return n;

}


cv::Mat Roi1,Roi2,Roi3;
cv::Mat Roi[3];

char ch[9][3];

int main()
{
    //cv::Mat src = cv::imread("polygon.png");
    cv::Mat src;
    cv::Mat gray;
    cv::Mat bw;
    cv::Mat dst;
    std::vector<std::vector > contours;
    std::vector approx;
 
    VideoCapture capture(0);

    int q;
    int y=0,p=0;
    int b=0;
     while(cvWaitKey(30) != 'q')
    {
        if(y==1)
        goto 2;

        1: 

        capture >> src;

        Rect Rec3(0, 150, 620, 320);
        rectangle(src, Rec3, Scalar(255), 1, 8, 0);
        Roi3 = src(Rec3);

        Rect Rec1(0, 235, 620, 235);
        rectangle(src, Rec1, Scalar(255), 1, 8, 0);
        Roi1 = src(Rec1);

        Rect Rec4(0, 150, 208, 320);
        rectangle(src, Rec4, Scalar(255), 1, 8, 0);
        Roi[0] = src(Rec4);
        
        Rect Rec5(206, 235, 414, 235);
        rectangle(src, Rec5, Scalar(255), 1, 8, 0);
        Roi[1] = src(Rec5);
        
        Rect Rec6(412, 235, 620, 235);
        rectangle(src, Rec6, Scalar(255), 1, 8, 0);
        Roi[2] = src(Rec6);



        y=check_yellow(Roi1);

        if(y==0)
        {
            goto 1;
        }


        2: p=0;
            
        capture >> src;
        
        Rect Rec1(0, 425, 620, 45);
        rectangle(src, Rec2, Scalar(255), 1, 8, 0);
        Roi2 = src(Rec2);

        p=check_purple(Roi2);
        
    
        if(p==1)
        {
            ctr++;
        }

        if(y==1||p>0)
        {   
            for(int i=0; i<3; i++)
            {
                // Convert to grayscale
                cv::cvtColor(Roi[i], gray, CV_BGR2GRAY);
 
                // Use Canny instead of threshold to catch squares with gradient shading
                blur( gray, bw, Size(3,3) );
                cv::Canny(gray, bw, 80, 240, 3);
                //cv::imshow("bw", bw);
                //cv::bitwise_not(bw, bw);
 
                // Find contours
                cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
 
                src.copyTo(dst);
 
                for (int i = 0; i < contours.size(); i++)
                {
                    // Approximate contour with accuracy proportional
                    // to the contour perimeter
                    cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);
 
                    // Skip small or non-convex objects
                    //if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
                    //    continue;
 
                    if (approx.size() == 3)
                    {
                        ;//setLabel(dst, "TRI", contours[i]);    // Triangles
                    }
                    else if (approx.size() >= 4 && approx.size() <= 6)
                    {
                        // Number of vertices of polygonal curve
                        int vtc = approx.size();
 
                        // Get the cosines of all corners
                        std::vector cos;
                        
                        for (int j = 2; j < vtc+1; j++)
                        cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));
 
                        // Sort ascending the cosine values
                        std::sort(cos.begin(), cos.end());
 
                        // Get the lowest and the highest cosine
                        double mincos = cos.front();
                        double maxcos = cos.back();
 
                        // Use the degrees obtained above and the number of vertices
                        // to determine the shape of the contour
                    
                        if (vtc == 4 )
                        {    
                            //setLabel(dst, "RECT", contours[i]);
                            b=check_yellow(Roi3);
                            if(b==1)
                            ch[ctr-1][i]=='f' //frag
                            else
                            ch[ctr-1][i]=='c' //large colony
                 
                        }

                        else if (vtc == 10 )
                        {
                             //setLabel(dst, "STAR", contours[i]);
                            ch[ctr-1][i]=='t' //thorrn star

                        }   
                    }
                    else
                    {
                        // Detect and label circles
                        double area = cv::contourArea(contours[i]);
                        cv::Rect r = cv::boundingRect(contours[i]);
                        int radius = r.width / 2;
 
                        if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
                            std::abs(1 - (area / (CV_PI * (radius*radius)))) <= 0.2)
                        {
                            //setLabel(dst, "CIR", contours[i]);
                            ch[ctr-1][i]=='s' //sponge
                        }
                        
                    }
                }
            }   
        }
           
     //cv::imshow("src", src);
     //cv::imshow("dst", dst);
 
     if(ctr==8)
     break;
    
    }
 
    for(int j=0; j<9; j++)
    {
        for(int k=0; k<3; k++)
        {
            if(ch[j][k]=='s')
            
        }
    }


    return 0;
} 


/* 1. Task to detect only horizontal purple lines
   2. after that, drawing circles on image

