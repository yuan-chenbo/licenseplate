#include "carID_Detection.h"

void RgbConvToGray(const Mat& inputImage,Mat & outpuImage)  //g = 0.3R+0.59G+0.11B
{
	outpuImage = Mat(inputImage.rows ,inputImage.cols ,CV_8UC1);  

	for (int i = 0 ;i<inputImage.rows ;++ i)
	{
		uchar *ptrGray = outpuImage.ptr<uchar>(i); 
		const Vec3b * ptrRgb = inputImage.ptr<Vec3b>(i);
		for (int j = 0 ;j<inputImage.cols ;++ j)
		{
			ptrGray[j] = 0.3*ptrRgb[j][2]+0.59*ptrRgb[j][1]+0.11*ptrRgb[j][0];	
		}
	}
}

bool verifySizes_closeImg(const RotatedRect & candidate)
{
	float error = 0.4;
	const float aspect = 44/14; //长宽比
	int min = 20*aspect*20; //最小区域
	int max = 180*aspect*180;  //最大区域
	float rmin = aspect - aspect*error; //考虑误差后的最小长宽比
	float rmax = aspect + aspect*error; //考虑误差后的最大长宽比

	int area = candidate.size.height * candidate.size.width;
	float r = (float)candidate.size.width/(float)candidate.size.height;
	if(r <1)
		r = 1/r;

	if( (area < min || area > max) || (r< rmin || r > rmax)  )
		return false;
	else
		return true;
}

void posDetect(Mat &inputImage , vector <RotatedRect> & rects  )   //初步找到候选区域 rects
{
	Mat img_sobel;
	Sobel(inputImage , img_sobel , CV_8U, 1,0,3,1,0);
	Mat img_threshold;
	threshold(img_sobel , img_threshold,0,255 , CV_THRESH_OTSU+CV_THRESH_BINARY); //otsu算法自动获得阈值
	Mat element = getStructuringElement(MORPH_RECT ,Size(17 ,3));  //闭形态学的结构元素
	morphologyEx(img_threshold ,img_threshold,CV_MOP_CLOSE,element); 
	morphologyEx(img_threshold ,img_threshold,MORPH_OPEN,element);
	//namedWindow("img",CV_WINDOW_NORMAL);
	//imshow("img",img_threshold);
	//waitKey();
	//寻找车牌区域的轮廓
	vector< vector <Point> > contours;
	findContours(img_threshold ,contours,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//只检测外轮廓
	//对候选的轮廓进行进一步筛选
	vector< vector <Point> > ::iterator itc = contours.begin();

	while( itc != contours.end())
	{
		RotatedRect mr = minAreaRect(Mat( *itc )); //返回每个轮廓的最小有界矩形区域
		if(!verifySizes(mr))  //判断矩形轮廓是否符合要求
		{
			itc = contours.erase(itc);
		}
		else     
		{
			rects.push_back(mr);
			++itc;
		}      
	}
}


bool verifySizes(const RotatedRect & candidate)
{
	float error = 0.4;
	const float aspect = 44/14; //长宽比
	int min = 20*aspect*20; //最小区域
	int max = 180*aspect*180;  //最大区域
	float rmin = aspect - 2*aspect*error; //考虑误差后的最小长宽比
	float rmax = aspect + 2*aspect*error; //考虑误差后的最大长宽比

	int area = candidate.size.height * candidate.size.width;
	float r = (float)candidate.size.width/(float)candidate.size.height;
	if(r <1)
		r = 1/r;

	if( (area < min || area > max) || (r< rmin || r > rmax)  ) //满足该条件才认为该candidate为车牌区域
		return false;
	else
		return true;
}


void normalPosArea(Mat &intputImg, vector<RotatedRect> &rects_optimal, vector <Mat>& output_area )
{
	float r,angle;
	for (int i = 0 ;i<rects_optimal.size(); ++i)
	{
		//旋转区域
		angle = rects_optimal[i].angle;
		r = (float)rects_optimal[i].size.width / (float) (float)rects_optimal[i].size.height;
		if(r<1)
			angle = 90 + angle;//旋转图像使其得到长大于高度图像。
		Mat rotmat = getRotationMatrix2D(rects_optimal[i].center , angle,1);//获得变形矩阵对象
		Mat img_rotated;
		warpAffine(intputImg ,img_rotated,rotmat, intputImg.size(),CV_INTER_CUBIC);
		//imshow("img_rotated",img_rotated);
		//waitKey();
		//裁剪图像
		Size rect_size = rects_optimal[i].size;
		if(r<1)
			swap(rect_size.width, rect_size.height); //交换高和宽
		Mat  img_crop;
		getRectSubPix(img_rotated ,rect_size,rects_optimal[i].center , img_crop );

		//用光照直方图调整所有裁剪得到的图像，使具有相同宽度和高度，适用于训练和分类
		Mat resultResized;
		resultResized.create(33,144,CV_8UC3);
		resize(img_crop , resultResized,resultResized.size() , 0,0,INTER_CUBIC);
		Mat grayResult;
		RgbConvToGray(resultResized ,grayResult);
		//blur(grayResult ,grayResult,Size(3,3));
		equalizeHist(grayResult,grayResult);

		output_area.push_back(grayResult);
	}
}

void svm_train(CvSVM & svmClassifier)
{
	FileStorage fs;

	fs.open("SVM.xml" , FileStorage::READ);
	Mat SVM_TrainningData;
	Mat SVM_Classes;	

	fs["TrainingData"] >>SVM_TrainningData;
	fs["classes"] >>SVM_Classes;
	CvSVMParams SVM_params;
	SVM_params.kernel_type = CvSVM::LINEAR;

	svmClassifier.train(SVM_TrainningData,SVM_Classes ,Mat(),Mat(),SVM_params); //SVM训练模型
	fs.release();
	
}

