@echo Updating Break-It... > dump.txt
:LOOP
tasklist /fi "imagename eq Break-It.exe" 2>NUL | find /i /n "Break-It.exe" >NUL
if "%ERRORLEVEL%"=="0" (
  @echo Breat-It running... >> dump.txt
  sleep 1
  goto LOOP
) else (
  @echo Breat-It stopped. Continue >> dump.txt
  goto CONTINUE
)

:CONTINUE
@echo Copy files from "temp_files" >> dump.txt
xcopy .\..\temp_files .\.. /s /e /y >> dump.txt

@echo Restarting... >> dump.txt
cd ..\
start "" .\Break-It.exe

@echo Removing temporary files... >> patcher\dump.txt
rmdir ".\temp_files" /s /q