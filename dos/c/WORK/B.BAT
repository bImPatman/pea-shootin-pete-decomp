@echo off
cd \WORK
bcc -ml -1 -O1 -r- -c SAMEMOD.C >> ..\OUT\LOG.TXT
if not exist SAMEMOD.OBJ goto failed
bcc -ml -1 -O1 -r- -S SAMEMOD.C >> ..\OUT\LOG.TXT
if not exist SAMEMOD.ASM goto failed
tlink @FARSTREQ.RSP >> ..\OUT\LOG.TXT
if not exist ..\OUT\FARSTREQ.EXE goto failed
echo BUILD_OK >> ..\OUT\LOG.TXT
exit
:failed
echo BUILD_FAILED >> ..\OUT\LOG.TXT
exit
