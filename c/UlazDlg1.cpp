// UlazDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "stdio.h"
#include "math.h"
#include "asc2win.h"
#include "UlazDlg1.h"
#include "afxtempl.h"       // MFC suport for Collections
#include "stdlib.h" 


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUlazDlg dialog


CUlazDlg::CUlazDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUlazDlg::IDD, pParent)
{

	m_sGrad = "ZAGREB, CROATIA                 ";   //defaultna poèetna vrijednost kod inicijalizacije klase dijaloga
			
	m_iSat = CTime::GetCurrentTime().GetHour();
	m_iMin = CTime::GetCurrentTime().GetMinute();


	m_iDan = CTime::GetCurrentTime().GetDay();
	m_iMje = CTime::GetCurrentTime().GetMonth();
	m_iGod = CTime::GetCurrentTime().GetYear();

	m_iGmTime = 1;	m_iGmt= 1;

	m_fDuz = -15.9811;
	m_fSir= 45.811;

	if (m_iMje >=4 && m_iMje <=10) m_iVri = 1;    // ljetno vrijeme
	else m_iVri = 0;
	
	m_chSpol='-';  // ovo je samo pomoæna varijabla
	m_cSpol='-';  //ovo je pravi spol od baznog zapisa
	m_chSustavKuca ='K';


	m_chkZanimanje = FALSE; m_iZanimSel=0;

	m_iListBoxCurSel =0;
	

	FILE *f1; 
	char buf[100]; 

	f1=fopen("pocetne_postavke.cfg","rt"); 
	if(f1==NULL) {MessageBox("nema fajla postavki horoskopa!"); m_sNazivBaze="BAZA.HOR"; }
	else { 
		fgets(buf,100,f1);fgets(buf,100,f1);fgets(buf,100,f1);fgets(buf,100,f1);
		fgets(buf,100,f1); fgets(buf,100,f1); 

		char nazivBaze[100];
		fgets(buf,100,f1); sscanf(buf,"%s",&nazivBaze[0]); m_sNazivBaze.Format("%s",nazivBaze);
		fclose(f1);
	}
                                             
                                               
int n=0,ndef=1;



f1=fopen("zvz_grad.dat","rt");                 // uèitaj sve gradove u polje stringova

if(f1!=NULL)  {                                // ako fajl postoji

do {                                       

fgets(buf,100,f1); if(feof(f1)) break;         // do kraja fajla
m_saGradovi.Add(buf);                          // dodaj kopiju cijele linije fajla  na kraj Arraya m_saGradovi, èlana klase

} while (TRUE);


fclose(f1);

}

Ucitaj_bazu();

   
//MessageBox("konstr.");


	//{{AFX_DATA_INIT(CUlazDlg)
	//m_iGmt = 0;
	m_sDatum = _T("");
	m_sSir = _T("");
	m_sDuz = _T("");
	m_radSpol = 2;    //-1 je bez selekta
	m_chkOb1 = FALSE;
	m_chkOb2 = FALSE;
	m_iSelDan = 0;
	m_iSelGod = 0;
	m_iSelMje = 0;
	m_chkZanimanje = FALSE;
	//}}AFX_DATA_INIT
}


