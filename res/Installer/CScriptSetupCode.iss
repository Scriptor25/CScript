; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName      "CScript"
#define MyAppVersion   "1.0"
#define MyAppPublisher "Scriptor25"
#define MyAppURL       "https://github.com/Scriptor25"
#define MyAppExeName   "CScriptRun.exe"
#define MyAppAssocName MyAppName + " File"
#define MyAppAssocExt  ".csc"
#define MyAppAssocKey  StringChange(MyAppAssocName, " ", "") + MyAppAssocExt

#define MyAppLicense   "license.txt"
#define MyAppPreInfo   "preinstall.txt"
#define MyAppPostInfo  "postinstall.txt"
#define MyAppIcon      "..\icon.ico"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId=     {{B52E3D66-8E90-47BE-B176-8406F013F4F3}
AppName=   {#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher=           {#MyAppPublisher}
AppPublisherURL=        {#MyAppURL}
AppSupportURL=          {#MyAppURL}
AppUpdatesURL=          {#MyAppURL}
DefaultDirName=         {autopf}\{#MyAppName}
ChangesAssociations=    yes
DisableProgramGroupPage=yes
LicenseFile=            {#MyAppLicense}
InfoBeforeFile=         {#MyAppPreInfo}
InfoAfterFile=          {#MyAppPostInfo}
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputBaseFilename=CScript-Setup
SetupIconFile=     {#MyAppIcon}
Compression=       lzma
SolidCompression=  yes

VersionInfoCompany=    {#MyAppPublisher}
VersionInfoProductName={#MyAppName}
VersionInfoVersion=    {#MyAppVersion}

WizardStyle=    modern
WizardImageFile=guitar.bmp

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "german";  MessagesFile: "compiler:Languages\German.isl"

[Types]
Name: "full";   Description: "Full installation"                    
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
Name: "main";     Description: "Main Files";         Types: full custom; Flags: fixed
Name: "udl";      Description: "Notepad++ UDL File"; Types: full
Name: "examples"; Description: "Example Files";      Types: full
Name: "source";   Description: "Source Code";        Types: full

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\..\x64\Release\{#MyAppExeName}";  DestDir: "{app}";          Components: main;     Flags: ignoreversion
Source: "..\CScript.xml";                     DestDir: "{app}\language"; Components: udl;      Flags: ignoreversion
Source: "..\Examples\*.csc";                  DestDir: "{app}\examples"; Components: examples; Flags: ignoreversion
Source: "..\..\src\*";                        DestDir: "{app}\source";   Components: source;   Flags: ignoreversion recursesubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Registry]
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt}\OpenWithProgids";            ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: "";                                Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}";                            ValueType: string; ValueName: "";                 ValueData: "{#MyAppAssocName}";               Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\DefaultIcon";                ValueType: string; ValueName: "";                 ValueData: "{app}\{#MyAppExeName},0"
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\open\command";         ValueType: string; ValueName: "";                 ValueData: """{app}\{#MyAppExeName}"" ""%1"""
Root: HKA; Subkey: "Software\Classes\Applications\{#MyAppExeName}\SupportedTypes"; ValueType: string; ValueName: ".myp";             ValueData: ""

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}";  Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

