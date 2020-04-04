# Black Headed Vulture
- Weather Station of the Atmospheric Discharges Research Laboratory - LPDA UFPA.

# Description
This is a project of a Weather Station based on:
- Microcontroller Atmega328p.

<img src=".imgs/328p.jpeg" width="170" height="100"/>

- Barometric Sensor BMP180

<img src=".imgs/bmp180.jpg" width="120" height="100"/>

- GPS Neo6m

<img src=".imgs/gps.jpg" width="120" height="120"/>

- Bluetooth module AT-09 

<img src=".imgs/at09.png" width="150" height="100"/>

- Humidity and Temperature sensor DHT22

<img src=".imgs/dht22.jpg" width="100" height="100"/>


# Structure
- #### app/ 
        Contains a main function
- #### flowcharts/
        Contains some flowcharts that describe how some driver works, bmp180 for example.
- #### src/avr_libs/ 
        Contains libraries to drive avr328p tools like spi, twi, adc, etc. and some drivers to work with modules.
- #### src/FatFs/
        Contains a implementations of FatFs filesystem developed by ChaN http://elm-chan.org/
- #### tests/
        Contains some routines to test libraries and functionalities separately
# LICENSE
        [This project is distributed by this license](LICENSE)