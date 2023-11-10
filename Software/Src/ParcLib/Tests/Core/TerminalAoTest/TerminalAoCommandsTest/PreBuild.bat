rmdir /Q Core
rmdir /Q Feather
rmdir /Q "Feather\Core"

mklink /D Core "..\..\..\..\Core"
REM mkdir Tests\Core\TerminalAoTest\
copy ..\TerminalAoTestShared.h .\TerminalAoTestShared.h
