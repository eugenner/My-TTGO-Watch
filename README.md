

A smartwatch based on ESP32 from LilyGo. Currently under development.

# Telegram chatgroup

Telegram chatgroup is here:
https://t.me/TTGO_Watch

# Install

Clone this repository and open it with platformIO. Build and upload. On a terminal in vscode you can do it with

```bash
pio run -t upload
```

or simple press "build and upload" in platformIO.

# known issues

* the webserver crashes the ESP32 really often
* the battery indicator is not accurate, rather a problem with the power management unit ( axp202 )
* from time to time the esp32 crashes accidentally
* and some other small things

# how to use

## weather app

On startup you see the main screen (time tile). It show the time and the current weather (if correct configure). Now you can swipe with you fingers up, down, left and right between the four main screens. The four screens are organized in time, apps, note and setup tile.
For the weather app you need an openweather.com api-id. http://openweathermap.org/appid is a good starting point.

## bluetooth

The bluetooth notification work with [gadgetbridge](https://gadgetbridge.org) very well. But keep in mind, bluetooth in standby reduces the battery runtime. In connection with [OsmAnd](https://osmand.net) the watch can also be used for navigation. Please use the osmand app, otherwise a lot of messages will be displayed.

# Forks that are recommended

[FantasyFactory](https://github.com/FantasyFactory/My-TTGO-Watch)<br>
[NorthernDIY](https://github.com/NorthernDIY/My-TTGO-Watch)<br>

# for the programmers

Internal RAM is very limited, use PSRAM as much as possible. When you work with ArduinoJson, include this

```#include "hardware/json_psram_allocator.h"```

and create your json with

```SpiRamJsonDocument doc( 1000 );```

A smartwatch based on ESP32 from LilyGo. Currently under development.


docs:
    https://docs.lvgl.io/latest/en/html/index.html
    https://github.com/lvgl/lv_platformio - Run LittlevGL via PlatformIO
