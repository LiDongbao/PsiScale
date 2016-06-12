#pragma once
#include <map>
#include "User.h"

class CPsiScale;
namespace Utilities
{
	class CXmlElement;
}

struct Answer
{
	unsigned answer;
	long time;
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
	CAnswerManager();
	~CAnswerManager();

	bool AddAnswer(const CString& scale_name, unsigned question_id, unsigned answer, unsigned time);
	unsigned GetAnswer(const CString& scale_name, unsigned question_id);
	bool SetScore(const CString& scale_name, const CString& sub_scale_name, unsigned score);
	bool IsAnswered(const CString& scale_name, unsigned question_id);

	void SetSubjectId(const TCHAR* subject_id);
	const CString& GetSubjectId() const;
	int CheckForUnansweredQuestion(CPsiScale& scale);
	unsigned GetTotalScore(const CString& scale_name, const CString& sub_scale_name);
	bool LoadScaleItem(Utilities::CXmlElement* scale_xml);
	bool SaveScaleItem(Utilities::CXmlElement* scale_xml, const CString& scale_name);
	bool Load(const CString& test_info_path, CUser& user);

	bool Save(const CString& test_info_path, CUser& user);
	bool ScaleFinished(const CString& scale_name);
	void FinishScale(const CString& scale_name);

	void SetScaleTime(const CString& scale_name, const CString& date, const CString& time);
	const ScaleTime GetScaleTime(const CString& sacle_name);
private:
	std::map<CString, std::map<unsigned, Answer>> _answers;
	std::map<CString, std::map<CString, unsigned>> _scores; // 用结构是不是更好
	std::map<CString, bool> _test_finished_info;
	std::map<CString, ScaleTime> _scale_time;

	// 被试uid
	CString _subject_uid;
};

