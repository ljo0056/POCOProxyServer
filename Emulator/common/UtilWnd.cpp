#include "pch.h"
#include "UtilWnd.h"
#include <processthreadsapi.h>


bool RectContains(const CRect& rt1, const CRect& rt2)
{
    if (rt1.PtInRect(rt2.TopLeft()) &&
        rt1.PtInRect(rt2.BottomRight()))
        return true;

    return false;
}

CPoint ConvertToPoint(CWnd* wnd_src, CWnd* wnd_dst, const CPoint& pt_src)
{
    CRect win_rt(pt_src, CSize(0, 0));
    wnd_src->ClientToScreen(win_rt);

    CPoint pt_convert = win_rt.TopLeft();
    wnd_dst->ScreenToClient(&pt_convert);

    return pt_convert;
}

void DrawRectangle(CDC* dc, const CRect& rt, COLORREF line_color)
{
    CPen pen(PS_SOLID, 1, line_color);
    CBrush brush;
    brush.CreateStockObject(NULL_BRUSH);

    CPen*   old_pen   = dc->SelectObject(&pen);
    CBrush* old_brush = dc->SelectObject(&brush);

    dc->Rectangle(rt);

    dc->SelectObject(old_pen);
    dc->SelectObject(old_brush);
}

void DrawRectangle(CDC* dc, const CRect& rt, COLORREF line_color, COLORREF inner_color)
{
    CPen pen(PS_SOLID, 1, line_color);
    CBrush brush(inner_color);

    CPen*   old_pen   = dc->SelectObject(&pen);
    CBrush* old_brush = dc->SelectObject(&brush);

    dc->Rectangle(rt);

    dc->SelectObject(old_pen);
    dc->SelectObject(old_brush);
}

COLORREF GetColor(const CBrush& brush)
{
    LOGBRUSH lb;
    brush.GetObject(sizeof(LOGBRUSH), &lb);
    byte r = GetRValue(lb.lbColor);
    byte g = GetGValue(lb.lbColor);
    byte b = GetBValue(lb.lbColor);

    return RGB(r, g, b);
}

// Combo Box
BOOL SetCurSel_ByData(CComboBox& combo_box, int nData)
{
    int nItem = combo_box.GetCount();
    for (int i = 0; i < nItem; i++) {
        DWORD data = (DWORD)combo_box.GetItemData(i);
        if (data == nData) {
            combo_box.SetCurSel(i);
            break;
        }
    }
    return TRUE;
}

BOOL SetCurSel_ByIndex(CComboBox& combo_box, int nIdx)
{
    int nItem = combo_box.GetCount();
    if (nItem == 0) return FALSE;
    if (nIdx < 0 || nIdx >= nItem) {
        combo_box.SetCurSel(0);
    }
    else combo_box.SetCurSel(nIdx);
    return TRUE;
}

