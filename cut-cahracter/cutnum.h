#include "cv.h"  
#include "highgui.h"  
#include <ml.h>  
#include <iostream>  
#include <fstream>  
#include <stdarg.h>

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include <stdio.h>
#include<string>
#include<math.h>
using namespace cv;  
using namespace std; 
  

int myCutOut(IplImage* src, IplImage* dst, int x, int y, int w, int h);  


	
	void RgbConvToGray(const Mat& inputImage,Mat & outpuImage); //rgb×ªÎª»Ò¶È