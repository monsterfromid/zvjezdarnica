#if !defined(AFX_GODISNJI_H__4E8A5881_3E5D_433E_8295_E58DD5E99840__INCLUDED_)
#define AFX_GODISNJI_H__4E8A5881_3E5D_433E_8295_E58DD5E99840__INCLUDED_

#include "UlazDlg1.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Godisnji.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGodisnji dialog

class CGodisnji : public CDialog
{
// Construction
public:
	CString m_sIme;
	void Postavi_Bazni_Datum(int d, int m, int g, int s, int mn, CString i, float du, float si, int v, int gm);
	int m_iMinBazni;
	int m_iSatBazni;
	int m_iGodBazni;
	int m_iMjeBazni;
	int m_iDanBazni;
	CGodisnji(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGodisnji)
	enum { IDD = IDD_GODISNJI };
	CSpinButtonCtrl	m_spinGod;
	CString	m_sPocVriijeme;
	CString	m_sIzracunatoVrijeme;
	int		m_iBrGod;
	int		m_iGod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGodisnji)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGodisnji)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeRbrgodine();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_iGmtBazni;
	int m_iVriBazni;
	float m_fSirBazni;
	float m_fDuzBazni;
	void Pronadji_Vrijeme();
	CString deg2dms(float deg, int mod);
	double frac(double x);
	double fnmo(double x);
	void ln1060(double xx, double *yy, double *rr, double *aa);
	double fns(double x);
	double fnp(double x, double mm);
	double sgn(double x);
	void readdata(int *i1, double *f1);
	void Planete(int dan, int mje, int god, int sat, int min);
	double objx[20];

	int fmod2 (double a, double b);
	double fabs2 (double a);
	double floor2 (double a);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GODISNJI_H__4E8A5881_3E5D_433E_8295_E58DD5E99840__INCLUDED_)
