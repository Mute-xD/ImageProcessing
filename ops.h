#pragma once

unsigned char* rotate(unsigned char* data, int height, int width, float degree,
	int& outHeight, int& outWidth);

float *calHostogram(unsigned char* data, int height, int width);

unsigned char* histogramNorm(unsigned char* data, int height, int width);