@ECHO OFF
SETLOCAL ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

CALL candle/vcenv.bat

CALL candle/build.bat /SAUCES resauces
SET /p LIBS=<candle\build\libs

cl main.c !LIBS!
