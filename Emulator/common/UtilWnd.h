#pragma once
#include <functional>

///  @author  Lee Jong Oh
///  @brief   윈도우 유틸리티 용으로 사용되는 api 모듬


///////////////////////////////////////////////////////////////////////////////
// Coordinate
///////////////////////////////////////////////////////////////////////////////

// rt1 에 rt2 가 포함되는지 체크한다.
bool RectContains(const CRect& rt1, const CRect& rt2);

// wnd_src 의 pt_src 좌표를 wnd_dst 의 기준 좌표로 변환해서 리턴한다.
CPoint ConvertToPoint(CWnd* wnd_src, CWnd* wnd_dst, const CPoint& pt_src);

///////////////////////////////////////////////////////////////////////////////
// Draw
///////////////////////////////////////////////////////////////////////////////

void DrawRectangle(CDC* dc, const CRect& rt, COLORREF line_color);
void DrawRectangle(CDC* dc, const CRect& rt, COLORREF line_color, COLORREF inner_color);
COLORREF GetColor(const CBrush& brush);

///////////////////////////////////////////////////////////////////////////////
// Utility for Common Controls & Windows
///////////////////////////////////////////////////////////////////////////////

// Combo Box
BOOL SetCurSel_ByData(CComboBox& combo_box, int nData);
BOOL SetCurSel_ByIndex(CComboBox& combo_box, int nIdx);
BOOL SetCurSel_ByText(CComboBox& combo_box, const CString& strText);
BOOL GetCurSel_ByText(CComboBox& combo_box, CString& strText);
BOOL GetCurSel_ByText(CComboBox& combo_box, LPSTR buffer, int len);
BOOL GetCurData(CComboBox& combo_box, int& nData);
BOOL GetCurData(CComboBox& combo_box, CString& strData);
BOOL AddComboBoxItem(CComboBox& combo_box, const CString& strItemStr, int nData);

// Edit Box
void GetEditText(CEdit& edit, int& nData);
void GetEditText(CEdit& edit, DWORD& nData);
void GetEditText(CEdit& edit, CString& value);
void GetEditText(CEdit& edit, std::string& value);
void GetEditText(CEdit& edit, std::wstring& value);

void SetEditText(CEdit& edit, const CString& value);
void SetEditText(CEdit& edit, const std::string& value);
void SetEditText(CEdit& edit, const std::wstring& value);
void SetEditText(CEdit& edit, int nData);

void SetFontCtrl(CWnd& wnd, CFont& font, int size);

BOOL GetButtonCheck(CWnd* wnd, int id);
void SetButtonCheck(CWnd* wnd, int id, BOOL value);

CButton*   GetButtonPtr(CWnd* wnd, int id);
CComboBox* GetComboBoxPtr(CWnd* wnd, int id);
CEdit*     GetEditPtr(CWnd* wnd, int id);

void FillCBFromString(HWND hWnd, UINT IDC, LPCTSTR str);
void FillCBFromResource(HWND hWnd, UINT IDC, UINT nResourceID);
void FillCBNumbers(HWND hWnd, UINT IDC, LPCTSTR sFormat, int nEnd);
void FillCBNumbers(HWND hWnd, UINT IDC, LPCTSTR sFormat, int nEnd, int nStart);

void EnableDlgItem(CWnd* pParent, UINT nIDC, bool bEnable, bool bAssert = true);
void ShowDlgItem(CWnd* pParent, UINT nIDC, int nCmd, bool bAssert = true);

// 소수 자리수 에서 불필요한 0을 제거 (지수표현법으로 표시 X)
// ex) number = "3.123000" 일때 -> "3.123" 을 리턴
CString DecimalPlaceZeroDel(const CString& number);

// DDX
// 2020-03-31 jolee
// MFC 에서 제공하는 DDX_Text 함수는 '-' 음수를 입력시에 "숫자를 입력하세요" 메세지 박스를 출력하는 문제가 있음
// 그럴때는 아래 함수를 사용해야함
void DDX_TextEx(CDataExchange* pDX, int nIDC, int& value);
void DDX_TextEx(CDataExchange* pDX, int nIDC, unsigned int& value);
void DDX_TextEx(CDataExchange* pDX, int nIDC, short& value);
void DDX_TextEx(CDataExchange* pDX, int nIDC, unsigned short& value);
void DDX_TextEx(CDataExchange* pDX, int nIDC, float& value);
void DDX_TextEx(CDataExchange* pDX, int nIDC, double& value);
void DDX_TextEx(CDataExchange* pDX, int IDC, CStringA& value);
void DDX_TextEx(CDataExchange* pDX, int IDC, LPSTR value, int nLen);

