﻿
// first_picDlg.h: файл заголовка
//

#pragma once
#include "wnd_gauss.h"
#include "Drawer.h"

// Диалоговое окно CfirstpicDlg
class CfirstpicDlg : public CDialogEx
{
// Создание
public:
	CfirstpicDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIRST_PIC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int width_pic;
	int heaght_pic;
	double alpha;
	double gamma;
	double error;
	bool image_start;
	bool inter_or_null;
	CListBox list_gauss;
	void add_gauss_in_list(gauss for_add, int row);
	afx_msg void OnBnClickedAddKup();
	wnd_gauss my_wnd_gauss;
	afx_msg void OnBnClickedChangeKup();
	std::vector<gauss> vec_gauss;
	void change_gauss_in_list(gauss for_change, int row = -1);
	afx_msg void OnBnClickedDeleteKup();
	afx_msg void OnBnClickedDeleteAll();
	void default_gauss();
	afx_msg void OnBnClickedButDefault();
	Drawer my_picture;
	Drawer pic_spectr;
	afx_msg void OnBnClickedOk();
	my_image image_all;
	Drawer pic_res;
	Drawer pic_shum;
	afx_msg void OnBnClickedRimage();
	afx_msg void OnBnClickedRgauss();
	afx_msg void OnBnClickedBloadimage();
	afx_msg void OnBnClickedBshum();
	afx_msg void OnBnClickedBclear();
	CButton r_gauss_kupol;
	CButton r_image;

	void SetCheckR(bool value); //true - gauss, false - image
	CButton but_default;
	CButton but_add;
	CButton but_change;
	CButton but_delete_one;
	CButton but_delete_all;
	CEdit edit_h;
	CEdit edit_w;
	CButton but_load;
	CStatic text_h;
	CStatic text_w;
	afx_msg void OnBnClickedRinter();
	afx_msg void OnBnClickedRnull();
	CButton r_inter;
	CButton r_null;
	double err_sig_shum;
	double per_error_shum;
	double per_error;
};

double my_round(double value, int num);