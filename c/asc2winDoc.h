// asc2winDoc.h : interface of the CAsc2winDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASC2WINDOC_H__EF54F266_ABBF_4F2C_9CB1_8BE70DCA51DE__INCLUDED_)
#define AFX_ASC2WINDOC_H__EF54F266_ABBF_4F2C_9CB1_8BE70DCA51DE__INCLUDED_

#include "UlazDlg1.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAsc2winDoc : public CDocument
{
protected: // create from serialization only
	CAsc2winDoc();
	DECLARE_DYNCREATE(CAsc2winDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAsc2winDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAsc2winDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAsc2winDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASC2WINDOC_H__EF54F266_ABBF_4F2C_9CB1_8BE70DCA51DE__INCLUDED_)
