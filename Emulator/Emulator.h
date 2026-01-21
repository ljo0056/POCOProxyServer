
// ChroZenEmulator.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// EmulatorApp:
// See ChroZenEmulator.cpp for the implementation of this class
//

class EmulatorApp : public CWinApp
{
public:
	EmulatorApp();

	int GetProcessPath(std::string& dest);
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern EmulatorApp theApp;
