// WebBrowserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GradutedProject.h"
#include "WebBrowserDlg.h"
#include "GradutedProjectDlg.h"


// CWebBrowserDlg dialog

IMPLEMENT_DYNCREATE(CWebBrowserDlg, CDHtmlDialog)

CWebBrowserDlg::CWebBrowserDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CWebBrowserDlg::IDD, CWebBrowserDlg::IDH, pParent)
{

}

CWebBrowserDlg::~CWebBrowserDlg()
{
}

void CWebBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER, m_WebBrowser);
}

BOOL CWebBrowserDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CWebBrowserDlg, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CWebBrowserDlg)
END_DHTML_EVENT_MAP()

void CWebBrowserDlg::SetParrent( CWnd* pParent )
{
	m_pParrentCWnd = pParent;
}

void CWebBrowserDlg::TestParrent()
{
	CGradutedProjectDlg *dlg = (CGradutedProjectDlg*)m_pParentWnd;
}