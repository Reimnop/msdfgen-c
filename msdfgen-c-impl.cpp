#include "msdfgen.h"

#include "msdfgen-c.h"

struct BaseVectorView {
    virtual ~BaseVectorView() { }

    virtual void* getData() = 0;
    virtual size_t getCount() = 0;
    virtual size_t getElementSize() = 0;
    virtual void add(void* element) = 0;
    virtual void insert(size_t index, void* element) = 0;
    virtual void remove(size_t index) = 0;
    virtual void clear() = 0;
};

template <typename T>
class VectorView : BaseVectorView {
public:
    std::vector<T>& data;

    VectorView(std::vector<T>& data) : data(data) { }

    void* getData() override { return data.data(); }
    size_t getCount() override { return data.size(); }
    size_t getElementSize() override { return sizeof(T); }
    void add(void* element) override { data.push_back(*reinterpret_cast<T*>(element)); }
    void insert(size_t index, void* element) override { data.insert(data.begin() + index, *reinterpret_cast<T*>(element)); }
    void remove(size_t index) override { data.erase(data.begin() + index); }
    void clear() override { data.clear(); }
};

// Vector view
msdfgen_Void msdfgen_VectorView_destroy(msdfgen_VectorViewHandle vectorView, msdfgen_Size* count, msdfgen_Void** data) {
    BaseVectorView* view = reinterpret_cast<BaseVectorView*>(vectorView);
    delete view;
}

msdfgen_Void msdfgen_VectorView_data(msdfgen_VectorViewHandle vectorView, msdfgen_Size* count, msdfgen_Void** data) {
    BaseVectorView* view = reinterpret_cast<BaseVectorView*>(vectorView);
    *count = view->getCount();
    *data = view->getData();
}

msdfgen_Size msdfgen_VectorView_elementSize(msdfgen_VectorViewHandle vectorView) {
    BaseVectorView* view = reinterpret_cast<BaseVectorView*>(vectorView);
    return view->getElementSize();
}

msdfgen_Size msdfgen_VectorView_count(msdfgen_VectorViewHandle vectorView) {
    BaseVectorView* view = reinterpret_cast<BaseVectorView*>(vectorView);
    return view->getCount();
}

msdfgen_Error msdfgen_VectorView_at(msdfgen_VectorViewHandle vectorView, msdfgen_Size index, msdfgen_Void* element) {
    BaseVectorView* view = reinterpret_cast<BaseVectorView*>(vectorView);
    if (index >= view->getCount())
        return msdfgen_Error_OutOfBounds;
    memcpy(element, (uint8_t*)view->getData() + index * view->getElementSize(), view->getElementSize());
    return msdfgen_Error_Ok;
}

msdfgen_Error msdfgen_VectorView_set(msdfgen_VectorViewHandle vectorView, msdfgen_Size index, msdfgen_Void* element) {
    BaseVectorView* view = reinterpret_cast<BaseVectorView*>(vectorView);
    if (index >= view->getCount())
        return msdfgen_Error_OutOfBounds;
    memcpy((uint8_t*)view->getData() + index * view->getElementSize(), element, view->getElementSize());
    return msdfgen_Error_Ok;
}

msdfgen_Void msdfgen_VectorView_add(msdfgen_VectorViewHandle vectorView, msdfgen_Void* element) {
    BaseVectorView* view = reinterpret_cast<BaseVectorView*>(vectorView);
    view->add(element);
}

msdfgen_Error msdfgen_VectorView_insert(msdfgen_VectorViewHandle vectorView, msdfgen_Size index, msdfgen_Void* element) {
    BaseVectorView* view = reinterpret_cast<BaseVectorView*>(vectorView);
    if (index > view->getCount())
        return msdfgen_Error_OutOfBounds;
    view->insert(index, element);
    return msdfgen_Error_Ok;
}

msdfgen_Error msdfgen_VectorView_remove(msdfgen_VectorViewHandle vectorView, msdfgen_Size index) {
    BaseVectorView* view = reinterpret_cast<BaseVectorView*>(vectorView);
    if (index >= view->getCount())
        return msdfgen_Error_OutOfBounds;
    view->remove(index);
    return msdfgen_Error_Ok;
}

