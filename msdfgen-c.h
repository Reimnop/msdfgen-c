
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

#include "msdfgen-c-common.h"

// Opaque pointer handle types
typedef struct msdfgen_Shape msdfgen_Shape;
typedef struct msdfgen_Contour msdfgen_Contour;
typedef struct msdfgen_SDFTransformation msdfgen_SDFTransformation;
typedef struct msdfgen_GeneratorConfig msdfgen_GeneratorConfig;
typedef struct msdfgen_MSDFGeneratorConfig msdfgen_MSDFGeneratorConfig;
typedef struct msdfgen_Projection msdfgen_Projection;
typedef struct msdfgen_Range msdfgen_Range;
typedef struct msdfgen_BitmapRef msdfgen_BitmapRef;
typedef struct msdfgen_Bitmap msdfgen_Bitmap;

// Interface
#ifdef __cplusplus
extern "C" {
#endif



MSDFGEN_PUBLIC msdfgen_Shape* msdfgen_createShape();
MSDFGEN_PUBLIC void msdfgen_destroyShape(msdfgen_Shape* shape);


MSDFGEN_PUBLIC msdfgen_Error msdfgen_generateSDF(msdfgen_BitmapRef* output, msdfgen_Shape* shape, msdfgen_SDFTransformation* transformation, msdfgen_GeneratorConfig* config);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_generatePSDF(msdfgen_BitmapRef* output, msdfgen_Shape* shape, msdfgen_SDFTransformation* transformation, msdfgen_GeneratorConfig* config);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_generateMSDF(msdfgen_BitmapRef* output, msdfgen_Shape* shape, msdfgen_SDFTransformation* transformation, msdfgen_MSDFGeneratorConfig* config);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_generateMTSDF(msdfgen_BitmapRef* output, msdfgen_Shape* shape, msdfgen_SDFTransformation* transformation, msdfgen_MSDFGeneratorConfig* config);

#ifdef __cplusplus
}
#endif