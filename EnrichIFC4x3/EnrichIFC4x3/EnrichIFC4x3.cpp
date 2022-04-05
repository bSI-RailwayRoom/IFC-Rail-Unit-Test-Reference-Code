#include <iostream>
#include <assert.h>

#include "./ifcengine/include/ifcengine.h"
#include "./ifcengine/include/engine.h"

#include "./business2geometry/ifcalignment.h"


static  const   uint64_t    flagbit0 = 1;                           // 2^^0                          0000.0000..0000.0001
static  const   uint64_t    flagbit1 = 2;                           // 2^^1                          0000.0000..0000.0010
static  const   uint64_t    flagbit2 = 4;                           // 2^^2                          0000.0000..0000.0100
static  const   uint64_t    flagbit3 = 8;                           // 2^^3                          0000.0000..0000.1000

static  const   uint64_t    flagbit4 = 16;                          // 2^^4                          0000.0000..0001.0000
static  const   uint64_t    flagbit5 = 32;                          // 2^^5                          0000.0000..0010.0000
static  const   uint64_t    flagbit6 = 64;                          // 2^^6                          0000.0000..0100.0000
static  const   uint64_t    flagbit7 = 128;                         // 2^^7                          0000.0000..1000.0000

static  const   uint64_t    flagbit8 = 256;                         // 2^^8                          0000.0001..0000.0000
static  const   uint64_t    flagbit9 = 512;                         // 2^^9                          0000.0010..0000.0000
static  const   uint64_t    flagbit10 = 1024;                       // 2^^10                         0000.0100..0000.0000
static  const   uint64_t    flagbit11 = 2048;                       // 2^^11                         0000.1000..0000.0000

static  const   uint64_t    flagbit12 = 4096;                       // 2^^12                         0001.0000..0000.0000
static  const   uint64_t    flagbit13 = 8192;                       // 2^^13                         0010.0000..0000.0000
static  const   uint64_t    flagbit14 = 16384;                      // 2^^14                         0100.0000..0000.0000
static  const   uint64_t    flagbit15 = 32768;                      // 2^^15                         1000.0000..0000.0000


#ifdef _DEBUG
extern  int_t   horizontalAlignmentParentCurveI, horizontalAlignmentParentCurveII;
#endif // _DEBUG


