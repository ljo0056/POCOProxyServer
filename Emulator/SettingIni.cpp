#include "pch.h"
#include "SettingIni.h"

#include "common/WinIniParser.h"
#include "common/Util.h"

#include <plog/Init.h>
#include <plog/Appenders/DynamicAppender.h>
#include <plog/Appenders/DebugOutputAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Custom/OneDayFileAppender.h>
#include <plog/Custom/WinConsoleAppender.h>


SettingIni::SettingIni(const plog::util::nstring& name, const plog::util::nstring& root_path)
{
    m_time_start_app  = CTime::GetCurrentTime();
    
    plog::util::nstring file_name = root_path + _T("SETTING.txt");

    WinIniParser ini_parser(file_name.c_str());

    if (ini_parser.IsValid())
    {
        static plog::DynamicAppender dynamicAppender;
        plog::init(plog::verbose, &dynamicAppender);
        {
            static plog::DebugOutputAppender<plog::TxtFormatter> debugoutput_appender;
            dynamicAppender.addAppender(&debugoutput_appender);

            bool console = false;
            ini_parser.GetValue(_T("DEBUG"), _T("CONSOLE"), console);
            if (console)
            { 
                static plog::WinConsoleAppender<plog::TxtFormatter>  console_appender;
                dynamicAppender.addAppender(&console_appender);
            }

            bool log = false;
            ini_parser.GetValue(_T("DEBUG"), _T("LOG"), log);
            if (log)
            {                
                plog::util::nstring log_dir = root_path + PLOG_NSTR("LOG");
                static plog::OneDayFileAppender<plog::TxtFormatter>  onedayfile_appender(log_dir.c_str());
                dynamicAppender.addAppender(&onedayfile_appender);
            }
            
            PLOGI << "<<====================     START     ====================>>";            
            PLOGI << fmt::format(_T("Name[{}] Build Date[{} {}]"), name, _T(__DATE__), _T(__TIME__));
            PLOGI << fmt::format("DEBUG -- CONSOLE[{}] LOG[{}]", console, log);
        }        
    }
}

SettingIni::~SettingIni()
{
    CTime time_cur = CTime::GetCurrentTime();
    CTimeSpan time_running = time_cur - m_time_start_app;
    int days  = (int)time_running.GetDays();
    int hours = time_running.GetHours();
    int mins  = time_running.GetMinutes();
    int secs  = time_running.GetSeconds();    

    PLOGI << fmt::format("Program Running Time: {}:days {}:hours {}:mins {}:secs", days, hours, mins, secs);
    PLOGI << "<<====================     END     ====================>>";
}

SettingIni& SettingIni::GetInstance()
{
    plog::util::nstring log_dir;
    GetProcessPath(log_dir);
    log_dir += _T("\\");
    static SettingIni setting_ini(_T("HT3X00AS"), log_dir);

    return setting_ini;
}