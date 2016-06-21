#include "stdafx.h"
#include "PersonInfoDlg.h"
#include "Resource.h"
#include <vector>
#include "..\Utilities\FileSystem.h"
#include "..\PsiCommon\AnswerManager.h"
#include <algorithm>
#include <afxstr.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_EASYSIZE_MAP(CPersonInfoDlg)
	EASYSIZE(IDC_EDIT_WORKING_FOLDER, ES_BORDER, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_EDIT1, ES_BORDER, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_PERSONINFO_LIST, ES_BORDER, ES_BORDER, ES_BORDER, ES_BORDER, 0)
END_EASYSIZE_MAP

using namespace std;

//bool IsShort(const CString& s1, const CString& s2)
//{
//	return (_ttoi(s1.Left(s1.Find(_T("."))).GetBuffer()) < _ttoi(s2.Left(s2.Find(_T("."))).GetBuffer()));
//}

// CAboutDlg dialog used for App About
//
//class CAboutDlg : public CDialogEx
//{
//public:
//	CAboutDlg();
//
//	// Dialog Data
//#ifdef AFX_DESIGN_TIME
//	enum { IDD = IDD_ABOUTBOX };
//#endif
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//
//														// Implementation
//protected:
//	DECLARE_MESSAGE_MAP()
//};
//
//CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
//{
//}
//
//void CAboutDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialogEx::DoDataExchange(pDX);
//}
//
//BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//END_MESSAGE_MAP()
//

// CPersonInfoDlg dialog

CPersonInfoDlg::CPersonInfoDlg(CWnd* pParent /*=NULL*/)
	: CEasySizeDialog(IDD_PERSONINFO_DIALOG, L"PersonViewer", pParent, true), 
	_working_folder(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPersonInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_WORKING_FOLDER, _working_folder);
	DDX_Control(pDX, IDC_EDIT_WORKING_FOLDER, _working_folder_edit);
	DDX_Control(pDX, IDC_PERSONINFO_LIST, _list);
}

BEGIN_MESSAGE_MAP(CPersonInfoDlg, CEasySizeDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_WORKING_FOLDER, &CPersonInfoDlg::OnEnChangeEditWorkingFolder)

END_MESSAGE_MAP()

// CPersonInfoDlg message handlers

BOOL CPersonInfoDlg::OnInitDialog()
{
	__super::OnInitDialog();

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

	_answer_manager = std::shared_ptr<CAnswerManager>(new CAnswerManager);
	_working_folder_edit.EnableFolderBrowseButton();

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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPersonInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else
	{
		__super::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPersonInfoDlg::OnPaint()
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
		__super::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPersonInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPersonInfoDlg::OnEnChangeEditWorkingFolder()
{
	UpdateData();
	
	if (_answer_manager->LoadAll(_working_folder))
	{
		UpdateListHeader();
		UpdateList();
	}
	
}

bool CPersonInfoDlg::UpdateListHeader()
{
	unsigned int num_columns = _list.GetHeaderCtrl()->GetItemCount();
	for (unsigned int i = 0; i < num_columns; ++i)
	{
		_list.DeleteColumn(0);
	}
	_list.DeleteAllItems();

	CRect mRect;
	_list.GetWindowRect(&mRect);     //获取控件矩形区域
	int kuan = mRect.Width();
	_list.InsertColumn(0, _T("用户名"), LVCFMT_LEFT, 100, -1);
	_list.InsertColumn(1, _T("密码"), LVCFMT_LEFT, 100, -1);
	_list.InsertColumn(2, _T("UID"), LVCFMT_LEFT, 150, -1);
	_list.InsertColumn(3, _T("姓名"), LVCFMT_CENTER, 60, -1);
	_list.InsertColumn(4, _T("拼音"), LVCFMT_CENTER, 80, -1);
	_list.InsertColumn(5, _T("出生年月"), LVCFMT_CENTER, 100, -1);
	_list.InsertColumn(6, _T("性别"), LVCFMT_CENTER, 60, -1);
	_list.InsertColumn(7, _T("民族"), LVCFMT_CENTER, 60, -1);
	_list.InsertColumn(8, _T("体重"), LVCFMT_CENTER, 60, -1);
	_list.InsertColumn(9, _T("电话"), LVCFMT_CENTER, 150, -1);
	_list.InsertColumn(10, _T("电邮"), LVCFMT_CENTER, 150, -1);
	DWORD dwStyle = _list.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	dwStyle |= LVS_EX_CHECKBOXES; //item前生成checkbox控件
	_list.SetExtendedStyle(dwStyle); //设置扩展风格

	return true;
}

bool CPersonInfoDlg::UpdateList()
{
	if (_answer_manager == nullptr)
	{
		return false;
	}
	auto users = CUserManager::GetInstance().Users();
	unsigned int row = 0;

	for (auto user = users.begin(); user != users.end(); ++user)
	{
		
		_list.InsertItem(row, user->second->GetUserId());
		_list.SetItemText(row, 1, user->second->GetPassword());
		_list.SetItemText(row, 2, user->second->GetUid());
		_list.SetItemText(row, 3, user->second->GetInfo().name);
		_list.SetItemText(row, 4, user->second->GetInfo().name_pinyin);
		_list.SetItemText(row, 5, user->second->GetInfo().birth_date.Format(_T("%Y-%M")));
		

		CString sex;
		switch (Sex(user->second->GetInfo().sex))
		{
		case Sex::SexMale:
			sex.Format(_T("男"));
			break;
		case Sex::SexFemale:
			sex.Format(_T("女"));
			break;
		case Sex::SexUnknown:
			sex.Format(_T("未知"));
			break;
		default:
			break;
		}
		_list.SetItemText(row, 6, sex);
		_list.SetItemText(row, 7, user->second->GetInfo().nationality);

		CString weight;
		weight.Format(_T("%d"), user->second->GetInfo().weight);
		_list.SetItemText(row, 8, weight);

		_list.SetItemText(row, 9, user->second->GetInfo().mobile);
		_list.SetItemText(row, 10, user->second->GetInfo().email);

		++row;
	}
	return true;
}
