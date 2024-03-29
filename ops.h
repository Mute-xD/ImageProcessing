#pragma once

unsigned char* rotate(unsigned char* data, int height, int width, float degree,
	int& outHeight, int& outWidth);

unsigned char* histogramNorm(unsigned char* data, int height, int width);

unsigned char* histogramNorm(unsigned char* data, int height, int width);

unsigned char* conv2d(unsigned char* data, int height, int width, float* kernel, int kernelSize);

unsigned char* padding(unsigned char* data, int height, int width, int paddingSize, int& outHeight, int& outWidth);

unsigned char* ft(unsigned char* data, int height, int width);

void FFT(double* x, double* y, int n, int Sign);