msdfgen_Void msdfgen_VectorView_clear(msdfgen_VectorViewHandle vectorView) {
    BaseVectorView* view = reinterpret_cast<BaseVectorView*>(vectorView);
    view->clear();
}

// Edge holder
msdfgen_EdgeSegmentHandle msdfgen_EdgeHolder_getSegment(msdfgen_EdgeHolderHandle edgeHolder) {
    return reinterpret_cast<msdfgen_EdgeSegmentHandle>(reinterpret_cast<msdfgen::EdgeHolder*>(edgeHolder)->edgeSegment);
}

msdfgen_Void msdfgen_EdgeHolder_setSegment(msdfgen_EdgeHolderHandle edgeHolder, msdfgen_EdgeSegmentHandle segment) {
    reinterpret_cast<msdfgen::EdgeHolder*>(edgeHolder)->edgeSegment = reinterpret_cast<msdfgen::EdgeSegment*>(segment);
}

// Edge segment
msdfgen_Void msdfgen_EdgeSegment_destroy(msdfgen_EdgeSegmentHandle edge) {
    delete reinterpret_cast<msdfgen::EdgeSegment*>(edge);
}

msdfgen_EdgeSegmentHandle msdfgen_EdgeSegment_clone(msdfgen_EdgeSegmentHandle edge) {
    return reinterpret_cast<msdfgen_EdgeSegmentHandle>(reinterpret_cast<msdfgen::EdgeSegment*>(edge)->clone());
}

msdfgen_EdgeType msdfgen_EdgeSegment_type(msdfgen_EdgeSegmentHandle edge) {
    return (msdfgen_EdgeType)reinterpret_cast<msdfgen::EdgeSegment*>(edge)->type();
}

msdfgen_Size msdfgen_EdgeSegment_getControlPointsCount(msdfgen_EdgeSegmentHandle edge) {
    return reinterpret_cast<msdfgen::EdgeSegment*>(edge)->type() + 1;
}

msdfgen_Void msdfgen_EdgeSegment_getControlPoints(msdfgen_EdgeSegmentHandle edge, msdfgen_Point2* points) {
    const msdfgen::Point2* controlPoints = reinterpret_cast<msdfgen::EdgeSegment*>(edge)->controlPoints();
    for (int i = 0; i < reinterpret_cast<msdfgen::EdgeSegment*>(edge)->type() + 1; ++i)
        points[i] = { controlPoints[i].x, controlPoints[i].y };
}

msdfgen_Point2 msdfgen_EdgeSegment_point(msdfgen_EdgeSegmentHandle edge, msdfgen_Double t) {
    msdfgen::Point2 result = reinterpret_cast<msdfgen::EdgeSegment*>(edge)->point(t);
    return { result.x, result.y };
}

msdfgen_Vector2 msdfgen_EdgeSegment_direction(msdfgen_EdgeSegmentHandle edge, msdfgen_Double t) {
    msdfgen::Vector2 result = reinterpret_cast<msdfgen::EdgeSegment*>(edge)->direction(t);
    return { result.x, result.y };
}

msdfgen_Vector2 msdfgen_EdgeSegment_directionChange(msdfgen_EdgeSegmentHandle edge, msdfgen_Double t) {
    msdfgen::Vector2 result = reinterpret_cast<msdfgen::EdgeSegment*>(edge)->directionChange(t);
    return { result.x, result.y };
}

msdfgen_SignedDistance msdfgen_EdgeSegment_signedDistance(msdfgen_EdgeSegmentHandle edge, msdfgen_Point2 origin, msdfgen_Double* t) {
    msdfgen::SignedDistance result = reinterpret_cast<msdfgen::EdgeSegment*>(edge)->signedDistance(msdfgen::Point2(origin.x, origin.y), *t);
    return { result.distance, result.dot };
}

msdfgen_Void msdfgen_EdgeSegment_distanceToPerpendicularDistance(msdfgen_EdgeSegmentHandle edge, msdfgen_SignedDistance* distance, msdfgen_Point2 origin, msdfgen_Double t) {
    reinterpret_cast<msdfgen::EdgeSegment*>(edge)->distanceToPerpendicularDistance(*reinterpret_cast<msdfgen::SignedDistance*>(distance), msdfgen::Point2(origin.x, origin.y), t);
}

