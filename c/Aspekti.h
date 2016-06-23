#if !defined(AFX_ASPEKTI_H__D76079D7_9444_44F8_86F9_919CE6D654A6__INCLUDED_)
#define AFX_ASPEKTI_H__D76079D7_9444_44F8_86F9_919CE6D654A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Aspekti.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAspekti dialog

class CAspekti : public CDialog
{
// Construction
public:
	boolean m_bIsActive;
	boolean m_bPaste;
	CRect m_rSlika;
	CAspekti(CWnd* pParent = NULL, CDC * pDCAsp = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAspekti)
	enum { IDD = IDD_ASPEKTI };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAspekti)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAspekti)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDC * pdAsp;
	CBitmap bmSlika;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASPEKTI_H__D76079D7_9444_44F8_86F9_919CE6D654A6__INCLUDED_)
