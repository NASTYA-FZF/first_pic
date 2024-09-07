
// first_picDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "first_pic.h"
#include "first_picDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CfirstpicDlg



CfirstpicDlg::CfirstpicDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FIRST_PIC_DIALOG, pParent)
	, width_pic(256)
	, heaght_pic(256)
	, alpha(0.8)
	, gamma(0.4)
	, error(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfirstpicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, width_pic);
	DDX_Text(pDX, IDC_EDIT2, heaght_pic);
	DDX_Text(pDX, IDC_EDIT4, alpha);
	DDX_Text(pDX, IDC_EDIT6, gamma);
	DDX_Text(pDX, IDC_EDIT5, error);
	DDX_Control(pDX, IDC_LIST2, list_gauss);
	DDX_Control(pDX, IDC_PIC, my_picture);
}

BEGIN_MESSAGE_MAP(CfirstpicDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD_KUP, &CfirstpicDlg::OnBnClickedAddKup)
	ON_BN_CLICKED(IDC_CHANGE_KUP, &CfirstpicDlg::OnBnClickedChangeKup)
	ON_BN_CLICKED(IDC_DELETE_KUP, &CfirstpicDlg::OnBnClickedDeleteKup)
	ON_BN_CLICKED(IDC_DELETE_ALL, &CfirstpicDlg::OnBnClickedDeleteAll)
	ON_BN_CLICKED(IDC_BUT_DEFAULT, &CfirstpicDlg::OnBnClickedButDefault)
END_MESSAGE_MAP()


// Обработчики сообщений CfirstpicDlg

BOOL CfirstpicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	default_gauss();
	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CfirstpicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CfirstpicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CfirstpicDlg::add_gauss_in_list(gauss for_add, int row = -1)
{
	CString str;
	str.Format(L"A = %.2f, x0 = %d, y0 = %d, sigma = %.2f", for_add.A, for_add.p0.first, for_add.p0.second, for_add.sigma);
	list_gauss.InsertString(row, str);
}



void CfirstpicDlg::OnBnClickedAddKup()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (my_wnd_gauss.DoModal() == IDOK)
	{
		add_gauss_in_list(my_wnd_gauss.my_gauss);
		vec_gauss.push_back(my_wnd_gauss.my_gauss);
		my_wnd_gauss.clear();
	}
}


void CfirstpicDlg::OnBnClickedChangeKup()
{
	// TODO: добавьте свой код обработчика уведомлений
	int num = list_gauss.GetCurSel();

	if (num == LB_ERR)
	{
		MessageBox(L"Выберите строку!", L"Предупреждение");
		return;
	}

	my_wnd_gauss.set_gauss(vec_gauss[num]);
	
	if (my_wnd_gauss.DoModal() == IDOK)
	{
		change_gauss_in_list(my_wnd_gauss.my_gauss, num);
		vec_gauss[num] = my_wnd_gauss.my_gauss;
		my_wnd_gauss.clear();
	}
}

void CfirstpicDlg::change_gauss_in_list(gauss for_change, int row)
{
	list_gauss.DeleteString(row);
	add_gauss_in_list(for_change, row);
}


void CfirstpicDlg::OnBnClickedDeleteKup()
{
	// TODO: добавьте свой код обработчика уведомлений
	int num = list_gauss.GetCurSel();

	if (num == LB_ERR)
	{
		MessageBox(L"Выберите строку!", L"Предупреждение");
		return;
	}

	list_gauss.DeleteString(num);
	vec_gauss.erase(vec_gauss.begin() + num);
}


void CfirstpicDlg::OnBnClickedDeleteAll()
{
	// TODO: добавьте свой код обработчика уведомлений
	list_gauss.ResetContent();
	vec_gauss.clear();
}

void CfirstpicDlg::default_gauss()
{
	vec_gauss.push_back(gauss(1, (int)(0.5 * width_pic), (int)(0.5 * heaght_pic), 0.25));
	add_gauss_in_list(vec_gauss.back());

	vec_gauss.push_back(gauss(1, (int)(0.2 * width_pic), (int)(0.2 * heaght_pic), 0.15));
	add_gauss_in_list(vec_gauss.back());

	vec_gauss.push_back(gauss(1, (int)(0.7 * width_pic), (int)(0.7 * heaght_pic), 0.35));
	add_gauss_in_list(vec_gauss.back());
}


void CfirstpicDlg::OnBnClickedButDefault()
{
	// TODO: добавьте свой код обработчика уведомлений
	default_gauss();
}
