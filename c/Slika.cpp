// Slika.cpp : implementation file
//
//#import "C:\Program Files\Microsoft Visual Studio\Vfp98\Distrib.src\System\OLEPRO32.DLL"


#include "stdafx.h"
#include "asc2win.h"
#include "Slika.h"
#include "olectl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSlika dialog


CSlika::CSlika(CWnd* pParent /*=NULL*/)
	: CDialog(CSlika::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSlika)
	//}}AFX_DATA_INIT

	m_rSlika.left=0;

}


void CSlika::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSlika)
	DDX_Control(pDX, IDC_PIC, m_cPic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSlika, CDialog)
	//{{AFX_MSG_MAP(CSlika)
	ON_WM_MOVE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlika message handlers

boolean CSlika::Ucitaj_sliku(CString sFajl, CString sTip)
{

		
	HBITMAP hBitmap ;
	CString sf1;
	char buff[100];

	sf1= sFajl + "." + sTip;

	strncpy(&buff[0],sf1,sf1.GetLength() );buff[sf1.GetLength()]=0;

	//MessageBox(buff);


	try {
		sTip.MakeLower();
		if(sTip.Compare("bmp")== 0) {

			FILE *fajl = fopen(buff,"rb");	if (!fajl) return FALSE;
			fclose(fajl);

			///*
			hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sf1, IMAGE_BITMAP, 0, 0, 
				                           LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			//*/
		}
		else {



		hBitmap = LoadAnImage(buff);

		if (hBitmap== NULL) return FALSE;

		}
	}
	catch (CException* e){
		return FALSE;
		
	}
	

	// Attach the loaded image to the CBitmap object.
	CBitmap bmTmp;

	bmTmp.Attach(hBitmap);  

	
  //bmTmp.LoadBitmap(IDB_NUM);   //ako se želi izravno resource

   BITMAP bm;
	 //bmTmp.GetObject( sizeof( BITMAP ), &bm );  // get source pic dimensions
	 bmTmp.GetBitmap( &bm );  // jednostavnija verzija



   //Get the Display area available (pozadinski nevidljivi frame)
    CRect lRect;
    GetDlgItem(IDC_FRM)->GetWindowRect(&lRect);
	 int dx=lRect.Width(), dy=lRect.Height();

	// resizing
	//CDC * pDC=GetDC();
	 CDC * pDC = GetDlgItem(IDC_PIC)->GetDC(); 

	 CDC dcMem1, dcMem2;
    dcMem1.CreateCompatibleDC(pDC);
    dcMem2.CreateCompatibleDC(pDC);
   
    //select the original bitmap into compatible device context
    CBitmap* pbmOld1 = (CBitmap*)dcMem1.SelectObject(&bmTmp);
 
    //resize 
	 m_bmPic.CreateCompatibleBitmap (&dcMem1, dx,dy);  // prihvatni cdc, važno je da se stavi originalni DC, a ne ciljni!!
	                                                      // (inaèe ne napravi toènu paletu nego monokromni bitmap)


	 int nOldStretchMode = dcMem2.SetStretchBltMode(HALFTONE);  

    CBitmap* pbmOld2 = (CBitmap*)dcMem2.SelectObject(&m_bmPic);  //selektiraj u prihvatni cdc prihvatni prazni bitmap objekt

    
	dcMem2.FloodFill(15,15,RGB(238, 232, 205 ));
	dcMem2.StretchBlt(0,0, (int)(dy*bm.bmWidth/bm.bmHeight),dy,&dcMem1,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);  //resizaj i kopiraj u ciljni bmp

 
	//pridruži picture objektu dijaloga
	m_cPic.SetBitmap(m_bmPic);


	//korigiraj velièinu dijaloga
	CRect wrect;
	this->GetWindowRect(&wrect);


	m_rSlika.left = m_rSlika.right- ((int)( wrect.Height() * bm.bmWidth/bm.bmHeight ));
	m_rSlika.top =  m_rSlika.bottom - wrect.Height();






	this->SetWindowPos(&CWnd::wndTop, m_rSlika.left, 	m_rSlika.top,
									 (int)( wrect.Height() * bm.bmWidth/bm.bmHeight ), wrect.Height(),
									 SWP_NOMOVE );
	

   //poèisti za sobom
	 dcMem2.SetStretchBltMode(nOldStretchMode);   //vrati natrag mod

	 dcMem1.SelectObject(pbmOld1);  //select old bitmaps
	 dcMem2.SelectObject(pbmOld2);
	 
	 bmTmp.Detach();             //detach bitmapove sa slikama
	 m_bmPic.Detach();
	 
	 DeleteObject(bmTmp);        // delete bmpove  u dcu
	 DeleteObject(m_bmPic);
	 
	 ReleaseDC(&dcMem1);   // oèisti memory dc-s
	 ReleaseDC(&dcMem2);

	 //delete bmTmp;


	Invalidate();

	 return TRUE;
}


//*************************************************************************************************
HBITMAP CSlika::LoadAnImage(char *FileName)
{

   // Use IPicture stuff to use JPG / GIF files
   IPicture* p;
   IStream* s;
   IPersistStream* ps;
   HGLOBAL hG;
   void* pp;
   FILE* fp;


   // Read file in memory
   fp = fopen(FileName,"rb");
   if (!fp)
   return NULL;

   fseek(fp,0,SEEK_END);
   int fs = ftell(fp);
   fseek(fp,0,SEEK_SET);
   hG = GlobalAlloc(GPTR,fs);
   if (!hG)
      {
      fclose(fp);
      return NULL;
      }
   pp = (void*)hG;
   fread(pp,1,fs,fp);
   fclose(fp);

   // Create an IStream so IPicture can
   CreateStreamOnHGlobal(hG,false,&s);
   if (!s)
      {
      GlobalFree(hG);
      return NULL;
      }

   OleLoadPicture(s,0,false,IID_IPicture,(void**)&p);

   if (!p)
      {
      s->Release();
      GlobalFree(hG);
      return NULL;
      }

   s->Release();
   GlobalFree(hG);


   HBITMAP hB = 0;
   p->get_Handle((unsigned int*)&hB);

   // Copy the image. Necessary, because upon p's release,
   // the handle is destroyed.
   HBITMAP hBB = (HBITMAP)CopyImage(hB,IMAGE_BITMAP,0,0,
                                    LR_COPYRETURNORG);

   p->Release();
   return hBB;

}

void CSlika::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here

	GetWindowRect(&m_rSlika);
	
}


void CSlika::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	GetParent()->Invalidate();
	CDialog::OnClose();
}
