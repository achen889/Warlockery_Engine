//==============================================================================================================
//Main_Win32.cpp
//by Albert Chen Jan-13-2015.
//==============================================================================================================

#pragma once//VS2010 bug that it doesn't allow it's code checking to stop checking until it sees some symbol that says it's seen this before

#define WIN32_LEAN_AND_MEAN
#include <windows.h>//search for file in default path

#include "TheApp.hpp"//search in current directory
#include <string>

LPCWSTR gameName = TEXT("A3_UDP_Server");
///----------------------------------------------------------------------------------------------------------
///processes game message
///----------------------------------------------------------------------------------------------------------
TheApp* s_theApp = NULL;
LRESULT _stdcall GameMessageProcessingFunction( HWND windowHandle, UINT MessageId, WPARAM WParam, LPARAM LParam ){
	bool wasHandled = false;
	switch( MessageId ){
	case WM_KEYDOWN:
		if(WParam == VK_ESCAPE)
		  PostQuitMessage( 0 );
		else
		wasHandled = s_theApp->ProcessKeyDown((int)WParam);
		if(wasHandled)
			return 0;
		break;
	case WM_KEYUP:
		wasHandled = s_theApp->ProcessKeyUp((int)WParam);
		if(wasHandled)
			return 0;
		break;
	default: 
		return DefWindowProc(windowHandle, MessageId, WParam, LParam );
	}//end of switch
	return 0;
}//end of GMPF

///----------------------------------------------------------------------------------------------------------
///creates app window
///----------------------------------------------------------------------------------------------------------
HWND CreateAppWindow( HINSTANCE thisAppInstance , int ShowCmd ){
	
	SetProcessDPIAware(); //tells windows to stop lying about it's resolution
	WNDCLASSEX windowDescription;
	//memset on every OS since 1975, sets memory to some value
	memset( &windowDescription, 0, sizeof( WNDCLASSEX ) );//sets all vars to 0
	//set window parameters
	windowDescription.cbSize = sizeof ( WNDCLASSEX );
	windowDescription.hInstance = thisAppInstance;
	windowDescription.lpszClassName = gameName;
	windowDescription.lpfnWndProc = GameMessageProcessingFunction; //GameMessageProcessingFunction
	windowDescription.style = CS_HREDRAW | CS_OWNDC | CS_VREDRAW;

	ATOM returnValue = RegisterClassEx( &windowDescription );
	UNUSED(returnValue);
	//variables for create window
	DWORD style = WS_OVERLAPPEDWINDOW;
	DWORD windowStyleFlags = WS_OVERLAPPEDWINDOW;
	DWORD windowStyleFlagsEx = WS_EX_APPWINDOW;
	
	RECT desktopWindowRect;
	HWND desktopWindow = GetDesktopWindow();
	GetClientRect(desktopWindow, &desktopWindowRect );
	RECT windowRect;

	windowRect.left = 100;
	windowRect.right = windowRect.left + 1500;
	windowRect.top = 100;
	windowRect.bottom = windowRect.top + 837;
	AdjustWindowRectEx(&windowRect, windowStyleFlags, false, windowStyleFlagsEx );

	int x = windowRect.left;//CW_USEDEFAULT;
	int y = windowRect.top;//0;
	int width = windowRect.right;//CW_USEDEFAULT;
	int height = windowRect.bottom;//0;

	HWND windowHandle = CreateWindow(gameName, gameName , style, x, y, width, height, NULL, NULL, thisAppInstance, NULL);
	
	if ( windowHandle ){
		ShowWindow( windowHandle, ShowCmd );
		UpdateWindow( windowHandle );
	}
	return windowHandle;
}//end of method

//-------------------------------------------------------------------------------------------------------
//_stdcall means using standard C call from 1969
int _stdcall WinMain( HINSTANCE thisAppInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd ){
	UNUSED(hPrevInstance);
	UNUSED(lpCmdLine);

	HWND myWindowHandle = CreateAppWindow( thisAppInstance , nShowCmd );

	s_theApp = new TheApp();
	
	s_theApp->StartUp( (void*)myWindowHandle );
	s_theApp->Run();
	s_theApp->ShutDown();

	delete s_theApp;
	s_theApp = NULL;

	ConsolePrintMemUseInformation();

}//end of WinMain