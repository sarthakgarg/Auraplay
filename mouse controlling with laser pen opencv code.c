/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php

*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include </usr/local/include/opencv/cv.h>
#include </usr/local/include/opencv/highgui.h>
#include "RaspiCamCV.h"
int i,j;
int adjustx=0,adjusty=0,constx=0,consty=0;
int lower=90,upper=255,count=0,low=0,high=255;
int cnt =0;
int xlatest1=0, ylatest1=0,xlatest3=0,ylatest3=0;
int flag=0;
char str1[5];
char str2[5];
char str3[100];
char temp[20];
char str4[20];
int newX;
int newY;
IplImage* GetThresholdedImage(IplImage* img)
{
	// Convert the image into an HSV image
	IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
	cvCvtColor(img, imgHSV, CV_BGR2HSV);

	IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);

	// Values 20,100,100 to 30,255,255 working perfect for yellow at around 6pm
	cvInRangeS(imgHSV, cvScalar(0, 100, 150,0), cvScalar(11, 255, 255,255), imgThreshed);

	cvReleaseImage(&imgHSV);

	return imgThreshed;
}

int main()
{
    cvNamedWindow("Trackbar",1);
  
               cvCreateTrackbar("thresh","Trackbar",&lower,120,NULL);
               
	 IplImage* frame=0;
	// Initialize capturing live feed from the camera

  RaspiCamCvCapture* capture = 0;	
capture = raspiCamCvCreateCameraCapture(0);
	// Couldn't get a device? Throw an error and quit
	if(!capture)
    {
       printf("Could not initialize capturing...\n");
        return -1;
    }

	// The two windows we'll be using
   // cvNamedWindow("video",1);
//	cvNamedWindow("thresh",1);

	// This image holds the "scribble" data...
	// the tracked positions of the ball
	IplImage* imgScribble = NULL;

	// An infinite loop
	
	 while(1){

			
       frame = raspiCamCvQueryFrame(capture);         
        if(!frame) break;
        frame=cvCloneImage(frame); 
         
//smooth the original image using Gaussian kernel
       // cvSmooth(frame, frame, CV_GAUSSIAN,3,3,0,0); 

//converting the original image into grayscale
	IplImage* imgHSV = cvCreateImage(cvGetSize(frame), 8, 3); 
	cvCvtColor(frame, imgHSV, CV_BGR2HSV);
	IplImage* imgThreshed = cvCreateImage(cvGetSize(frame), 8, 1);

          
       //thresholding the grayscale image to get better results
	cvInRangeS(imgHSV, cvScalar(low, 0,lower,0), cvScalar(high, 255,upper,255), imgThreshed);
            
        
        
        

        
             
        //cvShowImage("Video", frame);
	//cvShowImage("grey", imgThreshed);







CvSeq* contour;  //hold the pointer to a contour
CvSeq* result;   //hold sequence of points of a contour
CvMemStorage *storage = cvCreateMemStorage(0); //storage area for all contours







//finding all contours in the image
cvFindContours(imgThreshed, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));





CvPoint *pt[4];





//iterating through each contour and counting till 5000


while(contour)
{
	
	

//obtain a sequence of points of the countour, pointed by the variable 'countour'
	result = cvApproxPoly(contour, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 	cvContourPerimeter(contour)*0.02, 0);
      
//if there are 3 vertices  in the contour and the area of the triangle is more than 100 pixels
	if(result->total==4 && fabs(cvContourArea(result, CV_WHOLE_SEQ,-1))>100 )
	{
	//iterating through each point

	for( i=0;i<4;i++){
		pt[i] = (CvPoint*)cvGetSeqElem(result, i);
	}

	//drawing lines around the triangle
	cvLine(frame, *pt[0], *pt[1], cvScalar(255,0,0,100),5,8,0);
	cvLine(frame, *pt[1], *pt[2], cvScalar(0,255,0,100),5,8,0);
	cvLine(frame, *pt[2], *pt[3], cvScalar(0,0,255,100),5,8,0);
	cvLine(frame, *pt[3], *pt[0], cvScalar(255,0,0,100),5,8,0);



	printf("x1 = %d, y1 = %d x3 = %d y3 = %d count = %d\n",pt[1]->x,pt[1]->y,pt[3]->x,pt[3]->y,cnt);
	cnt++;
	xlatest1= pt[1]->x;
	ylatest1= pt[1]->y;
	xlatest3= pt[3]->x;
	ylatest3= pt[3]->y;

	
	
	 
	
	}
	//obtain the next contour
	contour = contour->h_next;
}

	if(cnt>=60)
	break;
	cvNamedWindow("Tracked",1);
	cvShowImage("Tracked",frame);

   //	printf("X avg : %d   Y avg : %d\n",avgx/10000,avgy/10000);
        //Clean up used images
        cvReleaseImage(&imgThreshed);            
        cvReleaseImage(&frame);

        //Wait 10mS
        int c = cvWaitKey(10);
        //If 'ESC' is pressed, break the loop
        if((char)c==27 ) break;      
   }
   printf("the final values of x and y are %d %d %d %d ",xlatest1,ylatest1,xlatest3,ylatest3);

    cvDestroyAllWindows();
	cvNamedWindow("Trackbar",1);
	 cvCreateTrackbar("X","Trackbar",&adjustx,40,NULL);
      cvCreateTrackbar("Y","Trackbar",&adjusty,40,NULL);
          cvCreateTrackbar("constX","Trackbar",&constx,40,NULL);
              cvCreateTrackbar("constY","Trackbar",&consty,40,NULL);
  

	while(1)
    {
		// Will hold a frame captured from the camera
		IplImage* frame = 0;
		
    frame = raspiCamCvQueryFrame(capture);  

		// If we couldn't grab a frame... quit
        if(!frame)
            break;

		// If this is the first frame, we need to initialize it
		//if(imgScribble == NULL)
	//	{
	//		imgScribble = cvCreateImage(cvGetSize(frame), 8, 3);
	//	}

		// Holds the yellow thresholded image (yellow = white, rest = black)
		IplImage* imgYellowThresh = GetThresholdedImage(frame);

		// Calculate the moments to estimate the position of the ball
		CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
		cvMoments(imgYellowThresh, moments, 1);

		// The actual moment values
		double moment10 = cvGetSpatialMoment(moments, 1, 0);
		double moment01 = cvGetSpatialMoment(moments, 0, 1);
		double area = cvGetCentralMoment(moments, 0, 0);

		// Holding the last and current ball positions
		static int posX = 0;
		static int posY = 0;

		int lastX = posX;
		int lastY = posY;
		
		strcpy(str4," ");
		posX = moment10/area;
		posY = moment01/area;
		newX=640*(posX-xlatest1-adjustx+constx)/(xlatest3-xlatest1-2*adjustx);
		newY=480*(posY-ylatest1-consty-adjusty)/(ylatest3-ylatest1-2*adjusty);
		snprintf(str1,sizeof(str1),"%d",newX);
		snprintf(str2,sizeof(str2),"%d",newY);
		printf("screen pos: %d %d\n",newX,newY );	
		strcpy(str3,"xdotool mousemove ");
		strcat(str3,str1);
		strcat(str3,str4);
		strcat(str3,str2);
	//	strcat("xdotool mousemove ",str1);
		if(newX>0)
		system(str3);
		if(newX>0&&flag==0)
		{
			flag=1;
			system("xdotool mousedown 1");
			printf("mouse is pressed down\n");
		}	
		if(newY<0&&flag==1)
		{ 
			flag=0;
			system("xdotool mouseup 1");	
			printf("mouse is pressed up\n");
		}	
		printf("%d\n",flag);
	/*	// Print it out for debugging purposes
		printf("position (%d,%d)\n", posX, posY);

		// We want to draw a line only if its a valid position
		if(lastX>0 && lastY>0 && posX>0 && posY>0)
		{
			// Draw a yellow line from the previous point to the current point
			cvLine(imgScribble, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(0,255,255,100), 5,8,0);
		}

		// Add the scribbling image and the frame... and we get a combination of the two
		cvAdd(frame, imgScribble, frame,NULL);*/
		//cvShowImage("thresh", imgYellowThresh);
		//cvShowImage("video", frame);

		
		int c = cvWaitKey(10);
		if(c!=-1)
		{
			
           		 break;
		}

		
		cvReleaseImage(&imgYellowThresh);

		
    }

	
  raspiCamCvReleaseCapture(&capture);   
    return 0;
}
