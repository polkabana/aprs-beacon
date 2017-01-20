Arduino APRS Beacon
===================



```
Pin-Mapping:
ESP8266	MAX7219
5V	Vcc
G	Gnd
D5	CLK
D8	CS
D7	DIN
```


Used firmware esp8266-20161007-v1.8.4-147-gdc43508.bin

Upload firmware:
```esptool.py --port COM4 --baud 460800 write_flash --flash_size=8m 0 esp8266-20161007-v1.8.4-147-gdc43508.bin```

Release esp8266-20160909-v1.8.4.bin was very strange. It have file that confilcts with http_client.py
