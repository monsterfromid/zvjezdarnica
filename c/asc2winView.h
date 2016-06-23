// asc2winView.h : interface of the CAsc2winView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASC2WINVIEW_H__C1050A58_8E6D_434B_A565_FB277302010A__INCLUDED_)
#define AFX_ASC2WINVIEW_H__C1050A58_8E6D_434B_A565_FB277302010A__INCLUDED_

#include "Slika.h"	// Added by ClassView
#include "UlazDlg1.h"	// Added by ClassView
#include "NumDlg.h"	// Added by ClassView
#include "Aspekti.h"	// Added by ClassView
#include "Postavke.h"	// Added by ClassView
#include "Tijelo.h"	// Added by ClassView
#include "Tumac.h"	// Added by ClassView
#include "Godisnji.h"	// Added by ClassView
#include "dSunSustav.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAsc2winView : public CView
{
protected: // create from serialization only
	CAsc2winView();
	DECLARE_DYNCREATE(CAsc2winView)

// Attributes
public:
	CAsc2winDoc* GetDocument();
 
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAsc2winView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	double findTrueAnomaly(double inM,  double inE, double inErr );
	double piMod(double fi);
	double absFloor(double x);
	double mod (double a, double b);
	double div(double a, double b);
	void Iscrtaj_Planetarij(CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt);
	void Nacrtaj_Kalendar(CDC * pDC, int opcija);
	CString Nadji_Egipatsku_Godinu(int iPocDan, int iPocMje, int iGod);
	void Izracunaj_Snage(int bd);
	void ArrowTo(CDC * pDC, int x1, int y1, int x2, int y2, double r, double fi);
    void Dispozitori();
	void Ucitaj_Glif(CDC *pDC, int iMod, int iGlif, int x, int y);
	CString Nadji_Kinesku_Godinu( int g);
	void Iscrtaj_Tumac(CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt);
	void TrueEllipse(CDC *pDC, double x0,double y0, double a, double b, double fi1, double fi2);
	CSlika * m_dSlika;
	CDC AspDC2;
	HCURSOR lhCursor_Nebo;
	boolean m_bImena ;
	double fns( double x);
	double fnp( double x,  double mm);
	void readdata(int *i1, double *f1);
	void ln1060( double xx, double *yy,  double *rr, double *aa);
	double sgn(double x);
	void Planete(int dan, int mje, int god, int sat, int min);
	void Odredjivanje_uskrsa();
	void Odredjivanje_dst();
	boolean bJesuSlickeLoudane;
	CBitmap Slicka[60];
	void Ucitaj_Znake(CDC *pDC);
	double fnmo(double x);
	double frac(double x);
	CUlazDlg m_dUlazDlg;
	long int Razlika_Datuma(int dar, int mjr, int gor,int mjb,int gob);
	virtual ~CAsc2winView();
	char * strupr_s2(char * string);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnUpdateIndicatorAsp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorKuce(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorDST(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorLOK(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorTS (CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorBaza(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorIme(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorZvr(CCmdUI *pCmdUI);
//{{AFX_MSG(CAsc2winView)
	afx_msg void OnEditUlaznepostavke();
	afx_msg void OnDanPrije();
	afx_msg void OnDanPoslije();
	afx_msg void OnMjePrije();
	afx_msg void OnMjePoslije();
	afx_msg void OnGodPrije();
	afx_msg void OnGodPoslije();
	afx_msg void OnMinPrije();
	afx_msg void OnMinPoslije();
	afx_msg void OnSatPrije();
	afx_msg void OnSatPoslije();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnUpdateViewImena(CCmdUI* pCmdUI);
	afx_msg void OnViewImena();
	afx_msg void OnSada();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAspekti();
	afx_msg void OnUpdateAspekti(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAspTablica(CCmdUI* pCmdUI);
	afx_msg void OnAspTablica();
	afx_msg void OnNaredbaHoroskopPostavke();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNaredbaGodinji();
	afx_msg void OnGregor();
	afx_msg void OnPla();
	afx_msg void OnHor();
	afx_msg void OnTumac();
	afx_msg void OnDoUp();
	afx_msg void OnDoDown();
	afx_msg void OnButtonNebo();
	afx_msg void OnButtonLuna();
	afx_msg void OnButtonSus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Iscrtaj_Horoskop_Tjela(CDC * pDC, int dan, int mje,int god,int sat,int min,int gmt);
	double m_fZvr;
	CGodisnji m_dGodisnjiHoroskop;
	void Iskljuci_sliku();
	void Ukljuci_Sliku();
	int m_iOpc;
	CPostavke m_dPostavke;
	CTijelo * m_pdTijelo;
	CTumac * m_pdTumac;
	dSunSustav * m_pdSunSustav;
	int m_iKorakSati;
	int m_iKorakMinute;
	void Ucitaj_Postavke_Horoskopa(boolean bStartup);
	CAspekti * m_pdTablicaAspekata;
	CDC dcWheel2b;
	CDC dcWheel1;
	boolean m_bAspekti;
	boolean m_bAspektiTablica;
	void rad(CDC * pDC, int x0, int y0, int r1, int r2, double alfa, double fi);
	double fnx( double x);
	void Kuce(double asce, double ra, double ob, int cmd);
	void Iscrtaj_Horoskop (CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt);
	CPoint pntCapPoz;
	CPoint pntCapPoz2;
	CDC dcPozadina;  //u nebo - mouse popup
	CDC dcPozadina2; //u gregkal-mouse popup 
	void Ucitaj_Jata();
	int Ucitaj_Zvijezde();
	void Koord(CDC *pDC, double al,double ab,char *ime,int objekt);
	void Iscrtaj_Nebo(CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt);
	void Ucitaj_Znacajne_Datume();
	void Ellipse(CDC *pDC, double x0,double y0, double ra, double rb, double fi1, double fi2);
	void Iscrtaj_Lunu(CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt);
	void Iscrtaj_Planete(CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt);
	void Iscrtaj_Ascendente(CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt);
	void Nacrtaj_Tablicu(CDC * pDC, int opcija);

	int fmod2 (double a, double b);
	double fabs2 (double a);
	double floor2 (double a);

};

#ifndef _DEBUG  // debug version in asc2winView.cpp
inline CAsc2winDoc* CAsc2winView::GetDocument()
   { return (CAsc2winDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASC2WINVIEW_H__C1050A58_8E6D_434B_A565_FB277302010A__INCLUDED_)