//*******************************************************************************************************
void CUlazDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUlazDlg)
	DDX_Control(pDX, IDC_ZANIMANJE, m_cbZanimanje);
	DDX_Control(pDX, IDC_SPIN_MJE, m_spinMje);
	DDX_Control(pDX, IDC_SPIN_GOD, m_spinGod);
	DDX_Control(pDX, IDC_SPIN_DAN, m_spinDan);
	DDX_Control(pDX, IDC_CB_ZN2, m_cbZn2);
	DDX_Control(pDX, IDC_CB_ZN1, m_cbZn1);
	DDX_Control(pDX, IDC_CB_OB2, m_cbOb2);
	DDX_Control(pDX, IDC_CB_OB1, m_cbOb1);
	DDX_Control(pDX, IDC_LIST_BAZA, m_lbBaza);
	DDX_Control(pDX, IDC_DATUM, m_cDatum);
	DDX_Control(pDX, IDC_GRAD, m_cbGrad);
	DDX_Text(pDX, IDC_GMT, m_iGmt);
	DDX_Text(pDX, IDC_SDATUM, m_sDatum);
	DDX_Text(pDX, IDC_SIR, m_sSir);
	DDX_Text(pDX, IDC_DUZ, m_sDuz);
	DDX_DateTimeCtrl(pDX, IDC_SAT, m_tSat);
	DDX_Radio(pDX, IDC_RAD_SPM, m_radSpol);
	DDX_Check(pDX, IDC_CHK_OB1, m_chkOb1);
	DDX_Check(pDX, IDC_CHK_OB2, m_chkOb2);
	DDX_Text(pDX, IDC_SEL_DAN, m_iSelDan);
	DDX_Text(pDX, IDC_SEL_GOD, m_iSelGod);
	DDX_Text(pDX, IDC_SEL_MJE, m_iSelMje);
	DDX_Check(pDX, IDC_CHK_ZANIM, m_chkZanimanje);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUlazDlg, CDialog)
	//{{AFX_MSG_MAP(CUlazDlg)
	ON_CBN_SELCHANGE(IDC_GRAD, OnSelchangeGrad)
	ON_NOTIFY(MCN_SELECT, IDC_DATUM, OnSelectDatum)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_SAT, OnDatetimechangeSat)
	ON_BN_CLICKED(IDC_CHK_OB1, OnChkOb1)
	ON_BN_CLICKED(IDC_CHK_OB2, OnChkOb2)
	ON_BN_CLICKED(IDC_RAD_SPM, OnRadSpm)
	ON_LBN_SELCHANGE(IDC_LIST_BAZA, OnSelchangeListBaza)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_EN_CHANGE(IDC_SEL_DAN, OnChangeSelDan)
	ON_BN_CLICKED(IDC_CHK_ZANIM, OnChkZanim)
	ON_CBN_SELENDOK(IDC_ZANIMANJE, OnSelchangeZanimanje)
	ON_BN_CLICKED(IDC_BUTTON2, Obrisi_Zapis)
	ON_BN_CLICKED(IDC_RAD_SP0, OnRadSpm)
	ON_BN_CLICKED(IDC_RAD_SPZ, OnRadSpm)
	ON_CBN_SELCHANGE(IDC_CB_ZN1, OnChkOb1)
	ON_CBN_SELCHANGE(IDC_CB_OB1, OnChkOb1)
	ON_CBN_SELCHANGE(IDC_CB_OB2, OnChkOb2)
	ON_CBN_SELCHANGE(IDC_CB_ZN2, OnChkOb2)
	ON_EN_CHANGE(IDC_SEL_MJE, OnChangeSelDan)
	ON_EN_CHANGE(IDC_SEL_GOD, OnChangeSelDan)
	ON_BN_CLICKED(IDC_IZMJENI, Izmjeni_Zapis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//*******************************************************************************************************
BOOL CUlazDlg::OnInitDialog() 
{

	
	CDialog::OnInitDialog();

m_bInit = TRUE;

int i,ndef=1;
char grad[40];

	m_iSat = CTime::GetCurrentTime().GetHour();
	m_iMin = CTime::GetCurrentTime().GetMinute();
	m_iDan = CTime::GetCurrentTime().GetDay();
	m_iMje = CTime::GetCurrentTime().GetMonth();
	m_iGod = CTime::GetCurrentTime().GetYear();

CTime t(1990,1,1,m_iSat,m_iMin,0);
CDateTimeCtrl* pSat = (CDateTimeCtrl*) GetDlgItem(IDC_SAT);
pSat->SetTime(&t);


UpdateData(TRUE);


if (m_sGrad == "<iz baze>") m_sGrad = "ZAGREB, CROATIA                 ";   //defaultna poèetna vrijednost kod inicijalizacije klase dijaloga



for(i=0; i<=m_saGradovi.GetSize()-1; i++)  {     // prelistaj polje s gradovima                                    


 
strncpy(&grad[0],m_saGradovi.GetAt(i),32);     // izdvoji naziv grada 
grad[32]=0;

if ( m_sGrad.Compare(grad) ==0 ) ndef=i;       // i provjeri da li je to ujedno i tekuæe selektirani grad 


m_cbGrad.AddString(&grad[0]);                  // dodaj svaki naziv grada u combobox  popis 
                                               // ComboBox na displeju nije èlan klase, pa se kod zatvaranja dijaloga uništava
 
} 


m_cbGrad.SetCurSel(ndef);                      // postavi selekt comboboxa na pronaðeni index 


this->OnSelchangeGrad();                       // apdejtaj stringove za ispis


m_cDatum.SetCurSel(&m_dDatum);                  

	m_cDatum.GetCurSel(&m_dDatum); 
	
    m_iDan = m_dDatum.wDay;
    m_iMje = m_dDatum.wMonth;
    m_iGod = m_dDatum.wYear;
	
    //sprintf(buf, "%i.%i.%i", m_iDan, m_iMje, m_iGod );  //stari naèin
	//m_sDatum=buf;

    m_sDatum.Format("%i.%i.%i", m_iDan, m_iMje, m_iGod );  //Novi naèin! (metoda CString-a)


UpdateData(FALSE);


//spin kontrole:
int pl[12]={31,28,31,30,31,30,31,31,30,31,30,31};  

m_spinDan.SetBuddy(GetDlgItem(IDC_SEL_DAN));
m_spinMje.SetBuddy(GetDlgItem(IDC_SEL_MJE));
m_spinGod.SetBuddy(GetDlgItem(IDC_SEL_GOD));

m_spinDan.SetBase(10);
m_spinMje.SetBase(10);
m_spinGod.SetBase(10);

UDACCEL uda, uda2[3];
uda.nSec = 0;
uda.nInc = 1;

uda2[0].nSec = 0;
uda2[0].nInc = 1;
uda2[1].nSec = 2;
uda2[1].nInc = 2;
uda2[2].nSec = 4;
uda2[2].nInc = 10;

m_spinDan.SetAccel (1, &uda);
m_spinMje.SetAccel (1, &uda);
m_spinGod.SetAccel (3, uda2);


UpdateData(FALSE);


/*  //ovo se više ne koristi

CButton * pRadio1 = (CButton*)this->GetDlgItem(IDC_SUNCANO);     // uzmi pokazivaèe na gumbe
CButton * pRadio2 = (CButton*)this->GetDlgItem(IDC_LJETNO);

if ( m_iVri==1 ) { 
	pRadio1->SetCheck(0);                                        // postavi radio butone
	pRadio2->SetCheck(1);
}
else { 
	pRadio1->SetCheck(1);              
	pRadio2->SetCheck(0);
}
*/

UpdateData(TRUE); 


//zanimanje

	int j;
	boolean b;

	for(i=1; i<m_aBaza.GetSize(); i++)  { 
		
		b=FALSE;
		for (j=0; j< m_cbZanimanje.GetCount(); j++)	{

			if ( m_cbZanimanje.FindString(0,  ( m_aBaza.GetAt(i))->kod )  >= 0 ) {  b=TRUE; break; }
		}
	
		if (!b) m_cbZanimanje.AddString( (m_aBaza.GetAt(i))->kod );   
	}


	m_cbZanimanje.SetCurSel(m_iZanimSel);
	m_cbZanimanje.EnableWindow(m_chkZanimanje);
	UpdateData(FALSE);





CString sZn[12]={"ari","tau","gem","can","leo","vir","lib","sco","sag","cap","aqu","pis"};
CString sOb[11]={"sun","mje","mer","ven","mar","jup","sat","ura","nep","plu","asc"};

for(i=1;i<=12;i++) {
	m_cbZn1.AddString(sZn[i-1]);
	m_cbZn2.AddString(sZn[i-1]);
}

for(i=1;i<=11;i++) {
	m_cbOb1.AddString(sOb[i-1]);
	m_cbOb2.AddString(sOb[i-1]);
}

m_cbOb1.SetCurSel(0);                    
m_cbOb2.SetCurSel(1);                    
m_cbZn1.SetCurSel(0);                    
m_cbZn2.SetCurSel(0);

  
//m_cbZn1.ShowWindow(SW_HIDE);  
//m_cbZn2.ShowWindow(SW_HIDE);  
m_cbZn1.EnableWindow(m_chkOb1);
m_cbOb1.EnableWindow(m_chkOb1);  
  
m_cbZn2.EnableWindow(m_chkOb2);  
m_cbOb2.EnableWindow(m_chkOb2);  


//m_lbBaza.AddString("(unos)");
/*
for(i=0; i<=m_aBaza.GetSize()-1; i++)  {     
 
	CString buff3 = (m_aBaza.GetAt(i))->ime;
	m_lbBaza.AddString(buff3);                 

}
*/
Osvjezi_Baznu_Listu();

m_lbBaza.SetCurSel(m_iListBoxCurSel);
OnSelchangeListBaza();

UpdateData(FALSE);                  
//Invalidate();



//spin
/*
if (fmod2(m_iGod,4)==0 && fmod2(m_iGod,100)!=0 ) pl[1]=29;  //prestupne godine
if (fmod2(m_iGod,400)==0) pl[1]=29;

m_spinDan.SetRange(1,pl[m_iMje-1]);
m_spinMje.SetRange(1,12);
m_spinGod.SetRange(1,3000);


m_spinDan.SetPos(m_iDan);
m_spinMje.SetPos(m_iMje);
m_spinGod.SetPos(m_iGod);

m_iSelDan=m_iDan;
m_iSelMje=m_iMje;
m_iSelGod=m_iGod;


UpdateData(FALSE);
*/



m_bInit = FALSE;

return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE


}


//*******************************************************************************************************
void CUlazDlg::OnSelchangeGrad() 
{
	// TODO: Add your control notification handler code here

CString sBuf="";

int index = m_cbGrad.GetCurSel();


if (index != LB_ERR)
{

sBuf= m_saGradovi.GetAt(index);

m_sGrad=sBuf.Left(32);

sscanf(sBuf.Mid(48,10),"%d",&m_iGmt); 
 
sscanf(sBuf.Mid(33,5),"%f",&m_fSir);
m_fSir = floor(m_fSir) + (m_fSir - floor(m_fSir) )/.6;
if(sBuf.GetAt(38)=='S') m_fSir=-m_fSir;

sscanf(sBuf.Mid(40,6),"%f",&m_fDuz);
m_fDuz=floor(m_fDuz)+(m_fDuz-floor(m_fDuz))/.6;
if(sBuf.GetAt(46)=='E') m_fDuz=-m_fDuz;


m_sDuz.Format("%03.0f%c%02.0f' %c",floor(fabs(m_fDuz)),176,(fabs(m_fDuz)-floor(fabs(m_fDuz)))*.6*100,sBuf.GetAt(46));	
m_sSir.Format("%02.0f%c%02.0f' %c",floor(fabs(m_fSir)),176,(fabs(m_fSir)-floor(fabs(m_fSir)))*.6*100,sBuf.GetAt(38));

m_iGmTime = m_iGmt;
	
UpdateData(FALSE);

}
}


//*******************************************************************************************************
void CUlazDlg::OnSelectDatum(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	//MessageBox("tucam");
	
	m_cDatum.GetCurSel(&m_dDatum); 
	
    m_iDan = m_dDatum.wDay;
    m_iMje = m_dDatum.wMonth;
    m_iGod = m_dDatum.wYear;
	
	m_sDatum.Format("%i.%i.%i", m_iDan, m_iMje, m_iGod );

	m_iListBoxCurSel = m_lbBaza.GetCurSel();

	UpdateData(FALSE);


		//Spin kontrole
		int pl[12]={31,28,31,30,31,30,31,31,30,31,30,31};  
		if (fmod2(m_iGod,4)==0 && fmod2(m_iGod,100)!=0 ) pl[1]=29;  //prestupne godine
		if (fmod2(m_iGod,400)==0) pl[1]=29;

		m_spinDan.SetRange(1,pl[m_iDan-1]);
		m_spinMje.SetRange(1,12);
		m_spinGod.SetRange(0,3000);


		m_spinDan.SetPos(m_iDan);
		m_spinMje.SetPos(m_iMje);
		m_spinGod.SetPos(m_iGod);

		m_iSelDan=m_iDan;
		m_iSelMje=m_iMje;
		m_iSelGod=m_iGod;

		UpdateData(FALSE);


	
	*pResult = 0;
}




void CUlazDlg::OnDatetimechangeSat(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	m_iSat = m_tSat.GetHour();
	m_iMin = m_tSat.GetMinute();


	*pResult = 0;
}

//***************************************************************************************************************
void CUlazDlg::Ucitaj_bazu()
{
	m_aBaza.SetSize(0,10);   

	m_pBZapis = new CBazniZapis(); 
	m_pBZapis->ime=" <sada>";
	m_pBZapis->kuce='K';
	m_pBZapis->spol='-';

	m_aBaza.Add(m_pBZapis);

	
	FILE *fajl;
	 char buff[220], im[100], kd[30];
	 float fgmt;
	 int i;
    
	 fajl=fopen(m_sNazivBaze,"rt");  if (fajl==NULL) { MessageBox("U bazi nema podataka"); return; }

    do {
      fgets(buff,219,fajl); if (feof(fajl)) break;

		 m_pBZapis = new CBazniZapis();

		 sscanf(buff,"%s = %s (%d.%d.%d.) %d:%d %f %f %f %c  spol:%c %f %f %f %f %f %f %f %f %f %f %f\n",
		 //sscanf(buff,"%s = %s (%d.%d.%d.)",
			&kd[0], &im[0],
			&(m_pBZapis->dan),&(m_pBZapis->mje),&(m_pBZapis->god),
			&(m_pBZapis->sat),&(m_pBZapis->min),
			&(m_pBZapis->duz),&(m_pBZapis->sir), &fgmt,
			&(m_pBZapis->kuce),&(m_pBZapis->spol),
			&(m_pBZapis->o[0]),&(m_pBZapis->o[1]),&(m_pBZapis->o[2]),&(m_pBZapis->o[3]),&(m_pBZapis->o[4]),&(m_pBZapis->o[5]),
			&(m_pBZapis->o[6]),&(m_pBZapis->o[7]),&(m_pBZapis->o[8]),&(m_pBZapis->o[9]),&(m_pBZapis->o[10])
		 );
		 m_pBZapis->kod.Format("%s",kd);
		 m_pBZapis->ime.Format("%s",im);
		 m_pBZapis->gmt = (int)floor(fgmt);

 		m_aBaza.Add(m_pBZapis);

     } while (!feof(fajl));
 
	 fclose(fajl);

	
 
}


//************************************************************************************
void CUlazDlg::OnChkOb1() 
{
	// TODO: Add your control notification handler code here

	if  ( (!m_chkOb2) && 
			( ((  ( (CButton*)this->GetDlgItem(IDC_CHK_OB1) )->GetState() ) & 0x0008 ) !=0 )  ){   //provjeri da li je checkbox bio fokusiran
																																//ako je promjeni njegovo stanje
																																//a ako ne onda su komboi pozvali f.

		m_chkOb1 = ! m_chkOb1;

		m_cbZn1.EnableWindow(m_chkOb1);  
		m_cbOb1.EnableWindow(m_chkOb1);  
		UpdateData(FALSE);
	}


	Osvjezi_Baznu_Listu();


}


//************************************************************************************
void CUlazDlg::OnChkOb2() 
{
	// TODO: Add your control notification handler code here
	if ( (m_chkOb1) && ( ((  ( (CButton*)this->GetDlgItem(IDC_CHK_OB2) )->GetState() ) & 0x0008 ) !=0 ) ) {   //provjeri da li je checkbox bio fokusiran
																														                   //ako je promjeni njegovo stanje
																															                //a ako ne onda su komboi pozvali f.

			m_chkOb2 = ! m_chkOb2;

			m_cbZn2.EnableWindow(m_chkOb2);  
			m_cbOb2.EnableWindow(m_chkOb2);  
		}

		UpdateData(FALSE);

		Osvjezi_Baznu_Listu();

}


//***************************************************************************************
void CUlazDlg::OnRadSpm() 
{
	char spol[3]={'M','Z','-'};

	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	m_chSpol=spol[m_radSpol];

	Osvjezi_Baznu_Listu();

}




//******************************************************************************************
void CUlazDlg::Osvjezi_Baznu_Listu()

{
	int i;
	CString ss;
	
	//for(i=0; i<=m_lbBaza.GetCount(); i++ ) m_lbBaza.DeleteString(i);  // prvo sve obriši u listi
	m_lbBaza.ResetContent();

	//****************************************
	// èitanje sadržaja selekcije kombo boxa *
	//****************************************

	   m_cbZanimanje.GetWindowText(ss);  //od parent klase CWnd, naslijeðena metoda!!!! 


	for(i=0; i<=m_aBaza.GetSize()-1; i++)  {              // onda dodaj one koji zadovoljavaju

		if (  ( (int)floor( (m_aBaza.GetAt(i))->o[m_cbOb1.GetCurSel()] /30   ) == m_cbZn1.GetCurSel() || m_chkOb1 ==FALSE ) 
		   && ( (m_aBaza.GetAt(i))->spol == m_chSpol || m_chSpol == '-')

			&& ( (int)floor( (m_aBaza.GetAt(i))->o[m_cbOb2.GetCurSel()] /30   ) == m_cbZn2.GetCurSel() || m_chkOb2 ==FALSE ) 

			&& (  ss.Compare( (m_aBaza.GetAt(i))->kod ) == 0  ||  m_chkZanimanje == FALSE )
			
		) {

			CString buff3 = (m_aBaza.GetAt(i))->ime;
			m_lbBaza.AddString(buff3);   
		}            

	}

	//m_lbBaza.SetCurSel(0); 
	UpdateData(FALSE);      //inicijaliziraj             
	//Invalidate();


}



//***************************************************************************************************************
void CUlazDlg::OnSelchangeListBaza() 
{
	// TODO: Add your control notification handler code here
	
	int i;
	boolean b = FALSE;
	
	char str[100];
	CString str2;

	m_iListBoxCurSel = m_lbBaza.GetCurSel();
	m_lbBaza.GetText(m_lbBaza.GetCurSel(), &str[0] );

	str2.Format("%s", str);

	m_sIme=CString(str2);



		for(i=0;i<=m_aBaza.GetSize()-1; i++)  {    


			if (  (m_aBaza.GetAt(i))->ime.Compare(str2)  ==0 )  {b=TRUE; break;	}

		}
	
	
	
	if (b) {	

		m_sGrad = "<iz baze>"; 
	
		if (i>0) {
			GetDlgItem(IDC_GRAD)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);  

			GetDlgItem(IDC_SEL_DAN)->EnableWindow(FALSE);
			GetDlgItem(IDC_SEL_MJE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SEL_GOD)->EnableWindow(FALSE);

			GetDlgItem(IDC_DATUM)->EnableWindow(FALSE);

			GetDlgItem(IDC_SAT)->EnableWindow(FALSE);


			m_iSat = (m_aBaza.GetAt(i))->sat;
			m_iMin = (m_aBaza.GetAt(i))->min;

			m_iDan = (m_aBaza.GetAt(i))->dan;
			m_iMje = (m_aBaza.GetAt(i))->mje;
			m_iGod = (m_aBaza.GetAt(i))->god;
	
			m_iGmt = (m_aBaza.GetAt(i))->gmt;

			m_fDuz = (m_aBaza.GetAt(i))->duz;
			m_fSir= (m_aBaza.GetAt(i))->sir;

			m_cSpol = (m_aBaza.GetAt(i))->spol;

		}

		else {
			GetDlgItem(IDC_GRAD)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);  

			GetDlgItem(IDC_SEL_DAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_SEL_MJE)->EnableWindow(TRUE);
			GetDlgItem(IDC_SEL_GOD)->EnableWindow(TRUE);

			GetDlgItem(IDC_DATUM)->EnableWindow(TRUE);

			GetDlgItem(IDC_SAT)->EnableWindow(TRUE);


			m_iSat = CTime::GetCurrentTime().GetHour();
			m_iMin = CTime::GetCurrentTime().GetMinute();
			m_iDan = CTime::GetCurrentTime().GetDay();
			m_iMje = CTime::GetCurrentTime().GetMonth();
			m_iGod = CTime::GetCurrentTime().GetYear();

			m_cSpol ='-';

			this->OnSelchangeGrad(); 
	
		}


		m_chSustavKuca = (m_aBaza.GetAt(i))->kuce;

		char buf1='N',buf2='W';
		if (m_fSir<0) buf1='S'; if (m_fDuz<0) buf2='E';

		m_sDuz.Format("%02.0f%c%02.0f' %c",floor(fabs(m_fDuz)),176,(fabs(m_fDuz)-floor(fabs(m_fDuz)))*.6*100,buf2);	
		m_sSir.Format("%02.0f%c%02.0f' %c",floor(fabs(m_fSir)),176,(fabs(m_fSir)-floor(fabs(m_fSir)))*.6*100,buf1);

		m_iGmTime = m_iGmt;

		m_sDatum.Format("%02i.%02i.%02i", m_iDan, m_iMje, m_iGod );  //Novi naèin! (metoda CString-a)


		if (m_iGod>1754){

			GetDlgItem(IDC_DATUM)->ShowWindow(SW_SHOW);
			SYSTEMTIME d;
			d.wDay =m_iDan;
			d.wMonth = m_iMje;
			d.wYear = m_iGod;
			d.wHour = m_iSat;
			d.wMinute = m_iMin;
			d.wSecond =0;

			m_cDatum.SetCurSel(&d);
		

			UpdateData(FALSE);
		}
		else  GetDlgItem(IDC_DATUM)->ShowWindow(SW_HIDE);



		CTime t(1990,1,1,m_iSat,m_iMin,0);
		CDateTimeCtrl* pSat = (CDateTimeCtrl*) GetDlgItem(IDC_SAT);
		pSat->SetTime(&t);

		UpdateData(TRUE);


		//Spin kontrole
		
		m_bInit = TRUE;
		int pl[12]={31,28,31,30,31,30,31,31,30,31,30,31};  
		if (fmod2(m_iGod,4)==0 && fmod2(m_iGod,100)!=0 ) pl[1]=29;  //prestupne godine
		if (fmod2(m_iGod,400)==0) pl[1]=29;

		m_spinDan.SetRange(1,pl[m_iMje-1]);
		m_spinMje.SetRange(1,12);
		m_spinGod.SetRange(0,3000);


		m_spinDan.SetPos(m_iDan);
		m_spinMje.SetPos(m_iMje);
		m_spinGod.SetPos(m_iGod);


		m_iSelDan=m_iDan;
		m_iSelMje=m_iMje;
		m_iSelGod=m_iGod;

		UpdateData(FALSE);

		m_bInit=FALSE;
	
		
	}


}
//*******************************************************************************************************
void CUlazDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
	m_dUnos.SetParams(0,0,"", "",m_iVri,TRUE ) ;    //oèisti rubrike za unos u dijalogu za dodatni unos
	if (m_dUnos.DoModal() != IDOK ) return;   //unos novog zapisa u bazu

	char spol[3]={'M','Z','-'};
	char kuce[3]={'K','J','B'};


