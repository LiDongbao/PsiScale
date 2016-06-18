
// PsycologyTestDlg.h : header file
//

#pragma once

#include "afxwin.h"
#include <memory>
#include <vector>
#include "..\PsiCommon\AnswerManager.h"

class CPsiScale;
class CAnswerManagerOld;
struct CQuestionChoice;

#define WM_SCALE_FINISHED WM_APP + 101
// CPsycologyTestDlg dialog
class CPsycologyTestDlg : public CDialogEx
{
	
// Construction
public:
	CPsycologyTestDlg(std::shared_ptr<CPsiScale> scale, CAnswerManager& answer_manager, 
		std::shared_ptr<CUser> user,
		HWND notify_wnd, CWnd* pParent = NULL);	// standard constructor
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PSYCOLOGYTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CAnswerManager& _answer_manager;
	std::shared_ptr<CPsiScale> _psi_scale;
	unsigned _current_question_index;
	CString _test_info_path;

	// Generated message map functions
	bool ShowQuestion(unsigned question_index);

	void UpdateSelectionButtons(std::vector<CQuestionChoice> &choices);

	bool ShowButtons(unsigned choice_count);
	void MoveButtonUp(CWnd& button, unsigned int y_pos);

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedPrev();
	afx_msg void OnBnClickedNext();
	CString _question;

	void ProcessAnswer(unsigned int answer);

	CString _timer_text;
	unsigned int _timer;

	clock_t _start;
	clock_t _end;
	bool _is_pause;
	
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	void Check(int button_to_check);
	void UncheckAll();

	void AdjustSize(int last_button);
	CString _question_number;
	HWND _notify_wnd;

	COleDateTime _start_time;
	std::shared_ptr<CUser> _user;
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedTimer();
	afx_msg void OnBnClickedButtonPrologue();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonStop();
};
