#include "pch.h"
#include "WinIniParser.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


WinIniParser::WinIniParser()
{

}

WinIniParser::WinIniParser(const CString& file_path)
    : m_file_path(file_path)
{
}

WinIniParser::~WinIniParser()
{

}

bool WinIniParser::SetFilePath(const CString& file_path)
{
    if (false == IsValid(file_path))
        return false;

    m_file_path = file_path;

    return true;
}

bool WinIniParser::IsValid(const CString& file_path)
{
    if (file_path.IsEmpty())
        return false;

    return IsExistFile(file_path.GetString());
}

bool WinIniParser::IsValid()
{
    return IsValid(m_file_path);
}

bool WinIniParser::GetValue(const CString& section, const CString& key, CString& value)
{
    CString ret;
    DWORD read_size = GetPrivateProfileString(section.GetString(), key.GetString(), _T(""), ret.GetBuffer(256), 256, m_file_path.GetString());
    ret.ReleaseBuffer();

    value = ret;

    if (0 == read_size)
        return false;

    return true;
}

bool WinIniParser::GetValue(const CString& section, const CString& key, int& value)
{
    CString str_buffer;
    if (false == GetValue(section, key, str_buffer))
        return false;

    value = _tstoi(str_buffer.GetString());

    return true;
}

bool WinIniParser::GetValue(const CString& section, const CString& key, float& value)
{
    CString str_buffer;
    if (false == GetValue(section, key, str_buffer))
        return false;

    value = (float)_tstof(str_buffer.GetString());

    return true;
}

bool WinIniParser::GetValue(const CString& section, const CString& key, double& value)
{
    CString str_buffer;
    if (false == GetValue(section, key, str_buffer))
        return false;

    value = _ttof(str_buffer.GetString());

    return true;
}

bool WinIniParser::GetValue(const CString& section, const CString& key, bool& value)
{
    CString str_buffer;
    if (false == GetValue(section, key, str_buffer))
        return false;

    str_buffer.MakeUpper();
    if (_T("TRUE") == str_buffer)
        value = true;
    else if (_T("FALSE") == str_buffer)
        value = false;
    else
        return false;

    return true;
}