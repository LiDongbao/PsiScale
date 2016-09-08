#include "stdafx.h"
#include "AnswerManager.h"
#include "PsiScale.h"
#include <algorithm>
#include "..\Utilities\xml.h"
#include "xml_name_space.h"
#include <utility>
#include "..\Utilities\macros.h"
#include "..\Utilities\FileSystem.h"
#include "Scorer.h"

using namespace XMLNameSpace;
using namespace std;

int CAnswerManager::GetAnswerIndex(const TCHAR* user_uid, 
								   const TCHAR* scale, 
								   COleDateTime start_time,
								   bool create_if_not_exist /*= true*/)
{
	auto key = std::make_tuple(user_uid, scale, start_time);
	auto iter = _answer_table_index.find(key);
	if (iter != _answer_table_index.end()) 
	{
		return iter->second;
	}
	else if (create_if_not_exist)
	{
		_answer_table.push_back(ScaleAnswers(user_uid, scale, start_time));
		_answer_table_index[key] = _answer_table.size() - 1;

		return _answer_table.size() - 1;
	}
	else
	{
		return -1;
	}
}

int CAnswerManager::AllocateForAnswers(const TCHAR* user_uid, const TCHAR* scale, COleDateTime start_time, unsigned length)
{
	_answer_table_index.insert(std::make_pair(std::make_tuple(user_uid, scale, start_time),
		_answer_table.size() - 1));
	return _answer_table.size() - 1;
}

bool CAnswerManager::IsAnswered(unsigned index, unsigned int question_id)
{
	return _answer_table[index].answer_info[question_id].answer != -1;
}

unsigned int CAnswerManager::GetAnswer(unsigned index, unsigned int question_id)
{
	return _answer_table[index].answer_info[question_id].answer;
}

void CAnswerManager::AddAnswer(unsigned index, unsigned int question_index, unsigned int answer, unsigned int reaction_time)
{
	ASSERT(question_index >= 0 && question_index < _answer_table[index].answer_info.size());
	_answer_table[index].answer_info[question_index].reaction_time = reaction_time;
	_answer_table[index].answer_info[question_index].answer = answer;
}

int CAnswerManager::CheckForUnansweredQuestion(unsigned index)
{
	assert(index < _answer_table.size());

	return _answer_table[index].GetFirstUnansweredQuenstion();
}

bool CAnswerManager::ScaleFinished(unsigned index)
{
	assert(index < _answer_table.size());

	return _answer_table[index].IsFinished();
}

const ScaleAnswers& CAnswerManager::GetScaleAnswers(unsigned int index)
{
	return _answer_table[index];
}

COleDateTime CAnswerManager::FindTimeByUserScale(const TCHAR* user_uid, const TCHAR* scale)
{
	for (auto iter = _answer_table_index.begin(); iter != _answer_table_index.end(); ++iter)
	{
		if (get<0>(iter->first) == user_uid && get<1>(iter->first) == scale)
		{
			return get<2>(iter->first);
		}
	}

	return COleDateTime::GetCurrentTime();
}

std::vector<int> CAnswerManager::GetIndexByUser(const TCHAR* user_uid)
{
	vector<int> saved_index;
	for (auto iter = _answer_table_index.begin(); iter != _answer_table_index.end(); ++iter)
	{
		if (get<0>(iter->first) == user_uid)
		{
			saved_index.push_back(iter->second);
		}
	}

	return saved_index;
}

std::vector<int> CAnswerManager::GetIndexByScale(const TCHAR* scale)
{
	vector<int> saved_index;
	for (auto iter = _answer_table_index.begin(); iter != _answer_table_index.end(); ++iter)
	{
		if (get<1>(iter->first) == scale)
		{
			saved_index.push_back(iter->second);
		}
	}

	return saved_index;
}

