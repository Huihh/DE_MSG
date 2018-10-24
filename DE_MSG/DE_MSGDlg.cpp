
// DE_MSGDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "DE_MSG.h"
#include "DE_MSGDlg.h"
#include "afxdialogex.h"

#include "messages.pb.h"
#include "pb_decode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define COLOR_RED		0xFF0000
#define COLOR_GREEN		0x00FF00
#define COLOR_BLUE		0x0000FF
#define COLOR_BLACK		0x000000
#define COLOR_WHITE		0xFFFFFF


/*****************************************************************/
//工程所用消息的相关数据结构

#define MSG_OFF		9    //3字节头 + 2字节消息ID + 4字节长度
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

	case MessageType_MessageType_Features:
		result = decoding_Features(g_Msg_Size);
		break;

	case MessageType_MessageType_ResetDevice:
		result = decoding_ResetDevice(g_Msg_Size);
		break;

	case MessageType_MessageType_EntropyRequest:
		result = decoding_EntropyRequest(g_Msg_Size);
		break;

	case MessageType_MessageType_EntropyAck:
		result = decoding_EntropyAck(g_Msg_Size);
		break;
		
	case MessageType_MessageType_Success:
		result = decoding_Success(g_Msg_Size);
		break;

	case MessageType_MessageType_GetFeatures:
		result = decoding_GetFeatures(g_Msg_Size);
		break;
		
	case MessageType_MessageType_GetPublicKey:
		result = decoding_GetPublicKey(g_Msg_Size);
		break;

	case MessageType_MessageType_PublicKey:
		result = decoding_PublicKey(g_Msg_Size);
		break;

	case MessageType_MessageType_GetAddress:
		result = decoding_GetAddress(g_Msg_Size);
		break;
		
	case MessageType_MessageType_Address:
		result = decoding_Address(g_Msg_Size);
		break;

	case MessageType_MessageType_BackupDevice:
		result = decoding_BackupDevice(g_Msg_Size);
		break;
		
	case MessageType_MessageType_ButtonRequest:
		result = decoding_ButtonRequest(g_Msg_Size);
		break;

	case MessageType_MessageType_ButtonAck:
		result = decoding_ButtonAck(g_Msg_Size);
		break;

	case MessageType_MessageType_ApplySettings:
		result = decoding_ApplySettings(g_Msg_Size);
		break;
		
	case MessageType_MessageType_ChangePin:
		result = decoding_ChangePin(g_Msg_Size);
		break;
		
	case MessageType_MessageType_ApplyFlags:
		result = decoding_ApplyFlags(g_Msg_Size);
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
	BOOL retCode;

	m_re_result.SetWindowTextA(_T(""));

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

	retCode = DecodeMessageByID(g_Msg_Id, g_Msg_Size);

	if (retCode) {
		ShowMessageString(_T("解码成功"), COLOR_BLACK);
	}
	else {
		ShowMessageString(_T("解码失败"), COLOR_RED);
	}


	

}


/* ---+++---+++---+++---+++  消息解码函数 开始  ---+++---+++---+++---+++ */


