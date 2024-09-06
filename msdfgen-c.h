
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
typedef struct msdfgen_VectorView msdfgen_VectorView;
typedef struct msdfgen_Shape msdfgen_Shape;
typedef struct msdfgen_Contour msdfgen_Contour;
typedef struct msdfgen_EdgeSegment msdfgen_EdgeSegment;
typedef struct msdfgen_EdgeHolder msdfgen_EdgeHolder;
typedef struct msdfgen_SDFTransformation msdfgen_SDFTransformation;
typedef struct msdfgen_GeneratorConfig msdfgen_GeneratorConfig;
typedef struct msdfgen_MSDFGeneratorConfig msdfgen_MSDFGeneratorConfig;
typedef struct msdfgen_Projection msdfgen_Projection;
typedef struct msdfgen_Range msdfgen_Range;
typedef struct msdfgen_BitmapRef msdfgen_BitmapRef;
typedef struct msdfgen_Bitmap msdfgen_Bitmap;

// C API functions
#ifdef __cplusplus
extern "C" {
#endif

MSDFGEN_PUBLIC msdfgen_Void  msdfgen_VectorView_destroy(msdfgen_VectorView* vectorView);
MSDFGEN_PUBLIC msdfgen_Void  msdfgen_VectorView_data(msdfgen_VectorView* vectorView, msdfgen_Size* size, msdfgen_Void** data);
MSDFGEN_PUBLIC msdfgen_Size  msdfgen_VectorView_elementSize(msdfgen_VectorView* vectorView);
MSDFGEN_PUBLIC msdfgen_Size  msdfgen_VectorView_count(msdfgen_VectorView* vectorView);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_VectorView_at(msdfgen_VectorView* vectorView, msdfgen_Size index, msdfgen_Void* element);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_VectorView_set(msdfgen_VectorView* vectorView, msdfgen_Size index, msdfgen_Void* element);
MSDFGEN_PUBLIC msdfgen_Void  msdfgen_VectorView_add(msdfgen_VectorView* vectorView, msdfgen_Void* element);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_VectorView_insert(msdfgen_VectorView* vectorView, msdfgen_Size index, msdfgen_Void* element);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_VectorView_remove(msdfgen_VectorView* vectorView, msdfgen_Size index);
MSDFGEN_PUBLIC msdfgen_Void  msdfgen_VectorView_clear(msdfgen_VectorView* vectorView);

MSDFGEN_PUBLIC msdfgen_EdgeHolder* msdfgen_EdgeHolder_create();
MSDFGEN_PUBLIC msdfgen_EdgeHolder* msdfgen_EdgeHolder_create0(msdfgen_EdgeSegment* segment);
MSDFGEN_PUBLIC msdfgen_EdgeHolder* msdfgen_EdgeHolder_create1(msdfgen_Point2 p0, msdfgen_Point2 p1, msdfgen_EdgeColor edgeColor);

MSDFGEN_PUBLIC msdfgen_Contour*    msdfgen_Contour_create();
MSDFGEN_PUBLIC msdfgen_Void        msdfgen_Contour_destroy(msdfgen_Contour* contour);
MSDFGEN_PUBLIC msdfgen_Void        msdfgen_Contour_addEdge(msdfgen_Contour* contour, msdfgen_EdgeHolder* edge);
MSDFGEN_PUBLIC msdfgen_EdgeHolder* msdfgen_Contour_addEdge(msdfgen_Contour* contour);
MSDFGEN_PUBLIC msdfgen_Void        msdfgen_Contour_bound(msdfgen_Contour* contour, msdfgen_Float* l, msdfgen_Float* b, msdfgen_Float* r, msdfgen_Float* t);
MSDFGEN_PUBLIC msdfgen_Void        msdfgen_Contour_boundMiters(msdfgen_Contour* contour, msdfgen_Float* l, msdfgen_Float* b, msdfgen_Float* r, msdfgen_Float* t, msdfgen_Float border, msdfgen_Float miterLimit, msdfgen_Int polarity);
MSDFGEN_PUBLIC msdfgen_Int         msdfgen_Contour_winding(msdfgen_Contour* contour);
MSDFGEN_PUBLIC msdfgen_Void        msdfgen_Contour_reverse(msdfgen_Contour* contour);
MSDFGEN_PUBLIC msdfgen_VectorView* msdfgen_Contour_getEdges();

MSDFGEN_PUBLIC msdfgen_Shape* msdfgen_Shape_create();
MSDFGEN_PUBLIC msdfgen_Void   msdfgen_Shape_destroy(msdfgen_Shape* shape);

MSDFGEN_PUBLIC msdfgen_Error msdfgen_generateSDF(msdfgen_BitmapRef* output, msdfgen_Shape* shape, msdfgen_SDFTransformation* transformation, msdfgen_GeneratorConfig* config);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_generatePSDF(msdfgen_BitmapRef* output, msdfgen_Shape* shape, msdfgen_SDFTransformation* transformation, msdfgen_GeneratorConfig* config);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_generateMSDF(msdfgen_BitmapRef* output, msdfgen_Shape* shape, msdfgen_SDFTransformation* transformation, msdfgen_MSDFGeneratorConfig* config);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_generateMTSDF(msdfgen_BitmapRef* output, msdfgen_Shape* shape, msdfgen_SDFTransformation* transformation, msdfgen_MSDFGeneratorConfig* config);

#ifdef __cplusplus
}
#endif