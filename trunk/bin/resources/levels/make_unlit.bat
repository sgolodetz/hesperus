@echo off

IF [%1]==[] GOTO Finished

cd %1
set PATH=%PATH%;..\..\..\tools

mef2input %1.mef %1.bru %1.ent %1.lum
hdivide %1.bru %1.rbr %1.cbr %1.dbr %1.hgm %1.sbr

REM #################
REM Rendering Section
REM #################

hcsg -r %1.rbr %1.rg1
hbsp -r %1.rg1 %1.hgm %1.rt1
hportal -r %1.rt1 %1.rp1
hflood -r %1.rt1 %1.rp1 %1.rg2
hbsp -r %1.rg2 %1.hgm %1.rt2
hportal -r %1.rt2 %1.rp2
hvis %1.rp2 %1.vis
hdetail %1.rt2 %1.dbr %1.rt3

REM #################
REM Collision Section
REM #################

hexpand ..\..\settings\test-entdef.xml %1.cbr

FOR %%f IN (*.ebr) DO (
hcsg -c %%f %%~nf.cg1
hbsp -c %%~nf.cg1 nohints %%~nf.ct1
hportal -c %%~nf.ct1 %%~nf.cp
hflood -c %%~nf.ct1 %%~nf.cp %%~nf.cg2
hbsp -c %%~nf.cg2 nohints %%~nf.ct2
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
hnav ..\..\settings\test-entdef.xml %1.ot %1.nav

REM #################
REM Collation Section
REM #################

hcollate -L %1.rt3 %1.rp2 %1.vis %1.ot %1.op %1.nav %1.ent %1.bsp

IF [%2]==[noclean] GOTO SkipCleanup

REM #######################
REM General Cleanup Section
REM #######################

del *.bru
del *.sbr

REM #########################
REM Cleanup Rendering Section
REM #########################

del *.dbr
del *.hgm
del *.lum
del *.rbr
del *.rg*
del *.rp*
del *.rt*
del *.vis

REM #########################
REM Cleanup Collision Section
REM #########################

del *.cbr
del *.cg*
del *.cp
del *.ct*
del *.ebr
del *.nav
del *.op
del *.ot

:SkipCleanup

cd ..

:Finished