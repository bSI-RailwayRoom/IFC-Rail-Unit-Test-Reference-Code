#pragma once


#include "ifcalignmentcant.h"
#include "ifcalignmenthorizontal.h"
#include "ifcalignmentvertical.h"
#include "ifcproject.h"
#include "ifcproductdefinitionshape.h"


static  inline  int_t   ___CreateFixedReferenceSweptAreaSolidInstance(
                                int_t   model,
                                int_t   ifcAlignmentCurve,
                                int_t   ifcProfileDef
                            )
{
    assert(sdaiGetInstanceType(ifcAlignmentCurve) == sdaiGetEntity(model, "IFCCOMPOSITECURVE") ||
           sdaiGetInstanceType(ifcAlignmentCurve) == sdaiGetEntity(model, "IFCGRADIENTCURVE")  ||
           sdaiGetInstanceType(ifcAlignmentCurve) == sdaiGetEntity(model, "IFCSEGMENTEDREFERENCECURVE"));

    int_t   ifcFixedReferenceSweptAreaSolidInstance = sdaiCreateInstanceBN(model, "IFCFIXEDREFERENCESWEPTAREASOLID");

    ___VECTOR3  fixedReference = { 0., 0., 1. };
    sdaiPutAttrBN(ifcFixedReferenceSweptAreaSolidInstance, "SweptArea", sdaiINSTANCE, (void*) ifcProfileDef);
    sdaiPutAttrBN(ifcFixedReferenceSweptAreaSolidInstance, "Directrix", sdaiINSTANCE, (void*) ifcAlignmentCurve);
    sdaiPutAttrBN(ifcFixedReferenceSweptAreaSolidInstance, "FixedReference", sdaiINSTANCE, (void*) ___CreateDirectionInstance(model, &fixedReference));
   
    return  ifcFixedReferenceSweptAreaSolidInstance;
}

static  inline  int_t   ___CreateDirectrixDerivedReferenceSweptAreaSolidInstance(
                                int_t   model,
                                int_t   ifcAlignmentCurve,
                                int_t   ifcProfileDef
                            )
{
    assert(sdaiGetInstanceType(ifcAlignmentCurve) == sdaiGetEntity(model, "IFCCOMPOSITECURVE") ||
           sdaiGetInstanceType(ifcAlignmentCurve) == sdaiGetEntity(model, "IFCGRADIENTCURVE")  ||
           sdaiGetInstanceType(ifcAlignmentCurve) == sdaiGetEntity(model, "IFCSEGMENTEDREFERENCECURVE"));

    int_t   ifcDirectrixDerivedReferenceSweptAreaSolidInstance = sdaiCreateInstanceBN(model, "IFCDIRECTRIXDERIVEDREFERENCESWEPTAREASOLID");

    ___VECTOR3  fixedReference = { 0., 0., 1. };
    sdaiPutAttrBN(ifcDirectrixDerivedReferenceSweptAreaSolidInstance, "SweptArea", sdaiINSTANCE, (void*) ifcProfileDef);
    sdaiPutAttrBN(ifcDirectrixDerivedReferenceSweptAreaSolidInstance, "Directrix", sdaiINSTANCE, (void*) ifcAlignmentCurve);
    sdaiPutAttrBN(ifcDirectrixDerivedReferenceSweptAreaSolidInstance, "FixedReference", sdaiINSTANCE, (void*) ___CreateDirectionInstance(model, &fixedReference));
   
    return  ifcDirectrixDerivedReferenceSweptAreaSolidInstance;
}
