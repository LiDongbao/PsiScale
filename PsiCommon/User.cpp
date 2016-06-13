#include "stdafx.h"
#include "User.h"
#include "..\Utilities\FileSystem.h"

using namespace std;

CUser::CUser(const CString& name, const CString& password) :
	_user_id(name), 
	_password(password)
{
}

CUser::CUser()
{

}

CString CUser::CreateUid()
{
	auto now = COleDateTime::GetCurrentTime();
	return _user_id + now.Format(_T("_%Y%m%d%H%M%S"));
}

const CString& CUser::GetUserId() const
{
	return _user_id;
}

void CUser::SetUserId(const CString& val)
{
	_user_id = val;
}

const CString& CUser::GetPassword() const
{
	return _password;
}

void CUser::SetPassword(const CString& passoword)
{
	_password = passoword;
}

void CUser::SetUid(const CString& uid)
{
	_uid = uid;
}

const CString& CUser::GetUid()
{
	if (_uid.IsEmpty()) {
		_uid = CreateUid();
	}
	return _uid;
}

void CUser::SetWorkingFolder(const CString& folder)
{
	_working_folder = folder;
}

const CString& CUser::GetWorkingFolder() const
{
	return _working_folder;
}

const PersonalInfo& CUser::GetInfo() const
{
	return _info;
}

void CUser::SetInfo(const PersonalInfo& info)
{
	_info = info;
}
