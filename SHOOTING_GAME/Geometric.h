#ifndef GEOMETRIC_H_
#define GEOMETRIC_H_

#include "CommonFunc.h"

typedef struct GeometricFormat {
public:
    GeometricFormat(int left, int top, int width, int height)
    : left_(left), top_(top), width_(width), height_(height) {};
    int left_;
    int top_;
    int width_;
    int height_;
};

typedef struct colorData {
public:

    colorData(Uint8 r, Uint8 g, Uint8 b)
    : red_(r), green_(g), blue_(b)
    {
    };

    Uint8 red_;
    Uint8 green_;
    Uint8 blue_;
};

class Geometric {
public:
    static void RenderRectangle(const GeometricFormat& geo_size, const colorData& color_data, SDL_Renderer* screen);
    static void RenderOutline(const GeometricFormat& geo_size, const colorData& color_data, SDL_Renderer* screen);
};

#endif // GEOMETRIC_H_
