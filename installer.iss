; Script for Inno Setup to create an installer for the retro block game

[Setup]
; Setup settings
AppName=Retro Block Game
AppVersion=1.0
DefaultDirName={pf}\RetroBlockGame
DefaultGroupName=Retro Block Game
OutputBaseFilename=RetroBlockGameSetup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; Flags: unchecked

[Files]
; Main executable and related files
Source: "Output\stackcore.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "Output\imgui.ini"; DestDir: "{app}"; Flags: ignoreversion
Source: "Output\score.dat"; DestDir: "{app}"; Flags: ignoreversion
Source: "Output\SDL2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Output\SDL2_image.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Output\SDL2_mixer.dll"; DestDir: "{app}"; Flags: ignoreversion

; Fonts
Source: "Output\fonts\LCD.ttf"; DestDir: "{app}\fonts"; Flags: ignoreversion

; Images
Source: "Output\images\gameisover.png"; DestDir: "{app}\images"; Flags: ignoreversion
Source: "Output\images\gamepaused.png"; DestDir: "{app}\images"; Flags: ignoreversion
Source: "Output\images\image2.png"; DestDir: "{app}\images"; Flags: ignoreversion
Source: "Output\images\sound_disabled.png"; DestDir: "{app}\images"; Flags: ignoreversion
Source: "Output\images\sound_enabled.png"; DestDir: "{app}\images"; Flags: ignoreversion

; Sounds
Source: "Output\sounds\gameover.mp3"; DestDir: "{app}\sounds"; Flags: ignoreversion
Source: "Output\sounds\parked.mp3"; DestDir: "{app}\sounds"; Flags: ignoreversion
Source: "Output\sounds\select.mp3"; DestDir: "{app}\sounds"; Flags: ignoreversion
Source: "Output\sounds\start.mp3"; DestDir: "{app}\sounds"; Flags: ignoreversion

[Icons]
Name: "{group}\Retro Block Game"; Filename: "{app}\stackcore.exe"
Name: "{userdesktop}\Retro Block Game"; Filename: "{app}\stackcore.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\stackcore.exe"; Description: "Launch Retro Block Game"; Flags: nowait postinstall skipifsilent
