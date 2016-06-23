#if !defined(AFX_SLIKA_H__34DB8513_8F18_4BB7_A67B_B019F9CB99DD__INCLUDED_)
#define AFX_SLIKA_H__34DB8513_8F18_4BB7_A67B_B019F9CB99DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Slika.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSlika dialog

class CSlika : public CDialog
{
// Construction
public:
	CRect m_rSlika;
	HBITMAP LoadAnImage(char* FileName);
	boolean Ucitaj_sliku(CString sFajl,CString sTip);
	CSlika(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSlika)
	enum { IDD = IDD_SLIKA };
	CStatic	m_cPic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlika)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSlika)
	afx_msg void OnMove(int x, int y);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBitmap m_bmPic;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIKA_H__34DB8513_8F18_4BB7_A67B_B019F9CB99DD__INCLUDED_)