int_t    ConvertFile(char * inputFileName, char * generatedFileName)
{
    int_t   convertedInstances = 0,
            model = sdaiOpenModelBN(0, inputFileName, "");

    if (model) {
#ifdef _DEBUG
        sdaiSaveModelBN(model, inputFileName);
#endif // _DEBUG

        int_t   * ifcAlignmentInstances = sdaiGetEntityExtentBN(model, "IFCALIGNMENT"),
                noIfcAlignmentInstances = sdaiGetMemberCount(ifcAlignmentInstances);
        if (noIfcAlignmentInstances) {
            int_t   ifcRailwayInstance = 0;
            engiGetAggrElement(sdaiGetEntityExtentBN(model, "IFCRAILWAY"), 0, sdaiINSTANCE, &ifcRailwayInstance);

            int_t   ifcRelationInstance = 0;
            engiGetAggrElement(sdaiGetEntityExtentBN(model, "IFCRELCONTAINEDINSPATIALSTRUCTURE"), 0, sdaiINSTANCE, &ifcRelationInstance);

            int_t   * aggrRelatedElements = nullptr;
            sdaiGetAttrBN(ifcRelationInstance, "RelatedElements", sdaiAGGR, &aggrRelatedElements);

            for (int_t i = 0; i < noIfcAlignmentInstances; i++) {
                int_t   ifcAlignmentInstance = 0;
                engiGetAggrElement(ifcAlignmentInstances, i, sdaiINSTANCE, &ifcAlignmentInstance);

                int_t   ifcProductRepresentationInstance = 0;
                sdaiGetAttrBN(ifcAlignmentInstance, "Representation", sdaiINSTANCE, &ifcProductRepresentationInstance);

                if (!ifcProductRepresentationInstance) {
                    AlignmentGenerateGeometry(
                            model,
                            ifcAlignmentInstance
                        );

#ifdef _DEBUG
                    {
                        int_t myInstance = sdaiCreateInstanceBN(model, "IFCBUILDINGELEMENTPROXY");
                        sdaiAppend((int_t)aggrRelatedElements, sdaiINSTANCE, (void*)myInstance);
                        sdaiPutAttrBN(
                                myInstance,
                                "Representation",
                                sdaiINSTANCE,
                                (void*) ___CreateProductDefinitionShapeInstance(
                                                model,
                                                ___CreateEdgeInstance(model, { 0., -150., 0. }, { 0. , 150., 0. }),
                                                true    
                                            )
                            );
                    }

                    {
                        int_t myInstance = sdaiCreateInstanceBN(model, "IFCBUILDINGELEMENTPROXY");
                        sdaiAppend((int_t)aggrRelatedElements, sdaiINSTANCE, (void*)myInstance);
                        sdaiPutAttrBN(
                                myInstance,
                                "Representation",
                                sdaiINSTANCE,
                                (void*) ___CreateProductDefinitionShapeInstance(
                                                model,
                                                ___CreateEdgeInstance(model, { -150., 0., 0. }, { 150. , 0., 0. }),
                                                true    
                                            )
                            );
                    }

                    {
                        int_t myInstance = sdaiCreateInstanceBN(model, "IFCBUILDINGELEMENTPROXY");
                        sdaiAppend((int_t)aggrRelatedElements, sdaiINSTANCE, (void*)myInstance);
                        sdaiPutAttrBN(
                                myInstance,
                                "Representation",
                                sdaiINSTANCE,
                                (void*) ___CreateProductDefinitionShapeInstance(
                                                model,
                                                horizontalAlignmentParentCurveI,
                                                true    
                                            )
                            );

                    }

                    {
                        if (horizontalAlignmentParentCurveII) {
                            int_t myInstance = sdaiCreateInstanceBN(model, "IFCBUILDINGELEMENTPROXY");
                            sdaiAppend((int_t)aggrRelatedElements, sdaiINSTANCE, (void*)myInstance);
                            sdaiPutAttrBN(
                                    myInstance,
                                    "Representation",
                                    sdaiINSTANCE,
                                    (void*) ___CreateProductDefinitionShapeInstance(
                                                    model,
                                                    horizontalAlignmentParentCurveII,
                                                    true    
                                                )
                                );
                        }
                    }


                    int_t   ifcProductInstance = sdaiCreateInstanceBN(model, "IFCBUILDINGELEMENTPROXY");
                    sdaiAppend((int_t)aggrRelatedElements, sdaiINSTANCE, (void*)ifcProductInstance);
                    int_t   ifcCircleHollowProfileDefInstance = sdaiCreateInstanceBN(model, "IFCCIRCLEPROFILEDEF");
//                    int_t   ifcCircleHollowProfileDefInstance = sdaiCreateInstanceBN(ifcModel, "IFCCIRCLEHOLLOWPROFILEDEF");

                    double  radius = 0.7;// , wallThickness = 0.01;
                    sdaiPutAttrBN(ifcCircleHollowProfileDefInstance, "Radius", sdaiREAL, &radius);
//                    sdaiPutAttrBN(ifcCircleHollowProfileDefInstance, "WallThickness", sdaiREAL, &wallThickness);

                    //
                    //  Create example sweep
                    //
                    AlignmentGenerateSweep(
                            model,
                            ifcAlignmentInstance,
                            ifcCircleHollowProfileDefInstance,
                            ifcProductInstance
                        );  //  */
    //               int_t   ifcRectangleProfileDefInstance = sdaiCreateInstanceBN(model, "IFCRECTANGLEPROFILEDEF");

            /*        double  x = 0.1, y = 0.2;
                    sdaiPutAttrBN(ifcRectangleProfileDefInstance, "XDim", sdaiREAL, &x);
                    sdaiPutAttrBN(ifcRectangleProfileDefInstance, "YDim", sdaiREAL, &y);

                    //
                    //  Create example sweep
                    //
                    AlignmentGenerateSweep(
                            model,
                            ifcAlignmentInstance,
                            ifcRectangleProfileDefInstance,
                            ifcProductInstance,
                            1.
                        );  //  */
#endif // _DEBUG

                    convertedInstances++;
                }
            }
        }
        else {
            std::cout << "No alignment instances found.\n";
        }

        sdaiSaveModelBN(model, generatedFileName);

        cleanMemory(model, 4);
        sdaiCloseModel(model);
    }

    return  convertedInstances;
}

