#include "stdafx.h"
#include "AnswerManager.h"
#include "PsiScale.h"
#include <algorithm>
#include "..\Utilities\xml.h"
#include "xml_name_space.h"
#include <utility>
#include "..\Utilities\macros.h"
#include "..\Utilities\FileSystem.h"

using namespace XMLNameSpace;
using namespace std;
// 
// CAnswerManagerOld::CAnswerManagerOld()
// {
// }
// 
// CAnswerManagerOld::~CAnswerManagerOld()
// {
// }
// 
// bool CAnswerManagerOld::AddAnswer(const CString& scale_name, 
// 	unsigned question_id, 
// 	unsigned answer, 
// 	unsigned time)
// {
// 	_test_finished_info[scale_name] = false;
// 	_answers[scale_name][question_id].answer = answer;
// 	_answers[scale_name][question_id].reaction_time = time;
// 	return true;
// }
// 
// unsigned CAnswerManagerOld::GetAnswer(const CString& scale_name, unsigned question_id)
// {
// 	auto table = _answers.find(scale_name);
// 	if (table == _answers.end())
// 	{
// 		throw _T("Table not found.");
// 	}
// 	if (question_id > table->second.size())
// 	{
// 		throw _T("Question not found.");
// 	}
// 	
// 	return table->second[question_id - 1].answer;
// }
// 
// bool CAnswerManagerOld::IsAnswered(const CString& scale_name, unsigned question_id)
// {
// 	auto scale = _answers.find(scale_name);
// 	if (scale == _answers.end())
// 	{
// 		return false;
// 	}
// 
// 	ASSERT(question_id > 0 && question_id <= scale->second.size());
// 	return scale->second[question_id - 1].answer != -1;	// -1, not answered.
// }
// 
// 
// void CAnswerManagerOld::SetSubjectId(const TCHAR* subject_id)
// {
// 	_subject_uid = subject_id;
// }
// 
// const CString& CAnswerManagerOld::GetSubjectId() const
// {
// 	return _subject_uid;
// }
// 
// /**
// @return 如果所有的问题都已经回答，返回-1；否则返回第一个没有回答的题目的索引
// */
// 
// int CAnswerManagerOld::CheckForUnansweredQuestion(CPsiScale& scale)
// {
// 	auto iter = _answers.find(scale.GetName());
// 	if (iter == _answers.end())
// 		return 0;
// 
// 	auto& answers = iter->second;
// 	if (answers.size() == scale.GetQuestionCount())
// 		return -1;
// 
// 	for (int i = 0; i < int(scale.GetQuestionCount()); ++i)
// 	{
// 		if (answers[i].answer == -1)
// 			return i;
// 	}
// 
// 	return -1;
// }
// 
// // 暂时以加法运算进行累加
// bool CAnswerManagerOld::SetScore(const CString& scale_name, const CString& sub_scale_name, unsigned score)
// {
// 	_group_scores[scale_name][sub_scale_name] += score;
// 	return true;
// }
// 
// unsigned CAnswerManagerOld::GetTotalScore(const CString& scale_name, const CString& sub_scale_name)
// {
// 	unsigned sum = 0;
// 	auto iter = _group_scores.find(scale_name);
// 	if (iter != _group_scores.end())
// 	{
// 		std::for_each(iter->second.begin(), iter->second.end(), [&](std::pair<CString, unsigned> item) {
// 			sum = item.second; });
// 	}
// 	return sum;
// }
// 
// // 每个scale绑定一个answermanager，暂时的设计
// bool CAnswerManagerOld::LoadScaleItem(Utilities::CXmlElement* scale_xml)
// {
// 	CString scale_name = scale_xml->GetAttrib(XML_TEST_NAME);
// 
// 	auto answers_element = scale_xml->GetElement(XML_TEST_ANSWERS);
// 	auto answer_items = answers_element->GetChildElements();
// 	std::vector<AnswerInfo> scale_answers(answer_items.size());
// 
// 	for (auto item : answer_items)
// 	{
// 		unsigned id = item->GetIntegerAttrib(XML_TEST_ID);
// 		if (id > 0 && id <= answer_items.size())
// 		{
// 			scale_answers[id - 1].answer = item->GetIntegerAttrib(XML_TEST_ANSWER);
// 			scale_answers[id - 1].reaction_time = item->GetIntegerAttrib(XML_TEST_TIME);
// 		}
// 	}
// 
// 	_answers[scale_name] = scale_answers;
// 
// 	auto subscales_element = scale_xml->GetElement(XML_TEST_SUBSCALES);
// 	auto subscales_items = subscales_element->GetChildElements();
// 	std::map<CString, unsigned int> scores;
// 	for_each(subscales_items.begin(), subscales_items.end(), [&](Utilities::CXmlElement* item) {
// 		scores.insert(std::make_pair(item->GetAttrib(XML_TEST_NAME), item->GetIntegerAttrib(XML_TEST_SCORE))); });
// 	_group_scores[scale_name] = scores;
// 	return true;
// }
// 
// bool CAnswerManagerOld::SaveScaleItem(Utilities::CXmlElement* scale_xml, const CString& scale_name)
// {
// 	auto scale_iter = _answers.find(scale_name);
// 
// 	if (scale_iter != _answers.end())
// 	{		
// 		auto answers_xml = scale_xml->AddElement(XML_TEST_ANSWERS);
// 		for (unsigned int i = 0; i < scale_iter->second.size(); ++i)
// 		{
// 			auto item = answers_xml->AddElement(XML_TEST_ITEM);
// 			item->SetIntegerAttrib(XML_TEST_ID, i + 1);
// 			item->SetIntegerAttrib(XML_TEST_ANSWER, scale_iter->second[i].answer);
// 			item->SetIntegerAttrib(XML_TEST_TIME, scale_iter->second[i].reaction_time);
// 		}
// 	}
// 
// 	auto subscale_iter = _group_scores.find(scale_name);
// 	if (subscale_iter != _group_scores.end())
// 	{
// 		auto subscales_xml = scale_xml->AddElement(XML_TEST_SUBSCALES);
// 		for (auto iter = subscale_iter->second.begin(); iter != subscale_iter->second.end(); ++iter)
// 		{
// 			auto sub_score = GetTotalScore(scale_name, iter->first);
// 
// 			auto item = subscales_xml->AddElement(XML_TEST_SUBSCALE);
// 			item->SetAttrib(XML_TEST_NAME, iter->first);
// 			item->SetIntegerAttrib(XML_TEST_SCORE, sub_score);
// 		}
// 	}
// 
// 	return true;
// }
// 
// bool CAnswerManagerOld::Load(const CString& test_info_path, CUser& user)
// {
// 	Utilities::CXml xml;
// 	if (!xml.Load(test_info_path))
// 	{
// 		return false;
// 	}
// 	_subject_uid = xml.GetAttrib(XML_TEST_PARTICIPANT_UID);
// 	auto temp_user = xml.GetElement(XML_USER_INFO);
// 	if (temp_user != nullptr)
// 	{
// 		user.SetUserId(temp_user->GetAttrib(XML_USER_USERID));
// 		user.SetPassword(temp_user->GetAttrib(XML_USER_PASSWORD));
// 		user.SetUid(temp_user->GetAttrib(XML_USER_UID));
// 		PersonalInfo info;
// 		info.name = temp_user->GetAttrib(XML_USER_NAME);
// 		info.name_pinyin = temp_user->GetAttrib(XML_USER_PINYIN);
// 		info.nationality = temp_user->GetAttrib(XML_USER_NATIONALITY);
// 		info.birth_date = temp_user->GetOleDateTimeAttrib(XML_USER_BIRTHDATE);
// 		info.sex = Sex(temp_user->GetIntegerAttrib(XML_USER_SEX));
// 		info.weight = temp_user->GetIntegerAttrib(XML_USER_WEIGHT);
// 		info.mobile = temp_user->GetAttrib(XML_USER_MOBILE);
// 		info.email = temp_user->GetAttrib(XML_USER_EMAIL);
// 		user.SetInfo(info);
// 	}
// 
// 	auto scales = xml.GetChildElements();
// 	for (unsigned int i = 0; i < scales.size(); ++i)  // 从1开始是因为第一个是被试信息
// 	{
// 		auto scale_xml = scales[i];
// 		if ((scale_xml->GetName()) != XML_USER_INFO)
// 		{
// 			auto name = scale_xml->GetAttrib(XML_TEST_NAME);
// 			if (scale_xml->GetIntegerAttrib(XML_TEST_FINISHED) == 0)
// 			{
// 				_test_finished_info.insert(std::make_pair(name, false));
// 			}
// 			else
// 			{
// 				_test_finished_info.insert(std::make_pair(name, true));
// 				_scale_time.insert(std::make_pair(name, ScaleTime(scale_xml->GetAttrib(XML_TEST_FINISHED_DATE), scale_xml->GetAttrib(XML_TEST_FINISHED_TIME))));
// 			}
// 			LoadScaleItem(scale_xml);
// 		}
// 	}
// 
// 	return true;
// }
// 
// bool CAnswerManagerOld::Save(const CString& test_info_path, CUser& user)
// {
// 	Utilities::CXml xml(XML_TEST);
// 	xml.SetAttrib(XML_TEST_PARTICIPANT_UID, _subject_uid);
// 
// 	// 在答案中保存用户信息
// 	auto user_info_xml = xml.AddElement(XML_USER_INFO);
// 	user_info_xml->SetAttrib(XML_USER_USERID, user.GetUserId());
// 	user_info_xml->SetAttrib(XML_USER_PASSWORD, user.GetPassword());
// 	user_info_xml->SetAttrib(XML_USER_UID, user.GetUid());
// 	user_info_xml->SetAttrib(XML_USER_NAME, user.GetInfo().name);
// 	user_info_xml->SetAttrib(XML_USER_PINYIN, user.GetInfo().name_pinyin);
// 	user_info_xml->SetAttrib(XML_USER_NATIONALITY, user.GetInfo().nationality);
// 	user_info_xml->SetOleDateTimeAttrib(XML_USER_BIRTHDATE, user.GetInfo().birth_date);
// 	user_info_xml->SetIntegerAttrib(XML_USER_SEX, user.GetInfo().sex);
// 	user_info_xml->SetIntegerAttrib(XML_USER_WEIGHT, user.GetInfo().weight);
// 	user_info_xml->SetAttrib(XML_USER_MOBILE, user.GetInfo().mobile);
// 	user_info_xml->SetAttrib(XML_USER_EMAIL, user.GetInfo().email);
// 
// 	for (auto iter = _test_finished_info.begin(); iter != _test_finished_info.end(); ++iter)
// 	{
// 		auto scale_xml = xml.AddElement(XML_TEST_SCALE);
// 		scale_xml->SetAttrib(XML_TEST_NAME, iter->first);
// 		scale_xml->SetIntegerAttrib(XML_TEST_FINISHED, iter->second);
// 		
// 		auto scale_time = _scale_time.find(iter->first);
// 		if (scale_time !=_scale_time.end())
// 		{
// 			scale_xml->SetAttrib(XML_TEST_FINISHED_DATE, scale_time->second.date);
// 			scale_xml->SetAttrib(XML_TEST_FINISHED_TIME, scale_time->second.time);
// 		}
// 		this->SaveScaleItem(scale_xml, iter->first);
// 	}
// 
// 	return xml.Save(test_info_path);
// }
// 
// bool CAnswerManagerOld::ScaleFinished(const CString& scale_name)
// {
// 	auto iter = _test_finished_info.find(scale_name);
// 	if (iter != _test_finished_info.end())
// 	{
// 		return iter->second;
// 	}
// 
// 	return false;
// }
// 
// void CAnswerManagerOld::FinishScale(const CString& scale_name)
// {
// 	_test_finished_info[scale_name] = true;
// }
// 
// void CAnswerManagerOld::SetScaleTime(const CString& scale_name, const CString& date, const CString& time)
// {
// 	ScaleTime scale_time(date, time);
// 	auto iter = _scale_time.find(scale_name);
// 	if (iter == _scale_time.end())
// 	{
// 		_scale_time.insert(make_pair(scale_name, scale_time));
// 	}
// 
// }
// 
// const ScaleTime CAnswerManagerOld::GetScaleTime(const CString& scale_name)
// {
// 	auto iter = _scale_time.find(scale_name);
// 	if (iter != _scale_time.end())
// 	{
// 		return iter->second;
// 	}
// 	else
// 	{
// 		return ScaleTime(_T(""), _T(""));
// 	}
// }

