// NumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "asc2win.h"
#include "NumDlg.h"
#include "MainFrm.h" // ja dodo jer nije prepoznavao

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "math.h"    //iz orig.prga
#include "dos.h"     //iz orig.prga

/////////////////////////////////////////////////////////////////////////////
// CNumDlg dialog


CNumDlg::CNumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNumDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNumDlg)
	m_sNumReza = _T("");
	m_sNumTxt = _T("");
	m_iNumSustav = 0;
	//}}AFX_DATA_INIT

	m_sNumReza = " ";
	m_sNumTxt = " ";

	m_pParent=pParent; //This is where I point m_parent to my main frame window

	int ii;

	FILE * fajl;

fajl=fopen("zvz_num.dat","rt"); if(fajl==NULL) {MessageBox("Nema fajla zvz_num.dat");return;}
 for(ii=1; ii<=52; ii++) fgets(broj[ii],138,fajl); 
fclose(fajl);


}


void CNumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNumDlg)
	DDX_Control(pDX, IDC_NUM_TAB, m_cNumTab);
	DDX_Control(pDX, IDC_HFRAME2, m_cHFrame2);
	DDX_Control(pDX, IDC_HFRAME, m_cHFrame);
	DDX_Control(pDX, IDC_HEADER, m_cHeader);
	DDX_Text(pDX, IDC_NUM_REZA, m_sNumReza);
	DDX_Text(pDX, IDC_NUM_TEKST, m_sNumTxt);
	DDX_Radio(pDX, IDC_RADIO1, m_iNumSustav);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNumDlg, CDialog)
	//{{AFX_MSG_MAP(CNumDlg)
	ON_BN_CLICKED(IDC_NUM_RACUNAJ, OnNumRacunaj)
	ON_EN_CHANGE(IDC_NUM_TEKST, OnChangeNumTekst)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO1, OnRadio123)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio123)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio123)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumDlg message handlers