float Gvr,k=0,ta, i,j,e,rsir,jj,ra,gmt=m_iGmt;
double pi=3.1415927;
double zvr=0,asc=0;
double tg=0,es=0,ec=0,ar=0,sb=0,cb=0,el=0,ea=0,x,y;
int ii,pl[12]={31,28,31,30,31,30,31,31,30,31,30,31};  

FILE *fajl, *fajl2;
int danx,mjex,godx,satx,minx;

char kod[30],uime[60];
int iVri;

m_dUnos.m_sNaziv.Replace(' ','_');
m_dUnos.m_sKod.Replace(' ','_');

//CString sVelSlova= m_dUnos.m_sNaziv;           //zastarjelo
//sVelSlova.MakeUpper();

strncpy(&uime[0],m_dUnos.m_sNaziv, __min(m_dUnos.m_sNaziv.GetLength(),59) );     // izdvoji naziv grada 
strncpy(&kod[0],m_dUnos.m_sKod, __min(m_dUnos.m_sKod.GetLength( ),29)  );     // izdvoji naziv grada 

uime[(int)__min(m_dUnos.m_sNaziv.GetLength(),59 ) ]=0;
kod[(int)__min(m_dUnos.m_sKod.GetLength(),29 ) ]=0;


   iVri=0; if (m_dUnos.m_bDST==TRUE) iVri=1;

   danx=m_iDan;
	mjex=m_iMje; 
	godx=m_iGod; 
	satx=m_iSat-iVri; 
	minx=m_iMin;

	if(satx<0) {
		satx+=24;
		danx-=1;}

	if (fmod2(godx,4)==0 && fmod2(godx,100)!=0 ) pl[1]=29;  //prestupne godine
	if (fmod2(godx,400)==0) pl[1]=29;

	if(danx<1) {
		mjex-=1;
		if(mjex<1) {
			mjex=12;
			godx-=1;	}	
		danx=pl[mjex-1];	
	}


	Planete(danx,mjex,godx,satx,minx);
 

	//raèunanje ascedenta
