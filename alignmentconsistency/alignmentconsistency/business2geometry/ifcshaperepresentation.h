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

static	inline	SdaiInstance    ___CreateShapeRepresentationInstance(
                                        SdaiModel       model,
                                        SdaiInstance    ifcRepresentationItemInstance,
                                        SdaiAggr        * pAggrItems,
                                        bool            is3DCurve
                                    )
{
    SdaiInstance    ifcShapeRepresentationInstance = sdaiCreateInstanceBN(model, "IFCSHAPEREPRESENTATION");

/////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!????????????????????    sdaiPutAttrBN(ifcShapeRepresentationInstance, "ContextOfItems", sdaiINSTANCE, (void*) ___CreateGeometricRepresentationContext(model));

    char    representationIdentifier[5] = "Axis";
    sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationIdentifier", sdaiSTRING, representationIdentifier);

    if (is3DCurve) {
        char    representationType[8] = "Curve3D";
        sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationType", sdaiSTRING, representationType);
    }
    else {
        char    representationType[8] = "Curve2D";
        sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationType", sdaiSTRING, representationType);
    }

	SdaiAggr    aggrItems = sdaiCreateAggrBN(ifcShapeRepresentationInstance, "Items");
	sdaiAppend(aggrItems, sdaiINSTANCE, (void*) ifcRepresentationItemInstance);

//    assert(ifcRepresentationItemInstance);

    if (pAggrItems) {
        (*pAggrItems) = aggrItems;
    }

	assert(ifcShapeRepresentationInstance);
	return	ifcShapeRepresentationInstance;
}

static	inline	SdaiInstance    ___CreateShapeRepresentationInstance(
                                        SdaiModel       model,
                                        SdaiInstance    ifcRepresentationItemInstance,
                                        bool            is3DCurve
                                    )
{
    return  ___CreateShapeRepresentationInstance(
                    model,
                    ifcRepresentationItemInstance,
                    nullptr,
                    is3DCurve
                );
}
