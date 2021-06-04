#pragma once

void imagePrinter(unsigned char* data, int height, int width, CClientDC* pdc,
	int posOffsetX = 200, int posOffsetY = 200);
unsigned char* imageLoader(TCHAR* fileName, int &height, int &width);