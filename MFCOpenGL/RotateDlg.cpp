// RotateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCOpenGL.h"
#include "RotateDlg.h"
#include "afxdialogex.h"


// CRotateDlg �Ի���

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


// CRotateDlg ��Ϣ�������


void CRotateDlg::OnEnChangeEditAngel()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
