@echo off
IF EXIST "C:\Program Files\NSIS\makensis.exe" SET NSIS="C:\Program Files\NSIS\makensis"
IF EXIST "C:\Program Files (x86)\NSIS\makensis.exe" SET NSIS="C:\Program Files (x86)\NSIS\makensis"

@echo on

%NSIS% /V1 /DVERSION=1.1 Chouine.nsi

pause
