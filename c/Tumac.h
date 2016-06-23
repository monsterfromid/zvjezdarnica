#if !defined(AFX_TUMAC_H__2A6FC522_853E_44F1_A60F_E8699D50D1D6__INCLUDED_)
#define AFX_TUMAC_H__2A6FC522_853E_44F1_A60F_E8699D50D1D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tumac.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTumac dialog

class CTumac : public CDialog
{
// Construction
public:
	int m_iOpcijaSpec;
	void UpdatePageDisplay();
	int m_iMax;
	int m_iPage;
	CTumac(CWnd* pParent = NULL);   // standard constructor
	boolean m_bIsActive;

// Dialog Data
	//{{AFX_DATA(CTumac)
	enum { IDD = IDD_TUMAC };
	CSpinButtonCtrl	m_spinPage;
	int		m_iOpcija;
	int		m_iPage2;
	CString	m_sPage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTumac)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTumac)
	afx_msg void OnCheck();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangePage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TUMAC_H__2A6FC522_853E_44F1_A60F_E8699D50D1D6__INCLUDED_)
