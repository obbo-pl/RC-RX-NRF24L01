EESchema Schematic File Version 4
LIBS:RX-Module-nRF24L01-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "RX-Module NRF24L01 G-24"
Date ""
Rev ""
Comp "obbo.pl"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RF:NRF24L01_Breakout U4
U 1 1 5D56B0CB
P 9700 4650
F 0 "U4" H 10080 4696 50  0000 L CNN
F 1 "NRF24L01_Breakout" H 10080 4605 50  0000 L CNN
F 2 "obbo_footprints:nRF24L01-GT24_Breakuot_PinHeader_2x04_Solder" H 9850 5250 50  0001 L CIN
F 3 "http://www.nordicsemi.com/eng/content/download/2730/34105/file/nRF24L01_Product_Specification_v2_0.pdf" H 9700 4550 50  0001 C CNN
	1    9700 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal_GND24 Y1
U 1 1 5D56F478
P 1600 6700
F 0 "Y1" V 1400 6500 50  0000 L CNN
F 1 "16MHz" V 1400 6750 50  0000 L CNN
F 2 "obbo_footprints:Crystal_SMD_5032-4Pin_5.0x3.2mm_HandSoldering" H 1600 6700 50  0001 C CNN
F 3 "~" H 1600 6700 50  0001 C CNN
	1    1600 6700
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR029
U 1 1 5D57053A
P 3400 7050
F 0 "#PWR029" H 3400 6800 50  0001 C CNN
F 1 "GND" H 3405 6877 50  0000 C CNN
F 2 "" H 3400 7050 50  0001 C CNN
F 3 "" H 3400 7050 50  0001 C CNN
	1    3400 7050
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR04
U 1 1 5D570CE4
P 10200 1050
F 0 "#PWR04" H 10200 900 50  0001 C CNN
F 1 "+3V3" H 10215 1223 50  0000 C CNN
F 2 "" H 10200 1050 50  0001 C CNN
F 3 "" H 10200 1050 50  0001 C CNN
	1    10200 1050
	1    0    0    -1  
$EndComp
$Comp
L MCU_Microchip_ATmega:ATmega328-AU U3
U 1 1 5DA68ED8
P 5600 4100
F 0 "U3" H 5800 2600 50  0000 C CNN
F 1 "ATmega328-AU" H 6050 2500 50  0000 C CNN
F 2 "Package_QFP:TQFP-32_7x7mm_P0.8mm" H 5600 4100 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 5600 4100 50  0001 C CNN
	1    5600 4100
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR01
U 1 1 5DA6B095
P 6450 1050
F 0 "#PWR01" H 6450 900 50  0001 C CNN
F 1 "+BATT" H 6465 1223 50  0000 C CNN
F 2 "" H 6450 1050 50  0001 C CNN
F 3 "" H 6450 1050 50  0001 C CNN
	1    6450 1050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5DA6B636
P 4000 4400
F 0 "R4" H 4100 4450 50  0000 L CNN
F 1 "47k*" H 4100 4350 50  0000 L CNN
F 2 "obbo_footprints:R_0805K" V 3930 4400 50  0001 C CNN
F 3 "~" H 4000 4400 50  0001 C CNN
	1    4000 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 5DA6BAC2
P 4000 5100
F 0 "R6" H 4070 5146 50  0000 L CNN
F 1 "10k" H 4070 5055 50  0000 L CNN
F 2 "obbo_footprints:R_0805K" V 3930 5100 50  0001 C CNN
F 3 "~" H 4000 5100 50  0001 C CNN
	1    4000 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR024
U 1 1 5DA6BD7B
P 4000 5350
F 0 "#PWR024" H 4000 5100 50  0001 C CNN
F 1 "GND" H 4005 5177 50  0000 C CNN
F 2 "" H 4000 5350 50  0001 C CNN
F 3 "" H 4000 5350 50  0001 C CNN
	1    4000 5350
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR018
U 1 1 5DA6C151
P 4000 4100
F 0 "#PWR018" H 4000 3950 50  0001 C CNN
F 1 "+BATT" H 4015 4273 50  0000 C CNN
F 2 "" H 4000 4100 50  0001 C CNN
F 3 "" H 4000 4100 50  0001 C CNN
	1    4000 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 4100 4000 4250
Wire Wire Line
	4000 4550 4000 4750
Wire Wire Line
	4000 5250 4000 5350
Text Label 4150 4750 0    50   ~ 0
VBAT
Wire Wire Line
	4150 4750 4000 4750
