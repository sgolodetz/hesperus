@echo off

IF [%1]==[] GOTO Finished

cd %1
set PATH=%PATH%;..\..\..\tools

mef2input %1.mef %1.rbr %1.ent %1.lum

REM Rendering Section

hcsg -r %1.rbr %1.rg1
hbsp -r %1.rg1 %1.rt1
hportal -r %1.rt1 %1.rp1
hflood -r %1.rt1 %1.rp1 %1.ent %1.rg2
hbsp -r %1.rg2 %1.rt2
hportal -r %1.rt2 %1.rp2
hvis %1.rp2 %1.vis
hlight %1.rt2 %1.vis %1.lum LM %1.lbt

REM Collision Section

hr2cbrush %1.rbr %1.cbr
hexpand ..\..\settings\AABBs.txt %1.cbr

FOR %%f IN (*.ebr) DO (
hcsg -c %%f %%~nf.cg1
hbsp -c %%~nf.cg1 %%~nf.ct1
hportal -c %%~nf.ct1 %%~nf.cp1
hflood -c %%~nf.ct1 %%~nf.cp1 %1.ent %%~nf.cg2
)

SETLOCAL ENABLEDELAYEDEXPANSION
set CG2FILES=
FOR %%f IN (*.cg2) DO (
set CG2FILES=!CG2FILES! %%f
)
echo hobsp %CG2FILES% %1.obt
set CG2FILES=
ENDLOCAL

echo hoportal %1.obt %1.opr
echo hnav %1.obt %1.opr %1.nav

REM Collation Section (this will need updating ultimately)

hcollate +L %1.lbt %1.rp2 %1.vis %1.bsp

REM Cleanup Rendering Section

del *.rbr
del *.rg*
del *.rt*
del *.rp*
del *.vis
del *.lbt
del *.bmp

REM Cleanup Collision Section

del *.cbr
del *.ebr
del *.cg*
del *.ct*
del *.cp*

cd ..

:Finished