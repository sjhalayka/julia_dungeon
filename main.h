

#ifndef MAIN_H
#define MAIN_H


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <thread>
using namespace std;

#include <glm/glm.hpp>

#include <opencv2/opencv.hpp>
using namespace cv;


#ifdef _MSC_VER
#pragma comment(lib, "opencv_world4100")
#endif






std::vector<cv::Mat> splitImage(cv::Mat& image, int M, int N)
{
	// All images should be the same size ...
	int width = image.cols / M;
	int height = image.rows / N;
	// ... except for the Mth column and the Nth row
	int width_last_column = width + (image.cols % width);
	int height_last_row = height + (image.rows % height);

	std::vector<cv::Mat> result;

	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			// Compute the region to crop from
			cv::Rect roi(width * j,
				height * i,
				(j == (M - 1)) ? width_last_column : width,
				(i == (N - 1)) ? height_last_row : height);

			result.push_back(image(roi));
		}
	}

	return result;
}


cv::Mat imageCollage(std::vector<cv::Mat>& array_of_images, int M, int N)
{
	// All images should be the same size
	const cv::Size images_size = array_of_images[0].size();
	// Create a black canvas
	cv::Mat image_collage(images_size.height * N, images_size.width * M, array_of_images[0].type(), cv::Scalar(0, 0, 0));

	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			if (((i * M) + j) >= array_of_images.size())
				break;

			cv::Rect roi(images_size.width * j, images_size.height * i, images_size.width, images_size.height);
			array_of_images[(i * M) + j].copyTo(image_collage(roi));
		}
	}

	return image_collage;
}


float iterate_2d(vector< complex<float> >& trajectory_points,
	complex<float> Z,
	const complex<float> C,
	const short unsigned int max_iterations,
	const float threshold)
{
	trajectory_points.clear();
	trajectory_points.push_back(Z);

	for (short unsigned int i = 0; i < max_iterations; i++)
	{
		Z = Z * Z + C;

		trajectory_points.push_back(Z);

		if (abs(Z) >= threshold)
			break;
	}

	return abs(Z);
}

#endif

