# MineFLTK

Minesweeper written with C++/FLTK. 
Can be compiled with Visual Studio.
Can be ported on Linux, but it has WinAPI calls in protected version and installer uses Visual Studio specific feature (resource file).
Intended to show some weird file-based DRM.

## Structure

Repository consist of 3 projects:
- MineFLTK
- MineFLTKDRM
- Installer

### MineFLTK

Just working game without any protection.

### MineFLTKDRM

Modified MineFLTK. Protection code can be found in MainWindow.cpp
There is encrypted folder path in spath (xor encryption)
First click triggers checking of program launches left.
Program flow was intentionally obscured.

Checking includes:
- Files count (not all files contain userful information, but all validated)
- File time modification
- Basic CRC
- Keys
- Hashes
- Username

### Installer

Installation steps could be seen in file InstallThread.cpp.
Compiled .exe file from MineFLTKDRM should be manually placed in Installer project.
File will be extracted from resources of Installer.exe after creating files for DRM.

## Build

For building these projects you should install and configure FLTK.
Building Installer could be done only after building MineFLTKDRM and making copy of resulted exe in Installer.rc
