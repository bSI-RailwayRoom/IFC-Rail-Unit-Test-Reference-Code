// addCheck.cpp : Defines the initialization routines for the DLL.
//

#include "pch.h"
#include "mathematicsGeometryDoublePrecision.h"
#include "processing.h"

#include <assert.h>
#include <ctime>

#include "./ifcengine/include/engine.h"


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


int_t	getTimeStamp()
{
    int_t   timeStamp = (int_t) time(0);

	return	timeStamp;
}

int_t	buildPersonInstance(int_t model)
{
    int_t	ifcPersonInstance = sdaiCreateInstanceBN(model, (char*) "IFCPERSON");

	sdaiPutAttrBN(ifcPersonInstance, (char*) "GivenName", sdaiSTRING, (char*) "Peter");
	sdaiPutAttrBN(ifcPersonInstance, (char*) "FamilyName", sdaiSTRING, (char*) "Bonsma");

	return	ifcPersonInstance;
}

int_t	buildPersonAndOrganizationInstance(int_t model, int_t ifcOrganizationInstance)
{
    int_t	ifcPersonAndOrganizationInstance = sdaiCreateInstanceBN(model, (char*) "IFCPERSONANDORGANIZATION");

	sdaiPutAttrBN(ifcPersonAndOrganizationInstance, (char*) "ThePerson", sdaiINSTANCE, (void*) buildPersonInstance(model));
	sdaiPutAttrBN(ifcPersonAndOrganizationInstance, (char*) "TheOrganization", sdaiINSTANCE, (void*) ifcOrganizationInstance);

	return	ifcPersonAndOrganizationInstance;
}


int_t	buildOrganizationInstance(int_t model)
{
    int_t    ifcOrganizationInstance = sdaiCreateInstanceBN(model, (char*) "IFCORGANIZATION");

    sdaiPutAttrBN(ifcOrganizationInstance, (char*) "Name", sdaiUNICODE, (char*) L"АР ДИ ЕФ ООД");
    sdaiPutAttrBN(ifcOrganizationInstance, (char*) "Description", sdaiSTRING, (char*) "RDF Ltd.");

    return	ifcOrganizationInstance;
}

int_t	buildApplicationInstance(int_t model, int_t ifcOrganizationInstance)
{
    int_t   ifcApplicationInstance = sdaiCreateInstanceBN(model, (char*) "IFCAPPLICATION");

	sdaiPutAttrBN(ifcApplicationInstance, (char*) "ApplicationDeveloper", sdaiINSTANCE, (void*) ifcOrganizationInstance);
	sdaiPutAttrBN(ifcApplicationInstance, (char*) "Version", sdaiSTRING, (char*) "0.99");
	sdaiPutAttrBN(ifcApplicationInstance, (char*) "ApplicationFullName", sdaiSTRING, (char*) "Add Check Library Application");
	sdaiPutAttrBN(ifcApplicationInstance, (char*) "ApplicationIdentifier", sdaiSTRING, (char*) "TA 1001");

	return	ifcApplicationInstance;
}

int_t	buildOwnerHistoryInstance(int_t model)
{
	int_t   ifcOwnerHistoryInstance = sdaiCreateInstanceBN(model, (char*) "IFCOWNERHISTORY"),
            ifcOrganizationInstance = buildOrganizationInstance(model);

	sdaiPutAttrBN(ifcOwnerHistoryInstance, (char*) "OwningUser", sdaiINSTANCE, (void*) buildPersonAndOrganizationInstance(model, ifcOrganizationInstance));
	sdaiPutAttrBN(ifcOwnerHistoryInstance, (char*) "OwningApplication", sdaiINSTANCE, (void*) buildApplicationInstance(model, ifcOrganizationInstance));
	sdaiPutAttrBN(ifcOwnerHistoryInstance, (char*) "ChangeAction", sdaiENUM, (char*) "ADDED");
    int_t   timeStamp = getTimeStamp();
    sdaiPutAttrBN(ifcOwnerHistoryInstance, (char*) "LastModifiedDate", sdaiINTEGER, (void*) &timeStamp);
    sdaiPutAttrBN(ifcOwnerHistoryInstance, (char*) "CreationDate", sdaiINTEGER, (void*) &timeStamp);

	return	ifcOwnerHistoryInstance;
}

