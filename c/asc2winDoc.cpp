// asc2winDoc.cpp : implementation of the CAsc2winDoc class
//

#include "stdafx.h"
#include "asc2win.h"

#include "asc2winDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAsc2winDoc

IMPLEMENT_DYNCREATE(CAsc2winDoc, CDocument)

BEGIN_MESSAGE_MAP(CAsc2winDoc, CDocument)
	//{{AFX_MSG_MAP(CAsc2winDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAsc2winDoc construction/destruction

CAsc2winDoc::CAsc2winDoc()
{
	// TODO: add one-time construction code here

}

CAsc2winDoc::~CAsc2winDoc()
{
}

BOOL CAsc2winDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)



	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAsc2winDoc serialization

void CAsc2winDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAsc2winDoc diagnostics

#ifdef _DEBUG
void CAsc2winDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAsc2winDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAsc2winDoc commands
