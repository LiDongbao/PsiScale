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

CUserManager::CUserManager()
{
	_user_info_path = FileSystem::GetStartPath() + _T("\\..\\Scales\\TestUsers\\UserInfo.xml");
	
	//\\ = start_parth.Left(start_parth.ReverseFind(_T('\\'))) ;
}


CUserManager::~CUserManager()
{
}

bool CUserManager::Init()
{
	CString folder_path = _user_info_path.Left(_user_info_path.ReverseFind(_T('\\')));
	if (!FileSystem::FileExists(folder_path) && !FileSystem::CreateFolderTree(folder_path))
	{
		// log
		return false;
	}
	
	return Load();
}

std::shared_ptr<CUser> CUserManager::CreateUser(const CString& name, 
	const CString& password)
{
	if (UserHasExist(name, password))
	{
		return std::shared_ptr<CUser>();
	}

	shared_ptr<CUser> user(new CUser(name, password));
	_users.insert(make_pair(name, user));
	
	_user_name_to_uid.insert(make_pair(name, user->GetUid()));

	return user;
}

bool CUserManager::UserHasExist(const CString& name, const CString& password)
{
	auto iter = _users.find(name);
	return (iter != _users.end() && iter->second && iter->second->GetPassword() == password);
}

std::shared_ptr<CUser> CUserManager::GetUser(const CString& name, const CString& password)
{
	auto iter = _users.find(name);
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

bool CUserManager::Load()
{
	Utilities::CXml users_info;
	if (!users_info.Load(_user_info_path))
	{
		return false;
	}
	auto child_elements = users_info.GetChildElements();
	try
	{
		for (unsigned int i = 0; i < child_elements.size(); ++i)
		{
			auto item = child_elements[i];
			CString name = item->GetAttrib(XML_USER_NAME);
			std::shared_ptr<CUser> user(new CUser(name, item->GetAttrib(XML_USER_PASSWORD)));
			_users.insert(make_pair(name, user));
			CString user_uid = item->GetAttrib(XML_USER_UID);
			user->SetUid(user_uid);
			_user_name_to_uid.insert(make_pair(name, user_uid));
			SetWorkingFolder(user);
		}
	}
	catch (std::bad_alloc&)
	{
		return false;
	}

	return true;
}

bool CUserManager::Save()
{
	Utilities::CXml users_info(XML_USERS_INFO);
	for (auto iter = _users.begin(); iter != _users.end(); ++iter)
	{
		auto user_info_element = users_info.AddElement(XML_USER_INFO);
		user_info_element->SetAttrib(XML_USER_NAME, iter->first);
		user_info_element->SetAttrib(XML_USER_PASSWORD, iter->second->GetPassword());
		user_info_element->SetAttrib(XML_USER_UID, iter->second->GetUid());
	}
	bool result = users_info.Save(_user_info_path);
	return result;
}

void CUserManager::SetWorkingFolder(std::shared_ptr<CUser> user)
{
	CString folder_path = _user_info_path.Left(_user_info_path.ReverseFind(_T('\\'))) + _T("\\Answers");
	if (!FileSystem::FileExists(folder_path) && !FileSystem::CreateFolderTree(folder_path))
	{
		//log
		return;
	}

	user->SetWorkingFolder(folder_path);
}