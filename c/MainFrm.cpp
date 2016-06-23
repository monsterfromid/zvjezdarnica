// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "asc2win.h"

#include "UlazDlg1.h" //dodano
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_NAREDBA_NUMEROLOGIJA, OnNaredbaNumerologija)
	ON_COMMAND(ID_POMOC_PDF1, OnPomocPdf1)
	ON_COMMAND(ID_POMOC_PDF2, OnPomocPdf2)
	ON_COMMAND(ID_POMOC_PDF3, OnPomocPdf3)
	ON_COMMAND(ID_POMOC_PDF4, OnPomocPdf4)
	ON_COMMAND(ID_POMOC_PDF5, OnPomocPdf5)
	ON_COMMAND(ID_POMOC_PDF6, OnPomocPdf6)
	ON_COMMAND(ID_POMOC_PDF7, OnPomocPdf7)
	ON_COMMAND(ID_POMOC_PDF8, OnPomocPdf8)
	ON_COMMAND(ID_POMOC_PDF9, OnPomocPdf9)
	ON_COMMAND(ID_POMOC_PDF10, OnPomocPdf10)
	ON_COMMAND(ID_POMOC_PDF11, OnPomocPdf11)
	ON_COMMAND(ID_POMOC_PDF12, OnPomocPdf12)
	ON_COMMAND(ID_POMOC_PDF13, OnPomocPdf13)
	ON_COMMAND(ID_POMOC_PDF14, OnPomocPdf14)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
	ON_CBN_SELCHANGE(IDC_CBOPCIJA, OnSelChangeOpcija)    // dodao - novi combo box u toolbaru

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_INDICATOR_LOKACIJA,
	ID_INDICATOR_TIMESTAMP,
	ID_INDICATOR_DST,
	ID_INDICATOR_ZVR,
	ID_INDICATOR_ASP,
	ID_INDICATOR_KUCE,
	ID_INDICATOR_BAZA,
	ID_INDICATOR_IME,
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
   pNumDlg = NULL;
	m_bAutoMenuEnable =FALSE;   //dodano, bez toga ne radi disable menu itemova...


}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	this->SetWindowText( "ZvzWin,  v.3.87" );

	
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)   // dodao ime windoza
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}


// Add the combo

if (!CreateIzbornikOpcija())
{
TRACE0("Failed to create izbornik opcija\n");
return -1; // fail to create
}




	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

//	cs.cx = GetSystemMetrics(SM_CYSCREEN)*4./5.*1100./754.;        //  velièina prozora i 
	cs.cx = GetSystemMetrics(SM_CXSCREEN)*4./5;                    //  velièina prozora i 
	cs.cy = GetSystemMetrics(SM_CYSCREEN)*4./5;
	
//	cs.style &= ~WS_SIZEBOX;         // nema resizea 


	cs.style &= ~(LONG) FWS_ADDTOTITLE;  // izbaci naziv dokumenta iz main window titlebara

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


//*****************************************************************************************************************
BOOL CMainFrame::CreateIzbornikOpcija()
{


// Add the Combo
int nWidth = 235;
int nHeight = 250;

// Configure the combo place holder
m_wndToolBar.SetButtonInfo(23, IDC_CBOPCIJA, TBBS_SEPARATOR, nWidth);  // pridružuje ime IDC_CBOPCIJA separatoru
                                                                       // i sreðuje širinu separatora

// Get the colorbar height
CRect rect;
m_wndToolBar.GetItemRect(23, &rect);             //uzmi velièinu mjesta na r.br. 17
rect.bottom = rect.top + nHeight;                // i sredi visinu, širina je veæ sreðena, samo je uèitaj

// Create the combo box
m_ctlOpcija.Create( WS_CHILD | WS_VISIBLE | WS_VSCROLL |
					CBS_DROPDOWNLIST, rect, &m_wndToolBar, IDC_CBOPCIJA);   

// Fill the combo box
CString szStyle;

szStyle = "-> Gregorijanski kalendar";
m_ctlOpcija.AddString((LPCTSTR)szStyle);

szStyle = "-> Mjes.kretanje planeta";
m_ctlOpcija.AddString((LPCTSTR)szStyle);

szStyle = "-> Zodijakalni horoskop";
m_ctlOpcija.AddString((LPCTSTR)szStyle);

szStyle = "-> Tumaè zod. horoskopa";
m_ctlOpcija.AddString((LPCTSTR)szStyle);

szStyle = "-> Medicinski horoskop";
m_ctlOpcija.AddString((LPCTSTR)szStyle);

szStyle = "-> Mjes.pomak ascendenta";
m_ctlOpcija.AddString((LPCTSTR)szStyle);

szStyle = "-> Zvjezdano nebo";
m_ctlOpcija.AddString((LPCTSTR)szStyle);

szStyle = "-> Mjeseèeve mjene";
m_ctlOpcija.AddString((LPCTSTR)szStyle);

szStyle = "-> Sunèev sustav";
m_ctlOpcija.AddString((LPCTSTR)szStyle);

szStyle = "-> Kineski Horoskop";
m_ctlOpcija.AddString((LPCTSTR)szStyle);

szStyle = "-> Asteèki Horoskop";
m_ctlOpcija.AddString((LPCTSTR)szStyle);

szStyle = "-> Majanski Kalendar";
m_ctlOpcija.AddString((LPCTSTR)szStyle);

szStyle = "-> Egipatski Kalendar";
m_ctlOpcija.AddString((LPCTSTR)szStyle);

m_ctlOpcija.SetCurSel(0);   // ne može se inicijalizirati na standardnom mjestu (konstruktor) jer tamo tada još ni ne postoji !!!

return TRUE;
}

