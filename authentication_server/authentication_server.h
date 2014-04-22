
// authentication_server.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cauthentication_serverApp:
// See authentication_server.cpp for the implementation of this class
//

class Cauthentication_serverApp : public CWinApp
{
public:
	Cauthentication_serverApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cauthentication_serverApp theApp;