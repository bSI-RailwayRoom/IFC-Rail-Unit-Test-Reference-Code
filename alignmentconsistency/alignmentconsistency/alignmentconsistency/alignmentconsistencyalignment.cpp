// alignmentconsistency.cpp : Defines the initialization routines for the DLL.
//

#include "alignmentconsistencyalignment.h"

#include "alignmentconsistencyalignmentmirror.h"

#include "../business2geometry/ifcalignmenthorizontal.h"
#include "../business2geometry/ifcalignmentvertical.h"
#include "../business2geometry/ifcalignmentcant.h"

#include <iostream>
#include <iterator>
#include <map>

std::map<int64_t, int_t> myMapExpressID;



