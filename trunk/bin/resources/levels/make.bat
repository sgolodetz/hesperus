@echo off

IF [%1]==[] GOTO Finished

cd %1
set PATH=%PATH%;..\..\..\tools

mef2input %1.mef %1.rbr %1.ent %1.lum

REM #################
REM Rendering Section
REM #################

hcsg -r %1.rbr %1.rg1
hbsp -r %1.rg1 %1.rt1
hportal -r %1.rt1 %1.rp1
hflood -r %1.rt1 %1.rp1 %1.ent %1.rg2
hbsp -r %1.rg2 %1.rt2
hportal -r %1.rt2 %1.rp2
hvis %1.rp2 %1.vis
hlight %1.rt2 %1.vis %1.lum LM %1.lbt

REM #################
REM Collision Section
REM #################

hr2cbrush %1.rbr %1.cbr
hexpand ..\..\settings\hesperus.edf %1.cbr

FOR %%f IN (*.ebr) DO (
hcsg -c %%f %%~nf.cg1
hbsp -c %%~nf.cg1 %%~nf.ct1
hportal -c %%~nf.ct1 %%~nf.cp
hflood -c %%~nf.ct1 %%~nf.cp %1.ent %%~nf.cg2
hbsp -c %%~nf.cg2 %%~nf.ct2
)

SETLOCAL ENABLEDELAYEDEXPANSION
set GEOMTREEPAIRS=
FOR %%f IN (*.cg2) DO (
set GEOMTREEPAIRS=!GEOMTREEPAIRS! %%f %%~nf.ct2
)
hobsp -c%GEOMTREEPAIRS% %1.ot
set GEOMTREEPAIRS=
ENDLOCAL

hoportal -c %1.ot %1.op
hnav ..\..\settings\hesperus.edf %1.ot %1.nav

REM #################
REM Collation Section (this will need updating ultimately)
REM #################

hcollate +L %1.lbt %1.rp2 %1.vis %1.ot %1.op %1.nav %1.bsp

IF [%2]==[noclean] GOTO SkipCleanup

REM #########################
REM Cleanup Rendering Section
REM #########################

del *.rbr
del *.rg*
del *.rt*
del *.rp*
del *.vis
del *.lbt
del *.bmp

REM #########################
REM Cleanup Collision Section
REM #########################

del *.cbr
del *.ebr
del *.cg*
del *.ct*
del *.cp
del *.ot
del *.op
del *.nav

:SkipCleanup

cd ..

:Finished