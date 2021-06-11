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
		histogram[i] = round(histogram[i] * 255.);
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

unsigned char* conv2d(unsigned char* data, int height, int width, float* kernel, int kernelSize)
{
	unsigned char* padded= nullptr;
	int paddedHeight; int paddedWidth;
	padded = padding(data, height, width, 1, paddedHeight, paddedWidth);
	float temp; float pix; float knl;
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			temp = 0;
			for (int kh = 0; kh < kernelSize; kh++)
			{
				for (int kw = 0; kw < kernelSize; kw++)
				{
					pix = *(padded + (h + kh) * paddedWidth + w + kw);
					knl = 1;
					temp += (pix * knl);
				}
			}
			temp /= pow(kernelSize, 2);
			temp = (unsigned char)temp;
			*(data + h * width + w) = temp;
		}
	}
	delete padded;
	return data;
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
	for (int h = paddingSize; h < outHeight - paddingSize; h++)
	{
		for (int w = paddingSize; w < outWidth - paddingSize; w++)
		{
			*(output + h * outWidth + w) = *(data+(h-paddingSize)*width + (w-paddingSize));
		}
	}
	return output;
}

unsigned char* ft(unsigned char* data, int height, int width)
{
	unsigned char* rData;
	unsigned char* iData;
	iData = (unsigned char*)malloc(sizeof(unsigned char) * height * width);
	rData = data;
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			*(iData + h * width + w) = 0;
		}
	}
	double* tempR;
	double*	tempI;
	tempR = (double*)malloc(sizeof(double) * width);
	tempI = (double*)malloc(sizeof(double) * width);
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			*(tempR + w) = *(rData + h * width + w);
			*(tempI + w) = *(iData + h * width + w);
		}
		FFT(tempR, tempI, width, 1);
		for (int w = 0; w < width; w++)
		{
			*(rData + h * width + w) = *(tempR + w);
			*(iData + h * width + w) = *(tempI + w);
		}
	}
	
	for (int w = 0; w < width; w++)
	{
		for (int h = 0; h< height; h++)
		{
			*(tempR + h) = *(rData + h * width + w);
			*(tempI + h) = *(iData + h * width + w);
		}
		FFT(tempR, tempI, width, 1);
		for (int h = 0; h < height; h++)
		{
			*(rData + h * width + w) = *(tempR + h);
			*(iData + h * width + w) = *(tempI + h);
		}
	}
	return rData;
}

void FFT(double* x, double* y, int n, int Sign)
{

	int i, j, k, l, m, n1, n2;
	double	c, c1, e, s, s1, t, tr, ti;
	//Get m  n=2^m  
	for (j = 1, i = 1; i < 16; i++)
	{
		m = i;
		j = 2 * j;
		if (j == n)break;
	}
	n1 = n - 1;
	for (j = 0, i = 0; i < n1; i++)
	{
		if (i < j)
		{
			tr = x[j];
			ti = y[j];
			x[j] = x[i];
			y[j] = y[i];
			x[i] = tr;
			y[i] = ti;
		}
		k = n / 2;
		while (k < (j + 1))
		{
			j = j - k;
			k = k / 2;
		}
		j += k;
	}
	n1 = 1;
	for (l = 1; l <= m; l++)
	{
		n1 *= 2;
		n2 = n1 / 2;
		e = 3.1415926 / n2;
		c = 1.0;
		s = 0.0;
		c1 = cos(e);
		s1 = -Sign * sin(e);
		for (j = 0; j < n2; j++)
		{
			for (i = j; i < n; i += n1)
			{
				k = i + n2;
				tr = c * x[k] - s * y[k];
				ti = c * y[k] + s * x[k];
				x[k] = x[i] - tr;
				y[k] = y[i] - ti;
				x[i] += tr;
				y[i] += ti;
			}
			t = c;
			c = c * c1 - s * s1;
			s = t * s1 + s * c1;
		}
	}
	if (Sign == -1)
	{
		for (i = 0; i < n; i++)
		{
			x[i] /= n;
			y[i] /= n;
		}
	}
}

