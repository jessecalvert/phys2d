@REM @Author: Jesse
@REM @Date:   2016-08-26 15:46:32
@REM @Last Modified by:   Jesse
@REM Modified time: 2017-07-16 22:50:29

@echo off

set CommonCompilerFlags=-Od -MTd -nologo -Gm- -GR- -EHa- -Oi -WX -W4 -wd4127 -wd4201 -wd4100 -wd4189 -wd4505 -FC -Z7 -F0x1000000
set CommonCompilerFlags=-DPHYS2D_DEBUG=1 -DPHYS2D_WIN32=1 %CommonCompilerFlags%
set CommonLinkerFlags= -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib opengl32.lib

IF NOT EXIST W:\phys2d\build mkdir W:\phys2d\build
pushd W:\phys2d\build
W:\ctime\ctime -begin phys2d.ctm
del *.pdb > NUL 2> NUL

REM cl %CommonCompilerFlags% -wd4456 W:\phys2d\code\asset_builder.cpp /link %CommonLinkerFlags%
REM cl %CommonCompilerFlags% -wd4456 W:\phys2d\code\simple_meta.cpp /link %CommonLinkerFlags%

REM 64-bit build
REM Optimization switches /O2
echo WAITING FOR PDB > lock.tmp
REM -EXPORT:GameGetSoundSamples  -EXPORT:DEBUGGameFrameEnd
w:\phys2d\build\simple_meta.exe w:\phys2d\code phys2d.cpp generated.h
cl %CommonCompilerFlags% W:\phys2d\code\phys2d.cpp -Fmphys2d.map -LD /link -incremental:no -opt:ref -PDB:phys2d_%random%.pdb -EXPORT:GameUpdate -EXPORT:GameRender -EXPORT:DebugFrameEnd
del lock.tmp
cl %CommonCompilerFlags% W:\phys2d\code\win32_phys2d.cpp -Fmwin32_phys2d.map /link %CommonLinkerFlags%

W:\ctime\ctime -end phys2d.ctm
popd
