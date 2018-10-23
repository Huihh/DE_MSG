
// DE_MSGDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "DE_MSG.h"
#include "DE_MSGDlg.h"
#include "afxdialogex.h"

#include "messages.pb.h"
#include "decode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




/*****************************************************************/
//工程所用到的全局变量
BYTE g_DataBuf[12 * 1024] = { 0 };

UINT16 g_Msg_Id = 0;
UINT   g_Msg_Size = 0;


/*****************************************************************/



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


// CDEMSGDlg 对话框



CDEMSGDlg::CDEMSGDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DE_MSG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDEMSGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_RESULT, m_re_result);
	DDX_Control(pDX, IDC_EDIT_DATA, m_ce_data);
}

BEGIN_MESSAGE_MAP(CDEMSGDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DECODE, &CDEMSGDlg::OnBnClickedButtonDecode)
END_MESSAGE_MAP()


// CDEMSGDlg 消息处理程序

BOOL CDEMSGDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDEMSGDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDEMSGDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDEMSGDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/***************************************************************************************/
void CDEMSGDlg::ShowMessageString(CString Message, int color)
{
	CHARFORMAT	cf;
	int r, g, b;
	int len;

	r = ((color >> 16) & 0xFF);
	g = ((color >> 8) & 0xFF);
	b = ((color >> 0) & 0xFF);

	memset(&cf, 0, sizeof(cf));

	m_re_result.GetDefaultCharFormat(cf);


	cf.dwMask = CFM_COLOR;
	cf.dwEffects &= ~CFE_AUTOCOLOR;

	cf.crTextColor = RGB(r, g, b);

	m_re_result.SetSel(-1, -1);
	m_re_result.SetSelectionCharFormat(cf);

	len = m_re_result.GetWindowTextLength();

	if ((len + Message.GetLength()) > (m_re_result.GetLimitText()))
	{
		m_re_result.SetWindowText(_T("Clear Screen ...\r\n"));
	}

	Message += "\r\n";


	m_re_result.SetSel(-1, -1);
	m_re_result.ReplaceSel(Message);

	m_re_result.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

}



void CDEMSGDlg::CstringToByte(CString sInput, BYTE bOutput[])
{
	BYTE srcBuf[12 * 1024] = { 0 };	// The Bigger the better     by Huihh 2016.9.12
	BYTE desBuf[12 * 1024] = { 0 };
	int srcLen = 0;


	strcpy((char *)srcBuf, sInput);
	srcLen = strlen(sInput);


	AsciiToHex(srcBuf, desBuf, srcLen);

	for (int i = 0; i<(srcLen / 2); i++)
	{
		bOutput[i] = desBuf[i];
	}

}



BOOL CDEMSGDlg::AsciiToHex(BYTE pAsciiArray[], BYTE pHexArray[], int Len)
{
	BYTE tempBuf[2] = { 0 };

	if (Len % 2 != 0)
	{
		AfxMessageBox(_T("Ascii Convert Hex Failed, Please input Convert Length in even numbers, Try Again Later"));
		return FALSE;
	}

	int HexLen = Len / 2;   // 2 Character Convert 1 Hex  by Huihh 2016.9.8

	for (int i = 0; i<HexLen; i++)
	{
		tempBuf[0] = *pAsciiArray++;
		tempBuf[1] = *pAsciiArray++;

		for (int j = 0; j<2; j++)
		{
			if (tempBuf[j] <= 'F' && tempBuf[j] >= 'A')
			{
				tempBuf[j] = tempBuf[j] - 'A' + 10;
			}
			else if (tempBuf[j] <= 'f' && tempBuf[j] >= 'a')
			{
				tempBuf[j] = tempBuf[j] - 'a' + 10;
			}
			else if (tempBuf[j] >= '0' && tempBuf[j] <= '9')
			{
				tempBuf[j] = tempBuf[j] - '0';
			}
			else
			{
				AfxMessageBox(_T("pAsciiArray Contain illegality Character, Please Try Again after Check "));
				return FALSE;
			}
		}

		pHexArray[i] = (tempBuf[0] << 4) + tempBuf[1];
	}

	return TRUE;
}

BOOL CDEMSGDlg::DecodeMessageByID(UINT16 g_Msg_Id, UINT g_Msg_Size)
{
	BOOL result = TRUE;

	switch (g_Msg_Id) {
	case MessageType_MessageType_Initialize:
		result = decoding_Initialize(g_Msg_Size);
		break;

	case MessageType_MessageType_Ping:
		break;

	case MessageType_MessageType_Success:
		break;

	case MessageType_MessageType_Failure:
		break;

	default:
		AfxMessageBox("Message ID Error, Try again after check");
		result = FALSE;
	}


	return result;
}



void CDEMSGDlg::OnBnClickedButtonDecode()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strData, strTemp, strConvert;
	INT currOff = 0;


	GetDlgItemText(IDC_EDIT_DATA, strData);

	if (strData.GetLength() == 0) {
		AfxMessageBox(_T("Please input decode data, Then try again"));
		return;
	}

	if (strData.GetLength() % 128 != 0) {
		AfxMessageBox(_T("decode data length is must be multiple of 64-Byte, Try again after check "));
		return;
	}

	//去掉连续包的包头无效字节 "3F"   by Huihh 2018.10.23
	while (strData.GetLength() > currOff) {
		strTemp = strData.Mid(currOff, 128);
		if (0 != currOff) {
			strTemp = strTemp.Mid(2, 126);
		}

		strConvert += strTemp;
		currOff += 128;
	}

	CstringToByte(strConvert, g_DataBuf);


	g_Msg_Id = (g_DataBuf[3] << 8) + g_DataBuf[4];
	g_Msg_Size = (g_DataBuf[5] << 24) + (g_DataBuf[6] << 16) + (g_DataBuf[7] << 8) + g_DataBuf[8];

	DecodeMessageByID(g_Msg_Id, g_Msg_Size);

}
