
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
typedef struct msdfgen_VectorView* msdfgen_VectorViewHandle;
typedef struct msdfgen_Scanline* msdfgen_ScanlineHandle;
typedef struct msdfgen_Shape* msdfgen_ShapeHandle;
typedef struct msdfgen_Contour* msdfgen_ContourHandle;
typedef struct msdfgen_EdgeHolder* msdfgen_EdgeHolderHandle;
typedef struct msdfgen_EdgeSegment* msdfgen_EdgeSegmentHandle;
typedef struct msdfgen_LinearSegment* msdfgen_LinearSegmentHandle;
typedef struct msdfgen_QuadraticSegment* msdfgen_QuadraticSegmentHandle;
typedef struct msdfgen_CubicSegment* msdfgen_CubicSegmentHandle;
typedef struct msdfgen_Projection* msdfgen_ProjectionHandle;
typedef struct msdfgen_SDFTransformation* msdfgen_SDFTransformationHandle;
typedef struct msdfgen_GeneratorConfig* msdfgen_GeneratorConfigHandle;
typedef struct msdfgen_MSDFGeneratorConfig* msdfgen_MSDFGeneratorConfigHandle;

// C API functions
#ifdef __cplusplus
extern "C" {
#endif

// Vector view
MSDFGEN_PUBLIC msdfgen_Void  msdfgen_VectorView_destroy(msdfgen_VectorViewHandle vectorView);
MSDFGEN_PUBLIC msdfgen_Void  msdfgen_VectorView_data(msdfgen_VectorViewHandle vectorView, msdfgen_Size* count, msdfgen_Void** data);
MSDFGEN_PUBLIC msdfgen_Size  msdfgen_VectorView_elementSize(msdfgen_VectorViewHandle vectorView);
MSDFGEN_PUBLIC msdfgen_Size  msdfgen_VectorView_count(msdfgen_VectorViewHandle vectorView);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_VectorView_at(msdfgen_VectorViewHandle vectorView, msdfgen_Size index, msdfgen_Void* element);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_VectorView_set(msdfgen_VectorViewHandle vectorView, msdfgen_Size index, msdfgen_Void* element);
MSDFGEN_PUBLIC msdfgen_Void  msdfgen_VectorView_add(msdfgen_VectorViewHandle vectorView, msdfgen_Void* element);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_VectorView_insert(msdfgen_VectorViewHandle vectorView, msdfgen_Size index, msdfgen_Void* element);
MSDFGEN_PUBLIC msdfgen_Error msdfgen_VectorView_remove(msdfgen_VectorViewHandle vectorView, msdfgen_Size index);
MSDFGEN_PUBLIC msdfgen_Void  msdfgen_VectorView_clear(msdfgen_VectorViewHandle vectorView);

// Edge holder
MSDFGEN_PUBLIC msdfgen_EdgeSegmentHandle msdfgen_EdgeHolder_getSegment(msdfgen_EdgeHolderHandle edgeHolder);
MSDFGEN_PUBLIC msdfgen_Void              msdfgen_EdgeHolder_setSegment(msdfgen_EdgeHolderHandle edgeHolder, msdfgen_EdgeSegmentHandle segment);

// Edge segment
MSDFGEN_PUBLIC msdfgen_Void              msdfgen_EdgeSegment_destroy(msdfgen_EdgeSegmentHandle edge);
MSDFGEN_PUBLIC msdfgen_EdgeSegmentHandle msdfgen_EdgeSegment_clone(msdfgen_EdgeSegmentHandle edge);
MSDFGEN_PUBLIC msdfgen_EdgeType          msdfgen_EdgeSegment_type(msdfgen_EdgeSegmentHandle edge);
MSDFGEN_PUBLIC msdfgen_Size              msdfgen_EdgeSegment_getControlPointsCount(msdfgen_EdgeSegmentHandle edge);
MSDFGEN_PUBLIC msdfgen_Void              msdfgen_EdgeSegment_getControlPoints(msdfgen_EdgeSegmentHandle edge, msdfgen_Point2* points);
MSDFGEN_PUBLIC msdfgen_Point2            msdfgen_EdgeSegment_point(msdfgen_EdgeSegmentHandle edge, msdfgen_Double t);
MSDFGEN_PUBLIC msdfgen_Vector2           msdfgen_EdgeSegment_direction(msdfgen_EdgeSegmentHandle edge, msdfgen_Double t);
MSDFGEN_PUBLIC msdfgen_Vector2           msdfgen_EdgeSegment_directionChange(msdfgen_EdgeSegmentHandle edge, msdfgen_Double t);
MSDFGEN_PUBLIC msdfgen_SignedDistance    msdfgen_EdgeSegment_signedDistance(msdfgen_EdgeSegmentHandle edge, msdfgen_Point2 origin, msdfgen_Double* t);
MSDFGEN_PUBLIC msdfgen_Void              msdfgen_EdgeSegment_distanceToPerpendicularDistance(msdfgen_EdgeSegmentHandle edge, msdfgen_SignedDistance* distance, msdfgen_Point2 origin, msdfgen_Double t);
MSDFGEN_PUBLIC msdfgen_Int               msdfgen_EdgeSegment_scanlineIntersections(msdfgen_EdgeSegmentHandle edge, msdfgen_Double x[3], msdfgen_Int dy[3], msdfgen_Double y);
MSDFGEN_PUBLIC msdfgen_Void              msdfgen_EdgeSegment_bound(msdfgen_EdgeSegmentHandle edge, msdfgen_Double* l, msdfgen_Double* b, msdfgen_Double* r, msdfgen_Double* t);
MSDFGEN_PUBLIC msdfgen_Void              msdfgen_EdgeSegment_reverse(msdfgen_EdgeSegmentHandle edge);
MSDFGEN_PUBLIC msdfgen_Void              msdfgen_EdgeSegment_moveStartPoint(msdfgen_EdgeSegmentHandle edge, msdfgen_Point2 to);
MSDFGEN_PUBLIC msdfgen_Void              msdfgen_EdgeSegment_moveEndPoint(msdfgen_EdgeSegmentHandle edge, msdfgen_Point2 to);
MSDFGEN_PUBLIC msdfgen_Void              msdfgen_EdgeSegment_splitInThirds(msdfgen_EdgeSegmentHandle edge, msdfgen_EdgeSegmentHandle* part0, msdfgen_EdgeSegmentHandle* part1, msdfgen_EdgeSegmentHandle* part2);

// Linear segment
MSDFGEN_PUBLIC msdfgen_LinearSegmentHandle msdfgen_LinearSegment_create(msdfgen_Point2 p0, msdfgen_Point2 p1, msdfgen_EdgeColor edgeColor);
MSDFGEN_PUBLIC msdfgen_EdgeSegmentHandle   msdfgen_LinearSegment_toBase(msdfgen_LinearSegmentHandle segment);

// Quadratic segment
MSDFGEN_PUBLIC msdfgen_QuadraticSegmentHandle msdfgen_QuadraticSegment_create(msdfgen_Point2 p0, msdfgen_Point2 p1, msdfgen_Point2 p2, msdfgen_EdgeColor edgeColor);
MSDFGEN_PUBLIC msdfgen_CubicSegmentHandle     msdfgen_QuadraticSegment_convertToCubic(msdfgen_QuadraticSegmentHandle segment);
MSDFGEN_PUBLIC msdfgen_EdgeSegmentHandle      msdfgen_QuadraticSegment_toBase(msdfgen_QuadraticSegmentHandle segment);

// Cubic segment
MSDFGEN_PUBLIC msdfgen_CubicSegmentHandle msdfgen_CubicSegment_create(msdfgen_Point2 p0, msdfgen_Point2 p1, msdfgen_Point2 p2, msdfgen_Point2 p3, msdfgen_EdgeColor edgeColor);
MSDFGEN_PUBLIC msdfgen_EdgeSegmentHandle  msdfgen_CubicSegment_toBase(msdfgen_CubicSegmentHandle segment);

// Contour
MSDFGEN_PUBLIC msdfgen_EdgeHolderHandle msdfgen_Contour_addEdge(msdfgen_ContourHandle contour);
MSDFGEN_PUBLIC msdfgen_Void             msdfgen_Contour_bound(msdfgen_ContourHandle contour, msdfgen_Double* l, msdfgen_Double* b, msdfgen_Double* r, msdfgen_Double* t);
MSDFGEN_PUBLIC msdfgen_Void             msdfgen_Contour_boundMiters(msdfgen_ContourHandle contour, msdfgen_Double* l, msdfgen_Double* b, msdfgen_Double* r, msdfgen_Double* t, msdfgen_Double border, msdfgen_Double miterLimit, msdfgen_Int polarity);
MSDFGEN_PUBLIC msdfgen_Int              msdfgen_Contour_winding(msdfgen_ContourHandle contour);
MSDFGEN_PUBLIC msdfgen_Void             msdfgen_Contour_reverse(msdfgen_ContourHandle contour);
MSDFGEN_PUBLIC msdfgen_VectorViewHandle msdfgen_Contour_createEdgesView(msdfgen_ContourHandle contour);

// TODO: Wrap Scanline

// Shape
MSDFGEN_PUBLIC msdfgen_ShapeHandle      msdfgen_Shape_create();
MSDFGEN_PUBLIC msdfgen_Void             msdfgen_Shape_destroy(msdfgen_ShapeHandle shape);
MSDFGEN_PUBLIC msdfgen_ContourHandle    msdfgen_Shape_addContour(msdfgen_ShapeHandle shape);
MSDFGEN_PUBLIC msdfgen_Void             msdfgen_Shape_normalize(msdfgen_ShapeHandle shape);
MSDFGEN_PUBLIC msdfgen_Bool             msdfgen_Shape_validate(msdfgen_ShapeHandle shape);
MSDFGEN_PUBLIC msdfgen_Void             msdfgen_Shape_bound(msdfgen_ShapeHandle shape, msdfgen_Double* l, msdfgen_Double* b, msdfgen_Double* r, msdfgen_Double* t);
MSDFGEN_PUBLIC msdfgen_Void             msdfgen_Shape_boundMiters(msdfgen_ShapeHandle shape, msdfgen_Double* l, msdfgen_Double* b, msdfgen_Double* r, msdfgen_Double* t, msdfgen_Double border, msdfgen_Double miterLimit, msdfgen_Int polarity);
MSDFGEN_PUBLIC msdfgen_Bounds           msdfgen_Shape_getBounds(msdfgen_ShapeHandle shape, msdfgen_Double border, msdfgen_Double miterLimit, msdfgen_Int polarity);
MSDFGEN_PUBLIC msdfgen_Void             msdfgen_Shape_scanline(msdfgen_ShapeHandle shape, msdfgen_ScanlineHandle line, msdfgen_Double y);
MSDFGEN_PUBLIC msdfgen_Int              msdfgen_Shape_edgeCount(msdfgen_ShapeHandle shape);
MSDFGEN_PUBLIC msdfgen_Void             msdfgen_Shape_orientContours(msdfgen_ShapeHandle shape);
MSDFGEN_PUBLIC msdfgen_Bool             msdfgen_Shape_getInverseYAxis(msdfgen_ShapeHandle shape);
MSDFGEN_PUBLIC msdfgen_Void             msdfgen_Shape_setInverseYAxis(msdfgen_ShapeHandle shape, msdfgen_Bool inverseYAxis);
MSDFGEN_PUBLIC msdfgen_VectorViewHandle msdfgen_Shape_createContoursView(msdfgen_ShapeHandle shape);

// Distance mapping
MSDFGEN_PUBLIC msdfgen_DistanceMapping msdfgen_DistanceMapping_createRange(msdfgen_Range range);
MSDFGEN_PUBLIC msdfgen_Double          msdfgen_DistanceMapping_map(msdfgen_DistanceMapping* distanceMapping, msdfgen_Double d);
MSDFGEN_PUBLIC msdfgen_Double          msdfgen_DistanceMapping_mapDelta(msdfgen_DistanceMapping* distanceMapping, msdfgen_Double d);
MSDFGEN_PUBLIC msdfgen_DistanceMapping msdfgen_DistanceMapping_inverse(msdfgen_DistanceMapping* distanceMapping);

// Projection
MSDFGEN_PUBLIC msdfgen_ProjectionHandle msdfgen_Projection_create();
MSDFGEN_PUBLIC msdfgen_ProjectionHandle msdfgen_Projection_createRange(msdfgen_Vector2* scale, msdfgen_Vector2* translate);
MSDFGEN_PUBLIC msdfgen_Void             msdfgen_Projection_destroy(msdfgen_ProjectionHandle projection);
MSDFGEN_PUBLIC msdfgen_Point2           msdfgen_Projection_project(msdfgen_ProjectionHandle projection, msdfgen_Point2* coord);
MSDFGEN_PUBLIC msdfgen_Point2           msdfgen_Projection_unproject(msdfgen_ProjectionHandle projection, msdfgen_Point2* coord);
MSDFGEN_PUBLIC msdfgen_Vector2          msdfgen_Projection_projectVector(msdfgen_ProjectionHandle projection, msdfgen_Vector2* vector);
MSDFGEN_PUBLIC msdfgen_Vector2          msdfgen_Projection_unprojectVector(msdfgen_ProjectionHandle projection, msdfgen_Vector2* vector);
MSDFGEN_PUBLIC msdfgen_Double           msdfgen_Projection_projectX(msdfgen_ProjectionHandle projection, msdfgen_Double x);
MSDFGEN_PUBLIC msdfgen_Double           msdfgen_Projection_projectY(msdfgen_ProjectionHandle projection, msdfgen_Double y);
MSDFGEN_PUBLIC msdfgen_Double           msdfgen_Projection_unprojectX(msdfgen_ProjectionHandle projection, msdfgen_Double x);
MSDFGEN_PUBLIC msdfgen_Double           msdfgen_Projection_unprojectY(msdfgen_ProjectionHandle projection, msdfgen_Double y);
MSDFGEN_PUBLIC msdfgen_Vector2          msdfgen_Projection_getScale(msdfgen_ProjectionHandle projection);
MSDFGEN_PUBLIC msdfgen_Void             msdfgen_Projection_setScale(msdfgen_ProjectionHandle projection, msdfgen_Vector2* scale);
MSDFGEN_PUBLIC msdfgen_Vector2          msdfgen_Projection_getTranslate(msdfgen_ProjectionHandle projection);
MSDFGEN_PUBLIC msdfgen_Void             msdfgen_Projection_setTranslate(msdfgen_ProjectionHandle projection, msdfgen_Vector2* translate);

// SDF transformation
MSDFGEN_PUBLIC msdfgen_SDFTransformationHandle msdfgen_SDFTransformation_create();
MSDFGEN_PUBLIC msdfgen_SDFTransformationHandle msdfgen_SDFTransformation_createProjectionDistanceMapping(msdfgen_ProjectionHandle projection, msdfgen_DistanceMapping* distanceMapping);
MSDFGEN_PUBLIC msdfgen_DistanceMapping         msdfgen_SDFTransformation_getDistanceMapping(msdfgen_SDFTransformationHandle transformation);
MSDFGEN_PUBLIC msdfgen_Void                    msdfgen_SDFTransformation_setDistanceMapping(msdfgen_SDFTransformationHandle transformation, msdfgen_DistanceMapping* distanceMapping);
MSDFGEN_PUBLIC msdfgen_ProjectionHandle        msdfgen_SDFTransformation_toBase(msdfgen_SDFTransformationHandle transformation);

// Generator config
MSDFGEN_PUBLIC msdfgen_GeneratorConfigHandle msdfgen_GeneratorConfig_create(msdfgen_Bool overlapSupport);
MSDFGEN_PUBLIC msdfgen_Void                  msdfgen_GeneratorConfig_destroy(msdfgen_GeneratorConfigHandle config);
MSDFGEN_PUBLIC msdfgen_Bool                  msdfgen_GeneratorConfig_getOverlapSupport(msdfgen_GeneratorConfigHandle config);
MSDFGEN_PUBLIC msdfgen_Void                  msdfgen_GeneratorConfig_setOverlapSupport(msdfgen_GeneratorConfigHandle config, msdfgen_Bool overlapSupport);

// MSDF generator config
MSDFGEN_PUBLIC msdfgen_MSDFGeneratorConfigHandle msdfgen_MSDFGeneratorConfig_create(msdfgen_Bool overlapSupport, msdfgen_ErrorCorrectionConfig* errorCorrectionConfig);
MSDFGEN_PUBLIC msdfgen_ErrorCorrectionConfig     msdfgen_MSDFGeneratorConfig_getErrorCorrectionConfig(msdfgen_MSDFGeneratorConfigHandle config);
MSDFGEN_PUBLIC msdfgen_Void                      msdfgen_MSDFGeneratorConfig_setErrorCorrectionConfig(msdfgen_MSDFGeneratorConfigHandle config, msdfgen_ErrorCorrectionConfig* errorCorrectionConfig);
MSDFGEN_PUBLIC msdfgen_GeneratorConfigHandle     msdfgen_MSDFGeneratorConfig_toBase(msdfgen_MSDFGeneratorConfigHandle config);

// Edge coloring
MSDFGEN_PUBLIC msdfgen_Void msdfgen_edgeColoringSimple(msdfgen_ShapeHandle shape, msdfgen_Double angleThreshold, msdfgen_ULong seed);
MSDFGEN_PUBLIC msdfgen_Void msdfgen_edgeColoringInkTrap(msdfgen_ShapeHandle shape, msdfgen_Double angleThreshold, msdfgen_ULong seed);
MSDFGEN_PUBLIC msdfgen_Void msdfgen_edgeColoringByDistance(msdfgen_ShapeHandle shape, msdfgen_Double angleThreshold, msdfgen_ULong seed);

// SDF generation
MSDFGEN_PUBLIC msdfgen_Void msdfgen_generateSDF(msdfgen_BitmapRef* output, msdfgen_ShapeHandle shape, msdfgen_SDFTransformationHandle transformation, msdfgen_GeneratorConfigHandle config);
MSDFGEN_PUBLIC msdfgen_Void msdfgen_generatePSDF(msdfgen_BitmapRef* output, msdfgen_ShapeHandle shape, msdfgen_SDFTransformationHandle transformation, msdfgen_GeneratorConfigHandle config);
MSDFGEN_PUBLIC msdfgen_Void msdfgen_generateMSDF(msdfgen_BitmapRef* output, msdfgen_ShapeHandle shape, msdfgen_SDFTransformationHandle transformation, msdfgen_MSDFGeneratorConfigHandle config);
MSDFGEN_PUBLIC msdfgen_Void msdfgen_generateMTSDF(msdfgen_BitmapRef* output, msdfgen_ShapeHandle shape, msdfgen_SDFTransformationHandle transformation, msdfgen_MSDFGeneratorConfigHandle config);

#ifdef __cplusplus
}
#endif