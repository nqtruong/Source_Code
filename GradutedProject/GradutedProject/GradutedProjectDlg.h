
// GradutedProjectDlg.h : header file
//

#pragma once
#include "NeuronNetwork.h"
#include "resource.h"
#include "WebBrowserDlg.h"

#define WM_NOTIFY_TRAINING_PERCENT		WM_USER + 200

// CGradutedProjectDlg dialog
class CGradutedProjectDlg : public CDialog
{
	// Construction
public:
	CGradutedProjectDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	enum { IDD = IDD_GRADUTEDPROJECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Attribute of class CGradutedProjectDlg
public:
	CWebBrowserDlg			*m_pMapDialog;

	CNeuronNetwork			m_NeuronNetwork;
	double					m_dAlphaMAX, m_dAlphaMIN;
	INT						m_iFunction;
	DWORD					m_dwIteration;
	DWORD					m_dwDimensionOfOutputLayer;
	DWORD					m_Topology;

	CString					m_sDataFilePath;
	CString					m_sWeightFilePath;
	CString					m_sSavedFilePath;

	BOOL					m_bLoadedData;
	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBrowse1();
	afx_msg void OnBnClickedBrowse2();
	afx_msg void OnBnClickedBrowse3();
	afx_msg void OnBnClickedLoaddata();
	afx_msg void OnBnClickedTraining();
	afx_msg void OnBnClickedTrain1pattern();
	afx_msg void OnBnClickedTrainepoch();

	LRESULT OnUpdateTrainingPercentage(WPARAM wParam, LPARAM lParam);

protected:
	CProgressCtrl	m_progressBar;
};
