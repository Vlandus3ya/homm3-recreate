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
english.GameDataPrompt=Optional: import your own Heroes III: Shadow of Death or Complete files now? Select the folder containing Data and Maps (Mp3 is optional). You can choose No and install the VCMI engine without game data. The game will need original files before it can be played.
russian.GameDataPrompt=Необязательно: импортировать ваши файлы Heroes III: Дыхание смерти или Complete сейчас? Выберите папку с Data и Maps (Mp3 необязательна). Можно нажать «Нет» и установить VCMI без игровых данных. Для игры оригинальные файлы всё равно понадобятся.
english.SelectGameFolder=Select the Heroes III game folder
russian.SelectGameFolder=Выберите папку с файлами Heroes III
english.InvalidGameFolder=The selected folder must contain both folders: Data and Maps. Mp3 is optional. Nothing was copied.
russian.InvalidGameFolder=В выбранной папке должны находиться обе папки: Data и Maps. Mp3 необязательна. Файлы не скопированы.
english.ImportComplete=Game data import finished. Existing files were not overwritten. The custom Recreate folders are ready.
russian.ImportComplete=Импорт игровых файлов завершён. Существующие файлы не перезаписывались. Папки Recreate подготовлены.
english.ImportIncomplete=Some files could not be copied. Existing files were left unchanged. Check Game Engine\\vcmi and try again if necessary.
russian.ImportIncomplete=Не все файлы удалось скопировать. Существующие файлы не изменялись. Проверьте Game Engine\\vcmi и при необходимости повторите импорт.
english.ImportSkipped=Game data import was skipped. The engine was installed, but you must add your own original Heroes III Data and Maps files before playing.
russian.ImportSkipped=Импорт игровых файлов пропущен. Движок установлен, но перед игрой нужно добавить собственные оригинальные папки Heroes III Data и Maps.
english.InstallNotice=Installation is complete. Modding Studio is installed. To play Heroes III, import your original Data and Maps. If the VCMI Map Editor fails to start, Modding Studio will try the original editor if it was present in the selected game folder. This installer does not include copyrighted game files.
russian.InstallNotice=Установка завершена. Modding Studio установлена. Для запуска Heroes III потребуется импортировать оригинальные Data и Maps. Если VCMI Map Editor не запустится, Modding Studio попробует открыть оригинальный редактор (если он есть в выбранной папке игры). Установщик не содержит защищённые авторским правом игровые файлы.

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional shortcuts:"
Name: "launch"; Description: "Launch Recreate after setup"; GroupDescription: "Additional options:"; Flags: unchecked

[Files]
; The workflow stages the complete VCMI install tree in dist\\Game Engine\\vcmi and
; the native Recreate front ends at the root of dist.
Source: "..\\dist\\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\\Heroes III Recreate"; Filename: "{app}\\h3r.exe"; WorkingDir: "{app}"
Name: "{group}\\Recreate Modding Studio"; Filename: "{app}\\Modding Studio.exe"; WorkingDir: "{app}"
Name: "{group}\\Recreate Diagnostics"; Filename: "{app}\\h3r.exe"; Parameters: "/check"; WorkingDir: "{app}"
Name: "{autodesktop}\\Heroes III Recreate"; Filename: "{app}\\h3r.exe"; WorkingDir: "{app}"; Tasks: desktopicon
Name: "{autodesktop}\\Recreate Modding Studio"; Filename: "{app}\\Modding Studio.exe"; WorkingDir: "{app}"; Tasks: desktopicon

[Run]
Filename: "{app}\\h3r.exe"; Description: "Launch Heroes III Recreate"; WorkingDir: "{app}"; Flags: postinstall nowait skipifsilent; Tasks: launch

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
  Result := HasFilesInFolder(BaseDir + '\\data')
    and HasFilesInFolder(BaseDir + '\\h3mmaps');
end;

procedure CreateRecreateFolders;
var
  Root: String;
begin
  Root := VCMIRootDir;
  ForceDirectories(Root + '\\data');
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

procedure ImportHeroes3Data;
var
  DestDir, SourceDir: String;
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

  SourceDir := '';
  if not BrowseForFolder(CustomMessage('SelectGameFolder'), SourceDir, False) then
  begin
    MsgBox(CustomMessage('ImportSkipped'), mbInformation, MB_OK);
    exit;
  end;

  if not HasSourceGameFolders(SourceDir) then
  begin
    MsgBox(CustomMessage('InvalidGameFolder'), mbError, MB_OK);
    MsgBox(CustomMessage('ImportSkipped'), mbInformation, MB_OK);
    exit;
  end;

  CreateRecreateFolders;
  if CopyGameFolder(SourceDir + '\\Data', DestDir + '\\data')
    and CopyGameFolder(SourceDir + '\\Maps', DestDir + '\\h3mmaps') then
  begin
    // Keep a separate complete local copy for the original fallback editors.
    // Registration files and registry scripts are intentionally not copied.
    ForceDirectories(ExpandConstant('{app}') + '\\Original Heroes III');
    if CopyGameFolder(SourceDir + '\\Data', ExpandConstant('{app}') + '\\Original Heroes III\\Data')
      and CopyGameFolder(SourceDir + '\\Maps', ExpandConstant('{app}') + '\\Original Heroes III\\Maps') then
    begin
      if FileExists(SourceDir + '\\h3maped.exe') then
        FileCopy(SourceDir + '\\h3maped.exe', ExpandConstant('{app}') + '\\Original Heroes III\\h3maped.exe', False);
      if FileExists(SourceDir + '\\h3ccmped.exe') then
        FileCopy(SourceDir + '\\h3ccmped.exe', ExpandConstant('{app}') + '\\Original Heroes III\\h3ccmped.exe', False);
    end;
    if FileExists(SourceDir + '\\Data\\VIDEO.VID') and
      not FileExists(DestDir + '\\Video\\VIDEO.VID') then
      FileCopy(SourceDir + '\\Data\\VIDEO.VID', DestDir + '\\Video\\VIDEO.VID', False);
    if FileExists(SourceDir + '\\Data\\Heroes3.snd') and
      not FileExists(DestDir + '\\sounds\\Heroes3.snd') then
      FileCopy(SourceDir + '\\Data\\Heroes3.snd', DestDir + '\\sounds\\Heroes3.snd', False);
    MsgBox(CustomMessage('ImportComplete'), mbInformation, MB_OK);
  end
  else
    MsgBox(CustomMessage('ImportIncomplete'), mbError, MB_OK);
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
