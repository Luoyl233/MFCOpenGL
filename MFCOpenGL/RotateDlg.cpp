// RotateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCOpenGL.h"
#include "RotateDlg.h"
#include "afxdialogex.h"


// CRotateDlg 对话框

IMPLEMENT_DYNAMIC(CRotateDlg, CDialogEx)

CRotateDlg::CRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ROTATE, pParent)
	, m_angel(0)
{

}

BOOL CRotateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

/*	CString cstr;
	cstr.Format(_T("%f"), m_angel);
	((CEdit *)GetDlgItem(IDC_EDIT_ANGEL))->SetWindowText(cstr);
	*/
	return TRUE;
}

CRotateDlg::~CRotateDlg()
{
}

void CRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ANGEL, m_angel);
	DDV_MinMaxDouble(pDX, m_angel, 0.0, 360.0);
}


BEGIN_MESSAGE_MAP(CRotateDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_ANGEL, &CRotateDlg::OnEnChangeEditAngel)
END_MESSAGE_MAP()


// CRotateDlg 消息处理程序


void CRotateDlg::OnEnChangeEditAngel()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
