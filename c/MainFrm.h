// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__EF519C5C_96CB_4EB8_BA09_9483FF523DBF__INCLUDED_)
#define AFX_MAINFRM_H__EF519C5C_96CB_4EB8_BA09_9483FF523DBF__INCLUDED_

#include "UlazDlg1.h"	// Added by ClassView
#include "NumDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
//	CUlazDlg m_dUlazDlg;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	CToolBar    m_wndToolBar;
	BOOL CreateIzbornikOpcija();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

// Generated message map functions
public:
	CComboBox m_ctlOpcija;
	CNumDlg * pNumDlg;
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNaredbaNumerologija();
	afx_msg void OnPomocPdf1();
	afx_msg void OnPomocPdf2();
	afx_msg void OnPomocPdf3();
	afx_msg void OnPomocPdf4();
	afx_msg void OnPomocPdf5();
	afx_msg void OnPomocPdf6();
	afx_msg void OnPomocOastrologiji();
	afx_msg void OnPomocOastrologijioffline();
	afx_msg void OnPomocPdf7();
	afx_msg void OnPomocPdf8();
	afx_msg void OnPomocPdf9();
	afx_msg void OnPomocPdf10();
	afx_msg void OnPomocPdf11();
	afx_msg void OnPomocPdf12();
	afx_msg void OnPomocPdf13();
	afx_msg void OnPomocPdf14();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnSelChangeOpcija();

private:


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__EF519C5C_96CB_4EB8_BA09_9483FF523DBF__INCLUDED_)
