// PsiAnswerViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PsiAnswerViewer.h"
#include "PsiAnswerViewerDlg.h"
#include "afxdialogex.h"
#include "..\Utilities\FileSystem.h"
#include "..\Utilities\macros.h"
#include "..\PsiCommon\PsiScale.h"
#include "..\PsiCommon\AnswerManager.h"
#include "..\PsiCommon\User.h"
#include <afxstr.h>
#include <vector>
#include <algorithm>
#include "..\Utilities\Clipboard.h"
#include "..\Utilities\xml.h"
#include "..\PsiCommon\xml_name_space.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_EASYSIZE_MAP(CPsiAnswerViewerDlg)
	EASYSIZE(IDC_EDIT_WORKING_FOLDER, ES_BORDER, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_COMBO_SCALE, ES_BORDER, ES_BORDER, ES_KEEPSIZE, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_ANSWER_TABLE, ES_BORDER, ES_BORDER, ES_BORDER, ES_BORDER, 0)
END_EASYSIZE_MAP

using namespace std;
using namespace XMLNameSpace;

const unsigned int num_info = 7;

bool IsShort(const CString& s1, const CString& s2)
{
	return (_ttoi(s1.Left(s1.Find(_T("."))).GetBuffer()) < _ttoi(s2.Left(s2.Find(_T("."))).GetBuffer()));
}
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

														// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPsiAnswerViewerDlg dialog

CPsiAnswerViewerDlg::CPsiAnswerViewerDlg(CWnd* pParent /*=NULL*/)
	: CEasySizeDialog(IDD_PSIANSWERVIEWER_DIALOG, L"PsiAnswerViewer", pParent, true),
	_row(0),
	_working_folder(_T("")),
	_is_answer(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//_working_folder.Format(_T("../Scales"));
}

void CPsiAnswerViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANSWER_TABLE, _answer_table);
	DDX_Control(pDX, IDC_COMBO_SCALE, _combo_scale);
	DDX_Control(pDX, IDC_COMBO_PERSON, _combo_person);
	DDX_Control(pDX, IDC_EDIT_WORKING_FOLDER, _working_folder_edit);
	DDX_Text(pDX, IDC_EDIT_WORKING_FOLDER, _working_folder);
}

BEGIN_MESSAGE_MAP(CPsiAnswerViewerDlg, CEasySizeDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_SCALE, &CPsiAnswerViewerDlg::OnCbnSelchangeComboScale)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CPsiAnswerViewerDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CPsiAnswerViewerDlg::OnBnClickedButtonRemove)
	ON_EN_CHANGE(IDC_EDIT_WORKING_FOLDER, &CPsiAnswerViewerDlg::OnEnChangeEditWorkingFolder)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CPsiAnswerViewerDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_MERGE, &CPsiAnswerViewerDlg::OnBnClickedButtonMerge)
	ON_BN_CLICKED(IDC_CHECK_ANSWER, &CPsiAnswerViewerDlg::OnBnClickedCheckAnswer)
END_MESSAGE_MAP()

// CPsiAnswerViewerDlg message handlers

