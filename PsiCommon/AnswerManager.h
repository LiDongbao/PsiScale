#pragma once
#include <map>
#include "User.h"
#include <vector>
#include <memory>

class CPsiScale;
namespace Utilities
{
	class CXmlElement;
}


struct AnswerInfo
{
	int answer;
	long reaction_time;
	AnswerInfo(int answer_, long time_) : answer(answer_), reaction_time(time_) {}
	AnswerInfo() : answer(-1), reaction_time(0) {}
};

struct ScaleAnswers
{
	ScaleAnswers() {}
	ScaleAnswers(CString user_uid_, CString scale_name_, COleDateTime time, unsigned length):
	user_uid(user_uid_), scale_name(scale_name_), start_time(time)
	{
		answer_info.resize(length);
	}

	CString user_uid;
	CString scale_name;
	COleDateTime start_time;
	std::vector<AnswerInfo> answer_info;
};

struct ScaleTime
{
	ScaleTime(const CString& d, const CString& t): date(d), time(t) {}
	CString date;	//2016-01-01
	CString time;	//12:01
};

class CAnswerManager
{
public:
	CAnswerManager() {}
	~CAnswerManager() {}

	int GetAnswerIndex(const TCHAR* user_uid, const TCHAR* scale, COleDateTime start_time, bool create_if_not_exist = true);
	int AddScaleAnswer(const TCHAR* user_uid, const TCHAR* scale, COleDateTime start_time, unsigned length);
	bool IsAnswered(unsigned index, unsigned int question_id);
	unsigned int GetAnswer(unsigned index, unsigned int question_id);
	void AddAnswer(unsigned index, unsigned int question_index, unsigned int answer, unsigned int reaction_time);
	int CheckForUnansweredQuestion(unsigned index);
	bool ScaleFinished(unsigned index);
	void SetScaleFinished(unsigned int index, bool state);
	const ScaleAnswers& GetScaleAnswers(unsigned int index);

	COleDateTime FindTimeByUserScale(const TCHAR* user_uid, const TCHAR* scale);

	std::vector<int> GetIndexByUser(const TCHAR* user_uid);
	std::vector<int> GetIndexByScale(const TCHAR* scale);

	bool Load(const CString& test_info_path);
	void LoadScaleItem(Utilities::CXmlElement* scale_xml, OUT ScaleAnswers& scale_answer);
	bool Save(const CString& test_info_path, const TCHAR* user_uid);
	void SaveScaleItem(Utilities::CXmlElement* scale_xml, unsigned index);
	bool LoadAll(CString folder_path);

	bool SetUser(CString user_uid, CUser& user);
	std::shared_ptr<CUser> GetUser(const CString user_uid);
	std::map<CString, std::shared_ptr<CUser>> GetAllUsers();


protected:
	std::map<std::tuple<CString, CString, COleDateTime>, unsigned int> _answer_table_index;
	
	std::vector<ScaleAnswers> _answer_table;
	std::map<CString, std::shared_ptr<CUser>> _users; // user_uid
	std::vector<bool> _answer_table_finished;
};

// class CAnswerManagerOld
// {
// public:
// 	CAnswerManagerOld();
// 	~CAnswerManagerOld();
// 
// 	bool AddAnswer(const CString& scale_name, unsigned question_id, unsigned answer, unsigned time);
// 	unsigned GetAnswer(const CString& scale_name, unsigned question_id);
// 	bool SetScore(const CString& scale_name, const CString& sub_scale_name, unsigned score);
// 	bool IsAnswered(const CString& scale_name, unsigned question_id);
// 
// 	void SetSubjectId(const TCHAR* subject_id);
// 	const CString& GetSubjectId() const;
// 	int CheckForUnansweredQuestion(CPsiScale& scale);
// 	unsigned GetTotalScore(const CString& scale_name, const CString& sub_scale_name);
// 	bool LoadScaleItem(Utilities::CXmlElement* scale_xml);
// 	bool SaveScaleItem(Utilities::CXmlElement* scale_xml, const CString& scale_name);
// 	bool Load(const CString& test_info_path, CUser& user);
// 
// 	bool Save(const CString& test_info_path, CUser& user);
// 	bool ScaleFinished(const CString& scale_name);
// 	void FinishScale(const CString& scale_name);
// 
// 	void SetScaleTime(const CString& scale_name, const CString& date, const CString& time);
// 	const ScaleTime GetScaleTime(const CString& sacle_name);
// private:
// 	std::map<CString, std::vector<AnswerInfo>> _answers;
// 	std::map<CString, std::map<CString, unsigned>> _group_scores; // 用结构是不是更好
// 	std::map<CString, bool> _test_finished_info;
// 	std::map<CString, ScaleTime> _scale_time;
// 
// 	// 被试uid
// 	CString _subject_uid;
// };

