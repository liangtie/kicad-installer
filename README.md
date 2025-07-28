# KiCad Installer

https://windowsforum.com/threads/get-all-cli-parameters-switches-from-nsis-installer.337804/


<!-- Displays help -->
./kicad-huaqiu-9.0.3-r1-x86_64.exe /? 

<!-- The dir can be new dir ,which will be created -->

<!-- Requires admin privileges -->
.\kicad-huaqiu-9.0.3-r1-x86_64.exe  /allusers /S  /D=C:\kicad

.\kicad-huaqiu-9.0.3-r1-x86_64.exe  /currentuser /S  /D=C:\kicad


<!-- Not running in Silent mode -->

.\kicad-huaqiu-9.0.3-r1-x86_64.exe  /allusers /D=C:\kicad

.\kicad-huaqiu-9.0.3-r1-x86_64.exe  /currentuser /D=C:\kicad

## Compress

upx --best .\kicad-installer.exe