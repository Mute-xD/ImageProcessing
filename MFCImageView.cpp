
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
#include "imageBase.h"
#include "ops.h"

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

	int height = 0; 
	int width = 0;
	TCHAR* fileName = _T("./res/raw.bmp");
	TCHAR* fileNameA = _T("./res/a.bmp");
	unsigned char* data;
	data = imageLoader(fileName, height, width);
	unsigned char* dataA;
	dataA = imageLoader(fileNameA, height, width);
	CClientDC* pdc = new CClientDC(this);
	//imagePrinter(data, height, width, pdc);
	//unsigned char* rotated = nullptr;
	//int rotatedHeight, rotatedWidth;
	//rotated =  rotate(data, height, width, 45, rotatedHeight, rotatedWidth);
	//imagePrinter(rotated, rotatedHeight, rotatedWidth, pdc, 600, 600);
	unsigned char* histoNormed;
	histoNormed = histogramNorm(data, height, width);
	imagePrinter(data, height, width, pdc);
	imagePrinter(histoNormed, height, width, pdc, 800, 200);
	unsigned char* histoNormedA;
	histoNormedA = histogramNorm(dataA, height, width);
	imagePrinter(dataA, height, width, pdc, 200, 600);
	imagePrinter(histoNormedA, height, width, pdc, 800, 600);

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
