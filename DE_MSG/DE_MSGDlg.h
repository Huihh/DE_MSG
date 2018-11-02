
// DE_MSGDlg.h: 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDEMSGDlg 对话框
class CDEMSGDlg : public CDialog
{
// 构造
public:
	CDEMSGDlg(CWnd* pParent = NULL);	// 标准构造函数



	//Show the message
	void ShowMessageString(CString Message, int color);

	//CString convert to BYTE
	void CstringToByte(CString sInput, BYTE bOutput[]);

	//Ascii convert to Hex
	BOOL AsciiToHex(BYTE pAsciiArray[], BYTE pHexArray[], int Len);

	//decode message by ID through protobuf  
	BOOL DecodeMessageByID(UINT16 g_Msg_Id, UINT g_Msg_Size);

/**********************************************************************/
//消息解码函数集合
	BOOL decoding_Initialize(UINT g_Msg_Size);
	BOOL decoding_Features(UINT g_Msg_Size);
	BOOL decoding_ResetDevice(UINT g_Msg_Size);
	BOOL decoding_EntropyRequest(UINT g_Msg_Size);
	BOOL decoding_EntropyAck(UINT g_Msg_Size);
	BOOL decoding_Success(UINT g_Msg_Size);
	BOOL decoding_GetFeatures(UINT g_Msg_Size);
	BOOL decoding_GetPublicKey(UINT g_Msg_Size);
	BOOL decoding_PublicKey(UINT g_Msg_Size);
	BOOL decoding_GetAddress(UINT g_Msg_Size);
	BOOL decoding_Address(UINT g_Msg_Size);
	BOOL decoding_BackupDevice(UINT g_Msg_Size);
	BOOL decoding_ButtonRequest(UINT g_Msg_Size);
	BOOL decoding_ButtonAck(UINT g_Msg_Size);
	BOOL decoding_ApplySettings(UINT g_Msg_Size);
	BOOL decoding_ChangePin(UINT g_Msg_Size);
	BOOL decoding_ApplyFlags(UINT g_Msg_Size);
	BOOL decoding_Failure(UINT g_Msg_Size);
	
/**********************************************************************/


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DE_MSG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CRichEditCtrl m_re_result;
	CEdit m_ce_data;

	afx_msg void OnBnClickedButtonDecode();
};
