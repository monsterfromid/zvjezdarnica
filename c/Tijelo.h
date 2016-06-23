#if !defined(AFX_TIJELO_H__7F199BE9_B7F3_4024_83F8_CD6E486641A8__INCLUDED_)
#define AFX_TIJELO_H__7F199BE9_B7F3_4024_83F8_CD6E486641A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tijelo.h : header file
//
#include "UlazDlg1.h"

/////////////////////////////////////////////////////////////////////////////
// CTijelo dialog

class CTijelo : public CDialog
{
// Construction
public:
	boolean m_bIsActive;
	CTijelo(CWnd* pParent = NULL, CUlazDlg * pUlazi = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTijelo)
	enum { IDD = IDD_TIJELO };
	BOOL	m_bAspDobri;
	BOOL	m_AspLosi;
	BOOL	m_bObj1;
	BOOL	m_bObj2;
	BOOL	m_bObj3;
	BOOL	m_bObj4;
	BOOL	m_bObj5;
	BOOL	m_bObj6;
	BOOL	m_bObj7;
	BOOL	m_bObj8;
	BOOL	m_bObj9;
	BOOL	m_bObj10;
	BOOL	m_bObj11;
	BOOL	m_bSvi;
	BOOL	m_bSviAsp;
	int		m_iAspObj;
	BOOL	m_bDST;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTijelo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTijelo)
	afx_msg void OnCheck();
	afx_msg void OnChecksvi();
	afx_msg void OnChecksvia();
	afx_msg void OnChkDst();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CUlazDlg * m_pUlazi;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIJELO_H__7F199BE9_B7F3_4024_83F8_CD6E486641A8__INCLUDED_)
