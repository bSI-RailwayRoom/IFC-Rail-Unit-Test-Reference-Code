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
    if (noIfcGeometricRepresentationContextInstances) {
        sdaiGetAggrByIndex(ifcGeometricRepresentationContextInstances, 0, sdaiINSTANCE, &reusedIfcGeometricRepresentationContextInstance);
    }

    bool    hasAlignmentHorizontal =
                (___GetAlignmentHorizontal(
                        model,
                        ifcAlignmentInstance,
                        nullptr
                    )) ? true : false,
            hasAlignmentVertical =
                (___GetAlignmentVertical(
                        model,
                        ifcAlignmentInstance,
                        nullptr
                    )) ? true : false,
            hasAlignmentCant =
                (___GetAlignmentCant(
                        model,
                        ifcAlignmentInstance,
                        nullptr
                    )) ? true : false;

    assert(hasAlignmentHorizontal);

    SdaiInstance   ifcRepresentationItem_compositeCurveInstance =
		                ___CreateCompositeCurve__alignmentHorizontal(
		                        model,
		                        ifcAlignmentInstance,
		                        ___GetAlignmentHorizontal(
		                                model,
		                                ifcAlignmentInstance,
		                                nullptr
		                            ),
		                        ___GetPlaneAngleUnitConversionFactor(
		                                model
		                            )
		                    ),
		            ifcRepresentationItem,
    			   ifcRepresentationItem_gradientCurveInstance = 0;

    if (hasAlignmentVertical || hasAlignmentCant) {
        double  startDistAlongHorizontalAlignment = 0.;
        
        sdaiGetAttrBN(
                ___GetAlignmentHorizontal(
                        model,
                        ifcAlignmentInstance,
                        nullptr
                    ),
                "StartDistAlong",
                sdaiREAL,
                &startDistAlongHorizontalAlignment
            );

        ifcRepresentationItem_gradientCurveInstance =
                    ___CreateGradientCurve__alignmentVertical(
                            model,
                            ___GetAlignmentVertical(
                                    model,
                                    ifcAlignmentInstance,
                                    nullptr
                                ),
                            startDistAlongHorizontalAlignment
                        );

        if (ifcRepresentationItem_gradientCurveInstance) {
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
                            ifcAlignmentInstance,
                            nullptr
                        ),
                    "ObjectPlacement",
                    sdaiINSTANCE,
                    (void*) ___CreateObjectPlacement(
                                    model
                                )
                );
        }
        else
            ifcRepresentationItem_gradientCurveInstance = ifcRepresentationItem_compositeCurveInstance;


        if (hasAlignmentCant) {
            int_t   ifcRepresentationItem_segmentedReferenceCurveInstance =
                        ___CreateSegmentedReferenceCurve__alignmentCant(
                                model,
                                ___GetAlignmentCant(
                                        model,
                                        ifcAlignmentInstance,
                                        nullptr
                                    ),
                                startDistAlongHorizontalAlignment
                            );

            assert(ifcRepresentationItem_segmentedReferenceCurveInstance);

            sdaiPutAttrBN(
                    ifcRepresentationItem_segmentedReferenceCurveInstance,
                    "BaseCurve",
                    sdaiINSTANCE,
                    (void*) ifcRepresentationItem_gradientCurveInstance
                );


            //
            //  Add geometry for IfcCantAlignment
            //
            sdaiPutAttrBN(
                    ___GetAlignmentCant(
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

            if (ifcRepresentationItem_segmentedReferenceCurveInstance) {
                assert(___GetAlignmentCant(model, ifcAlignmentInstance, nullptr));
                sdaiPutAttrBN(
                        ___GetAlignmentCant(
                                model,
                                ifcAlignmentInstance,
                                nullptr
                            ),
                        "Representation",
                        sdaiINSTANCE,
                        (void*) ___CreateProductDefinitionShapeInstance(
                                        model,
                                        ifcRepresentationItem_segmentedReferenceCurveInstance,
                                        true
                                    )
                    );
            }
            else {
                assert(false);
            }

            if (ifcRepresentationItem_gradientCurveInstance) {
                ifcRepresentationItem = ifcRepresentationItem_gradientCurveInstance;
            }
            else {
                assert(false);
                ifcRepresentationItem = ifcRepresentationItem_segmentedReferenceCurveInstance;
            }
        }
        else {
            ifcRepresentationItem = ifcRepresentationItem_gradientCurveInstance;
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
        sdaiPutAttrBN(
                ifcAlignmentInstance,
                "Representation",
                sdaiINSTANCE,
                (void*) ___CreateProductDefinitionShapeInstance(
                                model,
                                ifcRepresentationItem,
                                true
                            )
            );
    }

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

    if (ifcRepresentationItem != ifcRepresentationItem_compositeCurveInstance) {
        assert(___GetAlignmentHorizontal(model, ifcAlignmentInstance, nullptr));
        sdaiPutAttrBN(
                ___GetAlignmentHorizontal(
                        model,
                        ifcAlignmentInstance,
                        nullptr
                    ),
                "Representation",
                sdaiINSTANCE,
                (void*) ___CreateProductDefinitionShapeInstance(
                                model,
                                ifcRepresentationItem_compositeCurveInstance,
                                false
                            )
            );
    }

    return  ifcRepresentationItem_gradientCurveInstance ? ifcRepresentationItem_gradientCurveInstance : ifcRepresentationItem_compositeCurveInstance;
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
                            true    
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

    sdaiAppend(aggrProfiles, sdaiINSTANCE, (void*) ___CreateDerivedProfileDefInstance(model, ifcProfileInstance, -(gauge / 2.)));
    sdaiAppend(aggrProfiles, sdaiINSTANCE, (void*) ___CreateDerivedProfileDefInstance(model, ifcProfileInstance,   gauge / 2.));

    return  AlignmentGenerateSweep(
                    model,
                    ifcAlignmentInstance,
                    ifcCompositeProfileDefInstance,
                    ifcProductInstance
                );
}