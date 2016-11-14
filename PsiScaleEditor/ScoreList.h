#pragma once
#include "afxvslistbox.h"


class CScoreList : public CVSListBox
{
	DECLARE_DYNAMIC(CScoreList)
public:
	CScoreList();
	~CScoreList();
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnBeforeRemoveItem(int) override;

	virtual void OnAfterAddItem(int) override;

	virtual void OnAfterRenameItem(int) override;

};