//************************************************************************************************************
void CNumDlg::OnNumRacunaj() 
{
	// TODO: Add your control notification handler code here


int ii,jj,kk,x,pok=0;   
float d1,d2,d3;
CString buff3="",buff4, bufftmp="", oldrez;

char tablica[3][10][8]={ 
{
{0},   // chaldean, oriðiði
{0,'a','i','j','q','y'},
{0,'b','k','r'},
{0,'c','g','l','s','è','æ','š'},
{0,'d','m','t','ð'},
{0,'e','h','n','x'},
{0,'u','v','w'},
{0,'z','o','ž'},
{0,'f','p'} 
},

{
{0},  //Pythagorean 
{0,'a','j','s','š'},
{0,'b','k','t'},
{0,'c','l','u','s','è','æ','š'},
{0,'d','m','v','ð'},
{0,'e','n','w','x'},
{0,'f','o','x'},
{0,'g','p','y'},
{0,'h','q','z','ž' },
{0,'i','r'} 
},

{
{0},  //Pythagorean Hrvatski
{0,'a','f','n','v'},
{0,'b','g','q','z'}, //q=nj
{0,'c','h','o','ž'},
{0,'è','i','p'},
{0,'æ','j','r'},
{0,'d','k','s'},
{0,'x','l','š'},     //x=dž
{0,'ð','y','t' },   //y=lj
{0,'e','m','u'}
}
};

int njj[3]={8,9,9},nkk[3]={7,7,4};
CString sSustav[3]={"  (babilonski sustav)","  (pitagorejski sustav)","  (pit.sustav s hrv.znakovima)"};

buff4= zamjeni_sumnike (m_sNumTxt);
oldrez=m_sNumReza;

m_sNumTxt="";

x=0;

if (m_iNumSustav==2) {
	CString buff5=buff4;
	buff5.Replace("x","dž");
	buff5.Replace("X","Dž");
	buff5.Replace("y","lj");
	buff5.Replace("Y","Lj");
	buff5.Replace("q","Nj");
	buff5.Replace("Q","Nj");
	buff3.Format("%s = ", buff5);  
 }
else buff3.Format("%s = ", buff4);  

 for (ii=1; ii<=buff4.GetLength(); ii++) {
	 for (jj=1; jj<=njj[m_iNumSustav]; jj++) {    // redovi
		 for (kk=1; kk<=nkk[m_iNumSustav]; kk++) {  // èlanovi reda

                   if ( tolower( buff4.GetAt(pok) ) ==tablica[m_iNumSustav][jj][kk] ) {
					   x+=jj; 
					   buff3+=char(48+jj); 
				   }
          }}
    pok++;
 }

bufftmp.Format(" = %d", x);  buff3 += bufftmp;

if (x==0) {m_sNumReza = buff3 + sSustav[m_iNumSustav] + "\r\n\r\n" + oldrez;   UpdateData(FALSE); return; }


if (x>100) {
              d3=floor(x/100.);
              d2=floor( (x-d3*100)/10.);
              d1=x-d3*100-d2*10;

              x=(int)(d1+d2+d3);

bufftmp.Format(" = %d", x);  buff3 += bufftmp; 
}

if (x>52) {

              d2=floor(x/10.);
              d1=x-d2*10;
              x=(int)(d1*10+d2);

bufftmp.Format(" = %d", x);  buff3 += bufftmp; 
}

if (x>52) {
              d2=floor( x/10.);
              d1=x-d2*10;
              x=(int)(d1+d2);

bufftmp.Format(" = %d", x);  buff3 += bufftmp; 
}


if (x<53) { 
	bufftmp.Format("%s\r\n%s", sSustav[m_iNumSustav],broj[x]);  
	bufftmp.Replace("\n","\r\n");  // string ima sam /n a to nije dovoljno za novi red u edit-boxu
	buff3 += bufftmp; 
}
else buff3 +=  sSustav[m_iNumSustav] + "\r\n"; 

m_sNumReza = buff3 + "\r\n" + oldrez;
	

	UpdateData(FALSE);

 	/*
	CWnd *pNumRez = this->GetDlgItem(IDC_NUM_REZA);
	pNumRez->SetScrollPos(SB_VERT , 100 );
	*/


 return; 
	
}

void CNumDlg::OnChangeNumTekst() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);  // uèitaj novu vrij. iz edit boxa u varijablu
	
}

//*****************************************************************************************************
CString CNumDlg::zamjeni_sumnike(CString sStr)
{
CString sOutStr = sStr;

if (m_iNumSustav!=2) {
sOutStr.Replace("è","c");
sOutStr.Replace("æ","c");
sOutStr.Replace("ð","d");
sOutStr.Replace("š","s");
sOutStr.Replace("ž","z");

sOutStr.Replace("È","C");
sOutStr.Replace("Æ","C");
sOutStr.Replace("Š","S");
sOutStr.Replace("Ž","Z");
sOutStr.Replace("Ð","D");
}
else {
	sOutStr.Replace("x","-");
	sOutStr.Replace("y","-");
	sOutStr.Replace("q","-");
	sOutStr.Replace("w","-");
	sOutStr.Replace("X","-");
	sOutStr.Replace("Y","-");
	sOutStr.Replace("Q","-");
	sOutStr.Replace("W","-");

	sOutStr.Replace("dž","x");
	sOutStr.Replace("lj","y");

	sOutStr.Replace("Dž","X");
	sOutStr.Replace("Lj","Y");

	sOutStr.Replace("DŽ","X");
	sOutStr.Replace("LJ","Y");

	sOutStr.Replace("dŽ","x");
	sOutStr.Replace("lJ","y");

	sOutStr.Replace("Nj","Q");
	sOutStr.Replace("NJ","Q");
	sOutStr.Replace("nj","Q");
	sOutStr.Replace("nJ","q");

}

return sOutStr;
}


void CNumDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

   ((CMainFrame*)(m_pParent) )->pNumDlg=NULL;	// postavi na NULL pNumDlg

	 m_bmTab1.Detach();   
	 m_bmTab2.Detach();   
	 m_bmTab3.Detach();   
	 
	 DeleteObject(m_bmTab1);        // delete bmpove  u dcu
	 DeleteObject(m_bmTab2);        // delete bmpove  u dcu
	 DeleteObject(m_bmTab3);        // delete bmpove  u dcu
	 
 
    CDialog::PostNcDestroy();   // otvoren kao nonmodal na hipu, pa ga treba obrisat, valjda je to to
	 delete this;
	
}

void CNumDlg::OnOK() 
{
	// TODO: Add extra validation here
   // if(UpdateData(true))   {  // inaèe za OK 
   DestroyWindow();	           // domodal, inaèe, uvijek samo uništi prozor, ne i sam dialog (podatke)!!
	                             // zato kod novog zvanja uvijek su stari podaci !!!
   // }
	//CDialog::OnOK();
}

void CNumDlg::OnRadio123() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	switch( m_iNumSustav )
	{
	case 0:
		m_cNumTab.SetBitmap(m_bmTab1);
		break;
	case 1:
		m_cNumTab.SetBitmap(m_bmTab2);
		break;
	case 2:
		m_cNumTab.SetBitmap(m_bmTab3);
		break;
	}	
	
	//Invalidate();
}

