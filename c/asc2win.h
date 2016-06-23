// asc2win.h : main header file for the ASC2WIN application
//


#if !defined(AFX_ASC2WIN_H__91508975_E4CA_4898_BBC2_0C88451DB87E__INCLUDED_)
#define AFX_ASC2WIN_H__91508975_E4CA_4898_BBC2_0C88451DB87E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAsc2winApp:
// See asc2win.cpp for the implementation of this class
//

const COLORREF Colors[20] = {                     //ovo ja dodao, globalna var
		RGB( 0, 0, 0),      // Black,0
		RGB( 0, 0, 255),    // blue,1
		RGB( 0, 255,0),    // green,2
		RGB( 0, 255, 255),  // cyan,3
		RGB( 255, 0, 0),    // red,4
		RGB( 255, 0,255),   // magenta,5
		RGB( 255, 255, 0),  // yellow,6
		RGB( 255, 255, 255),// white,7

		RGB( 0, 0, 128),    // dark blue,8
		RGB( 0, 128, 0),    // dark green,9
		RGB( 0, 128,128),   // dark cyan,10
		RGB( 128, 0, 0),    // dark red, 11
		RGB( 128, 0, 128),  // dark magenta, 12
		RGB( 128, 128,0),   // dark yellow, 13
		RGB( 128, 128, 128),// dark gray,14
		RGB( 192, 192, 192), // bright gray,15

		RGB(  255,165,0 ), // orange,16
		RGB( 238,130,238 ) // violet,17
	};

class CAsc2winApp : public CWinApp
{
public:
	CAsc2winApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAsc2winApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAsc2winApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASC2WIN_H__91508975_E4CA_4898_BBC2_0C88451DB87E__INCLUDED_)