bool CAnswerManager::Load(const CString& test_info_path)
{
	Utilities::CXml xml;
	if (!xml.Load(test_info_path))
	{
		return false;
	}

	auto user_item = xml.GetElement(XML_USER_INFO);
	if (user_item == nullptr)
		return false;

	// user->SetUid(temp_user->GetAttrib(XML_USER_UID));
	auto user = CUserManager::GetInstance().GetUser(user_item->GetAttrib(XML_USER_UID));
	if (!user)
	{
		user = CUserManager::GetInstance().CreateUser(user_item->GetAttrib(XML_USER_USERID),
			user_item->GetAttrib(XML_USER_PASSWORD));
	}

	PersonalInfo info;
	info.name = user_item->GetAttrib(XML_USER_NAME);
	info.name_pinyin = user_item->GetAttrib(XML_USER_PINYIN);
	info.nationality = user_item->GetAttrib(XML_USER_NATIONALITY);
	info.birth_date = user_item->GetOleDateTimeAttrib(XML_USER_BIRTHDATE);
	info.sex = Sex(user_item->GetIntegerAttrib(XML_USER_SEX));
	info.weight = user_item->GetIntegerAttrib(XML_USER_WEIGHT);
	info.mobile = user_item->GetAttrib(XML_USER_MOBILE);
	info.email = user_item->GetAttrib(XML_USER_EMAIL);
	info.mental = user_item->GetIntegerAttrib(XML_USER_MENTAL);
	info.sight = user_item->GetIntegerAttrib(XML_USER_SIGHT);
	info.others = user_item->GetAttrib(XML_USER_OTHERS);
	user->SetInfo(info);

	auto scales = xml.GetChildElements();
	for (unsigned int i = 0; i < scales.size(); ++i)  
	{
		auto scale_xml = scales[i];
		if ((scale_xml->GetName()) != XML_USER_INFO)
		{
			ScaleAnswers scale_answers;
			scale_answers.user_uid = user->GetUid();
			scale_answers.scale_name = scale_xml->GetAttrib(XML_TEST_NAME);
			scale_answers.start_time = scale_xml->GetOleDateTimeAttrib(XML_TEST_START_TIME);

			_answer_table_finished.push_back((scale_xml->GetBoolAttrib(XML_TEST_FINISHED) == 0) ? false : true);

			LoadScaleItem(scale_xml, scale_answers);
			_answer_table.push_back(scale_answers);
			_answer_table_index.insert(make_pair(make_tuple(scale_answers.user_uid, scale_answers.scale_name, scale_answers.start_time), 
				_answer_table.size() - 1));
		}
	}

	return true;
}

void CAnswerManager::LoadScaleItem(Utilities::CXmlElement* scale_xml, OUT ScaleAnswers& scale_answer)
{
	auto answers_element = scale_xml->GetElement(XML_TEST_ANSWERS);
	auto answer_items = answers_element->GetChildElements();
	
	scale_answer.answer_info.resize(answer_items.size());
	
	for (auto item : answer_items)
	{
		unsigned id = item->GetIntegerAttrib(XML_TEST_ID);	// id 应该是从1开始
		if (id > 0 && id <= answer_items.size())
		{
			scale_answer.answer_info[id - 1].answer = item->GetIntegerAttrib(XML_TEST_ANSWER);
			scale_answer.answer_info[id - 1].reaction_time = item->GetIntegerAttrib(XML_TEST_TIME);
		}
	}
}

bool CAnswerManager::Save(const CString& test_info_path, const TCHAR* user_uid)
{
	 Utilities::CXml xml(XML_TEST);

	 auto user = CUserManager::GetInstance().GetUser(user_uid);
	 if (!user)
		 return false;


	 // 在答案中保存用户信息
	 auto user_info_xml = xml.AddElement(XML_USER_INFO);
	 user_info_xml->SetAttrib(XML_USER_USERID, user->GetUserId());
	 user_info_xml->SetAttrib(XML_USER_PASSWORD, user->GetPassword());
	 user_info_xml->SetAttrib(XML_USER_UID, user->GetUid());
	 user_info_xml->SetAttrib(XML_USER_NAME, user->GetInfo().name);
	 user_info_xml->SetAttrib(XML_USER_PINYIN, user->GetInfo().name_pinyin);
	 user_info_xml->SetAttrib(XML_USER_NATIONALITY, user->GetInfo().nationality);
	 user_info_xml->SetOleDateTimeAttrib(XML_USER_BIRTHDATE, user->GetInfo().birth_date);
	 user_info_xml->SetIntegerAttrib(XML_USER_SEX, user->GetInfo().sex);
	 user_info_xml->SetIntegerAttrib(XML_USER_WEIGHT, user->GetInfo().weight);
	 user_info_xml->SetAttrib(XML_USER_MOBILE, user->GetInfo().mobile);
	 user_info_xml->SetAttrib(XML_USER_EMAIL, user->GetInfo().email);
	 user_info_xml->SetIntegerAttrib(XML_USER_MENTAL, user->GetInfo().mental);
	 user_info_xml->SetIntegerAttrib(XML_USER_SIGHT, user->GetInfo().sight);
	 user_info_xml->SetAttrib(XML_USER_OTHERS, user->GetInfo().others);
	 
	 vector<int> saved_index = GetIndexByUser(user->GetUid());

	 for (auto iter = saved_index.begin(); iter != saved_index.end(); ++iter)
	 {
	 	auto scale_xml = xml.AddElement(XML_TEST_SCALE);
	 	scale_xml->SetAttrib(XML_TEST_NAME, _answer_table[*iter].scale_name);
	 	scale_xml->SetOleDateTimeAttrib(XML_TEST_START_TIME, _answer_table[*iter].start_time);
		scale_xml->SetBoolAttrib(XML_TEST_FINISHED, _answer_table_finished[*iter]);
	 	
	 	this->SaveScaleItem(scale_xml, *iter);
	 }
	 
	 return xml.Save(test_info_path);
}

