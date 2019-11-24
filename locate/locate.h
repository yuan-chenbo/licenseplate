#include "cv.h"  
#include "highgui.h"  
#include <ml.h>  
#include <iostream>  
#include <fstream>  

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/gpu/gpu.hpp>

#include <stdio.h>
#include<string>
#include<math.h>
using namespace cv;  
using namespace std;  


void RgbConvToGray(const Mat& inputImage,Mat & outpuImage); //rgbתΪ�Ҷ�

void posDetect(Mat &, vector <RotatedRect> &); //�ֲ�ѡȡ��ѡ��������

bool verifySizes(const RotatedRect & );  //����������Ҫ���������

void normalPosArea(Mat &intputImg, vector<RotatedRect> &rects_optimal, vector <Mat>& output_area );  //���Ʋü�����׼��Ϊ144*33

void svm_train(CvSVM & );  //ȡ��SVM.xml�е���������ͱ�ǩ�������ѵ��

bool verifySizes_closeImg(const RotatedRect & candidate);

void charRegion(const Mat &char_area,vector<Mat> &char_dst);