Connection ~ 4000 4750
Wire Wire Line
	4000 4750 4000 4950
Wire Wire Line
	9200 1400 9050 1400
Wire Wire Line
	9050 1400 9050 1050
Wire Wire Line
	9200 1500 9050 1500
Wire Wire Line
	9050 1500 9050 1400
Connection ~ 9050 1400
$Comp
L power:GND #PWR09
U 1 1 5DA6F522
P 9600 1850
F 0 "#PWR09" H 9600 1600 50  0001 C CNN
F 1 "GND" H 9605 1677 50  0000 C CNN
F 2 "" H 9600 1850 50  0001 C CNN
F 3 "" H 9600 1850 50  0001 C CNN
	1    9600 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 1850 9600 1800
Wire Wire Line
	10000 1400 10200 1400
$Comp
L Device:R R3
U 1 1 5DA73EA7
P 6900 4150
F 0 "R3" H 6970 4196 50  0000 L CNN
F 1 "10k" H 6970 4105 50  0000 L CNN
F 2 "obbo_footprints:R_0805K" V 6830 4150 50  0001 C CNN
F 3 "~" H 6900 4150 50  0001 C CNN
	1    6900 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 3950 6900 4000
Wire Wire Line
	6900 4400 6900 4300
Text Label 7000 4400 0    50   ~ 0
RESET
Connection ~ 6900 4400
$Comp
L power:GND #PWR028
U 1 1 5DA77434
P 2550 6750
F 0 "#PWR028" H 2550 6500 50  0001 C CNN
F 1 "GND" H 2555 6577 50  0000 C CNN
F 2 "" H 2550 6750 50  0001 C CNN
F 3 "" H 2550 6750 50  0001 C CNN
	1    2550 6750
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 5DA781AE
P 2450 1550
F 0 "J1" H 2530 1592 50  0000 L CNN
F 1 "Conn_01x03" H 2530 1501 50  0000 L CNN
F 2 "obbo_footprints:PinHeader_3x01_P2.54mm_Horizontal" H 2450 1550 50  0001 C CNN
F 3 "~" H 2450 1550 50  0001 C CNN
	1    2450 1550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J2
U 1 1 5DA78967
P 2450 2000
F 0 "J2" H 2530 2042 50  0000 L CNN
F 1 "Conn_01x03" H 2530 1951 50  0000 L CNN
F 2 "obbo_footprints:PinHeader_3x01_P2.54mm_Horizontal" H 2450 2000 50  0001 C CNN
F 3 "~" H 2450 2000 50  0001 C CNN
	1    2450 2000
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J3
U 1 1 5DA78CAD
P 2450 2450
F 0 "J3" H 2530 2492 50  0000 L CNN
F 1 "Conn_01x03" H 2530 2401 50  0000 L CNN
F 2 "obbo_footprints:PinHeader_3x01_P2.54mm_Horizontal" H 2450 2450 50  0001 C CNN
F 3 "~" H 2450 2450 50  0001 C CNN
	1    2450 2450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J4
U 1 1 5DA78F47
P 2450 2900
F 0 "J4" H 2530 2942 50  0000 L CNN
F 1 "Conn_01x03" H 2530 2851 50  0000 L CNN
F 2 "obbo_footprints:PinHeader_3x01_P2.54mm_Horizontal" H 2450 2900 50  0001 C CNN
F 3 "~" H 2450 2900 50  0001 C CNN
	1    2450 2900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J5
U 1 1 5DA792F7
P 2450 3350
F 0 "J5" H 2530 3392 50  0000 L CNN
F 1 "Conn_01x03" H 2530 3301 50  0000 L CNN
F 2 "obbo_footprints:PinHeader_3x01_P2.54mm_Horizontal" H 2450 3350 50  0001 C CNN
F 3 "~" H 2450 3350 50  0001 C CNN
	1    2450 3350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J6
U 1 1 5DA7977C
P 2450 3800
F 0 "J6" H 2530 3842 50  0000 L CNN
F 1 "Conn_01x03" H 2530 3751 50  0000 L CNN
F 2 "obbo_footprints:PinHeader_3x01_P2.54mm_Horizontal" H 2450 3800 50  0001 C CNN
F 3 "~" H 2450 3800 50  0001 C CNN
	1    2450 3800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR022
