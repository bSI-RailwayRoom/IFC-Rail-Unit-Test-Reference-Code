#pragma once


#include "ifcalignmentcant.h"
#include "ifcalignmenthorizontal.h"
#include "ifcalignmentvertical.h"
#include "ifcderivedprofiledef.h"
#include "ifcfixedreferencesweptareasolid.h"
#include "ifcproject.h"


extern  int_t   reusedIfcGeometricRepresentationContextInstance;


static  inline  void    AlignmentGenerateGeometry(
                                int_t   model,
                                int_t   ifcAlignmentInstance
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

    int_t   ifcRepresentationItem_compositeCurveInstance =
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
            ifcRepresentationItem;

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

        int_t   ifcRepresentationItem_gradientCurveInstance =
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
                                        ifcAlignmentInstance,
                                        nullptr
                                    ),
                                startDistAlongHorizontalAlignment
                            );

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

    if (ifcRepresentationItem != ifcRepresentationItem_compositeCurveInstance)
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