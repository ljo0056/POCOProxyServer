
// ChroZenEmulatorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Emulator.h"
#include "EmulatorDlg.h"
#include "afxdialogex.h"
#include "CPOCOProxyServer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define POCO_MESSAGE (WM_USER + 1024)

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// EmulatorDlg dialog
#define TIMER_ID_AS_UPDATE	(100)


EmulatorDlg::EmulatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHROZENEMULATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_back_ip = "127.0.0.1";
	m_server = std::make_unique<CPOCOProxyServer>();	
}

EmulatorDlg::~EmulatorDlg()
{

}

BOOL EmulatorDlg::GetButtonCheck(CWnd* wnd, int id)
{
	return GetButtonPtr(wnd, id)->GetCheck() == BST_CHECKED ? TRUE : FALSE;
}

void EmulatorDlg::SetButtonCheck(CWnd* wnd, int id, BOOL value)
{
	GetButtonPtr(wnd, id)->SetCheck(value == TRUE ? BST_CHECKED : BST_UNCHECKED);
}

CButton* EmulatorDlg::GetButtonPtr(CWnd* wnd, int id)
{
	return static_cast<CButton*>(wnd->GetDlgItem(id));
}

void EmulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_FRONT_PORT, m_front_port);
	DDX_Text(pDX, IDC_EDIT_BACK_IP, m_back_ip);
	DDX_Text(pDX, IDC_EDIT_BACK_PORT, m_back_port);
}

BEGIN_MESSAGE_MAP(EmulatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()	
	
	ON_BN_CLICKED(IDC_ACTIVATE, &EmulatorDlg::OnBnClickedFrontActivate)	
END_MESSAGE_MAP()

BOOL EmulatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here	
	

	SetTimer(TIMER_ID_AS_UPDATE, 1000, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void EmulatorDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(TIMER_ID_AS_UPDATE);

	m_server->Deactivate();	
}

void EmulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void EmulatorDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case TIMER_ID_AS_UPDATE:
			UpdateData(FALSE);
			break;		
		default:
			break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void EmulatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

LRESULT EmulatorDlg::OnPocoMessage(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR EmulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void EmulatorDlg::OnBnClickedFrontActivate()
{
	bool check = GetButtonCheck(this, IDC_ACTIVATE);
	if (check)
	{
		UpdateData(TRUE);

		auto handler = [](Poco::Net::StreamSocket& front, Poco::Net::StreamSocket& back)
		{
			return new CPOCOProxyByPassHandler(front, back);
		};
		m_server->Activate(m_front_port, std::string(CT2CA(m_back_ip)), m_back_port, handler);
	}
	else
	{
		m_server->Deactivate();		
	}	
}
