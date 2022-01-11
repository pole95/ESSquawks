// NonDiscreteSquawk.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CNonDiscreteSquawk2.h"

CNonDiscreteSquawk *pMyPlugIn = NULL;

void __declspec (dllexport)
EuroScopePlugInInit(EuroScopePlugIn::CPlugIn ** ppPlugInInstance)
{
	// allocate
	*ppPlugInInstance = pMyPlugIn =
		new CNonDiscreteSquawk;
}

void __declspec (dllexport)
EuroScopePlugInExit(void)
{
	delete pMyPlugIn;
}



