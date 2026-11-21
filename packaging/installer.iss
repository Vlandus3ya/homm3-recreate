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
DefaultDirName={autopf}\\Heroes of Might and Magic III Recreate
DefaultGroupName=Heroes III Recreate
DisableProgramGroupPage=yes
OutputDir=..\\dist-installers
OutputBaseFilename=Heroes-of-Might-and-Magic-III-Recreate-{#AppArch}-Setup
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
WizardSizePercent=110
WizardResizable=no
DisableWelcomePage=no
DisableFinishedPage=no
AppComments=Heroes III Recreate engine and Modding Studio
VersionInfoDescription=Heroes III Recreate Installer
UninstallDisplayIcon={app}\\h3r.exe
SetupIconFile=assets\\icons\\heroes3.ico
ArchitecturesAllowed={#AppArch}
#if AppArch == "x64"
ArchitecturesInstallIn64BitMode=x64
#endif
PrivilegesRequired=admin
UninstallDisplayName={#AppName} ({#AppArch})
SetupLogging=yes
RestartApplications=no


[Messages]
english.WelcomeLabel1=Welcome to Heroes III: Recreate!
english.WelcomeLabel2=This wizard will install the Recreate engine and Modding Studio.\n\nYou can install the tools without original Heroes III game files. Those files are only needed to play the game.
russian.WelcomeLabel1=Добро пожаловать в Heroes III: Recreate!
russian.WelcomeLabel2=Мастер установит движок Recreate и Modding Studio.\n\nИнструменты можно установить без оригинальных файлов Heroes III. Игровые файлы потребуются только для запуска самой игры.
english.FinishedHeadingLabel=Heroes III: Recreate is ready
english.FinishedLabel=The installation has completed successfully.
russian.FinishedHeadingLabel=Heroes III: Recreate готова
russian.FinishedLabel=Установка успешно завершена.

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "russian"; MessagesFile: "compiler:Languages\\Russian.isl"

[CustomMessages]
english.GameDataFound=Heroes III game data was found in Game Engine\\vcmi. No import is needed.
russian.GameDataFound=Файлы Heroes III уже найдены в Game Engine\\vcmi. Импорт не требуется.
english.GameDataPrompt=Import your Heroes III ZIP archive now? The installer extracts it temporarily and imports Data, Maps, h3maped.exe and h3ccmped.exe. Heroes3.exe, RegisterSOD and hmm3sod.reg are ignored.
russian.GameDataPrompt=Импортировать ZIP-архив Heroes III? Установщик временно распакует его и импортирует Data, Maps, h3maped.exe и h3ccmped.exe. Heroes3.exe, RegisterSOD и hmm3sod.reg игнорируются.
english.SelectGameFolder=Select the Heroes III game folder
russian.SelectGameFolder=Выберите папку с файлами Heroes III
english.InvalidGameFolder=The selected ZIP must contain the Heroes III folder with Data and Maps. Nothing was copied.
russian.InvalidGameFolder=В ZIP-архиве должна находиться папка Heroes III с Data и Maps. Файлы не скопированы.
english.ImportComplete=Game data, original H3M maps, and available legacy editors were imported. Existing files were not overwritten.
russian.ImportComplete=Игровые данные, оригинальные карты H3M и доступные старые редакторы импортированы. Существующие файлы не перезаписывались.
english.ImportIncomplete=Some files could not be copied. Existing files were left unchanged. Check Game Engine\\vcmi and try again if necessary.
russian.ImportIncomplete=Не все файлы удалось скопировать. Существующие файлы не изменялись. Проверьте Game Engine\\vcmi и при необходимости повторите импорт.
english.ImportSkipped=Game data import was skipped. The engine was installed, but you must add your own original Heroes III Data and Maps files before playing.
russian.ImportSkipped=Импорт игровых файлов пропущен. Движок установлен, но перед игрой нужно добавить собственные оригинальные папки Heroes III Data и Maps.
english.InstallNotice=Installation is complete. Modding Studio prefers the imported original map/campaign editors. Original Data is installed in Game Engine\\vcmi\\Data; original H3M maps are in h3mmaps. Converted VMap maps and downloaded VCMI maps belong in vmapmaps. This public installer does not include proprietary game files.
russian.InstallNotice=Установка завершена. Modding Studio сначала использует импортированные оригинальные редакторы карт и кампаний. Оригинальная Data находится в Game Engine\\vcmi\\Data, карты H3M — в h3mmaps. Конвертированные VMap и скачанные карты VCMI помещайте в vmapmaps. В публичный установщик игровые ресурсы не включены.

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional shortcuts:"
Name: "launch"; Description: "Launch Recreate after setup"; GroupDescription: "Additional options:"; Flags: unchecked

[Files]
; Public Recreate/VCMI runtime files.
Source: "..\\dist\\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; Two user-provided application icons (not proprietary game data).
Source: "assets\\icons\\*.ico"; DestDir: "{app}\\assets\\icons"; Flags: ignoreversion

[Icons]
Name: "{group}\\Heroes III Recreate"; Filename: "{app}\\h3r.exe"; IconFilename: "{app}\\assets\\icons\\heroes3.ico"; WorkingDir: "{app}"
Name: "{group}\\Recreate Modding Studio"; Filename: "{app}\\Modding Studio.exe"; IconFilename: "{app}\\assets\\icons\\modding-studio.ico"; WorkingDir: "{app}"
Name: "{group}\\Recreate Diagnostics"; Filename: "{app}\\h3r.exe"; IconFilename: "{app}\\assets\\icons\\heroes3.ico"; Parameters: "/check"; WorkingDir: "{app}"
Name: "{autodesktop}\\Heroes III Recreate"; Filename: "{app}\\h3r.exe"; IconFilename: "{app}\\assets\\icons\\heroes3.ico"; WorkingDir: "{app}"; Tasks: desktopicon
Name: "{autodesktop}\\Recreate Modding Studio"; Filename: "{app}\\Modding Studio.exe"; IconFilename: "{app}\\assets\\icons\\modding-studio.ico"; WorkingDir: "{app}"; Tasks: desktopicon

[Run]
Filename: "{app}\\h3r.exe"; Description: "Launch Heroes III Recreate"; WorkingDir: "{app}"; Flags: postinstall nowait skipifsilent; Tasks: launch

[UninstallDelete]
; Remove imported game data, maps, mods, settings, and all other user-created files
; inside this installation directory, then remove the now-empty directory itself.
Type: filesandordirs; Name: "{app}\\*"
Type: dirifempty; Name: "{app}"

[Code]
function VCMIRootDir: String;
begin
  Result := ExpandConstant('{app}') + '\\Game Engine\\vcmi';
end;

function HasSourceGameFolders(const BaseDir: String): Boolean;
begin
  Result := DirExists(BaseDir + '\\Data')
    and DirExists(BaseDir + '\\Maps');
end;

function HasFilesInFolder(const Folder: String): Boolean;
var
  FindRec: TFindRec;
begin
  Result := False;
  if FindFirst(Folder + '\\*', FindRec) then
  begin
    try
      repeat
        if (FindRec.Name <> '.') and (FindRec.Name <> '..')
          and ((FindRec.Attributes and FILE_ATTRIBUTE_DIRECTORY) = 0) then
        begin
          Result := True;
          exit;
        end;
      until not FindNext(FindRec);
    finally
      FindClose(FindRec);
    end;
  end;
end;

function HasInstalledGameFolders(const BaseDir: String): Boolean;
begin
  // Empty directories created by the installer must not count as imported game data.
  Result := HasFilesInFolder(BaseDir + '\\Data')
    and HasFilesInFolder(BaseDir + '\\h3mmaps');
end;

procedure CreateRecreateFolders;
var
  Root: String;
begin
  Root := VCMIRootDir;
  ForceDirectories(Root + '\\Data');
  ForceDirectories(Root + '\\h3mmaps');
  ForceDirectories(Root + '\\vmapmaps');
  ForceDirectories(Root + '\\Video');
  ForceDirectories(Root + '\\sounds');
  ForceDirectories(ExpandConstant('{app}') + '\\Profile');
  ForceDirectories(ExpandConstant('{app}') + '\\settings');
  ForceDirectories(ExpandConstant('{app}') + '\\InternetIpConnections');
  ForceDirectories(ExpandConstant('{app}') + '\\Bots');
end;

// Copies missing files only. Existing destination files are deliberately preserved.
function CopyGameFolder(const SourceDir, DestDir: String): Boolean;
var
  FindRec: TFindRec;
  SourceName, DestName: String;
begin
  Result := True;
  if not DirExists(DestDir) then
  begin
    if not ForceDirectories(DestDir) then
    begin
      Result := False;
      exit;
    end;
  end;

  if FindFirst(SourceDir + '\\*', FindRec) then
  begin
    try
      repeat
        if (FindRec.Name <> '.') and (FindRec.Name <> '..') then
        begin
          SourceName := SourceDir + '\\' + FindRec.Name;
          DestName := DestDir + '\\' + FindRec.Name;
          if (FindRec.Attributes and FILE_ATTRIBUTE_DIRECTORY) <> 0 then
          begin
            if not CopyGameFolder(SourceName, DestName) then
              Result := False;
          end
          else if not FileExists(DestName) then
          begin
            if not FileCopy(SourceName, DestName, False) then
              Result := False;
          end;
        end;
      until not FindNext(FindRec);
    finally
      FindClose(FindRec);
    end;
  end
  else
    Result := False;
end;

function EscapePowerShell(const Value: String): String;
begin
  Result := Value;
  StringChangeEx(Result, '''', '''''', True);
end;

function FindGameRoot(const BaseDir: String): String;
var
  FindRec: TFindRec;
  Candidate, Nested: String;
begin
  Result := '';
  if HasSourceGameFolders(BaseDir) then
  begin
    Result := BaseDir;
    exit;
  end;

  if FindFirst(BaseDir + '\\*', FindRec) then
  begin
    try
      repeat
        if (FindRec.Name <> '.') and (FindRec.Name <> '..')
          and ((FindRec.Attributes and FILE_ATTRIBUTE_DIRECTORY) <> 0) then
        begin
          Candidate := BaseDir + '\\' + FindRec.Name;
          Nested := FindGameRoot(Candidate);
          if Nested <> '' then
          begin
            Result := Nested;
            exit;
          end;
        end;
      until not FindNext(FindRec);
    finally
      FindClose(FindRec);
    end;
  end;
end;

procedure ImportHeroes3Data;
var
  DestDir, ArchivePath, TempDir, PowerShellPath, Parameters: String;
  SourceDir: String;
  ResultCode: Integer;
begin
  DestDir := VCMIRootDir;

  if HasInstalledGameFolders(DestDir) then
  begin
    MsgBox(CustomMessage('GameDataFound'), mbInformation, MB_OK);
    exit;
  end;

  if MsgBox(CustomMessage('GameDataPrompt'), mbConfirmation, MB_YESNO) <> IDYES then
  begin
    MsgBox(CustomMessage('ImportSkipped'), mbInformation, MB_OK);
    exit;
  end;

  if not GetOpenFileName(
    'Select your Heroes III ZIP archive',
    ExpandConstant('{userdocs}'),
    'Файлы герои 3.zip',
    'ZIP archives (*.zip)|*.zip|All files (*.*)|*.*',
    ArchivePath) then
  begin
    MsgBox(CustomMessage('ImportSkipped'), mbInformation, MB_OK);
    exit;
  end;

  TempDir := ExpandConstant('{tmp}\\Recreate-H3-Import');
  DelTree(TempDir, True, True, True);
  if not ForceDirectories(TempDir) then
  begin
    MsgBox('Could not create a temporary extraction folder.', mbError, MB_OK);
    exit;
  end;

  PowerShellPath := ExpandConstant('{sys}\\WindowsPowerShell\\v1.0\\powershell.exe');
  Parameters := '-NoProfile -ExecutionPolicy Bypass -Command "Expand-Archive -LiteralPath '
    + #39 + EscapePowerShell(ArchivePath) + #39
    + ' -DestinationPath ' + #39 + EscapePowerShell(TempDir) + #39 + ' -Force"';

  if not Exec(PowerShellPath, Parameters, '', SW_HIDE, ewWaitUntilTerminated, ResultCode)
    or (ResultCode <> 0) then
  begin
    MsgBox('The Heroes III ZIP archive could not be extracted. Nothing was imported.', mbError, MB_OK);
    exit;
  end;

  SourceDir := FindGameRoot(TempDir);
  if (SourceDir = '') or not HasSourceGameFolders(SourceDir) then
  begin
    MsgBox(CustomMessage('InvalidGameFolder'), mbError, MB_OK);
    MsgBox(CustomMessage('ImportSkipped'), mbInformation, MB_OK);
    DelTree(TempDir, True, True, True);
    exit;
  end;

  CreateRecreateFolders;
  if CopyGameFolder(SourceDir + '\\Data', DestDir + '\\Data')
    and CopyGameFolder(SourceDir + '\\Maps', DestDir + '\\h3mmaps') then
  begin
    // Import only the two requested legacy editors, not the original game executable.
    if FileExists(SourceDir + '\\h3maped.exe') then
      FileCopy(SourceDir + '\\h3maped.exe', DestDir + '\\h3maped.exe', False);
    if FileExists(SourceDir + '\\h3ccmped.exe') then
      FileCopy(SourceDir + '\\h3ccmped.exe', DestDir + '\\h3ccmped.exe', False);
    MsgBox(CustomMessage('ImportComplete'), mbInformation, MB_OK);
  end
  else
    MsgBox(CustomMessage('ImportIncomplete'), mbError, MB_OK);

  DelTree(TempDir, True, True, True);
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    CreateRecreateFolders;
    ImportHeroes3Data;
    MsgBox(CustomMessage('InstallNotice'), mbInformation, MB_OK);
  end;
end;
