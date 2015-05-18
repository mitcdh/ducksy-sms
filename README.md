# Ducksy SMS
A [Teensy 3.1](https://www.pjrc.com/store/teensy31.html) sketch to combine with an [Adafruit FONA](http://www.adafruit.com/product/1963).

Includes a live [Duckyscript](https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Duckyscript) (from the [USB-Rubber-Ducky](https://github.com/hak5darren/USB-Rubber-Ducky) project) interpreter. With some additional commands for Mouse control allowed by the Teensy.

* MOUSE CLICK (MOUSE_LEFT|MOUSE_MIDDLE|MOUSE_RIGHT)
* MOUSE PRESS (MOUSE_LEFT|MOUSE_MIDDLE|MOUSE_RIGHT)
* MOUSE MOVE X,Y

| Teensy PIN    | FONA PIN      |
| :------------ |:-------------:|
| 3.3V          | VIO           |
| Pin0          | TX            |
| Pin1          | RX            |
| GND           | Key           |
| Pin4          | RST           |
| AGND          | AGND          |


