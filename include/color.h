#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <MLV/MLV_color.h>

typedef struct ColorHSV {
    uint16_t h;
    uint8_t s;
    uint8_t v;
} ColorHSV ;

MLV_Color Color_hsv_to_mlvrgb(ColorHSV c);

#endif
