@echo off

dir *.bmp /b > a

findstr /r /v "HOG" a > b

for /f "delims=" %%i in (b) do (
	echo �t�@�C����: %%i
	
	echo %%i > c
	HOGCreater < c

)
pause