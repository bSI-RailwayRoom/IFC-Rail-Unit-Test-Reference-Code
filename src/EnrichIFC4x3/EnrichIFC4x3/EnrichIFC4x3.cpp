// EnrichIFC4x3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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


int_t    ConvertFile(char * inputFileName, char * generatedFileName)
{
    int_t   convertedInstances = 0,
            ifcModel = sdaiOpenModelBN(0, inputFileName, "");

    if (ifcModel) {
        int_t   * ifcAlignmentInstances = sdaiGetEntityExtentBN(ifcModel, "IFCALIGNMENT"),
                noIfcAlignmentInstances = sdaiGetMemberCount(ifcAlignmentInstances);
        if (noIfcAlignmentInstances) {
            for (int_t i = 0; i < noIfcAlignmentInstances; i++) {
                int_t   ifcAlignmentInstance = 0;
                engiGetAggrElement(ifcAlignmentInstances, i, sdaiINSTANCE, &ifcAlignmentInstance);

                int_t   ifcProductRepresentationInstance = 0;
                sdaiGetAttrBN(ifcAlignmentInstance, "Representation", sdaiINSTANCE, &ifcProductRepresentationInstance);

                if (!ifcProductRepresentationInstance) {
                    AlignmentGenerateGeometry(
                            ifcModel,
                            ifcAlignmentInstance
                        );
                    convertedInstances++;
                }
            }
        }
        else {
            std::cout << "No alignment instances found.\n";
        }

        sdaiSaveModelBN(ifcModel, generatedFileName);

        cleanMemory(ifcModel, 4);
        sdaiCloseModel(ifcModel);
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

        setSegmentation(ifcModel, 100, 0.);

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

            VECTOR3 * pStartPoint = new VECTOR3[noIfcSegmentInstances];
            VECTOR3 * pStartVector = new VECTOR3[noIfcSegmentInstances];
            VECTOR3 * pEndPoint = new VECTOR3[noIfcSegmentInstances];
            VECTOR3 * pEndVector = new VECTOR3[noIfcSegmentInstances];

            for (int_t j = 0; j < noIfcSegmentInstances; j++) {
                int_t   ifcSegmentInstance = 0;
                engiGetAggrElement(ifcSegmentInstances, j, sdaiINSTANCE, &ifcSegmentInstance);

                int64_t owlInstance = 0;
                owlBuildInstance(ifcModel, ifcSegmentInstance, &owlInstance);

                int64_t vertexBufferSize = 0, indexBufferSize = 0;
                CalculateInstance(owlInstance, &vertexBufferSize, &indexBufferSize, nullptr);
                if (vertexBufferSize && indexBufferSize) {
                    double  * vertices = new double[3 * (int_t) vertexBufferSize];
                    UpdateInstanceVertexBuffer(owlInstance, vertices);

                    int32_t * indices = new int32_t[(int_t) indexBufferSize];
                    UpdateInstanceIndexBuffer(owlInstance, indices);

                    VECTOR3 * pPnt = new VECTOR3[(int_t) indexBufferSize];

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

                    VECTOR3 * pnt0 = &pPnt[0],
                            * pnt1 = &pPnt[1],
                            * pntNm2 = &pPnt[offset - 2],
                            * pntNm1 = &pPnt[offset - 1];
                    pStartPoint[j].x = pnt0->x;
                    pStartPoint[j].y = pnt0->y;
                    pStartPoint[j].z = pnt0->z;
                    Vec3Subtract(&pStartVector[j], pnt1, pnt0);
                    Vec3Normalize(&pStartVector[j]);
                    pEndPoint[j].x = pntNm1->x;
                    pEndPoint[j].y = pntNm1->y;
                    pEndPoint[j].z = pntNm1->z;
                    Vec3Subtract(&pEndVector[j], pntNm1, pntNm2);
                    Vec3Normalize(&pEndVector[j]);
                }
                else {
                    assert(false);
                }
            }

            for (int_t j = 1; j < noIfcSegmentInstances; j++) {
                VECTOR3 vecDiff;
                Vec3Subtract(&vecDiff, &pEndPoint[j - 1], &pStartPoint[j]);
                    
                double  epsilon = 0.001,
                        distance = Vec3Length(&vecDiff);
                if (std::fabs(distance) > epsilon) {
                    std::cout << "  ISSUE HORIZONTAL ALIGNMENT distance end / start point too large, distance : " << distance << " (epsilon : " << epsilon << ").\n";
                }

                double  dotProduct = Vec3Dot(&pEndVector[j - 1], &pStartVector[j]);
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

            VECTOR3 * pStartPoint = new VECTOR3[noIfcSegmentInstances];
            VECTOR3 * pStartVector = new VECTOR3[noIfcSegmentInstances];
            VECTOR3 * pEndPoint = new VECTOR3[noIfcSegmentInstances];
            VECTOR3 * pEndVector = new VECTOR3[noIfcSegmentInstances];

            for (int_t j = 0; j < noIfcSegmentInstances; j++) {
                int_t   ifcSegmentInstance = 0;
                engiGetAggrElement(ifcSegmentInstances, j, sdaiINSTANCE, &ifcSegmentInstance);

                int64_t owlInstance = 0;
                owlBuildInstance(ifcModel, ifcSegmentInstance, &owlInstance);

                int64_t vertexBufferSize = 0, indexBufferSize = 0;
                CalculateInstance(owlInstance, &vertexBufferSize, &indexBufferSize, nullptr);
                if (vertexBufferSize && indexBufferSize) {
                    double  * vertices = new double[3 * (int_t) vertexBufferSize];
                    UpdateInstanceVertexBuffer(owlInstance, vertices);

                    int32_t * indices = new int32_t[(int_t) indexBufferSize];
                    UpdateInstanceIndexBuffer(owlInstance, indices);

                    VECTOR3 * pPnt = new VECTOR3[(int_t) indexBufferSize];

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

                    VECTOR3 * pnt0 = &pPnt[0],
                            * pnt1 = &pPnt[1],
                            * pntNm2 = &pPnt[offset - 2],
                            * pntNm1 = &pPnt[offset - 1];
                    pStartPoint[j].x = pnt0->x;
                    pStartPoint[j].y = pnt0->y;
                    pStartPoint[j].z = pnt0->z;
                    Vec3Subtract(&pStartVector[j], pnt1, pnt0);
                    Vec3Normalize(&pStartVector[j]);
                    pEndPoint[j].x = pntNm1->x;
                    pEndPoint[j].y = pntNm1->y;
                    pEndPoint[j].z = pntNm1->z;
                    Vec3Subtract(&pEndVector[j], pntNm1, pntNm2);
                    Vec3Normalize(&pEndVector[j]);
                }
                else {
                    assert(false);
                }
            }

            for (int_t j = 1; j < noIfcSegmentInstances; j++) {
                VECTOR3 vecDiff;
                Vec3Subtract(&vecDiff, &pEndPoint[j - 1], &pStartPoint[j]);
                    
                double  epsilon = 0.05, //   0.001,
                        distance = Vec3Length(&vecDiff);
                if (std::fabs(distance) > epsilon) {
                    std::cout << "  ISSUE VERTICAL ALIGNMENT distance end / start point too large, distance : " << distance << " (epsilon : " << epsilon << ").\n";
                }

                epsilon = 0.001;
                double  dotProduct = Vec3Dot(&pEndVector[j - 1], &pStartVector[j]);
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

            VECTOR3 * pStartPoint = new VECTOR3[noIfcSegmentInstances];
            VECTOR3 * pStartVector = new VECTOR3[noIfcSegmentInstances];
            VECTOR3 * pEndPoint = new VECTOR3[noIfcSegmentInstances];
            VECTOR3 * pEndVector = new VECTOR3[noIfcSegmentInstances];

            for (int_t j = 0; j < noIfcSegmentInstances; j++) {
                int_t   ifcSegmentInstance = 0;
                engiGetAggrElement(ifcSegmentInstances, j, sdaiINSTANCE, &ifcSegmentInstance);

                int64_t owlInstance = 0;
                owlBuildInstance(ifcModel, ifcSegmentInstance, &owlInstance);

                int64_t vertexBufferSize = 0, indexBufferSize = 0;
                CalculateInstance(owlInstance, &vertexBufferSize, &indexBufferSize, nullptr);
                if (vertexBufferSize && indexBufferSize) {
                    double  * vertices = new double[3 * (int_t) vertexBufferSize];
                    UpdateInstanceVertexBuffer(owlInstance, vertices);

                    int32_t * indices = new int32_t[(int_t) indexBufferSize];
                    UpdateInstanceIndexBuffer(owlInstance, indices);

                    VECTOR3 * pPnt = new VECTOR3[(int_t) indexBufferSize];

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

                    VECTOR3 * pnt0 = &pPnt[0],
                            * pnt1 = &pPnt[1],
                            * pntNm2 = &pPnt[offset - 2],
                            * pntNm1 = &pPnt[offset - 1];
                    pStartPoint[j].x = pnt0->x;
                    pStartPoint[j].y = pnt0->y;
                    pStartPoint[j].z = pnt0->z;
                    Vec3Subtract(&pStartVector[j], pnt1, pnt0);
                    Vec3Normalize(&pStartVector[j]);
                    pEndPoint[j].x = pntNm1->x;
                    pEndPoint[j].y = pntNm1->y;
                    pEndPoint[j].z = pntNm1->z;
                    Vec3Subtract(&pEndVector[j], pntNm1, pntNm2);
                    Vec3Normalize(&pEndVector[j]);
                }
                else {
                    assert(false);
                }
            }

            for (int_t j = 1; j < noIfcSegmentInstances; j++) {
                VECTOR3 vecDiff;
                Vec3Subtract(&vecDiff, &pEndPoint[j - 1], &pStartPoint[j]);
                    
                double  epsilon = 0.05, //   0.001,
                        distance = Vec3Length(&vecDiff);
                if (std::fabs(distance) > epsilon) {
                    std::cout << "  ISSUE CANT ALIGNMENT distance end / start point too large, distance : " << distance << " (epsilon : " << epsilon << ").\n";
                }

                epsilon = 0.001;
                double  dotProduct = Vec3Dot(&pEndVector[j - 1], &pStartVector[j]);
                if (dotProduct <= 1. - epsilon || dotProduct >= 1. + epsilon) {
                    std::cout << "  ISSUE CANT ALIGNMENT difference angle end / start point too large, dotProduct : " << dotProduct << " (epsilon : " << epsilon << ").\n";
                }
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

void    GeneratePointList(char * generatedFileName, char * pointListFileName, int_t segmentationParts)
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

            int_t   * ifcCompositeCurveInstances = sdaiGetEntityExtentBN(ifcModel, "IFCCOMPOSITECURVE"),
                    noIfcCompositeCurveInstances = sdaiGetMemberCount(ifcCompositeCurveInstances);
            for (int_t i = 0; i < noIfcCompositeCurveInstances; i++) {
                int_t   ifcCompositeCurveInstance = 0;
                engiGetAggrElement(ifcCompositeCurveInstances, i, sdaiINSTANCE, &ifcCompositeCurveInstance);
            
                VECTOR3 vecLast;
                int     cnt = 0;

                int_t   * ifcSegmentInstances = nullptr;
                sdaiGetAttrBN(ifcCompositeCurveInstance, "Segments", sdaiAGGR, &ifcSegmentInstances);
                int_t   noIfcSegmentInstances = sdaiGetMemberCount(ifcSegmentInstances);
                for (int_t j = 0; j < noIfcSegmentInstances; j++) {
                    int_t   ifcSegmentInstance = 0;
                    engiGetAggrElement(ifcSegmentInstances, j, sdaiINSTANCE, &ifcSegmentInstance);

                    int64_t owlInstance = 0;
                    owlBuildInstance(ifcModel, ifcSegmentInstance, &owlInstance);

                    int64_t vertexBufferSize = 0, indexBufferSize = 0;
                    CalculateInstance(owlInstance, &vertexBufferSize, &indexBufferSize, nullptr);
                    if (vertexBufferSize && indexBufferSize) {
                        double  * vertices = new double[3 * (int_t) vertexBufferSize];
                        UpdateInstanceVertexBuffer(owlInstance, vertices);

                        int32_t * indices = new int32_t[(int_t) indexBufferSize];
                        UpdateInstanceIndexBuffer(owlInstance, indices);

                        VECTOR3 * pPnt = new VECTOR3[(int_t) indexBufferSize];

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

                            for (int_t m = 0; m < noIndicesLines; m++) {
                                VECTOR3 vec = {
                                                vertices[3 * indices[startIndicesLines + m] + 0],
                                                vertices[3 * indices[startIndicesLines + m] + 1],
                                                vertices[3 * indices[startIndicesLines + m] + 2]
                                            };
                                assert(vec.z == 0.);

                                if (m == 0 && cnt) {
                                    if (vecLast.x != vec.x || vecLast.y != vec.y || vecLast.z != vec.z) {
                                        fprintf(fp, "INTERNAL ISSUE\n");
                                    }
                                }
                                else {
                                    fprintf(fp, "%i\t%9.8f\t%9.8f\n", cnt++, vec.x, vec.y);
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
            GeneratePointList(
                    argv[2],
                    argv[3],
                    (argc >= 5) ? atoi(argv[4]) : 36
                );
        }
    }

    std::cout << "Ready!\n";
}
