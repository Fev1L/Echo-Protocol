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

[Files]
; exe
Source: "build\Release\EchoProtocol.exe"; DestDir: "{app}"; Flags: ignoreversion
; Assets
Source: "assets\*"; DestDir: "{app}\assets"; Flags: recursesubdirs createallsubdirs
; DLL SDL3
Source: "build\Release\*.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs

[Icons]
Name: "{group}\EchoProtocol"; Filename: "{app}\EchoProtocol.exe"
Name: "{userdesktop}\EchoProtocol"; Filename: "{app}\EchoProtocol.exe"; Tasks: desktopicon