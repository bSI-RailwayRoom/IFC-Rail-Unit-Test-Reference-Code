#pragma once


#include "ifcalignmentcant.h"
#include "ifcalignmenthorizontal.h"
#include "ifcalignmentvertical.h"
#include "ifcderivedprofiledef.h"
#include "ifcfixedreferencesweptareasolid.h"
#include "ifcproject.h"


extern  SdaiInstance   reusedIfcGeometricRepresentationContextInstance;


static  inline  SdaiInstance    AlignmentGenerateGeometry(
		                                SdaiModel	   model,
		                                SdaiInstance   ifcAlignmentInstance
		                            )
{
    SdaiAggr        ifcGeometricRepresentationContextInstances = sdaiGetEntityExtentBN(model, "IFCGEOMETRICREPRESENTATIONCONTEXT");
    SdaiInstance    noIfcGeometricRepresentationContextInstances = sdaiGetMemberCount(ifcGeometricRepresentationContextInstances);
    if (noIfcGeometricRepresentationContextInstances)
        sdaiGetAggrByIndex(ifcGeometricRepresentationContextInstances, 0, sdaiINSTANCE, &reusedIfcGeometricRepresentationContextInstance);

    bool    hasAlignmentHorizontal =
                (___GetAlignmentHorizontal(
                        model,
                        ifcAlignmentInstance
                    )) ? true : false,
            hasAlignmentVertical =
                (___GetAlignmentVertical(
                        model,
                        ifcAlignmentInstance
                    )) ? true : false,
            hasAlignmentCant =
                (___GetAlignmentCant(
                        model,
                        ifcAlignmentInstance
                    )) ? true : false;

    assert(hasAlignmentHorizontal);

    SdaiInstance   ifcRepresentationItem_compositeCurveInstance =
		                ___CreateCompositeCurve__alignmentHorizontal(
		                        model,
		                        ifcAlignmentInstance,
		                        ___GetAlignmentHorizontal(
		                                model,
		                                ifcAlignmentInstance
		                            ),
		                        ___GetPlaneAngleUnitConversionFactor(
		                                model
		                            )
		                    ),
		            ifcRepresentationItem = ifcRepresentationItem_compositeCurveInstance;

    //
    //  Add geometry for IfcHorizontalAlignment
    //
    sdaiPutAttrBN(
            ___GetAlignmentHorizontal(
                    model,
                    ifcAlignmentInstance,
                    nullptr
                ),
            "ObjectPlacement",
            sdaiINSTANCE,
            (void*) ___CreateObjectPlacement(
                            model
                        )
        );

    if (hasAlignmentVertical || hasAlignmentCant) {
        double  startDistAlongHorizontalAlignment = 0.;
        
        sdaiGetAttrBN(
                ___GetAlignmentHorizontal(
                        model,
                        ifcAlignmentInstance
                    ),
                "StartDistAlong",
                sdaiREAL,
                &startDistAlongHorizontalAlignment
            );

		if (hasAlignmentVertical) {
            SdaiInstance    ifcRepresentationItem_gradientCurveInstance =
			                    ___CreateGradientCurve__alignmentVertical(
			                            model,
			                            ___GetAlignmentVertical(
			                                    model,
			                                    ifcAlignmentInstance
			                                ),
			                            startDistAlongHorizontalAlignment
			                        );

            assert(ifcRepresentationItem_gradientCurveInstance);

            sdaiPutAttrBN(
                    ifcRepresentationItem_gradientCurveInstance,
                    "BaseCurve",
                    sdaiINSTANCE,
                    (void*) ifcRepresentationItem_compositeCurveInstance
                );

            //
            //  Add geometry for IfcVerticalAlignment
            //
            sdaiPutAttrBN(
                    ___GetAlignmentVertical(
                            model,
                            ifcAlignmentInstance
                        ),
                    "ObjectPlacement",
                    sdaiINSTANCE,
                    (void*) ___CreateObjectPlacement(
                                    model
                                )
                );
				
			ifcRepresentationItem = ifcRepresentationItem_gradientCurveInstance;
		}

        if (hasAlignmentCant) {
            SdaiInstance	ifcRepresentationItem_segmentedReferenceCurveInstance =
                                ___CreateSegmentedReferenceCurve__alignmentCant(
                                        model,
                                        ___GetAlignmentCant(
                                                model,
                                                ifcAlignmentInstance
                                            ),
                                        startDistAlongHorizontalAlignment
                                    );

            assert(ifcRepresentationItem_segmentedReferenceCurveInstance);

            sdaiPutAttrBN(
                    ifcRepresentationItem_segmentedReferenceCurveInstance,
                    "BaseCurve",
                    sdaiINSTANCE,
                    (void*) ifcRepresentationItem
                );


            //
            //  Add geometry for IfcCantAlignment
            //
            sdaiPutAttrBN(
                    ___GetAlignmentCant(
                            model,
                            ifcAlignmentInstance
                        ),
                    "ObjectPlacement",
                    sdaiINSTANCE,
                    (void*) ___CreateObjectPlacement(
                                    model
                                )
                );

			ifcRepresentationItem = ifcRepresentationItem_segmentedReferenceCurveInstance;
        }
    }
    else {
        ifcRepresentationItem = ifcRepresentationItem_compositeCurveInstance;
    }

    //
    //  Add geometry for IfcAlignment
    //
    sdaiPutAttrBN(
            ifcAlignmentInstance,
            "ObjectPlacement",
            sdaiINSTANCE,
            (void*) ___CreateObjectPlacement(
                            model
                        )
        );

    if (ifcRepresentationItem) {
        assert(ifcAlignmentInstance);
        SdaiAggr    aggrRepresentations = nullptr;
        sdaiPutAttrBN(
                ifcAlignmentInstance,
                "Representation",
                sdaiINSTANCE,
                (void*) ___CreateProductDefinitionShapeInstance(
                                model,
                                &aggrRepresentations
                            )
            );

        if (ifcRepresentationItem && ifcRepresentationItem != ifcRepresentationItem_compositeCurveInstance)
            sdaiAppend(aggrRepresentations, sdaiINSTANCE, (void*) ___CreateShapeRepresentationInstance(model, ifcRepresentationItem, "Curve3D"));

        sdaiAppend(aggrRepresentations, sdaiINSTANCE, (void*) ___CreateShapeRepresentationInstance(model, ifcRepresentationItem_compositeCurveInstance, "Curve2D"));
    }

    return  ifcRepresentationItem;
}

