#include "stdafx.h"
#include "PsiScale.h"
#include <fstream>
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
const TCHAR * XML_SUB_SCALE = _T("SubScale");
const TCHAR * XML_CHOICES = _T("Choices");
const TCHAR * XML_SAME_CHOICES = _T("SameChoices");
const TCHAR * XML_PSYCOLOGYTEST = _T("PsycologyTest");

bool CPsiScale::Load(const CString& file_path)
{
	CXml xml;
	if (!xml.Load(file_path))
		return false;

	Reset();

	_id = xml.GetIntegerAttrib(XML_ID);
	_name = xml.GetAttrib(XML_NAME);
	_description = xml.GetAttrib(XML_DESCRIPTION);
	_prologue = xml.GetElementData(XML_PROLOGUE);

	auto groups_element = xml.GetElement(XML_GROUPS);
	if (groups_element == nullptr)
		return false;

	//Get Groups and push back to the _groups.
	auto& group_items = groups_element->GetChildElements();
	for (auto item : group_items)
	{
		_groups.push_back(item->GetData());
	}

	_same_choice = xml.GetBoolAttrib(XML_SAME_CHOICES);
	if (_same_choice)
	{
		auto choices_element = xml.GetElement(XML_CHOICES);
		if (choices_element == nullptr)
			return false;

		for (auto item : choices_element->GetChildElements())
		{
			CQuestionChoice choice;
			choice.id = item->GetIntegerAttrib(XML_ID);
			choice.text = item->GetData();
			_choices.push_back(choice);
		}
	}

	auto questions_element = xml.GetElement(XML_QUESTIONS);
	if (questions_element == nullptr)
		return false;

	for (auto question_item : questions_element->GetChildElements())
	{
		if (_same_choice)
		{
			CPsiScaleQuestion question(question_item->GetIntegerAttrib(XML_ID),
				question_item->GetData(),
				question_item->GetBoolAttrib(XML_REVERSE_SCORE),
				question_item->GetAttrib(XML_SUB_SCALE));
			_questions.push_back(question);
		}
		else
		{
			CPsiScaleQuestion question(question_item->GetIntegerAttrib(XML_ID),
				question_item->GetElementData(XML_TEXT),
				question_item->GetBoolAttrib(XML_REVERSE_SCORE),
				question_item->GetAttrib(XML_SUB_SCALE));

			auto choices_element = question_item->GetElement(XML_CHOICES);
			if (choices_element == nullptr)
				return false;

			for (auto choice_item : choices_element->GetChildElements())
			{
				CQuestionChoice choice;
				choice.id = choice_item->GetIntegerAttrib(XML_ID);
				choice.text = choice_item->GetData();
				question.Choices().push_back(choice);
			}

			_questions.push_back(question);
		}

	}

	return true;
}

const CPsiScaleQuestion& CPsiScale::GetQuestion(unsigned int index) const
{
	if (index >= _questions.size())
		throw (CString(_T("指定索引的问题未找到，索引太大。")));
	
	return _questions[index];
}

CPsiScaleQuestion& CPsiScale::Question(unsigned index)
{
	return _questions[index];
}

CPsiScale::CPsiScale() : _id(0)
{

}

CPsiScale::CPsiScale(unsigned id, const TCHAR* name, const TCHAR* description, const TCHAR* prologue,bool samechoice) :
	_id(id), _name(name), _description(description), _prologue(prologue), _same_choice(samechoice)
{

}

void CPsiScale::SetId(unsigned id)
{
	_id = id;
}

unsigned CPsiScale::GetId() const
{
	return _id;
}

void CPsiScale::SetName(const TCHAR* name)
{
	_name = name;
}

const CString& CPsiScale::GetName() const
{
	return _name;
}

void CPsiScale::SetDescription(const TCHAR* description)
{
	_description = description;
}

const CString& CPsiScale::GetDescription() const
{
	return _description;
}

void CPsiScale::SetPrologue(const TCHAR* prologue)
{
	_prologue = prologue;
}

const CString& CPsiScale::GetPrologue() const
{
	return _prologue;
}

void CPsiScale::AddGroup(const CString& group)
{
	_groups.push_back(group);
}

const CString& CPsiScale::GetGroup(unsigned index) const
{
	ASSERT(index < _groups.size());
	return _groups[index];
}

unsigned int CPsiScale::GetGroupCount() const
{
	return _groups.size();
}

