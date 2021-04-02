rmdir /Q Domain
rmdir /Q Feather
rmdir /Q "..\ParcLib\Feather\Domain"

mklink /D Domain "..\ParcLib\Domain"
mklink /D Feather "..\ParcLib\Feather"
mklink /D "Feather\Domain" "..\..\ParcLib\Domain"
