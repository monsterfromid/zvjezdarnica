// Postavke.cpp : implementation file
//

#include "stdafx.h"
#include "asc2win.h"
#include "Postavke.h"
#include "afxtempl.h"       // MFC suport for Collections
#include "stdlib.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPostavke dialog


CPostavke::CPostavke(CWnd* pParent /*=NULL*/)
	: CDialog(CPostavke::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPostavke)
	m_iAspTip = -1;
	m_bAspTab = FALSE;
	m_iKuceIzr = -1;
	m_bDST = FALSE;
	m_iOrbAsp1 = 0;
	m_iOrbAsp2 = 0;
	m_iOrbAsp3 = 0;
	m_iOrbAsp4 = 0;
	m_iOrbAsp5 = 0;
	m_iOrbAsp6 = 0;
	m_iOrbPla1 = 0;
	m_iOrbPla2 = 0;
	m_iOrbPla3 = 0;
	m_iOrbPla4 = 0;
	m_iOrbPla5 = 0;
	m_iOrbPla6 = 0;
	m_iOrbPla7 = 0;
	m_iOrbPla8 = 0;
	m_iOrbPla9 = 0;
	m_iOrbPla10 = 0;
	m_iOrbPla11 = 0;
	m_iOrbPla12 = 0;
	m_iTipIspisa = -1;
	m_sImeBaze = _T("");
	m_bNebo = FALSE;
	m_bSotGodPrecesija = FALSE;
	m_bGregKal = FALSE;
	//}}AFX_DATA_INIT
}


void CPostavke::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPostavke)
	DDX_Control(pDX, IDC_POC, m_cbPoc);
	DDX_Control(pDX, IDC_KOR_SAT, m_cbKorSat);
	DDX_Control(pDX, IDC_KOR_MIN, m_cbKorMin);
	DDX_Radio(pDX, IDC_ASP_NORM, m_iAspTip);
	DDX_Check(pDX, IDC_ASP_TAB, m_bAspTab);
	DDX_Radio(pDX, IDC_KUCE_KOCH, m_iKuceIzr);
	DDX_Check(pDX, IDC_LJETNO, m_bDST);
	DDX_Text(pDX, IDC_ORB0, m_iOrbAsp1);
	DDX_Text(pDX, IDC_ORB1, m_iOrbAsp2);
	DDX_Text(pDX, IDC_ORB2, m_iOrbAsp3);
	DDX_Text(pDX, IDC_ORB3, m_iOrbAsp4);
	DDX_Text(pDX, IDC_ORB4, m_iOrbAsp5);
	DDX_Text(pDX, IDC_ORB5, m_iOrbAsp6);
	DDX_Text(pDX, IDC_ORBS0, m_iOrbPla1);
	DDX_Text(pDX, IDC_ORBS1, m_iOrbPla2);
	DDX_Text(pDX, IDC_ORBS2, m_iOrbPla3);
	DDX_Text(pDX, IDC_ORBS3, m_iOrbPla4);
	DDX_Text(pDX, IDC_ORBS4, m_iOrbPla5);
	DDX_Text(pDX, IDC_ORBS5, m_iOrbPla6);
	DDX_Text(pDX, IDC_ORBS6, m_iOrbPla7);
	DDX_Text(pDX, IDC_ORBS7, m_iOrbPla8);
	DDX_Text(pDX, IDC_ORBS8, m_iOrbPla9);
	DDX_Text(pDX, IDC_ORBS9, m_iOrbPla10);
	DDX_Text(pDX, IDC_ORBS10, m_iOrbPla11);
	DDX_Text(pDX, IDC_ORBS11, m_iOrbPla12);
	DDX_Radio(pDX, IDC_TIP_KUCE, m_iTipIspisa);
	DDX_Text(pDX, IDC_IME_BAZE, m_sImeBaze);
	DDX_Check(pDX, IDC_NEBO, m_bNebo);
	DDX_Check(pDX, IDC_SOTGOD, m_bSotGodPrecesija);
	DDX_Check(pDX, IDC_GREGKAL, m_bGregKal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPostavke, CDialog)
	//{{AFX_MSG_MAP(CPostavke)
	ON_EN_CHANGE(IDC_IME_BAZE, OnChangeImeBaze)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPostavke message handlers

BOOL CPostavke::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

CString sMin[6]={"01","05","10","15","30","45"};
CString sSat[6]={"01","02","03","06","12"};

CString sPoc[14]={"Gregorijanski kalendar","Mjes.kretanje planeta","Zodijakalni horoskop","Tumaè zod. horoskopa",
				"Medicinski horoskop","Mjes.pomak ascendenta","Zvjezdano nebo","Mjeseèeve mjene","Sunèev sustav",
				"Kineski Horoskop","Asteèki Horoskop","Majanski Kalendar","Egipatski Kalendar" };

for(int i=1;i<=6;i++) 	m_cbKorMin.AddString(sMin[i-1]);
for(int i=1;i<=5;i++) 	m_cbKorSat.AddString(sSat[i-1]);
for(int i=1;i<=13;i++) 	m_cbPoc.AddString(sPoc[i-1]);

