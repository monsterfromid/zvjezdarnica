#if !defined(AFX_UNOS_H__B1F19AF4_3A75_48B4_AE02_A3730B8CE3D7__INCLUDED_)
#define AFX_UNOS_H__B1F19AF4_3A75_48B4_AE02_A3730B8CE3D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Unos.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUnos dialog

class CUnos : public CDialog
{
// Construction
public:
	boolean m_bNoviZapis;
	void SetParams(int iKucice, int iSpol, CString sKod, CString sNaziv, int iVri, boolean bNovi );
	CString m_sKod;
	CUnos(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUnos)
	enum { IDD = IDD_UNOS };
	CComboBox	m_cbKod;
	int		m_radSpol;
	int		m_radKucice;
	CString	m_sNaziv;
	BOOL	m_bDST;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnos)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CUnos)
	virtual void OnOK();
	afx_msg void OnChangeEdit1();
	afx_msg void OnSelchangeSelZan();
	afx_msg void OnEditchangeSelZan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Ucita_Bazu();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNOS_H__B1F19AF4_3A75_48B4_AE02_A3730B8CE3D7__INCLUDED_)
