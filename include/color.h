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

/**
 * @brief Create a new Color object from HSV values
 * 
 * @param c HSV values
 * @return Color 
 */
Color Color_new_from_hsv(ColorHSV c);

/**
 * @brief Create a new random Color object
 * 
 * @return Color 
 */
Color Color_new_random(void);

#endif