void    CheckResults(char * fileName)
{
    int_t   ifcModel = sdaiOpenModelBN(0, fileName, "");

    {
        int64_t owlModel = 0;
        owlGetModel(ifcModel, &owlModel);
        int64_t mask = GetFormat(0, 0),
                setting = flagbit2 + flagbit4 + flagbit9;
        SetFormat(owlModel, setting, mask);

        setSegmentation(ifcModel, 200, 0.);

        //
        //  HORIZONTAL ALIGNMENT
        //
        int_t   * ifcCompositeCurveInstances = sdaiGetEntityExtentBN(ifcModel, "IFCCOMPOSITECURVE"),
                noIfcCompositeCurveInstances = sdaiGetMemberCount(ifcCompositeCurveInstances);
        for (int_t i = 0; i < noIfcCompositeCurveInstances; i++) {
            int_t   ifcCompositeCurveInstance = 0;
            engiGetAggrElement(ifcCompositeCurveInstances, i, sdaiINSTANCE, &ifcCompositeCurveInstance);
            
            int_t   * ifcSegmentInstances = nullptr;
            sdaiGetAttrBN(ifcCompositeCurveInstance, "Segments", sdaiAGGR, &ifcSegmentInstances);
            int_t   noIfcSegmentInstances = sdaiGetMemberCount(ifcSegmentInstances);

            ___VECTOR3  * pStartPoint  = new ___VECTOR3[noIfcSegmentInstances],
                        * pStartVector = new ___VECTOR3[noIfcSegmentInstances],
                        * pEndPoint    = new ___VECTOR3[noIfcSegmentInstances],
                        * pEndVector   = new ___VECTOR3[noIfcSegmentInstances];

            for (int_t j = 0; j < noIfcSegmentInstances; j++) {
                int_t   ifcSegmentInstance = 0;
                engiGetAggrElement(ifcSegmentInstances, j, sdaiINSTANCE, &ifcSegmentInstance);

 cleanMemory(ifcModel, 1);

                int64_t owlInstance = 0;
//                owlBuildInstance(ifcModel, ifcSegmentInstance, &owlInstance);
//                owlBuildInstanceInContext(ifcSegmentInstance, ifcCompositeCurveInstance, &owlInstance);
                owlBuildInstanceInContext(ifcSegmentInstance, sdaiGetInstanceType(ifcCompositeCurveInstance), &owlInstance);

                int64_t vertexBufferSize = 0, indexBufferSize = 0;
                CalculateInstance(owlInstance, &vertexBufferSize, &indexBufferSize, nullptr);
                if (vertexBufferSize && indexBufferSize) {
                    double  * vertices = new double[3 * (int_t) vertexBufferSize];
                    UpdateInstanceVertexBuffer(owlInstance, vertices);

                    int32_t * indices = new int32_t[(int_t) indexBufferSize];
                    UpdateInstanceIndexBuffer(owlInstance, indices);

                    ___VECTOR3  * pPnt = new ___VECTOR3[(int_t) indexBufferSize];

                    int_t    offset = 0;
                    int64_t  conceptualFaceCnt = GetConceptualFaceCnt(owlInstance);
                    for (int_t k = 0; k < conceptualFaceCnt; k++) {
                        int64_t  startIndicesLines = 0, noIndicesLines = 0;
                        GetConceptualFaceEx(
                                owlInstance, k,
                                nullptr, nullptr,
                                &startIndicesLines, &noIndicesLines,
                                nullptr, nullptr,
                                nullptr, nullptr,
                                nullptr, nullptr
                            );
                        assert(noIndicesLines);

                        if (offset) {
                            assert(std::fabs(pPnt[offset - 1].x - vertices[3 * indices[startIndicesLines] + 0]) < 0.000000001);
                            assert(std::fabs(pPnt[offset - 1].y - vertices[3 * indices[startIndicesLines] + 1]) < 0.000000001);
                            assert(std::fabs(pPnt[offset - 1].z - vertices[3 * indices[startIndicesLines] + 2]) < 0.000000001);
                            offset--;
                        }

                        for (int_t m = 0; m < noIndicesLines; m++) {
                            pPnt[offset].x = vertices[3 * indices[startIndicesLines + m] + 0];
                            pPnt[offset].y = vertices[3 * indices[startIndicesLines + m] + 1];
                            pPnt[offset].z = vertices[3 * indices[startIndicesLines + m] + 2];
                            offset++;
                        }
                    }
                    assert(offset >= 2);

                    ___VECTOR3  * pnt0 = &pPnt[0],
                                * pnt1 = &pPnt[1],
                                * pntNm2 = &pPnt[offset - 2],
                                * pntNm1 = &pPnt[offset - 1];
                    pStartPoint[j].x = pnt0->x;
                    pStartPoint[j].y = pnt0->y;
                    pStartPoint[j].z = pnt0->z;
                    ___Vec3Subtract(&pStartVector[j], pnt1, pnt0);
                    ___Vec3Normalize(&pStartVector[j]);
                    pEndPoint[j].x = pntNm1->x;
                    pEndPoint[j].y = pntNm1->y;
                    pEndPoint[j].z = pntNm1->z;
                    ___Vec3Subtract(&pEndVector[j], pntNm1, pntNm2);
                    ___Vec3Normalize(&pEndVector[j]);
                }
                else {
                    if (j == noIfcSegmentInstances - 1 && j) {
                        pStartPoint[j] = pEndPoint[j - 1];
                        pStartVector[j] = pEndVector[j - 1];
                    }
                    else {
                        assert(false);
                    }
                }
            }

            for (int_t j = 1; j < noIfcSegmentInstances; j++) {
                ___VECTOR3  vecDiff;
                ___Vec3Subtract(&vecDiff, &pEndPoint[j - 1], &pStartPoint[j]);
                    
                double  epsilon = 0.0001,
                        distance = ___Vec3Length(&vecDiff);
                if (std::fabs(distance) > epsilon) {
                    std::cout << "  ISSUE HORIZONTAL ALIGNMENT distance end / start point too large, distance : " << distance << " (epsilon : " << epsilon << ").\n";
                }

                double  dotProduct = ___Vec3Dot(&pEndVector[j - 1], &pStartVector[j]);
                if (dotProduct <= 1. - epsilon || dotProduct >= 1. + epsilon) {
                    std::cout << "  ISSUE HORIZONTAL ALIGNMENT difference angle end / start point too large, dotProduct : " << dotProduct << " (epsilon : " << epsilon << ").\n";
                }
            }

            delete[] pStartPoint;
            delete[] pStartVector;
            delete[] pEndPoint;
            delete[] pEndVector;
        }

        //
        //  VERTICAL ALIGNMENT
        //
        int_t   * ifcGradientCurveInstances = sdaiGetEntityExtentBN(ifcModel, "IFCGRADIENTCURVE"),
                noIfcGradientCurveInstances = sdaiGetMemberCount(ifcGradientCurveInstances);
        for (int_t i = 0; i < noIfcGradientCurveInstances; i++) {
            int_t   ifcGradientCurveInstance = 0;
            engiGetAggrElement(ifcGradientCurveInstances, i, sdaiINSTANCE, &ifcGradientCurveInstance);
            
            int_t   * ifcSegmentInstances = nullptr;
            sdaiGetAttrBN(ifcGradientCurveInstance, "Segments", sdaiAGGR, &ifcSegmentInstances);
            int_t   noIfcSegmentInstances = sdaiGetMemberCount(ifcSegmentInstances);

            ___VECTOR3  * pStartPoint  = new ___VECTOR3[noIfcSegmentInstances],
                        * pStartVector = new ___VECTOR3[noIfcSegmentInstances],
                        * pEndPoint    = new ___VECTOR3[noIfcSegmentInstances],
                        * pEndVector   = new ___VECTOR3[noIfcSegmentInstances];

            for (int_t j = 0; j < noIfcSegmentInstances; j++) {
                int_t   ifcSegmentInstance = 0;
                engiGetAggrElement(ifcSegmentInstances, j, sdaiINSTANCE, &ifcSegmentInstance);

                int64_t owlInstance = 0;
//                owlBuildInstance(ifcModel, ifcSegmentInstance, &owlInstance);
                owlBuildInstanceInContext(ifcSegmentInstance, ifcGradientCurveInstance, &owlInstance);

                int64_t vertexBufferSize = 0, indexBufferSize = 0;
                CalculateInstance(owlInstance, &vertexBufferSize, &indexBufferSize, nullptr);
                if (vertexBufferSize && indexBufferSize) {
                    double  * vertices = new double[3 * (int_t) vertexBufferSize];
                    UpdateInstanceVertexBuffer(owlInstance, vertices);

                    int32_t * indices = new int32_t[(int_t) indexBufferSize];
                    UpdateInstanceIndexBuffer(owlInstance, indices);

                    ___VECTOR3  * pPnt = new ___VECTOR3[(int_t) indexBufferSize];

                    int_t    offset = 0;
                    int64_t  conceptualFaceCnt = GetConceptualFaceCnt(owlInstance);
                    for (int_t k = 0; k < conceptualFaceCnt; k++) {
                        int64_t  startIndicesLines = 0, noIndicesLines = 0;
                        GetConceptualFaceEx(
                                owlInstance, k,
                                nullptr, nullptr,
                                &startIndicesLines, &noIndicesLines,
                                nullptr, nullptr,
                                nullptr, nullptr,
                                nullptr, nullptr
                            );
                        assert(noIndicesLines);

                        if (offset) {
                            assert(std::fabs(pPnt[offset - 1].x - vertices[3 * indices[startIndicesLines] + 0]) < 0.000000001);
                            assert(std::fabs(pPnt[offset - 1].y - vertices[3 * indices[startIndicesLines] + 1]) < 0.000000001);
                            assert(std::fabs(pPnt[offset - 1].z - vertices[3 * indices[startIndicesLines] + 2]) < 0.000000001);
                            offset--;
                        }

                        for (int_t m = 0; m < noIndicesLines; m++) {
                            pPnt[offset].x = vertices[3 * indices[startIndicesLines + m] + 0];
                            pPnt[offset].y = vertices[3 * indices[startIndicesLines + m] + 1];
                            pPnt[offset].z = vertices[3 * indices[startIndicesLines + m] + 2];
                            offset++;
                        }
                    }
                    assert(offset >= 2);

                    ___VECTOR3  * pnt0 = &pPnt[0],
                                * pnt1 = &pPnt[1],
                                * pntNm2 = &pPnt[offset - 2],
                                * pntNm1 = &pPnt[offset - 1];
                    pStartPoint[j].x = pnt0->x;
                    pStartPoint[j].y = pnt0->y;
                    pStartPoint[j].z = pnt0->z;
                    ___Vec3Subtract(&pStartVector[j], pnt1, pnt0);
                    ___Vec3Normalize(&pStartVector[j]);
                    pEndPoint[j].x = pntNm1->x;
                    pEndPoint[j].y = pntNm1->y;
                    pEndPoint[j].z = pntNm1->z;
                    ___Vec3Subtract(&pEndVector[j], pntNm1, pntNm2);
                    ___Vec3Normalize(&pEndVector[j]);
                }
                else {
                    if (j == noIfcSegmentInstances - 1 && j) {
                        pStartPoint[j] = pEndPoint[j - 1];
                        pStartVector[j] = pEndVector[j - 1];
                    }
                    else {
                        assert(false);
                    }
                }
            }

            for (int_t j = 1; j < noIfcSegmentInstances; j++) {
                ___VECTOR3  vecDiff;
                ___Vec3Subtract(&vecDiff, &pEndPoint[j - 1], &pStartPoint[j]);
                    
                double  epsilon = 0.0001,
                        distance = ___Vec3Length(&vecDiff);
                if (std::fabs(distance) > epsilon) {
                    std::cout << "  ISSUE VERTICAL ALIGNMENT distance end / start point too large, distance : " << distance << " (epsilon : " << epsilon << ").\n";
                }

                double  dotProduct = ___Vec3Dot(&pEndVector[j - 1], &pStartVector[j]);
                if (dotProduct <= 1. - epsilon || dotProduct >= 1. + epsilon) {
                    std::cout << "  ISSUE VERTICAL ALIGNMENT difference angle end / start point too large, dotProduct : " << dotProduct << " (epsilon : " << epsilon << ").\n";
                }
            }

            delete[] pStartPoint;
            delete[] pStartVector;
            delete[] pEndPoint;
            delete[] pEndVector;
        }

        //
        //  CANT ALIGNMENT
        //
        int_t   * ifcSegmentedReferenceCurveInstances = sdaiGetEntityExtentBN(ifcModel, "IFCSEGMENTEDREFERENCECURVE"),
                noIfcSegmentedReferenceCurveInstances = sdaiGetMemberCount(ifcGradientCurveInstances);
        for (int_t i = 0; i < noIfcSegmentedReferenceCurveInstances; i++) {
            int_t   ifcSegmentedReferenceCurveInstance = 0;
            engiGetAggrElement(ifcSegmentedReferenceCurveInstances, i, sdaiINSTANCE, &ifcSegmentedReferenceCurveInstance);
            
            int_t   * ifcSegmentInstances = nullptr;
            sdaiGetAttrBN(ifcSegmentedReferenceCurveInstance, "Segments", sdaiAGGR, &ifcSegmentInstances);
            int_t   noIfcSegmentInstances = sdaiGetMemberCount(ifcSegmentInstances);

            ___VECTOR3  * pStartPoint  = new ___VECTOR3[noIfcSegmentInstances],
                        * pStartVector = new ___VECTOR3[noIfcSegmentInstances],
                        * pEndPoint    = new ___VECTOR3[noIfcSegmentInstances],
                        * pEndVector   = new ___VECTOR3[noIfcSegmentInstances];

            for (int_t j = 0; j < noIfcSegmentInstances; j++) {
                int_t   ifcSegmentInstance = 0;
                engiGetAggrElement(ifcSegmentInstances, j, sdaiINSTANCE, &ifcSegmentInstance);

                int64_t owlInstance = 0;
//                owlBuildInstance(ifcModel, ifcSegmentInstance, &owlInstance);
                owlBuildInstanceInContext(ifcSegmentInstance, ifcSegmentedReferenceCurveInstance, &owlInstance);

//SaveInstanceTree(owlInstance, "c:\\9\\test.bin");
//SaveInstanceTree(owlInstance, "c:\\9\\test.rdf");
                int64_t vertexBufferSize = 0, indexBufferSize = 0;
                CalculateInstance(owlInstance, &vertexBufferSize, &indexBufferSize, nullptr);
                if (vertexBufferSize && indexBufferSize) {
                    double  * vertices = new double[3 * (int_t) vertexBufferSize];
                    UpdateInstanceVertexBuffer(owlInstance, vertices);

                    int32_t * indices = new int32_t[(int_t) indexBufferSize];
                    UpdateInstanceIndexBuffer(owlInstance, indices);

                    ___VECTOR3  * pPnt = new ___VECTOR3[(int_t) indexBufferSize];

                    int_t    offset = 0;
                    int64_t  conceptualFaceCnt = GetConceptualFaceCnt(owlInstance);
                    for (int_t k = 0; k < conceptualFaceCnt; k++) {
                        int64_t  startIndicesLines = 0, noIndicesLines = 0;
                        GetConceptualFaceEx(
                                owlInstance, k,
                                nullptr, nullptr,
                                &startIndicesLines, &noIndicesLines,
                                nullptr, nullptr,
                                nullptr, nullptr,
                                nullptr, nullptr
                            );
                        assert(noIndicesLines);

                        if (offset) {
                            assert(std::fabs(pPnt[offset - 1].x - vertices[3 * indices[startIndicesLines] + 0]) < 0.000000001);
                            assert(std::fabs(pPnt[offset - 1].y - vertices[3 * indices[startIndicesLines] + 1]) < 0.000000001);
                            assert(std::fabs(pPnt[offset - 1].z - vertices[3 * indices[startIndicesLines] + 2]) < 0.000000001);
                            offset--;
                        }

                        for (int_t m = 0; m < noIndicesLines; m++) {
                            pPnt[offset].x = vertices[3 * indices[startIndicesLines + m] + 0];
                            pPnt[offset].y = vertices[3 * indices[startIndicesLines + m] + 1];
                            pPnt[offset].z = vertices[3 * indices[startIndicesLines + m] + 2];
                            offset++;
                        }
                    }
                    assert(offset >= 2);

                    ___VECTOR3  * pnt0 = &pPnt[0],
                                * pnt1 = &pPnt[1],
                                * pntNm2 = &pPnt[offset - 2],
                                * pntNm1 = &pPnt[offset - 1];
                    pStartPoint[j].x = pnt0->x;
                    pStartPoint[j].y = pnt0->y;
                    pStartPoint[j].z = pnt0->z;
                    ___Vec3Subtract(&pStartVector[j], pnt1, pnt0);
                    ___Vec3Normalize(&pStartVector[j]);
                    pEndPoint[j].x = pntNm1->x;
                    pEndPoint[j].y = pntNm1->y;
                    pEndPoint[j].z = pntNm1->z;
                    ___Vec3Subtract(&pEndVector[j], pntNm1, pntNm2);
                    ___Vec3Normalize(&pEndVector[j]);
                }
                else {
                    if (j == noIfcSegmentInstances - 1 && j) {
                        pStartPoint[j] = pEndPoint[j - 1];
                        pStartVector[j] = pEndVector[j - 1];
                    }
                    else {
                        assert(false);
                    }
                }
            }

            for (int_t j = 1; j < noIfcSegmentInstances; j++) {
                ___VECTOR3  vecDiff;
                ___Vec3Subtract(&vecDiff, &pEndPoint[j - 1], &pStartPoint[j]);
                    
                double  epsilon = 0.0001,
                        distance = ___Vec3Length(&vecDiff);
                if (std::fabs(distance) > epsilon) {
                    std::cout << "  ISSUE CANT ALIGNMENT distance end / start point too large, distance : " << distance << " (epsilon : " << epsilon << ").\n";
                }

//                double  dotProduct = Vec3Dot(&pEndVector[j - 1], &pStartVector[j]);
//                if (dotProduct <= 1. - epsilon || dotProduct >= 1. + epsilon) {
//                    std::cout << "  ISSUE CANT ALIGNMENT difference angle end / start point too large, dotProduct : " << dotProduct << " (epsilon : " << epsilon << ").\n";
//                }
            }

            delete[] pStartPoint;
            delete[] pStartVector;
            delete[] pEndPoint;
            delete[] pEndVector;
        }

        cleanMemory(ifcModel, 4);
        sdaiCloseModel(ifcModel);
    }
}

