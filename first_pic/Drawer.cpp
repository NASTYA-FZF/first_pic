// Drawer.cpp: файл реализации
//

#include "pch.h"
#include "first_pic.h"
#include "Drawer.h"
using namespace std;
using namespace Gdiplus;

// Drawer

IMPLEMENT_DYNAMIC(Drawer, CStatic)

Drawer::Drawer()
{
	GdiplusStartupInput input;
	Status s;
	s = GdiplusStartup(&token, &input, NULL);
	if (s != Ok) MessageBox(L"s != Ok", L"Error!");
}

Drawer::~Drawer()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(Drawer, CStatic)
//	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// Обработчики сообщений Drawer




//void Drawer::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	
//}


void Drawer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Добавьте исходный код для отображения указанного элемента
	Graphics wnd(lpDrawItemStruct->hDC);
	Bitmap buffer(lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, &wnd);
	Graphics draw_in_buffer(&buffer);

	draw_in_buffer.Clear(Color(125, 0, 0));

	wnd.DrawImage(&buffer, 0, 0, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, UnitPixel);
}