U 1 1 5DA79A7E
P 9700 5300
F 0 "#PWR022" H 9700 5050 50  0001 C CNN
F 1 "GND" H 9705 5127 50  0000 C CNN
F 2 "" H 9700 5300 50  0001 C CNN
F 3 "" H 9700 5300 50  0001 C CNN
	1    9700 5300
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR014
U 1 1 5DA79E20
P 9700 3400
F 0 "#PWR014" H 9700 3250 50  0001 C CNN
F 1 "+3V3" H 9715 3573 50  0000 C CNN
F 2 "" H 9700 3400 50  0001 C CNN
F 3 "" H 9700 3400 50  0001 C CNN
	1    9700 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C10
U 1 1 5DA7A311
P 2100 6900
F 0 "C10" H 2215 6946 50  0000 L CNN
F 1 "18p" H 2215 6855 50  0000 L CNN
F 2 "obbo_footprints:C_0805K" H 2138 6750 50  0001 C CNN
F 3 "~" H 2100 6900 50  0001 C CNN
	1    2100 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 1800 5600 1850
Wire Wire Line
	5700 2600 5700 2500
Wire Wire Line
	5700 2500 5600 2500
Connection ~ 5600 2500
Wire Wire Line
	5600 2500 5600 2600
$Comp
L Device:C C9
U 1 1 5DA7C0B9
P 2100 6500
F 0 "C9" H 2215 6546 50  0000 L CNN
F 1 "18p" H 2215 6455 50  0000 L CNN
F 2 "obbo_footprints:C_0805K" H 2138 6350 50  0001 C CNN
F 3 "~" H 2100 6500 50  0001 C CNN
	1    2100 6500
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR05
U 1 1 5DA7C2C6
P 1550 1250
F 0 "#PWR05" H 1550 1100 50  0001 C CNN
F 1 "+BATT" H 1565 1423 50  0000 C CNN
F 2 "" H 1550 1250 50  0001 C CNN
F 3 "" H 1550 1250 50  0001 C CNN
	1    1550 1250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 5DA7C5C8
P 1700 5000
F 0 "#PWR020" H 1700 4750 50  0001 C CNN
F 1 "GND" H 1705 4827 50  0000 C CNN
F 2 "" H 1700 5000 50  0001 C CNN
F 3 "" H 1700 5000 50  0001 C CNN
	1    1700 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 5000 1700 4800
Connection ~ 1700 2100
Wire Wire Line
	1700 2100 1700 1650
Connection ~ 1700 2550
Wire Wire Line
	1700 2550 1700 2100
Connection ~ 1700 3000
Wire Wire Line
	1700 3000 1700 2550
Connection ~ 1700 3450
Wire Wire Line
	1700 3450 1700 3000
Connection ~ 1700 3900
Wire Wire Line
	1700 3900 1700 3450
Wire Wire Line
	1550 1250 1550 1550
Connection ~ 1550 3350
Wire Wire Line
	1550 3350 1550 3800
Connection ~ 1550 2900
Wire Wire Line
	1550 2900 1550 3350
Connection ~ 1550 2450
Wire Wire Line
	1550 2450 1550 2900
Connection ~ 1550 2000
Wire Wire Line
	1550 2000 1550 2450
Connection ~ 1550 1550
Wire Wire Line
	1550 1550 1550 2000
$Comp
L power:GND #PWR027
U 1 1 5DA8CE25
P 1050 6750
F 0 "#PWR027" H 1050 6500 50  0001 C CNN
F 1 "GND" H 1055 6577 50  0000 C CNN
F 2 "" H 1050 6750 50  0001 C CNN
F 3 "" H 1050 6750 50  0001 C CNN
	1    1050 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 6750 1050 6700
Wire Wire Line
	1050 6700 1400 6700
Wire Wire Line
	1800 6700 2100 6700
Wire Wire Line
	2550 6700 2550 6750
Wire Wire Line
	2100 6750 2100 6700
Connection ~ 2100 6700
Wire Wire Line
	2100 6700 2550 6700
Wire Wire Line
	2100 6650 2100 6700
Text Label 1300 6250 0    50   ~ 0
xtl1
Text Label 1300 7150 0    50   ~ 0
xtl2
Wire Wire Line
	2100 7150 2100 7050
Wire Wire Line
	2100 6250 2100 6350
Text Label 6350 3500 0    50   ~ 0
xtl1
Text Label 6350 3600 0    50   ~ 0
xtl2
Wire Wire Line
	6350 3500 6200 3500
Wire Wire Line
	6350 3600 6200 3600
$Comp
L Device:C C4
U 1 1 5DA9B575
P 4300 3100
F 0 "C4" H 4415 3146 50  0000 L CNN
F 1 "100n" H 4415 3055 50  0000 L CNN
F 2 "obbo_footprints:C_0805K" H 4338 2950 50  0001 C CNN
F 3 "~" H 4300 3100 50  0001 C CNN
	1    4300 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5DA9BA9F
