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
	const float aspect = 44/14; //�����
	int min = 20*aspect*20; //��С����
	int max = 180*aspect*180;  //�������
	float rmin = aspect - aspect*error; //�����������С�����
	float rmax = aspect + aspect*error; //�����������󳤿��

	int area = candidate.size.height * candidate.size.width;
	float r = (float)candidate.size.width/(float)candidate.size.height;
	if(r <1)
		r = 1/r;

	if( (area < min || area > max) || (r< rmin || r > rmax)  )
		return false;
	else
		return true;
}

void posDetect(Mat &inputImage , vector <RotatedRect> & rects  )   //�����ҵ���ѡ���� rects
{
	Mat img_sobel;
	Sobel(inputImage , img_sobel , CV_8U, 1,0,3,1,0);
	Mat img_threshold;
	threshold(img_sobel , img_threshold,0,255 , CV_THRESH_OTSU+CV_THRESH_BINARY); //otsu�㷨�Զ������ֵ
	Mat element = getStructuringElement(MORPH_RECT ,Size(17 ,3));  //����̬ѧ�ĽṹԪ��
	morphologyEx(img_threshold ,img_threshold,CV_MOP_CLOSE,element); 
	morphologyEx(img_threshold ,img_threshold,MORPH_OPEN,element);
	//namedWindow("img",CV_WINDOW_NORMAL);
	//imshow("img",img_threshold);
	//waitKey();
	//Ѱ�ҳ������������
	vector< vector <Point> > contours;
	findContours(img_threshold ,contours,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//ֻ���������
	//�Ժ�ѡ���������н�һ��ɸѡ
	vector< vector <Point> > ::iterator itc = contours.begin();

	while( itc != contours.end())
	{
		RotatedRect mr = minAreaRect(Mat( *itc )); //����ÿ����������С�н��������
		if(!verifySizes(mr))  //�жϾ��������Ƿ����Ҫ��
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
	const float aspect = 44/14; //�����
	int min = 20*aspect*20; //��С����
	int max = 180*aspect*180;  //�������
	float rmin = aspect - 2*aspect*error; //�����������С�����
	float rmax = aspect + 2*aspect*error; //�����������󳤿��

	int area = candidate.size.height * candidate.size.width;
	float r = (float)candidate.size.width/(float)candidate.size.height;
	if(r <1)
		r = 1/r;

	if( (area < min || area > max) || (r< rmin || r > rmax)  ) //�������������Ϊ��candidateΪ��������
		return false;
	else
		return true;
}


void normalPosArea(Mat &intputImg, vector<RotatedRect> &rects_optimal, vector <Mat>& output_area )
{
	float r,angle;
	for (int i = 0 ;i<rects_optimal.size(); ++i)
	{
		//��ת����
		angle = rects_optimal[i].angle;
		r = (float)rects_optimal[i].size.width / (float) (float)rects_optimal[i].size.height;
		if(r<1)
			angle = 90 + angle;//��תͼ��ʹ��õ������ڸ߶�ͼ��
		Mat rotmat = getRotationMatrix2D(rects_optimal[i].center , angle,1);//��ñ��ξ������
		Mat img_rotated;
		warpAffine(intputImg ,img_rotated,rotmat, intputImg.size(),CV_INTER_CUBIC);
		//imshow("img_rotated",img_rotated);
		//waitKey();
		//�ü�ͼ��
		Size rect_size = rects_optimal[i].size;
		if(r<1)
			swap(rect_size.width, rect_size.height); //�����ߺͿ�
		Mat  img_crop;
		getRectSubPix(img_rotated ,rect_size,rects_optimal[i].center , img_crop );

		//�ù���ֱ��ͼ�������вü��õ���ͼ��ʹ������ͬ��Ⱥ͸߶ȣ�������ѵ���ͷ���
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

	svmClassifier.train(SVM_TrainningData,SVM_Classes ,Mat(),Mat(),SVM_params); //SVMѵ��ģ��
	fs.release();
	
}

