#include <iostream>
#include "opencv2/opencv.hpp"
#include "windows.h"
#include "fstream"
using namespace std;
using namespace cv;
int main()
{
	vector<string> img_path;//�����ļ�������   
	vector<int> img_catg;    
	//int nLine = 0;    
	string buf;    
	unsigned long n;     
	//svm_data.close();//�ر��ļ�  
	string prefix="D:/vs/eam1/qiegenum/qiegenum/car/";

	for (int i=0;i<63;i++)
	{	
		string res;
		stringstream ss;
		ss<<i;
		ss>>res;
		for (int j=0;j<150;j++)
		{string res2;
		stringstream ss2;
		ss2<<j;
		ss2>>res2;
		img_path.push_back(prefix+res+"/"+res2+".bmp");
		}
	}


	for (int j=0;j<300;j++)
	{
		img_catg.push_back(j/20);
	}



	CvMat *data_mat, *res_mat;    
	int nImgNum = 9450; 
	data_mat = cvCreateMat( nImgNum, 2268, CV_32FC1 );  
	cvSetZero( data_mat );      
	res_mat = cvCreateMat( nImgNum, 1, CV_32FC1 );    
	cvSetZero( res_mat );    
	IplImage* src;    
	IplImage* trainImg=cvCreateImage(cvSize(40,32),8,3);

	cout<<"����HOG����"<<endl;  

	
	for( string::size_type i = 0; i != img_path.size(); i++ )    
	{    
		src=cvLoadImage(img_path[i].c_str(),1);    
		if( src == NULL )    
		{    
			cout<<" can not load the image: "<<img_path[i].c_str()<<endl;    
			continue;    
		}    

		cout<<" ���� "<<img_path[i].c_str()<<endl;    

		cvResize(src,trainImg);     
		HOGDescriptor *hog=new HOGDescriptor(cvSize(40,32),cvSize(8,8),cvSize(4,4),cvSize(4,4),9);      
		vector<float>descriptors;//��Ž��     
		hog->compute(trainImg, descriptors,Size(1,1), Size(0,0)); //Hog��������      
		cout<<"HOG dims: "<<descriptors.size()<<endl;        
		n=0;    
		for(vector<float>::iterator iter=descriptors.begin();iter!=descriptors.end();iter++)    
		{    
			cvmSet(data_mat,i,n,*iter);//�洢HOG���� 
			n++;    
		}       
		cvmSet( res_mat, i, 0, img_catg[i] );    
		cout<<" �������: "<<img_path[i].c_str()<<" "<<img_catg[i]<<endl;    
	}    
cout<<"ok"<<endl;

	CvSVM svm;//�½�һ��SVM      
	CvSVMParams param;//������SVMѵ����ز���  
	CvTermCriteria criteria;      
	criteria = cvTermCriteria( CV_TERMCRIT_EPS, 1000, FLT_EPSILON );      
	param = CvSVMParams( CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.09, 1.0, 10.0, 0.5, 1.0, NULL, criteria );          

	svm.train( data_mat, res_mat, NULL, NULL, param );//ѵ������     
	//����ѵ���õķ�����      
	svm.save( "d:\\HOG_SVM_DATA.xml" );  
	cout<<"ѵ���õķ�����"<<endl;
	

	//�������    
	IplImage *test;  
	char result[512]; 
	vector<string> img_tst_path;
	ifstream img_tst( "D:\\SVM_TEST.txt" );  //������ҪԤ���ͼƬ���ϣ�������һ�����ɵ�num.txt�ļ�����
	while( img_tst )  
	{  
		if( getline( img_tst, buf ) )  
		{  
			img_tst_path.push_back( buf );  
		}  
	}  
	img_tst.close(); 
cout<<"������� "<<endl;
	ofstream predict_txt( "D:\\SVM_PREDICT.txt" );
	cout<<"��Ԥ�����洢������ı���"<<endl;   
	for( string::size_type j = 0; j != img_tst_path.size(); j++ )    
	{    
		test = cvLoadImage( img_tst_path[j].c_str(), 1);    
		if( test == NULL )    
		{    
			cout<<" can not load the image: "<<img_tst_path[j].c_str()<<endl;    
			continue;    
		}
		IplImage* trainTempImg=cvCreateImage(cvSize(40,32),8,3);
		cvZero(trainTempImg);    
		cvResize(test,trainTempImg);    
		HOGDescriptor *hog=new HOGDescriptor(cvSize(40,32),cvSize(8,8),cvSize(4,4),cvSize(4,4),9);       
		vector<float>descriptors;//�������       
		hog->compute(trainTempImg, descriptors,Size(1,1), Size(0,0));     
		cout<<"HOG dims: "<<descriptors.size()<<endl;    
		CvMat* SVMtrainMat=cvCreateMat(1,descriptors.size(),CV_32FC1);    
		int n=0;    
		for(vector<float>::iterator iter=descriptors.begin();iter!=descriptors.end();iter++)    
		{    
			cvmSet(SVMtrainMat,0,n,*iter);    
			n++;    
		}    

		int ret = svm.predict(SVMtrainMat);//�����
		sprintf( result, "%s  %d\r\n",img_tst_path[j].c_str(),ret );
		predict_txt<<result;  //�����������ı�
	}
	cout<<"���α������еĴ����ͼƬ"<<endl;
	predict_txt.close();    
	while (1)
	{waitKey();
	}
	return 0;
}
