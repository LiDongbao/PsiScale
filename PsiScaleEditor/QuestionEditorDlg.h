
// PsiScaleEditorDlg.h : ͷ�ļ�
//

#pragma once

#include <memory>
#include "afxwin.h"
#include "afxvslistbox.h"

class PsiScale;
class PsiScaleQuestion;

// CQuestionEditorDlg �Ի���
class CQuestionEditorDlg : public CDialogEx
{
// ����
public:
	CQuestionEditorDlg(std::shared_ptr<PsiScale> scale, CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUESTION_EDITOR };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	std::shared_ptr<PsiScale> _scale;

	int _current_question;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonNew();

	CString _question_text;
	CListBox _question_list;
	void UpdateUi();

	afx_msg void OnBnClickedButtonAddQuestion();
	afx_msg void OnEnChangeEditQuestion();
private:
	CComboBox _group_combo;
public:
	CVSListBox _choices_list;
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedDeleteQuestion();
};
