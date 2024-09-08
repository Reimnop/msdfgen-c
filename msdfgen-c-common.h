
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

typedef bool   msdfgen_Bool;
typedef int    msdfgen_Int;
typedef float  msdfgen_Float;
typedef double msdfgen_Double;
typedef size_t msdfgen_Size;
typedef void   msdfgen_Void;

enum msdfgen_Error : msdfgen_Int {
    msdfgen_Error_Ok = 0,
    msdfgen_Error_OutOfBounds = 1
};

enum msdfgen_EdgeColor : msdfgen_Int {
    msdfgen_EdgeColor_Black = 0,
    msdfgen_EdgeColor_Red = 1,
    msdfgen_EdgeColor_Green = 2,
    msdfgen_EdgeColor_Yellow = 3,
    msdfgen_EdgeColor_Blue = 4,
    msdfgen_EdgeColor_Magenta = 5,
    msdfgen_EdgeColor_Cyan = 6,
    msdfgen_EdgeColor_White = 7
};

enum msdfgen_EdgeType : msdfgen_Int {
    msdfgen_EdgeType_Unknown = 0,
    msdfgen_EdgeType_Linear = 1,
    msdfgen_EdgeType_Quadratic = 2,
    msdfgen_EdgeType_Cubic = 3
};

enum msdfgen_ErrorCorrectionConfig_Mode : msdfgen_Int {
    msdfgen_ErrorCorrectionConfig_Mode_Disabled = 0,
    msdfgen_ErrorCorrectionConfig_Mode_Indiscriminate = 1,
    msdfgen_ErrorCorrectionConfig_Mode_EdgePriority = 2,
    msdfgen_ErrorCorrectionConfig_Mode_EdgeOnly = 3
};

enum msdfgen_ErrorCorrectionConfig_DistanceCheckMode : msdfgen_Int {
    msdfgen_ErrorCorrectionConfig_Mode_DoNotCheckDistance = 0,
    msdfgen_ErrorCorrectionConfig_Mode_CheckDistanceAtEdge = 1,
    msdfgen_ErrorCorrectionConfig_Mode_AlwaysCheckDistance = 2
};

struct msdfgen_Vector2 {
    msdfgen_Double x, y;
};

typedef msdfgen_Vector2 msdfgen_Point2;

struct msdfgen_Bounds {
    msdfgen_Double l, b, r, t;
};

struct msdfgen_Range {
    msdfgen_Double lower, upper;
};

struct msdfgen_SignedDistance {
    msdfgen_Double distance, dot;
};

struct msdfgen_DistanceMapping {
    msdfgen_Double scale, translate;
};

struct msdfgen_ErrorCorrectionConfig {
    msdfgen_ErrorCorrectionConfig_Mode mode;
    msdfgen_ErrorCorrectionConfig_DistanceCheckMode distanceCheckMode;
    msdfgen_Double minDeviationRatio;
    msdfgen_Double minImproveRatio;
    msdfgen_Void* buffer;
};

struct msdfgen_BitmapRef {
    msdfgen_Void* data;
    msdfgen_Int width, height;
};
