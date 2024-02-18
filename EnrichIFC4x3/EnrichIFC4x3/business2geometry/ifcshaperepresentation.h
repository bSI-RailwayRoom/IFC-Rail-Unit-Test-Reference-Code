#pragma once


#include "ifcaxis2placement3d.h"


extern  SdaiInstance   reusedIfcGeometricRepresentationContextInstance;


static	inline	SdaiInstance    ___CreateObjectPlacement(
                                        SdaiModel       model
                                    )
{
    SdaiInstance   ifcObjectPlacementInstance = sdaiCreateInstanceBN(model, "IFCLOCALPLACEMENT");

    sdaiPutAttrBN(ifcObjectPlacementInstance, "RelativePlacement", sdaiINSTANCE, (void*) ___CreateAxis2Placement3DInstance(model));

    assert(ifcObjectPlacementInstance);
    return	ifcObjectPlacementInstance;
}

static	inline	SdaiInstance    ___CreateGeometricRepresentationContext(
                                        SdaiModel       model
                                    )
{
    if (reusedIfcGeometricRepresentationContextInstance) {
        return  reusedIfcGeometricRepresentationContextInstance;
    }

    SdaiInstance    ifcGeometricRepresentationContextInstance = sdaiCreateInstanceBN(model, "IFCGEOMETRICREPRESENTATIONCONTEXT");

    char    contextType[6] = "Model";
    sdaiPutAttrBN(ifcGeometricRepresentationContextInstance, "ContextType", sdaiSTRING, contextType);

    int_t   coordinateSpaceDimension = 3;
    sdaiPutAttrBN(ifcGeometricRepresentationContextInstance, "CoordinateSpaceDimension", sdaiINTEGER, &coordinateSpaceDimension);

    double  precision = 0.000001;
	sdaiPutAttrBN(ifcGeometricRepresentationContextInstance, "Precision", sdaiREAL, &precision);
	sdaiPutAttrBN(ifcGeometricRepresentationContextInstance, "WorldCoordinateSystem", sdaiINSTANCE, (void*) ___CreateAxis2Placement3DInstance(model));

    reusedIfcGeometricRepresentationContextInstance = ifcGeometricRepresentationContextInstance;

    assert(ifcGeometricRepresentationContextInstance);
    return	ifcGeometricRepresentationContextInstance;
}

static	inline	SdaiInstance    ___FindGeometricRepresentationContext(
                                        SdaiModel       model
                                    )
{
    SdaiAggr    ifcGeometricRepresentationContextInstances = sdaiGetEntityExtentBN(model, "IFCGEOMETRICREPRESENTATIONCONTEXT");
    if (sdaiGetMemberCount(ifcGeometricRepresentationContextInstances) == 1) {
        SdaiInstance   ifcGeometricRepresentationContextInstance = 0;
        sdaiGetAggrByIndex(ifcGeometricRepresentationContextInstances, 0, sdaiINSTANCE, &ifcGeometricRepresentationContextInstance);
        return  ifcGeometricRepresentationContextInstance;
    }

    return  ___CreateGeometricRepresentationContext(model);
}

//
//  "Curve2D"
//  "Curve3D"
//  "Segment"
//
static	inline	SdaiInstance    ___CreateShapeRepresentationInstance(
                                        SdaiModel       model,
                                        SdaiInstance    ifcRepresentationItemInstance,
                                        SdaiAggr        * pAggrItems,
                                        const char      * representationType
                                    )
{
    SdaiInstance    ifcShapeRepresentationInstance = sdaiCreateInstanceBN(model, "IFCSHAPEREPRESENTATION");

    sdaiPutAttrBN(ifcShapeRepresentationInstance, "ContextOfItems", sdaiINSTANCE, (void*) ___FindGeometricRepresentationContext(model));

    char    representationIdentifier[5] = "Axis";
    sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationIdentifier", sdaiSTRING, representationIdentifier);

    sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationType", sdaiSTRING, representationType);

	SdaiAggr    aggrItems = sdaiCreateAggrBN(ifcShapeRepresentationInstance, "Items");
	sdaiAppend(aggrItems, sdaiINSTANCE, (void*) ifcRepresentationItemInstance);

//    assert(ifcRepresentationItemInstance);

    if (pAggrItems) {
        (*pAggrItems) = aggrItems;
    }

	assert(ifcShapeRepresentationInstance);
	return	ifcShapeRepresentationInstance;
}

//
//  "Curve2D"
//  "Curve3D"
//  "Segment"
//
static	inline	SdaiInstance    ___CreateShapeRepresentationInstance(
                                        SdaiModel       model,
                                        SdaiInstance    ifcRepresentationItemInstance,
                                        const char      * representationType
                                    )
{
    return  ___CreateShapeRepresentationInstance(
                    model,
                    ifcRepresentationItemInstance,
                    nullptr,
                    representationType
                );
}