BOOL SetCurSel_ByText(CComboBox& combo_box, const CString& strText)
{
    CString strValue;
    int i;
    int nItem = combo_box.GetCount();
    for (i = 0; i < nItem; i++) {
        combo_box.GetLBText(i, strValue);
        if (strValue == strText)
        {
            combo_box.SetCurSel(i);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL GetCurSel_ByText(CComboBox& combo_box, CString& strText)
{
    combo_box.GetLBText(combo_box.GetCurSel(), strText);
    return TRUE;
}

BOOL GetCurSel_ByText(CComboBox& combo_box, LPSTR buffer, int len)
{
    CString strText;
    combo_box.GetLBText(combo_box.GetCurSel(), strText);

    if (FALSE == strText.IsEmpty())
    {
        CStringA str_a;
        str_a = strText;
        strncpy_s(buffer, len, str_a.GetString(), len);
    }
    return TRUE;
}

BOOL GetCurData(CComboBox& combo_box, int& nData)
{
    if (combo_box.GetCount() == 0) return FALSE;
    int nCurSelIdx = combo_box.GetCurSel();
    nData = (int)combo_box.GetItemData(nCurSelIdx);
    return TRUE;
}

BOOL GetCurData(CComboBox& combo_box, CString& strData)
{
    if (combo_box.GetCount() == 0) return FALSE;
    CString* pStringData = (CString*)combo_box.GetItemData(combo_box.GetCurSel());
    if (pStringData) {
        strData = *pStringData;
        return TRUE;
    }
    return FALSE;
}

BOOL AddComboBoxItem(CComboBox& combo_box, const CString& strItemStr, int nData)
{
    int nItemIdx = combo_box.AddString(strItemStr);
    combo_box.SetItemData(nItemIdx, DWORD_PTR(nData));
    return TRUE;
}

// Edit Box
void GetEditText(CEdit& edit, int& nData)
{
    CString strText;
    edit.GetWindowText(strText);
    nData = _ttoi(strText);
}

void GetEditText(CEdit& edit, DWORD& nData)
{
    CString strText;
    edit.GetWindowText(strText);
    nData = _ttoi(strText);
}

void GetEditText(CEdit& edit, CString& value)
{
    CString text;
    GetWindowText(edit.GetSafeHwnd(), text.GetBuffer(256), 256);
    text.ReleaseBuffer();

    value = text.GetString();
}

void GetEditText(CEdit& edit, std::string& value)
{
    CStringA text;
    GetWindowTextA(edit.GetSafeHwnd(), text.GetBuffer(256), 256);
    text.ReleaseBuffer();

    value = text.GetString();
}

void GetEditText(CEdit& edit, std::wstring& value)
{
    CStringW text;
    GetWindowTextW(edit.GetSafeHwnd(), text.GetBuffer(256), 256);
    text.ReleaseBuffer();

    value = text.GetString();
}

void SetEditText(CEdit& edit, const CString& value)
{
    edit.SetWindowText(value);
}

void SetEditText(CEdit& edit, const std::string& value)
{
    SetWindowTextA(edit.GetSafeHwnd(), value.c_str());
}

void SetEditText(CEdit& edit, const std::wstring& value)
{
    SetWindowTextW(edit.GetSafeHwnd(), value.c_str());
}

void SetEditText(CEdit& edit, int nData)
{
    CString strText;
    strText.Format(_T("%d"), nData);
    edit.SetWindowText(strText);
}

void SetFontCtrl(CWnd& wnd, CFont& font, int size)
{
    LOGFONT log_font;
    wnd.GetFont()->GetLogFont(&log_font);
    log_font.lfHeight = size * 10;

    font.DeleteObject();
    font.CreatePointFontIndirect(&log_font);
    wnd.SetFont(&font);
}

BOOL GetButtonCheck(CWnd* wnd, int id)
{
    return GetButtonPtr(wnd, id)->GetCheck() == BST_CHECKED ? TRUE : FALSE;
}

void SetButtonCheck(CWnd* wnd, int id, BOOL value)
{
    GetButtonPtr(wnd, id)->SetCheck(value == TRUE ? BST_CHECKED : BST_UNCHECKED);
}

CButton* GetButtonPtr(CWnd* wnd, int id)
{
    return static_cast<CButton*>(wnd->GetDlgItem(id));
}

CComboBox* GetComboBoxPtr(CWnd* wnd, int id)
{
    return static_cast<CComboBox*>(wnd->GetDlgItem(id));
}

CEdit* GetEditPtr(CWnd* wnd, int id)
{
    return static_cast<CEdit*>(wnd->GetDlgItem(id));
}

inline void CB_PREPARE_HELPER(HWND hCombo)
{
    ::SendMessage(hCombo, WM_SETREDRAW, WPARAM(FALSE), 0);
    ::SendMessage(hCombo, CB_RESETCONTENT, 0, 0);
}

inline void CB_DONE_HELPER(HWND hCombo)
{
    ::SendMessage(hCombo, WM_SETREDRAW, WPARAM(TRUE), 0);
    ::RedrawWindow(hCombo, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

void FillCBFromString(HWND hWnd, UINT IDC, LPCTSTR str)
{
    HWND hCombo = ::GetDlgItem(hWnd, IDC);
    ASSERT(hCombo);
    CB_PREPARE_HELPER(hCombo);

    CString sLines(str);

    int nLastNewline = 0;  // Pozice prvniho znaku po konci radku
    while (sLines.Find(_T('\n'), nLastNewline) != -1)
    {
        int nNewNewline = sLines.Find(_T('\n'), nLastNewline);
        CString sItem = sLines.Mid(nLastNewline, nNewNewline - nLastNewline);
        ::SendMessage(hCombo, CB_ADDSTRING, 0, LPARAM(LPCTSTR(sItem)));
        nLastNewline = nNewNewline + 1;
    }

    if (sLines.GetLength() != nLastNewline)
    {  // za poslednim \n jeste zbyvaji znaky, tj. string typu "AAA\nBBB\nCCC"
        CString sItem = sLines.Mid(nLastNewline);
        ::SendMessage(hCombo, CB_ADDSTRING, 0, LPARAM(LPCTSTR(sItem)));
    }

    CB_DONE_HELPER(hCombo);
}

void FillCBFromResource(HWND hWnd, UINT IDC, UINT nResourceID)
{
    CString sLines;
    VERIFY(sLines.LoadString(nResourceID));

    FillCBFromString(hWnd, IDC, sLines);
}

void FillCBNumbers(HWND hWnd, UINT IDC, LPCTSTR sFormat, int nEnd)
{
    HWND hCombo = ::GetDlgItem(hWnd, IDC);
    ASSERT(hCombo);
    CB_PREPARE_HELPER(hCombo);

    for (int i = 1; i <= nEnd; i++)
    {
        CString strEntry;
        strEntry.Format(sFormat, i);
        ::SendMessage(hCombo, CB_ADDSTRING, 0, LPARAM(LPCTSTR(strEntry)));
    }
    CB_DONE_HELPER(hCombo);
}

void FillCBNumbers(HWND hWnd, UINT IDC, LPCTSTR sFormat, int nEnd, int nStart)
{
    HWND hCombo = ::GetDlgItem(hWnd, IDC);
    ASSERT(hCombo);
    CB_PREPARE_HELPER(hCombo);

    if (nStart > nEnd && nEnd >= 1)
        nStart = 1;
    if (nStart > nEnd)
        nEnd = nStart;

    for (int i = nStart; i <= nEnd; i++)
    {
        CString strEntry;
        strEntry.Format(sFormat, i);
        ::SendMessage(hCombo, CB_ADDSTRING, 0, LPARAM(LPCTSTR(strEntry)));
    }
    CB_DONE_HELPER(hCombo);
}

void EnableDlgItem(CWnd* pParent, UINT nIDC, bool bEnable, bool bAssert)
{
    if (!pParent)
        return;
    CWnd* pWnd = pParent->GetDlgItem(nIDC);
    if (pWnd)
    {
        pWnd->EnableWindow(bEnable);
    }
    else
    {
        if (bAssert)
        {
            ASSERT(FALSE);
        }
        TRACE(_T("No DLG Item with ID=%i"), nIDC);
    }
}

void ShowDlgItem(CWnd* pParent, UINT nIDC, int nCmd, bool bAssert)
{
    if (!pParent)
        return;
    CWnd* pWnd = pParent->GetDlgItem(nIDC);
    if (pWnd)
    {
        pWnd->ShowWindow(nCmd);
    }
    else
    {
        if (bAssert)
        {
            ASSERT(FALSE);
        }
        TRACE(_T("No DLG Item with ID=%i"), nIDC);
    }
}

CString DecimalPlaceZeroDel(const CString& number)
{
    CString text = number;
    int pos_dot = text.Find(_T('.'));
    int no_zero = -1;

    if (pos_dot > -1)
    {
        int text_len = text.GetLength();
        for (int ii = text_len - 1 ; ii >= pos_dot ; ii--)
        {
            if (_T('0') != text[ii])
            {
                no_zero = ii;
                break;
            }
        }

        if (pos_dot == no_zero)
            text.Delete(no_zero, text_len - (no_zero));
        else if (no_zero > -1)
            text.Delete(no_zero + 1, text_len - (no_zero + 1));
    }

    return text;
}

void DDX_TextFormatT(CDataExchange* pDX, int nIDC, std::function<void(const CString&)> save_func, std::function<CString()> load_func)
{
    CString text;
    HWND hwnd = NULL;

    pDX->PrepareEditCtrl(nIDC);
    pDX->m_pDlgWnd->GetDlgItem(nIDC, &hwnd);

    if (pDX->m_bSaveAndValidate)    // Save
    {
        GetWindowText(hwnd, text.GetBuffer(256), 256);
        text.ReleaseBuffer();

        if (text.IsEmpty())
        {
            AfxMessageBox(AFX_IDP_PARSE_REAL);
            pDX->Fail();
        }

        lconv * loc;
        TCHAR chDecSep;
        loc = localeconv();
        ASSERT(loc);
        if (loc)
            chDecSep = *loc->decimal_point;

        CString temp = _T("-.0123456789");
        temp += chDecSep;

        if (text != text.SpanIncluding(temp))
        {
            AfxMessageBox(AFX_IDP_PARSE_REAL);
            pDX->Fail();
        }

        save_func(text);
    }
    else // Load
    {
        text = load_func();
        SetWindowText(hwnd, text.GetString());
    }
}

void DDX_TextFormatIntT(CDataExchange* pDX, int nIDC, std::function<void(const CString&)> save_func, std::function<CString()> load_func)
{
    CString text;
    HWND hwnd = NULL;

    pDX->PrepareEditCtrl(nIDC);
    pDX->m_pDlgWnd->GetDlgItem(nIDC, &hwnd);

    if (pDX->m_bSaveAndValidate)    // Save
    {
        GetWindowText(hwnd, text.GetBuffer(256), 256);
        text.ReleaseBuffer();

        if (text != text.SpanIncluding(_T("-0123456789")))
        {
            AfxMessageBox(AFX_IDP_PARSE_REAL);
            pDX->Fail();
        }

        save_func(text);
    }
    else // Load
    {
        text = load_func();
        SetWindowText(hwnd, text.GetString());
    }
}

void DDX_TextEx(CDataExchange* pDX, int nIDC, int& value)
{
    auto save_func = [&value](const CString& text) -> void {
        value = _tstoi(text.GetString());
    };

    auto load_func = [value]() -> CString {
        CString text;
        text.Format(_T("%d"), value);
        return text;
    };

    DDX_TextFormatIntT(pDX, nIDC, save_func, load_func);
}

void DDX_TextEx(CDataExchange* pDX, int nIDC, unsigned int& value)
{
    auto save_func = [&value](const CString& text) -> void {
        value = _tstoi(text.GetString());
    };

    auto load_func = [value]() -> CString {
        CString text;
        text.Format(_T("%d"), value);
        return text;
    };

    DDX_TextFormatIntT(pDX, nIDC, save_func, load_func);
}

void DDX_TextEx(CDataExchange* pDX, int nIDC, short& value)
{
    auto save_func = [&value](const CString& text) -> void {
        value = _tstoi(text.GetString());
    };

    auto load_func = [value]() -> CString {
        CString text;
        text.Format(_T("%d"), value);
        return text;
    };

    DDX_TextFormatIntT(pDX, nIDC, save_func, load_func);
}

void DDX_TextEx(CDataExchange* pDX, int nIDC, unsigned short& value)
{
    auto save_func = [&value](const CString& text) -> void {
        value = _tstoi(text.GetString());
    };

    auto load_func = [value]() -> CString {
        CString text;
        text.Format(_T("%d"), value);
        return text;
    };

    DDX_TextFormatIntT(pDX, nIDC, save_func, load_func);
}

void DDX_TextEx(CDataExchange* pDX, int nIDC, float& value)
{
    auto save_func = [&value](const CString& text) -> void {
        value = (float)_tstof(text.GetString());
    };

    auto load_func = [value]() -> CString {
        CString text;
        text.Format(_T("%.*g"), FLT_DIG, value);
        return text;
    };

    DDX_TextFormatT(pDX, nIDC, save_func, load_func);
}

void DDX_TextEx(CDataExchange* pDX, int nIDC, double& value)
{
    auto save_func = [&value](const CString& text) -> void {
        value = _tstof(text.GetString());
    };

    auto load_func = [value]() -> CString {
        CString text;
        text.Format(_T("%.*g"), DBL_DIG, value);
        return text;
    };

    DDX_TextFormatT(pDX, nIDC, save_func, load_func);
}

void DDX_TextEx(CDataExchange* pDX, int IDC, LPSTR value, int nLen)
{
    CStringA str = value;
    DDX_TextEx(pDX, IDC, str);
    strncpy_s(value, nLen, str, nLen);
}

void DDX_TextEx(CDataExchange* pDX, int IDC, CStringA& value)
{
    CString str = CString(value);
    DDX_Text(pDX, IDC, str);
    value = str;
}

void DDX_TextEx(CDataExchange* pDX, int nIDC, int& value, std::function<void(int src, int dst)> save_callback)
{
    int b_value = value;
    DDX_TextEx(pDX, nIDC, value);

    if (pDX->m_bSaveAndValidate && b_value != value)
        save_callback(b_value, value);
}

void DDX_TextEx(CDataExchange* pDX, int nIDC, float& value, std::function<void(float src, float dst)> save_callback)
{
    float b_value = value;
    DDX_TextEx(pDX, nIDC, value);

    if (pDX->m_bSaveAndValidate && b_value != value)
        save_callback(b_value, value);
}

void DDX_TextEx(CDataExchange* pDX, int nIDC, short& value, std::function<void(short src, short dst)> save_callback)
{
    short b_value = value;
    DDX_TextEx(pDX, nIDC, value);

    if (pDX->m_bSaveAndValidate && b_value != value)
        save_callback(b_value, value);
}

void DDX_CBIndexEx(CDataExchange* pDX, int nIDC, byte& value)
{
    int b_value = value, n = value;
    DDX_CBIndex(pDX, nIDC, n);
    value = (byte)n;
}

void DDX_CBIndexEx(CDataExchange* pDX, int nIDC, int& value)
{
    int b_value = value, n = value;
    DDX_CBIndex(pDX, nIDC, n);
    value = (int)n;
}

void DDX_CBIndexEx(CDataExchange* pDX, int nIDC, unsigned int& value)
{
    int b_value = value, n = value;
    DDX_CBIndex(pDX, nIDC, n);
    value = (unsigned int)n;
}

void DDX_CBIndexEx(CDataExchange * pDX, int nIDC, byte& value, std::function<void(byte src, byte dst)> save_callback)
{
    int b_value = value, n = value;
    DDX_CBIndex(pDX, nIDC, n);
    value = (byte)n;

    if (pDX->m_bSaveAndValidate && b_value != value)
        save_callback((byte)b_value, value);
}

void DDX_CBIndexEx(CDataExchange * pDX, int nIDC, int& value, std::function<void(int src, int dst)> save_callback)
{
    int b_value = value;
    DDX_CBIndex(pDX, nIDC, value);

    if (pDX->m_bSaveAndValidate && b_value != value)
        save_callback(b_value, value);
}

void DDX_CBIndexEx(CDataExchange * pDX, int nIDC, short& value, std::function<void(short src, short dst)> save_callback)
{
    int b_value = value, n = value;
    DDX_CBIndex(pDX, nIDC, n);
    value = (short)n;

    if (pDX->m_bSaveAndValidate && b_value != value)
        save_callback((short)b_value, value);
}

void DDX_CheckEx(CDataExchange* pDX, int nIDC, int& value)
{
    int b_value = value, n = value;
    DDX_Check(pDX, nIDC, n);
    value = n;
}

void DDX_CheckEx(CDataExchange* pDX, int nIDC, unsigned int& value)
{
    int b_value = value, n = value;
    DDX_Check(pDX, nIDC, n);
    value = (unsigned int)n;
}

void DDX_CheckEx(CDataExchange * pDX, int nIDC, char& value)
{
    int n = value;
    DDX_Check(pDX, nIDC, n);
    value = (char)n;
}

void DDX_CheckEx(CDataExchange* pDX, int nIDC, byte& value)
{
    int n = value;
    DDX_Check(pDX, nIDC, n);
    value = (byte)n;
}

void DDX_CheckEx(CDataExchange * pDX, int nIDC, char& value, std::function<void(char src, char dst)> save_callback)
{
    int b_value = value, n = value;
    DDX_Check(pDX, nIDC, n);
    value = (char)n;

    if (pDX->m_bSaveAndValidate && b_value != n)
        save_callback((char)b_value, value);
}

void DDX_CheckEx(CDataExchange * pDX, int nIDC, byte& value, std::function<void(byte src, byte dst)> save_callback)
{
    int b_value = value, n = value;
    DDX_Check(pDX, nIDC, n);
    value = (byte)n;

    if (pDX->m_bSaveAndValidate && b_value != n)
        save_callback((byte)b_value, value);
}

void DDX_CheckEx(CDataExchange * pDX, int nIDC, bool& value, std::function<void(bool src, bool dst)> save_callback)
{
    int b_value = value == true ? BST_CHECKED : BST_UNCHECKED;
    int n = b_value;
    DDX_Check(pDX, nIDC, n);
    value = (n == BST_CHECKED);

    if (pDX->m_bSaveAndValidate && b_value != n)
        save_callback((byte)b_value, value);
}

void DDX_CheckEx(CDataExchange * pDX, int nIDC, int& value, std::function<void(int src, int dst)> save_callback)
{
    int b_value = value, n = value;
    DDX_Check(pDX, nIDC, n);
    value = n;

    if (pDX->m_bSaveAndValidate && b_value != n)
        save_callback(b_value, value);
}

void DDX_RadioEx(CDataExchange * pDX, int nIDC, int& value, std::function<void(int src, int dst)> save_callback)
{
    int b_value = value;
    DDX_Radio(pDX, nIDC, value);

    if (pDX->m_bSaveAndValidate && b_value != value)
        save_callback(b_value, value);
}


void DDX_TextFormatEx(CDataExchange* pDX, int nIDC, float& value, const CString& format)
{
    auto save_func = [&value](const CString& text) -> void {
        value = (float)_tstof(text.GetString());
    };

    auto load_func = [&value, &format]() -> CString {
        CString text;
        text.Format(format.GetString(), value);
        return text;
    };

    DDX_TextFormatT(pDX, nIDC, save_func, load_func);
}

void DDX_TextFormatEx(CDataExchange* pDX, int nIDC, double& value, const CString& format)
{
    auto save_func = [&value](const CString& text) -> void {
        value = _tstof(text.GetString());
    };

    auto load_func = [&value, &format]() -> CString {
        CString text;
        text.Format(format.GetString(), value);
        return text;
    };

    DDX_TextFormatT(pDX, nIDC, save_func, load_func);
}

template<typename T>
void DDV_MinMaxT(CDataExchange * pDX, T value, T MinVal, T MaxVal, CString & strErrorMessage)
{
    ASSERT(MinVal <= MaxVal);
    if (value < MinVal || value > MaxVal)
    {
        if (FALSE == pDX->m_bSaveAndValidate)
        {
            TRACE0("Warning: initial dialog data is out of range.\n");
            return; // don't stop now
        }

        AfxMessageBox(strErrorMessage, MB_ICONEXCLAMATION);
        pDX->Fail();
    }
}

void DDV_MinMaxFloatEx(CDataExchange * pDX, float value, float MinVal, float MaxVal, CString & strErrorMessage)
{
    DDV_MinMaxT(pDX, value, MinVal, MaxVal, strErrorMessage);
}

void DDV_MinMaxDoubleEx(CDataExchange* pDX, double value, double MinVal, double MaxVal, CString& strErrorMessage)
{
    DDV_MinMaxT(pDX, value, MinVal, MaxVal, strErrorMessage);
}

void DDV_MinMaxShortEx(CDataExchange * pDX, short value, short MinVal, short MaxVal, CString & strErrorMessage)
{
    DDV_MinMaxT(pDX, value, MinVal, MaxVal, strErrorMessage);
}

void DDV_MinMaxUShortEx(CDataExchange * pDX, unsigned short value, unsigned short MinVal, unsigned short MaxVal, CString & strErrorMessage)
{
    DDV_MinMaxT(pDX, value, MinVal, MaxVal, strErrorMessage);
}

void DDV_MinMaxIntEx(CDataExchange * pDX, int value, int MinVal, int MaxVal, CString & strErrorMessage)
{
    DDV_MinMaxT(pDX, value, MinVal, MaxVal, strErrorMessage);
}

void DDV_MinMaxUintEx(CDataExchange* pDX, unsigned int value, unsigned int MinVal, unsigned int MaxVal, CString& strErrorMessage)
{
    DDV_MinMaxT(pDX, value, MinVal, MaxVal, strErrorMessage);
}

void SetReplaceUnit(CWnd* wnd, int ctrl_id, const CString& unit)
{
    CString str;
    wnd->GetDlgItemText(ctrl_id, str);
    str.Replace(_T("%"), unit);
    wnd->SetDlgItemText(ctrl_id, str);
}

int StringSplit(const CString& str, CStringArray& arr_str, const CString& token)
{
    arr_str.RemoveAll();

    CString resToken;
    int curPos = 0;

    resToken = str.Tokenize(token, curPos);
    while (resToken != _T(""))
    {
        arr_str.Add(resToken);
        resToken = str.Tokenize(token, curPos);
    };

    return (int)arr_str.GetCount();
}

int StringSplit(int ids_id, CStringArray& arr_str, const CString& token)
{
    arr_str.RemoveAll();

    CString str;
    str.LoadString(ids_id);
    CString resToken;
    int curPos = 0;

    resToken = str.Tokenize(token, curPos);
    while (resToken != _T(""))
    {
        arr_str.Add(resToken);
        resToken = str.Tokenize(token, curPos);
    };

    return (int)arr_str.GetCount();
}

#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <WS2tcpip.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

int PingTest(const CString& ip_src)
{
    // 참고
    // https://docs.microsoft.com/ko-kr/windows/win32/api/icmpapi/nf-icmpapi-icmpsendecho?redirectedfrom=MSDN
    // 메모리 릭이 있는 샘플 코드이기 때문에 해제 코드를 추가 함.

    HANDLE hIcmpFile = NULL;
    LPVOID ReplyBuffer = NULL;
    int ret = 0;

    try
    {
        IN_ADDR ipaddr;
        //unsigned long ipaddr = INADDR_NONE;
        //ipaddr = inet_addr(ip_addr);
        int ret_ip = InetPton(AF_INET, ip_src.GetString(), &ipaddr);
        if (ret_ip != 1)
            throw 1;

        hIcmpFile = IcmpCreateFile();
        if (hIcmpFile == INVALID_HANDLE_VALUE)
            throw 2;

        char SendData[32] = "Data Buffer";
        DWORD ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
        ReplyBuffer = (VOID*)malloc(ReplySize);
        if (ReplyBuffer == NULL)
            throw 3;

        DWORD dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr.S_un.S_addr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
        if (dwRetVal != 0)
        {
            PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
            struct in_addr ReplyAddr;
            ReplyAddr.S_un.S_addr = pEchoReply->Address;

            //LOGI << fmt::format("Received {} icmp message responses", dwRetVal);
            //LOGI << fmt::format("Received from {}", inet_ntoa(ReplyAddr));
            //LOGI << fmt::format("Status = {}", pEchoReply->Status);
            //LOGI << fmt::format("Roundtrip time = {} milliseconds", pEchoReply->RoundTripTime);
        }
        else
        {
            //LOGI << fmt::format("Call to IcmpSendEcho failed.");
            //LOGI << fmt::format("IcmpSendEcho returned error: {}", GetLastError());
            throw 4;
        }
    }
    catch (int error)
    {
        ret = error;
    }

    if (ReplyBuffer)
        free(ReplyBuffer);

    if (hIcmpFile)
        IcmpCloseHandle(hIcmpFile);

    return ret;
}

bool GetLocalIPAdressList(std::vector<std::string>& strIPArray)
{
    std::string strIPAddress;

    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;

    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = new IP_ADAPTER_INFO[ulOutBufLen];

    if (pAdapterInfo == NULL)
        return FALSE;

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        delete pAdapterInfo;
        pAdapterInfo = new IP_ADAPTER_INFO[ulOutBufLen];
        if (pAdapterInfo == NULL)
            return FALSE;
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;

        while (pAdapter)
        {
            IP_ADDR_STRING* pAddr = &pAdapter->IpAddressList;
            while (pAddr)
            {
                strIPAddress = pAddr->IpAddress.String;

                if ("0.0.0.0" != strIPAddress)
                    strIPArray.push_back(strIPAddress);

                pAddr = pAddr->Next;
            }

            pAdapter = pAdapter->Next;
        }
    }

    delete pAdapterInfo;
    pAdapterInfo = NULL;
    return TRUE;
}

std::string GetMainLocalIpAddress()
{
    SOCKET ipSocket;

    struct sockaddr_in sockaddr_info1, sockaddr_info2;
    int sockaddr_info_size = sizeof(struct sockaddr_in);

    sockaddr_info1.sin_family = AF_INET;
    sockaddr_info1.sin_port = htons(4567);
    inet_pton(AF_INET, "8.8.8.8", &sockaddr_info1.sin_addr.s_addr);

    ipSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    connect(ipSocket, (struct sockaddr*)&sockaddr_info1, sizeof sockaddr_info1);
    getsockname(ipSocket, (struct sockaddr*)&sockaddr_info2, &sockaddr_info_size);
    closesocket(ipSocket);

    //std::string ret = inet_ntoa(sockaddr_info2.sin_addr);
    char str[64] = { 0, };
    inet_ntop(sockaddr_info2.sin_family, &sockaddr_info2.sin_addr, str, 64);

    return std::string(str);
}

CString CFormat(UINT nFormatID, ...)
{
    CString str;
    CString strFormat(str.GetManager());
    ATLENSURE(strFormat.LoadString(nFormatID));

    va_list argList;
    va_start(argList, nFormatID);
    str.FormatV(strFormat, argList);
    va_end(argList);

    return str;
}

CString CFormat(const TCHAR* format, ...)
{
    CString str;

    va_list argList;
    va_start(argList, format);
    str.FormatV(format, argList);
    va_end(argList);

    return str;
}