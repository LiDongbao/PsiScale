// ScaleList.cpp : implementation file
//

#include "stdafx.h"
#include "PsycologyTest.h"
#include "ScaleList.h"
#include <utility>

using namespace std;

// CScaleList

IMPLEMENT_DYNAMIC(CScaleList, CListCtrl)

CScaleList::CScaleList()
{

}

CScaleList::~CScaleList()
{
}


void CScaleList::InsertScale(const CString& scale_name, bool finished)
{
	_scale_states.push_back(make_pair(scale_name, finished));
	int count = GetItemCount();
	LVITEM item;
	item.mask = LVIF_TEXT;
	item.cchTextMax = 260;
	item.pszText = new TCHAR[item.cchTextMax];
	_tcscpy_s(item.pszText, item.cchTextMax, scale_name.GetString());
	item.iItem = count;
	item.iSubItem = 0;
	InsertItem(&item);

	item.iSubItem = 1;
	_tcscpy_s(item.pszText, 260, finished ? _T("完成") : _T("未完成"));
	InsertItem(&item);
}

void CScaleList::ChangeScale(const CString& scale_name, bool finished)
{
	std::vector<std::pair<CString, bool>> temp;
	std::swap(temp, _scale_states);
	Clear();
	for (unsigned int i = 0; i < temp.size(); ++i)
	{
		auto name = temp[i].first;
		name = name.Right(name.GetLength() - name.ReverseFind(_T('.')) - 1);
		if (name == scale_name)
		{
			temp[i].second = finished;	
		}
		InsertScale(temp[i].first, temp[i].second);
	}
	//LVFINDINFO info;
	//int nIndex;

	//info.flags = LVFI_PARTIAL | LVFI_STRING;
	//info.psz = _T("scale_name");

	//// Delete all of the items that begin with the string.
	//if ((nIndex = FindItem(&info)) != -1)
	//{
	//	SetItem(nIndex, 1, LVIF_STATE, finished ? _T("完成") : _T("未完成"), 0, LVIS_SELECTED,
	//		LVIS_SELECTED, 0);
	//}
	//
	//Invalidate(FALSE);
	//UpdateWindow();
}

void CScaleList::Clear()
{
	_scale_states.clear();
	DeleteAllItems();
}
BEGIN_MESSAGE_MAP(CScaleList, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CScaleList::OnLvnGetdispinfo)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CScaleList message handlers




void CScaleList::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *display_info = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	static TCHAR buffer[260];

	if (display_info->item.iSubItem == 0)
	{
		_tcscpy_s(buffer, 260, _scale_states[display_info->item.iItem].first);
	}
	else if (display_info->item.iSubItem == 1)
	{
		_tcscpy_s(buffer, 260, _scale_states[display_info->item.iItem].second ?
			_T("完成") : _T("未完成"));
	}
	display_info->item.pszText = buffer;
}

void CScaleList::Init()
{
	LVCOLUMNW column;
	column.mask = LVCF_WIDTH | LVCF_TEXT;
	column.cxMin = 100;
	column.cxDefault = 200;
	column.pszText = _T("量表");
	column.cx = 300;
	column.fmt = LVCFMT_LEFT;

	InsertColumn(0, &column);

	column.cxMin = 20;
	column.cxDefault = 100;
	column.cx = 100;
	column.pszText = _T("完成情况");
	column.fmt = LVCFMT_CENTER;
	InsertColumn(1, &column);
}

int CScaleList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}
