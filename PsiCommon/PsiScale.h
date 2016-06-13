#pragma once

#include <vector>
#include <map>
#include <TChar.h>
#include <afxstr.h>
#include <memory>

struct CQuestionChoice
{
	unsigned int id;
	CString text;
};

class CPsiScaleQuestion
{
public:
	CPsiScaleQuestion();
	CPsiScaleQuestion(unsigned id, const CString& text, bool reverse_score, const CString& group);

	void SetId(unsigned id);
	unsigned GetId() const;
	void SetText(const CString& text);
	const CString& GetText() const;
	void SetReverseScore(bool reverse_score);
	const bool GetReverseScore() const;
	void SetGroup(const CString& group);
	const CString& GetGroup() const;

	std::vector<CQuestionChoice>& Choices();
private:
	unsigned _id;
	CString _text;
	bool _reverse_score;
	CString _group;
	std::vector<CQuestionChoice> _choices;
};

class CPsiScale
{
public:
	CPsiScale();
	CPsiScale(unsigned id, const TCHAR* name, const TCHAR* description, const TCHAR* prologue, bool samechoice);

	bool Load(const CString& file_path);
	bool Save(const CString& file_path);

	void SetId(unsigned id);
	unsigned GetId() const;
	void SetName(const TCHAR* name);
	const CString& GetName() const;
	void SetDescription(const TCHAR* description);
	const CString& GetDescription() const;
	void SetPrologue(const TCHAR* prologue);
	const CString& GetPrologue() const;

	void AddGroup(const CString& group);
	const CString& GetGroup(unsigned index) const;
	unsigned int GetGroupCount() const;
	std::vector<CString>& Groups();

	void AddQuestion(const CPsiScaleQuestion& question);
	void DeleteQuestion(unsigned int index);
	const CPsiScaleQuestion& GetQuestion(unsigned int index) const;
	CPsiScaleQuestion& Question(unsigned index);
	unsigned int GetQuestionCount() const;

	const bool IsSameChoice() const;
	void SetSameChoice(bool samechoice);;
	std::vector<CQuestionChoice>& Choices();


	void Reset();
private:
	unsigned _id;
	CString _name;
	CString _description;
	CString _prologue;

	std::vector<CString> _groups;
	std::vector<CPsiScaleQuestion> _questions;
	std::vector<CQuestionChoice> _choices;

	bool _same_choice;
};

struct Score
{
	CString id;
	CString answer;
};
