#!/bin/bash
ROOT_PATH=$PWD
make rebuild || exit -1

APP_BUILD_DATE=$(date --date="+12 hours" +%Y%m%d)
APP_PACKET_NAME=STM32F103ZETx_$APP_BUILD_DATE.hex

$ROOT_PATH/boot/stm32_mkimage -A arm -O rtthread -T kernel -C none -a 0x08020040 -e 0x08020041 -n "rtthread app" -d out/bin/app.bin out/app.img
$ROOT_PATH/boot/stm32_bin2hex 0x08020000 out/app.img out/app.hex
$ROOT_PATH/boot/stm32_bin2hex 0x08000000 boot/iboot.bin out/iboot.hex
$ROOT_PATH/boot/stm32_hexmerge out/$APP_PACKET_NAME out/app.hex out/iboot.hex
cp out/$APP_PACKET_NAME /mnt/hgfs/share