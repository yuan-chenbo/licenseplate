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


void RgbConvToGray(const Mat& inputImage,Mat & outpuImage); //rgb转为灰度

void posDetect(Mat &, vector <RotatedRect> &); //粗步选取候选车牌区域

bool verifySizes(const RotatedRect & );  //车牌区域需要满足的条件

void normalPosArea(Mat &intputImg, vector<RotatedRect> &rects_optimal, vector <Mat>& output_area );  //车牌裁剪，标准化为144*33

void svm_train(CvSVM & );  //取出SVM.xml中的特征矩阵和标签矩阵进行训练

bool verifySizes_closeImg(const RotatedRect & candidate);

void charRegion(const Mat &char_area,vector<Mat> &char_dst);