// wnd_gauss.cpp: файл реализации
//

#include "pch.h"
#include "first_pic.h"
#include "afxdialogex.h"
#include "wnd_gauss.h"


// Диалоговое окно wnd_gauss

IMPLEMENT_DYNAMIC(wnd_gauss, CDialogEx)

wnd_gauss::wnd_gauss(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, A(0)
	, x0(0)
	, y0(0)
	, sigma(0)
{

}

wnd_gauss::~wnd_gauss()
{
}

void wnd_gauss::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, A);
	DDX_Text(pDX, IDC_EDIT2, x0);
	DDX_Text(pDX, IDC_EDIT3, y0);
	DDX_Text(pDX, IDC_EDIT4, sigma);
}


BEGIN_MESSAGE_MAP(wnd_gauss, CDialogEx)
	ON_BN_CLICKED(IDOK, &wnd_gauss::OnBnClickedOk)
END_MESSAGE_MAP()


// Обработчики сообщений wnd_gauss


void wnd_gauss::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	my_gauss = gauss(A, x0, y0, sigma);
	CDialogEx::OnOK();
}

void wnd_gauss::set_gauss(gauss for_set)
{
	A = for_set.A;
	x0 = for_set.p0.first;
	y0 = for_set.p0.second;
	sigma = for_set.sigma;
}

void wnd_gauss::clear()
{
	A = 0; x0 = 0; y0 = 0; sigma = 0;
	my_gauss = gauss();
}
