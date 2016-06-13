#pragma once
#include "..\PsiCommon\User.h"
#include "ATLComTime.h"


class CPersonalInfoDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPersonalInfoDialog)

public:
	CPersonalInfoDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPersonalInfoDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PERSONAL_INFO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	void SetInfo(const PersonalInfo& info);
	PersonalInfo GetInfo() const;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString name;
	CString pinyin;
	CString nationality;
	COleDateTime birth_date;
	int sex;
	UINT weight;
	CString mobile;
	CString email;
	afx_msg void OnEnKillfocusEditMobile();
	afx_msg void OnEnKillfocusEditEmail();
	afx_msg void OnEnKillfocusEditWeight();
};