//m_cbKorMin.SetCurSel(0);                    
//m_cbKorSat.SetCurSel(1);
                    
ucitaj_postavke();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//**************************************************************************************************
void CPostavke::ucitaj_postavke()
{

FILE * fajl;
int i;
char buff[400],c;

fajl=fopen("pocetne_postavke.cfg","rt"); if(fajl==NULL) {MessageBox("nema fajla postavki horoskopa!"); return; }

m_iAspTip=0; fgets(buff,100,fajl);c=buff[0]; if(toupper(c)=='I') m_iAspTip=1;

m_iKuceIzr=0; 
fgets(buff,100,fajl);c=buff[0]; 
if(toupper(c)=='J') m_iKuceIzr=1;
if(toupper(c)=='B') m_iKuceIzr=2;

m_iTipIspisa = 0; 
fgets(buff,100,fajl);c=buff[0]; if(toupper(c)=='A') m_iTipIspisa=1;

m_bAspTab = FALSE;
fgets(buff,100,fajl);c=buff[0]; if(toupper(c)=='1') m_bAspTab = TRUE;


int iKorakMinute,iKorakSati;
CString bf;

fgets(buff,100,fajl); sscanf(buff,"%d",&iKorakMinute);
bf.Format("%02d",iKorakMinute);

m_cbKorMin.SetCurSel( m_cbKorMin.FindString( 0, bf) );
 
fgets(buff,100,fajl); sscanf(buff,"%d",&iKorakSati);
bf.Format("%02d",iKorakSati);
//MessageBox(bf);
m_cbKorSat.SetCurSel( m_cbKorSat.FindString( 0, bf) );                                  


char nazivBaze[100];
fgets(buff,100,fajl); sscanf(buff,"%s",&nazivBaze[0]); m_sImeBaze.Format("%s",nazivBaze);

m_bDST=FALSE; 
fgets(buff,100,fajl); c=buff[0]; if(toupper(c)=='1') m_bDST=TRUE;

float tmpOrb[7];                    // orbovi 
float tmpPOrb[13];

for(i=0;i<=5;i++) { fgets(buff,100,fajl); sscanf(buff,"%f",&tmpOrb[i]); }
for(i=0;i<=11;i++) { fgets(buff,100,fajl);sscanf(buff,"%f",&tmpPOrb[i]); }

m_iOrbAsp1=tmpOrb[0];
m_iOrbAsp2=tmpOrb[1];
m_iOrbAsp3=tmpOrb[2];
m_iOrbAsp4=tmpOrb[3];
m_iOrbAsp5=tmpOrb[4];
m_iOrbAsp6=tmpOrb[5];

m_iOrbPla1=tmpPOrb[0];
m_iOrbPla2=tmpPOrb[1];
m_iOrbPla3=tmpPOrb[2];
m_iOrbPla4=tmpPOrb[3];
m_iOrbPla5=tmpPOrb[4];
m_iOrbPla6=tmpPOrb[5];
m_iOrbPla7=tmpPOrb[6];
m_iOrbPla8=tmpPOrb[7];
m_iOrbPla9=tmpPOrb[8];
m_iOrbPla10=tmpPOrb[9];
m_iOrbPla11=tmpPOrb[10];
m_iOrbPla12=tmpPOrb[11];

m_bNebo=FALSE; fgets(buff,100,fajl);c=buff[0];  if(toupper(c)=='1') m_bNebo=TRUE;

int iOpc=1; fgets(buff,200,fajl); sscanf(buff,"%d",&iOpc); m_cbPoc.SetCurSel(iOpc-1);

m_bSotGodPrecesija=TRUE;
fgets(buff,100,fajl);c=buff[0]; 
if(toupper(c)=='N') m_bSotGodPrecesija=FALSE;


m_bGregKal=TRUE;
fgets(buff,100,fajl);c=buff[0]; 
if(toupper(c)=='N') m_bGregKal=FALSE;

//MessageBox(buff);

fclose(fajl);

UpdateData(FALSE);

}