// 값이 변경되면 save_callback 함수 호출
void DDX_TextEx(CDataExchange* pDX, int nIDC, int& value, std::function<void(int src, int dst)> save_callback);
void DDX_TextEx(CDataExchange* pDX, int nIDC, float& value, std::function<void(float src, float dst)> save_callback);
void DDX_TextEx(CDataExchange* pDX, int nIDC, short& value, std::function<void(short src, short dst)> save_callback);

void DDX_CBIndexEx(CDataExchange * pDX, int nIDC, byte& value);
void DDX_CBIndexEx(CDataExchange* pDX, int nIDC, int& value);
void DDX_CBIndexEx(CDataExchange* pDX, int nIDC, unsigned int& value);
void DDX_CBIndexEx(CDataExchange * pDX, int nIDC, byte& value, std::function<void(byte src, byte dst)> save_callback);
void DDX_CBIndexEx(CDataExchange * pDX, int nIDC, int& value, std::function<void(int src, int dst)> save_callback);
void DDX_CBIndexEx(CDataExchange * pDX, int nIDC, short& value, std::function<void(short src, short dst)> save_callback);

void DDX_CheckEx(CDataExchange * pDX, int nIDC, int& value);
void DDX_CheckEx(CDataExchange* pDX, int nIDC, unsigned int& value);
void DDX_CheckEx(CDataExchange * pDX, int nIDC, char& value);
void DDX_CheckEx(CDataExchange* pDX, int nIDC, byte& value);
void DDX_CheckEx(CDataExchange * pDX, int nIDC, char& value, std::function<void(char src, char dst)> save_callback);
void DDX_CheckEx(CDataExchange * pDX, int nIDC, byte& value, std::function<void(byte src, byte dst)> save_callback);
void DDX_CheckEx(CDataExchange * pDX, int nIDC, bool& value, std::function<void(bool src, bool dst)> save_callback);
void DDX_CheckEx(CDataExchange * pDX, int nIDC, int& value, std::function<void(int src, int dst)> save_callback);

void DDX_RadioEx(CDataExchange * pDX, int nIDC, int& value, std::function<void(int src, int dst)> save_callback);

// float value = 3.14
// ex : DDX_TextFormatEx(pDX, CTRL_ID, value, _T("%0.1f"));
// EditBox -> "3.1" display
void DDX_TextFormatEx(CDataExchange* pDX, int nIDC, float& value, const CString& format = _T("%0.1f"));
void DDX_TextFormatEx(CDataExchange* pDX, int nIDC, double& value, const CString& format = _T("%0.1f"));

void DDV_MinMaxFloatEx(CDataExchange * pDX, float value, float MinVal, float MaxVal, CString & strErrorMessage);
void DDV_MinMaxDoubleEx(CDataExchange* pDX, double value, double MinVal, double MaxVal, CString& strErrorMessage);
void DDV_MinMaxShortEx(CDataExchange * pDX, short value, short MinVal, short MaxVal, CString & strErrorMessage);
void DDV_MinMaxUShortEx(CDataExchange * pDX, unsigned short value, unsigned short MinVal, unsigned short MaxVal, CString & strErrorMessage);
void DDV_MinMaxIntEx(CDataExchange * pDX, int value, int MinVal, int MaxVal, CString & strErrorMessage);
void DDV_MinMaxUintEx(CDataExchange* pDX, unsigned int value, unsigned int MinVal, unsigned int MaxVal, CString& strErrorMessage);

void SetReplaceUnit(CWnd* wnd, int ctrl_id, const CString& unit);
int  StringSplit(const CString& str, CStringArray& arr_str, const CString& token = _T("\n"));
int  StringSplit(int ids_id, CStringArray& arr_str, const CString& token = _T("\n"));

int PingTest(const CString& ip_src);
bool GetLocalIPAdressList(std::vector<std::string>& strIPArray);
std::string GetMainLocalIpAddress();

CString CFormat(UINT nFormatID, ...);
CString CFormat(const TCHAR* format, ...);