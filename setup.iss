#define MyAppName "Cake"
#define MyAppVersion "5.5-5"
#define MyAppPublisher "lanthora"
#define MyAppURL "https://github.com/lanthora/cake"
#define MyAppExeName "cake.exe"
#define MyAppBuildDir "artifacts"
#define MyAppOutputDir "installer"
#define MyAppLogo "logo.ico"

[Setup]
AppId={{5068A825-71E9-41D2-A72D-E7260E187122}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName=C:\Program Files\{#MyAppName}
DisableProgramGroupPage=yes
OutputDir={#MyAppOutputDir}
OutputBaseFilename={#MyAppName}-{#MyAppVersion}
SetupIconFile={#MyAppLogo}
Compression=lzma
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#MyAppBuildDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#MyAppLogo}"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: {app}\{#MyAppLogo}
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: {app}\{#MyAppLogo}; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Flags: nowait postinstall runascurrentuser
