#include "stdafx.h"
#include "ScoreList.h"

IMPLEMENT_DYNAMIC(CScoreList,CVSListBox)

CScoreList::CScoreList()
{
}


CScoreList::~CScoreList()
{
}


BOOL CScoreList::OnBeforeRemoveItem(int)
{
	return true;
}

void CScoreList::OnAfterAddItem(int)
{}

void CScoreList::OnAfterRenameItem(int) 
{}

BEGIN_MESSAGE_MAP(CScoreList, CVSListBox)
END_MESSAGE_MAP()

