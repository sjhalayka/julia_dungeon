#include "main.h"


int main(void)
{
	const int background_tile_size = 36;
	const int x_tiles = 53;
	const int y_tiles = 29;

	// Load from file
	Mat floor_tile = imread("floor_tile.png", IMREAD_UNCHANGED);

	if (floor_tile.empty() || floor_tile.channels() != 4)
	{
		cout << "floor_tile.png must be a 32-bit PNG" << endl;
		return -1;
	}

	Mat floor_empty = imread("floor_empty.png", IMREAD_UNCHANGED);

	if (floor_empty.empty() || floor_empty.channels() != 4)
	{
		cout << "floor_empty.png must be a 32-bit PNG" << endl;
		return -1;
	}

	Mat light_blocking_on = imread("light_blocking_on.png", IMREAD_UNCHANGED);

	if (light_blocking_on.empty() || light_blocking_on.channels() != 4)
	{
		cout << "light_blocking_on.png must be a 32-bit PNG" << endl;
		return -1;
	}

	Mat light_blocking_off = imread("light_blocking_off.png", IMREAD_UNCHANGED);

	if (light_blocking_off.empty() || light_blocking_off.channels() != 4)
	{
		cout << "light_blocking_off.png must be a 32-bit PNG" << endl;
		return -1;
	}

	Mat input_mat(Size(background_tile_size, background_tile_size), CV_32FC4);
	input_mat = Scalar(0, 0, 0, 1);

	std::vector<cv::Mat> array_of_images(x_tiles * y_tiles, input_mat);
	std::vector<cv::Mat> array_of_light_blockers(x_tiles * y_tiles, input_mat);








	const float x_grid_max = 5.3/2.0;
	const float x_grid_min = -x_grid_max;
	const size_t x_res = 53;
	const complex<float> x_step_size((x_grid_max - x_grid_min) / (x_res - 1), 0);

	const float y_grid_max = 2.9/2.0;
	const float y_grid_min = -y_grid_max;
	const size_t y_res = 29;
	const complex<float> y_step_size(0, (y_grid_max - y_grid_min) / (y_res - 1));

	const complex<float> C(-0.3f, -0.65f);
	const unsigned short int max_iterations = 8;
	const float threshold = 4.0f;

	complex<float> Z(x_grid_min, y_grid_min);

	vector< complex<float> > trajectory_points;


	for (size_t y = 0; y < y_res; y++, Z += y_step_size, Z.real(x_grid_min))
	{
		for (size_t x = 0; x < x_res; x++, Z += x_step_size)
		{
			const size_t array_of_images_index = y * x_res + x;

			float magnitude = iterate_2d(trajectory_points, Z, C, max_iterations, threshold);

			if (magnitude < threshold)
				array_of_images[array_of_images_index] = floor_tile;
			else
				array_of_images[array_of_images_index] = floor_empty;

			if (magnitude < threshold)
				array_of_light_blockers[array_of_images_index] = light_blocking_off;
			else
				array_of_light_blockers[array_of_images_index] = light_blocking_on;


			if (magnitude < threshold)
				cout << " *";
			else
				cout << "  ";

		}

		cout << endl;
	}

	cout << endl;








	cv::Mat image_collage = imageCollage(array_of_images, x_tiles, y_tiles);

	imwrite("out.png", image_collage * 255.0);

	cv::Mat light_blocking_collage = imageCollage(array_of_light_blockers, x_tiles, y_tiles);

	imwrite("light_blocker.png", light_blocking_collage * 255.0);











	return 0;
}