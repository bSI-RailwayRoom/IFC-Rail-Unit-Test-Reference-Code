#pragma once


#include "ifcalignmentcant.h"
#include "ifcalignmenthorizontal.h"
#include "ifcalignmentvertical.h"
#include "ifcderivedprofiledef.h"
#include "ifcfixedreferencesweptareasolid.h"
#include "ifcproject.h"


extern  int_t   reusedIfcGeometricRepresentationContextInstance;


static  inline  int_t    AlignmentGenerateGeometry(
                                int_t   model,
                                int_t   mirrorIfcAlignmentInstance,
                                int_t   originalIfcAlignmentInstance
                            )
{
    int_t   * ifcGeometricRepresentationContextInstances = sdaiGetEntityExtentBN(model, "IFCGEOMETRICREPRESENTATIONCONTEXT"),
            noIfcGeometricRepresentationContextInstances = sdaiGetMemberCount(ifcGeometricRepresentationContextInstances);
    if (noIfcGeometricRepresentationContextInstances) {
        engiGetAggrElement(ifcGeometricRepresentationContextInstances, 0, sdaiINSTANCE, &reusedIfcGeometricRepresentationContextInstance);
    }

    bool    hasAlignmentHorizontal =
                (___GetAlignmentHorizontal(
                        model,
                        mirrorIfcAlignmentInstance,
                        nullptr
                    )) ? true : false,
            hasAlignmentVertical =
                (___GetAlignmentVertical(
                        model,
                        mirrorIfcAlignmentInstance,
                        nullptr
                    )) ? true : false,
            hasAlignmentCant =
                (___GetAlignmentCant(
                        model,
                        mirrorIfcAlignmentInstance,
                        nullptr
                    )) ? true : false;

    assert(hasAlignmentHorizontal);

    int_t   ifcRepresentationItem_compositeCurveInstance =
                ___CreateCompositeCurve__alignmentHorizontal(
                        model,
                        mirrorIfcAlignmentInstance,
                        ___GetAlignmentHorizontal(
                                model,
                                mirrorIfcAlignmentInstance,
                                nullptr
                            ),
                        ___GetPlaneAngleUnitConversionFactor(
                                model
                            )
                    ),
            ifcRepresentationItem;

    int_t   ifcRepresentationItem_gradientCurveInstance = 0;
    if (hasAlignmentVertical || hasAlignmentCant) {
        double  startDistAlongHorizontalAlignment = 0.;
        
        sdaiGetAttrBN(
                ___GetAlignmentHorizontal(
                        model,
                        mirrorIfcAlignmentInstance,
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
                                    mirrorIfcAlignmentInstance,
                                    nullptr
                                ),
                            startDistAlongHorizontalAlignment,
                            originalIfcAlignmentInstance ?
                                ___GetAlignmentVertical(
                                        engiGetEntityModel(sdaiGetInstanceType(originalIfcAlignmentInstance)),
                                        originalIfcAlignmentInstance,
                                        nullptr
                                    ) :
                                0
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
                            mirrorIfcAlignmentInstance,
                            nullptr
                        ),
                    "ObjectPlacement",
                    sdaiINSTANCE,
                    (void*) ___CreateObjectPlacement(
                                    model
                                )
                );
/*            sdaiPutAttrBN(
                    ___GetAlignmentVertical(
                            model,
                            ifcAlignmentInstance,
                            nullptr
                        ),
                    "Representation",
                    sdaiINSTANCE,
                    (void*) ___CreateProductDefinitionShapeInstance(
                                    model,
                                    ifcRepresentationItem_gradientCurveInstance,
                                    true
                                )
                );  //  */
        }
        else
            ifcRepresentationItem_gradientCurveInstance = ifcRepresentationItem_compositeCurveInstance;


        if (hasAlignmentCant) {
            int_t   ifcRepresentationItem_segmentedReferenceCurveInstance =
                        ___CreateSegmentedReferenceCurve__alignmentCant(
                                model,
                                ___GetAlignmentCant(
                                        model,
                                        mirrorIfcAlignmentInstance,
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
                            mirrorIfcAlignmentInstance,
                            nullptr
                        ),
                    "ObjectPlacement",
                    sdaiINSTANCE,
                    (void*) ___CreateObjectPlacement(
                                    model
                                )
                );

            if (ifcRepresentationItem_segmentedReferenceCurveInstance) {
                assert(___GetAlignmentCant(model, mirrorIfcAlignmentInstance, nullptr));
                sdaiPutAttrBN(
                        ___GetAlignmentCant(
                                model,
                                mirrorIfcAlignmentInstance,
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
            mirrorIfcAlignmentInstance,
            "ObjectPlacement",
            sdaiINSTANCE,
            (void*) ___CreateObjectPlacement(
                            model
                        )
        );
    if (ifcRepresentationItem) {
        assert(mirrorIfcAlignmentInstance);
        sdaiPutAttrBN(
                mirrorIfcAlignmentInstance,
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
                    mirrorIfcAlignmentInstance,
                    nullptr
                ),
            "ObjectPlacement",
            sdaiINSTANCE,
            (void*) ___CreateObjectPlacement(
                            model
                        )
        );

    if (ifcRepresentationItem != ifcRepresentationItem_compositeCurveInstance) {
        assert(___GetAlignmentHorizontal(model, mirrorIfcAlignmentInstance, nullptr));
        sdaiPutAttrBN(
                ___GetAlignmentHorizontal(
                        model,
                        mirrorIfcAlignmentInstance,
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

static  inline  int_t    AlignmentGenerateSweep(
                                int_t   model,
                                int_t   ifcAlignmentInstance,
                                int_t   ifcProfileInstance,
                                int_t   ifcProductInstance
                            )
{
    int_t   ifcFixedReferenceSweptAreaSolidInstance =
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

static  inline  int_t    AlignmentGenerateSweep(
                                int_t   model,
                                int_t   ifcAlignmentInstance,
                                int_t   ifcProfileInstance,
                                int_t   ifcProductInstance,
                                double  gauge
                            )
{
    int_t   ifcCompositeProfileDefInstance, * aggrProfiles;
        
    ifcCompositeProfileDefInstance = sdaiCreateInstanceBN(model, "IFCCOMPOSITEPROFILEDEF");
 
    aggrProfiles = sdaiCreateAggrBN(ifcCompositeProfileDefInstance, "Profiles");

    sdaiAppend((int_t) aggrProfiles, sdaiINSTANCE, (void*) ___CreateDerivedProfileDefInstance(model, ifcProfileInstance, -(gauge / 2.)));
    sdaiAppend((int_t) aggrProfiles, sdaiINSTANCE, (void*) ___CreateDerivedProfileDefInstance(model, ifcProfileInstance, gauge / 2.));

    return  AlignmentGenerateSweep(
                    model,
                    ifcAlignmentInstance,
                    ifcCompositeProfileDefInstance,
                    ifcProductInstance
                );
}