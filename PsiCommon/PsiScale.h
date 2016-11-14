#pragma once

#include <vector>
#include <map>
#include <TChar.h>
#include <afxstr.h>
#include <memory>
#include "Scorer.h"

struct CQuestionChoice
{
	unsigned int id;
	CString text;
	unsigned int score;
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

struct Score
{
	unsigned int id;
	unsigned int answer;
};

class CPsiScale
{
public:
	CPsiScale();
	CPsiScale(unsigned id, const TCHAR* name, const TCHAR* description, const TCHAR* prologue, bool samechoice);

	bool Load(const CString& file_path);
	bool Save(const CString& file_path);

	bool LoadScore(const CString& file_score_path);
	bool SaveScore(const CString& file_score_path);

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
	void AddScore(const Score& score);
	const Score& GetScore(unsigned int index) const;
	std::vector<Score>& Scores();

	void Reset();
private:
	unsigned _id;
	CString _name;
	CString _description;
	CString _prologue;

	std::vector<CString> _groups;
	std::vector<CPsiScaleQuestion> _questions;
	std::vector<CQuestionChoice> _choices;
	std::vector<Score> _score;
	CScorer _scorer;
	bool _same_choice;
};
