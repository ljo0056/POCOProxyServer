#pragma once

#include <plog/Util.h>

class SettingIni
{
private:
    SettingIni(const plog::util::nstring& name, const plog::util::nstring& root_path);
    ~SettingIni();
    
    CTime m_time_start_app;

public:
    static SettingIni& GetInstance();    
};