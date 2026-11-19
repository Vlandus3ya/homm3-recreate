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
UninstallDisplayIcon={app}\\H3R-CL.exe
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
english.GameDataFound=Heroes III game data was found in your VCMI data folder. No import is needed.
russian.GameDataFound=Файлы Heroes III уже найдены в папке данных VCMI. Импорт не требуется.
english.GameDataPrompt=Optional: import your own Heroes III: Shadow of Death or Complete files now? Select the folder containing Data and Maps (Mp3 is optional). You can choose No and install the VCMI engine without game data. The game will need original files before it can be played.
russian.GameDataPrompt=Необязательно: импортировать ваши файлы Heroes III: Дыхание смерти или Complete сейчас? Выберите папку с Data и Maps (Mp3 необязательна). Можно нажать «Нет» и установить VCMI без игровых данных. Для игры оригинальные файлы всё равно понадобятся.
english.SelectGameFolder=Select the Heroes III game folder
russian.SelectGameFolder=Выберите папку с файлами Heroes III
english.InvalidGameFolder=The selected folder must contain both folders: Data and Maps. Mp3 is optional. Nothing was copied.
russian.InvalidGameFolder=В выбранной папке должны находиться обе папки: Data и Maps. Mp3 необязательна. Файлы не скопированы.
english.ImportComplete=Game data import finished. Existing files were not overwritten. VCMI can now scan the data folder.
russian.ImportComplete=Импорт игровых файлов завершён. Существующие файлы не перезаписывались. Теперь VCMI может просканировать папку с данными.
english.ImportIncomplete=Some files could not be copied. Existing files were left unchanged. Check the VCMI data folder and try again if necessary.
russian.ImportIncomplete=Не все файлы удалось скопировать. Существующие файлы не изменялись. Проверьте папку данных VCMI и при необходимости повторите импорт.
english.ImportSkipped=Game data import was skipped. The engine was installed, but you must add your own original Heroes III Data and Maps files before playing.
russian.ImportSkipped=Импорт игровых файлов пропущен. Движок установлен, но перед игрой нужно добавить собственные оригинальные папки Heroes III Data и Maps.
english.InstallNotice=Installation is complete. Modding Studio is installed. To play Heroes III, the original game data folders Data and Maps must be added later. This installer does not include copyrighted game files.
russian.InstallNotice=Установка завершена. Modding Studio установлена. Для запуска самой Heroes III позднее потребуется добавить оригинальные папки Data и Maps. Установщик не содержит защищённые авторским правом игровые файлы.

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional shortcuts:"
Name: "launch"; Description: "Launch Recreate after setup"; GroupDescription: "Additional options:"; Flags: unchecked

[Files]
; The workflow stages the complete VCMI install tree in dist\\VCMI and
; the native Recreate front ends at the root of dist.
Source: "..\\dist\\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\\Heroes III Recreate"; Filename: "{app}\\H3R-CL.exe"; WorkingDir: "{app}"
Name: "{group}\\Recreate Modding Studio"; Filename: "{app}\\HoMM3-Recreate-ModdingStudio.exe"; WorkingDir: "{app}"
Name: "{autodesktop}\\Heroes III Recreate"; Filename: "{app}\\H3R-CL.exe"; WorkingDir: "{app}"; Tasks: desktopicon
Name: "{autodesktop}\\Recreate Modding Studio"; Filename: "{app}\\HoMM3-Recreate-ModdingStudio.exe"; WorkingDir: "{app}"; Tasks: desktopicon

[Run]
Filename: "{app}\\H3R-CL.exe"; Description: "Launch Heroes III Recreate"; WorkingDir: "{app}"; Flags: postinstall nowait skipifsilent; Tasks: launch

[Code]
function VCMIDataDir: String;
begin
  Result := ExpandConstant('{userdocs}') + '\\My Games\\vcmi';
end;

function HasRequiredGameFolders(const BaseDir: String): Boolean;
begin
  Result := DirExists(BaseDir + '\\Data')
    and DirExists(BaseDir + '\\Maps');
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
  DestDir := VCMIDataDir;

  if HasRequiredGameFolders(DestDir) then
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

  if not HasRequiredGameFolders(SourceDir) then
  begin
    MsgBox(CustomMessage('InvalidGameFolder'), mbError, MB_OK);
    MsgBox(CustomMessage('ImportSkipped'), mbInformation, MB_OK);
    exit;
  end;

  if not ForceDirectories(DestDir) then
  begin
    MsgBox(CustomMessage('ImportIncomplete'), mbError, MB_OK);
    exit;
  end;

  if CopyGameFolder(SourceDir + '\\Data', DestDir + '\\Data')
    and CopyGameFolder(SourceDir + '\\Maps', DestDir + '\\Maps') then
  begin
    // Music files are optional; many game-data archives do not contain Mp3.
    if DirExists(SourceDir + '\\Mp3') then
      if not CopyGameFolder(SourceDir + '\\Mp3', DestDir + '\\Mp3') then
      begin
        MsgBox(CustomMessage('ImportIncomplete'), mbError, MB_OK);
        exit;
      end;
    MsgBox(CustomMessage('ImportComplete'), mbInformation, MB_OK);
  end
  else
    MsgBox(CustomMessage('ImportIncomplete'), mbError, MB_OK);
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
    MsgBox(CustomMessage('InstallNotice'), mbInformation, MB_OK);
end;
