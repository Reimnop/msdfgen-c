
#pragma once

/*
 * MULTI-CHANNEL SIGNED DISTANCE FIELD GENERATOR
 * ---------------------------------------------
 * A utility by Viktor Chlumsky, (c) 2014 - 2024
 *
 * The technique used to generate multi-channel distance fields in this code
 * has been developed by Viktor Chlumsky in 2014 for his master's thesis,
 * "Shape Decomposition for Multi-Channel Distance Fields". It provides improved
 * quality of sharp corners in glyphs and other 2D shapes compared to monochrome
 * distance fields. To reconstruct an image of the shape, apply the median of three
 * operation on the triplet of sampled signed distance values.
 *
 */

typedef int    msdfgen_Int;
typedef double msdfgen_Float;
typedef size_t msdfgen_Size;
typedef void   msdfgen_Void;

enum msdfgen_Error {
    msdfgen_Error_Ok,
    msdfgen_Error_InvalidArgument,
    msdfgen_Error_OutOfBounds,
    msdfgen_Error_Unknown
};

enum msdfgen_EdgeColor {
    msdfgen_EdgeColor_Black = 0,
    msdfgen_EdgeColor_Red = 1,
    msdfgen_EdgeColor_Green = 2,
    msdfgen_EdgeColor_Yellow = 3,
    msdfgen_EdgeColor_Blue = 4,
    msdfgen_EdgeColor_Magenta = 5,
    msdfgen_EdgeColor_Cyan = 6,
    msdfgen_EdgeColor_White = 7
};

struct msdfgen_Vector2 {
    msdfgen_Float x, y;
};

typedef msdfgen_Vector2 msdfgen_Point2;
