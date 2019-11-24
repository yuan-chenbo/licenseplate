#include "carID_Detection.h"
#include <string>
#include<opencv2/opencv.hpp>
 
using namespace std;
using namespace cv;
int main(int argc, char* argv[])
{    
	Mat img_input = imread("D:/vs/CarNumber/1.jpg");
	
	Mat img_input2;
	//img_input.copyTo
	 img_input2 = img_input.clone();
	//如果读入图像失败
	if(img_input.empty())
	{
		fprintf(stderr, "Can not load image %s\n", "testCarID.jpg");
		return -1;
	}
	Mat hsvImg ;
	cvtColor(img_input,hsvImg,CV_BGR2HSV);
    vector <Mat> hsvSplit;
    split(hsvImg,hsvSplit);
	equalizeHist(hsvSplit[2],hsvSplit[2]);
	merge(hsvSplit,hsvImg);

	const int min_blue =100;
	const int max_blue =140;
	int avg_h = (min_blue+max_blue)/2;
	int channels = hsvImg.channels();
	int nRows = hsvImg.rows;
	//图像数据列需要考虑通道数的影响；
	int nCols = hsvImg.cols * channels;

	if (hsvImg.isContinuous())//连续存储的数据，按一行处理
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* p;

   for (i = 0; i < nRows; ++i)
	
     {
		p = hsvImg.ptr<uchar>(i);
		for (j = 0; j < nCols; j += 3)
		{
			int H = int(p[j]); //0-180
			int S = int(p[j + 1]);  //0-255
			int V = int(p[j + 2]);  //0-255
			bool colorMatched = false;

	       if (H > min_blue && H < max_blue)
			
		   {
			
                if ((S > 70&& S < 255) &&(V > 70 && V < 255))
					colorMatched = true;
			}

			if (colorMatched == true) 
			{
				p[j] = 0; p[j + 1] = 0; p[j + 2] = 255;
			   }
			else 
			{
				p[j] = 0; p[j + 1] = 0; p[j + 2] = 0;
			  }
		}
	}
    namedWindow("hsvImg", CV_WINDOW_NORMAL);
	Mat src_grey;
	Mat img_threshold;
	vector<Mat> hsvSplit_done;
	split(hsvImg, hsvSplit_done);
	src_grey = hsvSplit_done[2];
	vector <RotatedRect>  rects;
	Mat element = getStructuringElement(MORPH_RECT ,Size(17 ,3));  //闭形态学的结构元素
	morphologyEx(src_grey ,img_threshold,CV_MOP_CLOSE,element); 
	                                                                    //形态学处理
	namedWindow("hsvImg1", CV_WINDOW_NORMAL);
	imshow("hsvImg1",img_threshold);
	
	
	vector< vector <Point> > contours;//寻找车牌区域的轮廓
	findContours(img_threshold ,contours,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//只检测外轮廓
	//对候选的轮廓进行进一步筛选
	vector< vector <Point> > ::iterator itc = contours.begin();
	while( itc != contours.end())
	{
		RotatedRect mr = minAreaRect(Mat( *itc )); //返回每个轮廓的最小有界矩形区域
		if(!verifySizes_closeImg(mr))  //判断矩形轮廓是否符合要求
		{
			itc = contours.erase(itc);
		}
		else     
		{

			rects.push_back(mr);
			++itc;
		}      
	}

	vector <Mat> output_area;
	normalPosArea(img_input ,rects,output_area);  //获得144*33的候选车牌区域output_area


	CvSVM  svmClassifier;
	svm_train(svmClassifier);  //使用SVM对正负样本进行训练
	vector<Mat> plates_svm;   //需要把候选车牌区域output_area图像中每个像素点作为一行特征向量，后进行预测
	for(int i=0;i< output_area.size(); ++i)
	{
		Mat img = output_area[i];
		Mat p = img.reshape(1,1);
		p.convertTo(p,CV_32FC1);
		int response = (int)svmClassifier.predict( p );
		if (response == 1)
			plates_svm.push_back(output_area[i]);    //保存预测结果
	}
	

	if (plates_svm.size()!=1)
	{
		Mat grayImg;
		RgbConvToGray(img_input2 ,grayImg);
		medianBlur(grayImg,grayImg,5); 
		vector <RotatedRect>  rects_closeImg;  //车牌区域较为贴近
		posDetect(grayImg ,rects_closeImg);
		vector <Mat> output_area1;
		normalPosArea(img_input ,rects_closeImg,output_area1);  //获得144*33的候选车牌区域output_area

		for(int i=0;i< output_area1.size(); ++i)
		{
			Mat img = output_area1[i];
			Mat p = img.reshape(1,1);
			p.convertTo(p,CV_32FC1);
			int response = (int)svmClassifier.predict( p );
			if (response == 1)
			plates_svm.push_back(output_area1[i]);    //保存预测结果
		}
		
	
      
	}
	
	imshow("output",plates_svm[0]);
	imwrite("D:/vs/CarNumber/100.jpg",plates_svm[0]);
	Mat src_num=imread("D:/vs/CarNumber/100.jpg");
	imshow("src_car",src_num);
	cvtColor(src_num,src_num,CV_BGR2GRAY);
	imshow("cvt",src_num);
	threshold(src_num,src_num,190,255,CV_THRESH_BINARY);
	imshow("二值化图像",src_num);
	
	waitKey();

	return 0;
}


