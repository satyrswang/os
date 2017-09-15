; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWinManageFileDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WinManageFile.h"

ClassCount=9
Class1=CWinManageFileApp
Class2=CWinManageFileDlg
Class3=CAboutDlg

ResourceCount=8
Resource1=IDD_FILESYSTEM
Resource2=IDR_MAINFRAME
Resource3=IDD_DIRNAME
Class4=CDisplayView
Class5=CFileSystemDlg
Resource4=IDD_DELETEFILE
Class6=CDirNameDlg
Resource5=IDD_ABOUTBOX
Class7=CFileDlg
Resource6=IDD_FILENAME
Class8=CDeleteFileDlg
Resource7=IDD_WINMANAGEFILE_DIALOG
Class9=CWriteFileDlg
Resource8=IDD_WRITEFILE

[CLS:CWinManageFileApp]
Type=0
HeaderFile=WinManageFile.h
ImplementationFile=WinManageFile.cpp
Filter=N

[CLS:CWinManageFileDlg]
Type=0
HeaderFile=WinManageFileDlg.h
ImplementationFile=WinManageFileDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_COMBODIR

[CLS:CAboutDlg]
Type=0
HeaderFile=WinManageFileDlg.h
ImplementationFile=WinManageFileDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_WINMANAGEFILE_DIALOG]
Type=1
Class=CWinManageFileDlg
ControlCount=28
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_COMBODIR,combobox,1344340227
Control4=IDC_STATIC,static,1342308352
Control5=IDC_PLACEHOLDER,static,1350701056
Control6=IDC_ABOUT,button,1342242816
Control7=IDC_CREATENEWDIR,button,1342242816
Control8=IDC_DELETEDIR,button,1342242816
Control9=IDC_SHOWDIRINFO,button,1342251008
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,button,1342177287
Control12=IDC_CREATENEWFILE,button,1342242816
Control13=IDC_DELETEFILE,button,1342251008
Control14=IDC_READFILE,button,1342242816
Control15=IDC_WRITEFILE,button,1342242816
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,button,1342177287
Control18=IDC_NUMFILE,static,1342308352
Control19=IDC_NUMDIR,static,1342308352
Control20=IDC_SIZESYSTEM,static,1342308352
Control21=IDC_NUMLEFT,static,1342308352
Control22=IDC_STATIC,button,1342177287
Control23=IDC_CREATENEWSYSTEM,button,1342251008
Control24=IDC_OPENSYSTEM,button,1342251008
Control25=IDC_SAVESYSTEM,button,1342251008
Control26=IDC_STATIC,button,1342177287
Control27=IDC_MODIFYFILE,button,1476468736
Control28=IDC_MODIFYDIR,button,1342242816

[CLS:CDisplayView]
Type=0
HeaderFile=DisplayView.h
ImplementationFile=DisplayView.cpp
BaseClass=CScrollView
Filter=C
LastObject=CDisplayView

[CLS:CFileSystemDlg]
Type=0
HeaderFile=FileSystemDlg.h
ImplementationFile=FileSystemDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CFileSystemDlg
VirtualFilter=dWC

[DLG:IDD_FILESYSTEM]
Type=1
Class=CFileSystemDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_FCB,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_FAT,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[DLG:IDD_DIRNAME]
Type=1
Class=CDirNameDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_DIRNAME,edit,1350631552

[CLS:CDirNameDlg]
Type=0
HeaderFile=DirNameDlg.h
ImplementationFile=DirNameDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CDirNameDlg
VirtualFilter=dWC

[DLG:IDD_FILENAME]
Type=1
Class=CFileDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_FILENAME,edit,1350631552

[CLS:CFileDlg]
Type=0
HeaderFile=FileDlg.h
ImplementationFile=FileDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CFileDlg
VirtualFilter=dWC

[CLS:CDeleteFileDlg]
Type=0
HeaderFile=DeleteFileDlg.h
ImplementationFile=DeleteFileDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CDeleteFileDlg
VirtualFilter=dWC

[DLG:IDD_DELETEFILE]
Type=1
Class=CDeleteFileDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COMBOFILE1,combobox,1344340227
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_WRITEFILE]
Type=1
Class=CWriteFileDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CONTENT,edit,1350570180

[CLS:CWriteFileDlg]
Type=0
HeaderFile=WriteFileDlg.h
ImplementationFile=WriteFileDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

