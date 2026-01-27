#pragma once
#include <plog/Appenders/IAppender.h>
#include <plog/Converters/UTF8Converter.h>
#include <plog/Converters/NativeEOLConverter.h>
#include <plog/Util.h>
#include <algorithm>

#ifndef _WIN32
    #error OneDayFileAppender only support WIN32
#endif

namespace plog
{
    template<class Formatter, class Converter = NativeEOLConverter<UTF8Converter>>
    class OneDayFileAppender : public IAppender
    {
    public:
        OneDayFileAppender(const util::nchar* rootPath)
            : m_rootPath(rootPath)
            , m_fileExt(PLOG_NSTR("log"))
        {
            checkRootPath();
        }

        void SetPrefixName(const util::nstring& prefix)
        {
            m_prefix = prefix;
        }

        void SetSuffixName(const util::nstring& prefix)
        {
            m_suffix = prefix;
        }

        virtual void write(const Record& record) PLOG_OVERRIDE
        {
            util::MutexLock lock(m_mutex);

            openLogFile();

            m_file.write(Converter::convert(Formatter::format(record)));
        }

    private:
        void checkRootPath()
        {
            if (m_rootPath.empty())
            {                
                util::nchar szPath[256] = { 0, };
                GetModuleFileName(NULL, szPath, _countof(szPath));

                util::nchar* pPos = _tcsrchr(szPath, PLOG_NSTR('\\'));
                *pPos = NULL;

                m_rootPath = szPath;
                m_rootPath += PLOG_NSTR("\\LOG");
            }

            CreateDirectory(m_rootPath.c_str(), NULL);
        }

        void openLogFile()
        {
            util::nstring fileName = buildFileName();

            if (m_fileName != fileName)
            {
                m_file.close();

                size_t fileSize = m_file.open(fileName.c_str());
                m_fileName = fileName;

                if (0 == fileSize)
                {
                    m_file.write(Converter::header(Formatter::header()));
                }
            }
        }

        util::nstring buildFileName()
        {
            util::nostringstream ss;
            ss << m_rootPath;

            if (m_rootPath.back() != PLOG_NSTR('\\'))
            {
                ss << PLOG_NSTR('\\');
            }

            tm t;
            util::Time utime;
            util::ftime(&utime);

            util::localtime_s(&t, &utime.time);
            if (!m_prefix.empty())
            {
                ss << m_prefix;
            }

            // yyyy-mm-dd
            ss << t.tm_year + 1900 << "-"
                << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon + 1 << PLOG_NSTR("-")
                << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << PLOG_NSTR(" ");

            if (!m_suffix.empty())
            {
                ss << m_suffix;
            }

            if (!m_fileExt.empty())
            {
                ss << '.' << m_fileExt;
            }

            return ss.str();
        }

    private:
        util::Mutex     m_mutex;
        util::File      m_file;
        util::nstring   m_fileExt;
        util::nstring   m_fileName;
        util::nstring   m_rootPath;
        util::nstring   m_prefix;
        util::nstring   m_suffix;
    };
}

