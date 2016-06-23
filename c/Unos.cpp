// Unos.cpp : implementation file
//

#include "stdafx.h"
#include "asc2win.h"
#include "Unos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnos dialog


CUnos::CUnos(CWnd* pParent /*=NULL*/)
	: CDialog(CUnos::IDD, pParent)
{

	//{{AFX_DATA_INIT(CUnos)
	m_radSpol = 0;
	m_radKucice = 0;
	m_sNaziv = _T("");
	m_bDST = FALSE;
	//}}AFX_DATA_INIT


}


void CUnos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnos)
	DDX_Control(pDX, IDC_SEL_ZAN, m_cbKod);
	DDX_Radio(pDX, IDC_RADIO4, m_radSpol);
	DDX_Radio(pDX, IDC_RADIO1, m_radKucice);
	DDX_Text(pDX, IDC_EDIT1, m_sNaziv);
	DDX_Check(pDX, IDC_DST, m_bDST);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUnos, CDialog)
	//{{AFX_MSG_MAP(CUnos)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_CBN_SELCHANGE(IDC_SEL_ZAN, OnSelchangeSelZan)
	ON_CBN_EDITCHANGE(IDC_SEL_ZAN, OnEditchangeSelZan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnos message handlers

void CUnos::OnOK() 
{
	// TODO: Add extra validation here


	//MessageBox(m_sKod);

	if(m_sNaziv.GetLength()>0 && m_sKod.GetLength()>0 )	CDialog::OnOK();
	return;

}

void CUnos::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here


	UpdateData(TRUE);


	
}

void CUnos::Ucita_Bazu()
{

	FILE *fajl;
	 char buff[220], kd[30];
	 int i,j;
    boolean b;

	 CString tmpKod;
    


	 m_cbKod.ResetContent();


	 fajl=fopen("pocetne_postavke.cfg","rt"); if(fajl==NULL) {MessageBox("nema fajla postavki horoskopa!"); return; }

	fgets(buff,100,fajl);fgets(buff,100,fajl);fgets(buff,100,fajl);fgets(buff,100,fajl);
	fgets(buff,100,fajl); fgets(buff,100,fajl); 
	
	char nazivBaze[100];
	CString sNazivBaze;

	fgets(buff,100,fajl); sscanf(buff,"%s",&nazivBaze[0]); sNazivBaze.Format("%s",nazivBaze);

	fclose(fajl);


	 fajl=fopen(sNazivBaze,"rt");  if (fajl==NULL) { MessageBox("u bazi nema podataka"); return; }

    do {
       fgets(buff,219,fajl); if (feof(fajl)) break;
   	 sscanf(buff,"%s", &kd[0]);
		 tmpKod.Format("%s",kd);
		 
		 b=FALSE;

		for (j=0; j< m_cbKod.GetCount(); j++)	{

			if ( m_cbKod.FindString(0,  tmpKod )  >= 0 ) { b=TRUE; break; }
		}

		if (!b) m_cbKod.AddString( tmpKod );

    } while (!feof(fajl));

 	 fclose(fajl);



	m_cbKod.SetCurSel(m_cbKod.FindString(0,m_sKod));
	UpdateData(TRUE);

	m_cbKod.GetWindowText(m_sKod);  //od parent klase CWnd, naslijeðena metoda!!!! 

}

BOOL CUnos::OnInitDialog()  //ovo ja dodo i radi, samo se inicijalizira
{
	CDialog::OnInitDialog();
	
	//MessageBox("init");

	Ucita_Bazu();

	this->GetDlgItem(IDC_DST)->EnableWindow(m_bNoviZapis);

	return TRUE;
}

void CUnos::OnSelchangeSelZan() 
{
	// TODO: Add your control notification handler code here
	
	int x;
	
	//MessageBox("combo");
	UpdateData(TRUE);

	x = m_cbKod.GetCurSel();
	m_cbKod.SetCurSel(x);
	UpdateData(FALSE);
	
	m_cbKod.GetWindowText(m_sKod);  //od parent klase CWnd, naslijeðena metoda!!!! 



}

void CUnos::OnEditchangeSelZan() 
{
	// TODO: Add your control notification handler code here
	//MessageBox("edit");
	UpdateData(TRUE);
	m_cbKod.GetWindowText(m_sKod);  //od parent klase CWnd, naslijeðena metoda!!!! 
	
}
//************************************************************************************************
void CUnos::SetParams(int iKucice, int iSpol, CString sKod, CString sNaziv, int iVri, boolean bNovi )
{

	m_radKucice = iKucice;
	m_radSpol = iSpol;
	m_sKod = sKod;
	m_sNaziv = sNaziv;

	m_bDST=FALSE; if (iVri == 1 ) m_bDST=TRUE;

	m_bNoviZapis = bNovi;

}