static  inline  SdaiInstance    AlignmentGenerateSweep(
		                                SdaiModel       model,
		                                SdaiInstance    ifcAlignmentInstance,
		                                SdaiInstance    ifcProfileInstance,
		                                SdaiInstance    ifcProductInstance
		                            )
{
    SdaiInstance    ifcFixedReferenceSweptAreaSolidInstance =
                        (___GetAlignmentCant(
                                model,
                                ifcAlignmentInstance,
                                nullptr
                            )) ?
                            ___CreateDirectrixDerivedReferenceSweptAreaSolidInstance(
                                    model,
                                    ___GetProductRepresentationItem(
                                            ifcAlignmentInstance
                                        ),
                                    ifcProfileInstance
                                ) :
                            ___CreateFixedReferenceSweptAreaSolidInstance(
                                    model,
                                    ___GetProductRepresentationItem(
                                            ifcAlignmentInstance
                                        ),
                                    ifcProfileInstance
                                );

    sdaiPutAttrBN(
            ifcProductInstance,
            "ObjectPlacement",
            sdaiINSTANCE,
            (void*) ___CreateObjectPlacement(
                            model
                        )
        );
		
    assert(ifcFixedReferenceSweptAreaSolidInstance && ifcProductInstance);
    sdaiPutAttrBN(
            ifcProductInstance,
            "Representation",
            sdaiINSTANCE,
            (void*) ___CreateProductDefinitionShapeInstance(
                            model,
                            ifcFixedReferenceSweptAreaSolidInstance,
                            "Curve3D"
                        )
        );

    return  ifcFixedReferenceSweptAreaSolidInstance;
}

static  inline  SdaiInstance    AlignmentGenerateSweep(
                                        SdaiModel       model,
                                        SdaiInstance    ifcAlignmentInstance,
                                        SdaiInstance    ifcProfileInstance,
                                        SdaiInstance    ifcProductInstance,
                                        double          gauge
                                    )
{
    SdaiInstance    ifcCompositeProfileDefInstance;
    SdaiAggr        aggrProfiles;
        
    ifcCompositeProfileDefInstance = sdaiCreateInstanceBN(model, "IFCCOMPOSITEPROFILEDEF");
 
    aggrProfiles = sdaiCreateAggrBN(ifcCompositeProfileDefInstance, "Profiles");

    sdaiAppend(aggrProfiles, sdaiINSTANCE, (void*) ___CreateDerivedProfileDefInstance(model, ifcProfileInstance, - (gauge / 2.)));
    sdaiAppend(aggrProfiles, sdaiINSTANCE, (void*) ___CreateDerivedProfileDefInstance(model, ifcProfileInstance,    gauge / 2.));

    return  AlignmentGenerateSweep(
                    model,
                    ifcAlignmentInstance,
                    ifcCompositeProfileDefInstance,
                    ifcProductInstance
                );
}