Gvr=(double)(satx-m_iGmt+minx/60.); //Globalno vrijeme
if(Gvr<0) Gvr+=24;

i=-(mjex<3);
k=danx+floor((mjex*153.-i*11.-162)/5.)+floor((godx*1461.+i)/4.)-(godx>=0)*366.;
if (k>577736) k-=floor((floor((godx+i)/100.)*3-5)/4.);

ta=k-693596; tg=k-711858+Gvr/24.;                   // tg je GLOBAL
jj=ta/36525. ; i=ta/36525.;
e=23.452294-jj*0.013125-jj*jj*1.639e-6+jj*jj*jj*5.028e-7;
es=sin(e*pi/180); ec=cos(e*pi/180);                // GLOBAL

ar=Gvr*360.985647/24+frac(ta/1461)*1440.02509;      // GLOBAL
ar+=floor(ta/1461)*0.0307572+99.2018973;
ar=fnmo(ar);
zvr=ar*24/360-m_fDuz*4/60; while(zvr>24)  zvr-=24;
            while(zvr<0)   zvr+=24; 

  ra=zvr*pi/12; rsir=m_fSir*pi/180;
  x=cos(rsir)*sin(ra)*ec+sin(rsir)*es; y=cos(rsir)*cos(ra);
  asc=atan(x/y); if(asc<0) asc+=pi;
		 if(x<0) asc+=pi;
  asc=180/pi*(asc+pi/2);
  while(asc>360) asc-=360;


 try {
	 CFile::Remove("BAZA.HOR.BAK");            //obriši stari backup
 }
 catch(CFileException* e){ MessageBox("nemoguæe obrisati stari backup horoteke!"); }

 //napravi bekap
