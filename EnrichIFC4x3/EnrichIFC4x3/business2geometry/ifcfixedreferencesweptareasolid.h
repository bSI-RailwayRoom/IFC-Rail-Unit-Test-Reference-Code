#pragma once


#include "ifcalignmentcant.h"
#include "ifcalignmenthorizontal.h"
#include "ifcalignmentvertical.h"
#include "ifcproject.h"
#include "ifcproductdefinitionshape.h"


static  inline  SdaiInstance    ___CreateFixedReferenceSweptAreaSolidInstance(
                                        SdaiModel       model,
                                        SdaiInstance    ifcAlignmentCurve,
                                        SdaiInstance    ifcProfileDef
                                    )
{
    assert(sdaiGetInstanceType(ifcAlignmentCurve) == sdaiGetEntity(model, "IFCCOMPOSITECURVE") ||
           sdaiGetInstanceType(ifcAlignmentCurve) == sdaiGetEntity(model, "IFCGRADIENTCURVE")  ||
           sdaiGetInstanceType(ifcAlignmentCurve) == sdaiGetEntity(model, "IFCSEGMENTEDREFERENCECURVE"));

    SdaiInstance    ifcFixedReferenceSweptAreaSolidInstance = sdaiCreateInstanceBN(model, "IFCFIXEDREFERENCESWEPTAREASOLID");

    ___VECTOR3  fixedReference = { 0., 0., 1. };
    sdaiPutAttrBN(ifcFixedReferenceSweptAreaSolidInstance, "SweptArea", sdaiINSTANCE, (void*) ifcProfileDef);
    sdaiPutAttrBN(ifcFixedReferenceSweptAreaSolidInstance, "Directrix", sdaiINSTANCE, (void*) ifcAlignmentCurve);
    sdaiPutAttrBN(ifcFixedReferenceSweptAreaSolidInstance, "FixedReference", sdaiINSTANCE, (void*) ___CreateDirectionInstance(model, &fixedReference));
   
    return  ifcFixedReferenceSweptAreaSolidInstance;
}

static  inline  SdaiInstance    ___CreateDirectrixDerivedReferenceSweptAreaSolidInstance(
                                        SdaiModel       model,
                                        SdaiInstance    ifcAlignmentCurve,
                                        SdaiInstance    ifcProfileDef
                                    )
{
    assert(sdaiGetInstanceType(ifcAlignmentCurve) == sdaiGetEntity(model, "IFCCOMPOSITECURVE") ||
           sdaiGetInstanceType(ifcAlignmentCurve) == sdaiGetEntity(model, "IFCGRADIENTCURVE")  ||
           sdaiGetInstanceType(ifcAlignmentCurve) == sdaiGetEntity(model, "IFCSEGMENTEDREFERENCECURVE"));

    SdaiInstance    ifcDirectrixDerivedReferenceSweptAreaSolidInstance = sdaiCreateInstanceBN(model, "IFCDIRECTRIXDERIVEDREFERENCESWEPTAREASOLID");

    ___VECTOR3  fixedReference = { 0., 0., 1. };
    sdaiPutAttrBN(ifcDirectrixDerivedReferenceSweptAreaSolidInstance, "SweptArea", sdaiINSTANCE, (void*) ifcProfileDef);
    sdaiPutAttrBN(ifcDirectrixDerivedReferenceSweptAreaSolidInstance, "Directrix", sdaiINSTANCE, (void*) ifcAlignmentCurve);
    sdaiPutAttrBN(ifcDirectrixDerivedReferenceSweptAreaSolidInstance, "FixedReference", sdaiINSTANCE, (void*) ___CreateDirectionInstance(model, &fixedReference));
   
    return  ifcDirectrixDerivedReferenceSweptAreaSolidInstance;
}
