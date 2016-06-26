
// PsycologyTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PsycologyTest.h"
#include "TestOverviewDialog.h"
#include "LogonDialog.h"
#include "PersonalInfoDialog.h"
#include "..\PsiCommon\User.h"
#include "..\PsiCommon\Scorer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPsycologyTestApp

BEGIN_MESSAGE_MAP(CPsycologyTestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPsycologyTestApp construction

CPsycologyTestApp::CPsycologyTestApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPsycologyTestApp object

CPsycologyTestApp theApp;


// CPsycologyTestApp initialization

BOOL CPsycologyTestApp::InitInstance()
{
	InitCommonControls();

	CWinApp::InitInstance();


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("SKMR"));

	CLogonDialog logon_dlg;

	for (;;)
	{

		INT_PTR nResponse = logon_dlg.DoModal();
		if (nResponse == IDOK)
		{
			if (logon_dlg.IsFirstTime())
			{
				CPersonalInfoDialog info_dlg;
				if (info_dlg.DoModal() == IDOK)
				{
					logon_dlg.GetUser()->SetInfo(info_dlg.GetInfo());
				}
				else
				{
					continue;
				}
			}
			
			auto folder = logon_dlg.GetUser()->GetWorkingFolder();
			CScorer::GetInstance().Init(folder + _T("\\..\\.."));
			CScaleOverviewDialog overview_dialog(logon_dlg.GetUser(), logon_dlg.IsFirstTime());
			overview_dialog.DoModal();
		}
		else if (nResponse == IDCANCEL)
		{
			break;
		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
			TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
			break;
		}
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

