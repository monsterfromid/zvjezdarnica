// asc2win.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "asc2win.h"

#include "MainFrm.h"
#include "asc2winDoc.h"
#include "asc2winView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAsc2winApp

BEGIN_MESSAGE_MAP(CAsc2winApp, CWinApp)
	//{{AFX_MSG_MAP(CAsc2winApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAsc2winApp construction

CAsc2winApp::CAsc2winApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAsc2winApp object

CAsc2winApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAsc2winApp initialization

BOOL CAsc2winApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CAsc2winDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CAsc2winView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	//ovo sam ja dodo
	CPoint pnt;
	pnt.x = 10; //GetSystemMetrics(SM_CYSCREEN)*1./8.*1100./754./6.;                     //  velièina prozora i 
	pnt.y = GetSystemMetrics(SM_CYSCREEN)*1./5/3.;
	
	m_pMainWnd->SetWindowPos(&CWnd::wndTop,  pnt.x,pnt.y,0,0, SWP_NOSIZE );

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_bmAutor;
	CString	m_sIzmjene;
	CString	m_sVer;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Ucitaj_sliku();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_sIzmjene = _T("");
	m_sVer = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDBM_AUTOR, m_bmAutor);
	DDX_Text(pDX, IDC_LISTA_IZMJENA, m_sIzmjene);
	DDX_Text(pDX, IDC_VER, m_sVer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CAsc2winApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAsc2winApp message handlers


//DEL bool CAboutDlg::m_bVri()
//DEL {
//DEL 
//DEL }

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	Ucitaj_sliku();

	m_sVer  = "ZvZwin - PC Zvjezdarnica (Windows OS), inaèica 3.87\r\n\r\n";
	m_sVer += "(c) 2007 - 2016 " ;

	m_sIzmjene = "3.87. (29.02.2016) -> korekcija za br.dana veljaèe za pla/asc \r\n"; 
	m_sIzmjene+= "3.86. (29.04.2015) -> promjena aspektacije s 4/3 na 16/9 ili 4/3 \r\n"; 
	m_sIzmjene+= "3.85. (08.10.2014) -> debug inicijalizacije poèetnog dana\r\n"; 
	m_sIzmjene+= "3.84. (06.04.2014) -> raèunanje dana prelaska na DST po EU/YU/HR pravilu\r\n"; 
	m_sIzmjene+= "3.83. (22.03.2014) -> debug rekompajla na Wis.Studio-u 2008\r\n"; 
	m_sIzmjene+= "3.82. (15.03.2014) -> rekompajl na Wis.Studio-u 2008, dodano raèunanje fašnika\r\n"; 
	m_sIzmjene+= "3.81. (27.07.2013) -> pozicioniranje slike osobe u donji desni kut\r\n"; 
	m_sIzmjene+= "3.80. (27.05.2013) -> dodano raè.kat.pr.duhovi\r\n"; 
	m_sIzmjene+= "3.79. (28.12.2012) -> longcount fix greške raè.baktuna, dodatna pdf literatura\r\n"; 
	m_sIzmjene+= "3.78. (24.01.2010) -> dodatna pdf literatura\r\n"; 
	m_sIzmjene+= "3.77. (26.04.2009) -> prikaz orbita - printanje - prilagodba boja i formata za ispis\r\n"; 
	m_sIzmjene+= "3.76. (25.04.2009) -> dodana 3 toolbar gumba za astronomske prikaze\r\n"; 
	m_sIzmjene+= "3.75. (23.04.2009) -> prikaz orbita - udalj.od sunca, toèke perihela, te šiber mjerila za lin. prikaz\r\n"; 
	m_sIzmjene+= "3.74. (20.04.2009) -> dodani gumbi za skroliranje toolbar menija\r\n"; 
	m_sIzmjene+= "3.73. (19.04.2009) -> planetne orbite- skaliranje prikaza i korekcije ispisa, mj.udaljenosti plan.do zemlje,\r\n"; 
	m_sIzmjene+= "                                   novi poredak glavnog toolbar menija\r\n"; 
	m_sIzmjene+= "3.72. (18.04.2009) -> planetne orbite- dodan proraèun putanje i pomoæni dijalog\r\n"; 
	m_sIzmjene+= "2.71. (07.12.2008) -> planetne orbite- dodana opcija i kostur funkc.\r\n"; 
	m_sIzmjene+= "2.70. (05.10.2008) -> glavni prozor-poveæanje okvira i pozicioniranje \r\n"; 
	m_sIzmjene+= "2.69. (04.09.2008) -> greg.kal. - korekcije crtanja popupa (trostruki popup za trostruke dogaðaje) \r\n"; 
	m_sIzmjene+= "2.68. (03.09.2008) -> greg.kal. - korekcije crtanja popupa (dvostruki popup za višestruke dogaðaje) \r\n"; 
	m_sIzmjene+= "2.67. (02.09.2008) -> greg.kal. - korekcije crtanja popupa kod pomaka miša \r\n"; 
	m_sIzmjene+= "2.66. (31.08.2008) -> greg.kal. - popup s opisom znaèajnih astroloških dogaðaja kod poz.miša, korekcije\r\n"; 
	m_sIzmjene+= "2.65. (30.08.2008) -> greg.kal. - popup s imenom praznika kod poz.miša na danu praznika\r\n"; 
	m_sIzmjene+= "2.64. (27.08.2008) -> greg.kal. - oznake (podvlaka) za ostale znaèajne datume koji nisu praznici\r\n"; 
	m_sIzmjene+= "2.63. (19.08.2008) -> nebo- korekc.rendera ekrana nakon dekonst. objekt popupa,\r\n"; 
	m_sIzmjene+= "                                  greg.kal - boje ispisa ml.mjeseca\r\n";
	m_sIzmjene+= "2.62. (16.08.2008) -> refresh nakon zatvaranja dijaloga slike, dan tj. u status baru, novi flash help\r\n";
	m_sIzmjene+= "2.61. (14.08.2008) -> greg.kal. - korekcija interaktivnog miša za bliska polja\r\n";
	m_sIzmjene+= "2.60. (13.08.2008) -> pla/asc opcije -  interaktivni miš za poj.dan, 4 nove toolbar ikone,\r\n";
	m_sIzmjene+= "                                  hor - pom.dijalog - lokalni DST checkbox\r\n";
	m_sIzmjene+= "2.59. (12.08.2008) -> gregorijanski kalendar - interaktivni miš za mjesec/dan \r\n";
	m_sIzmjene+= "2.58. (09.08.2008) -> gregorijanski kalendar - opcije stila ispisa, markiranje trenutnog dana\r\n";
	m_sIzmjene+= "2.57. (08.08.2008) -> gregorijanski kalendar - ispis i logika\r\n";
	m_sIzmjene+= "2.56. (15.06.2008) -> problemi u vezi s razl. windows postavkama desktopa; slika autora inkorporirana u .exe fajl\r\n";
	m_sIzmjene+= "2.55. (08.06.2008) -> tumaè - egip.kalendar, korekcije; postavke - opcija precesija egip.sot.godine d/n\r\n";
	m_sIzmjene+= "                                  pla - korekcije izraèuna nedjelja prije 4.10.1582 i korekcije iscrtavanja za X mj.1582\r\n";
	m_sIzmjene+= "2.54. (07.06.2008) -> tumaè - egip.kalendar\r\n";
	m_sIzmjene+= "2.53. (01.06.2008) -> hor - dispozitori - korekcije raèunanja za grananja, tumaè - kor. raè.snaga planeta\r\n";
	m_sIzmjene+= "                                  unos - korekcija baga kod biranja grada\r\n";
	m_sIzmjene+= "2.52. (31.05.2008) -> hor - dispozitori - korekcije raèunanja, titl bar, tumaè - asteèki hor.-osobine dana\r\n";
	m_sIzmjene+= "                                  novi flash vodiè za poèetnike\r\n";
	m_sIzmjene+= "2.51. (25.05.2008) -> tumaè - crtanje tablice aspekata, procjene muškog/ženskog principa, \r\n";
	m_sIzmjene+= "	                  tjel - boje elemenata planeta\r\n";
	m_sIzmjene+= "2.50. (24.05.2008) -> tumaè - crtanje dispozitora i procjena snaga planeta, znakova i elemenata\r\n";
	m_sIzmjene+= "2.49. (22.05.2008) -> hor - dispozitori - manji popravci crtanja\r\n";
	m_sIzmjene+= "2.48. (18.05.2008) -> hor - dispozitori - crtanje strelice\r\n";
	m_sIzmjene+= "2.47. (17.05.2008) -> hor - algoritam za crtanje mape planetarnih dispozitora\r\n";
	m_sIzmjene+= "2.46. (11.05.2008) -> pla/asc tablica - mod. raè. uskrsa i dodano raè. tjelova\r\n";
	m_sIzmjene+= "2.45. (08.05.2008) -> majanski kalendar - ispis i glifovi\r\n";
	m_sIzmjene+= "2.44. (06.05.2008) -> majanski kalendar - raèunanje\r\n";
	m_sIzmjene+= "2.43. (04.05.2008) -> tumaè - kin.i azt.kalendar - dodani glifovi(slikice) i dodani kao posebne opcije u gl.meni\r\n";
	m_sIzmjene+= "2.42. (03.05.2008) -> tumaè - asteèki horoskop - korekcija raèunanja znakova, svjetovni i vjerski kalendar,\r\n";
	m_sIzmjene+= "                                  raè. razl.datuma->korekcija raè. prestupnih god. i druge korekcije\r\n" ;
	m_sIzmjene+= "2.41. (02.05.2008) -> tumaè - ispis: asteèki horoskop\r\n" ;
	m_sIzmjene+= "2.40. (01.05.2008) -> tumaè - ispis: tjel.osobine znakova zod., kineski horoskop, arapski horoskop\r\n" ;
	m_sIzmjene+= "                                  pla/asc tablica - dodan praznik kineska nova godina, i korekcija (poveæanje)\r\n" ;
	m_sIzmjene+= "                                  toènosti raèunanja punog/mladog mjeseca\r\n" ;
	m_sIzmjene+= "2.39. (29.04.2008) -> tumaè - ispis evropski horoskop\r\n" ;
	m_sIzmjene+= "2.38. (28.04.2008) -> tumaè - printanje više stranica\r\n" ;
	m_sIzmjene+= "2.37. (27.04.2008) -> tumaè - ispis kuæica\r\n" ;
	m_sIzmjene+= "2.36. (21.04.2008) -> tumaè - ispis plan. aspekata\r\n" ;
	m_sIzmjene+= "2.35. (20.04.2008) -> tumaè - pom.dijalog i ispis plan.po znaku, po medicinskim indikacijama  i po kuæama\r\n" ;
	m_sIzmjene+= "2.34. (18.04.2008) -> lu - korekcija baga u ispisu mjene\r\n" ;
	m_sIzmjene+= "2.33. (14.04.2008) -> tjel - korekcija ispisa pom.dijaloga u pprint-modu, pla - kor.boja iscrtavanja\r\n" ;
	m_sIzmjene+= "2.32. (13.04.2008) -> tjel - pomoæni dijalog biranja ispisa-dodane opcije prikaza aspekata planeta\r\n" ;
	m_sIzmjene+= "                                  hor - dodan pomoæni dijalog opcija crtanja\r\n" ;
	m_sIzmjene+= "2.31. (12.04.2008) -> tjel - prikaz planeta i aspekata, pomoæni dijalog biranja ispisa, \r\n" ;
	m_sIzmjene+= "                                  lu - korekcija baga kod iscrta mj.faze\r\n" ;

	m_sIzmjene+= "2.30. (05.04.2008) -> tjel - osn.slika i raèunanje\r\n" ;
	m_sIzmjene+= "2.29. (02.04.2008) -> god - korekcija baga s DST, korekcija uvodne poruke neuskl.DST-a\r\n" ;
	m_sIzmjene+= "2.28. (30.03.2008) -> god - modif. raèunanja-preko stvarnog položaja sunca\r\n" ;
	m_sIzmjene+= "2.27. (24.03.2008) -> htm on/offline i teèaj (swf) pomoæ, opcija GOD, status-Zv.vr.\r\n" ;
	m_sIzmjene+= "2.26. (22.03.2008) -> tabl.aspekata-bug-pozicija, gumb numerologije, pdf-help fajlovi, about dlg.\r\n" ;
	m_sIzmjene+= "2.25. (21.03.2008) -> print-preview, resize gl.prozora, pozicije pom.prozora, ispravak baga-prozor aspekata,\r\n" ;
	m_sIzmjene+= "                                  nebo-ispravak baga vel.zvijezda\r\n" ;
	m_sIzmjene+= "2.24. (16.03.2008) -> hor - dodana linija aspekta za asp.konjukcije, podrška za uèit. jpeg-ova\r\n" ;
	m_sIzmjene+= "2.23. (15.03.2008) -> 2 dodatne opcije poè.postavki, dijalog za slike baze(samo .bmp tip),\r\n" ;
	m_sIzmjene+= "                                  pla/asc-indikator tek.dana, hor-kozmetika\r\n" ;
	m_sIzmjene+= "2.22. (10.03.2008) -> pla/asc tablica -dodani svi imendani (stara funkcionalnost)\r\n" ;
	m_sIzmjene+= "2.21. (09.03.2008) -> dialog edit zapisa, korekcije hendlanja DST,about dialog\r\n" ;
	m_sIzmjene+= "2.20. (08.03.2008) -> postavke-funkcionalnost\r\n" ;
	m_sIzmjene+= "2.19. (24.02.2008) -> hor-èišæenje memorije, kozmetika, dijalog postavki\r\n" ;
	m_sIzmjene+= "2.18. (23.02.2008) -> tabl.aspekata, dotjerano\r\n" ;
	m_sIzmjene+= "2.17. (18.02.2008) -> tabl.aspekata -popup kostur\r\n" ;
	m_sIzmjene+= "2.16. (17.02.2008) -> hor. - bagovi i kozmetika\r\n" ;
	m_sIzmjene+= "2.15. (13.02.2008) -> hor. - aspekti\r\n" ;
	m_sIzmjene+= "2.14. (10.02.2008) -> hor. - osn.funkcionalnost\r\n" ;
	m_sIzmjene+= "2.13. (09.02.2008) -> hor.\r\n" ;
	m_sIzmjene+= "2.12. (03.02.2008) -> brisanje zapisa\r\n" ;
	m_sIzmjene+= "2.11. (02.02.2008) -> unos- spin kontrole datuma te zanimanje kombo\r\n" ;
	m_sIzmjene+= "2.10. (27.01.2008) -> upis dijalog (upis novog zapisa u bazu)\r\n" ;
	m_sIzmjene+= "2.09. (26.01.2008) -> unos dijalog (funkcionalnost, selekti)\r\n" ;
	m_sIzmjene+= "2.08. (20.01.2008) -> unos dialog (baza, klasa baze, komboi)\r\n";
	m_sIzmjene+= "2.07. (16.01.2008) -> unos-time picker, bug u nebo (sunè.vri.)\r\n";
	m_sIzmjene+= "2.06. (09.01.2008) -> num-header bmp(boja)+bmp tablica sustav\r\n";
	m_sIzmjene+= "2.05. (08.01.2008) -> num-header bmp (bez boje)\r\n";          
	m_sIzmjene+= "2.04. (04.01.2008) -> nebo-kozmetika,num-otvaranje & nove num.tablice\r\n";
	m_sIzmjene+= "2.03. (03.01.2008) -> nebo - izbaèen popup, crtanje imena zvijezda, +bag u num\r\n";
	m_sIzmjene+= "2.02. (02.01.2008) -> crno nebo\r\n";
	m_sIzmjene+= "2.01. (01.01.2008) -> num opcija\r\n";          
	m_sIzmjene+= "1.11. (30.12.2007) -> toolbar gumb Sada,kvadratni kursor za nebo\r\n";
	m_sIzmjene+= "1.10. (29.12.2007) -> nebo-zvijezde, popup imena zvijezda, ukl.asc2 bag\r\n";
	m_sIzmjene+= "1.09. (28.12.2007) -> nebo-fix sunèsus,planete\r\n";
	m_sIzmjene+= "1.08. (21.10.2007) -> ikona\r\n";
	m_sIzmjene+= "1.07. (20.10.2007) -> novi toolbar gumbi, imena praznika i bugovi u PLA i LU\r\n";
	m_sIzmjene+= "1.06. (16.10.2007) -> status bar\r\n";	          
	m_sIzmjene+= "1.05. (15.10.2007) -> opcija LU\r\n";
	m_sIzmjene+= "1.04. (14.10.2007) -> opcija PLA\r\n";
	m_sIzmjene+= "1.03. (13.10.2007) -> bagovi izraèuna asc-a i novi brush praznika\r\n";
	m_sIzmjene+= "1.02. (08.10.2007) -> nedjelje i full-new moon\r\n";
	m_sIzmjene+= "1.01. (06.10.2007) -> obojeni znakovi\r\n";


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//*********************************************************************************************
void CAboutDlg::Ucitaj_sliku()
{

	/*
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
														"autor.bmp", IMAGE_BITMAP, 0, 0,
														 LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	*/
	//ovo ne radi (16 boja)
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_AUTOR1), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);


	
	// Attach the loaded image to the CBitmap object.
