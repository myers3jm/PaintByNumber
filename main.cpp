#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	if (argc > 1)
	{
		cv::Mat image = cv::imread(argv[1]);
		cv::imshow(image)
	}
	else
	{
		std::cerr << "PaintByNumberException: Insufficient arguments supplied.\nUsage is \"pbn <path to image>\"";
		return EXIT_FAILURE;
	}
		
	return EXIT_SUCCESS;
}