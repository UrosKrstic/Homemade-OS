@echo off
cd c:\bc31\bin
bcc +cmpsw.txt %1 APPLICAT.LIB
@echo on
pause