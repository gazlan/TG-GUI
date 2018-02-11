// TGDlg.h : header file
//

#if !defined(AFX_TGDLG_H__CB4F80C5_EBF1_4F95_A560_E40106B5A2C9__INCLUDED_)
#define AFX_TGDLG_H__CB4F80C5_EBF1_4F95_A560_E40106B5A2C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTGDlg dialog

class CTGDlg : public CDialog
{
// Construction
public:
	CTGDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTGDlg)
	enum { IDD = IDD_TG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTGDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTGDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
   afx_msg void OnDropFiles(HDROP hDropInfo); // For Drag-n-Drop
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TGDLG_H__CB4F80C5_EBF1_4F95_A560_E40106B5A2C9__INCLUDED_)
