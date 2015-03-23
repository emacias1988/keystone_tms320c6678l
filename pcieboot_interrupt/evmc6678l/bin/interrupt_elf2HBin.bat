set C6000_CG_DIR="C:\Program Files\Texas Instruments\C6000 Code Generation Tools 7.4.0"
set TOOL_DIR="..\..\..\..\..\..\"
set TARGET=6678
set ENDIAN=little
set PATH=%PATH%;%SystemRoot%\system32;%SystemRoot%;


@echo off

echo C6000_CG_DIR set as: %C6000_CG_DIR%
echo TARGET set as: %TARGET%
echo IBL_ROOT_DIR set as : %IBL_ROOT_DIR%

echo Converting .out to HEX ...
if %ENDIAN% == little (
%C6000_CG_DIR%\bin\hex6x -order L interrupt_image.rmd pcieboot_interrupt_evm%TARGET%l.out
) else (
%C6000_CG_DIR%\bin\hex6x -order M interrupt_image.rmd pcieboot_interrupt_evm%TARGET%l.out
)

..\..\..\..\..\..\bttbl2hfile\Bttbl2Hfile pcieboot_interrupt.btbl pcieboot_interrupt.h pcieboot_interrupt.bin

..\..\..\..\..\..\hfile2array\hfile2array pcieboot_interrupt.h pcieInterrupt.h pcieInterrupt

if %ENDIAN% == little (
move pcieInterrupt.h ..\..\..\linux_host_loader\LE\pcieInterrupt_%TARGET%.h
) else (
move pcieInterrupt.h ..\..\..\linux_host_loader\BE\pcieInterrupt_%TARGET%.h
)
