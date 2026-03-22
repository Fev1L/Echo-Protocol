[Setup]
AppName=EchoProtocol
AppVersion=1.0
DefaultDirName={commonpf}\EchoProtocol
DefaultGroupName=EchoProtocol
OutputDir=build\installer
OutputBaseFilename=EchoProtocolInstaller
SetupIconFile=resources\iconApp.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern
DisableProgramGroupPage=yes
PrivilegesRequired=admin

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "build\Release\EchoProtocol.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "assets\*"; DestDir: "{app}\assets"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "build\external\SDL\Release\SDL3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\external\SDL_ttf\Release\SDL3_ttf.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\external\SDL_image\Release\SDL3_image.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\EchoProtocol"; Filename: "{app}\EchoProtocol.exe"
Name: "{commondesktop}\EchoProtocol"; Filename: "{app}\EchoProtocol.exe"; Tasks: desktopicon