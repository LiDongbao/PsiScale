#pragma once
#include "afxwin.h"
#include <memory>

class CUserManager;
class CUser;

// CLogonDialog dialog

class CLogonDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CLogonDialog)

public:
	CLogonDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogonDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGON_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckFirstTime();
	afx_msg void OnBnClickedLogon();

	void RunScale(std::shared_ptr<CUser> user);
	std::shared_ptr<CUser> GetUser();
	bool IsFirstTime() const;

private:
	std::shared_ptr<CUser> _user;

	CString _user_id;
	CString _password;
	CString _password2;
	BOOL _first_time;
	CEdit _password2_edit;
	CStatic _password2_label;

	virtual BOOL OnInitDialog();
};
