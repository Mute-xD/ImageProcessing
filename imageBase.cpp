#include "pch.h"
#include "framework.h"
#include "imageBase.h"

void imagePrinter(unsigned char *data, int height, int width, CClientDC* pdc, int posOffsetX, int posOffsetY)
{
	unsigned char b;
	for (int i = 0; i < height; i++)    //PS×ø±êÏµ²î±ð
		for (int j = 0; j < width; j++)
		{
			b = *(data + i * width + j);
			pdc->SetPixel(j + posOffsetX, i + posOffsetY, RGB(b, b, b));
		}
}

unsigned char* imageLoader(TCHAR* fileName, int& height, int& width)
{
	CFile fp;
	fp.Open(fileName, CFile::modeRead);
	BITMAPFILEHEADER fileHead;
	fp.Read(&fileHead, sizeof(fileHead));
	BITMAPINFOHEADER infoHead;
	fp.Read(&infoHead, sizeof(infoHead));
	height = infoHead.biHeight;
	width = infoHead.biWidth;
	RGBQUAD cmap[256];
	for (int i = 0; i < 256; i++)
	{
		fp.Read(&cmap[i], sizeof(cmap[i]));
	}
	unsigned char *data = (unsigned char*)malloc(sizeof(unsigned char) * height * width);
	int b = width % 4;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			fp.Read(data + (height - 1 - i) * width + j, sizeof(char));
			if (b != 0)
				fp.Seek(4. - b, CFile::current);
		}
	}
	fp.Close();
	return data;
}
