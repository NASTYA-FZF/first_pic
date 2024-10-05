
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
	, width_pic(127)
	, heaght_pic(127)
	, alpha(0)
	, gamma(100)
	, error(0)
	, err_sig_shum(0)
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
	DDX_Control(pDX, IDC_SPECTR, pic_spectr);
	DDX_Control(pDX, IDC_PIC_RES, pic_res);
	DDX_Control(pDX, IDC_PIC_SHUM, pic_shum);
	DDX_Control(pDX, IDC_RGAUSS, r_gauss_kupol);
	DDX_Control(pDX, IDC_RIMAGE, r_image);
	DDX_Control(pDX, IDC_BUT_DEFAULT, but_default);
	DDX_Control(pDX, IDC_ADD_KUP, but_add);
	DDX_Control(pDX, IDC_CHANGE_KUP, but_change);
	DDX_Control(pDX, IDC_DELETE_KUP, but_delete_one);
	DDX_Control(pDX, IDC_DELETE_ALL, but_delete_all);
	DDX_Control(pDX, IDC_EDIT2, edit_h);
	DDX_Control(pDX, IDC_EDIT3, edit_w);
	DDX_Control(pDX, IDC_BLOADIMAGE, but_load);
	DDX_Control(pDX, IDC_TEXT_H, text_h);
	DDX_Control(pDX, IDC_TEXT_W, text_w);
	DDX_Control(pDX, IDC_RINTER, r_inter);
	DDX_Control(pDX, IDC_RNULL, r_null);
	DDX_Text(pDX, IDC_EDIT1, err_sig_shum);
}

BEGIN_MESSAGE_MAP(CfirstpicDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD_KUP, &CfirstpicDlg::OnBnClickedAddKup)
	ON_BN_CLICKED(IDC_CHANGE_KUP, &CfirstpicDlg::OnBnClickedChangeKup)
	ON_BN_CLICKED(IDC_DELETE_KUP, &CfirstpicDlg::OnBnClickedDeleteKup)
	ON_BN_CLICKED(IDC_DELETE_ALL, &CfirstpicDlg::OnBnClickedDeleteAll)
	ON_BN_CLICKED(IDC_BUT_DEFAULT, &CfirstpicDlg::OnBnClickedButDefault)
	ON_BN_CLICKED(IDOK, &CfirstpicDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RIMAGE, &CfirstpicDlg::OnBnClickedRimage)
	ON_BN_CLICKED(IDC_RGAUSS, &CfirstpicDlg::OnBnClickedRgauss)
	ON_BN_CLICKED(IDC_BLOADIMAGE, &CfirstpicDlg::OnBnClickedBloadimage)
	ON_BN_CLICKED(IDC_BSHUM, &CfirstpicDlg::OnBnClickedBshum)
	ON_BN_CLICKED(IDC_BCLEAR, &CfirstpicDlg::OnBnClickedBclear)
	ON_BN_CLICKED(IDC_RINTER, &CfirstpicDlg::OnBnClickedRinter)
	ON_BN_CLICKED(IDC_RNULL, &CfirstpicDlg::OnBnClickedRnull)
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
	image_start = false;
	SetCheckR(true);
	inter_or_null = true;
	r_inter.SetCheck(BST_CHECKED);
	r_null.SetCheck(BST_UNCHECKED);
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
	vec_gauss.push_back(gauss(1, (int)(0.5 * width_pic), (int)(0.5 * heaght_pic), 25));
	add_gauss_in_list(vec_gauss.back());

	vec_gauss.push_back(gauss(1, (int)(0.2 * width_pic), (int)(0.2 * heaght_pic), 15));
	add_gauss_in_list(vec_gauss.back());

	//vec_gauss.push_back(gauss(1, (int)(0.7 * width_pic), (int)(0.7 * heaght_pic), 35));
	//add_gauss_in_list(vec_gauss.back());
}


void CfirstpicDlg::OnBnClickedButDefault()
{
	// TODO: добавьте свой код обработчика уведомлений
	default_gauss();
}


void CfirstpicDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	//double w;
	//double h;
	if (!image_start)
	{
		image_all = my_image(vec_gauss, width_pic, heaght_pic, alpha, gamma, inter_or_null);
		//image_all = my_image(my_picture.GetMatr(), alpha, gamma);
	}
	else
	{
		if (my_picture.matr.empty())
		{
			MessageBox(L"Нет картинки!", L"Ошибка!");
			return;
		}
		image_all = my_image(my_picture.GetMatr(), alpha, gamma, inter_or_null);
	}

	image_all.Process();

	/*w = my_picture.w_start;
	h = my_picture.h_start;

	pic_shum.w_start = w;
	pic_shum.h_start = h;
	pic_spectr.w_start = w;
	pic_spectr.h_start = h;
	pic_res.w_start = w;
	pic_res.h_start = h;*/

	my_picture.SetMatr(image_all.GetImageStart(), image_all.lx, image_all.ly, image_all.wid, false);
	pic_shum.SetMatr(image_all.GetImageShum(), image_all.lx, image_all.ly, image_all.wid, false);
	pic_spectr.SetMatr(image_all.GetAmplSpectr(), image_all.lx, image_all.ly, image_all.wid, true);
	pic_res.SetMatr(image_all.GetImageRes(), image_all.lx, image_all.ly, image_all.wid, false);
	Invalidate(FALSE);
	error = my_round(image_all.find_error(my_picture.matr, pic_res.matr), 2);
	err_sig_shum = my_round(image_all.find_error(my_picture.matr, pic_shum.matr), 2);
	UpdateData(FALSE);
}


void CfirstpicDlg::OnBnClickedRimage()
{
	// TODO: добавьте свой код обработчика уведомлений
	image_start = true;
	SetCheckR(false);
}


void CfirstpicDlg::OnBnClickedRgauss()
{
	// TODO: добавьте свой код обработчика уведомлений
	image_start = false;
	SetCheckR(true);
}


void CfirstpicDlg::OnBnClickedBloadimage()
{
	// TODO: добавьте свой код обработчика уведомлений
	CFileDialog change_image(TRUE);
	change_image.DoModal();
	auto path = change_image.GetPathName();
	my_picture.LoadImage_(path);
	Invalidate(FALSE);
}


void CfirstpicDlg::OnBnClickedBshum()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	image_all.set_alpha(alpha);
	image_all.generate_pic_with_shum();

	pic_shum.SetMatr(image_all.GetImageShum(), image_all.lx, image_all.ly, image_all.wid, false);
	pic_spectr.SetMatr(image_all.GetAmplSpectr(), image_all.lx, image_all.ly, image_all.wid, true);
	pic_res.SetMatr(image_all.GetImageRes(), image_all.lx, image_all.ly, image_all.wid, false);
	Invalidate(FALSE);
}


void CfirstpicDlg::OnBnClickedBclear()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	image_all.set_gamma(gamma);
	image_all.ProcessClearImage();
	pic_spectr.SetMatr(image_all.GetAmplSpectr(), image_all.lx, image_all.ly, image_all.wid, true);
	pic_res.SetMatr(image_all.GetImageRes(), image_all.lx, image_all.ly, image_all.wid, false);
	Invalidate(FALSE);

	error = my_round(image_all.find_error(my_picture.matr, pic_res.matr), 2);
	UpdateData(FALSE);
}

void CfirstpicDlg::SetCheckR(bool value)
{
	if (value)
	{
		r_gauss_kupol.SetCheck(BST_CHECKED);
		r_image.SetCheck(BST_UNCHECKED);
		list_gauss.ShowWindow(SW_SHOW);
		but_default.ShowWindow(SW_SHOW);
		but_add.ShowWindow(SW_SHOW);
		but_change.ShowWindow(SW_SHOW);
		but_delete_one.ShowWindow(SW_SHOW);
		but_delete_all.ShowWindow(SW_SHOW);
		but_load.ShowWindow(SW_HIDE);
		text_h.ShowWindow(SW_SHOW);
		text_w.ShowWindow(SW_SHOW);
		edit_h.ShowWindow(SW_SHOW);
		edit_w.ShowWindow(SW_SHOW);
	}
	else
	{
		r_gauss_kupol.SetCheck(BST_UNCHECKED);
		r_image.SetCheck(BST_CHECKED);
		list_gauss.ShowWindow(SW_HIDE);
		but_default.ShowWindow(SW_HIDE);
		but_add.ShowWindow(SW_HIDE);
		but_change.ShowWindow(SW_HIDE);
		but_delete_one.ShowWindow(SW_HIDE);
		but_delete_all.ShowWindow(SW_HIDE);
		but_load.ShowWindow(SW_SHOW);
		text_h.ShowWindow(SW_HIDE);
		text_w.ShowWindow(SW_HIDE);
		edit_h.ShowWindow(SW_HIDE);
		edit_w.ShowWindow(SW_HIDE);
	}
}

double my_round(double value, int num)
{
	double step = pow(10, num);
	return round(value * step) / step;
}


void CfirstpicDlg::OnBnClickedRinter()
{
	// TODO: добавьте свой код обработчика уведомлений
	inter_or_null = true;
	r_inter.SetCheck(BST_CHECKED);
	r_null.SetCheck(BST_UNCHECKED);
}


void CfirstpicDlg::OnBnClickedRnull()
{
	// TODO: добавьте свой код обработчика уведомлений
	inter_or_null = false;
	r_inter.SetCheck(BST_UNCHECKED);
	r_null.SetCheck(BST_CHECKED);
}