msdfgen_Int msdfgen_EdgeSegment_scanlineIntersections(msdfgen_EdgeSegmentHandle edge, msdfgen_Double x[3], msdfgen_Int dy[3], msdfgen_Double y) {
    return reinterpret_cast<msdfgen::EdgeSegment*>(edge)->scanlineIntersections(x, dy, y);
}

msdfgen_Void msdfgen_EdgeSegment_bound(msdfgen_EdgeSegmentHandle edge, msdfgen_Double* l, msdfgen_Double* b, msdfgen_Double* r, msdfgen_Double* t) {
    reinterpret_cast<msdfgen::EdgeSegment*>(edge)->bound(*l, *b, *r, *t);
}

msdfgen_Void msdfgen_EdgeSegment_reverse(msdfgen_EdgeSegmentHandle edge) {
    reinterpret_cast<msdfgen::EdgeSegment*>(edge)->reverse();
}

msdfgen_Void msdfgen_EdgeSegment_moveStartPoint(msdfgen_EdgeSegmentHandle edge, msdfgen_Point2 to) {
    reinterpret_cast<msdfgen::EdgeSegment*>(edge)->moveStartPoint(msdfgen::Point2(to.x, to.y));
}

msdfgen_Void msdfgen_EdgeSegment_moveEndPoint(msdfgen_EdgeSegmentHandle edge, msdfgen_Point2 to) {
    reinterpret_cast<msdfgen::EdgeSegment*>(edge)->moveEndPoint(msdfgen::Point2(to.x, to.y));
}

msdfgen_Void msdfgen_EdgeSegment_splitInThirds(msdfgen_EdgeSegmentHandle edge, msdfgen_EdgeSegmentHandle* part0, msdfgen_EdgeSegmentHandle* part1, msdfgen_EdgeSegmentHandle* part2) {
    msdfgen::EdgeSegment* p0, *p1, *p2;
    reinterpret_cast<msdfgen::EdgeSegment*>(edge)->splitInThirds(p0, p1, p2);
    *part0 = reinterpret_cast<msdfgen_EdgeSegmentHandle>(p0);
    *part1 = reinterpret_cast<msdfgen_EdgeSegmentHandle>(p1);
    *part2 = reinterpret_cast<msdfgen_EdgeSegmentHandle>(p2);
}

// Linear segment
msdfgen_LinearSegmentHandle msdfgen_LinearSegment_create(msdfgen_Point2 p0, msdfgen_Point2 p1, msdfgen_EdgeColor edgeColor) {
    return reinterpret_cast<msdfgen_LinearSegmentHandle>(new msdfgen::LinearSegment(msdfgen::Point2(p0.x, p0.y), msdfgen::Point2(p1.x, p1.y), (msdfgen::EdgeColor)edgeColor));
}

msdfgen_EdgeSegmentHandle msdfgen_LinearSegment_toBase(msdfgen_LinearSegmentHandle segment) {
    return reinterpret_cast<msdfgen_EdgeSegmentHandle>(reinterpret_cast<msdfgen::LinearSegment*>(segment));
}

// Quadratic segment
msdfgen_QuadraticSegmentHandle msdfgen_QuadraticSegment_create(msdfgen_Point2 p0, msdfgen_Point2 p1, msdfgen_Point2 p2, msdfgen_EdgeColor edgeColor) {
    return reinterpret_cast<msdfgen_QuadraticSegmentHandle>(new msdfgen::QuadraticSegment(msdfgen::Point2(p0.x, p0.y), msdfgen::Point2(p1.x, p1.y), msdfgen::Point2(p2.x, p2.y), (msdfgen::EdgeColor)edgeColor));
}

msdfgen_CubicSegmentHandle msdfgen_QuadraticSegment_convertToCubic(msdfgen_QuadraticSegmentHandle segment) {
    return reinterpret_cast<msdfgen_CubicSegmentHandle>(reinterpret_cast<msdfgen::QuadraticSegment*>(segment)->convertToCubic());
}

