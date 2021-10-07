# Parc Software Build Procedure

The build steps to build the Parc simulator and the PCB production files.

## 0. Preparation

Required software:

- Microsoft Visual Studio 2019
- Fitzing

Create a package folder.

## 1. Build Simulator

1. In Visual Studio, open 'WinParcApp.sln' from '\Software\Src\WinParcApp'
2. Set 'WinParc.App' as startup project
3. Set the build configuration to 'Release' and 'x64'
4. Build the solution
5. Copy the 'Release' folder from '\Software\Src\WinParcApp' to the package folder and rename it to Simulator
6. Delete all files except `*.exe` and `*.dll`

## 2. Create PCB Gerber files 

1. In Fritzing, open 'Keypad-v2.fzz'
2. Create folder 'Keypad-v2-Gerber'
3. File, Export, for Production, Extended Gerber (RS-274x)
4. Compress the folder to 'Keypad-v2-Gerber.zip'

## 3. Copy projects

1. Copy the 'ParcLib' folder from '\Software\Src' to the package folder
2. Copy the 'ParcApp' folder from '\Software\Src' to the package folder
3. Copy the 'PcbParcApp' folder from '\Software\Src' to the package folder
4. Copy the 'UnoParcApp' folder from '\Software\Src' to the package folder
5. In all folders form above, remove the file `PreBuild.bat`
6. In all the folders form above,  remove the links to the folders 'Core' and 'Feather' (don't forget the Test-folders in the ParcLib)

## 4. Finalize the package

Package structure:

package_v0_0_1_0.zip/

​	/Software/Simulator/*

​	/Software/ParcLib/*

​	/Software/ParcApp/*

​	/Software/PcbParcApp/*

​	/Software/UnoParcApp/*

​	/Hardware/Keypad-v2-Gerber.zip

​	/Hardware/Keypad-v2.fzz



