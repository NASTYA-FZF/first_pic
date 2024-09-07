#pragma once
#include "afxdialogex.h"
#include "my_picture.h"

// Диалоговое окно wnd_gauss

class wnd_gauss : public CDialogEx
{
	DECLARE_DYNAMIC(wnd_gauss)

public:
	wnd_gauss(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~wnd_gauss();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
private:
	double A;
	int x0;
	int y0;
	double sigma;

public:
	gauss my_gauss;
	afx_msg void OnBnClickedOk();
	void set_gauss(gauss for_set);
	void clear();
};
