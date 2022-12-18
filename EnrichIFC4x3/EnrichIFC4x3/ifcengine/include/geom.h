//
// Helper classes (C++ wrappers)
//
#ifndef __RDF_LTD__GEOM_H
#define __RDF_LTD__GEOM_H

#include    <assert.h>
#include	"engine.h"

namespace GEOM
{
    class AdvancedFace;
    class AdvancedFace2D;
    class AdvancedFace3D;
    class AdvancedFaceMapped;
    class AlignedSegment;
    class AlignedSegments;
    class Alignment;
    class AmbientLight;
    class Appearance;
    class Arc3D;
    class BezierCurve;
    class BezierSurface;
    class BiQuadraticParabola;
    class Blend;
    class BlossCurve;
    class BooleanOperation;
    class BooleanOperation2D;
    class BoundaryRepresentation;
    class Box;
    class BSplineCurve;
    class BSplineSurface;
    class Circle;
    class CircleByPoints;
    class ClippedPyramid;
    class Clipping;
    class Clothoid;
    class ClothoidCurve;
    class Collection;
    class Color;
    class ColorComponent;
    class Cone;
    class ConicalCurve;
    class ConicalSurface;
    class Copy;
    class CosineCurve;
    class Cube;
    class CubicParabola;
    class Cuboid;
    class Curve;
    class CurvesFromSurface;
    class Cylinder;
    class CylindricalSurface;
    class DegenerateToroidalSurface;
    class Deviation;
    class DirectionalLight;
    class DirectLight;
    class Dummy;
    class Ellipse;
    class EllipticCone;
    class Environment;
    class ExtrudedPolygon;
    class ExtrudedPolygonTapered;
    class ExtrusionAreaSolid;
    class ExtrusionAreaSolidSet;
    class Face;
    class Face2D;
    class Face2DSet;
    class FiniteSurface;
    class FrustumCone;
    class GeometricItem;
    class HornTorus;
    class Hyperbola;
    class InfiniteSurface;
    class Intersection;
    class IntersectionCurve;
    class IntersectionPoint;
    class InverseMatrix;
    class InvertedCurve;
    class InvertedSurface;
    class Light;
    class Line3D;
    class Line3Dn;
    class Line3DSet;
    class Line4D;
    class LineByFace;
    class Material;
    class Mathematics;
    class Matrix;
    class MatrixMultiplication;
    class Mesh;
    class NURBSCurve;
    class NURBSSurface;
    class Parabola;
    class Plane;
    class PlaneSurface;
    class Point;
    class Point3D;
    class Point3DSet;
    class Point3DSetByGeometricItem;
    class Point4D;
    class PointLight;
    class PointLoop;
    class Polygon2D;
    class Polygon3D;
    class Polygon4D;
    class PolyLine3D;
    class Prism;
    class Profile;
    class Projection;
    class Pyramid;
    class RationalBezierCurve;
    class RationalBezierSurface;
    class RectangleCurve;
    class Repetition;
    class RingTorus;
    class Shadow;
    class SineCurve;
    class SkewedCone;
    class SkewedFrustumCone;
    class Solid;
    class SolidBySurface;
    class SolidLine;
    class Sphere;
    class SphericalSurface;
    class SpindleTorus;
    class Spiral;
    class SplineCurve;
    class SplineSurface;
    class SpotLight;
    class Surface;
    class SurfaceBySweptCurve;
    class SurfaceOfLinearExtrusion;
    class SurfaceOfRevolution;
    class SweptAreaSolid;
    class SweptAreaSolidSet;
    class SweptAreaSolidTapered;
    class SweptBlend;
    class Texture;
    class ToroidalSurface;
    class Torus;
    class Transformation;
    class TransitionalCurve;
    class TriangleCurve;
    class TriangleReduction;
    class TriangleSet;
    class Vector;
    class Vector3;
    class View;
    class World;


