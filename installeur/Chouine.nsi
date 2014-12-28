; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 


XPStyle on

!include "FileFunc.nsh"
!include LogicLib.nsh
!include "MUI2.nsh"

!insertmacro MUI_LANGUAGE "French"

!include WinVer.nsh

; The name of the installer
!define APPNAME "Chouine"
Name "Chouine V${VERSION}"

; The file to write
OutFile "Chouine_install.exe"

AutoCloseWindow false 

Var ConfMessage

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;!insertmacro MUI_PAGE_LICENSE "license.txt"

Function FileSizeNew 
 
  Exch $0
  Push $1
  FileOpen $1 $0 "r"
  FileSeek $1 0 END $0
  FileClose $1
  Pop $1
  Exch $0
 
FunctionEnd

   
;--------------------------------
Function .onInit
	
	StrCpy $Language "${NSISDIR}\Contrib\Language files\French.nlf"
	
	checkAccount:
    #
	# CHECK ADMIN PROVILEDGES
	#
	# call userInfo plugin to get user info.  The plugin puts the result in the stack
    userInfo::getAccountType
    # pop the result from the stack into $0
    pop $0
    # compare the result with the string "Admin" to see if the user is admin.
    # If match, jump 3 lines down.
    strCmp $0 "Admin" +3
 
    # if there is not a match, print message and return
    messageBox MB_OK "Les droits d'adminitrateurs sont nécessaires pour l'installation. Faites un 'click droit' sur le fichier, puis cliquez sur 'Exécuter en tant qu'administrateur'"
    Abort

		
	#
	# INSTALL OR UPGRADE ???
	#
	
	StrCpy $ConfMessage "Installation de Chouine ${VERSION} Voulez-vous continuer ?"
	IfFileExists $PROGRAMFILES\Chouine\chouine.exe 0 ConfirmInstall
		StrCpy $ConfMessage "Une autre version a été détectée, voulez-vous continuer l'installation de la version ${VERSION} ?"
	
	#
	# VERIFY IF FILES CAN BE OVERWRITTEN
	#
    ; first make a backup... 
	CopyFiles /SILENT $PROGRAMFILES\Chouine\chouine.exe $InstallDir\chouine.exe.backup
	ClearErrors
	FileOpen $0 $PROGRAMFILES\Chouine\chouine.exe w
	IfErrors WriteError
	FileWrite $0 "Checkinstall"
	IfErrors WriteError
	FileClose $0
	Goto ConfirmInstall
	
	WriteError:
		Delete $PROGRAMFILES\Chouine\chouine.exe.backup
		MessageBox MB_OK "Merci de quitter le jeu avant la mise à jour."
		Abort
  
	ConfirmInstall:
	#
	# ASK CONFIRMATION BEFORE INSTALLING
	#
	Delete $PROGRAMFILES\Chouine\chouine.exe
	CopyFiles /SILENT $InstallDir\chouine.exe.backup $InstallDir\chouine.exe
	Delete $PROGRAMFILES\Chouine\chouine.exe.backup
	MessageBox MB_YESNO $ConfMessage	 IDYES gogogo
		Abort
  
  gogogo:
FunctionEnd


 Function un.onInit
    MessageBox MB_YESNO "Voulez-vous déinstaller le jeu de la Chouine ?" IDYES NoAbort
      Abort ; causes uninstaller to quit.
    NoAbort:
	
	IfFileExists $PROGRAMFILES\Chouine\chouine.exe Found 0
    
	Found:
	
  FunctionEnd
  
 Function .onInstSuccess
	MessageBox MB_OK "La chouine a été installée."
 FunctionEnd

; Pages

Page instfiles

;--------------------------------

; The stuff to install
Section "GENERAL" ;No components page, name is not important

#	!insertmacro CheckDotNET ${DOTNET_VERSION}
	SetShellVarContext all  

	; Set output path to the installation directory.
   SetOutPath $PROGRAMFILES\Chouine
   
	# create the uninstaller  
    writeUninstaller "$PROGRAMFILES\Chouine\uninstall.exe"  
	   
   # install an icon for the unistaller
   File chouine.ico
   File chouine.exe
   File lwjgl64.dll
   File OpenAL64.dll
   
   SetOutPath $PROGRAMFILES\Chouine\lib
   File "lib\"

	# Start Menu
	createDirectory "$SMPROGRAMS\Chouine"
	createShortCut "$SMPROGRAMS\Chouine\Chouine.lnk" "$PROGRAMFILES\Chouine\chouine.exe" "" "$PROGRAMFILES\Chouine\chouine.ico"
	createShortCut "$SMPROGRAMS\Chouine\deinstallation.lnk" "$PROGRAMFILES\Chouine\uninstall.exe"
  
  
  #Writing registry keys  
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Chouine" "DisplayName" "Chouine ${VERSION}"  
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Chouine" "UninstallString" "$PROGRAMFILES\Chouine\uninstall.exe"  
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Chouine" "Publisher" "Christophe Vincent"  
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Chouine" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Chouine" "DisplayIcon" "$PROGRAMFILES\Chouine\chouine.ico"
 
 Push "$PROGRAMFILES\Chouine\"
  Call FileSizeNew
  Pop $0
  #IntFmt $0 "0x%08X" $0
  IntOp $1 $0 / 1024 ;
  IntFmt $1 "0x%08X" $1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Chouine" "EstimatedSize " "$1"
  
  
SectionEnd ; end the section


Section "Uninstall"
	SetShellVarContext all
	
	Delete "$SMPROGRAMS\Chouine\Chouine.lnk"
	Delete "$SMPROGRAMS\Chouine\deinstallation.lnk"
	RMDir /r "$SMPROGRAMS\Chouine"
	
   DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Chouine"
	DeleteRegValue  HKLM "Software\Microsoft\Windows\CurrentVersion\Run" "Chouine"
	Delete  "$PROGRAMFILES\Chouine\chouine.exe"
	RMDir /r "$PROGRAMFILES\Chouine" 
	
	
SectionEnd