msdfgen_EdgeSegmentHandle msdfgen_QuadraticSegment_toBase(msdfgen_QuadraticSegmentHandle segment) {
    return reinterpret_cast<msdfgen_EdgeSegmentHandle>(reinterpret_cast<msdfgen::QuadraticSegment*>(segment));
}

// Cubic segment
msdfgen_CubicSegmentHandle msdfgen_CubicSegment_create(msdfgen_Point2 p0, msdfgen_Point2 p1, msdfgen_Point2 p2, msdfgen_Point2 p3, msdfgen_EdgeColor edgeColor) {
    return reinterpret_cast<msdfgen_CubicSegmentHandle>(new msdfgen::CubicSegment(msdfgen::Point2(p0.x, p0.y), msdfgen::Point2(p1.x, p1.y), msdfgen::Point2(p2.x, p2.y), msdfgen::Point2(p3.x, p3.y), (msdfgen::EdgeColor)edgeColor));
}

msdfgen_EdgeSegmentHandle msdfgen_CubicSegment_toBase(msdfgen_CubicSegmentHandle segment) {
    return reinterpret_cast<msdfgen_EdgeSegmentHandle>(reinterpret_cast<msdfgen::CubicSegment*>(segment));
}

// Contour
msdfgen_EdgeHolderHandle msdfgen_Contour_addEdge(msdfgen_ContourHandle contour) {
    msdfgen::EdgeHolder& edge = reinterpret_cast<msdfgen::Contour*>(contour)->addEdge();
    return reinterpret_cast<msdfgen_EdgeHolderHandle>(&edge);
}

msdfgen_Void msdfgen_Contour_bound(msdfgen_ContourHandle contour, msdfgen_Double* l, msdfgen_Double* b, msdfgen_Double* r, msdfgen_Double* t) {
    reinterpret_cast<msdfgen::Contour*>(contour)->bound(*l, *b, *r, *t);
}

msdfgen_Void msdfgen_Contour_boundMiters(msdfgen_ContourHandle contour, msdfgen_Double* l, msdfgen_Double* b, msdfgen_Double* r, msdfgen_Double* t, msdfgen_Double border, msdfgen_Double miterLimit, msdfgen_Int polarity) {
    reinterpret_cast<msdfgen::Contour*>(contour)->boundMiters(*l, *b, *r, *t, border, miterLimit, polarity);
}

msdfgen_Int msdfgen_Contour_winding(msdfgen_ContourHandle contour) {
    return reinterpret_cast<msdfgen::Contour*>(contour)->winding();
}

msdfgen_Void msdfgen_Contour_reverse(msdfgen_ContourHandle contour) {
    reinterpret_cast<msdfgen::Contour*>(contour)->reverse();
}

msdfgen_VectorViewHandle msdfgen_Contour_createEdgesView(msdfgen_ContourHandle contour) {
    msdfgen::Contour* c = reinterpret_cast<msdfgen::Contour*>(contour);
    return reinterpret_cast<msdfgen_VectorViewHandle>(new VectorView<msdfgen::EdgeHolder>(c->edges));
}

// Shape
msdfgen_ShapeHandle msdfgen_Shape_create() {
    return reinterpret_cast<msdfgen_ShapeHandle>(new msdfgen::Shape());
}

msdfgen_Void msdfgen_Shape_destroy(msdfgen_ShapeHandle shape) {
    delete reinterpret_cast<msdfgen::Shape*>(shape);
}

msdfgen_ContourHandle msdfgen_Shape_addContour(msdfgen_ShapeHandle shape) {
    msdfgen::Contour& contour = reinterpret_cast<msdfgen::Shape*>(shape)->addContour();
    return reinterpret_cast<msdfgen_ContourHandle>(&contour);
}

msdfgen_Void msdfgen_Shape_normalize(msdfgen_ShapeHandle shape) {
    reinterpret_cast<msdfgen::Shape*>(shape)->normalize();
}

msdfgen_Bool msdfgen_Shape_validate(msdfgen_ShapeHandle shape) {
    return reinterpret_cast<msdfgen::Shape*>(shape)->validate();
}

msdfgen_Void msdfgen_Shape_bound(msdfgen_ShapeHandle shape, msdfgen_Double* l, msdfgen_Double* b, msdfgen_Double* r, msdfgen_Double* t) {
    reinterpret_cast<msdfgen::Shape*>(shape)->bound(*l, *b, *r, *t);
}