//
//
//


int_t	buildPropertySingleValueInstance(int_t model, double length)
{
    int_t   ifcPropertySingleValueInstance = sdaiCreateInstanceBN(model, (char*) "IFCPROPERTYSINGLEVALUE"),
            * nominalValueADB;

	sdaiPutAttrBN(ifcPropertySingleValueInstance, (char*) "Name", sdaiSTRING, (char*) "ValidationLength");
    nominalValueADB = (int_t*) sdaiCreateADB(sdaiREAL, &length);
    sdaiPutADBTypePath(nominalValueADB, 1, (char*) "IFCLENGTHMEASURE");
    sdaiPutAttrBN(ifcPropertySingleValueInstance, (char*) "NominalValue", sdaiADB, (void*) nominalValueADB);

    return	ifcPropertySingleValueInstance;
}

int_t	buildPropertySetInstance(int_t model, int_t ifcOwnerHistoryInstance, double length)
{
    int_t   ifcPropertySetInstance = sdaiCreateInstanceBN(model, (char*) "IFCPROPERTYSET"),
            * aggrHasProperties;

    sdaiPutAttrBN(ifcPropertySetInstance, (char*) "GlobalId", engiGLOBALID, (void*) 0);
    sdaiPutAttrBN(ifcPropertySetInstance, (char*) "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
	sdaiPutAttrBN(ifcPropertySetInstance, (char*) "Name", sdaiSTRING, (char*) "ValidationPset");
    aggrHasProperties = sdaiCreateAggrBN(ifcPropertySetInstance, (char*) "HasProperties");
    sdaiAppend((int_t) aggrHasProperties, sdaiINSTANCE, (void*) buildPropertySingleValueInstance(model, length));

	return	ifcPropertySetInstance;
}

int_t   buildRelDefinedByPropertiesInstance(int_t model, int_t ifcObjectInstance, int_t ifcOwnerHistoryInstance, double length)
{
    int_t   ifcRelDefinedByPropertiesInstance = sdaiCreateInstanceBN(model, (char*) "IFCRELDEFINESBYPROPERTIES"),
            * aggrRelatedObjects;

    sdaiPutAttrBN(ifcRelDefinedByPropertiesInstance, (char*) "GlobalId", engiGLOBALID, (void*) 0);
    sdaiPutAttrBN(ifcRelDefinedByPropertiesInstance, (char*) "OwnerHistory", sdaiINSTANCE, (void*) ifcOwnerHistoryInstance);
    aggrRelatedObjects = sdaiCreateAggrBN(ifcRelDefinedByPropertiesInstance, (char*) "RelatedObjects");
    sdaiAppend((int_t) aggrRelatedObjects, sdaiINSTANCE, (void*) ifcObjectInstance);
    sdaiPutAttrBN(ifcRelDefinedByPropertiesInstance, (char*) "RelatingPropertyDefinition", sdaiINSTANCE, (void*) buildPropertySetInstance(model, ifcOwnerHistoryInstance, length));

    return  ifcRelDefinedByPropertiesInstance;
}


int_t    AddInstancePointList(int_t model, double * vertexBuffer, int32_t * lineVertexBuffer, int64_t totalNoIndicesLines)
{
    if (totalNoIndicesLines) {
        bool    as3D = false;
        for (int_t i = 0; i < totalNoIndicesLines; i++) {
            if (vertexBuffer[3 * lineVertexBuffer[i] + 2]) {
                as3D = true;
            }
        }

        int_t   ifcCartesianPointListInstance;
        if (as3D) {
            int_t   ifcCartesianPointList3DInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANPOINTLIST3D");
            
            int_t   * aggrCoordList = sdaiCreateAggrBN(ifcCartesianPointList3DInstance, "CoordList");

            double  x = 0., y = 0., z = 0.;
            for (int_t i = 0; i < totalNoIndicesLines; i++) {
                if (i == 0 ||
                    x != vertexBuffer[3 * lineVertexBuffer[i] + 0] ||
                    y != vertexBuffer[3 * lineVertexBuffer[i] + 1] ||
                    z != vertexBuffer[3 * lineVertexBuffer[i] + 2]) {
                    x = vertexBuffer[3 * lineVertexBuffer[i] + 0];
                    y = vertexBuffer[3 * lineVertexBuffer[i] + 1];
                    z = vertexBuffer[3 * lineVertexBuffer[i] + 2];
                    int_t   * aggrPoint = sdaiCreateAggr(ifcCartesianPointList3DInstance, nullptr);
                    sdaiAppend((int_t) aggrPoint, sdaiREAL, &x);
                    sdaiAppend((int_t) aggrPoint, sdaiREAL, &y);
                    sdaiAppend((int_t) aggrPoint, sdaiREAL, &z);

                    sdaiAppend((int_t) aggrCoordList, sdaiAGGR, (void*) aggrPoint);
                }
            }

            ifcCartesianPointListInstance = ifcCartesianPointList3DInstance;
        }
        else {
            int_t   ifcCartesianPointList2DInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANPOINTLIST2D");
            
            int_t   * aggrCoordList = sdaiCreateAggrBN(ifcCartesianPointList2DInstance, "CoordList");

            double  x = 0., y = 0.;
            for (int_t i = 0; i < totalNoIndicesLines; i++) {
                assert(vertexBuffer[3 * lineVertexBuffer[i] + 2] == 0.);
                if (i == 0 ||
                    x != vertexBuffer[3 * lineVertexBuffer[i] + 0] ||
                    y != vertexBuffer[3 * lineVertexBuffer[i] + 1]) {
                    x = vertexBuffer[3 * lineVertexBuffer[i] + 0];
                    y = vertexBuffer[3 * lineVertexBuffer[i] + 1];
                    int_t   * aggrPoint = sdaiCreateAggr(ifcCartesianPointList2DInstance, nullptr);
                    sdaiAppend((int_t) aggrPoint, sdaiREAL, &x);
                    sdaiAppend((int_t) aggrPoint, sdaiREAL, &y);

                    sdaiAppend((int_t) aggrCoordList, sdaiAGGR, (void*) aggrPoint);
                }
            }

            ifcCartesianPointListInstance = ifcCartesianPointList2DInstance;
        }

        //
        //      #516 = IFCINDEXEDPOLYCURVE(#515, $, $);
        //      #517 = IFCSHAPEREPRESENTATION(#498, 'Body-Fallback', 'Curve', (#516));
        //

        int_t   ifcIndexedPolyCurveInstance = sdaiCreateInstanceBN(model, "IFCINDEXEDPOLYCURVE");
        sdaiPutAttrBN(ifcIndexedPolyCurveInstance, "Points", sdaiINSTANCE, (void*) ifcCartesianPointListInstance);

        int_t   ifcShapeRepresentationInstance = sdaiCreateInstanceBN(model, "IFCSHAPEREPRESENTATION");

        {
            int_t   * ifcGeometricRepresentationContextInstances = sdaiGetEntityExtentBN(model, "IFCGEOMETRICREPRESENTATIONCONTEXT"),
                    noIfcGeometricRepresentationContextInstances = sdaiGetMemberCount(ifcGeometricRepresentationContextInstances);
            if (noIfcGeometricRepresentationContextInstances) {
                int_t ifcGeometricRepresentationContextInstance = 0;
                engiGetAggrElement(ifcGeometricRepresentationContextInstances, 0, sdaiINSTANCE, &ifcGeometricRepresentationContextInstance);

                sdaiPutAttrBN(ifcShapeRepresentationInstance, "ContextOfItems", sdaiINSTANCE, (void*) ifcGeometricRepresentationContextInstance);
            }
            else {
                assert(false);
            }
        }

        char    representationIdentifier[14] = "Body-Fallback";
        sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationIdentifier", sdaiSTRING, (void*) representationIdentifier);
        char    representationType[6] = "Curve";
        sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationType", sdaiSTRING, (void*) representationType);
        int_t   * aggrItems = sdaiCreateAggrBN(ifcShapeRepresentationInstance, "Items");
        sdaiAppend((int_t) aggrItems, sdaiINSTANCE, (void*) ifcIndexedPolyCurveInstance);

        return  ifcShapeRepresentationInstance;
    }

    return  0;
}

int_t	WalkInstanceGK(int_t model, int64_t myModelGK, int64_t myInstanceGK, double * pLength)
{
    int64_t setting = 0,
            mask = GetFormat(0, 0);

    setting += 1 * flagbit2;        //    SINGLE / DOUBLE PRECISION (float / double)
    setting += 0 * flagbit3;        //    32 / 63 BIT INDEX ARRAY (int32_t / int64_t)

    setting += 1 * flagbit4;        //    OFF / ON VECTORS (x, y, z) 
    setting += 0 * flagbit5;        //    OFF / ON NORMALS (Nx, Ny, Nz)

    setting += 1 * flagbit8;        //    OFF / ON TRIANGLES
    setting += 1 * flagbit9;        //    OFF / ON LINES
    setting += 1 * flagbit10;       //    OFF / ON POINTS

    setting += 0 * flagbit12;       //    OFF / ON WIREFRAME FACES
    setting += 0 * flagbit13;       //    OFF / ON WIREFRAME CONCEPTUAL FACES
    int64_t vertexElementSize = SetFormat(myModelGK, setting, mask);
    assert(vertexElementSize == 3 * sizeof(double));

    int64_t vertexBufferSize = 0, indexBufferSize = 0;
    CalculateInstance(myInstanceGK, &vertexBufferSize, &indexBufferSize, nullptr);
    if (vertexBufferSize && indexBufferSize) {
        double  * vertexBuffer = new double[3 * (int_t) vertexBufferSize];
        UpdateInstanceVertexBuffer(myInstanceGK, vertexBuffer);

        int32_t * indexBuffer = new int32_t[(int_t) indexBufferSize];
        UpdateInstanceIndexBuffer(myInstanceGK, indexBuffer);

        int64_t totalNoIndicesLines = 0, totalNoIndicesPoints = 0,
                conceptualFaceCnt = GetConceptualFaceCnt(myInstanceGK);
        for (int64_t i = 0; i < conceptualFaceCnt; i++) {
            int64_t startIndexLines = 0, noIndicesLines = 0,
                    startIndexPoints = 0, noIndicesPoints = 0;
            GetConceptualFaceEx(
                    myInstanceGK, i,
                    nullptr, nullptr,
                    &startIndexLines, &noIndicesLines,
                    &startIndexPoints, &noIndicesPoints,
                    nullptr, nullptr,
                    nullptr, nullptr
                );
            totalNoIndicesLines += noIndicesLines;
            totalNoIndicesPoints += noIndicesPoints;

            for (int_t j = 1; j < noIndicesLines; j++) {
                int32_t indexI = indexBuffer[startIndexLines + j - 1],
                        indexII = indexBuffer[startIndexLines + j];
                (*pLength) += Vec3Distance((VECTOR3*) &vertexBuffer[3 * indexI], (VECTOR3*) &vertexBuffer[3 * indexII]);
            }
        }

        if (totalNoIndicesLines && totalNoIndicesPoints == 0) {
            int32_t * lineVertexBuffer = new int32_t[(int_t) totalNoIndicesLines];
#ifdef _DEBUG
            int64_t tmpCNT = totalNoIndicesLines;
#endif // _DEBUG

            totalNoIndicesLines = 0;
            for (int64_t i = 0; i < conceptualFaceCnt; i++) {
                int64_t startIndexLines = 0, noIndicesLines = 0;
                GetConceptualFaceEx(
                        myInstanceGK, i,
                        nullptr, nullptr,
                        &startIndexLines, &noIndicesLines,
                        nullptr, nullptr,
                        nullptr, nullptr,
                        nullptr, nullptr
                    );
                memcpy(&lineVertexBuffer[totalNoIndicesLines], &indexBuffer[startIndexLines], (int_t) noIndicesLines * sizeof(int32_t));
                totalNoIndicesLines += noIndicesLines;
            }
            assert(tmpCNT == totalNoIndicesLines);



            return  AddInstancePointList(model, vertexBuffer, lineVertexBuffer, totalNoIndicesLines);
        }
    }

    return  0;
}

int_t	WalkInstance(int_t model, int_t myInstance, double * pLength)
{
    int64_t myModelGK = 0;
    owlGetModel(model, &myModelGK);

    int64_t myInstanceGK = 0;
    owlBuildInstance(model, myInstance, &myInstanceGK);

    return  WalkInstanceGK(model, myModelGK, myInstanceGK, pLength);
}

void	WalkEntityInstances(int_t model, char * entityName, int_t * pIfcOwnerHistoryInstance)
{
    int_t   * ifcEntityInstances = sdaiGetEntityExtentBN(model, entityName),
            noIfcEntityInstances = sdaiGetMemberCount(ifcEntityInstances);
    for (int_t i = 0; i < noIfcEntityInstances; i++) {
        int_t   ifcEntityInstance = 0;
        engiGetAggrElement(ifcEntityInstances, i, sdaiINSTANCE, &ifcEntityInstance);

        double  length = 0.;
        int_t   myInstance = WalkInstance(model, ifcEntityInstance, &length);

        if (myInstance) {
            int_t   ifcProductRepresentationInstance = 0;
            sdaiGetAttrBN(ifcEntityInstance, "Representation", sdaiINSTANCE, (void*) &ifcProductRepresentationInstance);

            if (sdaiGetInstanceType(ifcProductRepresentationInstance) == sdaiGetEntity(model, "IFCPRODUCTDEFINITIONSHAPE")) {
                int_t   * aggrRepresentations = nullptr;
                sdaiGetAttrBN(ifcProductRepresentationInstance, "Representations", sdaiAGGR, (void*) &aggrRepresentations);

                sdaiAppend((int_t) aggrRepresentations, sdaiINSTANCE, (void*) myInstance);

                //
                //  Add property
                //

                if ((*pIfcOwnerHistoryInstance) == 0) {
                    (*pIfcOwnerHistoryInstance) = buildOwnerHistoryInstance(model);
                }

                //                #506 = IFCPROPERTYSINGLEVALUE('ValidationLength', $, IFCLENGTHMEASURE(2478.06641545676), $);
                //                #507 = IFCPROPERTYSET('1IsA5zKhbDOxgGW0MKAD7E', #513, 'ValidationPset', $, (#506));
                //                #514 = IFCRELDEFINESBYPROPERTIES('0zVYybWMPB9OucQZFU9z1E', #513, $, $, (#71), #507);

                buildRelDefinedByPropertiesInstance(model, ifcEntityInstance, (*pIfcOwnerHistoryInstance), length);
            }
            else {
                assert(false);
            }
        }
    }
    assert(ifcEntityInstances);
}

void	WalkInstanceChildren(int_t model, int_t entityParent, int_t * pIfcOwnerHistoryInstance)
{
    int_t   cnt = engiGetEntityCount(model);
    for (int_t i = 0; i < cnt; i++) {
        int_t   ifcEntity = engiGetEntityElement(model, i);

        if (engiGetEntityParent(ifcEntity) == entityParent) {
            WalkInstanceChildren(model, ifcEntity, pIfcOwnerHistoryInstance);
        }
    }

    char    * entityName = 0;
    engiGetEntityName(entityParent, sdaiSTRING, &entityName);
    WalkEntityInstances(model, entityName, pIfcOwnerHistoryInstance);
}

void	WalkInstances(int_t model)
{
    setFilter(model, 1 + 2, 1 + 2 + 4);

    int_t   ifcOwnerHistoryInstance = 0;
    WalkInstanceChildren(model, (int_t) sdaiGetEntity(model, "IFCOBJECT"), &ifcOwnerHistoryInstance);
}
