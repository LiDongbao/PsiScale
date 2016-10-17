#pragma once

#include <map>
#include <memory>
#include <vector>

class CScoreMatrix
{
public:
	bool Load(const wchar_t * path);

	double GetWeight(unsigned int question, unsigned int choice, unsigned int group);
	const std::vector<CString>& GetGroups() const;
protected:
	std::vector<std::vector<std::vector<double>>> _matrix;
	std::vector<CString> _groups;
};

class CScorer
{
public:
	CScorer();
	~CScorer();
	bool Init(const wchar_t * score_folder);
	CScoreMatrix * GetScoreMatrix(const wchar_t * scale_name);
private:
	bool _initialized;
	
	std::map<CString, std::shared_ptr<CScoreMatrix>> _all_score_matrix;
};