msdfgen_Void msdfgen_Shape_boundMiters(msdfgen_ShapeHandle shape, msdfgen_Double* l, msdfgen_Double* b, msdfgen_Double* r, msdfgen_Double* t, msdfgen_Double border, msdfgen_Double miterLimit, msdfgen_Int polarity) {
    reinterpret_cast<msdfgen::Shape*>(shape)->boundMiters(*l, *b, *r, *t, border, miterLimit, polarity);
}

msdfgen_Bounds msdfgen_Shape_getBounds(msdfgen_ShapeHandle shape, msdfgen_Double border, msdfgen_Double miterLimit, msdfgen_Int polarity) {
    msdfgen::Shape::Bounds bounds = reinterpret_cast<msdfgen::Shape*>(shape)->getBounds(border, miterLimit, polarity);
    return { bounds.l, bounds.b, bounds.r, bounds.t };
}

msdfgen_Void msdfgen_Shape_scanline(msdfgen_ShapeHandle shape, msdfgen_ScanlineHandle line, msdfgen_Double y) {
    reinterpret_cast<msdfgen::Shape*>(shape)->scanline(*reinterpret_cast<msdfgen::Scanline*>(line), y);
}

msdfgen_Int msdfgen_Shape_edgeCount(msdfgen_ShapeHandle shape) {
    return reinterpret_cast<msdfgen::Shape*>(shape)->edgeCount();
}

msdfgen_Void msdfgen_Shape_orientContours(msdfgen_ShapeHandle shape) {
    reinterpret_cast<msdfgen::Shape*>(shape)->orientContours();
}

msdfgen_Bool msdfgen_Shape_getInverseYAxis(msdfgen_ShapeHandle shape) {
    return reinterpret_cast<msdfgen::Shape*>(shape)->inverseYAxis;
}

msdfgen_Void msdfgen_Shape_setInverseYAxis(msdfgen_ShapeHandle shape, msdfgen_Bool inverseYAxis) {
    reinterpret_cast<msdfgen::Shape*>(shape)->inverseYAxis = inverseYAxis;
}

msdfgen_VectorViewHandle msdfgen_Shape_createContoursView(msdfgen_ShapeHandle shape) {
    msdfgen::Shape* s = reinterpret_cast<msdfgen::Shape*>(shape);
    return reinterpret_cast<msdfgen_VectorViewHandle>(new VectorView<msdfgen::Contour>(s->contours));
}

// Distance mapping
msdfgen_DistanceMapping msdfgen_DistanceMapping_createRange(msdfgen_Range range) {
    msdfgen::DistanceMapping mapping(msdfgen::Range(range.lower, range.upper));
    return { mapping.scale, mapping.translate };
}

msdfgen_Double msdfgen_DistanceMapping_map(msdfgen_DistanceMapping distanceMapping, msdfgen_Double d) {
    msdfgen::DistanceMapping mapping(distanceMapping.scale, distanceMapping.translate);
    return mapping(d);
}

msdfgen_Double msdfgen_DistanceMapping_map_delta(msdfgen_DistanceMapping distanceMapping, msdfgen_Double d) {
    msdfgen::DistanceMapping mapping(distanceMapping.scale, distanceMapping.translate);
    return mapping(msdfgen::DistanceMapping::Delta(d));
}

msdfgen_DistanceMapping msdfgen_DistanceMapping_inverse(msdfgen_DistanceMapping distanceMapping) {
    msdfgen::DistanceMapping mapping(distanceMapping.scale, distanceMapping.translate);
    msdfgen::DistanceMapping inversedMapping = mapping.inverse();
    return { inversedMapping.scale, inversedMapping.translate };
}

// Projection
msdfgen_ProjectionHandle msdfgen_Projection_create() {
    return reinterpret_cast<msdfgen_ProjectionHandle>(new msdfgen::Projection());
}

msdfgen_ProjectionHandle msdfgen_Projection_createRange(msdfgen_Vector2* scale, msdfgen_Vector2* translate) {
    return reinterpret_cast<msdfgen_ProjectionHandle>(new msdfgen::Projection(msdfgen::Vector2(scale->x, scale->y), msdfgen::Vector2(translate->x, translate->y)));
}

