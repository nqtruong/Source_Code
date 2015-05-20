
// GradutedProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GradutedProject.h"
#include "GradutedProjectDlg.h"
#include "Definition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGradutedProjectDlg dialog




CGradutedProjectDlg::CGradutedProjectDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGradutedProjectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bLoadedData = FALSE;
	m_pMapDialog = new CWebBrowserDlg(this);
	m_pMapDialog->SetParrent(this);
}

void CGradutedProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progressBar);
}

BEGIN_MESSAGE_MAP(CGradutedProjectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_NOTIFY_TRAINING_PERCENT, OnUpdateTrainingPercentage)
	ON_BN_CLICKED(IDC_BROWSE1, &CGradutedProjectDlg::OnBnClickedBrowse1)
	ON_BN_CLICKED(IDC_BROWSE2, &CGradutedProjectDlg::OnBnClickedBrowse2)
	ON_BN_CLICKED(IDC_BROWSE3, &CGradutedProjectDlg::OnBnClickedBrowse3)
	ON_BN_CLICKED(IDC_LOADDATA, &CGradutedProjectDlg::OnBnClickedLoaddata)
	ON_BN_CLICKED(IDC_TRAINING, &CGradutedProjectDlg::OnBnClickedTraining)
	ON_BN_CLICKED(IDC_TRAIN1PATTERN, &CGradutedProjectDlg::OnBnClickedTrain1pattern)
	ON_BN_CLICKED(IDC_TRAINEPOCH, &CGradutedProjectDlg::OnBnClickedTrainepoch)
END_MESSAGE_MAP()


// CGradutedProjectDlg message handlers

BOOL CGradutedProjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	this->CheckDlgButton(IDC_RECTANGLE, TRUE);
	this->SetDlgItemText(IDC_LRMIN, L"0.02");
	this->SetDlgItemText(IDC_LRMAX, L"2.0");
	this->SetDlgItemText(IDC_ITERATION, L"100");
	this->SetDlgItemText(IDC_SIZEOFSOM, L"15");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGradutedProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGradutedProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGradutedProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGradutedProjectDlg::OnBnClickedBrowse1()
{
	// TODO: Add your control notification handler code here
	CFileDialog FileDialog(TRUE,
		L"*.*",
		NULL,
		OFN_HIDEREADONLY,
		L"Text Files: (*.txt)|*.txt||");

	if(FileDialog.DoModal() == IDOK)
	{
		CString PathName = FileDialog.GetPathName();
		if (PathFileExists(PathName))
		{
			m_sDataFilePath = PathName;
			this->SetDlgItemText(IDC_EDIT1, PathName);
		}
		else
		{
			AfxMessageBox(L"Path to dataset file's not exist");
		}
	}
}

void CGradutedProjectDlg::OnBnClickedBrowse2()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CFileDialog FileDialog(TRUE,
		L"*.*",
		NULL,
		OFN_HIDEREADONLY,
		L"Text Files: (*.txt)|*.txt||");

	if(FileDialog.DoModal() == IDOK)
	{
		CString PathName = FileDialog.GetPathName();
		if (PathFileExists(PathName))
		{
			m_sWeightFilePath = PathName;
			this->SetDlgItemText(IDC_EDIT2, PathName);
		}
	}
}

void CGradutedProjectDlg::OnBnClickedBrowse3()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CFileDialog FileDialog(TRUE,
		L"*.*",
		NULL,
		OFN_HIDEREADONLY,
		L"Text Files: (*.txt)|*.txt||");

	if(FileDialog.DoModal() == IDOK)
	{
		CString PathName = FileDialog.GetPathName();
		if (PathFileExists(PathName))
		{
			this->SetDlgItemText(IDC_EDIT3, PathName);
		}
	}
}

