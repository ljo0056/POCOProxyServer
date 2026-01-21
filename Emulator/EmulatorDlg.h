#pragma once

#include <memory>

class CPOCOProxyServer;

class EmulatorDlg : public CDialogEx
{
	DECLARE_MESSAGE_MAP()

private:
	HICON			m_hIcon;	
	int				m_front_port = 1470;
	CString			m_back_ip;
	int				m_back_port = 4242;

	std::unique_ptr<CPOCOProxyServer>    m_server;

public:
	EmulatorDlg(CWnd* pParent = nullptr);

	// std::unique_ptr  사용시에 Destroy function 을 정의 및 구현부를 작성해야 한다.
	virtual ~EmulatorDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHROZENEMULATOR_DIALOG };
#endif

	BOOL GetButtonCheck(CWnd* wnd, int id);
	void SetButtonCheck(CWnd* wnd, int id, BOOL value);

	CButton* GetButtonPtr(CWnd* wnd, int id);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnPocoMessage(WPARAM wParam, LPARAM lParam);

	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();	
	
	afx_msg void OnBnClickedFrontActivate();	
};
