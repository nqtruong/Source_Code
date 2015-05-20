#pragma once
#include "explorer.h"

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// CWebBrowserDlg dialog

class CWebBrowserDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CWebBrowserDlg)

public:
	CWebBrowserDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWebBrowserDlg();

	// Dialog Data
	enum { IDD = IDD_WEBBROWSER_DIALOG, IDH = IDR_HTML_WEBBROWSERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()

public:
	CExplorer	m_WebBrowser;
	CWnd		*m_pParrentCWnd;

	void		SetParrent(CWnd* pParent);
	void		TestParrent();
};
