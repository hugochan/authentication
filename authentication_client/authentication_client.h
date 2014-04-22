
// authentication_client.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cauthentication_clientApp:
// See authentication_client.cpp for the implementation of this class
//

class Cauthentication_clientApp : public CWinApp
{
public:
	Cauthentication_clientApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cauthentication_clientApp theApp;