void    GeneratePointList__DIRECT(char * generatedFileName, char * pointListFileName, int_t segmentationParts, char * entityNameCurve)
{
    int_t   ifcModel = sdaiOpenModelBN(0, generatedFileName, "");

    {
        FILE    * fp = nullptr;
        fopen_s(&fp, pointListFileName, "w");

        if (fp) {
            setSegmentation(ifcModel, segmentationParts, 0.);

            int64_t owlModel = 0;
            owlGetModel(ifcModel, &owlModel);
            int64_t mask = GetFormat(0, 0),
                    setting = flagbit2 + flagbit4 + flagbit9;
            SetFormat(owlModel, setting, mask);

            int_t   * ifcCompositeCurveInstances = sdaiGetEntityExtentBN(ifcModel, entityNameCurve),
                    noIfcCompositeCurveInstances = sdaiGetMemberCount(ifcCompositeCurveInstances);
            for (int_t i = 0; i < noIfcCompositeCurveInstances; i++) {
                int_t   ifcCompositeCurveInstance = 0;
                engiGetAggrElement(ifcCompositeCurveInstances, i, sdaiINSTANCE, &ifcCompositeCurveInstance);
            
                ___VECTOR3  vecLast;
                int        cnt = 0;

                double  curveLength = 0.;

                int_t   * ifcSegmentInstances = nullptr;
                sdaiGetAttrBN(ifcCompositeCurveInstance, "Segments", sdaiAGGR, &ifcSegmentInstances);
                int_t   noIfcSegmentInstances = sdaiGetMemberCount(ifcSegmentInstances);
                for (int_t j = 0; j < noIfcSegmentInstances; j++) {
                    int_t   ifcSegmentInstance = 0;
                    engiGetAggrElement(ifcSegmentInstances, j, sdaiINSTANCE, &ifcSegmentInstance);

                    double  segmentStart = 0.;
                    sdaiGetAttrBN(ifcSegmentInstance, "SegmentStart", sdaiREAL, &segmentStart);

                    double  segmentLength = 0.;
                    sdaiGetAttrBN(ifcSegmentInstance, "SegmentLength", sdaiREAL, &segmentLength);

                    curveLength += segmentLength;
                }

                    int64_t owlInstance = 0;
//                    owlBuildInstance(ifcModel, ifcSegmentInstance, &owlInstance);
 //                   owlBuildInstance(ifcModel, ifcCompositeCurveInstance, &owlInstance);
                    owlBuildInstanceInContext(ifcCompositeCurveInstance, ifcCompositeCurveInstance, &owlInstance);

                    int64_t vertexBufferSize = 0, indexBufferSize = 0;
                    CalculateInstance(owlInstance, &vertexBufferSize, &indexBufferSize, nullptr);
                    if (vertexBufferSize && indexBufferSize) {
                        double  * vertices = new double[3 * (int_t) vertexBufferSize];
                        UpdateInstanceVertexBuffer(owlInstance, vertices);

                        int32_t * indices = new int32_t[(int_t) indexBufferSize];
                        UpdateInstanceIndexBuffer(owlInstance, indices);

                        ___VECTOR3  * pPnt = new ___VECTOR3[(int_t) indexBufferSize];

                        int_t    offset = 0;
                        int64_t  conceptualFaceCnt = GetConceptualFaceCnt(owlInstance);
                        for (int_t k = 0; k < conceptualFaceCnt; k++) {
                            int64_t  startIndicesLines = 0, noIndicesLines = 0;
                            GetConceptualFaceEx(
                                    owlInstance, k,
                                    nullptr, nullptr,
                                    &startIndicesLines, &noIndicesLines,
                                    nullptr, nullptr,
                                    nullptr, nullptr,
                                    nullptr, nullptr
                                );
                            assert(noIndicesLines);

                            bool    hasNonZeroZCoordinate = false;
                            for (int_t m = 0; m < noIndicesLines; m++) {
                                if (vertices[3 * indices[startIndicesLines + m] + 2]) {
                                    hasNonZeroZCoordinate = true;
                                }
                            }
                            for (int_t m = 0; m < noIndicesLines; m++) {
                                ___VECTOR3  vec = {
                                                    vertices[3 * indices[startIndicesLines + m] + 0],
                                                    vertices[3 * indices[startIndicesLines + m] + 1],
                                                    vertices[3 * indices[startIndicesLines + m] + 2]
                                                };
        //??????????????                        assert(vec.z == 0.);

                                if (m == 0 && cnt) {
//                                    if (vecLast.x != vec.x || vecLast.y != vec.y || vecLast.z != vec.z) {
                                    if (std::fabs(vecLast.x - vec.x) > 0.0000000000001 ||
                                        std::fabs(vecLast.y - vec.y) > 0.0000000000001 ||
                                        std::fabs(vecLast.z - vec.z) > 0.0000000000001) {
                                        assert(false);
                                        fprintf(fp, "INTERNAL ISSUE\n");
                                    }
                                }
                                else {
                                    if (hasNonZeroZCoordinate) {
                                        fprintf(fp, "%i\t%.16f\t%.16f\t%.16f\n", cnt, vec.x, vec.y, vec.z);
                                    }
                                    else {
                                        fprintf(fp, "%.9f\t%.16f\t%.16f\n", std::fabs(curveLength) * (double) (k + m) / (double) conceptualFaceCnt, vec.x, vec.y);
                                    }
                                    cnt++;
                                    vecLast = vec;
                                }
                            }
                        }
                    }
                    else {
                        assert(false);
                    }
//                }
            }

            fclose(fp);
        }

        cleanMemory(ifcModel, 4);
        sdaiCloseModel(ifcModel);
    }
}

