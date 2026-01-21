#pragma once

#include <plog/Log.h>

namespace plog
{
    class AutoFuncLog
    {
    private:
        Record                      m_record;
        Severity                    m_severity;
        std::string                 m_func;
        size_t                      m_line;
        std::string                 m_file;
        const void*                 m_object;

    public:
        AutoFuncLog(Severity severity, const char* func, size_t line, const char* file, const void* object)
            : m_record(severity, func, line, file, PLOG_GET_THIS())
            , m_severity(severity)
            , m_func(func)
            , m_line(line)
            , m_file(file)
            , m_object(object)
        {
            m_record << "Start ";
        }
        ~AutoFuncLog()
        {
            IF_LOG_(PLOG_DEFAULT_INSTANCE, m_severity) (*plog::get<PLOG_DEFAULT_INSTANCE>()) +=
                Record(m_severity, m_func.c_str(), m_line, m_file.c_str(), PLOG_GET_THIS()).ref() <<
                "End";
        }

        Record& ref()
        {
            return m_record;
        }
    };
}



#define LOGF_AFL \
    plog::AutoFuncLog log_afl(plog::fatal, __FUNCTION__, __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()); \
    IF_LOG_(PLOG_DEFAULT_INSTANCE, plog::fatal) (*plog::get<PLOG_DEFAULT_INSTANCE>()) += log_afl.ref() 

#define LOGE_AFL \
    plog::AutoFuncLog log_afl(plog::error, __FUNCTION__, __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()); \
    IF_LOG_(PLOG_DEFAULT_INSTANCE, plog::error) (*plog::get<PLOG_DEFAULT_INSTANCE>()) += log_afl.ref() 

#define LOGW_AFL \
    plog::AutoFuncLog log_afl(plog::warning, __FUNCTION__, __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()); \
    IF_LOG_(PLOG_DEFAULT_INSTANCE, plog::warning) (*plog::get<PLOG_DEFAULT_INSTANCE>()) += log_afl.ref() 

#define LOGI_AFL \
    plog::AutoFuncLog log_afl(plog::info, __FUNCTION__, __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()); \
    IF_LOG_(PLOG_DEFAULT_INSTANCE, plog::info) (*plog::get<PLOG_DEFAULT_INSTANCE>()) += log_afl.ref() 

#define LOGD_AFL \
    plog::AutoFuncLog log_afl(plog::debug, __FUNCTION__, __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()); \
    IF_LOG_(PLOG_DEFAULT_INSTANCE, plog::debug) (*plog::get<PLOG_DEFAULT_INSTANCE>()) += log_afl.ref() 

#define LOGV_AFL \
    plog::AutoFuncLog log_afl(plog::verbose, __FUNCTION__, __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()); \
    IF_LOG_(PLOG_DEFAULT_INSTANCE, plog::verbose) (*plog::get<PLOG_DEFAULT_INSTANCE>()) += log_afl.ref() 


