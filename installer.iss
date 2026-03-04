; Script for Inno Setup to create an installer for StackCore (Raylib Version)

[Setup]
; Setup settings
AppName=StackCore
AppVersion=1.1
DefaultDirName={pf}\StackCore
DefaultGroupName=StackCore
OutputBaseFilename=StackCoreSetup
Compression=lzma
SolidCompression=yes
SetupIconFile=icon.ico

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "es"; MessagesFile: "compiler:Languages\Spanish.isl"

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; Flags: unchecked

[Files]
; Main executable (Standalone - No DLLs needed thanks to Raylib static linking)
Source: "Output\stackcore.exe"; DestDir: "{app}"; Flags: ignoreversion

; Configuration and Data (Optional to include default ones)
Source: "Output\game.dat"; DestDir: "{app}"; Flags: ignoreversion
Source: "Output\game_state.dat"; DestDir: "{app}"; Flags: ignoreversion

; Fonts
Source: "Output\fonts\*.ttf"; DestDir: "{app}\fonts"; Flags: ignoreversion

; Images
Source: "Output\images\*.png"; DestDir: "{app}\images"; Flags: ignoreversion

; Sounds
Source: "Output\sounds\*.mp3"; DestDir: "{app}\sounds"; Flags: ignoreversion

[Icons]
Name: "{group}\StackCore"; Filename: "{app}\stackcore.exe"
Name: "{userdesktop}\StackCore"; Filename: "{app}\stackcore.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\stackcore.exe"; Description: "Launch StackCore"; Flags: nowait postinstall skipifsilent
