
// Holter_vs2015Dlg.cpp : implementation file
//


#include "Holter_vs2015.h"
#include "Holter_vs2015Dlg.h"
#include "afxdialogex.h"
#include "curlapi.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//store http authentication token


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


// CHolter_vs2015Dlg dialog



CHolter_vs2015Dlg::CHolter_vs2015Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HOLTER_VS2015_DIALOG, pParent)
	, m_uid(_T(""))
	, m_password(_T(""))
	, m_fpath(_T(""))
	, m_dlurl(_T(""))
	, curl_imp()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHolter_vs2015Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_uid);
	DDV_MaxChars(pDX, m_uid, 100);
	DDX_Text(pDX, IDC_EDIT2, m_password);
	DDV_MaxChars(pDX, m_password, 100);
	DDX_Text(pDX, IDC_EDIT3, m_fpath);
	DDV_MaxChars(pDX, m_fpath, 100);
	DDX_Text(pDX, IDC_EDIT4, m_dlurl);
	DDV_MaxChars(pDX, m_dlurl, 100);
}

BEGIN_MESSAGE_MAP(CHolter_vs2015Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CHolter_vs2015Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CHolter_vs2015Dlg::OnClickedupload)
	ON_BN_CLICKED(IDC_BUTTON2, &CHolter_vs2015Dlg::OnClickedDownLoad)
	ON_BN_CLICKED(IDC_BUTTON3, &CHolter_vs2015Dlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CHolter_vs2015Dlg message handlers

BOOL CHolter_vs2015Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHolter_vs2015Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHolter_vs2015Dlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHolter_vs2015Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//upload
void CHolter_vs2015Dlg::OnClickedupload()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//get upload list from db
	//send json head and zip file one by one
	//

	//CString json_buf=info2json();
	//postjson("http://172.104.78.181/case/postcase.do", json_buf, 0, 3);
	//CString ftphost = "ftp://172.104.78.181:2121";
	//CString ulurl = ftphost + "/" + m_uid + "/" + "save005.rar";
	//upload(ulurl, m_fpath, 0, 3); 
}


//download
void CHolter_vs2015Dlg::OnClickedDownLoad()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//download("ftp://172.104.78.181:2121/DEEP.iso", "f:\\DEEP.iso", 0, 3);
	//download(m_dlurl, "f:\\DEEP.iso", 0, 3);
}

//refresh
void CHolter_vs2015Dlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	//refresh_remotelist("f:\\token.txt","f:\\remotelist.json");
	//refresh_remotelist();
}

//login
void CHolter_vs2015Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	curl_imp.login(m_uid, m_password);
}