BOOL CDEMSGDlg::decoding_Initialize(UINT g_Msg_Size)
{
	
	CString strTemp, strDisp;
	struct _Initialize message = Initialize_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, Initialize_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_Initialize ***********"), COLOR_BLUE);

	if (message.has_state) {
		strDisp += _T("\n state = ");
		for (int i = 0; i < message.state.size; i++) {
			strTemp.Format("%2x", message.state.bytes[i]);
			strDisp += strTemp;
		}
	}

	if (message.has_skip_passphrase) {
		strDisp += _T("\n skip_passphrase = ");
		strTemp.Format("%2x", message.skip_passphrase);
		strDisp += strTemp;
	}

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_Initialize ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_Features(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	Features message = Features_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, Features_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_Features ***********"), COLOR_BLUE);

	if (message.has_vendor) {
		strDisp += _T("\n vendor = ");
		strTemp = message.vendor;
		strDisp += strTemp;
	}

	if (message.has_major_version) {
		strDisp += _T("\n major_version = ");
		strTemp.Format("%2x", message.major_version);
		strDisp += strTemp;
	}


	if (message.has_minor_version) {
		strDisp += _T("\n minor_version = ");
		strTemp.Format("%2x", message.minor_version);
		strDisp += strTemp;
	}


	if (message.has_patch_version) {
		strDisp += _T("\n patch_version = ");
		strTemp.Format("%2x", message.patch_version);
		strDisp += strTemp;
	}

	if (message.has_bootloader_mode) {
		strDisp += _T("\n bootloader_mode = ");
		strTemp.Format("%2x", message.bootloader_mode);
		strDisp += strTemp;


	}

	if (message.has_device_id) {
		strDisp += _T("\n device_id = ");
		strTemp = message.device_id;
		strDisp += strTemp;
	}

	if (message.has_pin_protection) {
		strDisp += _T("\n pin_protection = ");
		strTemp.Format("%2x", message.pin_protection);
		strDisp += strTemp;
	}

	if (message.has_passphrase_protection) {
		strDisp += _T("\n passphrase_protection = ");
		strTemp.Format("%2x", message.passphrase_protection);
		strDisp += strTemp;
	}

	if (message.has_language) {
		strDisp += _T("\n language = ");
		strTemp = message.language;
		strDisp += strTemp;
	}

	if (message.has_label) {
		strDisp += _T("\n label = ");
		strTemp = message.label;
		strDisp += strTemp;
	}

	if (message.has_initialized) {
		strDisp += _T("\n initialized = ");
		strTemp.Format("%2x", message.initialized);
		strDisp += strTemp;
	}

	if (message.has_revision) {
		strDisp += _T("\n revision = ");
		for (int i = 0; i < message.revision.size; i++) {
			strTemp.Format("%2x", message.revision.bytes[i]);
			strDisp += strTemp;
		}
	}

	if (message.has_bootloader_hash) {
		strDisp += _T("\n bootloader_hash = ");
		for (int i = 0; i < message.bootloader_hash.size; i++) {
			strTemp.Format("%2x", message.bootloader_hash.bytes[i]);
			strDisp += strTemp;
		}
	}

	if (message.has_imported) {
		strDisp += _T("\n imported = ");
		strTemp.Format("%2x", message.imported);
		strDisp += strTemp;
	}

	if (message.has_pin_cached) {
		strDisp += _T("\n pin_cached = ");
		strTemp.Format("%2x", message.pin_cached);
		strDisp += strTemp;
	}

	if (message.has_passphrase_cached) {
		strDisp += _T("\n passphrase_cached = ");
		strTemp.Format("%2x", message.passphrase_cached);
		strDisp += strTemp;
	}

	if (message.has_firmware_present) {
		strDisp += _T("\n firmware_present = ");
		strTemp.Format("%2x", message.firmware_present);
		strDisp += strTemp;
	}

	if (message.has_needs_backup) {
		strDisp += _T("\n needs_backup = ");
		strTemp.Format("%2x", message.needs_backup);
		strDisp += strTemp;
	}

	if (message.has_flags) {
		strDisp += _T("\n flags = ");
		strTemp.Format("%2x", message.flags);
		strDisp += strTemp;
	}

	if (message.has_model) {
		strDisp += _T("\n model = ");
		strTemp = message.model;
		strDisp += strTemp;
	}

	if (message.has_fw_major) {
		strDisp += _T("\n fw_major = ");
		strTemp.Format("%2x", message.fw_major);
		strDisp += strTemp;
	}

	if (message.has_fw_minor) {
		strDisp += _T("\n fw_minor = ");
		strTemp.Format("%2x", message.fw_minor);
		strDisp += strTemp;
	}

	if (message.has_fw_patch) {
		strDisp += _T("\n fw_patch = ");
		strTemp.Format("%2x", message.fw_patch);
		strDisp += strTemp;
	}

	if (message.has_fw_vendor) {
		strDisp += _T("\n fw_vendor = ");
		strTemp = message.fw_vendor;
		strDisp += strTemp;
	}

	if (message.has_fw_vendor_keys) {
		strDisp += _T("\n fw_vendor_keys = ");
		for (int i = 0; i < message.fw_vendor_keys.size; i++) {
			strTemp.Format("%2x", message.fw_vendor_keys.bytes[i]);
			strDisp += strTemp;
		}
	}

	if (message.has_unfinished_backup) {
		strDisp += _T("\n unfinished_backup = ");
		strTemp.Format("%2x", message.unfinished_backup);
		strDisp += strTemp;
	}

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_Features ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_ResetDevice(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	ResetDevice message = ResetDevice_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, ResetDevice_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_ResetDevice ***********"), COLOR_BLUE);

	if (message.has_display_random) {
		strDisp += _T("\n display_random = ");
		strTemp.Format("%2x", message.display_random);
		strDisp += strTemp;
	}

	if (message.has_strength) {
		strDisp += _T("\n strength = ");
		strTemp.Format("%2x", message.strength);
		strDisp += strTemp;
	}

	if (message.has_passphrase_protection) {
		strDisp += _T("\n passphrase_protection = ");
		strTemp.Format("%2x", message.passphrase_protection);
		strDisp += strTemp;
	}

	if (message.has_pin_protection) {
		strDisp += _T("\n pin_protection = ");
		strTemp.Format("%2x", message.pin_protection);
		strDisp += strTemp;
	}

	if (message.has_language) {
		strDisp += _T("\n language = ");
		strTemp = message.language;
		strDisp += strTemp;
	}

	if (message.has_label) {
		strDisp += _T("\n label = ");
		strTemp = message.label;
		strDisp += strTemp;
	}

	if (message.has_u2f_counter) {
		strDisp += _T("\n u2f_counter = ");
		strTemp.Format("%2x", message.u2f_counter);
		strDisp += strTemp;
	}

	if (message.has_skip_backup) {
		strDisp += _T("\n skip_backup = ");
		strTemp.Format("%2x", message.skip_backup);
		strDisp += strTemp;
	}

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_ResetDevice ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_EntropyRequest(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	EntropyRequest message = EntropyRequest_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, EntropyRequest_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_EntropyRequest ***********"), COLOR_BLUE);

	AfxMessageBox(_T("EntropyRequest is dummy_field"));

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_EntropyRequest ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_EntropyAck(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	EntropyAck message = EntropyAck_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, EntropyAck_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_EntropyAck ***********"), COLOR_BLUE);

	if (message.has_entropy) {
		strDisp += _T("\n entropy = ");
		for (int i = 0; i < message.entropy.size; i++) {
			strTemp.Format("%2x", message.entropy.bytes[i]);
			strDisp += strTemp;
		}
	}

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_EntropyAck ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_Success(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	Success message = Success_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, Success_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_Success ***********"), COLOR_BLUE);
	
	if (message.has_message) {
		strDisp += _T("\n message = ");
		strTemp = message.message;
		strDisp += strTemp;
	}

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_Success ***********"), COLOR_BLUE);

	return TRUE;
}



BOOL CDEMSGDlg::decoding_GetFeatures(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	GetFeatures message = GetFeatures_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, GetFeatures_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_GetFeatures ***********"), COLOR_BLUE);

	AfxMessageBox(_T("EntropyRequest is dummy_field"));

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_Success ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_GetPublicKey(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	GetPublicKey message = GetPublicKey_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, GetPublicKey_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_GetPublicKey ***********"), COLOR_BLUE);

	strDisp += _T("\n address_n_count = ");
	strTemp.Format("%2x", message.address_n_count);
	strDisp += strTemp;

	strDisp += _T("\n address_n[8] = ");
	for (int i = 0; i < 8; i++) {
		strTemp.Format("%4x ", message.address_n[i]);
		strDisp += strTemp;
	}
	
	if (message.has_ecdsa_curve_name) {
		strDisp += _T("\n ecdsa_curve_name = ");
		strTemp = message.ecdsa_curve_name;
		strDisp += strTemp;
	}

	if (message.has_show_display) {
		strDisp += _T("\n show_display = ");
		strTemp.Format("%2x", message.show_display);
		strDisp += strTemp;
	}

	if (message.has_coin_name) {
		strDisp += _T("\n coin_name = ");
		strTemp = message.coin_name;
		strDisp += strTemp;
	}

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_GetPublicKey ***********"), COLOR_BLUE);

	return TRUE;
}



BOOL CDEMSGDlg::decoding_PublicKey(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	PublicKey message = PublicKey_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, PublicKey_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_PublicKey ***********"), COLOR_BLUE);

	strDisp += _T("\n node.depth = ");
	strTemp.Format("%2x", message.node.depth);
	strDisp += strTemp;
	
	strDisp += _T("\n node.fingerprint = ");
	strTemp.Format("%2x", message.node.fingerprint);
	strDisp += strTemp;

	strDisp += _T("\n node.child_num = ");
	strTemp.Format("%2x", message.node.child_num);
	strDisp += strTemp;

	strDisp += _T("\n node.chain_code = ");
	for (int i = 0; i < message.node.chain_code.size; i++) {
		strTemp.Format("%2x", message.node.chain_code.bytes[i]);
		strDisp += strTemp;
	}

	if (message.node.has_private_key) {
		strDisp += _T("\n node.private_key = ");
		for (int i = 0; i < message.node.private_key.size; i++) {
			strTemp.Format("%2x", message.node.private_key.bytes[i]);
			strDisp += strTemp;
		}
	}

	if (message.node.has_public_key) {
		strDisp += _T("\n node.public_key = ");
		for (int i = 0; i < message.node.public_key.size; i++) {
			strTemp.Format("%2x", message.node.public_key.bytes[i]);
			strDisp += strTemp;
		}
	}

	if (message.has_xpub) {
		strDisp += _T("\n xpub = ");
		strTemp = message.xpub;
		strDisp += strTemp;
	}

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_PublicKey ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_GetAddress(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	GetAddress message = GetAddress_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, GetAddress_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_GetAddress ***********"), COLOR_BLUE);

	strDisp += _T("\n address_n_count = ");
	strTemp.Format("%2x", message.address_n_count);
	strDisp += strTemp;

	strDisp += _T("\n address_n[8] = ");
	for (int i = 0; i < 8; i++) {
		strTemp.Format("%4x ", message.address_n[i]);
		strDisp += strTemp;
	}

	if (message.has_show_display) {
		strDisp += _T("\n show_display = ");
		strTemp.Format("%2x", message.show_display);
		strDisp += strTemp;
	}

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_GetAddress ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_Address(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	Address message = Address_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, Address_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_Address ***********"), COLOR_BLUE);

	strDisp += _T("\n address = ");
	strTemp = message.address;
	strDisp += strTemp;

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_Address ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_BackupDevice(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	BackupDevice message = BackupDevice_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, BackupDevice_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_BackupDevice ***********"), COLOR_BLUE);

	AfxMessageBox(_T("BackupDevice is dummy_field"));

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_BackupDevice ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_ButtonRequest(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	ButtonRequest message = ButtonRequest_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, ButtonRequest_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_ButtonRequest ***********"), COLOR_BLUE);

	if (message.has_code) {
		strDisp += _T("\n code = ");
		strTemp.Format("%2x", message.code);
		strDisp += strTemp;
	}

	if (message.has_data) {
		strDisp += _T("\n data = ");
		strTemp = message.data;
		strDisp += strTemp;
	}

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_ButtonRequest ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_ButtonAck(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	ButtonAck message = ButtonAck_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, ButtonAck_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_ButtonAck ***********"), COLOR_BLUE);

	AfxMessageBox(_T("ButtonAck is dummy_field"));

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_ButtonAck ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_ApplySettings(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	ApplySettings message = ApplySettings_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, ApplySettings_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_ApplySettings ***********"), COLOR_BLUE);

	if (message.has_language) {
		strDisp += _T("\n language = ");
		strTemp = message.language;
		strDisp += strTemp;
	}

	if (message.has_label) {
		strDisp += _T("\n label = ");
		strTemp = message.label;
		strDisp += strTemp;
	}

	if (message.has_use_passphrase) {
		strDisp += _T("\n use_passphrase = ");
		strTemp.Format("%2x", message.use_passphrase);
		strDisp += strTemp;
	}

	if (message.has_homescreen) {
		strDisp += _T("\n homescreen = ");
		for (int i = 0; i < message.homescreen.size; i++) {
			strTemp.Format("%2x", message.homescreen.bytes[i]);
			strDisp += strTemp;
		}
	}

	if (message.has_passphrase_source) {
		strDisp += _T("\n passphrase_source = ");
		strTemp.Format("%2x", message.passphrase_source);
		strDisp += strTemp;
	}

	if (message.has_auto_lock_delay_ms) {
		strDisp += _T("\n auto_lock_delay_ms = ");
		strTemp.Format("%2x", message.auto_lock_delay_ms);
		strDisp += strTemp;
	}

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_ApplySettings ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_ChangePin(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	ChangePin message = ChangePin_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, ChangePin_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_ChangePin ***********"), COLOR_BLUE);

	if (message.has_remove) {
		strDisp += _T("\n remove = ");
		strTemp.Format("%2x", message.remove);
		strDisp += strTemp;
	}

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_ChangePin ***********"), COLOR_BLUE);

	return TRUE;
}


BOOL CDEMSGDlg::decoding_ApplyFlags(UINT g_Msg_Size)
{
	CString strTemp, strDisp;
	ApplyFlags message = ApplyFlags_init_default;
	pb_istream_t stream = pb_istream_from_buffer(&g_DataBuf[MSG_OFF], g_Msg_Size);
	BOOL status = pb_decode(&stream, ApplyFlags_fields, &message);
	if (!status) {
		AfxMessageBox(_T("pb_decode failed"));
		return FALSE;
	}

	ShowMessageString(_T("*********** Start MessageType_MessageType_ApplyFlags ***********"), COLOR_BLUE);

	if (message.has_flags) {
		strDisp += _T("\n flags = ");
		strTemp.Format("%2x", message.flags);
		strDisp += strTemp;
	}

	strDisp += "\n";
	ShowMessageString(strDisp, COLOR_BLACK);
	ShowMessageString(_T("*********** End  MessageType_MessageType_ApplyFlags ***********"), COLOR_BLUE);

	return TRUE;
}


/* ---+++---+++---+++---+++  消息解码函数 结束  ---+++---+++---+++---+++ */






