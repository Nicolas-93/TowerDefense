#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <MLV/MLV_color.h>

typedef struct ColorHSV {
    uint16_t h;
    uint8_t s;
    uint8_t v;
} ColorHSV;

typedef struct Color {
    ColorHSV hsv;
    MLV_Color mlvrgb;
} Color;

Color Color_new_from_hsv(ColorHSV c);

#endif
