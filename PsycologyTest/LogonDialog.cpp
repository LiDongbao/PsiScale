// LogonDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PsycologyTest.h"
#include "LogonDialog.h"
#include "afxdialogex.h"
#include "UserManager.h"
#include "TestOverviewDialog.h"
#include "User.h"

#include <memory>

using namespace std;

// CLogonDialog dialog

IMPLEMENT_DYNAMIC(CLogonDialog, CDialogEx)

CLogonDialog::CLogonDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGON_DIALOG, pParent)
	, _user_name(_T(""))
	, _password(_T(""))
	, _password2(_T(""))
	, _first_time(FALSE)
	, _user_manager(new CUserManager)
{
}

CLogonDialog::~CLogonDialog()
{
}

void CLogonDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, _user_name);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, _password);
	DDX_Text(pDX, IDC_EDIT_PASSWORD2, _password2);
	DDX_Check(pDX, IDC_CHECK_FIRST_TIME, _first_time);
	DDX_Control(pDX, IDC_EDIT_PASSWORD2, _password2_edit);
	DDX_Control(pDX, IDC_PASSWORD_LABEL, _password2_label);
}


BEGIN_MESSAGE_MAP(CLogonDialog, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_FIRST_TIME, &CLogonDialog::OnBnClickedCheckFirstTime)
	ON_BN_CLICKED(IDC_LOGON, &CLogonDialog::OnBnClickedLogon)
END_MESSAGE_MAP()


// CLogonDialog message handlers


void CLogonDialog::OnBnClickedCheckFirstTime()
{
	UpdateData();

	auto show_window = (_first_time != FALSE) ? SW_SHOW : SW_HIDE;
	_password2_label.ShowWindow(show_window);
	_password2_edit.ShowWindow(show_window);
}


void CLogonDialog::OnBnClickedLogon()
{
	UpdateData();
	
	if (_user_name.IsEmpty())
	{
		AfxMessageBox(_T("用户名不能为空。"));
		return;
	}
	if (_password.IsEmpty())
	{
		AfxMessageBox(_T("密码不能为空。"));
		return;
	}

	if (_first_time != FALSE)
	{
		if (_password != _password2)
		{
			AfxMessageBox(_T("两次输入的密码不一致，请重新输入。"));
			return;
		}
		else
		{
			auto user = _user_manager->CreateUser(_user_name, _password);
			if (!user)
			{
				AfxMessageBox(_T("不是第一次登陆，该用户名和密码已存在，若需重新生成账号，请修改用户名或密码。"));
				return;
			}
			RunScale(user);
		}
	}
	else
	{
		auto user = _user_manager->GetUser(_user_name, _password);
		if (!user)
		{
			AfxMessageBox(_T("用户名或者密码不正确，请重新输入。"));
			return;
		}

		RunScale(user);
	}
}

void CLogonDialog::RunScale(std::shared_ptr<CUser> user)
{
	if (_first_time)
	{
		_user_manager->Save();
	}

	CScaleOverviewDialog dlg(*user);
	if (dlg.DoModal() == IDOK)
	{
		_password.Empty();
		_password2.Empty();
		_user_name.Empty();

		UpdateData(FALSE);
	}

}


BOOL CLogonDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	_first_time = true;
	UpdateData(FALSE);
	_user_manager->Init();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