void CGradutedProjectDlg::OnBnClickedLoaddata()
{
	// TODO: Add your control notification handler code here
	if (!m_bLoadedData)
	{
		CString sTemp;
		double  dTemp = 0.0;
		DWORD	dwTemp = 0;

		this->GetDlgItemText(IDC_EDIT1, sTemp);
		m_sDataFilePath = sTemp;

		this->GetDlgItemText(IDC_EDIT2, sTemp);
		m_sWeightFilePath = sTemp;

		this->GetDlgItemText(IDC_EDIT3, sTemp);
		m_sSavedFilePath = sTemp;

		// default
		m_iFunction = MyNamespace::GAUS;

		if (m_sDataFilePath.IsEmpty() || !PathFileExists(m_sDataFilePath))
		{
			AfxMessageBox(L"Training data's not found!");
			return;
		}

		if (this->IsDlgButtonChecked(IDC_RECTANGLE))
			m_Topology = MyNamespace::RECTANGLE;
		else
			m_Topology = MyNamespace::HEXAGONAL;

		this->GetDlgItemText(IDC_LRMIN, sTemp);
		dTemp = _tstof(sTemp);
		if(sTemp.IsEmpty() || dTemp <= 0.0)
		{
			AfxMessageBox(L"Value of \"Learning rate MIN\" is wrong!");
			return;
		}
		m_dAlphaMIN = dTemp;

		this->GetDlgItemText(IDC_LRMAX, sTemp);
		dTemp = _tstof(sTemp);
		if(sTemp.IsEmpty() || dTemp <= m_dAlphaMIN)
		{
			AfxMessageBox(L"Value of \"Learning rate MAX\" is wrong!");
			return;
		}
		m_dAlphaMAX = dTemp;

		this->GetDlgItemText(IDC_ITERATION, sTemp);
		dwTemp = _wtoi(sTemp);
		if(sTemp.IsEmpty() || dwTemp <= 20)
		{
			AfxMessageBox(L"Value of \"Number of Iterations\" is wrong!\nSet it > 20, plz");
			return;
		}
		m_dwIteration = dwTemp;

		this->GetDlgItemText(IDC_SIZEOFSOM, sTemp);
		dwTemp = _wtoi(sTemp);
		if(sTemp.IsEmpty() || dwTemp <= 1)
		{
			AfxMessageBox(L"Value of \"Size of SOMs\" is wrong!\nSet it > 5, plz!");
			return;
		}
		m_dwDimensionOfOutputLayer = dwTemp;

		/*m_NeuronNetwork.SetDialog(this);*/
		m_NeuronNetwork.Init(this);

		m_bLoadedData = TRUE;
		this->SetDlgItemText(IDC_LOADDATA, L"Unload data");
	}
	else
	{
		if (AfxMessageBox(L"Training data's loaded. \nDo you want to unload ?", MB_OKCANCEL) == IDOK)
		{
			m_bLoadedData = FALSE;
			m_NeuronNetwork.ClearAll();
			this->SetDlgItemText(IDC_LOADDATA, L"Load data");
		}
	}
}

void CGradutedProjectDlg::OnBnClickedTrain1pattern()
{
	// TODO: Add your control notification handler code here
	if (!m_bLoadedData)
	{
		AfxMessageBox(L"Load data to training");
		return;
	}
	m_NeuronNetwork.LearnOnePatternEx();
}

void CGradutedProjectDlg::OnBnClickedTrainepoch()
{
	// TODO: Add your control notification handler code here
	if (!m_bLoadedData)
	{
		AfxMessageBox(L"Load data to training");
		return;
	}
	m_NeuronNetwork.LearnOneEpoch();
}

void CGradutedProjectDlg::OnBnClickedTraining()
{
	// TODO: Add your control notification handler code here
	if (!m_bLoadedData)
	{
		AfxMessageBox(L"Load data to training");
		return;
	}
	m_NeuronNetwork.ExecuteSDH(10);
	m_pMapDialog->TestParrent();
}

LRESULT CGradutedProjectDlg::OnUpdateTrainingPercentage(WPARAM wParam, LPARAM lParam)
{
	BOOL	bShowProgress = (BOOL)lParam;
	DWORD	dwPercent	= (DWORD)wParam;

	if (bShowProgress)
	{
		m_progressBar.ShowWindow(SW_SHOW);
		if(dwPercent < 0)
			dwPercent = 0;
		if (dwPercent > 100)
			dwPercent = 100;

		m_progressBar.SetPos(dwPercent);
	}
	else
		m_progressBar.ShowWindow(SW_HIDE);	

	return 0;
}