//*********************************************************************************************************************
void CPostavke::snimi_postavke()
{
FILE * fajl;
int i;
char buff[400],cc;

UpdateData(TRUE);//uèitaj sve promjene s dialoga u varijable

 CString backupName;
 backupName = "pocetne_postavke.cfg.bak";
 try {
	CFile::Remove(backupName); 
 }
 catch(CFileException* e){
 }

 //obriši stari backup
 CFile::Rename("pocetne_postavke.cfg", backupName);   


fajl=fopen("pocetne_postavke.cfg","wt"); if(fajl==NULL) {MessageBox("pogreška kod snimanja postavki horoskopa!"); return; }

char cDaNe[2]={'D','N'};
char cAspTip[2]={'N','I'};
char cKuceIzr[3]={'K','J','B'};
char cTipIspisa[2]={'K','A'};
char cAspTab[2]={'0','1'};
char * cKorMin[6]={"01","05","10","15","30","45"};
char * ckorSat[5]={"01","02","03","06","12"};
char cDST[2]={'0','1'};
char cNebo[2]={'0','1'};
char * sOrb[6]={
	"//orbisi konjukcija",
	"//opozicija",
	"//sekstil",
	"//trokut",
	"//kvadrat",
	"//polukvadrat"};
char * sPOrb[12]={
	"// orbisi Sunce",       
	"//Mjesec",              
	"//Merkur",              
	"//Venera",              
	"//Mars",                
	"//Jup",                 
	"//Sat",                 
	"//Ura",                 
	"//Nep",                 
	"//Plu",                 
	"//Asc",                 
	"//MC"                  
};


fprintf(fajl,"%c  //aspekti (N)ormalno ili (I)stoèno%c",cAspTip[m_iAspTip],10);
fprintf(fajl,"%c  //izraèun kuæica (K)och, (J)ednake ili (B)ez%c", cKuceIzr[m_iKuceIzr],10 );
fprintf(fajl,"%c  //poè. tip horoskopa (K)uæe ili (A)spekti%c", cTipIspisa[m_iTipIspisa], 10);

switch (m_bAspTab) {
	case FALSE: fprintf(fajl,"%c  //tablica aspekata aktivna po ukljuèenju (0/1)%c",cAspTab[0] ,10);break;
	case TRUE:  fprintf(fajl,"%c  //tablica aspekata aktivna po ukljuèenju (0/1)%c",cAspTab[1] ,10);break;
}

fprintf(fajl,"%s  //korak promjene gumba minute (xx minuta, može biti 01,05,10,15,30,45)%c", cKorMin[m_cbKorMin.GetCurSel()] ,10 );
fprintf(fajl,"%s  //korak promjene gumba sati   (xx sati, može biti 01,02,03,06,12)%c",      ckorSat[m_cbKorSat.GetCurSel()] ,10 );


strncpy(&buff[0],m_sImeBaze, m_sImeBaze.GetLength()  );  
buff[(int) m_sImeBaze.GetLength() ]=0;
fprintf(fajl, "%s  //fajl horoteke%c",  buff, 10 );

switch (m_bDST) {
	case FALSE: fprintf(fajl,"%c  //DST (Ljetno vrijeme) 0-ne/1-da%c",  cDST[0] ,10);break;
	case TRUE:  fprintf(fajl,"%c  //DST (Ljetno vrijeme) 0-ne/1-da%c",  cDST[1] ,10);break;
}


float tmpOrb[7];                    // orbovi 
float tmpPOrb[13];

tmpOrb[0]=m_iOrbAsp1;
tmpOrb[1]=m_iOrbAsp2;
tmpOrb[2]=m_iOrbAsp3;
tmpOrb[3]=m_iOrbAsp4;
tmpOrb[4]=m_iOrbAsp5;
tmpOrb[5]=m_iOrbAsp6;

tmpPOrb[0]=m_iOrbPla1;
tmpPOrb[1]=m_iOrbPla2;
tmpPOrb[2]=m_iOrbPla3;
tmpPOrb[3]=m_iOrbPla4;
tmpPOrb[4]=m_iOrbPla5;
tmpPOrb[5]=m_iOrbPla6;
tmpPOrb[6]=m_iOrbPla7;
tmpPOrb[7]=m_iOrbPla8;
tmpPOrb[8]=m_iOrbPla9;
tmpPOrb[9]=m_iOrbPla10;
tmpPOrb[10]=m_iOrbPla11;
tmpPOrb[11]=m_iOrbPla12;

for(i=0;i<=5;i++)  { fprintf(fajl,"%.1f  %s%c",tmpOrb[i] ,sOrb[i] ,10); }
for(i=0;i<=11;i++) { fprintf(fajl,"%.1f  %s%c",tmpPOrb[i],sPOrb[i],10); }

switch (m_bNebo) {
	case FALSE: fprintf(fajl,"%c  //auto ispis imena objekata u opciji nebo%c",cNebo[0],10); break;
	case TRUE:  fprintf(fajl,"%c  //auto ispis imena objekata u opciji nebo%c",cNebo[1],10); break;
}


fprintf(fajl,"%d  //poè.opcija (1=greg,2=pla,3=hor,4=tumaè,5=med,6-asc,7-nebo,8=luna,9=sun,10=kin,11=azt,12=maj.,13=egip.)%c",m_cbPoc.GetCurSel()+1 ,10);

cc=cDaNe[0]; if (!m_bSotGodPrecesija) cc=cDaNe[1];
fprintf(fajl,"%c  //utj. precesije na poè. egip. sotièke godine D/N (def.Da)%c",cc,10);


cc=cDaNe[0]; if (!m_bGregKal) cc=cDaNe[1];
fprintf(fajl,"%c  //èisti stil greg.kalendara D/N (def.Ne)%c",cc,10);


fprintf(fajl,"-%c",10);

fclose(fajl);

MessageBox("postavke snimljene!");

}
//********************************************************************************************************************
void CPostavke::OnOK() 
{
	// TODO: Add extra validation here

	snimi_postavke();


	CDialog::OnOK();
}


//********************************************************************************************************************
void CPostavke::OnChangeImeBaze() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	m_sImeBaze.Replace(' ','_');
	m_sImeBaze.MakeUpper();
	UpdateData(FALSE);

}