void    GeneratePointList__INDIRECT(char * generatedFileName, char * pointListFileName, int_t segmentationParts, char * entityNameCurve, double FACTOR_Y)
{
    int_t   ifcModel = sdaiOpenModelBN(0, generatedFileName, "");

    {
        FILE    * fp = nullptr;
        fopen_s(&fp, pointListFileName, "w");

        if (fp) {
            setSegmentation(ifcModel, segmentationParts, 0.);

            int64_t owlModel = 0;
            owlGetModel(ifcModel, &owlModel);
            int64_t mask = GetFormat(0, 0),
                    setting = flagbit2 + flagbit4 + flagbit9;
            SetFormat(owlModel, setting, mask);

            int_t   * ifcCompositeCurveInstances = sdaiGetEntityExtentBN(ifcModel, entityNameCurve),
                    noIfcCompositeCurveInstances = sdaiGetMemberCount(ifcCompositeCurveInstances);
            for (int_t i = 0; i < noIfcCompositeCurveInstances; i++) {
                int_t   ifcCompositeCurveInstance = 0;
                engiGetAggrElement(ifcCompositeCurveInstances, i, sdaiINSTANCE, &ifcCompositeCurveInstance);
            
                ___VECTOR3  vecLast;
                int        cnt = 0;

                int_t   * ifcSegmentInstances = nullptr;
                sdaiGetAttrBN(ifcCompositeCurveInstance, "Segments", sdaiAGGR, &ifcSegmentInstances);
                int_t   noIfcSegmentInstances = sdaiGetMemberCount(ifcSegmentInstances);
                for (int_t j = 0; j < noIfcSegmentInstances; j++) {
                    int_t   ifcSegmentInstance = 0;
                    engiGetAggrElement(ifcSegmentInstances, j, sdaiINSTANCE, &ifcSegmentInstance);

                    double  segmentLength = 0.;
                    sdaiGetAttrBN(ifcSegmentInstance, "SegmentLength", sdaiREAL, &segmentLength);



                    int64_t owlInstance = 0;
         //           owlBuildInstance(ifcModel, ifcSegmentInstance, &owlInstance);
//                    owlBuildInstance(ifcModel, ifcCompositeCurveInstance, &owlInstance);
                    owlBuildInstanceInContext(ifcSegmentInstance, ifcCompositeCurveInstance, &owlInstance);

                    int64_t vertexBufferSize = 0, indexBufferSize = 0;
                    CalculateInstance(owlInstance, &vertexBufferSize, &indexBufferSize, nullptr);
                    if (vertexBufferSize && indexBufferSize) {
                        double  * vertices = new double[3 * (int_t) vertexBufferSize];
                        UpdateInstanceVertexBuffer(owlInstance, vertices);

                        int32_t * indices = new int32_t[(int_t) indexBufferSize];
                        UpdateInstanceIndexBuffer(owlInstance, indices);

                        ___VECTOR3  * pPnt = new ___VECTOR3[(int_t) indexBufferSize];

                        int_t    offset = 0;
                        int64_t  conceptualFaceCnt = GetConceptualFaceCnt(owlInstance);
                        for (int_t k = 0; k < conceptualFaceCnt; k++) {
                            int64_t  startIndicesLines = 0, noIndicesLines = 0;
                            GetConceptualFaceEx(
                                    owlInstance, k,
                                    nullptr, nullptr,
                                    &startIndicesLines, &noIndicesLines,
                                    nullptr, nullptr,
                                    nullptr, nullptr,
                                    nullptr, nullptr
                                );
                            assert(noIndicesLines);

                            bool    hasNonZeroZCoordinate = false;
                            for (int_t m = 0; m < noIndicesLines; m++) {
                                if (vertices[3 * indices[startIndicesLines + m] + 2]) {
                                    hasNonZeroZCoordinate = true;
                                }
                            }
                            for (int_t m = 0; m < noIndicesLines; m++) {
                                ___VECTOR3  vec = {
                                                    vertices[3 * indices[startIndicesLines + m] + 0],
                                                    vertices[3 * indices[startIndicesLines + m] + 1],
                                                    vertices[3 * indices[startIndicesLines + m] + 2]
                                                };
        //??????????????                        assert(vec.z == 0.);

                                if (m == 0 && cnt) {
//                                    if (vecLast.x != vec.x || vecLast.y != vec.y || vecLast.z != vec.z) {
                                    if (std::fabs(vecLast.x - vec.x) > 0.0000000000001 ||
                                        std::fabs(vecLast.y - vec.y) > 0.0000000000001 ||
                                        std::fabs(vecLast.z - vec.z) > 0.0000000000001) {
                                        assert(false);
                                        fprintf(fp, "INTERNAL ISSUE\n");
                                    }
                                }
                                else {
                                    if (hasNonZeroZCoordinate) {
                                        fprintf(fp, "%.1f\t%.16f\t%.16f\t%.16f\n", segmentLength * (double) (k + m) / (double) conceptualFaceCnt, vec.x, vec.y, vec.z);
                                    }
                                    else {
                                        fprintf(fp, "%.10f\t%.10f\n", vec.x, vec.y * FACTOR_Y);
                                    }
                                    cnt++;
                                    vecLast = vec;
                                }
                            }
                        }
                    }
                    else {
                        assert(false);
                    }
                }
            }

            fclose(fp);
        }

        cleanMemory(ifcModel, 4);
        sdaiCloseModel(ifcModel);
    }
}

