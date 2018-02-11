/* ******************************************************************** **
** @@ Title Src File
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : Create/Replace HTML <title> tag with first occurrent <h1>content</h1>
** ******************************************************************** */

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include "..\shared\mmf.h"
#include "..\shared\text.h"
#include "..\shared\vector.h"
#include "..\shared\vector_sorted.h"
#include "..\shared\file.h"
#include "..\shared\file_walker.h"
#include "..\shared\search_bmh.h"

#include "TG.h"
#include "TGDlg.h"

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef NDEBUG
#pragma optimize("gsy",on)
#pragma comment(linker,"/FILEALIGN:512 /MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR /IGNORE:4078")
#endif

#define BEGIN_OF_TITLE        "<title>"
#define END_OF_TITLE          "</title>"

#define END_OF_HEAD           "</head>"
#define EOL_MARKER            "\r\n"

#define BEGIN_OF_HEADER1      "<h1>"
#define END_OF_HEADER1        "</h1>"

struct TANDEM
{
   int   _iStart;
   int   _iFinish;
};

/* ******************************************************************** **
** @@                   internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@                   external global variables
** ******************************************************************** */

extern DWORD   dwKeepError = 0;

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */
                  
static MMF        MF;
                  
static TANDEM     _Title;
static TANDEM     _Head;
static TANDEM     _Header1;

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ ForEach()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void ForEach(const char* const pszFileName)
{
   memset(&_Title,  0,sizeof(TANDEM));
   memset(&_Head,   0,sizeof(TANDEM));
   memset(&_Header1,0,sizeof(TANDEM));

   char     pszBackName[_MAX_PATH];
   char     pszDrive   [_MAX_DRIVE];
   char     pszDir     [_MAX_DIR];
   char     pszFName   [_MAX_FNAME];
   char     pszExt     [_MAX_EXT];

   _splitpath(pszFileName,pszDrive,pszDir,pszFName,pszExt);
   _makepath( pszBackName,pszDrive,pszDir,pszFName,"BAK");

   CopyFile(pszFileName,pszBackName,FALSE);

   HANDLE   hFile = CreateFile(pszFileName,CREATE_ALWAYS,0);

   VERIFY(hFile != INVALID_HANDLE_VALUE);

   if (hFile == INVALID_HANDLE_VALUE)
   {
      // Error !
      CloseHandle(hFile);
      hFile = INVALID_HANDLE_VALUE;
      return;
   }

   if (!MF.OpenReadOnly(pszBackName))
   {
      // Error !
      CloseHandle(hFile);
      hFile = INVALID_HANDLE_VALUE;
      return;
   }

   BYTE*    pText  = MF.Buffer();
   DWORD    dwSize = MF.Size();

   bool     bTitlePresent = false;
   bool     bHdrPresent   = false;

   // Title
   _Title._iStart = BMH_ISearch(pText,dwSize,(BYTE*)BEGIN_OF_TITLE,sizeof(BEGIN_OF_TITLE) - 1);

   if (_Title._iStart == -1)
   {
//      printf(">> %s:\n[*] Warning: No <title> HTML tag found in the file.\n\n",pszFileName);
   }

   if (_Title._iStart != -1)
   {
      _Title._iFinish = BMH_ISearch(pText,dwSize,(BYTE*)END_OF_TITLE,sizeof(END_OF_TITLE) - 1);

      bTitlePresent = _Title._iFinish > _Title._iStart  ?  true  :  false;

      if (_Title._iFinish < _Title._iStart)
      {
/*
         printf(">> %s:\n[!] Error: Incorrect HTML file.\nOccurrent </title> tag before <title> tag.\n",pszFileName);
         printf("</title> tag position %08X\n",_Title._iFinish);
         printf("<title> tag position  %08X\n\n",_Title._iStart);
*/
      }
   }

   // End of Head
   _Head._iFinish = BMH_ISearch(pText,dwSize,(BYTE*)END_OF_HEAD,sizeof(END_OF_HEAD) - 1);

   if (_Head._iFinish == -1)
   {
      // Error !
      CloseHandle(hFile);
      hFile = INVALID_HANDLE_VALUE;
      MF.Close();
//      printf(">> %s:\n[!] Error: Incorrect HTML file.\nNo </head> tag found in the file. Aborted.\n\n",pszFileName);
      return;
   }

   // Header1
   _Header1._iStart = BMH_ISearch(pText,dwSize,(BYTE*)BEGIN_OF_HEADER1,sizeof(BEGIN_OF_HEADER1) - 1);

   if (_Header1._iStart == -1)
   {
//      printf(">> %s:\n[*] Warning: No <H1> HTML tag found in the file.\n\n",pszFileName);
   }

   if (_Header1._iStart != -1)
   {
      _Header1._iFinish = BMH_ISearch(pText,dwSize,(BYTE*)END_OF_HEADER1,sizeof(END_OF_HEADER1) - 1);

      bHdrPresent = _Header1._iFinish > _Header1._iStart  ?  true  :  false;
     
      if (_Header1._iFinish < _Header1._iStart)
      {
/*
         printf(">> %s:\n[!] Error: Incorrect HTML file.\nOccurrent </H1> tag before <H1> tag.\n",pszFileName);
         printf("</H1> tag position %08X\n",_Header1._iFinish);
         printf("<H1> tag position  %08X\n\n",_Header1._iStart);
*/
      }
   }

   char     pszHeader[MAX_PATH + 1];

   bool     bHeader = false;

   int      iHdrLen = 0;

   if (bHdrPresent)
   {
      iHdrLen = _Header1._iFinish - (_Header1._iStart + sizeof(BEGIN_OF_HEADER1) - 1);

      memcpy(pszHeader,pText + _Header1._iStart + sizeof(BEGIN_OF_HEADER1) - 1,iHdrLen);

      pszHeader[iHdrLen] = 0;  // ASCIIZ

      Jammer(pszHeader,iHdrLen);

      pszHeader[iHdrLen] = 0; // ASCIIZ

      bHeader = iHdrLen > 0  ?  true  :  false;
   }

   if (bTitlePresent && bHdrPresent)
   {
      // Skip Title
      WriteBuffer(hFile,pText,_Title._iStart);
      WriteBuffer(hFile,pText + _Title._iFinish + sizeof(END_OF_TITLE) - 1,_Head._iFinish - (_Title._iFinish + sizeof(END_OF_TITLE) - 1));
   }
   else
   {
      WriteBuffer(hFile,pText,_Head._iFinish);
   }
          
   if (bHeader)
   {
      WriteBuffer(hFile,EOL_MARKER,sizeof(EOL_MARKER) - 1);
      WriteBuffer(hFile,BEGIN_OF_TITLE,sizeof(BEGIN_OF_TITLE) - 1);
      WriteBuffer(hFile,pszHeader,iHdrLen);
      WriteBuffer(hFile,END_OF_TITLE,sizeof(END_OF_TITLE) - 1);
      WriteBuffer(hFile,EOL_MARKER,sizeof(EOL_MARKER) - 1);
   }
   
   WriteBuffer(hFile,pText + _Head._iFinish,dwSize - _Head._iFinish);

   MF.Close();

   CloseHandle(hFile);
   hFile = INVALID_HANDLE_VALUE;
}