P 4300 3300
F 0 "#PWR013" H 4300 3050 50  0001 C CNN
F 1 "GND" H 4305 3127 50  0000 C CNN
F 2 "" H 4300 3300 50  0001 C CNN
F 3 "" H 4300 3300 50  0001 C CNN
	1    4300 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3250 4300 3300
Wire Wire Line
	4300 2950 4300 2900
Text Label 6350 2900 0    50   ~ 0
NRF_CSN
Text Label 6350 3200 0    50   ~ 0
MOSI_CH2
Text Label 6350 3300 0    50   ~ 0
MISO_CH3
Text Label 6350 3400 0    50   ~ 0
SCK_CH4
Text Label 6350 5000 0    50   ~ 0
SCK
Text Label 6350 5100 0    50   ~ 0
MOSI
Text Label 6350 5200 0    50   ~ 0
MISO
Wire Wire Line
	6350 3200 6200 3200
Wire Wire Line
	6350 3300 6200 3300
Wire Wire Line
	6350 3400 6200 3400
Wire Wire Line
	6350 5000 6200 5000
Wire Wire Line
	6350 5100 6200 5100
Wire Wire Line
	6350 5200 6200 5200
Wire Wire Line
	9700 4050 9700 3450
Wire Wire Line
	9700 5250 9700 5300
$Comp
L Device:C C6
U 1 1 5DAB7408
P 9900 3650
F 0 "C6" H 10015 3696 50  0000 L CNN
F 1 "100n" H 10015 3605 50  0000 L CNN
F 2 "obbo_footprints:C_0805K" H 9938 3500 50  0001 C CNN
F 3 "~" H 9900 3650 50  0001 C CNN
	1    9900 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5DAB7946
P 9900 3900
F 0 "#PWR016" H 9900 3650 50  0001 C CNN
F 1 "GND" H 9905 3727 50  0000 C CNN
F 2 "" H 9900 3900 50  0001 C CNN
F 3 "" H 9900 3900 50  0001 C CNN
	1    9900 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 3900 9900 3800
Wire Wire Line
	9900 3500 9900 3450
Wire Wire Line
	9900 3450 9700 3450
Connection ~ 9700 3450
Wire Wire Line
	9700 3450 9700 3400
Wire Wire Line
	6200 4400 6900 4400
Wire Wire Line
	6350 2900 6200 2900
$Comp
L Device:R R5
U 1 1 5DAC8E8F
P 8850 5100
F 0 "R5" H 8920 5146 50  0000 L CNN
F 1 "10k" H 8920 5055 50  0000 L CNN
F 2 "obbo_footprints:R_0805K" V 8780 5100 50  0001 C CNN
F 3 "~" H 8850 5100 50  0001 C CNN
	1    8850 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 4850 8850 4850
Text Label 8850 4350 0    50   ~ 0
MOSI
Text Label 8850 4450 0    50   ~ 0
MISO
Text Label 8850 4550 0    50   ~ 0
SCK
Text Label 8850 4650 0    50   ~ 0
NRF_CSN
Wire Wire Line
	8850 4350 9200 4350
Wire Wire Line
	8850 4450 9200 4450
Wire Wire Line
	8850 4550 9200 4550
Wire Wire Line
	8850 4650 9200 4650
$Comp
L Device:C C3
U 1 1 5DADBAD3
P 5250 2050
F 0 "C3" H 5365 2096 50  0000 L CNN
F 1 "100n" H 5365 2005 50  0000 L CNN
F 2 "obbo_footprints:C_0805K" H 5288 1900 50  0001 C CNN
F 3 "~" H 5250 2050 50  0001 C CNN
	1    5250 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5DADE8FF
P 5250 2250
F 0 "#PWR011" H 5250 2000 50  0001 C CNN
F 1 "GND" H 5255 2077 50  0000 C CNN
F 2 "" H 5250 2250 50  0001 C CNN
F 3 "" H 5250 2250 50  0001 C CNN
	1    5250 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2250 5250 2200
Wire Wire Line
	5250 1900 5250 1850
Wire Wire Line
	5250 1850 5600 1850
Connection ~ 5600 1850
Wire Wire Line
	5600 1850 5600 2500
Wire Wire Line
	4300 2900 5000 2900
Text Label 4750 3200 0    50   ~ 0
VBAT
Wire Wire Line
	4750 3200 5000 3200
