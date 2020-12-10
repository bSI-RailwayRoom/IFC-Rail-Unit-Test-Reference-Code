#pragma once

#include "ifcalignmentcant.h"
#include "ifcalignmenthorizontal.h"
#include "ifcalignmentvertical.h"
#include "ifcproductdefinitionshape.h"


static  inline  void    AlignmentGenerateGeometry(
                                int_t   model,
                                int_t   ifcAlignmentInstance
                            )
{
    int_t   ifcRepresentationItem_compositeCurveInstance =
                CreateCompositeCurve__alignmentHorizontal(
                        model,
                        GetAlignmentHorizontal(
                                model,
                                ifcAlignmentInstance
                            )
                    );

    int_t   ifcRepresentationItem_gradientCurveInstance =
                CreateGradientCurve__alignmentVertical(
                        model,
                        GetAlignmentVertical(
                                model,
                                ifcAlignmentInstance
                            )
                    );

    sdaiPutAttrBN(
            ifcRepresentationItem_gradientCurveInstance,
            "BaseCurve",
            sdaiINSTANCE,
            (void*) ifcRepresentationItem_compositeCurveInstance
        );

    int_t   ifcRepresentationItem_segmentedReferenceCurveInstance =
                CreateSegmentedReferenceCurve__alignmentCant(
                        model,
                        GetAlignmentCant(
                                model,
                                ifcAlignmentInstance
                            )
                    );

    sdaiPutAttrBN(
            ifcRepresentationItem_segmentedReferenceCurveInstance,
            "BaseCurve",
            sdaiINSTANCE,
            (void*) ifcRepresentationItem_gradientCurveInstance
        );

    sdaiPutAttrBN(
            ifcAlignmentInstance,
            "Representation",
            sdaiINSTANCE,
            (void*) CreateProductDefinitionShape(
                            model,
                            ifcRepresentationItem_gradientCurveInstance
     ///                       ifcRepresentationItem_compositeCurveInstance
//                                            ifcRepresentationItem_segmentedReferenceCurveInstance ?
//                                                ifcRepresentationItem_segmentedReferenceCurveInstance :
//                                                ifcRepresentationItem_gradientCurveInstance
                        )
        );
}