CTGDlg::CTGDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CTGDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CTGDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
   // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
   m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTGDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTGDlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTGDlg, CDialog)
   //{{AFX_MSG_MAP(CTGDlg)
   ON_WM_PAINT()
   ON_WM_QUERYDRAGICON()
   ON_WM_DROPFILES() // Drag-n-Drop message map entry
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTGDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   // Set the icon for this dialog.  The framework does this automatically
   //  when the application's main window is not a dialog
   SetIcon(m_hIcon, TRUE);       // Set big icon
   SetIcon(m_hIcon, FALSE);      // Set small icon
   
   // TODO: Add extra initialization here
   
   return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTGDlg::OnPaint() 
{
   if (IsIconic())
   {
      CPaintDC dc(this); // device context for painting

      SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
      CDialog::OnPaint();
   }
}

HCURSOR CTGDlg::OnQueryDragIcon()
{
   return (HCURSOR) m_hIcon;
}

void CTGDlg::OnDropFiles(HDROP hDrop) 
{
   CWaitCursor    Waiter;

   DWORD    dwObjectCnt = DragQueryFile(hDrop,0xFFFFFFFF,NULL,0);

   char     pszFilename[MAX_PATH + 1];

   for (DWORD ii = 0; ii < dwObjectCnt; ++ii)
   {
      DWORD    dwSize = DragQueryFile(hDrop,ii,pszFilename,MAX_PATH);

      ForEach(pszFilename);
   }
}