BOOL CPsiAnswerViewerDlg::OnInitDialog()
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
									// TODO: Add extra initialization here
	InitialScaleList();
	TODO(被试信息路径是硬编码);
	InitialPersonCombo();

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ANSWER);
	pBtn->SetCheck(1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPsiAnswerViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		__super::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPsiAnswerViewerDlg::OnPaint()
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
HCURSOR CPsiAnswerViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CPsiAnswerViewerDlg::InitialScaleList()
{
	return true;
}

bool CPsiAnswerViewerDlg::InitialPersonCombo()
{
	std::vector<CString> files;
	FileSystem::ForEachFile(_working_folder + _T("/TestUsers/Answers"), _T("*.xml"), false, [&](const CString& file) {
		CString filename = FileSystem::GetFileNameFromPath(file);
		files.push_back(filename);
	});

	for (auto iter = files.begin(); iter != files.end(); ++iter)
	{
		_combo_person.AddString(*iter);
	}

	return true;
}


void CPsiAnswerViewerDlg::UpdateAnswerScaleHeader()
{
	//删除表格中的原有信息
	ClearTable();

	CRect mRect;
	_answer_table.GetWindowRect(&mRect);     //获取控件矩形区域
	int kuan = mRect.Width();
	_answer_table.InsertColumn(0, _T("编号"), LVCFMT_LEFT, 60, -1);
	_answer_table.InsertColumn(1, _T("出生年月"), LVCFMT_CENTER, 120, -1);
	_answer_table.InsertColumn(2, _T("性别"), LVCFMT_CENTER, 60, -1);
	_answer_table.InsertColumn(3, _T("民族"), LVCFMT_CENTER, 60, -1);
	_answer_table.InsertColumn(4, _T("体重"), LVCFMT_CENTER, 60, -1);
	DWORD dwStyle = _answer_table.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	dwStyle |= LVS_EX_CHECKBOXES; //item前生成checkbox控件
	_answer_table.SetExtendedStyle(dwStyle); //设置扩展风格

	_answer_table.InsertColumn(5, _T("填表日期"), LVCFMT_CENTER, 120, -1);
	_answer_table.InsertColumn(6, _T("填表时间"), LVCFMT_CENTER, 120, -1);
	for (unsigned int i = 0; i < _scale->GetQuestionCount(); ++i)
	{
		CString str;
		str.Format(_T("No.%d"), i + 1);
		_answer_table.InsertColumn(i + num_info, str, LVCFMT_LEFT, 60, -1);
	}

	//for (unsigned int i = 0; i < _scale->GetGroupCount(); ++i)
	//{
	//	CString str;
	//	str.Format(_T("Group.%d"), i + 1);
	//	_answer_table.InsertColumn(i + num_info + _scale->GetQuestionCount(), str, LVCFMT_LEFT, 120, -1);
	//}

	//_answer_table.InsertColumn(7 + _scale->GetQuestionCount() + _scale->GetGroupCount(), _T("Total"), LVCFMT_LEFT, 120, -1);

	for (unsigned int i = 0; i < _scale->GetQuestionCount(); ++i)
	{
		CString str;
		str.Format(_T("RT %d"), i + 1);
		_answer_table.InsertColumn(i + num_info + _scale->GetQuestionCount(), str, LVCFMT_LEFT, 60, -1);
	}

}

bool CPsiAnswerViewerDlg::InsertAnswer(ScaleAnswers& scale_answers)
{
	CString date, time;
	
	date.Format(_T("%4d-%02d-%02d"), scale_answers.start_time.GetYear(), scale_answers.start_time.GetMonth(), scale_answers.start_time.GetDay());
	_answer_table.SetItemText(_row, num_info - 2, date);
	time.Format(_T("%02d:%02d"), scale_answers.start_time.GetHour(), scale_answers.start_time.GetMinute());
	_answer_table.SetItemText(_row, num_info - 1, time);
	

	for (unsigned int i = 0; i < _scale->GetQuestionCount(); ++i)
	{
		CString str;
		str.Format(_T("%d"), scale_answers.answer_info[i].answer);
		_answer_table.SetItemText(_row, num_info + i, str);
	}

	//for (unsigned int i = 0; i < _scale->GetGroupCount(); ++i)
	//{
	//	CString str;
	//	str.Format(_T("%d"), answer_manager.);
	//	_answer_table.InsertColumn(i + num_info + _scale->GetQuestionCount(), str, LVCFMT_LEFT, 120, -1);
	//}

	//CString str;
	//str.Format(_T("%d"), answer_manager.GetTotalScore(_scale->GetName(), L""));
	//_answer_table.SetItemText(_row, num_info + _scale->GetQuestionCount() + _scale->GetGroupCount(), str);


	for (unsigned int i = 0; i < _scale->GetQuestionCount(); ++i)
	{
		CString str;
		str.Format(_T("%d"), scale_answers.answer_info[i].reaction_time);
		_answer_table.SetItemText(_row, num_info + _scale->GetQuestionCount() + i, str);
	}
	return true;
}

bool CPsiAnswerViewerDlg::InsertInfo(CUser& user)
{
	CString row;
	row.Format(L"%d", _row + 1);
	_answer_table.InsertItem(_row, row.GetString());
	_answer_table.SetItemText(_row, 1, user.GetInfo().birth_date.Format(_T("%Y-%M")));

	CString sex;
	switch (Sex(user.GetInfo().sex))
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
	_answer_table.SetItemText(_row, 2, sex);

	_answer_table.SetItemText(_row, 3, user.GetInfo().nationality);

	CString weight; 
	weight.Format(_T("%d"), user.GetInfo().weight);
	_answer_table.SetItemText(_row, 4, weight);

	return true;
}

void CPsiAnswerViewerDlg::UpdateAnswerList(const TCHAR* scale)
{
	auto index = _answer_manager->GetIndexByScale(scale);

	for (auto iter = index.begin(); iter != index.end(); ++iter)
	{
		if (_answer_manager->ScaleFinished(*iter) == false)
		{
			continue;
		}

		auto user = CUserManager::GetInstance().GetUser(_answer_manager->GetScaleAnswers(*iter).user_uid);
		if (user)
		{
			InsertInfo(*user);
		}

		if (_is_answer)
		{
			auto scale_answers = _answer_manager->GetScaleAnswers(*iter);
			InsertAnswer(scale_answers);
		}
		
		++_row;
	}
}

bool CPsiAnswerViewerDlg::UpdateScaleTableHeaderForSubjects()
{
	//删除表格中的原有信息
	ClearTable();

	CRect mRect;
	_answer_table.GetWindowRect(&mRect);     //获取控件矩形区域
	int kuan = mRect.Width();
	_answer_table.InsertColumn(0, _T("用户名"), LVCFMT_LEFT, 100, -1);
	_answer_table.InsertColumn(1, _T("密码"), LVCFMT_LEFT, 100, -1);
	_answer_table.InsertColumn(2, _T("UID"), LVCFMT_LEFT, 150, -1);
	_answer_table.InsertColumn(3, _T("姓名"), LVCFMT_CENTER, 60, -1);
	_answer_table.InsertColumn(4, _T("拼音"), LVCFMT_CENTER, 80, -1);
	_answer_table.InsertColumn(5, _T("出生年月"), LVCFMT_CENTER, 100, -1);
	_answer_table.InsertColumn(6, _T("性别"), LVCFMT_CENTER, 60, -1);
	_answer_table.InsertColumn(7, _T("民族"), LVCFMT_CENTER, 60, -1);
	_answer_table.InsertColumn(8, _T("体重"), LVCFMT_CENTER, 60, -1);
	_answer_table.InsertColumn(9, _T("电话"), LVCFMT_CENTER, 150, -1);
	_answer_table.InsertColumn(10, _T("电邮"), LVCFMT_CENTER, 150, -1);
	DWORD dwStyle = _answer_table.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	dwStyle |= LVS_EX_CHECKBOXES; //item前生成checkbox控件
	_answer_table.SetExtendedStyle(dwStyle); //设置扩展风格

	return true;
}

bool CPsiAnswerViewerDlg::UpdateSubjectsTable()
{
	auto users = CUserManager::GetInstance().Users();

	for (auto iter = users.begin(); iter != users.end(); ++iter)
	{
		_answer_table.InsertItem(_row, iter->second->GetUserId());
		_answer_table.SetItemText(_row, 1, iter->second->GetPassword());
		_answer_table.SetItemText(_row, 2, iter->second->GetUid());
		_answer_table.SetItemText(_row, 3, iter->second->GetInfo().name);
		_answer_table.SetItemText(_row, 4, iter->second->GetInfo().name_pinyin);
		_answer_table.SetItemText(_row, 5, iter->second->GetInfo().birth_date.Format(_T("%Y-%M")));


		CString sex;
		switch (Sex(iter->second->GetInfo().sex))
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
		_answer_table.SetItemText(_row, 6, sex);
		_answer_table.SetItemText(_row, 7, iter->second->GetInfo().nationality);

		CString weight;
		weight.Format(_T("%d"), iter->second->GetInfo().weight);
		_answer_table.SetItemText(_row, 8, weight);

		_answer_table.SetItemText(_row, 9, iter->second->GetInfo().mobile);
		_answer_table.SetItemText(_row, 10, iter->second->GetInfo().email);

		++_row;
	}

	return true;
}

void CPsiAnswerViewerDlg::OnCbnSelchangeComboScale()
{
	int nIndex = _combo_scale.GetCurSel();
	CString scale_name;
	_combo_scale.GetLBText(nIndex, scale_name);

	CString file_path = _working_folder + _T("/") + scale_name + _T(".scale");
	
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
		UpdateAnswerScaleHeader();
		_row = 0;
		CString temp = scale_name.Right(scale_name.GetLength() - scale_name.ReverseFind(_T('.')) - 1);
		UpdateAnswerList(temp);
	}
}


