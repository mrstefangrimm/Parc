rmdir /Q Core
rmdir /Q Feather
rmdir /Q "..\ParcLib\Feather\Core"

mklink /D Core "..\ParcLib\Core"
mklink /D Feather "..\ParcLib\Feather"
