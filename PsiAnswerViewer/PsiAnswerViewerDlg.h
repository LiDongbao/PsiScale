#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "..\Utilities\EasySizeDialog.h"
#include <memory>
#include "afxeditbrowsectrl.h"

class CPsiScale;
class CAnswerManager;
class CUser;
struct ScaleAnswers;

	// CPsiAnswerViewerDlg dialog
	class CPsiAnswerViewerDlg : public CEasySizeDialog
{
	// Construction
public:
	CPsiAnswerViewerDlg(CWnd* pParent = NULL);	// standard constructor

												// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PSIANSWERVIEWER_DIALOG };
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

	bool InitialScaleList();
	bool InitialPersonCombo();
	void UpdateAnswerScaleHeader();
	bool InsertAnswer(ScaleAnswers& scale_answers);
	bool InsertInfo(CUser& user);

	void UpdateAnswerList(const TCHAR* scale);

	bool UpdateScaleTableHeaderForSubjects();
	bool UpdateSubjectsTable();

	CListCtrl _answer_table;
	CComboBox _combo_scale;
	CString _working_folder;
	CMFCEditBrowseCtrl _working_folder_edit;
	std::shared_ptr<CPsiScale> _scale;
	
	CComboBox _combo_person;
	std::shared_ptr<CAnswerManager> _answer_manager;
	unsigned int _row;
	bool _is_answer;
public:
	afx_msg void OnCbnSelchangeComboScale();
	
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnEnChangeEditWorkingFolder();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonMerge();
	afx_msg void OnBnClickedCheckAnswer();

	void ClearTable();

	};
