#pragma once
#include <memory>
#include <map>

class CUser;

class CUserManager
{
public:
	~CUserManager();

	bool Init();
	std::shared_ptr<CUser> CreateUser(const CString& name, const CString& password);
	std::shared_ptr<CUser> GetUser(const CString& name, const CString& password);
	std::shared_ptr<CUser> GetUser(const wchar_t * uid);
	bool LoadLogonInfo();
	bool SaveLogonInfo();

	static CUserManager& GetInstance();
	const std::map<CString, std::shared_ptr<CUser>>& Users() const;

private:
	CUserManager();
	bool AddUser(std::shared_ptr<CUser> user);

	bool UserExists(const CString& name, const CString& password);
	const CString GetWorkingFolder() const;

	std::map<CString, std::shared_ptr<CUser>> _users;		//_users  :  map<Uid ,shared_ptr<CUser>>
	std::multimap<CString, std::shared_ptr<CUser>>_userid_user; //_userid_user	:	map<Userid , shared_ptr<CUser>>
	std::multimap<CString, CString> _user_id_to_uid;	
	CString _logon_info_path;
	bool _initialized;

	static CUserManager s_instance;
};

