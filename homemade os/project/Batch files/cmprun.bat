@echo off
SET options=-mh -y -v -Ic:\bc31\include;%projdir%\include -Lc:\bc31\lib;%projdir%\lib -n%projdir%\output -eos.exe
cd c:\bc31\bin
bcc +cmpsw.txt %1 APPLICAT.LIB
cd %projdir%\output
os
@echo on
pause