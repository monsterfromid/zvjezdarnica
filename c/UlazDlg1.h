#if !defined(AFX_ULAZDLG1_H__66AC1FE6_A092_4567_BE01_F38AE78763B8__INCLUDED_)
#define AFX_ULAZDLG1_H__66AC1FE6_A092_4567_BE01_F38AE78763B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxtempl.h"  // ja dodo zbog cobarraya
#include "BazniZapis.h"  // ja dodo zbog cobarraya
#include "Unos.h"        // ja dodo zbog cobarraya

// UlazDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUlazDlg dialog

class CUlazDlg : public CDialog
{
// Construction
public:
	char m_cSpol;
	CString m_sIme;
	CString m_sNazivBaze;
	char m_chSpol;
	char m_chSustavKuca;
	int m_iVri;
	CString m_sGrad;
	//bool m_bVri;
	int m_iMin;
	int m_iSat;
	int m_iGod;
	int m_iMje;
	int m_iDan;
	int m_iGmTime;
	float	m_fDuz;
	float	m_fSir;
	float   m_iHorRez;

	SYSTEMTIME m_dDatum;
	CStringArray m_saGradovi;
	CUlazDlg(CWnd* pParent = NULL);   // standard constructor


   CBazniZapis  * m_pBZapis;
   //CObArray  m_oaBaza ;                      // s ovim ne radi..???!!!
	CArray<CBazniZapis*,CBazniZapis*> m_aBaza;  //ovak radi...


// Dialog Data
	//{{AFX_DATA(CUlazDlg)
	enum { IDD = IDD_Ulaz };
	CComboBox	m_cbZanimanje;
	CSpinButtonCtrl	m_spinMje;
	CSpinButtonCtrl	m_spinGod;
	CSpinButtonCtrl	m_spinDan;
	CComboBox	m_cbZn2;
	CComboBox	m_cbZn1;
	CComboBox	m_cbOb2;
	CComboBox	m_cbOb1;
	CListBox	m_lbBaza;
	CMonthCalCtrl	m_cDatum;
	CComboBox	m_cbGrad;
	int		m_iGmt;
	CString	m_sDatum;
	CString	m_sSir;
	CString	m_sDuz;
	CTime	m_tSat;
	int		m_radSpol;
	BOOL	m_chkOb1;
	BOOL	m_chkOb2;
	int		m_iSelDan;
	int		m_iSelGod;
	int		m_iSelMje;
	BOOL	m_chkZanimanje;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUlazDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Osvjezi_Baznu_Listu();

	void Ucitaj_bazu();

	// Generated message map functions
	//{{AFX_MSG(CUlazDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGrad();
	afx_msg void OnSelectDatum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeSat(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkOb1();
	afx_msg void OnChkOb2();
	afx_msg void OnRadSpm();
	afx_msg void OnSelchangeListBaza();
	afx_msg void OnButton1();
	afx_msg void OnChangeSelDan();
	afx_msg void OnChkZanim();
	afx_msg void OnSelchangeZanimanje();
	afx_msg void Obrisi_Zapis();
	afx_msg void Izmjeni_Zapis();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//void Obrisi_Zapis();
	int m_iZanimSel;
	boolean m_bInit;
	double fnp(double x, double mm);
	double fns(double x);
	void ln1060(double xx, double *yy, double *rr, double *aa);
	void readdata(int *i1, double *f1);
	double sgn(double x);
	double objx[20];
	double fnmo(double x);
	double frac(double x);
	void Planete(int dan, int mje, int god, int sat, int min);
	CUnos m_dUnos;
	int m_iListBoxCurSel;
	int fmod2 (double a, double b);
	double fabs2 (double a);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULAZDLG1_H__66AC1FE6_A092_4567_BE01_F38AE78763B8__INCLUDED_)
