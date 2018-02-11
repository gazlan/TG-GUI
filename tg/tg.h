// TG.h : main header file for the TG application
//

#if !defined(AFX_TG_H__057B6F94_F0D2_4B97_81EF_7792F4E70CCA__INCLUDED_)
#define AFX_TG_H__057B6F94_F0D2_4B97_81EF_7792F4E70CCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTGApp:
// See TG.cpp for the implementation of this class
//

class CTGApp : public CWinApp
{
public:
	CTGApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTGApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTGApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TG_H__057B6F94_F0D2_4B97_81EF_7792F4E70CCA__INCLUDED_)
