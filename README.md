# Color Badger

This project allows you to use AdaFruit IO to send hex encoded colors to a Feather and NeoPixel Wing
in order to render that color on the badge.

## Feed Values

In order to set just the color, send a message like this:

```json
{ "color": "#FF0000" }
```

To also set the brightness, send a message like this:

```json
{ "color": "#FF0000", "brightness": 255 }
```

and then look away from the device!
