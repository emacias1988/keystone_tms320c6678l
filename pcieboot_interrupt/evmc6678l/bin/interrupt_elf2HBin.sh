export C6000_CG_DIR=~/ti/TI_CGT_C6000_7.4.0
export TOOL_DIR="../../../../../../"
export TARGET=6678
export ENDIAN=little
export PATH=${PATH};${SystemRoot}/system32;${SystemRoot};

echo CGT_INSTALL_DIR set as: ${CGT_INSTALL_DIR}
echo TARGET set as: ${TARGET}
echo IBL_ROOT_DIR set as : ${IBL_ROOT_DIR}

echo Converting .out to HEX ...
if [ ${ENDIAN} == little ]
then
${CGT_INSTALL_DIR}/bin/hex6x -order L interrupt_image.rmd pcieboot_interrupt_evm${TARGET}l.out
else
${CGT_INSTALL_DIR}/bin/hex6x -order M interrupt_image.rmd pcieboot_interrupt_evm${TARGET}l.out
fi

../../../../../../bttbl2hfile/Bttbl2Hfile pcieboot_interrupt.btbl pcieboot_interrupt.h pcieboot_interrupt.bin

../../../../../../hfile2array/hfile2array pcieboot_interrupt.h pcieInterrupt.h Interrupt

if [ ${ENDIAN} == little ]
then
mv pcieBootCode.h ../../../linux_host_loader/LE/pcieInterrupt_${TARGET}.h
else
mv pcieBootCode.h ../../../linux_host_loader/BE/pcieInterrupt_${TARGET}.h
fi