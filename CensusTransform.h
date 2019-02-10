#ifndef __CENSUS_TRANSFORM_H__
#define __CENSUS_TRANSFORM_H__

#include <opencv2/opencv.hpp>

#define DATATYPE uchar
/* #define DATATYPE unsigned int */

// 关键是窗口的大小， 以及如果存储得到的编码

class CensusTransform
{
public:
	CensusTransform() {}
	CensusTransform(const cv::Mat& image, int h_size, int v_size);
	int GetDistance(int y1, int x1, int y2, int x2);
	int GetDistance(int y1, int x1, CensusTransform& transform, int y2, int x2);
private:

	void Transform(const cv::Mat& image);
	void SetBitVal(int pos, int val);
	int GetBitVal(int pos);


	int h_size_;
	int v_size_;
	int rows_;
	int cols_;
	DATATYPE* data_;
		
};

#endif //__CENSUS_TRANSFORM_H__
