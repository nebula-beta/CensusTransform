#include "CensusTransform.h"


CensusTransform::CensusTransform(const cv::Mat& image, int h_size, int v_size)
{	
	h_size_ = h_size;
	v_size_ = v_size;
	rows_ = image.rows;
	cols_ = image.cols;


	cv::Mat gray_image;
	if(image.channels() == 1)
		gray_image = image;
	else
		cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

	Transform(gray_image);
}

void CensusTransform::Transform(const cv::Mat& image)
{
	int half_h_size = h_size_ / 2;
	int half_v_size = v_size_ / 2;

	// 每个像素所需要的bit数
	int each_elem_bit_size = h_size_ * v_size_ - 1;
	// 总共所需要的字节数
	int total_elem_bit_size = rows_ * cols_ * each_elem_bit_size;
	// 需要多少个uchar来存储这些bit
	int size = (total_elem_bit_size + sizeof(DATATYPE) - 1) / sizeof(DATATYPE);


	data_ = new DATATYPE[size];
	data_ptr_ = std::shared_ptr<DATATYPE>(data_);
	memset(data_, 0, size * sizeof(DATATYPE));

	for(int y = 0; y < rows_; ++y)
	{
		for(int x = 0; x < cols_; ++x)
		{
			int elem_pos = y * cols_ + x;
			int bit_pos_start = elem_pos * each_elem_bit_size;

			uchar centre_val = image.at<uchar>(y, x);
			int num = 0;
			for(int i = -half_v_size; i <= half_v_size; ++i)
			{
				for(int j = -half_h_size; j <= half_h_size; ++j)
				{
					if(i == 0 && j == 0)
						continue;

					int win_x = std::max(0, std::min(x + j, cols_));
					int win_y = std::max(0, std::min(y + i, rows_));


					uchar win_val = image.at<uchar>(win_y, win_x);

					int bit_pos = bit_pos_start + num++;
					SetBitVal(bit_pos, win_val > centre_val);
				}
			}
		}
	}
}




void CensusTransform::SetBitVal(int pos, int val)
{
	if(val == 0)
		return;

	DATATYPE& elem = data_[pos / sizeof(DATATYPE)];
	int num_shift = pos % sizeof(DATATYPE);
	elem |= (1 << num_shift);
}

int CensusTransform::GetBitVal(int pos)
{
	DATATYPE elem = data_[pos / sizeof(DATATYPE)];
	int num_shift = pos % sizeof(DATATYPE);
	elem = (elem >> num_shift);
	return elem & 1;

}

int CensusTransform::GetDistance(int y1, int x1, int y2, int x2)
{
	int half_h_size = h_size_ / 2;
	int half_v_size = v_size_ / 2;
	// 每个像素所需要的bit数
	int each_elem_bit_size = h_size_ * v_size_ - 1;

	int bit_pos_start1 = (y1 * cols_ + x1) * each_elem_bit_size;
	int bit_pos_start2 = (y2 * cols_ + x2) * each_elem_bit_size;

	int dis = 0;
	int num = 0;
	for(int i = -half_v_size; i <= half_v_size; ++i)
	{
		for(int j = -half_h_size; j <= half_h_size; ++j)
		{

			int win_x1 = std::max(0, std::min(x1 + j, cols_));
			int win_y1 = std::max(0, std::min(y1 + i, rows_));

			int win_x2 = std::max(0, std::min(x2 + j, cols_));
			int win_y2 = std::max(0, std::min(y2 + i, rows_));


			int bit_pos1 = bit_pos_start1 + num;
			int bit_pos2 = bit_pos_start2 + num;
			printf("%d %d\n", bit_pos1, bit_pos2);
			num ++;
			int bit_val1 = GetBitVal(bit_pos1);
			int bit_val2 = GetBitVal(bit_pos2);

			dis += bit_val1 != bit_val2;
		}
	}
	return dis;
}


int CensusTransform::GetDistance(int y1, int x1, CensusTransform& transform, int y2, int x2)
{
	int half_h_size = h_size_ / 2;
	int half_v_size = v_size_ / 2;
	// 每个像素所需要的bit数
	int each_elem_bit_size = h_size_ * v_size_ - 1;

	int bit_pos_start1 = (y1 * cols_ + x1) * each_elem_bit_size;
	int bit_pos_start2 = (y2 * cols_ + x2) * each_elem_bit_size;

	int dis = 0;
	int num = 0;
	for(int i = -half_v_size; i <= half_v_size; ++i)
	{
		for(int j = -half_h_size; j <= half_h_size; ++j)
		{

			int win_x1 = std::max(0, std::min(x1 + j, cols_));
			int win_y1 = std::max(0, std::min(y1 + i, rows_));

			int win_x2 = std::max(0, std::min(x2 + j, cols_));
			int win_y2 = std::max(0, std::min(y2 + i, rows_));


			int bit_pos1 = bit_pos_start1 + num;
			int bit_pos2 = bit_pos_start2 + num;
			num ++;
			int bit_val1 = GetBitVal(bit_pos1);
			int bit_val2 = transform.GetBitVal(bit_pos2);

			dis += bit_val1 != bit_val2;
		}
	}
	return dis;
}
