// TestOverviewDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PsycologyTest.h"
#include "TestOverviewDialog.h"
#include "afxdialogex.h"
#include "../Utilities/FileSystem.h"
#include "../PsiCommon/PsiScale.h"
#include "PsycologyTestDlg.h"
#include <algorithm>
#include "..\PsiCommon\User.h"
#include "PersonalInfoDialog.h"

using namespace std;

// CTestOverviewDialog dialog
bool IsShort(const CString& s1, const CString& s2)
{
	return (_ttoi(s1.Left(s1.Find(_T("."))).GetBuffer()) < _ttoi(s2.Left(s2.Find(_T("."))).GetBuffer()));
}

IMPLEMENT_DYNAMIC(CScaleOverviewDialog, CDialogEx)

CScaleOverviewDialog::CScaleOverviewDialog(CUser& user, 
	CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_OVERVIEW, pParent),
	_user(user),
	_working_folder(_T(""))
{
}

CScaleOverviewDialog::~CScaleOverviewDialog()
{
}

void CScaleOverviewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_WORKING_FOLDER, _working_folder_edit);
	DDX_Text(pDX, IDC_EDIT_WORKING_FOLDER, _working_folder);
	DDX_Control(pDX, IDC_LIST_SCALES, _scale_list);
	DDX_Control(pDX, IDC_START, _start);
}


BEGIN_MESSAGE_MAP(CScaleOverviewDialog, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_WORKING_FOLDER, &CScaleOverviewDialog::OnEnChangeEditWorkingFolder)
	ON_BN_CLICKED(IDC_START, &CScaleOverviewDialog::OnBnClickedStart)
	ON_MESSAGE(WM_SCALE_FINISHED, OnScaleFinished)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SCALES, &CScaleOverviewDialog::OnLvnItemchangedListScales)
	ON_BN_CLICKED(IDC_MODIFY_PERSONAL_INFO, &CScaleOverviewDialog::OnBnClickedModifyPersonalInfo)
END_MESSAGE_MAP()


// CTestOverviewDialog message handlers

void CScaleOverviewDialog::GetTestInfoAndSetListInfo(std::vector<CString>& test_infos)
{
	_answer_manager.Load(_user.GetWorkingFolder() + _T("\\") + _user.GetUid() + _T(".xml"), _user);
	std::for_each(test_infos.begin(), test_infos.end(), [&, this](CString item) {
		CString temp = item.Right(item.GetLength() - item.ReverseFind(_T('.')) - 1);
		_scale_list.InsertScale(item, _answer_manager.ScaleFinished(temp)); });
}


void CScaleOverviewDialog::OnEnChangeEditWorkingFolder()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData();
	if (FileSystem::FileExists(_working_folder))
	{
		_scale_list.Clear();
		std::vector<CString> files;
		FileSystem::ForEachFile(_working_folder, _T("*.scale"), false, [&](const CString& file) {
			CString filename = FileSystem::GetFileNameFromPath(file);
			files.push_back(filename);
		});

		std::sort(files.begin(), files.end(), IsShort);
		GetTestInfoAndSetListInfo(files);

		CRegKey regkey;
		if (regkey.Open(HKEY_CURRENT_USER, _T("Software\\SKMR\\PsiScale"), KEY_WRITE) == ERROR_SUCCESS ||
			regkey.Create(HKEY_CURRENT_USER, _T("Software\\SKMR\\PsiScale")) == ERROR_SUCCESS)
		{
			regkey.SetStringValue(_T("TestFolder"), _working_folder);
			regkey.Close();
		}
	}
}

BOOL CScaleOverviewDialog::OnInitDialog()
{
	__super::OnInitDialog();

	_working_folder_edit.EnableFolderBrowseButton();
	_scale_list.Init();

	CRegKey regkey;
	if (regkey.Open(HKEY_CURRENT_USER, _T("Software\\SKMR\\PsiScale"), KEY_READ) == ERROR_SUCCESS)
	{
		static TCHAR buffer[512];
		ULONG count = 512;
		if (regkey.QueryStringValue(_T("TestFolder"), buffer, &count) == ERROR_SUCCESS)
		{
			if (FileSystem::FileExists(buffer))
			{
				_working_folder_edit.SetWindowText(buffer);
			}
		}
	}
	return TRUE;
}

void CScaleOverviewDialog::OnOK()
{
	// __super::OnOK();
}

void CScaleOverviewDialog::OnCancel()
{
	if (AfxMessageBox(_T("确认退出？"), MB_OKCANCEL) == IDOK)
	{
		_answer_manager.Save(_user.GetWorkingFolder() + _T("\\") + _user.GetUid() + _T(".xml"), _user);
		__super::OnCancel();
	}
}


void CScaleOverviewDialog::OnBnClickedStart()
{
	POSITION position = _scale_list.GetFirstSelectedItemPosition();
	int index = _scale_list.GetNextSelectedItem(position);
	
	CString file_path = _working_folder + _T("\\");
	file_path += _scale_list.GetItemText(index, 0) + _T(".scale");

	if (!_scale)
	{
		try
		{
			_scale = shared_ptr<CPsiScale>(new CPsiScale);
		}
		catch (CMemoryException*)
		{
			return;
		}
	}

	if (!_scale->Load(file_path))
	{
		AfxMessageBox(_T("无法打开量表文件。"));
	}
	else
	{
		ShowWindow(SW_HIDE);
		if (AfxMessageBox(_scale->GetPrologue(), MB_OKCANCEL) == IDOK)
		{
			CPsycologyTestDlg dlg(_scale, _answer_manager, m_hWnd);
			dlg.DoModal();
		}

		_answer_manager.Save(_user.GetWorkingFolder() + _T("\\") + _user.GetUid() + _T(".xml"), _user);
		ShowWindow(SW_SHOW);
	}
}

LRESULT CScaleOverviewDialog::OnScaleFinished(WPARAM, LPARAM)
{
	_scale_list.ChangeScale(_scale->GetName(), true); //不起作用
	return 0;
}

void CScaleOverviewDialog::OnLvnItemchangedListScales(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	POSITION position = _scale_list.GetFirstSelectedItemPosition();
	int index = _scale_list.GetNextSelectedItem(position);

	CString state;
	state = _scale_list.GetItemText(index, 1);

	_start.EnableWindow(state != _T("完成"));

	*pResult = 0;
}


void CScaleOverviewDialog::OnBnClickedModifyPersonalInfo()
{
	CPersonalInfoDialog Info_dlg;
	Info_dlg.SetInfo(_user.GetInfo());

	if (Info_dlg.DoModal() == IDOK)
	{
		_user.SetInfo(Info_dlg.GetInfo());
	}
}
