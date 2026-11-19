#define AppName "Heroes of Might and Magic III: Recreate"
#define AppVersion "0.1.0"
#ifndef AppArch
  #define AppArch "x64"
#endif

[Setup]
AppId={{A5B9D3E2-6F41-4B8C-9A72-3E1D8C4F0B65}
AppName={#AppName}
AppVersion={#AppVersion}
AppVerName={#AppName} {#AppVersion} ({#AppArch})
AppPublisher=Vlandus3ya
DefaultDirName={autopf}\Heroes of Might and Magic III Recreate
DefaultGroupName=Heroes III Recreate
DisableProgramGroupPage=yes
OutputDir=..\dist-installers
OutputBaseFilename=Heroes-of-Might-and-Magic-III-Recreate-{#AppArch}-Setup
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
ArchitecturesAllowed={#AppArch}
#if AppArch == "x64"
ArchitecturesInstallIn64BitMode=x64
#endif
PrivilegesRequired=admin
UninstallDisplayName={#AppName} ({#AppArch})
SetupLogging=yes
RestartApplications=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional shortcuts:"
Name: "launch"; Description: "Launch Recreate after setup"; GroupDescription: "Additional options:"; Flags: unchecked

[Files]
; The workflow stages the complete VCMI install tree in dist\VCMI and
; the native Recreate front ends at the root of dist.
Source: "..\dist\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\Heroes III Recreate"; Filename: "{app}\H3R-CL.exe"; WorkingDir: "{app}"
Name: "{group}\Recreate Modding Studio"; Filename: "{app}\HoMM3-Recreate-ModdingStudio.exe"; WorkingDir: "{app}"
Name: "{autodesktop}\Heroes III Recreate"; Filename: "{app}\H3R-CL.exe"; WorkingDir: "{app}"; Tasks: desktopicon

[Run]
Filename: "{app}\H3R-CL.exe"; Description: "Launch Heroes III Recreate"; WorkingDir: "{app}"; Flags: postinstall nowait skipifsilent; Tasks: launch
