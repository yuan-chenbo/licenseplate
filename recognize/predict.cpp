
#include "opencv2/opencv.hpp"
#include "windows.h"
#include "fstream"
using namespace std;
using namespace cv;
int main()
{

	CvSVM svm ;
	svm.load("d:\\HOG_SVM_DATA.xml");//加载训练好的xml文件，这里训练的是10K个手写数字
	//检测样本    
	IplImage *test;  
	char result[2268]; //存放预测结果 
	test = cvLoadImage("D:/vs/eam1/qiegenum/qiegenum/car/8/50.bmp", 1); //你自己随便在绘图板里写一个程序
	if (!test)
	{
		MessageBox(NULL,TEXT("待预测图像不存在！"),TEXT("提示"),MB_ICONWARNING);
		return -1;
	}
	IplImage* trainTempImg=cvCreateImage(cvSize(40,32),8,3);
	cvZero(trainTempImg);    
	cvResize(test,trainTempImg);     
	HOGDescriptor *hog=new HOGDescriptor(cvSize(40,32),cvSize(8,8),cvSize(4,4),cvSize(4,4),9);      
	vector<float>descriptors;//存放结果       
	hog->compute(trainTempImg, descriptors,Size(1,1), Size(0,0)); //Hog特征计算      
	cout<<"HOG dims: "<<descriptors.size()<<endl;  //打印Hog特征维数  ，这里是324
	
	CvMat* SVMtrainMat=cvCreateMat(1,descriptors.size(),CV_32FC1);   
	int n=0;    
	for(vector<float>::iterator iter=descriptors.begin();iter!=descriptors.end();iter++)    
	{    
		cvmSet(SVMtrainMat,0,n,*iter);    
		n++;    
	}   

	int ret = svm.predict(SVMtrainMat);//检测结果


	cout<<ret<<endl;
	cvNamedWindow("dst",1);
	cvShowImage("dst",test);
	while (1)
	{waitKey();
	}
	
	return 0;
}
