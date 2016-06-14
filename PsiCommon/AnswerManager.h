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
	CString user_uid;
	CString scale_uid;
	COleDateTime data_time;
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
	int GetAnswerIndex(const TCHAR* user_uid, const TCHAR* scale, COleDateTime start_time, bool create_if_not_exist = true) {
		auto iter = _answer_table_index.find(std::make_tuple(user_uid, scale, start_time));
		if (iter != _answer_table_index.end()) {
			return iter->second;
		}
		else if (create_if_not_exist)
		{
			_answer_table.push_back(ScaleAnswers());
			_answer_table_index.insert(std::make_pair(std::make_tuple(user_uid, scale, start_time),
				_answer_table.size() - 1));
		}
		return -1;
	}

	bool IsAnswered(unsigned index, unsigned int question_id) {
		return _answer_table[index].answer_info[question_id].answer != -1;
	}

	bool Load(const TCHAR* user_uid);
	bool Save(const TCHAR* user_uid);

	bool LoadAll(const TCHAR* folder_path);

protected:
	std::map<std::tuple<CString, CString, COleDateTime>, unsigned int> _answer_table_index;
	std::vector<ScaleAnswers> _answer_table;
	std::map<CString, std::shared_ptr<CUser>> _users; // user_uid
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

