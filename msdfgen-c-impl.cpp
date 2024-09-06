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

// Edge holder (+ Edge segment)
msdfgen_EdgeHolderHandle msdfgen_EdgeHolder_createLinear(msdfgen_Point2 p0, msdfgen_Point2 p1, msdfgen_EdgeColor edgeColor) {
    msdfgen::EdgeHolder* edgeHolder = new msdfgen::EdgeHolder(msdfgen::Point2(p0.x, p0.y), msdfgen::Point2(p1.x, p1.y), (msdfgen::EdgeColor)edgeColor);
    return reinterpret_cast<msdfgen_EdgeHolderHandle>(edgeHolder);
}

msdfgen_EdgeHolderHandle msdfgen_EdgeHolder_createQuadratic(msdfgen_Point2 p0, msdfgen_Point2 p1, msdfgen_Point2 p2, msdfgen_EdgeColor edgeColor) {
    msdfgen::EdgeHolder* edgeHolder = new msdfgen::EdgeHolder(msdfgen::Point2(p0.x, p0.y), msdfgen::Point2(p1.x, p1.y), msdfgen::Point2(p2.x, p2.y), (msdfgen::EdgeColor)edgeColor);
    return reinterpret_cast<msdfgen_EdgeHolderHandle>(edgeHolder);
}

msdfgen_EdgeHolderHandle msdfgen_EdgeHolder_createCubic(msdfgen_Point2 p0, msdfgen_Point2 p1, msdfgen_Point2 p2, msdfgen_Point2 p3, msdfgen_EdgeColor edgeColor) {
    msdfgen::EdgeHolder* edgeHolder = new msdfgen::EdgeHolder(msdfgen::Point2(p0.x, p0.y), msdfgen::Point2(p1.x, p1.y), msdfgen::Point2(p2.x, p2.y), msdfgen::Point2(p3.x, p3.y), (msdfgen::EdgeColor)edgeColor);
    return reinterpret_cast<msdfgen_EdgeHolderHandle>(edgeHolder);
}

msdfgen_Void msdfgen_EdgeHolder_destroy(msdfgen_EdgeHolderHandle edgeHolder) {
    delete reinterpret_cast<msdfgen::EdgeHolder*>(edgeHolder);
}

msdfgen_EdgeType msdfgen_EdgeHolder_type(msdfgen_EdgeHolderHandle edgeHolder) {
    msdfgen::EdgeHolder* holder = reinterpret_cast<msdfgen::EdgeHolder*>(edgeHolder);
    msdfgen::EdgeSegment* segment = *holder;
    return (msdfgen_EdgeType)segment->type();
}

msdfgen_Size msdfgen_EdgeHolder_getControlPointCount(msdfgen_EdgeHolderHandle edgeHolder) {
    msdfgen::EdgeHolder* holder = reinterpret_cast<msdfgen::EdgeHolder*>(edgeHolder);
    msdfgen::EdgeSegment* segment = *holder;
    return segment->type() + 1;
}

msdfgen_Void msdfgen_EdgeHolder_getControlPoints(msdfgen_EdgeHolderHandle edgeHolder, msdfgen_Point2* points) {
    msdfgen::EdgeHolder* holder = reinterpret_cast<msdfgen::EdgeHolder*>(edgeHolder);
    msdfgen::EdgeSegment* segment = *holder;
    const msdfgen::Point2* controlPoints = segment->controlPoints();
    for (size_t i = 0; i < segment->type() + 1; i++) {
        points[i].x = controlPoints[i].x;
        points[i].y = controlPoints[i].y;
    }
}

// Contour
msdfgen_ContourHandle msdfgen_Contour_create() {
    return reinterpret_cast<msdfgen_ContourHandle>(new msdfgen::Contour());
}

msdfgen_Void msdfgen_Contour_destroy(msdfgen_ContourHandle contour) {
    delete reinterpret_cast<msdfgen::Contour*>(contour);
}

msdfgen_Void msdfgen_Contour_addEdge(msdfgen_ContourHandle contour, msdfgen_EdgeHolderHandle edge) {
    reinterpret_cast<msdfgen::Contour*>(contour)->addEdge(*reinterpret_cast<msdfgen::EdgeHolder*>(edge));
}

msdfgen_EdgeHolderHandle msdfgen_Contour_addEdgeNew(msdfgen_ContourHandle contour) {
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

msdfgen_Void msdfgen_Shape_addContour(msdfgen_ShapeHandle shape, msdfgen_ContourHandle contour) {
    reinterpret_cast<msdfgen::Shape*>(shape)->addContour(*reinterpret_cast<msdfgen::Contour*>(contour));
}

msdfgen_ContourHandle msdfgen_Shape_addContourNew(msdfgen_ShapeHandle shape) {
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
msdfgen_DistanceMappingHandle msdfgen_DistanceMapping_create() {
    return reinterpret_cast<msdfgen_DistanceMappingHandle>(new msdfgen::DistanceMapping());
}

msdfgen_DistanceMappingHandle msdfgen_DistanceMapping_createRange(msdfgen_Range range) {
    return reinterpret_cast<msdfgen_DistanceMappingHandle>(new msdfgen::DistanceMapping(msdfgen::Range(range.lower, range.upper)));
}

msdfgen_Void msdfgen_DistanceMapping_destroy(msdfgen_DistanceMappingHandle distanceMapping) {
    delete reinterpret_cast<msdfgen::DistanceMapping*>(distanceMapping);
}

msdfgen_Double msdfgen_DistanceMapping_map(msdfgen_DistanceMappingHandle distanceMapping, msdfgen_Double d) {
    return (*reinterpret_cast<msdfgen::DistanceMapping*>(distanceMapping))(d);
}

msdfgen_Double msdfgen_DistanceMapping_map_delta(msdfgen_DistanceMappingHandle distanceMapping, msdfgen_Double d) {
    return (*reinterpret_cast<msdfgen::DistanceMapping*>(distanceMapping))(msdfgen::DistanceMapping::Delta(d));
}

msdfgen_DistanceMappingHandle msdfgen_DistanceMapping_inverse(msdfgen_DistanceMappingHandle distanceMapping) {
    return reinterpret_cast<msdfgen_DistanceMappingHandle>(new msdfgen::DistanceMapping(reinterpret_cast<msdfgen::DistanceMapping*>(distanceMapping)->inverse()));
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

msdfgen_SDFTransformationHandle msdfgen_SDFTransformation_createProjectionDistanceMapping(msdfgen_ProjectionHandle projection, msdfgen_DistanceMappingHandle distanceMapping) {
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