msdfgen_Void msdfgen_Projection_destroy(msdfgen_ProjectionHandle projection) {
    delete reinterpret_cast<msdfgen::Projection*>(projection);
}

msdfgen_Point2 msdfgen_Projection_project(msdfgen_ProjectionHandle projection, msdfgen_Point2* coord) {
    msdfgen::Point2 result = reinterpret_cast<msdfgen::Projection*>(projection)->project(msdfgen::Point2(coord->x, coord->y));
    return { result.x, result.y };
}

msdfgen_Point2 msdfgen_Projection_unproject(msdfgen_ProjectionHandle projection, msdfgen_Point2* coord) {
    msdfgen::Point2 result = reinterpret_cast<msdfgen::Projection*>(projection)->unproject(msdfgen::Point2(coord->x, coord->y));
    return { result.x, result.y };
}

msdfgen_Vector2 msdfgen_Projection_projectVector(msdfgen_ProjectionHandle projection, msdfgen_Vector2* vector) {
    msdfgen::Vector2 result = reinterpret_cast<msdfgen::Projection*>(projection)->projectVector(msdfgen::Vector2(vector->x, vector->y));
    return { result.x, result.y };
}

msdfgen_Vector2 msdfgen_Projection_unprojectVector(msdfgen_ProjectionHandle projection, msdfgen_Vector2* vector) {
    msdfgen::Vector2 result = reinterpret_cast<msdfgen::Projection*>(projection)->unprojectVector(msdfgen::Vector2(vector->x, vector->y));
    return { result.x, result.y };
}

msdfgen_Double msdfgen_Projection_projectX(msdfgen_ProjectionHandle projection, msdfgen_Double x) {
    return reinterpret_cast<msdfgen::Projection*>(projection)->projectX(x);
}

msdfgen_Double msdfgen_Projection_projectY(msdfgen_ProjectionHandle projection, msdfgen_Double y) {
    return reinterpret_cast<msdfgen::Projection*>(projection)->projectY(y);
}

msdfgen_Double msdfgen_Projection_unprojectX(msdfgen_ProjectionHandle projection, msdfgen_Double x) {
    return reinterpret_cast<msdfgen::Projection*>(projection)->unprojectX(x);
}

msdfgen_Double msdfgen_Projection_unprojectY(msdfgen_ProjectionHandle projection, msdfgen_Double y) {
    return reinterpret_cast<msdfgen::Projection*>(projection)->unprojectY(y);
}

// SDF transformation
msdfgen_SDFTransformationHandle msdfgen_SDFTransformation_create() {
    return reinterpret_cast<msdfgen_SDFTransformationHandle>(new msdfgen::SDFTransformation());
}

msdfgen_SDFTransformationHandle msdfgen_SDFTransformation_createProjectionDistanceMapping(msdfgen_ProjectionHandle projection, msdfgen_DistanceMapping* distanceMapping) {
    return reinterpret_cast<msdfgen_SDFTransformationHandle>(new msdfgen::SDFTransformation(*reinterpret_cast<msdfgen::Projection*>(projection), *reinterpret_cast<msdfgen::DistanceMapping*>(distanceMapping)));
}

msdfgen_ProjectionHandle msdfgen_SDFTransformation_toBase(msdfgen_SDFTransformationHandle transformation) {
    return reinterpret_cast<msdfgen_ProjectionHandle>(transformation);
}

// Generator config
msdfgen_GeneratorConfigHandle msdfgen_GeneratorConfig_create(msdfgen_Bool overlapSupport) {
    return reinterpret_cast<msdfgen_GeneratorConfigHandle>(new msdfgen::GeneratorConfig(overlapSupport));
}

msdfgen_Void msdfgen_GeneratorConfig_destroy(msdfgen_GeneratorConfigHandle config) {
    delete reinterpret_cast<msdfgen::GeneratorConfig*>(config);
}

msdfgen_Bool msdfgen_GeneratorConfig_getOverlapSupport(msdfgen_GeneratorConfigHandle config) {
    return reinterpret_cast<msdfgen::GeneratorConfig*>(config)->overlapSupport;
}