fajl=fopen(m_sNazivBaze,"rt");  if (fajl==NULL) { MessageBox("Nema fajla baze - nemoguæe napravit backup");return; }
fajl2=fopen("BAZA.HOR.BAK","wt"); 
char buff[220];

   do {
      fgets(buff,219,fajl); if (feof(fajl)) break;
	  fputs(buff,fajl2);
   } while (!feof(fajl));

fclose(fajl);
fclose(fajl2);




fajl=fopen(m_sNazivBaze,"at");  if (fajl==NULL) { MessageBox("Nema fajla baze - nemoguæ unos novog zapisa");return; }


fprintf(fajl,"%s = %s (%d.%d.%d.) %d:%d %f %f %f %c  spol:%c",kod,uime,m_iDan,m_iMje,m_iGod,m_iSat,m_iMin,m_fDuz,m_fSir,
		                                                        (float)gmt,toupper(kuce[m_dUnos.m_radKucice]),spol[m_dUnos.m_radSpol] );
for(ii=1;ii<=10;ii++) fprintf(fajl," %06.2f",(float)objx[ii]);
fprintf(fajl," %06.2f",(float)asc);
fprintf(fajl,"\n");
fclose(fajl);

MessageBox("Zapis uspješno pohranjen!");

//obriši staru bazu
ii = 0;
while (ii < m_aBaza.GetSize() )
{
    delete m_aBaza.GetAt( ii++ );  //obriši fizièke objekte tipa CBazniZapis
}
m_aBaza.RemoveAll();  //obriši reference u arrayu