void CAnswerManager::SaveScaleItem(Utilities::CXmlElement* scale_xml, unsigned index)
{
	auto answers_xml = scale_xml->AddElement(XML_TEST_ANSWERS);
	auto answers = _answer_table[index].answer_info;
	for (unsigned i = 0; i < answers.size(); ++i)
	{
		auto item = answers_xml->AddElement(XML_TEST_ITEM);
		item->SetIntegerAttrib(XML_TEST_ID, i + 1);
		item->SetIntegerAttrib(XML_TEST_ANSWER, answers[i].answer);
		item->SetIntegerAttrib(XML_TEST_TIME, answers[i].reaction_time);
	}

	auto group_scores = GetScore(_answer_table[index].scale_name, answers);

	if (!group_scores.empty())
	{
		auto subscales_xml = scale_xml->AddElement(XML_TEST_SUBSCALES);
		for (auto group : group_scores)
		{
			auto item = subscales_xml->AddElement(XML_TEST_SUBSCALE);
			item->SetAttrib(XML_TEST_NAME, group.first.c_str());
			item->SetFloatAttrib(XML_TEST_SCORE, group.second);
		}
	}
}

bool CAnswerManager::LoadAll(CString folder_path)
{
	if (FileSystem::FileExists(folder_path))
	{
		std::vector<CString> files;
		FileSystem::ForEachFile(folder_path, _T("*.xml"), false, [&](const CString& file) {
			CString filename = FileSystem::GetFileNameFromPath(file);
			Load(folder_path + _T("\\") + filename + _T(".xml"));
		});
		return true;
	}
	else
	{
		return false;
	}
}

std::map<std::wstring, double> CAnswerManager::GetScore(const wchar_t * scale_name, 
	const std::vector<AnswerInfo>& answers)
{
	std::map<std::wstring, double> result;
	auto score_matrix = CScorer::GetInstance().GetScoreMatrix(scale_name);
	if (score_matrix == nullptr)
		return result;

	auto& groups = score_matrix->GetGroups();
	for (auto group : groups)
	{
		result[group] = 0.0;
	}

	for (unsigned int question_index = 0; question_index < answers.size(); ++question_index)
	{
		for (unsigned int group_index = 0; group_index < groups.size(); ++group_index)
		{
			result[groups[group_index]] += score_matrix->GetWeight(question_index, 
				answers[question_index].answer, group_index);
		}
	}

	return result;
}

ScaleAnswers & CAnswerManager::GetAnswers(const TCHAR * user_uid, const TCHAR * scale, COleDateTime start_time)
{
	auto index = GetAnswerIndex(user_uid, scale, start_time, true);
	assert(index < _answer_table.size());

	return _answer_table[index];
}


int ScaleAnswers::GetFirstUnansweredQuenstion() const
{
	int question_index = 0;
	for (int i = 0; i < answer_info.size(); ++i)
	{
		if (-1 == answer_info[i].answer)
		{
			return i;
		}
	}

	return -1;
}

bool ScaleAnswers::IsFinished() const
{
	return GetFirstUnansweredQuenstion() == -1;
}

void ScaleAnswers::Resize(size_t size)
{
	answer_info.resize(size);
}
