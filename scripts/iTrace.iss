#define VERSION    630
#define VERSIONSTR "6.3.0"
#define NAME       "iTrace 6.3.0"

#ifndef YEAR
#define YEAR	  "2021"
#define DATE	  "0326"
#endif

#ifndef VCREDIST
#define VCREDIST "redist\VC_redist.x86.exe"
#endif

#ifndef VCREDIST_VERSION
#define VCREDIST_VERSION  29910
#endif

#define EXTRA    ""
#define BETA	  ""

#define MAINFOLDER      "C:\Tracey"
#define DEFAULTDESTDIR  "{#MAINFOLDER}\Bin\{#VERSION}"

#define OUTDIR   "..\_out\Win32\Release"
#define DEST     "{app}"
#define DRIVERS  "{#MAINFOLDER}\Drivers"

[Setup]
AppName={#NAME}
AppVerName={#NAME}
DefaultDirName="{#MAINFOLDER}\Bin\Ver{#VERSION}"
DefaultGroupName=iTrace
AppPublisher=Tracey Technologies
AppPublisherURL=http://www.traceytech.com
AppVersion={#VERSIONSTR}
LicenseFile=License.rtf
OutputDir=../_setup
OutputBaseFilename=iTrace_Setup_{#VERSION}_{#YEAR}_{#DATE}
SetupIconFile=..\src\gui\\res\iTrace.ico
WizardStyle=modern
UsePreviousAppDir=False
MinVersion=0,6.1

[Tasks]

Name: desktopicon; Description: "Create a desktop icon"; GroupDescription: "Additional icons:"

[Dirs]

Name: "C:\Tracey\Drivers";

[Files]
Source: "{#OUTDIR}\iTrace.exe"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\iTrace.json"; DestDir: "{#DEST}"
Source: "{#OUTDIR}\opencv_core.dll"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\opencv_imgproc.dll"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\rzdcx.dll"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\sqlite3.dll"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\tdbmsjet31.dll"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\tdbmsjet62.dll"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\thw.dll"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\TraceyDatabaseJet.dll"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\TraceyDicom.dll"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\TraceyHardware.dll"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\TraceyServices.dll"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\ueye_api.dll"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\zlib1.dll"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\TraceyRzdcxActivation.exe"; DestDir: "{#DEST}"; Flags: ignoreversion
Source: "{#OUTDIR}\TraceyRzdcxActivation.exe.config"; DestDir: "{#DEST}"
Source: "redist\Tracey_Lynx.inf"; DestDir: "C:\Tracey\Drivers"; Flags: uninsneveruninstall
Source: "redist\Tracey_Lynx.cat"; DestDir: "C:\Tracey\Drivers"; Flags: uninsneveruninstall
Source: "redist\Tracey_Lynx_a.sys"; DestDir: "C:\Tracey\Drivers"; Flags: uninsneveruninstall
Source: "redist\Tracey_Lynx_b.sys"; DestDir: "C:\Tracey\Drivers"; Flags: uninsneveruninstall

; C++ CRT
Source: "{#VCREDIST}"; DestDir: "{tmp}"; DestName: "VcRedist.exe"; Flags: deleteafterinstall; Check: ShouldInstallRedist

[Icons]

Name: "{group}\{#NAME}"            ; Filename: "{#DEST}\iTrace";
Name: "{group}\Uninstall {#NAME}"  ; Filename: "{uninstallexe}";
Name: "{userdesktop}\{#NAME}"      ; Filename: "{#DEST}\iTrace"; Tasks: desktopicon

[Registry]

Root: HKLM; Subkey: "Software\Tracey Technologies\iTrace50"                                                                                ;
Root: HKLM; Subkey: "Software\Tracey Technologies\iTrace50"; ValueType: string; ValueName: "Main Folder"    ; ValueData: "{#MAINFOLDER}"   ; Flags: uninsdeletevalue
Root: HKLM; Subkey: "Software\Tracey Technologies\iTrace50"; ValueType: dword ; ValueName: "Database Type"  ; ValueData: "0"               ;
Root: HKLM; Subkey: "Software\Tracey Technologies\iTrace50"; ValueType: string; ValueName: "Database Folder"; ValueData: "{olddata}"; Check: Exist()
Root: HKLM; Subkey: "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"; ValueType: dword ; ValueName: "EnableLinkedConnections"  ; ValueData: "1"     ;

[Run]
Filename: {tmp}\VcRedist.exe; Parameters: /q; Check: ShouldInstallRedist; BeforeInstall: SetMarqueeProgress(True)

[Code]

function SetupCopyOEMInf(
  SourceInfFileName, OEMSourceMediaLocation: String;
  OEMSourceMediaType, CopyStyle: Longint;
  DestinationInfFileName: Longint;
  DestinationInfFileNameSize: Longint;
  RequiredSize: Longint;
  DestinationInfFileNameComponent: Longint): Longint;
external 'SetupCopyOEMInfA@Setupapi.dll stdcall';

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssDone then begin
    SetupCopyOEMInf(ExpandConstant('{app}\Drivers\Tracey_Lynx.inf'), '', 1, 0, 0, 0, 0, 0);
  end;
end;

function Exist(): boolean;
var
  OrigPath: string;
begin
 if RegQueryStringValue(HKEY_LOCAL_MACHINE, 'Software\Tracey Technologies\iTrace50','Database Folder', OrigPath)
 then begin
    Result := True;
    exit;
 end;

 Result := False;
end;

var
     Y  :String;
     YY :String;
     Y1 :Integer;
     Y2 :Integer;
     Y3 :Integer;
     Y4 :Integer;
function InitializeSetup(): Boolean;
begin
  if NOT RegValueExists(HKEY_LOCAL_MACHINE,'Software\Tracey Technologies\iTrace50','Inst600')
  then begin

      Y := GetDateTimeString('dd/mm/yyyy', '-', ':');
      Y2 :=StringChange(Y, ' ', '');
      Y2 :=StringChange(Y, '-', '');
      Y2 :=StringChange(Y, ':', '');
      Y3 := StrToInt(Y);
      //MsgBox(Y,mbInformation,MB_OK);

      Y := GetDateTimeString('hh:nn:ss', '-', ':');
      Y2 :=StringChange(Y, ' ', '');
      Y2 :=StringChange(Y, '-', '');
      Y2 :=StringChange(Y, ':', '');

      Y4 := StrToInt(Y);

      Y1 := (Y3+Y4)*2;

      YY := IntToStr(Y1);
		 // MsgBox(Y,mbInformation,MB_OK);
      //MsgBox(YY,mbInformation,MB_OK);

     RegWriteStringValue(HKEY_LOCAL_MACHINE,'Software\Tracey Technologies\iTrace50','Inst600',YY);
  end;

  Result := True;
end;

Function GetVcredistBuild() : Integer;
var
  Bld: Cardinal;
Begin
  if RegQueryDwordValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86', 'Bld', Bld) then
    Result := Bld
  else if RegQueryDwordValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\WOW6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\x86', 'Bld', Bld) then
    Result := Bld
  else
    Result := 0;
End;

Function ShouldInstallRedist() : boolean;
Begin
  if (GetVcredistBuild() < {#VCREDIST_VERSION}) then
     Result := true
  else
     Result := false;
End;

procedure SetMarqueeProgress(Marquee: Boolean);
begin
  if Marquee then
  begin
    WizardForm.ProgressGauge.Style := npbstMarquee;
  end
    else
  begin
    WizardForm.ProgressGauge.Style := npbstNormal;
  end;
end;