
#include "opencv2/opencv.hpp"
#include "windows.h"
#include "fstream"
using namespace std;
using namespace cv;
int main()
{

	CvSVM svm ;
	svm.load("d:\\HOG_SVM_DATA.xml");//����ѵ���õ�xml�ļ�������ѵ������10K����д����
	//�������    
	IplImage *test;  
	char result[2268]; //���Ԥ���� 
	test = cvLoadImage("D:/vs/eam1/qiegenum/qiegenum/car/8/50.bmp", 1); //���Լ�����ڻ�ͼ����дһ������
	if (!test)
	{
		MessageBox(NULL,TEXT("��Ԥ��ͼ�񲻴��ڣ�"),TEXT("��ʾ"),MB_ICONWARNING);
		return -1;
	}
	IplImage* trainTempImg=cvCreateImage(cvSize(40,32),8,3);
	cvZero(trainTempImg);    
	cvResize(test,trainTempImg);     
	HOGDescriptor *hog=new HOGDescriptor(cvSize(40,32),cvSize(8,8),cvSize(4,4),cvSize(4,4),9);      
	vector<float>descriptors;//��Ž��       
	hog->compute(trainTempImg, descriptors,Size(1,1), Size(0,0)); //Hog��������      
	cout<<"HOG dims: "<<descriptors.size()<<endl;  //��ӡHog����ά��  ��������324
	
	CvMat* SVMtrainMat=cvCreateMat(1,descriptors.size(),CV_32FC1);   
	int n=0;    
	for(vector<float>::iterator iter=descriptors.begin();iter!=descriptors.end();iter++)    
	{    
		cvmSet(SVMtrainMat,0,n,*iter);    
		n++;    
	}   

	int ret = svm.predict(SVMtrainMat);//�����


	cout<<ret<<endl;
	cvNamedWindow("dst",1);
	cvShowImage("dst",test);
	while (1)
	{waitKey();
	}
	
	return 0;
}