std::vector<CString>& CPsiScale::Groups()
{
	return _groups;
}

void CPsiScale::AddQuestion(const CPsiScaleQuestion& question)
{
	_questions.push_back(question);
	_questions[_questions.size() - 1].SetId(_questions.size());
}

void CPsiScale::DeleteQuestion(unsigned int index)
{
	if (index < _questions.size())
	{
		_questions.erase(_questions.begin() + index);
	}
}

unsigned int CPsiScale::GetQuestionCount() const
{
	return _questions.size();
}

bool CPsiScale::Save(const CString& file_path)
{
	CXml xml(XML_PSYCOLOGYTEST);
	xml.SetIntegerAttrib(XML_ID, _id);
	xml.SetAttrib(XML_NAME, _name);
	xml.SetAttrib(XML_DESCRIPTION, _description);
	xml.SetBoolAttrib(XML_SAME_CHOICES, _same_choice);
	if (_same_choice)
	{
		auto choices = xml.AddElement(XML_CHOICES);
		for (auto choice : _choices)
		{
			auto choice_element = choices->AddElement(_T("Item"), choice.text);
			choice_element->SetIntegerAttrib(XML_ID, choice.id);
		}
	}

	auto prologue = xml.AddElement(XML_PROLOGUE, _prologue);

	auto groups = xml.AddElement(XML_GROUPS);
	for (unsigned int i = 0; i < _groups.size(); ++i)
	{
		auto group_element = groups->AddElement(_T("Item"));
		group_element->SetData(_groups[i]);
	}
	auto questions_element = xml.AddElement(XML_QUESTIONS);
	for (auto& question : _questions)
	{
		auto item_element = questions_element->AddElement(_T("Item"));
		item_element->SetIntegerAttrib(XML_ID, question.GetId());
		item_element->SetBoolAttrib(XML_REVERSE_SCORE, question.GetReverseScore());
		item_element->SetAttrib(XML_SUB_SCALE, question.GetGroup());
		if (_same_choice)
		{
			item_element->SetData(question.GetText());
		}
		else
		{
			item_element->AddElement(XML_TEXT, question.GetText());
			auto choices = item_element->AddElement(XML_CHOICES);
			for (auto choice : question.Choices())
			{
				auto choice_element = choices->AddElement(_T("Item"), choice.text);
				choice_element->SetIntegerAttrib(XML_ID, choice.id);
			}
		}
	}
	xml.Save(file_path);
	return true;
}

bool CPsiScale::LoadScore(const CString& file_score_path)
{
	CScoreMatrix score_matrix;
	score_matrix.Load(file_score_path);
	auto matrix_group = score_matrix.GetGroups();
	auto matrix = score_matrix.ScoreMatrix();
	if (matrix_group.size() != _groups.size())
		return false;
	if (matrix.size() != _questions.size())
		return false;
	if (_same_choice && matrix[0].size() != _choices.size())
		return false;

	for (size_t i = 0; i < matrix_group.size(); ++i)
	{
		if (matrix_group[i] != _groups[i])
			return false;
	}

	if (_same_choice)
	{
		
		for (size_t i = 0; i < _questions.size(); ++i)
			for (size_t j = 0; j < _choices.size(); ++j)
				for (size_t k = 0; k < GetGroupCount(); ++k)
					if (_questions[i].GetGroup() == _groups[k])
					{	
						if (_questions[i].GetReverseScore())
							_choices[_choices.size() - j - (size_t)1].score = (size_t)matrix[i][j][k];
						else
							_choices[j].score = (size_t)matrix[i][j][k];
					}

		_score.resize(matrix[0].size());
		for (size_t i = 0; i < _choices.size(); ++i)
		{
			_score[i].id = _choices[i].id;
			_score[i].answer = _choices[i].score;
		}
	}
	else
	{
		//non-same choice
		for (size_t i = 0; i < _questions.size(); ++i)
			for (size_t j = 0; j < _questions[i].Choices().size(); ++j)
				for (size_t k = 0; k < GetGroupCount(); ++k)
					if (_questions[i].GetGroup() == _groups[k])
					{
						if (_questions[i].GetReverseScore())
							_questions[i].Choices()[_questions[i].Choices().size() - j - (size_t)1].score= (size_t)matrix[i][j][k];
						else
							_questions[i].Choices()[j].score = (size_t)matrix[i][j][k];
					}
	}

	return true;
}

