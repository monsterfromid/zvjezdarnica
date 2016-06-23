// asc2winView.cpp : implementation of the CAsc2winView class
//

#include "stdafx.h"
#include "asc2win.h"
#include "MainFrm.h" //dodao, jer bez toga ne radi combo u mainfrejmu ..
#include "Afxwin.h"

#include "math.h"    //iz orig.prga
#include "string.h"  //iz orig.prga
#include "dos.h"     //iz orig.prga
#include "stdio.h"   //iz orig.prga
#include "stdlib.h"  //iz orig.prga
#include "process.h" //iz orig.prga
//#include <ctype.h>   //iz orig.prga
//#include <time.h>    //iz orig.prga 
 
#include "asc2winDoc.h"
#include "asc2winView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int pl[12]={31,28,31,30,31,30,31,31,30,31,30,31};  

static double orb[7]={8,8,6,7,7,3};                    // orbovi 
static double porb[13]={010,10,7,8,8,7,6,6,5,5,7,7};

static int dan_praznik[365], aspen=0;
static int mje_praznik[365];
static CStringArray saImenaPraznika, saImendani;
static int boja_praznik[365], br_praznika=0;

static int dan_ostalo[100];
static int mje_ostalo[100];
static CStringArray saImenaOstalo;
static int br_ostalo=0;

static double objx[430],objy[430];
static char obj[450][20];
static double pi=3.1415927;

static double zvr=0,asc=0;
static double tg=0,es=0,ec=0,ar=0,sb=0,cb=0,el=0,ea=0;
static int broj_zvijezda=0, sjaj[430]={0}, pripadnost[430]={0,23,23}, o_old=1000;
static char jato[45][36]={{0,0,0,0},{0,0}};
static CStringArray ImeObjekata;
static CBitmap bmCap, * bmOld; // u opciji nebo
static CBitmap bmCap2, * bmOld2; // u opciji gregkal
static int akt[12][12];        //pronaðeni aspekti

//za dispozitore
static int veza[11]={0,0,0,0,0,0,0,0,0,0,0};			 //veze planeta (1-10) prema dispozitorima
static int broj_veza[11]={0,0,0,0,0,0,0,0,0,0,0};		 //broj veza na neku planetu  

static int xp[11]={0,0,0,0,0,0,0,0,0,0,0};
static int yp[11]={0,0,0,0,0,0,0,0,0,0,0};			     //koordinate ispisa planete
static int yp2[11]={0,0,0,0,0,0,0,0,0,0,0};			     //koordinate ispisa planete, kopija za tumaè jer veæ ima varijablu yp

// snaga planeta
static double snaga_planeta[14];
static double snaga_znakova[14];
static double snaga_elemenata[6];
static double snaga_tipa[5];       //kard, fix,..
static double snaga_pricipa[4];  //muško/žensko

static double prim_snaga_planeta[13]={0,20, 15, 9, 12, 12, 10, 10, 9, 8, 7, 
									20,15};		 //asc i mc

static int dan_tj[13]={0,0,0,0,0,0,0,0,0,0,0};			     //dan u tjednu 1.dana u mjesecu
boolean invert_is_on=false;
int     inv_x1, inv_y1;
boolean invert2_is_on=false;
int     inv2_x1, inv2_y1;

boolean popup_is_on=false;  //za popup kod greg.kal.

int iStvarniIndex[15]    = {11,0,4,6,5,1,2,3,12,7,8,9,10};  //kad se pretvara iz sadašnjeg u stari
int iStvarniIndexSet[15] = {1,5,6,7,2,4,3,9,10,11,12,0,8};  //kad se pretvara iz starog u sadašnji


/////////////////////////////////////////////////////////////////////////////
// CAsc2winView

IMPLEMENT_DYNCREATE(CAsc2winView, CView)

BEGIN_MESSAGE_MAP(CAsc2winView, CView)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TIMESTAMP, OnUpdateIndicatorTS)   //dodano za novi status
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_LOKACIJA, OnUpdateIndicatorLOK)   //dodano za novi status
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_DST, OnUpdateIndicatorDST)   //dodano za novi status
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_KUCE, OnUpdateIndicatorKuce)   //dodano za novi status
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_ASP, OnUpdateIndicatorAsp)   //dodano za novi status
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_BAZA, OnUpdateIndicatorBaza)   //dodano za novi status
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_IME, OnUpdateIndicatorIme)   //dodano za novi status
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_ZVR,OnUpdateIndicatorZvr)
	//{{AFX_MSG_MAP(CAsc2winView)
	ON_COMMAND(ID_EDIT_ULAZNEPOSTAVKE, OnEditUlaznepostavke)
	ON_COMMAND(ID_DAN_PRIJE, OnDanPrije)
	ON_COMMAND(ID_DAN_POSLIJE, OnDanPoslije)
	ON_COMMAND(ID_MJE_PRIJE, OnMjePrije)
	ON_COMMAND(ID_MJE_POSLIJE, OnMjePoslije)
	ON_COMMAND(ID_GOD_PRIJE, OnGodPrije)
	ON_COMMAND(ID_GOD_POSLIJE, OnGodPoslije)
	ON_COMMAND(ID_MIN_PRIJE, OnMinPrije)
	ON_COMMAND(ID_MIN_POSLIJE, OnMinPoslije)
	ON_COMMAND(ID_SAT_PRIJE, OnSatPrije)
	ON_COMMAND(ID_SAT_POSLIJE, OnSatPoslije)
	ON_WM_MOUSEMOVE()
	ON_UPDATE_COMMAND_UI(ID_VIEW_IMENA, OnUpdateViewImena)
	ON_COMMAND(ID_VIEW_IMENA, OnViewImena)
	ON_COMMAND(ID_SADA, OnSada)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_ASPEKTI, OnAspekti)
	ON_UPDATE_COMMAND_UI(ID_ASPEKTI, OnUpdateAspekti)
	ON_UPDATE_COMMAND_UI(ID_ASP_TABLICA, OnUpdateAspTablica)
	ON_COMMAND(ID_ASP_TABLICA, OnAspTablica)
	ON_COMMAND(ID_NAREDBA_HOROSKOP_POSTAVKE, OnNaredbaHoroskopPostavke)
	ON_WM_SIZE()
	ON_COMMAND(ID_NAREDBA_GODINJI, OnNaredbaGodinji)
	ON_COMMAND(ID_GREGOR, OnGregor)
	ON_COMMAND(ID_PLA, OnPla)
	ON_COMMAND(ID_HOR, OnHor)
	ON_COMMAND(ID_TUMAC, OnTumac)
	ON_COMMAND(ID_DO_UP, OnDoUp)
	ON_COMMAND(ID_DO_DOWN, OnDoDown)
	ON_COMMAND(ID_BUTTON_NEBO, OnButtonNebo)
	ON_COMMAND(ID_BUTTON_LUNA, OnButtonLuna)
	ON_COMMAND(ID_BUTTON_SUS, OnButtonSus)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAsc2winView construction/destruction

CAsc2winView::CAsc2winView()
{
	// TODO: add construction code here


	bJesuSlickeLoudane =FALSE;
	if (!bJesuSlickeLoudane ) Ucitaj_Znacajne_Datume();

	m_bImena = TRUE;
	//m_bAspekti = FALSE;
	//m_bAspektiTablica = FALSE;
	m_iKorakMinute=5;
	m_iKorakSati=1;

	Ucitaj_Postavke_Horoskopa(TRUE);

   // modeless dialog, više ga ne koristim za prikaz imena zvijezda
	/*  pPopupDlg = new CPopupDlg;
       pPopupDlg->Create(IDD_Popup,this);
	*/

}

CAsc2winView::~CAsc2winView()
{
}

BOOL CAsc2winView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAsc2winView drawing

void CAsc2winView::OnDraw(CDC* pDC)
{
	CAsc2winDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data herež
	
	Odredjivanje_uskrsa(); 
	Odredjivanje_dst();

	if (!bJesuSlickeLoudane ) { 

        m_pdSunSustav = new dSunSustav(this);
		m_pdSunSustav->Create(IDD_SUNSUSTAV);
		m_pdSunSustav->ShowWindow(SW_HIDE);

        
		m_pdTijelo = new CTijelo(this, &m_dUlazDlg);
		m_pdTijelo->Create(IDD_TIJELO);
		m_pdTijelo->ShowWindow(SW_HIDE);

        m_pdTumac = new CTumac(this);
		m_pdTumac->Create(IDD_TUMAC);
		m_pdTumac->ShowWindow(SW_HIDE);
		

        m_pdTablicaAspekata = new CAspekti(this,&AspDC2);
		m_pdTablicaAspekata->Create(IDD_ASPEKTI);
 
		m_dSlika = new CSlika(this);
		m_dSlika->Create(IDD_SLIKA);
		m_dSlika->ShowWindow(SW_HIDE);

		Ucitaj_Znake(pDC);
		Ucitaj_Jata();

		lhCursor_Nebo = AfxGetApp()->LoadCursor(IDC_CURSOR_NEBO);


        ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.SetCurSel(m_iOpc-1);   //Handle na CMainFrame objekt!!!!!!




		this->GetWindowRect(&(m_dSlika->m_rSlika) );

		//m_dSlika->m_rSlika.left = m_dSlika->m_rSlika.right-200;
		//m_dSlika->m_rSlika.top = m_dSlika->m_rSlika.bottom-350;

		
		this->GetWindowRect(&(m_pdTablicaAspekata->m_rSlika) );

		m_pdTablicaAspekata->m_rSlika.left=m_pdTablicaAspekata->m_rSlika.right- 458;
		m_pdTablicaAspekata->m_rSlika.top=m_pdTablicaAspekata->m_rSlika.top-80;

	}

	int nIndex = iStvarniIndex[( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.GetCurSel()];   //Handle na CMainFrame objekt!!!!!!
//								 -------------------------------------------
//								 -------------------------------------------


	//ovo ne radi kao je ukljuèeno autoupdate menua (a to je default)

	/*
	CMenu* mmenu = ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->GetMenu();
	
	CMenu* submenu = mmenu->GetSubMenu(3);
	CMenu* submenu2 = submenu->GetSubMenu(1);
	
	if (nIndex !=4 ) submenu2->EnableMenuItem(ID_ASPEKTI, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	else				  submenu2->EnableMenuItem(ID_ASPEKTI, MF_BYCOMMAND | MF_ENABLED);
	*/

	if( m_pdSunSustav->m_bIsActive==TRUE && nIndex !=12 ) { 
		m_pdSunSustav->ShowWindow(SW_HIDE);
		m_pdSunSustav->m_bIsActive=FALSE; 
		Invalidate();
	}

	if(m_pdTablicaAspekata->m_bIsActive==TRUE && nIndex !=4 && nIndex !=5 ) { 
		m_pdTablicaAspekata->ShowWindow(SW_HIDE);
		m_pdTablicaAspekata->m_bIsActive=FALSE; 
		Invalidate();
	}


	if ( ( m_pdTijelo->m_bIsActive==TRUE && nIndex !=4 && nIndex !=5) ||
		 (m_pdTumac->m_bIsActive==TRUE && nIndex !=6)
	   ) 	{ 

		m_pdTijelo->ShowWindow(SW_HIDE); 
		m_pdTijelo->m_bIsActive=FALSE; 

		m_pdTumac->ShowWindow(SW_HIDE); 
		m_pdTumac->m_bIsActive=FALSE; 

		m_pdTumac->m_iPage=1; m_pdTumac->m_iPage2=1; 

		Invalidate();
	}

	CRect rect;
	GetClientRect(rect);
	if ((float)rect.Width()/rect.Height()<1.4) //4:3
			m_dUlazDlg.m_iHorRez=4/3.;
	else	m_dUlazDlg.m_iHorRez=16/9.;




	m_fZvr=0;

	// ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_wndToolBar.GetDlgItem(ID_ASPEKTI)->EnableWindow(FALSE);  // ne radi


	if (nIndex == 0 || nIndex == 1 )  Nacrtaj_Tablicu(pDC,nIndex);

	if (nIndex == 0) Iscrtaj_Planete   (pDC, m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod, m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin, m_dUlazDlg.m_iGmTime );
	if (nIndex == 1) Iscrtaj_Ascendente(pDC, m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod, m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin, m_dUlazDlg.m_iGmTime );
	if (nIndex == 2) Iscrtaj_Nebo      (pDC, m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod, m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin, m_dUlazDlg.m_iGmTime );
	if (nIndex == 3) Iscrtaj_Lunu      (pDC, m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod, m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin, m_dUlazDlg.m_iGmTime );
	if (nIndex == 4) {
		Iscrtaj_Horoskop  	(pDC, m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod, m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin, m_dUlazDlg.m_iGmTime );
		if( m_bAspektiTablica == TRUE)	 {
			m_pdTablicaAspekata->m_bIsActive=TRUE; 
			m_pdTablicaAspekata->ShowWindow(SW_SHOW);
		}
		if( pDC->IsPrinting() ==FALSE) { m_pdTijelo->ShowWindow(SW_SHOW);m_pdTijelo->m_bIsActive=TRUE;}
	}
	if (nIndex == 5) {
		Iscrtaj_Horoskop_Tjela (pDC, m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod, m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin, m_dUlazDlg.m_iGmTime );
		if( m_bAspektiTablica == TRUE)	 {
			m_pdTablicaAspekata->m_bIsActive=TRUE; 
			m_pdTablicaAspekata->ShowWindow(SW_SHOW);
		}
		if( pDC->IsPrinting() ==FALSE) { m_pdTijelo->ShowWindow(SW_SHOW);m_pdTijelo->m_bIsActive=TRUE;}
	}

	if (nIndex == 6)  { 
		 m_pdTumac->m_iOpcijaSpec=0;
	     Iscrtaj_Tumac    (pDC, m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod, m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin, m_dUlazDlg.m_iGmTime );

		if( pDC->IsPrinting() ==FALSE) { m_pdTumac->ShowWindow(SW_SHOW);m_pdTumac->m_bIsActive=TRUE;}

	}

	if (nIndex == 7)  {   //kin.hor

		 m_pdTumac->m_iOpcijaSpec=1;
	     Iscrtaj_Tumac    (pDC, m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod, m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin, m_dUlazDlg.m_iGmTime );

	}
	if (nIndex == 8)  {   //azt.hor

		 m_pdTumac->m_iOpcijaSpec=2;
	     Iscrtaj_Tumac    (pDC, m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod, m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin, m_dUlazDlg.m_iGmTime );

	}
	if (nIndex == 9)  {   //maj.hor

		 m_pdTumac->m_iOpcijaSpec=3;
	     Iscrtaj_Tumac    (pDC, m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod, m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin, m_dUlazDlg.m_iGmTime );

	}

	if (nIndex == 10)  {   //egip.hor

		 m_pdTumac->m_iOpcijaSpec=4;
	     Iscrtaj_Tumac    (pDC, m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod, m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin, m_dUlazDlg.m_iGmTime );

	}
	if (nIndex == 11)  {   //georg.kal.

		invert_is_on=false;
		invert2_is_on=false;
		Nacrtaj_Kalendar(pDC,nIndex);
	}
	if (nIndex == 12)  {   //planetarij

		m_pdSunSustav->m_bIsActive=TRUE; 
		m_pdSunSustav->ShowWindow(SW_SHOW);
		Iscrtaj_Planetarij(pDC, m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod, m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin, m_dUlazDlg.m_iGmTime );
	}


}

/////////////////////////////////////////////////////////////////////////////
// CAsc2winView printing

BOOL CAsc2winView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->SetMinPage(1);

	int nIndex = iStvarniIndex[( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.GetCurSel()];   //Handle na CMainFrame objekt!!!!!!


	if (nIndex==6) pInfo->SetMaxPage(	floor2(m_pdTumac->m_iMax * 32. / 80 ) + 1 );
	else           pInfo->SetMaxPage(1);

	return DoPreparePrinting(pInfo);
}

void CAsc2winView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAsc2winView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing

}

/////////////////////////////////////////////////////////////////////////////
// CAsc2winView diagnostics

#ifdef _DEBUG
void CAsc2winView::AssertValid() const
{
	CView::AssertValid();
}

void CAsc2winView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAsc2winDoc* CAsc2winView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAsc2winDoc)));
	return (CAsc2winDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAsc2winView message handlers

void CAsc2winView::Nacrtaj_Tablicu(CDC * pDC, int opcija)
{

	int ip,jp, ipo,jpo;
	int s=1;
	long int n;
	char buff[80];
	CRect pravokutnik(0,0,0,0);
	CRect * pPrav = &pravokutnik;


	pl[1]=28; 
	if (fmod2(m_dUlazDlg.m_iGod,4)==0 ) 
		if ( fmod2(m_dUlazDlg.m_iGod,100)!= 0 || fmod2(m_dUlazDlg.m_iGod,400)==0 || m_dUlazDlg.m_iGod<1600) pl[1]=29;



 
//	pDC->SetMapMode (MM_TEXT);
	pDC->SetMapMode (MM_ISOTROPIC);

	CRect rect;
	GetClientRect(rect);

if (pDC->IsPrinting() ==FALSE) {

	pDC->SetWindowExt( 600* m_dUlazDlg.m_iHorRez ,600);      // logièke koordinate

	pDC->SetViewportExt(rect.Width()*1.1,rect.Width()/m_dUlazDlg.m_iHorRez*1.1);  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike
	pDC->SetViewportOrg (0,700/20);  // pomakni poèetak k.s-a na dolje
}
else {

	pDC->SetWindowExt( 600* m_dUlazDlg.m_iHorRez, 600);      // logièke koordinate
	pDC->SetViewportExt(4800, 3600);  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike
    pDC->SetViewportOrg (050,100);        // pomakni poèetak k.s-a na dolje
}

CPen olovka(PS_SOLID, 1, Colors[15]);                 //tip, debljina, boja
CPen* staraOlovka = pDC->SelectObject(&olovka);           // pohrani staru olovku

if (m_dUlazDlg.m_iGod > 1582 || (m_dUlazDlg.m_iGod == 1582 && m_dUlazDlg.m_iMje>=10) ){

  if (m_dUlazDlg.m_iGod<1985) s=-1;
  if (m_dUlazDlg.m_iGod==1985 && m_dUlazDlg.m_iMje<12 ) s=-1;
  if(s<0)   n=Razlika_Datuma(1,m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod,12,1985)+0;
  else      n=Razlika_Datuma(1,12,1985,m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod)+1; //prvi je datum NEDJELJE
}
else {   //gregorijanski preskok s 4 na 15.10.1582, ali s èetvrtka na petak!

  s=-1; 
  n=Razlika_Datuma(1,m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod,9,1582)+30-1;   //30.9.1582 je navodno bila nedjelja

}

	invert_is_on = false;


    CBrush* brush;
//	brush = new CBrush(5, Colors[4]); //tip, boja
//	CBrush* oldBrush = pDC->SelectObject(brush);// Select the new brush into the DC.

	CBitmap m_bmpBrush;
	m_bmpBrush.LoadBitmap(IDB_FILUJ);

	brush = new CBrush(&m_bmpBrush); 
  	CBrush* oldBrush = pDC->SelectObject(brush);// Select the new brush into the DC.

	CBitmap m_bmpBrush3;
	m_bmpBrush3.LoadBitmap(IDB_FILUJ4);
	CBrush * brush3 = new CBrush(&m_bmpBrush3); 

	
	pPrav->left= 91;
	pPrav->right = 630+1;
	
	//nedjelje
	for (ip=1; ip<=pl[m_dUlazDlg.m_iMje -1]; ip++) if(fmod2(n+s*(ip-1),7)==0 || fmod2(n+s*(ip-1),7)==1) {

		
		if (m_dUlazDlg.m_iMje==10 && m_dUlazDlg.m_iGod ==1582 && ip>5 && ip<15) continue;
		
		
		
		if(ip==m_dUlazDlg.m_iDan) pDC->SelectObject(brush3);
		else pDC->SelectObject(brush);

		pPrav->top = 16+(ip-1)*10-1;
		pPrav->bottom = 24+(ip-1)*10;

		if(ip==m_dUlazDlg.m_iDan) pDC->FillRect(pPrav, brush3);
		else pDC->FillRect(pPrav, brush);
	}

	pDC->SelectObject(oldBrush);

	delete brush;
	delete brush3;
	
// praznici 

	CBitmap m_bmpBrush2;
	m_bmpBrush2.LoadBitmap(IDB_FILUJ2);


	brush = new CBrush(&m_bmpBrush2); 
  	oldBrush = pDC->SelectObject(brush);// Select the new brush into the DC.

	m_bmpBrush3.Detach();
	m_bmpBrush3.LoadBitmap(IDB_FILUJ3);
	brush3 = new CBrush(&m_bmpBrush3); 
	
	CBitmap m_bmpBrush4;
	m_bmpBrush4.LoadBitmap(IDB_FILUJ5);
	CBrush * brush4 = new CBrush(&m_bmpBrush4); 

	
	pPrav->left= 91;
	pPrav->right = 630+1;
	
	for (ip=1; ip<=pl[m_dUlazDlg.m_iMje -1]; ip++) {

	if (m_dUlazDlg.m_iMje==10 && m_dUlazDlg.m_iGod ==1582 && ip>5 && ip<15) continue;

	boolean bFound1=FALSE;
		
		for (jp=2; jp<=2+br_praznika+2+2-1; jp++) {

			if(ip==dan_praznik[jp-1] && m_dUlazDlg.m_iMje==mje_praznik[jp-1] && jp!=4 && jp!=5 && jp!=6 && jp!=7) {

				if(ip==m_dUlazDlg.m_iDan) pDC->SelectObject(brush3);
				else pDC->SelectObject(brush);

				pPrav->top = 16+(ip-1)*10-1;
				pPrav->bottom = 24+(ip-1)*10;

				if(ip==m_dUlazDlg.m_iDan) pDC->FillRect(pPrav, brush3);
				else pDC->FillRect(pPrav, brush);
				
				bFound1=TRUE;
				break;

			}
			
		}
		if (bFound1==FALSE && m_dUlazDlg.m_iDan==ip && (fmod2(n+s*(ip-1),7)!=0 && fmod2(n+s*(ip-1),7)!=1) ) {

			pPrav->top = 16+(ip-1)*10-1;
			pPrav->bottom = 24+(ip-1)*10;

			pDC->SelectObject(brush4);
			pDC->FillRect(pPrav, brush4);
		}

	}	
	
	pDC->SelectObject(oldBrush);
	m_bmpBrush3.Detach();
	m_bmpBrush2.Detach();
	m_bmpBrush4.Detach();
	delete brush;
	delete brush3;
	delete brush4;


// izrada tablice:


pDC->MoveTo(90,0); pDC->LineTo(631,0);

for(ip=1;ip<=pl[m_dUlazDlg.m_iMje-1]+1;ip++) {

  if (m_dUlazDlg.m_iMje==10 && m_dUlazDlg.m_iGod ==1582 && ip>5 && ip<15) continue;
  pDC->MoveTo(90,(5+ip*10) ); pDC->LineTo(630,(5+ip*10));
}


for(ip=0;ip<=11;ip++) {

  pDC->MoveTo(90+ip*45,2 ); pDC->LineTo(90+ip*45,15+pl[m_dUlazDlg.m_iMje -1]*10);
}

for(ip=0;ip<=11;ip++) for(jp=16;jp<=15+pl[m_dUlazDlg.m_iMje-1]*10; jp+=3 ) pDC->SetPixel(90+22.5+ip*45,jp,Colors[15]);

pDC->MoveTo(631,2 ); pDC->LineTo(631,15+pl[m_dUlazDlg.m_iMje -1]*10);


//ispis oznake punog mjeseca:

 CPen olovka2(PS_SOLID, 1, Colors[15]);                 //tip, debljina, boja
 CPen* staraOlovka2 = pDC->SelectObject(&olovka2);           // pohrani staru olovku

  brush = new CBrush(Colors[6]);
  oldBrush = pDC->SelectObject(brush);// Select the new brush into the DC.

float punimj=180,temppm,tmppm1,ipumj=1;

 int jsat,danPM=0, danPM2=0, priblizava;

          punimj=180;
		  priblizava=0;

		  for (ip=1;ip<=pl[m_dUlazDlg.m_iMje -1]; ip++) {   //korigirana rutina

				jsat=0;
looopPM:
                Planete(ip,m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod,jsat,0);
                
				tmppm1=fabs(objx[1]-objx[2]); if (tmppm1>180) tmppm1=360-tmppm1;
				temppm=fabs(180-tmppm1);

                if(temppm<punimj ) {
					if(punimj<180) priblizava=1;
					punimj=temppm;
				}

                if(temppm>punimj && priblizava==1) {danPM=ip;break; }

				jsat+=4; if (jsat<24) goto looopPM;
          }


		if (danPM!=0) if (!(m_dUlazDlg.m_iMje==10 && m_dUlazDlg.m_iGod ==1582 && ip>5 && ip<16))  pDC->Ellipse(636-4,21+(danPM-1)*10-4,636+4,21+(danPM-1)*10+4);

          punimj=180;
		  priblizava=0;

         for (ip=pl[m_dUlazDlg.m_iMje -1];ip>=1; ip--) {
		     
			    jsat=20;
looopPM2:
                Planete(ip,m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod,jsat,0);
                
				tmppm1=fabs(objx[1]-objx[2]); if (tmppm1>180) tmppm1=360-tmppm1;
				temppm=fabs(180-tmppm1);

                if(temppm<punimj ) {
					if(punimj<180) priblizava=1;
					punimj=temppm;
				}

                if(temppm>punimj && priblizava==1) {danPM2=ip;break; }

				jsat-=4; if (jsat>=0) goto looopPM2;
          }


		if (danPM2!=0 && fabs2(danPM-danPM2)>4 ) if (!(m_dUlazDlg.m_iMje==10 && m_dUlazDlg.m_iGod ==1582 && ip>5 && ip<16)) pDC->Ellipse(636-4,21+(danPM2-1)*10-4,636+4,21+(danPM2-1)*10+4);



	  pDC->SelectObject(oldBrush);
	  delete brush;



//ispis oznake mladog mjeseca:
		  danPM=0; danPM2=0;
          punimj=180;
		  priblizava=0;

		  for (ip=1;ip<=pl[m_dUlazDlg.m_iMje -1]; ip++) {   //korigirana rutina
		     
			
			    jsat=0;
looopMM:
                Planete(ip,m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod,jsat,0);
                temppm=fabs(objx[1]-objx[2]);if (temppm>180) temppm=360-temppm;

                if(temppm<punimj ) {
					if(punimj<180) priblizava=1;
					punimj=temppm;
				}

                if(temppm>punimj && priblizava==1) {danPM=ip;break; }

				jsat+=4; if (jsat<24) goto looopMM;
          }

		brush = new CBrush(Colors[14]);
		oldBrush = pDC->SelectObject(brush);// Select the new brush into the DC.

		if (danPM!=0) if (!(m_dUlazDlg.m_iMje==10 && m_dUlazDlg.m_iGod ==1582 && ip>5 && ip<16)) pDC->Ellipse(636-4,21+(danPM-1)*10-4,636+4,21+(danPM-1)*10+4);

          punimj=180;
		  priblizava=0;

         for (ip=pl[m_dUlazDlg.m_iMje -1];ip>=1; ip--) {



				jsat=20;
looopMM2:
                Planete(ip,m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod,jsat,0);
                temppm=fabs(objx[1]-objx[2]); if (temppm>180) temppm=360-temppm;

                if(temppm<punimj ) {
					if(punimj<180) priblizava=1;
					punimj=temppm;
				}

                if(temppm>punimj && priblizava==1) {danPM2=ip;break; }

				jsat-=4; if (jsat>=0) goto looopMM2;
          }


		if (danPM2!=0 && fabs2(danPM-danPM2)>4 ) if (!(m_dUlazDlg.m_iMje==10 && m_dUlazDlg.m_iGod ==1582 && ip>5 && ip<16)) pDC->Ellipse(636-4,21+(danPM2-1)*10-4,636+4,21+(danPM2-1)*10+4);



		  pDC->SelectObject(oldBrush);
	      delete brush;
		  
		  pDC->SelectObject(staraOlovka2);  // vrati staru olovku
	


// ispiši tekst :

LOGFONT logFont;        // setup fonta
logFont.lfHeight = 8;
logFont.lfWidth = 0;
logFont.lfEscapement = 0;
logFont.lfOrientation = 0;
logFont.lfWeight = FW_BOLD;
logFont.lfItalic = 0;
logFont.lfUnderline = 0;
logFont.lfStrikeOut = 0;
logFont.lfCharSet = ANSI_CHARSET;
logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
logFont.lfQuality = PROOF_QUALITY;
logFont.lfPitchAndFamily = FIXED_PITCH | FF_ROMAN;
strcpy(logFont.lfFaceName, "Arial");

if (pDC->IsPrinting() ==FALSE) logFont.lfHeight = 11;  // Set the new font’s height.
else logFont.lfHeight = 10;


CFont font;
font.CreateFontIndirect(&logFont);            // Create a new font and select it into the DC.
CFont* oldFont = pDC->SelectObject(&font);


// Print text with the new font.
if (opcija == 1) {
	pDC->SetTextColor(Colors[11]);

	for(ip=0;ip<=11;ip++) {
	sprintf(buff,"  %02d-%02d h",ip*2,ip*2+2);
	pDC->TextOut(94+ip*45,4,buff);
	}
}

//sprintf(buff,"  %d. mj. %d.",m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod);  pDC->TextOut(0,0,buff);
//pDC->MoveTo(0,11); pDC->LineTo(10*8,11);

for(ip=1;ip<=pl[m_dUlazDlg.m_iMje-1];ip++) {

	if (m_dUlazDlg.m_iMje==10 && m_dUlazDlg.m_iGod ==1582 && ip>4 && ip<15) continue;

		
		pDC->SetTextColor(Colors[15]);

		for(jp=1;jp<=saImenaPraznika.GetSize();jp++) 
			if(ip==dan_praznik[jp-1] && m_dUlazDlg.m_iMje==mje_praznik[jp-1] ) { 
			pDC->SetTextColor(Colors[boja_praznik[jp-1]]);

			pDC->TextOut(8,ip*10+8-3,saImenaPraznika.GetAt(jp-1));

			if (jp>br_praznika || jp==1) pDC->SetTextColor(Colors[15]);
		
 			break;
		}

		if(fmod2(n+s*(ip-1),7)==0 || fmod2(n+s*(ip-1),7)==1) {
			pDC->SetTextColor(Colors[11]);
		}

		if (ip==m_dUlazDlg.m_iDan) {
			pDC->SetTextColor(Colors[2]);
		}

	

	sprintf(buff,"%02d.",ip);
    pDC->TextOut(72,ip*10+8-3,buff);
	

}


//ispiši stare imendane

    
	int tmpPl1=pl[1]; pl[1]=29;

	int iPoz=0;
    if(m_dUlazDlg.m_iMje>1) for(ip=1;ip<m_dUlazDlg.m_iMje;ip++) for(jp=1;jp<=pl[ip-1];jp++) iPoz++;

	pl[1]=tmpPl1;

	for(ip=1;ip<=pl[m_dUlazDlg.m_iMje-1];ip++) {


		if (m_dUlazDlg.m_iMje==10 && m_dUlazDlg.m_iGod ==1582 && ip>4 && ip<15) continue;

	
		pDC->SetTextColor(Colors[15]);
		
		if(fmod2(n+s*(ip-1),7)==0 || fmod2(n+s*(ip-1),7)==1 ) 	pDC->SetTextColor(Colors[11]);


		for(jp=1;jp<=saImenaPraznika.GetSize();jp++) 
			if(ip==dan_praznik[jp-1] && m_dUlazDlg.m_iMje==mje_praznik[jp-1] ) pDC->SetTextColor(Colors[boja_praznik[jp-1]]);




        pDC->TextOut(641,ip*10+8-3,saImendani.GetAt(iPoz+ip-1));

	}
	

// iscrtaj znakove

 CDC MemDC;
 MemDC.CreateCompatibleDC( pDC );
 BITMAP bm;

 CBitmap *pOldBitmap;

if (opcija == 0) {

	for(ip=0;ip<=11;ip++) {

	pOldBitmap = MemDC.SelectObject( &Slicka[12+ip] );
	Slicka[12+ip].GetObject( sizeof( BITMAP ), &bm );

 	pDC->BitBlt( 105+ip*45, 4, bm.bmWidth, bm.bmHeight, &MemDC,0, 0, SRCCOPY );

	MemDC.SelectObject( pOldBitmap );

	}
}






pDC->SelectObject(oldFont);      // vrati stari font

pDC->SelectObject(staraOlovka);  // vrati staru olovku


}
//********************************************************************************
long int CAsc2winView::Razlika_Datuma(int dar, int mjr, int gor,int mjb,int gob)
{
// (funkcija je prijevod sa BASIC-a, u kom je prvotno bila napisana)

       int x,z;
       long int d=1;
	   int plold=pl[1];


l960:  z=mjr;
	 if (gob==gor) goto l1170;
	 if (abs(gob-gor)<3) goto l1020;
	 for (x=gor+1; x<=gob-1; x++)
	   {
		  if (x==0) continue;  //ne postoji godina 0, samo 1BC, i odmah zatim 1AD
	      d=d+365;
		  if (fmod2(x,4)==0 ) if ( fmod2(x,100)!= 0 || fmod2(x,400)==0 || x<1600) d++;  //prestupne god. dj. s 4 ako nisu sa 100, ili jesu sa 400
		  if (x==1582) d=d-10;  //1582 je preskoèeno 10 dana
	   }
	 pl[1]=28;
l1020:  if (fmod2(gor,4)==0 ) if ( fmod2(gor,100)!= 0 || fmod2(gor,400)==0 || gor<1600) pl[1]=pl[1]+1;
l1030:  d=d+pl[z-1];
	  if (z==12) goto l1080;
	  z=z+1;
	  goto l1030;
l1080:  z=1;
	  if (mjb==1) goto l1150;
	  pl[1]=28; if (fmod2(gob,4)==0 ) if ( fmod2(gob,100)!= 0 || fmod2(gob,400)==0 || gob<1600) pl[1]++;
l1110:  d=d+pl[z-1];
	  if (z==mjb-1) goto l1150;
	  z=z+1;
	  goto l1110;
l1150:  d=d-dar;
      pl[1]=plold;
	  return (d);
l1170:  if (mjb==mjr) { d=d-dar; pl[1]=plold; return(d); };
	  z=mjr;
l1190:  d=d+pl[z-1];
	  if (z==mjb-1) goto l1220;
	  z=z+1;
	  goto l1190;
l1220:  d=d-dar;
      pl[1]=plold;
	  return (d);
}

//********************************************************************************
void CAsc2winView::OnEditUlaznepostavke() 
{
	if (m_dUlazDlg.DoModal () == IDOK ) {

		if (m_dUlazDlg.m_sGrad != "<iz baze>") {
			
			Ucitaj_Postavke_Horoskopa(FALSE);  //ako nije iz baze uzmi defaultne postavke
			Iskljuci_sliku();
		}
		else { 
			 m_dUlazDlg.m_iVri=0;  //svi unosi u bazu imaju vrijeme pretvoreno u stvarno sunèano vrijeme
			 Ukljuci_Sliku();
			}


		Invalidate();              // ponovo nacrtaj 

	}	
}
//******************************************************************************
void CAsc2winView::Iscrtaj_Ascendente(CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt)
{
int vri, dn, xx1,xx2, xo1=0,xo2=0, trajanje, danx,mjex,godx;
double Gvr, ta, tg=0 , i, j, k=0, e, es=0, ec=0,  ar;
double duz, sir;
double ra, rsir, x, y, zvr, oldasc=0, asc=0 ;
double xmin[33][13];
int mje_orig = mje;
CString buff;

duz = m_dUlazDlg.m_fDuz;
sir = m_dUlazDlg.m_fSir;
vri = m_dUlazDlg.m_iVri;

// CString MBuf;
// MBuf.Format("%d",gmt);
// MessageBox(MBuf);


for(dn=1;dn<=pl[mje_orig-1]+1;dn++) {
   dan=dn;

     if (dan>pl[mje_orig-1]) {
 	   dan=1; mje=mje+1;
 	   if (mje==13) {mje =1; god =god +1; }
	 }


xx1=0;
do {
    xx2=0;
    do {

sat=xx1-vri;
min=xx2;

danx=dan; 
mjex=mje; 
godx=god; 

if(sat<0) {
	sat+=24;
	danx-=1;

}
if(danx<1) {
	mjex-=1;
	if(mjex<1) {
		mjex=12;
		godx-=1;
	
	}	
	danx=pl[mjex-1];
	
}


Gvr=(double)(sat-gmt+min/60.); //Globalno vrijeme
if(Gvr<0) {
	Gvr+=24;
	danx-=1;

}
if(danx<1) {
	mjex-=1;
	if(mjex<1) {
		mjex=12;
		godx-=1;
	
	}	
	danx=pl[mjex-1];
	
}





i=-(mjex<3);
k=danx+floor2((mjex*153.-i*11.-162)/5.)+floor2((godx*1461.+i)/4.)-(godx>=0)*366.;
if (k>577736) k-=floor2((floor2((godx+i)/100.)*3-5)/4.);

ta=k-693596; tg=k-711858+Gvr/24.;                   // tg je GLOBAL
j=ta/36525. ; i=ta/36525.;
e=23.452294-j*0.013125-j*j*1.639e-6+j*j*j*5.028e-7;
es=sin(e*pi/180); ec=cos(e*pi/180);                // GLOBAL

ar=Gvr*360.985647/24+frac(ta/1461)*1440.02509;      // GLOBAL
ar+=floor2(ta/1461)*0.0307572+99.2018973;
ar=fnmo(ar);
zvr=ar*24/360-duz*4/60; while(zvr>24)  zvr-=24;
            while(zvr<0)   zvr+=24; 
m_fZvr=zvr;

  ra=zvr*pi/12; rsir=sir*pi/180;
  x=cos(rsir)*sin(ra)*ec+sin(rsir)*es; y=cos(rsir)*cos(ra);
  asc=atan(x/y); if(asc<0) asc+=pi;
         if(x<0) asc+=pi;
  asc=180/pi*(asc+pi/2);
  while(asc>360) asc-=360;

if( floor2(asc/30)!=floor2(oldasc/30) && (int)oldasc!=0) {
    trajanje=(xx1-xo1)*60+xx2-xo2;
//    printf(" - %02d:%02d  (%d:%02d)   %s\n  %02d:%02d",xx1,xx2,(int)floor2(trajanje/60.),(int)floor2(trajanje-floor2(trajanje/60.)*60.),znk[floor2(asc/30)],xx1,xx2);
    xo1=xx1;xo2=xx2;

				

    xmin[dn][(int)floor2(asc/30)]=xx1*60+xx2;



    }

oldasc=asc;
xx2++;
} while(xx2<60);
xx1++;
} while(xx1<24);
}

//trajanje=(23-xo1)*60+59-xo2;
//printf(" - 24:00  (%d:%02d)   %s",(int)floor2(trajanje/60.),(int)floor2(trajanje-floor2(trajanje/60.)*60.),znk[1+floor2(asc/30)]);



CPen olovka[20];
int ii;
for (ii = 0 ; ii <= 17 ; ii++) olovka[ii].CreatePen(PS_SOLID, 2, Colors[ii]);  //tip, debljina, boja

CPen *staraOlovka = pDC->SelectObject(&olovka[0]) ;


int acj,cl;
int ox[13];
double tmp,tmp2;


for(dn=1;dn<=pl[mje_orig-1]+1;dn++) {
   cl=-1;
 for(acj=11;acj>=0;acj--) {
    xmin[dn][acj]=xmin[dn][acj]/24/60*360;
    cl++;  if (cl==6) cl+=2;
	pDC->SelectObject (&olovka[cl]);  // izaberi boju olovke

    
	if(dn==1) { ox[acj]=90+xmin[dn][acj]*1.5 ;
				pDC->MoveTo(ox[acj],dn*10-5);
				continue; }  //prvi dan preskoèi
	
	pDC->MoveTo(ox[acj],dn*10-5);

 
   tmp=xmin[dn][acj]*1.5+90;

   if(fabs(tmp-(float)ox[acj])>450 && tmp<160) {


	    tmp2=dn*10+5-10*(tmp-90)/(tmp-90+630-ox[acj]);
		pDC->LineTo(630,tmp2); 
		
		ox[acj]=90; 
		
		pDC->MoveTo(90,tmp2);

		goto ln8;
   }



   if(fabs(tmp-(float)ox[acj])>450 && tmp>540) {

		tmp2=dn*10+5-10*(630-tmp)/(ox[acj]-90+630-tmp);
		pDC->LineTo(90,tmp2); 

		ox[acj]=630; 
		pDC->MoveTo(630,tmp2);

 }


   ln8:
   ox[acj]=tmp;
   pDC->LineTo(ox[acj],dn*10+5);

/*	if(dn==20 || dn==21) {
	buff.Format("dn=%d newx=%f oldx=%d xmin=%f xminOld=%f",dn, tmp,ox[acj],xmin[dn][acj],xmin[dn-1][acj]);
	AfxMessageBox(buff);

	}
*/


}}


pDC->SelectObject(staraOlovka);  // vrati staru olovku




// iscrtaj znakove

 CDC MemDC;
 MemDC.CreateCompatibleDC( pDC );
 BITMAP bm;

 CBitmap *pOldBitmap;

	 danx=pl[mje_orig-1]+1;


for(ii=0;ii<=11;ii++) {

  pOldBitmap = MemDC.SelectObject( &Slicka[ii] );

  Slicka[ii].GetObject( sizeof( BITMAP ), &bm );


 pDC->BitBlt( xmin[danx][ii]*1.5+83-2, 15+1*6+(danx-1)*10+2+8, bm.bmWidth, bm.bmHeight, &MemDC,0, 0, SRCCOPY );

 MemDC.SelectObject( pOldBitmap );

}


}

//***********************************************************************************
double CAsc2winView::frac(double x)
{
return x-floor(x);
}

//***********************************************************************************
double CAsc2winView::fnmo(double x)
{
return(x-floor(x/360)*360);
}

//***********************************************************************************
void CAsc2winView::Ucitaj_Znake(CDC *pDC)
{
    FILE *fajl;
    int i,j,k,x,y,r,v,vel,cl,cl2,clc=13;
	char bf[90];
    float min;
	int boje_planeta[12]= {16,14,15,11,4,9,12,2,1,13,3,0};

	unsigned int mask[4]= { 0, 0x0000ff, 0x00ff00, 0xff0000};
    
	unsigned int VgaColors16[20]= {         
		0x0000, // Black,0
		0x000f, // blue,1
		0x07f0, // green,2
		0x07ff, // cyan,3
		0xf000, // red,4
		0xf00f, // magenta,5
		0xff00, // yellow,6
		0xffff, // white,7

		0x0007, // dark blue,8
		0x03c0, // dark green,9
		0x03cc, // dark cyan,10
		0x8000, // dark red, 11
		0xc004, // dark magenta, 12
		0x4920, // dark yellow, 13
		0x1111, // dark gray,14
		0x5bb5, // bright gray,15;               
        0xfc00  //orange
	};

	unsigned int VgaColors24[20] = {         
		0x000000, // Black,0
		0x0000ff, // blue,1
		0x00ff00, // green,2
		0x00ffff, // cyan,3
		0xff0000, // red,4
		0xff00ff, // magenta,5
		0xffff00, // yellow,6
		0xffffff, // white,7

		0x000080, // dark blue,8
		0x008000, // dark green,9
		0x008080, // dark cyan,10
		0x800000, // dark red, 11
		0x800080, // dark magenta, 12
		0x808000, // dark yellow, 13
		0x808080, // dark gray,14
		0xc0c0c0, // bright gray,15
		0xff8000  // orange
	}; 
	
	
	unsigned int Colour,Colour2;

	BITMAP bm,bm2;


	fajl=fopen("zvz_znki.dat","rt");
	
	BYTE bmpBuffer[9 * 15 * 4]; // najgori sluèaj, 4 bajta po pixelu
	BYTE bmpBuffer2[9 * 15 * 4]; // najgori sluèaj, 4 bajta po pixelu

 for(i=0; i<=11; i++) {

	Slicka[i].CreateCompatibleBitmap (pDC, 15, 9);
	Slicka[i].GetObject( sizeof( BITMAP ), &bm );

	Slicka[12+i].CreateCompatibleBitmap (pDC, 15, 9);
	Slicka[12+i].GetObject( sizeof( BITMAP ), &bm2 );


     for(y=0; y<=8; y++) {
	  fgets(bf,79,fajl);
	  for (x=0; x<=14; x++) {

		  if (bf[21+x]=='b') cl=clc;  // višebojni znaci
		  else cl=7;
		  if (bf[21+x]=='b') cl2=14;  //jednobojni znaci
		  else cl2=7;

		  if (bm.bmBitsPixel/8==2) Colour = VgaColors16[cl];
			else Colour = VgaColors24[cl];
			
		  if (bm2.bmBitsPixel/8==2) Colour2 = VgaColors16[cl2];
			else Colour2 = VgaColors24[cl2];

		   for (j=1; j<=bm.bmBitsPixel/8; j++) {
			
			   byte p =  (Colour & mask[j]) >> (j-1)*8;
			   bmpBuffer[x * bm.bmBitsPixel/8+j-1 + y*15*bm.bmBitsPixel/8] = p;
		   }

		   for (j=1; j<=bm2.bmBitsPixel/8; j++) {
			
			   byte p2 = (Colour2 & mask[j]) >> (j-1)*8;
			   bmpBuffer2[x * bm2.bmBitsPixel/8+j-1 + y*15*bm2.bmBitsPixel/8] = p2;
		   }

     }}
   clc--; if (clc==7) clc-=2;
   fgets(bf,79,fajl);

	Slicka[i].SetBitmapBits( bm.bmHeight * bm.bmWidthBytes , &bmpBuffer[0]);      // raznobojni znaci
	Slicka[12+i].SetBitmapBits( bm2.bmHeight * bm2.bmWidthBytes , &bmpBuffer2[0]);  //jednobojni znaci

 }

// uèitavanje planeta

for(i=0; i<=11; i++) {

	Slicka[24+i].CreateCompatibleBitmap (pDC, 15, 9);   //višebojni
	Slicka[46+i].CreateCompatibleBitmap (pDC, 15, 9);  //jednobojni

	Slicka[24+i].GetObject( sizeof( BITMAP ), &bm );
	Slicka[46+i].GetObject( sizeof( BITMAP ), &bm );


     for(y=0; y<=8; y++) {
	  fgets(bf,79,fajl);
	  for (x=0; x<=14; x++) {

		  if (bf[21+x]=='b') cl=boje_planeta[i];
		  else cl=7;
		  
		  cl2=cl; if (cl2 != 7) cl2=0;  //jednobojni znaci

		  if (bm.bmBitsPixel/8==2) Colour = VgaColors16[cl];
			else Colour = VgaColors24[cl];

		  if (bm.bmBitsPixel/8==2) Colour2 = VgaColors16[cl2];
			else Colour2 = VgaColors24[cl2];

		   for (j=1; j<=bm.bmBitsPixel/8; j++) {
			
			   byte p = (Colour & mask[j]) >> (j-1)*8;
			   byte p2 = (Colour2 & mask[j]) >> (j-1)*8;

			   bmpBuffer[x * bm.bmBitsPixel/8+j-1 + y*15*bm.bmBitsPixel/8] = p;
			   bmpBuffer2[x * bm.bmBitsPixel/8+j-1 + y*15*bm.bmBitsPixel/8] = p2;
		   }
     }}
    fgets(bf,79,fajl);

	Slicka[24+i].SetBitmapBits( bm.bmHeight * bm.bmWidthBytes , &bmpBuffer[0]);
	Slicka[46+i].SetBitmapBits( bm.bmHeight * bm.bmWidthBytes , &bmpBuffer2[0]);

 }



// uèitavanje ostalih znakiæa

for(i=0; i<=9; i++) {

	Slicka[36+i].CreateCompatibleBitmap (pDC, 15, 9);
	
	Slicka[36+i].GetObject( sizeof( BITMAP ), &bm );


     for(y=0; y<=8; y++) {
	  fgets(bf,79,fajl);
	  for (x=0; x<=14; x++) {

		  if (bf[21+x]=='b') cl= 0; //boje_planeta[i];
		  else cl=7;

		  if (bm.bmBitsPixel/8==2) Colour = VgaColors16[cl];
			else Colour = VgaColors24[cl];

		   for (j=1; j<=bm.bmBitsPixel/8; j++) {
			
			   byte p = (Colour & mask[j]) >> (j-1)*8;
			   bmpBuffer[x * bm.bmBitsPixel/8+j-1 + y*15*bm.bmBitsPixel/8] = p;
		   }
     }}
    fgets(bf,79,fajl);

	Slicka[36+i].SetBitmapBits( bm.bmHeight * bm.bmWidthBytes , &bmpBuffer[0]);

 }




fclose(fajl);

bJesuSlickeLoudane =TRUE;


}

//*********************************************************************************************************************
void CAsc2winView::Odredjivanje_uskrsa()
{
// odreðivanje uskrsa
// odreð.punog mjeseca    

// naði i kinesku novu godinu i dodaj kao praznik

	CString datKNG=Nadji_Kinesku_Godinu(m_dUlazDlg.m_iGod);
	int danKNG, mjeKNG, kinZnk; 
	sscanf(datKNG,"%d %d %d",&danKNG, &mjeKNG, &kinZnk);
	mje_praznik[0]=mjeKNG;
	dan_praznik[0]=danKNG;


    int i,j,s=1,ipumj=0;
    long n;
	float punimj,temppm,tmppm1;
	CString buff;




	 if (m_dUlazDlg.m_iGod > 1582 ){

		 if (m_dUlazDlg.m_iGod<1986) s=-1;

         if(s<0)   n=Razlika_Datuma(1,3,m_dUlazDlg.m_iGod,12,1985)+1;
         else      n=Razlika_Datuma(1,12,1985,3,m_dUlazDlg.m_iGod)+1; //prvi je datum NEDJELJE
	 }
	 else {   //gregorijanski preskok s 4 na 15.10.1582, ali s èetvrtka na petak!

		s=-1; 
		n=Razlika_Datuma(1,3,m_dUlazDlg.m_iGod,9,1582)+30;  //30.9.1582 je navodno bila nedjelja

	 }
 
  
  punimj=360;mje_praznik[1]=3;

  for (i=22; i<=pl[3-1]; i++)   {             // pronaði minimum orb-a opozicije (puni mjesec) za te dane
          Planete(i, 3, m_dUlazDlg.m_iGod,12,0); // (dan, mje, god, sat,min)

          if(fabs(180-fabs(objx[1]-objx[2]))<punimj) {punimj=fabs(180-fabs(objx[1]-objx[2]));ipumj=i;}
       }

	if (punimj>13) {                      // ako je orb veæi od 13 stupnjeva, uskrs je u travnju, naði dan
		mje_praznik[1]=4;
        for (i=1;i<=pl[4-1]; i++) {

                Planete(i, 4, m_dUlazDlg.m_iGod,12,0);

                temppm=fabs(180-fabs(objx[1]-objx[2]));

                if(temppm<punimj) {punimj=temppm;ipumj=i;}
                if(temppm<6) break;
                if(temppm-punimj>0 && punimj<13) break;

        }
	}


	//buff.Format(" %d %f %d",mje_praznik[1],punimj,ipumj);
	//MessageBox(buff);


if (/* m_dUlazDlg.m_iMje==3 && */ mje_praznik[1]==3) {
  if(ipumj<31) for(i=ipumj+1;i<=pl[3-1]; i++) if(fmod2(n+s*(i-1),7)==1) {dan_praznik[1]=i+1; goto skipuskrs;}

   mje_praznik[1]=4;
   for(i=1;i<=pl[4-1]; i++) if(fmod2(n+s*31+s*(i-1),7)==1) {dan_praznik[1]=i+1; break;}
}
else {
  if(ipumj<31) for(i=ipumj+1;i<=pl[4-1]; i++) if(fmod2(n+s*(31+i-1),7)==1) {dan_praznik[1]=i+1; break;}
}  
   /*
   tmppm1=23+dan_praznik[1];
   if(tmppm1>=ipumj) dan_praznik[1]=32;
   if(tmppm1>=ipumj && tmppm1==31) dan_praznik[1]=1;
	*/


/*
//skipmj3:
//if(m_dUlazDlg.m_iMje==3 && mje_praznik[1]==4) goto skipuskrs;

if( m_dUlazDlg.m_iMje==4 &&  mje_praznik[1]==4) {
   if(ipumj==31) goto skipmj4;
   for(i=ipumj+1;i<=pl[4-1]; i++)  if(fmod(n+31+s*(i-1),7)==1) {dan_praznik[1]=i+1; goto skipuskrs;}
}
*/

skipuskrs:
	//buff.Format(" %d  %d",mje_praznik[1],dan_praznik[1]);
	//MessageBox(buff);
if (dan_praznik[1]==32) { //ako je našao 31.+1
	dan_praznik[1]=1;
	mje_praznik[1]++;
}
//tjelovo: 59 dana iza uskrsa
dan_praznik[2]=dan_praznik[1];
mje_praznik[2]=mje_praznik[1];

for (i=1; i<=59; i++) {
	
	dan_praznik[2]++;
	if (dan_praznik[2] > pl[mje_praznik[2]-1 ] ) {

		dan_praznik[2]=1;
		mje_praznik[2]++;

	}
}

//duhovi: 48 dana iza uskrsa
dan_praznik[3]=dan_praznik[1];
mje_praznik[3]=mje_praznik[1];

for (i=1; i<=48; i++) {
	
	dan_praznik[3]++;
	if (dan_praznik[3] > pl[mje_praznik[3]-1 ] ) {

		dan_praznik[3]=1;
		mje_praznik[3]++;

	}
}


//fašnik: 48 dana prije uskrsa
dan_praznik[4]=dan_praznik[1];
mje_praznik[4]=mje_praznik[1];

//if (fmod2(x,4)==0 ) if ( fmod2(x,100)!= 0 || fmod2(x,400)==0 || x<1600) d++; 

for (i=1; i<=48; i++) {
	
	dan_praznik[4]--;
	if (dan_praznik[4] <1  ) {

		mje_praznik[4]--;
		dan_praznik[4]= pl[mje_praznik[4]-1 ]  ;

		//prstupne godine:
		if (     mje_praznik[4]==2    //ako je veljaèa
			 &&  fmod2( m_dUlazDlg.m_iGod,4)==0  
			 && ( fmod2(m_dUlazDlg.m_iGod,100)!= 0 || fmod2(m_dUlazDlg.m_iGod,400)==0 || m_dUlazDlg.m_iGod<1600)
			) dan_praznik[4]++;

	}
}




return;
}



//*********************************************************************************************************************
void CAsc2winView::Odredjivanje_dst()
{
// odreðivanje dst-a po eu-pravilu (zadnja nedjelja u 3. i zadnja nedjelja u 10)
	
	dan_praznik[5]=32;
	dan_praznik[6]=32;

    if ( m_dUlazDlg.m_iGod < 1983 ) return ;//za jugu


	int i,j,s=1;
    long n;



		 if (m_dUlazDlg.m_iGod<1986) s=-1;

         if(s<0)   n=Razlika_Datuma(1,3,m_dUlazDlg.m_iGod,12,1985);
         else      n=Razlika_Datuma(1,12,1985,3,m_dUlazDlg.m_iGod); //prvi je datum NEDJELJE

 
     for(i=pl[3-1]; i>=1; i--) if(fmod2(n+s*(i-1),7)==0) {dan_praznik[5]=i; break;}





         if(s<0)   n=Razlika_Datuma(1,10,m_dUlazDlg.m_iGod,12,1985);
         else      n=Razlika_Datuma(1,12,1985,10,m_dUlazDlg.m_iGod); //prvi je datum NEDJELJE



	 for(i=pl[10-1]; i>=1; i--) if(fmod2(n+s*(i-1),7)==0) {dan_praznik[6]=i; break;}


return;
}




//*****************************************************************************************************************
void CAsc2winView::Planete(int dan, int mje, int god, int sat, int min)

{
 double D[13],X[13],Y[13],Z[13],K[13],C[13],M[13];
 int br=0,rvs,ix; //brojac podataka
 double a,b,d,e,f,g,g1,ii,l,ll,l1,m,n,r,s,s1,s2,t,u,v,x,y,z,au,ea,fl,k1,qq=3,tn,td,ml,jd,Gvr;
 int i,j;

 double gmt;

 gmt = m_dUlazDlg.m_iGmt;

Gvr=(double)(sat-gmt+min/60.); //Globalno vrijeme

jd=367*(double)god-floor2(7.*((double)god+floor2(((double)mje+9)/12.))/4.);
jd+=floor2(275.*mje/9.)+(double)dan+1721013.5;
jd=jd-.5*sgn(100.*god+(double)mje-190002.5)+.5;

ix=18;
do {
if( (god<ix*100 || (god==ix*100 && mje<3)) && fmod2(ix*100,400)!=0) jd++;
else break;
ix--;
} while(1);
if(god<1582 || (god==1582 && mje<10) ||(god==1582 && mje==10 && dan<15)) jd+=10;

t=td=((jd-2415020)+Gvr/24)/36525.;

for(i=1;i<=9;i++) {
   readdata(&br,&s);readdata(&br,&s1);readdata(&br,&s2); s+=s1*td+s2*td*td;
  m=pi/180*fnmo(s);
   readdata(&br,&s);readdata(&br,&s1);readdata(&br,&s2); s+=s1*td+s2*td*td;
  e=s; ea=m;
  for(a=1;a<=5;a++) ea=m+e*sin(ea);
   readdata(&br,&au); Z[i]=pi/180*au*(1-e*cos(ea));
  x=au*(cos(ea)-e); y=au*sin(ea)*sqrt(1-e*e); ln1060(x,&y,&r,&a);
   readdata(&br,&s);readdata(&br,&s1);readdata(&br,&s2); s+=s1*td+s2*td*td;
  a=s+a*180/pi;
   readdata(&br,&s);readdata(&br,&s1);readdata(&br,&s2); s+=s1*td+s2*td*td;
  v=fnmo(a+s); m=pi/180*s; b=pi/180*v;
   readdata(&br,&s);readdata(&br,&s1);readdata(&br,&s2); s+=s1*td+s2*td*td;
  ii=pi/180*s; a=atan(cos(ii)*tan(b-m)); if(a<0) a+=pi;
  a=180/pi*(a+m); if(fabs(v-a)>10) a-=180;
  a=fnmo(a); b=pi/180*a; b=180/pi*(atan(sin(b-m)*tan(ii)));
  C[i]=a; D[i]=b; if(D[i]>20) D[i]-=360;
} //next i

u=9;

ln460:   fl=1; if(s==1) fl=0;
for(i=1; i<=u; i++) K[i]=C[i];
for(i=1; i<=u-1; i++) {
  for(j=i+1; j<=u; j++) {
      if(K[j]>=K[i]) continue;
      k1=K[i]; K[i]=K[j]; K[j]=k1;
  }
}
a=1;
for(i=1;i<=u;i++) {
  for(j=1;j<=u;j++) if(K[(int)a]==C[j]) break;

	 rvs=0; if(qq==1 && Z[j]==1) {Z[j]=pi; rvs=1;}
	 z=K[(int)a]; K[(int)a]=pi; if(fl==1) goto ln540;
	  objx[j+1]=z; objy[j+1]=rvs;
ln540:   a++;
} //next i;

u=11; if(qq==1) goto ln1600;

for(i=1;i<=9;i++) {
	C[i]*=pi/180; D[i]*=pi/180; if(i==1) {M[1]=Z[i]; goto ln600;}
	x=( (sqrt(Z[i])+sqrt(M[1]))*(sqrt(M[1])*sqrt(Z[i])) )/(pow(Z[i],1.5)+pow(M[1],1.5));
	x-=cos(C[1]-C[i]); if(x<0) {M[i]=1; goto ln600;}
	M[i]=0;
ln600:  X[i]=Z[i]*cos(D[i])*cos(C[i]); Y[i]=Z[i]*cos(D[i])*sin(C[i]);
	Z[i]*=sin(D[i]);
}

for(i=2;i<=9;i++) { X[i]-=X[1]; Y[i]-=Y[1]; Z[i]-=Z[1]; y=Y[i]; x=X[i];
		    ln1060(x,&y,&r,&a); g=a; y=Z[i]; x=r; ln1060(x,&y,&r,&a);
		    C[i]=180/pi*g; Z[i]=M[i]; M[i]=pi; D[i]=pi;
		  }
C[1]=fnmo(180/pi*C[1]+180);

ll=973563+1732564379*t-4*t*t;
g=1012395+6189*t; n=933060-6962911*t+7.5*t*t;
g1=1203586+14648523*t-37*t*t; d=1262655+1602961611*t-5*t*t; m=3600;

l=(ll-g1)/m; l1=((ll-d)-g)/m; f=(ll-n)/m; d/=m; y=2*d;

ml=22639.6*fns(l)-4586.4*fns(l-y)+2369.9*fns(y)+769*fns(2*l)-669*fns(l1);
ml=ml-411.6*fns(2*f)-212*fns(2*l-y)-206*fns(l+l1-y)+192*fns(l+y);
ml=ml-165*fns(l1-y)+148*fns(l-l1)-125*fns(d)-110*fns(l+l1)-55*fns(2*f-y);
tn=n+5392*fns(2*f-y)-541*fns(l1)-442*fns(y)+423*fns(2*f)-291*fns(2*l-2*f);
tn+=221*fns(l1+2*f-y)-176*fns(l-y)+147*fns(l-2*f)+118*fns(l);

C[10]=fnp(ll+ml,m); C[11]=fnmo(fnp(tn,m));
qq=1; s=1; goto ln460;

ln1600:
objx[1]=objx[2]; objy[1]=objy[2];
objx[2]=objx[11]; objy[2]=objy[11];
for(i=1;i<=12;i++) objx[i]=fnmo(objx[i]);
return;}
//********************************************************************************************************************

double CAsc2winView::sgn(double x)
{
 if(x<0) return (-1);
 if(x==0) return (0);
 return (1);
}
//********************************************************************************************************************

double CAsc2winView::fnp(double x, double mm)
{
return( sgn(x)* ( (fabs(x)/mm)/360.-floor2((fabs(x)/mm)/360))*360);
}

//********************************************************************************************************************

double CAsc2winView::fns(double x)
{
return(sin(x*pi/180)); 
}

//********************************************************************************************************************


void CAsc2winView::ln1060(double xx, double *yy, double *rr, double *aa)
{
if(*yy==0) *yy=1.745e-9;
*rr=sqrt(xx*xx+(*yy)*(*yy)); *aa=atan(*yy/xx); if(*aa<0) (*aa)+=pi; if(*yy<0) (*aa)+=pi;

}

//********************************************************************************************************************

void CAsc2winView::readdata(int *i1, double *f1)
{
static  double data[]={
358.4748445, 35999.04975, -.150278e-3, .01675104, -.418e-4, -.126e-6,
1.00000023, 101.2208333, 1.719175, .000452778, 0, 0, 0, 0, 0, 0, 102.2793806,
149472.5152, .6389e-5, .20561421, .2046e-4, -.3e-7, .3870984 ,28.75375278,
.370280556, .000120833, 47.14594444, 1.185208333, .000173889, 7.002880556,
.001860833, -.18333e-4, 212.6032194, 58517.80386, .001286056, .00682069,
-.4774e-4, .91e-7, .7233316, 54.38418611, .508186111, -.1386389e-2,
75.77964722, .89985, .41e-3, 3.393630556, .1005833e-2, -.972e-6, 319.529425,
19139.8585, .180806e-3, .0933129, .92064e-4, -.77e-7, 1.5236915,
285.4317611, 1.069766667, .13125e-3, 48.78644167, .770991667, -1.389e-5,
1.850333333, -.675e-3, .12611e-4, 225.4928135, 3033.687936, 0,
.04838144, -.155e-4, 0,
5.20290493, 273.3930152, 1.33834464, 0, 99.41984827, 1.05829152,
0, 1.3096585, -.515613e-2, 0, 174.215296, 1223.507963,
0, .05422831, -.20495e-3, 0, 9.55251745, 338.911673,
-.31667941, 0, 112.8261394, .82587569, 0, 2.49080547,
-.00466035, 0, 74.17574887, 427.2742717, 0, .04681664,
.0041875, 0, 19.22150505, 95.68630387, 2.05082548, 0,
73.52220082, .52415598, 0, .77256652, .12824e-3, 0,
30.1329437, 240.4551595, 0, .00912805, -.00127185, 0,
30.1137593, 284.168255, -21.6328615, 0, 130.6841531, 1.10046492,
0, 1.77939281, -.00975088, 0, 229.7810007, 145.1781092, 0, .24797376,
.00289875, 0, 39.53903455, 113.5365761, .20863761, 0, 108.94405,
1.37395444, 0, 17.15140319, -.01611824, 0 };

*f1=data[*i1]; (*i1)++;
}

//********************************************************************************************************************

void CAsc2winView::Iscrtaj_Planete(CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt)
{
int i,j, vri, danx,mjex,godx,satx,minx;
double duz, sir;
double xmin[33][13];
int mje_orig = mje;
CString buff;

vri = m_dUlazDlg.m_iVri;


CPen olovka[20];
for (i = 0 ; i <= 17 ; i++) olovka[i].CreatePen(PS_SOLID, 2, Colors[i]);  //tip, debljina, boja

CPen *staraOlovka = pDC->SelectObject(&olovka[0]) ;

int acj;
int ox[13];
double tmp,tmp2;

int boje_planeta[12]= {16,14,15,11,4,9,12,2,1,13,3,0};


for(i=1;i<=pl[mje_orig-1]+1;i++) {
 
    danx=i;
	mjex=mje; 
	godx=god; 
	satx=0-vri; 
	minx=0;

     if (danx>pl[mje_orig-1]) {
 	   danx=1; mjex=mjex+1;
 	   if (mjex==13) {mjex =1; godx =godx +1; }
	 }


	if(satx<0) {
		satx+=24;
		danx-=1;}

	if(danx<1) {
		mjex-=1;
		if(mjex<1) {
			mjex=12;
			godx-=1;	}	
		danx=pl[mjex-1];	
	}

	if (mjex==10 && godx ==1582 && i>5 && i<16) {
		for(j=9;j>=0;j--) {
		pDC->SelectObject (&olovka[boje_planeta[j]]);  // izaberi boju olovke
		pDC->MoveTo(ox[j],i*10-5);
		pDC->LineTo(ox[j],i*10+5);
		}
		continue;
	}

	Planete(danx,mjex,godx,satx,minx);
                
	for(j=9;j>=0;j--) {

		pDC->SelectObject (&olovka[boje_planeta[j]]);  // izaberi boju olovke


		if(i==1) { ox[j]=90+objx[j+1]*1.5 ;continue; }
			pDC->MoveTo(ox[j],i*10-5);
			tmp=objx[j+1]*1.5+90;

		if(fabs(tmp-(float)ox[j])>450 && tmp<160) {
			tmp2=i*10+5-10*(tmp-90)/(tmp-90+630-ox[j]);
			pDC->LineTo(630,tmp2); ox[j]=90; 
			pDC->MoveTo(90,tmp2);goto ln8;}

		if(fabs(tmp-(float)ox[j])>450 && tmp>540) {
			tmp2=i*10+5-10*(630-tmp)/(ox[j]-90+630-tmp);
			pDC->LineTo(90,tmp2); ox[j]=630; pDC->MoveTo(630,tmp2);}

		ln8:
		ox[j]=tmp;
		pDC->LineTo(ox[j],i*10+5);

}}


// iscrtaj ikone planeta
 
 CDC MemDC;
 MemDC.CreateCompatibleDC( pDC );
 BITMAP bm;
 CBitmap *pOldBitmap;
 int k,r;


danx=pl[mje_orig-1];

for(i=1;i<=10;i++) {

min=1000;

  for(j=1;j<=10;j++) {
    if(objx[j]<min) { min=objx[j]; k=j; }
  }
 objx[40+i]=min; objx[60+i]=k; objx[k]=1000;
}
r=0;
for(i=1;i<=10;i++) {
  if(i==1) goto ln12;

if(fabs(objx[40+i]-objx[40+i-1])<15) { r++; if(r==2) r=0; }
else r=0;
ln12:

	pOldBitmap = MemDC.SelectObject( &Slicka[(int)(24+objx[60+i]-1)] );
	Slicka[(int)(24+objx[60+i]-1)].GetObject( sizeof( BITMAP ), &bm );

 	pDC->BitBlt(objx[40+i]*1.5+83, 15+r*12+danx*10+2, bm.bmWidth, bm.bmHeight, &MemDC,0, 0, SRCCOPY );

	MemDC.SelectObject( pOldBitmap );

}



pDC->SelectObject(staraOlovka);  // vrati staru olovku


}


//***********************************************************************************************
void CAsc2winView::Iscrtaj_Lunu(CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt)
{

const float P=29.530590278;
char buff[40];
float faza,radius;
int k1=270,k2=90,k,s=1;

long int n;

 
  if (god<1985) s=-1;
  if (god==1985 && mje<4 ) s=-1;
  if (god==1985 && mje==4 && dan<5) s=-1;

if(s<0)   n=Razlika_Datuma(dan,mje,god,4,1985)+4;
else      n=Razlika_Datuma(5,4,1985,mje,god)+dan; //prvi je datum USTAPA


faza=fmod(n,P)/P;    if (s<0) faza=1-faza;
radius=288*cos(2*pi*faza);
if (faza*360<180)   { k=k1;k1=k2;k2=k; }

	pDC->SetMapMode (MM_ISOTROPIC);
	CRect rect;
	GetClientRect(rect);


if (pDC->IsPrinting() ==FALSE) {

	pDC->SetWindowExt( 600* m_dUlazDlg.m_iHorRez ,600);   // logièke koordinate
	pDC->SetViewportExt(rect.Width(),rect.Width()/m_dUlazDlg.m_iHorRez);  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike
																		  // bilo *700/1000
	
	pDC->SetViewportOrg (1000/20,700/20);  // pomakni poèetak k.s-a na dolje
}
else {

	pDC->SetWindowExt( 600* m_dUlazDlg.m_iHorRez ,600);       // logièke koordinate
	pDC->SetViewportExt(5500, 3850);  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike
    pDC->SetViewportOrg (050,100);        // pomakni poèetak k.s-a na dolje
}




CPen olovka(PS_SOLID, 2, Colors[0]);                      //tip, debljina, boja
CPen* staraOlovka = pDC->SelectObject(&olovka);           // pohrani staru olovku

CBrush* brush;
CBrush* oldBrush;
brush = new CBrush(Colors[0]);
oldBrush = pDC->SelectObject(brush);// Select the new brush into the DC.


if(radius<0)   TrueEllipse(pDC,370,200,fabs(radius)/1.5,280/1.5,k1,k2);
else           TrueEllipse(pDC,370,200,fabs(radius)/1.5,280/1.5,k2,k1);

TrueEllipse(pDC,370,200,288/1.5,280/1.5,k1,k2);


pDC->FloodFill(1,1,Colors[0]);


pDC->SelectObject(oldBrush);
delete brush;

pDC->SelectObject(staraOlovka);  // vrati staru olovku

}



//************************************************************************************************

void CAsc2winView::Ellipse(CDC *pDC, double x0,double y0, double a, double b, double fi1, double fi2)
{

	double X1,Y1,X2,Y2,Xstart,Ystart,Xend,Yend;
	double alfa;
	
	X1 = x0 - a;
	Y1 = y0 - b;

    X2 = x0 + a;
	Y2 = y0 + b;


	alfa = fi1 / 180 * pi; 

	Xstart = x0 +  a * cos(alfa);
	Ystart = y0 +  b * sin(alfa);


	alfa = fi2 / 180 * pi; 

	Xend = x0 + a * cos(alfa);
	Yend = y0 + b * sin(alfa);


	pDC->Arc(X1,Y1,X2,Y2,Xstart,Ystart,Xend,Yend);


}

//************************************************************************************************
void CAsc2winView::OnUpdateIndicatorTS(CCmdUI *pCmdUI)
{

    char *mjesec[12]={"Sij.","Velj.","Ozuj.","Trav.","Svib.","Lip.","Srp.","Kol.","Ruj.","List.","Stud.","Pros."};
    char *danTj[7]={"Pon.","Uto.","Sri.","Cet.","Pet.","Sub.","Ned."};
	CString buff;

		int s=1,n,nedjelja;

		if (m_dUlazDlg.m_iGod > 1582 || (m_dUlazDlg.m_iGod == 1582 && m_dUlazDlg.m_iMje>=10) ){

			if (m_dUlazDlg.m_iGod<1985) s=-1;
			else 
				if (m_dUlazDlg.m_iGod==1985 && m_dUlazDlg.m_iMje<12 ) s=-1;
				else
					if (m_dUlazDlg.m_iGod==1985 && m_dUlazDlg.m_iMje==12 && m_dUlazDlg.m_iDan<30) s=-1;


			if(s<0)   n=Razlika_Datuma(m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje, m_dUlazDlg.m_iGod,12,1985)+0;
			else      n=Razlika_Datuma(1,12,1985,m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod)+m_dUlazDlg.m_iDan; //prvi je datum NEDJELJE
		}
		else {   //gregorijanski preskok s 4 na 15.10.1582, ali s èetvrtka na petak!

			s=-1; 
			n=Razlika_Datuma(m_dUlazDlg.m_iDan,m_dUlazDlg.m_iMje,m_dUlazDlg.m_iGod,9,1582)+30-1;   //30.9.1582 je navodno bila nedjelja
		}


		if (s>0) {
			nedjelja=fmod2(n,7); 
			nedjelja=fmod2(nedjelja+7-2,7);  //korekcija da je ponedjeljak=0, za potrebe ove funkcije
		}
		else   {
			nedjelja=6-fmod2(n,7);
		}




	buff.Format(" %s %02d. %s %04d, %02d:%02d ", danTj[nedjelja],  m_dUlazDlg.m_iDan,mjesec[m_dUlazDlg.m_iMje-1],m_dUlazDlg.m_iGod,
											m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin);
									



	pCmdUI->Enable(TRUE);

// Set the text of the status bar pane
// to the current color

	pCmdUI->SetText(buff);

}


//************************************************************************************************
void CAsc2winView::OnUpdateIndicatorLOK(CCmdUI *pCmdUI)
{
	CString buff;

	buff.Format(" %26s",m_dUlazDlg.m_sGrad);

	pCmdUI->Enable(TRUE);

// Set the text of the status bar pane
// to the current color

	pCmdUI->SetText(buff);

}

//************************************************************************************************
void CAsc2winView::OnUpdateIndicatorDST(CCmdUI *pCmdUI)
{
	CString buff;

	if (m_dUlazDlg.m_iVri == 1) buff =" DST: da";
	else buff = " DST: ne";


	pCmdUI->Enable(TRUE);

// Set the text of the status bar pane
// to the current color

	pCmdUI->SetText(buff);


}

//************************************************************************************************
void CAsc2winView::OnDanPrije() 
{
	// TODO: Add your command handler code here

	m_dUlazDlg.m_iDan-=1;

	if(m_dUlazDlg.m_iDan<1) {

		m_dUlazDlg.m_iMje-=1;

		if(m_dUlazDlg.m_iMje<1) {

			m_dUlazDlg.m_iMje=12;
			m_dUlazDlg.m_iGod-=1;	}
		
		m_dUlazDlg.m_iDan=pl[m_dUlazDlg.m_iMje-1];	

	}

Invalidate();              // ponovo nacrtaj 
}

//************************************************************************************************

void CAsc2winView::OnDanPoslije() 
{
	// TODO: Add your command handler code here
	m_dUlazDlg.m_iDan+=1;

	if(m_dUlazDlg.m_iDan>pl[m_dUlazDlg.m_iMje-1]) {

		m_dUlazDlg.m_iMje+=1;

		if(m_dUlazDlg.m_iMje>12) {

			m_dUlazDlg.m_iMje=1;
			m_dUlazDlg.m_iGod+=1;	}
		
		m_dUlazDlg.m_iDan=1;	

	}
	
Invalidate();              // ponovo nacrtaj 
}


//************************************************************************************************
void CAsc2winView::OnMjePrije() 
{

		m_dUlazDlg.m_iMje-=1;

		if(m_dUlazDlg.m_iMje<1) {

			m_dUlazDlg.m_iMje=12;
			m_dUlazDlg.m_iGod-=1;	}
		
		m_dUlazDlg.m_iDan=__min(m_dUlazDlg.m_iDan, pl[m_dUlazDlg.m_iMje-1]);

	
	
Invalidate();              // ponovo nacrtaj 
}


//************************************************************************************************
void CAsc2winView::OnMjePoslije() 
{
	// TODO: Add your command handler code here

		m_dUlazDlg.m_iMje+=1;

		if(m_dUlazDlg.m_iMje>12) {

			m_dUlazDlg.m_iMje=1;
			m_dUlazDlg.m_iGod+=1;	}
		
		m_dUlazDlg.m_iDan=__min(m_dUlazDlg.m_iDan, pl[m_dUlazDlg.m_iMje-1]);
	
Invalidate();              // ponovo nacrtaj 
}


//************************************************************************************************
void CAsc2winView::OnGodPrije() 
{
	// TODO: Add your command handler code here
			m_dUlazDlg.m_iGod-=1;	
	
Invalidate();              // ponovo nacrtaj 
}


//************************************************************************************************
void CAsc2winView::OnGodPoslije() 
{
	// TODO: Add your command handler code here
			m_dUlazDlg.m_iGod+=1;	
	
Invalidate();              // ponovo nacrtaj 
}


//************************************************************************************************
void CAsc2winView::OnMinPrije() 
{
	// TODO: Add your command handler code here

m_dUlazDlg.m_iMin-=m_iKorakMinute;	

if (m_dUlazDlg.m_iMin <0) {
	
	m_dUlazDlg.m_iMin =60 + m_dUlazDlg.m_iMin;
	m_dUlazDlg.m_iSat -=1;

	if (m_dUlazDlg.m_iSat <0) {

		m_dUlazDlg.m_iSat =23;
		m_dUlazDlg.m_iDan-=1;

		if(m_dUlazDlg.m_iDan<1) {

			m_dUlazDlg.m_iMje-=1;

			if(m_dUlazDlg.m_iMje<1) {

				m_dUlazDlg.m_iMje=12;
				m_dUlazDlg.m_iGod-=1;	
			}
		
		m_dUlazDlg.m_iDan=pl[m_dUlazDlg.m_iMje-1];	

		}
	}

}

Invalidate();              // ponovo nacrtaj 
}


//************************************************************************************************
void CAsc2winView::OnMinPoslije() 
{
	// TODO: Add your command handler code here

m_dUlazDlg.m_iMin+=m_iKorakMinute;	

if (m_dUlazDlg.m_iMin >59) {
	
	m_dUlazDlg.m_iMin =m_dUlazDlg.m_iMin-60;
	m_dUlazDlg.m_iSat +=1;

	if (m_dUlazDlg.m_iSat >23) {

		m_dUlazDlg.m_iSat =0;
		m_dUlazDlg.m_iDan+=1;

		if(m_dUlazDlg.m_iDan>pl[m_dUlazDlg.m_iMje-1]) {

			m_dUlazDlg.m_iMje+=1;

			if(m_dUlazDlg.m_iMje>12) {

				m_dUlazDlg.m_iMje=1;
				m_dUlazDlg.m_iGod+=1;	
			}
		
		m_dUlazDlg.m_iDan=1;	

}}}

Invalidate();              // ponovo nacrtaj 
}


//************************************************************************************************
void CAsc2winView::OnSatPrije() 
{
	// TODO: Add your command handler code here
m_dUlazDlg.m_iSat-=m_iKorakSati;

	if (m_dUlazDlg.m_iSat <0) {

		m_dUlazDlg.m_iSat =24 + m_dUlazDlg.m_iSat;
		m_dUlazDlg.m_iDan-=1;

		if(m_dUlazDlg.m_iDan<1) {

			m_dUlazDlg.m_iMje-=1;

			if(m_dUlazDlg.m_iMje<1) {

				m_dUlazDlg.m_iMje=12;
				m_dUlazDlg.m_iGod-=1;	
			}
		
		m_dUlazDlg.m_iDan=pl[m_dUlazDlg.m_iMje-1];	

		}
	}


Invalidate();              // ponovo nacrtaj 
}


//************************************************************************************************
void CAsc2winView::OnSatPoslije() 
{
	// TODO: Add your command handler code here
m_dUlazDlg.m_iSat+=m_iKorakSati;

if (m_dUlazDlg.m_iSat >23) {
	m_dUlazDlg.m_iSat =m_dUlazDlg.m_iSat-24;
		m_dUlazDlg.m_iDan+=1;

		if(m_dUlazDlg.m_iDan>pl[m_dUlazDlg.m_iMje-1]) {

			m_dUlazDlg.m_iMje+=1;

			if(m_dUlazDlg.m_iMje>12) {

				m_dUlazDlg.m_iMje=1;
				m_dUlazDlg.m_iGod+=1;	
			}
		
		m_dUlazDlg.m_iDan=1;	

}}

Invalidate();              // ponovo nacrtaj 
}
//************************************************************************************************
void CAsc2winView::Ucitaj_Znacajne_Datume()
{
    FILE *fajl;
    int i=0,j;
	char c, bf[150], bf2[16];
	CString bff;


	//stari popis imendana
	int tmpPl1;

	fajl=fopen("zvz_imdn.dat","rt");

    if (fajl==NULL) MessageBox("nema fajla s imendanima ZVZ_IMDN.DAT..");
    
	else {
		tmpPl1=pl[1]; pl[1]=29;

		for(i=1;i<=12;i++) {
			
			fgets(bf,145,fajl);

			for(j=1;j<= pl[i-1];j++) {
				
				fgets(bf,145,fajl); bf[strlen(bf)-1]=0;

				saImendani.Add(bf);

			}
		}
			
		pl[1]=tmpPl1;

		fclose(fajl);

    }
	i=0;



	fajl=fopen("datumi.dat","rt");
	if (fajl==NULL) {
		MessageBox("nema fajla datumi.dat"); 
		return;
	}

	do {
           fgets(bf,150,fajl); 	if(feof(fajl)) break;

            sscanf(bf,"%c",&c);	

			if (c=='P') {boja_praznik[i] =11; br_praznika++; }     // praznici
			else if (c=='K') boja_praznik[i] =14; // katolièki javni imendani i zn. datumi
			else if (c=='R') boja_praznik[i] =9;  // roðendani
			else if (c=='<') break;
			else boja_praznik[i] =15;             // ostali znaè. priv. datumi

			for(j=0; j<=14; j++) {
            sscanf(&bf[2+j],"%c",&bf2[j]);
			}
			bf2[15] =0;
			saImenaPraznika.Add(bf2);

			sscanf(&bf[17],"%d.%d.",&dan_praznik[i],&mje_praznik[i]);
			
			//MessageBox(bf);

			//bff.Format("%d: %d.%d.", i,dan_praznik[i],mje_praznik[i]);
			//MessageBox(bff);

			
		i++;
		if(feof(fajl)) {fclose(fajl);break;}


        } while (i<=366);
  
fclose(fajl);

}


//**********************************************************************************************************************
void CAsc2winView::Iscrtaj_Nebo(CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt)
{
double be,ls,lm,pm,km,am,ms,m1,m2,m3,bm;
double Gvr,k=0,ta,e,rsir,j,x,y,ra;

double duz, sir;
char ime[40];
int i,vri;
float as=0;
CString buff2;


duz = m_dUlazDlg.m_fDuz;
sir = m_dUlazDlg.m_fSir;
vri = m_dUlazDlg.m_iVri;


//	pDC->SetMapMode (MM_TEXT);
	pDC->SetMapMode (MM_ISOTROPIC);

	CRect rect;
	GetClientRect(rect);


if (pDC->IsPrinting() ==FALSE) {

	pDC->SetWindowExt( 600* m_dUlazDlg.m_iHorRez ,600);   // logièke koordinate


	//pDC->SetViewportExt(700, 525);  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike

	pDC->SetViewportExt(rect.Height()*.9* m_dUlazDlg.m_iHorRez,rect.Height()*.9); // bilo 700/525

	
	pDC->SetViewportOrg (220,700/140);  // pomakni poèetak k.s-a na dolje

}
else {

	pDC->SetWindowExt( 600* m_dUlazDlg.m_iHorRez,600);       // logièke koordinate
	
	pDC->SetViewportExt(5600, 4200);  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike

    pDC->SetViewportOrg (150,100);        // pomakni poèetak k.s-a na dolje
}


	CPen olovka0(PS_SOLID, 1, Colors[14]);                 //tip, debljina, boja
	CPen* staraOlovka = pDC->SelectObject(&olovka0);           // pohrani staru olovku

   CBrush* brush;
	brush = new CBrush(Colors[15]); //tip, boja
	CBrush* oldBrush = pDC->SelectObject(brush);// Select the new brush into the DC.

	pDC->FloodFill(2,2,Colors[0]); 

	delete brush;
	brush = new CBrush(Colors[0]); //tip, boja
					pDC->SelectObject(brush);

   //pDC->Rectangle(0,0,640,640);                     // bilo do       639,347
	//Ellipse(pDC, 320,320,320,320,0,360);        // bilo radiusi  174, 1.32*173
	pDC->Ellipse(0,0,640,640);

	delete brush;
	brush = new CBrush(Colors[7]); //tip, boja
					pDC->SelectObject(brush);


	CPen olovka(PS_SOLID, 1, Colors[7]);                 //tip, debljina, boja
	pDC->SelectObject(&olovka);          


// za restore pozadine popupa nakon hide popupa
	static boolean bInit = TRUE;   // samo jedamput inicijalizira

   if( bInit) dcPozadina.CreateCompatibleDC(pDC);
 //dcPozadina.SetMapMode(pDC->GetMapMode());  //sjebala bi 0 kordinate
	if (bInit) bmCap.CreateCompatibleBitmap (pDC, 300,50);  // ako još nije kreiran
	if (bInit) bmOld =  dcPozadina.SelectObject(&bmCap);

  bInit = FALSE;


  
  CFont font1;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 24;                      // request a 12-pixel-height font
	strcpy(lf.lfFaceName, "Arial");        // request a face name "Arial"
	VERIFY(font1.CreateFontIndirect(&lf));  // create the font

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(Colors[14]);

	CFont* def_font = pDC->SelectObject(&font1);
	pDC->TextOut(310, -10, "N", 1);
	pDC->TextOut(310, 624, "S", 1);
	pDC->TextOut(-22, 310, "W", 1);
	pDC->TextOut(642, 310, "E", 1);

	pDC->SelectObject(def_font);
	font1.DeleteObject();

/* 
// font ni netreba zasad
	CFont font2;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 10;                      // request a 12-pixel-height font
	strcpy(lf.lfFaceName, "Times New Roman");   // request a face name "Arial"
	VERIFY(font2.CreateFontIndirect(&lf));  // create the font
	pDC->SelectObject(&font2);
*/

Gvr=(double)(sat-vri-gmt+min/60.); //Globalno vrijeme
if(Gvr<0) { Gvr+=24;dan-=1;
			if (dan<1) {
				mje -=1; 
				if (mje<1) { mje=12; god-=1; } 
				dan=pl[mje-1];
			}
}

i=-(mje<3);

//buff2.Format("%d",i);
//MessageBox(buff2);

k=dan+floor2((mje*153.-i*11.-162)/5.)+floor2((god*1461.+i)/4.)-(god>=0)*366.;
if (k>577736) k-=floor2((floor2((god+i)/100.)*3-5)/4.);

ta=k-693596; tg=k-711858+Gvr/24.;                   // tg je GLOBAL
j=ta/36525. ; i=ta/36525.;
e=23.452294-j*0.013125-j*j*1.639e-6+j*j*j*5.028e-7;
es=sin(e*pi/180); ec=cos(e*pi/180);                // GLOBAL

ar=Gvr*360.985647/24+frac(ta/1461)*1440.02509;      // GLOBAL
ar+=floor2(ta/1461)*0.0307572+99.2018973;
ar=fnmo(ar);
zvr=ar*24/360-duz*4/60; 
buff2.Format("zvr=%f ar=%f es=%f ec=%f tg=%f",zvr,ar,es,ec,tg);
//MessageBox(buff2);
while(zvr>24)  zvr-=24;
while(zvr<0)   zvr+=24; 

m_fZvr=zvr;

	
be=fnmo(tg*0.985609121+99.18);         // zemlja
el=fnmo(be+sin((be-102.2)*pi/180)*1.845);   // GLOBAL
ea=1+sin((el-192.2)*pi/180)*0.0167;       // GLOBAL

ls=fnmo(el+180);           // sunce

lm=tg*13.1763976+51.23;     // mjesec
pm=tg*0.111399014+208.9;
km=372.1-tg*0.052953643;
lm=fnmo(lm); pm=fnmo(pm); km=fnmo(km);
am=lm-pm;
km-=sin(as*pi/180)*0.16;
ms=(lm-ls)*2-am;
am+=sin(ms*pi/180)*1.27388889-sin(as*pi/180)*(0.18638889+0.36);
lm+=sin(ms*pi/180)*1.27388889-sin(as*pi/180)*0.18638889+sin(am*pi/180)*6.28833333;
m1=lm-ls;
lm+=sin(m1*2*pi/180)*0.658333333;
m2=lm-km;
lm-=sin(m2*2*pi/180)*0.12;
bm=sin(m2*pi/180)*5.14539;
m3=(lm-ls)*2-m2;
bm+=sin(m3*pi/180)*0.15;

strcpy(ime,"Sunce"); 

//MessageBox("ok");


Koord(pDC, ls,0,ime,1);

//MessageBox("vratio se");

strcpy(ime,"Mjesec"); 
Koord(pDC, lm,bm,ime,2);



//planete 

int z;
float tb,ep,ph,mp,e2,kn,i2,ae;
float ml,wl,sp,ws,si,fl,we,al,wt,ab;

char data[6][80]={
	  "Merkur 4.0923 31.19 76.987 23.00 0.2056 47.826 7.004 0.3871",
	  "Venera 1.6021 80.85 131.149 0.76 0.0068 76.41 3.394 0.7233",
	  "Mars 0.5240 144.14 335.507 11.00 0.0934 49.326 1.85 1.5237",
	  "Jupiter 0.0831 316.19 13.839 5.30 0.0485 100.146 1.305 5.2028",
	  "Saturn 0.0335 158.36 92.460 5.50 0.0557 113.511 2.486 9.581",
	  "Uran 0.0117 98.38 170.173 5.7 0.0472 73.847 0.773 19.1823" };

  for (z=0; z<6; z++) {
     sscanf(data[z],"%s %f %f %f %f %f %f %f %f",ime,&tb,&ep,&ph,&mp,&e2,&kn,&i2,&ae);
     
	 pripadnost[z+3]=23;  

       ml=fnmo(tb*tg+ep);
       wl=ml+sin((ml-ph)*pi/180)*mp;
       sp=ae+sin((wl-ph-90)*pi/180)*e2*ae;
       ws=fnmo(360+el-wl)*pi/180;
       si=sin(ws);
       fl=ea/sp-cos(ws);
       we=atan(si/fl)*180/pi;
       al=fnmo(el+we+180*(fl>=0));
       wt=sin((wl-kn)*pi/180)*i2;
       ab=atan(tan(wt*pi/180)*abs(sin(we*pi/180)/si))*180/pi;

     Koord(pDC, al,ab,ime,3+z);
  }

// zvijezde 

  broj_zvijezda=Ucitaj_Zvijezde();

  if (broj_zvijezda > 0)  for(z=9; z<=8+broj_zvijezda;  z++) {
       x=objx[z];y=objy[z];
       switch(sjaj[z]) {
      case 1: pDC->MoveTo(x-1,y); pDC->LineTo(x+1,y); 
		      pDC->MoveTo(x,y-1); pDC->LineTo(x,y+1); 
		      break;
	  case 2: pDC->MoveTo(x-1,y); pDC->LineTo(x+1,y);
		      pDC->MoveTo(x,y-1); pDC->LineTo(x,y+1);

			  pDC->MoveTo(x-1,y-1); pDC->LineTo(x+1,y+1);
		      pDC->MoveTo(x+1,y-1); pDC->LineTo(x-1,y+1);
			  break;
      case 3: Ellipse(pDC,x,y,1,1,0,360); /* pDC->FloodFill(x,y,Colors[0]) */  ;break;
      case 4: Ellipse(pDC,x,y,2,2,0,360); /* pDC->FloodFill(x,y,Colors[0]) */ ;break;
      case 5: Ellipse(pDC,x,y,2,2,0,360);/* pDC->FloodFill(x,y,Colors[0]); */
		      pDC->MoveTo(x-3,y); pDC->LineTo(x+3,y);
			  pDC->MoveTo(x,y-3); pDC->LineTo(x,y+3); 
      }
  }


	pDC->SelectObject(oldBrush);
	delete brush;

    pDC->SelectObject(staraOlovka);  // vrati staru olovku
}


//**********************************************************************************************************************

void CAsc2winView::Koord(CDC *pDC, double al, double ab, char *ime, int objekt)
{

int vri,x,y,z;
double sn,cs,sl,cl,de,re;
double sw,h,a,r,kut;
char buff[80];
double duz, sir;
CString buff2;

duz = m_dUlazDlg.m_fDuz;
sir = m_dUlazDlg.m_fSir;
vri = m_dUlazDlg.m_iVri;

sb=sin(sir*pi/180); cb=cos(sir*pi/180);    // GLOBAL

sn=sin(ab*pi/180); cs=cos(ab*pi/180);
sl=sin(al*pi/180); cl=cos(al*pi/180);
de=asin(ec*sn+es*cs*sl);
re=2*atan((ec*cs*sl-es*sn)/(cos(de)+cs*cl));
re=fnmo(re*180/pi);

sw=fnmo(ar-re-duz); sw=sw*pi/180;
h=asin(sb*sin(de)+cb*cos(de)*cos(sw)); if (h<0) return;

buff2.Format("sb=%f cb=%f",sb, cb );
//MessageBox(buff2);

buff2.Format("h=%f",h);
//MessageBox(buff2);

a=acos((sin(de)-sb*sin(h))/(cb*cos(h)));
if(sw<pi) a=-a;
r=320-h*638/pi;    // bilo r=173-h*346/pi;
kut=-a-pi/2;

buff2.Format("kut=%d r=%f",kut,r);
//MessageBox(buff2);


while(kut<0) kut=2*pi+kut;
while(kut>2*pi) kut=kut-2*pi;

x=floor2(-cos(kut)*r+319.5);  //17.1.08: stavljeno -cos, invertira W i E na slici
y=floor2(sin(kut)*r+319.5);
//x=100; y=100;

objx[objekt]=x; objy[objekt]=y;
strcpy(obj[objekt],ime);

buff2.Format("x= %d y=%d",x,y);
//MessageBox(buff2);

   switch(objekt) {
   case 1: Ellipse(pDC,x,y,6,6,0,360);
	   
	   for(z=0; z<360; z+=45) {
		   pDC->MoveTo( x+8*cos(z*pi/180),y+8*sin(z*pi/180) ); 
           pDC->LineTo(x+11*cos(z*pi/180),y+11*sin(z*pi/180) );
	   }
	   
	   break;  // sunce

   case 2: Ellipse(pDC,x,y,6,6,0,360);
	       pDC->MoveTo(x-1,y-6); 
	       pDC->LineTo(x-1,y+6); 
	       pDC->FloodFill(x-3,y,Colors[7]);
		   break;   // mjesec

   default: Ellipse(pDC,x,y,3,3,0,360);    //planete
  }


}

//********************************************************************************************************
int CAsc2winView::Ucitaj_Zvijezde()
{

FILE *fajl;

int x=0,y=0,z=0,sj,pr;

float arg1,re=0,de=0;

float sw=0,h=0,a=0,r=0,kut=0;

char ime[20],buff[90];

double duz;
CString buff2;

duz = m_dUlazDlg.m_fDuz;

fajl=fopen("zvz_zvj.dat","rt"); if(fajl==NULL) {MessageBox("nema fajla 'zvz_zvj.dat' ?"); return 0; }
  do {
     fgets(buff,79,fajl); if(feof(fajl)) break;
			  if(strstr(buff,"kraj")!=NULL) break;
     sscanf(buff,"%s %f %f %d %d ",ime,&re,&de,&pr,&sj);
      de*=pi/180;
       sw=fnmo(ar-re-duz); sw*=pi/180;
       h=asin(sb*sin(de)+cb*cos(de)*cos(sw)); if (h<0) continue;
       
       arg1=(sin(de)-sb*sin(h) )/(cb*cos(h));
       if(arg1<-1.) arg1=-1.0;

       a=acos(arg1); if(sw<pi) a=-a;
       r=320-h*638/pi;   kut=-a-pi/2;

	while(kut<0) kut=2*pi+kut;
	while(kut>2*pi) kut=kut-2*pi;

    x=floor2(cos(kut)*r+319.5);
	y=floor2(sin(kut)*r+319.5);
	
	objx[9+z]=x; objy[9+z]=y; 
	strcpy(obj[9+z],ime);
	sjaj[9+z]=sj;pripadnost[9+z]=pr;
	z++;
  } while(1);

fclose(fajl);

buff2.Format("z=%d",z);
//MessageBox(buff2);

return z;

}


//******************************************************************************************************************
void CAsc2winView::Ucitaj_Jata()
{

	FILE *fajl;
	int  z;

	fajl=fopen("zvz_jata.dat","rt"); if(fajl==NULL) { MessageBox("nema fajla 'zvz_jata.dat' ?"); return; }
	
	for(z=0; z<=43; z++) { fgets(jato[z],35,fajl);jato[z][strlen(jato[z])-1]=0; } 

    fclose(fajl);

}

//******************************************************************************************************************
void CAsc2winView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRect rect;
	GetClientRect(rect);
	
	//CString bf1; bf1.Format("%d %d",rect.Width(), rect.Height()); MessageBox(bf1);

  double r;	//kscr = 1.14695340501792;
  //double kscr = 800/(rect.Width()*.7);

  double kscr =  600*m_dUlazDlg.m_iHorRez/(rect.Height()*.9*m_dUlazDlg.m_iHorRez); // bilo 700/525
  int z,min=1000,o=1000;

  int xm= (point.x  - 220)*kscr +7; // +7 je za kvadratni kursor
  int ym= (point.y - 6)*kscr +7;


  CString buff2;
				 
 int nIndex = iStvarniIndex[( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.GetCurSel()];   //Handle na CMainFrame objekt!!!!!!
 
 if (nIndex ==2 ) {  // opcija nebo

		for (z=1; z<=8+broj_zvijezda; z++) {
	      r=hypot( objx[z]-xm, objy[z]-ym );
	       if ( abs(objx[z]-xm)<6 && abs(objy[z]-ym)<6 && r<min) { min=r; o=z; }
	     }


		if (o != 1000 && o != o_old && o_old==1000 && m_bImena==TRUE ) {

				/*                                    // popup više ne koristim
				 pPopupDlg->m_sNaziv = obj[o];
				 pPopupDlg->m_sGrupa = jato[pripadnost[o]];
	          CRect rct; GetWindowRect(rct);
				 pPopupDlg->SetWindowPos(NULL, rct.left+30+floor2(point.x/20)*20, rct.top+10+floor2(point.y/20)*20, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				 pPopupDlg->UpdateData(FALSE);
				*/


				 // skini pozadinu prozora
 					CDC * pDC=this->GetDC();

					//bmCap.CreateCompatibleBitmap (pDC, 300,50);  // prebaèeno u init da se izvodi samo jednom
					//bmOld = dcPozadina.SelectObject(&bmCap);

					pntCapPoz = CPoint (point.x, floor2((point.y)/10)*10-1 );
				   dcPozadina.BitBlt(0, 0, 300,50, pDC, pntCapPoz.x, pntCapPoz.y,	SRCCOPY   );  


				 //pPopupDlg->ShowWindow(SW_SHOW);

				  CFont font1;
					LOGFONT lf;
					memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
					lf.lfHeight = 17;                      // request a 12-pixel-height font
					strcpy(lf.lfFaceName, "Book Antiqua");        // request a face name "Arial"
					VERIFY(font1.CreateFontIndirect(&lf));  // create the font
					CFont* def_font = pDC->SelectObject(&font1);

					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(Colors[13]);
					pDC->TextOut(floor2((point.x+30)/10)*10,floor2((point.y)/10)*10,obj[o]);
					pDC->TextOut(floor2((point.x+30)/10)*10,floor2((point.y)/10)*10+15,jato[pripadnost[o]]);
					
					pDC->SelectObject(def_font);
					font1.DeleteObject();

				 o_old=o;

		}
		else if (o != 1000 && o != o_old && o_old !=1000 && m_bImena==TRUE ) {

				 CDC * pDC=this->GetDC();

				// prvo pobriši staro

				 pDC->BitBlt(pntCapPoz.x, pntCapPoz.y, 300, 50, &dcPozadina, 0, 0, SRCCOPY    );
				 //dcPozadina.SelectObject(bmOld);
				 //bmCap.Detach();

					// onda dalje
					//bmCap.CreateCompatibleBitmap (pDC, 300,50);  // prebaèeno u init da se izvodi samo jednom
					//bmOld = dcPozadina.SelectObject(&bmCap);

					pntCapPoz = CPoint (point.x, floor2((point.y)/10)*10-1 );
				   dcPozadina.BitBlt(0, 0, 300,50, pDC, pntCapPoz.x, pntCapPoz.y,	SRCCOPY   );  


				 //pPopupDlg->ShowWindow(SW_SHOW);

				  CFont font1;
					LOGFONT lf;
					memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
					lf.lfHeight = 17;                      // request a 12-pixel-height font
					strcpy(lf.lfFaceName, "Book Antiqua");        // request a face name "Arial"
					VERIFY(font1.CreateFontIndirect(&lf));  // create the font
					CFont* def_font = pDC->SelectObject(&font1);

					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(Colors[13]);
					pDC->TextOut(floor2((point.x+30)/10)*10,floor2((point.y)/10)*10,obj[o]);
					pDC->TextOut(floor2((point.x+30)/10)*10,floor2((point.y)/10)*10+15,jato[pripadnost[o]]);
					
					pDC->SelectObject(def_font);
					font1.DeleteObject();

				 o_old=o;



		}
		else if (o == 1000 && o != o_old ) {
				 
				 //pPopupDlg->ShowWindow(SW_HIDE);

				 // vrati pozadinu
				 //CRect rct;
				 //GetWindowRect(rct);
 				 CDC * pDC=this->GetDC();

				 pDC->BitBlt(pntCapPoz.x, pntCapPoz.y, 300, 50, &dcPozadina, 0, 0, SRCCOPY    );

				 //dcPozadina.SelectObject(bmOld);
				 //bmCap.Detach();
				 
				 //pDC->Rectangle(pntCapPoz.x, pntCapPoz.y, pntCapPoz.x+300, pntCapPoz.y+50  );
				 
				 o_old = o;
                 

		}

}









 //greg.kal.
  double kscr2 = 600/(rect.Width()/m_dUlazDlg.m_iHorRez); //bilo *750/1050

  int xm2= (point.x  - 40*m_dUlazDlg.m_iHorRez)*kscr2 ; 
  int ym2= (point.y  - 50)*kscr2 ;
  boolean click=false, click2, click3, click4;
  int mj,dn;
  static int inv_x1_old=99999, inv_y1_old=99999;

  CRect * pPrav	= new CRect(0,0,0,0);

  CDC * pDC=this->GetDC();

  if ( nIndex ==11) {  // opcija greg.kal.

    int plold=pl[1]; 
	pl[1]=28; 
	if (fmod2(m_dUlazDlg.m_iGod,4)==0 ) 
		if ( fmod2(m_dUlazDlg.m_iGod,100)!= 0 || fmod2(m_dUlazDlg.m_iGod,400)==0 || m_dUlazDlg.m_iGod<1600) pl[1]=29;

	  for (mj=1; mj<=12; mj++) {

		  for (dn=0; dn<pl[mj-1] ; dn++) {
			  	  
		  
			pPrav->left=  fmod2( (mj-1),4)*200+ fmod2(dn+	dan_tj[mj-1] ,7) *25;	
			pPrav->right =fmod2( (mj-1),4)*200+ fmod2(dn+	dan_tj[mj-1] ,7) *25+22;
			
			pPrav->top =    floor2( (mj-1)/4)*144    + (1+(dn + dan_tj[mj-1] )/ 7) *18;
			pPrav->bottom = floor2( (mj-1)/4)*144    + (1+(dn + dan_tj[mj-1] )/ 7) *18 + 15;

			
			if (xm2 <= pPrav->right &&
				xm2 >= pPrav->left  &&
				ym2 <= pPrav->bottom &&
				ym2 >= pPrav->top ) {

				click=true;
				break;

			}  //if

           }   //for 
   
		if (click==true) break;
		}      //for


		pl[1]=plold; 

		int jx2, jx3,jx4;
		click2=false;click3=false;click4=false;

		if(click==true){

			//hendlanje popupa s nazivom praznika:
			for (jx2=1; jx2<=3+2+(br_praznika-1); jx2++) {

				if(    dn+1 == dan_praznik[jx2-1] 
					&& mj   == mje_praznik[jx2-1]) {

					click2=true;
					break;

				}

			}
			//hendlanje popupa s nazivom praznika:
			for (jx4=3+2+(br_praznika-1)+1; jx4<=saImenaPraznika.GetSize(); jx4++) {

				if(    dn+1 == dan_praznik[jx4-1] 
					&& mj   == mje_praznik[jx4-1]) {

					click4=true;
					break;

				}

			}
			//if (click2==false) 
			  for (jx3=1; jx3<=saImenaOstalo.GetSize(); jx3++) {

				if(    dn+1 == dan_ostalo[jx3-1] 
					&& mj   == mje_ostalo[jx3-1]) {

					click3=true;
					break;

				}

			}



			inv_x1= pPrav->left   /kscr2 +40*m_dUlazDlg.m_iHorRez;
			inv_y1= pPrav->top    /kscr2 +50;
				
			if(	invert_is_on==true &&  (    inv_y1  != inv_y1_old || inv_x1  != inv_x1_old) ) {


				//MessageBox("tucam!");

				if (popup_is_on == true) {
				
					//izbriši popup i vrati pozadinu:

					pDC->BitBlt(pntCapPoz2.x, pntCapPoz2.y, 300, 50, &dcPozadina2, 0, 0, SRCCOPY    );
					
					dcPozadina2.SelectObject(bmOld2);
					bmCap2.Detach();
					DeleteObject(bmCap2);



					/*
					CString buff;
					buff.Format("%d, %d",pntCapPoz2.x, pntCapPoz2.y);
					MessageBox(buff);
					*/

					popup_is_on=false;

				}






				CRect pPrav2(inv_x1_old,inv_y1_old,inv_x1_old +23/kscr2,inv_y1_old +16/kscr2);

		   		if (inv_x1_old!=99999) pDC->InvertRect(pPrav2);

				invert_is_on=false;

				inv_x1_old=9999;	inv_y1_old=9999;
			}

			if (invert_is_on==false) {




				CRect pPrav2(0,0,0,0);

				pPrav2.left   = pPrav->left   /kscr2 +40*m_dUlazDlg.m_iHorRez;
				pPrav2.right  = pPrav2.left +23/kscr2;
				pPrav2.top    = pPrav->top    /kscr2 +50;
				pPrav2.bottom = pPrav2.top +16/kscr2;

		   		pDC->InvertRect(pPrav2);

			
				invert_is_on=true;

				inv_x1=pPrav2.left;
				inv_y1=pPrav2.top;

				inv_x1_old=inv_x1;
				inv_y1_old=inv_y1;


			//ispiši popup ako treba
			if ( (click2==true || click3==true  || click4==true) && popup_is_on == false ) {   

					//uzmi pozadinu i prikaži popup

					bmCap2.CreateCompatibleBitmap (pDC, 300,50);  // ako još nije kreiran
					bmOld2 =  dcPozadina2.SelectObject(&bmCap2);

					if (xm2 <600) pntCapPoz2 = CPoint (point.x, floor2((point.y)/10)*10-1 );
					else pntCapPoz2 = CPoint (point.x-250, floor2((point.y)/10)*10-1 );
				    dcPozadina2.BitBlt(0, 0, 300,50, pDC, pntCapPoz2.x, pntCapPoz2.y,	SRCCOPY   );  


				  CFont font1;
					LOGFONT lf;
					memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
					lf.lfHeight = 17;                      // request a 12-pixel-height font
					strcpy(lf.lfFaceName, "Book Antiqua");        // request a face name "Arial"
					VERIFY(font1.CreateFontIndirect(&lf));  // create the font
					CFont* def_font = pDC->SelectObject(&font1);

					pDC->SetBkMode(OPAQUE);
					pDC->SetTextColor(Colors[0]);
					CString s1;	
					CSize size2;


					int linija=0;

					if (click2==true){

					if      (boja_praznik[jx2-1]==11)	pDC->SetBkColor( /*Colors[4]*/RGB( 0xFF, 0x3f, 0x7f ) );
					else if (boja_praznik[jx2-1]==14)	pDC->SetBkColor( Colors[16]  );
					else if (boja_praznik[jx2-1]==9 )	pDC->SetBkColor( Colors[2]  );
					else 
						pDC->SetBkColor(Colors[15]);

						s1= saImenaPraznika.GetAt(jx2-1);
						s1.TrimRight(' ');
						size2=pDC->GetTextExtent(s1, s1.GetLength() );

						if (xm2 < 600) pDC->TextOut(floor2((point.x+17)/10)*10,floor2((point.y+7)/10)*10, s1 );
						else           pDC->TextOut(floor2((point.x-size2.cx-5)/10)*10,floor2((point.y+7)/10)*10, s1 );

					linija++;
					}

					if (click4==true){

					if      (boja_praznik[jx4-1]==11)	pDC->SetBkColor( /*Colors[4]*/RGB( 0xFF, 0x3f, 0x7f) );
					else if (boja_praznik[jx4-1]==14)	pDC->SetBkColor( Colors[16]  );
					else if (boja_praznik[jx4-1]==9 )	pDC->SetBkColor( Colors[2]  );
					else 
						pDC->SetBkColor(Colors[15]);

						s1= saImenaPraznika.GetAt(jx4-1);
						s1.TrimRight(' ');
						
						if (xm2 < 600) { 
						    
							if (click2==true) pDC->TextOut(floor2((point.x+17)/10)*10+10+size2.cx,floor2((point.y+7)/10)*10, s1 );	
							else pDC->TextOut(floor2((point.x+17)/10)*10,floor2((point.y+7)/10)*10, s1 );	
						
						}
						else { 

							CSize size1;
							size1=pDC->GetTextExtent(s1, s1.GetLength() );

							if (click2==true) pDC->TextOut(floor2((point.x-size1.cx-size2.cx-10)/10)*10,floor2((point.y+7)/10)*10, s1 );
							else pDC->TextOut(floor2((point.x-size1.cx-5)/10)*10,floor2((point.y+7)/10)*10, s1 );
						}

						if (linija==0) linija++;
					}
					
					if (click3==true){

						pDC->SetBkColor( Colors[7] );

						s1= saImenaOstalo.GetAt(jx3-1);
						s1.TrimRight(' ');
						if (xm2 < 600) pDC->TextOut(floor2((point.x+17)/10)*10,floor2((point.y+7)/10)*10+linija*18, s1 );
						else { 

							CSize size1;
							size1=pDC->GetTextExtent(s1, s1.GetLength() );
							pDC->TextOut(floor2((point.x-size1.cx-5)/10)*10,floor2((point.y+7)/10)*10+linija*18, s1 );
						}
					}



					pDC->SetBkMode(TRANSPARENT);
					
					pDC->SelectObject(def_font);
					font1.DeleteObject();

					popup_is_on = true;

				} //popup_is_on=false





			}
		}
		else {   //click =false




			if (invert_is_on==true) {



				if (popup_is_on == true) {
				
					//izbriši popup i vrati pozadinu:

					pDC->BitBlt(pntCapPoz2.x, pntCapPoz2.y, 300, 50, &dcPozadina2, 0, 0, SRCCOPY    );
					dcPozadina2.SelectObject(bmOld2);
					bmCap2.Detach();
					DeleteObject(bmCap2);


					/*
					CString buff;
					buff.Format("%d, %d",pntCapPoz2.x, pntCapPoz2.y);
					MessageBox(buff);
					*/

					popup_is_on=false;

				}



				CRect pPrav2(inv_x1_old,inv_y1_old,inv_x1_old +23/kscr2,inv_y1_old +16/kscr2);

		   		if (inv_x1_old!=9999) pDC->InvertRect(pPrav2);

				invert_is_on=false;

				inv_x1_old=9999; inv_y1_old=9999;



				//MessageBox("inv off");




			}
		}













	//provjera da li je kliknuto na mjesec:

	
		  click=false;
		  for (mj=0; mj<=11; mj++) {

			pPrav->left  =  fmod2(mj,4)*200;	
			pPrav->right =  fmod2(mj,4)*200+ 6*25+22;
			
			pPrav->top = floor2(mj/4)*144 ;
			pPrav->bottom = floor2(mj/4)*144 + 15;


			if (xm2 <= pPrav->right &&
				xm2 >= pPrav->left  &&
				ym2 <= pPrav->bottom &&
				ym2 >= pPrav->top ) {

				click=true;
				break;

			}  //if
		} //for



		if(click==true){

			if (invert2_is_on==false) {

				CRect pPrav2(0,0,0,0);

				pPrav2.left   = pPrav->left   /kscr2 +40*m_dUlazDlg.m_iHorRez;
				pPrav2.right  = pPrav2.left +(6*25+22)/kscr2;
				pPrav2.top    = pPrav->top    /kscr2 +50;
				pPrav2.bottom = pPrav2.top + 15/kscr2;

		   		pDC->InvertRect(pPrav2);

			
				invert2_is_on=true;

				inv2_x1=pPrav2.left;
				inv2_y1=pPrav2.top;

			}
		}
		else {   //click =false
			if (invert2_is_on==true) {

				CRect pPrav2(inv2_x1,inv2_y1,inv2_x1 +(6*25+22)/kscr2,inv2_y1+ 15/kscr2);

		   		pDC->InvertRect(pPrav2);

				invert2_is_on=false;
			}
		}



	}//if






  //pla i asc interaktivnost


 if ( nIndex ==0 || nIndex ==1  ) {  // opcija pla

 	kscr2 = 600/1.1/(rect.Width()/m_dUlazDlg.m_iHorRez); //bilo 750/1050

	xm2= (point.x  - 0 )*kscr2 ; 
	ym2= (point.y  - 35)*kscr2 ;
	
	click=false;
	

    int plold=pl[1]; 
	pl[1]=28; 
	
	if (fmod2(m_dUlazDlg.m_iGod,4)==0 ) 
		if ( fmod2(m_dUlazDlg.m_iGod,100)!= 0 || fmod2(m_dUlazDlg.m_iGod,400)==0 || m_dUlazDlg.m_iGod<1600) pl[1]=29;


	  for (dn=0; dn<pl[m_dUlazDlg.m_iMje-1] ; dn++) {
			  	  
		  
		pPrav->left= 91;
		pPrav->right = 630+1;
	
		pPrav->top = 16+(dn)*10-1;
		pPrav->bottom = 24+(dn)*10;



		if (xm2 <= pPrav->right &&
			xm2 >= pPrav->left  &&
			ym2 <= pPrav->bottom &&
			ym2 >= pPrav->top ) {

			click=true;
			break;
		}  //if

     }   //for 
   

		pl[1]=plold; 


		if(click==true) {

			if(	invert_is_on==true &&  pPrav->top/kscr2 +35  != inv_y1) {


				CRect pPrav2(inv_x1,inv_y1,inv_x1 +(631-91)/kscr2,inv_y1 +10/kscr2);

		   		pDC->InvertRect(pPrav2);

				invert_is_on=false;

			}

			if (invert_is_on==false) {

				CRect pPrav2(0,0,0,0);

				pPrav2.left   = pPrav->left   /kscr2 +0;
				pPrav2.right  = pPrav2.left +(631-91)/kscr2;

				pPrav2.top    = pPrav->top    /kscr2 +35;
				pPrav2.bottom = pPrav2.top +10/kscr2;

		   		pDC->InvertRect(pPrav2);

			
				invert_is_on=true;

				inv_x1=pPrav2.left;
				inv_y1=pPrav2.top;

			}

		}
		else {   //click =false
			if (invert_is_on==true) {

				CRect pPrav2(inv_x1,inv_y1,inv_x1 +(631-91)/kscr2,inv_y1 +10/kscr2);

		   		pDC->InvertRect(pPrav2);

				invert_is_on=false;
			}
		}




	}  //if opcija pla 











 
 	delete pPrav;






	CView::OnMouseMove(nFlags, point);
}

//*******************************************************************************************
void CAsc2winView::OnUpdateViewImena(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

	if( m_bImena == TRUE) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
	
	
}

//************************************************************************************************
void CAsc2winView::OnUpdateIndicatorZvr(CCmdUI *pCmdUI)
{
	CString buff;

	double minute = (m_fZvr-floor2(m_fZvr))*60;

	double sekunde = (minute-floor2(minute))*60;


	if (m_fZvr!=0) 	buff.Format(" Zvj.vr.: %02d:%02d'%02d''",(int)floor2(m_fZvr), (int)floor2(minute), (int)floor2(sekunde)  ); 
	else buff="Zvj.vr.: - ";
		
	

pCmdUI->Enable(TRUE);

// Set the text of the status bar pane
// to the current color

	pCmdUI->SetText(buff);

}

//*******************************************************************************************
void CAsc2winView::OnViewImena() 
{
	// TODO: Add your command handler code here
	m_bImena = ! ( m_bImena);
	
}

//*******************************************************************************************
void CAsc2winView::OnSada() 
{
	// TODO: Add your command handler code here

	m_dUlazDlg.m_iSat = CTime::GetCurrentTime().GetHour();
	m_dUlazDlg.m_iMin = CTime::GetCurrentTime().GetMinute();

	m_dUlazDlg.m_iDan = CTime::GetCurrentTime().GetDay();
	m_dUlazDlg.m_iMje = CTime::GetCurrentTime().GetMonth();
	m_dUlazDlg.m_iGod = CTime::GetCurrentTime().GetYear();
	
	Ucitaj_Postavke_Horoskopa(FALSE);
	m_dUlazDlg.m_sIme="<sada>";
	m_dSlika->ShowWindow(SW_HIDE);

	Invalidate();
}

//**********************************************************************************************************
BOOL CAsc2winView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	int nIndex = iStvarniIndex[( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.GetCurSel()];   //Handle na CMainFrame objekt!!!!!!

	if (nIndex == 2 ) {

		SetCursor(lhCursor_Nebo);
		return TRUE;
	}
	return CView::OnSetCursor(pWnd, nHitTest, message);
}

//**********************************************************************************************************

void CAsc2winView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
  double r;	//kscr = 1.14695340501792;

  CRect rect;
  GetClientRect(rect);
	
  //CString bf1; bf1.Format("%d %d",rect.Width(), rect.Height()); MessageBox(bf1);

  //double kscr =  m_dUlazDlg.m_iHorRez/(rect.Width() *.7);
    double kscr =  600*m_dUlazDlg.m_iHorRez/(rect.Height()*.9*m_dUlazDlg.m_iHorRez); // bilo 700/525


  int z,min=1000,o=1000;

  int xm= (point.x  - 220)*kscr +7; // +7 je za kvadratni kursor
  int ym= (point.y - 6)*kscr +7;



  CString buff2;
				 
 int nIndex = iStvarniIndex[( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.GetCurSel()];   //Handle na CMainFrame objekt!!!!!!
 

/*
  if (nIndex ==5) {
   CString bf1; bf1.Format("%d,%d / %d,%d",point.x ,point.y, xm,ym); MessageBox(bf1); 
  }

*/



 if (xm>0 && ym>0 && nIndex ==2) {  // opcija nebo

		for (z=1; z<=8+broj_zvijezda; z++) {
	      r=hypot( objx[z]-xm, objy[z]-ym );
	       if ( abs(objx[z]-xm)<6 && abs(objy[z]-ym)<6 && r<min) { min=r; o=z; }
	     }


		if (o != 1000 && o_old==1000 && o!=o_old) {

				/*                             // popup se više ne koristi
				 pPopupDlg->m_sNaziv = obj[o];
				 pPopupDlg->m_sGrupa = jato[pripadnost[o]];
				 CRect rct; 
				 GetWindowRect(rct);  //okir glavnog prozora
				 pPopupDlg->SetWindowPos(NULL, rct.left+30+floor2(point.x/20)*20, rct.top+10+floor2(point.y/20)*20, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
     			 pPopupDlg->UpdateData(FALSE);
				*/
				 
				 if (m_bImena==FALSE) {

					 // skini pozadinu prozora

 					CDC * pDC=this->GetDC();

					//bmCap.CreateCompatibleBitmap (pDC, 300,50); //prebaèeno u init
					//bmOld = dcPozadina.SelectObject(&bmCap);

					pntCapPoz = CPoint (point.x, floor2((point.y)/10)*10-1 );
				   dcPozadina.BitBlt(0, 0, 300,50, pDC, pntCapPoz.x, pntCapPoz.y,	SRCCOPY   );  

					//pPopupDlg->ShowWindow(SW_SHOW);
				  CFont font1;
					LOGFONT lf;
					memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
					lf.lfHeight = 17;                      // request a 12-pixel-height font
					strcpy(lf.lfFaceName, "Book Antiqua");        // request a face name "Arial"
					VERIFY(font1.CreateFontIndirect(&lf));  // create the font
					CFont* def_font = pDC->SelectObject(&font1);

					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(Colors[13]);
					pDC->TextOut(floor2((point.x+30)/10)*10,floor2((point.y)/10)*10,obj[o]);
					pDC->TextOut(floor2((point.x+30)/10)*10,floor2((point.y)/10)*10+15,jato[pripadnost[o]]);
					
					pDC->SelectObject(def_font);
					font1.DeleteObject();
				 
				 }

				 o_old=o;


		}

}
		



 	double kscr2 = 600/(rect.Width()/m_dUlazDlg.m_iHorRez); //bilo * 750/1050



	
 //greg.kal.

  int xm2= (point.x  - 40*m_dUlazDlg.m_iHorRez)*kscr2 ; 
  int ym2= (point.y  - 50)*kscr2 ;
  boolean click=false;
  int mj,dn;

  CRect * pPrav	= new CRect(0,0,0,0);

  
  if (xm2>0 && ym2>0 && nIndex ==11) {  // opcija greg.kal.

    int plold=pl[1]; 
	pl[1]=28; 
	if (fmod2(m_dUlazDlg.m_iGod,4)==0 ) 
		if ( fmod2(m_dUlazDlg.m_iGod,100)!= 0 || fmod2(m_dUlazDlg.m_iGod,400)==0 || m_dUlazDlg.m_iGod<1600) pl[1]=29;

	  for (mj=1; mj<=12; mj++) {

		  for (dn=0; dn<pl[mj-1] ; dn++) {
			  	  
		  
			pPrav->left=  fmod2( (mj-1),4)*200+ fmod2(dn+	dan_tj[mj-1] ,7) *25;	
			pPrav->right =fmod2( (mj-1),4)*200+ fmod2(dn+	dan_tj[mj-1] ,7) *25+22;
			
			pPrav->top =    floor2( (mj-1)/4)*144    + (1+(dn + dan_tj[mj-1] )/ 7) *18;
			pPrav->bottom = floor2( (mj-1)/4)*144    + (1+(dn + dan_tj[mj-1] )/ 7) *18 + 15;




			
			
			if (xm2 <= pPrav->right &&
				xm2 >= pPrav->left  &&
				ym2 <= pPrav->bottom &&
				ym2 >= pPrav->top ) {

				click=true;
				m_dUlazDlg.m_iMje=mj;
				m_dUlazDlg.m_iDan=dn+1;	
				break;

			}  //if

           }   //for 
   
		if (click==true) break;
		}      //for


		pl[1]=plold; 

		if(click==true){

			//Ucitaj_Postavke_Horoskopa(FALSE);  //ako nije iz baze uzmi defaultne postavke
			//Iskljuci_sliku();

			m_dUlazDlg.m_sIme="<kalendar>";

        ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.SetCurSel(iStvarniIndexSet[4]);   //Handle na CMainFrame objekt!!!!!!

		invert_is_on = false; 
		popup_is_on  = false;



			Invalidate();

		}


	//provjera da li je kliknuto na mjesec:


	  if (click==false) {
		  for (mj=0; mj<=11; mj++) {

			pPrav->left  =  fmod2(mj,4)*200;	
			pPrav->right =  fmod2(mj,4)*200+ 6*25+22;
			
			pPrav->top = floor2(mj/4)*144 ;
			pPrav->bottom = floor2(mj/4)*144 + 15;


			if (xm2 <= pPrav->right &&
				xm2 >= pPrav->left  &&
				ym2 <= pPrav->bottom &&
				ym2 >= pPrav->top ) {

				click=true;
				m_dUlazDlg.m_iMje=mj+1;
				break;

			}  //if
		} //for



		if(click==true){

			//Ucitaj_Postavke_Horoskopa(FALSE);  //ako nije iz baze uzmi defaultne postavke
			//Iskljuci_sliku();
			m_dUlazDlg.m_sIme="<kalendar>";

        ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.SetCurSel(iStvarniIndexSet[0]);   //Handle na CMainFrame objekt!!!!!!

			Invalidate();

		}
	}


 
	}//if
 
 
  
 


  //pla i asc interaktivnost


 if (xm>0 && ym>0 && ( nIndex ==0 || nIndex ==1)  ) {  // opcija pla

 	kscr2 = 600/1.1/(rect.Width()/m_dUlazDlg.m_iHorRez); // bilo * 750/1050

	xm2= (point.x  - 0 )*kscr2 ; 
	ym2= (point.y  - 35)*kscr2 ;
	
	click=false;
	

    int plold=pl[1]; 
	pl[1]=28; 
	
	if (fmod2(m_dUlazDlg.m_iGod,4)==0 ) 
		if ( fmod2(m_dUlazDlg.m_iGod,100)!= 0 || fmod2(m_dUlazDlg.m_iGod,400)==0 || m_dUlazDlg.m_iGod<1600) pl[1]=29;


	  for (dn=0; dn<pl[m_dUlazDlg.m_iMje-1] ; dn++) {
			  	  
		  
		pPrav->left= 91;
		pPrav->right = 630+1;
	
		pPrav->top = 16+(dn)*10-1;
		pPrav->bottom = 24+(dn)*10;



		if (xm2 <= pPrav->right &&
			xm2 >= pPrav->left  &&
			ym2 <= pPrav->bottom &&
			ym2 >= pPrav->top ) {

			click=true;
			m_dUlazDlg.m_iDan=dn+1;	
			break;
		}  //if

     }   //for 
   

	 pl[1]=plold; 

		if(click==true){

			//Ucitaj_Postavke_Horoskopa(FALSE);  //ako nije iz baze uzmi defaultne postavke
			//Iskljuci_sliku();
			m_dUlazDlg.m_sIme="<kalendar>";

        ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.SetCurSel(iStvarniIndexSet[4]);   //Handle na CMainFrame objekt!!!!!!

			Invalidate();

		}


	}  //if opcija pla 





  delete pPrav;



  CView::OnLButtonDown(nFlags, point);
}


//************************************************************************************************
void CAsc2winView::Iscrtaj_Horoskop(CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt)
{

double be,ls,lm,pm,km,am,ms,m1,m2,m3,bm;
double Gvr,k=0,ta,e,rsir,jj,x,y,ra,fi;

double duz, sir;
char ime[40];
int i,vri, send=1,bd;
int danx, mjex, godx, satx, minx;
float as=0;
CString buff2;


duz = m_dUlazDlg.m_fDuz;
sir = m_dUlazDlg.m_fSir;
vri = m_dUlazDlg.m_iVri;

	


	//uzmi podatke za ispis iz dijaloga:

	boolean bObj[14], bAsp[3];

	bObj[ 0] = m_pdTijelo->m_bObj1;//objekti
	bObj[ 1] = m_pdTijelo->m_bObj2;
	bObj[ 2] = m_pdTijelo->m_bObj3;
	bObj[ 3] = m_pdTijelo->m_bObj4;
	bObj[ 4] = m_pdTijelo->m_bObj5;
	bObj[ 5] = m_pdTijelo->m_bObj6;	
	bObj[ 6] = m_pdTijelo->m_bObj7;
	bObj[ 7] = m_pdTijelo->m_bObj8;
	bObj[ 8] = m_pdTijelo->m_bObj9;
	bObj[ 9] = m_pdTijelo->m_bObj10;
	bObj[10] = m_pdTijelo->m_bObj11; //asc.
	
	bAsp[0] = m_pdTijelo->m_bAspDobri; //asp dobri
	bAsp[1] = m_pdTijelo->m_AspLosi; //asp loši da-ne


	if (vri==1) m_pdTijelo->m_bDST=true;
	else		m_pdTijelo->m_bDST=false;
	m_pdTijelo->UpdateData(FALSE);

   danx=dan;	mjex=mje; 	godx=god; 	satx=sat-vri; 	minx=min;


 	if(satx<0) {
		satx+=24;
		danx-=1;}

	if(danx<1) {
		mjex-=1;
		if(mjex<1) {
			mjex=12;
			godx-=1;	}	
		danx=pl[mjex-1];	
	}



// raè. ascendenta :

Gvr=(double)(sat-vri-gmt+min/60.); //Globalno vrijeme
if(Gvr<0) { Gvr+=24;dan-=1;
			if (dan<1) {
				mje -=1; 
				if (mje<1) { mje=12; god-=1; } 
				dan=pl[mje-1];
			}
}

i=-(mje<3);

//buff2.Format("%d",i);
//MessageBox(buff2);


k=dan+floor2((mje*153.-i*11.-162)/5.)+floor2((god*1461.+i)/4.)-(god>=0)*366.;
if (k>577736) k-=floor2((floor2((god+i)/100.)*3-5)/4.);

ta=k-693596; tg=k-711858+Gvr/24.;                   // tg je GLOBAL
jj=ta/36525. ; i=ta/36525.;
e=23.452294-jj*0.013125-jj*jj*1.639e-6+jj*jj*jj*5.028e-7;
es=sin(e*pi/180); ec=cos(e*pi/180);                // GLOBAL

ar=Gvr*360.985647/24+frac(ta/1461)*1440.02509;      // GLOBAL
ar+=floor2(ta/1461)*0.0307572+99.2018973;
ar=fnmo(ar);
zvr=ar*24/360-duz*4/60; 

//buff2.Format("zvr=%f ar=%f es=%f ec=%f tg=%f",zvr,ar,es,ec,tg);
//MessageBox(buff2);

while(zvr>24)  zvr-=24;
while(zvr<0)   zvr+=24; 

m_fZvr =zvr;



  ra=zvr*pi/12; rsir=sir*pi/180;
  x=cos(rsir)*sin(ra)*ec+sin(rsir)*es; y=cos(rsir)*cos(ra);
  asc=atan(x/y); if(asc<0) asc+=pi;
		 if(x<0) asc+=pi;
  asc=180/pi*(asc+pi/2);
  while(asc>360) asc-=360;
  //ascd=asc/30; deg2dms(asc-ascd*30,buff,0);



        if(m_dUlazDlg.m_chSustavKuca=='J') { send=0;}
   else if(m_dUlazDlg.m_chSustavKuca=='B' || bObj[10]==FALSE) { send=-1;}
  bd=1+send;




 
  Planete(danx,mjex,godx,satx,minx);

  Kuce(asc,ra,e*pi/180,send);



// odavde poèinje iscrtavanje horoskopa:
FILE *fajl;
char buff[400], bf[80];

double kut[25], temp, flagsgn, ic , temp_ic, mn, kkk=0;                         
int    retro[12]={0,0,0,0,0,0,0,0,0,0};

double kt=0,brz[10]={7,10,9,8,6,5,4,3,2,1};


double asp[7]={0,180,60,120,90,45};       // tipovi aspekata


int Boja[5]={4,6,3,2},xx,yy;
int r,j,z, kk, max, flag=0, clor=0, mod=1, kuca_planete[11];
int vl_kuce[30]={5,0,4,0,3,0,2,0,1,0,3,0,4,0,10,5,6,0,7,0,8,0,9,6};
int nagl_kuca[14]={0,0,0,0,0,0,0,0,0,0,0,0,0};

/*const COLORREF Colors[16] = {                     //ovo ja dodao, globalna var
		RGB( 0, 0, 0),      // Black,0
		RGB( 0, 0, 255),    // blue,1
		RGB( 0, 255,0,),    // green,2
		RGB( 0, 255, 255),  // cyan,3
		RGB( 255, 0, 0),    // red,4
		RGB( 255, 0,255),   // magenta,5
		RGB( 255, 255, 0),  // yellow,6
		RGB( 255, 255, 255),// white,7

		RGB( 0, 0, 128),    // dark blue,8
		RGB( 0, 128, 0),    // dark green,9
		RGB( 0, 128,128),   // dark cyan,10
		RGB( 128, 0, 0),    // dark red, 11
		RGB( 128, 0, 128),  // dark magenta, 12
		RGB( 128, 128,0),   // dark yellow, 13
		RGB( 128, 128, 128),// dark gray,14
		RGB( 192, 192, 192) // bright gray,15
	};
*/

int jakost[11][9]={
{5,0,11,0,1,0,7,0},{4,0,10,0,12,2,6,8},{6,3,12,9,8,0,2,0},{7,2,1,8,4,12,10,6},
{1,8,7,2,10,0,4,0},{9,12,3,6,2,4,8,10},{10,11,4,5,7,0,1,0},{11,10,5,4,6,8,12,2},
{9,12,3,6,11,0,5,0},{1,8,7,2,3,0,9,0} };


for(i=0;i<=11;i++) for(j=0;j<=11;j++) akt[i][j]=0;
for(z=0;z<=9;z++) { kut[z]=objx[z+1]; retro[z]=objy[z+1];}
for(z=1;z<=6;z++) kut[9+z]=objx[20+z];
for(i=1;i<=6;i++) kut[i+9+6]=fnmo(kut[i+9]+180);




	//--------------------------
	// ekran - podešavanje



//	pDC->SetMapMode (MM_TEXT);
	pDC->SetMapMode (MM_ISOTROPIC);

	CRect rect;
	GetClientRect(rect);

	//CString bf1; bf1.Format("%d, %d",rect.Width(), rect.Height()  );
	//MessageBox(bf1);

if (pDC->IsPrinting() ==FALSE) {

    pDC->SetWindowExt(650*  m_dUlazDlg.m_iHorRez,650);   // logièke koordinate

	pDC->SetViewportExt(rect.Width()*0.91, rect.Width()/m_dUlazDlg.m_iHorRez*0.91 );  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike
																					  // bilo 564/988
	pDC->SetViewportOrg (rect.Width()*1.05/988*1000/5, (double)(rect.Width()*1.05/988) * 564/988* 10);  // pomakni poèetak k.s-a na dolje

}
else {

	pDC->SetWindowExt( 650* m_dUlazDlg.m_iHorRez,650);       // logièke koordinate
	pDC->SetViewportExt(3500, 2625);  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike
    pDC->SetViewportOrg (950,100);        // pomakni poèetak k.s-a na dolje
}

	CPen olovka(PS_SOLID, 1, Colors[14]);                 //tip, debljina, boja
	CPen* staraOlovka = pDC->SelectObject(&olovka);           // pohrani staru olovku
 
	CBrush* brush;
	brush = new CBrush(Colors[14]); //tip, boja
	CBrush* oldBrush =	pDC->SelectObject(brush);

   //pDC->Rectangle(0,0,640,640);                     // bilo do       639,347
	//Ellipse(pDC, 320,320,320,320,0,360);        // bilo radiusi  174, 1.32*173
	//pDC->Ellipse(0,0,640,640);

 
  CFont font1;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 12;                      // request a 12-pixel-height font
	strcpy(lf.lfFaceName, "Arial");        // request a face name "Arial"
	VERIFY(font1.CreateFontIndirect(&lf));  // create the font

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(Colors[14]);

	CFont* def_font = pDC->SelectObject(&font1);
	
	/*
	pDC->TextOut(310, -10, "N", 1);
	pDC->TextOut(310, 624, "S", 1);
	pDC->TextOut(-22, 310, "W", 1);
	pDC->TextOut(642, 310, "E", 1);

	pDC->SelectObject(def_font);
	font1.DeleteObject();
	*/

	CRect wn;
	this->GetWindowRect(wn);
		
	m_pdTijelo->SetWindowPos(&CWnd::wndTop, wn.right-120, wn.top-29, 120, wn.Height()+29, SWP_FRAMECHANGED    );


 BITMAP bm2;
 CBitmap *pOldBitmap;

 CDC MemDC;
 MemDC.CreateCompatibleDC( pDC );   // pomoæni panel za crtanje slièica

 CBitmap bmAspSlabi, bmAspJaca;
 bmAspSlabi.LoadBitmap(IDB_SLABI);
 bmAspJaca.LoadBitmap(IDB_JACA);


  CBitmap bmWheel1, * bmOldWheel1; 
 if (dcWheel1==NULL) dcWheel1.CreateCompatibleDC(pDC);    //èuva aspekte

  CBitmap bmWheel2b, * bmOldWheel2b; 
 if (dcWheel2b==NULL) dcWheel2b.CreateCompatibleDC(pDC);   //èuva kuæice

   CBitmap bmWheel2, * bmOldWheel2; 
 
 CDC dcWheel2;  //pomoæni za crtanje kuæica
 dcWheel2.CreateCompatibleDC(pDC);

 bmWheel1.CreateCompatibleBitmap (pDC, 640,640);  
 bmWheel2.CreateCompatibleBitmap (pDC, 640,640);  
 bmWheel2b.CreateCompatibleBitmap (pDC, 640,640);
 
  CDC AspDC;                              //pomoæni za crtanje
  AspDC.CreateCompatibleDC(pDC);

  if (AspDC2==NULL)   AspDC2.CreateCompatibleDC( pDC );   //panel s raportom aspekata
  
  CBitmap bmAsp, * bmOldAsp; 
  bmAsp.CreateCompatibleBitmap (pDC, 455,250);
  
  CBitmap bmAsp2, * bmOldAsp2; 
  bmAsp2.CreateCompatibleBitmap (pDC, 455,250);
  

  pOldBitmap = MemDC.SelectObject( &Slicka[46] );  //uzmi stari bitmap

  

//-----------------------
//aspekti

CPen olovkaAsp(PS_SOLID, 1, Colors[15]);                 //tip, debljina, boja
CPen* staraOlovkaAsp = AspDC.SelectObject(&olovkaAsp);           // pohrani staru olovku
bmOldAsp = AspDC.SelectObject(&bmAsp);

CBrush* aspbrush;
aspbrush = new CBrush(Colors[7]); //tip, boja
CBrush* oldaspBrush =	AspDC.SelectObject(aspbrush);
 
AspDC.FloodFill(2,2,Colors[7]);


xx=0; max=0;
for(z=0;z<=5;z++) {
	yy=0;
	if(	 !(
		   ( (z==0 || z==2 || z==3)  && bAsp[0]==FALSE ) 
		|| ( (z==1 || z==4 || z==5)  && bAsp[1]==FALSE ) 
		 ) 
	  ) for(i=0;i<9;i++) {

		if (bObj[i]==FALSE) continue;
		if(aspen==1 && z==5) continue;

		for (j=i+1;j<=9;j++) {

			if (bObj[j]==FALSE) continue;

			temp=fabs(kut[i]-kut[j]);
			flagsgn=1; if(temp>200) {temp=360-temp;flagsgn=-1;}
			temp=temp-asp[z];

			if(aspen==1) {

				temp=fabs(floor2(kut[i]/30)*30-floor2(kut[j]/30)*30);if(temp>180) temp=360-temp;

				if( temp==asp[z] ) 	 {
					temp=fabs(kut[i]-kut[j]); 
					if(temp>200) temp=360-temp;
					goto kkp;
				}
				else continue;
          }

			if (fabs(temp)<=orb[z] && fabs(temp)<=(porb[i]+porb[j])/2.) {
kkp:

			if (m_pdTijelo->m_iAspObj!=0 ) if (i!= m_pdTijelo->m_iAspObj -1 && j!=m_pdTijelo->m_iAspObj -1 ) continue;

			ic=sgn(temp)*((1-retro[i]-retro[j])*sgn(brz[j]-brz[i])+(retro[i]-retro[j]))*sgn(kut[i]-kut[j]);
			    yy++;

				MemDC.SelectObject( &Slicka[46+i] );
				Slicka[24+i].GetObject( sizeof( BITMAP ), &bm2 );

 				AspDC.BitBlt(3+xx*55,  15+(yy-1)*14 , //znak[i],
								bm2.bmWidth, bm2.bmHeight, 
								&MemDC, 0, 0,  SRCCOPY   );

				MemDC.SelectObject( &Slicka[46+j] );
				Slicka[24+j].GetObject( sizeof( BITMAP ), &bm2 );

 				AspDC.BitBlt(3+xx*55+35,  15+(yy-1)*14 , //znak[i],
								bm2.bmWidth, bm2.bmHeight, 
								&MemDC, 0, 0,  SRCCOPY   );

               akt[i][j]=z+1;

			    if(flagsgn*ic==-1) {MemDC.SelectObject(&bmAspSlabi);bmAspSlabi.GetObject( sizeof( BITMAP ), &bm2 );}
                if(flagsgn*ic==1) {MemDC.SelectObject(&bmAspJaca);bmAspJaca.GetObject( sizeof( BITMAP ), &bm2 );}
                AspDC.BitBlt(3+xx*55+16,15+(yy-1)*14,bm2.bmWidth, bm2.bmHeight,&MemDC, 0, 0,  MERGECOPY );
                /*
				sprintf(bf,"  ");
				if(flagsgn*ic==1) sprintf(bf,"%c%c",187,171);
                if(flagsgn*ic==-1) sprintf(bf,"%c%c",171,187);
                AspDC.SetTextColor(Colors[4]);
					 AspDC.TextOut(3+xx*55+17,11+(yy-1)*14,bf);
                AspDC.SetTextColor(Colors[15]);
				*/

          }
		}  //j
	}  //i
	if(yy>max) max=yy;

	xx++; if(aspen==1 && z==5) continue;

		MemDC.SelectObject( &Slicka[36+z] );
		Slicka[36+z].GetObject( sizeof( BITMAP ), &bm2 );

			AspDC.BitBlt((xx-1)*55+20, 2 , //znak[i],
 							  bm2.bmWidth, bm2.bmHeight, 
							  &MemDC, 0, 0,  SRCCOPY   );


} //z



if(bd==0) goto xx0;   //ako je bez kuæa

//  aspekti ascedenta i m.c.-a
yy=0;
if (bObj[10]!=FALSE) for (z=0;z<=5;z++) {

	if( ! (
		   ( (z==0 || z==2 || z==3)  && bAsp[0]==FALSE ) 
		|| ( (z==1 || z==4 || z==5)  && bAsp[1]==FALSE ) 
		 ) 
	  ) for (j=0;j<=9;j++) {

		if (bObj[j]==FALSE) continue;
	
		if(aspen==1 && z==5) continue;
		temp=fabs(kut[10]-kut[j]);
		flagsgn=1; if(temp>200) { temp=360-temp; flagsgn=-1;}
		temp=temp-asp[z];
		if(aspen==1) {
			 temp=fabs(floor2(kut[10]/30)*30-floor2(kut[j]/30)*30);if(temp>180) temp=360-temp;
			 if( temp==asp[z] ) goto kkp2;
          else continue;
      }
		if (fabs(temp)<=orb[z] && fabs(temp)<=(6+porb[j])/2.) {

kkp2:

			if (m_pdTijelo->m_iAspObj!=0 ) if ( m_pdTijelo->m_iAspObj -1!=10 && j!=m_pdTijelo->m_iAspObj -1 ) continue;
			ic=sgn(temp)*(1-2*retro[j])*sgn(kut[10]-kut[j]);
			yy++;

				MemDC.SelectObject( &Slicka[36+z] );
				Slicka[36+z].GetObject( sizeof( BITMAP ), &bm2 );

 				AspDC.BitBlt(3+xx*55,  15+(yy-1)*14 , //znak[i],
								bm2.bmWidth, bm2.bmHeight, 
								&MemDC, 0, 0,  SRCCOPY   );

				MemDC.SelectObject( &Slicka[46+j] );
				Slicka[24+j].GetObject( sizeof( BITMAP ), &bm2 );

 				AspDC.BitBlt(3+xx*55+15,  15+(yy-1)*14 , //znak[i],
								bm2.bmWidth, bm2.bmHeight, 
								&MemDC, 0, 0,  SRCCOPY   );

				
					 akt[j][10]=z+1;

				/*
				sprintf(bf,"  ");
                if(flagsgn*ic==1) sprintf(bf,"%c%c",187,171);
                if(flagsgn*ic==-1) sprintf(bf,"%c%c",171,187);

                AspDC.SetTextColor(Colors[4]);
					 AspDC.TextOut(3+xx*55+31+9,11+(yy-1)*14,bf);
				*/
				AspDC.SetTextColor(Colors[15]);
				AspDC.TextOut(3+xx*55+33,11+(yy-1)*14,"(      )");

                if(flagsgn*ic==-1) {MemDC.SelectObject(&bmAspSlabi);bmAspSlabi.GetObject( sizeof( BITMAP ), &bm2 );}
                if(flagsgn*ic==1) {MemDC.SelectObject(&bmAspJaca);bmAspJaca.GetObject( sizeof( BITMAP ), &bm2 );}
                AspDC.BitBlt(3+xx*55+40,15+(yy-1)*14,bm2.bmWidth, bm2.bmHeight,&MemDC, 0, 0,  MERGECOPY );
 


      }
	}	 //j
} //z


if(yy>max) max=yy;
flag=0;if(yy!=0) flag=1;
yy=0;

if (bObj[10]!=FALSE) for (z=0;z<=5;z++) {
	if( ! (
		   ( (z==0 || z==2 || z==3)  && bAsp[0]==FALSE ) 
		|| ( (z==1 || z==4 || z==5)  && bAsp[1]==FALSE ) 
		 ) 
	  ) for (j=0;j<=9;j++) {

		if (bObj[j]==FALSE) continue;


		if(aspen==1 && z==5) continue;
		if(aspen==1) {
			  temp=fabs((kut[19]/30)*30-floor2(kut[j]/30)*30);if(temp>180) temp=360-temp;
				if( temp==asp[z] ) goto kkp3;
            else continue;
      }
		temp_ic=fabs(kut[9+10]-kut[j]);
		if(temp_ic>200) temp_ic=360-temp_ic;
		temp_ic=temp_ic-asp[z];

		if (fabs(temp_ic)<=orb[z] && fabs(temp_ic)<=(6+porb[j])/2.) {
kkp3:


 			if (m_pdTijelo->m_iAspObj!=0 ) if ( m_pdTijelo->m_iAspObj -1!=10 && j!=m_pdTijelo->m_iAspObj -1 ) continue;

			yy++;


				MemDC.SelectObject( &Slicka[36+z] );
				Slicka[36+z].GetObject( sizeof( BITMAP ), &bm2 );

 				AspDC.BitBlt(3+xx*55+70,  15+(yy-1)*14 , //znak[i],
								bm2.bmWidth, bm2.bmHeight, 
								&MemDC, 0, 0,  SRCCOPY   );

				MemDC.SelectObject( &Slicka[46+j] );
				Slicka[24+j].GetObject( sizeof( BITMAP ), &bm2 );

 				AspDC.BitBlt(3+xx*55+90,  15+(yy-1)*14 , //znak[i],
								bm2.bmWidth, bm2.bmHeight, 
								&MemDC, 0, 0,  SRCCOPY   );

			    akt[j][11]=z+1;
		}
	} //j
}//z

if(yy>max) max=yy;

xx0:

AspDC.SetTextColor(Colors[14]);
AspDC.TextOut(xx*55+70+15,-3,"MC");
AspDC.TextOut(xx*55+15,-3,"ASC");

AspDC.MoveTo(0,0); AspDC.LineTo(0,454);
AspDC.MoveTo(455,0); AspDC.LineTo(455,max*14+20);
AspDC.MoveTo(455,max*14+20); AspDC.LineTo(0,max*14+20);
AspDC.MoveTo(0,max*14+20); AspDC.LineTo(0,0);


AspDC.MoveTo(0,12); AspDC.LineTo(455,12);
AspDC.MoveTo(xx*55+70,0); AspDC.LineTo(xx*55+70,max*14+20);

for(i=1;i<=6;i++) {
	AspDC.MoveTo(i*55,0); AspDC.LineTo(i*55,max*14+20);
}

// skidanje slike tablice aspekata

	bmOldAsp2 = AspDC2.SelectObject(&bmAsp2);
	AspDC2.BitBlt(0, 0, 455,250, &AspDC, 0,0,	SRCCOPY   );  

//getimage(0,0,455,max*12+20,aspr);

m_pdTablicaAspekata->m_bPaste=TRUE; 
m_pdTablicaAspekata->SetWindowPos(&CWnd::wndTop, 
								  m_pdTablicaAspekata->m_rSlika.left, m_pdTablicaAspekata->m_rSlika.top,
								  456,(max+1)*14+30,SWP_FRAMECHANGED );
m_pdTablicaAspekata->Invalidate();


//oèisti sve korisštene ðiðe od aspekata:

AspDC.SelectObject( bmOldAsp );
AspDC.SelectObject( oldaspBrush);
AspDC.SelectObject(staraOlovkaAsp);      
delete aspbrush;
DeleteDC(AspDC);


//-----------------------------------------
//ispis toènih položaja planeta
	
	pDC->SelectObject(def_font);
	font1.DeleteObject();

	lf.lfHeight = 17;                      // request a 12-pixel-height font
	lf.lfWeight = FW_BOLD;
	VERIFY(font1.CreateFontIndirect(&lf));  // create the font

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(Colors[14]);

	def_font = pDC->SelectObject(&font1);
	


	pDC->MoveTo(147-210,0); 	pDC->LineTo(147-210,15+9*20);

for(z=0;z<=9;z++) {
	temp=kut[z]-floor2(kut[z]/30)*30;

	MemDC.SelectObject( &Slicka[46+z] );
	Slicka[46+z].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(-210,z*20, 19,12,  &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight, SRCCOPY );
	
	sprintf(bf,"=> %02d%c%02d'",(int)floor2(temp),186,(int)( (temp-floor2(temp) )*60) ); 
	pDC->TextOut(25-210,z*20-3,bf);
	if(retro[z]==1) pDC->TextOut(136-210,z*20-3,"r",1);

	MemDC.SelectObject( &Slicka[12+(int)floor2(kut[z]/30)] );
	Slicka[12+(int)floor2(kut[z]/30)].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(90-210,z*20, 19,12,  &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight, SRCCOPY );
	
	
	for(j=0;j<=3;j++) 
		if(floor2(kut[z]/30)==jakost[z][j*2]-1 || floor2(kut[z]/30)==jakost[z][j*2+1]-1 ) {
			MemDC.SelectObject( &Slicka[42+j] );
			Slicka[42+j].GetObject( sizeof( BITMAP ), &bm2 );
 			pDC->StretchBlt(114-210,z*20,19,12,  &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight,  SRCCOPY );
			
		}


	//treæina
	MemDC.SelectObject( &Slicka[12+(int)fmod(4*floor2((kut[z]-floor2(kut[z]/30)*30)/10)+floor2(kut[z]/30),12)] );
	Slicka[12+(int)fmod(4*floor2((kut[z]-floor2(kut[z]/30)*30)/10)+floor2(kut[z]/30),12)].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(152-210,z*20,19,12,    &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight, SRCCOPY );

	//dvanaestina
	MemDC.SelectObject( &Slicka[12+(int)fmod(floor2((kut[z]-floor2(kut[z]/30)*30)/2.5)+floor2(kut[z]/30),12)] );
	Slicka[12+(int)fmod(floor2((kut[z]-floor2(kut[z]/30)*30)/2.5)+floor2(kut[z]/30),12)].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(174-210,z*20,19,12,    &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight, SRCCOPY );
	
} //z


	temp=objx[12]-floor2(objx[12]/30)*30; // cvorovi

	MemDC.SelectObject( &Slicka[46+11] );
	Slicka[46+11].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(0-210,10*20+5,19,12,   &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight,  SRCCOPY );

	sprintf(bf,"=> %02d%c%02d'",(int)floor2(temp),186,(int)((temp-floor2(temp))*60) ); 
	pDC->TextOut(25-210,10*20+1,bf);
	if(objy[12]==1) pDC->TextOut(90-210,10*20,"r");

	MemDC.SelectObject( &Slicka[12+ (int)floor2(objx[12]/30)] );
	Slicka[12+ (int)floor2(objx[12]/30) ].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(90-210,10*20+5,19,12,   &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight,  SRCCOPY );
	
	if(bd>0) {        //ascendent
		temp=kut[9+1]-floor2(kut[9+1]/30)*30;		
		sprintf(bf,"ASC=> %02d%c%02d'",(int)floor2(temp),186,(int)((temp-floor2(temp))*60) );
		pDC->TextOut(0-200,340-1,bf);
		
		MemDC.SelectObject(12+ &Slicka[ (int)(kut[9+1]/30) ] );
		Slicka[12+ (int)(kut[9+1]/30) ].GetObject( sizeof( BITMAP ), &bm2 );
 		pDC->StretchBlt(92-200,340,19,12,  &MemDC, 0, 0,  bm2.bmWidth, bm2.bmHeight,  SRCCOPY );
		
		pDC->MoveTo(116-200,340);pDC->LineTo(116-200,15+340);
		
		MemDC.SelectObject( &Slicka[12+ (int)fmod(4*floor2((kut[9+1]-floor2(kut[9+1]/30)*30)/10)+floor2(kut[9+1]/30),12) ] );
		Slicka[12+ (int)fmod(4*floor2((kut[9+1]-floor2(kut[9+1]/30)*30)/10)+floor2(kut[9+1]/30),12) ].GetObject( sizeof( BITMAP ), &bm2 );
 		pDC->StretchBlt(122-200,340,19,12,    &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight, SRCCOPY );
		
		MemDC.SelectObject( &Slicka[12+ (int)fmod(floor2((kut[9+1]-floor2(kut[9+1]/30)*30)/2.5)+floor2(kut[9+1]/30),12)] );
		Slicka[12+ (int)fmod(floor2((kut[9+1]-floor2(kut[9+1]/30)*30)/2.5)+floor2(kut[9+1]/30),12) ].GetObject( sizeof( BITMAP ), &bm2 );
 		pDC->StretchBlt(144-200,340,19,12,    &MemDC, 0, 0,bm2.bmWidth, bm2.bmHeight,  SRCCOPY );


	}


	 //više nije potrebno, sad je po defaultu crno
	/*
	if(clor==0) {  //prefarbaj u crno
		for(j=0;j<=9*13;j++) {
			for(i=-120;i<=0;i++) if(pDC->GetPixel(i,j)!=Colors[7]) pDC->SetPixel(i,j,Colors[0]);
		}
		for(j=335;j<=350;j++) {
			for(i=-120;i<=0;i++) if(pDC->GetPixel(i,j)!=Colors[7]) pDC->SetPixel(i,j,Colors[0]);
		}


	}
	*/


	/*
	*/




//ispis helpa

	CString sOpisi[12]={"sunce","mjesec","merkur","venera","mars","jupiter","saturn","uran","neptun","pluton","lilit","sj.cvor"};
	CString sOpisi2[12]={"ovan","bik","blizanci","rak","lav","djevica","vaga","škorpion","strijelac","jarac","vodenjak","ribe"};
	CString sOpis;
	pDC->TextOut(655,0," POMOC :");
	pDC->TextOut(655,12,"-----------");
	
	pDC->TextOut(655,40," planete :");
	pDC->TextOut(655,52,"-----------");


	for(z=0;z<=11;z++) {

	MemDC.SelectObject( &Slicka[46+z] );
	Slicka[46+z].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(655,70+z*20, 19,12 ,  &MemDC, 0, 0,  bm2.bmWidth, bm2.bmHeight,SRCCOPY );
	
	sOpis=" - " + sOpisi[z];
	 
	pDC->TextOut(655+20,70+z*20-1,sOpis);

}
	pDC->TextOut(655,340," znakovi :");
	pDC->TextOut(655,352,"-----------");

	for(z=0;z<=11;z++) {

	MemDC.SelectObject( &Slicka[12+z] );
	Slicka[12+z].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(655,370+(z)*20, 19,12 ,  &MemDC, 0, 0,  bm2.bmWidth, bm2.bmHeight,SRCCOPY );
	
	sOpis=" - " + sOpisi2[z];
	 
	pDC->TextOut(655+20,370+(z)*20-1,sOpis);

}

CPen tankaOlovka(PS_SOLID, 1, Colors[14]);                 //tip, debljina, boja
CPen debelaOlovka(PS_SOLID, 2, Colors[14]);                 //tip, debljina, boja
CPen svjetlaOlovka(PS_SOLID, 2, Colors[15]);                 //tip, debljina, boja
CPen svjetlaTankaOlovka(PS_SOLID, 1, Colors[15]);                 //tip, debljina, boja











//---------------------------------
// glavni ekran


fi=180-kut[10];
if(bd==0) fi=0;


Ellipse(pDC, 320,320,320,320, 0,360);  //170
Ellipse(pDC, 320,320,264,264, 0,360);  //140
Ellipse(pDC, 320,320,282,282, 0,360);  //150

for(i=0;i<360;i+=30) rad(pDC,320,320,264,321,i,fi);
for(i=0;i<360;i+=10) rad(pDC,320,320,264,275,i,fi);
for(i=0;i<360;i+=5)  rad(pDC,320,320,264,269,i,fi);


   for(i=0;i<12;i++) {

	delete brush;
	brush = new CBrush( Colors [  Boja[(int)fmod2(i,4)]  ]); 
	pDC->SelectObject(brush);


    
		pDC->FloodFill(313+301 * cos((i*30+fi+15)*pi/180),
			            322-301 * sin((i*30+fi+15)*pi/180),
							Colors[14] );

		// iscrtaj znakove

		MemDC.SelectObject( &Slicka[12+i] );
		Slicka[12+i].GetObject( sizeof( BITMAP ), &bm2 );

 		/*
		pDC->BitBlt( 313+301*cos((i*30+fi+15)*pi/180),
						 322-301*sin((i*30+fi+15)*pi/180), //znak[i],
						 bm2.bmWidth, bm2.bmHeight, 
						 &MemDC, 0, 0,  SRCAND                            );
		*/
		pDC->StretchBlt( 313+301*cos((i*30+fi+15)*pi/180),
						 322-301*sin((i*30+fi+15)*pi/180), //znak[i],
						 17,10,
						 &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight,  SRCAND                            );


		
	}



//------------------------------------
// aspekti - iscrtavanje linija


if(mod!=0 || bd==0) {  

	CPen dobri_asp(PS_SOLID, 1, Colors[2]);                 //tip, debljina, boja
	CPen losi_asp(PS_SOLID, 1, Colors[4]);                 //tip, debljina, boja

	pDC->SelectObject(&dobri_asp);           
	pDC->MoveTo(0-210,323-60); pDC->LineTo(36-210,323-60);
	pDC->TextOut(48-210,316-60,"Dobri aspekti");

	pDC->SelectObject(&losi_asp);           
	pDC->MoveTo(0-210,341-60); pDC->LineTo(36-210,341-60);
	pDC->TextOut(48-210,334-60,"Losi aspekti");

	for(i=0;i<9;i++) {
		for (j=i+1;j<=9;j++) {
			temp=akt[i][j];
			if(temp>0) {
				if(temp==3 || temp==4 || temp==1) {pDC->SelectObject(&dobri_asp); }
				else                   {pDC->SelectObject(&losi_asp); }
				pDC->MoveTo(320+256*cos((kut[i]+fi)*pi/180),320-256*sin((kut[i]+fi)*pi/180));
				pDC->LineTo(320+256*cos((kut[j]+fi)*pi/180),320-256*sin((kut[j]+fi)*pi/180));
			}
		}
	}

	if(bd>0) {
		float fx,fy;
		
		for (j=0;j<=1;j++) {
			for (i=0;i<=9;i++) {
				temp=akt[i][10+j];
				if(temp>0) {
					if(temp==3 || temp==4 || temp==1) {pDC->SelectObject(&dobri_asp); }
					else                   {pDC->SelectObject(&losi_asp); }
					pDC->MoveTo(320+262*cos((kut[10+j*9]+fi)*pi/180),320-262*sin((kut[10+j*9]+fi)*pi/180));
					pDC->LineTo(320+256*cos((kut[i]+fi)*pi/180),320-256*sin((kut[i]+fi)*pi/180));
				}
			}
		}

		fx=320+263*cos((fi+kut[19])*pi/180);
		fy=320-263*sin((fi+kut[19])*pi/180);

		pDC->SelectObject(&olovka);                             //iscrtavanje oznake MC-a
		pDC->MoveTo(fx-4,fy-4); pDC->LineTo(fx+4,fy+4);
		pDC->MoveTo(fx+4,fy-4); pDC->LineTo(fx-4,fy+4); 
		pDC->TextOut(fx-2,fy+5,"MC");
	}
	
}

for(i=0;i<=10;i++) objx[100+i]=objx[i];
objx[100+11]=kut[9+1];
objx[100+12]=kut[9+10];

for(i=0;i<=9;i++) objx[i+1]=kut[i];

for(i=0;i<=9;i++) {
	mn=1000;
	for(j=0;j<=9;j++) {
		if(objx[j+1]<mn) { mn=kut[j]; kk=j; }
	}
	objx[40+i]=mn; objx[60+i]=kk; objx[kk+1]=1000;
}



//---------------------------------
// iscrtavanje strelice ascendenta

if(bd>0) { 

	pDC->MoveTo(313+340*cos(pi),320-340*sin(pi));
	pDC->LineTo(313+322*cos((180-1.2)*pi/180),320-322*sin((180-1.0)*pi/180));

	pDC->MoveTo(313+340*cos(pi),320-340*sin(pi));
	pDC->LineTo(313+322*cos((180+1.2)*pi/180),320-322*sin((180+1.0)*pi/180));

	pDC->MoveTo(313+340*cos(pi),320-340*sin(pi));
	pDC->LineTo(313+313*cos(pi),320-313*sin(pi));

}



// crtanje planeta
xx2:

	delete brush;
	brush = new CBrush( Colors[14] ); 
	pDC->SelectObject(brush);

r=0;
for(i=0;i<=9;i++) {    //nacrtaj krugove planeta

		if (bObj[ (int)objx[60+i] ]==FALSE) continue;
		//else { CString bf5; bf5.Format("TRUE za %d",i); AfxMessageBox(bf5);}

		pDC->SelectObject(&debelaOlovka);    
      //Ellipse(pDC, 320+257*cos((objx[40+i]+fi)*pi/180),320-257*sin((objx[40+i]+fi)*pi/180), 5,5,0,360);
		pDC->Ellipse(320+257*cos((objx[40+i]+fi)*pi/180)-4,320-257*sin((objx[40+i]+fi)*pi/180)-4,
						 320+257*cos((objx[40+i]+fi)*pi/180)+4,320-257*sin((objx[40+i]+fi)*pi/180)+4);
      
		//pDC->FloodFill(320+256*cos((objx[40+i]+fi)*pi/180),320-256*sin((objx[40+i]+fi)*pi/180), Colors[14]);

		//Ellipse(pDC, 320+256*cos((objx[40+i]+fi)*pi/180),320-256*sin((objx[40+i]+fi)*pi/180), 1,1,0,360);
		//pDC->SetPixel(320+256*cos((objx[40+i]+fi)*pi/180),320-256*sin((objx[40+i]+fi)*pi/180), Colors[0] );

	   if(i==0) goto ln12;

		if(fabs(objx[40+i]-objx[40+i-1])<8) { 
			r++;
			if(r==3) r=0; 
		}
		else r=0;
ln12:
		MemDC.SelectObject( &Slicka[ 46+ (int)objx[60+i] ] );   //metni ikone planeta
		Slicka[ 46 + (int)objx[60+i] ].GetObject( sizeof( BITMAP ), &bm2 );
 		//pDC->BitBlt(315+(230-r*18)*cos((objx[40+i]+fi)*pi/180),320-(230-r*18)*sin((objx[40+i]+fi)*pi/180), bm2.bmWidth, bm2.bmHeight,  &MemDC, 0, 0,  SRCCOPY );
		pDC->StretchBlt(315+(230-r*18)*cos((objx[40+i]+fi)*pi/180),320-(230-r*18)*sin((objx[40+i]+fi)*pi/180), 
			19,11,  &MemDC, 0, 0,  bm2.bmWidth, bm2.bmHeight, SRCCOPY );


		if(retro[(int)objx[60+i]]==1) {       //postavi oznaku za retrogradne planete
			pDC->SelectObject(&tankaOlovka);    
			pDC->MoveTo(315+   (230-r*18)*cos((objx[40+i]+fi)*pi/180),320-5-(230-r*18)*sin((objx[40+i]+fi)*pi/180));
			pDC->LineTo(315+14+(230-r*18)*cos((objx[40+i]+fi)*pi/180),320-5-(230-r*18)*sin((fi+objx[40+i])*pi/180));
		}
}



//----------------------------------
// skidanje slike kola s aspektnim linijama

	bmOldWheel1 = dcWheel1.SelectObject(&bmWheel1);
	dcWheel1.BitBlt(0, 0, 640,640, pDC, 0,0,	SRCCOPY   );  

//---------------------------------------
// kucice


	CPen tankaOlovkaW2(PS_SOLID, 1, Colors[14]);                 //tip, debljina, boja
	CPen debelaOlovkaW2(PS_SOLID, 2, Colors[14]);                 //tip, debljina, boja
	CPen svjetlaOlovkaW2(PS_SOLID, 2, Colors[15]);                 //tip, debljina, boja
	CPen* staraOlovkaW2 = dcWheel2.SelectObject(&debelaOlovkaW2);           // pohrani staru olovku
 
	CBrush* brushW2;
	brushW2 = new CBrush(Colors[14]); //tip, boja
	CBrush* oldBrushW2 =	dcWheel2.SelectObject(brushW2);
	dcWheel2.SetBkMode(TRANSPARENT);

	bmOldWheel2 = dcWheel2.SelectObject(&bmWheel2);
	dcWheel2.BitBlt(0,0, 640, 640, &dcWheel1, 0, 0, SRCCOPY  );
 
if(bd==0) goto xx3;


for(i=0; i<=9; i++ ) {
	int fnd;
	mn=1000; fnd=0;
	for(j=1; j<=12; j++) if(kut[i]>=kut[9+j] && fabs(kut[i]-kut[9+j])<mn) { mn=fabs(kut[i]-kut[9+j]); fnd=j; }
	if(fnd==0) {
		mn=0;
		for(j=1;j<=12;j++) if(kut[9+j]>mn) {mn=kut[9+j]; fnd=j; }
	}
	kuca_planete[i]=fnd;
	if(vl_kuce[(fnd-1)*2]==i+1 || vl_kuce[(fnd-1)*2+1]==i+1) nagl_kuca[fnd]=1;
}



//if(mod!=0) goto xx2;    //mod=1 , pa preskaèe crtanje kuæa

for(i=1;i<=6;i++) {   //crtaj kuæe
 
	 dcWheel2.MoveTo(320+264*cos((fi+kut[9+i])*pi/180),320-264*sin((fi+kut[9+i])*pi/180));
	 dcWheel2.LineTo(320+264*cos((fi+kut[9+i]+180)*pi/180),320-264*sin((fi+kut[9+i]+180)*pi/180));
}

  for(i=0;i<=11;i++) {   // pofarbaj kuæe
 
  	delete brushW2;
	brushW2 = new CBrush( Colors [  Boja[(int)fmod2(i,4)]  ]); 
	dcWheel2.SelectObject(brushW2);
	dcWheel2.FloodFill(320+110*cos((fi+kut[10+i]+2)*pi/180),320-110*sin((fi+kut[10+i]+2)*pi/180), Colors[14] );

}

for(i=0;i<=11;i++) {     // ispis brojeva kuæa i zaokruživanje onih koje su naglašene
	kkk=kut[10+i]+fi;
	if (kkk>360) kkk-=360;
	if (kkk<0) kkk+=360;
	sprintf(bf,"%d.",i+1);
	
	if ((kkk)>180 && (kkk)<270) {
		
		dcWheel2.TextOut(320+110*cos((fi+kut[10+i]+2)*pi/180),320-110*sin((fi+kut[10+i]+2)*pi/180),bf);
		
		if(nagl_kuca[i+1]==1) 
			Ellipse(&dcWheel2, 326+110*cos((fi+kut[10+i]+2)*pi/180),326-110*sin((fi+kut[10+i]+2)*pi/180),22,15,0,360);
	}
	else if ( (kkk)>30 && (kkk)<135 && i>8) {
		dcWheel2.TextOut(320+110*cos((fi+kut[10+i]+16)*pi/180),320-110*sin((fi+kut[10+i]+16)*pi/180),bf);
		if(nagl_kuca[i+1]==1) 
			Ellipse(&dcWheel2, 326+110*cos((fi+kut[10+i]+16)*pi/180),326-110*sin((fi+kut[10+i]+16)*pi/180),22,15,0,360);
	}
	else {
		dcWheel2.TextOut(320+110*cos((fi+kut[10+i]+10)*pi/180),320-110*sin((fi+kut[10+i]+10)*pi/180),bf);
		if(nagl_kuca[i+1]==1) 
			Ellipse(&dcWheel2, 326+110*cos((fi+kut[10+i]+10)*pi/180),326-110*sin((fi+kut[10+i]+10)*pi/180),22,15,0,360);
	}
}


	// crtanje planeta još jednom

	delete brushW2;
	brushW2 = new CBrush( Colors[15] ); 
	dcWheel2.SelectObject(brushW2);

r=0;
for(i=0;i<=9;i++) {    //nacrtaj krugove planeta

		if (bObj[(int)objx[60+i]]==FALSE) continue;
		//else { CString bf5; bf5.Format("TRUE za %d",i); AfxMessageBox(bf5);}

		dcWheel2.SelectObject(&svjetlaOlovkaW2);    
      //Ellipse(pDC, 320+257*cos((objx[40+i]+fi)*pi/180),320-257*sin((objx[40+i]+fi)*pi/180), 5,5,0,360);
		dcWheel2.Ellipse(320+257*cos((objx[40+i]+fi)*pi/180)-4,320-257*sin((objx[40+i]+fi)*pi/180)-4,
						 320+257*cos((objx[40+i]+fi)*pi/180)+4,320-257*sin((objx[40+i]+fi)*pi/180)+4);
      
		//pDC->FloodFill(320+256*cos((objx[40+i]+fi)*pi/180),320-256*sin((objx[40+i]+fi)*pi/180), Colors[14]);

		//Ellipse(pDC, 320+256*cos((objx[40+i]+fi)*pi/180),320-256*sin((objx[40+i]+fi)*pi/180), 1,1,0,360);
		//pDC->SetPixel(320+256*cos((objx[40+i]+fi)*pi/180),320-256*sin((objx[40+i]+fi)*pi/180), Colors[0] );

	   if(i==0) goto ln122;

		if(fabs(objx[40+i]-objx[40+i-1])<8) { 
			r++;
			if(r==3) r=0; 
		}
		else r=0;
ln122:
		MemDC.SelectObject( &Slicka[ 46+ (int)objx[60+i] ] );   //metni ikone planeta
		Slicka[ 46 + (int)objx[60+i] ].GetObject( sizeof( BITMAP ), &bm2 );
 		dcWheel2.StretchBlt(315+(230-r*18)*cos((objx[40+i]+fi)*pi/180),320-(230-r*18)*sin((objx[40+i]+fi)*pi/180), 
			19,11,   &MemDC, 0, 0,  bm2.bmWidth, bm2.bmHeight,SRCAND             );


		if(retro[(int)objx[60+i]]==1) {       //postavi oznaku za retrogradne planete
			dcWheel2.SelectObject(&tankaOlovka);    
			dcWheel2.MoveTo(315+   (230-r*18)*cos((objx[40+i]+fi)*pi/180),320-5-(230-r*18)*sin((objx[40+i]+fi)*pi/180));
			dcWheel2.LineTo(315+14+(230-r*18)*cos((objx[40+i]+fi)*pi/180),320-5-(230-r*18)*sin((fi+objx[40+i])*pi/180));
		}
}


xx3:


// skidanje slike kola s kuæicama

	bmOldWheel2b = dcWheel2b.SelectObject(&bmWheel2b);
	dcWheel2b.BitBlt(0, 0, 640,640, &dcWheel2, 0,0,	SRCCOPY   );  




//oèisti sve korisštene ðiðe od kuæica
dcWheel2.SelectObject( bmOldWheel2 );
delete brushW2;
dcWheel2.SelectObject( oldBrushW2);
dcWheel2.SelectObject(staraOlovkaW2);      
DeleteDC(dcWheel2);





	
// ako treba vrati aspekte
	
	if (m_bAspekti == TRUE)  pDC->BitBlt(0,0, 640, 640, &dcWheel1, 0, 0, SRCCOPY  );
	else pDC->BitBlt(0,0, 640, 640, &dcWheel2b, 0, 0, SRCCOPY  );
	
//----------------------------------------------------------------------------------------------
  //iscrtaj dispozitore
	Dispozitori();

	/*
	CString sx,sbuf=""; for (z=1; z<=10;z++) { sx.Format("%d",veza[z]); sbuf=sbuf+ sx + ", ";}
	pDC->TextOut(200,400-100, sbuf);

	sbuf="";for (z=1; z<=10;z++) { sx.Format("%d",broj_veza[z]); sbuf=sbuf+ sx + ", ";}
	pDC->TextOut(200,400-80, sbuf);
	*/

	//iscrtaj strelice:
	int x1d, y1d, x2d, y2d, x3d, y3d, x0d=-210 , y0d=410;
	double fid;

	pDC->TextOut(x0d,y0d-30," dispozicija :");

	for (z=1; z<=10; z++) {

		x1d= x0d + 32*(int)xp[(int)z]+9;
		y1d= y0d + 40*(int)yp[(int)z]+7;

		if(z==veza[z] || veza[z]==0 ) {

			continue;
		}

		x2d= x0d + 32*(int)xp[veza[z]]+9;
		y2d= y0d + 40*(int)yp[veza[z]]+7;


        fid=atan2((double)(y2d-y1d),(double)(x2d-x1d)); if (fid<0) fid=2*pi + fid;
		

		x1d= x1d + (int)(11*cos(fid));
		y1d= y1d + (int)(11*sin(fid));

		fid = pi + fid; 

		x2d= x2d + (int)(11*cos(fid));
		y2d= y2d + (int)(11*sin(fid));

 		pDC->SelectObject(&svjetlaTankaOlovka);           

		//pDC->MoveTo((int)x1d,(int)y1d); 	pDC->LineTo((int)x2d,(int)y2d);

		ArrowTo(pDC, x1d,y1d,x2d,y2d,8,25);

	}


	for (z=1; z<=10; z++) {    //iscrtaj planete
		MemDC.SelectObject( &Slicka[46+z-1] );
		Slicka[46+z-1].GetObject( sizeof( BITMAP ), &bm2 );
 		pDC->StretchBlt(x0d + 32*xp[z] ,y0d+40*yp[z], 19,12,  &MemDC, 0, 0, bm2.bmWidth,bm2.bmHeight,  SRCCOPY );
	}


	for (z=1; z<=10; z++) {   //zaokruži disp.

		x1d= x0d + 32*(int)xp[(int)z]+10;
		y1d= y0d + 40*(int)yp[(int)z]+7;

		if(   (  z==veza[z] || veza[z]==0 || z==veza[veza[z]]   ) && broj_veza[z]>0                   ) {

			if ( broj_veza[z]>0 ) {
				pDC->SelectObject(&svjetlaOlovka);           
				Ellipse(pDC,x1d-1,y1d-1, 12,12,360,0);
			}
		}
	}

	pDC->SelectObject(&olovka);





//oèisti sve korisštene ðiðe od MemDC pomoænog cdc-
MemDC.SelectObject( pOldBitmap );
DeleteDC(MemDC);
	


//oèisti sve korisštene ðiðe od glavnog dc-a
pDC->SelectObject(oldBrush);
delete brush;
pDC->SelectObject(staraOlovka);  // vrati staru olovku

pDC->SelectObject(def_font);
font1.DeleteObject();


}
//********************************************************************************************************
void CAsc2winView::Kuce(double asce, double ra, double ob, int cmd)
{

double  ad,b4,oa,sa,ax,aa,ko,ab,lo;
int i=0;

double duz, sir;

duz = m_dUlazDlg.m_fDuz;
sir = m_dUlazDlg.m_fSir;


if(cmd==0) { for(i=0;i<=5;i++) objx[21+i]=fnmo(asce+i*30); return; } 

b4=pi/180*sir;
ad=fnx(sin(ra)*tan(fabs(ob))*tan(b4));
oa=ra-ad;
sa=pi/2+ad; 
ax=sa/3; 

for(i=0;i<=5;i++) {
    ko=pi/180*fnmo(180/pi*(oa+ax*i));
    aa=atan(tan(b4)/cos(ko));
    ab=aa+fabs(ob);
    lo=atan(tan(ko)*cos(aa)/cos(ab)); if(lo<0) lo+=pi;
    if(sin(ko)<0) lo+=pi;

    objx[30+i+1]=fnmo(180/pi*lo);
}
objx[20+1]=objx[30+4];
objx[20+2]=objx[30+5];
objx[20+3]=objx[30+6];
objx[20+4]=objx[30+1]-180;
objx[20+5]=objx[30+2]-180;
objx[20+6]=objx[30+3]-180;
for(i=1;i<=6;i++) objx[20+i]=fnmo(objx[20+i]);
}


//***************************************************************************************************
double CAsc2winView::fnx(double x)
{
 return(atan(x/sqrt(1-x*x))); 
}


//***************************************************************************************************
void CAsc2winView::rad(CDC * pDC, int x0, int y0, int r1, int r2, double alfa, double fi)
{

int x1,y1,x2,y2;

alfa+=fi;

x1=x0+r1*cos(alfa*3.14159265/180.);
x2=x0+r2*cos(alfa*3.14159265/180.);
y1=y0-r1*sin(alfa*3.14159265/180.);
y2=y0-r2*sin(alfa*3.14159265/180.);

pDC->MoveTo(x1,y1); pDC->LineTo(x2,y2);
return;

}

//********************************************************************
void CAsc2winView::OnAspekti() 
{

	int nIndex = iStvarniIndex[( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.GetCurSel()];   //Handle na CMainFrame objekt!!!!!!

	if (nIndex == 4) {
	
	// TODO: Add your command handler code here
	m_bAspekti = ! ( m_bAspekti);

	CDC * pDC=this->GetDC();

	pDC->SetMapMode (MM_ISOTROPIC);

	CRect rect;
	GetClientRect(rect);

	pDC->SetWindowExt( 650* m_dUlazDlg.m_iHorRez,650);   // logièke koordinate
	pDC->SetViewportExt(rect.Width()*0.91, rect.Width()/m_dUlazDlg.m_iHorRez*0.91 );  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike
	
	pDC->SetViewportOrg (rect.Width()*1.05/988*1000/5, (double)(rect.Width()*1.05/988) * 564/988* 10);  // pomakni poèetak k.s-a na dolje



	if( m_bAspekti == TRUE)	 pDC->BitBlt(0,0, 640, 640, &dcWheel1, 0, 0, SRCCOPY  );
	else                     pDC->BitBlt(0,0, 640, 640, &dcWheel2b, 0, 0, SRCCOPY  );


	}

}

//***************************************************************************
void CAsc2winView::OnUpdateAspekti(CCmdUI* pCmdUI) 
{

	int nIndex = iStvarniIndex[( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.GetCurSel()];   //Handle na CMainFrame objekt!!!!!!

		if (nIndex == 4) {

			if( m_bAspekti == TRUE) pCmdUI->SetCheck(1);
			else pCmdUI->SetCheck(0);

			pCmdUI->Enable(TRUE);
		}
		else pCmdUI->Enable(FALSE);
}

//****************************************************************************
void CAsc2winView::OnUpdateAspTablica(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

int nIndex = iStvarniIndex[( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.GetCurSel()];   //Handle na CMainFrame objekt!!!!!!

		if (nIndex == 4 || nIndex == 5) {

			if( m_bAspektiTablica == TRUE) pCmdUI->SetCheck(1);
			else pCmdUI->SetCheck(0);


			pCmdUI->Enable(TRUE);
		}
		else pCmdUI->Enable(FALSE);

}

//******************************************************************
void CAsc2winView::OnAspTablica() 
{
	// TODO: Add your command handler code here

	int nIndex = iStvarniIndex[( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.GetCurSel()];   //Handle na CMainFrame objekt!!!!!!

	if (nIndex == 4 || nIndex == 5) {
	
	// TODO: Add your command handler code here
	m_bAspektiTablica = ! ( m_bAspektiTablica);


	if( m_bAspektiTablica == TRUE)	 {
		
		CRect rect;
		this->GetWindowRect(&rect);

		m_pdTablicaAspekata->ShowWindow(SW_SHOW);
		m_pdTablicaAspekata->m_bIsActive=TRUE;
	
	
	}
	else  {
	  m_pdTablicaAspekata->ShowWindow(SW_HIDE);
      m_pdTablicaAspekata->m_bIsActive=FALSE;

	  Invalidate();
	}


  }

	
}
//************************************************************************************
void CAsc2winView::Ucitaj_Postavke_Horoskopa(boolean bStartup) 
{
FILE * fajl;
int i;
char buff[400],c;

fajl=fopen("pocetne_postavke.cfg","rt"); if(fajl==NULL) {MessageBox("nema fajla postavki horoskopa!"); return; }

aspen=0; fgets(buff,100,fajl);c=buff[0]; if(toupper(c)=='I') aspen=1;


m_dUlazDlg.m_chSustavKuca='K'; 
fgets(buff,100,fajl);c=buff[0]; 
if(toupper(c)=='J') m_dUlazDlg.m_chSustavKuca='J';
if(toupper(c)=='B') m_dUlazDlg.m_chSustavKuca='B';

if (bStartup == TRUE) {
	m_bAspekti = FALSE; 
	fgets(buff,100,fajl);c=buff[0]; if(toupper(c)=='A') m_bAspekti = TRUE;

	m_bAspektiTablica = FALSE;
	fgets(buff,100,fajl);c=buff[0]; if(toupper(c)=='1') m_bAspektiTablica = TRUE;
}
else {
	fgets(buff,100,fajl);  //ako nije startup, preskoèi ove postavke
	fgets(buff,100,fajl);
}


fgets(buff,100,fajl); sscanf(buff,"%d",&m_iKorakMinute);
fgets(buff,100,fajl); sscanf(buff,"%d",&m_iKorakSati);

char nazivBaze[100];
fgets(buff,100,fajl); sscanf(buff,"%s",&nazivBaze[0]); m_dUlazDlg.m_sNazivBaze.Format("%s",nazivBaze);

if (bStartup==TRUE) {
	int vri=0; 
	fgets(buff,100,fajl); c=buff[0]; if(toupper(c)=='1') vri=1;



	//korekcija DST-a 
	Odredjivanje_dst();
    if ( m_dUlazDlg.m_iGod >= 1983 ) 
		if (   (m_dUlazDlg.m_iMje > mje_praznik[5] && m_dUlazDlg.m_iMje < mje_praznik[6] )
			|| (m_dUlazDlg.m_iMje == mje_praznik[5] && m_dUlazDlg.m_iDan >=dan_praznik[5] )
			|| (m_dUlazDlg.m_iMje == mje_praznik[6] && m_dUlazDlg.m_iDan < dan_praznik[6] )
		   ) m_dUlazDlg.m_iVri = 1;    // ljetno vrijeme

		else m_dUlazDlg.m_iVri = 0;

	else m_dUlazDlg.m_iVri = 0;



	if (m_dUlazDlg.m_iVri != vri ) {

		CString sVri[2]={"Bez DST","Sa DST"};
		CString sPoruka; 
		sPoruka.Format("Upozorenje: Defaultna postavka za DST (%s) razlièita je od pretpostavljene (%s) za današnji dan.",sVri[vri], sVri[m_dUlazDlg.m_iVri] );
		AfxMessageBox(sPoruka, MB_OK );

		m_dUlazDlg.m_iVri = vri;
	}
}
else {
	m_dUlazDlg.m_iVri=0;
	fgets(buff,100,fajl); c=buff[0]; if(toupper(c)=='1') m_dUlazDlg.m_iVri=1;
}


for(i=0;i<=5;i++) { fgets(buff,100,fajl); sscanf(buff,"%f",&orb[i]); }
for(i=0;i<=11;i++) { fgets(buff,100,fajl);sscanf(buff,"%f",&porb[i]); }

m_bImena=FALSE; fgets(buff,100,fajl);c=buff[0];  if(toupper(c)=='1') m_bImena=TRUE;


m_iOpc=0; fgets(buff,200,fajl);c=buff[0];  sscanf(buff,"%d",&m_iOpc);


m_dPostavke.m_bSotGodPrecesija=TRUE;
fgets(buff,100,fajl);c=buff[0]; 
if(toupper(c)=='N') m_dPostavke.m_bSotGodPrecesija=FALSE;

m_dPostavke.m_bGregKal=TRUE;
fgets(buff,100,fajl);c=buff[0]; 
if(toupper(c)=='N') m_dPostavke.m_bGregKal=FALSE;


fclose(fajl);

}

//*****************************************************************************************
void CAsc2winView::OnNaredbaHoroskopPostavke() 
{
	// TODO: Add your command handler code here

	if (m_dPostavke.DoModal () == IDOK) {
			Ucitaj_Postavke_Horoskopa(FALSE);
			Invalidate();              // ponovo nacrtaj 
	}

}


//*******************************************************************************************
void CAsc2winView::OnUpdateIndicatorKuce(CCmdUI *pCmdUI)
{
	CString buff;

	char *buf [3]= {"Koch","jed.","bez "};
	int i;

	     if (m_dUlazDlg.m_chSustavKuca=='B') i=2;
	else if (m_dUlazDlg.m_chSustavKuca=='J') i=1;
	else i=0;

	buff.Format(" Kuæe: %s",buf[i]);

	pCmdUI->SetText(buff);

	int nIndex = iStvarniIndex[( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.GetCurSel()];   //Handle na CMainFrame objekt!!!!!!
	if (nIndex == 4 || nIndex == 5) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);


}

//*******************************************************************************************

void CAsc2winView::OnUpdateIndicatorAsp(CCmdUI *pCmdUI)
{
	CString buff;
	char *buf [2]= {"norm.","ist. "};
	
	buff.Format(" Asp.: %s",buf[aspen]);
	pCmdUI->SetText(buff);

	int nIndex = iStvarniIndex[( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.GetCurSel()];   //Handle na CMainFrame objekt!!!!!!
	if (nIndex == 4 || nIndex == 5) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);

}

//*******************************************************************************************

void CAsc2winView::OnUpdateIndicatorBaza(CCmdUI *pCmdUI)
{
	CString buff;
	
	buff.Format(" Baza: %20s",m_dUlazDlg.m_sNazivBaze);

	pCmdUI->SetText(buff);
	pCmdUI->Enable(TRUE);

}

//*******************************************************************************************

void CAsc2winView::OnUpdateIndicatorIme(CCmdUI *pCmdUI)
{
	CString buff;
	
	buff.Format(" Ime: %25s",m_dUlazDlg.m_sIme);

	pCmdUI->SetText(buff);
	pCmdUI->Enable(TRUE);

}

//*******************************************************************************************
void CAsc2winView::Ukljuci_Sliku()
{
	CString sPic;

	sPic = "slike\\" + m_dUlazDlg.m_sIme;

	
	if ( m_dSlika->Ucitaj_sliku(sPic,"jpg") == FALSE) {
		if ( m_dSlika->Ucitaj_sliku(sPic,"gif") == FALSE) {

			m_dSlika->ShowWindow(SW_HIDE);
			return;
	}}

	//repozicioniraj na rub ovog prozora
	m_dSlika->SetWindowPos(&CWnd::wndTopMost   , m_dSlika->m_rSlika.left, m_dSlika->m_rSlika.top,
									 0,0, SWP_NOSIZE );


	m_dSlika->ShowWindow(SW_SHOW);
	m_dSlika->GetWindowRect(&(m_dSlika->m_rSlika));



}

//*******************************************************************************************

void CAsc2winView::Iskljuci_sliku()
{
	m_dSlika->ShowWindow(SW_HIDE);
}

//*******************************************************************************************
void CAsc2winView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	if (bJesuSlickeLoudane==TRUE ) {

		this->GetWindowRect(&(m_pdTablicaAspekata->m_rSlika) );

		m_pdTablicaAspekata->m_rSlika.left=m_pdTablicaAspekata->m_rSlika.right- 458;
		m_pdTablicaAspekata->m_rSlika.top=m_pdTablicaAspekata->m_rSlika.top-45;
	}

}

//******************************************************************************
void CAsc2winView::OnNaredbaGodinji() 
{
	// TODO: Add your command handler code here

	m_dGodisnjiHoroskop.Postavi_Bazni_Datum(m_dUlazDlg.m_iDan, m_dUlazDlg.m_iMje, m_dUlazDlg.m_iGod, 
		                                    m_dUlazDlg.m_iSat, m_dUlazDlg.m_iMin, m_dUlazDlg.m_sIme,
											m_dUlazDlg.m_fDuz, m_dUlazDlg.m_fSir, m_dUlazDlg.m_iVri,
											m_dUlazDlg.m_iGmt);
	
	//CString bx; bx.Format("%d", m_dUlazDlg.m_iVri); MessageBox(bx);
	
		if (m_dGodisnjiHoroskop.DoModal () == IDOK ) {

		m_dUlazDlg.m_iSat = m_dGodisnjiHoroskop.m_iSatBazni;
		m_dUlazDlg.m_iMin = m_dGodisnjiHoroskop.m_iMinBazni;

		m_dUlazDlg.m_iDan = m_dGodisnjiHoroskop.m_iDanBazni;
		m_dUlazDlg.m_iMje = m_dGodisnjiHoroskop.m_iMjeBazni;
		m_dUlazDlg.m_iGod = m_dGodisnjiHoroskop.m_iGodBazni;
	
		int iPersistVri = m_dUlazDlg.m_iVri;
		Ucitaj_Postavke_Horoskopa(FALSE);
		m_dUlazDlg.m_iVri=iPersistVri;

		m_dUlazDlg.m_sIme="<godišnji horoskop>";
		m_dSlika->ShowWindow(SW_HIDE);

		Invalidate();
	}

}




//****************************************************************************************************
void CAsc2winView::Iscrtaj_Horoskop_Tjela(CDC * pDC, int dan, int mje,int god,int sat,int min,int gmt)
{

	// ekran - podešavanje
	CRect wn;
	this->GetWindowRect(wn);
		
	m_pdTijelo->SetWindowPos(&CWnd::wndNoTopMost    , wn.right-120, wn.top-29, 120, wn.Height()+29, SWP_FRAMECHANGED    );



//	pDC->SetMapMode (MM_TEXT);
	pDC->SetMapMode (MM_ISOTROPIC);

	CRect rect;
	GetClientRect(rect);


if (pDC->IsPrinting() ==FALSE) {

    pDC->SetWindowExt( 600* m_dUlazDlg.m_iHorRez,600);   // logièke koordinate

	pDC->SetViewportExt(rect.Width()*0.93, rect.Width()/m_dUlazDlg.m_iHorRez*0.93 );  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike
	                                                                                  // bilo *564/988
	
	pDC->SetViewportOrg (rect.Width()*1.05/988*1000/5, (double)(rect.Width()*1.05/988) * 564/988* 10);  // pomakni poèetak k.s-a na dolje

		
}
else {

	pDC->SetWindowExt( 600* m_dUlazDlg.m_iHorRez,600);       // logièke koordinate
	pDC->SetViewportExt(3500, 2625);  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike
    pDC->SetViewportOrg (950,100);        // pomakni poèetak k.s-a na dolje


}



	//uzmi podatke za ispis iz dijaloga:

	boolean bObj[14], bAsp[3];

	bObj[ 0] = m_pdTijelo->m_bObj1;//objekti
	bObj[ 1] = m_pdTijelo->m_bObj2;
	bObj[ 2] = m_pdTijelo->m_bObj3;
	bObj[ 3] = m_pdTijelo->m_bObj4;
	bObj[ 4] = m_pdTijelo->m_bObj5;
	bObj[ 5] = m_pdTijelo->m_bObj6;	
	bObj[ 6] = m_pdTijelo->m_bObj7;
	bObj[ 7] = m_pdTijelo->m_bObj8;
	bObj[ 8] = m_pdTijelo->m_bObj9;
	bObj[ 9] = m_pdTijelo->m_bObj10;
	bObj[10] = m_pdTijelo->m_bObj11; //asc.
	
	bAsp[0] = m_pdTijelo->m_bAspDobri; //asp dobri
	bAsp[1] = m_pdTijelo->m_AspLosi; //asp loši da-ne



	if (m_dUlazDlg.m_iVri==1)	m_pdTijelo->m_bDST=true;
	else						m_pdTijelo->m_bDST=false;
	m_pdTijelo->UpdateData(FALSE);



	CPen olovka(PS_SOLID, 1, Colors[14]);                 //tip, debljina, boja
	CPen* staraOlovka = pDC->SelectObject(&olovka);           // pohrani staru olovku
 
	CBrush* brush;
	brush = new CBrush(Colors[14]); //tip, boja
	CBrush* oldBrush =	pDC->SelectObject(brush);


	
  CFont font1;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 12;                      // request a 12-pixel-height font
	strcpy(lf.lfFaceName, "Arial");        // request a face name "Arial"
	VERIFY(font1.CreateFontIndirect(&lf));  // create the font

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(Colors[14]);

	CFont* def_font = pDC->SelectObject(&font1);
	



	HBITMAP hBitmap ;
	CString sImeFajla="silueta-zenska.bmp";
	char buff[100], bf[80] ;

	if (m_dUlazDlg.m_cSpol=='M' )  sImeFajla="silueta-muska.bmp";

//	CString bf1; bf1.Format("%c",m_dUlazDlg.m_cSpol  );
//	MessageBox(bf1);

	try {
		FILE *fajl = fopen(sImeFajla,"rb");	if (!fajl) {MessageBox("Nema fajla "+sImeFajla);return;}
			fclose(fajl);

			///*
			hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImeFajla, IMAGE_BITMAP, 0, 0, 
				                           LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			//*/
	}
	catch (CException* e){
		MessageBox("greška kod uèitavanja"+sImeFajla);
		return;
		
	}
	

	// Attach the loaded image to the CBitmap object.
	CBitmap bmTmp;
	CBitmap bmTmp2;

	bmTmp.Attach(hBitmap);  

	
   BITMAP bmx;
	 bmTmp.GetBitmap( &bmx );  // jednostavnija verzija



   //Get the Display area available 

    CSize szRect = pDC->GetWindowExt();
	int dx=szRect.cx,  dy=szRect.cy; 

	// CRect rcRect; GetClientRect(&rcRect);
	// int dx= rcRect.Width(), dy=rcRect.Height();

	//CString buf; 	buf.Format("%d %d", (int)dx, (int)dy); MessageBox(buf);

	// resizing

	 CDC dcMem1;
     dcMem1.CreateCompatibleDC(pDC);
   
    //select the original bitmap into compatible device context
    CBitmap* pbmOld1 = (CBitmap*)dcMem1.SelectObject(&bmTmp);
 
    //resize 
	 bmTmp2.CreateCompatibleBitmap (&dcMem1, (int)(dy*bmx.bmWidth/bmx.bmHeight),(int)(dy) );  // prihvatni cdc, važno je da se stavi originalni DC, a ne ciljni!!
	                                                      // (inaèe ne napravi toènu paletu nego monokromni bitmap)


	 int nOldStretchMode = pDC->SetStretchBltMode(HALFTONE);  

    CBitmap* pbmOld2 = (CBitmap*)pDC->SelectObject(&bmTmp2);  //selektiraj u prihvatni cdc prihvatni prazni bitmap objekt

    
	pDC->StretchBlt(160,20, (int)(dy*bmx.bmWidth/bmx.bmHeight),(int)(dy),&dcMem1,0,0,bmx.bmWidth,bmx.bmHeight,SRCCOPY);  //resizaj i kopiraj u ciljni bmp

 
   //poèisti za sobom
	 pDC->SetStretchBltMode(nOldStretchMode);   //vrati natrag mod

	 dcMem1.SelectObject(pbmOld1);  //select old bitmaps
	 
	 bmTmp.Detach();             //detach bitmapove sa slikama
	 bmTmp2.Detach();
	 
	 DeleteObject(bmTmp);        // delete bmpove  u dcu
	 DeleteObject(bmTmp2);
	 
	 ReleaseDC(&dcMem1);   // oèisti memory dc-s



//izraèunaj

double be,ls,lm,pm,km,am,ms,m1,m2,m3,bm;
double Gvr,k=0,ta,e,rsir,jj,x,y,ra,fi;

double duz, sir;
char ime[40];
int i,vri, send=1,bd;
int danx, mjex, godx, satx, minx, bd2;
float as=0;
CString buff2;


duz = m_dUlazDlg.m_fDuz;
sir = m_dUlazDlg.m_fSir;
vri = m_dUlazDlg.m_iVri;


   danx=dan;	mjex=mje; 	godx=god; 	satx=sat-vri; 	minx=min;


 	if(satx<0) {
		satx+=24;
		danx-=1;}

	if(danx<1) {
		mjex-=1;
		if(mjex<1) {
			mjex=12;
			godx-=1;	}	
		danx=pl[mjex-1];	
	}



// raè. ascendenta :

Gvr=(double)(sat-vri-gmt+min/60.); //Globalno vrijeme
if(Gvr<0) { Gvr+=24;dan-=1;
			if (dan<1) {
				mje -=1; 
				if (mje<1) { mje=12; god-=1; } 
				dan=pl[mje-1];
			}
}

i=-(mje<3);

//buff2.Format("%d",i);
//MessageBox(buff2);


k=dan+floor2((mje*153.-i*11.-162)/5.)+floor2((god*1461.+i)/4.)-(god>=0)*366.;
if (k>577736) k-=floor2((floor2((god+i)/100.)*3-5)/4.);

ta=k-693596; tg=k-711858+Gvr/24.;                   // tg je GLOBAL
jj=ta/36525. ; i=ta/36525.;
e=23.452294-jj*0.013125-jj*jj*1.639e-6+jj*jj*jj*5.028e-7;
es=sin(e*pi/180); ec=cos(e*pi/180);                // GLOBAL

ar=Gvr*360.985647/24+frac(ta/1461)*1440.02509;      // GLOBAL
ar+=floor2(ta/1461)*0.0307572+99.2018973;
ar=fnmo(ar);
zvr=ar*24/360-duz*4/60; 

//buff2.Format("zvr=%f ar=%f es=%f ec=%f tg=%f",zvr,ar,es,ec,tg);
//MessageBox(buff2);

while(zvr>24)  zvr-=24;
while(zvr<0)   zvr+=24; 

m_fZvr =zvr;



  ra=zvr*pi/12; rsir=sir*pi/180;
  x=cos(rsir)*sin(ra)*ec+sin(rsir)*es; y=cos(rsir)*cos(ra);
  asc=atan(x/y); if(asc<0) asc+=pi;
		 if(x<0) asc+=pi;
  asc=180/pi*(asc+pi/2);
  while(asc>360) asc-=360;
  //ascd=asc/30; deg2dms(asc-ascd*30,buff,0);



        if(m_dUlazDlg.m_chSustavKuca=='J') { send=0;}
   else if(m_dUlazDlg.m_chSustavKuca=='B') { send=-1;}
  bd=1+send;
  bd2=0; if(bd>0) bd2=1;

 
  Planete(danx,mjex,godx,satx,minx);

  Kuce(asc,ra,e*pi/180,send);






// odavde poèinje iscrtavanje horoskopa:
FILE *fajl;

double kut[25], temp, flagsgn, ic , temp_ic, mn, kkk=0;                         
int    retro[12]={0,0,0,0,0,0,0,0,0,0};

double kt=0,brz[10]={7,10,9,8,6,5,4,3,2,1};


double asp[7]={0,180,60,120,90,45};       // tipovi aspekata


int Boja[5]={4,6,3,2},xx,yy;
int r,j,z, kk, max, flag, clor=0, mod=1, kuca_planete[11];
int vl_kuce[30]={5,0,4,0,3,0,2,0,1,0,3,0,4,0,10,5,6,0,7,0,8,0,9,6};
int nagl_kuca[14]={0,0,0,0,0,0,0,0,0,0,0,0,0};


int jakost[11][9]={
{5,0,11,0,1,0,7,0},{4,0,10,0,12,2,6,8},{6,3,12,9,8,0,2,0},{7,2,1,8,4,12,10,6},
{1,8,7,2,10,0,4,0},{9,12,3,6,2,4,8,10},{10,11,4,5,7,0,1,0},{11,10,5,4,6,8,12,2},
{9,12,3,6,11,0,5,0},{1,8,7,2,3,0,9,0} };


for(i=0;i<=11;i++) for(j=0;j<=11;j++) akt[i][j]=0;
for(z=0;z<=9;z++) { kut[z]=objx[z+1]; retro[z]=objy[z+1];}
for(z=1;z<=6;z++) kut[9+z]=objx[20+z];
for(i=1;i<=6;i++) kut[i+9+6]=fnmo(kut[i+9]+180);



 BITMAP bm2;
 CBitmap *pOldBitmap;

 CDC MemDC;
 MemDC.CreateCompatibleDC( pDC );   // pomoæni panel za crtanje slièica

 CBitmap bmAspSlabi, bmAspJaca;
 bmAspSlabi.LoadBitmap(IDB_SLABI);
 bmAspJaca.LoadBitmap(IDB_JACA);

  CDC AspDC;                              //pomoæni za crtanje
  AspDC.CreateCompatibleDC(pDC);

  if (AspDC2==NULL)   AspDC2.CreateCompatibleDC( pDC );   //panel s raportom aspekata
 
  CBitmap bmAsp, * bmOldAsp; 
  bmAsp.CreateCompatibleBitmap (pDC, 455,250);
  
  CBitmap bmAsp2, * bmOldAsp2; 
  bmAsp2.CreateCompatibleBitmap (pDC, 455,250);

  
CPen olovkaAsp(PS_SOLID, 1, Colors[15]);                 //tip, debljina, boja
CPen* staraOlovkaAsp = AspDC.SelectObject(&olovkaAsp);           // pohrani staru olovku
bmOldAsp = AspDC.SelectObject(&bmAsp);

CBrush* aspbrush;
aspbrush = new CBrush(Colors[7]); //tip, boja
CBrush* oldaspBrush =	AspDC.SelectObject(aspbrush);
 
AspDC.FloodFill(2,2,Colors[7]);



  pOldBitmap = MemDC.SelectObject( &Slicka[46] );  //uzmi stari bitmap




//crtanje tablice aspekata i raèunanje aspekata:

//CString bf4; bf4.Format("%d",m_pdTijelo->m_iAspObj); AfxMessageBox(bf4);

xx=0; max=0;
for(z=0;z<=5;z++) {
	yy=0;

	if(! (
		   ( (z==0 || z==2 || z==3)  && bAsp[0]==FALSE ) 
		|| ( (z==1 || z==4 || z==5)  && bAsp[1]==FALSE ) 
		 ) 
	  ) for(i=0;i<9;i++) {

		if (bObj[i]==FALSE) continue;

		if(aspen==1 && z==5) continue;

		for (j=i+1;j<=9;j++) {
		if (bObj[j]==FALSE) continue;

			temp=fabs(kut[i]-kut[j]);
			flagsgn=1; if(temp>200) {temp=360-temp;flagsgn=-1;}
			temp=temp-asp[z];

			if(aspen==1) {

				temp=fabs(floor2(kut[i]/30)*30-floor2(kut[j]/30)*30);if(temp>180) temp=360-temp;

				if( temp==asp[z] ) 	 {
					temp=fabs(kut[i]-kut[j]); 
					if(temp>200) temp=360-temp;
					goto kkp;
				}
				else continue;
          }

			if (fabs(temp)<=orb[z] && fabs(temp)<=(porb[i]+porb[j])/2.) {

kkp:
				if (m_pdTijelo->m_iAspObj!=0 ) if (i!= m_pdTijelo->m_iAspObj -1 && j!=m_pdTijelo->m_iAspObj -1 ) continue;

				ic=sgn(temp)*((1-retro[i]-retro[j])*sgn(brz[j]-brz[i])+(retro[i]-retro[j]))*sgn(kut[i]-kut[j]);
			    yy++;

				MemDC.SelectObject( &Slicka[46+i] );
				Slicka[24+i].GetObject( sizeof( BITMAP ), &bm2 );

 				AspDC.BitBlt(3+xx*55,  15+(yy-1)*14 , //znak[i],
								bm2.bmWidth, bm2.bmHeight, 
								&MemDC, 0, 0,  SRCCOPY   );

				MemDC.SelectObject( &Slicka[46+j] );
				Slicka[24+j].GetObject( sizeof( BITMAP ), &bm2 );

 				AspDC.BitBlt(3+xx*55+35,  15+(yy-1)*14 , //znak[i],
								bm2.bmWidth, bm2.bmHeight, 
								&MemDC, 0, 0,  SRCCOPY   );

               akt[i][j]=z+1;

			    if(flagsgn*ic==-1) {MemDC.SelectObject(&bmAspSlabi);bmAspSlabi.GetObject( sizeof( BITMAP ), &bm2 );}
                if(flagsgn*ic==1) {MemDC.SelectObject(&bmAspJaca);bmAspJaca.GetObject( sizeof( BITMAP ), &bm2 );}
                AspDC.BitBlt(3+xx*55+16,15+(yy-1)*14,bm2.bmWidth, bm2.bmHeight,&MemDC, 0, 0,  MERGECOPY );
                /*
				sprintf(bf,"  ");
				if(flagsgn*ic==1) sprintf(bf,"%c%c",187,171);
                if(flagsgn*ic==-1) sprintf(bf,"%c%c",171,187);
                AspDC.SetTextColor(Colors[4]);
					 AspDC.TextOut(3+xx*55+17,11+(yy-1)*14,bf);
                AspDC.SetTextColor(Colors[15]);
				*/

          }
		}  //j
	}  //i
	if(yy>max) max=yy;

	xx++; if(aspen==1 && z==5) continue;

		MemDC.SelectObject( &Slicka[36+z] );
		Slicka[36+z].GetObject( sizeof( BITMAP ), &bm2 );

			AspDC.BitBlt((xx-1)*55+20, 2 , //znak[i],
 							  bm2.bmWidth, bm2.bmHeight, 
							  &MemDC, 0, 0,  SRCCOPY   );


} //z



if(bd==0) goto xx0;   //ako je bez kuæa

//  aspekti ascedenta i m.c.-a
yy=0;
if (bObj[10]!=FALSE) for (z=0;z<=5;z++) {

	if(! (
		   ( (z==0 || z==2 || z==3)  && bAsp[0]==FALSE ) 
		|| ( (z==1 || z==4 || z==5)  && bAsp[1]==FALSE ) 
		 ) 
	  )	for (j=0;j<=9;j++) {

		if (bObj[j]==FALSE) continue;
		if(aspen==1 && z==5) continue;


		temp=fabs(kut[10]-kut[j]);
		flagsgn=1; if(temp>200) { temp=360-temp; flagsgn=-1;}
		temp=temp-asp[z];
		if(aspen==1) {
			 temp=fabs(floor2(kut[10]/30)*30-floor2(kut[j]/30)*30);if(temp>180) temp=360-temp;
			 if( temp==asp[z] ) goto kkp2;
          else continue;
      }
		if (fabs(temp)<=orb[z] && fabs(temp)<=(6+porb[j])/2.) {



kkp2:
			if (m_pdTijelo->m_iAspObj!=0 ) if ( m_pdTijelo->m_iAspObj -1!=10 && j!=m_pdTijelo->m_iAspObj -1 ) continue;
			ic=sgn(temp)*(1-2*retro[j])*sgn(kut[10]-kut[j]);
			yy++;

				MemDC.SelectObject( &Slicka[36+z] );
				Slicka[36+z].GetObject( sizeof( BITMAP ), &bm2 );

 				AspDC.BitBlt(3+xx*55,  15+(yy-1)*14 , //znak[i],
								bm2.bmWidth, bm2.bmHeight, 
								&MemDC, 0, 0,  SRCCOPY   );

				MemDC.SelectObject( &Slicka[46+j] );
				Slicka[24+j].GetObject( sizeof( BITMAP ), &bm2 );

 				AspDC.BitBlt(3+xx*55+15,  15+(yy-1)*14 , //znak[i],
								bm2.bmWidth, bm2.bmHeight, 
								&MemDC, 0, 0,  SRCCOPY   );

				
					 akt[j][10]=z+1;

				/*
				sprintf(bf,"  ");
                if(flagsgn*ic==1) sprintf(bf,"%c%c",187,171);
                if(flagsgn*ic==-1) sprintf(bf,"%c%c",171,187);

                AspDC.SetTextColor(Colors[4]);
					 AspDC.TextOut(3+xx*55+31+9,11+(yy-1)*14,bf);
				*/
				AspDC.SetTextColor(Colors[15]);
				AspDC.TextOut(3+xx*55+33,11+(yy-1)*14,"(      )");

                if(flagsgn*ic==-1) {MemDC.SelectObject(&bmAspSlabi);bmAspSlabi.GetObject( sizeof( BITMAP ), &bm2 );}
                if(flagsgn*ic==1) {MemDC.SelectObject(&bmAspJaca);bmAspJaca.GetObject( sizeof( BITMAP ), &bm2 );}
                AspDC.BitBlt(3+xx*55+40,15+(yy-1)*14,bm2.bmWidth, bm2.bmHeight,&MemDC, 0, 0,  MERGECOPY );
 


      }
	}	 //j
} //z


if(yy>max) max=yy;
flag=0;if(yy!=0) flag=1;
yy=0;

if (bObj[10]!=FALSE) for (z=0;z<=5;z++) {
	if(! (
		   ( (z==0 || z==2 || z==3)  && bAsp[0]==FALSE ) 
		|| ( (z==1 || z==4 || z==5)  && bAsp[1]==FALSE ) 
		 ) 
	  )	for (j=0;j<=9;j++) {
		if (bObj[j]==FALSE) continue;
		if(aspen==1 && z==5) continue;
		if (m_pdTijelo->m_iAspObj!=0 ) if (m_pdTijelo->m_iAspObj -1!=10 ) continue;

		if(aspen==1) {
			  temp=fabs((kut[19]/30)*30-floor2(kut[j]/30)*30);if(temp>180) temp=360-temp;
				if( temp==asp[z] ) goto kkp3;
            else continue;
      }
		temp_ic=fabs(kut[9+10]-kut[j]);
		if(temp_ic>200) temp_ic=360-temp_ic;
		temp_ic=temp_ic-asp[z];

		if (fabs(temp_ic)<=orb[z] && fabs(temp_ic)<=(6+porb[j])/2.) {
kkp3:
			if (m_pdTijelo->m_iAspObj!=0 ) if ( m_pdTijelo->m_iAspObj -1!=10 && j!=m_pdTijelo->m_iAspObj -1 ) continue;
                yy++;


				MemDC.SelectObject( &Slicka[36+z] );
				Slicka[36+z].GetObject( sizeof( BITMAP ), &bm2 );

 				AspDC.BitBlt(3+xx*55+70,  15+(yy-1)*14 , //znak[i],
								bm2.bmWidth, bm2.bmHeight, 
								&MemDC, 0, 0,  SRCCOPY   );

				MemDC.SelectObject( &Slicka[46+j] );
				Slicka[24+j].GetObject( sizeof( BITMAP ), &bm2 );

 				AspDC.BitBlt(3+xx*55+90,  15+(yy-1)*14 , //znak[i],
								bm2.bmWidth, bm2.bmHeight, 
								&MemDC, 0, 0,  SRCCOPY   );

			    akt[j][11]=z+1;
		}
	} //j
}//z

if(yy>max) max=yy;

xx0:

AspDC.SetTextColor(Colors[14]);
AspDC.TextOut(xx*55+70+15,-3,"MC");
AspDC.TextOut(xx*55+15,-3,"ASC");

AspDC.MoveTo(0,0); AspDC.LineTo(0,454);
AspDC.MoveTo(455,0); AspDC.LineTo(455,max*14+20);
AspDC.MoveTo(455,max*14+20); AspDC.LineTo(0,max*14+20);
AspDC.MoveTo(0,max*14+20); AspDC.LineTo(0,0);


AspDC.MoveTo(0,12); AspDC.LineTo(455,12);
AspDC.MoveTo(xx*55+70,0); AspDC.LineTo(xx*55+70,max*14+20);

for(i=1;i<=6;i++) {
	AspDC.MoveTo(i*55,0); AspDC.LineTo(i*55,max*14+20);
}

// skidanje slike tablice aspekata

	bmOldAsp2 = AspDC2.SelectObject(&bmAsp2);
	AspDC2.BitBlt(0, 0, 455,250, &AspDC, 0,0,	SRCCOPY   );  

//getimage(0,0,455,max*12+20,aspr);

m_pdTablicaAspekata->m_bPaste=TRUE; 
m_pdTablicaAspekata->SetWindowPos(&CWnd::wndTop, 
								  m_pdTablicaAspekata->m_rSlika.left, m_pdTablicaAspekata->m_rSlika.top,
								  456,(max+1)*14+30,SWP_FRAMECHANGED );
m_pdTablicaAspekata->Invalidate();


//oèisti sve korisštene ðiðe od aspekata:

AspDC.SelectObject( bmOldAsp );
AspDC.SelectObject( oldaspBrush);
AspDC.SelectObject(staraOlovkaAsp);      
delete aspbrush;
DeleteDC(AspDC);




//-----------------------------------------
//ispis toènih položaja planeta
	
	pDC->SelectObject(def_font);
	font1.DeleteObject();

	lf.lfHeight = 17;                      // request a 12-pixel-height font
	lf.lfWeight = FW_BOLD;
	VERIFY(font1.CreateFontIndirect(&lf));  // create the font

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(Colors[14]);

	def_font = pDC->SelectObject(&font1);
	


	pDC->MoveTo(147-210,0); 	pDC->LineTo(147-210,15+11*20+5);

for(z=0;z<=9;z++) {

	temp=kut[z]-floor2(kut[z]/30)*30;

	MemDC.SelectObject( &Slicka[46+z] );
	Slicka[46+z].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(-210,z*20, 19,12,  &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight, SRCCOPY );
	
	sprintf(bf,"=> %02d%c%02d'",(int)floor2(temp),186,(int)( (temp-floor2(temp) )*60) ); 
	pDC->TextOut(25-210,z*20-3,bf);
	if(retro[z]==1) pDC->TextOut(136-210,z*20-3,"r",1);

	MemDC.SelectObject( &Slicka[12+(int)floor2(kut[z]/30)] );
	Slicka[12+(int)floor2(kut[z]/30)].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(90-210,z*20, 19,12,  &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight, SRCCOPY );
	
	
	for(j=0;j<=3;j++) 
		if(floor2(kut[z]/30)==jakost[z][j*2]-1 || floor2(kut[z]/30)==jakost[z][j*2+1]-1 ) {
			MemDC.SelectObject( &Slicka[42+j] );
			Slicka[42+j].GetObject( sizeof( BITMAP ), &bm2 );
 			pDC->StretchBlt(114-210,z*20,19,12,  &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight,  SRCCOPY );
			
		}


	//treæina
	MemDC.SelectObject( &Slicka[12+(int)fmod(4*floor2((kut[z]-floor2(kut[z]/30)*30)/10)+floor2(kut[z]/30),12)] );
	Slicka[12+(int)fmod(4*floor2((kut[z]-floor2(kut[z]/30)*30)/10)+floor2(kut[z]/30),12)].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(152-210,z*20,19,12,    &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight, SRCCOPY );

	//dvanaestina
	MemDC.SelectObject( &Slicka[12+(int)fmod(floor2((kut[z]-floor2(kut[z]/30)*30)/2.5)+floor2(kut[z]/30),12)] );
	Slicka[12+(int)fmod(floor2((kut[z]-floor2(kut[z]/30)*30)/2.5)+floor2(kut[z]/30),12)].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(174-210,z*20,19,12,    &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight, SRCCOPY );
	
} //z


	temp=objx[12]-floor2(objx[12]/30)*30; // cvorovi

	MemDC.SelectObject( &Slicka[46+11] );
	Slicka[46+11].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(0-210,10*20+5,19,12,   &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight,  SRCCOPY );

	sprintf(bf,"=> %02d%c%02d'",(int)floor2(temp),186,(int)((temp-floor2(temp))*60) ); 
	pDC->TextOut(25-210,10*20+1,bf);
	if(objy[12]==1) pDC->TextOut(136-210,10*20,"r");

	MemDC.SelectObject( &Slicka[12+ (int)floor2(objx[12]/30)] );
	Slicka[12+ (int)floor2(objx[12]/30) ].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(90-210,10*20+5,19,12,   &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight,  SRCCOPY );
	
	if(bd>0) {        //ascendent
		temp=kut[9+1]-floor2(kut[9+1]/30)*30;		
		sprintf(bf,"ASC=> %02d%c%02d'",(int)floor2(temp),186,(int)((temp-floor2(temp))*60) );
		pDC->TextOut(0-210,11*20+5-1,bf);
		
		MemDC.SelectObject(12+ &Slicka[ (int)(kut[9+1]/30) ] );
		Slicka[12+ (int)(kut[9+1]/30) ].GetObject( sizeof( BITMAP ), &bm2 );
 		pDC->StretchBlt(90-210,11*20+5,19,12,  &MemDC, 0, 0,  bm2.bmWidth, bm2.bmHeight,  SRCCOPY );
		
		MemDC.SelectObject( &Slicka[12+ (int)fmod(4*floor2((kut[9+1]-floor2(kut[9+1]/30)*30)/10)+floor2(kut[9+1]/30),12) ] );
		Slicka[12+ (int)fmod(4*floor2((kut[9+1]-floor2(kut[9+1]/30)*30)/10)+floor2(kut[9+1]/30),12) ].GetObject( sizeof( BITMAP ), &bm2 );
 		pDC->StretchBlt(152-210,11*20+5,19,12,    &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight, SRCCOPY );
		
		MemDC.SelectObject( &Slicka[12+ (int)fmod(floor2((kut[9+1]-floor2(kut[9+1]/30)*30)/2.5)+floor2(kut[9+1]/30),12)] );
		Slicka[12+ (int)fmod(floor2((kut[9+1]-floor2(kut[9+1]/30)*30)/2.5)+floor2(kut[9+1]/30),12) ].GetObject( sizeof( BITMAP ), &bm2 );
 		pDC->StretchBlt(174-210,11*20+5,19,12,    &MemDC, 0, 0,bm2.bmWidth, bm2.bmHeight,  SRCCOPY );


	}


	 //više nije potrebno, sad je po defaultu crno
	/*
	if(clor==0) {  //prefarbaj u crno
		for(j=0;j<=9*13;j++) {
			for(i=-120;i<=0;i++) if(pDC->GetPixel(i,j)!=Colors[7]) pDC->SetPixel(i,j,Colors[0]);
		}
		for(j=335;j<=350;j++) {
			for(i=-120;i<=0;i++) if(pDC->GetPixel(i,j)!=Colors[7]) pDC->SetPixel(i,j,Colors[0]);
		}


	}
	*/


	/*
	*/


//ispis helpa

	CString sOpisi[12]={"sunce","mjesec","merkur","venera","mars","jupiter","saturn","uran","neptun","pluton","lilit","sj.cvor"};
	CString sOpisi2[12]={"ovan","bik","blizanci","rak","lav","djevica","vaga","škorpion","strijelac","jarac","vodenjak","ribe"};
	CString sOpis;
	pDC->TextOut(640,0," POMOC :");
	pDC->TextOut(640,12,"-----------");
	
	pDC->TextOut(640,40," planete :");
	pDC->TextOut(640,52,"-----------");


	for(z=0;z<=11;z++) {

	MemDC.SelectObject( &Slicka[46+z] );
	Slicka[46+z].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(640,70+z*20, 19,12 ,  &MemDC, 0, 0,  bm2.bmWidth, bm2.bmHeight,SRCCOPY );
	
	sOpis=" - " + sOpisi[z];
	 
	pDC->TextOut(640+20,70+z*20-1,sOpis);

}
	pDC->TextOut(640,340," znakovi :");
	pDC->TextOut(640,352,"-----------");

	for(z=0;z<=11;z++) {

	MemDC.SelectObject( &Slicka[12+z] );
	Slicka[12+z].GetObject( sizeof( BITMAP ), &bm2 );
 	pDC->StretchBlt(640,370+(z)*20, 19,12 ,  &MemDC, 0, 0,  bm2.bmWidth, bm2.bmHeight,SRCCOPY );
	
	sOpis=" - " + sOpisi2[z];
	 
	pDC->TextOut(640+20,370+(z)*20-1,sOpis);

}






CPen tankaOlovka(PS_SOLID, 1, Colors[14]);                 //tip, debljina, boja
CPen debelaOlovka(PS_SOLID, 2, Colors[14]);                 //tip, debljina, boja
CPen svjetlaOlovka(PS_SOLID, 2, Colors[15]);                 //tip, debljina, boja
CPen svjetlaTankaOlovka(PS_SOLID, 1, Colors[15]);                 //tip, debljina, boja

	
	for(z=0;z<=10;z++) objx[100+z]=objx[z];
	objx[100+11]=kut[9+1];
	objx[100+12]=kut[9+10];




//*******************************************************************
//crtanje aspekata

int iXTjel[13]={285,285,225,285,304,285,  245,285,316,267,313,268};
int iYTjel[13]={ 35,98,119,184,156,216,  240,288,345,434,501,590};

double iNPoz[13]={ 0,0,0,0,0,0,0,0,0,0,0,0};
int iZn;

//planete
for(i=0;i<=9+bd2;i++) {  
	   if (bObj[i]==FALSE) continue;
	   iZn= (int)floor2(kut[i]/30); 
       iNPoz[iZn]++;
}




int ir[13]={ 0,0,0,0,0,0,0,0,0,0,0,0};  //broj objekata po znaku

int ixp[13]={ 0,0,0,0,0,0,0,0,0,0,0,0}; //x koordinata objekta

for(i=0;i<=9+bd2;i++) {   
	   if (bObj[i]==FALSE) continue;
	   iZn= (int)floor2(kut[i]/30);
     	ixp[i]=iXTjel[iZn]+(ir[iZn]-iNPoz[iZn]/2)*25;
		ir[iZn]++;


}



//------------------------------------
// aspekti - iscrtavanje linija
int iZnI, iZnJ;

if(mod!=0 || bd==0) {  

	CPen dobri_asp(PS_SOLID, 1, Colors[2]);                 //tip, debljina, boja
	CPen losi_asp(PS_SOLID, 1, Colors[4]);                 //tip, debljina, boja

	pDC->SelectObject(&dobri_asp);           
	pDC->MoveTo(0-210,323-50); pDC->LineTo(36-210,323-50);
	pDC->TextOut(48-210,316-50,"Dobri aspekti");

	pDC->SelectObject(&losi_asp);           
	pDC->MoveTo(0-210,341-50); pDC->LineTo(36-210,341-50);
	pDC->TextOut(48-210,334-50,"Losi aspekti");





	for(i=0;i<=9+bd2;i++) {
		if (bObj[i]==FALSE) continue;
		for (j=i+1;j<=10;j++) {
			if (bObj[j]==FALSE) continue;
			temp=akt[i][j];
	        iZnI= (int)floor2(kut[i]/30);
			iZnJ= (int)floor2(kut[j]/30);

			if(temp>0 ) {
				if(temp==3 || temp==4 || temp==1) {
					pDC->SelectObject(&dobri_asp); 
					if (bAsp[0]==FALSE) continue;
				}
				else                   {
					pDC->SelectObject(&losi_asp); 
					if (bAsp[1]==FALSE) continue;
				}
				pDC->MoveTo(ixp[i]+8,iYTjel[iZnI]+8);
				pDC->LineTo(ixp[j]+8,iYTjel[iZnJ]+8);
			}
		}
	
	}
	//AfxMessageBox("");

	/*
	if(bd>0) {
		float fx,fy;
		
		for (j=0;j<=0;j++) { //samo ASC
			iZnJ= (int)floor2(kut[j]/30);
			for (i=0;i<=9;i++) {
			iZnI= (int)floor2(kut[i]/30);

				temp=akt[i][10+j];
				if(temp>0 ) {
					if(temp==3 || temp==4 || temp==1) {pDC->SelectObject(&dobri_asp); }
					else                   {pDC->SelectObject(&losi_asp); }
					pDC->MoveTo(ixp[i]+8,iYTjel[iZnI]+8);
					pDC->LineTo(ixp[j]+8,iYTjel[iZnJ]+8);
				}
				//CString bf1; bf1.Format("%d",i);AfxMessageBox(bf1);
			}
		}

	}
	*/



}


	CPen Olovka01(PS_SOLID, 1, Colors[4]);                 //tip, debljina, boja
	CPen Olovka02(PS_SOLID, 1, Colors[16]);                 //tip, debljina, boja
	CPen Olovka03(PS_SOLID, 1, Colors[3]);                 //tip, debljina, boja
	CPen Olovka04(PS_SOLID, 1, Colors[2]);                 //tip, debljina, boja

	delete brush;
	brush = new CBrush( Colors[6] ); 

	CBrush *bsh1, *bsh2, *bsh3, *bsh4;
	bsh1 = new CBrush(Colors[4]); //tip, boja
	bsh2 = new CBrush(Colors[16]); //tip, boja
	bsh3 = new CBrush(Colors[3]); //tip, boja
	bsh4 = new CBrush(Colors[2]); //tip, boja


for(i=0;i<=9+bd2;i++) {   //planete
	   if (bObj[i]==FALSE) continue;
	   iZn= (int)floor2(kut[i]/30);

		int elem=iZn+1;
	    if (elem==1 || elem==5 || elem==9  ) {pDC->SelectObject(bsh1);pDC->SelectObject(&Olovka01);}
		if (elem==2 || elem==6 || elem==10 ) {pDC->SelectObject(bsh2);pDC->SelectObject(&Olovka02);}
		if (elem==3 || elem==7 || elem==11 ) {pDC->SelectObject(bsh3);pDC->SelectObject(&Olovka03);}
		if (elem==4 || elem==8 || elem==12 ) {pDC->SelectObject(bsh4);pDC->SelectObject(&Olovka04);}

	    if (i==10 ) {	
			pDC->Ellipse(ixp[i]+12-14,iYTjel[iZn]+7-13,ixp[i]+12+14,iYTjel[iZn]+7+13); 
		
			pDC->SelectObject(&tankaOlovka);    
			pDC->TextOut(ixp[i],iYTjel[iZn],"Asc");
		}
		else {
		pDC->Ellipse(ixp[i]+9-13,iYTjel[iZn]+7-13,ixp[i]+9+13,iYTjel[iZn]+7+13); 


	   	MemDC.SelectObject( &Slicka[ 46+ i ] );   //metni ikone planeta
		Slicka[ 46 + i].GetObject( sizeof( BITMAP ), &bm2 );
		pDC->StretchBlt(ixp[i],iYTjel[iZn], 
			19,13,  &MemDC, 0, 0,  bm2.bmWidth, bm2.bmHeight, SRCAND             );


		if(retro[(int)objx[60+i]]==1) {       //postavi oznaku za retrogradne planete
			pDC->SelectObject(&tankaOlovka);    
			pDC->MoveTo(ixp[i],   iYTjel[iZn]-3 );
			pDC->LineTo(ixp[i],iYTjel[iZn]-3 );
		}
		}
    

}
	delete bsh1;
	delete bsh2;	
	delete bsh3;	
	delete bsh4;	
	pDC->SelectObject(brush);


//----------------------------------------------------------------------------------------------
  //iscrtaj dispozitore
	Dispozitori();

	/*
	CString sx,sbuf=""; for (z=1; z<=10;z++) { sx.Format("%d",veza[z]); sbuf=sbuf+ sx + ", ";}
	pDC->TextOut(200,400-100, sbuf);

	sbuf="";for (z=1; z<=10;z++) { sx.Format("%d",broj_veza[z]); sbuf=sbuf+ sx + ", ";}
	pDC->TextOut(200,400-80, sbuf);
	*/

	//iscrtaj strelice:
	int x1d, y1d, x2d, y2d, x3d, y3d, x0d=-210 , y0d=410;
	double fid;

	pDC->TextOut(x0d,y0d-30," dispozicija :");

	for (z=1; z<=10; z++) {

		x1d= x0d + 30*(int)xp[(int)z]+9;
		y1d= y0d + 40*(int)yp[(int)z]+7;

		if(z==veza[z] || veza[z]==0 ) {

			continue;
		}

		x2d= x0d + 30*(int)xp[veza[z]]+9;
		y2d= y0d + 40*(int)yp[veza[z]]+7;


        fid=atan2((double)(y2d-y1d),(double)(x2d-x1d)); if (fid<0) fid=2*pi + fid;
		

		x1d= x1d + (int)(11*cos(fid));
		y1d= y1d + (int)(11*sin(fid));

		fid = pi + fid; 

		x2d= x2d + (int)(11*cos(fid));
		y2d= y2d + (int)(11*sin(fid));

 		pDC->SelectObject(&svjetlaTankaOlovka);           

		//pDC->MoveTo((int)x1d,(int)y1d); 	pDC->LineTo((int)x2d,(int)y2d);

		ArrowTo(pDC, x1d,y1d,x2d,y2d,10,25);

	}


	for (z=1; z<=10; z++) {    //iscrtaj planete
		MemDC.SelectObject( &Slicka[46+z-1] );
		Slicka[46+z-1].GetObject( sizeof( BITMAP ), &bm2 );
 		pDC->StretchBlt(x0d + 30*xp[z] ,y0d+40*yp[z], 19,12,  &MemDC, 0, 0, bm2.bmWidth,bm2.bmHeight,  SRCCOPY );
	}


	for (z=1; z<=10; z++) {   //zaokruži disp.

		x1d= x0d + 30*(int)xp[(int)z]+10;
		y1d= y0d + 40*(int)yp[(int)z]+7;

		if(   (  z==veza[z] || veza[z]==0 || z==veza[veza[z]]   ) && broj_veza[z]>0                   ) {

			if ( broj_veza[z]>0 ) {
				pDC->SelectObject(&svjetlaOlovka);           
				Ellipse(pDC,x1d-1,y1d-1, 12,12,360,0);
			}
		}
	}

	pDC->SelectObject(&olovka);





	 
	 

	 
// sve vrati

//oèisti sve korisštene ðiðe od MemDC pomoænog cdc-
MemDC.SelectObject( pOldBitmap );
DeleteDC(MemDC);

pDC->SelectObject(def_font);
font1.DeleteObject();

pDC->SelectObject(oldBrush);
delete brush;
pDC->SelectObject(staraOlovka);  // vrati staru olovku



}





//************************************************************************************************************
//za iscrtavanje lune

void CAsc2winView::TrueEllipse(CDC *pDC, double x0, double y0, double a, double b, double fi1, double fi2)
{
	double X1,Y1,X2,Y2,Xstart,Ystart,Xend,Yend;
	double alfa;
	
	X1 = x0 - a;
	Y1 = y0 - b;

    X2 = x0 + a;
	Y2 = y0 + b;


	alfa = fi1 / 180 * pi; 

	Xstart = x0 +  a * cos(alfa);
	Ystart = y0 +  b * sin(alfa);


	alfa = fi2 / 180 * pi; 

	Xend = x0 + a * cos(alfa);
	Yend = y0 + b * sin(alfa);


	//pDC->Arc(X1,Y1,X2,Y2,Xstart,Ystart,Xend,Yend);

	double fi, kk=fabs(fi1-fi2)/1000;
	int xold,yold;
	int x,y;

	if (fi1<fi2) {
		for (fi=fi1; fi<=fi2; fi+=kk) {
		x = x0 +  a * cos(fi*pi/180);
		y = y0 +  b * sin(fi*pi/180);


		if (fi!=fi1) {
			pDC->MoveTo(xold,yold);
			pDC->LineTo(x,y);
		
		}
		xold=x; yold=y;
		}
	}
	else {
		for (fi=fi1; fi<=360+fi2; fi+=kk) {
		x = x0 +  a * cos(fi*pi/180);
		y = y0 +  b * sin(fi*pi/180);


		if (fi!=fi1) {
			pDC->MoveTo(xold,yold);
			pDC->LineTo(x,y);
		
		}
		xold=x; yold=y;
		}

 	}

}


//*******************************************************************************************************
void CAsc2winView::Iscrtaj_Tumac(CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt)
{

double be,ls,lm,pm,km,am,ms,m1,m2,m3,bm;
double Gvr,k=0,ta,e,rsir,jj,x,y,ra,fi;

double kut[25], temp, flagsgn, ic , temp_ic, mn, kkk=0;                         
int    retro[12]={0,0,0,0,0,0,0,0,0,0}, iBrLinija=32;

double duz, sir;
char ime[40];
int i,j,z,vri, send=1,bd, flag=0;
int danx, mjex, godx, satx, minx;
float as=0;
CString buff2;
CString sBuf6;

duz = m_dUlazDlg.m_fDuz;
sir = m_dUlazDlg.m_fSir;
vri = m_dUlazDlg.m_iVri;

int yp;

int opc, opcSpec;


opc     = m_pdTumac->m_iOpcija;
opcSpec = m_pdTumac->m_iOpcijaSpec; 

if (opcSpec>0) opc=1000;  //ako postoji spec. opcija, ne radi po obiènoj opciji iz dijaloga nego po spec.

//sBuf6.Format("opc %d     opcspec %d",opc,opcSpec); MessageBox(sBuf6);


   danx=dan;	mjex=mje; 	godx=god; 	satx=sat-vri; 	minx=min;


 	if(satx<0) {
		satx+=24;
		danx-=1;}

	if(danx<1) {
		mjex-=1;
		if(mjex<1) {
			mjex=12;
			godx-=1;	}	
		danx=pl[mjex-1];	
	}



// raè. ascendenta :

Gvr=(double)(sat-vri-gmt+min/60.); //Globalno vrijeme
if(Gvr<0) { Gvr+=24;dan-=1;
			if (dan<1) {
				mje -=1; 
				if (mje<1) { mje=12; god-=1; } 
				dan=pl[mje-1];
			}
}

i=-(mje<3);

//buff2.Format("%d",i);
//MessageBox(buff2);


k=dan+floor2((mje*153.-i*11.-162)/5.)+floor2((god*1461.+i)/4.)-(god>=0)*366.;
if (k>577736) k-=floor2((floor2((god+i)/100.)*3-5)/4.);

ta=k-693596; tg=k-711858+Gvr/24.;                   // tg je GLOBAL
jj=ta/36525. ; i=ta/36525.;
e=23.452294-jj*0.013125-jj*jj*1.639e-6+jj*jj*jj*5.028e-7;
es=sin(e*pi/180); ec=cos(e*pi/180);                // GLOBAL

ar=Gvr*360.985647/24+frac(ta/1461)*1440.02509;      // GLOBAL
ar+=floor2(ta/1461)*0.0307572+99.2018973;
ar=fnmo(ar);
zvr=ar*24/360-duz*4/60; 

//buff2.Format("zvr=%f ar=%f es=%f ec=%f tg=%f",zvr,ar,es,ec,tg);
//MessageBox(buff2);

while(zvr>24)  zvr-=24;
while(zvr<0)   zvr+=24; 

m_fZvr =zvr;



  ra=zvr*pi/12; rsir=sir*pi/180;
  x=cos(rsir)*sin(ra)*ec+sin(rsir)*es; y=cos(rsir)*cos(ra);
  asc=atan(x/y); if(asc<0) asc+=pi;
		 if(x<0) asc+=pi;
  asc=180/pi*(asc+pi/2);
  while(asc>360) asc-=360;
  //ascd=asc/30; deg2dms(asc-ascd*30,buff,0);



        if(m_dUlazDlg.m_chSustavKuca=='J') { send=0;}
   else if(m_dUlazDlg.m_chSustavKuca=='B') { send=-1;}
  bd=1+send;



  Planete(danx,mjex,godx,satx,minx);

  Kuce(asc,ra,e*pi/180,send);


for(z=0;z<=9;z++) { kut[z]=objx[z+1]; retro[z]=objy[z+1];}
for(z=1;z<=6;z++) kut[9+z]=objx[20+z];
for(i=1;i<=6;i++) kut[i+9+6]=fnmo(kut[i+9]+180);


for(z=1;z<=10;z++) objx[100+z]=objx[z];//kopija radnih podataka za dispozitore
objx[100+11]=kut[9+1];
objx[100+12]=kut[9+10];


int jakost[11][9]={
{5,0,11,0,1,0,7,0},{4,0,10,0,12,2,6,8},{6,3,12,9,8,0,2,0},{7,2,1,8,4,12,10,6},
{1,8,7,2,10,0,4,0},{9,12,3,6,2,4,8,10},{10,11,4,5,7,0,1,0},{11,10,5,4,6,8,12,2},
{9,12,3,6,11,0,5,0},{1,8,7,2,3,0,9,0} };


	// ekran - podešavanje
	CRect wn;
	this->GetWindowRect(wn);
		
	m_pdTumac->SetWindowPos(&CWnd::wndNoTopMost    , wn.right-120, wn.top-29, 120, wn.Height()+29, SWP_FRAMECHANGED    );

	(m_pdTumac->GetDlgItem(IDC_PAGEDISP) )->SetWindowPos(&CWnd::wndTop , 10, wn.Height()-35, 0,0, SWP_NOSIZE);
	(m_pdTumac->GetDlgItem(IDC_SPIN1) )->SetWindowPos(&CWnd::wndTop , 10, wn.Height()-10, 0,0, SWP_NOSIZE);


//inicijalizacija bitmapa

/*const COLORREF Colors[16] = {                     //ovo ja dodao, globalna var
		RGB( 0, 0, 0),      // Black,0
		RGB( 0, 0, 255),    // blue,1
		RGB( 0, 255,0,),    // green,2
		RGB( 0, 255, 255),  // cyan,3
		RGB( 255, 0, 0),    // red,4
		RGB( 255, 0,255),   // magenta,5
		RGB( 255, 255, 0),  // yellow,6
		RGB( 255, 255, 255),// white,7

		RGB( 0, 0, 128),    // dark blue,8
		RGB( 0, 128, 0),    // dark green,9
		RGB( 0, 128,128),   // dark cyan,10
		RGB( 128, 0, 0),    // dark red, 11
		RGB( 128, 0, 128),  // dark magenta, 12
		RGB( 128, 128,0),   // dark yellow, 13
		RGB( 128, 128, 128),// dark gray,14
		RGB( 192, 192, 192) // bright gray,15
	};
*/


	pDC->SetMapMode (MM_ISOTROPIC);

	CRect rect;
	GetClientRect(rect);

	//CString bf1; bf1.Format("%d, %d",rect.Width(), rect.Height()  );
	//MessageBox(bf1);

if (pDC->IsPrinting() ==FALSE) {

    pDC->SetWindowExt(600*  m_dUlazDlg.m_iHorRez,600);   // logièke koordinate

	pDC->SetViewportExt(rect.Width()*0.93, rect.Width()/m_dUlazDlg.m_iHorRez*0.93 );  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike
	
	//pDC->SetViewportOrg (rect.Width()*1.05/988*1000/5, (double)(rect.Width()*1.05/988) * 564/988* 10);  // pomakni poèetak k.s-a na dolje

}
else {
	iBrLinija=80;

	pDC->SetWindowExt( 600* m_dUlazDlg.m_iHorRez,600);       // logièke koordinate
	pDC->SetViewportExt(3500, 2625);  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike
    pDC->SetViewportOrg (300,150);        // pomakni poèetak k.s-a na dolje
}

	CPen crvena_olovka(PS_SOLID, 1, Colors[4]);                 //tip, debljina, boja
	CPen siva_olovka(PS_SOLID, 1, Colors[14]);                 //tip, debljina, boja
	CPen crna_olovka(PS_SOLID, 1, Colors[0]);                //tip, debljina, boja
	CPen* staraOlovka = pDC->SelectObject(&siva_olovka);   // pohrani staru olovku
 
	CBrush* brush;
	brush = new CBrush(Colors[14]); //tip, boja
	CBrush* oldBrush =	pDC->SelectObject(brush);

  CFont font1;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 18;                      // request a 12-pixel-height font
	strcpy(lf.lfFaceName, "Times New Roman");        // request a face name "Arial"
	VERIFY(font1.CreateFontIndirect(&lf));  // create the font

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(Colors[0]);

	CFont* def_font = pDC->SelectObject(&font1);
 
	CFont font2;
	lf.lfHeight = 16;                      // request a 12-pixel-height font
	strcpy(lf.lfFaceName, "Arial");        // request a face name "Arial"
	VERIFY(font2.CreateFontIndirect(&lf));  // create the font

	pDC->SelectObject(&font2);

	CFont font3;
	lf.lfHeight = 17;                      
	strcpy(lf.lfFaceName, "Times New Roman");     
	VERIFY(font3.CreateFontIndirect(&lf));  




BITMAP bm2;
 CBitmap *pOldBitmap;

 CDC MemDC;
 MemDC.CreateCompatibleDC( pDC );   // pomoæni panel za crtanje slièica


 CBitmap bmAspSlabi, bmAspJaca;

 bmAspSlabi.LoadBitmap(IDB_SLABI);
 bmAspJaca.LoadBitmap(IDB_JACA);





//ispis tumaèa
int kk;
int opc1=opc;
boolean bOpc0=FALSE;

FILE *fajl;
char buff[400], buff3[400], bf[80];


y=yp=0;



int pretvorba[15]={4,5,6,7,8,9,10,11,12,13,2,3};
int sunp, ascp, dp, r2;
static char *zod_kar[3]={"Aktivan,muski","Pasivan,zenski","Aktivan,muski"};
static char *zod_znak[15]={" ","Jarac","Vodenjak","Ribe","Ovan","Bik","Blizanci","Rak","Lav","Djevica","Vaga","Skorpion","Strijelac","Jarac" };
static char *zod_boja[15]={" ","plavocrna","ljubicasta","purpurnocrvena","ljubicastocrvena","crvenonarandzasta","narandzasta","zlatna","zuta","zuckasta svj.crvena","zelena","plavozelena","plava","plavocrna"};
static char *zod_elemt[15]={" ","zemlja","zrak","voda","vatra","zemlja","zrak","voda","vatra","zemlja","zrak","voda","vatra","zemlja"};
static char *zod_plnt[15]={" ","Saturn","Uran, Saturn","Neptun","Mars","Venera","Merkur","Mjesec","Sunce","Merkur","Venera","Pluton,Mars","Jupiter","Saturn" };
static char *zod_anat[15]={" ","kostima,nogama,zubima,kozom,kosom","listovi na nogama, gleznjevima, nervnim sustavom","stopalima, limfom, sokovima","glavom, jetrom","grlom, vratom","plucima, rukama, nervnim sustavom, ramenima","gprsima,trbuhom","srcem,protokom krvi,ledjima","trbuhom,crijevima,nervnim sustavom","krizima,mokracnim mjehurom,zenskim spol.org.","spolnim organima opcenito, prostatom","butinama, zglobovima, jetrom",
"kostima, nogama, zubima, kozom, kosom" };
static char *zod_ziv[15]={" ","koza, jarac, riba, dupin","orao, paun","riba","ovan, janje","bik","labud, konj","rak","lav","konj, zdral, zmija","kuja, medvjedica","skorpion, orao","konj","koza, jarac, riba, dupin" };
static char *zod_biljka[15]={" ","Kukurijek","Kukurijek","Hrast","bodljikave biljke","Ruza, Ljubica","Persin, Bazga","Lopoc, vodene biljke","Lovor, Suncokret","Persin, Bazga","Ruza,Ljubica","bodljikave biljke","Hrast","Kukurijek" };
static char *zod_broj[15]={" ","8","4","7","9","6","5","2","1","5","6","0","3","8" };
static char *zod_dan[15]={" ","Subota","Subota","Cetvrtak","Utorak","Petak","Srijeda","Ponedjeljak","Nedjelja","Srijeda","Petak","Utorak","Cetvrtak","Subota" };
static char *zod_kamen[15]={" ","crni oniks, crno kamenje","crni oniks,crno kamenje","ametist","rubin, crveno kamenje","zad, safir, smaragd","topaz, ahat","biser","dijamant","ahat, topaz","zad, safir, smaragd","rubin, crveno kamenje","ametist","crni oniks, crno kamenje" };
static char *zod_metal[15]={" ","olovo","olovo, titan","lim, bronca","zeljezo, celik","bakar","aluminij","srebro","zlato","aluminij","bakar","zeljezo, celik","lim, bronca","olovo" };
static char *zod_god[7]={"Merkura","Mjeseca","Saturna","Jupitera","Marsa","Sunca","Venere"};
static int  izod_god[7]={ 2,1,6,5,4,0,3   };
int zod_dkd[15][13]={ {0},
{1,5,9},{2,6,10},{3,7,11},{4,8,12},{5,9,1},{6,10,2},
{7,11,3},{8,12,4},{9,1,5},{10,2,6},{11,3,7},{12,4,8},{1,5,9}
};


char *trecina[3]={"Prva","Druga","Treca"};

char bff[400],bff2[400];
CString OpisZnaka[30], OpisKinZnaka[14], sTjelKarZnaka[13], sOpisAztZnaka[30], sOpisAztZnaka2[30];
CSize size;
char * cBuff1=NULL, * cBuff2=NULL, * cBuff3=NULL, * cBuff4=NULL ,* cBuff5=NULL;


//aspekti:
char buffa1[251], buffa2[251], buffa3[251];
double asp[7]={0,180,60,120,90,45};       // tipovi aspekata
double brz[10]={7,10,9,8,6,5,4,3,2,1};
int icx[12][12];



for(i=0;i<=11;i++) for(j=0;j<=11;j++) akt[i][j]=0;
for(i=0;i<=11;i++) for(j=0;j<=11;j++) icx[i][j]=0;


for(z=0;z<=5;z++) {

	for(i=0;i<11-((int)(bd>0))*2;i++) {
		
		if(aspen==1 && z==5) continue;
		
		for (j=i+1;j<=11-( (int)(bd==0) )*2 ;j++) {
			
			temp=fabs(kut[i]-kut[j]); if (j==11) temp=fabs(kut[i]-kut[9+10]);

			flagsgn=1; 
			
			if(temp>200) {
				temp=360-temp;
				flagsgn=-1;
			}
			
			temp=temp-asp[z];
			
			if(aspen==1) {
				temp=fabs(floor2(kut[i]/30)*30-floor2(kut[j]/30)*30);
				
				if(temp>180) temp=360-temp;

				if( temp==asp[z] ) goto kkpx;
				else continue;
            }
			
			if (fabs(temp)<=orb[z] && fabs(temp)<=(porb[i]+porb[j])/2.) {
kkpx:
				ic=sgn(temp)*((1-retro[i]-retro[j])*sgn(brz[j]-brz[i])+(retro[i]-retro[j]))*sgn(kut[i]-kut[j]);
				akt[i][j]=z+1;
				icx[i][j]=ic;
			}
		}
	}

 if(aspen==1 && z==5) continue;

}



CPen tankaOlovka(PS_SOLID, 1, Colors[14]);                 //tip, debljina, boja
CPen svjetlaOlovka(PS_SOLID, 2, Colors[15]);                 //tip, debljina, boja
CPen svjetlaTankaOlovka(PS_SOLID, 1, Colors[15]);                 //tip, debljina, boja

//buff2.Format("%d",opc); MessageBox(buff2);

//************************************ statistika ************************************************
if (opc==0 || opc==8)  { 

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	pDC->SelectObject(&font1);
	pDC->TextOut(440,yp*18+5,"S T A T I S T I K A :");

	pDC->MoveTo(435,yp*18+22); pDC->LineTo(585,yp*18+22);
	yp++;
}
y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;

int  ypp=yp;


	 //iscrtaj dispozitore
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) )	Dispozitori();

	//iscrtaj strelice:
	int x1d, y1d, x2d, y2d, x3d, y3d, x0d , y0d;
	double fid;
	x1d=0; y1d=0; x2d=0; y2d=0;x3d=0; y3d=0; x0d=0; y0d=0;
	fid=0;


	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		
		pDC->TextOut(10,yp*18,"karta planetarnih dispozicija :");
		pDC->TextOut(390-10,yp*18,"procjena snaga planeta :");
		pDC->TextOut(680-10,yp*18,"procjena snaga znakova :");
		yp++;
	}
	y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	x0d=10; y0d= yp*18;

	for (z=1; z<=10; z++) {

		x1d= x0d + 30*(int)xp[(int)z]+9;
		y1d= y0d + 40*(int)yp2[(int)z]+7;

		if(z==veza[z] || veza[z]==0 ) {

			continue;
		}

		x2d= x0d + 30*(int)xp[veza[z]]+9;
		y2d= y0d + 40*(int)yp2[veza[z]]+7;


        fid=atan2((double)(y2d-y1d),(double)(x2d-x1d)); if (fid<0) fid=2*pi + fid;
		

		x1d= x1d + (int)(11*cos(fid));
		y1d= y1d + (int)(11*sin(fid));

		fid = pi + fid; 

		x2d= x2d + (int)(11*cos(fid));
		y2d= y2d + (int)(11*sin(fid));

 		pDC->SelectObject(&svjetlaTankaOlovka);           

		//pDC->MoveTo((int)x1d,(int)y1d); 	pDC->LineTo((int)x2d,(int)y2d);

	    if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) ArrowTo(pDC, x1d,y1d,x2d,y2d,10,25);

	}


	int maxy=0;
	for (z=1; z<=10; z++) {    //iscrtaj planete
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ){
			MemDC.SelectObject( &Slicka[46+z-1] );
			Slicka[46+z-1].GetObject( sizeof( BITMAP ), &bm2 );
 			pDC->StretchBlt(x0d + 30*xp[z] ,y0d+40*yp2[z], 19,13,  &MemDC, 0, 0, bm2.bmWidth,bm2.bmHeight,  SRCCOPY );
		}
		if (40*yp2[z] > maxy) maxy=40*yp2[z];
	}


	for (z=1; z<=10; z++) {   //zaokruži disp.

		x1d= x0d + 30*(int)xp[(int)z]+10;
		y1d= y0d + 40*(int)yp2[(int)z]+7;

		if(   (  z==veza[z] || veza[z]==0 || z==veza[veza[z]]   ) && broj_veza[z]>0                   ) {

			if ( broj_veza[z]>0 ) {
				if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ){
					pDC->SelectObject(&svjetlaOlovka);           
					Ellipse(pDC,x1d-1,y1d-1, 12,12,360,0);
				}
			}
		}
	}


	//if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp+=(int)floor2(maxy/18)+3;
	//y+=(int)floor2(maxy/18)+3;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp+=8;
	y+=8;

	
	
	//tablica aspekata:

//-----------------------
//aspekti
int xx,yy, xx0=10,yy0=(floor2(maxy/18)+8)*18;
xx=0; maxy=0;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ){


pDC->SelectObject(&font1);
pDC->TextOut(10,yy0,"karta aspekata :");
pDC->SelectObject(&font2);

yy0+=18+9;

for(z=1;z<=6;z++) {
	yy=0;
	  for(i=0;i<9;i++) {
		for (j=i+1;j<=11;j++) {
                if( akt[i][j]==z) {

					yy++;

					MemDC.SelectObject( &Slicka[46+i] );
					Slicka[24+i].GetObject( sizeof( BITMAP ), &bm2 );

 					pDC->StretchBlt(xx0+xx*56+1,  yy0+(yy)*16 , //znak[i],
								   15,12, 
								   &MemDC, 0, 0,  bm2.bmWidth, bm2.bmHeight, SRCCOPY   );

					
					if (j<10){
					
						MemDC.SelectObject( &Slicka[46+j] );
						Slicka[24+j].GetObject( sizeof( BITMAP ), &bm2 );

 						pDC->StretchBlt(xx0+xx*56+35+1,  yy0+(yy)*16 , //znak[j],
								15,12, 
								&MemDC, 0, 0,  bm2.bmWidth, bm2.bmHeight,SRCCOPY   );
					 }
					else {
						if (j==10) pDC->TextOut(xx0+xx*56+35+2+1,  yy0+(yy)*16-3,"ac");
						else       pDC->TextOut(xx0+xx*56+35+2+1,  yy0+(yy)*16-3,"mc");

					}

			    if(icx[i][j]==-1) {MemDC.SelectObject(&bmAspSlabi);bmAspSlabi.GetObject( sizeof( BITMAP ), &bm2 );}
                if(icx[i][j]==1) {MemDC.SelectObject(&bmAspJaca);bmAspJaca.GetObject( sizeof( BITMAP ), &bm2 );}
                pDC->BitBlt(xx0+xx*56+16+1,yy0+(yy)*16+2,bm2.bmWidth, bm2.bmHeight,&MemDC, 0, 0,  SRCCOPY );
          }
		}  //j
	}  //i
	if(yy>maxy) maxy=yy;

	xx++; if(aspen==1 && z==5) continue;

		MemDC.SelectObject( &Slicka[36+z-1] );
		Slicka[36+z-1].GetObject( sizeof( BITMAP ), &bm2 );

			pDC->BitBlt(xx0+(xx-1)*56+20+1, yy0 , //znak[i],
 							  bm2.bmWidth, bm2.bmHeight, 
							  &MemDC, 0, 0,  SRCCOPY   );


} //z


pDC->SelectObject(&svjetlaTankaOlovka);

pDC->MoveTo(xx0, yy0+16-4); pDC->LineTo(xx0+6*56,yy0+16-4);

for(i=1;i<=5;i++) {
	pDC->MoveTo(xx0+(i)*56,yy0); pDC->LineTo(xx0+(i)*56,yy0+maxy*16+10);
}








} //if printaj



	
	//snage
	Izracunaj_Snage(bd);

	pDC->SelectObject(&svjetlaOlovka);
	pDC->SelectObject(&font2);

	
	double visina=0;
	for (i=1; i<=10; i++) if (snaga_planeta[i] > visina) visina=snaga_planeta[i];
	visina= (5*18)*1/(visina/100);

	
	delete brush;
	CBrush *bsh1, *bsh2, *bsh3, *bsh4;
	bsh1 = new CBrush(Colors[4]); //tip, boja
	bsh2 = new CBrush(Colors[16]); //tip, boja
	bsh3 = new CBrush(Colors[3]); //tip, boja
	bsh4 = new CBrush(Colors[2]); //tip, boja

	CRect rect1;

	for (i=1; i<=10; i++) {
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ){
			MemDC.SelectObject( &Slicka[46+i-1] );
			Slicka[46+i-1].GetObject( sizeof( BITMAP ), &bm2 );
 			pDC->StretchBlt(390 + 25*(i-1)-9 ,(ypp+8)*18, 19,13,  &MemDC, 0, 0, bm2.bmWidth,bm2.bmHeight,  SRCCOPY );

			//pDC->FillSolidRect(420 + 30*(i-1)-13 ,(ypp+5)*18-snaga_planeta[i]/100*visina,26,snaga_planeta[i]/100*visina , Colors[2]);

			rect1.left = 390 + 25*(i-1)-10;
			rect1.right = 390 + 25*(i-1)+10;

			rect1.top = (ypp+7)*18-snaga_planeta[i]/100*visina+8;
			rect1.bottom = (ypp+7)*18+8;


			int elem= floor2(objx[i]/30)+1;
				if (elem==1 || elem==5 || elem==9  ) pDC->FillRect(rect1, bsh1 );
				if (elem==2 || elem==6 || elem==10 ) pDC->FillRect(rect1, bsh2 );
				if (elem==3 || elem==7 || elem==11 ) pDC->FillRect(rect1, bsh3 );
				if (elem==4 || elem==8 || elem==12 ) pDC->FillRect(rect1, bsh4 );


			sBuf6.Format("%2.0f",snaga_planeta[i]);
			if (snaga_planeta[i]>0)pDC->TextOut(rect1.left+1, rect1.top-18,sBuf6);

		}
	}
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ){
		pDC->MoveTo(390-10-2, rect1.bottom +4 ); pDC->LineTo(390 + 25*(10-1)+10+2, rect1.bottom +4);
	}


	//snage znakova:
	visina=0;
	for (i=1; i<=12; i++) if (snaga_znakova[i] > visina) visina=snaga_znakova[i];
	visina= (5*18)*1/(visina/100);

	for (i=1; i<=12; i++) {
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ){
			MemDC.SelectObject( &Slicka[12+i-1] );
			Slicka[12+i-1].GetObject( sizeof( BITMAP ), &bm2 );
 			pDC->StretchBlt(680 + 25*(i-1)-9 ,(ypp+8)*18, 19,13,  &MemDC, 0, 0, bm2.bmWidth,bm2.bmHeight,  SRCCOPY );

			//pDC->FillSolidRect(420 + 30*(i-1)-13 ,(ypp+5)*18-snaga_planeta[i]/100*visina,26,snaga_planeta[i]/100*visina , Colors[2]);

			rect1.left = 680 + 25*(i-1)-10;
			rect1.right = 680 + 25*(i-1)+10;

			rect1.top = (ypp+7)*18-snaga_znakova[i]/100*visina+8;
			rect1.bottom = (ypp+7)*18+8;

			int elem=i;
			if (elem==1 || elem==5 || elem==9  ) pDC->FillRect(rect1, bsh1 );
			if (elem==2 || elem==6 || elem==10 ) pDC->FillRect(rect1, bsh2 );
			if (elem==3 || elem==7 || elem==11 ) pDC->FillRect(rect1, bsh3 );
			if (elem==4 || elem==8 || elem==12 ) pDC->FillRect(rect1, bsh4 );

			sBuf6.Format("%2.0f",snaga_znakova[i]);
			if (snaga_znakova[i]>0) pDC->TextOut(rect1.left+1, rect1.top-18,sBuf6);
		}
	}
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ){
		pDC->MoveTo(680-10-2, rect1.bottom +4 ); 
		pDC->LineTo(680+10+2+25*(12-1), rect1.bottom +4);
	}


	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp+=11;
	y+=11;

	//elementi
	pDC->SelectObject(&font1);
	pDC->TextOut(380,(yp-10)*18-9,"procjena snaga elemenata :");
	pDC->TextOut(610,(yp-10)*18-9,"procjena snaga nacela :");
	pDC->TextOut(850,(yp-10)*18-9,"procjena principa :");
	
	pDC->SelectObject(&font2);

	visina=0;
	for (i=1; i<=4; i++) if (snaga_elemenata[i] > visina) visina=snaga_elemenata[i];
	visina= (6*18)*1/(visina/100);

	CString sElem[5]={"vatra","zemlja","zrak","voda"};

	for (i=1; i<=4; i++) {
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ){


			pDC->TextOut(380, (yp-8)*18+(i-1)*25-9,sElem[i-1]);
			
			rect1.left =  380+60;
			rect1.right = 380+60+snaga_elemenata[i]/100*visina;

			rect1.top =    (yp-8)*18+(i-1)*25 -10;
			rect1.bottom = (yp-8)*18+(i-1)*25 +10;


			int elem=i;
			if (elem==1 ) pDC->FillRect(rect1, bsh1 );
			if (elem==2 ) pDC->FillRect(rect1, bsh2 );
			if (elem==3 ) pDC->FillRect(rect1, bsh3 );
			if (elem==4 ) pDC->FillRect(rect1, bsh4 );

			sBuf6.Format("%2.0f%c",snaga_elemenata[i], '%');
			if (snaga_elemenata[i]>0) pDC->TextOut(rect1.right+11, (yp-8)*18+(i-1)*25-9,sBuf6);

		}
	}

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ){

			pDC->MoveTo(rect1.left-3, (yp-8)*18+(1-1)*25 -10 -3 ); pDC->LineTo(rect1.left-3, (yp-8)*18+(4-1)*25 +10 +3);
	}


	//poèela

	delete bsh1;
	delete bsh2;
	delete bsh3;
	delete bsh4;
	brush = new CBrush(Colors[15]); //tip, boja


	visina=0;
	for (i=1; i<=3; i++) if (snaga_tipa[i] > visina) visina=snaga_tipa[i];
	visina= (6*18)*1/(visina/100);

	CString sElem2[4]={"kardinalni","fiksni","promjenjljivi"};

	for (i=1; i<=3; i++) {
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ){


			pDC->TextOut(610, (yp-8)*18+(i-1)*25-9,sElem2[i-1]);
			
			rect1.left =  610+80;
			rect1.right = 610+80+snaga_tipa[i]/100*visina;

			rect1.top =    (yp-8)*18+(i-1)*25 -10;
			rect1.bottom = (yp-8)*18+(i-1)*25 +10;


			pDC->FillRect(rect1, brush );


			sBuf6.Format("%2.0f%c",snaga_tipa[i], '%');
			if (snaga_tipa[i]>0) pDC->TextOut(rect1.right+11, (yp-8)*18+(i-1)*25-9,sBuf6);

		}
	}

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ){
		pDC->MoveTo(rect1.left-3, (yp-8)*18+(1-1)*25 -10 -3 ); pDC->LineTo(rect1.left-3, (yp-8)*18+(3-1)*25 +10 +3);
	}


	pDC->SelectObject(&siva_olovka);


	//princip:

	bsh1 = new CBrush(Colors[6]); //tip, boja
	bsh2 = new CBrush(Colors[15]); //tip, boja

	pDC->SelectObject(bsh1);

	Ellipse(pDC, 850+60, (yp-10)*18-9+100, 40, 40,0,360);
	//pDC->Ellipse(850+60-40, (yp-10)*18-9+100-40,  850+60+40, (yp-10)*18-9+100+40);

	pDC->MoveTo(850+60, (yp-10)*18-9+100); 	
	pDC->LineTo(850+60, (yp-10)*18-9+100-40); 
    
	pDC->MoveTo(850+60, (yp-10)*18-9+100); 	

	double fi01=pi/2-snaga_pricipa[1]/100*2*pi; if (fi01<0) fi01+=2*pi;
	
	//sBuf6.Format("%f",fi01); MessageBox(sBuf6);

	
	pDC->LineTo(850+60+40*cos(fi01) , (yp-10)*18-9+100-40*sin(fi01)   );     

	pDC->SelectObject(bsh1);
	pDC->FloodFill(850+60+2, (yp-10)*18-9+100-40+5,Colors[14]);


	pDC->SelectObject(bsh2);
	pDC->FloodFill(850+60-2, (yp-10)*18-9+100-40+5,Colors[14]);
	/*
	*/
	pDC->TextOut(850+5, (yp-10)*18-9+32,"zenski            muski");

	sBuf6.Format("  %02.0f%c             %02.0f%c",100-snaga_pricipa[1],'%', snaga_pricipa[1],'%' );
	pDC->TextOut(850+5, (yp-10)*18-9+46,sBuf6);


	delete brush;
	delete bsh1;
	delete bsh2;
	brush = new CBrush(Colors[14]); //tip, boja
	pDC->SelectObject(brush);




	//korekcija dužine ispisa:
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ){

			if ( (yy0+maxy*16)/18+4 > yp  ) {

				yp = (int)((yy0+maxy*16)/18+4);

		y  = yp-1;

			}

	}





}//end if opcija=8

//*************************************znak/podznak************************************************
if (opc==0 || opc==6)  { 



    fajl=fopen("zvz_zod.dat","rt"); if(fajl==NULL) {MessageBox("Nema fajla ZVZ_ZOD.DAT !");return;}
    for(i=1;i<=26;i++) {
		fgets(buff,199,fajl); buff[strlen(buff)-1]=0; OpisZnaka[i+1].Format("%s",buff);
	}
    fclose(fajl);

	fajl=fopen("zvz_tjel.dat","rt");if(fajl==NULL) {MessageBox("Nema fajla ZVZ_TJEL.DAT !");return;}

		for(i=1;i<=floor2(11*2+9);i++) {
			fgets(buff,250,fajl);
			sBuf6.Format("%s",buff); buff[strlen(buff)-1]=0; 
			sTjelKarZnaka[(int)floor2( ( (i-8)*((i-8)>0)  )/2)].Format("%s",buff);
		}	
	fclose(fajl);



	
    
dp=floor2((kut[0]-30*floor2(kut[0]/30))/10);

sunp = pretvorba[(int)(floor2(kut[0]/30))];
ascp = pretvorba[(int)(floor2(kut[10]/30))];

pDC->SetTextColor(Colors[0]);
pDC->SelectObject(&font1);

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	pDC->TextOut(140,yp*18+5,"E V R O P S K I   H O R O S K O P :");

	pDC->MoveTo(135,yp*18+22); pDC->LineTo(385,yp*18+22);
	yp++;
}
y++;


if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18,"Suncani znak       :");

sBuf6.Format("%s (     )", (cBuff1=strupr_s2( zod_znak[sunp] )) );    
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
	pDC->SetTextColor(Colors[4]);
	pDC->TextOut(140,yp*18,sBuf6);

	MemDC.SelectObject( &Slicka[12+(int)(floor2(kut[0]/30))] );
	Slicka[12+(int)(floor2(kut[0]/30))].GetObject( sizeof( BITMAP ), &bm2 );
	size=pDC->GetTextExtent(sBuf6, sBuf6.GetLength() );
 	pDC->BitBlt(140+size.cx-26,  yp*18+4, bm2.bmWidth, 13, &MemDC, 0, 0,  SRCCOPY   );

	yp++;
}
y++;
pDC->SetTextColor(Colors[0]);



sBuf6.Format("%s",OpisZnaka[(sunp)*2]);
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;

sBuf6.Format("%s",OpisZnaka[(sunp)*2+1]);
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18,"tjel. osobine         :"); 

sBuf6.Format("%s",sTjelKarZnaka[(int)(floor2(kut[0]/30))]  ); 

    if(sBuf6.GetLength()>130) { 

         if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(140,yp*18,sBuf6.Mid(0,130)+'-' );
			  
		 if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;				
		 y++;
			
         if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(140,yp*18,sBuf6.Mid(130) );
	}

    else  if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(140,yp*18,sBuf6);


if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;


if(bd>0) {
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) )	pDC->TextOut(10,yp*18,"Podznak            :");

	sBuf6.Format("%s (     )",(cBuff2=strupr_s2(zod_znak[ascp] )) );
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->SetTextColor(Colors[4]);
		pDC->TextOut(140,yp*18,sBuf6);

		MemDC.SelectObject( &Slicka[12+(int)(floor2(kut[10]/30))] );
		Slicka[12+(int)(floor2(kut[10]/30))].GetObject( sizeof( BITMAP ), &bm2 );
		size=pDC->GetTextExtent(sBuf6, sBuf6.GetLength() );
 		pDC->BitBlt(140+size.cx-26,  yp*18+4, bm2.bmWidth, 13, &MemDC, 0, 0,  SRCCOPY   );

		yp++;
	}
	y++;
	pDC->SetTextColor(Colors[0]);


	sBuf6.Format("%s",OpisZnaka[(ascp)*2]);
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->TextOut(140,yp*18,sBuf6);
		yp++;
	}
y++;
	sBuf6.Format("%s",OpisZnaka[(ascp)*2+1]);
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->TextOut(140,yp*18,sBuf6);
		yp++;
	}
y++;


if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18,"tjel. osobine         :"); 

if ( floor2(kut[10]/30) == floor2(kut[0]/30) ) sBuf6.Format("Vas je Ascedent u istom znaku kao i Sunce, pa ce tjelesne osobine tog znaka bitijos naglasenije !");
else  sBuf6.Format("%s",sTjelKarZnaka[(int)(floor2(kut[10]/30))]  ); 

    if(sBuf6.GetLength()>130) { 

         if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(140,yp*18,sBuf6.Mid(0,130)+'-' );
			  
		 if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;				
		 y++;
			
         if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(140,yp*18,sBuf6.Mid(130) );
	}

    else  if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(140,yp*18,sBuf6);


if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;

}
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
 	sBuf6.Format("Trecina (po 10%c)   :",186);
	pDC->TextOut(10,yp*18,sBuf6);
	sBuf6.Format("%s, su-vladar %s (%s)",trecina[dp],zod_plnt[zod_dkd[sunp][dp]],zod_znak[zod_dkd[sunp][dp]]);
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;
sBuf6.Format("%s",zod_elemt[sunp]);
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
 	pDC->TextOut(10,yp*18,"Element            :");
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;
sBuf6.Format("%s",zod_kar[(int)fmod2(sunp+2,2)]);
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
 	pDC->TextOut(10,yp*18,"Karakter           :");
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;
sBuf6.Format("%s",zod_plnt[sunp]);
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
 	pDC->TextOut(10,yp*18,"Glavna planeta     :");
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;
sBuf6.Format("upravlja %s",zod_anat[sunp]);
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
 	pDC->TextOut(10,yp*18,"Anatomija          :");
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;
sBuf6.Format("%s",zod_biljka[sunp]);
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
 	pDC->TextOut(10,yp*18,"Biljka             :");
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;
sBuf6.Format("%s",zod_ziv[sunp]);
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
 	pDC->TextOut(10,yp*18,"Zivotinja          :");
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;
sBuf6.Format("%s",zod_broj[sunp]);
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
 	pDC->TextOut(10,yp*18,"Sretan broj        :");
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;
sBuf6.Format("%s",zod_dan[sunp]);
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
 	pDC->TextOut(10,yp*18,"Sretan dan         :");
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;
sBuf6.Format("%s",zod_kamen[sunp]);
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
 	pDC->TextOut(10,yp*18,"Dragi kamen        :");
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;
sBuf6.Format("%s",zod_boja[sunp]);
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
 	pDC->TextOut(10,yp*18,"Draga boja         :");
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;
sBuf6.Format("%s",zod_metal[sunp]);
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
 	pDC->TextOut(10,yp*18,"Metal              :");
	pDC->TextOut(140,yp*18,sBuf6);
	yp++;
}
y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;

if(mjex<3 || (mjex==3 && danx<21)) godx--;

if (godx>=1900) r2=(int)fmod2(fabs2(godx-1900),7);
else { 
	r2=(int)fmod2(fabs2(1899-godx),7);
	r2=6-r2; 
};
sBuf6.Format("%s (     )",zod_god[r2]);

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
 	pDC->TextOut(10,yp*18,"Godina rodnjenja je pod utjecajem :");
	pDC->TextOut(250,yp*18,sBuf6);

	MemDC.SelectObject( &Slicka[46+izod_god[r2]]);
	Slicka[46+ izod_god[r2]].GetObject( sizeof( BITMAP ), &bm2 );
	size=pDC->GetTextExtent(sBuf6, sBuf6.GetLength() );
 	pDC->BitBlt(250+size.cx-26,  yp*18+6, bm2.bmWidth, 13, &MemDC, 0, 0,  SRCCOPY   );


	yp++;
}
y++;

if(mjex<3 || (mjex==3 && danx<21)) godx++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;




fajl=fopen("zvz_estr.dat","rt");if(fajl==NULL) {MessageBox("Nema fajla ZVZ_ESTR.DAT !");return;}

if(floor2(objx[1]/30)>0) for(i=1;i<=floor2(objx[1]/30)*3;i++) fgets(buff,400,fajl);
fgets(bff,400,fajl);fgets(bff2,400,fajl);
fclose(fajl);

bff[strlen(bff)-1]=0;bff2[strlen(bff2)-1]=0;

sBuf6.Format("%s%s",bff,bff2);

pDC->SelectObject(&font3);

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

 	pDC->TextOut(10,yp*18,"Poznati ljudi istoga suncevog znaka :");
}

temp = floor2(sBuf6.GetLength()/120);
	for(kk=0;kk<=temp;kk++) {

		if (kk<temp) sprintf(bff,"%.120s-",sBuf6.Mid(120*kk) );
		else		 sprintf(bff,"%.120s",sBuf6.Mid(120*kk)  );


			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(250,yp*18,bff);
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
			y++;
	} 


if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;

pDC->SelectObject(&font1);



}



if (opc==0) {bOpc0=TRUE; opc++;}

do {

if ( opc==1 || opc==2 ) {


if(opc==1) fajl=fopen("zvz_plan-win.dat","rt");
if(opc==2) fajl=fopen("zvz_medc-win.dat","rt");
if(fajl==NULL) {MessageBox("Nema potrebnog .DAT fajla s tumaèenjima!"); return; }

pDC->SetTextColor(Colors[0]);

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	if(opc==1) pDC->TextOut(160,yp*18+5,"P O L O Z A J   P L A N E T A :");
	if(opc==2) pDC->TextOut(160,yp*18+5," M O G U C E    B O L E S T I :");

	pDC->MoveTo(155,yp*18+22); 
	pDC->LineTo(380,yp*18+22);
	yp++;
}
y++;


for(i=0;i<=9-(opc-1);i++) {

  fgets(buff,255,fajl);

  for(j=0;j<=11;j++) {

    fgets(buff,399,fajl);

    if(floor2(kut[i]/30)==j) { 
		y++;
		temp=kut[i]-floor2(kut[i]/30)*30;
		
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) )	{
			yp++;
			MemDC.SelectObject( &Slicka[46+i] );
			Slicka[46+i].GetObject( sizeof( BITMAP ), &bm2 );
 			pDC->BitBlt(8,  yp*18 , bm2.bmWidth, bm2.bmHeight, &MemDC, 0, 0,  SRCCOPY   );
		
     	sprintf(buff3,"=> %02d%c%02d'",(int)floor2(temp),186,(int)( (temp-floor2(temp) )*60) ); 
	
		pDC->SelectObject(&font2);
		pDC->TextOut(27,yp*18-4,buff3);
		
		if(retro[i]==1) pDC->TextOut(103,yp*18-4,"r");
		
		MemDC.SelectObject( &Slicka[12+j] );
		Slicka[12+j].GetObject( sizeof( BITMAP ), &bm2 );
 		pDC->BitBlt(85,  yp*18 , bm2.bmWidth, bm2.bmHeight, &MemDC, 0, 0,  SRCCOPY   );
		
		for(kk=0;kk<=3;kk++) if(j==jakost[i][kk*2]-1 || j==jakost[i][kk*2+1]-1 ) {
			
		pDC->TextOut(110,yp*18-4,"(     )");
			
			MemDC.SelectObject( &Slicka[42+kk] );
			Slicka[42+kk].GetObject( sizeof( BITMAP ), &bm2 );
 			pDC->BitBlt(115,yp*18,bm2.bmWidth, bm2.bmHeight, &MemDC, 0, 0,   SRCCOPY );

		}

		}
		
		buff[strlen(buff)-1]=0; 
		temp=floor2(strlen(buff)/120.);
		
		pDC->SelectObject(&font1);

		if(temp>0)  for(kk=0;kk<=temp;kk++) {


			if (kk<temp) sprintf(buff3,"%.120s-",&buff[120*kk]);
			else 		 sprintf(buff3,"%.120s",&buff[120*kk]);


			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(160,yp*18-4,buff3);

			y++;
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
		} 
		else   	{
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(160,yp*18-4,buff);
			y++;
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
		}
		
    } //if
  } //for
} //for

if(flag==1) kut[10]=ic;

fclose(fajl);

y++; 
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;

} //if


if (bOpc0==TRUE) opc++;

} while ( !(bOpc0==FALSE || opc==3) );

if (bOpc0==TRUE) opc=opc1;


//planete po kuæama **************************************

int kuca_planete[11];
double f1,f2,f3,f4;
int vl_kuce[30]={5,0,4,0,3,0,2,0,1,0,3,0,4,0,10,5,6,0,7,0,8,0,9,6};
int nagl_kuca[14]={0,0,0,0,0,0,0,0,0,0,0,0,0};

pDC->SelectObject(&font3);


//buff2.Format("%d",opc); MessageBox(buff2);

// kucice
for(i=0; i<=9; i++ ) {
 int fnd;
 mn=1000; fnd=0;
 for(j=1; j<=12; j++) if(kut[i]>=kut[9+j] && fabs(kut[i]-kut[9+j])<mn) { mn=fabs(kut[i]-kut[9+j]); fnd=j; }
 if(fnd==0) {
   mn=0;
   for(j=1;j<=12;j++) if(kut[9+j]>mn) {mn=kut[9+j]; fnd=j; }
 }
 kuca_planete[i]=fnd;
 if(vl_kuce[(fnd-1)*2]==i+1 || vl_kuce[(fnd-1)*2+1]==i+1) nagl_kuca[fnd]=1;
}


if ( (opc==0 || opc==3) && bd != 0) {   //ako je definiran ascendent





fajl=fopen("zvz_plku.dat","rt"); if(fajl==NULL || bd==0) {MessageBox("Nema fajla ZVZ_PLKU.DAT"); return; }

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	pDC->TextOut(70,yp*18+5," P L A N E T E   U   K U C A M A");
	pDC->MoveTo(65,yp*18+22); pDC->LineTo(275,yp*18+22);
    yp++;
}

pDC->SetTextColor(Colors[0]);
y++;


for(i=1;i<=12;i++) {

  f1=kut[9+i];


  fgets(buff,400,fajl);

  flag=0;
  for(j=0;j<=9;j++) {
  
	  fgets(buff,400,fajl);
	
	 pDC->SelectObject(&font2);

     f3=kut[j];
     if(f1<7) if(f3>353) f3=360-f3;
     f4=f1-f3; 

  if( kuca_planete[j]==i  || (fabs(f4)<7 && f4>0)   ) {

	  if(flag==0) {

		  flag++;
		  y++;

		  sprintf(buff3," %d. kuca :",i);
		  if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

		  yp++;
		  
		  pDC->TextOut(0,yp*18-4-3+1,buff3);
		  
		  pDC->MoveTo(0,yp*18-4+14);
		  pDC->LineTo(8*12,yp*18-4+14);
		  }

		  //y++;
	  }

    y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	
	flag=1;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		
		MemDC.SelectObject( &Slicka[46+j] );
		Slicka[46+j].GetObject( sizeof( BITMAP ), &bm2 );
 		pDC->BitBlt(8,  yp*18 , bm2.bmWidth, bm2.bmHeight, &MemDC, 0, 0,  SRCCOPY   );
	}
	
	if( kuca_planete[j]!=i) 
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(40,yp*18-4,"V");
	
	if(retro[j]==1) 
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(30,yp*18-4,"r");

    buff[strlen(buff)-1]=0;
    

	temp=floor2(strlen(buff)/140.);
 	
	pDC->SelectObject(&font1);
    
	 if(temp>0)  for(kk=0;kk<=temp;kk++) {

		 if (kk<temp) sprintf(buff3,"%.140s-",&buff[140*kk]);
		 else         sprintf(buff3,"%.140s",&buff[140*kk]);

		 if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(70,yp*18-4,buff3);
		 
		 y++;
		 if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	 }
     else  {
		 if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(70,yp*18-4,buff);
		 y++;
		 if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	 }
}

}
}
fclose(fajl);

y++;
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;


}



// ********************************** aspekti ********************************

if (opc==0 || opc==4)  {   


fajl=fopen("zvz_asp.dat","rt");if(fajl==NULL) {MessageBox("Nema fajla ZVZ_ASP.DAT"); return; }


if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	pDC->SelectObject(&font3);
	pDC->SetTextColor(Colors[0]);
	pDC->TextOut(100,yp*18+5,"T U M A C     A S P E K A T A :");
	pDC->MoveTo(95,yp*18+22); pDC->LineTo(295,yp*18+22);
}

y++; 
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;

for(i=0;i<=9;i++) {

    fgets(buff,250,fajl);
    flag=0;

	for(j=i+1;j<=11;j++) {

    fgets(buffa1,250,fajl);
    fgets(buffa2,250,fajl);
    fgets(buffa3,250,fajl);

	for(z=0;z<=5;z++) if(akt[i][j]==z+1) {

			    y++;  
				flag=1;

				if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
				
					yp++;				
				
					pDC->SelectObject(&font2);

					MemDC.SelectObject( &Slicka[46+i] );
					Slicka[46+i].GetObject( sizeof( BITMAP ), &bm2 );
 					pDC->BitBlt(8,  yp*18 , bm2.bmWidth, bm2.bmHeight, &MemDC, 0, 0,  SRCCOPY   );

					MemDC.SelectObject( &Slicka[36+z] );
					Slicka[36+z].GetObject( sizeof( BITMAP ), &bm2 );
 					pDC->BitBlt(30,  yp*18 , bm2.bmWidth, bm2.bmHeight, &MemDC, 0, 0,  SRCCOPY   );


					if(j<10) {
						MemDC.SelectObject( &Slicka[46+j] );
						Slicka[46+j].GetObject( sizeof( BITMAP ), &bm2 );
 						pDC->BitBlt(50,  yp*18 , bm2.bmWidth, bm2.bmHeight, &MemDC, 0, 0,  SRCCOPY   );

					}
					else if(j==10) pDC->TextOut(50,yp*18-4,"ASC");
					else if(j==11) pDC->TextOut(50,yp*18-4,"MC");

					if(icx[i][j]==-1) {MemDC.SelectObject(&bmAspSlabi);bmAspSlabi.GetObject( sizeof( BITMAP ), &bm2 );}
					if(icx[i][j]==1)  {MemDC.SelectObject(&bmAspJaca);bmAspJaca.GetObject( sizeof( BITMAP ), &bm2 );}
					pDC->BitBlt(30-2,(yp+1)*18-6,bm2.bmWidth, bm2.bmHeight,&MemDC, 0, 0,  SRCCOPY );
				}


    if (z==1 || z==4 || z==5) strcpy(buffa1,buffa2);
    else if(z==0)             strcpy(buffa1,buffa3);

    buffa1[strlen(buffa1)-1]=0;


	pDC->SelectObject(&font1);

    if(strlen(buffa1)>130) { 

		sprintf(buff3,"%.130s-",buffa1);


              if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(100,yp*18-4,buff3);
			  
			  y++;
			  if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;				
				
              if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(100,yp*18-4,&buffa1[130]);

			}
    else      if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(100,yp*18-4,buffa1);




	y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;

	} //for z
	
	}  //for j

	/*
	if(flag==1) {
		y++;
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	}
	*/

}  //for i


fclose(fajl);
y++; 
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++; 
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
}  //if opc



//********************************* kuæice *******************************************************************

char zn[12][11]={"OVNU","BIKU","BLIZANCIMA","RAKU","LAVU","DJEVICI","VAGI","SKORPIONU","STRIJELCU","JARCU","VODENJAKU","RIBAMA"};
char ops[10][33]={"snaga,vitalnost,ponos","ojecaji,intuicija,javnost","razum,komunikacija","osjecaji,umjetnost","borba,akcija","zastita,napredak","ogranicenja,ambicija,tradicija","originalnost,promjene","intuicija,umjetnost,duh.poriv","okultna moc,nasilne promjene"};
char pla[12][9]={"Sunce","Mjesec","Merkur","Venera","Mars","Jupiter","Saturn","Uran","Neptun","Pluton","Asc","MC "};


if ((opc==0 || opc==5) && bd>0)  {   


fajl=fopen("zvz_kuce.dat","rt"); if(fajl==NULL) {MessageBox("Nema fajla ZVZ_KUCE.DAT"); return; }

pDC->SelectObject(&font1);

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
	pDC->SetTextColor(Colors[0]);
	pDC->TextOut(100,yp*18+5,"T U M A C     K U C I C A :");
	pDC->MoveTo(95,yp*18+22); pDC->LineTo(285,yp*18+22);
    
}


for(i=1;i<=12;i++) {

f1=kut[9+i];if(i<12) f2=kut[i+10]; else f2=kut[9+1];

fgets(buff,255,fajl);  buff[strlen(buff)-1]=0;

sBuf6.Format("%.2d. kuca (%s):",i,buff); 

y++; 
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;

y++; 
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	yp++;
	pDC->SelectObject(&font1);
	pDC->TextOut(10,yp*18,sBuf6); 
	pDC->MoveTo(10,yp*18+18); pDC->LineTo(69,yp*18+18);
}

y++; 
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;

y++; 
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	yp++;
	pDC->SelectObject(&font2);
	sBuf6.Format("VRH kuce u %s (      )",zn[(int)(f1/30)] ); 
	pDC->TextOut(30,yp*18,sBuf6);

	MemDC.SelectObject( &Slicka[12+(int)(f1/30)] );
	Slicka[12+(int)(f1/30)].GetObject( sizeof( BITMAP ), &bm2 );
	size=pDC->GetTextExtent(sBuf6, sBuf6.GetLength() );
 	pDC->BitBlt(30+size.cx-24,  yp*18+4, bm2.bmWidth, 13, &MemDC, 0, 0,  SRCCOPY   );

}

pDC->SelectObject(&font2);


y++; 
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;


if(floor2(f1/30)!=floor2(f2/30)) {
	for(j=1;j<=12;j++) {
		if(f2>f1)  {
			if(j-1>floor2(f1/30) && j-1<floor2(f2/30)) { 
				sBuf6.Format("U %s (      )",zn[j-1]); 
			    if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
					pDC->TextOut(30,yp*18,sBuf6);

					MemDC.SelectObject( &Slicka[12+j-1] );
					Slicka[12+j-1].GetObject( sizeof( BITMAP ), &bm2 );
					size=pDC->GetTextExtent(sBuf6, sBuf6.GetLength() );
					pDC->BitBlt(30+size.cx-24,  yp*18+4, bm2.bmWidth, 13, &MemDC, 0, 0,  SRCCOPY   );

					yp++;
				}
				y++; 
			}
		}
		else if(j-1>floor2(f1/30) || j-1<floor2(f2/30)) { 
				 sBuf6.Format("U %s",zn[j-1]); 
		         if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
					 pDC->TextOut(30,yp*18,sBuf6);

					MemDC.SelectObject( &Slicka[12+j-1] );
					Slicka[12+j-1].GetObject( sizeof( BITMAP ), &bm2 );
					size=pDC->GetTextExtent(sBuf6, sBuf6.GetLength() );
					pDC->BitBlt(30+size.cx-24,  yp*18+4, bm2.bmWidth, 13, &MemDC, 0, 0,  SRCCOPY   );

					 yp++;
				 }
				 y++; 
			 }
		}
	}

sBuf6.Format("Kraj kuce u %s (      )",zn[(int)(f2/30)]); 
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
	pDC->TextOut(30,yp*18,sBuf6);

	MemDC.SelectObject( &Slicka[12+(int)(f2/30)] );
	Slicka[12+(int)(f2/30)].GetObject( sizeof( BITMAP ), &bm2 );
	size=pDC->GetTextExtent(sBuf6, sBuf6.GetLength() );
	pDC->BitBlt(30+size.cx-24,  yp*18+4, bm2.bmWidth, 13, &MemDC, 0, 0,  SRCCOPY   );

	yp++;
}	
y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
	pDC->MoveTo(30,yp*18+7); pDC->LineTo(210,yp*18+7);
	yp++;
}
y++; 

pDC->SelectObject(&font1);

for(j=1;j<=12;j++) {
    fgets(buff,255,fajl); buff[strlen(buff)-1]=0;

     if( j-1==floor2(f1/30) ) {
		 sBuf6.Format("U %s:",zn[j-1]);	

			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(30,yp*18,sBuf6);
			sBuf6.Format("%s",buff);		

			 if(sBuf6.GetLength()>120) {
				 	sprintf(buff3,"%.120s-",sBuf6);
					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(150,yp*18,buff3);
					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
					y++;
					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(150,yp*18,sBuf6.Mid(120));
			 }
			 else { 
				 sBuf6.Format("%s",buff); 
				 if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(150,yp*18,sBuf6); 
			 }
			 if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
			 y++; 
	 }

     if(floor2(f1/30)==floor2(f2/30)) continue;

     if(j-1==floor2(f2/30)) {
		 sBuf6.Format("U %s:",zn[j-1]);

			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(30,yp*18,sBuf6);
			sBuf6.Format("%s",buff);

			 if(sBuf6.GetLength()>120) {
					
				 	sprintf(buff3,"%.120s-",sBuf6);
					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(150,yp*18,buff3);
					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
					y++;
					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(150,yp*18,sBuf6.Mid(120));
			 }
			 else { 
				 sBuf6.Format("%s",buff); 
				 if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) )pDC->TextOut(150,yp*18,sBuf6); 
			 }
			 if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
			 y++; 
	 }

     if(f2>f1)  {
		 if(j-1>floor2(f1/30) && j-1<floor2(f2/30)) {
			 sBuf6.Format("U %s:",zn[j-1]);
				
				if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(30,yp*18,sBuf6);
				sBuf6.Format("%s",buff);		
				
				if(sBuf6.GetLength()>120) {

				 	sprintf(buff3,"%.120s-",sBuf6);
					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(150,yp*18,buff3);

					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
					y++;
					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(150,yp*18,sBuf6.Mid(120));
				}
				else { 
					sBuf6.Format("%s",buff); 
					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(150,yp*18,sBuf6);
				}
			 if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
			 y++; 
		 }
	 }
     else if(j-1>floor2(f1/30) || j-1<floor2(f2/30)) {
		 sBuf6.Format("U %s:",zn[j-1]);

				if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(30,yp*18,sBuf6);
				sBuf6.Format("%s",buff);	
				
				if(sBuf6.GetLength()>120) {

				 	sprintf(buff3,"%.120s-",sBuf6);
					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(150,yp*18,buff3);

					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
					y++;
					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(150,yp*18,sBuf6.Mid(120));
				}
				else { 
					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(30,yp*18,sBuf6);
					sBuf6.Format("%s",buff); 
					if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(150,yp*18,sBuf6); 
				}
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) )yp++;
			y++; 
	}

}

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
	yp++;
}
y++; 

if(f2<f1) f2+=360;


if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	sBuf6.Format("%.2f prirodne (one od 30%c)",fabs(f2-f1)/30,186);

	pDC->SelectObject(&font1);
	pDC->TextOut(30,yp*18,"Sirina kuce :");
	pDC->TextOut(150,yp*18,sBuf6);
}
y++;
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) 	yp++;

y++;  
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
	pDC->TextOut(30,yp*18,"Planete u kuci :");
	pDC->SetTextColor(Colors[4]);
}

flag=0; 
for(kk=0;kk<=9;kk++) if(kuca_planete[kk]==i) {
	sBuf6.Format("- %s (%s)",pla[kk],ops[kk]);
	flag=1;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

		MemDC.SelectObject( &Slicka[46+kk] );
		Slicka[46+kk].GetObject( sizeof( BITMAP ), &bm2 );
 		pDC->BitBlt(250,  yp*18+4 , bm2.bmWidth, 13, &MemDC, 0, 0,  SRCCOPY   );

		pDC->TextOut(270,yp*18,sBuf6);
		yp++;
	}
	y++;
}
if(flag==0) {
	pDC->SetTextColor(Colors[4]);

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->TextOut(250,yp*18,"/");
	    yp++;
	}
	y++;

}

pDC->SetTextColor(Colors[0]);

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(30,yp*18,"Planete blizu vrha kuce (ispred):");
flag=0; 
for(kk=0;kk<=9;kk++) {
     f3=kut[kk];
     if(f1<7) if(f3>353) f3=360-f3;
     f4=f1-f3; 

     if(fabs(f4)<7 && f4>0) {

		 sBuf6.Format("- %s (%s)",pla[kk],ops[kk]);
		 flag=1;

		 if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

			MemDC.SelectObject( &Slicka[46+kk] );
			Slicka[46+kk].GetObject( sizeof( BITMAP ), &bm2 );
 			pDC->BitBlt(250,  yp*18+4 , bm2.bmWidth, 13, &MemDC, 0, 0,  SRCCOPY   );

		     pDC->SetTextColor(Colors[4]);
			 pDC->TextOut(270,yp*18,sBuf6);
			 yp++;
		 }
		 y++; 
		

	 }
}

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->SetTextColor(Colors[0]);

if(flag==0) {
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
			pDC->SetTextColor(Colors[4]);
			pDC->TextOut(250,yp*18,"/");
			yp++;
		}
		y++; 
}

pDC->SetTextColor(Colors[0]);

}
fclose(fajl);

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;


}//if



//****************************************************************************

int rb, r,iKinZnak, iKinPodznak, iKinElement, iKinNacelo;
int prethodna;

char *kin_elmt[6]={"zemlja","metal","voda","drvo","vatra","zemlja"};
char *kin_nacel[3]={"jin","jang","jin"};
char *kin_nacel_hrv[3]={"zensko","musko","zensko"};
char *kin_znak[13]={"Divlja svinja","Stakor","Bik","Tigar","Zec","Zmaj","Zmija","Konj","Koza","Majmun","Pijetao","Pas","Divlja svinja"};
int pocetak[210]={
31,1, 19,2, 8,2,  29,1, 16,2, 4,2,  25,1, 13,2, 2,2,  22,1, 10,2, 30,1, 18,2,
6,2,  26,1, 14,2, 3,2,  23,1, 11,2, 1,2,  20,2, 8,2,  28,1, 16,2, 5,2,  25,1,
13,2, 2,2,  23,1, 10,2, 30,1, 17,2, 6,2,  26,1, 14,2, 4,2,  24,1, 11,2, 31,1,
19,2, 8,2,  27,1, 15,2, 5,2,  25,1, 13,2, 2,2,  22,1, 10,2, 29,1, 17,2, 6,2,
27,1, 14,2, 3,2,  24,1, 12,2, 31,1, 18,2, 8,2,  28,1, 15,2, 5,2,  25,1, 13,2,
2,2,  21,1, 9,2,  30,1, 17,2, 6,2,  27,1, 16,1, 3,2,  23,1, 11,2, 31,1, 18,2,
7,2,  28,1, 16,2, 5,2,  25,1, 13,2, 2,2,  20,2, 9,2,  29,1, 17,2, 6,2,  27,1,
15,2, 4,2,  23,1, 10,2, 31,1, 19,2, 8,2,  27,1, 15,2, 5,2};


if (opc==0 || opc==7 || opcSpec>0)  {   

if(opcSpec>0 && opcSpec!=1) goto skipKineski;

    fajl=fopen("zvz_kin.dat","rt"); if(fajl==NULL) {MessageBox("Nema fajla ZVZ_KIN.DAT !");return;}
    for(i=0;i<=12;i++) {
		fgets(buff,199,fajl); buff[strlen(buff)-1]=0;
		OpisKinZnaka[i].Format("%s",buff);
	}
	fclose(fajl);

	pDC->SetTextColor(Colors[0]);
	pDC->SelectObject(&font1);

if(opcSpec==0) {

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

		pDC->TextOut(140,yp*18+5,"O S T A L I    H O R O S K O P S K I    S U S T A V I");

		pDC->MoveTo(135,yp*18+22); pDC->LineTo(515,yp*18+22);
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;
}
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

//**********************RAÈUNANJE***********************************

rb=godx-1900; r=0; prethodna=0;

iKinPodznak=floor2((satx - 1 )/2)+2;	if (iKinPodznak==13 || satx==0) iKinPodznak=1; 

	//sBuf6.Format("%d, sat=%d",iKinPodznak,satx); MessageBox(sBuf6);

if(rb>0 && rb<=100) {
	r=0; 
	if(mjex<pocetak[rb*2+1] || (danx<pocetak[rb*2] && mjex==pocetak[rb*2+1]) ) r=1;

	iKinZnak=fmod2(rb,12)-r+1; if (iKinZnak==0) iKinZnak=12;
	
	iKinElement=(int)(fmod2(rb,10)/2)-r*(1-fmod2(rb,2))+1;
	
	iKinNacelo=fmod2(rb,2)-r+1;

	prethodna=r;
}
else {
	CString datKNG=Nadji_Kinesku_Godinu(god);
	int danKNG, mjeKNG; 
	

	sscanf(datKNG,"%d %d",&danKNG, &mjeKNG);
	

	if (mjex < mjeKNG  || (mjex==mjeKNG && danx < danKNG)  ) prethodna=1;

	if (godx>=1900) iKinZnak=fmod2(fabs2(1900-godx),12)-prethodna+1;
	else           iKinZnak=11-fmod2(fabs2(godx-1899),12) - prethodna+1;

	if (godx>=1900) iKinElement= fmod2(fabs2(godx-1900),10)/2 +1 -prethodna;
	else 			iKinElement= 4 - floor2(fmod2(fabs2(1899-(godx) ),10)/2) +1 -prethodna;

	
	
	
	
	r=fmod(fabs2(1899-(godx) ),2); r=1-r+1;
	iKinNacelo=r;

}
//************************************************************************


	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->TextOut(10,yp*18, "KINESKI HOROSKOP (godišnji)");
		pDC->MoveTo(5,yp*18+22); pDC->LineTo(235,yp*18+22);
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;



	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Kineska godina :");
	
	sBuf6.Format("%d",godx+2697-prethodna);
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		
		pDC->SetTextColor(Colors[4]);
		pDC->TextOut(210,yp*18, sBuf6);

		pDC->SetTextColor(Colors[0]);
		pDC->SelectObject(&font2);
		pDC->TextOut(260,yp*18, "<- (od g. 2697 pr.n.e.)");
		pDC->SelectObject(&font1);

		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;


	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Znak    : ");
	
	sBuf6.Format("%s",(cBuff1=strupr_s2( kin_znak[iKinZnak])));
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

		Ucitaj_Glif(pDC, 1, iKinZnak, 130, yp*18);

		pDC->SetTextColor(Colors[4]);
		pDC->TextOut(210,yp*18, sBuf6);
		yp++;
	}
	y++;

	pDC->SetTextColor(Colors[0]);
	sBuf6=OpisKinZnaka[iKinZnak];
	if(sBuf6.GetLength()>120) {

		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,sBuf6.Mid(0,120)+"-");

		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
		y++;
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,sBuf6.Mid(120));
	}
	else { 
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,sBuf6);
		
	}

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;


	if(bd>0) {

		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
		y++;

		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

			pDC->SetTextColor(Colors[0]);
			pDC->TextOut(10,yp*18,"Podznak : ");

			sBuf6.Format("%s",(cBuff2=strupr_s2( kin_znak[iKinPodznak])) );

			Ucitaj_Glif(pDC, 1, iKinPodznak, 130, yp*18);

			pDC->SetTextColor(Colors[4]);
			pDC->TextOut(210,yp*18,sBuf6);

			yp++;
		}
		y++;

		pDC->SetTextColor(Colors[0]);
		sBuf6=OpisKinZnaka[iKinPodznak];
		
		if(sBuf6.GetLength()>120) {

			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,sBuf6.Mid(0,120)+"-");

			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
			y++;
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,sBuf6.Mid(120));
		}
		else { 
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,sBuf6);
		
		}	

		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
		y++;

	}
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

			pDC->SetTextColor(Colors[0]);
			pDC->TextOut(10,yp*18,"Element :");

			Ucitaj_Glif(pDC, 3, iKinElement, 130, yp*18);

			pDC->SetTextColor(Colors[4]);
			sBuf6.Format("%s",(cBuff3=strupr_s2( kin_elmt[iKinElement])) );
			pDC->TextOut(210,yp*18,sBuf6);
			yp++;
	}
	y++;

	pDC->SetTextColor(Colors[0]);

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	pDC->SelectObject(&font3);

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,"Inace, elementi u kin. astrlogiji su:");

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,"  - Voda, odgovara joj Merkur, a od dijela tjela kriza.");

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,"  - Vatra, odgovara joj Mars, a od dijela tjela srce.");

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,"  - Drvo, odgovara joj Jupiter, a od dijela tjela jetra.");

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,"  - Metal, odgovara joj Venera, a od dijela tjela pluca.");

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,"  - Zemlja, odgovara joj Saturn, a od dijela tjela slezena.");

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;
	pDC->SelectObject(&font1);
	
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
	
		pDC->SetTextColor(Colors[0]);
		pDC->TextOut(10,yp*18,"Nacelo  : ");

		Ucitaj_Glif(pDC, 3, 6, 130, yp*18-10);

		pDC->SetTextColor(Colors[4]);

		sBuf6.Format("%s (hrv. %s)",(cBuff4=strupr_s2( kin_nacel[iKinNacelo])), (cBuff5=strupr_s2( kin_nacel_hrv[iKinNacelo]) ) );
		pDC->TextOut(210,yp*18,sBuf6);
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;



skipKineski:
CString sT1, sT2;
int AstZnTonal, AstDanTonal, Trecena, flagNeg;
CString aztec_znk2[21]={"kajman (Cipactli)","vjetar (Ehecatl)","kuca (Calli)","guster (Cuetzpalin)","zmija (Coatl)",
                        "lubanja (Miquiztli)","jelen (Mazatl)","zec (Tochtli)","voda (Atl)","ofucani pas (Itzcuintli)",
						"majmun (Ozomahtli)","trava (Malinalli)", "trska (Acatl)","jaguar (Ocelotl)","orao (Cuauhtli)",
						"lešinar (Cozcacuauhtli)", "potres (Ollin)","kremen (Tecpatl)","kiša (Quiahuitl)","cvijet (Xochitl)",
						"prazni (nesretni) dani"};
int  trecena[20]={ 0,13, 6,19,12, 5, 18, 11, 4,17,
                  10, 3,16, 9, 2, 15, 8, 1, 14, 7     };
char * ast_mj[21]={"vode","proljeca","cvijeca","polja","suse","namirnica","soli","kuruze","veselica","vatre","majke-zemlje","povratka bogova","planine","lova","perja","kise","zvijezda","rasta", "prazni (nesretni) dani"};


if(opcSpec>0 && opcSpec!=2) goto skipAstecki;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;


	//********************** azteèki ******************************************
    fajl=fopen("zvz_azt.dat","rt"); if(fajl==NULL) { MessageBox("Nema fajla ZVZ.AZT.DAT !"); return;}
    for(i=1;i<=20;i++) {
		fgets(buff,199,fajl); buff[strlen(buff)-1]=0; 
		sOpisAztZnaka[i-1].Format("%s",buff);
		fgets(buff,199,fajl); buff[strlen(buff)-1]=0; 
		sOpisAztZnaka2[i-1].Format("%s",buff);
	}
	fclose(fajl);


// sakralni kalendar  //uzeto da 1.kajmana poèinje 11.10.1900

int pDan, pMje;  
pDan=13; pMje=2;   //svjetovna god. 13.2.

int tDan, tMje,  tGod; 
tDan=11; tMje=10; tGod=1900;  //1.kajmana uzet dan 11.10.1900

flagNeg=0;



if (godx  >tGod  || ( godx==tGod && mjex>tMje)  )  r=Razlika_Datuma(tDan,tMje,tGod, mjex,godx)+(danx-1); // 1.kajmana
if (godx== tGod &&   mjex==tMje && danx>=tDan )    r=danx-tDan;
if (godx== tGod &&   mjex==tMje && danx<tDan)    { r=-(tDan-1-danx); flagNeg=1;}
if (godx  <tGod || ( godx==tGod && mjex<tMje) )  { r=-(Razlika_Datuma( danx, mjex, godx, tMje,tGod)+(tDan-1-1) );flagNeg=1;}

//sBuf6.Format("r:%d",r);MessageBox(sBuf6);

if (flagNeg==0) {
	AstZnTonal= fmod2(r,20); 
	AstDanTonal= fmod2(r,13); 
	Trecena = floor2(fmod2(r,260)/13);
}
else {
	AstZnTonal= 19-fmod2(fabs2(r),20); 
	AstDanTonal= 12-fmod2(fabs2(r),13); 
	Trecena= 19-floor2(fmod2(fabs2(r),260)/13);
}

// sunèev kalendar
int AstZn, AstMJ;
flagNeg=0;

if (mjex>pMje )				     r=Razlika_Datuma(pDan,pMje,godx, mjex,godx)+(danx-1); // poè. 2,2,
if (mjex==pMje && danx>=pDan)    r=danx-pDan; 
if (mjex==pMje && danx<pDan)    {r=danx-(pDan-1); flagNeg=1;}

if (mjex<pMje )	{                r=-(Razlika_Datuma(danx, mjex,godx,pMje,godx)+pDan-1-1); flagNeg=1; } // 1.kajmana

if (flagNeg==0){ 
	AstZn=fmod2(r,20); 
	AstMJ= floor2(r/20);	
}
else  { 
	int prestupna=0; if (fmod2(godx,4)==0 ) if ( fmod2(godx,100)!= 0 || fmod2(godx,400)==0 || godx<1600) prestupna=1;
	if (fabs2(r)>4+prestupna) {
		r=fabs2(r)-(5+prestupna);   //oduzmi prazne dane
		AstZn=19-fmod2(r,20); 
		AstMJ=17-floor2(r/20);	 
	}
	else {AstZn=20; AstMJ=18;	 }

}




//sBuf6.Format("%d",AstZn); MessageBox(sBuf6);

//crtanje


	pDC->SetTextColor(Colors[0]);

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->TextOut(10,yp*18, "ASTECKI HOROSKOP ");
		
		pDC->MoveTo(5,yp*18+22); pDC->LineTo(170,yp*18+22);
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;


	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->SelectObject(&font3);
		pDC->TextOut(10,yp*18, "Tonalpohualli - obredni kalendar 260 dana (oznacen kombinacijom rednog broja dana (1.-13.) i jednim od 20 znakova)");
	
		pDC->MoveTo(5,yp*18+20); pDC->LineTo(655,yp*18+20);
		pDC->SelectObject(&font1);
		
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Dan       :");
	
sT1=aztec_znk2[AstZnTonal]; sT1.MakeUpper( );

sBuf6.Format("%d. %s",AstDanTonal+1,sT1 );

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {


	Ucitaj_Glif(pDC, 6, AstDanTonal+1, 85, yp*18);  //brojevi
	Ucitaj_Glif(pDC, 2, AstZnTonal+1, 85+15*(0.1+floor2((AstDanTonal+1)/5)+(fmod2(AstDanTonal+1,5)>0)), yp*18);


	pDC->SetTextColor(Colors[4]);
	pDC->TextOut(210,yp*18, sBuf6);

	yp++;
}
y++;


pDC->SetTextColor(Colors[0]);

		sBuf6=sOpisAztZnaka2[AstZnTonal];
		
temp = floor2(sBuf6.GetLength()/120);
	for(kk=0;kk<=temp;kk++) {

		if (kk<temp) sprintf(bff,"%.120s-",sBuf6.Mid(120*kk) );
		else		 sprintf(bff,"%.120s",sBuf6.Mid(120*kk)  );


			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,bff);
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
			y++;
	} 

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Osobine :");

		sBuf6=sOpisAztZnaka[AstZnTonal];
		
temp = floor2(sBuf6.GetLength()/120);
	for(kk=0;kk<=temp;kk++) {

		if (kk<temp) sprintf(bff,"%.120s-",sBuf6.Mid(120*kk) );
		else		 sprintf(bff,"%.120s",sBuf6.Mid(120*kk)  );


			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,bff);
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
			y++;
	} 

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;



if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Trecena   :");
	
sT1=aztec_znk2[trecena[Trecena]]; sT1.MakeUpper( );

sBuf6.Format("%d. (%s)",Trecena+1,sT1 );

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	Ucitaj_Glif(pDC, 2, trecena[Trecena]+1, 125, yp*18);

	pDC->SetTextColor(Colors[4]);
	pDC->TextOut(210,yp*18, sBuf6);

 	pDC->SetTextColor(Colors[0]);
	size=pDC->GetTextExtent(sBuf6, sBuf6.GetLength() );
	pDC->SelectObject(&font3);	
	pDC->TextOut(210+size.cx+20,yp*18,"  -> redni broj 13-dnevnog bloka unutar 260-dnevnog ciklusa");
	pDC->SelectObject(&font1);
 
	yp++;
}
y++;

pDC->SetTextColor(Colors[0]);

		sBuf6=sOpisAztZnaka2[trecena[Trecena]];
		
temp = floor2(sBuf6.GetLength()/120);
	for(kk=0;kk<=temp;kk++) {

		if (kk<temp) sprintf(bff,"%.120s-",sBuf6.Mid(120*kk) );
		else		 sprintf(bff,"%.120s",sBuf6.Mid(120*kk)  );


			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,bff);
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
			y++;
	} 

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Osobine :");

		sBuf6=sOpisAztZnaka[trecena[Trecena]];
		
temp = floor2(sBuf6.GetLength()/120);
	for(kk=0;kk<=temp;kk++) {

		if (kk<temp) sprintf(bff,"%.120s-",sBuf6.Mid(120*kk) );
		else		 sprintf(bff,"%.120s",sBuf6.Mid(120*kk)  );


			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,bff);
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
			y++;
	} 

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;


if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;




//********************************************************
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->SelectObject(&font3);
		pDC->TextOut(10,yp*18, "Xiuhpohualli - svjetovni suncani kalendar (18 mjeseci x 20 dana + 5 praznih dana), pretpostavlja se da je pocinjao 13.Veljace ");
		pDC->MoveTo(5,yp*18+20); pDC->LineTo(695,yp*18+20);
		pDC->SelectObject(&font1);
	
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;



if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "dnevni znak :");
	
sT1=aztec_znk2[AstZn]; sT1.MakeUpper( );

sBuf6.Format("%s",sT1 );

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	Ucitaj_Glif(pDC, 2, AstZn+1, 125, yp*18);
	
	pDC->SetTextColor(Colors[4]);
	pDC->TextOut(210,yp*18, sBuf6);

	yp++;
}
y++;

pDC->SetTextColor(Colors[0]);

		sBuf6=sOpisAztZnaka[AstZn];
		
temp = floor2(sBuf6.GetLength()/120);
	for(kk=0;kk<=temp;kk++) {

		if (kk<temp) sprintf(bff,"%.120s-",sBuf6.Mid(120*kk) );
		else		 sprintf(bff,"%.120s",sBuf6.Mid(120*kk)  );


			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(210,yp*18,bff);
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
			y++;
	} 

		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
		y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;


if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Mjesec :");
	
sT2=ast_mj[AstMJ]; sT2.MakeUpper( );

sBuf6.Format("%s", sT2 );

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
	pDC->SetTextColor(Colors[4]);
	pDC->TextOut(210,yp*18, sBuf6);

	yp++;
}
y++;


pDC->SetTextColor(Colors[0]);

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
y++;





pDC->SelectObject(&font3);

for(i=1; i<=2; i++) {


	if(i==1) {

		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) )  {
			pDC->TextOut(10,yp*18, "20 dnevnih znakova su: ");
		}
		sBuf6=""; for(kk=0;kk<=19;kk++) {
			if (kk==AstZnTonal || kk==AstZn)  aztec_znk2[kk].MakeUpper(); 
			else aztec_znk2[kk].MakeLower();
			sBuf6=sBuf6 + aztec_znk2[kk];
			if (kk<19) sBuf6+=", ";
		}
	}


	if(i==2) {
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) )  {
			pDC->TextOut(10,yp*18,"18 mjeseca u Asteckom horoskopu :");
		}
		sBuf6=""; for(kk=0;kk<=17;kk++) {
					if (kk!=AstMJ ) sBuf6=sBuf6 + _strlwr(ast_mj[kk]) ; 
					else {
						
						sBuf6=sBuf6 + (cBuff1=strupr_s2( ast_mj[kk]));
						} 
					if (kk<17) sBuf6+=", ";
					}	

	}

temp = floor2(sBuf6.GetLength()/120);
	for(kk=0;kk<=temp;kk++) {

		if (kk<temp) sprintf(bff,"%.120s-",sBuf6.Mid(120*kk) );
		else		 sprintf(bff,"%.120s",sBuf6.Mid(120*kk)  );


			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(240,yp*18,bff);
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
			y++;
	} 

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

}

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;


//****************************majanski horoskop ***********************************************************

skipAstecki:

CString MAYAN_HAAB_MONTHS[19] = {"Pop (otirac)", "Uo (crno spajanje)", "Zip (crveno spajanje)", "Zotz (sismis)", "Tzec (?)", 
                                 "Xul (pas)",  "Yaxkin (novo sunce)", "Mol (voda)", "Chen (crna oluja)", "Yax (zelena oluja)",
							     "Zac (bijela oluja)", "Ceh (crvena oluja)","Mac (zatvoren)", "Kankin (zuto sunce)", "Muan (sova)", 
								 "Pax (doba sadnje)", "Kayab (kornjaca)", "Cumku (ambar)", "Uayeb (nesretni dani)"};

CString MAYAN_TZOLKIN_MONTHS[21] = { "Imix (lopoc)", "Ik (vjetar)", "Akbal (noc)", "Kan (kuruza)", "Chicchan (zmija)",
                                     "Cimi (smrt)", "Manik (ruka)", "Lamat (venera)", "Muluc (voda)", "Oc (pas)",
                                     "Chuen (zaba)", "Eb (lubanja)", "Ben (stabljika)", "Ix (jaguar)", "Men (covjek)",
                                     "Cib (skoljka)", "Caban (zemlja)", "Etznab (kremen)", "Cauac (oblak)", "Ahau (gospodar)"};

CString redbr[22] = { "-", "I", "II", "III", "IV", "V",
                                     "VI", "VII", "VIII", "IX", "X",
                                     "XI", "XII", "XIII", "XIV", "XV",
                                     "XVI", "XVII", "XVIII", "XIX", "XX"};

if(opcSpec>0 && opcSpec!=3) goto skipMajanski;


//raèunanje

//longcount

tDan=21; tMje=12; tGod=2012;  //13.0.0.0.0

int  d, baktun, katun, tun, uinal, kin;

flagNeg=0;
if (godx  >tGod  || ( godx==tGod && mjex>tMje)  )  r=Razlika_Datuma(tDan,tMje,tGod, mjex,godx)+(danx-1); 
if (godx== tGod &&   mjex==tMje && danx>=tDan )    r=danx-tDan;
if (godx== tGod &&   mjex==tMje && danx<tDan)    { r=-(tDan-1-danx); flagNeg=1;}
if (godx  <tGod || ( godx==tGod && mjex<tMje) )  { r=-(Razlika_Datuma( danx, mjex, godx, tMje,tGod)+(tDan-1-1) );flagNeg=1;}

//sBuf6.Format("%d",r); MessageBox(sBuf6);

/*
    jd = Math.floor2(jd) + 0.5;
    d = jd - MAYAN_COUNT_EPOCH;
    baktun = Math.floor2(d / 144000);
    d = mod(d, 144000);
    katun = Math.floor2(d / 7200);
    d = mod(d, 7200);
    tun = Math.floor2(d / 360);
    d = mod(d, 360);
    uinal = Math.floor2(d / 20);
    kin = mod(d, 20);
*/


if (flagNeg==0) {

    kin    = fmod2(r, 20);
    uinal  = fmod2(floor2(r/20),18);
    tun    = fmod2(floor2(r / 360),20);
    katun  = fmod2(floor2(r / 7200),20);
    baktun = fmod2(12+floor2(r / 144000),20)+1;
}
else {

    kin = 19-fmod2(fabs2(r), 20);
    uinal = 17-fmod2(floor2(fabs2(r)/20),18);
    tun = 19-fmod2(floor2(fabs2(r)/360),20);
    katun = 19-fmod2(floor2(fabs2(r)/7200),20);
    baktun = 19-fmod2(floor2(fabs2(r)/144000),20);

}

//sBuf6.Format("%d %d %d %d %d",baktun, katun, tun, uinal,kin);  MessageBox(sBuf6);






//haab
/*   var lcount, day;

    jd = Math.floor2(jd) + 0.5;
    lcount = jd - MAYAN_COUNT_EPOCH;
    day = mod(lcount + 8 + ((18 - 1) * 20), 365);

    return new Array (Math.floor2(day / 20) + 1, mod(day, 20));

*/
// sunèev kalendar

	pDan=2; pMje=4;   //svjetovna god. 2.4.


int MayZn, MayMJ;
flagNeg=0;

if (mjex>pMje )				     r=Razlika_Datuma(pDan,pMje,godx, mjex,godx)+(danx-1); 
if (mjex==pMje && danx>=pDan)    r=danx-pDan; 
if (mjex==pMje && danx<pDan)    {r=danx-(pDan-1); flagNeg=1;}

if (mjex<pMje )	{                r=-(Razlika_Datuma(danx, mjex,godx,pMje,godx)+pDan-1-1); flagNeg=1; } 

if (flagNeg==0){ 
	MayZn=fmod2(r,20); 
	MayMJ= floor2(r/20);	
}
else  { 
	int prestupna=0; if (fmod2(godx,4)==0 ) if ( fmod2(godx,100)!= 0 || fmod2(godx,400)==0 || godx<1600) prestupna=1;
	if (fabs2(r)>4+prestupna) {
		r=fabs2(r)-(5+prestupna);   //oduzmi prazne dane
		MayZn=19-fmod2(r,20); 
		MayMJ=17-floor2(r/20);	 
	}
	else {
		MayZn=4+prestupna-fabs2(r); 
		MayMJ=18;	 }

}




//tzolkin

/*
	jd = Math.floor2(jd) + 0.5;
    lcount = jd - MAYAN_COUNT_EPOCH;
    return new Array (amod(lcount + 20, 20), amod(lcount + 4, 13));
*/

// sakralni kalendar  
tDan=28; tMje=6; tGod=1901;  //1.imix 

int MayZnTzolkin, MayDanTzolkin;
flagNeg=0;
if (godx  >tGod  || ( godx==tGod && mjex>tMje)  )  r=Razlika_Datuma(tDan,tMje,tGod, mjex,godx)+(danx-1); 
if (godx== tGod &&   mjex==tMje && danx>=tDan )    r=danx-tDan;
if (godx== tGod &&   mjex==tMje && danx<tDan)    { r=-(tDan-1-danx); flagNeg=1;}
if (godx  <tGod || ( godx==tGod && mjex<tMje) )  { r=-(Razlika_Datuma( danx, mjex, godx, tMje,tGod)+(tDan-1-1) );flagNeg=1;}

if (flagNeg==0) {
	MayZnTzolkin= fmod2(r,20); 
	MayDanTzolkin= fmod2(r,13); 
	
}
else {
	MayZnTzolkin= 19-fmod2(fabs2(r),20); 
	MayDanTzolkin= 12-fmod2(fabs2(r),13); 
}


//****************************************************************************










	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;



	pDC->SetTextColor(Colors[0]);

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->TextOut(10,yp*18, "MAJANSKI KALENDAR ");
		
		pDC->MoveTo(5,yp*18+22); pDC->LineTo(170,yp*18+22);
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->SelectObject(&font3);
		pDC->TextOut(10,yp*18, "Tzolk'in - obredni kalendar 260 dana (oznacen kombinacijom rednog broja dana (1.-13.) i jednim od 20 tzv. 'mjeseci')");
	
		pDC->MoveTo(5,yp*18+20); pDC->LineTo(655,yp*18+20);
		pDC->SelectObject(&font1);
		
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Dan :");
	
sT1=MAYAN_TZOLKIN_MONTHS[MayZnTzolkin]; sT1.MakeUpper( );

sBuf6.Format("%d. %s (%s)",MayDanTzolkin+1,sT1, redbr[MayZnTzolkin+1] );

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	Ucitaj_Glif(pDC, 6, MayDanTzolkin+1, 85, yp*18);  //brojevi
	Ucitaj_Glif(pDC, 4, MayZnTzolkin+1, 85+15*(0.5+floor2((MayDanTzolkin+1)/5)+(fmod2(MayDanTzolkin+1,5)>0)), yp*18);

	pDC->SetTextColor(Colors[4]);
	pDC->TextOut(250,yp*18, sBuf6);

	yp++;
}
y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;


pDC->SetTextColor(Colors[0]);

//********************************************************
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->SelectObject(&font3);
		pDC->TextOut(10,yp*18, "Haab - svjetovni suncani kalendar (18 mjeseci x 20 dana + 5 praznih dana), pretpostavlja se da je pocinjao 2.Travnja ");
		pDC->MoveTo(5,yp*18+20); pDC->LineTo(655,yp*18+20);
		pDC->SelectObject(&font1);
	
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;



if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Dan :");
	
sT1=MAYAN_HAAB_MONTHS[MayMJ]; sT1.MakeUpper( );

sBuf6.Format("%d. %s (%s)",MayZn+1, sT1, redbr[MayMJ+1] );

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	Ucitaj_Glif(pDC, 6, MayZn+1, 85, yp*18);
	Ucitaj_Glif(pDC, 5, MayMJ+1, 85+15*(0.5+floor2((MayZn+1)/5)+(fmod2(MayZn+1,5)>0)), yp*18);
	
	pDC->SetTextColor(Colors[4]);
	pDC->TextOut(250,yp*18, sBuf6);

	yp++;
}
y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;


// upute ************************************************************

pDC->SetTextColor(Colors[0]);
pDC->SelectObject(&font3);

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

for(i=1; i<=2; i++) {

	if(i==1) {

		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) )  {
			pDC->TextOut(10,yp*18, "20 mjeseca Tzolkina su: ");
		}
		sBuf6=""; for(kk=0;kk<=19;kk++) {
			if (kk==MayZnTzolkin)  MAYAN_TZOLKIN_MONTHS[kk].MakeUpper(); 
			else MAYAN_TZOLKIN_MONTHS[kk].MakeLower();
			sBuf6=sBuf6 + MAYAN_TZOLKIN_MONTHS[kk];
			if (kk<19) sBuf6+=", ";
		}
	}


	if(i==2) {
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) )  {
			pDC->TextOut(10,yp*18,"18+1 mjeseca Haaba su :");
		}
		sBuf6=""; for(kk=0;kk<=18;kk++) {

					if (kk!=MayMJ ) MAYAN_HAAB_MONTHS[kk].MakeLower() ; 
					else MAYAN_HAAB_MONTHS[kk].MakeUpper(); 
					
					sBuf6=sBuf6 + MAYAN_HAAB_MONTHS[kk];

					if (kk<18) sBuf6+=", ";
					}	

	}

temp = floor2(sBuf6.GetLength()/120);
	for(kk=0;kk<=temp;kk++) {

		if (kk<temp) sprintf(bff,"%.120s-",sBuf6.Mid(120*kk) );
		else		 sprintf(bff,"%.120s",sBuf6.Mid(120*kk)  );


			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(250,yp*18,bff);
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
			y++;
	} 

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

}
pDC->SetTextColor(Colors[0]);

//********************************************************

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->SelectObject(&font3);
		pDC->TextOut(10,yp*18, "Dugi kalendar");
		pDC->MoveTo(5,yp*18+20); pDC->LineTo(95,yp*18+20);
		pDC->SelectObject(&font1);
	
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;



if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Dan :");
	
sBuf6.Format("%d.%d.%d.%d.%d", baktun, katun, tun, uinal,kin );

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	pDC->SetTextColor(Colors[4]);
	pDC->TextOut(250,yp*18, sBuf6);

	pDC->SetTextColor(Colors[0]);
	pDC->SelectObject(&font3);
	pDC->TextOut(380,yp*18, "-> Baktun (1-13) . Katun (0-19) . Tun (0-19) . Uinal (0-17) . Kin (0-19 dana) ");
	pDC->SelectObject(&font1);


	yp++;
}
y++;

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	pDC->SelectObject(&font3);
	pDC->TextOut(390,yp*18, "uzeto da novi baktun pocinje 21.12.2012 (13.baktun je ujedno kod Maja bio tzv. 'poc.novog stvaranja') ");
	pDC->SelectObject(&font1);


	yp++;
}
y++;



	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;



skipMajanski:

int pomak=0;

CString Egip_Mjeseci[5] = {"I","II","III","IV"};
CString Egip_Mjeseci_Imena[15] = {"Tekh (Thoth)","Menhet (Phaophi)","Hwt-hwr (Hathor)","Ka-hr-ka (Nehebkau)",
                                  "Sf-bdt (Ta-aabet)","Rekh-wer (Mekhir)","Rekh-neds (Paremhat)","Renwet (Pharmuthi)",
								  "Hnsw (Pakhon)","Hnt-hjt (Payni)","Lpt-hmt (Epip)","Wep-renpet (Mesori)", " (epagomenalni dani)"};

CString Egip_Doba[5] = {"Akhet (Poplava)","Peret (Rast,Zima)", "Shemu (Žetva, Ljeto)", "epagomenalni dani"};


if(opcSpec>0 && opcSpec!=4) goto skipEgipacki;

//********************  egipacki ********************************************
tDan=20; tMje=7; tGod=139;  //novi sotièki ciklus i uz to 1 thot - nova civ.godina
flagNeg=0;


if (godx  >tGod  || ( godx==tGod && mjex>tMje)  )  r=Razlika_Datuma(tDan,tMje,tGod, mjex,godx)+(danx-1); // 1.kajmana
if (godx== tGod &&   mjex==tMje && danx>=tDan )    r=danx-tDan;
if (godx== tGod &&   mjex==tMje && danx<tDan)    { r=-(tDan-1-danx); flagNeg=1;}
if (godx  <tGod || ( godx==tGod && mjex<tMje) )  { r=-(Razlika_Datuma( danx, mjex, godx, tMje,tGod)+(tDan-1-1) );flagNeg=1;}

int EgDan, EgMj, EgDoba, SotGod;


//civilna nova godina:
if (flagNeg==0) {

    EgDan   = fmod2(fabs2(r), 365);
    EgMj    = floor2(EgDan/30);
    EgDoba  = floor2(EgMj/4);
	SotGod  = fmod2(godx-tGod,1460);
}
else {

    EgDan   = 364-fmod2(fabs2(r), 365);   //dani idu od 0..364
    EgMj    = floor2(EgDan/30);  // mj od 0..11+1
    EgDoba  = floor2(EgMj/4);
	SotGod  = 1459-fmod2(tGod-godx,1460);

}

//sBuf6.Format("r:%d sd:%d sm:%d sdb:%d",r, EgSotDan, EgSotMj,EgSotDoba);MessageBox(sBuf6);



//sotièka nova godina:
int EgSotDan, EgSotMj, EgSotDoba;

if (m_dPostavke.m_bSotGodPrecesija==TRUE) {


	pomak= floor2((godx-139)*36./(4210+139)); //pomak sotièke godine (izlazak siriusa) zbog precesije
	tDan=tDan+pomak;

	while (tDan<1 ) {

		tMje--;
		tDan=tDan+pl[tMje-1];

	} 
	while (tDan>pl[tMje-1] ) {

		tDan=tDan-pl[tMje-1];
		tMje++;

	} 
} 

//sBuf6.Format("pomak:%d dan:%d mje:%d",pomak, tDan,tMje);MessageBox(sBuf6);



	sBuf6=Nadji_Egipatsku_Godinu(tDan, tMje, godx);
	//f. razjebava objx polja, pa nakon nje  treba ponovo pozvati planete 
	Planete(danx,mjex,godx,satx,minx);

	int ppDan, ppMje;                         //poèetak sotièke n.g., lunarni kal. <- poèinje s mladim mjesecom
	sscanf(sBuf6,"%d %d",&ppDan, &ppMje);




flagNeg=0;
if (mjex>ppMje )				   r=Razlika_Datuma(ppDan,ppMje,godx, mjex,godx)+(danx-1); 
if (mjex==ppMje && danx>=ppDan)    r=danx-ppDan; 
if (mjex==ppMje && danx<ppDan)    {r=danx-(ppDan-1); flagNeg=1;}

if (mjex<ppMje )	{              r=-(Razlika_Datuma(danx, mjex,godx,ppMje,godx)+ppDan-1-1); flagNeg=1; } 

if (flagNeg==0){ 
    EgSotDan   = fabs2(r);
    EgSotMj    = floor2(EgSotDan/30);
    EgSotDoba  = floor2(EgSotMj/4);
}
else  { 

    EgSotDan    = 364-fabs2(r);   //dani idu od 0..364
    EgSotMj    = floor2(EgSotDan/30);  // mj od 0..11+1
    EgSotDoba  = floor2(EgSotMj/4);

}




	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;



	pDC->SetTextColor(Colors[0]);

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->TextOut(10,yp*18, "EGIPATSKI KALENDAR ");
		
		pDC->MoveTo(5,yp*18+22); pDC->LineTo(170,yp*18+22);
		yp++;
	}
	y++;



	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;




	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->SelectObject(&font3);
		pDC->TextOut(10,yp*18, "Soticko-lunarna godina - vezana uz izlazak Sirijusa (grc.Sothis, egip.Sodpet); u doba starog egipta koincidirao je s  periodicnim god.poplavama Nila");
	
		pDC->MoveTo(5,yp*18+20); pDC->LineTo(820,yp*18+20);
		pDC->SelectObject(&font1);
		
		yp++;
	}
	y++;


	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;


	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Izlazak Siriusa (Sodpeta):");

	
	sBuf6.Format("%02d.%02d.",tDan,tMje );

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

		pDC->SetTextColor(Colors[4]);
		pDC->TextOut(250,yp*18, sBuf6);

		Ucitaj_Glif(pDC, 7, 9, 935, yp*18-45);

		pDC->SetTextColor(Colors[0]);
		
		if ( m_dPostavke.m_bSotGodPrecesija==TRUE ) sBuf6.Format("(pomak od  %d dana u odn. na 139AD)",pomak );
		else                                        sBuf6.Format("(ignoriran utj.precesije -> vidi postavke)" );
		
		
		
		
		
		pDC->TextOut(310,yp*18, sBuf6);

		pDC->SelectObject(&font3);
		pDC->TextOut(600,yp*18, "<- pocetak se mijenja zbog precesije; pretpostavlja se da je ");
		pDC->SelectObject(&font1);

		yp++;
	}
	y++;


	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->SelectObject(&font3);
		pDC->TextOut(600,yp*18, "   god. 139AD na podr. Egipta sluzbeno pocinjala 20.7. ");
		pDC->SelectObject(&font1);
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;






	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Pocetak nove godine :");

	
	sBuf6.Format("%02d.%02d.",ppDan,ppMje );

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

		pDC->SetTextColor(Colors[4]);
		pDC->TextOut(250,yp*18, sBuf6);

		pDC->SetTextColor(Colors[0]);
		pDC->SelectObject(&font3);
		pDC->TextOut(600,yp*18, "<- prvi mladi mjesec nakon izlaska Siriusa");

		pDC->SelectObject(&font1);

		yp++;
	}
	y++;


	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->SelectObject(&font3);
		pDC->TextOut(940,yp*18-8, "Sodpet");
		pDC->SelectObject(&font1);
	}
	
	
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;



if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Dan (staro kraljevstvo):");
	
sT1=Egip_Doba[(int)floor2(EgSotMj/4)]; sT1.MakeUpper( );
sT2=Egip_Mjeseci_Imena[(int)EgSotMj]; sT2.MakeUpper( );

sBuf6.Format("-> %d. dan, %s mjeseca, god.doba %s",
			  (int)fmod2(EgSotDan,30)+1, Egip_Mjeseci[(int)fmod2(EgSotMj,4)], sT1 );

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	if(EgSotDan<360){
		Ucitaj_Glif(pDC, 7, 8, 250, yp*18-4);
		Ucitaj_Glif(pDC, 7, (int)fmod2(EgSotMj,4)+1, 250+35, yp*18-4+3);
		Ucitaj_Glif(pDC, 7, (int)floor2(EgSotMj/4)+5, 250+60, yp*18-4);
	}

	pDC->SetTextColor(Colors[4]);
	pDC->TextOut(380,yp*18, sBuf6);
	pDC->SetTextColor(Colors[0]);

	yp++;
}
y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;





if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Dan (novo kraljevstvo):");
	
sBuf6.Format("-> %d. dan,  mjeseca %s",
			  (int)fmod2(EgSotDan,30)+1, sT2 );

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	//Ucitaj_Glif(pDC, 4, MayZnTzolkin+1, 85+15*(0.5+floor2((MayDanTzolkin+1)/5)+(fmod2(MayDanTzolkin+1,5)>0)), yp*18);


	pDC->SetTextColor(Colors[4]);
	pDC->TextOut(380,yp*18, sBuf6);
	pDC->SetTextColor(Colors[0]);

	yp++;
}
y++;


	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;






	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->SelectObject(&font3);
		pDC->TextOut(10,yp*18, "Civilna godina - imala je tocno 365 dana, tj. 12 mj x 30 dana  + 5 dodatna dana, bez prestupnih godina (klizajuca)");
	
		pDC->MoveTo(5,yp*18+20); pDC->LineTo(635,yp*18+20);
		pDC->SelectObject(&font1);
		
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;




if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Dan (staro kraljevstvo):");
	
sT1=Egip_Doba[(int)floor2(EgMj/4)]; sT1.MakeUpper( );
sT2=Egip_Mjeseci_Imena[(int)EgMj]; sT2.MakeUpper( );

sBuf6.Format("%d. dan, %s mjeseca, god.doba %s",
			  (int)fmod2(EgDan,30)+1, Egip_Mjeseci[(int)fmod2(EgMj,4)], sT1 );

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	if(EgDan<360){
		Ucitaj_Glif(pDC, 7, 8, 250, yp*18-4);
		Ucitaj_Glif(pDC, 7, (int)fmod2(EgMj,4)+1, 250+35, yp*18-4+3);
		Ucitaj_Glif(pDC, 7, (int)floor2(EgMj/4)+5, 250+60, yp*18-4);
	}

	pDC->SetTextColor(Colors[4]);
	pDC->TextOut(380,yp*18, sBuf6);
	pDC->SetTextColor(Colors[0]);


	yp++;
}
y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;




if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Dan (novo kraljevstvo):");
	
sBuf6.Format("%d. dan,  mjeseca %s",
			  (int)fmod2(EgDan,30)+1, sT2 );

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {


	pDC->SetTextColor(Colors[4]);
	pDC->TextOut(380,yp*18, sBuf6);
	pDC->SetTextColor(Colors[0]);

	pDC->SelectObject(&font3);
	pDC->TextOut(690,yp*18, "<- pretpostavlja se da je 139AD godina pocinjala");
	pDC->SelectObject(&font1);


	yp++;
}
y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

		pDC->SelectObject(&font3);
		pDC->TextOut(690,yp*18, "   20.7, i koincidirala s izlaskom Sothisa");
		pDC->SelectObject(&font1);

		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;





/*
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Godina sotickog ciklusa :");
	
sBuf6.Format("%d.",SotGod );

if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

	pDC->SetTextColor(Colors[4]);
	pDC->TextOut(380,yp*18, sBuf6);
	pDC->SetTextColor(Colors[0]);

	yp++;
}
y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;


*/

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;



//legenda za oznaku mjeseca:
	
if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {

 	pDC->SelectObject(&svjetlaTankaOlovka);           
	ArrowTo(pDC,  250+14, (yp)*18+5,250+14, (yp-4)*18+12,10,20);
	pDC->MoveTo(250+14, (yp)*18+5); pDC->LineTo(250+34, (yp)*18+5);
	pDC->SelectObject(&siva_olovka);

	pDC->SelectObject(&font3);

	pDC->TextOut(250+40,yp*18-4, "hijeroglif za oznaku mjeseca, predstavlja pojavu novog mjesecevog srpa iza mladog mjeseca, nakon izlaska zvijezde Sirijusa");
	pDC->SelectObject(&font1);

}

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;




//tumaè naziva mjeseci
pDC->SelectObject(&font3);

for(i=1; i<=2; i++) {

	if(i==1) {

		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) )  {
			pDC->TextOut(10,yp*18, "12+1 egip. mjeseca su: ");
		}
		sBuf6=""; for(kk=0;kk<=12;kk++) {
			if (kk==EgMj || kk==EgSotMj )  Egip_Mjeseci_Imena[kk].MakeUpper(); 
			else Egip_Mjeseci_Imena[kk].MakeLower();
			sBuf6=sBuf6 + Egip_Mjeseci_Imena[kk];
			if (kk<12) sBuf6+=", ";
		}
	}


	if(i==2) {
		if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) )  {
			pDC->TextOut(10,yp*18,"3 egip. godisnja doba su :");
		}
		sBuf6=""; for(kk=0;kk<=2;kk++) {

					if (kk!=floor2(EgMj/4) && kk!=floor2(EgSotMj/4) ) Egip_Doba[kk].MakeLower() ; 
					else Egip_Doba[kk].MakeUpper(); 
					
					sBuf6=sBuf6 + Egip_Doba[kk];

					if (kk<2) sBuf6+=", ";
					}	

	}


temp = floor2(sBuf6.GetLength()/120);
	for(kk=0;kk<=temp;kk++) {

		if (kk<temp) sprintf(bff,"%.120s-",sBuf6.Mid(120*kk) );
		else		 sprintf(bff,"%.120s",sBuf6.Mid(120*kk)  );


			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(250,yp*18,bff);
			if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
			y++;
	} 

	//if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	//y++;

}





	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;


pDC->SetTextColor(Colors[0]);
pDC->SelectObject(&font1);

/*

*/







skipEgipacki:
char *arap_znak[15]={" ","Koplje","Pracka","Bradva","Kama","Toljaga","Buzdovan","Veliki noz","Mac","Noz","Lanac","Arapska kama","Luk","Koplje" };
char *arap_elemt[15]={" ","dugo","dugo","srednje dugo","kratko","srednje dugo","srednje dugo","kratko","dugo","srednje dugo","kratko","kratko","dugo","dugo"};
if(opcSpec>0) goto skipArapski;

//************************* ARAPSKI ***********************************************
	
	pDC->SetTextColor(Colors[0]);
	pDC->SelectObject(&font1);

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	sunp = pretvorba[(int)(floor2(kut[0]/30))];


	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->TextOut(10,yp*18, "ARAPSKI HOROSKOP (po nazivima oružja)");
		pDC->MoveTo(5,yp*18+22); pDC->LineTo(305,yp*18+22);
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Suncani znak :");
	
	sBuf6.Format("%s",(cBuff2=strupr_s2( arap_znak[sunp])) );
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->SetTextColor(Colors[4]);
		pDC->TextOut(140,yp*18, sBuf6);
		yp++;
	}
	y++;

	pDC->SetTextColor(Colors[0]);

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;



	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) pDC->TextOut(10,yp*18, "Vrsta            :");
	
	sBuf6.Format("%s oruzje",(cBuff3=strupr_s2(arap_elemt[sunp])) );
	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) {
		pDC->SetTextColor(Colors[4]);
		pDC->TextOut(140,yp*18, sBuf6);
		yp++;
	}
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;

	if ((y>= (m_pdTumac->m_iPage-1)*iBrLinija && y<(m_pdTumac->m_iPage)*iBrLinija) ) yp++;
	y++;
}//if

skipArapski:


m_pdTumac->m_spinPage.SetRange(1, floor2(y/iBrLinija)+1); 
m_pdTumac->m_iMax = floor2(y/iBrLinija)+1; 
m_pdTumac->UpdatePageDisplay() ; 


// sve vrati

//oèisti sve korisštene ðiðe od MemDC pomoænog cdc-
//MemDC.SelectObject( pOldBitmap );
//DeleteDC(MemDC);

pDC->SelectObject(def_font);
font1.DeleteObject();
font2.DeleteObject();
font3.DeleteObject();

pDC->SelectObject(oldBrush);
delete brush;
pDC->SelectObject(staraOlovka);  // vrati staru olovku


if (cBuff1!=NULL) free(cBuff1);
if (cBuff2!=NULL) free(cBuff2);
if (cBuff3!=NULL) free(cBuff3);
if (cBuff4!=NULL) free(cBuff4);
if (cBuff5!=NULL) free(cBuff5);

}

//**********************************************************************************************************
void CAsc2winView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class


	m_pdTumac->m_iPage = pInfo->m_nCurPage;
	m_pdTumac->m_iPage2 = pInfo->m_nCurPage;

	
	CView::OnPrint(pDC, pInfo);

}





//**************************************************************************************************

CString CAsc2winView::Nadji_Kinesku_Godinu(int g)
{

	int DanPM=0, DanMM=0, MjeMM=1, iKinZnak;

	float punimj=180,temp, priblizava=0,  ip,j;

         // pronaði prvi puni mjesec u novoj godini, al da novi nije prije 21.1:
         for (ip=6;ip<=pl[0]; ip++) {

                Planete(ip,1,g,12,0);

                temp=fabs2(objx[1]-objx[2]); if (temp>180) temp=360-temp;
                temp=fabs2(180-temp);

                if(temp<punimj) {punimj=temp;}

                if(temp<6) {	DanPM=ip; break; }
                if(temp-punimj>0 && punimj<13) {	DanPM=ip; break; }
	}
//pronaði prvi mladi mjesec iza punog mjeseca u novoj godini, i to je koneska nova godina

    if (DanPM!=0) {
		  punimj=180;
		  priblizava=0;

		  for (ip=DanPM;ip<=pl[0]; ip++) {
				j=0;
looop:
                Planete(ip,1,g,j,0);
                temp=fabs2(objx[1]-objx[2]); if (temp>180) temp=360-temp;

                if(temp<punimj ) {
					if(punimj<180) priblizava=1;
					punimj=temp;
				}

                if(temp>punimj && priblizava==1) {	DanMM=ip; break; }

				j+=4; if (j<24) goto looop;
          }
	}

	if (DanMM==0) {
		  punimj=180;
		  MjeMM=2;
		  priblizava=0;

          for (ip=1;ip<=pl[1]; ip++) {
				j=0;
looop2:
                Planete(ip,2,g,j,0);
                temp=fabs2(objx[1]-objx[2]); if (temp>180) temp=360-temp;

                if(temp<punimj ) {
					if(punimj<180) priblizava=1;
					punimj=temp;
				}

                if(temp>punimj && priblizava==1) {	DanMM=ip; break; }

				j+=4; if (j<24) goto looop2;

          }
	}

	if (g>=1900)   iKinZnak=fmod2(fabs2(1900-g),12)+1;    //kin.znak od 1-12
	else           iKinZnak=11-fmod2(fabs2(g-1899),12) +1;





	CString bf;
	bf.Format("%02d %02d %02d", DanMM, MjeMM, iKinZnak);
	
	return bf;
}



//*******************************************************************************
void CAsc2winView::Ucitaj_Glif(CDC *pDC, int iMod, int iGlif, int x, int y)

{

	if(iMod<1 || iMod>7) return;

	if( iMod==1 && (iGlif<0 || iGlif>12) ) return;
	if( iMod==2 && (iGlif<1 || iGlif>20) ) return;
	if( iMod==3 && (iGlif<0 || iGlif>6) ) return;
	if( iMod==4 && (iGlif<1 || iGlif>20) ) return;
	if( iMod==5 && (iGlif<1 || iGlif>19) ) return;
	if( iMod==6 && (iGlif<1 || iGlif>20) ) return;
	if( iMod==7 && (iGlif<1 || iGlif>9)  ) return; //egip.



	if (iMod==6) {  //majanski brojevi


		int brCrta=floor2(iGlif/5);
		int brTocki=fmod2(iGlif,5);
		
		int i;

		CPen olovka(PS_SOLID, 6, Colors[9]);                 //tip, debljina, boja
		CPen* staraOlovka = pDC->SelectObject(&olovka);      // pohrani staru olovku

		CBrush* brush;
		brush = new CBrush(Colors[9]); //tip, boja
		CBrush* oldBrush = pDC->SelectObject(brush);// Select the new brush into the DC.

		for (i=1; i<=brTocki; i++) {

			pDC->Ellipse(x-4,y+(4-brTocki+fmod2(brTocki,2)*.5 )*15/2+(i-1)*15+7-4,x+4,y+(4-brTocki+0.5*fmod2(brTocki,2))*15/2+(i-1)*15+7+4);

		}

		for (i=1; i<=brCrta; i++) {

			pDC->RoundRect(x+(i-1+(brTocki>0) )*15-3,y,x+(i-1+(brTocki>0) )*15+3,y+60,4,4);

		}



		pDC->SelectObject(oldBrush);
		delete brush;
		pDC->SelectObject(staraOlovka);  // vrati staru olovku

		return;
}



	CBitmap bmTmp, bmDest;
    BITMAP bm;

	if (iMod==1){  //kineski
	switch (iGlif) {
	case 1:		bmTmp.LoadBitmap(IDB_KIN01); break;  //ako se želi izravno resource
	case 2:		bmTmp.LoadBitmap(IDB_KIN02); break;  //ako se želi izravno resource
	case 3:		bmTmp.LoadBitmap(IDB_KIN03); break;  //ako se želi izravno resource
	case 4:		bmTmp.LoadBitmap(IDB_KIN04); break;  //ako se želi izravno resource
	case 5:		bmTmp.LoadBitmap(IDB_KIN05); break;  //ako se želi izravno resource
	case 6:		bmTmp.LoadBitmap(IDB_KIN06); break;  //ako se želi izravno resource
	case 7:		bmTmp.LoadBitmap(IDB_KIN07); break;  //ako se želi izravno resource
	case 8:		bmTmp.LoadBitmap(IDB_KIN08); break;  //ako se želi izravno resource
	case 9:		bmTmp.LoadBitmap(IDB_KIN09); break;  //ako se želi izravno resource
	case 10:	bmTmp.LoadBitmap(IDB_KIN10); break;  //ako se želi izravno resource
	case 11:	bmTmp.LoadBitmap(IDB_KIN11); break;  //ako se želi izravno resource
	case 12:	bmTmp.LoadBitmap(IDB_KIN12); break;  //ako se želi izravno resource
	case 0:	    bmTmp.LoadBitmap(IDB_KIN12); break;  //ako se želi izravno resource
}}
	if (iMod==2){ //asteèki
	switch (iGlif) {
	case 1:	bmTmp.LoadBitmap(IDB_AZT01); break;   //ako se želi izravno resource
	case 2:	bmTmp.LoadBitmap(IDB_AZT02); break;  //ako se želi izravno resource
	case 3:	bmTmp.LoadBitmap(IDB_AZT03); break;  //ako se želi izravno resource
	case 4:	bmTmp.LoadBitmap(IDB_AZT04); break;  //ako se želi izravno resource
	case 5:	bmTmp.LoadBitmap(IDB_AZT05); break ; //ako se želi izravno resource
	case 6:	bmTmp.LoadBitmap(IDB_AZT06); break;  //ako se želi izravno resource
	case 7:	bmTmp.LoadBitmap(IDB_AZT07); break;  //ako se želi izravno resource
	case 8:	bmTmp.LoadBitmap(IDB_AZT08); break;  //ako se želi izravno resource
	case 9:	bmTmp.LoadBitmap(IDB_AZT09); break;  //ako se želi izravno resource
	case 10:	bmTmp.LoadBitmap(IDB_AZT10); break;  //ako se želi izravno resource
	case 11:	bmTmp.LoadBitmap(IDB_AZT11); break;  //ako se želi izravno resource
	case 12:	bmTmp.LoadBitmap(IDB_AZT12); break;  //ako se želi izravno resource
	case 13:	bmTmp.LoadBitmap(IDB_AZT13); break;  //ako se želi izravno resource
	case 14:	bmTmp.LoadBitmap(IDB_AZT14); break;  //ako se želi izravno resource
	case 15:	bmTmp.LoadBitmap(IDB_AZT15); break;  //ako se želi izravno resource
	case 16:	bmTmp.LoadBitmap(IDB_AZT16); break;  //ako se želi izravno resource
	case 17:	bmTmp.LoadBitmap(IDB_AZT17); break;  //ako se želi izravno resource
	case 18:	bmTmp.LoadBitmap(IDB_AZT18); break;  //ako se želi izravno resource
	case 19:	bmTmp.LoadBitmap(IDB_AZT19); break;  //ako se želi izravno resource
	case 20:	bmTmp.LoadBitmap(IDB_AZT20); break;  //ako se želi izravno resource
}}
	if (iMod==3){  //kineski-elementi
	switch (iGlif) {
	case 0:	    bmTmp.LoadBitmap(IDB_KINEL01); break;  //ako se želi izravno resource
	case 1:		bmTmp.LoadBitmap(IDB_KINEL04); break;  //ako se želi izravno resource
	case 2:		bmTmp.LoadBitmap(IDB_KINEL05); break;  //ako se želi izravno resource
	case 3:		bmTmp.LoadBitmap(IDB_KINEL02); break;  //ako se želi izravno resource
	case 4:		bmTmp.LoadBitmap(IDB_KINEL03); break;  //ako se želi izravno resource
	case 5:		bmTmp.LoadBitmap(IDB_KINEL01); break;  //ako se želi izravno resource
	case 6:		bmTmp.LoadBitmap(IDB_KINEL06); break;  //jin-jang znak, nije element

}}

	if (iMod==4){ //MAJANSKI -mjesec tzolkina
	switch (iGlif) {
	case 1:	bmTmp.LoadBitmap(IDB_MAYDAY01); break;   //ako se želi izravno resource
	case 2:	bmTmp.LoadBitmap(IDB_MAYDAY02); break;  //ako se želi izravno resource
	case 3:	bmTmp.LoadBitmap(IDB_MAYDAY03); break;  //ako se želi izravno resource
	case 4:	bmTmp.LoadBitmap(IDB_MAYDAY04); break;  //ako se želi izravno resource
	case 5:	bmTmp.LoadBitmap(IDB_MAYDAY05); break ; //ako se želi izravno resource
	case 6:	bmTmp.LoadBitmap(IDB_MAYDAY06); break;  //ako se želi izravno resource
	case 7:	bmTmp.LoadBitmap(IDB_MAYDAY07); break;  //ako se želi izravno resource
	case 8:	bmTmp.LoadBitmap(IDB_MAYDAY08); break;  //ako se želi izravno resource
	case 9:	bmTmp.LoadBitmap(IDB_MAYDAY09); break;  //ako se želi izravno resource
	case 10:	bmTmp.LoadBitmap(IDB_MAYDAY10); break;  //ako se želi izravno resource
	case 11:	bmTmp.LoadBitmap(IDB_MAYDAY11); break;  //ako se želi izravno resource
	case 12:	bmTmp.LoadBitmap(IDB_MAYDAY12); break;  //ako se želi izravno resource
	case 13:	bmTmp.LoadBitmap(IDB_MAYDAY13); break;  //ako se želi izravno resource
	case 14:	bmTmp.LoadBitmap(IDB_MAYDAY14); break;  //ako se želi izravno resource
	case 15:	bmTmp.LoadBitmap(IDB_MAYDAY15); break;  //ako se želi izravno resource
	case 16:	bmTmp.LoadBitmap(IDB_MAYDAY16); break;  //ako se želi izravno resource
	case 17:	bmTmp.LoadBitmap(IDB_MAYDAY17); break;  //ako se želi izravno resource
	case 18:	bmTmp.LoadBitmap(IDB_MAYDAY18); break;  //ako se želi izravno resource
	case 19:	bmTmp.LoadBitmap(IDB_MAYDAY19); break;  //ako se želi izravno resource
	case 20:	bmTmp.LoadBitmap(IDB_MAYDAY20); break;  //ako se želi izravno resource
}}

	if (iMod==5){ //MAJANSKI -mjesec HAABA
	switch (iGlif) {
	case 1:	bmTmp.LoadBitmap(IDB_MAYM01); break;   //ako se želi izravno resource
	case 2:	bmTmp.LoadBitmap(IDB_MAYM02); break;  //ako se želi izravno resource
	case 3:	bmTmp.LoadBitmap(IDB_MAYM03); break;  //ako se želi izravno resource
	case 4:	bmTmp.LoadBitmap(IDB_MAYM04); break;  //ako se želi izravno resource
	case 5:	bmTmp.LoadBitmap(IDB_MAYM05); break ; //ako se želi izravno resource
	case 6:	bmTmp.LoadBitmap(IDB_MAYM06); break;  //ako se želi izravno resource
	case 7:	bmTmp.LoadBitmap(IDB_MAYM07); break;  //ako se želi izravno resource
	case 8:	bmTmp.LoadBitmap(IDB_MAYM08); break;  //ako se želi izravno resource
	case 9:	bmTmp.LoadBitmap(IDB_MAYM09); break;  //ako se želi izravno resource
	case 10:	bmTmp.LoadBitmap(IDB_MAYM10); break;  //ako se želi izravno resource
	case 11:	bmTmp.LoadBitmap(IDB_MAYM11); break;  //ako se želi izravno resource
	case 12:	bmTmp.LoadBitmap(IDB_MAYM12); break;  //ako se želi izravno resource
	case 13:	bmTmp.LoadBitmap(IDB_MAYM13); break;  //ako se želi izravno resource
	case 14:	bmTmp.LoadBitmap(IDB_MAYM14); break;  //ako se želi izravno resource
	case 15:	bmTmp.LoadBitmap(IDB_MAYM15); break;  //ako se želi izravno resource
	case 16:	bmTmp.LoadBitmap(IDB_MAYM16); break;  //ako se želi izravno resource
	case 17:	bmTmp.LoadBitmap(IDB_MAYM17); break;  //ako se želi izravno resource
	case 18:	bmTmp.LoadBitmap(IDB_MAYM18); break;  //ako se želi izravno resource
	case 19:	bmTmp.LoadBitmap(IDB_MAYM19); break;  //ako se želi izravno resource
}}

	if (iMod==7){ //egip.
	switch (iGlif) {
	case 1:	bmTmp.LoadBitmap(IDB_EG_MJ_B1); break;   //ako se želi izravno resource
	case 2:	bmTmp.LoadBitmap(IDB_EG_MJ_B2); break;  //ako se želi izravno resource
	case 3:	bmTmp.LoadBitmap(IDB_EG_MJ_B3); break;  //ako se želi izravno resource
	case 4:	bmTmp.LoadBitmap(IDB_EG_MJ_B4); break;  //ako se želi izravno resource

	case 5:	bmTmp.LoadBitmap(IDB_EG_MJ1); break;   //ako se želi izravno resource
	case 6:	bmTmp.LoadBitmap(IDB_EG_MJ2); break;  //ako se želi izravno resource
	case 7:	bmTmp.LoadBitmap(IDB_EG_MJ3); break;  //ako se želi izravno resource

	case 8:	bmTmp.LoadBitmap(IDB_EG_MJ); break;  //ako se želi izravno resource

	case 9:	bmTmp.LoadBitmap(IDB_EG_SODPET); break;  //ako se želi izravno resource
}}



	 bmTmp.GetBitmap( &bm );  // jednostavnija verzija

	CDC dcMem;
    dcMem.CreateCompatibleDC(pDC);

    CBitmap* pbmOldSrc = (CBitmap*)dcMem.SelectObject(&bmTmp);
 
   	bmDest.CreateCompatibleBitmap (&dcMem, bm.bmWidth, bm.bmHeight);  // prihvatni cdc, važno je da se stavi originalni DC, a ne ciljni!!
	                                                      // (inaèe ne napravi toènu paletu nego monokromni bitmap)


    CBitmap* pbmOldDest = (CBitmap*)pDC->SelectObject(&bmDest);  //selektiraj u prihvatni cdc prihvatni prazni bitmap objekt

    pDC->BitBlt(x,y, bm.bmWidth,bm.bmHeight,&dcMem,0,0,SRCCOPY);  //resizaj i kopiraj u ciljni bmp

 

	 dcMem.SelectObject(pbmOldSrc);  //select old bitmaps
	 pDC->SelectObject(pbmOldDest);
	 
	 bmTmp.Detach();             //detach bitmapove sa slikama
	 bmDest.Detach();
	 
	 DeleteObject(bmTmp);        // delete bmpove  u dcu
	 DeleteObject(bmDest);
	 
	 ReleaseDC(&dcMem);   // oèisti memory dc-s
	 
	 
	//Invalidate();

}


//*************************************************************
void CAsc2winView::Dispozitori()
{

 

	CString sBuf6=""; 

	int vladar[20]={5,4,3,2,1,3,4,10,6,7,8,9};     //vladari (1-10) znakova (0-11)
	
	int i,j,k,l,m, ostali=0, grana=0,tr2=0, granaold=0, traziold=0;
	CString buf;

	//prvo resetiraj sva izl.polja:
	for(i=0; i<=10; i++) {
		veza[i]=0;
		broj_veza[i]=0;
		xp[i]=0;
		yp[i]=0;
		yp2[i]=0;
	}


	for(i=1; i<=10; i++) 	if ( vladar[(int)floor2(objx[100+i]/30)] != i)   veza[i]=vladar[(int)floor2(objx[100+i]/30)];

	
	int brv, minBrv=0, maxBrv=0, minj, maxj, maxx,tr, nadjeno_ista=0, nadjeno_ista2=0;

	for (i=1; i<=10; i++) {
		
		brv=0;
		for (j=1; j	<=10; j++) if (i==veza[j] )	brv++;	
		
		broj_veza[i]=brv;
		if(brv>=maxBrv) {maxBrv=brv;maxj=i;}
		if(brv<=minBrv) {minBrv=brv;minj=i;}
	}

	
	sBuf6="veze: "; for (i=1; i<=10; i++) {buf.Format("%d, ", veza[i]); sBuf6+=buf;} 	//MessageBox(sBuf6);
	sBuf6="br.veza: "; for (i=1; i<=10; i++) {buf.Format("%d, ", broj_veza[i]); sBuf6+=buf;	} 	//MessageBox(sBuf6);

	sBuf6.Format("max:%d",maxBrv);//MessageBox(sBuf6);



	int maxBrv2=maxBrv+1;

	//ispiši najviši red:
	int ispisan[20]={0,0,0,0,0,0,0,0,0,0,0};
	
	int jx[12]={0,0,0,0,0,0,0,0,0,0,0,0}, jy, trazi, nadjeno=0;



back:
	for (i=1; i	<=10; i++) {                                //prvi red, 
		jy=0;
		if (   (broj_veza[i]==maxBrv && ispisan[i]!=1) &&
			   ( (veza[i]==0 || ( veza[i]!=0 && veza[ veza[i] ] == i ))  || ostali==1)   ) {	//samo pl. bez veza ili one s povratnom vezom
		
			ispisan[i]=1;	

			if ( (jx[jy]-xp[veza[i]]==1 && ispisan[veza[i]]==1) &&
				(veza[ veza[i] ] == i && ( (broj_veza[i]< broj_veza[veza[i] ] ) || ( broj_veza[i]== broj_veza[veza[i] ]  && i>veza[i] ) )
				&& broj_veza[veza[i]]<=2)  ) jx[jy]++;  //novododano


			xp[i]=jx[jy]; yp[i]=jy;
			
			nadjeno_ista=0;

			for (k=1; k<=10; k++) {                        //drugi red
				 trazi=i; jy=1;
				 if(ispisan[k]==1) continue;
				 if (veza[k]==trazi && veza[trazi]!=k ) {
				 
				//sBuf6.Format("i=%d, k=%d, veza(i)=%d, veza(k)=%d",i,k, veza[i],veza[k] ); MessageBox(sBuf6);

				nadjeno_ista=1;
					
					ispisan[k]=1;

					xp[k]=jx[jy]; yp[k]=jy;
					jx[jy]++; jy++;
									
				 for (l=1;l<=10; l++) {    //treæi red 
					                       //jer može biti više od 1 planeta 
					trazi=k;
					jy=2;
					nadjeno_ista2=0;
				
					grana=0;

				 do {

					 do {
						 
						 
						 if (nadjeno==1)  jx[jy]=jx[jy-1]-1;

						 nadjeno=0;	
						 

							 for (j=1;j<=10; j++) {  //traži koja je
			
							//sBuf6.Format("drugi red k=%d, treci red (redni br) l=%d, trazi=%d, tekuci planet j=%d",k, l,trazi,j ); MessageBox(sBuf6);
						

  						if(ispisan[j]==1) continue;
				
							if (veza[j]==trazi) {
						
								nadjeno=1; tr=j;
								nadjeno_ista=1;

								ispisan[j]=1;	

								xp[j]=jx[jy]; yp[j]=jy;
								jx[jy]++; 
								break;
							} //if
		
						} //for j
						

						if (nadjeno==1) {trazi=tr; jy++;}

						if (nadjeno==1 /* && grana==0 */ && broj_veza[trazi]>1) { 

							if (grana>0) {
								granaold=grana; traziold = tr2;
								grana=jy; tr2=trazi; 
							}
							else{ 
								grana=jy; tr2=trazi; 
							}
						    //CString sBuf6; 
							//sBuf6.Format("trazi:%d, brv tr:%d, j:%d", trazi, broj_veza[trazi],j); MessageBox(sBuf6);
						
						
						}

						    //CString sBuf6; 
							//sBuf6.Format("nadjeno:%d, trazi: %d, tr:%d       grana:%d tr2:%d", nadjeno, trazi, tr, grana, tr2); MessageBox(sBuf6);
						
					 } while (nadjeno==1);

					 
					 if (nadjeno == 0) nadjeno_ista2++;

					 if (grana>0) {

						maxx=0; for (m=0;m<=jy; m++) if(jx[m]>maxx) maxx=jx[m];
						for (m=0;m<=11; m++) jx[m]=maxx;

						jy=grana;
						nadjeno=0;	
						trazi=tr2;
											 
					 }

				 
				 if (nadjeno_ista2 > 5) {

						jy=granaold;
						nadjeno=0;	
						trazi=traziold;
				}
					 



				 } while ( (grana>0 ||  granaold>0) && nadjeno_ista2<10);


				 } //for l


				} //if

			maxx=0; for (l=0;l<=jy; l++) if(jx[l]>maxx) maxx=jx[l];
			for (l=0;l<=11; l++) jx[l]=maxx;
			

			} //for k
			if (nadjeno_ista ==0) jx[0]++;
			maxx=0; for (l=0;l<=jy; l++) if(jx[l]>maxx) maxx=jx[l];
			for (l=0;l<=11; l++) jx[l]=maxx;


		} //if

		maxx=0; for (l=0;l<=jy; l++) if(jx[l]>maxx) maxx=jx[l];
		for (l=0;l<=11; l++) jx[l]=maxx;


	} //for i

	if (ostali==0) {maxBrv--; if (maxBrv>=0) goto back;	}


	if (ostali==0) for (i=1; i<=10; i++) if (ispisan[i]==0) ostali=1;

	if (ostali==1) {maxBrv2--; maxBrv=maxBrv2; if (maxBrv2>=0) goto back;	}
	
		

	for(i=0;i<=10;i++) yp2[i]=yp[i];

}
//***********************************************************************************************
void CAsc2winView::ArrowTo(CDC *pDC, int x1, int y1, int x2, int y2, double r, double fi)
{

	double pi=3.1415927, alfa, fir=fi*pi/180; //fi je kut strelice, r je dužina vrha strelice

	pDC->MoveTo(x1,y1); pDC->LineTo(x2,y2);  //tijelo strelice
	
	int x3,y3;


	alfa = atan2((double)(y2-y1) , (double)(x2-x1));	if (alfa<0) alfa=2*pi + alfa;

	alfa = pi + alfa; //ekvivalentan kut na kružnici sa središtem na kraju linije

	x3= x2 + r * cos (alfa+fir);
	y3= y2 + r * sin (alfa+fir);

	pDC->MoveTo(x2,y2); pDC->LineTo((int)x3,(int)y3);  //prva ruka vrha
	
	x3= x2 + r * cos (alfa-fir);
	y3= y2 + r * sin (alfa-fir);

	pDC->MoveTo(x2,y2); pDC->LineTo((int)x3,(int)y3);  // druga ruka vrha
	

}

//*****************************************************************************************************
void CAsc2winView::Izracunaj_Snage(int bd)
{

	int i,j,k,x, xx,x0, l, y,z;
	int found;
	double pkut[13];
	CString sBuf6,buf; 

	for (i=1; i<=12; i++) pkut[i]= objx[100+i];

	//prvo uèitaj defaultne vrij. snaga
	for (i=1; i<=12; i++) snaga_planeta[i]=prim_snaga_planeta[i];


	//sBuf6="korak1:prim:"; for (i=1; i<=10; i++) { buf.Format("%2.0f,", snaga_planeta[i] ); sBuf6=sBuf6+buf;}
	//MessageBox(sBuf6);


	// preth. mpra biti ranana f. dispozitori, jer koristi polja veze[11]
	//preth. mora biti popunjena i tablica aspekata akt(i)(ij)
	
	//snage planeta:

	//ovisno o poziciji:
	double jakost[11][9]={
						{5,0,11,0,1,0,7,0},{4,0,10,0,12,2,6,8},{6,3,12,9,8,0,2,0},{7,2,1,8,4,12,10,6},
						{1,8,7,2,10,0,4,0},{9,12,3,6,2,4,8,10},{10,11,4,5,7,0,1,0},{11,10,5,4,6,8,12,2},
						{9,12,3,6,11,0,5,0},{1,8,7,2,3,0,9,0} };

	for (i=1; i<=10; i++) {
		if ( floor2(pkut[i]/30)+1==jakost[i-1][0] || floor2(pkut[i]/30)+1==jakost[i-1][1] ) snaga_planeta[i]=snaga_planeta[i]*1.5;
		if ( floor2(pkut[i]/30)+1==jakost[i-1][2] || floor2(pkut[i]/30)+1==jakost[i-1][3] ) snaga_planeta[i]=snaga_planeta[i]/1.5;
		if ( floor2(pkut[i]/30)+1==jakost[i-1][3] || floor2(pkut[i]/30)+1==jakost[i-1][4] ) snaga_planeta[i]=snaga_planeta[i]*2;
		if ( floor2(pkut[i]/30)+1==jakost[i-1][5] || floor2(pkut[i]/30)+1==jakost[i-1][6] ) snaga_planeta[i]=snaga_planeta[i]/2;
	}

	//sBuf6="korak2:položaj:"; for (i=1; i<=10; i++) { buf.Format("%2.0f,", snaga_planeta[i] ); sBuf6=sBuf6+buf;}
	//MessageBox(sBuf6);

	//dispozitori:

	int postoji_subclan[11]={0,0,0,0,0,0,0,0,0,0,0};
	int postoji_sljred[11]={0,0,0,0,0,0,0,0,0,0,0};

    

	

	for (i=1; i<=10; i++) {  // za svih 10 pl.
		
		for (j=0; j<=9; j++) postoji_subclan[j]=0;
		x=xp[i]; y=yp[i]; 
		postoji_subclan[x]=i;

		y++; xx=x; x0=x;

		do {   //po dubini

			found=0; //naðena grana
		
			do {  

				
				do {    
				
					for (j=1; j<=10; j++) if (xp[j]==xx && yp[j]==y && veza[j]==postoji_subclan[x] && veza[j]!=0 ) {
				
						snaga_planeta[i]=snaga_planeta[i]+ 0.20*prim_snaga_planeta[j];
						postoji_sljred[xx]=j;
						found++; 

					
						//sBuf6.Format("i=%d, j=%d,    x=%d, xx=%d, y=%d", i,j,x, xx,y); MessageBox(sBuf6);


					}

					xx++;
				} while (xx<10);

				x++;xx=x;

			} while (x<10);
			
			//CString sBuf6; 
			//sBuf6.Format("fnd=%d, ", found); MessageBox(sBuf6);

			y++;

			//sBuf6=""; for (j=0; j<=9; j++) { buf.Format("%d,", postoji_sljred[j] ); sBuf6=sBuf6+buf;}
			//MessageBox(sBuf6);



			for (j=0; j<=9; j++) postoji_subclan[j]=postoji_sljred[j];
			for (j=0; j<=9; j++) postoji_sljred[j]=0;
			x=x0; xx=x;

		} while (found>0);

	} 

	//sBuf6="korak3:disp:"; for (i=1; i<=10; i++) { buf.Format("%2.0f,", snaga_planeta[i] ); sBuf6=sBuf6+buf;}
	//MessageBox(sBuf6);
	
//aspekti:
double asptip[8]={0, 
                  0,180,60,120,90,45};       // tipovi aspekata
double korekcija_snage[8]={0, 
                           +0.5 ,-0.5, +0.25, +0.25, -0.25, -0.15};      
	
	for (i=1; i<=10+(bd>0)*2; i++) {   //planeta koja se raèuna

		sBuf6.Format("%d :",i);

		for (z=1; z<=6; z++) {   //aspekt

			for (j=i+1; j<=10; j++) {  //skeniraj red tablice
				
				if (akt[i-1][j-1]==z) {
					snaga_planeta[i]=snaga_planeta[i]* (1+korekcija_snage[z] *prim_snaga_planeta[j]/(snaga_planeta[i]+prim_snaga_planeta[j])    );
					
					//buf.Format("%d->asp.%d, ", j,z ); sBuf6=sBuf6+buf;

				} 
			} 

			for (j=1; j<=i-1; j++) {  //skeniraj stupac tablice
				
				if (akt[j-1][i-1]==z) {
					snaga_planeta[i]=snaga_planeta[i]* (1+korekcija_snage[z] *prim_snaga_planeta[j]/(snaga_planeta[i]+prim_snaga_planeta[j]) );
				
					//buf.Format("%d->asp.%d, ",j,z ); sBuf6=sBuf6+buf;
				}
			} 
		

		} 
		//MessageBox(sBuf6);
	} 

	//sBuf6="korak4: asp:"; for (i=1; i<=12; i++) { buf.Format("%2.0f,", snaga_planeta[i] ); sBuf6=sBuf6+buf;}
	//MessageBox(sBuf6);
	


	//snage znakova
	double suma_snaga;  


	for (i=1; i<=12; i++) snaga_znakova[i]= 0;

	for (i=1; i<=12; i++) for (j=1; j<=12; j++) if ( floor2(objx[100+j]/30)+1==i) snaga_znakova[i]+=snaga_planeta[j];

	//sBuf6="korak5: znaci:nenorm:"; for (i=1; i<=12; i++) { buf.Format("%2.0f,", snaga_znakova[i] ); sBuf6=sBuf6+buf;}
	//MessageBox(sBuf6);




	//snage elemenata


	for (i=1; i<=4; i++) snaga_elemenata[i]= 0;

	for (i=1; i<=4; i++) for (j=1; j<=12; j++) if ( fmod2(j-1,4)+1==i) snaga_elemenata[i]+=snaga_znakova[j];

	//sBuf6="korak6: elementi:nenorm:"; for (i=1; i<=4; i++) { buf.Format("%2.0f,", snaga_elemenata[i] ); sBuf6=sBuf6+buf;}
	//MessageBox(sBuf6);


	//snage poèela


	for (i=1; i<=3; i++) snaga_tipa[i]= 0;

	for (i=1; i<=3; i++) for (j=1; j<=12; j++) if ( fmod2(j-1,3)+1==i) snaga_tipa[i]+=snaga_znakova[j];

	//sBuf6="korak7: pocela:nenorm:"; for (i=1; i<=3; i++) { buf.Format("%2.0f,", snaga_tipa[i] ); sBuf6=sBuf6+buf;}
	//MessageBox(sBuf6);


	//snage principa


	for (i=1; i<=2; i++) snaga_pricipa[i]= 0;

	for (i=1; i<=2; i++) for (j=1; j<=12; j++) if ( fmod2(j-1,2)+1==i) snaga_pricipa[i]+=snaga_znakova[j];

	//sBuf7="korak8: princip:nenorm:"; for (i=1; i<=2; i++) { buf.Format("%2.0f,", snaga_pricipa[i] ); sBuf6=sBuf6+buf;}
	//MessageBox(sBuf6);

	//--------------------------------
	//normaliziraj:


	//pocela
	suma_snaga=0;  
	for (i=1; i<=2; i++) suma_snaga+=snaga_pricipa[i];
	for (i=1; i<=2; i++) snaga_pricipa[i]=floor2(snaga_pricipa[i]/suma_snaga*100+0.5); //u postocima

	//sBuf8="korak9: princip:norm:"; for (i=1; i<=2; i++) { buf.Format("%2.0f,", snaga_pricipa[i] ); sBuf6=sBuf6+buf;}
	//MessageBox(sBuf6);
	

	
	//pocela
	suma_snaga=0;  
	for (i=1; i<=3; i++) suma_snaga+=snaga_tipa[i];
	for (i=1; i<=3; i++) snaga_tipa[i]=floor2(snaga_tipa[i]/suma_snaga*100+0.5); //u postocima

	//sBuf6="korak10: poèela:norm:"; for (i=1; i<=3; i++) { buf.Format("%2.0f,", snaga_tipa[i] ); sBuf6=sBuf6+buf;}
	//MessageBox(sBuf6);
	



	//elemente
	suma_snaga=0;  
	for (i=1; i<=4; i++) suma_snaga+=snaga_elemenata[i];
	for (i=1; i<=4; i++) snaga_elemenata[i]=floor2(snaga_elemenata[i]/suma_snaga*100+0.5); //u postocima

	//sBuf6="korak11: elementi:norm:"; for (i=1; i<=4; i++) { buf.Format("%2.0f,", snaga_elemenata[i] ); sBuf6=sBuf6+buf;}
	//MessageBox(sBuf6);





	//normaliziraj znakove:
	suma_snaga=0;
	for (i=1; i<=12; i++) suma_snaga+=snaga_znakova[i];
	for (i=1; i<=12; i++) snaga_znakova[i]=floor2(snaga_znakova[i]/suma_snaga*100+0.5); //u postocima

	//sBuf6="korak12: znaci:norm:"; for (i=1; i<=12; i++) { buf.Format("%2.0f,", snaga_znakova[i] ); sBuf6=sBuf6+buf;}
	//MessageBox(sBuf6);





	//normalizacija planeta:
	suma_snaga=0;

	for (i=1; i<=10; i++) suma_snaga+=snaga_planeta[i];

	for (i=1; i<=10; i++) snaga_planeta[i]=floor2(snaga_planeta[i]/suma_snaga*100+0.5); //u postocima

	//sBuf6="korak13: plan.norm :"; for (i=1; i<=10; i++) { buf.Format("%2.0f,", snaga_planeta[i] ); sBuf6=sBuf6+buf;}
	//MessageBox(sBuf6);




}


//**************************************************************************************************
CString CAsc2winView::Nadji_Egipatsku_Godinu(int iPocDan, int iPocMje, int iGod)
{
	//datumi padaju max. oko 5.-9. mjeseca pa nema podrške za veljaèu ili prelazak u novu godinu
	
	int DanMM=0, MjeMM;

	float punimj=180,temp, priblizava=0,  ip,j;

  	MjeMM=iPocMje;

       // pronaði prvi mladi mjesec iza ul.datuma:

         for (ip=iPocDan;ip<=pl[iPocMje-1]; ip++) {

                Planete(ip,iPocMje,iGod,12,0);

                temp=fabs2(objx[1]-objx[2]); if (temp>180) temp=360-temp;

                if(temp<punimj) {punimj=temp;}

                if(temp<6) {	DanMM=ip;break; }
                if(temp-punimj>0 && punimj<13) {	DanMM=ip; break; }
	}


//ako nisinašao, preði u slj.mj.

    if (DanMM==0) {

		  punimj=180;
		  priblizava=0;
		  MjeMM=iPocMje+1;

		  for (ip=1;ip<=pl[iPocMje+1 -1]; ip++) {
				j=0;
looop:
                Planete(ip,iPocMje+1,iGod,j,0);
                temp=fabs2(objx[1]-objx[2]); if (temp>180) temp=360-temp;

                if(temp<punimj ) {
					if(punimj<180) priblizava=1;
					punimj=temp;
				}

                if(temp>punimj && priblizava==1) {	DanMM=ip; break; }

				j+=4; if (j<24) goto looop;
          }
	}


	CString bf;
	bf.Format("%02d %02d ", DanMM, MjeMM);
	
	return bf;

}


//*****************************************************************************************************************
void CAsc2winView::Nacrtaj_Kalendar(CDC *pDC, int opcija)
{

	int ip,jp, ipo,jpo;
	int s;
	long int n;
	char buff[80];
	CRect pravokutnik(0,0,0,0);
	CRect * pPrav = &pravokutnik;
	CString sBuff;
	CString sMjesec[12]={"Sijecanj","Veljaca","Ozujak","Travanj","Svibanj","Lipanj","Srpanj","Kolovoz","Rujan","Listopad","Studeni","Prosinac"};
	CString sZnakMjeseca[12]={"Vodenjaka","Riba","Ovna","Bika","Blizanaca","Raka","Lava","Djevice","Vage","Škorpiona","Strijelca","Jarca"};
	int iZnakMjeseca[12]={11,12,1,2,3,4,5,6,7,8,9,10};
	CString sDanuTj[7]={'P','U','S','C','P','S','N'};
	CSize size;	

    int plold=pl[1]; 
	pl[1]=28; 
	if (fmod2(m_dUlazDlg.m_iGod,4)==0 ) 
		if ( fmod2(m_dUlazDlg.m_iGod,100)!= 0 || fmod2(m_dUlazDlg.m_iGod,400)==0 || m_dUlazDlg.m_iGod<1600) pl[1]=29;

	//resetiraj popup stringove posebnih dana:
	saImenaOstalo.RemoveAll( );
	br_ostalo=0;


//	pDC->SetMapMode (MM_TEXT);
	pDC->SetMapMode (MM_ISOTROPIC);

	CRect rect;
	GetClientRect(rect);

if (pDC->IsPrinting() ==FALSE) {

	pDC->SetWindowExt( 600* m_dUlazDlg.m_iHorRez,600);      // logièke koordinate
	pDC->SetViewportExt(rect.Width(),rect.Width()/m_dUlazDlg.m_iHorRez);  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike
	pDC->SetViewportOrg (40*m_dUlazDlg.m_iHorRez,50 );  // pomakni poèetak k.s-a na dolje
}
else {

	pDC->SetWindowExt( 600* m_dUlazDlg.m_iHorRez,600);       // logièke koordinate
	pDC->SetViewportExt(4800, 3600);  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike
    pDC->SetViewportOrg (060,250);        // pomakni poèetak k.s-a na dolje
}





// za restore pozadine popupa nakon hide popupa
	static boolean bInit2 = TRUE;   // samo jedamput inicijalizira

   if( bInit2) dcPozadina2.CreateCompatibleDC(pDC);
 //dcPozadina.SetMapMode(pDC->GetMapMode());  //sjebala bi 0 kordinate
	//if (bInit2) bmCap2.CreateCompatibleBitmap (pDC, 300,50);  // ako još nije kreiran
	//if (bInit2) bmOld2 =  dcPozadina2.SelectObject(&bmCap2);

  bInit2 = FALSE;





CPen olovka1(PS_SOLID, 1, Colors[15]);                 //svj.siva
CPen olovka2(PS_SOLID, 1, Colors[6]);                  //žuta
CPen olovka3(PS_SOLID, 2, Colors[14]);                 //tamno siva
CPen olovka4(PS_SOLID, 2, Colors[0]);                  //crna
CPen olovka5(PS_SOLID, 1, Colors[14]);                 //svj.siva
CPen* staraOlovka = pDC->SelectObject(&olovka1);           // pohrani staru olovku

LOGFONT logFont;        // setup fonta
logFont.lfHeight = 8;
logFont.lfWidth = 0;
logFont.lfEscapement = 0;
logFont.lfOrientation = 0;
logFont.lfWeight = FW_BOLD;
logFont.lfItalic = 0;
logFont.lfUnderline = 0;
logFont.lfStrikeOut = 0;
logFont.lfCharSet = ANSI_CHARSET;
logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
logFont.lfQuality = PROOF_QUALITY;
logFont.lfPitchAndFamily = FIXED_PITCH | FF_ROMAN;
strcpy(logFont.lfFaceName, "Arial");

if (pDC->IsPrinting() ==FALSE) logFont.lfHeight = 12;  // Set the new font’s height.
else logFont.lfHeight = 11;


CFont font;
font.CreateFontIndirect(&logFont);            // Create a new font and select it into the DC.
CFont* oldFont = pDC->SelectObject(&font);

	pDC->SetTextColor(Colors[7]);
	pDC->SetBkMode(TRANSPARENT);


	CBrush* brush1 = new CBrush(RGB( 0xa9, 0xa9, 0xa9) ); //tamna
	CBrush* brush2 = new CBrush(Colors[15] );             //polutamna
	CBrush* brush3 = new CBrush(RGB( 0xF5, 0xF5, 0xf5) ); //svjetla
	CBrush* brush4 = new CBrush(RGB( 0xFF, 0x69, 0xB4) ); //nedjelja
	CBrush* brush5 = new CBrush(RGB( 0xff, 0xf4, 0xf1) ); //svjetli nedjelja

	//CBrush* brush6 = new CBrush(RGB( 0xFF, 0x14, 0xb8) ); //praznik
	CBrush* brush6 = new CBrush(RGB( 0xFF, 0x3f, 0x7f) ); //praznik

	CBrush* brush7 = new CBrush(Colors[6]);			      //puni mj.
	CBrush* brush8 = new CBrush(Colors[0]);			  //mladi mj.
	CBrush* brush9 = new CBrush(Colors[2]);				  //zeleni marker.
	CBrush* brush10= new CBrush(RGB( 0xf0, 0xf8, 0xf0)); // marker mjeseca

	CBrush* oldBrush = pDC->SelectObject(brush7);// Select the new brush into the DC.



	CDC MemDC;
	MemDC.CreateCompatibleDC( pDC );
	BITMAP bm;

	CBitmap *pOldBitmap;


	pPrav->left=    fmod2(m_dUlazDlg.m_iMje-1,4)*200-10;	
	pPrav->right =  fmod2(m_dUlazDlg.m_iMje-1,4)*200+ 7*25+10;
	
	pPrav->top =    floor2((m_dUlazDlg.m_iMje-1)/4)*144 + (0)*18-10;
	pPrav->bottom = floor2((m_dUlazDlg.m_iMje-1)/4)*144 + (6+1)*18+10;

	pDC->FillRect(pPrav, brush10);


	int mj, tj,dn, nedjelja,jx;

	for (mj=0; mj<=11; mj++) {

		
		//ispis oznake punog mjeseca:
		float punimj=180,temppm,tmppm1,ipumj=1;
		int jsat,danPM=0, danPM2=0, priblizava;
        punimj=180;
	    priblizava=0;

		for (jx=1;jx<=pl[mj]; jx++) {   //korigirana rutina

			jsat=0;
looopPM:
            Planete(jx,mj+1,m_dUlazDlg.m_iGod,jsat,0);
                
				tmppm1=fabs2(objx[1]-objx[2]); if (tmppm1>180) tmppm1=360-tmppm1;
				temppm=fabs2(180-tmppm1);

                if(temppm<punimj ) {
					if(punimj<180) priblizava=1;
					punimj=temppm;
				}

                if(temppm>punimj && priblizava==1) {danPM=jx;break; }

				jsat+=4; if (jsat<24) goto looopPM;
          }

		  if (danPM!=0) if (mj+1==10 && m_dUlazDlg.m_iGod ==1582 && jx>5 && jx<16 )  danPM=0;



          punimj=180;
		  priblizava=0;

         for (jx=pl[mj];jx>=1; jx--) {
		     
			    jsat=20;
looopPM2:
                Planete(jx,mj+1,m_dUlazDlg.m_iGod,jsat,0);
                
				tmppm1=fabs2(objx[1]-objx[2]); if (tmppm1>180) tmppm1=360-tmppm1;
				temppm=fabs2(180-tmppm1);

                if(temppm<punimj ) {
					if(punimj<180) priblizava=1;
					punimj=temppm;
				}

                if(temppm>punimj && priblizava==1) {danPM2=jx;break; }

				jsat-=4; if (jsat>=0) goto looopPM2;
          }


		if (danPM2!=0 ) 
			if (  ( ! ( fabs2(danPM-danPM2) >4) ) || ( m_dUlazDlg.m_iMje==10 && m_dUlazDlg.m_iGod ==1582 && jx>5 && jx<16) ) 
				danPM2=0;



//ispis oznake mladog mjeseca:
		  int danMM=0, danMM2=0;
          punimj=180;
		  priblizava=0;

		  for (jx=1;jx<=pl[mj]; jx++) {   //korigirana rutina
		     
			
			    jsat=0;
looopMM:
                Planete(jx,mj+1,m_dUlazDlg.m_iGod,jsat,0);
                temppm=fabs2(objx[1]-objx[2]);if (temppm>180) temppm=360-temppm;

                if(temppm<punimj ) {
					if(punimj<180) priblizava=1;
					punimj=temppm;
				}

                if(temppm>punimj && priblizava==1) {danMM=jx;break; }

				jsat+=4; if (jsat<24) goto looopMM;
          }


		if (danMM!=0) if (mj+1==10 && m_dUlazDlg.m_iGod ==1582 && jx>5 && jx<16) danMM=0;
			


          punimj=180;
		  priblizava=0;

         for (jx=pl[mj];jx>=1; jx--) {



				jsat=20;
looopMM2:
                Planete(jx,mj+1,m_dUlazDlg.m_iGod,jsat,0);
                temppm=fabs2(objx[1]-objx[2]); if (temppm>180) temppm=360-temppm;

                if(temppm<punimj ) {
					if(punimj<180) priblizava=1;
					punimj=temppm;
				}

                if(temppm>punimj && priblizava==1) {danMM2=jx;break; }

				jsat-=4; if (jsat>=0) goto looopMM2;
          }


		if (danMM2!=0 )
			if( (!( fabs2(danMM-danMM2)>4 ) ) || 	(mj+1==10 && m_dUlazDlg.m_iGod ==1582 && jx>5 && jx<16)  ) danMM2=0;



          //prelazak sunca u drugi znak u mjesecu
		 int danDZ=0, objxold=0;

         for (jx=1; jx<=pl[mj]; jx++) {
		     
               Planete(jx,mj+1,m_dUlazDlg.m_iGod,12,0);
                
			   if (jx==1)	{
				   objxold=objx[1];
				   continue;
			   }

                if( floor2(objx[1]/30.)!=floor2(objxold/30.) ) {danDZ=jx;break; }

				objxold=objx[1];
          }


		if (danDZ!=0 ) 
			if (  mj+1==10 && m_dUlazDlg.m_iGod ==1582 && jx>5 && jx<16 ) 	danDZ=0;



			pPrav->left  =  fmod2(mj,4)*200;	
			pPrav->right =  fmod2(mj,4)*200+ 6*25+22;
			
			pPrav->top = floor2(mj/4)*144 ;
			pPrav->bottom = floor2(mj/4)*144 + 15;

			pDC->FillRect(pPrav, brush1);
			pDC->SetTextColor(Colors[15]);


			pDC->SetTextColor(Colors[7]);
			size=pDC->GetTextExtent(sMjesec[mj], sMjesec[mj].GetLength() );
			pDC->TextOut(pPrav->left+(172-size.cx)/2,pPrav->top+2, sMjesec[mj] );
			

			pDC->SetTextColor(Colors[14]);

		s=1;
		if (m_dUlazDlg.m_iGod > 1582 || (m_dUlazDlg.m_iGod == 1582 && mj+1>=10) ){

			if (m_dUlazDlg.m_iGod<1985) s=-1;
			if (m_dUlazDlg.m_iGod==1985 && mj+1<12 ) s=-1;
			if(s<0)   n=Razlika_Datuma(1,mj+1,m_dUlazDlg.m_iGod,12,1985)+0;
			else      n=Razlika_Datuma(1,12,1985,mj+1,m_dUlazDlg.m_iGod)+1; //prvi je datum NEDJELJE
		}
		else {   //gregorijanski preskok s 4 na 15.10.1582, ali s èetvrtka na petak!

			s=-1; 
			n=Razlika_Datuma(1,mj+1,m_dUlazDlg.m_iGod,9,1582)+30-1;   //30.9.1582 je navodno bila nedjelja
		}


		if (s>0) {
			nedjelja=fmod2(n,7); 
			nedjelja=fmod2(nedjelja+7-2,7);  //korekcija da je ponedjeljak=0, za potrebe ove funkcije
		}
		else   {
			nedjelja=6-fmod2(n,7);
		}

		dan_tj[mj]=nedjelja;


		for (tj=0; tj<=5; tj++)  {
			for (dn=0; dn<=6; dn++)  {


			//provjera praznika
			boolean bPraznik=FALSE;
		
			//if(dn<5)
			for (jx=2; jx<=2+2+2+br_praznika-1; jx++) {

				if(1+tj*7+dn- nedjelja == dan_praznik[jx-1] 
				   && mj+1==			  mje_praznik[jx-1]
				   && jx!=4 && jx!=5 && jx!=6 && jx!=7) {

				
					bPraznik=TRUE;
					break;

				}
			}


			pPrav->left=  fmod2(mj,4)*200+ dn*25;	
			pPrav->right =fmod2(mj,4)*200+ dn*25+22;
			
			pPrav->top = floor2(mj/4)*144    + (tj+1)*18;
			pPrav->bottom = floor2(mj/4)*144 + (tj+1)*18+15;

			
			//ako je tekuæi dan, nacrtaj zeleni markerni rect
			if ( 1+tj*7+dn- nedjelja == m_dUlazDlg.m_iDan && mj+1==m_dUlazDlg.m_iMje) {

				CRect pPrav2(pPrav);
				pPrav2.left   -=3;	
				pPrav2.right  +=3;
				pPrav2.top    -=3;
				pPrav2.bottom +=3;
				pDC->FillRect(pPrav2, brush9);
			}

			if (  ( tj*7+dn >= nedjelja ) && ( tj*7+dn < nedjelja+pl[mj]  )   )	{

				if(bPraznik==TRUE)				pDC->FillRect(pPrav, brush6);
				else if (dn>=5)					pDC->FillRect(pPrav, brush4);
				else							pDC->FillRect(pPrav, brush2);

				
				pDC->SetTextColor(Colors[14]);
				
				for (jx=1; jx<=saImenaPraznika.GetSize(); jx++) {

					if (jx>1 && jx!=4 && jx!=5 && jx!=6 && jx!=7 && jx<=3+2+br_praznika) continue;  //ako nisu praznici povuci crtu

					if(1+tj*7+dn- nedjelja == dan_praznik[jx-1] 
						&& mj+1== mje_praznik[jx-1]) {
	
						pDC->SelectObject(&olovka3); 
						
						sBuff.Format("%2d",(int)(1+tj*7+dn- nedjelja )  );

						size=pDC->GetTextExtent(sBuff, sBuff.GetLength() );

 						pDC->MoveTo(pPrav->left+2,pPrav->top+11); pDC->LineTo(pPrav->left+size.cx+1,pPrav->top+11);
						break;

					}
				}
			
					
				sBuff.Format("%2d",(int)(1+tj*7+dn- nedjelja )  );
				pDC->TextOut(pPrav->left+2,pPrav->top, sBuff);
	
			}
			else   //nema tog dana u mjesecu:
				if (m_dPostavke.m_bGregKal!=TRUE) {
					if (dn>=5 ) pDC->FillRect(pPrav, brush5);
					else		pDC->FillRect(pPrav, brush3);
				}

			//iscrtaj puni mjesec:
				if ( (1+tj*7+dn- nedjelja == danPM && danPM>0) || (1+tj*7+dn- nedjelja == danPM2 && danPM2>0) ){
					pDC->SelectObject(brush7);
					pDC->SelectObject(&olovka2);  
					pDC->Ellipse(pPrav->left+15,pPrav->top+1,
								 pPrav->left+21,pPrav->top+7);

					saImenaOstalo.Add("puni mjesec");
					dan_ostalo[br_ostalo]= 1+tj*7+dn- nedjelja;
					mje_ostalo[br_ostalo++]= mj+1;

				}
			//iscrtaj mladi mjesec:
				if ( (1+tj*7+dn- nedjelja == danMM && danMM>0) || (1+tj*7+dn- nedjelja == danMM2 && danMM2>0) ){
					pDC->SelectObject(brush8);
					pDC->SelectObject(&olovka4);  
					pDC->Ellipse(pPrav->left+15,pPrav->top+1,
								 pPrav->left+21,pPrav->top+7);

					saImenaOstalo.Add("mladi mjesec");
					dan_ostalo[br_ostalo]= 1+tj*7+dn- nedjelja;
					mje_ostalo[br_ostalo++]= mj+1;
				
				
				
				}


			//iscrtaj prelazak u drugi znak:
			if ( 1+tj*7+dn- nedjelja == danDZ && danDZ>0) {
				
				pOldBitmap = MemDC.SelectObject( &Slicka[12+ iZnakMjeseca[mj]-1 ] );
				Slicka[12+ iZnakMjeseca[mj]-1].GetObject( sizeof( BITMAP ), &bm );


				CRect pPrav3(pPrav);
				pPrav3.left   += 11;	
				pPrav3.right  = pPrav3.left+10;
				pPrav3.top    += 7;
				pPrav3.bottom = pPrav3.top+7;


				if(bPraznik==TRUE)				pDC->FillRect(pPrav3, brush6);
				else if (dn>=5)					pDC->FillRect(pPrav3, brush4);
				else							pDC->FillRect(pPrav3, brush2);

	
 				//pDC->BitBlt( pPrav->left+14, pPrav->top+10, bm.bmWidth, bm.bmHeight, &MemDC,0, 0, SRCAND          );
				pDC->StretchBlt( pPrav->left+12, pPrav->top+8, 9,6,    
					             &MemDC, 0,0, bm.bmWidth, bm.bmHeight, SRCAND   );
				MemDC.SelectObject( pOldBitmap );

				pDC->SetTextColor(Colors[0]);
				sBuff.Format("%2d",(int)(1+tj*7+dn- nedjelja )  );
				pDC->TextOut(pPrav->left+1,pPrav->top, sBuff);


				CString ss01;
				ss01="vladavina " + sZnakMjeseca[mj];

				if(danPM==danDZ || danMM==danDZ ||  danPM2==danDZ || danMM2==danDZ ){

					ss01= ss01 + ", " + saImenaOstalo.GetAt(br_ostalo-1);
					saImenaOstalo.SetAt(br_ostalo-1, ss01);
				}
				else{
				saImenaOstalo.Add(ss01);
				dan_ostalo[br_ostalo]= 1+tj*7+dn- nedjelja;
				mje_ostalo[br_ostalo++]= mj+1;
				}







			}


			}
	
		}
	}

	//ispis zaglavlja s nazivima dana u tj.

	for (mj=0; mj<=3; mj++)  {
		for (dn=0; dn<=6; dn++)  {

			pPrav->left=  fmod2(mj,4)*200+ dn*25;	
			pPrav->right =fmod2(mj,4)*200+ dn*25+22;
			
			pPrav->top = 0-27   ;
			pPrav->bottom = 0-27+13 ;

			if (dn>=5 ) pDC->FillRect(pPrav, brush5);
			else		pDC->FillRect(pPrav, brush3);

			pDC->SetTextColor(Colors[15]);
			pDC->TextOut(pPrav->left+6,pPrav->top+0, sDanuTj[dn] );


		}
	}
	


//vrati sve i oèisti:
pDC->SelectObject(oldBrush);
delete brush1;
delete brush2;
delete brush3;
delete brush4;
delete brush5;
delete brush6;
delete brush7;
delete brush8;
delete brush9;
delete brush10;

pDC->SelectObject(oldFont);      // vrati stari font
font.DeleteObject();

pDC->SelectObject(staraOlovka);  // vrati staru olovku

DeleteDC(MemDC);


pl[1]=plold;

}




//*************************************************************************************************
void CAsc2winView::OnGregor() 
{
	// TODO: Add your command handler code here
	//Ucitaj_Postavke_Horoskopa(FALSE);  //ako nije iz baze uzmi defaultne postavke
	//Iskljuci_sliku();

   ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.SetCurSel(iStvarniIndexSet[11]);   //Handle na CMainFrame objekt!!!!!!

	Invalidate();

}

void CAsc2winView::OnPla() 
{
	// TODO: Add your command handler code here
	//Ucitaj_Postavke_Horoskopa(FALSE);  //ako nije iz baze uzmi defaultne postavke
	//Iskljuci_sliku();

   ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.SetCurSel(iStvarniIndexSet[0]);   //Handle na CMainFrame objekt!!!!!!

	Invalidate();

	
}

void CAsc2winView::OnHor() 
{
	// TODO: Add your command handler code here
	//Ucitaj_Postavke_Horoskopa(FALSE);  //ako nije iz baze uzmi defaultne postavke
	//Iskljuci_sliku();

   ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.SetCurSel(iStvarniIndexSet[4]);   //Handle na CMainFrame objekt!!!!!!

	Invalidate();

	
}

void CAsc2winView::OnTumac() 
{
	// TODO: Add your command handler code here
	//Ucitaj_Postavke_Horoskopa(FALSE);  //ako nije iz baze uzmi defaultne postavke
	//Iskljuci_sliku();

   ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.SetCurSel(iStvarniIndexSet[6]);   //Handle na CMainFrame objekt!!!!!!

	Invalidate();

	
}



//***************************************************************************************************************

void CAsc2winView::Iscrtaj_Planetarij(CDC *pDC, int dan, int mje, int god, int sat, int min, int gmt)
{


double be,ls,lm,pm,km,am,ms,m1,m2,m3,bm;
double Gvr,k=0,ta,e,rsir,j,x,y,ra;

double duz, sir;
char ime[40];
int i,vri;
float as=0;
CString buff2;


duz = m_dUlazDlg.m_fDuz;
sir = m_dUlazDlg.m_fSir;
vri = m_dUlazDlg.m_iVri;


//	pDC->SetMapMode (MM_TEXT);
	pDC->SetMapMode (MM_ISOTROPIC);

	CRect rect;
	GetClientRect(rect);


if (pDC->IsPrinting() ==FALSE) {

	pDC->SetWindowExt( 600* m_dUlazDlg.m_iHorRez,600);   // logièke koordinate


	//pDC->SetViewportExt(700, 525);  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike

	pDC->SetViewportExt(rect.Height()*.9*m_dUlazDlg.m_iHorRez,rect.Height()*.9); // bilo 700/525

	
	pDC->SetViewportOrg (80,60);  

}
else {

	pDC->SetWindowExt( 600* m_dUlazDlg.m_iHorRez,600);       // logièke koordinate
	
	pDC->SetViewportExt(4000, 3000);  // fizièke koordinate -> veæe od logièkih -> uveæanje logièke slike

    pDC->SetViewportOrg (400,400);        // pomakni poèetak k.s-a na dolje
}


	CPen olovka0(PS_SOLID, 1, Colors[14]);                 //siva
	CPen olovka1(PS_SOLID, 1, Colors[7]);                 //bijela
	CPen olovka2(PS_SOLID, 1, Colors[11]);                 //crvena
	CPen olovka3(PS_SOLID, 2, Colors[11]);                 //debela crvena
	CPen olovka5(PS_SOLID, 1, Colors[16]);                 //naranðasta
	CPen* staraOlovka = pDC->SelectObject(&olovka0);           // pohrani staru olovku

   CBrush* brush;
	brush = new CBrush(Colors[0]); //tip, boja
	CBrush* oldBrush = pDC->SelectObject(brush);// Select the new brush into the DC.


	
	if (pDC->IsPrinting() ==FALSE)  pDC->FloodFill(2,2,Colors[0]); 


CRect wn;
this->GetWindowRect(wn);
m_pdSunSustav->SetWindowPos(&CWnd::wndNoTopMost    , wn.right-150, wn.top-29, 150, wn.Height()+29, SWP_FRAMECHANGED    );



  CFont font1;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 17;                      // request a 12-pixel-height font
	strcpy(lf.lfFaceName, "Times New Roman");        // request a face name "Arial"
	VERIFY(font1.CreateFontIndirect(&lf));  // create the font

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(Colors[14]);

	CFont* def_font = pDC->SelectObject(&font1);

	

/* 
// font ni netreba zasad
	CFont font2;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 10;                      // request a 12-pixel-height font
	strcpy(lf.lfFaceName, "Times New Roman");   // request a face name "Arial"
	VERIFY(font2.CreateFontIndirect(&lf));  // create the font
	pDC->SelectObject(&font2);
*/


 BITMAP bm2;
 CBitmap *pOldBitmap;

 CDC MemDC;
 MemDC.CreateCompatibleDC( pDC );   // pomoæni panel za crtanje slièica
 
   pDC->SelectObject(&olovka2);           // pohrani staru olovku

	ArrowTo(pDC, 850, 300, 738, 300, 10,20);

	pDC->TextOut(770, 281, "0 ");
	
	pDC->TextOut(748, 304, "(proljetna toèka)");

	pDC->SelectObject(&olovka0);           // pohrani staru olovku
	pDC->Ellipse(782-2, 285-2, 782+2, 285+2);

	pOldBitmap=MemDC.SelectObject( &Slicka[12+0] );
	Slicka[12].GetObject( sizeof( BITMAP ), &bm2 );

	if (pDC->IsPrinting() ==FALSE) {
		pDC->BitBlt(790,  285 , //znak[i],
				bm2.bmWidth, bm2.bmHeight, 
				&MemDC, 0, 0,  MERGEPAINT         );
	}
	else {
		pDC->BitBlt(790,  285 , //znak[i],
				bm2.bmWidth, bm2.bmHeight, 
				&MemDC, 0, 0,  SRCAND    );

	}
    
	pDC->SelectObject(&olovka2);           // pohrani staru olovku

	Ellipse(pDC, 400,300, 405,405, 270+50, 270+40);

	ArrowTo(pDC, 400+405*cos(49*pi/180.), 300-405*sin(49*pi/180.),
				 400+405*cos(50*pi/180.), 300-405*sin(50*pi/180.),
				 10,25		);

	pDC->TextOut(400+420*cos(45*pi/180.), 300-420*sin(45*pi/180.), "smjer kretanja planeta");




	 CBitmap bmAspSlabi, bmAspJaca;
 	if (pDC->IsPrinting() ==FALSE) {

		 bmAspSlabi.LoadBitmap(IDB_SLABI2);
		 bmAspJaca.LoadBitmap(IDB_JACA2);
	}
	else{
		 bmAspSlabi.LoadBitmap(IDB_SLABI);
		 bmAspJaca.LoadBitmap(IDB_JACA);
	}
int z;


Gvr=(double)(sat-vri-gmt+min/60.); //Globalno vrijeme
if(Gvr<0) { Gvr+=24;dan-=1;
			if (dan<1) {
				mje -=1; 
				if (mje<1) { mje=12; god-=1; } 
				dan=pl[mje-1];
			}
}

// br. dana do/od J2000 (jan 1.5, 2000)
double brDana, stoljece;
brDana= 367.*god-div( 7*(god+ div(mje+9,12) ), 4)+div(275*mje,9) + dan -730531.5 + Gvr/24;
stoljece = brDana/36525.;


double data[20][6]={
//L				w			e				O			i				a              
//ep			ph			e2				kn			i2				ae               nazivi iz mog prg-a. korišteni u nebo-opciji
//																		AU               jedinice			              
{ 252.25084,	77.45645,	0.20563069,		48.33167,	7.00487,		0.3870983		}, //merkur
{ 181.97973,	131.53298,	0.00677323,		76.68069,	3.39471,		0.72333199		}, //venera
{ 100.46435,	102.94719,	0.01671022,		-11.26064,	0.00005,		1.00000011		}, //zemlja
{ 355.454332,	336.04084,	0.09341233,		49.57854,	1.85061,		1.52366231		}, //mars
{ 34.40438,		14.75385,	0.04839266,		100.55615,	1.30530,		5.20336301		}, //jupiter
{ 49.94432,		92.43194,	0.05415060,		113.71504,	2.48446,		9.53707032		}, //saturn
{ 313.23218,	170.96424,	0.04716771,		74.22988,	0.76986,		19.19126393		}, //uran
{ 304.88003,	44.97135,	0.00858587,		131.72169,	1.76917,		30.06896348		}, //neptun
{ 238.92881,	224.06676,	0.24880766,		110.30347,	17.14175,		39.48168677		}  //pluton
};

double cy_korekc[20][6]={
//		L			w			e				O			i			a
//      "           "                           "		    "			AU            jedinice			              
	{ 538101628.29,	573.57,		0.00002527,		-446.3,		-23.51,		0.00000066		}, //merkur
	{ 210664136.06,	-108.80,	0.00004938,		-996.89,	-2.86,		0.00000092		}, //venera
	{ 129597740.63,	1198.28,	0.00003804,		-18228.25,	-46.94,		-0.00000005      }, //zemlja                                  }, //zemlja
	{ 68905103.78,	1560.78,	0.00011902,		-1020.19,	-25.47,		-0.00007221		}, //mars
	{ 10925078.35,  839.93,		0.0001288,		1217.17,	-4.15,		0.00060737		}, //jupiter
	{ 4401052.95,   -1948.89,	0.00036762,		-1591.05,	6.11,		-0.00301530		}, //saturn
	{ 1542547.79,	1312.56,	-0.0001915,		-1681.4,	-2.09,		0.00152025		}, //uran
	{ 786449.21,    -844.43,	0.0000251,		-151.25,	-3.64,		-0.00125196		}, //neptun
	{ 522747.9,		-132.25,	0.00006465,	    -37.33,		11.07,		-0.00076912		}  //pluton
};


double orb_L[20], orb_w[20], orb_e[20], orb_O[20], orb_i[20], orb_a[20];
double pi=3.141592654,  deg2rad=pi/180.;

double meanAnomaly, trueAnomaly, radius[20], fi[20];

double Xp,  Yp;
double Xpl[20],  Ypl[20];
double Xpl2[20], Ypl2[20], rad2[20];

//za tek. dan:
for (z=0; z<=8; z++) {
     
	orb_L[z]= piMod( deg2rad* (data[z][0] + cy_korekc[z][0] * stoljece/3600.) );

	orb_w[z]= deg2rad* ( data[z][1] + cy_korekc[z][1] * stoljece/3600.);
	orb_O[z]= deg2rad* ( data[z][3] + cy_korekc[z][3] * stoljece/3600.);
	orb_i[z]= deg2rad* ( data[z][4] + cy_korekc[z][4] * stoljece/3600.);
	
	orb_e[z]= data[z][2] + cy_korekc[z][2] * stoljece;
	orb_a[z]= data[z][5] + cy_korekc[z][5] * stoljece;
	


	meanAnomaly  = piMod( orb_L[z] -orb_w[z] );
	trueAnomaly  = findTrueAnomaly(meanAnomaly, orb_e[z], 1.0e-12 );


	radius[z] = orb_a[z]*( 1-orb_e[z]*orb_e[z] ) / (1+orb_e[z]*cos(trueAnomaly) );

	fi[z] = trueAnomaly + orb_w[z]- orb_O[z] ; 
	if (fi[z] < 0)    fi[z]    += (2*pi);
	if (fi[z] > 2*pi) fi[z] -= (2*pi);

	Xpl[z]= radius[z]*( cos(orb_O[z])*cos(fi[z]) - sin(orb_O[z])*sin(fi[z])*cos(orb_i[z]) )   ;
	
	Ypl[z]= radius[z]*( sin(orb_O[z])*cos(fi[z]) + cos(orb_O[z])*sin(fi[z])*cos(orb_i[z]) )   ;


}
//za slj.dan:
double orbL, orbw, orbe, orbO, orbi, orba;
double meanAnomaly3, trueAnomaly3, fi3, stoljece2;

stoljece2 = (brDana+1)/36525.;
for (z=0; z<=8; z++) {
     
	orbL= piMod( deg2rad* (data[z][0] + cy_korekc[z][0] * stoljece2/3600.) );

	orbw= deg2rad* ( data[z][1] + cy_korekc[z][1] * stoljece2/3600.);
	orbO= deg2rad* ( data[z][3] + cy_korekc[z][3] * stoljece2/3600.);
	orbi= deg2rad* ( data[z][4] + cy_korekc[z][4] * stoljece2/3600.);
	
	orbe= data[z][2] + cy_korekc[z][2] * stoljece2;
	orba= data[z][5] + cy_korekc[z][5] * stoljece2;

	meanAnomaly3  = piMod( orbL -orbw );
	trueAnomaly3  = findTrueAnomaly(meanAnomaly3, orbe, 1.0e-12 );


	rad2[z] = orba*( 1-orbe*orbe ) / (1+orbe*cos(trueAnomaly3) );

	fi3 = trueAnomaly3 + orbw- orbO ; 
	if (fi3 < 0)    fi3    += (2*pi);
	if (fi3 > 2*pi) fi3    -= (2*pi);

	Xpl2[z]= rad2[z]*( cos(orbO)*cos(fi3) - sin(orbO)*sin(fi3)*cos(orbi) )   ;
	
	Ypl2[z]= rad2[z]*( sin(orbO)*cos(fi3) + cos(orbO)*sin(fi3)*cos(orbi) )   ;


}
		



int mapx[20]={ 2,3,10,4,5,6,7,8,9};

if (pDC->IsPrinting() ==FALSE) pDC->SelectObject(&olovka1);
else						   pDC->SelectObject(&olovka5);

//sunce:
	delete brush;
	if (pDC->IsPrinting() ==FALSE) brush = new CBrush(Colors[6]); //tip, boja
	else						   brush = new CBrush(Colors[16]); //tip, boja
	pDC->SelectObject(brush);

	pDC->Ellipse(400-4,300-4, 400+4, 300+4);
	   
   for(z=0; z<360; z+=45) {
	   pDC->MoveTo( 400+5*cos(z*pi/180),300+5*sin(z*pi/180) ); 
       pDC->LineTo( 400+8*cos(z*pi/180),300+8*sin(z*pi/180) );
   }

if (pDC->IsPrinting() ==FALSE)	pDC->SelectObject(&olovka1);
else					     	pDC->SelectObject(&olovka0);

boolean bLog=false; if (m_pdSunSustav->m_iMj==1) bLog=true;
int bShowAll=0;		if (m_pdSunSustav->m_iSvePlanete==1) bShowAll=1;  //0 ili 1, all planets display

for (z=4+bShowAll*4; z>=0; z--) {

	if (bShowAll==1 && bLog==false && (z==0 || z==1)) continue; // preskoèi za prve dvije pl. za lin. prikaz cijelog sustava


	//prilagodi ekranu:
	if(bLog==false) {
		Xp= 400+300*Xpl[z]/(5.3+34.2*bShowAll);
		Yp= 300-300*Ypl[z]/(5.3+34.2*bShowAll);
	}
	else {
		Xp= 400+300/(22+bShowAll*16)* 23.6*log10(1.+1.2*fabs2(Xpl[z]))*sgn(Xpl[z]);
		Yp= 300-300/(22+bShowAll*16)* 23.6*log10(1.+1.2*fabs2(Ypl[z]))*sgn(Ypl[z]);
	} 

	if (mapx[z] != 10){

		int divovi=0; if (z==4 || z==5 || z==6 || z==7 ) divovi=2;

		MemDC.SelectObject( &Slicka[46+mapx[z]] );
		Slicka[46+mapx[z]].GetObject( sizeof( BITMAP ), &bm2 );

		if (pDC->IsPrinting() ==FALSE) {
			pDC->BitBlt(Xp-5,  Yp+(5+divovi)*sgn(Yp-300) +5*(1-sgn(Yp-300))*sgn(Yp-300), //znak[i],
				bm2.bmWidth, bm2.bmHeight, 
				&MemDC, 0, 0,  MERGEPAINT         );
		}
		else {
			pDC->BitBlt(Xp-5,  Yp+(5+divovi)*sgn(Yp-300) +5*(1-sgn(Yp-300))*sgn(Yp-300), //znak[i],
				bm2.bmWidth, bm2.bmHeight, 
				&MemDC, 0, 0,  SRCAND      );

		}

		delete brush;
		if (z==0) brush = new CBrush(Colors[15]); //tip, boja
		if (z==1) brush = new CBrush(Colors[5]); //tip, boja
		if (z==3) brush = new CBrush(Colors[4]); //tip, boja
		if (z==4) brush = new CBrush(Colors[16]); //tip, boja
		if (z==5) brush = new CBrush(Colors[14]); //tip, boja
		if (z==6) brush = new CBrush(Colors[2]); //tip, boja
		if (z==7) brush = new CBrush(Colors[1]); //tip, boja
		if (z==8) brush = new CBrush(Colors[7]); //tip, boja
		pDC->SelectObject(brush);



		pDC->Ellipse(Xp-(3+divovi), Yp-(3+divovi), Xp+(3+divovi), Yp+(3+divovi));
	}
	else{
		delete brush;
		brush = new CBrush(Colors[10]); //tip, boja
		pDC->SelectObject(brush);

		pDC->Ellipse(Xp-3, Yp-3, Xp+3, Yp+3);

		int ypea=Yp+5*sgn(Yp-300) +5*(1-sgn(Yp-300))*sgn(Yp-300);

		Ellipse(pDC, Xp, ypea+5, 5,5,0,360);

		pDC->MoveTo(Xp, ypea); 	pDC->LineTo(Xp, ypea+10);
		pDC->MoveTo(Xp-5, ypea+5); 	pDC->LineTo(Xp+5, ypea+5);
	}

	//iscrtaj putanje
	double meanAnomaly2, trueAnomaly2, radius2, fi2, z2;
	double Xx2, Yy2;


	for (z2=orb_L[z]; z2<=orb_L[z]+2*pi; z2+=2*pi/20./(z*2+2)) {

		meanAnomaly2  = z2 -orb_w[z] ; 
		if(meanAnomaly2<0) meanAnomaly2+=2*pi;	if(meanAnomaly2>2*pi) meanAnomaly2-=2*pi;

		trueAnomaly2  = findTrueAnomaly(meanAnomaly2, orb_e[z] , 1.0e-12);
		radius2 = orb_a[z]*( 1-orb_e[z]*orb_e[z] ) / (1+orb_e[z]*cos(trueAnomaly2) );
		fi2 = trueAnomaly2 + orb_w[z]- orb_O[z] ; 
		if (fi2 < 0)    fi2    += (2*pi);
		if (fi2 > 2*pi) fi2 -= (2*pi);


		Xx2= radius2*( cos(orb_O[z])*cos(fi2) - sin(orb_O[z])*sin(fi2)*cos(orb_i[z]) )   ;
	
		Yy2= radius2*( sin(orb_O[z])*cos(fi2) + cos(orb_O[z])*sin(fi2)*cos(orb_i[z]) )   ;
		
		//prilagodi ekranu:
		if(bLog==false) {
			Xx2= 400+300.*Xx2/(5.3+34.2*bShowAll);
			Yy2= 300-300.*Yy2/(5.3+34.2*bShowAll);
		}
		else {
			Xx2= 400+300/(22+bShowAll*16)* 23.6*log10(1.+1.2*fabs2(Xx2))*sgn(Xx2);
			Yy2= 300-300/(22+bShowAll*16)* 23.6*log10(1.+1.2*fabs2(Yy2))*sgn(Yy2);
		} 

		pDC->SetPixel (Xx2, Yy2, Colors[14] );
	}

}


if (pDC->IsPrinting() ==FALSE)	pDC->SelectObject(&olovka1);
else					     	pDC->SelectObject(&olovka0);

//ispiši statistiku udaljenosti
int iz=0, iz2=0;
double ud1, ud2;
CString bff;

for (z=0; z<=5+4*bShowAll; z++) {
	if (z==3) continue;  //preskoèi zemlju, ovdje je zemlja 3 a ne 2

	MemDC.SelectObject( &Slicka[46+iz] );
	Slicka[46].GetObject( sizeof( BITMAP ), &bm2 );

	if (pDC->IsPrinting() ==FALSE) {
		pDC->BitBlt(-45,-15+iz2*20+10 , //znak[i],
			bm2.bmWidth, bm2.bmHeight, 
			&MemDC, 0, 0,  MERGEPAINT         );
	}
	else {
		pDC->BitBlt(-45,-15+iz2*20+10 , //znak[i],
			bm2.bmWidth, bm2.bmHeight, 
			&MemDC, 0, 0,  SRCAND       );

	}
	
	
	
	
	

	if (z==0) { //sunce
		ud1=radius[2];
		ud2=rad2[2];

		bff.Format("%02.4f AU", ud1);
		pDC->TextOut(-45+25, -15-5+iz2*20+10, bff);

		
	}
	else{
		ud1= sqrt( (Xpl[z-1]-Xpl[2])*(Xpl[z-1]-Xpl[2]) + (Ypl[z-1]-Ypl[2])*(Ypl[z-1]-Ypl[2]) );
		ud2= sqrt( (Xpl2[z-1]-Xpl2[2])*(Xpl2[z-1]-Xpl2[2]) + (Ypl2[z-1]-Ypl2[2])*(Ypl2[z-1]-Ypl2[2]) );

		bff.Format("%02.4f AU", ud1);
		pDC->TextOut(-45+25, -15-5+iz2*20+10, bff);
	
	}
	if (ud2-ud1<0) {
		MemDC.SelectObject(&bmAspJaca);bmAspJaca.GetObject( sizeof( BITMAP ), &bm2 );
	}
	else{
		MemDC.SelectObject(&bmAspSlabi);bmAspSlabi.GetObject( sizeof( BITMAP ), &bm2 );
	}

	pDC->TextOut(-45+29+75, -15-5+iz2*20+10,  "(    )" );

	pDC->StretchBlt(-45+29+81,-15+iz2*20+2+10, 11,7,  &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight, SRCCOPY );


	iz++;
	if (z==0) iz++;  //preskoèi ikonu mjeseca, iz je za dohvat ikone

	iz2++;           // iz2 je brojanje linije


}

	pDC->TextOut(-50, -40-5+10,  "tren. udaljenosti od Zemlje:" );
	pDC->SelectObject(olovka0);
	pDC->MoveTo(-50, -23-5+10); pDC->LineTo(-50+150, -23-5+10);





if (pDC->IsPrinting() ==FALSE)	pDC->SelectObject(&olovka1);
else					     	pDC->SelectObject(&olovka0);


//udaljenosti od sunca
	iz=0;

	for (z=0; z<=4+4*bShowAll; z++) {

	ud1=radius[z];
	ud2=rad2[z];

	if (z!=2) { //zemlja
		MemDC.SelectObject( &Slicka[46+2+iz] );
		Slicka[46].GetObject( sizeof( BITMAP ), &bm2 );

	if (pDC->IsPrinting() ==FALSE) {
		pDC->BitBlt(-45,410-15+z*20+10+(1-bShowAll)*70 , //znak[i],
			bm2.bmWidth, bm2.bmHeight, 
			&MemDC, 0, 0,  MERGEPAINT         );
	}
	else {
		pDC->BitBlt(-45,410-15+z*20+10+(1-bShowAll)*70 , //znak[i],
			bm2.bmWidth, bm2.bmHeight, 
			&MemDC, 0, 0,  SRCAND         );

	}

		iz++;
	}
	else{
		Ellipse(pDC, -44+5, 410-15+z*20+10+5+(1-bShowAll)*70, 5,5,0,360);

		pDC->MoveTo(-44+5, 410-15+z*20+10+(1-bShowAll)*70); 	pDC->LineTo(-44+5, 410-15+z*20+10+10+(1-bShowAll)*70);
		pDC->MoveTo(-44, 410-15+z*20+10+5+(1-bShowAll)*70); 	pDC->LineTo(-44+10, 410-15+z*20+10+5+(1-bShowAll)*70);

		
	}

	bff.Format("%02.4f AU", ud1);
	pDC->TextOut(-45+25, 410-15-5+z*20+10+(1-bShowAll)*70, bff);

	
	if (ud2-ud1<0) {
		MemDC.SelectObject(&bmAspJaca);bmAspJaca.GetObject( sizeof( BITMAP ), &bm2 );
	}
	else{
		MemDC.SelectObject(&bmAspSlabi);bmAspSlabi.GetObject( sizeof( BITMAP ), &bm2 );
	}

	pDC->TextOut(-45+29+75, 410-15-5+z*20+10+(1-bShowAll)*70,  "(    )" );

	pDC->StretchBlt(-45+29+81,410-15+z*20+2+10+(1-bShowAll)*70, 11,7,  &MemDC, 0, 0, bm2.bmWidth, bm2.bmHeight, SRCCOPY );


}

	pDC->TextOut(-50, 410-40-5+10+(1-bShowAll)*70,  "tren. udaljenosti od Sunca:" );
	pDC->SelectObject(olovka0);
	pDC->MoveTo(-50, 410-23-5+10+(1-bShowAll)*70); pDC->LineTo(-50+150, 410-23-5+10+(1-bShowAll)*70);

	
	pDC->SelectObject(olovka3);
	//šiber mjerilo
	if(bLog==false) {
	
						   pDC->MoveTo(735, 575-5); 					   
						   pDC->LineTo(735, 575);
						   pDC->LineTo(735+ 300.*1*(1+9*bShowAll)/(5.3+34.2*bShowAll), 575);

						   pDC->LineTo(735+ 300.*1*(1+9*bShowAll)/(5.3+34.2*bShowAll), 575-5);

		bff.Format("%d AU", 1+9*bShowAll );
		pDC->TextOut(735+300.*1.*(1+9*bShowAll)/(5.3+34.2*bShowAll)/2-18, 575-18, bff);
	} 


	//nacrtaj perigeje:
	pDC->SelectObject(olovka0);
	double rad;
	for (z=0; z<=4+4*bShowAll; z++) {

		
		trueAnomaly3  = findTrueAnomaly( 0, orb_e[z], 1.0e-12 );


		rad = orb_a[z]*( 1-orb_e[z]*orb_e[z] ) / (1+orb_e[z]*cos(trueAnomaly3) );

		fi3 = trueAnomaly3 + orb_w[z]- orb_O[z] ; 

		Xp= rad*( cos(orb_O[z])*cos(fi3) - sin(orb_O[z])*sin(fi3)*cos(orb_i[z]) )   ;
	
		Yp= rad*( sin(orb_O[z])*cos(fi3) + cos(orb_O[z])*sin(fi3)*cos(orb_i[z]) )   ;

	//prilagodi ekranu:
	if(bLog==false) {
		Xp= 400+300*Xp/(5.3+34.2*bShowAll);
		Yp= 300-300*Yp/(5.3+34.2*bShowAll);
	}
	else {
		Xp= 400+300/(22+bShowAll*16)* 23.6*log10(1.+1.2*fabs2(Xp))*sgn(Xp);
		Yp= 300-300/(22+bShowAll*16)* 23.6*log10(1.+1.2*fabs2(Yp))*sgn(Yp);
	} 

	pDC->MoveTo (Xp-2,Yp-2);pDC->LineTo (Xp+2,Yp+2);
	pDC->MoveTo (Xp+2,Yp-2);pDC->LineTo (Xp-2,Yp+2);



	if (z==4+4*bShowAll) pDC->TextOut( Xp+10-(1-sgn(Xp-400))*27, Yp-8,  "perihel" );

	}

















	//oèisti korištene ðjiðe
	MemDC.SelectObject( pOldBitmap );
	DeleteDC(MemDC);

	pDC->SelectObject(oldBrush);
	delete brush;

    pDC->SelectObject(staraOlovka);  // vrati staru olovku

	pDC->SelectObject(def_font);
	font1.DeleteObject();



}  //end planetarij

//*******************************************************************
double CAsc2winView::div(double a, double b)
{
return  floor(a/b);
}


//*******************************************************************
double CAsc2winView::mod(double a, double b)
{
return  (a - b*floor(a/b));
}

//*******************************************************************
int CAsc2winView::fmod2(double a, double b)
{
return  fmod(a,b);
}

//*******************************************************************
double CAsc2winView::fabs2(double a)
{
return  fabs(a);
}

//*******************************************************************
double CAsc2winView::floor2(double a)
{
return  floor(a);
}


//*******************************************************************
double CAsc2winView::absFloor(double x)
{

	double xr;
	if (x>=0)	xr= floor(  x );
	else		xr=-floor( fabs(x) );

	return xr;
}

//*******************************************************************
double CAsc2winView::piMod(double fi)
{
	double fia, fib, pi=3.14159265;
	
	fib= fi/(2*pi);
	fia=(2*pi)*(fib-absFloor(fib));
	if(fia<0) fia=(2*pi)+fia;

	return fia;

}

char * CAsc2winView::strupr_s2(char * string)  //standardna f. nemože raditi na static charovima tipa char*="hhh"
{
	char *s;   
	s=(char*)malloc(strlen(string)+1);
	strcpy (s, string);


        for ( int i=0; i<strlen(string) ; i++ )
        {
			if ( string[i]>='a' && string[i]<='z'  )
            {
                s[i] = char((int)(string[i]) - 32);
            }
        }
	return s;
}

//*******************************************************************
double CAsc2winView::findTrueAnomaly(double inM, double inE, double inErr)  //ulaz je meanAnomaly i eccentricity
{

	double ee, eold,vv;

	ee= inM + inE*sin(inM)*(1.0+inE*cos(inM));  //eccentric anomaly, prva aproksimacija
	
	do {
		eold=ee;        
		ee= eold - (eold-inE*sin(eold)-inM)/(1-inE*cos(eold));
	} while ( fabs(ee-eold) > inErr);                               //iteracija procjene


	vv= 2.*atan(sqrt((1+inE)/(1-inE))*tan(0.5*ee)); if (vv<0) vv+=2*pi;        //true anomaly


	return vv;

}

//*******************************************************************
void CAsc2winView::OnDoUp() 
{
	int nIndex = ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.GetCurSel();   //Handle na CMainFrame objekt!!!!!!
//								 -------------------------------------------
//								 -------------------------------------------
	if (nIndex>0) {

		nIndex--;

		( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.SetCurSel(nIndex);   //Handle na CMainFrame objekt!!!!!!

   
		Invalidate();
	}

}

void CAsc2winView::OnDoDown() 
{
	int nIndex = ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.GetCurSel();   //Handle na CMainFrame objekt!!!!!!
//								 -------------------------------------------
//								 -------------------------------------------
	if (nIndex<12) {

		nIndex++;

		( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.SetCurSel(nIndex);   //Handle na CMainFrame objekt!!!!!!

   
		Invalidate();
	}
	
}

void CAsc2winView::OnButtonNebo() 
{
   ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.SetCurSel(6);   //Handle na CMainFrame objekt!!!!!!

	Invalidate();	
}

void CAsc2winView::OnButtonLuna() 
{
   ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.SetCurSel(7);   //Handle na CMainFrame objekt!!!!!!

	Invalidate();	
}

void CAsc2winView::OnButtonSus() 
{
   ( (CMainFrame*)(AfxGetApp()->m_pMainWnd)  )->m_ctlOpcija.SetCurSel(8);   //Handle na CMainFrame objekt!!!!!!

	Invalidate();	
}
