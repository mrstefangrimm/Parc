rmdir /Q Domain
rmdir /Q "Feather\Domain"
rmdir /Q Feather

mklink /D Domain "..\..\..\Domain"
mklink /D Feather "..\..\..\Feather"
mklink /D "Feather\Domain" "..\..\..\Software\ParcLib\Domain"
