@echo off

dir *.bmp /b > a

findstr /r /v "HOG" a > b

for /f "delims=" %%i in (b) do (
	echo ƒtƒ@ƒCƒ‹–¼: %%i
	
	echo %%i > c
	HOGCreater < c

)
pause