void CPsiAnswerViewerDlg::OnBnClickedButtonAdd()
{
	//int nIndex = _combo_person.GetCurSel();
	//if (nIndex != -1)
	//{
	//	CString user_uid;
	//	_combo_person.GetLBText(nIndex, user_uid);

	//	CString file_path = _working_folder + _T("/TestUsers/Answers/") + user_uid + _T(".xml");

	//	CAnswerManager answer_manager;
	//	CUser user(L"Temp", L"0");
	//	if (answer_manager.Load(file_path, user) && answer_manager.ScaleFinished(_scale->GetName()))
	//	{
	//		if (InsertInfo(user))
	//		{
	//			if (InsertAnswer(answer_manager))
	//			{
	//				++_row;
	//			}
	//			else
	//				AfxMessageBox(_T("插入答案有错."));
	//		}
	//		else
	//		{
	//			AfxMessageBox(_T("插入用户信息有错."));
	//		}
	//	}
	//	else
	//	{
	//		AfxMessageBox(_T("载入错误或者该被试并未答题."));
	//	}
	//}
}


void CPsiAnswerViewerDlg::OnBnClickedButtonRemove()
{
	_answer_table.SetExtendedStyle(LVS_EX_CHECKBOXES);
	vector<unsigned int> checked_line;
	for (int i = 0; i < _answer_table.GetItemCount(); i++)
	{
		if (_answer_table.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || _answer_table.GetCheck(i))
		{
			checked_line.push_back(i);
		}
	}

	for (unsigned int i = 0; i < checked_line.size(); ++i)
	{
		_answer_table.DeleteItem(checked_line[i] - i);
		--_row;
	}
}


