// dSunSustav.cpp : implementation file
//

#include "stdafx.h"
#include "asc2win.h"
#include "dSunSustav.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dSunSustav dialog


dSunSustav::dSunSustav(CWnd* pParent /*=NULL*/)
	: CDialog(dSunSustav::IDD, pParent)
{
	//{{AFX_DATA_INIT(dSunSustav)
	m_iSvePlanete = 1;
	m_bIsActive=FALSE;
	m_iMj = 0;
	//}}AFX_DATA_INIT
}


void dSunSustav::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dSunSustav)
	DDX_Radio(pDX, IDC_SUS1, m_iSvePlanete);
	DDX_Radio(pDX, IDC_MJ1, m_iMj);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dSunSustav, CDialog)
	//{{AFX_MSG_MAP(dSunSustav)
	ON_BN_CLICKED(IDC_SUS2, OnSus1)
	ON_BN_CLICKED(IDC_MJ1,  OnSus2)
	ON_BN_CLICKED(IDC_SUS1, OnSus1)
	ON_BN_CLICKED(IDC_MJ2,  OnSus2)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dSunSustav message handlers

void dSunSustav::OnSus1() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	
	GetParent()->Invalidate();

	
		
}
void dSunSustav::OnSus2() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	
	GetParent()->Invalidate();

	
		
}


void dSunSustav::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	GetParent()->Invalidate();
	
}


