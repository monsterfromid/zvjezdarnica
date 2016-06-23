#if !defined(AFX_NUMDLG_H__035C8609_E479_4780_8E2B_D4E07E41891E__INCLUDED_)
#define AFX_NUMDLG_H__035C8609_E479_4780_8E2B_D4E07E41891E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NumDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNumDlg dialog

class CNumDlg : public CDialog
{
// Construction
public:
	CNumDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNumDlg)
	enum { IDD = IDD_NUM };
	CStatic	m_cNumTab;
	CStatic	m_cHFrame2;
	CStatic	m_cHFrame;
	CStatic	m_cHeader;
	CString	m_sNumReza;
	CString	m_sNumTxt;
	int		m_iNumSustav;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNumDlg)
	afx_msg void OnNumRacunaj();
	afx_msg void OnChangeNumTekst();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnRadio123();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBitmap m_bmHeader;
	CBitmap m_bmTab1, m_bmTab2, m_bmTab3;
	void Ucitaj_sliku();
	CWnd* m_pParent;
	CString zamjeni_sumnike (CString sStr);
	char broj[53][140];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMDLG_H__035C8609_E479_4780_8E2B_D4E07E41891E__INCLUDED_)
