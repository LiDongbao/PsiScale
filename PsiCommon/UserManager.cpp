#include "stdafx.h"
#include "UserManager.h"
#include "..\PsiCommon\User.h"
#include "..\Utilities\macros.h"
#include "..\Utilities\xml.h"
#include "..\Utilities\FileSystem.h"
#include "..\PsiCommon\xml_name_space.h"
#include <algorithm>
#include <cassert>

//const TCHAR* TESTS = _T("Tests");
//const TCHAR* TABLE = _T("Table");
//const TCHAR* ID = _T("Id");
//const TCHAR* PATH = _T("Path");

using namespace std;
using namespace XMLNameSpace;

CUserManager CUserManager::s_instance;

CUserManager::CUserManager() :
	_initialized(false)
{
	CRegKey regkey;
	if (regkey.Open(HKEY_CURRENT_USER, _T("Software\\SKMR\\PsiScale")) == ERROR_SUCCESS ||	//default is KEY_ALL_ACCESS, KEY_WRITE can only write, KEY_READ only read.
		regkey.Create(HKEY_CURRENT_USER, _T("Software\\SKMR\\PsiScale")) == ERROR_SUCCESS)
	{
		static TCHAR buffer[512];
		ULONG count = 512;
		if (regkey.QueryStringValue(_T("WorkingFolder"), buffer, &count) == ERROR_SUCCESS)
		{
			if (!FileSystem::FileExists(buffer))
			{
				if (!FileSystem::CreateFolderTree(buffer))
					AfxMessageBox(L"无法直接打开和创建工作目录！", MB_ICONSTOP);
				else
				{
					CString s;
					s.Format(_T("无工作目录！！但已经自动为您创建工作目录：%s，如需改动请在答题前打开PsiScaleEdit程序修改"), buffer);
					AfxMessageBox(s);
				}
			}
			_logon_info_path = buffer;
		}
		else
		{
			CString path = L"C:\\SKMR\\PsiScale\\Scales";
			regkey.SetStringValue(_T("WorkingFolder"), path);
			CString s;
			s.Format(_T("无工作目录！！但已经自动为您创建工作目录：%s，如需改动请在答题前打开PsiScaleEdit程序修改"), path);
			AfxMessageBox(s);
			_logon_info_path = path;
		}
		regkey.Close();
	}
	
	_logon_info_path = _logon_info_path + _T("\\..\\Scales\\TestUsers\\UserInfo.xml");

}


CUserManager::~CUserManager()
{
}

bool CUserManager::Init()
{
	CString folder_path = _logon_info_path.Left(_logon_info_path.ReverseFind(_T('\\')));
	if (!FileSystem::FileExists(folder_path) && !FileSystem::CreateFolderTree(folder_path))
		return false;
	
	return LoadLogonInfo();
}

shared_ptr<CUser> CUserManager::CreateUser(const CString& user_id, 
	const CString& password)
{
	auto isExist = UserExists(user_id, password);
	if (isExist)
	{
		return shared_ptr<CUser>();
	}

	try
	{
		shared_ptr<CUser> user(new CUser(user_id, password));
		user->SetWorkingFolder(GetWorkingFolder());

		AddUser(user);

		return user;
	}
	catch (bad_alloc&)
	{
		return shared_ptr<CUser>();
	}
}

bool CUserManager::UserExists(const CString& name, const CString& password)
{
	for (auto iter : _userid_user)
	{
		if (iter.first == name && iter.second && iter.second->GetPassword() == password)
		{
			return true;
		}
	}
	return false;
}

std::shared_ptr<CUser> CUserManager::GetUser(const CString& user_id, 
	const CString& password)
{
	for (auto it : _userid_user)
	{
		if (it.first == user_id)
		{
			assert(it.second);
			if (it.second->GetPassword() == password)
			{
				return it.second;
			}
		}
	}
	return shared_ptr<CUser>();
}

std::shared_ptr<CUser> CUserManager::GetUser(const wchar_t * uid)
{
	auto iter = _users.find(CString(uid));
	return (iter != _users.end()) ? iter->second : std::shared_ptr<CUser>();
}

bool CUserManager::AddUser(std::shared_ptr<CUser> user)
{
	if (!user)
		return false;

	if (GetUser(user->GetUid()))
		return false;

	_userid_user.insert(make_pair(user->GetUserId(), user));
	_users.insert(make_pair(user->GetUid(), user));
	_user_id_to_uid.insert(make_pair(user->GetUserId(), user->GetUid()));

	return true;
}

bool CUserManager::LoadLogonInfo()
{
	Utilities::CXml users_info;
	if (!users_info.Load(_logon_info_path))
	{
		return false;
	}

	try
	{
		for (auto item : users_info.GetChildElements())
		{
			std::shared_ptr<CUser> user(new CUser(item->GetAttrib(XML_USER_USERID), item->GetAttrib(XML_USER_PASSWORD)));
			user->SetUid(item->GetAttrib(XML_USER_UID));
			user->SetWorkingFolder(CString(GetWorkingFolder()));

			AddUser(user);
		}
	}
	catch (std::bad_alloc&)
	{
		return false;
	}

	return true;
}

bool CUserManager::SaveLogonInfo()
{
	Utilities::CXml users_info(XML_USERS_INFO);

	for (auto user : _users)
	{
		auto user_info_element = users_info.AddElement(XML_USER_INFO);
		user_info_element->SetAttrib(XML_USER_USERID, user.second->GetUserId());
		user_info_element->SetAttrib(XML_USER_PASSWORD, user.second->GetPassword());
		user_info_element->SetAttrib(XML_USER_UID, user.first);
	}

	return users_info.Save(_logon_info_path);
}

//静态对象，首先调用GetInstance()函数初始化静态对象。
CUserManager& CUserManager::GetInstance()
{
	if (!s_instance._initialized)
	{
		s_instance._initialized = s_instance.Init();
	}
	return s_instance;
}

const std::map<CString, std::shared_ptr<CUser>>& CUserManager::Users() const
{
	return _users;
}

const CString CUserManager::GetWorkingFolder() const
{
	CString folder_path = _logon_info_path.Left(_logon_info_path.ReverseFind(_T('\\'))) + _T("\\Answers");
	if (!FileSystem::FileExists(folder_path) && !FileSystem::CreateFolderTree(folder_path))
	{
		//log
		return nullptr;
	}

	return folder_path;
}