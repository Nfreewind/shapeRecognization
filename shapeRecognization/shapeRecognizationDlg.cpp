
// shapeRecognizationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "shapeRecognization.h"
#include "shapeRecognizationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CshapeRecognizationDlg �Ի���



CshapeRecognizationDlg::CshapeRecognizationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SHAPERECOGNIZATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CshapeRecognizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CshapeRecognizationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CshapeRecognizationDlg::OnBnClickedButton1_shapeReco)
	ON_BN_CLICKED(IDC_BUTTON2, &CshapeRecognizationDlg::OnBnClickedButton2_setIdFunction)
	ON_BN_CLICKED(IDC_BUTTON3, &CshapeRecognizationDlg::OnBnClickedButton3s_setRateErrorSup)
	ON_BN_CLICKED(IDC_BUTTON4, &CshapeRecognizationDlg::OnBnClickedButton4_setNumsSteps)
	ON_BN_CLICKED(IDC_BUTTON5, &CshapeRecognizationDlg::OnBnClickedButton5_setK)
END_MESSAGE_MAP()


// CshapeRecognizationDlg ��Ϣ�������

BOOL CshapeRecognizationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������


	opt_function = 2;// 1;
	rateErrorSup = 0.88;// 0.3;// 0.15;
	//rateErrorSup = 

	numsSteps = 20;

	this->k_solid = 4;

	CString  cs = CString("opt_function =  ")
		+ CString(to_string(opt_function).c_str()) + CString("\r\n");
	CString str21;
	GetDlgItemText(IDC_EDIT2, str21);
	//str21 = str21;
	cs = str21 + cs;// +CString("\r\n");
	SetDlgItemText(IDC_EDIT2, cs);


	cs = CString("rateErrorSup =  ")
		+ CString(to_string(rateErrorSup).c_str()) + CString("\r\n");
	GetDlgItemText(IDC_EDIT2, str21);
	//str21 = str21 + CString("\r\n");
	cs = str21 + cs;// +CString("\r\n");
	SetDlgItemText(IDC_EDIT2, cs);


	cs = CString("numsSteps =  ")
		+ CString(to_string(numsSteps).c_str()) + CString("\r\n");
	GetDlgItemText(IDC_EDIT2, str21);
	//str21 = str21 + CString("\r\n");
	cs = str21 + cs;// +CString("\r\n");
	SetDlgItemText(IDC_EDIT2, cs);

	cs = CString("k_solid =  ")
		+ CString(to_string(k_solid).c_str()) + CString("\r\n");
	GetDlgItemText(IDC_EDIT2, str21);
	//str21 = str21 + CString("\r\n");
	cs = str21 + cs;// +CString("\r\n");
	SetDlgItemText(IDC_EDIT2, cs);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CshapeRecognizationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CshapeRecognizationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CshapeRecognizationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CshapeRecognizationDlg::fetchFileName(string & nameOut)
{
	CString picPath;   //����ͼƬ·������  
	CFileDialog dlg(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, NULL, this);   //ѡ���ļ��Ի���  

	if (dlg.DoModal() == IDOK)
	{
		picPath = dlg.GetPathName();  //��ȡͼƬ·��  
	}
	else
	{
		return;
	}
	if (picPath.IsEmpty())
		return;
	////CString to string  ʹ����������ǵ��ַ���ѡ�á�ʹ�ö��ֽ��ַ�������Ȼ�ᱨ��  
	//string picpath = picPath.GetBuffer(0);

	//�����￪ʼ����ת��������һ���궨��
	USES_CONVERSION;
	//����ת��
	char* keyChar = T2A(picPath.GetBuffer(0));
	picPath.ReleaseBuffer();
	string picpath(keyChar);
	//string   picpath = string(keyChar);
	nameOut = picpath;
}




void CshapeRecognizationDlg::OnBnClickedButton2_setIdFunction()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str2("0");
	GetDlgItemText(IDC_EDIT1, str2);
	//str2.
	int i = 0;
	LPTSTR lpsz;// = 20;
	lpsz = (LPTSTR)(LPCTSTR)str2;
	i = _ttoi(str2);
	//str2.Format(_T("%d"), i)   ;
	opt_function = i;

	//AfxMessageBox(_T("�����Ƶ֡��������ɣ�лл�� "));
	CString cstr;
	cstr.Format(_T("opt_function������ɣ�лл��id_cameraΪ�� %d"), opt_function);
	AfxMessageBox(cstr);

	//append_string_on_edit_controler(IDC_EDIT2, CString("id_camera������ɣ�лл��id_cameraΪ�� ")
	//	+ CString(to_string(id_camera).c_str()));

	CString  cs = CString("opt_function������ɣ�лл��opt_functionΪ�� ")
		+ CString(to_string(opt_function).c_str());// ("");
	CString str21;
	GetDlgItemText(IDC_EDIT2, str21);
	str21 = str21 + CString("\r\n");
	cs = str21 + cs;
	SetDlgItemText(IDC_EDIT2, cs);

}


