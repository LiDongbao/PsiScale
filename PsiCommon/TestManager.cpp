#include "stdafx.h"
#include "TestManager.h"

#include "../Utilities/xml.h"

using namespace Utilities;
using namespace std;


const TCHAR * XML_ID = _T("Id");
const TCHAR * XML_NAME = _T("Name");
const TCHAR * XML_DESCRIPTION = _T("Description");
const TCHAR * XML_PROLOGUE = _T("Prologue");
const TCHAR * XML_GROUPS = _T("Groups");
const TCHAR * XML_QUESTIONS = _T("Questions");
const TCHAR * XML_TEXT = _T("Text");
const TCHAR * XML_LEVEL_COUNT = _T("LevelCount");
const TCHAR * XML_REVERSE_SCORE = _T("ReverseScore");
const TCHAR * XML_GROUP_ID = _T("GroupId");
const TCHAR * XML_CHOICES = _T("Choices");
const TCHAR * XML_SAME_CHOICES = _T("SameChoices");

CTestManager::CTestManager()
{
}


CTestManager::~CTestManager()
{
}

bool CTestManager::LoadPsiScale(const CString& file_path)
{
	CXml xml;
	if (!xml.Load(file_path))
		return false;

	PsiScale scale;
	scale.SetId(xml.GetIntegerAttrib(XML_ID));
	scale.SetName(xml.GetAttrib(XML_NAME));
	scale.SetDescription(xml.GetAttrib(XML_DESCRIPTION));

	auto prologue_element = xml.GetElement(XML_PROLOGUE);
	if (prologue_element == nullptr)
		return false;

	scale.SetPrologue(prologue_element->GetAttrib(XML_TEXT));

	auto groups_element = xml.GetElement(XML_GROUPS);
	if (groups_element == nullptr)
		return false;

	auto& group_items = groups_element->GetChildElements();
	for (auto item : group_items)
	{
		PsiScaleGroup group;
		group.id = item->GetIntegerAttrib(XML_ID);
		group.description = item->GetAttrib(XML_DESCRIPTION);
		scale.AddGroup(group);
	}

	bool same_choices = xml.GetBoolAttrib(XML_SAME_CHOICES);
	if (same_choices)
	{
		auto choices_element = xml.GetElement(XML_CHOICES);
		if (choices_element == nullptr)
			return false;

		for (auto item : choices_element->GetChildElements())
		{
			QuestionChoice choice;
			choice.id = item->GetIntegerAttrib(XML_ID);
			choice.text = item->GetAttrib(XML_TEXT);
			scale.Choices().push_back(choice);
		}
	}

	auto questions_element = xml.GetElement(XML_QUESTIONS);
	if (questions_element == nullptr)
		return false;

	auto& question_items = questions_element->GetChildElements();
	for (auto item : question_items)
	{
		PsiScaleQuestion question(item->GetIntegerAttrib(XML_ID), 
			item->GetAttrib(XML_TEXT), 
			item->GetBoolAttrib(XML_REVERSE_SCORE),
			item->GetIntegerAttrib(XML_GROUP_ID));

		scale.AddQuestion(question);
	}

	_scales.insert(make_pair(scale.GetId(), scale));

	return true;
}

PsiScale & CTestManager::GetPsiScale(unsigned id)
{
	auto iter = _scales.find(id);
	if (iter != _scales.end())
	{
		return iter->second;
	}
	else
	{
		throw CString(_T("����δ�ҵ���"));
	}
}

const PsiScaleQuestion& PsiScale::GetQuestion(unsigned int index) const
{
	if (index >= _questions.size())
		throw (CString(_T("ָ������������δ�ҵ�������̫��")));
	
	return _questions[index];
}

PsiScaleQuestion& PsiScale::Question(unsigned index)
{
	return _questions[index];
}

PsiScale::PsiScale() : _id(0)
{

}

PsiScale::PsiScale(unsigned id, const TCHAR* name, const TCHAR* description, const TCHAR* prologue) :
	_id(id), _name(name), _description(description), _prologue(prologue)
{

}

void PsiScale::SetId(unsigned id)
{
	_id = id;
}

unsigned PsiScale::GetId() const
{
	return _id;
}

void PsiScale::SetName(const TCHAR* name)
{
	_name = name;
}

const CString& PsiScale::GetName() const
{
	return _name;
}

void PsiScale::SetDescription(const TCHAR* description)
{
	_description = description;
}

const CString& PsiScale::GetDescription() const
{
	return _description;
}

void PsiScale::SetPrologue(const TCHAR* prologue)
{
	_prologue = prologue;
}

const CString& PsiScale::GetPrologue() const
{
	return _prologue;
}

void PsiScale::AddGroup(const PsiScaleGroup& group)
{
	_groups.push_back(group);
}

const PsiScaleGroup& PsiScale::GetGroup(unsigned index) const
{
	ASSERT(index < _groups.size());
	return _groups[index];
}

PsiScaleGroup& PsiScale::Group(unsigned int index)
{
	return _groups[index];
}

unsigned int PsiScale::GetGroupCount() const
{
	return _groups.size();
}

void PsiScale::AddQuestion(const PsiScaleQuestion& question)
{
	_questions.push_back(question);
}

unsigned int PsiScale::GetQuestionCount() const
{
	return _questions.size();
}

bool PsiScale::Save(const CString& file_path)
{
	return false;
}

std::vector<QuestionChoice>& PsiScale::Choices()
{
	return _shared_choices;
}

PsiScaleQuestion::PsiScaleQuestion() :
	_reverse_score(false), _group_id(0)
{
}

PsiScaleQuestion::PsiScaleQuestion(unsigned id, 
	const CString& text,
	bool reverse_score, 
	unsigned short group_id) :
	_id(id), _text(text), _reverse_score(reverse_score), _group_id(group_id)
{
	_answer = _T(' ');
}

void PsiScaleQuestion::SetId(unsigned id)
{
	_id = id;
}

unsigned PsiScaleQuestion::GetId() const
{
	return _id;
}

void PsiScaleQuestion::SetText(const CString& text)
{
	_text = text;
}

const CString& PsiScaleQuestion::GetText() const
{
	return _text;
}

void PsiScaleQuestion::SetReverseScore(bool reverse_score)
{
	_reverse_score = reverse_score;
}

bool PsiScaleQuestion::GetReverseScore()
{
	return _reverse_score;
}

void PsiScaleQuestion::SetGroup(unsigned short group_id)
{
	_group_id = group_id;
}

unsigned short PsiScaleQuestion::GetGroupId()
{
	return _group_id;
}

std::vector<QuestionChoice>& PsiScaleQuestion::Choices()
{
	return _choices;
}
