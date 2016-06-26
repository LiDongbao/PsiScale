#include "stdafx.h"
#include "UserManager.h"
#include "..\PsiCommon\User.h"
#include "..\Utilities\macros.h"
#include "..\Utilities\xml.h"
#include "..\Utilities\FileSystem.h"
#include "..\PsiCommon\xml_name_space.h"
#include <algorithm>


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
	_logon_info_path = FileSystem::GetStartPath() + _T("\\..\\Scales\\TestUsers\\UserInfo.xml");
	
	//\\ = start_parth.Left(start_parth.ReverseFind(_T('\\'))) ;
}


CUserManager::~CUserManager()
{
}

bool CUserManager::Init()
{
	CString folder_path = _logon_info_path.Left(_logon_info_path.ReverseFind(_T('\\')));
	if (!FileSystem::FileExists(folder_path) && !FileSystem::CreateFolderTree(folder_path))
	{
		// log
		return false;
	}
	
	return LoadLogonInfo();
}

shared_ptr<CUser> CUserManager::CreateUser(const CString& user_id, 
	const CString& password)
{
	if (UserExists(user_id, password))
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
	auto iter = _users.find(name);
	return (iter != _users.end() && iter->second && iter->second->GetPassword() == password);
}

std::shared_ptr<CUser> CUserManager::GetUser(const CString& user_id, 
	const CString& password)
{
	auto uid = _user_name_to_uid.find(user_id);
	
	auto iter = _users.find(uid->second);
	if (iter != _users.end())
	{
		assert(iter->second);
		if (iter->second->GetPassword() == password)
		{
			return iter->second;
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

	_users.insert(make_pair(user->GetUid(), user));
	_user_name_to_uid.insert(make_pair(user->GetUserId(), user->GetUid()));
	_users[user->GetUid()] = user;

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
		user_info_element->SetAttrib(XML_USER_UID, user.second->GetUid());
	}

	return users_info.Save(_logon_info_path);
}

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