Ucitaj_bazu();
Osvjezi_Baznu_Listu();

OnOK();

}
//***********************************************************************
void CUlazDlg::Planete(int dan, int mje, int god, int sat, int min)
{
 double D[13],X[13],Y[13],Z[13],K[13],C[13],M[13];
 int br=0,rvs,ix; //brojac podataka
 double a,b,d,e,f,g,g1,ii,l,ll,l1,m,n,r,s,s1,s2,t,u,v,x,y,z,au,ea,fl,k1,qq=3,tn,td,ml,jd,Gvr;
 int i,j;
 double pi=3.1415927;
 double gmt=m_iGmt;

Gvr=(double)(sat-gmt+min/60.); //Globalno vrijeme

jd=367*(double)god-floor(7.*((double)god+floor(((double)mje+9)/12.))/4.);
jd+=floor(275.*mje/9.)+(double)dan+1721013.5;
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
	  objx[j+1]=z; //objy[j+1]=rvs;
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
objx[1]=objx[2]; //objy[1]=objy[2];
objx[2]=objx[11]; //objy[2]=objy[11];
for(i=1;i<=12;i++) objx[i]=fnmo(objx[i]);
return;
}
//*********************************************
double CUlazDlg::frac(double x)
{
return x-floor(x);

}
//**********************************************
double CUlazDlg::fnmo(double x)
{
return(x-floor(x/360)*360);
}
//************************************************
double CUlazDlg::sgn(double x)
{
 if(x<0) return (-1);
 if(x==0) return (0);
 return (1);

}
//**************************************************
void CUlazDlg::readdata(int *i1, double *f1)
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
//********************************************************************
void CUlazDlg::ln1060(double xx, double *yy, double *rr, double *aa)
{
double pi=3.1415927;

if(*yy==0) *yy=1.745e-9;
*rr=sqrt(xx*xx+(*yy)*(*yy)); *aa=atan(*yy/xx); if(*aa<0) (*aa)+=pi; if(*yy<0) (*aa)+=pi;

}
//************************************
double CUlazDlg::fns(double x)
{
double pi=3.1415927;
return(sin(x*pi/180)); 

}
//************************************
double CUlazDlg::fnp(double x, double mm)
{
return( sgn(x)* ( (fabs(x)/mm)/360.-floor((fabs(x)/mm)/360))*360);
}
//*******************************************************************
int CUlazDlg::fmod2(double a, double b)
{
return  fmod(a,b);
}

