
// shapeRecognizationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "shapeRecognization.h"
#include "shapeRecognizationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CshapeRecognizationDlg 对话框



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


// CshapeRecognizationDlg 消息处理程序

BOOL CshapeRecognizationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


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


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CshapeRecognizationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CshapeRecognizationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CshapeRecognizationDlg::fetchFileName(string & nameOut)
{
	CString picPath;   //定义图片路径变量  
	CFileDialog dlg(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, NULL, this);   //选择文件对话框  

	if (dlg.DoModal() == IDOK)
	{
		picPath = dlg.GetPathName();  //获取图片路径  
	}
	else
	{
		return;
	}
	if (picPath.IsEmpty())
		return;
	////CString to string  使用这个方法记得字符集选用“使用多字节字符”，不然会报错  
	//string picpath = picPath.GetBuffer(0);

	//从这里开始进行转化，这是一个宏定义
	USES_CONVERSION;
	//进行转换
	char* keyChar = T2A(picPath.GetBuffer(0));
	picPath.ReleaseBuffer();
	string picpath(keyChar);
	//string   picpath = string(keyChar);
	nameOut = picpath;
}




void CshapeRecognizationDlg::OnBnClickedButton2_setIdFunction()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CString str2("0");
	GetDlgItemText(IDC_EDIT1, str2);
	//str2.
	int i = 0;
	LPTSTR lpsz;// = 20;
	lpsz = (LPTSTR)(LPCTSTR)str2;
	i = _ttoi(str2);
	//str2.Format(_T("%d"), i)   ;
	opt_function = i;

	//AfxMessageBox(_T("最大视频帧数设置完成，谢谢！ "));
	CString cstr;
	cstr.Format(_T("opt_function设置完成，谢谢！id_camera为： %d"), opt_function);
	AfxMessageBox(cstr);

	//append_string_on_edit_controler(IDC_EDIT2, CString("id_camera设置完成，谢谢！id_camera为： ")
	//	+ CString(to_string(id_camera).c_str()));

	CString  cs = CString("opt_function设置完成，谢谢！opt_function为： ")
		+ CString(to_string(opt_function).c_str());// ("");
	CString str21;
	GetDlgItemText(IDC_EDIT2, str21);
	str21 = str21 + CString("\r\n");
	cs = str21 + cs;
	SetDlgItemText(IDC_EDIT2, cs);

}


void CshapeRecognizationDlg::OnBnClickedButton3s_setRateErrorSup()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CString str2("0");
	GetDlgItemText(IDC_EDIT3, str2);
	//str2.
	double  i = 0;
	LPTSTR lpsz;// = 20;
	lpsz = (LPTSTR)(LPCTSTR)str2;
	i = _ttof(str2);
	//str2.Format(_T("%d"), i)   ;
	rateErrorSup = i;

	//AfxMessageBox(_T("最大视频帧数设置完成，谢谢！ "));
	CString cstr;
	cstr.Format(_T("rateErrorSup设置完成，谢谢！rateErrorSup为： %f"), rateErrorSup);
	AfxMessageBox(cstr);

	//append_string_on_edit_controler(IDC_EDIT2, CString("id_camera设置完成，谢谢！id_camera为： ")
	//	+ CString(to_string(id_camera).c_str()));

	CString  cs = CString("rateErrorSup设置完成，谢谢！rateErrorSup为： ")
		+ CString(to_string(rateErrorSup).c_str());// ("");
	CString str21;
	GetDlgItemText(IDC_EDIT2, str21);
	str21 = str21 + CString("\r\n");
	cs = str21 + cs;
	SetDlgItemText(IDC_EDIT2, cs);
}


void CshapeRecognizationDlg::OnBnClickedButton4_setNumsSteps()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CString str2("0");
	GetDlgItemText(IDC_EDIT4, str2);
	//str2.
	int  i = 0;
	LPTSTR lpsz;// = 20;
	lpsz = (LPTSTR)(LPCTSTR)str2;
	i = _ttoi(str2);
	//str2.Format(_T("%d"), i)   ;
	numsSteps = i;

	//AfxMessageBox(_T("最大视频帧数设置完成，谢谢！ "));
	CString cstr;
	cstr.Format(_T("numsSteps设置完成，谢谢！numsSteps为： %d"), numsSteps);
	AfxMessageBox(cstr);

	//append_string_on_edit_controler(IDC_EDIT2, CString("id_camera设置完成，谢谢！id_camera为： ")
	//	+ CString(to_string(id_camera).c_str()));

	CString  cs = CString("numsSteps设置完成，谢谢！numsSteps为： ")
		+ CString(to_string(numsSteps).c_str());// ("");
	CString str21;
	GetDlgItemText(IDC_EDIT2, str21);
	str21 = str21 + CString("\r\n");
	cs = str21 + cs;
	SetDlgItemText(IDC_EDIT2, cs);
}


void CshapeRecognizationDlg::OnBnClickedButton5_setK()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str2("0");
	GetDlgItemText(IDC_EDIT5, str2);
	//str2.
	int  i = 0;
	LPTSTR lpsz;// = 20;
	lpsz = (LPTSTR)(LPCTSTR)str2;
	i = _ttoi(str2);
	//str2.Format(_T("%d"), i)   ;
	k_solid = i;

	//AfxMessageBox(_T("最大视频帧数设置完成，谢谢！ "));
	CString cstr;
	cstr.Format(_T("k_solid设置完成，谢谢！k_solid为： %d"), k_solid);
	AfxMessageBox(cstr);

	//append_string_on_edit_controler(IDC_EDIT2, CString("id_camera设置完成，谢谢！id_camera为： ")
	//	+ CString(to_string(id_camera).c_str()));

	CString  cs = CString("k_solid设置完成，谢谢！k_solid为： ")
		+ CString(to_string(k_solid).c_str());// ("");
	CString str21;
	GetDlgItemText(IDC_EDIT2, str21);
	str21 = str21 + CString("\r\n");
	cs = str21 + cs;
	SetDlgItemText(IDC_EDIT2, cs);
}


void CshapeRecognizationDlg::OnBnClickedButton1_shapeReco()
{
	// TODO: 在此添加控件通知处理程序代码
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


