// stbapi.cpp : Defines the entry point for the DLL application.//

#include "stdafx.h"#include "stbapi.h"#include <ace/ACE.h>

BOOL APIENTRY DllMain( HANDLE hModule,                        DWORD  ul_reason_for_call,                        LPVOID lpReserved					 ){    switch (ul_reason_for_call)	{		case DLL_PROCESS_ATTACH:		ACE::init();
		break;
		case DLL_THREAD_ATTACH:		break;
		case DLL_THREAD_DETACH:		break;
		case DLL_PROCESS_DETACH:		ACE::fini();
			break;    }    return TRUE;}
