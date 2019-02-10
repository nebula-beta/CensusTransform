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



void NCC(const cv::Mat& left_image, const cv::Mat& right_image, int min_disparity, int max_disparity, int win_size)
{
	int rows = left_image.rows;
	int cols = left_image.cols;

	int half_win_size = win_size / 2;
	cv::Mat disparity(rows, cols, CV_8UC1);


	CensusTransform left_census_transform(left_image, 3, 3); 
	CensusTransform right_census_transform(right_image, 3, 3); 

	for(int y = 0; y < rows; ++y)
	{
		for(int x = 0; x < cols; ++x)
		{
			float min_cost = 1 << 30;
			int dis = 0;
			for(int d = min_disparity; d <= max_disparity; ++d)
			{

				if(x - d < 0)
					continue;

				float cost = 0;
				int count = 0;
				for(int i = -half_win_size; i <= half_win_size; ++i)
				{
					for(int j = -half_win_size; j <= half_win_size; ++j)
					{
						int win_x = x + j;
						int win_y = y + i;
						int corr_win_x = win_x - d;

						if(win_x < 0 || win_y < 0 || win_x >= cols || win_y >= rows || corr_win_x < 0 || corr_win_x >= cols)
							continue;

						//cost +=  cv::norm((cv::Vec3f)left_image.at<cv::Vec3b>(win_y, win_x) -(cv::Vec3f) right_image.at<cv::Vec3b>(win_y, corr_win_x), cv::NORM_L1);
						cost += left_census_transform.GetDistance(win_y, win_x, right_census_transform, win_y, corr_win_x);
						count += 1;
					}
				}
				cost = cost / count;
				if(cost < min_cost)
				{
					min_cost = cost;
					disparity.at<uchar>(y, x) = d * 3;
					dis = d * 3;
				}
			}
		}
	}
	cv::imshow("disparity", disparity);
	cv::waitKey(0);
}
int main(int argc, char* argv[])
{
    /* freopen("in.txt","r",stdin); */
    /* freopen("out.txt","w",stdout); */
    
	cv::Mat left_image = cv::imread(argv[1]);
	cv::Mat right_image = cv::imread(argv[2]);
	
	NCC(left_image, right_image, 1, 80, 11);

	return 0;
}

