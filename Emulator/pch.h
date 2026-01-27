// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

// plog
#include <plog/Log.h>
#include <plog/Custom/LogAFL.h>

// fmt
// fmt/color.h(1,1) and fmt/chrono.h : warning C4819: The file contains a character that cannot be represented in the current code page (949)
#pragma warning (disable:4819) 
#define FMT_UNICODE 0
#define FMT_HEADER_ONLY
#include <fmt/base.h>
#include <fmt/xchar.h>
#pragma warning (default:4819)

#endif //PCH_H