//*******************************************************************
double CUlazDlg::fabs2(double a)
{
return  fabs(a);
}


//*******************************************************************************************************************

void CUlazDlg::OnChangeSelDan() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	if (!m_bInit) {


	
		UpdateData(TRUE);

		/*
		CString buff;
		buff.Format("%d %d %d",m_iSelDan, m_iSelMje, m_iSelGod);
		MessageBox(buff);
		*/

		m_iDan=m_iSelDan;
		m_iMje=m_iSelMje;
		m_iGod=m_iSelGod;

		int pl[12]={31,28,31,30,31,30,31,31,30,31,30,31};  
		if (fmod2(m_iGod,4)==0 && fmod2(m_iGod,100)!=0 ) pl[1]=29;  //prestupne godine
		if (fmod2(m_iGod,400)==0) pl[1]=29;

		m_spinDan.SetRange(1,pl[m_iMje-1]);

		m_spinDan.SetPos(m_iDan);
		m_spinMje.SetPos(m_iMje);
		m_spinGod.SetPos(m_iGod);

		SYSTEMTIME d;
		d.wDay =m_iDan;
		d.wMonth = m_iMje;
		d.wYear = m_iGod;
		d.wHour = m_iSat;
		d.wMinute = m_iMin;
		d.wSecond =0;

		m_cDatum.SetCurSel(&d);
		

		UpdateData(FALSE);



	}
	
}
//*****************************************************************
void CUlazDlg::OnChkZanim() 
{
	// TODO: Add your control notification handler code here

	//m_chkZanimanje = !m_chkZanimanje;
	UpdateData(TRUE);

	m_cbZanimanje.EnableWindow(m_chkZanimanje);

	Osvjezi_Baznu_Listu();


	
}
//*********************************************************
void CUlazDlg::OnSelchangeZanimanje() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	m_iZanimSel = m_cbZanimanje.GetCurSel();
	
	
	m_cbZanimanje.SetCurSel(m_iZanimSel);
	UpdateData(FALSE);


	Osvjezi_Baznu_Listu();

	
}

//*********************************************************************************
void CUlazDlg::Obrisi_Zapis()
{

	int i, delcount=0;
	boolean b = FALSE;
	
	char str[100];
	CString s1,s2;

	m_lbBaza.GetText(m_lbBaza.GetCurSel(), &str[0] );
	s1.Format("%s", str);
	
	if (AfxMessageBox("zelis li stvarno obrisati zapis '" + s1 + "'  ??", MB_OKCANCEL|MB_DEFBUTTON2 )==IDCANCEL ) return;

	
	FILE *fajl1, *fajl2;
	 char buff[220], im[100], kd[30];
	 float fgmt;
   
	 fajl1=fopen(m_sNazivBaze,"rt");  if (fajl1==NULL) { MessageBox("nemoguæe obrisati"); return; }
	 fajl2=fopen("baza_v22.txt","wt");  if (fajl2==NULL) { fclose(fajl1); MessageBox("nemoguæe obrisati"); return; }

    do {
      fgets(buff,219,fajl1); if (feof(fajl1)) break;

		 sscanf(buff,"%s = %s",	&kd[0], &im[0]);

		 s2.Format("%s",im);

		 if ( delcount==0  && s1.Compare(s2)==0) {delcount++; continue;}  //ako ima više istih zapisa, obriši samo jednog
			else fputs(buff,fajl2);

     } while (!feof(fajl1));
 
	 fclose(fajl1);
	 fclose(fajl2);


 try {
	 CFile::Remove("BAZA.HOR.BAK");            //obriši stari backup
 }
 catch(CFileException* e){ MessageBox("Nemoguæe obrisati stari backup horoteke!"); }

 try {
	 CFile::Rename(m_sNazivBaze, "BAZA.HOR.BAK");            //od stare baze napravi backup
 }
 catch(CFileException* e){ MessageBox("Nemoguæe postaviti novi backup horoteke!");}


 try {
	 CFile::Rename("baza_v22.txt",m_sNazivBaze);        //preimenuj novu u naziv stare
 }
 catch(CFileException* e){ MessageBox("Nemoguæe obrisati novu horoteku!");}



MessageBox("Zapis uspješno obrisan!");

i = 0;
while (i < m_aBaza.GetSize() )
{
    delete m_aBaza.GetAt( i++ );  //obriši fizièke objekte tipa CBazniZapis
}
m_aBaza.RemoveAll();  //obriši reference u arrayu

Ucitaj_bazu();
Osvjezi_Baznu_Listu();

OnOK();


}