int CAnswerManager::GetAnswerIndex(const TCHAR* user_uid, const TCHAR* scale, COleDateTime start_time, bool create_if_not_exist /*= true*/)
{
	auto iter = _answer_table_index.find(std::make_tuple(user_uid, scale, start_time));
	if (iter != _answer_table_index.end()) {
		return iter->second;
	}

	return -1;
}

int CAnswerManager::AddScaleAnswer(const TCHAR* user_uid, const TCHAR* scale, COleDateTime start_time, unsigned length)
{
	_answer_table.push_back(ScaleAnswers(user_uid, scale, start_time, length));
	_answer_table_finished.push_back(false);
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
	if (-1 == index)
	{
		return 0;
	}

	auto answer_info = _answer_table[index].answer_info;
	int question_index = 0;
	for (auto iter = answer_info.begin(); iter != answer_info.end(); ++iter, ++question_index)
	{
		if (-1 == iter->answer)
		{
			return question_index;
		}
	}
	return -1;
}

bool CAnswerManager::ScaleFinished(unsigned index)
{
	if (-1 == index)
	{
		return false;
	}

	return _answer_table_finished[index];
}

void CAnswerManager::SetScaleFinished(unsigned int index, bool state)
{
	if (index != -1 && index < _answer_table_finished.size())
	{
		_answer_table_finished[index] = state;
	}
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

	auto user = shared_ptr<CUser>(new CUser);

	auto temp_user = xml.GetElement(XML_USER_INFO);
	if (temp_user != nullptr)
	{
		user->SetUserId(temp_user->GetAttrib(XML_USER_USERID));
		user->SetPassword(temp_user->GetAttrib(XML_USER_PASSWORD));
		user->SetUid(temp_user->GetAttrib(XML_USER_UID));
		PersonalInfo info;
		info.name = temp_user->GetAttrib(XML_USER_NAME);
		info.name_pinyin = temp_user->GetAttrib(XML_USER_PINYIN);
		info.nationality = temp_user->GetAttrib(XML_USER_NATIONALITY);
		info.birth_date = temp_user->GetOleDateTimeAttrib(XML_USER_BIRTHDATE);
		info.sex = Sex(temp_user->GetIntegerAttrib(XML_USER_SEX));
		info.weight = temp_user->GetIntegerAttrib(XML_USER_WEIGHT);
		info.mobile = temp_user->GetAttrib(XML_USER_MOBILE);
		info.email = temp_user->GetAttrib(XML_USER_EMAIL);
		user->SetInfo(info);
	}
	_users.insert(make_pair(user->GetUid(), user));

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
			_answer_table_index.insert(make_pair(make_tuple(scale_answers.user_uid, scale_answers.scale_name, scale_answers.start_time), _answer_table.size() - 1));
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

	TODO(计分还没处理);
}

