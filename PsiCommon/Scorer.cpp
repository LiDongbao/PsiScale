#pragma once
#include "stdafx.h"
#include "Scorer.h"
#include <fstream>
#include "../Utilities/FileSystem.h"
#include <utility>
#include <algorithm>

using namespace std;

CScorer::CScorer() : 
	_initialized(false)
{
}


CScorer::~CScorer()
{
}

bool CScorer::Init(const wchar_t * score_folder)
{
	if (_initialized)
		return true;

	FileSystem::ForEachFile(score_folder, L"*.score", false, [&](CString file_path) {
		wstring scale_name = FileSystem::GetFileNameFromPath(file_path);
		auto pt = scale_name.find_first_of(_T("."));
		shared_ptr<CScoreMatrix> score_matrix(new CScoreMatrix);
		score_matrix->Load(file_path);
		_all_score_matrix.insert(make_pair(scale_name.substr(pt + 1), score_matrix));
	});

	_initialized = true;
	return true;
}

CScoreMatrix * CScorer::GetScoreMatrix(const wchar_t * scale_name)
{
	assert(_initialized);

	auto iter = _all_score_matrix.find(scale_name);
	return (iter == _all_score_matrix.end()) ? nullptr : iter->second.get();
}

bool CScoreMatrix::Load(const wchar_t * path)
{
	ifstream file(path);
	if (!file)
		return false;

	unsigned int question_count = 0;
	unsigned int choice_count = 0;
	unsigned int group_count = 0;

	file >> question_count >> choice_count >> group_count;
	_groups.resize(group_count);

	char line_buffer[256];
	file.getline(line_buffer, 256);	//ÍÌµô×îºóµÄ\n
	file.getline(line_buffer, 256);

	USES_CONVERSION;
	wstring line(A2CW(line_buffer));
	for (unsigned int i = 0; i < group_count; ++i)
	{
		auto start = line.find_first_not_of(L'\t');
		auto stop = line.find(L'\t', start);
		if (stop == wstring::npos) 
		{
			_groups[i] = line.substr(start);
		}
		else
		{
			_groups[i] = line.substr(start, stop - start);
			line = line.substr(stop + 1);
		}
	}

	_matrix.resize(question_count);
	for (unsigned int question = 0; question < question_count; ++question)
	{
		_matrix[question].resize(choice_count);
		for (unsigned int choice = 0; choice < choice_count; ++choice)
		{
			unsigned int question_number, choice_number;
			file >> question_number >> choice_number;

			_matrix[question][choice].resize(group_count);
			for (unsigned int group = 0; group < group_count; ++group)
			{
				file >> _matrix[question][choice][group];
			}
		}
	}

	if (!file)
		return false;

	return true;
}

double CScoreMatrix::GetWeight(unsigned int question, unsigned int choice, unsigned int group)
{
	return _matrix[question][choice - 1][group];
}

const std::vector<std::wstring>& CScoreMatrix::GetGroups() const
{
	return _groups;
}