$Comp
L Connector_Generic:Conn_01x03 J10
U 1 1 5DAF37BA
P 3950 6900
F 0 "J10" H 4030 6942 50  0000 L CNN
F 1 "Conn_01x03" H 4030 6851 50  0000 L CNN
F 2 "obbo_footprints:PinHeader_3x01_P2.54mm_Horizontal" H 3950 6900 50  0001 C CNN
F 3 "~" H 3950 6900 50  0001 C CNN
	1    3950 6900
	1    0    0    -1  
$EndComp
Text Label 3400 6900 0    50   ~ 0
Tx
Text Label 3400 6800 0    50   ~ 0
Rx
Wire Wire Line
	3750 7000 3400 7000
Wire Wire Line
	3400 7000 3400 7050
Wire Wire Line
	3400 6900 3750 6900
Wire Wire Line
	3400 6800 3750 6800
Text Label 6350 4600 0    50   ~ 0
Rx
Text Label 6350 4700 0    50   ~ 0
Tx
Wire Wire Line
	6350 4700 6200 4700
Wire Wire Line
	6350 4600 6200 4600
Text Notes 3850 7150 0    50   ~ 0
Serial
$Comp
L Connector_Generic:Conn_01x03 J11
U 1 1 5DB19D89
P 6250 7050
F 0 "J11" H 6330 7092 50  0000 L CNN
F 1 "Conn_01x03" H 6330 7001 50  0000 L CNN
F 2 "obbo_footprints:PinHeader_3x01_P2.54mm_Horizontal" H 6250 7050 50  0001 C CNN
F 3 "~" H 6250 7050 50  0001 C CNN
	1    6250 7050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR030
U 1 1 5DB1A0CF
P 5700 7200
F 0 "#PWR030" H 5700 6950 50  0001 C CNN
F 1 "GND" H 5705 7027 50  0000 C CNN
F 2 "" H 5700 7200 50  0001 C CNN
F 3 "" H 5700 7200 50  0001 C CNN
	1    5700 7200
	1    0    0    -1  
$EndComp
Text Label 5000 6950 0    50   ~ 0
SDA
Text Label 5000 7050 0    50   ~ 0
SCL
Wire Wire Line
	5000 6950 5200 6950
Wire Wire Line
	5000 7050 5500 7050
Wire Wire Line
	5700 7200 5700 7150
Wire Wire Line
	5700 7150 6050 7150
Text Label 6350 4200 0    50   ~ 0
SDA
Text Label 6350 4300 0    50   ~ 0
SCL
Wire Wire Line
	6350 4200 6200 4200
Wire Wire Line
	6350 4300 6200 4300
Text Notes 6150 7300 0    50   ~ 0
I2C
Text Label 6350 3100 0    50   ~ 0
CH1
Text Label 6350 3900 0    50   ~ 0
CH6
Text Label 6350 4000 0    50   ~ 0
CH7
Wire Wire Line
	6350 3100 6200 3100
Wire Wire Line
	6350 3900 6200 3900
Wire Wire Line
	6350 4000 6200 4000
Text Label 1800 1450 0    50   ~ 0
CH1
Text Label 1800 3700 0    50   ~ 0
CH6
Text Label 1800 4150 0    50   ~ 0
CH7
Text Label 1800 2350 0    50   ~ 0
MISO_CH3
Text Label 1800 1900 0    50   ~ 0
MOSI_CH2
Text Label 1800 2800 0    50   ~ 0
SCK_CH4
Wire Wire Line
	1800 1450 2250 1450
Wire Wire Line
	1550 1550 2250 1550
Wire Wire Line
	1700 1650 2250 1650
Wire Wire Line
	1800 3700 2250 3700
Wire Wire Line
	1550 2000 2250 2000
Wire Wire Line
	1700 2100 2250 2100
Wire Wire Line
	1800 4150 2250 4150
Wire Wire Line
	1550 2450 2250 2450
Wire Wire Line
	1700 2550 2250 2550
Wire Wire Line
	1800 2350 2250 2350
Wire Wire Line
	1550 2900 2250 2900
Wire Wire Line
	1700 3000 2250 3000
Wire Wire Line
	1800 1900 2250 1900
Wire Wire Line
	1550 3350 2250 3350
Wire Wire Line
	1700 3450 2250 3450
Wire Wire Line
	1800 2800 2250 2800
Wire Wire Line
	1550 3800 2250 3800
Wire Wire Line
	1700 3900 2250 3900
