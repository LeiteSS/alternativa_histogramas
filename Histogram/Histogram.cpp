////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "opencv2/highgui/highgui.hpp"
#include "opencv2\opencv.hpp"
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

void myhist(Mat &med, Mat &asd)
{
	asd.setTo(0);//inicialize asd to zero
	float maxi = 0;//used in drawing the histogram(required while drawing)
				   //med.convert(med, CV_32FC1);//converted it float for easing the access
				   // Open the file in write mode.
	ofstream outputFile;
	outputFile.open("value.txt");

	float *y = asd.ptr<float>(0);//y points to row of med
	for (int i = 0; i<med.rows; i++)//run throught the cols
	{
		uchar *x = med.ptr<uchar>(i);//x points to row of med
		for (int j = 0; j<med.cols; j++)//run throught the cols
		{
			for (int k = 0; k<256; k++)//255 intresities
			{
				if (k == (int)x[j])//check if med.at(K) is equal to kth intensity level
				{
					y[k] += 1;//if true increment the count at kth position in asd
					if (maxi < y[k])//to calculate maximum in the array(it is calculated only when array val
					{
						maxi = y[k];
						
					}
					break;
				}
				Scalar &pixel = &med.at<Scalar>(i, j);
				outputFile << pixel << '\t';
			}
			outputFile << endl;
		}
	}
	y[256] = maxi;//maximum will be storeed in the max in 257th location of array
}
void drawHist(Mat &asd, char *winName)
{
	Mat histFig(640, 512, CV_8U);//256*2 = 512, so thickness of line drawn will be 2
	histFig.setTo(255);//white background image
	namedWindow(winName);

	//to print maximum available in asd
	cout << "max in asd[256] for" << asd.at<float>(256) << endl;

	float  *y = asd.ptr<float>(0);//y points to row of asd
	float max = y[256];//get the max value present in asd(256)
	float factor = ((float)640 / max);//amounts to 1 pixel height

	for (int i = 0; i<256; i++)
	{
		y[i] *= factor;
		Point pt1, pt2;
		pt1 = Point(2 * i, 648);
		pt2 = Point(2 * i, 640 - (int)y[i]);
		const Scalar col = Scalar(0, 0, 0);
		line(histFig, pt1, pt2, col, 2, CV_AA);
	}
	imshow(winName, histFig);
}

int main(int argc, const char** argv)

{

	Mat img = imread("C:\\mel01.jpg", CV_LOAD_IMAGE_COLOR); //open and read the image


	if (img.empty())
	{
		cout << "Image cannot be loaded..!!" << endl;
		return -1;
	}

	namedWindow("input", CV_WINDOW_AUTOSIZE);
	imshow("input", img);

	Mat asd(1, 257, CV_32F); //element at 257 is the maximum element
	myhist(img, asd);//calculates the histogram 

	//drawHist(asd, "Without Blurred"); //draws the histogram

	Mat blurred;
	medianBlur(img, blurred, 5);
	namedWindow("BlurredInput", 1);
	imshow("BlurredInput", blurred);
	myhist(blurred, asd);//calculates the histogram

	//drawHist(asd, "With Blurred");//draws the histogram*/
	
	waitKey(0); //wait for key press

	destroyAllWindows(); //destroy all open windows

	return 0;
}

