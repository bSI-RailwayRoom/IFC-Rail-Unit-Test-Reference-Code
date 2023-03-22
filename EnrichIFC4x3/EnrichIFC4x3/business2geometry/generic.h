#pragma once


#include "engine.h"
#include "ifcengine.h"
#include "mathematics.h"

#include <assert.h>


static  const   uint64_t    __flagbit0 = 1;                           // 2^^0                          0000.0000..0000.0001
static  const   uint64_t    __flagbit1 = 2;                           // 2^^1                          0000.0000..0000.0010
static  const   uint64_t    __flagbit2 = 4;                           // 2^^2                          0000.0000..0000.0100
static  const   uint64_t    __flagbit3 = 8;                           // 2^^3                          0000.0000..0000.1000

static  const   uint64_t    __flagbit4 = 16;                          // 2^^4                          0000.0000..0001.0000
static  const   uint64_t    __flagbit5 = 32;                          // 2^^5                          0000.0000..0010.0000
static  const   uint64_t    __flagbit6 = 64;                          // 2^^6                          0000.0000..0100.0000
static  const   uint64_t    __flagbit7 = 128;                         // 2^^7                          0000.0000..1000.0000

static  const   uint64_t    __flagbit8 = 256;                         // 2^^8                          0000.0001..0000.0000
static  const   uint64_t    __flagbit9 = 512;                         // 2^^9                          0000.0010..0000.0000
static  const   uint64_t    __flagbit10 = 1024;                       // 2^^10                         0000.0100..0000.0000
static  const   uint64_t    __flagbit11 = 2048;                       // 2^^11                         0000.1000..0000.0000

static  const   uint64_t    __flagbit12 = 4096;                       // 2^^12                         0001.0000..0000.0000
static  const   uint64_t    __flagbit13 = 8192;                       // 2^^13                         0010.0000..0000.0000
static  const   uint64_t    __flagbit14 = 16384;                      // 2^^14                         0100.0000..0000.0000
static  const   uint64_t    __flagbit15 = 32768;                      // 2^^15                         1000.0000..0000.0000


static	inline	bool    ___equals(
                                const char    * txtI,
                                const char    * txtII
                            )
{
    if (txtI && txtII) {
        int_t   i = 0;
        
        while (txtI[i]) {
            if (txtI[i] != txtII[i]) {
                return  false;
            }
            i++;
        }

        if (txtII[i] == 0) {
            return  true;
        }
    }

    return  false;
}

