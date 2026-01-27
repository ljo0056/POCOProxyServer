#pragma once

class WinIniParser
{
private:
    CString        m_file_path;

public:
    WinIniParser();
    WinIniParser(const CString& file_path);
    ~WinIniParser();

    bool SetFilePath(const CString& file_path);
    bool IsValid(const CString& file_path);
    bool IsValid();

    bool GetValue(const CString& section, const CString& key, CString& value);
    bool GetValue(const CString& section, const CString& key, int& value);
    bool GetValue(const CString& section, const CString& key, float& value);
    bool GetValue(const CString& section, const CString& key, double& value);
    bool GetValue(const CString& section, const CString& key, bool& value);
};
