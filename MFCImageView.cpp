
// MFCImageView.cpp: CMFCImageView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCImage.h"
#endif

#include "MFCImageDoc.h"
#include "MFCImageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCImageView

IMPLEMENT_DYNCREATE(CMFCImageView, CView)

BEGIN_MESSAGE_MAP(CMFCImageView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFCImageView 构造/析构

CMFCImageView::CMFCImageView() noexcept
{
	// TODO: 在此处添加构造代码

}

CMFCImageView::~CMFCImageView()
{
}

BOOL CMFCImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCImageView 绘图

void CMFCImageView::OnDraw(CDC* /*pDC*/)
{
	CMFCImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	CFile fp;
	TCHAR* fileName = _T("./res/test.bmp");
	fp.Open(fileName, CFile::modeRead);
	short bftype;
	long bfsize;
	long bfreserved;
	long bfoffbits;


	fp.Read(&bftype, sizeof(bftype));
	fp.Read(&bfsize, sizeof(bfsize));
	fp.Read(&bfreserved, sizeof(bfreserved));
	fp.Read(&bfoffbits, sizeof(bfoffbits));

	fp.Read(&ih, sizeof(ih));
	int i;
	for (i = 0; i < 256; i++)
	{
		fp.Read(&pallete[i], sizeof(pallete[i]));
	}

	int b = ih.biwidth % 4;
	int j;
	unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * ih.biheight * ih.biwidth);
	for (i = 0; i < ih.biheight; i++)
	{
		for (int j = 0; j < ih.biwidth; j++)
		{
			fp.Read(data + (ih.biheight - 1 - i) * ih.biwidth + j, sizeof(char));
			if (b != 0)
				fp.Seek(4 - b, CFile::current);
		}
	}
	fp.Close();
	CClientDC* pdc = new CClientDC(this);

	unsigned int m = ih.biheight;
	unsigned int n = ih.biwidth;
	unsigned char a;
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
		{
			a = *(data + i * n + j);
			pdc->SetPixel(j + 400, i + 200, RGB(a, a, a));
		}
	float beta;
	beta = 45 * 3.1415926 / 180;
	int originx, originy;
	originx = ih.biheight / 2;//新的图像中心_1
	originy = ih.biwidth / 2;//新的图像中心_1

	int M, N;
	M = ih.biheight * fabs(cos(beta)) + ih.biwidth * fabs(sin(beta));
	N = ih.biheight * fabs(sin(beta)) + ih.biwidth * fabs(cos(beta));

	unsigned char* outimage;
	outimage = (unsigned char*)malloc(sizeof(unsigned char) * M * N);  //分配新的存储空间M*N
	float zxx, zxy;
	zxx = M / 2;
	zxy = N / 2;//新图像中心_2

	double x_2, y_2;
	float x_1, y_1;
	float x_3, y_3;
	int x_4, y_4;
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
		{
			x_1 = i - zxx;
			y_1 = j - zxy;

			x_2 = x_1 * cos(beta) - y_1 * sin(beta);
			y_2 = x_1 * sin(beta) + y_1 * cos(beta);

			x_3 = x_2 + originx;
			y_3 = y_2 + originy;

			x_4 = int(x_3 + 0.5);
			y_4 = int(y_3 + 0.5);

			if ((x_4 >= ih.biheight) || (x_4 < 0) || (y_4 >= ih.biwidth) || (y_4 < 0))
			{
				x_4 = 0;
				y_4 = 0;
			}
			*(outimage + i * N + j) = *(data + x_4 * ih.biwidth + y_4);


		}

	unsigned int b_new;

	m = ih.biheight;
	n = ih.biwidth;
	for (i = 0; i < m; i++)    //PS坐标系差别
		for (j = 0; j < n; j++)
		{
			b = *(data + i * n + j);
			pdc->SetPixel(j + 500, i + 200, RGB(b, b, b));
		}
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
		{
			b_new = *(outimage + i * N + j);
			pdc->SetPixel(j + 20, i + 20, RGB(b_new, b_new, b_new));
		}
}


// CMFCImageView 打印

BOOL CMFCImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCImageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCImageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMFCImageView 诊断

#ifdef _DEBUG
void CMFCImageView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCImageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCImageDoc* CMFCImageView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCImageDoc)));
	return (CMFCImageDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCImageView 消息处理程序
