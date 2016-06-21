#pragma once
#include "..\Utilities\EasySizeDialog.h"
#include "afxeditbrowsectrl.h"
#include <memory>
#include "afxwin.h"
#include "afxcmn.h"

class CAnswerManager;

class CPersonInfoDlg : public CEasySizeDialog
{
public:
	CPersonInfoDlg(CWnd* pParent = NULL);	// standard constructor

												// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PERSONINFO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
														// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	DECLARE_EASYSIZE;

	bool UpdateListHeader();
	bool UpdateList();

	std::shared_ptr<CAnswerManager> _answer_manager;
	CString _working_folder;
	CMFCEditBrowseCtrl _working_folder_edit;
	CListCtrl _list;
	
public:
	afx_msg void OnEnChangeEditWorkingFolder();
	
	
	
};

