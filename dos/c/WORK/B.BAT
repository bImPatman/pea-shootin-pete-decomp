@echo off
cd \WORK
bcc -ml -1 -O2 -r- -c SAMEMOD.C >> ..\OUT\LOG.TXT
if not exist SAMEMOD.OBJ goto failed
bcc -ml -1 -O2 -r- -S SAMEMOD.C >> ..\OUT\LOG.TXT
if not exist SAMEMOD.ASM goto failed
bcc -ml -1 -O2 -r- -c G13B2.C >> ..\OUT\LOG.TXT
if not exist G13B2.OBJ goto failed
bcc -ml -1 -O2 -r- -S G13B2.C >> ..\OUT\LOG.TXT
if not exist G13B2.ASM goto failed
bcc -ml -1 -O2 -r- -c XE25978.C >> ..\OUT\LOG.TXT
if not exist XE25978.OBJ goto failed
bcc -ml -1 -O2 -r- -S XE25978.C >> ..\OUT\LOG.TXT
if not exist XE25978.ASM goto failed
bcc -ml -1 -O2 -r- -c XE256FE.C >> ..\OUT\LOG.TXT
if not exist XE256FE.OBJ goto failed
bcc -ml -1 -O2 -r- -S XE256FE.C >> ..\OUT\LOG.TXT
if not exist XE256FE.ASM goto failed
bcc -ml -1 -O2 -r- -c XE25B0F.C >> ..\OUT\LOG.TXT
if not exist XE25B0F.OBJ goto failed
bcc -ml -1 -O2 -r- -S XE25B0F.C >> ..\OUT\LOG.TXT
if not exist XE25B0F.ASM goto failed
tlink @M013B.RSP >> ..\OUT\LOG.TXT
if not exist ..\OUT\M013B.EXE goto failed
echo BUILD_OK >> ..\OUT\LOG.TXT
exit
:failed
echo BUILD_FAILED >> ..\OUT\LOG.TXT
exit
