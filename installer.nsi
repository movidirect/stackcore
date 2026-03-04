!include "MUI2.nsh"

Name "StackCore 1.1"
OutFile "Output/WindowsStackCoreSetup.exe"
InstallDir "$PROGRAMFILES\StackCore"
SetCompressor /SOLID lzma

; Request application privileges for Windows Vista+
RequestExecutionLevel admin

; Variables
Var StartMenuFolder

;--------------------------------
; Interface Settings

!define MUI_ABORTWARNING
!define MUI_ICON "icon.ico"
!define MUI_UNICON "icon.ico"

;--------------------------------
; Pages

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES

; Checkbox to run the game after installation finishes
!define MUI_FINISHPAGE_RUN "$INSTDIR\stackcore.exe"
!define MUI_FINISHPAGE_RUN_TEXT "Launch StackCore"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
; Languages

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Spanish"

LangString DESC_SecMain ${LANG_ENGLISH} "Core game files (Required)."
LangString DESC_SecMain ${LANG_SPANISH} "Archivos principales del juego (Requerido)."

LangString DESC_SecDesktop ${LANG_ENGLISH} "Create a shortcut on the desktop."
LangString DESC_SecDesktop ${LANG_SPANISH} "Crear un acceso directo en el escritorio."

;--------------------------------
; Installer Sections

Section "StackCore (Required)" SecMain
  SectionIn RO
  SetOutPath "$INSTDIR"
  
  ; Main executable
  File "Output\stackcore.exe"
  
  ; Configuration and Data (Optional)
  File /nonfatal "Output\game.dat"
  File /nonfatal "Output\game_state.dat"
  
  ; Fonts
  SetOutPath "$INSTDIR\fonts"
  File "Output\fonts\*.ttf"
  
  ; Images
  SetOutPath "$INSTDIR\images"
  File "Output\images\*.png"
  
  ; Sounds
  SetOutPath "$INSTDIR\sounds"
  File "Output\sounds\*.mp3"

  ; Create uninstaller
  SetOutPath "$INSTDIR"
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  ; Start Menu Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\StackCore.lnk" "$INSTDIR\stackcore.exe"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Uninstall StackCore.lnk" "$INSTDIR\Uninstall.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

; /o means unselected by default, mirroring Inno Setup's "Flags: unchecked"
Section /o "Desktop Icon" SecDesktop
  CreateShortcut "$DESKTOP\StackCore.lnk" "$INSTDIR\stackcore.exe"
SectionEnd

;--------------------------------
; Descriptions

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} $(DESC_SecMain)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecDesktop} $(DESC_SecDesktop)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
; Uninstaller Section

Section "Uninstall"
  ; Delete Files
  Delete "$INSTDIR\stackcore.exe"
  Delete "$INSTDIR\game.dat"
  Delete "$INSTDIR\game_state.dat"
  Delete "$INSTDIR\Uninstall.exe"
  
  Delete "$INSTDIR\fonts\*.ttf"
  Delete "$INSTDIR\images\*.png"
  Delete "$INSTDIR\sounds\*.mp3"
  
  ; Remove Directories
  RMDir "$INSTDIR\fonts"
  RMDir "$INSTDIR\images"
  RMDir "$INSTDIR\sounds"
  RMDir "$INSTDIR"
  
  ; Remove Start Menu shortcuts
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  Delete "$SMPROGRAMS\$StartMenuFolder\StackCore.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall StackCore.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  ; Remove Desktop shortcut
  Delete "$DESKTOP\StackCore.lnk"
SectionEnd