void CMainFrame::OnSelChangeOpcija()
{	
	Invalidate();
}


//********************************************************************************************************************

void CMainFrame::OnNaredbaNumerologija() 
{
	// TODO: Add your command handler code here
	
	CString buff2;

if(!pNumDlg)
{

   // nova ver. s modeless dialogom
	   pNumDlg = new CNumDlg(this);
      pNumDlg->Create(IDD_NUM);
      pNumDlg->ShowWindow(SW_SHOW);

	//int i = m_dNumDlg.DoModal ();

//	buff2.Format("domodal=%d",i);
//	if(i!=IDOK) MessageBox(buff2);

}
else
{		
    pNumDlg->SetForegroundWindow();
}


}

//********************************************************************
void CMainFrame::OnPomocPdf1() 
{
	// TODO: Add your command handler code here
	
	/*
	CString Path(*__argv);                     //ovaj kod vraæa tekuæi puni path aplikacije!
    int i=Path.ReverseFind('\\')+1;
    if(i) Path=Path.Left(i);

	MessageBox(Path);
	*/
	
	 ShellExecute(NULL, _T("open"), 
		 _T("Tracy Marks - The Art Of Chart Interpretation.pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;
}

void CMainFrame::OnPomocPdf2() 
{
	// TODO: Add your command handler code here
	 ShellExecute(NULL, _T("open"), 
		 _T("Stephen Arroyo - Chart Interpretation Handbook.pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;
	
}

void CMainFrame::OnPomocPdf3() 
{
	// TODO: Add your command handler code here
	 ShellExecute(NULL, _T("open"), 
		 _T("Bil Tierney - All Around The Zodiac.pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;
	
}

void CMainFrame::OnPomocPdf4() 
{
	// TODO: Add your command handler code here
	 ShellExecute(NULL, _T("open"), 
		 _T("Howard Sasportas - The Houses.pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;
	
}

void CMainFrame::OnPomocPdf5() 
{
	// TODO: Add your command handler code here
	 ShellExecute(NULL, _T("open"), 
		 _T("Mohan Koparkar - Lunar Nodes.pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;
	
}

void CMainFrame::OnPomocPdf6() 
{
	// TODO: Add your command handler code here
	 ShellExecute(NULL, _T("open"), 
		 _T("Stephen Arroyo - Astrology, Psychology And The Four Elements.pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;
	
}
//*********************************************************************

void CMainFrame::OnPomocOastrologiji() 
{
	// TODO: Add your command handler code here
	 ShellExecute(NULL, _T("open"), 
		 _T("ZvZwin.htm"), 
		 NULL, 
		 _T("manuali\\wink\\"),
		 SW_SHOWNORMAL) ;
	
}

void CMainFrame::OnPomocOastrologijioffline() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	 ShellExecute(NULL, _T("open"), 
		 _T("index.htm"), 
		 NULL, 
		 _T("manuali\\html\\"), 
		 SW_SHOWNORMAL) ;
	
}

void CMainFrame::OnPomocPdf7() 
{
	// TODO: Add your command handler code here
		 ShellExecute(NULL, _T("open"), 
		 _T("The Astrology Book.pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;

}


//dodano 28.12.
void CMainFrame::OnPomocPdf8() 
{
	// TODO: Add your command handler code here
		 ShellExecute(NULL, _T("open"), 
		 _T("ANCIENT ASTROLOGY.pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;

}
void CMainFrame::OnPomocPdf9() 
{
	// TODO: Add your command handler code here
		 ShellExecute(NULL, _T("open"), 
		 _T("AleksandarImsiragic-Tajne_stepena_Zodijaka.pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;

}
void CMainFrame::OnPomocPdf10() 
{
	// TODO: Add your command handler code here
		 ShellExecute(NULL, _T("open"), 
		 _T("Analiziranje_horoskopa.pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;

}
void CMainFrame::OnPomocPdf11() 
{
	// TODO: Add your command handler code here
		 ShellExecute(NULL, _T("open"), 
		 _T("Donna Cunningham - How To Read Your Astrological Chart.pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;

}
void CMainFrame::OnPomocPdf12() 
{
	// TODO: Add your command handler code here
		 ShellExecute(NULL, _T("open"), 
		 _T("Astrology And Stock Market Forecasting (Mcwhirter).pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;

}
void CMainFrame::OnPomocPdf13()
{
	// TODO: Add your command handler code here		 ShellExecute(NULL, _T("open"), 
		 ShellExecute(NULL, _T("open"), 
		 _T("Maja Loncaric - Astrologija - zvijezde govore.pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;


}

void CMainFrame::OnPomocPdf14()
{
	// TODO: Add your command handler code here		 ShellExecute(NULL, _T("open"), 
		 ShellExecute(NULL, _T("open"), 
		 _T("The Only Astrology Book You'll Ever Need.pdf"), 
		 NULL, 
		 _T("manuali\\"), 
		 SW_SHOWNORMAL) ;


}