bool CPsiScale::SaveScore(const CString & file_path)
{
	ofstream fout(file_path);
	if (!fout)
		return false;
	if (_same_choice && !_choices.empty())
		fout << GetQuestionCount() << "\t" << _choices.size() << "\t" << GetGroupCount() <<endl;
	else if (!_same_choice && !_questions.empty() && !_questions[0].Choices().empty())
		fout << GetQuestionCount() << "\t" << _questions[0].Choices().size() << "\t" << GetGroupCount() << endl;
	else
		return false;

	fout << "\t";
	for (unsigned int i = 0; i < GetGroupCount(); ++i)
	{
		//Change CODE: From Unicode to ANSI
		int size = WideCharToMultiByte(CP_ACP, 0, _groups[i], -1, NULL, 0, NULL, NULL);
		char * p =new char[size + 1];
		WideCharToMultiByte(CP_ACP, 0, _groups[i], -1, p, size, NULL, NULL);
		fout << "\t" <<p;
	}
	fout << endl;
	if (_same_choice)
	{
		for (size_t i = 0; i < _questions.size(); ++i)
			for (size_t j = 0; j < _choices.size(); ++j)
				{
					fout << _questions[i].GetId() << "\t" << _choices[j].id;
					for (size_t k = 0; k < GetGroupCount(); ++k)
					{
						int text;
						if ((_questions[i].GetGroup() == _groups[k]))
							if (_questions[i].GetReverseScore())
								text = _choices[_choices.size() - j - (size_t)1].score;
							else
								text = _choices[j].score;
						else
							text = 0;
						fout << "\t" << text;
					}
					fout << endl;
				}
	}
	else
	{
		//non-same choice
		for (size_t i = 0; i < _questions.size(); ++i)
			for (size_t j = 0; j < _questions[i].Choices().size(); ++j)
			{
				fout << _questions[i].GetId() << "\t" << _questions[i].Choices()[j].id;
				for (size_t k = 0; k < GetGroupCount(); ++k)
				{
					int text;
					if (_questions[i].GetGroup() == _groups[k])
						if (_questions[i].GetReverseScore())
							text = _questions[i].Choices()[_questions[i].Choices().size() - j - 1].score;
						else
							text = _questions[i].Choices()[j].score;
					else
							text = 0;
					fout << "\t" << text;
				}
				fout << endl;
			}
	}
	fout.close();
	return true;
}

std::vector<CQuestionChoice>& CPsiScale::Choices()
{
	return _choices;
}

void CPsiScale::Reset()
{
	_id = 0;
	_name.Empty();
	_description.Empty();
	_prologue.Empty();
	_groups.clear();
	_questions.clear();
	_choices.clear();
	_score.clear();
	_same_choice = true;
}

const bool CPsiScale::IsSameChoice() const
{
	return _same_choice;
}

void CPsiScale::SetSameChoice(bool samechoice)
{
	_same_choice = samechoice;
}

CPsiScaleQuestion::CPsiScaleQuestion() :
	_reverse_score(false)
{
}

CPsiScaleQuestion::CPsiScaleQuestion(unsigned id, 
	const CString& text,
	bool reverse_score, 
	const CString& group) :
	_id(id), _text(text), _reverse_score(reverse_score), _group(group)
{
}

void CPsiScaleQuestion::SetId(unsigned id)
{
	_id = id;
}

unsigned CPsiScaleQuestion::GetId() const
{
	return _id;
}

void CPsiScaleQuestion::SetText(const CString& text)
{
	_text = text;
}

const CString& CPsiScaleQuestion::GetText() const
{
	return _text;
}

void CPsiScaleQuestion::SetReverseScore(bool reverse_score)
{
	_reverse_score = reverse_score;
}

const bool CPsiScaleQuestion::GetReverseScore() const
{
	return _reverse_score;
}

void CPsiScaleQuestion::SetGroup(const CString& group)
{
	_group = group;
}

const CString& CPsiScaleQuestion::GetGroup() const
{
	return _group;
}

std::vector<CQuestionChoice>& CPsiScaleQuestion::Choices()
{
	return _choices;
}

void CPsiScale::AddScore(const Score& score)
{
	_score.push_back(score);
}
const Score& CPsiScale::GetScore(unsigned int index) const 
{
	ASSERT(index < _score.size());
	return _score[index];
}

std::vector<Score>& CPsiScale::Scores()
{
	return _score;
}