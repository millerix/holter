
// Holter_vs2015.h : main header file for the PROJECT_NAME application
//

#pragma once
#include "resource.h"		// main symbols
#include "stdafx.h"

// CHolter_vs2015App:
// See Holter_vs2015.cpp for the implementation of this class
//

class CHolter_vs2015App : public CWinApp
{
public:
	CHolter_vs2015App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CHolter_vs2015App theApp;