//**************************************************************************************************************
void CUlazDlg::Izmjeni_Zapis()
{
	CString s1,s2;
	char str[100];
	int i,ii, kuce,spol;
	boolean b =FALSE;

	FILE *fajl1, *fajl2;
	char buff[220];
	char kod[30],uime[60];
	char kd[30],im[60];



	m_lbBaza.GetText(m_lbBaza.GetCurSel(), &str[0] );
	s1.Format("%s", str);


	for(i=0;i<=m_aBaza.GetSize()-1; i++)  {    //naði u bazi

		if (  (m_aBaza.GetAt(i))->ime.Compare(s1)  ==0 )  {b=TRUE; break;	}

	}



if (b==FALSE) return;

if ( (m_aBaza.GetAt(i))->kuce == 'K' ) kuce=0;                
else if (  (m_aBaza.GetAt(i))->kuce == 'J' ) kuce=1;
else kuce =2;

if ( (m_aBaza.GetAt(i))->spol == 'M' ) spol=0;
else if (  (m_aBaza.GetAt(i))->spol == 'Z' ) spol=1;
else spol=2;


m_dUnos.SetParams(kuce,spol,(m_aBaza.GetAt(i))->kod, s1, 0, FALSE);    //postavi 


if (m_dUnos.DoModal()!=IDOK) return;




	char cspol[3]={'M','Z','-'};
	char ckuce[3]={'K','J','B'};


m_dUnos.m_sNaziv.Replace(' ','_');
m_dUnos.m_sKod.Replace(' ','_');


strncpy(&uime[0],m_dUnos.m_sNaziv, __min(m_dUnos.m_sNaziv.GetLength(),59) );     // izdvoji naziv grada 
strncpy(&kod[0],m_dUnos.m_sKod, __min(m_dUnos.m_sKod.GetLength( ),29)  );     // izdvoji naziv grada 

uime[(int)__min(m_dUnos.m_sNaziv.GetLength(),59 ) ]=0;
kod[(int)__min(m_dUnos.m_sKod.GetLength(),29 ) ]=0;



//promjeni podatke zapisa u fajlu:


 
	 fajl1=fopen(m_sNazivBaze,"rt");  if (fajl1==NULL) { MessageBox("nemoguæe promijeniti"); return; }
	 fajl2=fopen("baza_v22.txt","wt");  if (fajl2==NULL) { fclose(fajl1); MessageBox("nemoguæe promijeniti"); return; }

    do {
      fgets(buff,219,fajl1); if (feof(fajl1)) break;

		 sscanf(buff,"%s = %s",	&kd[0], &im[0]);

		 s2.Format("%s",im);

		if (s1.Compare(s2)==0 ) {
		
			fprintf(fajl2,"%s = %s (%d.%d.%d.) %d:%d %f %f %f %c  spol:%c",
				kod,uime,(m_aBaza.GetAt(i))->dan, (m_aBaza.GetAt(i))->mje, (m_aBaza.GetAt(i))->god,
				(m_aBaza.GetAt(i))->sat, (m_aBaza.GetAt(i))->min,
				(m_aBaza.GetAt(i))->duz, (m_aBaza.GetAt(i))->sir,
				(float)(m_aBaza.GetAt(i))->gmt,
				toupper(ckuce[m_dUnos.m_radKucice]),cspol[m_dUnos.m_radSpol] );
				
			for(ii=0;ii<=10;ii++) fprintf(fajl2," %06.2f",(m_aBaza.GetAt(i))->o[ii]  );
			fprintf(fajl2,"\n");

		}
		else fputs(buff,fajl2);

     } while (!feof(fajl1));
 
	 fclose(fajl1);
	 fclose(fajl2);


 try {
	 CFile::Remove("BAZA.HOR.BAK");            //obriši stari backup
 }
 catch(CFileException* e){ MessageBox("Nemoguæe obrisati stari backup horoteke!"); }

 try {
	 CFile::Rename(m_sNazivBaze, "BAZA.HOR.BAK");            //od stare baze napravi backup
 }
 catch(CFileException* e){ MessageBox("Nemoguæe postaviti novi backup horoteke!");}


 try {
	 CFile::Rename("baza_v22.txt",m_sNazivBaze);        //preimenuj novu u naziv stare
 }
 catch(CFileException* e){ MessageBox("Nemoguæe obrisati novu horoteku!");}


	MessageBox("Zapis uspješno promjenjen!");
	

//obriši staru bazu
 i = 0;
while (i< m_aBaza.GetSize() )
{
    delete m_aBaza.GetAt( i++ );  //obriši fizièke objekte tipa CBazniZapis
}
m_aBaza.RemoveAll();  //obriši reference u arrayu

Ucitaj_bazu();
Osvjezi_Baznu_Listu();

return;

}

