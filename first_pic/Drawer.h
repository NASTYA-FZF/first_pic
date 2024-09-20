#pragma once
#include <gdiplus.h>
#include <vector>

// Drawer

class Drawer : public CStatic
{
	DECLARE_DYNAMIC(Drawer)

public:
	Drawer();
	virtual ~Drawer();

protected:
	DECLARE_MESSAGE_MAP()
public:
	ULONG_PTR token;
	bool first_start, dec_log;
	double pWidth, pHeight;
	Gdiplus::Bitmap* picture;
	std::vector<std::vector<double>> matr;
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void ConvertWB();
	double Inten(Gdiplus::Color my_col);
	void CreateImageFromArr();
	void LoadImage_(const wchar_t* path_file);
	void Norma();
	void normirovka(double min, double max, std::vector<std::vector<double>>& mat);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


