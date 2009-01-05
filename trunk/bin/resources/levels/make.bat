@echo off

IF [%1]==[] GOTO Finished

cd %1
set PATH=%PATH%;..\..\..\tools

mef2input %1.mef %1.bru %1.ent %1.lum
hcsg -r %1.bru %1.rg1
hbsp -r %1.rg1 %1.rt1
hportal -r %1.rt1 %1.pr1
hflood -r %1.rt1 %1.pr1 %1.ent %1.rg2
hbsp -r %1.rg2 %1.rt2
hportal -r %1.rt2 %1.pr2
hvis %1.pr2 %1.vis
hlight %1.rt2 %1.vis %1.lum LM %1.lbt
hcollate +L %1.lbt %1.pr2 %1.vis %1.bsp

del *.bru
del *.rg*
del *.rt*
del *.pr*
del *.vis
del *.lbt
del *.bmp

cd ..

:Finished