int     main(int argc, char *argv[], char *envp[])
{
    if (argc >= 3) {
        int_t   alignmentInstanceCnt = ConvertFile(argv[1], argv[2]);

        if (alignmentInstanceCnt) {
            std::cout << "File '" << argv[1] << "' converted (into '" << argv[2] << "') and found " << alignmentInstanceCnt << " alignment instances.\n";
        }
        else {
            std::cout << "File '" << argv[1] << "' not found, or no alignment instances without geometry.\n";
        }

        CheckResults(argv[2]);

        if (argc >= 4) {
            if (argc >= 6) {
                if ((argv[5])[0] == 'h' || (argv[5])[0] == 'H') {
                    GeneratePointList__DIRECT(
                            argv[2],
                            argv[3],
                            (argc >= 5) ? atoi(argv[4]) : 36,
                            (char*) "IFCCOMPOSITECURVE"
                        );
                }
                else if ((argv[5])[0] == 'v' || (argv[5])[0] == 'V') {
                    GeneratePointList__DIRECT(
                            argv[2],
                            argv[3],
                            (argc >= 5) ? atoi(argv[4]) : 36,
                            (char*) "IFCGRADIENTCURVE"
                        );
                }
                else {
                    GeneratePointList__INDIRECT(
                            argv[2],
                            argv[3],
                            (argc >= 5) ? atoi(argv[4]) : 36,
                            (char*) "IFCSEGMENTEDREFERENCECURVE",
                            2.
                        );
                }
            }
            else {
                GeneratePointList__DIRECT(
                        argv[2],
                        argv[3],
                        (argc >= 5) ? atoi(argv[4]) : 36,
                        (char*) "IFCCOMPOSITECURVE"
                    );
            }
        }
    }

    std::cout << "Ready!\n";
}