bool CAnswerManager::Save(const CString& test_info_path, const TCHAR* user_uid)
{
	 Utilities::CXml xml(XML_TEST);

	 auto iter = _users.find(user_uid);
	 if (iter == _users.end())
	 {
		 return false;
	 }
	 auto user = iter->second;
	 
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

	TODO(计分)
	// 	auto subscale_iter = _group_scores.find(scale_name);
	// 	if (subscale_iter != _group_scores.end())
	// 	{
	// 		auto subscales_xml = scale_xml->AddElement(XML_TEST_SUBSCALES);
	// 		for (auto iter = subscale_iter->second.begin(); iter != subscale_iter->second.end(); ++iter)
	// 		{
	// 			auto sub_score = GetTotalScore(scale_name, iter->first);
	// 
	// 			auto item = subscales_xml->AddElement(XML_TEST_SUBSCALE);
	// 			item->SetAttrib(XML_TEST_NAME, iter->first);
	// 			item->SetIntegerAttrib(XML_TEST_SCORE, sub_score);
	// 		}
	// 	}
	// 
	// 	return true;
}

bool CAnswerManager::SetUser(CString user_uid, CUser& user)
{
	auto user_iter = _users.find(user_uid);
	if (user_iter != _users.end())
	{
		user_iter->second->SetInfo(user.GetInfo());
		return false;
	}
	
	_users.insert(make_pair(user_uid, shared_ptr<CUser>(&user)));

	return true;
}

std::shared_ptr<CUser> CAnswerManager::GetUser(const CString user_uid)
{
	auto user = _users.find(user_uid);
	if (user != _users.end())
	{
		return user->second;
	}
	return nullptr;
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
	}

	return true;
}