$Comp
L Connector_Generic:Conn_01x01 J9
U 1 1 5DA33881
P 7450 4400
F 0 "J9" H 7530 4442 50  0000 L CNN
F 1 "Conn_01x01" H 7530 4351 50  0000 L CNN
F 2 "obbo_footprints:Pin_D1.0mm_L10.0mm_LooseFit" H 7450 4400 50  0001 C CNN
F 3 "~" H 7450 4400 50  0001 C CNN
	1    7450 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 4400 7250 4400
$Comp
L power:GND #PWR021
U 1 1 5DA74737
P 8850 5300
F 0 "#PWR021" H 8850 5050 50  0001 C CNN
F 1 "GND" H 8855 5127 50  0000 C CNN
F 2 "" H 8850 5300 50  0001 C CNN
F 3 "" H 8850 5300 50  0001 C CNN
	1    8850 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 4950 8850 4850
Wire Wire Line
	8850 5300 8850 5250
Text Label 8650 4850 0    50   ~ 0
CE
Wire Wire Line
	8650 4850 8850 4850
Connection ~ 8850 4850
Text Label 6350 5300 0    50   ~ 0
CE
Wire Wire Line
	6350 5300 6200 5300
Wire Wire Line
	1300 7150 1600 7150
Wire Wire Line
	1300 6250 1600 6250
Wire Wire Line
	1600 6550 1600 6250
Connection ~ 1600 6250
Wire Wire Line
	1600 6250 2100 6250
Wire Wire Line
	1600 6850 1600 7150
Connection ~ 1600 7150
Wire Wire Line
	1600 7150 2100 7150
$Comp
L power:GND #PWR025
U 1 1 5DA31E28
P 5600 5700
F 0 "#PWR025" H 5600 5450 50  0001 C CNN
F 1 "GND" H 5605 5527 50  0000 C CNN
F 2 "" H 5600 5700 50  0001 C CNN
F 3 "" H 5600 5700 50  0001 C CNN
	1    5600 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 5700 5600 5600
$Comp
L Device:CP C5
U 1 1 5DAEB7C6
P 9350 3650
F 0 "C5" H 9468 3696 50  0000 L CNN
F 1 "10u" H 9468 3605 50  0000 L CNN
F 2 "Capacitor_SMD:C_1210_3225Metric_Pad1.42x2.65mm_HandSolder" H 9388 3500 50  0001 C CNN
F 3 "~" H 9350 3650 50  0001 C CNN
	1    9350 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5DAEBF4E
P 9350 3900
F 0 "#PWR015" H 9350 3650 50  0001 C CNN
F 1 "GND" H 9355 3727 50  0000 C CNN
F 2 "" H 9350 3900 50  0001 C CNN
F 3 "" H 9350 3900 50  0001 C CNN
	1    9350 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 3900 9350 3800
Wire Wire Line
	9350 3500 9350 3450
Wire Wire Line
	9350 3450 9700 3450
$Comp
L Device:C C8
U 1 1 5DA1137B
P 3550 5100
F 0 "C8" H 3665 5146 50  0000 L CNN
F 1 "100n" H 3665 5055 50  0000 L CNN
F 2 "obbo_footprints:C_0805K" H 3588 4950 50  0001 C CNN
F 3 "~" H 3550 5100 50  0001 C CNN
	1    3550 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR023
U 1 1 5DA116F2
P 3550 5350
F 0 "#PWR023" H 3550 5100 50  0001 C CNN
F 1 "GND" H 3555 5177 50  0000 C CNN
F 2 "" H 3550 5350 50  0001 C CNN
F 3 "" H 3550 5350 50  0001 C CNN
	1    3550 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 5350 3550 5250
Wire Wire Line
	3550 4950 3550 4750
Wire Wire Line
	3550 4750 4000 4750
$Comp
L Device:R R7
U 1 1 5DA34EA6
P 5200 6750
F 0 "R7" H 5270 6796 50  0000 L CNN
F 1 "3k3" H 5270 6705 50  0000 L CNN
F 2 "obbo_footprints:R_0805K" V 5130 6750 50  0001 C CNN
F 3 "~" H 5200 6750 50  0001 C CNN
	1    5200 6750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5DA3543F
P 5500 6750
F 0 "R8" H 5570 6796 50  0000 L CNN
F 1 "3k3" H 5570 6705 50  0000 L CNN
F 2 "obbo_footprints:R_0805K" V 5430 6750 50  0001 C CNN
F 3 "~" H 5500 6750 50  0001 C CNN
	1    5500 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 6900 5500 7050
