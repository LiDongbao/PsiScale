#include "stdafx.h"
#include "AnswerManager.h"
#include "PsiScale.h"
#include <algorithm>
#include "..\Utilities\xml.h"
#include "xml_name_space.h"
#include <utility>

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
