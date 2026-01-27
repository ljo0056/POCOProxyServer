#pragma once

#include <plog/Log.h>

namespace plog
{
    class AutoFuncLog
    {
    private:
        Record m_record;        

    public:
        AutoFuncLog(Severity severity, const char* func, size_t line, const char* file, const void* object)
            : m_record(severity, func, line, file, PLOG_GET_THIS(), PLOG_DEFAULT_INSTANCE_ID)                    
        {
            m_record << "Start ";
        }
        ~AutoFuncLog()
        {
            IF_PLOG_(PLOG_DEFAULT_INSTANCE_ID, m_record.getSeverity()) (*plog::get<PLOG_DEFAULT_INSTANCE_ID>()) +=
                Record(
                    m_record.getSeverity(),
                    m_record.getFunc(),
                    m_record.getLine(),
                    m_record.getFile(),
                    m_record.getObject(),
                    m_record.getInstanceId()).ref() PLOG_PRINT_MESSAGE_PREFIX()
                << "End";
        }

        Record& ref()
        {
            return m_record;
        }
    };
}

#define LOGF_AFL \
    plog::AutoFuncLog log_afl(plog::fatal, __FUNCTION__, __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()); \
    IF_PLOG_(PLOG_DEFAULT_INSTANCE_ID, plog::fatal) (*plog::get<PLOG_DEFAULT_INSTANCE_ID>()) += log_afl.ref() 

#define LOGE_AFL \
    plog::AutoFuncLog log_afl(plog::error, __FUNCTION__, __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()); \
    IF_PLOG_(PLOG_DEFAULT_INSTANCE_ID, plog::error) (*plog::get<PLOG_DEFAULT_INSTANCE_ID>()) += log_afl.ref() 

#define LOGW_AFL \
    plog::AutoFuncLog log_afl(plog::warning, __FUNCTION__, __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()); \
    IF_PLOG_(PLOG_DEFAULT_INSTANCE_ID, plog::warning) (*plog::get<PLOG_DEFAULT_INSTANCE_ID>()) += log_afl.ref() 

#define LOGI_AFL \
    plog::AutoFuncLog log_afl(plog::info, __FUNCTION__, __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()); \
    IF_PLOG_(PLOG_DEFAULT_INSTANCE_ID, plog::info) (*plog::get<PLOG_DEFAULT_INSTANCE_ID>()) += log_afl.ref() 

#define LOGD_AFL \
    plog::AutoFuncLog log_afl(plog::debug, __FUNCTION__, __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()); \
    IF_PLOG_(PLOG_DEFAULT_INSTANCE_ID, plog::debug) (*plog::get<PLOG_DEFAULT_INSTANCE_ID>()) += log_afl.ref() 

#define LOGV_AFL \
    plog::AutoFuncLog log_afl(plog::verbose, __FUNCTION__, __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()); \
    IF_PLOG_(PLOG_DEFAULT_INSTANCE_ID, plog::verbose) (*plog::get<PLOG_DEFAULT_INSTANCE_ID>()) += log_afl.ref() 