Connection ~ 5500 7050
Wire Wire Line
	5500 7050 6050 7050
Wire Wire Line
	5200 6900 5200 6950
Connection ~ 5200 6950
Wire Wire Line
	5200 6950 6050 6950
Wire Wire Line
	5200 6600 5200 6500
Wire Wire Line
	5500 6600 5500 6500
Wire Wire Line
	5500 6500 5200 6500
Connection ~ 5200 6500
Wire Wire Line
	5200 6500 5200 6450
$Comp
L Device:C C7
U 1 1 5E7E9168
P 6900 4650
F 0 "C7" H 7015 4696 50  0000 L CNN
F 1 "100n" H 7015 4605 50  0000 L CNN
F 2 "obbo_footprints:C_0805K" H 6938 4500 50  0001 C CNN
F 3 "~" H 6900 4650 50  0001 C CNN
	1    6900 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 5E7E916E
P 6900 4850
F 0 "#PWR019" H 6900 4600 50  0001 C CNN
F 1 "GND" H 6905 4677 50  0000 C CNN
F 2 "" H 6900 4850 50  0001 C CNN
F 3 "" H 6900 4850 50  0001 C CNN
	1    6900 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 4850 6900 4800
$Comp
L power:GND #PWR07
U 1 1 5E8155BF
P 7000 1850
F 0 "#PWR07" H 7000 1600 50  0001 C CNN
F 1 "GND" H 7005 1677 50  0000 C CNN
F 2 "" H 7000 1850 50  0001 C CNN
F 3 "" H 7000 1850 50  0001 C CNN
	1    7000 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 1850 7000 1800
Wire Wire Line
	7750 1400 8250 1400
$Comp
L power:+5V #PWR03
U 1 1 5E82154B
P 9050 1050
F 0 "#PWR03" H 9050 900 50  0001 C CNN
F 1 "+5V" H 9065 1223 50  0000 C CNN
F 2 "" H 9050 1050 50  0001 C CNN
F 3 "" H 9050 1050 50  0001 C CNN
	1    9050 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 1050 6450 1400
Wire Wire Line
	6450 1500 6600 1500
Wire Wire Line
	6600 1400 6450 1400
Connection ~ 6450 1400
Wire Wire Line
	6450 1400 6450 1500
$Comp
L Device:CP C1
U 1 1 5E82DED8
P 8250 1650
F 0 "C1" H 8368 1696 50  0000 L CNN
F 1 "10u" H 8368 1605 50  0000 L CNN
F 2 "Capacitor_SMD:C_1210_3225Metric_Pad1.42x2.65mm_HandSolder" H 8288 1500 50  0001 C CNN
F 3 "~" H 8250 1650 50  0001 C CNN
	1    8250 1650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5E82DEDE
P 8250 1850
F 0 "#PWR08" H 8250 1600 50  0001 C CNN
F 1 "GND" H 8255 1677 50  0000 C CNN
F 2 "" H 8250 1850 50  0001 C CNN
F 3 "" H 8250 1850 50  0001 C CNN
	1    8250 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 1850 8250 1800
Wire Wire Line
	8250 1500 8250 1400
$Comp
L power:+5V #PWR017
U 1 1 5E8398F0
P 6900 3950
F 0 "#PWR017" H 6900 3800 50  0001 C CNN
F 1 "+5V" H 6915 4123 50  0000 C CNN
F 2 "" H 6900 3950 50  0001 C CNN
F 3 "" H 6900 3950 50  0001 C CNN
	1    6900 3950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR06
U 1 1 5E839DA6
P 5600 1800
F 0 "#PWR06" H 5600 1650 50  0001 C CNN
F 1 "+5V" H 5615 1973 50  0000 C CNN
F 2 "" H 5600 1800 50  0001 C CNN
F 3 "" H 5600 1800 50  0001 C CNN
	1    5600 1800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR026
U 1 1 5E83A141
P 5200 6450
F 0 "#PWR026" H 5200 6300 50  0001 C CNN
F 1 "+5V" H 5215 6623 50  0000 C CNN
F 2 "" H 5200 6450 50  0001 C CNN
F 3 "" H 5200 6450 50  0001 C CNN
	1    5200 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 4400 6900 4500
$Comp
L Regulator_Linear:TPS73633DBV U2
U 1 1 5E80DC3F
P 9600 1500
F 0 "U2" H 9600 1842 50  0000 C CNN
F 1 "TPS73633DBV" H 9600 1751 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 9600 1825 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/tps736.pdf" H 9600 1450 50  0001 C CNN
	1    9600 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 1050 10200 1400
