# Ecat2can 

## 使用说明：
* ### 烧写eeprom
    * 在ubuntu下编译soem
    * 运行soem/build/test/linux下的程序(需要root权限) 
        ```
        ./eepromtool/eepromtool enp4s0 1 -w /home/evence/code_file/Ethercat_ws/Ecat2Can/1.Firmware/stm32F4_ax58100_hw/Lib/soes-esi/eeprom.bin
        ```
    * 其中/home/evence/code_file/Ethercat_ws/Ecat2Can/1.Firmware/stm32F4_ax58100_hw/Lib/soes-esi/eeprom.bin 为对应固件 lib/soes-esi下的.bin文件
* ### 烧写固件
    * 没有openocd环境的直接用elf烧写就行
* ### 启动主站
    * 需要root权限，运行test_demo ros版本humble ubuntu版本22.04。
