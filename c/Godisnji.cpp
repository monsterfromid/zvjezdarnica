// Godisnji.cpp : implementation file
//

#include "stdafx.h"
#include "asc2win.h"
#include "Godisnji.h"
#include "stdio.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGodisnji dialog


CGodisnji::CGodisnji(CWnd* pParent /*=NULL*/)
	: CDialog(CGodisnji::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGodisnji)
	m_sPocVriijeme = _T("");
	m_sIzracunatoVrijeme = _T("");
	m_iBrGod = 0;
	m_iGod = 0;
	//}}AFX_DATA_INIT
}


void CGodisnji::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGodisnji)
	DDX_Control(pDX, IDC_SPIN1, m_spinGod);
	DDX_Text(pDX, IDC_POCETNO_VRIJEME, m_sPocVriijeme);
	DDX_Text(pDX, IDC_IZRACUN, m_sIzracunatoVrijeme);
	DDX_Text(pDX, IDC_RBRGODINE, m_iBrGod);
	DDX_Text(pDX, IDC_GODINA, m_iGod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGodisnji, CDialog)
	//{{AFX_MSG_MAP(CGodisnji)
	ON_EN_CHANGE(IDC_RBRGODINE, OnChangeRbrgodine)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGodisnji message handlers

BOOL CGodisnji::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
CString bf;


m_sPocVriijeme.Format("natalna astr.karta: %20s\r\n-----------------------------------------------------------------------------------------------------------------------------\r\n", m_sIme);

bf.Format("vrijeme: %02d.%02d.%d,  %02d:%02d'", m_iDanBazni,m_iMjeBazni,m_iGodBazni,m_iSatBazni,m_iMinBazni );
m_sPocVriijeme+=bf;
 
if (m_iVriBazni!=0) {
	bf.Format(" (DST), %02d:%02d' (sunè.vrijeme)", m_iSatBazni-m_iVriBazni,m_iMinBazni );
	m_sPocVriijeme+=bf;
}

bf.Format("\r\nzemlj.koord. ( %s , %s ) , GMT:%02d", deg2dms(m_fDuzBazni,2), deg2dms(m_fSirBazni,3),  m_iGmtBazni);
m_sPocVriijeme+=bf;

UpdateData(FALSE);


m_spinGod.SetBuddy(GetDlgItem(IDC_RBRGODINE));

m_spinGod.SetBase(10);
m_spinGod.SetRange(1,100);


int iGod = CTime::GetCurrentTime().GetYear();

if (iGod >= m_iGodBazni+99 ) m_spinGod.SetPos(100);
else if (iGod >= m_iGodBazni ) m_spinGod.SetPos(iGod - m_iGodBazni +1);
else m_spinGod.SetPos(1);

UpdateData(TRUE);
UpdateData(FALSE);


m_iGod=m_iGodBazni-1+m_iBrGod;

m_sIzracunatoVrijeme=_T("");

UpdateData(FALSE);


GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);  
GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);

GetDlgItem(IDOK)->ShowWindow(SW_HIDE);  
GetDlgItem(IDOK)->EnableWindow(FALSE);

UpdateData(TRUE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//***************************************************************************************
void CGodisnji::Postavi_Bazni_Datum(int d, int m, int g, int s, int mn,  CString i, float du, float si, int v, int gm)
{

	m_iDanBazni=d;
	m_iMjeBazni=m;
	m_iGodBazni=g;
	
	m_iSatBazni=s;
	m_iMinBazni=mn;

	m_sIme=i;

	m_sIme.TrimRight();



	m_fDuzBazni = du;
	m_fSirBazni = si;
	m_iVriBazni = v;
	m_iGmtBazni = gm;

}
//*************************************************************************************
void CGodisnji::OnChangeRbrgodine() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_iGod=m_iGodBazni-1+m_iBrGod;
	UpdateData(FALSE);

}