/*	
	CBitmap bmTmp;
	bmTmp.Attach(hBitmap);                  

	m_bmAutor.SetBitmap(bmTmp);  
	
   //poèisti za sobom
	 bmTmp.Detach();             //detach bitmapove sa slikama
   	 DeleteObject(bmTmp);        // delete bmpove  u dcu


	//Invalidate();
*/

//s resizeom je malo kompliciranije:

	// Attach the loaded image to the CBitmap object.
	CBitmap bmTmp, bmTmp2;

	bmTmp.Attach(hBitmap);  

	
   BITMAP bm;
	 bmTmp.GetBitmap( &bm );  // jednostavnija verzija

   CRect lRect;
    GetDlgItem(IDF_AUTOR)->GetWindowRect(&lRect);
	 int dx=lRect.Width(), dy=lRect.Height();

	// resizing
	 CDC * pDC = GetDlgItem(IDBM_AUTOR)->GetDC(); 

	 CDC dcMem1, dcMem2;
    dcMem1.CreateCompatibleDC(pDC);
    dcMem2.CreateCompatibleDC(pDC);
   
    //select the original bitmap into compatible device context
    CBitmap* pbmOld1 = (CBitmap*)dcMem1.SelectObject(&bmTmp);
 
    //resize 
	 bmTmp2.CreateCompatibleBitmap (&dcMem1, dx,dy);  // prihvatni cdc, važno je da se stavi originalni DC, a ne ciljni!!
	                                                      // (inaèe ne napravi toènu paletu nego monokromni bitmap)


	 int nOldStretchMode = dcMem2.SetStretchBltMode(HALFTONE);  

	 CBitmap* pbmOld2 = (CBitmap*)dcMem2.SelectObject(&bmTmp2);  //selektiraj u prihvatni cdc prihvatni prazni bitmap objekt

    
	dcMem2.FloodFill(15,15,RGB(238, 232, 205 ));
	dcMem2.StretchBlt(0,0, (int)(dy*bm.bmWidth/bm.bmHeight),dy,&dcMem1,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);  //resizaj i kopiraj u ciljni bmp



	//pridruži picture objektu dijaloga
	m_bmAutor.SetBitmap(bmTmp2);



	   //poèisti za sobom
	 dcMem2.SetStretchBltMode(nOldStretchMode);   //vrati natrag mod

	 dcMem1.SelectObject(pbmOld1);  //select old bitmaps
	 dcMem2.SelectObject(pbmOld2);
	 
	 bmTmp.Detach();             //detach bitmapove sa slikama
	 bmTmp2.Detach();
	 
	 DeleteObject(bmTmp);        // delete bmpove  u dcu
	 DeleteObject(bmTmp2);
	 
	 ReleaseDC(&dcMem1);   // oèisti memory dc-s
	 ReleaseDC(&dcMem2);



}
