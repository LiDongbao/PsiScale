#pragma once
#include <map>
#include "User.h"
#include <vector>
#include <memory>
#include "UserManager.h"
#include "Scorer.h"

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
	CString date;	//格式是: 2016-01-01
	CString time;	//格式是: 12:01
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

	std::map<CString, double> GetScore(const wchar_t * scale_name, const std::vector<AnswerInfo>& answers);

protected:
	std::map<std::tuple<CString, CString, COleDateTime>, unsigned int> _answer_table_index;	//map< tuple< user_id, scale_name, start_time>,unsigned int answer_index>
	
	std::vector<ScaleAnswers> _answer_table;
	std::vector<bool> _answer_table_finished;

	CScorer _score;
};
