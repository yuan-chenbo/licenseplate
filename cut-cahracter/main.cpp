#include"cutnum.h"

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


int myCutOut(IplImage* src, IplImage* dst, int x, int y, int w, int h)    
{    
	//x,y为矩形框左上角点坐标,w为宽度，h为高度    
	int width_src = src->widthStep;    
	int height_src = src->height;    
	unsigned char* gray_src = (unsigned char*)src -> imageData;    

	int width_dst = dst ->widthStep;    
	int height_dst = dst ->height;    
	unsigned char* gray_dst = (unsigned char*)dst -> imageData;    

	for (int i = 0; i < height_dst; i++)    
	{    
		for (int j = 0; j < width_dst; j++)    
		{    
			gray_dst[i * width_dst + j] = gray_src[(y + i) * width_src + x + j];    
		}    
	}    
	return 0;    
}  


