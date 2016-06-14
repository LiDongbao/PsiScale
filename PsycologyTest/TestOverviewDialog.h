#pragma once
#include "afxeditbrowsectrl.h"
#include "afxcmn.h"
#include "ScaleList.h"
#include <memory>
#include "..\PsiCommon\AnswerManager.h"
#include "afxwin.h"

class CPsiScale;
class CUser;

// CTestOverviewDialog dialog

class CScaleOverviewDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CScaleOverviewDialog)

public:
	CScaleOverviewDialog(CUser& user, CWnd* pParent = NULL);   // standard constructor
	virtual ~CScaleOverviewDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OVERVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	void GetTestInfoAndSetListInfo(std::vector<CString>& test_infos);
	CMFCEditBrowseCtrl _working_folder_edit;
public:
	CString _working_folder;
	afx_msg void OnEnChangeEditWorkingFolder();
	afx_msg LRESULT OnScaleFinished(WPARAM, LPARAM);
	virtual BOOL OnInitDialog() override;

	virtual void OnOK() override;

	virtual void OnCancel() override;

	CScaleList _scale_list;
	std::shared_ptr<CPsiScale> _scale;
	CUser& _user;
	CAnswerManagerOld _answer_manager;
	afx_msg void OnBnClickedStart();
	afx_msg void OnLvnItemchangedListScales(NMHDR *pNMHDR, LRESULT *pResult);
	CButton _start;
	afx_msg void OnBnClickedModifyPersonalInfo();
};
