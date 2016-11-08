// PersonalInfoDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PsycologyTest.h"
#include "PersonalInfoDialog.h"
#include "afxdialogex.h"
#include "regex"

// CPersonalInfoDialog dialog

IMPLEMENT_DYNAMIC(CPersonalInfoDialog, CDialogEx)

CPersonalInfoDialog::CPersonalInfoDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PERSONAL_INFO_DIALOG, pParent)
	, name(_T(""))
	, pinyin(_T(""))
	, nationality(_T(""))
	, birth_date(COleDateTime::GetCurrentTime())
	, sex(0)
	, weight(0)
	, mobile(_T(""))
	, email(_T(""))
	, mental(FALSE)
	, sight(FALSE)
	, others(_T(""))
{

}

CPersonalInfoDialog::~CPersonalInfoDialog()
{
}


void CPersonalInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME_INFO, name);
	DDX_Text(pDX, IDC_EDIT_PINYIN, pinyin);
	DDX_Text(pDX, IDC_EDIT_NATIONALITY, nationality);
	DDX_DateTimeCtrl(pDX, IDC_BIRTH_DATE, birth_date);
	DDX_CBIndex(pDX, IDC_COMBO_SEX, sex);
	DDX_Text(pDX, IDC_EDIT_WEIGHT, weight);
	DDX_Text(pDX, IDC_EDIT_MOBILE, mobile);
	DDX_Text(pDX, IDC_EDIT_EMAIL, email);
	DDX_Check(pDX, IDC_CHECK_MENTAL, mental);
	DDX_Check(pDX, IDC_CHECK_SIGHT, sight);
	DDX_Text(pDX, IDC_EDIT_OTHERS, others);
}


BEGIN_MESSAGE_MAP(CPersonalInfoDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPersonalInfoDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CPersonalInfoDialog message handlers


void CPersonalInfoDialog::SetInfo(const PersonalInfo& info)
{
	name = info.name;
	pinyin = info.name_pinyin;
	nationality = info.nationality;
	birth_date = info.birth_date;
	sex = info.sex;
	weight = info.weight;
	mobile = info.mobile;
	email = info.email;
	mental = info.mental;
	sight = info.sight;
	others = info.others;
}


PersonalInfo CPersonalInfoDialog::GetInfo() const
{
	PersonalInfo info;
	info.name = name;
	info.name_pinyin = pinyin;
	info.nationality = nationality;
	info.birth_date = birth_date;
	info.sex = Sex(sex);
	info.weight = weight;
	info.mobile = mobile;
	info.email = email;
	info.mental = mental;
	info.sight = sight;
	info.others = others;
	return info;
}

void CPersonalInfoDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialogEx::OnOK();
}