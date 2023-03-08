// STL includes
#include <iostream>
#include <filesystem>

// OpenCV includes
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/hal/interface.h>

struct Shape
{
	int rows;
	int cols;
	int chans;
};

// Read an image from a path and return it as a cv::Mat
cv::Mat readImage(const std::filesystem::path& imagePath)
{
	return cv::imread(imagePath.string());
}

// Convert an image to the CIELAB color space 
void rgbToCIELAB(cv::Mat& image)
{
	cv::cvtColor(image, image, cv::COLOR_BGR2Lab);
}

// Convert an image from the CIELAB color space
void cielabToRGB(cv::Mat& image)
{
    cv::cvtColor(image, image, cv::COLOR_Lab2BGR);
}

// Kmeans color segmentation
//cv::Mat kmeansSegmentation(cv::Mat& image, int clusters = 8, int rounds = 1)
//{
//	Shape shape{ image.rows, image.cols, image.channels() };
//	cv::Mat samples{cv::Mat::zeros(shape.rows, shape.cols, CV_32F)}, labels, centers;
//	int count{ 0 };
//
//	for (int x{ 0 }; x < shape.rows; x++)
//	{
//		for (int y{ 0 }; y < shape.cols; y++ )
//		{
//			samples.at<int>(count) = image.at<int>(x, y);
//			count++;
//		}
//	}
//	double compactness{ cv::kmeans(samples, clusters, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 10000, 0.0001), rounds, cv::KMEANS_PP_CENTERS, centers) };
//
//	cv::Mat clusteredImage{ image.size(), image.type() };
//	for (int row = 0; row != image.rows; ++row) {
//		auto clusteredImageBegin = clusteredImage.ptr<uchar>(row);
//		auto clusteredImageEnd = clusteredImageBegin + clusteredImage.cols * 3;
//		auto labels_ptr = labels.ptr<int>(row * image.cols);
//
//		while (clusteredImageBegin != clusteredImageEnd) {
//			int const cluster_idx = *labels_ptr;
//			auto centers_ptr = centers.ptr<float>(cluster_idx);
//			clusteredImageBegin[0] = centers_ptr[0];
//			clusteredImageBegin[1] = centers_ptr[1];
//			clusteredImageBegin[2] = centers_ptr[2];
//			clusteredImageBegin += 3; ++labels_ptr;
//		}
//	}
//
//	return clusteredImage;
//}

cv::Mat imageQuantization(cv::Mat inputImage, int numberOfClusters = 2, int iterations = 1) {

    //step 1 : map the src to the samples
    cv::Mat samples(inputImage.total(), 3, CV_32F);
    auto samples_ptr = samples.ptr<float>(0);
    for (int row = 0; row != inputImage.rows; ++row) {
        auto src_begin = inputImage.ptr<uchar>(row);
        auto src_end = src_begin + inputImage.cols * inputImage.channels();
        //auto samples_ptr = samples.ptr<float>(row * src.cols);
        while (src_begin != src_end) {
            samples_ptr[0] = src_begin[0];
            samples_ptr[1] = src_begin[1];
            samples_ptr[2] = src_begin[2];
            samples_ptr += 3; src_begin += 3;
        }
    }

    //step 2 : apply kmeans to find labels and centers
    int clusterCount = numberOfClusters; //Number of clusters to split the set by
    cv::Mat labels;
    int attempts = iterations; //Number of times the algorithm is executed using different initial labels
    cv::Mat centers;
    int flags = cv::KMEANS_PP_CENTERS;
    cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
        10, 0.01);

    //the call to kmeans:
    cv::kmeans(samples, clusterCount, labels, criteria, attempts, flags, centers);

    //step 3 : map the centers to the output
    cv::Mat clusteredImage(inputImage.size(), inputImage.type());
    for (int row = 0; row != inputImage.rows; ++row) {
        auto clusteredImageBegin = clusteredImage.ptr<uchar>(row);
        auto clusteredImageEnd = clusteredImageBegin + clusteredImage.cols * 3;
        auto labels_ptr = labels.ptr<int>(row * inputImage.cols);

        while (clusteredImageBegin != clusteredImageEnd) {
            int const cluster_idx = *labels_ptr;
            auto centers_ptr = centers.ptr<float>(cluster_idx);
            clusteredImageBegin[0] = centers_ptr[0];
            clusteredImageBegin[1] = centers_ptr[1];
            clusteredImageBegin[2] = centers_ptr[2];
            clusteredImageBegin += 3; ++labels_ptr;
        }
    }

    //return the output:
    return clusteredImage;
}

int main(int argc, char** argv) {
	std::filesystem::path path{ "bear.jpeg" };
	cv::Mat image = readImage(path);
	cv::imshow("RGB", image);
	rgbToCIELAB(image);
	cv::imshow("CIELAB", image);
	image = imageQuantization(image);
	cv::imshow("CLUSTERED", image);
    cielabToRGB(image);
    cv::imshow("FINAL", image);
	int k = cv::waitKey(0); // Wait for a keystroke in the window
		
	return EXIT_SUCCESS;
}