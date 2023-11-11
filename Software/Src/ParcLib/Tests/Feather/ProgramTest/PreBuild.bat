rmdir /Q Core
rmdir /Q "Feather\Core"
rmdir /Q Feather

mklink /D Core "..\..\..\Core"
mklink /D Feather "..\..\..\Feather"
mklink /D "Feather\Core" "..\Core"