//**********************************************************************************************************************
void CNumDlg::Ucitaj_sliku()
{
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
														"header.bmp", IMAGE_BITMAP, 0, 0,
														 LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	// Attach the loaded image to the CBitmap object.
	CBitmap bmTmp;

 
	bmTmp.Attach(hBitmap);                  
  //bmTmp.LoadBitmap(IDB_NUM);   //ako se želi izravno resource

   BITMAP bm;
	 //bmTmp.GetObject( sizeof( BITMAP ), &bm );  // get source pic dimensions
	 bmTmp.GetBitmap( &bm );  // jednostavnija verzija


   //Get the Display area available (pozadinski nevidljivi frame)
    CRect lRect;
    m_cHFrame.GetWindowRect(&lRect);
	 int dx=lRect.Width(), dy=lRect.Height();

	// resizing
	//CDC * pDC=GetDC();
	 CDC * pDC = GetDlgItem(IDC_HEADER)->GetDC(); 

	 CDC dcMem1, dcMem2;
    dcMem1.CreateCompatibleDC(pDC);
    dcMem2.CreateCompatibleDC(pDC);
   
    //select the original bitmap into compatible device context
    CBitmap* pbmOld1 = (CBitmap*)dcMem1.SelectObject(&bmTmp);
 
    //resize 
	 m_bmHeader.CreateCompatibleBitmap (&dcMem1, dx,dy);  // prihvatni cdc, važno je da se stavi originalni DC, a ne ciljni!!
	                                                      // (inaèe ne napravi toènu paletu nego monokromni bitmap)


	 int nOldStretchMode = dcMem2.SetStretchBltMode(HALFTONE);  

    CBitmap* pbmOld2 = (CBitmap*)dcMem2.SelectObject(&m_bmHeader);  //selektiraj u prihvatni cdc prihvatni prazni bitmap objekt

    dcMem2.StretchBlt(0,0, dx,dy,&dcMem1,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);  //resizaj i kopiraj u ciljni bmp

 
	//pridruži picture objektu dijaloga
	m_cHeader.SetBitmap(m_bmHeader);


   //poèisti za sobom
	 dcMem2.SetStretchBltMode(nOldStretchMode);   //vrati natrag mod

	 dcMem1.SelectObject(pbmOld1);  //select old bitmaps
	 dcMem2.SelectObject(pbmOld2);
	 
	 bmTmp.Detach();             //detach bitmapove sa slikama
	 m_bmHeader.Detach();
	 
	 DeleteObject(bmTmp);        // delete bmpove  u dcu
	 DeleteObject(m_bmHeader);
	 
	 ReleaseDC(&dcMem1);   // oèisti memory dc-s
	 ReleaseDC(&dcMem2);

	 //delete bmTmp;


	//Invalidate();


//*******************************************
	 
	 //uèitaj tablice;
	CBitmap bmT1,bmT2,bmT3;

 
   bmT1.LoadBitmap(IDB_TAB1);   //ako se želi izravno resource
   bmT2.LoadBitmap(IDB_TAB2);   //ako se želi izravno resource
   bmT3.LoadBitmap(IDB_TAB3);   //ako se želi izravno resource

	 bmT1.GetBitmap( &bm );  // jednostavnija verzija

   //Get the Display area available (pozadinski nevidljivi frame)
    m_cHFrame2.GetWindowRect(&lRect);
	 dx=lRect.Width(); dy=lRect.Height();

	// resizing

	 CDC dcMem1i, dcMem1o;
    dcMem1i.CreateCompatibleDC(pDC);
    dcMem1o.CreateCompatibleDC(pDC);
   
    //select the original bitmap into compatible device context
    CBitmap* pbmOld1i = (CBitmap*)dcMem1i.SelectObject(&bmT1);

    //resize 
	 m_bmTab1.CreateCompatibleBitmap (&dcMem1i, dx,dy);  // prihvatni cdc, važno je da se stavi originalni DC, a ne ciljni!!

	 int nOldStretchMode1 = dcMem1o.SetStretchBltMode(HALFTONE);  

    CBitmap* pbmOld1o = (CBitmap*)dcMem1o.SelectObject(&m_bmTab1);  //selektiraj u prihvatni cdc prihvatni prazni bitmap objekt
    dcMem1o.StretchBlt(0,0, dx,dy,&dcMem1i,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);  //resizaj i kopiraj u ciljni bmp


    dcMem1i.SelectObject(&bmT2);
    //resize 
	 m_bmTab2.CreateCompatibleBitmap (&dcMem1i, dx,dy);  // prihvatni cdc, važno je da se stavi originalni DC, a ne ciljni!!

	 dcMem1o.SelectObject(&m_bmTab2);  //selektiraj u prihvatni cdc prihvatni prazni bitmap objekt
    dcMem1o.StretchBlt(0,0, dx,dy,&dcMem1i,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);  //resizaj i kopiraj u ciljni bmp

    dcMem1i.SelectObject(&bmT3);
    //resize 
	 m_bmTab3.CreateCompatibleBitmap (&dcMem1i, dx,dy);  // prihvatni cdc, važno je da se stavi originalni DC, a ne ciljni!!

	 dcMem1o.SelectObject(&m_bmTab3);  //selektiraj u prihvatni cdc prihvatni prazni bitmap objekt
    dcMem1o.StretchBlt(0,0, dx,dy,&dcMem1i,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);  //resizaj i kopiraj u ciljni bmp

 
   //poèisti za sobom
	 dcMem1o.SetStretchBltMode(nOldStretchMode1);   //vrati natrag mod

	 dcMem1i.SelectObject(pbmOld1i);  //select old bitmaps
	 dcMem1o.SelectObject(pbmOld1o);
	 
	 bmT1.Detach();             //detach bitmapove sa slikama
	 bmT2.Detach();             //detach bitmapove sa slikama
	 bmT3.Detach();             //detach bitmapove sa slikama
	 
	 DeleteObject(bmT1);        // delete bmpove  u dcu
	 DeleteObject(bmT2);        // delete bmpove  u dcu
	 DeleteObject(bmT3);        // delete bmpove  u dcu
	 
	 ReleaseDC(&dcMem1i);   // oèisti memory dc-s
	 ReleaseDC(&dcMem1o);


}

BOOL CNumDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Ucitaj_sliku();

	/*
	m_bmTab1.LoadBitmap(IDB_TAB1);
	m_bmTab2.LoadBitmap(IDB_TAB2);
	m_bmTab3.LoadBitmap(IDB_TAB3);
	*/

	m_cNumTab.SetBitmap(m_bmTab1);  // odabir slike tablice num.sustava



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