void CPsiAnswerViewerDlg::OnEnChangeEditWorkingFolder()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEasySizeDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData();
	std::vector<CString> files;
	FileSystem::ForEachFile(_working_folder, _T("*.scale"), false, [&](const CString& file) {
		CString filename = FileSystem::GetFileNameFromPath(file);
		files.push_back(filename);
	});

	std::sort(files.begin(), files.end(), IsShort);

	for (auto iter = files.begin(); iter != files.end(); ++iter)
	{
		_combo_scale.AddString(*iter);
	}

	CString file_path = _working_folder + _T("\\TestUsers\\Answers");
	_answer_manager->LoadAll(file_path);
}

void CPsiAnswerViewerDlg::OnBnClickedButtonCopy()
{
	CString str;
	for (unsigned int i = 0; i < _row; ++i)
	{
		for (int j = 0; j < _answer_table.GetHeaderCtrl()->GetItemCount(); ++j)
		{
			str += _answer_table.GetItemText(i, j);
			str += "\t";
		}
		str += "\n";
	}
	
	if (!Utilities::OS::SetClipboardText(str.GetString()))
	{
		AfxMessageBox(_T("Copy Failed"));
	}

}


void CPsiAnswerViewerDlg::OnBnClickedButtonMerge()
{
	ASSERT(_answer_manager);
	
	Utilities::CXml users_info(XML_USERS_INFO);

	if (CUserManager::GetInstance().Users().empty())
	{
		AfxMessageBox(L"No users");
		return;
	}

	for (auto item : CUserManager::GetInstance().Users())
	{
		auto user_info_element = users_info.AddElement(XML_USER_INFO);
		user_info_element->SetAttrib(XML_USER_NAME, item.second->GetUserId());
		user_info_element->SetAttrib(XML_USER_PASSWORD, item.second->GetPassword());
		user_info_element->SetAttrib(XML_USER_UID, item.second->GetUid());
	}

	bool result = users_info.Save(_working_folder + _T("\\TestUsers\\UserInfo.xml"));
	if (result)
	{
		AfxMessageBox(L"Merge Successfully.");
	}
	else
	{
		AfxMessageBox(L"Merge Failed!");
	}
}


void CPsiAnswerViewerDlg::OnBnClickedCheckAnswer()
{
	_is_answer = !_is_answer;

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ANSWER);
	pBtn->SetCheck(_is_answer ? 1 : 0);

	_row = 0;
	
	if (!_is_answer)
	{
		_combo_scale.EnableWindow(FALSE);
		_combo_person.EnableWindow(FALSE);
		UpdateScaleTableHeaderForSubjects();
		UpdateSubjectsTable();
	}
	else
	{
		_combo_scale.EnableWindow(TRUE);
		_combo_person.EnableWindow(TRUE);

		if (_combo_scale.GetCurSel() == -1)
		{
			ClearTable();
		}
		else
		{
			OnCbnSelchangeComboScale();
		}
	}
}

void CPsiAnswerViewerDlg::ClearTable()
{
	unsigned int num_columns = _answer_table.GetHeaderCtrl()->GetItemCount();
	for (unsigned int i = 0; i < num_columns; ++i)
	{
		_answer_table.DeleteColumn(0);
	}
	_answer_table.DeleteAllItems();
}
