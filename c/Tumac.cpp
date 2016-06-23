// Tumac.cpp : implementation file
//

#include "stdafx.h"
#include "asc2win.h"
#include "Tumac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTumac dialog


CTumac::CTumac(CWnd* pParent /*=NULL*/)
	: CDialog(CTumac::IDD, pParent)
{
	m_iOpcijaSpec=0;

	//{{AFX_DATA_INIT(CTumac)
	m_iOpcija = 0;
	m_iPage2 = 1;
	m_iPage = 1;
	m_iMax=10;
	m_sPage = _T("");
	//}}AFX_DATA_INIT
}


void CTumac::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTumac)
	DDX_Control(pDX, IDC_SPIN1, m_spinPage);
	DDX_Radio(pDX, IDC_RADIO1, m_iOpcija);
	DDX_Text(pDX, IDC_PAGE, m_iPage2);
	DDX_Text(pDX, IDC_PAGEDISP, m_sPage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTumac, CDialog)
	//{{AFX_MSG_MAP(CTumac)
	ON_BN_CLICKED(IDC_RADIO1, OnCheck)
	ON_EN_CHANGE(IDC_PAGE, OnChangePage)
	ON_BN_CLICKED(IDC_RADIO2, OnCheck)
	ON_BN_CLICKED(IDC_RADIO3, OnCheck)
	ON_BN_CLICKED(IDC_RADIO4, OnCheck)
	ON_BN_CLICKED(IDC_RADIO5, OnCheck)
	ON_BN_CLICKED(IDC_RADIO6, OnCheck)
	ON_BN_CLICKED(IDC_RADIO7, OnCheck)
	ON_BN_CLICKED(IDC_RADIO8, OnCheck)
	ON_BN_CLICKED(IDC_RADIO9, OnCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTumac message handlers

void CTumac::OnCheck() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	m_spinPage.SetPos(1);
	m_iPage = 1;
	m_iPage2 = 1;
	UpdateData(FALSE);

	GetParent()->Invalidate();

}

BOOL CTumac::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

m_spinPage.SetBuddy(GetDlgItem(IDC_PAGE));
m_spinPage.SetBase (10);
m_spinPage.SetRange(1, 10);  
m_spinPage.SetPos(1);
m_iPage = 1;
m_iPage2 = 1;
UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CTumac::OnChangePage() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	m_iPage = m_spinPage.GetPos();
	UpdateData(TRUE);

	m_sPage.Format("%d / %d", m_iPage, m_iMax);
	UpdateData(FALSE);
	
//	CString bf; bf.Format("%d, %d",m_iPage, m_iPage2); MessageBox(bf);

	GetParent()->Invalidate();


}

void CTumac::UpdatePageDisplay()
{

	m_sPage.Format("%d / %d", m_iPage, m_iMax);
	UpdateData(FALSE);

}