    /// <summary>
    /// Provides utility methods to interact with a genetic instance of OWL class
    /// You also can use object of this class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Instance
    {
    protected:
        /// <summary>
        /// underlyed instance handle
        /// </summary>
        int64_t m_instance;

    protected:
        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="className"></param>
        /// <param name="instanceName"></param>
        /// <param name="instanceNameW"></param>
        /// <returns></returns>
        static int64_t Create(int64_t model, const char* className, const char* instanceName, const wchar_t* instanceNameW)
        {
            int64_t clsid = GetClassByName(model, className);
            assert(clsid != 0);

            int64_t instance = 0;
            if (instanceNameW) {
                instance = CreateInstanceW(clsid, instanceNameW);
            }
            else {
                instance = CreateInstance(clsid, instanceName);
            }

            return instance;
        }
        
        /// <summary>
        /// Constructs object that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        /// <param name="checkClassName">Expected OWL class of the instance, used for diagnostic (optionally)</param>
        Instance(int64_t instance, const char* cls)
        {
            m_instance = instance;
            if (cls != NULL && !::IsInstanceOfClass(instance, cls)) {
                m_instance = NULL;
            }
        }

    public:
        /// <summary>
        /// Conversion to instance handle, so the object of the class can be used anywhere where a handle required
        /// </summary>
        operator int64_t() const { return m_instance; }

        /// <summary>
        /// Get property id from property name
        /// </summary>
        int64_t GetPropertyId(const char* name, int64_t checkCardinality = -1)
        {
            int64_t model = GetModel(m_instance);

            int64_t propId = GetPropertyByName(model, name);

            if (propId) {
                int64_t clsId = GetInstanceClass(m_instance);
                int64_t minCard = 0, maxCard = 0;
                GetPropertyRestrictionsConsolidated(clsId, propId, &minCard, &maxCard);
                if (minCard < 0) {
                    propId = NULL; //property is not assigned to the class
                }
                if (checkCardinality > 0) { //chek cardinatity when set property
                    if (checkCardinality < minCard || (maxCard > 0 && checkCardinality > maxCard)) {
                        propId = NULL; //cardinality is out of range
                    }
                }
            }

            return propId;
        }

        ///<summary></summary>
        template<typename TElem> bool SetDatatypeProperty(const char* name, const TElem* values, int64_t count)
        {
            int64_t propId = GetPropertyId(name, count);
            int64_t res = ::SetDatatypeProperty(m_instance, propId, values, count);
            return (res == 0);
        }


        ///<summary>The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        template<typename TElem> const TElem*  GetDatatypeProperty(const char* name, int64_t* pCount)
        {
            int64_t propId = GetPropertyId(name);

            TElem* values = NULL;
            int64_t count = 0;
            int64_t res = ::GetDatatypeProperty(m_instance, propId, (void**)&values, &count);
            if (res != 0) {
                count = 0;
            }

            if (pCount) {
                *pCount = count;
            }

            if (count > 0) {
                return values;
            }
            else {
                return NULL;
            }
        }


        ///<summary></summary>
        template<class TInstance> bool SetObjectProperty(const char* name, const TInstance* instances, int64_t count)
        {
            int64_t propId = GetPropertyId(name, count);
            int64_t res = ::SetObjectProperty(m_instance, propId, (int64_t*)instances, count);
            return(res == 0);
        }

        ///<summary>The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        template<class TInstance> const TInstance* GetObjectProperty(const char* name, int64_t* pCount)
        {
            int64_t propId = GetPropertyId(name);

            int64_t* values = NULL;
            int64_t count = 0;
            int64_t res = ::GetObjectProperty(m_instance, propId, &values, &count);
            if (res != 0) {
                count = 0;            
            }

            if (pCount) {
                *pCount = count;
            }

            if (count > 0) {
                return (TInstance*)values;
            }
            else {
                return NULL;
            }
        }
    };

    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="TList"></typeparam>
    /// <typeparam name="TElem"></typeparam>
    /// <param name="values"></param>
    /// <param name="arr"></param>
    /// <param name="count"></param>
    template <typename TList, typename TElem> void ListToArray(const TList& values, TElem** arr, int64_t* count)
    {
        *count = 0;
        const TElem* supress_compilation_warningC4189 = NULL;
        for (const TElem v : values) {
            (*count)++;
            supress_compilation_warningC4189 = &v;
        }

        if (*count) {
            *arr = new TElem[*count];
            int64_t i = 0;
            for (const TElem v : values) {
                (*arr)[i++] = v;
            }
        }
        else {
            *arr = NULL;
        }
    }



    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class GeometricItem
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class GeometricItem : public Instance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class GeometricItem and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static GeometricItem Create(int64_t model, const char* name=NULL) { return GeometricItem(Instance::Create(model, "GeometricItem", name, NULL), "GeometricItem");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static GeometricItem CreateW(int64_t model, const wchar_t* name = NULL) { return GeometricItem(Instance::Create(model, "GeometricItem", NULL, name), "GeometricItem"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        GeometricItem(int64_t instance = NULL)
            : Instance(instance, "GeometricItem")
        {}

    protected:
        GeometricItem(int64_t instance, const char* checkClassName)
            : Instance(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to GeometricItem
       //

        ///<summary>Sets relationship from this instance to an instance of Material</summary>
        bool set_material(const Material& instance) { return SetObjectProperty<Material>("material", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Material* get_material() { return GetObjectProperty<Material>("material", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class AdvancedFace
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AdvancedFace : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AdvancedFace and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AdvancedFace Create(int64_t model, const char* name=NULL) { return AdvancedFace(Instance::Create(model, "AdvancedFace", name, NULL), "AdvancedFace");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AdvancedFace CreateW(int64_t model, const wchar_t* name = NULL) { return AdvancedFace(Instance::Create(model, "AdvancedFace", NULL, name), "AdvancedFace"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        AdvancedFace(int64_t instance = NULL)
            : GeometricItem(instance, "AdvancedFace")
        {}

    protected:
        AdvancedFace(int64_t instance, const char* checkClassName)
            : GeometricItem(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to AdvancedFace
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        bool set_bounds(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("bounds", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        bool set_bounds(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("bounds", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_bounds(int64_t* pCount) { return GetObjectProperty<Curve>("bounds", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_bounds_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("bounds", pCount); }
        ///<summary>Sets relationship from this instance to an instance of Surface</summary>
        bool set_surface(const Surface& instance) { return SetObjectProperty<Surface>("surface", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Surface* get_surface() { return GetObjectProperty<Surface>("surface", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Face
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Face : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Face and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Face Create(int64_t model, const char* name=NULL) { return Face(Instance::Create(model, "Face", name, NULL), "Face");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Face CreateW(int64_t model, const wchar_t* name = NULL) { return Face(Instance::Create(model, "Face", NULL, name), "Face"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Face(int64_t instance = NULL)
            : GeometricItem(instance, "Face")
        {}

    protected:
        Face(int64_t instance, const char* checkClassName)
            : GeometricItem(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class AdvancedFace2D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AdvancedFace2D : public AdvancedFace
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AdvancedFace2D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AdvancedFace2D Create(int64_t model, const char* name=NULL) { return AdvancedFace2D(Instance::Create(model, "AdvancedFace2D", name, NULL), "AdvancedFace2D");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AdvancedFace2D CreateW(int64_t model, const wchar_t* name = NULL) { return AdvancedFace2D(Instance::Create(model, "AdvancedFace2D", NULL, name), "AdvancedFace2D"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        AdvancedFace2D(int64_t instance = NULL)
            : AdvancedFace(instance, "AdvancedFace2D")
        {}

    protected:
        AdvancedFace2D(int64_t instance, const char* checkClassName)
            : AdvancedFace(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to AdvancedFace2D
       //

        ///<summary>Sets value of clipped</summary>
        bool set_clipped(bool value) { return SetDatatypeProperty ("clipped", &value, 1); }
        ///<summary>Gets a value of clipped, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_clipped() { return GetDatatypeProperty<bool>("clipped", NULL); }
        ///<summary>Sets value of scalingX</summary>
        bool set_scalingX(double value) { return SetDatatypeProperty ("scalingX", &value, 1); }
        ///<summary>Gets a value of scalingX, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_scalingX() { return GetDatatypeProperty<double>("scalingX", NULL); }
        ///<summary>Sets value of scalingY</summary>
        bool set_scalingY(double value) { return SetDatatypeProperty ("scalingY", &value, 1); }
        ///<summary>Gets a value of scalingY, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_scalingY() { return GetDatatypeProperty<double>("scalingY", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class AdvancedFace3D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AdvancedFace3D : public AdvancedFace
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AdvancedFace3D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AdvancedFace3D Create(int64_t model, const char* name=NULL) { return AdvancedFace3D(Instance::Create(model, "AdvancedFace3D", name, NULL), "AdvancedFace3D");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AdvancedFace3D CreateW(int64_t model, const wchar_t* name = NULL) { return AdvancedFace3D(Instance::Create(model, "AdvancedFace3D", NULL, name), "AdvancedFace3D"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        AdvancedFace3D(int64_t instance = NULL)
            : AdvancedFace(instance, "AdvancedFace3D")
        {}

    protected:
        AdvancedFace3D(int64_t instance, const char* checkClassName)
            : AdvancedFace(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to AdvancedFace3D
       //

        ///<summary>Sets values of isOuterBound. OWL cardinality 0..-1</summary>
        bool set_isOuterBound(const bool* values, int64_t count) { return SetDatatypeProperty ("isOuterBound", values, count); }
        ///<summary>Sets values of isOuterBound from enumerable collection (std::vector, std::list, etc.) of elements convertible to bool. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_isOuterBound(const TList& values) { bool* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_isOuterBound(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of isOuterBound. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_isOuterBound(int64_t* pCount) { return GetDatatypeProperty<bool>("isOuterBound", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Curve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Curve : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Curve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Curve Create(int64_t model, const char* name=NULL) { return Curve(Instance::Create(model, "Curve", name, NULL), "Curve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Curve CreateW(int64_t model, const wchar_t* name = NULL) { return Curve(Instance::Create(model, "Curve", NULL, name), "Curve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Curve(int64_t instance = NULL)
            : GeometricItem(instance, "Curve")
        {}

    protected:
        Curve(int64_t instance, const char* checkClassName)
            : GeometricItem(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class AdvancedFaceMapped
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AdvancedFaceMapped : public AdvancedFace
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AdvancedFaceMapped and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AdvancedFaceMapped Create(int64_t model, const char* name=NULL) { return AdvancedFaceMapped(Instance::Create(model, "AdvancedFaceMapped", name, NULL), "AdvancedFaceMapped");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AdvancedFaceMapped CreateW(int64_t model, const wchar_t* name = NULL) { return AdvancedFaceMapped(Instance::Create(model, "AdvancedFaceMapped", NULL, name), "AdvancedFaceMapped"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        AdvancedFaceMapped(int64_t instance = NULL)
            : AdvancedFace(instance, "AdvancedFaceMapped")
        {}

    protected:
        AdvancedFaceMapped(int64_t instance, const char* checkClassName)
            : AdvancedFace(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to AdvancedFaceMapped
       //

        ///<summary>Sets value of scalingX</summary>
        bool set_scalingX(double value) { return SetDatatypeProperty ("scalingX", &value, 1); }
        ///<summary>Gets a value of scalingX, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_scalingX() { return GetDatatypeProperty<double>("scalingX", NULL); }
        ///<summary>Sets value of scalingY</summary>
        bool set_scalingY(double value) { return SetDatatypeProperty ("scalingY", &value, 1); }
        ///<summary>Gets a value of scalingY, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_scalingY() { return GetDatatypeProperty<double>("scalingY", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class AlignedSegment
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AlignedSegment : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AlignedSegment and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AlignedSegment Create(int64_t model, const char* name=NULL) { return AlignedSegment(Instance::Create(model, "AlignedSegment", name, NULL), "AlignedSegment");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AlignedSegment CreateW(int64_t model, const wchar_t* name = NULL) { return AlignedSegment(Instance::Create(model, "AlignedSegment", NULL, name), "AlignedSegment"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        AlignedSegment(int64_t instance = NULL)
            : Curve(instance, "AlignedSegment")
        {}

    protected:
        AlignedSegment(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to AlignedSegment
       //

        ///<summary>Sets value of length</summary>
        bool set_length(double value) { return SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets a value of length, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets value of offsetX</summary>
        bool set_offsetX(double value) { return SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets a value of offsetX, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of offsetY</summary>
        bool set_offsetY(double value) { return SetDatatypeProperty ("offsetY", &value, 1); }
        ///<summary>Gets a value of offsetY, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetY() { return GetDatatypeProperty<double>("offsetY", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_segment(const Curve& instance) { return SetObjectProperty<Curve>("segment", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_segment() { return GetObjectProperty<Curve>("segment", NULL); }
        ///<summary>Sets values of tangentDirectionStart. OWL cardinality 3..3</summary>
        bool set_tangentDirectionStart(const double* values, int64_t count) { return SetDatatypeProperty ("tangentDirectionStart", values, count); }
        ///<summary>Sets values of tangentDirectionStart from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 3..3</summary>
        template <typename TList> bool set_tangentDirectionStart(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_tangentDirectionStart(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of tangentDirectionStart. OWL cardinality 3..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_tangentDirectionStart(int64_t* pCount) { return GetDatatypeProperty<double>("tangentDirectionStart", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class AlignedSegments
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AlignedSegments : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AlignedSegments and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AlignedSegments Create(int64_t model, const char* name=NULL) { return AlignedSegments(Instance::Create(model, "AlignedSegments", name, NULL), "AlignedSegments");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AlignedSegments CreateW(int64_t model, const wchar_t* name = NULL) { return AlignedSegments(Instance::Create(model, "AlignedSegments", NULL, name), "AlignedSegments"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        AlignedSegments(int64_t instance = NULL)
            : Curve(instance, "AlignedSegments")
        {}

    protected:
        AlignedSegments(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to AlignedSegments
       //

        ///<summary>Sets relationships from this instance to an array of AlignedSegment. OWL cardinality 0..-1</summary>
        bool set_segments(const AlignedSegment* instances, int64_t count) { return SetObjectProperty<AlignedSegment>("segments", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        bool set_segments(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("segments", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const AlignedSegment* get_segments(int64_t* pCount) { return GetObjectProperty<AlignedSegment>("segments", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segments_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("segments", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Alignment
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Alignment : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Alignment and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Alignment Create(int64_t model, const char* name=NULL) { return Alignment(Instance::Create(model, "Alignment", name, NULL), "Alignment");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Alignment CreateW(int64_t model, const wchar_t* name = NULL) { return Alignment(Instance::Create(model, "Alignment", NULL, name), "Alignment"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Alignment(int64_t instance = NULL)
            : Curve(instance, "Alignment")
        {}

    protected:
        Alignment(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Alignment
       //

        ///<summary>Sets relationship from this instance to an instance of AlignedSegments</summary>
        bool set_horizontal(const AlignedSegments& instance) { return SetObjectProperty<AlignedSegments>("horizontal", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const AlignedSegments* get_horizontal() { return GetObjectProperty<AlignedSegments>("horizontal", NULL); }
        ///<summary>Sets value of offsetX</summary>
        bool set_offsetX(double value) { return SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets a value of offsetX, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of type</summary>
        bool set_type(int64_t value) { return SetDatatypeProperty ("type", &value, 1); }
        ///<summary>Gets a value of type, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_type() { return GetDatatypeProperty<int64_t>("type", NULL); }
        ///<summary>Sets relationship from this instance to an instance of AlignedSegments</summary>
        bool set_vertical(const AlignedSegments& instance) { return SetObjectProperty<AlignedSegments>("vertical", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const AlignedSegments* get_vertical() { return GetObjectProperty<AlignedSegments>("vertical", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Environment
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Environment : public Instance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Environment and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Environment Create(int64_t model, const char* name=NULL) { return Environment(Instance::Create(model, "Environment", name, NULL), "Environment");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Environment CreateW(int64_t model, const wchar_t* name = NULL) { return Environment(Instance::Create(model, "Environment", NULL, name), "Environment"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Environment(int64_t instance = NULL)
            : Instance(instance, "Environment")
        {}

    protected:
        Environment(int64_t instance, const char* checkClassName)
            : Instance(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Light
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Light : public Environment
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Light and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Light Create(int64_t model, const char* name=NULL) { return Light(Instance::Create(model, "Light", name, NULL), "Light");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Light CreateW(int64_t model, const wchar_t* name = NULL) { return Light(Instance::Create(model, "Light", NULL, name), "Light"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Light(int64_t instance = NULL)
            : Environment(instance, "Light")
        {}

    protected:
        Light(int64_t instance, const char* checkClassName)
            : Environment(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Light
       //

        ///<summary>Sets relationship from this instance to an instance of Color</summary>
        bool set_color(const Color& instance) { return SetObjectProperty<Color>("color", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Color* get_color() { return GetObjectProperty<Color>("color", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class AmbientLight
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class AmbientLight : public Light
    {
    public:
        /// <summary>
        /// Create new instace of OWL class AmbientLight and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AmbientLight Create(int64_t model, const char* name=NULL) { return AmbientLight(Instance::Create(model, "AmbientLight", name, NULL), "AmbientLight");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static AmbientLight CreateW(int64_t model, const wchar_t* name = NULL) { return AmbientLight(Instance::Create(model, "AmbientLight", NULL, name), "AmbientLight"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        AmbientLight(int64_t instance = NULL)
            : Light(instance, "AmbientLight")
        {}

    protected:
        AmbientLight(int64_t instance, const char* checkClassName)
            : Light(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Appearance
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Appearance : public Instance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Appearance and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Appearance Create(int64_t model, const char* name=NULL) { return Appearance(Instance::Create(model, "Appearance", name, NULL), "Appearance");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Appearance CreateW(int64_t model, const wchar_t* name = NULL) { return Appearance(Instance::Create(model, "Appearance", NULL, name), "Appearance"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Appearance(int64_t instance = NULL)
            : Instance(instance, "Appearance")
        {}

    protected:
        Appearance(int64_t instance, const char* checkClassName)
            : Instance(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Arc3D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Arc3D : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Arc3D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Arc3D Create(int64_t model, const char* name=NULL) { return Arc3D(Instance::Create(model, "Arc3D", name, NULL), "Arc3D");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Arc3D CreateW(int64_t model, const wchar_t* name = NULL) { return Arc3D(Instance::Create(model, "Arc3D", NULL, name), "Arc3D"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Arc3D(int64_t instance = NULL)
            : Curve(instance, "Arc3D")
        {}

    protected:
        Arc3D(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Arc3D
       //

        ///<summary>Sets value of hasNormals</summary>
        bool set_hasNormals(bool value) { return SetDatatypeProperty ("hasNormals", &value, 1); }
        ///<summary>Gets a value of hasNormals, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_hasNormals() { return GetDatatypeProperty<bool>("hasNormals", NULL); }
        ///<summary>Sets value of radius</summary>
        bool set_radius(double value) { return SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets a value of radius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
        ///<summary>Sets value of size</summary>
        bool set_size(double value) { return SetDatatypeProperty ("size", &value, 1); }
        ///<summary>Gets a value of size, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_size() { return GetDatatypeProperty<double>("size", NULL); }
        ///<summary>Sets value of start</summary>
        bool set_start(double value) { return SetDatatypeProperty ("start", &value, 1); }
        ///<summary>Gets a value of start, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_start() { return GetDatatypeProperty<double>("start", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SplineCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SplineCurve : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SplineCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SplineCurve Create(int64_t model, const char* name=NULL) { return SplineCurve(Instance::Create(model, "SplineCurve", name, NULL), "SplineCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SplineCurve CreateW(int64_t model, const wchar_t* name = NULL) { return SplineCurve(Instance::Create(model, "SplineCurve", NULL, name), "SplineCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SplineCurve(int64_t instance = NULL)
            : Curve(instance, "SplineCurve")
        {}

    protected:
        SplineCurve(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SplineCurve
       //

        ///<summary>Sets value of closed</summary>
        bool set_closed(bool value) { return SetDatatypeProperty ("closed", &value, 1); }
        ///<summary>Gets a value of closed, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_closed() { return GetDatatypeProperty<bool>("closed", NULL); }
        ///<summary>Sets relationships from this instance to an array of Point3D. OWL cardinality 2..-1</summary>
        bool set_controlPoints(const Point3D* instances, int64_t count) { return SetObjectProperty<Point3D>("controlPoints", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 2..-1</summary>
        bool set_controlPoints(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("controlPoints", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 2..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Point3D* get_controlPoints(int64_t* pCount) { return GetObjectProperty<Point3D>("controlPoints", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 2..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_controlPoints_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("controlPoints", pCount); }
        ///<summary>Sets value of count</summary>
        bool set_count(int64_t value) { return SetDatatypeProperty ("count", &value, 1); }
        ///<summary>Gets a value of count, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_count() { return GetDatatypeProperty<int64_t>("count", NULL); }
        ///<summary>Sets value of degree</summary>
        bool set_degree(int64_t value) { return SetDatatypeProperty ("degree", &value, 1); }
        ///<summary>Gets a value of degree, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_degree() { return GetDatatypeProperty<int64_t>("degree", NULL); }
        ///<summary>Sets value of segmentationLength</summary>
        bool set_segmentationLength(double value) { return SetDatatypeProperty ("segmentationLength", &value, 1); }
        ///<summary>Gets a value of segmentationLength, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_segmentationLength() { return GetDatatypeProperty<double>("segmentationLength", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class BezierCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BezierCurve : public SplineCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BezierCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BezierCurve Create(int64_t model, const char* name=NULL) { return BezierCurve(Instance::Create(model, "BezierCurve", name, NULL), "BezierCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BezierCurve CreateW(int64_t model, const wchar_t* name = NULL) { return BezierCurve(Instance::Create(model, "BezierCurve", NULL, name), "BezierCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        BezierCurve(int64_t instance = NULL)
            : SplineCurve(instance, "BezierCurve")
        {}

    protected:
        BezierCurve(int64_t instance, const char* checkClassName)
            : SplineCurve(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Surface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Surface : public Face
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Surface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Surface Create(int64_t model, const char* name=NULL) { return Surface(Instance::Create(model, "Surface", name, NULL), "Surface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Surface CreateW(int64_t model, const wchar_t* name = NULL) { return Surface(Instance::Create(model, "Surface", NULL, name), "Surface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Surface(int64_t instance = NULL)
            : Face(instance, "Surface")
        {}

    protected:
        Surface(int64_t instance, const char* checkClassName)
            : Face(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class FiniteSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class FiniteSurface : public Surface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class FiniteSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static FiniteSurface Create(int64_t model, const char* name=NULL) { return FiniteSurface(Instance::Create(model, "FiniteSurface", name, NULL), "FiniteSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static FiniteSurface CreateW(int64_t model, const wchar_t* name = NULL) { return FiniteSurface(Instance::Create(model, "FiniteSurface", NULL, name), "FiniteSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        FiniteSurface(int64_t instance = NULL)
            : Surface(instance, "FiniteSurface")
        {}

    protected:
        FiniteSurface(int64_t instance, const char* checkClassName)
            : Surface(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SplineSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SplineSurface : public FiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SplineSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SplineSurface Create(int64_t model, const char* name=NULL) { return SplineSurface(Instance::Create(model, "SplineSurface", name, NULL), "SplineSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SplineSurface CreateW(int64_t model, const wchar_t* name = NULL) { return SplineSurface(Instance::Create(model, "SplineSurface", NULL, name), "SplineSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SplineSurface(int64_t instance = NULL)
            : FiniteSurface(instance, "SplineSurface")
        {}

    protected:
        SplineSurface(int64_t instance, const char* checkClassName)
            : FiniteSurface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SplineSurface
       //

        ///<summary>Sets relationships from this instance to an array of Point3D. OWL cardinality 4..-1</summary>
        bool set_controlPoints(const Point3D* instances, int64_t count) { return SetObjectProperty<Point3D>("controlPoints", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 4..-1</summary>
        bool set_controlPoints(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("controlPoints", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 4..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Point3D* get_controlPoints(int64_t* pCount) { return GetObjectProperty<Point3D>("controlPoints", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 4..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_controlPoints_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("controlPoints", pCount); }
        ///<summary>Sets value of segmentationLength</summary>
        bool set_segmentationLength(double value) { return SetDatatypeProperty ("segmentationLength", &value, 1); }
        ///<summary>Gets a value of segmentationLength, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_segmentationLength() { return GetDatatypeProperty<double>("segmentationLength", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
        ///<summary>Sets value of uClosed</summary>
        bool set_uClosed(bool value) { return SetDatatypeProperty ("uClosed", &value, 1); }
        ///<summary>Gets a value of uClosed, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_uClosed() { return GetDatatypeProperty<bool>("uClosed", NULL); }
        ///<summary>Sets value of uCount</summary>
        bool set_uCount(int64_t value) { return SetDatatypeProperty ("uCount", &value, 1); }
        ///<summary>Gets a value of uCount, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_uCount() { return GetDatatypeProperty<int64_t>("uCount", NULL); }
        ///<summary>Sets value of uDegree</summary>
        bool set_uDegree(int64_t value) { return SetDatatypeProperty ("uDegree", &value, 1); }
        ///<summary>Gets a value of uDegree, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_uDegree() { return GetDatatypeProperty<int64_t>("uDegree", NULL); }
        ///<summary>Sets value of uSegmentationParts</summary>
        bool set_uSegmentationParts(int64_t value) { return SetDatatypeProperty ("uSegmentationParts", &value, 1); }
        ///<summary>Gets a value of uSegmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_uSegmentationParts() { return GetDatatypeProperty<int64_t>("uSegmentationParts", NULL); }
        ///<summary>Sets value of vClosed</summary>
        bool set_vClosed(bool value) { return SetDatatypeProperty ("vClosed", &value, 1); }
        ///<summary>Gets a value of vClosed, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_vClosed() { return GetDatatypeProperty<bool>("vClosed", NULL); }
        ///<summary>Sets value of vCount</summary>
        bool set_vCount(int64_t value) { return SetDatatypeProperty ("vCount", &value, 1); }
        ///<summary>Gets a value of vCount, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_vCount() { return GetDatatypeProperty<int64_t>("vCount", NULL); }
        ///<summary>Sets value of vDegree</summary>
        bool set_vDegree(int64_t value) { return SetDatatypeProperty ("vDegree", &value, 1); }
        ///<summary>Gets a value of vDegree, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_vDegree() { return GetDatatypeProperty<int64_t>("vDegree", NULL); }
        ///<summary>Sets value of vSegmentationParts</summary>
        bool set_vSegmentationParts(int64_t value) { return SetDatatypeProperty ("vSegmentationParts", &value, 1); }
        ///<summary>Gets a value of vSegmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_vSegmentationParts() { return GetDatatypeProperty<int64_t>("vSegmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class BezierSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BezierSurface : public SplineSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BezierSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BezierSurface Create(int64_t model, const char* name=NULL) { return BezierSurface(Instance::Create(model, "BezierSurface", name, NULL), "BezierSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BezierSurface CreateW(int64_t model, const wchar_t* name = NULL) { return BezierSurface(Instance::Create(model, "BezierSurface", NULL, name), "BezierSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        BezierSurface(int64_t instance = NULL)
            : SplineSurface(instance, "BezierSurface")
        {}

    protected:
        BezierSurface(int64_t instance, const char* checkClassName)
            : SplineSurface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to BezierSurface
       //

        ///<summary>Sets value of setting</summary>
        bool set_setting(int64_t value) { return SetDatatypeProperty ("setting", &value, 1); }
        ///<summary>Gets a value of setting, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_setting() { return GetDatatypeProperty<int64_t>("setting", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class TransitionalCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class TransitionalCurve : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class TransitionalCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static TransitionalCurve Create(int64_t model, const char* name=NULL) { return TransitionalCurve(Instance::Create(model, "TransitionalCurve", name, NULL), "TransitionalCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static TransitionalCurve CreateW(int64_t model, const wchar_t* name = NULL) { return TransitionalCurve(Instance::Create(model, "TransitionalCurve", NULL, name), "TransitionalCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        TransitionalCurve(int64_t instance = NULL)
            : Curve(instance, "TransitionalCurve")
        {}

    protected:
        TransitionalCurve(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to TransitionalCurve
       //

        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        bool set_direction(const Vector& instance) { return SetObjectProperty<Vector>("direction", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Vector* get_direction() { return GetObjectProperty<Vector>("direction", NULL); }
        ///<summary>Sets value of length</summary>
        bool set_length(double value) { return SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets a value of length, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_path(const Curve& instance) { return SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets value of radiusI</summary>
        bool set_radiusI(double value) { return SetDatatypeProperty ("radiusI", &value, 1); }
        ///<summary>Gets a value of radiusI, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radiusI() { return GetDatatypeProperty<double>("radiusI", NULL); }
        ///<summary>Sets value of radiusII</summary>
        bool set_radiusII(double value) { return SetDatatypeProperty ("radiusII", &value, 1); }
        ///<summary>Gets a value of radiusII, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radiusII() { return GetDatatypeProperty<double>("radiusII", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class BiQuadraticParabola
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BiQuadraticParabola : public TransitionalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BiQuadraticParabola and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BiQuadraticParabola Create(int64_t model, const char* name=NULL) { return BiQuadraticParabola(Instance::Create(model, "BiQuadraticParabola", name, NULL), "BiQuadraticParabola");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BiQuadraticParabola CreateW(int64_t model, const wchar_t* name = NULL) { return BiQuadraticParabola(Instance::Create(model, "BiQuadraticParabola", NULL, name), "BiQuadraticParabola"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        BiQuadraticParabola(int64_t instance = NULL)
            : TransitionalCurve(instance, "BiQuadraticParabola")
        {}

    protected:
        BiQuadraticParabola(int64_t instance, const char* checkClassName)
            : TransitionalCurve(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Solid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Solid : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Solid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Solid Create(int64_t model, const char* name=NULL) { return Solid(Instance::Create(model, "Solid", name, NULL), "Solid");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Solid CreateW(int64_t model, const wchar_t* name = NULL) { return Solid(Instance::Create(model, "Solid", NULL, name), "Solid"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Solid(int64_t instance = NULL)
            : GeometricItem(instance, "Solid")
        {}

    protected:
        Solid(int64_t instance, const char* checkClassName)
            : GeometricItem(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Blend
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Blend : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Blend and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Blend Create(int64_t model, const char* name=NULL) { return Blend(Instance::Create(model, "Blend", name, NULL), "Blend");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Blend CreateW(int64_t model, const wchar_t* name = NULL) { return Blend(Instance::Create(model, "Blend", NULL, name), "Blend"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Blend(int64_t instance = NULL)
            : Solid(instance, "Blend")
        {}

    protected:
        Blend(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Blend
       //

        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_bottomPolygon(const Curve& instance) { return SetObjectProperty<Curve>("bottomPolygon", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_bottomPolygon() { return GetObjectProperty<Curve>("bottomPolygon", NULL); }
        ///<summary>Sets value of forceSolid</summary>
        bool set_forceSolid(bool value) { return SetDatatypeProperty ("forceSolid", &value, 1); }
        ///<summary>Gets a value of forceSolid, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_forceSolid() { return GetDatatypeProperty<bool>("forceSolid", NULL); }
        ///<summary>Sets value of fraction</summary>
        bool set_fraction(double value) { return SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets a value of fraction, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets value of hasBottom</summary>
        bool set_hasBottom(bool value) { return SetDatatypeProperty ("hasBottom", &value, 1); }
        ///<summary>Gets a value of hasBottom, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_hasBottom() { return GetDatatypeProperty<bool>("hasBottom", NULL); }
        ///<summary>Sets value of hasTop</summary>
        bool set_hasTop(bool value) { return SetDatatypeProperty ("hasTop", &value, 1); }
        ///<summary>Gets a value of hasTop, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_hasTop() { return GetDatatypeProperty<bool>("hasTop", NULL); }
        ///<summary>Sets value of height</summary>
        bool set_height(double value) { return SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets a value of height, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_topPolygon(const Curve& instance) { return SetObjectProperty<Curve>("topPolygon", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_topPolygon() { return GetObjectProperty<Curve>("topPolygon", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class BlossCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BlossCurve : public TransitionalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BlossCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BlossCurve Create(int64_t model, const char* name=NULL) { return BlossCurve(Instance::Create(model, "BlossCurve", name, NULL), "BlossCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BlossCurve CreateW(int64_t model, const wchar_t* name = NULL) { return BlossCurve(Instance::Create(model, "BlossCurve", NULL, name), "BlossCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        BlossCurve(int64_t instance = NULL)
            : TransitionalCurve(instance, "BlossCurve")
        {}

    protected:
        BlossCurve(int64_t instance, const char* checkClassName)
            : TransitionalCurve(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class BooleanOperation
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BooleanOperation : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BooleanOperation and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BooleanOperation Create(int64_t model, const char* name=NULL) { return BooleanOperation(Instance::Create(model, "BooleanOperation", name, NULL), "BooleanOperation");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BooleanOperation CreateW(int64_t model, const wchar_t* name = NULL) { return BooleanOperation(Instance::Create(model, "BooleanOperation", NULL, name), "BooleanOperation"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        BooleanOperation(int64_t instance = NULL)
            : Solid(instance, "BooleanOperation")
        {}

    protected:
        BooleanOperation(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to BooleanOperation
       //

        ///<summary>Sets relationship from this instance to an instance of Deviation</summary>
        bool set_deviation(const Deviation& instance) { return SetObjectProperty<Deviation>("deviation", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Deviation* get_deviation() { return GetObjectProperty<Deviation>("deviation", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        bool set_firstObject(const GeometricItem& instance) { return SetObjectProperty<GeometricItem>("firstObject", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const GeometricItem* get_firstObject() { return GetObjectProperty<GeometricItem>("firstObject", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        bool set_secondObject(const GeometricItem& instance) { return SetObjectProperty<GeometricItem>("secondObject", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const GeometricItem* get_secondObject() { return GetObjectProperty<GeometricItem>("secondObject", NULL); }
        ///<summary>Sets value of setting</summary>
        bool set_setting(int64_t value) { return SetDatatypeProperty ("setting", &value, 1); }
        ///<summary>Gets a value of setting, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_setting() { return GetDatatypeProperty<int64_t>("setting", NULL); }
        ///<summary>Sets value of type</summary>
        bool set_type(int64_t value) { return SetDatatypeProperty ("type", &value, 1); }
        ///<summary>Gets a value of type, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_type() { return GetDatatypeProperty<int64_t>("type", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class BooleanOperation2D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BooleanOperation2D : public Face
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BooleanOperation2D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BooleanOperation2D Create(int64_t model, const char* name=NULL) { return BooleanOperation2D(Instance::Create(model, "BooleanOperation2D", name, NULL), "BooleanOperation2D");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BooleanOperation2D CreateW(int64_t model, const wchar_t* name = NULL) { return BooleanOperation2D(Instance::Create(model, "BooleanOperation2D", NULL, name), "BooleanOperation2D"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        BooleanOperation2D(int64_t instance = NULL)
            : Face(instance, "BooleanOperation2D")
        {}

    protected:
        BooleanOperation2D(int64_t instance, const char* checkClassName)
            : Face(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to BooleanOperation2D
       //

        ///<summary>Sets relationship from this instance to an instance of Deviation</summary>
        bool set_deviation(const Deviation& instance) { return SetObjectProperty<Deviation>("deviation", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Deviation* get_deviation() { return GetObjectProperty<Deviation>("deviation", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        bool set_firstObject(const GeometricItem& instance) { return SetObjectProperty<GeometricItem>("firstObject", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const GeometricItem* get_firstObject() { return GetObjectProperty<GeometricItem>("firstObject", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        bool set_secondObject(const GeometricItem& instance) { return SetObjectProperty<GeometricItem>("secondObject", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const GeometricItem* get_secondObject() { return GetObjectProperty<GeometricItem>("secondObject", NULL); }
        ///<summary>Sets value of type</summary>
        bool set_type(int64_t value) { return SetDatatypeProperty ("type", &value, 1); }
        ///<summary>Gets a value of type, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_type() { return GetDatatypeProperty<int64_t>("type", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class BoundaryRepresentation
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BoundaryRepresentation : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BoundaryRepresentation and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BoundaryRepresentation Create(int64_t model, const char* name=NULL) { return BoundaryRepresentation(Instance::Create(model, "BoundaryRepresentation", name, NULL), "BoundaryRepresentation");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BoundaryRepresentation CreateW(int64_t model, const wchar_t* name = NULL) { return BoundaryRepresentation(Instance::Create(model, "BoundaryRepresentation", NULL, name), "BoundaryRepresentation"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        BoundaryRepresentation(int64_t instance = NULL)
            : Solid(instance, "BoundaryRepresentation")
        {}

    protected:
        BoundaryRepresentation(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to BoundaryRepresentation
       //

        ///<summary>Sets value of consistencyCheck</summary>
        bool set_consistencyCheck(int64_t value) { return SetDatatypeProperty ("consistencyCheck", &value, 1); }
        ///<summary>Gets a value of consistencyCheck, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_consistencyCheck() { return GetDatatypeProperty<int64_t>("consistencyCheck", NULL); }
        ///<summary>Sets value of epsilon</summary>
        bool set_epsilon(double value) { return SetDatatypeProperty ("epsilon", &value, 1); }
        ///<summary>Gets a value of epsilon, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_epsilon() { return GetDatatypeProperty<double>("epsilon", NULL); }
        ///<summary>Sets relationships from this instance to an array of Face. OWL cardinality 0..-1</summary>
        bool set_faces(const Face* instances, int64_t count) { return SetObjectProperty<Face>("faces", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        bool set_faces(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("faces", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Face* get_faces(int64_t* pCount) { return GetObjectProperty<Face>("faces", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_faces_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("faces", pCount); }
        ///<summary>Sets values of flags. OWL cardinality 0..-1</summary>
        bool set_flags(const int64_t* values, int64_t count) { return SetDatatypeProperty ("flags", values, count); }
        ///<summary>Sets values of flags from enumerable collection (std::vector, std::list, etc.) of elements convertible to int64_t. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_flags(const TList& values) { int64_t* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_flags(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of flags. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_flags(int64_t* pCount) { return GetDatatypeProperty<int64_t>("flags", pCount); }
        ///<summary>Sets value of fraction</summary>
        bool set_fraction(double value) { return SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets a value of fraction, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets values of indices. OWL cardinality 0..-1</summary>
        bool set_indices(const int64_t* values, int64_t count) { return SetDatatypeProperty ("indices", values, count); }
        ///<summary>Sets values of indices from enumerable collection (std::vector, std::list, etc.) of elements convertible to int64_t. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_indices(const TList& values) { int64_t* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_indices(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of indices. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_indices(int64_t* pCount) { return GetDatatypeProperty<int64_t>("indices", pCount); }
        ///<summary>Sets values of normalCoordinates. OWL cardinality 0..-1</summary>
        bool set_normalCoordinates(const double* values, int64_t count) { return SetDatatypeProperty ("normalCoordinates", values, count); }
        ///<summary>Sets values of normalCoordinates from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_normalCoordinates(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_normalCoordinates(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of normalCoordinates. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_normalCoordinates(int64_t* pCount) { return GetDatatypeProperty<double>("normalCoordinates", pCount); }
        ///<summary>Sets values of normalIndices. OWL cardinality 0..-1</summary>
        bool set_normalIndices(const int64_t* values, int64_t count) { return SetDatatypeProperty ("normalIndices", values, count); }
        ///<summary>Sets values of normalIndices from enumerable collection (std::vector, std::list, etc.) of elements convertible to int64_t. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_normalIndices(const TList& values) { int64_t* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_normalIndices(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of normalIndices. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_normalIndices(int64_t* pCount) { return GetDatatypeProperty<int64_t>("normalIndices", pCount); }
        ///<summary>Sets value of relativeEpsilon</summary>
        bool set_relativeEpsilon(double value) { return SetDatatypeProperty ("relativeEpsilon", &value, 1); }
        ///<summary>Gets a value of relativeEpsilon, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_relativeEpsilon() { return GetDatatypeProperty<double>("relativeEpsilon", NULL); }
        ///<summary>Sets values of textureCoordinates. OWL cardinality 0..-1</summary>
        bool set_textureCoordinates(const double* values, int64_t count) { return SetDatatypeProperty ("textureCoordinates", values, count); }
        ///<summary>Sets values of textureCoordinates from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_textureCoordinates(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_textureCoordinates(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of textureCoordinates. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_textureCoordinates(int64_t* pCount) { return GetDatatypeProperty<double>("textureCoordinates", pCount); }
        ///<summary>Sets values of textureIndices. OWL cardinality 0..-1</summary>
        bool set_textureIndices(const int64_t* values, int64_t count) { return SetDatatypeProperty ("textureIndices", values, count); }
        ///<summary>Sets values of textureIndices from enumerable collection (std::vector, std::list, etc.) of elements convertible to int64_t. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_textureIndices(const TList& values) { int64_t* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_textureIndices(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of textureIndices. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_textureIndices(int64_t* pCount) { return GetDatatypeProperty<int64_t>("textureIndices", pCount); }
        ///<summary>Sets value of vertexEpsilon</summary>
        bool set_vertexEpsilon(double value) { return SetDatatypeProperty ("vertexEpsilon", &value, 1); }
        ///<summary>Gets a value of vertexEpsilon, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_vertexEpsilon() { return GetDatatypeProperty<double>("vertexEpsilon", NULL); }
        ///<summary>Sets values of vertices. OWL cardinality 0..-1</summary>
        bool set_vertices(const double* values, int64_t count) { return SetDatatypeProperty ("vertices", values, count); }
        ///<summary>Sets values of vertices from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_vertices(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_vertices(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of vertices. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_vertices(int64_t* pCount) { return GetDatatypeProperty<double>("vertices", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Box
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Box : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Box and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Box Create(int64_t model, const char* name=NULL) { return Box(Instance::Create(model, "Box", name, NULL), "Box");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Box CreateW(int64_t model, const wchar_t* name = NULL) { return Box(Instance::Create(model, "Box", NULL, name), "Box"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Box(int64_t instance = NULL)
            : Solid(instance, "Box")
        {}

    protected:
        Box(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Box
       //

        ///<summary>Sets value of height</summary>
        bool set_height(double value) { return SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets a value of height, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of length</summary>
        bool set_length(double value) { return SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets a value of length, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets value of width</summary>
        bool set_width(double value) { return SetDatatypeProperty ("width", &value, 1); }
        ///<summary>Gets a value of width, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_width() { return GetDatatypeProperty<double>("width", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class BSplineCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BSplineCurve : public SplineCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BSplineCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BSplineCurve Create(int64_t model, const char* name=NULL) { return BSplineCurve(Instance::Create(model, "BSplineCurve", name, NULL), "BSplineCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BSplineCurve CreateW(int64_t model, const wchar_t* name = NULL) { return BSplineCurve(Instance::Create(model, "BSplineCurve", NULL, name), "BSplineCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        BSplineCurve(int64_t instance = NULL)
            : SplineCurve(instance, "BSplineCurve")
        {}

    protected:
        BSplineCurve(int64_t instance, const char* checkClassName)
            : SplineCurve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to BSplineCurve
       //

        ///<summary>Sets values of knotMultiplicities. OWL cardinality 0..-1</summary>
        bool set_knotMultiplicities(const int64_t* values, int64_t count) { return SetDatatypeProperty ("knotMultiplicities", values, count); }
        ///<summary>Sets values of knotMultiplicities from enumerable collection (std::vector, std::list, etc.) of elements convertible to int64_t. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_knotMultiplicities(const TList& values) { int64_t* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_knotMultiplicities(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of knotMultiplicities. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_knotMultiplicities(int64_t* pCount) { return GetDatatypeProperty<int64_t>("knotMultiplicities", pCount); }
        ///<summary>Sets values of knots. OWL cardinality 2..-1</summary>
        bool set_knots(const double* values, int64_t count) { return SetDatatypeProperty ("knots", values, count); }
        ///<summary>Sets values of knots from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 2..-1</summary>
        template <typename TList> bool set_knots(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_knots(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of knots. OWL cardinality 2..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_knots(int64_t* pCount) { return GetDatatypeProperty<double>("knots", pCount); }
        ///<summary>Sets value of setting</summary>
        bool set_setting(int64_t value) { return SetDatatypeProperty ("setting", &value, 1); }
        ///<summary>Gets a value of setting, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_setting() { return GetDatatypeProperty<int64_t>("setting", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class BSplineSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class BSplineSurface : public SplineSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class BSplineSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BSplineSurface Create(int64_t model, const char* name=NULL) { return BSplineSurface(Instance::Create(model, "BSplineSurface", name, NULL), "BSplineSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static BSplineSurface CreateW(int64_t model, const wchar_t* name = NULL) { return BSplineSurface(Instance::Create(model, "BSplineSurface", NULL, name), "BSplineSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        BSplineSurface(int64_t instance = NULL)
            : SplineSurface(instance, "BSplineSurface")
        {}

    protected:
        BSplineSurface(int64_t instance, const char* checkClassName)
            : SplineSurface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to BSplineSurface
       //

        ///<summary>Sets value of setting</summary>
        bool set_setting(int64_t value) { return SetDatatypeProperty ("setting", &value, 1); }
        ///<summary>Gets a value of setting, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_setting() { return GetDatatypeProperty<int64_t>("setting", NULL); }
        ///<summary>Sets values of uKnotMultiplicities. OWL cardinality 0..-1</summary>
        bool set_uKnotMultiplicities(const int64_t* values, int64_t count) { return SetDatatypeProperty ("uKnotMultiplicities", values, count); }
        ///<summary>Sets values of uKnotMultiplicities from enumerable collection (std::vector, std::list, etc.) of elements convertible to int64_t. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_uKnotMultiplicities(const TList& values) { int64_t* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_uKnotMultiplicities(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of uKnotMultiplicities. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_uKnotMultiplicities(int64_t* pCount) { return GetDatatypeProperty<int64_t>("uKnotMultiplicities", pCount); }
        ///<summary>Sets values of uKnots. OWL cardinality 2..-1</summary>
        bool set_uKnots(const double* values, int64_t count) { return SetDatatypeProperty ("uKnots", values, count); }
        ///<summary>Sets values of uKnots from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 2..-1</summary>
        template <typename TList> bool set_uKnots(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_uKnots(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of uKnots. OWL cardinality 2..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_uKnots(int64_t* pCount) { return GetDatatypeProperty<double>("uKnots", pCount); }
        ///<summary>Sets values of vKnotMultiplicities. OWL cardinality 0..-1</summary>
        bool set_vKnotMultiplicities(const int64_t* values, int64_t count) { return SetDatatypeProperty ("vKnotMultiplicities", values, count); }
        ///<summary>Sets values of vKnotMultiplicities from enumerable collection (std::vector, std::list, etc.) of elements convertible to int64_t. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_vKnotMultiplicities(const TList& values) { int64_t* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_vKnotMultiplicities(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of vKnotMultiplicities. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_vKnotMultiplicities(int64_t* pCount) { return GetDatatypeProperty<int64_t>("vKnotMultiplicities", pCount); }
        ///<summary>Sets values of vKnots. OWL cardinality 2..-1</summary>
        bool set_vKnots(const double* values, int64_t count) { return SetDatatypeProperty ("vKnots", values, count); }
        ///<summary>Sets values of vKnots from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 2..-1</summary>
        template <typename TList> bool set_vKnots(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_vKnots(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of vKnots. OWL cardinality 2..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_vKnots(int64_t* pCount) { return GetDatatypeProperty<double>("vKnots", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class ConicalCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ConicalCurve : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ConicalCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ConicalCurve Create(int64_t model, const char* name=NULL) { return ConicalCurve(Instance::Create(model, "ConicalCurve", name, NULL), "ConicalCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ConicalCurve CreateW(int64_t model, const wchar_t* name = NULL) { return ConicalCurve(Instance::Create(model, "ConicalCurve", NULL, name), "ConicalCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        ConicalCurve(int64_t instance = NULL)
            : Curve(instance, "ConicalCurve")
        {}

    protected:
        ConicalCurve(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to ConicalCurve
       //

        ///<summary>Sets value of a</summary>
        bool set_a(double value) { return SetDatatypeProperty ("a", &value, 1); }
        ///<summary>Gets a value of a, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_a() { return GetDatatypeProperty<double>("a", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
        ///<summary>Sets value of size</summary>
        bool set_size(double value) { return SetDatatypeProperty ("size", &value, 1); }
        ///<summary>Gets a value of size, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_size() { return GetDatatypeProperty<double>("size", NULL); }
        ///<summary>Sets value of start</summary>
        bool set_start(double value) { return SetDatatypeProperty ("start", &value, 1); }
        ///<summary>Gets a value of start, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_start() { return GetDatatypeProperty<double>("start", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Circle
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Circle : public ConicalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Circle and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Circle Create(int64_t model, const char* name=NULL) { return Circle(Instance::Create(model, "Circle", name, NULL), "Circle");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Circle CreateW(int64_t model, const wchar_t* name = NULL) { return Circle(Instance::Create(model, "Circle", NULL, name), "Circle"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Circle(int64_t instance = NULL)
            : ConicalCurve(instance, "Circle")
        {}

    protected:
        Circle(int64_t instance, const char* checkClassName)
            : ConicalCurve(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class CircleByPoints
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class CircleByPoints : public Circle
    {
    public:
        /// <summary>
        /// Create new instace of OWL class CircleByPoints and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CircleByPoints Create(int64_t model, const char* name=NULL) { return CircleByPoints(Instance::Create(model, "CircleByPoints", name, NULL), "CircleByPoints");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CircleByPoints CreateW(int64_t model, const wchar_t* name = NULL) { return CircleByPoints(Instance::Create(model, "CircleByPoints", NULL, name), "CircleByPoints"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        CircleByPoints(int64_t instance = NULL)
            : Circle(instance, "CircleByPoints")
        {}

    protected:
        CircleByPoints(int64_t instance, const char* checkClassName)
            : Circle(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to CircleByPoints
       //

        ///<summary>Sets relationships from this instance to an array of Point3D. OWL cardinality 3..3</summary>
        bool set_pointReferences(const Point3D* instances, int64_t count) { return SetObjectProperty<Point3D>("pointReferences", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 3..3</summary>
        bool set_pointReferences(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("pointReferences", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 3..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Point3D* get_pointReferences(int64_t* pCount) { return GetObjectProperty<Point3D>("pointReferences", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 3..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_pointReferences_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("pointReferences", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class ClippedPyramid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ClippedPyramid : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ClippedPyramid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ClippedPyramid Create(int64_t model, const char* name=NULL) { return ClippedPyramid(Instance::Create(model, "ClippedPyramid", name, NULL), "ClippedPyramid");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ClippedPyramid CreateW(int64_t model, const wchar_t* name = NULL) { return ClippedPyramid(Instance::Create(model, "ClippedPyramid", NULL, name), "ClippedPyramid"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        ClippedPyramid(int64_t instance = NULL)
            : Solid(instance, "ClippedPyramid")
        {}

    protected:
        ClippedPyramid(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to ClippedPyramid
       //

        ///<summary>Sets values of coordinates. OWL cardinality 12..12</summary>
        bool set_coordinates(const double* values, int64_t count) { return SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Sets values of coordinates from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 12..12</summary>
        template <typename TList> bool set_coordinates(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_coordinates(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of coordinates. OWL cardinality 12..12. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets values of points. OWL cardinality 0..12</summary>
        bool set_points(const double* values, int64_t count) { return SetDatatypeProperty ("points", values, count); }
        ///<summary>Sets values of points from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..12</summary>
        template <typename TList> bool set_points(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_points(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of points. OWL cardinality 0..12. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Clipping
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Clipping : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Clipping and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Clipping Create(int64_t model, const char* name=NULL) { return Clipping(Instance::Create(model, "Clipping", name, NULL), "Clipping");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Clipping CreateW(int64_t model, const wchar_t* name = NULL) { return Clipping(Instance::Create(model, "Clipping", NULL, name), "Clipping"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Clipping(int64_t instance = NULL)
            : Solid(instance, "Clipping")
        {}

    protected:
        Clipping(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Clipping
       //

        ///<summary>Sets relationship from this instance to an instance of Deviation</summary>
        bool set_deviation(const Deviation& instance) { return SetObjectProperty<Deviation>("deviation", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Deviation* get_deviation() { return GetObjectProperty<Deviation>("deviation", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        bool set_object(const GeometricItem& instance) { return SetObjectProperty<GeometricItem>("object", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const GeometricItem* get_object() { return GetObjectProperty<GeometricItem>("object", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Plane</summary>
        bool set_plane(const Plane& instance) { return SetObjectProperty<Plane>("plane", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Plane* get_plane() { return GetObjectProperty<Plane>("plane", NULL); }
        ///<summary>Sets value of type</summary>
        bool set_type(int64_t value) { return SetDatatypeProperty ("type", &value, 1); }
        ///<summary>Gets a value of type, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_type() { return GetDatatypeProperty<int64_t>("type", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Clothoid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Clothoid : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Clothoid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Clothoid Create(int64_t model, const char* name=NULL) { return Clothoid(Instance::Create(model, "Clothoid", name, NULL), "Clothoid");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Clothoid CreateW(int64_t model, const wchar_t* name = NULL) { return Clothoid(Instance::Create(model, "Clothoid", NULL, name), "Clothoid"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Clothoid(int64_t instance = NULL)
            : Curve(instance, "Clothoid")
        {}

    protected:
        Clothoid(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Clothoid
       //

        ///<summary>Sets value of A</summary>
        bool set_A(double value) { return SetDatatypeProperty ("A", &value, 1); }
        ///<summary>Gets a value of A, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_A() { return GetDatatypeProperty<double>("A", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        bool set_direction(const Vector& instance) { return SetObjectProperty<Vector>("direction", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Vector* get_direction() { return GetObjectProperty<Vector>("direction", NULL); }
        ///<summary>Sets value of length</summary>
        bool set_length(double value) { return SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets a value of length, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets value of orientation</summary>
        bool set_orientation(int64_t value) { return SetDatatypeProperty ("orientation", &value, 1); }
        ///<summary>Gets a value of orientation, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_orientation() { return GetDatatypeProperty<int64_t>("orientation", NULL); }
        ///<summary>Sets value of radius</summary>
        bool set_radius(double value) { return SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets a value of radius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class ClothoidCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ClothoidCurve : public TransitionalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ClothoidCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ClothoidCurve Create(int64_t model, const char* name=NULL) { return ClothoidCurve(Instance::Create(model, "ClothoidCurve", name, NULL), "ClothoidCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ClothoidCurve CreateW(int64_t model, const wchar_t* name = NULL) { return ClothoidCurve(Instance::Create(model, "ClothoidCurve", NULL, name), "ClothoidCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        ClothoidCurve(int64_t instance = NULL)
            : TransitionalCurve(instance, "ClothoidCurve")
        {}

    protected:
        ClothoidCurve(int64_t instance, const char* checkClassName)
            : TransitionalCurve(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Collection
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Collection : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Collection and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Collection Create(int64_t model, const char* name=NULL) { return Collection(Instance::Create(model, "Collection", name, NULL), "Collection");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Collection CreateW(int64_t model, const wchar_t* name = NULL) { return Collection(Instance::Create(model, "Collection", NULL, name), "Collection"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Collection(int64_t instance = NULL)
            : GeometricItem(instance, "Collection")
        {}

    protected:
        Collection(int64_t instance, const char* checkClassName)
            : GeometricItem(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Collection
       //

        ///<summary>Sets value of consistencyCheck</summary>
        bool set_consistencyCheck(int64_t value) { return SetDatatypeProperty ("consistencyCheck", &value, 1); }
        ///<summary>Gets a value of consistencyCheck, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_consistencyCheck() { return GetDatatypeProperty<int64_t>("consistencyCheck", NULL); }
        ///<summary>Sets value of epsilon</summary>
        bool set_epsilon(double value) { return SetDatatypeProperty ("epsilon", &value, 1); }
        ///<summary>Gets a value of epsilon, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_epsilon() { return GetDatatypeProperty<double>("epsilon", NULL); }
        ///<summary>Sets value of forceSolid</summary>
        bool set_forceSolid(bool value) { return SetDatatypeProperty ("forceSolid", &value, 1); }
        ///<summary>Gets a value of forceSolid, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_forceSolid() { return GetDatatypeProperty<bool>("forceSolid", NULL); }
        ///<summary>Sets value of fraction</summary>
        bool set_fraction(double value) { return SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets a value of fraction, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets relationships from this instance to an array of GeometricItem. OWL cardinality 0..-1</summary>
        bool set_objects(const GeometricItem* instances, int64_t count) { return SetObjectProperty<GeometricItem>("objects", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        bool set_objects(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("objects", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const GeometricItem* get_objects(int64_t* pCount) { return GetObjectProperty<GeometricItem>("objects", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_objects_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("objects", pCount); }
        ///<summary>Sets value of relativeEpsilon</summary>
        bool set_relativeEpsilon(double value) { return SetDatatypeProperty ("relativeEpsilon", &value, 1); }
        ///<summary>Gets a value of relativeEpsilon, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_relativeEpsilon() { return GetDatatypeProperty<double>("relativeEpsilon", NULL); }
        ///<summary>Sets value of representsSolid</summary>
        bool set_representsSolid(bool value) { return SetDatatypeProperty ("representsSolid", &value, 1); }
        ///<summary>Gets a value of representsSolid, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_representsSolid() { return GetDatatypeProperty<bool>("representsSolid", NULL); }
        ///<summary>Sets value of vertexEpsilon</summary>
        bool set_vertexEpsilon(double value) { return SetDatatypeProperty ("vertexEpsilon", &value, 1); }
        ///<summary>Gets a value of vertexEpsilon, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_vertexEpsilon() { return GetDatatypeProperty<double>("vertexEpsilon", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Color
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Color : public Appearance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Color and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Color Create(int64_t model, const char* name=NULL) { return Color(Instance::Create(model, "Color", name, NULL), "Color");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Color CreateW(int64_t model, const wchar_t* name = NULL) { return Color(Instance::Create(model, "Color", NULL, name), "Color"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Color(int64_t instance = NULL)
            : Appearance(instance, "Color")
        {}

    protected:
        Color(int64_t instance, const char* checkClassName)
            : Appearance(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Color
       //

        ///<summary>Sets relationship from this instance to an instance of ColorComponent</summary>
        bool set_ambient(const ColorComponent& instance) { return SetObjectProperty<ColorComponent>("ambient", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const ColorComponent* get_ambient() { return GetObjectProperty<ColorComponent>("ambient", NULL); }
        ///<summary>Sets value of ambientReflectance</summary>
        bool set_ambientReflectance(double value) { return SetDatatypeProperty ("ambientReflectance", &value, 1); }
        ///<summary>Gets a value of ambientReflectance, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_ambientReflectance() { return GetDatatypeProperty<double>("ambientReflectance", NULL); }
        ///<summary>Sets relationship from this instance to an instance of ColorComponent</summary>
        bool set_diffuse(const ColorComponent& instance) { return SetObjectProperty<ColorComponent>("diffuse", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const ColorComponent* get_diffuse() { return GetObjectProperty<ColorComponent>("diffuse", NULL); }
        ///<summary>Sets relationship from this instance to an instance of ColorComponent</summary>
        bool set_emissive(const ColorComponent& instance) { return SetObjectProperty<ColorComponent>("emissive", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const ColorComponent* get_emissive() { return GetObjectProperty<ColorComponent>("emissive", NULL); }
        ///<summary>Sets relationship from this instance to an instance of ColorComponent</summary>
        bool set_specular(const ColorComponent& instance) { return SetObjectProperty<ColorComponent>("specular", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const ColorComponent* get_specular() { return GetObjectProperty<ColorComponent>("specular", NULL); }
        ///<summary>Sets value of transparency</summary>
        bool set_transparency(double value) { return SetDatatypeProperty ("transparency", &value, 1); }
        ///<summary>Gets a value of transparency, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_transparency() { return GetDatatypeProperty<double>("transparency", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class ColorComponent
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ColorComponent : public Appearance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ColorComponent and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ColorComponent Create(int64_t model, const char* name=NULL) { return ColorComponent(Instance::Create(model, "ColorComponent", name, NULL), "ColorComponent");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ColorComponent CreateW(int64_t model, const wchar_t* name = NULL) { return ColorComponent(Instance::Create(model, "ColorComponent", NULL, name), "ColorComponent"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        ColorComponent(int64_t instance = NULL)
            : Appearance(instance, "ColorComponent")
        {}

    protected:
        ColorComponent(int64_t instance, const char* checkClassName)
            : Appearance(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to ColorComponent
       //

        ///<summary>Sets value of B</summary>
        bool set_B(double value) { return SetDatatypeProperty ("B", &value, 1); }
        ///<summary>Gets a value of B, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_B() { return GetDatatypeProperty<double>("B", NULL); }
        ///<summary>Sets value of G</summary>
        bool set_G(double value) { return SetDatatypeProperty ("G", &value, 1); }
        ///<summary>Gets a value of G, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_G() { return GetDatatypeProperty<double>("G", NULL); }
        ///<summary>Sets value of R</summary>
        bool set_R(double value) { return SetDatatypeProperty ("R", &value, 1); }
        ///<summary>Gets a value of R, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_R() { return GetDatatypeProperty<double>("R", NULL); }
        ///<summary>Sets value of W</summary>
        bool set_W(double value) { return SetDatatypeProperty ("W", &value, 1); }
        ///<summary>Gets a value of W, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_W() { return GetDatatypeProperty<double>("W", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Cone
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Cone : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Cone and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Cone Create(int64_t model, const char* name=NULL) { return Cone(Instance::Create(model, "Cone", name, NULL), "Cone");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Cone CreateW(int64_t model, const wchar_t* name = NULL) { return Cone(Instance::Create(model, "Cone", NULL, name), "Cone"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Cone(int64_t instance = NULL)
            : Solid(instance, "Cone")
        {}

    protected:
        Cone(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Cone
       //

        ///<summary>Sets value of height</summary>
        bool set_height(double value) { return SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets a value of height, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of radius</summary>
        bool set_radius(double value) { return SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets a value of radius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class InfiniteSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class InfiniteSurface : public Surface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class InfiniteSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static InfiniteSurface Create(int64_t model, const char* name=NULL) { return InfiniteSurface(Instance::Create(model, "InfiniteSurface", name, NULL), "InfiniteSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static InfiniteSurface CreateW(int64_t model, const wchar_t* name = NULL) { return InfiniteSurface(Instance::Create(model, "InfiniteSurface", NULL, name), "InfiniteSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        InfiniteSurface(int64_t instance = NULL)
            : Surface(instance, "InfiniteSurface")
        {}

    protected:
        InfiniteSurface(int64_t instance, const char* checkClassName)
            : Surface(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class ConicalSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ConicalSurface : public InfiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ConicalSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ConicalSurface Create(int64_t model, const char* name=NULL) { return ConicalSurface(Instance::Create(model, "ConicalSurface", name, NULL), "ConicalSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ConicalSurface CreateW(int64_t model, const wchar_t* name = NULL) { return ConicalSurface(Instance::Create(model, "ConicalSurface", NULL, name), "ConicalSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        ConicalSurface(int64_t instance = NULL)
            : InfiniteSurface(instance, "ConicalSurface")
        {}

    protected:
        ConicalSurface(int64_t instance, const char* checkClassName)
            : InfiniteSurface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to ConicalSurface
       //

        ///<summary>Sets value of height</summary>
        bool set_height(double value) { return SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets a value of height, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of radius</summary>
        bool set_radius(double value) { return SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets a value of radius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
        ///<summary>Sets value of semiVerticalAngle</summary>
        bool set_semiVerticalAngle(double value) { return SetDatatypeProperty ("semiVerticalAngle", &value, 1); }
        ///<summary>Gets a value of semiVerticalAngle, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_semiVerticalAngle() { return GetDatatypeProperty<double>("semiVerticalAngle", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Copy
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Copy : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Copy and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Copy Create(int64_t model, const char* name=NULL) { return Copy(Instance::Create(model, "Copy", name, NULL), "Copy");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Copy CreateW(int64_t model, const wchar_t* name = NULL) { return Copy(Instance::Create(model, "Copy", NULL, name), "Copy"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Copy(int64_t instance = NULL)
            : GeometricItem(instance, "Copy")
        {}

    protected:
        Copy(int64_t instance, const char* checkClassName)
            : GeometricItem(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Copy
       //

        ///<summary>Sets value of instanceReference</summary>
        bool set_instanceReference(int64_t value) { return SetDatatypeProperty ("instanceReference", &value, 1); }
        ///<summary>Gets a value of instanceReference, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_instanceReference() { return GetDatatypeProperty<int64_t>("instanceReference", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class CosineCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class CosineCurve : public TransitionalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class CosineCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CosineCurve Create(int64_t model, const char* name=NULL) { return CosineCurve(Instance::Create(model, "CosineCurve", name, NULL), "CosineCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CosineCurve CreateW(int64_t model, const wchar_t* name = NULL) { return CosineCurve(Instance::Create(model, "CosineCurve", NULL, name), "CosineCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        CosineCurve(int64_t instance = NULL)
            : TransitionalCurve(instance, "CosineCurve")
        {}

    protected:
        CosineCurve(int64_t instance, const char* checkClassName)
            : TransitionalCurve(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Cube
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Cube : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Cube and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Cube Create(int64_t model, const char* name=NULL) { return Cube(Instance::Create(model, "Cube", name, NULL), "Cube");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Cube CreateW(int64_t model, const wchar_t* name = NULL) { return Cube(Instance::Create(model, "Cube", NULL, name), "Cube"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Cube(int64_t instance = NULL)
            : Solid(instance, "Cube")
        {}

    protected:
        Cube(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Cube
       //

        ///<summary>Sets value of length</summary>
        bool set_length(double value) { return SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets a value of length, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class CubicParabola
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class CubicParabola : public TransitionalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class CubicParabola and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CubicParabola Create(int64_t model, const char* name=NULL) { return CubicParabola(Instance::Create(model, "CubicParabola", name, NULL), "CubicParabola");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CubicParabola CreateW(int64_t model, const wchar_t* name = NULL) { return CubicParabola(Instance::Create(model, "CubicParabola", NULL, name), "CubicParabola"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        CubicParabola(int64_t instance = NULL)
            : TransitionalCurve(instance, "CubicParabola")
        {}

    protected:
        CubicParabola(int64_t instance, const char* checkClassName)
            : TransitionalCurve(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Cuboid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Cuboid : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Cuboid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Cuboid Create(int64_t model, const char* name=NULL) { return Cuboid(Instance::Create(model, "Cuboid", name, NULL), "Cuboid");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Cuboid CreateW(int64_t model, const wchar_t* name = NULL) { return Cuboid(Instance::Create(model, "Cuboid", NULL, name), "Cuboid"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Cuboid(int64_t instance = NULL)
            : Solid(instance, "Cuboid")
        {}

    protected:
        Cuboid(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Cuboid
       //

        ///<summary>Sets value of height</summary>
        bool set_height(double value) { return SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets a value of height, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of length</summary>
        bool set_length(double value) { return SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets a value of length, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets value of width</summary>
        bool set_width(double value) { return SetDatatypeProperty ("width", &value, 1); }
        ///<summary>Gets a value of width, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_width() { return GetDatatypeProperty<double>("width", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class CurvesFromSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class CurvesFromSurface : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class CurvesFromSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CurvesFromSurface Create(int64_t model, const char* name=NULL) { return CurvesFromSurface(Instance::Create(model, "CurvesFromSurface", name, NULL), "CurvesFromSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CurvesFromSurface CreateW(int64_t model, const wchar_t* name = NULL) { return CurvesFromSurface(Instance::Create(model, "CurvesFromSurface", NULL, name), "CurvesFromSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        CurvesFromSurface(int64_t instance = NULL)
            : Curve(instance, "CurvesFromSurface")
        {}

    protected:
        CurvesFromSurface(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to CurvesFromSurface
       //

        ///<summary>Sets relationships from this instance to an array of Face. OWL cardinality 1..-1</summary>
        bool set_faces(const Face* instances, int64_t count) { return SetObjectProperty<Face>("faces", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        bool set_faces(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("faces", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Face* get_faces(int64_t* pCount) { return GetObjectProperty<Face>("faces", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_faces_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("faces", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Cylinder
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Cylinder : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Cylinder and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Cylinder Create(int64_t model, const char* name=NULL) { return Cylinder(Instance::Create(model, "Cylinder", name, NULL), "Cylinder");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Cylinder CreateW(int64_t model, const wchar_t* name = NULL) { return Cylinder(Instance::Create(model, "Cylinder", NULL, name), "Cylinder"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Cylinder(int64_t instance = NULL)
            : Solid(instance, "Cylinder")
        {}

    protected:
        Cylinder(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Cylinder
       //

        ///<summary>Sets value of length</summary>
        bool set_length(double value) { return SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets a value of length, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets value of radius</summary>
        bool set_radius(double value) { return SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets a value of radius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class CylindricalSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class CylindricalSurface : public InfiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class CylindricalSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CylindricalSurface Create(int64_t model, const char* name=NULL) { return CylindricalSurface(Instance::Create(model, "CylindricalSurface", name, NULL), "CylindricalSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static CylindricalSurface CreateW(int64_t model, const wchar_t* name = NULL) { return CylindricalSurface(Instance::Create(model, "CylindricalSurface", NULL, name), "CylindricalSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        CylindricalSurface(int64_t instance = NULL)
            : InfiniteSurface(instance, "CylindricalSurface")
        {}

    protected:
        CylindricalSurface(int64_t instance, const char* checkClassName)
            : InfiniteSurface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to CylindricalSurface
       //

        ///<summary>Sets value of radius</summary>
        bool set_radius(double value) { return SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets a value of radius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class DegenerateToroidalSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class DegenerateToroidalSurface : public FiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class DegenerateToroidalSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static DegenerateToroidalSurface Create(int64_t model, const char* name=NULL) { return DegenerateToroidalSurface(Instance::Create(model, "DegenerateToroidalSurface", name, NULL), "DegenerateToroidalSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static DegenerateToroidalSurface CreateW(int64_t model, const wchar_t* name = NULL) { return DegenerateToroidalSurface(Instance::Create(model, "DegenerateToroidalSurface", NULL, name), "DegenerateToroidalSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        DegenerateToroidalSurface(int64_t instance = NULL)
            : FiniteSurface(instance, "DegenerateToroidalSurface")
        {}

    protected:
        DegenerateToroidalSurface(int64_t instance, const char* checkClassName)
            : FiniteSurface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to DegenerateToroidalSurface
       //

        ///<summary>Sets value of majorRadius</summary>
        bool set_majorRadius(double value) { return SetDatatypeProperty ("majorRadius", &value, 1); }
        ///<summary>Gets a value of majorRadius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_majorRadius() { return GetDatatypeProperty<double>("majorRadius", NULL); }
        ///<summary>Sets value of minorRadius</summary>
        bool set_minorRadius(double value) { return SetDatatypeProperty ("minorRadius", &value, 1); }
        ///<summary>Gets a value of minorRadius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_minorRadius() { return GetDatatypeProperty<double>("minorRadius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Deviation
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Deviation : public Instance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Deviation and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Deviation Create(int64_t model, const char* name=NULL) { return Deviation(Instance::Create(model, "Deviation", name, NULL), "Deviation");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Deviation CreateW(int64_t model, const wchar_t* name = NULL) { return Deviation(Instance::Create(model, "Deviation", NULL, name), "Deviation"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Deviation(int64_t instance = NULL)
            : Instance(instance, "Deviation")
        {}

    protected:
        Deviation(int64_t instance, const char* checkClassName)
            : Instance(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Deviation
       //

        ///<summary>Sets value of absoluteEpsilon</summary>
        bool set_absoluteEpsilon(double value) { return SetDatatypeProperty ("absoluteEpsilon", &value, 1); }
        ///<summary>Gets a value of absoluteEpsilon, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_absoluteEpsilon() { return GetDatatypeProperty<double>("absoluteEpsilon", NULL); }
        ///<summary>Sets value of relativeEpsilon</summary>
        bool set_relativeEpsilon(double value) { return SetDatatypeProperty ("relativeEpsilon", &value, 1); }
        ///<summary>Gets a value of relativeEpsilon, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_relativeEpsilon() { return GetDatatypeProperty<double>("relativeEpsilon", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class DirectLight
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class DirectLight : public Light
    {
    public:
        /// <summary>
        /// Create new instace of OWL class DirectLight and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static DirectLight Create(int64_t model, const char* name=NULL) { return DirectLight(Instance::Create(model, "DirectLight", name, NULL), "DirectLight");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static DirectLight CreateW(int64_t model, const wchar_t* name = NULL) { return DirectLight(Instance::Create(model, "DirectLight", NULL, name), "DirectLight"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        DirectLight(int64_t instance = NULL)
            : Light(instance, "DirectLight")
        {}

    protected:
        DirectLight(int64_t instance, const char* checkClassName)
            : Light(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to DirectLight
       //

        ///<summary>Sets value of attenuation</summary>
        bool set_attenuation(double value) { return SetDatatypeProperty ("attenuation", &value, 1); }
        ///<summary>Gets a value of attenuation, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_attenuation() { return GetDatatypeProperty<double>("attenuation", NULL); }
        ///<summary>Sets value of range</summary>
        bool set_range(double value) { return SetDatatypeProperty ("range", &value, 1); }
        ///<summary>Gets a value of range, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_range() { return GetDatatypeProperty<double>("range", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class DirectionalLight
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class DirectionalLight : public DirectLight
    {
    public:
        /// <summary>
        /// Create new instace of OWL class DirectionalLight and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static DirectionalLight Create(int64_t model, const char* name=NULL) { return DirectionalLight(Instance::Create(model, "DirectionalLight", name, NULL), "DirectionalLight");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static DirectionalLight CreateW(int64_t model, const wchar_t* name = NULL) { return DirectionalLight(Instance::Create(model, "DirectionalLight", NULL, name), "DirectionalLight"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        DirectionalLight(int64_t instance = NULL)
            : DirectLight(instance, "DirectionalLight")
        {}

    protected:
        DirectionalLight(int64_t instance, const char* checkClassName)
            : DirectLight(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to DirectionalLight
       //

        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        bool set_direction(const Vector& instance) { return SetObjectProperty<Vector>("direction", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Vector* get_direction() { return GetObjectProperty<Vector>("direction", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Dummy
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Dummy : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Dummy and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Dummy Create(int64_t model, const char* name=NULL) { return Dummy(Instance::Create(model, "Dummy", name, NULL), "Dummy");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Dummy CreateW(int64_t model, const wchar_t* name = NULL) { return Dummy(Instance::Create(model, "Dummy", NULL, name), "Dummy"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Dummy(int64_t instance = NULL)
            : GeometricItem(instance, "Dummy")
        {}

    protected:
        Dummy(int64_t instance, const char* checkClassName)
            : GeometricItem(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Ellipse
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Ellipse : public ConicalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Ellipse and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Ellipse Create(int64_t model, const char* name=NULL) { return Ellipse(Instance::Create(model, "Ellipse", name, NULL), "Ellipse");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Ellipse CreateW(int64_t model, const wchar_t* name = NULL) { return Ellipse(Instance::Create(model, "Ellipse", NULL, name), "Ellipse"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Ellipse(int64_t instance = NULL)
            : ConicalCurve(instance, "Ellipse")
        {}

    protected:
        Ellipse(int64_t instance, const char* checkClassName)
            : ConicalCurve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Ellipse
       //

        ///<summary>Sets value of b</summary>
        bool set_b(double value) { return SetDatatypeProperty ("b", &value, 1); }
        ///<summary>Gets a value of b, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_b() { return GetDatatypeProperty<double>("b", NULL); }
        ///<summary>Sets value of radiusI</summary>
        bool set_radiusI(double value) { return SetDatatypeProperty ("radiusI", &value, 1); }
        ///<summary>Gets a value of radiusI, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radiusI() { return GetDatatypeProperty<double>("radiusI", NULL); }
        ///<summary>Sets value of radiusII</summary>
        bool set_radiusII(double value) { return SetDatatypeProperty ("radiusII", &value, 1); }
        ///<summary>Gets a value of radiusII, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radiusII() { return GetDatatypeProperty<double>("radiusII", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class EllipticCone
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class EllipticCone : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class EllipticCone and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static EllipticCone Create(int64_t model, const char* name=NULL) { return EllipticCone(Instance::Create(model, "EllipticCone", name, NULL), "EllipticCone");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static EllipticCone CreateW(int64_t model, const wchar_t* name = NULL) { return EllipticCone(Instance::Create(model, "EllipticCone", NULL, name), "EllipticCone"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        EllipticCone(int64_t instance = NULL)
            : Solid(instance, "EllipticCone")
        {}

    protected:
        EllipticCone(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to EllipticCone
       //

        ///<summary>Sets value of height</summary>
        bool set_height(double value) { return SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets a value of height, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of radiusI</summary>
        bool set_radiusI(double value) { return SetDatatypeProperty ("radiusI", &value, 1); }
        ///<summary>Gets a value of radiusI, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radiusI() { return GetDatatypeProperty<double>("radiusI", NULL); }
        ///<summary>Sets value of radiusII</summary>
        bool set_radiusII(double value) { return SetDatatypeProperty ("radiusII", &value, 1); }
        ///<summary>Gets a value of radiusII, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radiusII() { return GetDatatypeProperty<double>("radiusII", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class ExtrudedPolygon
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ExtrudedPolygon : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ExtrudedPolygon and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ExtrudedPolygon Create(int64_t model, const char* name=NULL) { return ExtrudedPolygon(Instance::Create(model, "ExtrudedPolygon", name, NULL), "ExtrudedPolygon");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ExtrudedPolygon CreateW(int64_t model, const wchar_t* name = NULL) { return ExtrudedPolygon(Instance::Create(model, "ExtrudedPolygon", NULL, name), "ExtrudedPolygon"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        ExtrudedPolygon(int64_t instance = NULL)
            : Solid(instance, "ExtrudedPolygon")
        {}

    protected:
        ExtrudedPolygon(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to ExtrudedPolygon
       //

        ///<summary>Sets value of extrusionLength</summary>
        bool set_extrusionLength(double value) { return SetDatatypeProperty ("extrusionLength", &value, 1); }
        ///<summary>Gets a value of extrusionLength, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_extrusionLength() { return GetDatatypeProperty<double>("extrusionLength", NULL); }
        ///<summary>Sets value of fraction</summary>
        bool set_fraction(double value) { return SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets a value of fraction, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets values of openingPoints. OWL cardinality 0..-1</summary>
        bool set_openingPoints(const double* values, int64_t count) { return SetDatatypeProperty ("openingPoints", values, count); }
        ///<summary>Sets values of openingPoints from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_openingPoints(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_openingPoints(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of openingPoints. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_openingPoints(int64_t* pCount) { return GetDatatypeProperty<double>("openingPoints", pCount); }
        ///<summary>Sets values of openingSizes. OWL cardinality 0..-1</summary>
        bool set_openingSizes(const int64_t* values, int64_t count) { return SetDatatypeProperty ("openingSizes", values, count); }
        ///<summary>Sets values of openingSizes from enumerable collection (std::vector, std::list, etc.) of elements convertible to int64_t. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_openingSizes(const TList& values) { int64_t* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_openingSizes(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of openingSizes. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_openingSizes(int64_t* pCount) { return GetDatatypeProperty<int64_t>("openingSizes", pCount); }
        ///<summary>Sets values of points. OWL cardinality 6..-1</summary>
        bool set_points(const double* values, int64_t count) { return SetDatatypeProperty ("points", values, count); }
        ///<summary>Sets values of points from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 6..-1</summary>
        template <typename TList> bool set_points(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_points(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of points. OWL cardinality 6..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
        ///<summary>Sets values of polygonDirection. OWL cardinality 0..3</summary>
        bool set_polygonDirection(const double* values, int64_t count) { return SetDatatypeProperty ("polygonDirection", values, count); }
        ///<summary>Sets values of polygonDirection from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..3</summary>
        template <typename TList> bool set_polygonDirection(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_polygonDirection(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of polygonDirection. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_polygonDirection(int64_t* pCount) { return GetDatatypeProperty<double>("polygonDirection", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class ExtrudedPolygonTapered
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ExtrudedPolygonTapered : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ExtrudedPolygonTapered and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ExtrudedPolygonTapered Create(int64_t model, const char* name=NULL) { return ExtrudedPolygonTapered(Instance::Create(model, "ExtrudedPolygonTapered", name, NULL), "ExtrudedPolygonTapered");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ExtrudedPolygonTapered CreateW(int64_t model, const wchar_t* name = NULL) { return ExtrudedPolygonTapered(Instance::Create(model, "ExtrudedPolygonTapered", NULL, name), "ExtrudedPolygonTapered"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        ExtrudedPolygonTapered(int64_t instance = NULL)
            : Solid(instance, "ExtrudedPolygonTapered")
        {}

    protected:
        ExtrudedPolygonTapered(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to ExtrudedPolygonTapered
       //

        ///<summary>Sets value of extrusionLength</summary>
        bool set_extrusionLength(double value) { return SetDatatypeProperty ("extrusionLength", &value, 1); }
        ///<summary>Gets a value of extrusionLength, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_extrusionLength() { return GetDatatypeProperty<double>("extrusionLength", NULL); }
        ///<summary>Sets value of fraction</summary>
        bool set_fraction(double value) { return SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets a value of fraction, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets values of openingPoints. OWL cardinality 0..-1</summary>
        bool set_openingPoints(const double* values, int64_t count) { return SetDatatypeProperty ("openingPoints", values, count); }
        ///<summary>Sets values of openingPoints from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_openingPoints(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_openingPoints(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of openingPoints. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_openingPoints(int64_t* pCount) { return GetDatatypeProperty<double>("openingPoints", pCount); }
        ///<summary>Sets values of openingPointsEnd. OWL cardinality 0..-1</summary>
        bool set_openingPointsEnd(const double* values, int64_t count) { return SetDatatypeProperty ("openingPointsEnd", values, count); }
        ///<summary>Sets values of openingPointsEnd from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_openingPointsEnd(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_openingPointsEnd(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of openingPointsEnd. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_openingPointsEnd(int64_t* pCount) { return GetDatatypeProperty<double>("openingPointsEnd", pCount); }
        ///<summary>Sets values of openingSizes. OWL cardinality 0..-1</summary>
        bool set_openingSizes(const int64_t* values, int64_t count) { return SetDatatypeProperty ("openingSizes", values, count); }
        ///<summary>Sets values of openingSizes from enumerable collection (std::vector, std::list, etc.) of elements convertible to int64_t. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_openingSizes(const TList& values) { int64_t* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_openingSizes(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of openingSizes. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_openingSizes(int64_t* pCount) { return GetDatatypeProperty<int64_t>("openingSizes", pCount); }
        ///<summary>Sets values of openingSizesEnd. OWL cardinality 0..-1</summary>
        bool set_openingSizesEnd(const int64_t* values, int64_t count) { return SetDatatypeProperty ("openingSizesEnd", values, count); }
        ///<summary>Sets values of openingSizesEnd from enumerable collection (std::vector, std::list, etc.) of elements convertible to int64_t. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_openingSizesEnd(const TList& values) { int64_t* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_openingSizesEnd(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of openingSizesEnd. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_openingSizesEnd(int64_t* pCount) { return GetDatatypeProperty<int64_t>("openingSizesEnd", pCount); }
        ///<summary>Sets values of points. OWL cardinality 6..-1</summary>
        bool set_points(const double* values, int64_t count) { return SetDatatypeProperty ("points", values, count); }
        ///<summary>Sets values of points from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 6..-1</summary>
        template <typename TList> bool set_points(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_points(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of points. OWL cardinality 6..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
        ///<summary>Sets values of pointsEnd. OWL cardinality 6..-1</summary>
        bool set_pointsEnd(const double* values, int64_t count) { return SetDatatypeProperty ("pointsEnd", values, count); }
        ///<summary>Sets values of pointsEnd from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 6..-1</summary>
        template <typename TList> bool set_pointsEnd(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_pointsEnd(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of pointsEnd. OWL cardinality 6..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_pointsEnd(int64_t* pCount) { return GetDatatypeProperty<double>("pointsEnd", pCount); }
        ///<summary>Sets values of polygonDirection. OWL cardinality 0..3</summary>
        bool set_polygonDirection(const double* values, int64_t count) { return SetDatatypeProperty ("polygonDirection", values, count); }
        ///<summary>Sets values of polygonDirection from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..3</summary>
        template <typename TList> bool set_polygonDirection(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_polygonDirection(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of polygonDirection. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_polygonDirection(int64_t* pCount) { return GetDatatypeProperty<double>("polygonDirection", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class ExtrusionAreaSolid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ExtrusionAreaSolid : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ExtrusionAreaSolid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ExtrusionAreaSolid Create(int64_t model, const char* name=NULL) { return ExtrusionAreaSolid(Instance::Create(model, "ExtrusionAreaSolid", name, NULL), "ExtrusionAreaSolid");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ExtrusionAreaSolid CreateW(int64_t model, const wchar_t* name = NULL) { return ExtrusionAreaSolid(Instance::Create(model, "ExtrusionAreaSolid", NULL, name), "ExtrusionAreaSolid"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        ExtrusionAreaSolid(int64_t instance = NULL)
            : Solid(instance, "ExtrusionAreaSolid")
        {}

    protected:
        ExtrusionAreaSolid(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to ExtrusionAreaSolid
       //

        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_extrusionArea(const Curve& instance) { return SetObjectProperty<Curve>("extrusionArea", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_extrusionArea() { return GetObjectProperty<Curve>("extrusionArea", NULL); }
        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        bool set_extrusionAreaOpenings(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("extrusionAreaOpenings", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        bool set_extrusionAreaOpenings(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("extrusionAreaOpenings", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_extrusionAreaOpenings(int64_t* pCount) { return GetObjectProperty<Curve>("extrusionAreaOpenings", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_extrusionAreaOpenings_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("extrusionAreaOpenings", pCount); }
        ///<summary>Sets values of extrusionDirection. OWL cardinality 0..3</summary>
        bool set_extrusionDirection(const double* values, int64_t count) { return SetDatatypeProperty ("extrusionDirection", values, count); }
        ///<summary>Sets values of extrusionDirection from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..3</summary>
        template <typename TList> bool set_extrusionDirection(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_extrusionDirection(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of extrusionDirection. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_extrusionDirection(int64_t* pCount) { return GetDatatypeProperty<double>("extrusionDirection", pCount); }
        ///<summary>Sets value of extrusionLength</summary>
        bool set_extrusionLength(double value) { return SetDatatypeProperty ("extrusionLength", &value, 1); }
        ///<summary>Gets a value of extrusionLength, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_extrusionLength() { return GetDatatypeProperty<double>("extrusionLength", NULL); }
        ///<summary>Sets value of fraction</summary>
        bool set_fraction(double value) { return SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets a value of fraction, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class ExtrusionAreaSolidSet
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ExtrusionAreaSolidSet : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ExtrusionAreaSolidSet and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ExtrusionAreaSolidSet Create(int64_t model, const char* name=NULL) { return ExtrusionAreaSolidSet(Instance::Create(model, "ExtrusionAreaSolidSet", name, NULL), "ExtrusionAreaSolidSet");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ExtrusionAreaSolidSet CreateW(int64_t model, const wchar_t* name = NULL) { return ExtrusionAreaSolidSet(Instance::Create(model, "ExtrusionAreaSolidSet", NULL, name), "ExtrusionAreaSolidSet"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        ExtrusionAreaSolidSet(int64_t instance = NULL)
            : Solid(instance, "ExtrusionAreaSolidSet")
        {}

    protected:
        ExtrusionAreaSolidSet(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to ExtrusionAreaSolidSet
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 1..-1</summary>
        bool set_extrusionAreaSet(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("extrusionAreaSet", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        bool set_extrusionAreaSet(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("extrusionAreaSet", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_extrusionAreaSet(int64_t* pCount) { return GetObjectProperty<Curve>("extrusionAreaSet", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_extrusionAreaSet_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("extrusionAreaSet", pCount); }
        ///<summary>Sets values of extrusionDirection. OWL cardinality 0..3</summary>
        bool set_extrusionDirection(const double* values, int64_t count) { return SetDatatypeProperty ("extrusionDirection", values, count); }
        ///<summary>Sets values of extrusionDirection from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..3</summary>
        template <typename TList> bool set_extrusionDirection(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_extrusionDirection(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of extrusionDirection. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_extrusionDirection(int64_t* pCount) { return GetDatatypeProperty<double>("extrusionDirection", pCount); }
        ///<summary>Sets value of extrusionLength</summary>
        bool set_extrusionLength(double value) { return SetDatatypeProperty ("extrusionLength", &value, 1); }
        ///<summary>Gets a value of extrusionLength, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_extrusionLength() { return GetDatatypeProperty<double>("extrusionLength", NULL); }
        ///<summary>Sets value of fraction</summary>
        bool set_fraction(double value) { return SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets a value of fraction, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Face2D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Face2D : public Face
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Face2D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Face2D Create(int64_t model, const char* name=NULL) { return Face2D(Instance::Create(model, "Face2D", name, NULL), "Face2D");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Face2D CreateW(int64_t model, const wchar_t* name = NULL) { return Face2D(Instance::Create(model, "Face2D", NULL, name), "Face2D"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Face2D(int64_t instance = NULL)
            : Face(instance, "Face2D")
        {}

    protected:
        Face2D(int64_t instance, const char* checkClassName)
            : Face(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Face2D
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        bool set_innerPolygons(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("innerPolygons", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        bool set_innerPolygons(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("innerPolygons", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_innerPolygons(int64_t* pCount) { return GetObjectProperty<Curve>("innerPolygons", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_innerPolygons_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("innerPolygons", pCount); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_outerPolygon(const Curve& instance) { return SetObjectProperty<Curve>("outerPolygon", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_outerPolygon() { return GetObjectProperty<Curve>("outerPolygon", NULL); }
        ///<summary>Sets value of setting</summary>
        bool set_setting(int64_t value) { return SetDatatypeProperty ("setting", &value, 1); }
        ///<summary>Gets a value of setting, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_setting() { return GetDatatypeProperty<int64_t>("setting", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Face2DSet
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Face2DSet : public Face
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Face2DSet and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Face2DSet Create(int64_t model, const char* name=NULL) { return Face2DSet(Instance::Create(model, "Face2DSet", name, NULL), "Face2DSet");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Face2DSet CreateW(int64_t model, const wchar_t* name = NULL) { return Face2DSet(Instance::Create(model, "Face2DSet", NULL, name), "Face2DSet"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Face2DSet(int64_t instance = NULL)
            : Face(instance, "Face2DSet")
        {}

    protected:
        Face2DSet(int64_t instance, const char* checkClassName)
            : Face(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Face2DSet
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        bool set_polygons(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("polygons", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        bool set_polygons(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("polygons", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_polygons(int64_t* pCount) { return GetObjectProperty<Curve>("polygons", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_polygons_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("polygons", pCount); }
        ///<summary>Sets value of setting</summary>
        bool set_setting(int64_t value) { return SetDatatypeProperty ("setting", &value, 1); }
        ///<summary>Gets a value of setting, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_setting() { return GetDatatypeProperty<int64_t>("setting", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class FrustumCone
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class FrustumCone : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class FrustumCone and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static FrustumCone Create(int64_t model, const char* name=NULL) { return FrustumCone(Instance::Create(model, "FrustumCone", name, NULL), "FrustumCone");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static FrustumCone CreateW(int64_t model, const wchar_t* name = NULL) { return FrustumCone(Instance::Create(model, "FrustumCone", NULL, name), "FrustumCone"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        FrustumCone(int64_t instance = NULL)
            : Solid(instance, "FrustumCone")
        {}

    protected:
        FrustumCone(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to FrustumCone
       //

        ///<summary>Sets value of factor</summary>
        bool set_factor(double value) { return SetDatatypeProperty ("factor", &value, 1); }
        ///<summary>Gets a value of factor, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_factor() { return GetDatatypeProperty<double>("factor", NULL); }
        ///<summary>Sets value of height</summary>
        bool set_height(double value) { return SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets a value of height, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of radius</summary>
        bool set_radius(double value) { return SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets a value of radius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Torus
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Torus : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Torus and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Torus Create(int64_t model, const char* name=NULL) { return Torus(Instance::Create(model, "Torus", name, NULL), "Torus");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Torus CreateW(int64_t model, const wchar_t* name = NULL) { return Torus(Instance::Create(model, "Torus", NULL, name), "Torus"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Torus(int64_t instance = NULL)
            : Solid(instance, "Torus")
        {}

    protected:
        Torus(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Torus
       //

        ///<summary>Sets value of majorRadius</summary>
        bool set_majorRadius(double value) { return SetDatatypeProperty ("majorRadius", &value, 1); }
        ///<summary>Gets a value of majorRadius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_majorRadius() { return GetDatatypeProperty<double>("majorRadius", NULL); }
        ///<summary>Sets value of minorRadius</summary>
        bool set_minorRadius(double value) { return SetDatatypeProperty ("minorRadius", &value, 1); }
        ///<summary>Gets a value of minorRadius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_minorRadius() { return GetDatatypeProperty<double>("minorRadius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class HornTorus
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class HornTorus : public Torus
    {
    public:
        /// <summary>
        /// Create new instace of OWL class HornTorus and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static HornTorus Create(int64_t model, const char* name=NULL) { return HornTorus(Instance::Create(model, "HornTorus", name, NULL), "HornTorus");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static HornTorus CreateW(int64_t model, const wchar_t* name = NULL) { return HornTorus(Instance::Create(model, "HornTorus", NULL, name), "HornTorus"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        HornTorus(int64_t instance = NULL)
            : Torus(instance, "HornTorus")
        {}

    protected:
        HornTorus(int64_t instance, const char* checkClassName)
            : Torus(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Hyperbola
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Hyperbola : public ConicalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Hyperbola and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Hyperbola Create(int64_t model, const char* name=NULL) { return Hyperbola(Instance::Create(model, "Hyperbola", name, NULL), "Hyperbola");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Hyperbola CreateW(int64_t model, const wchar_t* name = NULL) { return Hyperbola(Instance::Create(model, "Hyperbola", NULL, name), "Hyperbola"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Hyperbola(int64_t instance = NULL)
            : ConicalCurve(instance, "Hyperbola")
        {}

    protected:
        Hyperbola(int64_t instance, const char* checkClassName)
            : ConicalCurve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Hyperbola
       //

        ///<summary>Sets value of b</summary>
        bool set_b(double value) { return SetDatatypeProperty ("b", &value, 1); }
        ///<summary>Gets a value of b, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_b() { return GetDatatypeProperty<double>("b", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Intersection
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Intersection : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Intersection and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Intersection Create(int64_t model, const char* name=NULL) { return Intersection(Instance::Create(model, "Intersection", name, NULL), "Intersection");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Intersection CreateW(int64_t model, const wchar_t* name = NULL) { return Intersection(Instance::Create(model, "Intersection", NULL, name), "Intersection"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Intersection(int64_t instance = NULL)
            : GeometricItem(instance, "Intersection")
        {}

    protected:
        Intersection(int64_t instance, const char* checkClassName)
            : GeometricItem(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class IntersectionCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class IntersectionCurve : public Intersection
    {
    public:
        /// <summary>
        /// Create new instace of OWL class IntersectionCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static IntersectionCurve Create(int64_t model, const char* name=NULL) { return IntersectionCurve(Instance::Create(model, "IntersectionCurve", name, NULL), "IntersectionCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static IntersectionCurve CreateW(int64_t model, const wchar_t* name = NULL) { return IntersectionCurve(Instance::Create(model, "IntersectionCurve", NULL, name), "IntersectionCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        IntersectionCurve(int64_t instance = NULL)
            : Intersection(instance, "IntersectionCurve")
        {}

    protected:
        IntersectionCurve(int64_t instance, const char* checkClassName)
            : Intersection(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to IntersectionCurve
       //

        ///<summary>Sets relationships from this instance to an array of Surface. OWL cardinality 2..2</summary>
        bool set_surfaces(const Surface* instances, int64_t count) { return SetObjectProperty<Surface>("surfaces", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 2..2</summary>
        bool set_surfaces(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("surfaces", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 2..2. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Surface* get_surfaces(int64_t* pCount) { return GetObjectProperty<Surface>("surfaces", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 2..2. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_surfaces_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("surfaces", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class IntersectionPoint
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class IntersectionPoint : public Intersection
    {
    public:
        /// <summary>
        /// Create new instace of OWL class IntersectionPoint and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static IntersectionPoint Create(int64_t model, const char* name=NULL) { return IntersectionPoint(Instance::Create(model, "IntersectionPoint", name, NULL), "IntersectionPoint");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static IntersectionPoint CreateW(int64_t model, const wchar_t* name = NULL) { return IntersectionPoint(Instance::Create(model, "IntersectionPoint", NULL, name), "IntersectionPoint"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        IntersectionPoint(int64_t instance = NULL)
            : Intersection(instance, "IntersectionPoint")
        {}

    protected:
        IntersectionPoint(int64_t instance, const char* checkClassName)
            : Intersection(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to IntersectionPoint
       //

        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_curve(const Curve& instance) { return SetObjectProperty<Curve>("curve", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_curve() { return GetObjectProperty<Curve>("curve", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Surface</summary>
        bool set_surface(const Surface& instance) { return SetObjectProperty<Surface>("surface", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Surface* get_surface() { return GetObjectProperty<Surface>("surface", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Mathematics
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Mathematics : public Instance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Mathematics and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Mathematics Create(int64_t model, const char* name=NULL) { return Mathematics(Instance::Create(model, "Mathematics", name, NULL), "Mathematics");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Mathematics CreateW(int64_t model, const wchar_t* name = NULL) { return Mathematics(Instance::Create(model, "Mathematics", NULL, name), "Mathematics"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Mathematics(int64_t instance = NULL)
            : Instance(instance, "Mathematics")
        {}

    protected:
        Mathematics(int64_t instance, const char* checkClassName)
            : Instance(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Matrix
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Matrix : public Mathematics
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Matrix and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Matrix Create(int64_t model, const char* name=NULL) { return Matrix(Instance::Create(model, "Matrix", name, NULL), "Matrix");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Matrix CreateW(int64_t model, const wchar_t* name = NULL) { return Matrix(Instance::Create(model, "Matrix", NULL, name), "Matrix"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Matrix(int64_t instance = NULL)
            : Mathematics(instance, "Matrix")
        {}

    protected:
        Matrix(int64_t instance, const char* checkClassName)
            : Mathematics(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Matrix
       //

        ///<summary>Sets value of _11</summary>
        bool set__11(double value) { return SetDatatypeProperty ("_11", &value, 1); }
        ///<summary>Gets a value of _11, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get__11() { return GetDatatypeProperty<double>("_11", NULL); }
        ///<summary>Sets value of _12</summary>
        bool set__12(double value) { return SetDatatypeProperty ("_12", &value, 1); }
        ///<summary>Gets a value of _12, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get__12() { return GetDatatypeProperty<double>("_12", NULL); }
        ///<summary>Sets value of _13</summary>
        bool set__13(double value) { return SetDatatypeProperty ("_13", &value, 1); }
        ///<summary>Gets a value of _13, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get__13() { return GetDatatypeProperty<double>("_13", NULL); }
        ///<summary>Sets value of _21</summary>
        bool set__21(double value) { return SetDatatypeProperty ("_21", &value, 1); }
        ///<summary>Gets a value of _21, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get__21() { return GetDatatypeProperty<double>("_21", NULL); }
        ///<summary>Sets value of _22</summary>
        bool set__22(double value) { return SetDatatypeProperty ("_22", &value, 1); }
        ///<summary>Gets a value of _22, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get__22() { return GetDatatypeProperty<double>("_22", NULL); }
        ///<summary>Sets value of _23</summary>
        bool set__23(double value) { return SetDatatypeProperty ("_23", &value, 1); }
        ///<summary>Gets a value of _23, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get__23() { return GetDatatypeProperty<double>("_23", NULL); }
        ///<summary>Sets value of _31</summary>
        bool set__31(double value) { return SetDatatypeProperty ("_31", &value, 1); }
        ///<summary>Gets a value of _31, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get__31() { return GetDatatypeProperty<double>("_31", NULL); }
        ///<summary>Sets value of _32</summary>
        bool set__32(double value) { return SetDatatypeProperty ("_32", &value, 1); }
        ///<summary>Gets a value of _32, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get__32() { return GetDatatypeProperty<double>("_32", NULL); }
        ///<summary>Sets value of _33</summary>
        bool set__33(double value) { return SetDatatypeProperty ("_33", &value, 1); }
        ///<summary>Gets a value of _33, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get__33() { return GetDatatypeProperty<double>("_33", NULL); }
        ///<summary>Sets value of _41</summary>
        bool set__41(double value) { return SetDatatypeProperty ("_41", &value, 1); }
        ///<summary>Gets a value of _41, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get__41() { return GetDatatypeProperty<double>("_41", NULL); }
        ///<summary>Sets value of _42</summary>
        bool set__42(double value) { return SetDatatypeProperty ("_42", &value, 1); }
        ///<summary>Gets a value of _42, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get__42() { return GetDatatypeProperty<double>("_42", NULL); }
        ///<summary>Sets value of _43</summary>
        bool set__43(double value) { return SetDatatypeProperty ("_43", &value, 1); }
        ///<summary>Gets a value of _43, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get__43() { return GetDatatypeProperty<double>("_43", NULL); }
        ///<summary>Sets values of coordinates. OWL cardinality 0..12</summary>
        bool set_coordinates(const double* values, int64_t count) { return SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Sets values of coordinates from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..12</summary>
        template <typename TList> bool set_coordinates(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_coordinates(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of coordinates. OWL cardinality 0..12. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class InverseMatrix
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class InverseMatrix : public Matrix
    {
    public:
        /// <summary>
        /// Create new instace of OWL class InverseMatrix and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static InverseMatrix Create(int64_t model, const char* name=NULL) { return InverseMatrix(Instance::Create(model, "InverseMatrix", name, NULL), "InverseMatrix");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static InverseMatrix CreateW(int64_t model, const wchar_t* name = NULL) { return InverseMatrix(Instance::Create(model, "InverseMatrix", NULL, name), "InverseMatrix"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        InverseMatrix(int64_t instance = NULL)
            : Matrix(instance, "InverseMatrix")
        {}

    protected:
        InverseMatrix(int64_t instance, const char* checkClassName)
            : Matrix(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to InverseMatrix
       //

        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        bool set_matrix(const Matrix& instance) { return SetObjectProperty<Matrix>("matrix", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Matrix* get_matrix() { return GetObjectProperty<Matrix>("matrix", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class InvertedCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class InvertedCurve : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class InvertedCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static InvertedCurve Create(int64_t model, const char* name=NULL) { return InvertedCurve(Instance::Create(model, "InvertedCurve", name, NULL), "InvertedCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static InvertedCurve CreateW(int64_t model, const wchar_t* name = NULL) { return InvertedCurve(Instance::Create(model, "InvertedCurve", NULL, name), "InvertedCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        InvertedCurve(int64_t instance = NULL)
            : Curve(instance, "InvertedCurve")
        {}

    protected:
        InvertedCurve(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to InvertedCurve
       //

        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_curve(const Curve& instance) { return SetObjectProperty<Curve>("curve", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_curve() { return GetObjectProperty<Curve>("curve", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class InvertedSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class InvertedSurface : public Surface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class InvertedSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static InvertedSurface Create(int64_t model, const char* name=NULL) { return InvertedSurface(Instance::Create(model, "InvertedSurface", name, NULL), "InvertedSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static InvertedSurface CreateW(int64_t model, const wchar_t* name = NULL) { return InvertedSurface(Instance::Create(model, "InvertedSurface", NULL, name), "InvertedSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        InvertedSurface(int64_t instance = NULL)
            : Surface(instance, "InvertedSurface")
        {}

    protected:
        InvertedSurface(int64_t instance, const char* checkClassName)
            : Surface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to InvertedSurface
       //

        ///<summary>Sets relationship from this instance to an instance of Surface</summary>
        bool set_surface(const Surface& instance) { return SetObjectProperty<Surface>("surface", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Surface* get_surface() { return GetObjectProperty<Surface>("surface", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Line3D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Line3D : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Line3D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Line3D Create(int64_t model, const char* name=NULL) { return Line3D(Instance::Create(model, "Line3D", name, NULL), "Line3D");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Line3D CreateW(int64_t model, const wchar_t* name = NULL) { return Line3D(Instance::Create(model, "Line3D", NULL, name), "Line3D"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Line3D(int64_t instance = NULL)
            : Curve(instance, "Line3D")
        {}

    protected:
        Line3D(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Line3D
       //

        ///<summary>Sets value of asOpenGL</summary>
        bool set_asOpenGL(bool value) { return SetDatatypeProperty ("asOpenGL", &value, 1); }
        ///<summary>Gets a value of asOpenGL, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_asOpenGL() { return GetDatatypeProperty<bool>("asOpenGL", NULL); }
        ///<summary>Sets values of points. OWL cardinality 6..6</summary>
        bool set_points(const double* values, int64_t count) { return SetDatatypeProperty ("points", values, count); }
        ///<summary>Sets values of points from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 6..6</summary>
        template <typename TList> bool set_points(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_points(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of points. OWL cardinality 6..6. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Line3Dn
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Line3Dn : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Line3Dn and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Line3Dn Create(int64_t model, const char* name=NULL) { return Line3Dn(Instance::Create(model, "Line3Dn", name, NULL), "Line3Dn");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Line3Dn CreateW(int64_t model, const wchar_t* name = NULL) { return Line3Dn(Instance::Create(model, "Line3Dn", NULL, name), "Line3Dn"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Line3Dn(int64_t instance = NULL)
            : Curve(instance, "Line3Dn")
        {}

    protected:
        Line3Dn(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Line3Dn
       //

        ///<summary>Sets value of asOpenGL</summary>
        bool set_asOpenGL(bool value) { return SetDatatypeProperty ("asOpenGL", &value, 1); }
        ///<summary>Gets a value of asOpenGL, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_asOpenGL() { return GetDatatypeProperty<bool>("asOpenGL", NULL); }
        ///<summary>Sets values of endDirection. OWL cardinality 0..3</summary>
        bool set_endDirection(const double* values, int64_t count) { return SetDatatypeProperty ("endDirection", values, count); }
        ///<summary>Sets values of endDirection from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..3</summary>
        template <typename TList> bool set_endDirection(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_endDirection(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of endDirection. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_endDirection(int64_t* pCount) { return GetDatatypeProperty<double>("endDirection", pCount); }
        ///<summary>Sets values of points. OWL cardinality 0..6</summary>
        bool set_points(const double* values, int64_t count) { return SetDatatypeProperty ("points", values, count); }
        ///<summary>Sets values of points from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..6</summary>
        template <typename TList> bool set_points(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_points(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of points. OWL cardinality 0..6. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
        ///<summary>Sets values of startDirection. OWL cardinality 0..3</summary>
        bool set_startDirection(const double* values, int64_t count) { return SetDatatypeProperty ("startDirection", values, count); }
        ///<summary>Sets values of startDirection from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..3</summary>
        template <typename TList> bool set_startDirection(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_startDirection(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of startDirection. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_startDirection(int64_t* pCount) { return GetDatatypeProperty<double>("startDirection", pCount); }
        ///<summary>Sets value of x0</summary>
        bool set_x0(double value) { return SetDatatypeProperty ("x0", &value, 1); }
        ///<summary>Gets a value of x0, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_x0() { return GetDatatypeProperty<double>("x0", NULL); }
        ///<summary>Sets value of x1</summary>
        bool set_x1(double value) { return SetDatatypeProperty ("x1", &value, 1); }
        ///<summary>Gets a value of x1, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_x1() { return GetDatatypeProperty<double>("x1", NULL); }
        ///<summary>Sets value of y0</summary>
        bool set_y0(double value) { return SetDatatypeProperty ("y0", &value, 1); }
        ///<summary>Gets a value of y0, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_y0() { return GetDatatypeProperty<double>("y0", NULL); }
        ///<summary>Sets value of y1</summary>
        bool set_y1(double value) { return SetDatatypeProperty ("y1", &value, 1); }
        ///<summary>Gets a value of y1, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_y1() { return GetDatatypeProperty<double>("y1", NULL); }
        ///<summary>Sets value of z0</summary>
        bool set_z0(double value) { return SetDatatypeProperty ("z0", &value, 1); }
        ///<summary>Gets a value of z0, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_z0() { return GetDatatypeProperty<double>("z0", NULL); }
        ///<summary>Sets value of z1</summary>
        bool set_z1(double value) { return SetDatatypeProperty ("z1", &value, 1); }
        ///<summary>Gets a value of z1, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_z1() { return GetDatatypeProperty<double>("z1", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Line3DSet
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Line3DSet : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Line3DSet and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Line3DSet Create(int64_t model, const char* name=NULL) { return Line3DSet(Instance::Create(model, "Line3DSet", name, NULL), "Line3DSet");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Line3DSet CreateW(int64_t model, const wchar_t* name = NULL) { return Line3DSet(Instance::Create(model, "Line3DSet", NULL, name), "Line3DSet"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Line3DSet(int64_t instance = NULL)
            : Curve(instance, "Line3DSet")
        {}

    protected:
        Line3DSet(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Line3DSet
       //

        ///<summary>Sets value of asOpenGL</summary>
        bool set_asOpenGL(bool value) { return SetDatatypeProperty ("asOpenGL", &value, 1); }
        ///<summary>Gets a value of asOpenGL, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_asOpenGL() { return GetDatatypeProperty<bool>("asOpenGL", NULL); }
        ///<summary>Sets values of points. OWL cardinality 0..-1</summary>
        bool set_points(const double* values, int64_t count) { return SetDatatypeProperty ("points", values, count); }
        ///<summary>Sets values of points from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_points(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_points(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of points. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Line4D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Line4D : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Line4D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Line4D Create(int64_t model, const char* name=NULL) { return Line4D(Instance::Create(model, "Line4D", name, NULL), "Line4D");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Line4D CreateW(int64_t model, const wchar_t* name = NULL) { return Line4D(Instance::Create(model, "Line4D", NULL, name), "Line4D"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Line4D(int64_t instance = NULL)
            : Curve(instance, "Line4D")
        {}

    protected:
        Line4D(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Line4D
       //

        ///<summary>Sets relationship from this instance to an instance of Point</summary>
        bool set_firstPoint(const Point& instance) { return SetObjectProperty<Point>("firstPoint", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Point* get_firstPoint() { return GetObjectProperty<Point>("firstPoint", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Point</summary>
        bool set_secondPoint(const Point& instance) { return SetObjectProperty<Point>("secondPoint", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Point* get_secondPoint() { return GetObjectProperty<Point>("secondPoint", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class LineByFace
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class LineByFace : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class LineByFace and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static LineByFace Create(int64_t model, const char* name=NULL) { return LineByFace(Instance::Create(model, "LineByFace", name, NULL), "LineByFace");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static LineByFace CreateW(int64_t model, const wchar_t* name = NULL) { return LineByFace(Instance::Create(model, "LineByFace", NULL, name), "LineByFace"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        LineByFace(int64_t instance = NULL)
            : Curve(instance, "LineByFace")
        {}

    protected:
        LineByFace(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to LineByFace
       //

        ///<summary>Sets relationships from this instance to an array of Face. OWL cardinality 1..-1</summary>
        bool set_faces(const Face* instances, int64_t count) { return SetObjectProperty<Face>("faces", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        bool set_faces(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("faces", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Face* get_faces(int64_t* pCount) { return GetObjectProperty<Face>("faces", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_faces_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("faces", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Material
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Material : public Appearance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Material and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Material Create(int64_t model, const char* name=NULL) { return Material(Instance::Create(model, "Material", name, NULL), "Material");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Material CreateW(int64_t model, const wchar_t* name = NULL) { return Material(Instance::Create(model, "Material", NULL, name), "Material"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Material(int64_t instance = NULL)
            : Appearance(instance, "Material")
        {}

    protected:
        Material(int64_t instance, const char* checkClassName)
            : Appearance(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Material
       //

        ///<summary>Sets relationship from this instance to an instance of Color</summary>
        bool set_color(const Color& instance) { return SetObjectProperty<Color>("color", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Color* get_color() { return GetObjectProperty<Color>("color", NULL); }
        ///<summary>Sets relationships from this instance to an array of Texture. OWL cardinality 0..2</summary>
        bool set_textures(const Texture* instances, int64_t count) { return SetObjectProperty<Texture>("textures", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..2</summary>
        bool set_textures(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("textures", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..2. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Texture* get_textures(int64_t* pCount) { return GetObjectProperty<Texture>("textures", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..2. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_textures_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("textures", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class MatrixMultiplication
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class MatrixMultiplication : public Matrix
    {
    public:
        /// <summary>
        /// Create new instace of OWL class MatrixMultiplication and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static MatrixMultiplication Create(int64_t model, const char* name=NULL) { return MatrixMultiplication(Instance::Create(model, "MatrixMultiplication", name, NULL), "MatrixMultiplication");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static MatrixMultiplication CreateW(int64_t model, const wchar_t* name = NULL) { return MatrixMultiplication(Instance::Create(model, "MatrixMultiplication", NULL, name), "MatrixMultiplication"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        MatrixMultiplication(int64_t instance = NULL)
            : Matrix(instance, "MatrixMultiplication")
        {}

    protected:
        MatrixMultiplication(int64_t instance, const char* checkClassName)
            : Matrix(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to MatrixMultiplication
       //

        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        bool set_firstMatrix(const Matrix& instance) { return SetObjectProperty<Matrix>("firstMatrix", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Matrix* get_firstMatrix() { return GetObjectProperty<Matrix>("firstMatrix", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        bool set_secondMatrix(const Matrix& instance) { return SetObjectProperty<Matrix>("secondMatrix", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Matrix* get_secondMatrix() { return GetObjectProperty<Matrix>("secondMatrix", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Mesh
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Mesh : public Face
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Mesh and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Mesh Create(int64_t model, const char* name=NULL) { return Mesh(Instance::Create(model, "Mesh", name, NULL), "Mesh");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Mesh CreateW(int64_t model, const wchar_t* name = NULL) { return Mesh(Instance::Create(model, "Mesh", NULL, name), "Mesh"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Mesh(int64_t instance = NULL)
            : Face(instance, "Mesh")
        {}

    protected:
        Mesh(int64_t instance, const char* checkClassName)
            : Face(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Mesh
       //

        ///<summary>Sets relationship from this instance to an instance of Point3DSet</summary>
        bool set_pointSet(const Point3DSet& instance) { return SetObjectProperty<Point3DSet>("pointSet", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Point3DSet* get_pointSet() { return GetObjectProperty<Point3DSet>("pointSet", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class NURBSCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class NURBSCurve : public BSplineCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class NURBSCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static NURBSCurve Create(int64_t model, const char* name=NULL) { return NURBSCurve(Instance::Create(model, "NURBSCurve", name, NULL), "NURBSCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static NURBSCurve CreateW(int64_t model, const wchar_t* name = NULL) { return NURBSCurve(Instance::Create(model, "NURBSCurve", NULL, name), "NURBSCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        NURBSCurve(int64_t instance = NULL)
            : BSplineCurve(instance, "NURBSCurve")
        {}

    protected:
        NURBSCurve(int64_t instance, const char* checkClassName)
            : BSplineCurve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to NURBSCurve
       //

        ///<summary>Sets values of weights. OWL cardinality 2..-1</summary>
        bool set_weights(const double* values, int64_t count) { return SetDatatypeProperty ("weights", values, count); }
        ///<summary>Sets values of weights from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 2..-1</summary>
        template <typename TList> bool set_weights(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_weights(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of weights. OWL cardinality 2..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_weights(int64_t* pCount) { return GetDatatypeProperty<double>("weights", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class NURBSSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class NURBSSurface : public BSplineSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class NURBSSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static NURBSSurface Create(int64_t model, const char* name=NULL) { return NURBSSurface(Instance::Create(model, "NURBSSurface", name, NULL), "NURBSSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static NURBSSurface CreateW(int64_t model, const wchar_t* name = NULL) { return NURBSSurface(Instance::Create(model, "NURBSSurface", NULL, name), "NURBSSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        NURBSSurface(int64_t instance = NULL)
            : BSplineSurface(instance, "NURBSSurface")
        {}

    protected:
        NURBSSurface(int64_t instance, const char* checkClassName)
            : BSplineSurface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to NURBSSurface
       //

        ///<summary>Sets values of weights. OWL cardinality 4..-1</summary>
        bool set_weights(const double* values, int64_t count) { return SetDatatypeProperty ("weights", values, count); }
        ///<summary>Sets values of weights from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 4..-1</summary>
        template <typename TList> bool set_weights(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_weights(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of weights. OWL cardinality 4..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_weights(int64_t* pCount) { return GetDatatypeProperty<double>("weights", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Parabola
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Parabola : public ConicalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Parabola and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Parabola Create(int64_t model, const char* name=NULL) { return Parabola(Instance::Create(model, "Parabola", name, NULL), "Parabola");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Parabola CreateW(int64_t model, const wchar_t* name = NULL) { return Parabola(Instance::Create(model, "Parabola", NULL, name), "Parabola"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Parabola(int64_t instance = NULL)
            : ConicalCurve(instance, "Parabola")
        {}

    protected:
        Parabola(int64_t instance, const char* checkClassName)
            : ConicalCurve(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Plane
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Plane : public Mathematics
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Plane and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Plane Create(int64_t model, const char* name=NULL) { return Plane(Instance::Create(model, "Plane", name, NULL), "Plane");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Plane CreateW(int64_t model, const wchar_t* name = NULL) { return Plane(Instance::Create(model, "Plane", NULL, name), "Plane"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Plane(int64_t instance = NULL)
            : Mathematics(instance, "Plane")
        {}

    protected:
        Plane(int64_t instance, const char* checkClassName)
            : Mathematics(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Plane
       //

        ///<summary>Sets value of A</summary>
        bool set_A(double value) { return SetDatatypeProperty ("A", &value, 1); }
        ///<summary>Gets a value of A, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_A() { return GetDatatypeProperty<double>("A", NULL); }
        ///<summary>Sets value of B</summary>
        bool set_B(double value) { return SetDatatypeProperty ("B", &value, 1); }
        ///<summary>Gets a value of B, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_B() { return GetDatatypeProperty<double>("B", NULL); }
        ///<summary>Sets value of C</summary>
        bool set_C(double value) { return SetDatatypeProperty ("C", &value, 1); }
        ///<summary>Gets a value of C, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_C() { return GetDatatypeProperty<double>("C", NULL); }
        ///<summary>Sets value of D</summary>
        bool set_D(double value) { return SetDatatypeProperty ("D", &value, 1); }
        ///<summary>Gets a value of D, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_D() { return GetDatatypeProperty<double>("D", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class PlaneSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class PlaneSurface : public InfiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class PlaneSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static PlaneSurface Create(int64_t model, const char* name=NULL) { return PlaneSurface(Instance::Create(model, "PlaneSurface", name, NULL), "PlaneSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static PlaneSurface CreateW(int64_t model, const wchar_t* name = NULL) { return PlaneSurface(Instance::Create(model, "PlaneSurface", NULL, name), "PlaneSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        PlaneSurface(int64_t instance = NULL)
            : InfiniteSurface(instance, "PlaneSurface")
        {}

    protected:
        PlaneSurface(int64_t instance, const char* checkClassName)
            : InfiniteSurface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to PlaneSurface
       //

        ///<summary>Sets relationship from this instance to an instance of Plane</summary>
        bool set_plane(const Plane& instance) { return SetObjectProperty<Plane>("plane", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Plane* get_plane() { return GetObjectProperty<Plane>("plane", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Point
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Point : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Point and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point Create(int64_t model, const char* name=NULL) { return Point(Instance::Create(model, "Point", name, NULL), "Point");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point CreateW(int64_t model, const wchar_t* name = NULL) { return Point(Instance::Create(model, "Point", NULL, name), "Point"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Point(int64_t instance = NULL)
            : GeometricItem(instance, "Point")
        {}

    protected:
        Point(int64_t instance, const char* checkClassName)
            : GeometricItem(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Point3D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Point3D : public Point
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Point3D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point3D Create(int64_t model, const char* name=NULL) { return Point3D(Instance::Create(model, "Point3D", name, NULL), "Point3D");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point3D CreateW(int64_t model, const wchar_t* name = NULL) { return Point3D(Instance::Create(model, "Point3D", NULL, name), "Point3D"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Point3D(int64_t instance = NULL)
            : Point(instance, "Point3D")
        {}

    protected:
        Point3D(int64_t instance, const char* checkClassName)
            : Point(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Point3D
       //

        ///<summary>Sets value of asOpenGL</summary>
        bool set_asOpenGL(bool value) { return SetDatatypeProperty ("asOpenGL", &value, 1); }
        ///<summary>Gets a value of asOpenGL, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_asOpenGL() { return GetDatatypeProperty<bool>("asOpenGL", NULL); }
        ///<summary>Sets values of coordinates. OWL cardinality 0..3</summary>
        bool set_coordinates(const double* values, int64_t count) { return SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Sets values of coordinates from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..3</summary>
        template <typename TList> bool set_coordinates(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_coordinates(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of coordinates. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets values of points. OWL cardinality 0..3</summary>
        bool set_points(const double* values, int64_t count) { return SetDatatypeProperty ("points", values, count); }
        ///<summary>Sets values of points from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..3</summary>
        template <typename TList> bool set_points(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_points(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of points. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
        ///<summary>Sets value of x</summary>
        bool set_x(double value) { return SetDatatypeProperty ("x", &value, 1); }
        ///<summary>Gets a value of x, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_x() { return GetDatatypeProperty<double>("x", NULL); }
        ///<summary>Sets value of y</summary>
        bool set_y(double value) { return SetDatatypeProperty ("y", &value, 1); }
        ///<summary>Gets a value of y, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_y() { return GetDatatypeProperty<double>("y", NULL); }
        ///<summary>Sets value of z</summary>
        bool set_z(double value) { return SetDatatypeProperty ("z", &value, 1); }
        ///<summary>Gets a value of z, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_z() { return GetDatatypeProperty<double>("z", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Point3DSet
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Point3DSet : public Point
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Point3DSet and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point3DSet Create(int64_t model, const char* name=NULL) { return Point3DSet(Instance::Create(model, "Point3DSet", name, NULL), "Point3DSet");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point3DSet CreateW(int64_t model, const wchar_t* name = NULL) { return Point3DSet(Instance::Create(model, "Point3DSet", NULL, name), "Point3DSet"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Point3DSet(int64_t instance = NULL)
            : Point(instance, "Point3DSet")
        {}

    protected:
        Point3DSet(int64_t instance, const char* checkClassName)
            : Point(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Point3DSet
       //

        ///<summary>Sets value of asOpenGL</summary>
        bool set_asOpenGL(bool value) { return SetDatatypeProperty ("asOpenGL", &value, 1); }
        ///<summary>Gets a value of asOpenGL, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_asOpenGL() { return GetDatatypeProperty<bool>("asOpenGL", NULL); }
        ///<summary>Sets values of coordinates. OWL cardinality 0..-1</summary>
        bool set_coordinates(const double* values, int64_t count) { return SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Sets values of coordinates from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_coordinates(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_coordinates(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of coordinates. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets values of points. OWL cardinality 0..-1</summary>
        bool set_points(const double* values, int64_t count) { return SetDatatypeProperty ("points", values, count); }
        ///<summary>Sets values of points from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_points(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_points(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of points. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Point3DSetByGeometricItem
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Point3DSetByGeometricItem : public Point3DSet
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Point3DSetByGeometricItem and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point3DSetByGeometricItem Create(int64_t model, const char* name=NULL) { return Point3DSetByGeometricItem(Instance::Create(model, "Point3DSetByGeometricItem", name, NULL), "Point3DSetByGeometricItem");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point3DSetByGeometricItem CreateW(int64_t model, const wchar_t* name = NULL) { return Point3DSetByGeometricItem(Instance::Create(model, "Point3DSetByGeometricItem", NULL, name), "Point3DSetByGeometricItem"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Point3DSetByGeometricItem(int64_t instance = NULL)
            : Point3DSet(instance, "Point3DSetByGeometricItem")
        {}

    protected:
        Point3DSetByGeometricItem(int64_t instance, const char* checkClassName)
            : Point3DSet(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Point3DSetByGeometricItem
       //

        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        bool set_object(const GeometricItem& instance) { return SetObjectProperty<GeometricItem>("object", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const GeometricItem* get_object() { return GetObjectProperty<GeometricItem>("object", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Point4D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Point4D : public Point
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Point4D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point4D Create(int64_t model, const char* name=NULL) { return Point4D(Instance::Create(model, "Point4D", name, NULL), "Point4D");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Point4D CreateW(int64_t model, const wchar_t* name = NULL) { return Point4D(Instance::Create(model, "Point4D", NULL, name), "Point4D"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Point4D(int64_t instance = NULL)
            : Point(instance, "Point4D")
        {}

    protected:
        Point4D(int64_t instance, const char* checkClassName)
            : Point(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Point4D
       //

        ///<summary>Sets relationship from this instance to an instance of Vector3</summary>
        bool set_normal(const Vector3& instance) { return SetObjectProperty<Vector3>("normal", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Vector3* get_normal() { return GetObjectProperty<Vector3>("normal", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Point3D</summary>
        bool set_point(const Point3D& instance) { return SetObjectProperty<Point3D>("point", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Point3D* get_point() { return GetObjectProperty<Point3D>("point", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Vector3</summary>
        bool set_tangent(const Vector3& instance) { return SetObjectProperty<Vector3>("tangent", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Vector3* get_tangent() { return GetObjectProperty<Vector3>("tangent", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class PointLight
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class PointLight : public DirectLight
    {
    public:
        /// <summary>
        /// Create new instace of OWL class PointLight and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static PointLight Create(int64_t model, const char* name=NULL) { return PointLight(Instance::Create(model, "PointLight", name, NULL), "PointLight");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static PointLight CreateW(int64_t model, const wchar_t* name = NULL) { return PointLight(Instance::Create(model, "PointLight", NULL, name), "PointLight"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        PointLight(int64_t instance = NULL)
            : DirectLight(instance, "PointLight")
        {}

    protected:
        PointLight(int64_t instance, const char* checkClassName)
            : DirectLight(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to PointLight
       //

        ///<summary>Sets relationship from this instance to an instance of Point3D</summary>
        bool set_position(const Point3D& instance) { return SetObjectProperty<Point3D>("position", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Point3D* get_position() { return GetObjectProperty<Point3D>("position", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class PointLoop
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class PointLoop : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class PointLoop and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static PointLoop Create(int64_t model, const char* name=NULL) { return PointLoop(Instance::Create(model, "PointLoop", name, NULL), "PointLoop");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static PointLoop CreateW(int64_t model, const wchar_t* name = NULL) { return PointLoop(Instance::Create(model, "PointLoop", NULL, name), "PointLoop"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        PointLoop(int64_t instance = NULL)
            : Curve(instance, "PointLoop")
        {}

    protected:
        PointLoop(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to PointLoop
       //

        ///<summary>Sets values of coordinates. OWL cardinality 0..3</summary>
        bool set_coordinates(const double* values, int64_t count) { return SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Sets values of coordinates from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..3</summary>
        template <typename TList> bool set_coordinates(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_coordinates(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of coordinates. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets value of x</summary>
        bool set_x(double value) { return SetDatatypeProperty ("x", &value, 1); }
        ///<summary>Gets a value of x, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_x() { return GetDatatypeProperty<double>("x", NULL); }
        ///<summary>Sets value of y</summary>
        bool set_y(double value) { return SetDatatypeProperty ("y", &value, 1); }
        ///<summary>Gets a value of y, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_y() { return GetDatatypeProperty<double>("y", NULL); }
        ///<summary>Sets value of z</summary>
        bool set_z(double value) { return SetDatatypeProperty ("z", &value, 1); }
        ///<summary>Gets a value of z, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_z() { return GetDatatypeProperty<double>("z", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Polygon2D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Polygon2D : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Polygon2D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Polygon2D Create(int64_t model, const char* name=NULL) { return Polygon2D(Instance::Create(model, "Polygon2D", name, NULL), "Polygon2D");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Polygon2D CreateW(int64_t model, const wchar_t* name = NULL) { return Polygon2D(Instance::Create(model, "Polygon2D", NULL, name), "Polygon2D"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Polygon2D(int64_t instance = NULL)
            : Curve(instance, "Polygon2D")
        {}

    protected:
        Polygon2D(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Polygon2D
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 1..-1</summary>
        bool set_lineParts(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("lineParts", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        bool set_lineParts(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("lineParts", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_lineParts(int64_t* pCount) { return GetObjectProperty<Curve>("lineParts", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_lineParts_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("lineParts", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Polygon3D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Polygon3D : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Polygon3D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Polygon3D Create(int64_t model, const char* name=NULL) { return Polygon3D(Instance::Create(model, "Polygon3D", name, NULL), "Polygon3D");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Polygon3D CreateW(int64_t model, const wchar_t* name = NULL) { return Polygon3D(Instance::Create(model, "Polygon3D", NULL, name), "Polygon3D"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Polygon3D(int64_t instance = NULL)
            : Curve(instance, "Polygon3D")
        {}

    protected:
        Polygon3D(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Polygon3D
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 1..-1</summary>
        bool set_lineParts(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("lineParts", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        bool set_lineParts(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("lineParts", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_lineParts(int64_t* pCount) { return GetObjectProperty<Curve>("lineParts", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_lineParts_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("lineParts", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Polygon4D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Polygon4D : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Polygon4D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Polygon4D Create(int64_t model, const char* name=NULL) { return Polygon4D(Instance::Create(model, "Polygon4D", name, NULL), "Polygon4D");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Polygon4D CreateW(int64_t model, const wchar_t* name = NULL) { return Polygon4D(Instance::Create(model, "Polygon4D", NULL, name), "Polygon4D"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Polygon4D(int64_t instance = NULL)
            : Curve(instance, "Polygon4D")
        {}

    protected:
        Polygon4D(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Polygon4D
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 1..-1</summary>
        bool set_lineParts(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("lineParts", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        bool set_lineParts(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("lineParts", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_lineParts(int64_t* pCount) { return GetObjectProperty<Curve>("lineParts", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_lineParts_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("lineParts", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class PolyLine3D
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class PolyLine3D : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class PolyLine3D and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static PolyLine3D Create(int64_t model, const char* name=NULL) { return PolyLine3D(Instance::Create(model, "PolyLine3D", name, NULL), "PolyLine3D");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static PolyLine3D CreateW(int64_t model, const wchar_t* name = NULL) { return PolyLine3D(Instance::Create(model, "PolyLine3D", NULL, name), "PolyLine3D"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        PolyLine3D(int64_t instance = NULL)
            : Curve(instance, "PolyLine3D")
        {}

    protected:
        PolyLine3D(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to PolyLine3D
       //

        ///<summary>Sets values of coordinates. OWL cardinality 0..-1</summary>
        bool set_coordinates(const double* values, int64_t count) { return SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Sets values of coordinates from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_coordinates(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_coordinates(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of coordinates. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets relationships from this instance to an array of Point3D. OWL cardinality 0..-1</summary>
        bool set_pointReferences(const Point3D* instances, int64_t count) { return SetObjectProperty<Point3D>("pointReferences", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        bool set_pointReferences(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("pointReferences", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Point3D* get_pointReferences(int64_t* pCount) { return GetObjectProperty<Point3D>("pointReferences", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_pointReferences_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("pointReferences", pCount); }
        ///<summary>Sets values of points. OWL cardinality 0..-1</summary>
        bool set_points(const double* values, int64_t count) { return SetDatatypeProperty ("points", values, count); }
        ///<summary>Sets values of points from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_points(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_points(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of points. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_points(int64_t* pCount) { return GetDatatypeProperty<double>("points", pCount); }
        ///<summary>Sets values of tangentArray. OWL cardinality 0..-1</summary>
        bool set_tangentArray(const double* values, int64_t count) { return SetDatatypeProperty ("tangentArray", values, count); }
        ///<summary>Sets values of tangentArray from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_tangentArray(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_tangentArray(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of tangentArray. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_tangentArray(int64_t* pCount) { return GetDatatypeProperty<double>("tangentArray", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Prism
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Prism : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Prism and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Prism Create(int64_t model, const char* name=NULL) { return Prism(Instance::Create(model, "Prism", name, NULL), "Prism");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Prism CreateW(int64_t model, const wchar_t* name = NULL) { return Prism(Instance::Create(model, "Prism", NULL, name), "Prism"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Prism(int64_t instance = NULL)
            : Solid(instance, "Prism")
        {}

    protected:
        Prism(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Prism
       //

        ///<summary>Sets value of height</summary>
        bool set_height(double value) { return SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets a value of height, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of length</summary>
        bool set_length(double value) { return SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets a value of length, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Profile
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Profile : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Profile and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Profile Create(int64_t model, const char* name=NULL) { return Profile(Instance::Create(model, "Profile", name, NULL), "Profile");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Profile CreateW(int64_t model, const wchar_t* name = NULL) { return Profile(Instance::Create(model, "Profile", NULL, name), "Profile"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Profile(int64_t instance = NULL)
            : Curve(instance, "Profile")
        {}

    protected:
        Profile(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Profile
       //

        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        bool set_innerPolygons(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("innerPolygons", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        bool set_innerPolygons(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("innerPolygons", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_innerPolygons(int64_t* pCount) { return GetObjectProperty<Curve>("innerPolygons", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_innerPolygons_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("innerPolygons", pCount); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_outerPolygon(const Curve& instance) { return SetObjectProperty<Curve>("outerPolygon", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_outerPolygon() { return GetObjectProperty<Curve>("outerPolygon", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Projection
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Projection : public Environment
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Projection and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Projection Create(int64_t model, const char* name=NULL) { return Projection(Instance::Create(model, "Projection", name, NULL), "Projection");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Projection CreateW(int64_t model, const wchar_t* name = NULL) { return Projection(Instance::Create(model, "Projection", NULL, name), "Projection"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Projection(int64_t instance = NULL)
            : Environment(instance, "Projection")
        {}

    protected:
        Projection(int64_t instance, const char* checkClassName)
            : Environment(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Projection
       //

        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        bool set_matrix(const Matrix& instance) { return SetObjectProperty<Matrix>("matrix", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Matrix* get_matrix() { return GetObjectProperty<Matrix>("matrix", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Pyramid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Pyramid : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Pyramid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Pyramid Create(int64_t model, const char* name=NULL) { return Pyramid(Instance::Create(model, "Pyramid", name, NULL), "Pyramid");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Pyramid CreateW(int64_t model, const wchar_t* name = NULL) { return Pyramid(Instance::Create(model, "Pyramid", NULL, name), "Pyramid"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Pyramid(int64_t instance = NULL)
            : Solid(instance, "Pyramid")
        {}

    protected:
        Pyramid(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Pyramid
       //

        ///<summary>Sets value of height</summary>
        bool set_height(double value) { return SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets a value of height, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of length</summary>
        bool set_length(double value) { return SetDatatypeProperty ("length", &value, 1); }
        ///<summary>Gets a value of length, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_length() { return GetDatatypeProperty<double>("length", NULL); }
        ///<summary>Sets value of width</summary>
        bool set_width(double value) { return SetDatatypeProperty ("width", &value, 1); }
        ///<summary>Gets a value of width, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_width() { return GetDatatypeProperty<double>("width", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class RationalBezierCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class RationalBezierCurve : public BezierCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class RationalBezierCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static RationalBezierCurve Create(int64_t model, const char* name=NULL) { return RationalBezierCurve(Instance::Create(model, "RationalBezierCurve", name, NULL), "RationalBezierCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static RationalBezierCurve CreateW(int64_t model, const wchar_t* name = NULL) { return RationalBezierCurve(Instance::Create(model, "RationalBezierCurve", NULL, name), "RationalBezierCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        RationalBezierCurve(int64_t instance = NULL)
            : BezierCurve(instance, "RationalBezierCurve")
        {}

    protected:
        RationalBezierCurve(int64_t instance, const char* checkClassName)
            : BezierCurve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to RationalBezierCurve
       //

        ///<summary>Sets values of weights. OWL cardinality 2..-1</summary>
        bool set_weights(const double* values, int64_t count) { return SetDatatypeProperty ("weights", values, count); }
        ///<summary>Sets values of weights from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 2..-1</summary>
        template <typename TList> bool set_weights(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_weights(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of weights. OWL cardinality 2..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_weights(int64_t* pCount) { return GetDatatypeProperty<double>("weights", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class RationalBezierSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class RationalBezierSurface : public BezierSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class RationalBezierSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static RationalBezierSurface Create(int64_t model, const char* name=NULL) { return RationalBezierSurface(Instance::Create(model, "RationalBezierSurface", name, NULL), "RationalBezierSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static RationalBezierSurface CreateW(int64_t model, const wchar_t* name = NULL) { return RationalBezierSurface(Instance::Create(model, "RationalBezierSurface", NULL, name), "RationalBezierSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        RationalBezierSurface(int64_t instance = NULL)
            : BezierSurface(instance, "RationalBezierSurface")
        {}

    protected:
        RationalBezierSurface(int64_t instance, const char* checkClassName)
            : BezierSurface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to RationalBezierSurface
       //

        ///<summary>Sets values of weights. OWL cardinality 4..-1</summary>
        bool set_weights(const double* values, int64_t count) { return SetDatatypeProperty ("weights", values, count); }
        ///<summary>Sets values of weights from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 4..-1</summary>
        template <typename TList> bool set_weights(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_weights(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of weights. OWL cardinality 4..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_weights(int64_t* pCount) { return GetDatatypeProperty<double>("weights", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class RectangleCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class RectangleCurve : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class RectangleCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static RectangleCurve Create(int64_t model, const char* name=NULL) { return RectangleCurve(Instance::Create(model, "RectangleCurve", name, NULL), "RectangleCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static RectangleCurve CreateW(int64_t model, const wchar_t* name = NULL) { return RectangleCurve(Instance::Create(model, "RectangleCurve", NULL, name), "RectangleCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        RectangleCurve(int64_t instance = NULL)
            : Curve(instance, "RectangleCurve")
        {}

    protected:
        RectangleCurve(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to RectangleCurve
       //

        ///<summary>Sets value of height</summary>
        bool set_height(double value) { return SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets a value of height, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of offsetX</summary>
        bool set_offsetX(double value) { return SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets a value of offsetX, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of offsetY</summary>
        bool set_offsetY(double value) { return SetDatatypeProperty ("offsetY", &value, 1); }
        ///<summary>Gets a value of offsetY, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetY() { return GetDatatypeProperty<double>("offsetY", NULL); }
        ///<summary>Sets value of offsetZ</summary>
        bool set_offsetZ(double value) { return SetDatatypeProperty ("offsetZ", &value, 1); }
        ///<summary>Gets a value of offsetZ, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetZ() { return GetDatatypeProperty<double>("offsetZ", NULL); }
        ///<summary>Sets value of width</summary>
        bool set_width(double value) { return SetDatatypeProperty ("width", &value, 1); }
        ///<summary>Gets a value of width, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_width() { return GetDatatypeProperty<double>("width", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Repetition
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Repetition : public GeometricItem
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Repetition and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Repetition Create(int64_t model, const char* name=NULL) { return Repetition(Instance::Create(model, "Repetition", name, NULL), "Repetition");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Repetition CreateW(int64_t model, const wchar_t* name = NULL) { return Repetition(Instance::Create(model, "Repetition", NULL, name), "Repetition"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Repetition(int64_t instance = NULL)
            : GeometricItem(instance, "Repetition")
        {}

    protected:
        Repetition(int64_t instance, const char* checkClassName)
            : GeometricItem(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Repetition
       //

        ///<summary>Sets value of count</summary>
        bool set_count(int64_t value) { return SetDatatypeProperty ("count", &value, 1); }
        ///<summary>Gets a value of count, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_count() { return GetDatatypeProperty<int64_t>("count", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        bool set_matrix(const Matrix& instance) { return SetObjectProperty<Matrix>("matrix", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Matrix* get_matrix() { return GetObjectProperty<Matrix>("matrix", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        bool set_object(const GeometricItem& instance) { return SetObjectProperty<GeometricItem>("object", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const GeometricItem* get_object() { return GetObjectProperty<GeometricItem>("object", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class RingTorus
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class RingTorus : public Torus
    {
    public:
        /// <summary>
        /// Create new instace of OWL class RingTorus and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static RingTorus Create(int64_t model, const char* name=NULL) { return RingTorus(Instance::Create(model, "RingTorus", name, NULL), "RingTorus");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static RingTorus CreateW(int64_t model, const wchar_t* name = NULL) { return RingTorus(Instance::Create(model, "RingTorus", NULL, name), "RingTorus"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        RingTorus(int64_t instance = NULL)
            : Torus(instance, "RingTorus")
        {}

    protected:
        RingTorus(int64_t instance, const char* checkClassName)
            : Torus(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Shadow
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Shadow : public Face
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Shadow and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Shadow Create(int64_t model, const char* name=NULL) { return Shadow(Instance::Create(model, "Shadow", name, NULL), "Shadow");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Shadow CreateW(int64_t model, const wchar_t* name = NULL) { return Shadow(Instance::Create(model, "Shadow", NULL, name), "Shadow"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Shadow(int64_t instance = NULL)
            : Face(instance, "Shadow")
        {}

    protected:
        Shadow(int64_t instance, const char* checkClassName)
            : Face(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Shadow
       //

        ///<summary>Sets relationship from this instance to an instance of Vector3</summary>
        bool set_lightDirection(const Vector3& instance) { return SetObjectProperty<Vector3>("lightDirection", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Vector3* get_lightDirection() { return GetObjectProperty<Vector3>("lightDirection", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Point3D</summary>
        bool set_lightPoint(const Point3D& instance) { return SetObjectProperty<Point3D>("lightPoint", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Point3D* get_lightPoint() { return GetObjectProperty<Point3D>("lightPoint", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        bool set_object(const GeometricItem& instance) { return SetObjectProperty<GeometricItem>("object", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const GeometricItem* get_object() { return GetObjectProperty<GeometricItem>("object", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Plane</summary>
        bool set_plane(const Plane& instance) { return SetObjectProperty<Plane>("plane", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Plane* get_plane() { return GetObjectProperty<Plane>("plane", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Vector3</summary>
        bool set_planeRefDirection(const Vector3& instance) { return SetObjectProperty<Vector3>("planeRefDirection", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Vector3* get_planeRefDirection() { return GetObjectProperty<Vector3>("planeRefDirection", NULL); }
        ///<summary>Sets value of type</summary>
        bool set_type(int64_t value) { return SetDatatypeProperty ("type", &value, 1); }
        ///<summary>Gets a value of type, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_type() { return GetDatatypeProperty<int64_t>("type", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SineCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SineCurve : public TransitionalCurve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SineCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SineCurve Create(int64_t model, const char* name=NULL) { return SineCurve(Instance::Create(model, "SineCurve", name, NULL), "SineCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SineCurve CreateW(int64_t model, const wchar_t* name = NULL) { return SineCurve(Instance::Create(model, "SineCurve", NULL, name), "SineCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SineCurve(int64_t instance = NULL)
            : TransitionalCurve(instance, "SineCurve")
        {}

    protected:
        SineCurve(int64_t instance, const char* checkClassName)
            : TransitionalCurve(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SkewedCone
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SkewedCone : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SkewedCone and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SkewedCone Create(int64_t model, const char* name=NULL) { return SkewedCone(Instance::Create(model, "SkewedCone", name, NULL), "SkewedCone");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SkewedCone CreateW(int64_t model, const wchar_t* name = NULL) { return SkewedCone(Instance::Create(model, "SkewedCone", NULL, name), "SkewedCone"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SkewedCone(int64_t instance = NULL)
            : Solid(instance, "SkewedCone")
        {}

    protected:
        SkewedCone(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SkewedCone
       //

        ///<summary>Sets value of height</summary>
        bool set_height(double value) { return SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets a value of height, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of offsetX</summary>
        bool set_offsetX(double value) { return SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets a value of offsetX, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of offsetY</summary>
        bool set_offsetY(double value) { return SetDatatypeProperty ("offsetY", &value, 1); }
        ///<summary>Gets a value of offsetY, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetY() { return GetDatatypeProperty<double>("offsetY", NULL); }
        ///<summary>Sets value of radius</summary>
        bool set_radius(double value) { return SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets a value of radius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SkewedFrustumCone
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SkewedFrustumCone : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SkewedFrustumCone and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SkewedFrustumCone Create(int64_t model, const char* name=NULL) { return SkewedFrustumCone(Instance::Create(model, "SkewedFrustumCone", name, NULL), "SkewedFrustumCone");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SkewedFrustumCone CreateW(int64_t model, const wchar_t* name = NULL) { return SkewedFrustumCone(Instance::Create(model, "SkewedFrustumCone", NULL, name), "SkewedFrustumCone"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SkewedFrustumCone(int64_t instance = NULL)
            : Solid(instance, "SkewedFrustumCone")
        {}

    protected:
        SkewedFrustumCone(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SkewedFrustumCone
       //

        ///<summary>Sets value of factor</summary>
        bool set_factor(double value) { return SetDatatypeProperty ("factor", &value, 1); }
        ///<summary>Gets a value of factor, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_factor() { return GetDatatypeProperty<double>("factor", NULL); }
        ///<summary>Sets value of height</summary>
        bool set_height(double value) { return SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets a value of height, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of offsetX</summary>
        bool set_offsetX(double value) { return SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets a value of offsetX, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of offsetY</summary>
        bool set_offsetY(double value) { return SetDatatypeProperty ("offsetY", &value, 1); }
        ///<summary>Gets a value of offsetY, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetY() { return GetDatatypeProperty<double>("offsetY", NULL); }
        ///<summary>Sets value of radius</summary>
        bool set_radius(double value) { return SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets a value of radius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SolidBySurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SolidBySurface : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SolidBySurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SolidBySurface Create(int64_t model, const char* name=NULL) { return SolidBySurface(Instance::Create(model, "SolidBySurface", name, NULL), "SolidBySurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SolidBySurface CreateW(int64_t model, const wchar_t* name = NULL) { return SolidBySurface(Instance::Create(model, "SolidBySurface", NULL, name), "SolidBySurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SolidBySurface(int64_t instance = NULL)
            : Solid(instance, "SolidBySurface")
        {}

    protected:
        SolidBySurface(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SolidBySurface
       //

        ///<summary>Sets relationships from this instance to an array of Face. OWL cardinality 1..-1</summary>
        bool set_faces(const Face* instances, int64_t count) { return SetObjectProperty<Face>("faces", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        bool set_faces(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("faces", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Face* get_faces(int64_t* pCount) { return GetObjectProperty<Face>("faces", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_faces_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("faces", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SolidLine
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SolidLine : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SolidLine and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SolidLine Create(int64_t model, const char* name=NULL) { return SolidLine(Instance::Create(model, "SolidLine", name, NULL), "SolidLine");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SolidLine CreateW(int64_t model, const wchar_t* name = NULL) { return SolidLine(Instance::Create(model, "SolidLine", NULL, name), "SolidLine"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SolidLine(int64_t instance = NULL)
            : Solid(instance, "SolidLine")
        {}

    protected:
        SolidLine(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SolidLine
       //

        ///<summary>Sets values of coordinates. OWL cardinality 0..6</summary>
        bool set_coordinates(const double* values, int64_t count) { return SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Sets values of coordinates from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..6</summary>
        template <typename TList> bool set_coordinates(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_coordinates(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of coordinates. OWL cardinality 0..6. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
        ///<summary>Sets value of thickness</summary>
        bool set_thickness(double value) { return SetDatatypeProperty ("thickness", &value, 1); }
        ///<summary>Gets a value of thickness, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_thickness() { return GetDatatypeProperty<double>("thickness", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Sphere
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Sphere : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Sphere and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Sphere Create(int64_t model, const char* name=NULL) { return Sphere(Instance::Create(model, "Sphere", name, NULL), "Sphere");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Sphere CreateW(int64_t model, const wchar_t* name = NULL) { return Sphere(Instance::Create(model, "Sphere", NULL, name), "Sphere"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Sphere(int64_t instance = NULL)
            : Solid(instance, "Sphere")
        {}

    protected:
        Sphere(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Sphere
       //

        ///<summary>Sets value of radius</summary>
        bool set_radius(double value) { return SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets a value of radius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SphericalSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SphericalSurface : public FiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SphericalSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SphericalSurface Create(int64_t model, const char* name=NULL) { return SphericalSurface(Instance::Create(model, "SphericalSurface", name, NULL), "SphericalSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SphericalSurface CreateW(int64_t model, const wchar_t* name = NULL) { return SphericalSurface(Instance::Create(model, "SphericalSurface", NULL, name), "SphericalSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SphericalSurface(int64_t instance = NULL)
            : FiniteSurface(instance, "SphericalSurface")
        {}

    protected:
        SphericalSurface(int64_t instance, const char* checkClassName)
            : FiniteSurface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SphericalSurface
       //

        ///<summary>Sets value of invert</summary>
        bool set_invert(bool value) { return SetDatatypeProperty ("invert", &value, 1); }
        ///<summary>Gets a value of invert, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_invert() { return GetDatatypeProperty<bool>("invert", NULL); }
        ///<summary>Sets value of radius</summary>
        bool set_radius(double value) { return SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets a value of radius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SpindleTorus
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SpindleTorus : public Torus
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SpindleTorus and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SpindleTorus Create(int64_t model, const char* name=NULL) { return SpindleTorus(Instance::Create(model, "SpindleTorus", name, NULL), "SpindleTorus");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SpindleTorus CreateW(int64_t model, const wchar_t* name = NULL) { return SpindleTorus(Instance::Create(model, "SpindleTorus", NULL, name), "SpindleTorus"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SpindleTorus(int64_t instance = NULL)
            : Torus(instance, "SpindleTorus")
        {}

    protected:
        SpindleTorus(int64_t instance, const char* checkClassName)
            : Torus(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Spiral
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Spiral : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Spiral and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Spiral Create(int64_t model, const char* name=NULL) { return Spiral(Instance::Create(model, "Spiral", name, NULL), "Spiral");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Spiral CreateW(int64_t model, const wchar_t* name = NULL) { return Spiral(Instance::Create(model, "Spiral", NULL, name), "Spiral"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Spiral(int64_t instance = NULL)
            : Curve(instance, "Spiral")
        {}

    protected:
        Spiral(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Spiral
       //

        ///<summary>Sets value of height</summary>
        bool set_height(double value) { return SetDatatypeProperty ("height", &value, 1); }
        ///<summary>Gets a value of height, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_height() { return GetDatatypeProperty<double>("height", NULL); }
        ///<summary>Sets value of offsetZ</summary>
        bool set_offsetZ(double value) { return SetDatatypeProperty ("offsetZ", &value, 1); }
        ///<summary>Gets a value of offsetZ, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetZ() { return GetDatatypeProperty<double>("offsetZ", NULL); }
        ///<summary>Sets value of radius</summary>
        bool set_radius(double value) { return SetDatatypeProperty ("radius", &value, 1); }
        ///<summary>Gets a value of radius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_radius() { return GetDatatypeProperty<double>("radius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
        ///<summary>Sets value of size</summary>
        bool set_size(double value) { return SetDatatypeProperty ("size", &value, 1); }
        ///<summary>Gets a value of size, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_size() { return GetDatatypeProperty<double>("size", NULL); }
        ///<summary>Sets value of start</summary>
        bool set_start(double value) { return SetDatatypeProperty ("start", &value, 1); }
        ///<summary>Gets a value of start, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_start() { return GetDatatypeProperty<double>("start", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SpotLight
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SpotLight : public DirectLight
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SpotLight and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SpotLight Create(int64_t model, const char* name=NULL) { return SpotLight(Instance::Create(model, "SpotLight", name, NULL), "SpotLight");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SpotLight CreateW(int64_t model, const wchar_t* name = NULL) { return SpotLight(Instance::Create(model, "SpotLight", NULL, name), "SpotLight"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SpotLight(int64_t instance = NULL)
            : DirectLight(instance, "SpotLight")
        {}

    protected:
        SpotLight(int64_t instance, const char* checkClassName)
            : DirectLight(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SurfaceBySweptCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SurfaceBySweptCurve : public FiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SurfaceBySweptCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SurfaceBySweptCurve Create(int64_t model, const char* name=NULL) { return SurfaceBySweptCurve(Instance::Create(model, "SurfaceBySweptCurve", name, NULL), "SurfaceBySweptCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SurfaceBySweptCurve CreateW(int64_t model, const wchar_t* name = NULL) { return SurfaceBySweptCurve(Instance::Create(model, "SurfaceBySweptCurve", NULL, name), "SurfaceBySweptCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SurfaceBySweptCurve(int64_t instance = NULL)
            : FiniteSurface(instance, "SurfaceBySweptCurve")
        {}

    protected:
        SurfaceBySweptCurve(int64_t instance, const char* checkClassName)
            : FiniteSurface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SurfaceBySweptCurve
       //

        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_path(const Curve& instance) { return SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_sweptArea(const Curve& instance) { return SetObjectProperty<Curve>("sweptArea", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_sweptArea() { return GetObjectProperty<Curve>("sweptArea", NULL); }
        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        bool set_sweptAreaOpenings(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("sweptAreaOpenings", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        bool set_sweptAreaOpenings(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("sweptAreaOpenings", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_sweptAreaOpenings(int64_t* pCount) { return GetObjectProperty<Curve>("sweptAreaOpenings", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_sweptAreaOpenings_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("sweptAreaOpenings", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SurfaceOfLinearExtrusion
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SurfaceOfLinearExtrusion : public FiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SurfaceOfLinearExtrusion and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SurfaceOfLinearExtrusion Create(int64_t model, const char* name=NULL) { return SurfaceOfLinearExtrusion(Instance::Create(model, "SurfaceOfLinearExtrusion", name, NULL), "SurfaceOfLinearExtrusion");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SurfaceOfLinearExtrusion CreateW(int64_t model, const wchar_t* name = NULL) { return SurfaceOfLinearExtrusion(Instance::Create(model, "SurfaceOfLinearExtrusion", NULL, name), "SurfaceOfLinearExtrusion"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SurfaceOfLinearExtrusion(int64_t instance = NULL)
            : FiniteSurface(instance, "SurfaceOfLinearExtrusion")
        {}

    protected:
        SurfaceOfLinearExtrusion(int64_t instance, const char* checkClassName)
            : FiniteSurface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SurfaceOfLinearExtrusion
       //

        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        bool set_extrusion(const Vector& instance) { return SetObjectProperty<Vector>("extrusion", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Vector* get_extrusion() { return GetObjectProperty<Vector>("extrusion", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_polygon(const Curve& instance) { return SetObjectProperty<Curve>("polygon", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_polygon() { return GetObjectProperty<Curve>("polygon", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SurfaceOfRevolution
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SurfaceOfRevolution : public Surface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SurfaceOfRevolution and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SurfaceOfRevolution Create(int64_t model, const char* name=NULL) { return SurfaceOfRevolution(Instance::Create(model, "SurfaceOfRevolution", name, NULL), "SurfaceOfRevolution");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SurfaceOfRevolution CreateW(int64_t model, const wchar_t* name = NULL) { return SurfaceOfRevolution(Instance::Create(model, "SurfaceOfRevolution", NULL, name), "SurfaceOfRevolution"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SurfaceOfRevolution(int64_t instance = NULL)
            : Surface(instance, "SurfaceOfRevolution")
        {}

    protected:
        SurfaceOfRevolution(int64_t instance, const char* checkClassName)
            : Surface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SurfaceOfRevolution
       //

        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_path(const Curve& instance) { return SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SweptAreaSolid
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SweptAreaSolid : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SweptAreaSolid and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SweptAreaSolid Create(int64_t model, const char* name=NULL) { return SweptAreaSolid(Instance::Create(model, "SweptAreaSolid", name, NULL), "SweptAreaSolid");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SweptAreaSolid CreateW(int64_t model, const wchar_t* name = NULL) { return SweptAreaSolid(Instance::Create(model, "SweptAreaSolid", NULL, name), "SweptAreaSolid"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SweptAreaSolid(int64_t instance = NULL)
            : Solid(instance, "SweptAreaSolid")
        {}

    protected:
        SweptAreaSolid(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SweptAreaSolid
       //

        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        bool set_direction(const Vector& instance) { return SetObjectProperty<Vector>("direction", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Vector* get_direction() { return GetObjectProperty<Vector>("direction", NULL); }
        ///<summary>Sets value of fraction</summary>
        bool set_fraction(double value) { return SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets a value of fraction, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_path(const Curve& instance) { return SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_sweptArea(const Curve& instance) { return SetObjectProperty<Curve>("sweptArea", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_sweptArea() { return GetObjectProperty<Curve>("sweptArea", NULL); }
        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        bool set_sweptAreaOpenings(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("sweptAreaOpenings", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        bool set_sweptAreaOpenings(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("sweptAreaOpenings", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_sweptAreaOpenings(int64_t* pCount) { return GetObjectProperty<Curve>("sweptAreaOpenings", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_sweptAreaOpenings_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("sweptAreaOpenings", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SweptAreaSolidSet
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SweptAreaSolidSet : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SweptAreaSolidSet and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SweptAreaSolidSet Create(int64_t model, const char* name=NULL) { return SweptAreaSolidSet(Instance::Create(model, "SweptAreaSolidSet", name, NULL), "SweptAreaSolidSet");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SweptAreaSolidSet CreateW(int64_t model, const wchar_t* name = NULL) { return SweptAreaSolidSet(Instance::Create(model, "SweptAreaSolidSet", NULL, name), "SweptAreaSolidSet"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SweptAreaSolidSet(int64_t instance = NULL)
            : Solid(instance, "SweptAreaSolidSet")
        {}

    protected:
        SweptAreaSolidSet(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SweptAreaSolidSet
       //

        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        bool set_direction(const Vector& instance) { return SetObjectProperty<Vector>("direction", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Vector* get_direction() { return GetObjectProperty<Vector>("direction", NULL); }
        ///<summary>Sets value of fraction</summary>
        bool set_fraction(double value) { return SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets a value of fraction, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_path(const Curve& instance) { return SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 1..-1</summary>
        bool set_sweptAreaSet(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("sweptAreaSet", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 1..-1</summary>
        bool set_sweptAreaSet(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("sweptAreaSet", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_sweptAreaSet(int64_t* pCount) { return GetObjectProperty<Curve>("sweptAreaSet", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 1..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_sweptAreaSet_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("sweptAreaSet", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SweptAreaSolidTapered
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SweptAreaSolidTapered : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SweptAreaSolidTapered and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SweptAreaSolidTapered Create(int64_t model, const char* name=NULL) { return SweptAreaSolidTapered(Instance::Create(model, "SweptAreaSolidTapered", name, NULL), "SweptAreaSolidTapered");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SweptAreaSolidTapered CreateW(int64_t model, const wchar_t* name = NULL) { return SweptAreaSolidTapered(Instance::Create(model, "SweptAreaSolidTapered", NULL, name), "SweptAreaSolidTapered"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SweptAreaSolidTapered(int64_t instance = NULL)
            : Solid(instance, "SweptAreaSolidTapered")
        {}

    protected:
        SweptAreaSolidTapered(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SweptAreaSolidTapered
       //

        ///<summary>Sets relationship from this instance to an instance of Vector</summary>
        bool set_direction(const Vector& instance) { return SetObjectProperty<Vector>("direction", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Vector* get_direction() { return GetObjectProperty<Vector>("direction", NULL); }
        ///<summary>Sets value of fraction</summary>
        bool set_fraction(double value) { return SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets a value of fraction, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_path(const Curve& instance) { return SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_sweptArea(const Curve& instance) { return SetObjectProperty<Curve>("sweptArea", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_sweptArea() { return GetObjectProperty<Curve>("sweptArea", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_sweptAreaEnd(const Curve& instance) { return SetObjectProperty<Curve>("sweptAreaEnd", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_sweptAreaEnd() { return GetObjectProperty<Curve>("sweptAreaEnd", NULL); }
        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        bool set_sweptAreaEndOpenings(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("sweptAreaEndOpenings", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        bool set_sweptAreaEndOpenings(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("sweptAreaEndOpenings", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_sweptAreaEndOpenings(int64_t* pCount) { return GetObjectProperty<Curve>("sweptAreaEndOpenings", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_sweptAreaEndOpenings_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("sweptAreaEndOpenings", pCount); }
        ///<summary>Sets relationships from this instance to an array of Curve. OWL cardinality 0..-1</summary>
        bool set_sweptAreaOpenings(const Curve* instances, int64_t count) { return SetObjectProperty<Curve>("sweptAreaOpenings", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..-1</summary>
        bool set_sweptAreaOpenings(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("sweptAreaOpenings", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Curve* get_sweptAreaOpenings(int64_t* pCount) { return GetObjectProperty<Curve>("sweptAreaOpenings", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_sweptAreaOpenings_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("sweptAreaOpenings", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class SweptBlend
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class SweptBlend : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class SweptBlend and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SweptBlend Create(int64_t model, const char* name=NULL) { return SweptBlend(Instance::Create(model, "SweptBlend", name, NULL), "SweptBlend");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static SweptBlend CreateW(int64_t model, const wchar_t* name = NULL) { return SweptBlend(Instance::Create(model, "SweptBlend", NULL, name), "SweptBlend"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        SweptBlend(int64_t instance = NULL)
            : Solid(instance, "SweptBlend")
        {}

    protected:
        SweptBlend(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to SweptBlend
       //

        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_bottomPolygon(const Curve& instance) { return SetObjectProperty<Curve>("bottomPolygon", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_bottomPolygon() { return GetObjectProperty<Curve>("bottomPolygon", NULL); }
        ///<summary>Sets values of connectionMap. OWL cardinality 0..-1</summary>
        bool set_connectionMap(const int64_t* values, int64_t count) { return SetDatatypeProperty ("connectionMap", values, count); }
        ///<summary>Sets values of connectionMap from enumerable collection (std::vector, std::list, etc.) of elements convertible to int64_t. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_connectionMap(const TList& values) { int64_t* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_connectionMap(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of connectionMap. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_connectionMap(int64_t* pCount) { return GetDatatypeProperty<int64_t>("connectionMap", pCount); }
        ///<summary>Sets values of forcedStaticDirection. OWL cardinality 0..3</summary>
        bool set_forcedStaticDirection(const double* values, int64_t count) { return SetDatatypeProperty ("forcedStaticDirection", values, count); }
        ///<summary>Sets values of forcedStaticDirection from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..3</summary>
        template <typename TList> bool set_forcedStaticDirection(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_forcedStaticDirection(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of forcedStaticDirection. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_forcedStaticDirection(int64_t* pCount) { return GetDatatypeProperty<double>("forcedStaticDirection", pCount); }
        ///<summary>Sets value of forceSolid</summary>
        bool set_forceSolid(bool value) { return SetDatatypeProperty ("forceSolid", &value, 1); }
        ///<summary>Gets a value of forceSolid, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_forceSolid() { return GetDatatypeProperty<bool>("forceSolid", NULL); }
        ///<summary>Sets value of fraction</summary>
        bool set_fraction(double value) { return SetDatatypeProperty ("fraction", &value, 1); }
        ///<summary>Gets a value of fraction, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_fraction() { return GetDatatypeProperty<double>("fraction", NULL); }
        ///<summary>Sets value of hasBottom</summary>
        bool set_hasBottom(bool value) { return SetDatatypeProperty ("hasBottom", &value, 1); }
        ///<summary>Gets a value of hasBottom, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_hasBottom() { return GetDatatypeProperty<bool>("hasBottom", NULL); }
        ///<summary>Sets value of hasTop</summary>
        bool set_hasTop(bool value) { return SetDatatypeProperty ("hasTop", &value, 1); }
        ///<summary>Gets a value of hasTop, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_hasTop() { return GetDatatypeProperty<bool>("hasTop", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_path(const Curve& instance) { return SetObjectProperty<Curve>("path", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_path() { return GetObjectProperty<Curve>("path", NULL); }
        ///<summary>Sets relationship from this instance to an instance of Curve</summary>
        bool set_topPolygon(const Curve& instance) { return SetObjectProperty<Curve>("topPolygon", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Curve* get_topPolygon() { return GetObjectProperty<Curve>("topPolygon", NULL); }
        ///<summary>Sets value of usesAbsolutePlacement</summary>
        bool set_usesAbsolutePlacement(bool value) { return SetDatatypeProperty ("usesAbsolutePlacement", &value, 1); }
        ///<summary>Gets a value of usesAbsolutePlacement, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const bool* get_usesAbsolutePlacement() { return GetDatatypeProperty<bool>("usesAbsolutePlacement", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Texture
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Texture : public Appearance
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Texture and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Texture Create(int64_t model, const char* name=NULL) { return Texture(Instance::Create(model, "Texture", name, NULL), "Texture");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Texture CreateW(int64_t model, const wchar_t* name = NULL) { return Texture(Instance::Create(model, "Texture", NULL, name), "Texture"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Texture(int64_t instance = NULL)
            : Appearance(instance, "Texture")
        {}

    protected:
        Texture(int64_t instance, const char* checkClassName)
            : Appearance(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Texture
       //

        ///<summary>Sets value of name</summary>
        bool set_name(const char* const value) { return SetDatatypeProperty ("name", &value, 1); }
        ///<summary>Gets a value of name, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const char* const* get_name() { return GetDatatypeProperty<const char* const>("name", NULL); }
        ///<summary>Sets value of offsetX</summary>
        bool set_offsetX(double value) { return SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets a value of offsetX, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of offsetY</summary>
        bool set_offsetY(double value) { return SetDatatypeProperty ("offsetY", &value, 1); }
        ///<summary>Gets a value of offsetY, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetY() { return GetDatatypeProperty<double>("offsetY", NULL); }
        ///<summary>Sets values of origin. OWL cardinality 0..3</summary>
        bool set_origin(const double* values, int64_t count) { return SetDatatypeProperty ("origin", values, count); }
        ///<summary>Sets values of origin from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..3</summary>
        template <typename TList> bool set_origin(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_origin(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of origin. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_origin(int64_t* pCount) { return GetDatatypeProperty<double>("origin", pCount); }
        ///<summary>Sets value of rotation</summary>
        bool set_rotation(double value) { return SetDatatypeProperty ("rotation", &value, 1); }
        ///<summary>Gets a value of rotation, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_rotation() { return GetDatatypeProperty<double>("rotation", NULL); }
        ///<summary>Sets value of scalingX</summary>
        bool set_scalingX(double value) { return SetDatatypeProperty ("scalingX", &value, 1); }
        ///<summary>Gets a value of scalingX, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_scalingX() { return GetDatatypeProperty<double>("scalingX", NULL); }
        ///<summary>Sets value of scalingY</summary>
        bool set_scalingY(double value) { return SetDatatypeProperty ("scalingY", &value, 1); }
        ///<summary>Gets a value of scalingY, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_scalingY() { return GetDatatypeProperty<double>("scalingY", NULL); }
        ///<summary>Sets value of type</summary>
        bool set_type(int64_t value) { return SetDatatypeProperty ("type", &value, 1); }
        ///<summary>Gets a value of type, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_type() { return GetDatatypeProperty<int64_t>("type", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class ToroidalSurface
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class ToroidalSurface : public FiniteSurface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class ToroidalSurface and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ToroidalSurface Create(int64_t model, const char* name=NULL) { return ToroidalSurface(Instance::Create(model, "ToroidalSurface", name, NULL), "ToroidalSurface");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static ToroidalSurface CreateW(int64_t model, const wchar_t* name = NULL) { return ToroidalSurface(Instance::Create(model, "ToroidalSurface", NULL, name), "ToroidalSurface"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        ToroidalSurface(int64_t instance = NULL)
            : FiniteSurface(instance, "ToroidalSurface")
        {}

    protected:
        ToroidalSurface(int64_t instance, const char* checkClassName)
            : FiniteSurface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to ToroidalSurface
       //

        ///<summary>Sets value of majorRadius</summary>
        bool set_majorRadius(double value) { return SetDatatypeProperty ("majorRadius", &value, 1); }
        ///<summary>Gets a value of majorRadius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_majorRadius() { return GetDatatypeProperty<double>("majorRadius", NULL); }
        ///<summary>Sets value of minorRadius</summary>
        bool set_minorRadius(double value) { return SetDatatypeProperty ("minorRadius", &value, 1); }
        ///<summary>Gets a value of minorRadius, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_minorRadius() { return GetDatatypeProperty<double>("minorRadius", NULL); }
        ///<summary>Sets value of segmentationParts</summary>
        bool set_segmentationParts(int64_t value) { return SetDatatypeProperty ("segmentationParts", &value, 1); }
        ///<summary>Gets a value of segmentationParts, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_segmentationParts() { return GetDatatypeProperty<int64_t>("segmentationParts", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Transformation
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Transformation : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Transformation and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Transformation Create(int64_t model, const char* name=NULL) { return Transformation(Instance::Create(model, "Transformation", name, NULL), "Transformation");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Transformation CreateW(int64_t model, const wchar_t* name = NULL) { return Transformation(Instance::Create(model, "Transformation", NULL, name), "Transformation"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Transformation(int64_t instance = NULL)
            : Curve(instance, "Transformation")
        {}

    protected:
        Transformation(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Transformation
       //

        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        bool set_matrix(const Matrix& instance) { return SetObjectProperty<Matrix>("matrix", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Matrix* get_matrix() { return GetObjectProperty<Matrix>("matrix", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        bool set_object(const GeometricItem& instance) { return SetObjectProperty<GeometricItem>("object", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const GeometricItem* get_object() { return GetObjectProperty<GeometricItem>("object", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class TriangleCurve
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class TriangleCurve : public Curve
    {
    public:
        /// <summary>
        /// Create new instace of OWL class TriangleCurve and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static TriangleCurve Create(int64_t model, const char* name=NULL) { return TriangleCurve(Instance::Create(model, "TriangleCurve", name, NULL), "TriangleCurve");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static TriangleCurve CreateW(int64_t model, const wchar_t* name = NULL) { return TriangleCurve(Instance::Create(model, "TriangleCurve", NULL, name), "TriangleCurve"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        TriangleCurve(int64_t instance = NULL)
            : Curve(instance, "TriangleCurve")
        {}

    protected:
        TriangleCurve(int64_t instance, const char* checkClassName)
            : Curve(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to TriangleCurve
       //

        ///<summary>Sets values of coordinates. OWL cardinality 0..9</summary>
        bool set_coordinates(const double* values, int64_t count) { return SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Sets values of coordinates from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..9</summary>
        template <typename TList> bool set_coordinates(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_coordinates(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of coordinates. OWL cardinality 0..9. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets value of offsetX</summary>
        bool set_offsetX(double value) { return SetDatatypeProperty ("offsetX", &value, 1); }
        ///<summary>Gets a value of offsetX, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetX() { return GetDatatypeProperty<double>("offsetX", NULL); }
        ///<summary>Sets value of offsetY</summary>
        bool set_offsetY(double value) { return SetDatatypeProperty ("offsetY", &value, 1); }
        ///<summary>Gets a value of offsetY, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetY() { return GetDatatypeProperty<double>("offsetY", NULL); }
        ///<summary>Sets value of offsetZ</summary>
        bool set_offsetZ(double value) { return SetDatatypeProperty ("offsetZ", &value, 1); }
        ///<summary>Gets a value of offsetZ, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_offsetZ() { return GetDatatypeProperty<double>("offsetZ", NULL); }
        ///<summary>Sets relationships from this instance to an array of Point3D. OWL cardinality 0..3</summary>
        bool set_pointReferences(const Point3D* instances, int64_t count) { return SetObjectProperty<Point3D>("pointReferences", instances, count); }
        ///<summary>Sets relationships from this instance to an array of int64_t. OWL cardinality 0..3</summary>
        bool set_pointReferences(const int64_t* instances, int64_t count) { return SetObjectProperty<int64_t>("pointReferences", instances, count); }
        ///<summary>Get an array of related instances. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const Point3D* get_pointReferences(int64_t* pCount) { return GetObjectProperty<Point3D>("pointReferences", pCount); }
        ///<summary>Get an array of related instance handles. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_pointReferences_int64(int64_t* pCount) { return GetObjectProperty<int64_t>("pointReferences", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class TriangleReduction
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class TriangleReduction : public Solid
    {
    public:
        /// <summary>
        /// Create new instace of OWL class TriangleReduction and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static TriangleReduction Create(int64_t model, const char* name=NULL) { return TriangleReduction(Instance::Create(model, "TriangleReduction", name, NULL), "TriangleReduction");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static TriangleReduction CreateW(int64_t model, const wchar_t* name = NULL) { return TriangleReduction(Instance::Create(model, "TriangleReduction", NULL, name), "TriangleReduction"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        TriangleReduction(int64_t instance = NULL)
            : Solid(instance, "TriangleReduction")
        {}

    protected:
        TriangleReduction(int64_t instance, const char* checkClassName)
            : Solid(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to TriangleReduction
       //

        ///<summary>Sets value of innerFraction</summary>
        bool set_innerFraction(double value) { return SetDatatypeProperty ("innerFraction", &value, 1); }
        ///<summary>Gets a value of innerFraction, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_innerFraction() { return GetDatatypeProperty<double>("innerFraction", NULL); }
        ///<summary>Sets relationship from this instance to an instance of GeometricItem</summary>
        bool set_object(const GeometricItem& instance) { return SetObjectProperty<GeometricItem>("object", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const GeometricItem* get_object() { return GetObjectProperty<GeometricItem>("object", NULL); }
        ///<summary>Sets value of outerFraction</summary>
        bool set_outerFraction(double value) { return SetDatatypeProperty ("outerFraction", &value, 1); }
        ///<summary>Gets a value of outerFraction, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_outerFraction() { return GetDatatypeProperty<double>("outerFraction", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class TriangleSet
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class TriangleSet : public Surface
    {
    public:
        /// <summary>
        /// Create new instace of OWL class TriangleSet and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static TriangleSet Create(int64_t model, const char* name=NULL) { return TriangleSet(Instance::Create(model, "TriangleSet", name, NULL), "TriangleSet");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static TriangleSet CreateW(int64_t model, const wchar_t* name = NULL) { return TriangleSet(Instance::Create(model, "TriangleSet", NULL, name), "TriangleSet"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        TriangleSet(int64_t instance = NULL)
            : Surface(instance, "TriangleSet")
        {}

    protected:
        TriangleSet(int64_t instance, const char* checkClassName)
            : Surface(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to TriangleSet
       //

        ///<summary>Sets values of indices. OWL cardinality 0..-1</summary>
        bool set_indices(const int64_t* values, int64_t count) { return SetDatatypeProperty ("indices", values, count); }
        ///<summary>Sets values of indices from enumerable collection (std::vector, std::list, etc.) of elements convertible to int64_t. OWL cardinality 0..-1</summary>
        template <typename TList> bool set_indices(const TList& values) { int64_t* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_indices(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of indices. OWL cardinality 0..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const int64_t* get_indices(int64_t* pCount) { return GetDatatypeProperty<int64_t>("indices", pCount); }
        ///<summary>Sets values of vertices. OWL cardinality 3..-1</summary>
        bool set_vertices(const double* values, int64_t count) { return SetDatatypeProperty ("vertices", values, count); }
        ///<summary>Sets values of vertices from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 3..-1</summary>
        template <typename TList> bool set_vertices(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_vertices(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of vertices. OWL cardinality 3..-1. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_vertices(int64_t* pCount) { return GetDatatypeProperty<double>("vertices", pCount); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Vector
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Vector : public Mathematics
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Vector and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Vector Create(int64_t model, const char* name=NULL) { return Vector(Instance::Create(model, "Vector", name, NULL), "Vector");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Vector CreateW(int64_t model, const wchar_t* name = NULL) { return Vector(Instance::Create(model, "Vector", NULL, name), "Vector"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Vector(int64_t instance = NULL)
            : Mathematics(instance, "Vector")
        {}

    protected:
        Vector(int64_t instance, const char* checkClassName)
            : Mathematics(instance, checkClassName)
        {}
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class Vector3
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class Vector3 : public Vector
    {
    public:
        /// <summary>
        /// Create new instace of OWL class Vector3 and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Vector3 Create(int64_t model, const char* name=NULL) { return Vector3(Instance::Create(model, "Vector3", name, NULL), "Vector3");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static Vector3 CreateW(int64_t model, const wchar_t* name = NULL) { return Vector3(Instance::Create(model, "Vector3", NULL, name), "Vector3"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        Vector3(int64_t instance = NULL)
            : Vector(instance, "Vector3")
        {}

    protected:
        Vector3(int64_t instance, const char* checkClassName)
            : Vector(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to Vector3
       //

        ///<summary>Sets values of coordinates. OWL cardinality 0..3</summary>
        bool set_coordinates(const double* values, int64_t count) { return SetDatatypeProperty ("coordinates", values, count); }
        ///<summary>Sets values of coordinates from enumerable collection (std::vector, std::list, etc.) of elements convertible to double. OWL cardinality 0..3</summary>
        template <typename TList> bool set_coordinates(const TList& values) { double* arr = NULL; int64_t count = 0; ListToArray(values, &arr, &count); bool ok = set_coordinates(arr, count); if (arr) delete[] arr; return ok; }
        ///<summary>Gets values array of coordinates. OWL cardinality 0..3. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_coordinates(int64_t* pCount) { return GetDatatypeProperty<double>("coordinates", pCount); }
        ///<summary>Sets value of x</summary>
        bool set_x(double value) { return SetDatatypeProperty ("x", &value, 1); }
        ///<summary>Gets a value of x, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_x() { return GetDatatypeProperty<double>("x", NULL); }
        ///<summary>Sets value of y</summary>
        bool set_y(double value) { return SetDatatypeProperty ("y", &value, 1); }
        ///<summary>Gets a value of y, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_y() { return GetDatatypeProperty<double>("y", NULL); }
        ///<summary>Sets value of z</summary>
        bool set_z(double value) { return SetDatatypeProperty ("z", &value, 1); }
        ///<summary>Gets a value of z, returns NULL is the property was not set. The method returns pointer to inernal buffer, a caller should not free or change it.</summary>
        const double* get_z() { return GetDatatypeProperty<double>("z", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class View
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class View : public Environment
    {
    public:
        /// <summary>
        /// Create new instace of OWL class View and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static View Create(int64_t model, const char* name=NULL) { return View(Instance::Create(model, "View", name, NULL), "View");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static View CreateW(int64_t model, const wchar_t* name = NULL) { return View(Instance::Create(model, "View", NULL, name), "View"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        View(int64_t instance = NULL)
            : Environment(instance, "View")
        {}

    protected:
        View(int64_t instance, const char* checkClassName)
            : Environment(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to View
       //

        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        bool set_matrix(const Matrix& instance) { return SetObjectProperty<Matrix>("matrix", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Matrix* get_matrix() { return GetObjectProperty<Matrix>("matrix", NULL); }
    };

    /// <summary>
    /// Provides utility methods to interact with an instance of OWL class World
    /// You also can use object of this C++ class instead of int64_t handle of the OWL instance in any place where the handle is required
    /// </summary>
    class World : public Environment
    {
    public:
        /// <summary>
        /// Create new instace of OWL class World and returns object of this C++ class to interact with
        /// </summary>
        /// <param name="model">The handle to the model</param>
        /// <param name="name">This attribute represents the name of the instance (given as char array / ASCII). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static World Create(int64_t model, const char* name=NULL) { return World(Instance::Create(model, "World", name, NULL), "World");}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="model"></param>
        /// <param name="name">This attribute represents the name of the instance (given as wchar_t array / Unicode). The name is given by the host and the attribute is not changed</param>
        /// <returns></returns>
        static World CreateW(int64_t model, const wchar_t* name = NULL) { return World(Instance::Create(model, "World", NULL, name), "World"); }

    public:
        /// <summary>
        /// Constructs object of this C++ class that wraps existing OWL instance
        /// </summary>
        /// <param name="instance">OWL instance to interact with</param>
        ///
        World(int64_t instance = NULL)
            : Environment(instance, "World")
        {}

    protected:
        World(int64_t instance, const char* checkClassName)
            : Environment(instance, checkClassName)
        {}

    public:
       //
       // Properties with known cardinality restrictions to World
       //

        ///<summary>Sets relationship from this instance to an instance of Matrix</summary>
        bool set_matrix(const Matrix& instance) { return SetObjectProperty<Matrix>("matrix", &instance, 1); }
        ///<summary>Get related instance. The method returns pointer to inernal buffer, a caller should not free or change it</summary>
        const Matrix* get_matrix() { return GetObjectProperty<Matrix>("matrix", NULL); }
    };
}

#endif
