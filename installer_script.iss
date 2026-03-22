[Setup]
AppName=EchoProtocol
AppVersion=1.0
DefaultDirName={pf}\EchoProtocol
DefaultGroupName=EchoProtocol
OutputBaseFilename=EchoProtocolInstaller
SetupIconFile=resources\iconApp.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern
DisableProgramGroupPage=yes

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; exe
Source: "build\Release\EchoProtocol.exe"; DestDir: "{app}"; Flags: ignoreversion
; Assets
Source: "assets\*"; DestDir: "{app}\assets"; Flags: recursesubdirs createallsubdirs
; DLL SDL3
Source: "build\external\SDL\Release\SDL3.dll"; DestDir: "{app}"; Flags: ignoreversion ignoremissing
Source: "build\external\SDL_ttf\Release\SDL3_ttf.dll"; DestDir: "{app}"; Flags: ignoreversion ignoremissing
Source: "build\external\SDL_image\Release\SDL3_image.dll"; DestDir: "{app}"; Flags: ignoreversion ignoremissing

[Icons]
Name: "{group}\EchoProtocol"; Filename: "{app}\EchoProtocol.exe"
Name: "{userdesktop}\EchoProtocol"; Filename: "{app}\EchoProtocol.exe"; Tasks: desktopicon
