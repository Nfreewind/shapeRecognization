
// shapeRecognization.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CshapeRecognizationApp: 
// �йش����ʵ�֣������ shapeRecognization.cpp
//

class CshapeRecognizationApp : public CWinApp
{
public:
	CshapeRecognizationApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CshapeRecognizationApp theApp;