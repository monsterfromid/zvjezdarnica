#if !defined(AFX_POSTAVKE_H__2175928D_D52A_4178_A0EE_1A88E9CD7D28__INCLUDED_)
#define AFX_POSTAVKE_H__2175928D_D52A_4178_A0EE_1A88E9CD7D28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Postavke.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPostavke dialog

class CPostavke : public CDialog
{
// Construction
public:
	CPostavke(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPostavke)
	enum { IDD = IDD_HOR_POSTAVKE };
	CComboBox	m_cbPoc;
	CComboBox	m_cbKorSat;
	CComboBox	m_cbKorMin;
	int		m_iAspTip;
	BOOL	m_bAspTab;
	int		m_iKuceIzr;
	BOOL	m_bDST;
	double	m_iOrbAsp1;
	double	m_iOrbAsp2;
	double	m_iOrbAsp3;
	double	m_iOrbAsp4;
	double	m_iOrbAsp5;
	double	m_iOrbAsp6;
	double	m_iOrbPla1;
	double	m_iOrbPla2;
	double	m_iOrbPla3;
	double	m_iOrbPla4;
	double	m_iOrbPla5;
	double	m_iOrbPla6;
	double	m_iOrbPla7;
	double	m_iOrbPla8;
	double	m_iOrbPla9;
	double	m_iOrbPla10;
	double	m_iOrbPla11;
	double	m_iOrbPla12;
	int		m_iTipIspisa;
	CString	m_sImeBaze;
	BOOL	m_bNebo;
	BOOL	m_bSotGodPrecesija;
	BOOL	m_bGregKal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPostavke)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPostavke)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeImeBaze();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void snimi_postavke();
	void ucitaj_postavke();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POSTAVKE_H__2175928D_D52A_4178_A0EE_1A88E9CD7D28__INCLUDED_)