static  inline  int_t   ___GetAlignmentSegments(
                                int_t   model,
                                int_t   ifcAlignmentXyzInstance,
                                int_t   * pAlignmentSegment
                            )
{
    int_t   alignmentSegmentCnt = 0;

    {
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentXyzInstance, "IsNestedBy", sdaiAGGR, &aggrIfcRelAggregates);
        noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (int_t i = 0; i < noAggrIfcRelAggregates; i++) {
            int_t   ifcRelAggregatesInstance = 0;
            sdaiGetAggrByIndex(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

    	    int_t	* aggrIfcObjectDefinition = nullptr, noAggrIfcObjectDefinition;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (int_t j = 0; j < noAggrIfcObjectDefinition; j++) {
                int_t   ifcObjectDefinitionInstance = 0;
                sdaiGetAggrByIndex(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT")) {
                    if (pAlignmentSegment) {
                        pAlignmentSegment[alignmentSegmentCnt] = ifcObjectDefinitionInstance;
                    }
                    alignmentSegmentCnt++;
                }
            }
        }
    }

    {
	    int_t	* aggrIfcRelAggregates = nullptr, noAggrIfcRelAggregates;
        sdaiGetAttrBN(ifcAlignmentXyzInstance, "IsDecomposedBy", sdaiAGGR, &aggrIfcRelAggregates);
        noAggrIfcRelAggregates = sdaiGetMemberCount(aggrIfcRelAggregates);
        for (int_t i = 0; i < noAggrIfcRelAggregates; i++) {
            int_t   ifcRelAggregatesInstance = 0;
            sdaiGetAggrByIndex(aggrIfcRelAggregates, i, sdaiINSTANCE, &ifcRelAggregatesInstance);

    	    int_t	* aggrIfcObjectDefinition = nullptr, noAggrIfcObjectDefinition;
            sdaiGetAttrBN(ifcRelAggregatesInstance, "RelatedObjects", sdaiAGGR, &aggrIfcObjectDefinition);
            noAggrIfcObjectDefinition = sdaiGetMemberCount(aggrIfcObjectDefinition);
            for (int_t j = 0; j < noAggrIfcObjectDefinition; j++) {
                int_t   ifcObjectDefinitionInstance = 0;
                sdaiGetAggrByIndex(aggrIfcObjectDefinition, j, sdaiINSTANCE, &ifcObjectDefinitionInstance);

                if (sdaiGetInstanceType(ifcObjectDefinitionInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT")) {
                    if (pAlignmentSegment) {
                        pAlignmentSegment[alignmentSegmentCnt] = ifcObjectDefinitionInstance;
                    }
                    alignmentSegmentCnt++;

                    assert(false);  //  should be a nested relations
                }
            }
        }
    }

 //   assert(alignmentSegmentCnt || ifcAlignmentXyzInstance == 0);
    return  alignmentSegmentCnt;
}

#ifdef _DEBUG
static  inline  bool	GetGeometryFromGK__point4D__SEMANTICS(
								OwlModel	owlModel,
								OwlInstance	owlInstancePoint4D,
								___POINT4D	* point4D
							)
{
//	SaveInstanceTree(owlInstancePoint4D, "c:\\0\\poinbt4d.bin");

	
	bool	asExpected = true;

	OwlClass myClass = GetInstanceClass(owlInstancePoint4D);
	if (myClass == GetClassByName(owlModel, "Point4D")) {
		{
			OwlClass	* values = nullptr;
			int64_t		card = 0;
			GetObjectProperty(owlInstancePoint4D, GetPropertyByName(owlModel, "point"), &values, &card);
			if (card == 1 && values[0]) {
				OwlInstance	owlInstancePoint3D = values[0];
				assert(GetInstanceClass(owlInstancePoint3D) == GetClassByName(owlModel, "Point3D"));

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "x"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->point.x = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->point.x = 0.;
					}
				}

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "y"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->point.y = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->point.y = 0.;
					}
				}

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "z"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->point.z = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->point.z = 0.;
					}
				}
			}
			else {
				assert(false);
				asExpected = false;
			}
		}

		{
			OwlClass	* values = nullptr;
			int64_t		card = 0;
			GetObjectProperty(owlInstancePoint4D, GetPropertyByName(owlModel, "tangent"), &values, &card);
			if (card == 1 && values[0]) {
				OwlInstance	owlInstancePoint3D = values[0];
				assert(GetInstanceClass(owlInstancePoint3D) == GetClassByName(owlModel, "Vector3"));

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "x"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->tangent.x = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->tangent.x = 0.;
					}
				}

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "y"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->tangent.y = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->tangent.y = 0.;
					}
				}

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "z"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->tangent.z = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->tangent.z = 0.;
					}
				}

				point4D->direction__DERIVED = atan2(point4D->tangent.y, point4D->tangent.x);
			}
			else {
				assert(false);
				asExpected = false;
			}
		}

		{
			OwlClass	* values = nullptr;
			int64_t		card = 0;
			GetObjectProperty(owlInstancePoint4D, GetPropertyByName(owlModel, "normal"), &values, &card);
			if (card == 1 && values[0]) {
				OwlInstance	owlInstancePoint3D = values[0];
				assert(GetInstanceClass(owlInstancePoint3D) == GetClassByName(owlModel, "Vector3"));

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "x"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->normal.x = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->normal.x = 0.;
					}
				}

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "y"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->normal.y = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->normal.y = 0.;
					}
				}

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "z"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->normal.z = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->normal.z = 0.;
					}
				}
			}
			else {
				assert(false);
				asExpected = false;
			}
		}
	}
	else {
		assert(false);
		asExpected = false;
	}

	return	asExpected;
}

