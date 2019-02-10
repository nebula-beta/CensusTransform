#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <opencv2/opencv.hpp>
#include "CensusTransform.h"
using namespace std;

int datas[6][5] = 
{
	//(1, 2)
	//(4, 2)
	{0, 0, 0, 0, 0},
	{0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0},
	{0, 1, 1, 1, 0},
	{0, 1, 0, 1, 0},
	{0, 1, 1, 1, 0},
};

int main()
{
	cv::Mat image1 = cv::Mat::ones(6, 5, CV_8UC1);
	cv::Mat image2 = cv::Mat::ones(6, 5, CV_8UC1);
	for(int i = 0; i < 5; ++i)
	{
		for(int j = 0; j < 5; ++j)
		{
			image2.at<uchar>(i, j) = image1.at<uchar>(i, j) = datas[i][j];
		}
	}


	CensusTransform census_transform1(image1, 3, 3); 
	CensusTransform census_transform2(image2, 3, 3); 

	int dis = census_transform1.GetDistance(1, 2, census_transform2, 4, 2);
	std::cout << dis << std::endl;
	return 0;
}

