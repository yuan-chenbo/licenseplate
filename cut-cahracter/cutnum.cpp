#include"cutnum.h"

int main(int argc, char* argv[])
{
	//图像灰度二值化
IplImage* src_num=cvLoadImage("D:/vs/CarNumber/100.jpg");
Mat dst_num;
RgbConvToGray(src_num,dst_num);
imshow("cvt",dst_num);
IplImage dst_numb=dst_num;

	CvMemStorage* storage=cvCreateMemStorage(0);
	CvSeq* first_contours=NULL;

	IplImage src = dst_num;

	IplImage *dsw=cvCreateImage(dst_num.size(),8,3);
	IplImage *dst=cvCreateImage(dst_num.size(),8,3);
	cvThreshold(&dst_numb,&src,190,255,CV_THRESH_BINARY);
	//cvShowImage("二值化图像",&src);
	uchar* temp;
	int sum=0;        
		
		
		
		
		//去边框
	for (int j=0;j<src.height;j++)
	{
		for (int cok=1;cok<src.width;cok++)
		{
			sum=sum+abs(cvGet2D(&src,j,cok).val[0]-cvGet2D(&src,j,cok-1).val[0]);
		}
		cout<<"第"<<j<<"行的值为"<<sum/255<<endl;
		if (sum<30*255)
		{
			for (int cok2=0;cok2<src.width;cok2++)
		{

			cvSet2D(&src,j,cok2,cvScalar(0));
		}
			
		}
	}
	
	//cvShowImage("去边框图像",&src);
	IplImage src_copy=src;
	//cvShowImage("去边框图像2",&src_copy);
	
	
	//找边界
	cvFindContours(&src_copy,storage,&first_contours,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);
	cvZero(dst);
	int cnt=0;
	int s=200;
	int counter=0;
	CvSize size;
	int pointcut[7];
	int pointwidth[7];
	int tempint=0;


	//cvNamedWindow("找边界");
	//cvShowImage("找边界",src_num);
	
	
	
	
	//画边界
	for(;first_contours!=0;first_contours=first_contours->h_next)
	{
		cnt++;
		CvScalar color1=CV_RGB(255,255,255);
		CvScalar color2=CV_RGB(255,255,255);
		cvDrawContours(dst,first_contours,color1,color2,0,2,8,cvPoint(0,0));
		cvShowImage("dstaa",dst);
		CvRect rect=cvBoundingRect(first_contours,0);
		if (rect.height*rect.width>=s)		
		{ counter=counter+1;
		cvRectangle(dst,cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),CV_RGB(255,0,0),1,8,0);	
		cout<<"第"<<counter<<"个矩形框的大小为"<<rect.height*rect.width<<endl;
		
		
		/*size.height =src.height,size.width=rect.width;
		cvSetImageROI(&src,rect);
		IplImage* dst_img=cvCreateImage(size,8,1);
		cvCopy(&src,dst_img);
		cvResetImageROI(&src);

       cvShowImage("0"+counter,dst_img);*/
		
			pointcut[counter-1]=rect.x;
			pointwidth[counter-1]=rect.width;					
	}
	}
	for(int i=0;i<6;i++)
	{
		for(int j=i+1;j<7;j++){if(pointcut[j]<pointcut[i]){tempint=pointcut[j],pointcut[j]=pointcut[i],pointcut[i]=tempint;
		tempint=pointwidth[j],pointwidth[j]=pointwidth[i],pointwidth[i]=tempint;}}
	
	}
	
	
	//cvNamedWindow("画边界");
	//cvShowImage("画边界",&src);

	
	//找出坐标值
	for (int l=0;l<7;l++)
	{
		cout<<pointcut[l]<<"\n"<<endl;

	}





	
	for (int i=0;i<7;i++)
	{

		cout<<pointcut[i]<<endl;
	IplImage* Img_dst = cvCreateImage(cvSize(pointwidth[i],32),8,3);    
	CvRect r;
	r.x=pointcut[i];
	r.y=0;
	r.width =pointwidth[i];
	r.height=32;
	cvSetImageROI(src_num,r);
	cvCopy(src_num,Img_dst);
	cvResetImageROI(src_num);
	
	
	switch (i)
	
   {case 0:  
	{Mat dst_num0;
	RgbConvToGray(Img_dst,dst_num0);
	threshold(dst_num0,dst_num0,170,255,CV_THRESH_BINARY);
	imshow("img1",dst_num0);
	imwrite("101.jpg",dst_num0);
		dst_num0.release();
	}
	case 1:
		{Mat dst_num1;
		RgbConvToGray(Img_dst,dst_num1);
	threshold(dst_num1,dst_num1,170,255,CV_THRESH_BINARY);
		imshow("img2",dst_num1);
		imwrite("102.jpg",dst_num1);
				dst_num1.release();
		}
	case 2:
		{Mat dst_num2;
		RgbConvToGray(Img_dst,dst_num2);
		threshold(dst_num2,dst_num2,170,255,CV_THRESH_BINARY);
		imshow("img3",dst_num2);
		imwrite("103.jpg",dst_num2);
				dst_num2.release();
		}
	case 3:
		{Mat dst_num3;
	RgbConvToGray(Img_dst,dst_num3);
		threshold(dst_num3,dst_num3,170,255,CV_THRESH_BINARY);
		imshow("img4",dst_num3);
		imwrite("104.jpg",dst_num3);
			dst_num3.release();
		}
	case 4:
		{Mat dst_num4;
		RgbConvToGray(Img_dst,dst_num4);
		threshold(dst_num4,dst_num4,170,255,CV_THRESH_BINARY);
		imshow("img5",dst_num4);
		imwrite("105.jpg",dst_num4);
		dst_num4.release();
		}
	case 5:
		{Mat dst_num5;
		RgbConvToGray(Img_dst,dst_num5);
		threshold(dst_num5,dst_num5,170,255,CV_THRESH_BINARY);
		imshow("img6",dst_num5);
		imwrite("106.jpg",dst_num5);
		dst_num5.release();
		}
	case 6:
		{Mat dst_num6;
		RgbConvToGray(Img_dst,dst_num6);
		threshold(dst_num6,dst_num6,170,255,CV_THRESH_BINARY);
		imshow("img7",dst_num6);	
		imwrite("107.jpg",dst_num6);
		dst_num6.release();
		}

	}

	}
	//cvNamedWindow("dst");
	//cvShowImage("dst",dst);
	cout<<"num of contours:"<<counter<<endl;
	waitKey();
	return 0;
}