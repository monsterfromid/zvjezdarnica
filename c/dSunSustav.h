#if !defined(AFX_DSUNSUSTAV_H__37F6751E_9F9A_4118_B3D3_C79B99936DF5__INCLUDED_)
#define AFX_DSUNSUSTAV_H__37F6751E_9F9A_4118_B3D3_C79B99936DF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dSunSustav.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dSunSustav dialog

class dSunSustav : public CDialog
{
// Construction
public:
	boolean m_bIsActive;
	dSunSustav(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dSunSustav)
	enum { IDD = IDD_SUNSUSTAV };
	int		m_iSvePlanete;
	int		m_iMj;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dSunSustav)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dSunSustav)
	afx_msg void OnSus1();
	afx_msg void OnSus2();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSUNSUSTAV_H__37F6751E_9F9A_4118_B3D3_C79B99936DF5__INCLUDED_)
