# I2C Self-Test Example 
对EEPRom进行读写稳定性试验
I2C_MASTER_SCL_IO 33    
I2C_MASTER_SDA_IO 32     
BLINK_BLUE_GPIO 21
BLINK_Yellow_GPIO 17

# 试验方法 2021.9.15
1、电脑串口发送9字节的数据，
2、当mcu判断接收到数据后，将EEPROM中的指定位置全部置1，并将串口数据写入到指定内存中；
3、EEPROM读取这个位置的数据；
4、在EEPROM中的指定位置写入固定的数据；
5、EEPROM读取写入的固定数据；
6、判断存储的串口发送的数据在EEPROM中有没有改变，改变就亮红灯，没变就亮蓝灯。

## 2021.9.16 
1、通过在人感的板子上试验EEPROM的写保护；
2、GPIO15为控制WP引脚；
3、当不用GPI05控制时，直接用按键进行控制，案件按下WP=0；按键松开WP=1；
x-special/nautilus-clipboard
copy
file:///tmp/VMwareDnD/MtbUn9/IMG_20210916_110602.jpg
x-special/nautilus-clipboard
copy
file:///tmp/VMwareDnD/KawUI7/IMG_20210916_110616.jpg