void CGodisnji::Pronadji_Vrijeme()
{

double pi=3.1415927;
int ii,pl[12]={31,28,31,30,31,30,31,31,30,31,30,31};  

float duz,sir;

int danx,mjex,godx,satx,minx;

danx=m_iDanBazni;
mjex=m_iMjeBazni;
godx=m_iGodBazni;



	if (fmod2(godx,4)==0 && fmod2(godx,100)!=0 ) pl[1]=29;  //prestupne godine
	if (fmod2(godx,400)==0) pl[1]=29;


satx= m_iSatBazni-m_iVriBazni;
minx= m_iMinBazni;


duz = m_fDuzBazni;
sir = m_fSirBazni;


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



Planete(danx,mjex,godx,satx,minx);



//CString buff;  buff.Format("%f",objx[1]);MessageBox(buff);

//raèunanje vremena:
godx=m_iGod;


double sunold=10000,  sun, sun0 = objx[1];
int ko,pas;

satx=12;minx=0; pas=0;ko=10;

do {

Planete(danx,mjex,godx,satx,minx);

if(pas==3) break;

sun=objx[1];

if(pas==0) {
    pas=1;
    if(sun>sun0)  ko=-ko;
goto jkj;
}


if( fabs2(sun-sun0) > fabs2(sunold-sun0)   ) {
    pas++;
    ko=-ko;
    ko=ko/fabs2(ko);
}

jkj:
sunold=sun;
minx+=ko;

if (minx <0) {
	
	minx =60 + minx;
	satx -=1;

	if (satx <0) {

		satx =23;
		danx -=1;

		if(danx<1) {

			mjex-=1;

			if(mjex<1) {

				mjex=12;
				godx-=1;	
			}
		
		danx=pl[mjex-1];	

		}
	}

}


if (minx >59) {
	
	minx =minx-60;
	satx +=1;

	if (satx >23) {

		satx =0;
		danx+=1;

		if(danx>pl[mjex-1]) {

			mjex+=1;

			if(mjex>12) {

				mjex=1;
				godx+=1;	
			}
		
			danx=1;	
		}
	}
}


} while(1);





satx+= m_iVriBazni;

if (satx >23) {
	satx =satx-24;
		danx+=1;

		if(danx>pl[mjex-1]) {

			mjex+=1;

			if(mjex>12) {

				mjex=1;
				godx+=1;	
			}
		
		    danx=1;	
		}

}



m_iDanBazni=danx;
m_iMjeBazni=mjex;
m_iGodBazni=godx;

m_iSatBazni=satx;
m_iMinBazni=minx;


m_sIzracunatoVrijeme.Format("Traženi položaj sunca      : %s\r\nPronaðeni položaj sunca : %s\r\n-----------------------------------------------------------------------\r\nIzraèunato vrijeme :  %02d.%02d.%d, %02d:%02d", 
							 deg2dms(sun0,0), deg2dms(sun,0),danx,mjex,godx,satx,minx);

if(m_iVriBazni!=0) m_sIzracunatoVrijeme+=" (DST)";

UpdateData(FALSE);


}


//*********************************************
double CGodisnji::frac(double x)
{
return x-floor(x);

}
//**********************************************
double CGodisnji::fnmo(double x)
{
return(x-floor(x/360)*360);
}

void CGodisnji::OnButton1() 
{
	// TODO: Add your control notification handler code here
	Pronadji_Vrijeme();

	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);  
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);

	GetDlgItem(IDOK)->ShowWindow(SW_SHOW);  
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	
}

//***************************************************************************
CString CGodisnji::deg2dms(float deg, int mod) {

	CString Pokaz,predznak;

	float minute,sekunde;

	predznak="";
	if (mod==2)  predznak="W";
	if (mod==3)  predznak="N";

	if (deg<0) { 
		 if(mod==0 || mod==1) predznak="-";
	     if (mod==2)  predznak="E";
	     if (mod==3)  predznak="S";
	     deg=-deg;
	   }


	minute=(deg-floor(deg))*60;

	sekunde=(minute-floor(minute))*60;

	if (mod==0) Pokaz.Format("%03d%c %02d' %02d\"",(int)deg,176,(int)minute, (int)sekunde);  //stupnjevi
	if (mod==1) Pokaz.Format("%02dh %02d' %02d\"",(int)deg,(int)minute, (int)sekunde);       //sati
	if (mod==2) Pokaz.Format("%03d%c %02d' %02d\"",(int)deg,176,(int)minute, (int)sekunde);       //meridijan
	if (mod==3) Pokaz.Format("%03d%c %02d' %02d\"",(int)deg,176,(int)minute, (int)sekunde);       //paralela


	if(mod==0 || mod==1) Pokaz= predznak + Pokaz;
	else Pokaz = Pokaz + " " + predznak;

	return Pokaz;


}


//**************************novi naèin raèunanja *********************************


//***********************************************************************
void CGodisnji::Planete(int dan, int mje, int god, int sat, int min)
{
 double D[13],X[13],Y[13],Z[13],K[13],C[13],M[13];
 int br=0,rvs,ix; //brojac podataka
 double a,b,d,e,f,g,g1,ii,l,ll,l1,m,n,r,s,s1,s2,t,u,v,x,y,z,au,ea,fl,k1,qq=3,tn,td,ml,jd,Gvr;
 int i,j;
 double pi=3.1415927;
 double gmt=m_iGmtBazni;

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
  a=180/pi*(a+m); if(fabs2(v-a)>10) a-=180;
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

//************************************************
double CGodisnji::sgn(double x)
{
 if(x<0) return (-1);
 if(x==0) return (0);
 return (1);

}
//**************************************************
void CGodisnji::readdata(int *i1, double *f1)
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
void CGodisnji::ln1060(double xx, double *yy, double *rr, double *aa)
{
double pi=3.1415927;

if(*yy==0) *yy=1.745e-9;
*rr=sqrt(xx*xx+(*yy)*(*yy)); *aa=atan(*yy/xx); if(*aa<0) (*aa)+=pi; if(*yy<0) (*aa)+=pi;

}
//************************************
double CGodisnji::fns(double x)
{
double pi=3.1415927;
return(sin(x*pi/180)); 

}
//************************************
double CGodisnji::fnp(double x, double mm)
{
return( sgn(x)* ( (fabs(x)/mm)/360.-floor((fabs(x)/mm)/360))*360);
}

//*******************************************************************
int CGodisnji::fmod2(double a, double b)
{
return  fmod(a,b);
}

//*******************************************************************
double CGodisnji::fabs2(double a)
{
return  fabs(a);
}

//*******************************************************************
double CGodisnji::floor2(double a)
{
return  floor(a);
}