msdfgen_Void msdfgen_GeneratorConfig_setOverlapSupport(msdfgen_GeneratorConfigHandle config, msdfgen_Bool overlapSupport) {
    reinterpret_cast<msdfgen::GeneratorConfig*>(config)->overlapSupport = overlapSupport;
}

// MSDF generator config
msdfgen_MSDFGeneratorConfigHandle msdfgen_MSDFGeneratorConfig_create(msdfgen_Bool overlapSupport, msdfgen_ErrorCorrectionConfig* errorCorrectionConfig) {
    return reinterpret_cast<msdfgen_MSDFGeneratorConfigHandle>(new msdfgen::MSDFGeneratorConfig(overlapSupport, *reinterpret_cast<msdfgen::ErrorCorrectionConfig*>(errorCorrectionConfig)));
}

msdfgen_ErrorCorrectionConfig msdfgen_MSDFGeneratorConfig_getErrorCorrectionConfig(msdfgen_MSDFGeneratorConfigHandle config) {
    msdfgen::ErrorCorrectionConfig& cfg = reinterpret_cast<msdfgen::MSDFGeneratorConfig*>(config)->errorCorrection;
    return { (msdfgen_ErrorCorrectionConfig_Mode)cfg.mode, (msdfgen_ErrorCorrectionConfig_DistanceCheckMode)cfg.distanceCheckMode, cfg.minDeviationRatio, cfg.minImproveRatio, cfg.buffer };
}

msdfgen_Void msdfgen_MSDFGeneratorConfig_setErrorCorrectionConfig(msdfgen_MSDFGeneratorConfigHandle config, msdfgen_ErrorCorrectionConfig* errorCorrectionConfig) {
    reinterpret_cast<msdfgen::MSDFGeneratorConfig*>(config)->errorCorrection = *reinterpret_cast<msdfgen::ErrorCorrectionConfig*>(errorCorrectionConfig);
}

// SDF generation
msdfgen_Void msdfgen_generateSDF(msdfgen_BitmapRef* output, msdfgen_ShapeHandle shape, msdfgen_SDFTransformationHandle transformation, msdfgen_GeneratorConfigHandle config) {
    msdfgen::generateSDF(*reinterpret_cast<msdfgen::BitmapRef<float, 1>*>(output), *reinterpret_cast<msdfgen::Shape*>(shape), *reinterpret_cast<msdfgen::SDFTransformation*>(transformation), *reinterpret_cast<msdfgen::GeneratorConfig*>(config));
}

msdfgen_Void msdfgen_generatePSDF(msdfgen_BitmapRef* output, msdfgen_ShapeHandle shape, msdfgen_SDFTransformationHandle transformation, msdfgen_GeneratorConfigHandle config) {
    msdfgen::generatePSDF(*reinterpret_cast<msdfgen::BitmapRef<float, 1>*>(output), *reinterpret_cast<msdfgen::Shape*>(shape), *reinterpret_cast<msdfgen::SDFTransformation*>(transformation), *reinterpret_cast<msdfgen::GeneratorConfig*>(config));
}

msdfgen_Void msdfgen_generateMSDF(msdfgen_BitmapRef* output, msdfgen_ShapeHandle shape, msdfgen_SDFTransformationHandle transformation, msdfgen_MSDFGeneratorConfigHandle config) {
    msdfgen::generateMSDF(*reinterpret_cast<msdfgen::BitmapRef<float, 3>*>(output), *reinterpret_cast<msdfgen::Shape*>(shape), *reinterpret_cast<msdfgen::SDFTransformation*>(transformation), *reinterpret_cast<msdfgen::MSDFGeneratorConfig*>(config));
}

msdfgen_Void msdfgen_generateMTSDF(msdfgen_BitmapRef* output, msdfgen_ShapeHandle shape, msdfgen_SDFTransformationHandle transformation, msdfgen_MSDFGeneratorConfigHandle config) {
    msdfgen::generateMTSDF(*reinterpret_cast<msdfgen::BitmapRef<float, 4>*>(output), *reinterpret_cast<msdfgen::Shape*>(shape), *reinterpret_cast<msdfgen::SDFTransformation*>(transformation), *reinterpret_cast<msdfgen::MSDFGeneratorConfig*>(config));
}
