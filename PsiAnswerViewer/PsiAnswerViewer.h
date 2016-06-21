
// PsiAnswerViewer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPsiAnswerViewerApp:
// See PsiAnswerViewer.cpp for the implementation of this class
//

class CPsiAnswerViewerApp : public CWinApp
{
public:
	CPsiAnswerViewerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedPersoninfoButton();
};

extern CPsiAnswerViewerApp theApp;