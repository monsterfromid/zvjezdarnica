// BazniZapis.h: interface for the CBazniZapis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAZNIZAPIS_H__F2663BFA_9D5E_41E9_BF9C_064E05BEBE26__INCLUDED_)
#define AFX_BAZNIZAPIS_H__F2663BFA_9D5E_41E9_BF9C_064E05BEBE26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBazniZapis //: public CObject  
{
public:
	CBazniZapis();
	virtual ~CBazniZapis();
	
	CString kod;
	CString ime;
	int dan;
	int mje;
	int god;
	int sat;
	int min;
	float duz;
	float sir;
	int gmt;
	char kuce;
	char spol;
	float o[12];
	
};

#endif // !defined(AFX_BAZNIZAPIS_H__F2663BFA_9D5E_41E9_BF9C_064E05BEBE26__INCLUDED_)
