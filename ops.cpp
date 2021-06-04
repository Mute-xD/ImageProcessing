#include "pch.h"
#include "framework.h"
#include "ops.h"

unsigned char *rotate(unsigned char* data, int height, int width, float degree, int& outHeight, int& outWidth)
{
	float beta;
	beta = degree * 3.1415926 / 180;
	int originx, originy;
	originx = height / 2;//新的图像中心_1
	originy = width / 2;//新的图像中心_1

	outHeight = height * fabs(cos(beta)) + width * fabs(sin(beta));
	outWidth = height * fabs(sin(beta)) + width * fabs(cos(beta));
	unsigned char* output;
	output = (unsigned char*)malloc(sizeof(unsigned char) * outHeight * outWidth);  //分配新的存储空间M*N
	float zxx, zxy;
	zxx = outHeight / 2;
	zxy = outWidth / 2;//新图像中心_2

	double x_2, y_2;
	float x_1, y_1;
	float x_3, y_3;
	int x_4, y_4;
	for (int i = 0; i < outHeight; i++)
		for (int j = 0; j < outWidth; j++)
		{
			x_1 = i - zxx;
			y_1 = j - zxy;

			x_2 = x_1 * cos(beta) - y_1 * sin(beta);
			y_2 = x_1 * sin(beta) + y_1 * cos(beta);

			x_3 = x_2 + originx;
			y_3 = y_2 + originy;

			x_4 = int(x_3 + 0.5);
			y_4 = int(y_3 + 0.5);

			if ((x_4 >= height) || (x_4 < 0) || (y_4 >= width) || (y_4 < 0))
			{
				x_4 = 0;
				y_4 = 0;
			}
			*(output + i * outWidth + j) = *(data + x_4 * width + y_4);
		}
	return output;
}

float *calHistogram(unsigned char* data, int height, int width)
{
	float histogram[256] = {0};
	unsigned char pix = 0;
	for (int i = 0; i < height; i ++)
	{
		for (int j = 0; j < width; j++)
		{
			pix =  *(data + i * width + j);
			histogram[pix] += 1;
		}
	}
	return histogram;
}

unsigned char* histogramNorm(unsigned char* data, int height, int width)
{
	int pixCount = height * width;
	unsigned char* output;
	output = (unsigned char*)malloc(sizeof(unsigned char) * height * width);

	float* histogram = calHistogram(data, height, width);
	int sum = 0;
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = histogram[i] / pixCount;
	}
	for (int i = 1; i < 256; i++)
	{
		histogram[i] += histogram[i - 1];
	}
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = round(histogram[i] * 255);
	}
	for (int i = 0; i < 256; i++)
	{
		for (int h = 0; h < height; h ++)
		{
			for (int w = 0; w < width; w++)
			{
				if (*(data + h * width + w) == i)
				{
					*(output + h * width + w) = histogram[i];
				}
			}
		}
	}
	return output;
}

unsigned char* conv2d(unsigned char* data, int height, int width, unsigned char* kernel, int kernelSize)
{
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			;
		}
	}

	return nullptr;
}
unsigned char* padding(unsigned char* data, int height, int width, int paddingSize, int &outHeight, int &outWidth)
{
	outHeight = height + paddingSize * 2;
	outWidth = width + paddingSize * 2;
	unsigned char* output;
	output = (unsigned char*)malloc(sizeof(unsigned char) * outHeight * outWidth);
	for (int h = 0; h < outHeight; h++)
	{
		for (int w = 0; w < outWidth; w++)
		{
			*(output + h * outWidth + w) = 0;
		}
	}
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			*(output + h+ paddingSize * outWidth + w + paddingSize) = *(output + h * width + w);
		}
	}
	return output;
}