#include "color.h"
#include "utils.h"
#include <math.h>

static inline double _Color_hsv_to_rgb_k_aux(ColorHSV c, double n) {
    return fmod(n + c.h / 60.0, 6);
}

static inline double _Color_hsv_to_rgb_f_aux(ColorHSV c, double n) {
    double k = _Color_hsv_to_rgb_k_aux(c, n);
    return (
        c.v - (
            c.v * c.s *
            fmax(0, fmin(fmin(k, 4 - k), 1))
        )
    );
}

Color Color_new_from_hsv(ColorHSV c) {
    return (Color) {
        .hsv = c,
        .mlvrgb = MLV_convert_rgba_to_color(
            _Color_hsv_to_rgb_f_aux(c, 5) * 255,
            _Color_hsv_to_rgb_f_aux(c, 3) * 255,
            _Color_hsv_to_rgb_f_aux(c, 1) * 255,
            255
        )
    };
}

Color Color_new_random(void) {
    return Color_new_from_hsv((ColorHSV) {
        .h = rand() % 360,
        .s = 1,
        .v = 1,
    });
}
