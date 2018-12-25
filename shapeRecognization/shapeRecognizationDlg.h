
// shapeRecognizationDlg.h : ͷ�ļ�
//

#pragma once

#include "shapeRecognizer.h"

// CshapeRecognizationDlg �Ի���
class CshapeRecognizationDlg : public CDialogEx
{
// ����
public:
	CshapeRecognizationDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHAPERECOGNIZATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1_shapeReco();

	void CshapeRecognizationDlg::fetchFileName(string & nameOut);

	int  opt_function;
	afx_msg void OnBnClickedButton2_setIdFunction();
	afx_msg void OnBnClickedButton3s_setRateErrorSup();

	double  rateErrorSup;
	int  numsSteps;
	afx_msg void OnBnClickedButton4_setNumsSteps();
	afx_msg void OnBnClickedButton5_setK();

	int  k_solid ;
};