void CshapeRecognizationDlg::OnBnClickedButton3s_setRateErrorSup()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str2("0");
	GetDlgItemText(IDC_EDIT3, str2);
	//str2.
	double  i = 0;
	LPTSTR lpsz;// = 20;
	lpsz = (LPTSTR)(LPCTSTR)str2;
	i = _ttof(str2);
	//str2.Format(_T("%d"), i)   ;
	rateErrorSup = i;

	//AfxMessageBox(_T("�����Ƶ֡��������ɣ�лл�� "));
	CString cstr;
	cstr.Format(_T("rateErrorSup������ɣ�лл��rateErrorSupΪ�� %f"), rateErrorSup);
	AfxMessageBox(cstr);

	//append_string_on_edit_controler(IDC_EDIT2, CString("id_camera������ɣ�лл��id_cameraΪ�� ")
	//	+ CString(to_string(id_camera).c_str()));

	CString  cs = CString("rateErrorSup������ɣ�лл��rateErrorSupΪ�� ")
		+ CString(to_string(rateErrorSup).c_str());// ("");
	CString str21;
	GetDlgItemText(IDC_EDIT2, str21);
	str21 = str21 + CString("\r\n");
	cs = str21 + cs;
	SetDlgItemText(IDC_EDIT2, cs);
}


void CshapeRecognizationDlg::OnBnClickedButton4_setNumsSteps()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str2("0");
	GetDlgItemText(IDC_EDIT4, str2);
	//str2.
	int  i = 0;
	LPTSTR lpsz;// = 20;
	lpsz = (LPTSTR)(LPCTSTR)str2;
	i = _ttoi(str2);
	//str2.Format(_T("%d"), i)   ;
	numsSteps = i;

	//AfxMessageBox(_T("�����Ƶ֡��������ɣ�лл�� "));
	CString cstr;
	cstr.Format(_T("numsSteps������ɣ�лл��numsStepsΪ�� %d"), numsSteps);
	AfxMessageBox(cstr);

	//append_string_on_edit_controler(IDC_EDIT2, CString("id_camera������ɣ�лл��id_cameraΪ�� ")
	//	+ CString(to_string(id_camera).c_str()));

	CString  cs = CString("numsSteps������ɣ�лл��numsStepsΪ�� ")
		+ CString(to_string(numsSteps).c_str());// ("");
	CString str21;
	GetDlgItemText(IDC_EDIT2, str21);
	str21 = str21 + CString("\r\n");
	cs = str21 + cs;
	SetDlgItemText(IDC_EDIT2, cs);
}


void CshapeRecognizationDlg::OnBnClickedButton5_setK()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str2("0");
	GetDlgItemText(IDC_EDIT5, str2);
	//str2.
	int  i = 0;
	LPTSTR lpsz;// = 20;
	lpsz = (LPTSTR)(LPCTSTR)str2;
	i = _ttoi(str2);
	//str2.Format(_T("%d"), i)   ;
	k_solid = i;

	//AfxMessageBox(_T("�����Ƶ֡��������ɣ�лл�� "));
	CString cstr;
	cstr.Format(_T("k_solid������ɣ�лл��k_solidΪ�� %d"), k_solid);
	AfxMessageBox(cstr);

	//append_string_on_edit_controler(IDC_EDIT2, CString("id_camera������ɣ�лл��id_cameraΪ�� ")
	//	+ CString(to_string(id_camera).c_str()));

	CString  cs = CString("k_solid������ɣ�лл��k_solidΪ�� ")
		+ CString(to_string(k_solid).c_str());// ("");
	CString str21;
	GetDlgItemText(IDC_EDIT2, str21);
	str21 = str21 + CString("\r\n");
	cs = str21 + cs;
	SetDlgItemText(IDC_EDIT2, cs);
}


void CshapeRecognizationDlg::OnBnClickedButton1_shapeReco()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AllocConsole();
	freopen("CONOUT$", "w", stdout);


	string   nameOut;
	fetchFileName(nameOut);

	cv::Mat  src = imread(nameOut);
	if (!src.data)
	{
		FreeConsole();
		return;
	}



	cv::imshow("src", src);
	cv::waitKey(10);

	shapeRecognizer   sr;
	vector<vector<Point> >     contours_poly;
	//int   opt_function_t = this->opt_function;
	//int  numsSteps;
	sr.getContours(src, contours_poly,
		this->opt_function, this->rateErrorSup,
		this->numsSteps, this->k_solid,
		nameOut
	);

	cv::waitKey();

	FreeConsole();
}


