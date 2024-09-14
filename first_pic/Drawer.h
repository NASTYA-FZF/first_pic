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
	bool first_start;
	double pWidth, pHeight;
	Gdiplus::Bitmap* picture;
	std::vector<std::vector<double>> matr;
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void ConvertWB();
	double Inten(Gdiplus::Color my_col);
	void CreateImageFromArr();
	void LoadImage_(const wchar_t* path_file);
};