$Comp
L Regulator_Linear:TPS73601DBV U1
U 1 1 5E85BDEC
P 7000 1500
F 0 "U1" H 7000 1867 50  0000 C CNN
F 1 "TPS73601DBV" H 7000 1776 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 7000 1825 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/tps736.pdf" H 7000 1450 50  0001 C CNN
	1    7000 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5E861CCD
P 7750 1650
F 0 "R1" H 7820 1696 50  0000 L CNN
F 1 "68k*" H 7820 1605 50  0000 L CNN
F 2 "obbo_footprints:R_0805K" V 7680 1650 50  0001 C CNN
F 3 "~" H 7750 1650 50  0001 C CNN
	1    7750 1650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5E862208
P 7750 2050
F 0 "R2" H 7820 2096 50  0000 L CNN
F 1 "22k" H 7820 2005 50  0000 L CNN
F 2 "obbo_footprints:R_0805K" V 7680 2050 50  0001 C CNN
F 3 "~" H 7750 2050 50  0001 C CNN
	1    7750 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5E862483
P 7750 2250
F 0 "#PWR012" H 7750 2000 50  0001 C CNN
F 1 "GND" H 7755 2077 50  0000 C CNN
F 2 "" H 7750 2250 50  0001 C CNN
F 3 "" H 7750 2250 50  0001 C CNN
	1    7750 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 2250 7750 2200
Wire Wire Line
	7750 1900 7750 1850
Wire Wire Line
	7750 1500 7750 1400
Wire Wire Line
	7750 1400 7400 1400
Connection ~ 7750 1400
Wire Wire Line
	7400 1500 7550 1500
Wire Wire Line
	7550 1500 7550 1850
Wire Wire Line
	7550 1850 7750 1850
Connection ~ 7750 1850
Wire Wire Line
	7750 1850 7750 1800
Wire Wire Line
	6350 4100 6200 4100
Text Label 6350 4100 0    50   ~ 0
CH8
Wire Wire Line
	6350 3800 6200 3800
Text Label 6350 3800 0    50   ~ 0
CH5
Wire Wire Line
	8250 1400 8250 1050
$Comp
L power:+5V #PWR02
U 1 1 5E932A15
P 8250 1050
F 0 "#PWR02" H 8250 900 50  0001 C CNN
F 1 "+5V" H 8265 1223 50  0000 C CNN
F 2 "" H 8250 1050 50  0001 C CNN
F 3 "" H 8250 1050 50  0001 C CNN
	1    8250 1050
	1    0    0    -1  
$EndComp
Connection ~ 8250 1400
$Comp
L Connector_Generic:Conn_01x03 J7
U 1 1 5E9E2A85
P 2450 4250
F 0 "J7" H 2530 4292 50  0000 L CNN
F 1 "Conn_01x03" H 2530 4201 50  0000 L CNN
F 2 "obbo_footprints:PinHeader_3x01_P2.54mm_Horizontal" H 2450 4250 50  0001 C CNN
F 3 "~" H 2450 4250 50  0001 C CNN
	1    2450 4250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J8
U 1 1 5E9E2D24
P 2450 4700
F 0 "J8" H 2530 4742 50  0000 L CNN
F 1 "Conn_01x03" H 2530 4651 50  0000 L CNN
F 2 "obbo_footprints:PinHeader_3x01_P2.54mm_Horizontal" H 2450 4700 50  0001 C CNN
F 3 "~" H 2450 4700 50  0001 C CNN
	1    2450 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 4350 1700 4350
Connection ~ 1700 4350
Wire Wire Line
	1700 4350 1700 3900
Wire Wire Line
	2250 4800 1700 4800
Connection ~ 1700 4800
Wire Wire Line
	1700 4800 1700 4350
Wire Wire Line
	2250 4700 1550 4700
Wire Wire Line
	1550 4700 1550 4250
Connection ~ 1550 3800
Wire Wire Line
	2250 4250 1550 4250
Connection ~ 1550 4250
Wire Wire Line
	1550 4250 1550 3800
Wire Wire Line
	1800 3250 2250 3250
Wire Wire Line
	1800 4600 2250 4600
Text Label 1800 3250 0    50   ~ 0
CH5
Text Label 1800 4600 0    50   ~ 0
CH8
Text Notes 4100 4550 0    50   ~ 0
Vref 1.1V
Text Notes 4100 4100 0    50   ~ 0
4.8V - 6.2V
$EndSCHEMATC
