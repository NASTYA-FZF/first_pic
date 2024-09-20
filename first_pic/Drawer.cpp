// Drawer.cpp: файл реализации
//

#include "pch.h"
#define my_rect lpDrawItemStruct->rcItem
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
	first_start = true;
	dec_log = true;
}

Drawer::~Drawer()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(Drawer, CStatic)
//	ON_WM_DRAWITEM()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// Обработчики сообщений Drawer




//void Drawer::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	
//}


void Drawer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Добавьте исходный код для отображения указанного элемента
	if (matr.empty())
	{
		//MessageBox(L"Нечего выводить! Матрица пуста!", L"Ошибка");
		return;
	}

	Norma();

	Graphics wnd(lpDrawItemStruct->hDC);
	Bitmap buffer(lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, &wnd);
	Graphics draw_in_buffer(&buffer);

	pWidth = matr[0].size();
	pHeight = matr.size();
	Bitmap pic(pWidth, pHeight);
	picture = &pic;
	CreateImageFromArr();
	double scaleX = (double)(my_rect.right - my_rect.left) / pWidth;
	double scaleY = (double)(my_rect.bottom - my_rect.top) / pHeight;
	draw_in_buffer.ScaleTransform(scaleX, scaleY);

	draw_in_buffer.DrawImage(picture, Rect(0, 0, picture->GetWidth(), picture->GetHeight()));

	wnd.DrawImage(&buffer, 0, 0, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, UnitPixel);
}

void Drawer::ConvertWB()
{
	pHeight = picture->GetHeight();
	pWidth = picture->GetWidth();
	matr = vector<vector<double>>(pHeight, vector<double>(pWidth));

	if (pHeight == 0 || pWidth == 0)
	{
		MessageBox(L"Изображение некорректно загружено!", L"Ошибка");
		return;
	}

	Status ctrl;
	Color my_color;
	for (int i = 0; i < pWidth; i++)
	{
		for (int j = 0; j < pHeight; j++)
		{
			ctrl = picture->GetPixel(i, j, &my_color);
			matr[j][i] = Inten(my_color);
			if (ctrl != Ok)
			{
				MessageBox(L"Что-то пошло не так при получении пикселя!", L"Ошибка");
				break;
			}
		}
	}
}

double Drawer::Inten(Color my_col)
{
	return 0.299 * my_col.GetRed() + 0.587 * my_col.GetGreen() + 0.114 * my_col.GetBlue();
}

void Drawer::CreateImageFromArr()
{
	Status ctrl;

	for (int i = 0; i < pWidth; i++)
	{
		for (int j = 0; j < pHeight; j++)
		{
			ctrl = picture->SetPixel(i, j, Color(matr[j][i], matr[j][i], matr[j][i]));
			if (ctrl != Ok)
			{
				MessageBox(L"Что-то пошло не так при установлении цвета пикселя!", L"Ошибка");
				break;
			}
		}
	}
}

void Drawer::LoadImage_(const wchar_t* path_file)
{
	Bitmap pic(path_file);
	picture = &pic;
	ConvertWB();
}

void Drawer::Norma()
{
	int h = matr.size();
	int w = matr[0].size();
	double min = 0, max = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (max < matr[i][j])
				max = matr[i][j];
			if (min > matr[i][j])
				min = matr[i][j];
		}
	}

	if (dec_log)
	{
		normirovka(min, max, matr);
	}
	else
	{
		double lmin = 0, lmax = 0;
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				matr[i][j] += min;
				if (matr[i][j] != 0)
					matr[i][j] = -log10(matr[i][j] / max);
				if (lmin > matr[i][j])
					lmin = matr[i][j];
				if (lmax < matr[i][j])
					lmax = matr[i][j];
			}
		}
		normirovka(lmin, lmax, matr);
	}
}

void Drawer::normirovka(double min, double max, vector<vector<double>>& mat)
{
	for (int i = 0; i < mat.size(); i++)
	{
		for (int j = 0; j < mat[0].size(); j++)
			mat[i][j] = (mat[i][j] - min) * 255 / (max - min);
	}
}


void Drawer::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	dec_log = !dec_log;
	Invalidate(FALSE);
	CStatic::OnLButtonDown(nFlags, point);
}
