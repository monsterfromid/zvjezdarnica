// Tijelo.cpp : implementation file
//

#include "stdafx.h"
#include "asc2win.h"
#include "Tijelo.h"
#include "UlazDlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTijelo dialog


CTijelo::CTijelo(CWnd* pParent /*=NULL*/, CUlazDlg * pUlazi)
	: CDialog(CTijelo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTijelo)
	m_bAspDobri = TRUE;
	m_AspLosi = TRUE;
	m_bObj1 = TRUE;
	m_bObj2 = TRUE;
	m_bObj3 = TRUE;
	m_bObj4 = TRUE;
	m_bObj5 = TRUE;
	m_bObj6 = TRUE;
	m_bObj7 = TRUE;
	m_bObj8 = TRUE;
	m_bObj9 = TRUE;
	m_bObj10 = TRUE;
	m_bObj11 = TRUE;
	m_bSvi = TRUE;
	m_bSviAsp = TRUE;
	m_iAspObj = 0;
	m_bIsActive = FALSE;
	m_bDST = FALSE;
	//}}AFX_DATA_INIT

	m_pUlazi = pUlazi;
}


void CTijelo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTijelo)
	DDX_Check(pDX, IDC_CHECKA1, m_bAspDobri);
	DDX_Check(pDX, IDC_CHECKA2, m_AspLosi);
	DDX_Check(pDX, IDC_CHECK1, m_bObj1);
	DDX_Check(pDX, IDC_CHECK2, m_bObj2);
	DDX_Check(pDX, IDC_CHECK3, m_bObj3);
	DDX_Check(pDX, IDC_CHECK4, m_bObj4);
	DDX_Check(pDX, IDC_CHECK5, m_bObj5);
	DDX_Check(pDX, IDC_CHECK6, m_bObj6);
	DDX_Check(pDX, IDC_CHECK7, m_bObj7);
	DDX_Check(pDX, IDC_CHECK8, m_bObj8);
	DDX_Check(pDX, IDC_CHECK9, m_bObj9);
	DDX_Check(pDX, IDC_CHECK10, m_bObj10);
	DDX_Check(pDX, IDC_CHECK11, m_bObj11);
	DDX_Check(pDX, IDC_CHECKSVI, m_bSvi);
	DDX_Check(pDX, IDC_CHECKSVIA, m_bSviAsp);
	DDX_Radio(pDX, IDC_RADIO1, m_iAspObj);
	DDX_Check(pDX, IDC_CHK_DST, m_bDST);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTijelo, CDialog)
	//{{AFX_MSG_MAP(CTijelo)
	ON_BN_CLICKED(IDC_CHECKA1, OnCheck)
	ON_BN_CLICKED(IDC_CHECKSVI, OnChecksvi)
	ON_BN_CLICKED(IDC_CHECKSVIA, OnChecksvia)
	ON_BN_CLICKED(IDC_CHECKA2, OnCheck)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck)
	ON_BN_CLICKED(IDC_RADIO1, OnCheck)
	ON_BN_CLICKED(IDC_RADIO7, OnCheck)
	ON_BN_CLICKED(IDC_RADIO8, OnCheck)
	ON_BN_CLICKED(IDC_RADIO9, OnCheck)
	ON_BN_CLICKED(IDC_RADIO10, OnCheck)
	ON_BN_CLICKED(IDC_RADIO11, OnCheck)
	ON_BN_CLICKED(IDC_RADIO12, OnCheck)
	ON_BN_CLICKED(IDC_RADIO13, OnCheck)
	ON_BN_CLICKED(IDC_RADIO14, OnCheck)
	ON_BN_CLICKED(IDC_RADIO15, OnCheck)
	ON_BN_CLICKED(IDC_RADIO16, OnCheck)
	ON_BN_CLICKED(IDC_RADIO17, OnCheck)
	ON_BN_CLICKED(IDC_CHK_DST, OnChkDst)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTijelo message handlers

void CTijelo::OnCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	GetParent()->Invalidate();

}

void CTijelo::OnChecksvi() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	if (m_bSvi==TRUE) {
		m_bObj1=TRUE;
		m_bObj2=TRUE;
		m_bObj3=TRUE;
		m_bObj4=TRUE;
		m_bObj5=TRUE;
		m_bObj6=TRUE;
		m_bObj7=TRUE;
		m_bObj8=TRUE;
		m_bObj9=TRUE;
		m_bObj10=TRUE;
		m_bObj11=TRUE;
	}

	else {
		m_bObj1=FALSE;
		m_bObj2=FALSE;
		m_bObj3=FALSE;
		m_bObj4=FALSE;
		m_bObj5=FALSE;
		m_bObj6=FALSE;
		m_bObj7=FALSE;
		m_bObj8=FALSE;
		m_bObj9=FALSE;
		m_bObj10=FALSE;
		m_bObj11=FALSE;

	}
	UpdateData(FALSE);
	
	GetParent()->Invalidate();

	
	
}

void CTijelo::OnChecksvia() 
{

	UpdateData(TRUE);

	if (m_bSviAsp==TRUE) {

		m_AspLosi=TRUE;
		m_bAspDobri=TRUE;
	}

	else {
		m_AspLosi=FALSE;
		m_bAspDobri=FALSE;
	}
	UpdateData(FALSE);
	
	GetParent()->Invalidate();

	
}





void CTijelo::OnChkDst() 
{

	UpdateData(TRUE);

	if (m_bDST==true) m_pUlazi->m_iVri=1;
	else			  m_pUlazi->m_iVri=0;
	
	
	GetParent()->Invalidate();
	
}