static  inline  bool	___GetBorderPoints__SEMANTICS(
								OwlInstance		owlInstance,
								___POINT4D		* startPoint,
								___POINT4D		* endPoint
							)
{
	bool	asExpected = true;

	OwlModel	owlModel = GetModel(owlInstance);
	OwlClass	myClass = GetInstanceClass(owlInstance);

	if (myClass == GetClassByName(owlModel, "PolyLine3D")) {
		OwlClass	* values = nullptr;
		int64_t		card = 0;
		GetObjectProperty(owlInstance, GetPropertyByName(owlModel, "pointReferences"), &values, &card);
		if (card && values[0] && values[card - 1]) {
			if (GetGeometryFromGK__point4D__SEMANTICS(owlModel, values[0], startPoint) == false)
				asExpected = false;
			if (GetGeometryFromGK__point4D__SEMANTICS(owlModel, values[card - 1], endPoint) == false)
				asExpected = false;
		}
		else {
			assert(false);
			asExpected = false;
		}
	}
	else if (myClass == GetClassByName(owlModel, "Point4D")) {
		if (GetGeometryFromGK__point4D__SEMANTICS(owlModel, owlInstance, startPoint) == false)
			asExpected = false;
		if (GetGeometryFromGK__point4D__SEMANTICS(owlModel, owlInstance, endPoint) == false)
			asExpected = false;
	}
	else {
		assert(false);
		asExpected = false;
	}

	return	asExpected;
}

static  inline  bool    ___GetBorderPoints(
                                SdaiInstance    ifcCurveSegmentInstance,
                                SdaiEntity      ifcBoundedCurveEntity,
                                ___POINT4D      * startPoint,
                                ___POINT4D      * endPoint
                            )
{
	assert(GetRevision() >= 1650);

	OwlInstance	owlInstance = 0;
    owlBuildInstanceInContext(ifcCurveSegmentInstance, ifcBoundedCurveEntity, &owlInstance);

	return	___GetBorderPoints__SEMANTICS(
					owlInstance,
                    startPoint,
                    endPoint
				);
}
#endif // _DEBUG

static  inline  void    ___GetEndPoint(
                                int_t           model,
                                ___VECTOR2      * endPoint,
#ifdef _DEBUG
                                ___VECTOR2      * startPoint,
#endif // _DEBUG
                                SdaiInstance    ifcCurveSegmentInstance,
                                SdaiEntity      ifcBoundedCurveEntity
                            )
{
    setFilter(model, 1 + 2, 1 + 2 + 4);	//	we need to recognize line representations
    int64_t mask = GetFormat(0, 0),
            setting =
			      1 * __flagbit2         //    SINGLE / DOUBLE PRECISION (float / double)
			    + 0 * __flagbit3         //    32 / 63 BIT INDEX ARRAY (int32_t / int64_t)

			    + 1 * __flagbit4         //    OFF / ON VECTORS (x, y, z) 
			    + 0 * __flagbit5         //    OFF / ON NORMALS (Nx, Ny, Nz)

			    + 0 * __flagbit8         //    OFF / ON TRIANGLES
			    + 1 * __flagbit9         //    OFF / ON LINES
			    + 0 * __flagbit10        //    OFF / ON POINTS

			    + 0 * __flagbit12        //    OFF / ON WIREFRAME FACES
			    + 0 * __flagbit13;       //    OFF / ON WIREFRAME CONCEPTUAL FACES

	int64_t vertexElementSizeInBytes = SetFormat(model, setting, mask);
	assert(vertexElementSizeInBytes == 3 * sizeof(double));

    int64_t elementSize = SetFormat(model, setting, mask);
    assert(elementSize == 3 * 8);

    int64_t owlInstance = 0;
    owlBuildInstanceInContext(ifcCurveSegmentInstance, ifcBoundedCurveEntity, &owlInstance);

    int64_t vertexBufferSize = 0, indexBufferSize = 0;
    CalculateInstance(owlInstance, &vertexBufferSize, &indexBufferSize, nullptr);
    if (vertexBufferSize && indexBufferSize) {
        double  * vertices = new double[3 * (int_t) vertexBufferSize];
        UpdateInstanceVertexBuffer(owlInstance, vertices);

        int32_t * indices = new int32_t[(int_t) indexBufferSize];
        UpdateInstanceIndexBuffer(owlInstance, indices);

        ___VECTOR3  startVec = {
                            vertices[3 * 0 + 0],
                            vertices[3 * 0 + 1],
                            vertices[3 * 0 + 2]
                        },
                    endVec = {
                            vertices[3 * (vertexBufferSize - 1) + 0],
                            vertices[3 * (vertexBufferSize - 1) + 1],
                            vertices[3 * (vertexBufferSize - 1) + 2]
                        };
        assert(startVec.x == startPoint->x &&
               startVec.y == startPoint->y &&
               startVec.z == 0.);
        endPoint->x = endVec.x;
        endPoint->y = endVec.y;
        assert(endVec.z == 0.);

        delete[] vertices;
        delete[] indices;
    }
    else {
        assert(false);
    }
}