// Aspekti.cpp : implementation file
//

#include "stdafx.h"
#include "asc2win.h"
#include "Aspekti.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAspekti dialog


CAspekti::CAspekti(CWnd* pParent /*=NULL*/, CDC * pDCAsp)
	: CDialog(CAspekti::IDD, pParent)
{
	pdAsp = pDCAsp;

	m_bPaste = TRUE;

	m_bIsActive=FALSE;

	//{{AFX_DATA_INIT(CAspekti)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAspekti::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAspekti)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAspekti, CDialog)
	//{{AFX_MSG_MAP(CAspekti)
	ON_WM_PAINT()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAspekti message handlers


void CAspekti::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages

if (m_bPaste==TRUE) {
	dc.SelectObject(&bmSlika);
	dc.BitBlt(0,0, 455, 250, pdAsp, 0, 0, SRCCOPY  );
	m_bPaste=FALSE;
}


}

//********************************************************************
BOOL CAspekti::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	 //CDC * pDC=this->GetDC();

	 bmSlika.CreateCompatibleBitmap (pdAsp, 455,150);  

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAspekti::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
	GetWindowRect(&m_rSlika);
	GetParent()->Invalidate();

}


