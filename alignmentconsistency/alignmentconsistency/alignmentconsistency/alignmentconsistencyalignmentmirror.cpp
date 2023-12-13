// alignmentconsistency.cpp : Defines the initialization routines for the DLL.
//

#include "alignmentconsistencyalignmentmirror.h"
#include "issues.h"

#include "../business2geometry/ifcalignment.h"

#include "../include/engine.h"

#include <iostream>
#include <iterator>
#include <map>

extern	std::map<int64_t, int_t> myMapExpressID;

extern	bool	PARSE_GEOMETRY;


static  const   uint64_t    flagbit0 = 1;                           // 2^^0                          0000.0000..0000.0001
static  const   uint64_t    flagbit1 = 2;                           // 2^^1                          0000.0000..0000.0010
static  const   uint64_t    flagbit2 = 4;                           // 2^^2                          0000.0000..0000.0100
static  const   uint64_t    flagbit3 = 8;                           // 2^^3                          0000.0000..0000.1000

static  const   uint64_t    flagbit4 = 16;                          // 2^^4                          0000.0000..0001.0000
static  const   uint64_t    flagbit5 = 32;                          // 2^^5                          0000.0000..0010.0000
static  const   uint64_t    flagbit6 = 64;                          // 2^^6                          0000.0000..0100.0000
static  const   uint64_t    flagbit7 = 128;                         // 2^^7                          0000.0000..1000.0000

static  const   uint64_t    flagbit8 = 256;                         // 2^^8                          0000.0001..0000.0000
static  const   uint64_t    flagbit9 = 512;                         // 2^^9                          0000.0010..0000.0000
static  const   uint64_t    flagbit10 = 1024;                       // 2^^10                         0000.0100..0000.0000
static  const   uint64_t    flagbit11 = 2048;                       // 2^^11                         0000.1000..0000.0000

static  const   uint64_t    flagbit12 = 4096;                       // 2^^12                         0001.0000..0000.0000
static  const   uint64_t    flagbit13 = 8192;                       // 2^^13                         0010.0000..0000.0000
static  const   uint64_t    flagbit14 = 16384;                      // 2^^14                         0100.0000..0000.0000
static  const   uint64_t    flagbit15 = 32768;                      // 2^^15                         1000.0000..0000.0000

enum class enum_alignment : unsigned char
{
	HORIZONTAL,
	VERTICAL,
	CANT
};

enum class enum_error : unsigned char
{
	MANDATORY_ATTRIBUTE,										//		0	0
	REAL_BOUNDARY_ISSUE,										//		0	1
	REAL_DERIVED_ISSUE,											//		0	2
	UNKNOWN_ENUM_VALUE,											//		0	3
	UNKNOWN_SCHEMA,												//		0	4
	HORIZONTAL_ALIGNMENT_CONNECTION,							//		1	0
	ALIGNMENT_HORIZONTAL_MISSING,								//		1	0
	VERTICAL_ALIGNMENT_CONNECTION,								//		1	1
	ALIGNMENT_VERTICAL_MISSING,									//		1	1
	HORIZONTAL_ALIGNMENT_NESTS,									//		1	0
	VERTICAL_ALIGNMENT_NESTS,									//		1	1
	CANT_ALIGNMENT_NESTS,										//		1	2
	HORIZONTAL_SEGMENT_DISTANCE,								//		2	0
	VERTICAL_SEGMENT_DISTANCE,									//		2	1
	CANT_SEGMENT_DISTANCE,										//		2	2
	HORIZONTAL_SEGMENT_TANGENT_DEVIATION,						//		2	3
	VERTICAL_SEGMENT_TANGENT_DEVIATION,							//		2	4
	CANT_SEGMENT_TANGENT_DEVIATION,								//		2	5
	ALIGNMENT_MISSING_GEOMETRY,									//		3	0
	ALIGNMENT_INCORRECT_GEOMETRY_ENTITY,						//		3	0
	ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY,							//		3	0
	ALIGNMENT_SHOULD_NOT_HAVE_2P_GEOMETRY,						//		3	0
	ALIGNMENT_SHOULD_HAVE_H_GEOMETRY,							//		3	0
	HORIZONTAL_ALIGNMENT_MISSING_GEOMETRY,						//		3	1
	HORIZONTAL_ALIGNMENT_INCORRECT_GEOMETRY_ENTITY,				//		3	1
	HORIZONTAL_ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY,				//		3	1
	VERTICAL_ALIGNMENT_MISSING_GEOMETRY,						//		3	2
	VERTICAL_ALIGNMENT_INCORRECT_GEOMETRY_ENTITY,				//		3	2
	VERTICAL_ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY,				//		3	2
	CANT_ALIGNMENT_MISSING_GEOMETRY,							//		3	3
	CANT_ALIGNMENT_INCORRECT_GEOMETRY_ENTITY,					//		3	3
	CANT_ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY,					//		3	3
	ALIGNMENT_SEGMENT_INCORRECT_ORDER_BOUNDED_CURVE,			//		3	4
	ALIGNMENT_SEGMENT_INCORRECT_SEGMENT_COUNT_BOUNDED_CURVE,	//		3	4
	ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE,					//		3	4
	ALIGNMENT_SEGMENT_INCONSISTENT_PARENT_CURVE,				//		3	4
	ALIGNMENT_SEGMENT_MISSES_GEOMETRY_HA,						//		3	4
	ALIGNMENT_SEGMENT_MISSES_GEOMETRY_VA,						//		3	4
	ALIGNMENT_SEGMENT_MISSES_GEOMETRY_CA,						//		3	4
	SCHEMA_VALIDATION,											//		4	0
	MODEL_ZERO,													//		A   A
	LIBRARY_OUTDATED											//		A   A
};

//
//	"IFC Schema Validation Issues (inconsistency against the latest IFC4x3 schema)"
//		"Mandatory Attribute Missing"
//		"REAL value boundary issues"
//		"REAL value derived issues"
//		"Incorrect ENUMERATION values"
// 		"File schema definition"
//
//	"Relation Inconsistencies (valid against the schema, however inconsistent according to agreements)"
//		"Horizontal Alignment inconsistencies"
//		"Vertical Alignment inconsistencies"
//		"Cant Alignment inconsistencies"
//
//	"Geometrical Distances (based on Business Logic Knowledge)"
//		"Horizontal Alignment distances / deviations"
//		"Vertical Alignment distances / deviations"
//		"Cant Alignment distances / deviations"
//		"Horizontal Alignment Angle differences"
//		"Vertical Alignment Angle differences"
//		"Cant Alignment Angle differences"
//
//	"Geometrical Content Issue"
//		"Alignment Issues"
//		"Horizontal Alignment Issues"
//		"Vertical Alignment Issues"
//		"Cant Alignment Issues"
//		"Curve Segment Issues"
//

void	assert__error(
				enum_error	myError
			)
{
	switch (myError) {
		case  enum_error::MODEL_ZERO:
			AddIssue(0, 0, (char*) "IFC File not Loaded");
			AddIssue(0, 1, (char*) "IFC File not Loaded");
			AddIssue(0, 2, (char*) "IFC File not Loaded");
			AddIssue(0, 3, (char*) "IFC File not Loaded");
			AddIssue(0, 4, (char*) "IFC File not Loaded");

			AddIssue(1, 0, (char*) "IFC File not Loaded");
			AddIssue(1, 1, (char*) "IFC File not Loaded");
			AddIssue(1, 2, (char*) "IFC File not Loaded");

			AddIssue(2, 0, (char*) "IFC File not Loaded");
			AddIssue(2, 1, (char*) "IFC File not Loaded");
			AddIssue(2, 2, (char*) "IFC File not Loaded");
			AddIssue(2, 3, (char*) "IFC File not Loaded");
			AddIssue(2, 4, (char*) "IFC File not Loaded");

			AddIssue(3, 0, (char*) "IFC File not Loaded");
			AddIssue(3, 1, (char*) "IFC File not Loaded");
			AddIssue(3, 2, (char*) "IFC File not Loaded");
			AddIssue(3, 3, (char*) "IFC File not Loaded");
			AddIssue(3, 4, (char*) "IFC File not Loaded");
			break;
		case  enum_error::LIBRARY_OUTDATED:
			AddIssue(0, 0, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(0, 1, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(0, 2, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(0, 3, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(0, 4, (char*) "IFC Engine outdated, use a more recent version");

			AddIssue(1, 0, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(1, 1, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(1, 2, (char*) "IFC Engine outdated, use a more recent version");

			AddIssue(2, 0, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(2, 1, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(2, 2, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(2, 3, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(2, 4, (char*) "IFC Engine outdated, use a more recent version");

			AddIssue(3, 0, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(3, 1, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(3, 2, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(3, 3, (char*) "IFC Engine outdated, use a more recent version");
			AddIssue(3, 4, (char*) "IFC Engine outdated, use a more recent version");
			break;
		default:
			assert(false);
			break;
	}
}

void	assert__error(
				enum_error	myError,
				char		* argument
			)
{
	switch (myError) {
		case  enum_error::UNKNOWN_SCHEMA:
			AddIssue(0, 4, (char*) "Schema expected to be defined as 'IFC4x3_ADD2', currently defined as ", argument);
			break;
		default:
			assert(false);
			break;
	}
}

void	assert__error(
				enum_error	myError,
				int_t		ifcInstance
			)
{
	switch (myError) {	
		case  enum_error::ALIGNMENT_HORIZONTAL_MISSING:
			AddIssue(1, 0, (char*) "Cannot find IfcAlignmentHorizontal", ifcInstance);
			break;
		case  enum_error::ALIGNMENT_VERTICAL_MISSING:
			AddIssue(1, 1, (char*) "Cannot find IfcAlignmentVertical", ifcInstance);
			break;
		case  enum_error::HORIZONTAL_ALIGNMENT_CONNECTION:
			AddIssue(1, 0, (char*) "Cannot find connection with IfcAlignmentHorizontal", ifcInstance);
			break;
		case  enum_error::VERTICAL_ALIGNMENT_CONNECTION:
			AddIssue(1, 1, (char*) "Cannot find connection with IfcAlignmentVertical", ifcInstance);
			break;
		case  enum_error::ALIGNMENT_MISSING_GEOMETRY:
			AddIssue(3, 0, (char*) "Cannot find expected geometry for Alignment (GradientCurve)", ifcInstance);
			break;
		case  enum_error::ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY:
			AddIssue(3, 0, (char*) "Found unexpected geometry for Alignment (expected to be empty)", ifcInstance);
			break;
		case  enum_error::ALIGNMENT_SHOULD_NOT_HAVE_2P_GEOMETRY:
			AddIssue(3, 0, (char*) "Found unexpected geometry for IfcAlignment (by maximum two geometry items expected for IfcAlignment)", ifcInstance);
			break;
		case  enum_error::ALIGNMENT_SHOULD_HAVE_H_GEOMETRY:
			AddIssue(3, 0, (char*) "Found unexpected geometry for IfcAlignment there should be at least an IfcCompositeCurve reference", ifcInstance);
			break;
		case  enum_error::HORIZONTAL_ALIGNMENT_MISSING_GEOMETRY:
			AddIssue(3, 1, (char*) "Cannot find expected geometry for Horizontal Alignment (IfcCompositeCurve expected)", ifcInstance);
			break;
		case  enum_error::HORIZONTAL_ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY:
			AddIssue(3, 1, (char*) "Found unexpected geometry for Horizontal Alignment (expected to be empty)", ifcInstance);
			break;
		case  enum_error::VERTICAL_ALIGNMENT_MISSING_GEOMETRY:
			AddIssue(3, 2, (char*) "Cannot find expected geometry for Vertical Alignment (IfcGradientCurve expected)", ifcInstance);
			break;
		case  enum_error::VERTICAL_ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY:
			AddIssue(3, 2, (char*) "Found unexpected geometry for Vertical Alignment (expected to be empty)", ifcInstance);
			break;
		case  enum_error::CANT_ALIGNMENT_MISSING_GEOMETRY:
			AddIssue(3, 3, (char*) "Cannot find expected geometry for Cant Alignment (IfcSegmentedReferenceCurve expected)", ifcInstance);
			break;
		case  enum_error::CANT_ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY:
			AddIssue(3, 3, (char*) "Found unexpected geometry for Cant Alignment (expected to be empty)", ifcInstance);
			break;
		case  enum_error::ALIGNMENT_SEGMENT_MISSES_GEOMETRY_HA:
			AddIssue(3, 4, (char*) "Cannot find expected geometry for Alignment Segment [Horizontal Alignment] (IfcCurveSegment expected)", ifcInstance);
			break;
		case  enum_error::ALIGNMENT_SEGMENT_MISSES_GEOMETRY_VA:
			AddIssue(3, 4, (char*) "Cannot find expected geometry for Alignment Segment [Vertical Alignment] (IfcCurveSegment expected)", ifcInstance);
			break;
		case  enum_error::ALIGNMENT_SEGMENT_MISSES_GEOMETRY_CA:
			AddIssue(3, 4, (char*) "Cannot find expected geometry for Alignment Segment [Cant Alignment] (IfcCurveSegment expected)", ifcInstance);
			break;
		default:
			assert(false);
			break;
	}
}

void	assert__error(
				enum_error	myError,
				int_t		ifcInstanceI,
				int_t		ifcInstanceII
			)
{
	switch (myError) {
		case  enum_error::HORIZONTAL_ALIGNMENT_NESTS:
			AddIssue(1, 0, (char*) "Found incorrect relation (probably decomposition not nests) between Alignment and Horizontal Alignment", ifcInstanceI, ifcInstanceII);
			break;
		case  enum_error::VERTICAL_ALIGNMENT_NESTS:
			AddIssue(1, 1, (char*) "Found incorrect relation (probably decomposition not nests) between Alignment and Vertical Alignment", ifcInstanceI, ifcInstanceII);
			break;
		case  enum_error::CANT_ALIGNMENT_NESTS:
			AddIssue(1, 2, (char*) "Found incorrect relation (probably decomposition not nests) between Alignment and Cant Alignment", ifcInstanceI, ifcInstanceII);
			break;
		case  enum_error::ALIGNMENT_INCORRECT_GEOMETRY_ENTITY:
			AddIssue(3, 0, (char*) "Found unexpected geometry for Alignment (expected GradientCurve and in line with Vertical Alignment)", ifcInstanceI, ifcInstanceII);
			break;
		case  enum_error::HORIZONTAL_ALIGNMENT_INCORRECT_GEOMETRY_ENTITY:
			AddIssue(3, 1, (char*) "Found unexpected geometry for Horizontal Alignment (expected CompositeCurve)", ifcInstanceI, ifcInstanceII);
			break;
		case  enum_error::VERTICAL_ALIGNMENT_INCORRECT_GEOMETRY_ENTITY:
			AddIssue(3, 2, (char*) "Found unexpected geometry for Vertical Alignment (expected GradientCurve)", ifcInstanceI, ifcInstanceII);
			break;
		case  enum_error::CANT_ALIGNMENT_INCORRECT_GEOMETRY_ENTITY:
			AddIssue(3, 3, (char*) "Found unexpected geometry for Cant Alignment (expected SegmentedReferenceCurve)", ifcInstanceI, ifcInstanceII);
			break;
		case  enum_error::ALIGNMENT_SEGMENT_INCORRECT_ORDER_BOUNDED_CURVE:
			AddIssue(3, 4, (char*) "Found unexpected order of segments referenced by bounded curve", ifcInstanceI, ifcInstanceII);
			break;
		case  enum_error::ALIGNMENT_SEGMENT_INCORRECT_SEGMENT_COUNT_BOUNDED_CURVE:
			AddIssue(3, 4, (char*) "Found unexpected segment count referenced by bounded curve", ifcInstanceI, ifcInstanceII);
			break;
		case  enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_PARENT_CURVE:
assert(false);
			AddIssue(3, 4, (char*) "Unexpected parent curve for the geometry", ifcInstanceI, ifcInstanceII);
			break;
		default:
			assert(false);
			break;
	}
}

char	* MergeText(
				char	* txtI,
				char	* txtII,
				char	* txtIII
			);

void	assert__error__(
				enum_error	myError,
				int_t		ifcInstanceI,
				int_t		ifcInstanceII,
				char		* entityName
			)
{
	switch (myError) {
		case  enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_PARENT_CURVE:
			AddIssue(3, 4, MergeText((char*) "Unexpected parent curve for the geometry (expected entity ", entityName, (char*) ")"), ifcInstanceI, ifcInstanceII);
//			AddIssue(3, 4, (char*) "Unexpected parent curve for the geometry (expected entity ...)", ifcInstanceI, ifcInstanceII);
			break;
		default:
			assert(false);
			break;
	}
}

void	assert__error(
				void		* content,
				enum_error	myError,
				int_t		ifcInstance,
				char		* attributeName
			)
{
	switch (myError) {
		case  enum_error::MANDATORY_ATTRIBUTE:
			if (content == nullptr) {
				AddIssue(0, 0, (char*) "Manditory attribute is missing, attrribute: ", ifcInstance, attributeName);
			}
			break;
		default:
			assert(false);
			break;
	}
}

void	assert__error(
				enum_error	myError,
				int_t		ifcInstance,
				char		* attributeName
			)
{
	switch (myError) {
		case  enum_error::SCHEMA_VALIDATION:
			AddIssue(0, 1, (char*) "Value of real value out of scope, attribute: ", ifcInstance, attributeName);
			break;
		case  enum_error::REAL_DERIVED_ISSUE:
			AddIssue(0, 2, (char*) "Derived value of real value incorrect, attribute: ", ifcInstance, attributeName);
			break;
//		case  enum_error::UNKNOWN_ENUM_VALUE:
//			AddIssue(0, 3, (char*) "Enumeration value unknow - schema incompatibility, attribute: ", ifcInstance, attributeName);
//			break;
		default:
			assert(false);
			break;
	}
}

void	assert__error(
				enum_error	myError,
				char		* text,
				int_t		ifcInstance
			)
{
	switch (myError) {
		case  enum_error::REAL_BOUNDARY_ISSUE:
			AddIssue(4, 0, text, ifcInstance, "");
			break;
		default:
			assert(false);
			break;
	}
}

void	assert__error__DERIVED_ISSUE(
				int_t		ifcInstance,
				char		* attributeName,
                double      foundValue,
                double      expectedValue
			)
{
	AddIssue(0, 3, (char*) "Enumeration value unknow - schema incompatibility, attribute: ", ifcInstance, attributeName, foundValue, expectedValue);
}

void	assert__error(
				enum_error	myError,
				int_t		ifcInstanceI,
				int_t		ifcInstanceII,
				char		* attributeName
			)
{
	switch (myError) {
		case  enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE:
			AddIssue(3, 4, (char*) "IfcCurveSegment inconsistent with Business Logic attribute: ", ifcInstanceI, ifcInstanceII, attributeName);
			break;
		default:
			assert(false);
			break;
	}
}

void	assert__error__(
				enum_error	myError,
				int_t		ifcInstanceSegmentI,
				int_t		ifcInstanceSegmentII
			)
{
	switch (myError) {
		case  enum_error::HORIZONTAL_SEGMENT_TANGENT_DEVIATION:
			AddIssue__OK(2, 3, (char*) "IGNORED CHECK, Transition attribute allows deviation. Angle difference in degrees (continues 1st order) between end of segment I and start of segment II (horizontal segments), ", ifcInstanceSegmentI, ifcInstanceSegmentII);
			break;
		case  enum_error::VERTICAL_SEGMENT_TANGENT_DEVIATION:
			AddIssue__OK(2, 4, (char*) "IGNORED CHECK, Transition attribute allows deviation. Angle difference in degrees (continues 1st order) between end of segment I and start of segment II (vertical segments), ", ifcInstanceSegmentI, ifcInstanceSegmentII);
			break;
		case  enum_error::CANT_SEGMENT_TANGENT_DEVIATION:
			AddIssue__OK(2, 5, (char*) "IGNORED CHECK, Transition attribute allows deviation. Angle difference in degrees (continues 1st order) between end of segment I and start of segment II (cant segments), ", ifcInstanceSegmentI, ifcInstanceSegmentII);
			break;
		default:
			assert(false);
			break;
	}
}

void	assert__error(
				enum_error	myError,
				int_t		ifcInstanceSegmentI,
				int_t		ifcInstanceSegmentII,
				double		deviation
			)
{
	switch (myError) {
		case  enum_error::HORIZONTAL_SEGMENT_DISTANCE:
			AddIssue(2, 0, (char*) "Distance between (continues 0th order) end of segment I and start of segment II (horizontal segments), ", ifcInstanceSegmentI, ifcInstanceSegmentII, deviation);
			break;
		case  enum_error::VERTICAL_SEGMENT_DISTANCE:
			AddIssue(2, 1, (char*) "Distance between (continues 0th order) end of segment I and start of segment II (vertical segments), ", ifcInstanceSegmentI, ifcInstanceSegmentII, deviation);
			break;
		case  enum_error::CANT_SEGMENT_DISTANCE:
			AddIssue(2, 2, (char*)"Distance between (continues 0th order) end of segment I and start of segment II (cant segments), ", ifcInstanceSegmentI, ifcInstanceSegmentII, deviation);
			break;
		case  enum_error::HORIZONTAL_SEGMENT_TANGENT_DEVIATION:
			AddIssue(2, 3, (char*) "Angle difference in degrees (continues 1st order) between end of segment I and start of segment II (horizontal segments), ", ifcInstanceSegmentI, ifcInstanceSegmentII, deviation);
			break;
		case  enum_error::VERTICAL_SEGMENT_TANGENT_DEVIATION:
			AddIssue(2, 4, (char*) "Angle difference in degrees (continues 1st order) between end of segment I and start of segment II (vertical segments), ", ifcInstanceSegmentI, ifcInstanceSegmentII, deviation);
			break;
		case  enum_error::CANT_SEGMENT_TANGENT_DEVIATION:
			AddIssue(2, 5, "Angle difference in degrees (continues 1st order) between end of segment I and start of segment II (cant segments), ", ifcInstanceSegmentI, ifcInstanceSegmentII, deviation);
			break;
		default:
			assert(false);
			break;
	}
}

void	CopyADB(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);

	int_t	* ADBValue = nullptr;
	sdaiGetAttrBN(ifcInstance, attributeName, sdaiADB, &ADBValue);
	int_t	sdaiTYPE = sdaiGetADBType(ADBValue);

	const char	* path = sdaiGetADBTypePath(ADBValue, sdaiSTRING);

	void	* value = nullptr;
	sdaiGetADBValue(ADBValue, sdaiTYPE, &value);

	void	* mirrorADBValue = sdaiCreateADB(sdaiTYPE, (void*) &value);
    sdaiPutADBTypePath(mirrorADBValue, 1, path);
    sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiADB, (void*) mirrorADBValue);
}

void	CopyString(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	char	* content = nullptr;
	sdaiGetAttrBN(ifcInstance, attributeName, sdaiSTRING, &content);
	sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiSTRING, content);
	assert__error(content, enum_error::MANDATORY_ATTRIBUTE, ifcInstance, attributeName);
}

void	CopyInteger(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	int_t	content = 1234567891;
	sdaiGetAttrBN(ifcInstance, attributeName, sdaiINTEGER, &content);
	if (content != 1234567891) {
		sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiINTEGER, &content);
	}
	else {
		assert(content);
	}
}

void	CopyLengthMeasure(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	double	content = 123456789.98765;
	sdaiGetAttrBN(ifcInstance, attributeName, sdaiREAL, &content);
	if (content != 123456789.98765) {
		sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiREAL, &content);
	}
	else {
		assert(content);
	}
}

void	CopyLengthMeasureOPTIONAL(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	double	content = 123456789.98765;
	if (sdaiGetAttrBN(ifcInstance, attributeName, sdaiREAL, &content)) {
		sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiREAL, &content);
	}
}

void	CopyNonNegativeLengthMeasure(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	double	content = 123456789.98765;
	sdaiGetAttrBN(ifcInstance, attributeName, sdaiREAL, &content);
	if (content != 123456789.98765) {
		assert(content >= 0.);
		sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiREAL, &content);
	}
	else {
		assert(content);
	}
}

void	CopyNonNegativeLengthMeasureOPTIONAL(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	double	content = 123456789.98765;
	if (sdaiGetAttrBN(ifcInstance, attributeName, sdaiREAL, &content)) {
		assert(content >= 0.);
		sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiREAL, &content);
	}
}

void	CopyPositiveLengthMeasure(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	double	content = 123456789.98765;
	void	* rValue = sdaiGetAttrBN(ifcInstance, attributeName, sdaiREAL, &content);
	if (rValue) {
		if (content <= 0.) {
			assert__error(enum_error::REAL_BOUNDARY_ISSUE, ifcInstance, attributeName);
		}
		sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiREAL, &content);
	}
	else {
		assert(content == 0.);
		assert__error(enum_error::REAL_BOUNDARY_ISSUE, ifcInstance, attributeName);
	}
}

void	CopyPositiveLengthMeasureOPTIONAL(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	double	content = 0.;
	void	* rValue = sdaiGetAttrBN(ifcInstance, attributeName, sdaiREAL, &content);
	if (rValue) {
		if (content <= 0.) {
			assert__error(enum_error::REAL_BOUNDARY_ISSUE, ifcInstance, attributeName);
		}
		sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiREAL, &content);
	}
	else {
		assert(content == 0.);
		int_t	attrType = engiGetInstanceAttrTypeBN(ifcInstance, attributeName);
		if (attrType != 0) {
			assert__error(enum_error::REAL_BOUNDARY_ISSUE, ifcInstance, attributeName);
		}
	}
}

void	CopyRatioMeasure(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	CopyLengthMeasure(mirrorIfcInstance, ifcInstance, attributeName);
}

void	CopyPlaneAngleMeasure(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	CopyLengthMeasure(mirrorIfcInstance, ifcInstance, attributeName);
}

void	CopyStringOPTIONAL(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	char	* content = nullptr;
	void	* rValue = sdaiGetAttrBN(ifcInstance, attributeName, sdaiSTRING, &content);
	if (rValue) {
		assert(content);
		sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiSTRING, content);
	}
	else {
		assert(content == nullptr);
	}
}

bool	equals(const char * txtI, const char * txtII)
{
	if (txtI && txtII) {
		int_t	i = 0;
		while (txtI[i] && txtI[i] == txtII[i]) i++;
		if (txtI[i] == 0 && txtII[i] == 0) return true;
	}
	return	false;
}

void	CopyEnum(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	char	* content = nullptr;
	sdaiGetAttrBN(ifcInstance, attributeName, sdaiENUM, &content);
	if (content) {
		sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiENUM, content);
	}
	else {
//		assert(false);
	}
}

void	CopyEnumOPTIONAL(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	char	* content = nullptr;
	if (sdaiGetAttrBN(ifcInstance, attributeName, sdaiENUM, &content)) {
		sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiENUM, content);
	}
}

void	CopyAlignmentTypeEnumOPTIONAL(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	char	* content = nullptr;
	sdaiGetAttrBN(ifcInstance, attributeName, sdaiENUM, &content);
	sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiENUM, content);
	if (content) {
		if (!equals(content, (char*) "USERDEFINED") &&
			!equals(content, (char*) "NOTDEFINED")) {
			assert__error(enum_error::UNKNOWN_ENUM_VALUE, ifcInstance, attributeName);
		}
	}
}

void	CopyAlignmentHorizontalSegmentTypeEnum(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	char	* content = nullptr;
int64_t expressID = internalGetP21Line(ifcInstance);
	sdaiGetAttrBN(ifcInstance, attributeName, sdaiENUM, &content);
	sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiENUM, content);
	assert__error(content, enum_error::MANDATORY_ATTRIBUTE, ifcInstance, attributeName);
	if (content) {
		if (!equals(content, (char*) "LINE") &&
			!equals(content, (char*) "CIRCULARARC") &&
			!equals(content, (char*) "CLOTHOID") &&
			!equals(content, (char*) "CUBIC") &&
			!equals(content, (char*) "HELMERTCURVE") &&
			!equals(content, (char*) "BLOSSCURVE") &&
			!equals(content, (char*) "CUBICSPIRAL") &&
			!equals(content, (char*) "COSINECURVE") &&
			!equals(content, (char*) "SINECURVE") &&
			!equals(content, (char*) "VIENNESEBEND")) {
			assert__error(enum_error::UNKNOWN_ENUM_VALUE, ifcInstance, attributeName);
		}
	}
}

void	CopyAlignmentVerticalSegmentTypeEnum(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	char	* content = nullptr;
	sdaiGetAttrBN(ifcInstance, attributeName, sdaiENUM, &content);
	sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiENUM, content);
	assert__error(content, enum_error::MANDATORY_ATTRIBUTE, ifcInstance, attributeName);
	if (content) {
		if (!equals(content, (char*) "CONSTANTGRADIENT") &&
			!equals(content, (char*) "CIRCULARARC") &&
			!equals(content, (char*) "PARABOLICARC") &&
			!equals(content, (char*) "CLOTHOID")) {
			assert__error(enum_error::UNKNOWN_ENUM_VALUE, ifcInstance, attributeName);
		}
	}
}

void	CopyAlignmentCantSegmentTypeEnum(
				int_t	mirrorIfcInstance,
				int_t	ifcInstance,
				char	* attributeName
			)
{
	assert(ifcInstance && mirrorIfcInstance);
	char	* content = nullptr;
	sdaiGetAttrBN(ifcInstance, attributeName, sdaiENUM, &content);
	sdaiPutAttrBN(mirrorIfcInstance, attributeName, sdaiENUM, content);
	assert__error(content, enum_error::MANDATORY_ATTRIBUTE, ifcInstance, attributeName);
	if (content) {
		if (!equals(content, (char*) "CONSTANTCANT") &&
			!equals(content, (char*) "LINEARTRANSITION") &&
			!equals(content, (char*) "HELMERTCURVE") &&
			!equals(content, (char*) "BLOSSCURVE") &&
			!equals(content, (char*) "COSINECURVE") &&
			!equals(content, (char*) "SINECURVE") &&
			!equals(content, (char*) "VIENNESEBEND")) {
			assert__error(enum_error::UNKNOWN_ENUM_VALUE, ifcInstance, attributeName);
		}
	}
}

SdaiInstance	CopyCartesianPoint2D(
						SdaiModel		mirrorModel,
						SdaiInstance	ifcCartesianPointInstance
					)
{
	SdaiInstance	mirrorIfcCartesianPointInstance = sdaiCreateInstanceBN(mirrorModel, "IFCCARTESIANPOINT");
	SdaiAggr		aggrCoordinates = nullptr;
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcCartesianPointInstance), ifcCartesianPointInstance));

//int_t epxressID = internalGetP21Line(ifcCartesianPointInstance);
	sdaiGetAttrBN(ifcCartesianPointInstance, (char*) "Coordinates", sdaiAGGR, &aggrCoordinates);
	int_t	cnt = sdaiGetMemberCount(aggrCoordinates);
//	assert(cnt == 2);
	if (cnt >= 2) {
		double	x = 0., y = 0.;
		sdaiGetAggrByIndex(aggrCoordinates, 0, sdaiREAL, &x);
		sdaiGetAggrByIndex(aggrCoordinates, 1, sdaiREAL, &y);
		SdaiAggr	mirrorAggrCoordinates = sdaiCreateAggrBN(mirrorIfcCartesianPointInstance, "Coordinates");
		sdaiAppend(mirrorAggrCoordinates, sdaiREAL, &x);
		sdaiAppend(mirrorAggrCoordinates, sdaiREAL, &y);
	}
	return	mirrorIfcCartesianPointInstance;
}

SdaiInstance	CopyInstanceIfcAlignmentHorizontalSegment(
						SdaiModel		mirrorModel,
						SdaiInstance	ifcAlignmentParameterSegmentInstance
					)
{
	SdaiInstance	mirrorIfcAlignmentHorizontalSegmentInstance = sdaiCreateInstanceBN(mirrorModel, (char*) "IFCALIGNMENTHORIZONTALSEGMENT");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcAlignmentHorizontalSegmentInstance), ifcAlignmentParameterSegmentInstance));

	CopyStringOPTIONAL(mirrorIfcAlignmentHorizontalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "StartTag");
	CopyStringOPTIONAL(mirrorIfcAlignmentHorizontalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "EndTag");

	SdaiInstance	ifcCartesianPointInstance = 0;
	sdaiGetAttrBN(ifcAlignmentParameterSegmentInstance, "StartPoint", sdaiINSTANCE, &ifcCartesianPointInstance);
	sdaiPutAttrBN(mirrorIfcAlignmentHorizontalSegmentInstance, "StartPoint", sdaiINSTANCE, (void*) CopyCartesianPoint2D(mirrorModel, ifcCartesianPointInstance));
	CopyPlaneAngleMeasure(mirrorIfcAlignmentHorizontalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "StartDirection");
	CopyLengthMeasure(mirrorIfcAlignmentHorizontalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "StartRadiusOfCurvature");
	CopyLengthMeasure(mirrorIfcAlignmentHorizontalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "EndRadiusOfCurvature");
	CopyNonNegativeLengthMeasure(mirrorIfcAlignmentHorizontalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "SegmentLength");
	CopyPositiveLengthMeasureOPTIONAL(mirrorIfcAlignmentHorizontalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "GravityCenterLineHeight");
	CopyAlignmentHorizontalSegmentTypeEnum(mirrorIfcAlignmentHorizontalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "PredefinedType");

	return	mirrorIfcAlignmentHorizontalSegmentInstance;
}

int_t	CopyInstanceIfcAlignmentVerticalSegment(
				int_t	mirrorModel,
				int_t	ifcAlignmentParameterSegmentInstance
			)
{
	int_t	mirrorIfcAlignmentVerticalSegmentInstance = sdaiCreateInstanceBN(mirrorModel, (char*) "IFCALIGNMENTVERTICALSEGMENT");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcAlignmentVerticalSegmentInstance), ifcAlignmentParameterSegmentInstance));

	CopyStringOPTIONAL(mirrorIfcAlignmentVerticalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "StartTag");
	CopyStringOPTIONAL(mirrorIfcAlignmentVerticalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "EndTag");

	CopyLengthMeasure(mirrorIfcAlignmentVerticalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "StartDistAlong");
	CopyNonNegativeLengthMeasure(mirrorIfcAlignmentVerticalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "HorizontalLength");
	CopyLengthMeasure(mirrorIfcAlignmentVerticalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "StartHeight");
	CopyRatioMeasure(mirrorIfcAlignmentVerticalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "StartGradient");
	CopyRatioMeasure(mirrorIfcAlignmentVerticalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "EndGradient");
	CopyLengthMeasureOPTIONAL(mirrorIfcAlignmentVerticalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "RadiusOfCurvature");
	CopyAlignmentVerticalSegmentTypeEnum(mirrorIfcAlignmentVerticalSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "PredefinedType");

	return	mirrorIfcAlignmentVerticalSegmentInstance;
}

int_t	CopyInstanceIfcAlignmentCantSegment(
				int_t	mirrorModel,
				int_t	ifcAlignmentParameterSegmentInstance
			)
{
	int_t	mirrorIfcAlignmentCantSegmentInstance = sdaiCreateInstanceBN(mirrorModel, (char*) "IFCALIGNMENTCANTSEGMENT");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcAlignmentCantSegmentInstance), ifcAlignmentParameterSegmentInstance));

	CopyStringOPTIONAL(mirrorIfcAlignmentCantSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "StartTag");
	CopyStringOPTIONAL(mirrorIfcAlignmentCantSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "EndTag");

	CopyLengthMeasure(mirrorIfcAlignmentCantSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "StartDistAlong");
	CopyNonNegativeLengthMeasure(mirrorIfcAlignmentCantSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "HorizontalLength");
	CopyLengthMeasure(mirrorIfcAlignmentCantSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "StartCantLeft");
	CopyLengthMeasureOPTIONAL(mirrorIfcAlignmentCantSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "EndCantLeft");
	CopyLengthMeasure(mirrorIfcAlignmentCantSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "StartCantRight");
	CopyLengthMeasureOPTIONAL(mirrorIfcAlignmentCantSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "EndCantRight");
	CopyPositiveLengthMeasureOPTIONAL(mirrorIfcAlignmentCantSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "SmoothingLength");
	CopyAlignmentCantSegmentTypeEnum(mirrorIfcAlignmentCantSegmentInstance, ifcAlignmentParameterSegmentInstance, (char*) "PredefinedType");

	return	mirrorIfcAlignmentCantSegmentInstance;
}

int_t	CopyInstanceIfcAlignmentSegment(
				int_t			mirrorModel,
				int_t			model,
				int_t			ifcAlignmentSegmentInstance,
				enum_alignment	EnumAlignment
			)
{
	int_t	mirrorIfcAlignmentSegmentInstance = sdaiCreateInstanceBN(mirrorModel, (char*) "IFCALIGNMENTSEGMENT");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcAlignmentSegmentInstance), ifcAlignmentSegmentInstance));

	CopyString(mirrorIfcAlignmentSegmentInstance, ifcAlignmentSegmentInstance, (char*) "GlobalId");
	//	OwnerHistory
	CopyStringOPTIONAL(mirrorIfcAlignmentSegmentInstance, ifcAlignmentSegmentInstance, (char*) "Name");
	CopyStringOPTIONAL(mirrorIfcAlignmentSegmentInstance, ifcAlignmentSegmentInstance, (char*) "Description");

	CopyStringOPTIONAL(mirrorIfcAlignmentSegmentInstance, ifcAlignmentSegmentInstance, (char*) "ObjectType");

	int_t	ifcAlignmentParameterSegmentInstance = 0;
	sdaiGetAttrBN(ifcAlignmentSegmentInstance, (char*) "DesignParameters", sdaiINSTANCE, &ifcAlignmentParameterSegmentInstance);

	int_t	mirrorifcAlignmentParameterSegmentInstance;
	if (sdaiGetInstanceType(ifcAlignmentParameterSegmentInstance) == sdaiGetEntity(model, (char*) "IFCALIGNMENTHORIZONTALSEGMENT")) {
		assert(EnumAlignment == enum_alignment::HORIZONTAL);
		mirrorifcAlignmentParameterSegmentInstance = CopyInstanceIfcAlignmentHorizontalSegment(mirrorModel, ifcAlignmentParameterSegmentInstance);
	}
	else if (sdaiGetInstanceType(ifcAlignmentParameterSegmentInstance) == sdaiGetEntity(model, (char*) "IFCALIGNMENTVERTICALSEGMENT")) {
		assert(EnumAlignment == enum_alignment::VERTICAL);
		mirrorifcAlignmentParameterSegmentInstance = CopyInstanceIfcAlignmentVerticalSegment(mirrorModel, ifcAlignmentParameterSegmentInstance);
	}
	else {
		assert(sdaiGetInstanceType(ifcAlignmentParameterSegmentInstance) == sdaiGetEntity(model, (char*) "IFCALIGNMENTCANTSEGMENT"));
		assert(EnumAlignment == enum_alignment::CANT);
		mirrorifcAlignmentParameterSegmentInstance = CopyInstanceIfcAlignmentCantSegment(mirrorModel, ifcAlignmentParameterSegmentInstance);
	}
	sdaiPutAttrBN(mirrorIfcAlignmentSegmentInstance, (char*) "DesignParameters", sdaiINSTANCE, (void*) mirrorifcAlignmentParameterSegmentInstance);

	return	mirrorIfcAlignmentSegmentInstance;
}

int_t	CopyInstanceIfcAlignmentHorizontal(
				int_t	mirrorModel,
				int_t	model,
				int_t	ifcAlignmentHorizontalInstance
			)
{
	int_t	mirrorIfcAlignmentHorizontalInstance = sdaiCreateInstanceBN(mirrorModel, (char*) "IFCALIGNMENTHORIZONTAL");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcAlignmentHorizontalInstance), ifcAlignmentHorizontalInstance));

	CopyString(mirrorIfcAlignmentHorizontalInstance, ifcAlignmentHorizontalInstance, (char*) "GlobalId");
	//	OwnerHistory
	CopyStringOPTIONAL(mirrorIfcAlignmentHorizontalInstance, ifcAlignmentHorizontalInstance, (char*) "Name");
	CopyStringOPTIONAL(mirrorIfcAlignmentHorizontalInstance, ifcAlignmentHorizontalInstance, (char*) "Description");

	CopyStringOPTIONAL(mirrorIfcAlignmentHorizontalInstance, ifcAlignmentHorizontalInstance, (char*) "ObjectType");

	CopyLengthMeasureOPTIONAL(mirrorIfcAlignmentHorizontalInstance, ifcAlignmentHorizontalInstance, (char*) "StartDistAlong");

	//
	//	Get Segments
	//

    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
                        ifcAlignmentHorizontalInstance,
                        nullptr
                    );

    if (noSegmentInstances) {
		int_t	mirrorIfcRelNestsInstance = sdaiCreateInstanceBN(mirrorModel, (char*) "IFCRELNESTS"),
				* mirrorAggrRelatedObjects = sdaiCreateAggrBN(mirrorIfcRelNestsInstance, (char*)"RelatedObjects");
		sdaiPutAttrBN(mirrorIfcRelNestsInstance, (char*) "RelatingObject", sdaiINSTANCE, (void*) mirrorIfcAlignmentHorizontalInstance);

        int_t   * segmentInstances = new int_t[noSegmentInstances];
        ___GetAlignmentSegments(
                model,
                ifcAlignmentHorizontalInstance,
                segmentInstances
            );

		for (int_t i = 0; i < noSegmentInstances; i++) {
			int_t	mirrorIfcAlignmentSegment = CopyInstanceIfcAlignmentSegment(mirrorModel, model, segmentInstances[i], enum_alignment::HORIZONTAL);
			sdaiAppend(mirrorAggrRelatedObjects, sdaiINSTANCE, (void*) mirrorIfcAlignmentSegment);
		}
	}
	else {
		assert(false);
	}

	return	mirrorIfcAlignmentHorizontalInstance;
}

int_t	CopyInstanceIfcAlignmentVertical(
				int_t	mirrorModel,
				int_t	model,
				int_t	ifcAlignmentVerticalInstance
			)
{
	int_t	mirrorIfcAlignmentVerticalInstance = sdaiCreateInstanceBN(mirrorModel, (char*) "IFCALIGNMENTVERTICAL");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcAlignmentVerticalInstance), ifcAlignmentVerticalInstance));

	CopyString(mirrorIfcAlignmentVerticalInstance, ifcAlignmentVerticalInstance, (char*) "GlobalId");
	//	OwnerHistory
	CopyStringOPTIONAL(mirrorIfcAlignmentVerticalInstance, ifcAlignmentVerticalInstance, (char*) "Name");
	CopyStringOPTIONAL(mirrorIfcAlignmentVerticalInstance, ifcAlignmentVerticalInstance, (char*) "Description");

	CopyStringOPTIONAL(mirrorIfcAlignmentVerticalInstance, ifcAlignmentVerticalInstance, (char*) "ObjectType");

	//
	//	Get Segments
	//

    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
						ifcAlignmentVerticalInstance
                    );

    if (noSegmentInstances) {
		int_t	mirrorIfcRelNestsInstance = sdaiCreateInstanceBN(mirrorModel, (char*) "IFCRELNESTS"),
				* mirrorAggrRelatedObjects = sdaiCreateAggrBN(mirrorIfcRelNestsInstance, (char*) "RelatedObjects");
		sdaiPutAttrBN(mirrorIfcRelNestsInstance, (char*) "RelatingObject", sdaiINSTANCE, (void*) mirrorIfcAlignmentVerticalInstance);

        int_t   * segmentInstances = new int_t[noSegmentInstances];
        ___GetAlignmentSegments(
                model,
				ifcAlignmentVerticalInstance,
                segmentInstances
            );

		for (int_t i = 0; i < noSegmentInstances; i++) {
			int_t	mirrorIfcAlignmentSegment = CopyInstanceIfcAlignmentSegment(mirrorModel, model, segmentInstances[i], enum_alignment::VERTICAL);
			sdaiAppend(mirrorAggrRelatedObjects, sdaiINSTANCE, (void*) mirrorIfcAlignmentSegment);
		}
	}
	else {
		assert(false);
	}

	return	mirrorIfcAlignmentVerticalInstance;
}

SdaiInstance	CopyInstanceIfcAlignmentCant(
						SdaiModel	mirrorModel,
						SdaiModel	model,
						int_t	ifcAlignmentCantInstance
					)
{
	SdaiInstance	mirrorIfcAlignmentCantInstance = sdaiCreateInstanceBN(mirrorModel, "IFCALIGNMENTCANT");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcAlignmentCantInstance), ifcAlignmentCantInstance));

	CopyString(mirrorIfcAlignmentCantInstance, ifcAlignmentCantInstance, "GlobalId");
	//	OwnerHistory
	CopyStringOPTIONAL(mirrorIfcAlignmentCantInstance, ifcAlignmentCantInstance, "Name");
	CopyStringOPTIONAL(mirrorIfcAlignmentCantInstance, ifcAlignmentCantInstance, "Description");

	CopyStringOPTIONAL(mirrorIfcAlignmentCantInstance, ifcAlignmentCantInstance, "ObjectType");

	CopyPositiveLengthMeasure(mirrorIfcAlignmentCantInstance, ifcAlignmentCantInstance, "RailHeadDistance");

	//
	//	Get Segments
	//

    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
						ifcAlignmentCantInstance,
                        nullptr
                    );

    if (noSegmentInstances) {
		SdaiInstance	mirrorIfcRelNestsInstance = sdaiCreateInstanceBN(mirrorModel, "IFCRELNESTS");
		SdaiAggr		mirrorAggrRelatedObjects = sdaiCreateAggrBN(mirrorIfcRelNestsInstance, "RelatedObjects");
		sdaiPutAttrBN(mirrorIfcRelNestsInstance, "RelatingObject", sdaiINSTANCE, (void*) mirrorIfcAlignmentCantInstance);

        int_t   * segmentInstances = new int_t[noSegmentInstances];
        ___GetAlignmentSegments(
                model,
				ifcAlignmentCantInstance,
                segmentInstances
            );

		for (int_t i = 0; i < noSegmentInstances; i++) {
			int_t	mirrorIfcAlignmentSegment = CopyInstanceIfcAlignmentSegment(mirrorModel, model, segmentInstances[i], enum_alignment::CANT);
			sdaiAppend(mirrorAggrRelatedObjects, sdaiINSTANCE, (void*) mirrorIfcAlignmentSegment);
		}
	}
	else {
//		assert(false);
	}

	return	mirrorIfcAlignmentCantInstance;
}

void	AddIfcRelNests(
				SdaiModel		model,
				SdaiInstance	ifcInstanceParent,
				SdaiInstance	ifcInstanceChild
			)
{
	SdaiInstance	ifcRelNestsInstance = sdaiCreateInstanceBN(model, "IFCRELNESTS");
	SdaiAggr		aggrRelatedObjects = sdaiCreateAggrBN(ifcRelNestsInstance, "RelatedObjects");
	sdaiPutAttrBN(ifcRelNestsInstance, "RelatingObject", sdaiINSTANCE, (void*) ifcInstanceParent);
	sdaiAppend(aggrRelatedObjects, sdaiINSTANCE, (void*) ifcInstanceChild);
}

SdaiInstance	CopyInstanceIfcAlignment(
						SdaiModel		mirrorModel,
						SdaiModel		model,
						SdaiInstance	ifcAlignmentInstance
					)
{
	SdaiInstance	mirrorIfcAlignmentInstance = sdaiCreateInstanceBN(mirrorModel, "IFCALIGNMENT");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcAlignmentInstance), ifcAlignmentInstance));

	CopyString(mirrorIfcAlignmentInstance, ifcAlignmentInstance, "GlobalId");
	//	OwnerHistory
	CopyStringOPTIONAL(mirrorIfcAlignmentInstance, ifcAlignmentInstance, "Name");
	CopyStringOPTIONAL(mirrorIfcAlignmentInstance, ifcAlignmentInstance, "Description");
	CopyStringOPTIONAL(mirrorIfcAlignmentInstance, ifcAlignmentInstance, "ObjectType");
	//	ObjectPlacement
	//	Representation
	CopyAlignmentTypeEnumOPTIONAL(mirrorIfcAlignmentInstance, ifcAlignmentInstance, "PredefinedType");

	{
		SdaiInstance	ifcAlignmentHorizontalInstance =
							___GetAlignmentHorizontal(
									model,
									ifcAlignmentInstance,
									nullptr
								);
		if (ifcAlignmentHorizontalInstance == 0) {
			assert__error(enum_error::HORIZONTAL_ALIGNMENT_CONNECTION, ifcAlignmentInstance);
		}
		else {
			SdaiInstance	mirrorIfcAlignmentHorizontalInstance =
								CopyInstanceIfcAlignmentHorizontal(mirrorModel, model, ifcAlignmentHorizontalInstance);
			AddIfcRelNests(mirrorModel, mirrorIfcAlignmentInstance, mirrorIfcAlignmentHorizontalInstance);
		}
	}

	{
		SdaiInstance	ifcAlignmentVerticalInstance =
							___GetAlignmentVertical(
									model,
									ifcAlignmentInstance,
									nullptr
								);
		if (ifcAlignmentVerticalInstance) {
			SdaiInstance	mirrorIfcAlignmentVerticalInstance =
								CopyInstanceIfcAlignmentVertical(mirrorModel, model, ifcAlignmentVerticalInstance);
			AddIfcRelNests(mirrorModel, mirrorIfcAlignmentInstance, mirrorIfcAlignmentVerticalInstance);
		}
	}

	{
		SdaiInstance	ifcAlignmentCantInstance =
							___GetAlignmentCant(
									model,
									ifcAlignmentInstance,
									nullptr
								);

		if (ifcAlignmentCantInstance) {
			SdaiInstance	mirrorIfcAlignmentCantInstance =
								CopyInstanceIfcAlignmentCant(mirrorModel, model, ifcAlignmentCantInstance);
			AddIfcRelNests(mirrorModel, mirrorIfcAlignmentInstance, mirrorIfcAlignmentCantInstance);
		}
	}

	return	mirrorIfcAlignmentInstance;
}

bool	IsRelevantNamedUnit(
				SdaiModel		model,
				SdaiInstance	ifcNamedUnitInstance
			)
{
	char	* unitType = 0;
	sdaiGetAttrBN(ifcNamedUnitInstance, "UnitType", sdaiENUM, &unitType);
	assert(unitType);
	if (equals(unitType, "LENGTHUNIT") ||
		equals(unitType, "PLANEANGLEUNIT")) {
		return	true;
	}
	return	false;
}

SdaiInstance	CopyInstanceSIUnit(
						SdaiModel		mirrorModel,
						SdaiModel		model,
						SdaiInstance	ifcSIUnitInstance
					)
{
	assert(sdaiGetInstanceType(ifcSIUnitInstance) == sdaiGetEntity(model, "IFCSIUNIT"));

	SdaiInstance	mirrorIfcSIUnitInstance = sdaiCreateInstanceBN(mirrorModel, "IFCSIUNIT");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcSIUnitInstance), ifcSIUnitInstance));

	CopyEnumOPTIONAL(mirrorIfcSIUnitInstance, ifcSIUnitInstance, "UnitType");
	CopyEnumOPTIONAL(mirrorIfcSIUnitInstance, ifcSIUnitInstance, "Prefix");
	CopyEnumOPTIONAL(mirrorIfcSIUnitInstance, ifcSIUnitInstance, "Name");

	return	mirrorIfcSIUnitInstance;
}

SdaiInstance	CopyInstanceDimensionalExponents(
						SdaiModel		mirrorModel,
						SdaiModel		model,
						SdaiInstance	ifcDimensionalExponentsInstance
					)
{
	assert(sdaiGetInstanceType(ifcDimensionalExponentsInstance) == sdaiGetEntity(model, "IFCDIMENSIONALEXPONENTS"));

	SdaiInstance	mirrorIfcDimensionalExponentsInstance = sdaiCreateInstanceBN(mirrorModel, "IFCDIMENSIONALEXPONENTS");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcDimensionalExponentsInstance), ifcDimensionalExponentsInstance));

	CopyInteger(mirrorIfcDimensionalExponentsInstance, ifcDimensionalExponentsInstance, (char*) "LengthExponent");
	CopyInteger(mirrorIfcDimensionalExponentsInstance, ifcDimensionalExponentsInstance, (char*) "MassExponent");
	CopyInteger(mirrorIfcDimensionalExponentsInstance, ifcDimensionalExponentsInstance, (char*) "TimeExponent");
	CopyInteger(mirrorIfcDimensionalExponentsInstance, ifcDimensionalExponentsInstance, (char*) "ElectricCurrentExponent");
	CopyInteger(mirrorIfcDimensionalExponentsInstance, ifcDimensionalExponentsInstance, (char*) "ThermodynamicTemperatureExponent");
	CopyInteger(mirrorIfcDimensionalExponentsInstance, ifcDimensionalExponentsInstance, (char*) "AmountOfSubstanceExponent");
	CopyInteger(mirrorIfcDimensionalExponentsInstance, ifcDimensionalExponentsInstance, (char*) "LuminousIntensityExponent");

	return	mirrorIfcDimensionalExponentsInstance;
}

SdaiInstance	CopyInstanceMeasureWithUnit(
						SdaiModel		mirrorModel,
						SdaiModel		model,
						SdaiInstance	ifcMeasureWithUnitInstance
					)
{
	assert(sdaiGetInstanceType(ifcMeasureWithUnitInstance) == sdaiGetEntity(model, (char*) "IFCMEASUREWITHUNIT"));

	SdaiInstance	mirrorIfcMeasureWithUnitInstance = sdaiCreateInstanceBN(mirrorModel, (char*) "IFCMEASUREWITHUNIT");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcMeasureWithUnitInstance), ifcMeasureWithUnitInstance));

	CopyADB(mirrorIfcMeasureWithUnitInstance, ifcMeasureWithUnitInstance, (char*) "ValueComponent");

	SdaiInstance	ifcUnitInstance = 0;
	sdaiGetAttrBN(ifcMeasureWithUnitInstance, (char*) "UnitComponent", sdaiINSTANCE, &ifcUnitInstance);
	sdaiPutAttrBN(mirrorIfcMeasureWithUnitInstance, (char*) "UnitComponent", sdaiINSTANCE, (void*) CopyInstanceSIUnit(mirrorModel, model, ifcUnitInstance));

	return	mirrorIfcMeasureWithUnitInstance;
}

SdaiInstance	CopyInstanceConversionBasedUnit(
						SdaiModel		mirrorModel,
						SdaiModel		model,
						SdaiInstance	ifcConversionBasedUnitInstance
					)
{
	assert(sdaiGetInstanceType(ifcConversionBasedUnitInstance) == sdaiGetEntity(model, (char*) "IFCCONVERSIONBASEDUNIT"));

	SdaiInstance	mirrorIfcConversionBasedUnitInstance = sdaiCreateInstanceBN(mirrorModel, (char*) "IFCCONVERSIONBASEDUNIT");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcConversionBasedUnitInstance), ifcConversionBasedUnitInstance));

	SdaiInstance	ifcDimensionalExponentsInstance = 0;
	sdaiGetAttrBN(ifcConversionBasedUnitInstance, (char*) "Dimensions", sdaiINSTANCE, &ifcDimensionalExponentsInstance);
	sdaiPutAttrBN(mirrorIfcConversionBasedUnitInstance, (char*) "Dimensions", sdaiINSTANCE, (void*) CopyInstanceDimensionalExponents(mirrorModel, model, ifcDimensionalExponentsInstance));

	CopyEnum(mirrorIfcConversionBasedUnitInstance, ifcConversionBasedUnitInstance, (char*) "UnitType");
	CopyString(mirrorIfcConversionBasedUnitInstance, ifcConversionBasedUnitInstance, (char*) "Name");

	SdaiInstance	ifcMeasureWithUnitInstance = 0;
	sdaiGetAttrBN(ifcConversionBasedUnitInstance, (char*) "ConversionFactor", sdaiINSTANCE, &ifcMeasureWithUnitInstance);
	sdaiPutAttrBN(mirrorIfcConversionBasedUnitInstance, (char*) "ConversionFactor", sdaiINSTANCE, (void*) CopyInstanceMeasureWithUnit(mirrorModel, model, ifcMeasureWithUnitInstance));

	return	mirrorIfcConversionBasedUnitInstance;
}

SdaiInstance	CopyInstanceUnitAssignment(
						SdaiModel		mirrorModel,
						SdaiModel		model,
						SdaiInstance	ifcUnitAssignmentInstance
					)
{
	SdaiInstance	mirrorIfcUnitAssignmenInstance = sdaiCreateInstanceBN(mirrorModel, (char*)"IFCUNITASSIGNMENT");
	SdaiAggr		mirrorAggrUnits = sdaiCreateAggrBN(mirrorIfcUnitAssignmenInstance, (char*) "Units");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcUnitAssignmenInstance), ifcUnitAssignmentInstance));

	SdaiAggr	aggrUnits = nullptr;
	sdaiGetAttrBN(ifcUnitAssignmentInstance, "Units", sdaiAGGR, &aggrUnits);
	SdaiInteger	aggrUnitsCnt = sdaiGetMemberCount(aggrUnits);
	for (SdaiInteger i = 0; i < aggrUnitsCnt; i++) {
		SdaiInstance	ifcUnitInstance = 0;
		sdaiGetAggrByIndex(aggrUnits, i, sdaiINSTANCE, &ifcUnitInstance);
		if (sdaiGetInstanceType(ifcUnitInstance) == sdaiGetEntity(model, "IFCSIUNIT")) {
			if (IsRelevantNamedUnit(model, ifcUnitInstance)) {
				sdaiAppend(mirrorAggrUnits, sdaiINSTANCE, (void*) CopyInstanceSIUnit(mirrorModel, model, ifcUnitInstance));
			}
		}
		else if (sdaiGetInstanceType(ifcUnitInstance) == sdaiGetEntity(model, "IFCCONVERSIONBASEDUNIT")) {
			sdaiAppend(mirrorAggrUnits, sdaiINSTANCE, (void*) CopyInstanceConversionBasedUnit(mirrorModel, model, ifcUnitInstance));
		}
		else {
			assert(false);
		}
	}

	return	mirrorIfcUnitAssignmenInstance;
}

SdaiInstance	CopyInstanceIfcProject(
						SdaiModel		mirrorModel,
						SdaiModel		model,
						SdaiInstance	ifcProjectInstance
					)
{
	SdaiInstance	mirrorIfcProjectInstance = sdaiCreateInstanceBN(mirrorModel, (char*) "IFCPROJECT");
	myMapExpressID.insert(std::pair<int64_t, SdaiInstance>(internalGetP21Line(mirrorIfcProjectInstance), ifcProjectInstance));

	SdaiInstance	ifcUnitAssignmenInstance = 0;
	sdaiGetAttrBN(ifcProjectInstance, "UnitsInContext", sdaiINSTANCE, &ifcUnitAssignmenInstance);
	sdaiPutAttrBN(mirrorIfcProjectInstance,  "UnitsInContext", sdaiINSTANCE, (void*) CopyInstanceUnitAssignment(mirrorModel, model, ifcUnitAssignmenInstance));

	return	mirrorIfcProjectInstance;
}

SdaiInstance	CreateMirror(
						SdaiModel		mirrorModel,
						SdaiModel		model,
						SdaiInstance	ifcAlignmentInstance
					)
{
	SdaiAggr		ifcProjectInstances = sdaiGetEntityExtentBN(model, "IFCPROJECT");
	SdaiInteger		noIfcProjectInstances = sdaiGetMemberCount(ifcProjectInstances);
    if (noIfcProjectInstances == 1) {
		SdaiInstance	ifcProjectInstance = 0;
		sdaiGetAggrByIndex(ifcProjectInstances, 0, sdaiINSTANCE, &ifcProjectInstance);

		CopyInstanceIfcProject(mirrorModel, model, ifcProjectInstance);

		return	CopyInstanceIfcAlignment(mirrorModel, model, ifcAlignmentInstance);
	}

	return	0;
}

void	EnrichMirror(
				int_t	mirrorModel,
				int_t	ifcAlignmentInstance
			)
{
	AlignmentGenerateGeometry(mirrorModel, ifcAlignmentInstance);
}



//
//	CompareMirror
//



int_t	FindRepresentation__(
				int_t	model,
				int_t	ifcProductInstance,
				int_t	index
			)
{
	int_t	ifcProductRepresentationInstance = 0;
	sdaiGetAttrBN(ifcProductInstance, (char*) "Representation", sdaiINSTANCE, &ifcProductRepresentationInstance);

	if (sdaiGetInstanceType(ifcProductRepresentationInstance) == sdaiGetEntity(model, (char*) "IFCPRODUCTDEFINITIONSHAPE")) {
		int_t	* aggrRepresentations = nullptr;
		sdaiGetAttrBN(ifcProductRepresentationInstance, (char*) "Representations", sdaiAGGR, &aggrRepresentations);
		int_t	aggrRepresentationsCnt = sdaiGetMemberCount(aggrRepresentations);
		for (int_t i = 0; i < aggrRepresentationsCnt; i++) {
			int_t	ifcRepresentationInstance = 0;
			sdaiGetAggrByIndex(aggrRepresentations, i, sdaiINSTANCE, &ifcRepresentationInstance);

			int_t	* aggrItems = nullptr;
			sdaiGetAttrBN(ifcRepresentationInstance, (char*) "Items", sdaiAGGR, &aggrItems);
			int_t	aggrItemsCnt = sdaiGetMemberCount(aggrItems);

			for (int_t j = 0; j < aggrItemsCnt; j++) {
				int_t	ifcRepresentationItemInstance = 0;
				sdaiGetAggrByIndex(aggrItems, j, sdaiINSTANCE, &ifcRepresentationItemInstance);

				if (sdaiGetInstanceType(ifcRepresentationItemInstance) == sdaiGetEntity(model, (char*) "IFCCOMPOSITECURVE") ||
					sdaiGetInstanceType(ifcRepresentationItemInstance) == sdaiGetEntity(model, (char*) "IFCGRADIENTCURVE") ||
					sdaiGetInstanceType(ifcRepresentationItemInstance) == sdaiGetEntity(model, (char*) "IFCSEGMENTEDREFERENCECURVE")) {
					if (index) {
						index--;
					}
					else {
						return	ifcRepresentationItemInstance;
					}
				}
				else {
					assert(false);
				}
			}
		}
	}
	return	0;
}

void	FindRepresentation(
				int_t	model,
				int_t	ifcProductInstance,
				int_t	* representationItemI,
				int_t	* representationItemII
			)
{
	assert((*representationItemI) == 0 && (*representationItemII) == 0);

	int_t	ifcProductRepresentationInstance = 0;
	sdaiGetAttrBN(ifcProductInstance, (char*) "Representation", sdaiINSTANCE, &ifcProductRepresentationInstance);

	if (sdaiGetInstanceType(ifcProductRepresentationInstance) == sdaiGetEntity(model, (char*) "IFCPRODUCTDEFINITIONSHAPE")) {
		int_t	* aggrRepresentations = nullptr;
		sdaiGetAttrBN(ifcProductRepresentationInstance, (char*) "Representations", sdaiAGGR, &aggrRepresentations);
		int_t	aggrRepresentationsCnt = sdaiGetMemberCount(aggrRepresentations);
		for (int_t i = 0; i < aggrRepresentationsCnt; i++) {
			int_t	ifcRepresentationInstance = 0;
			sdaiGetAggrByIndex(aggrRepresentations, i, sdaiINSTANCE, &ifcRepresentationInstance);

			int_t	* aggrItems = nullptr;
			sdaiGetAttrBN(ifcRepresentationInstance, (char*) "Items", sdaiAGGR, &aggrItems);
			int_t	aggrItemsCnt = sdaiGetMemberCount(aggrItems);
			for (int_t j = 0; j < aggrItemsCnt; j++) {
				int_t	ifcRepresentationItemInstance = 0;
				sdaiGetAggrByIndex(aggrItems, i, sdaiINSTANCE, &ifcRepresentationItemInstance);

				if (sdaiGetInstanceType(ifcRepresentationItemInstance) == sdaiGetEntity(model, (char*) "IFCCURVESEGMENT")) {
					if ((*representationItemI)) {
						if ((*representationItemII)) {
							(*representationItemI) = 0;
							(*representationItemII) = 0;
							return;
						}
						else {
							(*representationItemII) = ifcRepresentationItemInstance;
						}
					}
					else {
						(*representationItemI) = ifcRepresentationItemInstance;
					}
				}
			}
		}
	}
}

int_t	GetIndex(
				int_t	model,
				int_t	ifcBoundedCurveInstance,
				int_t	ifcSegmentCurveInstance
			)
{
	assert(sdaiGetInstanceType(ifcBoundedCurveInstance) == sdaiGetEntity(model, (char*) "IFCCOMPOSITECURVE") ||
		   sdaiGetInstanceType(ifcBoundedCurveInstance) == sdaiGetEntity(model, (char*) "IFCGRADIENTCURVE") ||
		   sdaiGetInstanceType(ifcBoundedCurveInstance) == sdaiGetEntity(model, (char*) "IFCSEGMENTEDREFERENCECURVE"));
	assert(sdaiGetInstanceType(ifcSegmentCurveInstance) == sdaiGetEntity(model, (char*) "IFCCURVESEGMENT"));

	int_t	* aggrSegments = nullptr;
	sdaiGetAttrBN(ifcBoundedCurveInstance, (char*) "Segments", sdaiAGGR, &aggrSegments);
	int_t	aggrSegmentsCnt = sdaiGetMemberCount(aggrSegments);
	for (int_t i = 0; i < aggrSegmentsCnt; i++) {
		int_t	ifcSegmentInstance = 0;
		sdaiGetAggrByIndex(aggrSegments, i, sdaiINSTANCE, &ifcSegmentInstance);
		if (ifcSegmentInstance == ifcSegmentCurveInstance) {
			return	i;
		}
	}
////////////////	assert(false);
	return	-1;
}

void	CompareEnum(
				int_t	mirrorIfcCurveSegmentInstance,
				int_t	ifcCurveSegmentInstance,
				char	* attributeName,
				int_t	ifcContextInstance
			)
{
	char	* mirrorEnumValue = nullptr, * enumValue = nullptr;
	sdaiGetAttrBN(mirrorIfcCurveSegmentInstance, attributeName, sdaiENUM, &mirrorEnumValue);
	sdaiGetAttrBN(ifcCurveSegmentInstance, attributeName, sdaiENUM, &enumValue);
	if (!equals(mirrorEnumValue, enumValue)) {
		assert__error(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE, ifcCurveSegmentInstance, ifcContextInstance, attributeName);
	}
}

void	CompareREAL(
				int_t	mirrorIfcCurveSegmentInstance,
				int_t	ifcCurveSegmentInstance,
				char	* attributeName,
				int_t	ifcContextInstance,
				double	relativeEpsilon
			)
{
	double	mirrorValue = 0., value = 0.;
	sdaiGetAttrBN(mirrorIfcCurveSegmentInstance, attributeName, sdaiREAL, &mirrorValue);
	sdaiGetAttrBN(ifcCurveSegmentInstance, attributeName, sdaiREAL, &value);
	if (std::fabs(mirrorValue - value) > relativeEpsilon) {
		assert__error(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE, ifcCurveSegmentInstance, ifcContextInstance, attributeName);
	}
}

void	CompareADB(
				int_t	mirrorIfcCurveSegmentInstance,
				int_t	ifcCurveSegmentInstance,
				char	* attributeName,
				int_t	ifcContextInstance,
				double	relativeEpsilon,
				double	factor__P2L
			)
{
	int_t	* mirrorADBValue = nullptr, * ADBValue = nullptr;
	sdaiGetAttrBN(mirrorIfcCurveSegmentInstance, attributeName, sdaiADB, &mirrorADBValue);
	sdaiGetAttrBN(ifcCurveSegmentInstance, attributeName, sdaiADB, &ADBValue);

	int64_t	expressID_original = internalGetP21Line(ifcCurveSegmentInstance),
			expressID_mirror = internalGetP21Line(mirrorIfcCurveSegmentInstance);

	if (mirrorADBValue && ADBValue) {
		const char	* mirrorPath = sdaiGetADBTypePath(mirrorADBValue, sdaiSTRING),
					* path = sdaiGetADBTypePath(ADBValue, sdaiSTRING);

		double	factor;
		if (equals(mirrorPath, "IFCPARAMETERVALUE") &&
			(equals(path, "IFCNONNEGATIVELENGTHMEASURE") || equals(path, "IFCLENGTHMEASURE"))) {
			factor = factor__P2L;
		}
		else if ((equals(mirrorPath, "IFCNONNEGATIVELENGTHMEASURE") || equals(mirrorPath, "IFCLENGTHMEASURE")) &&
			equals(path, "IFCPARAMETERVALUE")) {
			factor = 1. / factor__P2L;
		}
		else {
			assert(equals(mirrorPath, path) || (equals(mirrorPath, "IFCLENGTHMEASURE") || equals(mirrorPath, "IFCNONNEGATIVELENGTHMEASURE")));
			factor = 1.;
		}
			
		double	mirrorValue = 0., value = 0.;
		sdaiGetADBValue(mirrorADBValue, sdaiREAL, &mirrorValue);
		sdaiGetADBValue(ADBValue, sdaiREAL, &value);

		mirrorValue *= factor;

		if (std::fabs(mirrorValue - value) > relativeEpsilon) {
			assert__error(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE, ifcCurveSegmentInstance, ifcContextInstance, attributeName);
		}
		return;
	}

	assert__error(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE, ifcCurveSegmentInstance, ifcContextInstance, attributeName);
}

void	CompareREAL_SET(
				int_t	mirrorIfcCurveSegmentInstance,
				int_t	ifcCurveSegmentInstance,
				char	* attributeName,
				int_t	ifcContextInstance,
				double	relativeEpsilon
			)
{
	assert(relativeEpsilon);

	int_t	* mirrorAggrValue = nullptr, * aggrValue = nullptr;
	sdaiGetAttrBN(mirrorIfcCurveSegmentInstance, attributeName, sdaiAGGR, &mirrorAggrValue);
	sdaiGetAttrBN(ifcCurveSegmentInstance, attributeName, sdaiAGGR, &aggrValue);

	int64_t	expressID_original = internalGetP21Line(ifcCurveSegmentInstance),
			expressID_mirror = internalGetP21Line(mirrorIfcCurveSegmentInstance);

	int_t	mirrorAggrValueCnt = sdaiGetMemberCount(mirrorAggrValue),
			aggrValueCnt = sdaiGetMemberCount(aggrValue);
	if (mirrorAggrValueCnt == aggrValueCnt) {
		for (int_t i = 0; i < aggrValueCnt; i++) {
			double	mirrorValue = 0., value = 0.;
			sdaiGetAggrByIndex(mirrorAggrValue, i, sdaiREAL, &mirrorValue);
			sdaiGetAggrByIndex(aggrValue, i, sdaiREAL, &value);

			if (std::fabs(mirrorValue - value) > relativeEpsilon) {
				assert__error(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE, ifcCurveSegmentInstance, ifcContextInstance, attributeName);
			}
		}
		return;
	}

	assert__error(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE, ifcCurveSegmentInstance, ifcContextInstance, attributeName);
}

void	CompareCartesianPoint(
				int_t	mirrorModel,
				int_t	mirrorIfcCartesianPointInstance,
				int_t	model,
				int_t	ifcCartesianPointInstance,
				int_t	ifcAlignmentParameterSegment,
				double	relativeEpsilon
			)
{
	//	DirectionRatios
	CompareREAL_SET(
			mirrorIfcCartesianPointInstance,
			ifcCartesianPointInstance,
			(char*) "Coordinates",
			ifcAlignmentParameterSegment,
			relativeEpsilon
		);
}


void	CompareDirection(
				int_t	mirrorModel,
				int_t	mirrorIfcDirectionInstance,
				int_t	model,
				int_t	ifcDirectionInstance,
				int_t	ifcAlignmentParameterSegment,
				double	relativeEpsilon
			)
{
	//	DirectionRatios
	CompareREAL_SET(
			mirrorIfcDirectionInstance,
			ifcDirectionInstance,
			(char*) "DirectionRatios",
			ifcAlignmentParameterSegment,
			relativeEpsilon
		);
}

void	ComparePlacement(
				int_t	mirrorModel,
				int_t	mirrorIfcPlacementInstance,
				int_t	model,
				int_t	ifcPlacementInstance,
				int_t	ifcAlignmentParameterSegment,
				double	relativeEpsilon
			)
{
	if (sdaiGetInstanceType(mirrorIfcPlacementInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCAXIS2PLACEMENT2D")) {
		{
			//	Location
			int_t	mirrorIfcPointInstance = 0, ifcPointInstance = 0;
			sdaiGetAttrBN(mirrorIfcPlacementInstance, (char*) "Location", sdaiINSTANCE, &mirrorIfcPointInstance);
			sdaiGetAttrBN(ifcPlacementInstance, (char*) "Location", sdaiINSTANCE, &ifcPointInstance);
			if (mirrorIfcPointInstance && ifcPointInstance &&
				(sdaiGetInstanceType(mirrorIfcPointInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCCARTESIANPOINT") && sdaiGetInstanceType(ifcPointInstance) == sdaiGetEntity(model, (char*) "IFCCARTESIANPOINT"))) {
				CompareCartesianPoint(mirrorModel, mirrorIfcPointInstance, model, ifcPointInstance, ifcAlignmentParameterSegment, relativeEpsilon);
			}
			else {
				int64_t	mirrorExpressID = internalGetP21Line(mirrorIfcPointInstance),
						expressID = internalGetP21Line(ifcPointInstance);
				assert__error(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE, ifcPlacementInstance, ifcAlignmentParameterSegment, (char*) "Location");
			}
		}

		{
			//	RefDirection
			int_t	mirrorIfcDirectionInstance = 0, ifcDirectionInstance = 0;
			sdaiGetAttrBN(mirrorIfcPlacementInstance, (char*) "RefDirection", sdaiINSTANCE, &mirrorIfcDirectionInstance);
			sdaiGetAttrBN(ifcPlacementInstance, (char*) "RefDirection", sdaiINSTANCE, &ifcDirectionInstance);
			if (mirrorIfcDirectionInstance && ifcDirectionInstance &&
				(sdaiGetInstanceType(mirrorIfcDirectionInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCDIRECTION") && sdaiGetInstanceType(ifcDirectionInstance) == sdaiGetEntity(model, (char*) "IFCDIRECTION"))) {
				CompareDirection(mirrorModel, mirrorIfcDirectionInstance, model, ifcDirectionInstance, ifcAlignmentParameterSegment, relativeEpsilon);
			}
			else {
				assert__error(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE, ifcPlacementInstance, ifcAlignmentParameterSegment, (char*) "RefDirection");
			}
		}
	}
	else if (sdaiGetInstanceType(mirrorIfcPlacementInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCAXIS2PLACEMENT3D")) {
		{
			//	Location
			int_t	mirrorIfcPointInstance = 0, ifcPointInstance = 0;
			sdaiGetAttrBN(mirrorIfcPlacementInstance, (char*) "Location", sdaiINSTANCE, &mirrorIfcPointInstance);
			sdaiGetAttrBN(ifcPlacementInstance, (char*) "Location", sdaiINSTANCE, &ifcPointInstance);
			if (mirrorIfcPointInstance && ifcPointInstance &&
				(sdaiGetInstanceType(mirrorIfcPointInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCCARTESIANPOINT") && sdaiGetInstanceType(ifcPointInstance) == sdaiGetEntity(model, (char*) "IFCCARTESIANPOINT"))) {
				CompareCartesianPoint(mirrorModel, mirrorIfcPointInstance, model, ifcPointInstance, ifcAlignmentParameterSegment, relativeEpsilon);
			}
			else {
				assert__error(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE, ifcPlacementInstance, ifcAlignmentParameterSegment, (char*) "Location");
			}
		}

		{
			//	Axis
			int_t	mirrorIfcDirectionInstance = 0, ifcDirectionInstance = 0;
			sdaiGetAttrBN(mirrorIfcPlacementInstance, (char*) "Axis", sdaiINSTANCE, &mirrorIfcDirectionInstance);
			sdaiGetAttrBN(ifcPlacementInstance, (char*) "Axis", sdaiINSTANCE, &ifcDirectionInstance);
			if (mirrorIfcDirectionInstance && ifcDirectionInstance &&
				(sdaiGetInstanceType(mirrorIfcDirectionInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCDIRECTION") && sdaiGetInstanceType(ifcDirectionInstance) == sdaiGetEntity(model, (char*) "IFCDIRECTION"))) {
				CompareDirection(mirrorModel, mirrorIfcDirectionInstance, model, ifcDirectionInstance, ifcAlignmentParameterSegment, relativeEpsilon);
			}
			else {
				assert__error(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE, ifcPlacementInstance, ifcAlignmentParameterSegment, (char*) "Axis");
			}
		}

		{
			//	RefDirection
			int_t	mirrorIfcDirectionInstance = 0, ifcDirectionInstance = 0;
			sdaiGetAttrBN(mirrorIfcPlacementInstance, (char*) "RefDirection", sdaiINSTANCE, &mirrorIfcDirectionInstance);
			sdaiGetAttrBN(ifcPlacementInstance, (char*) "RefDirection", sdaiINSTANCE, &ifcDirectionInstance);
			if (mirrorIfcDirectionInstance && ifcDirectionInstance &&
				(sdaiGetInstanceType(mirrorIfcDirectionInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCDIRECTION") && sdaiGetInstanceType(ifcDirectionInstance) == sdaiGetEntity(model, (char*) "IFCDIRECTION"))) {
				CompareDirection(mirrorModel, mirrorIfcDirectionInstance, model, ifcDirectionInstance, ifcAlignmentParameterSegment, relativeEpsilon);
			}
			else {
				assert__error(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE, ifcPlacementInstance, ifcAlignmentParameterSegment, (char*) "RefDirection");
			}
		}
	}
	else {
		assert(false);
	}
}

void	CompareParentCurve(
				int_t	mirrorModel,
				int_t	mirrorIfcCurveInstance,
				int_t	model,
				int_t	ifcCurveInstance,
				int_t	ifcContentInstance,
				double	relativeEpsilon
			)
{
	if (sdaiGetInstanceType(mirrorIfcCurveInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCLINE") &&
		sdaiGetInstanceType(ifcCurveInstance) == sdaiGetEntity(model, (char*) "IFCLINE")) {
	}
	else if (sdaiGetInstanceType(mirrorIfcCurveInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCCIRCLE") &&
			 sdaiGetInstanceType(ifcCurveInstance) == sdaiGetEntity(model, (char*) "IFCCIRCLE")) {
		//	Radius
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "Radius", ifcContentInstance, relativeEpsilon);
	}
	else if (sdaiGetInstanceType(mirrorIfcCurveInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCCLOTHOID") &&
			 sdaiGetInstanceType(ifcCurveInstance) == sdaiGetEntity(model, (char*) "IFCCLOTHOID")) {
		//	ClothoidConstant
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "ClothoidConstant", ifcContentInstance, relativeEpsilon);
	}
	else if (sdaiGetInstanceType(mirrorIfcCurveInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCCOSINE") &&
			 sdaiGetInstanceType(ifcCurveInstance) == sdaiGetEntity(model, (char*) "IFCCOSINE")) {
		//	CosineTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "CosineTerm", ifcContentInstance, relativeEpsilon);
		//	ConstantTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "ConstantTerm", ifcContentInstance, relativeEpsilon);
	}
	else if (sdaiGetInstanceType(mirrorIfcCurveInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCSINE") &&
			 sdaiGetInstanceType(ifcCurveInstance) == sdaiGetEntity(model, (char*) "IFCSINE")) {
		//	SineTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "SineTerm", ifcContentInstance, relativeEpsilon);
		//	ConstantTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "ConstantTerm", ifcContentInstance, relativeEpsilon);
	}
	else if (sdaiGetInstanceType(mirrorIfcCurveInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCSECONDORDERPOLYNOMIALSPIRAL") &&
			 sdaiGetInstanceType(ifcCurveInstance) == sdaiGetEntity(model, (char*) "IFCSECONDORDERPOLYNOMIALSPIRAL")) {
		//	QuadraticTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "QuadraticTerm", ifcContentInstance, relativeEpsilon);
		//	LinearTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "LinearTerm", ifcContentInstance, relativeEpsilon);
		//	ConstantTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "ConstantTerm", ifcContentInstance, relativeEpsilon);
	}
	else if (sdaiGetInstanceType(mirrorIfcCurveInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCTHIRDORDERPOLYNOMIALSPIRAL") &&
			 sdaiGetInstanceType(ifcCurveInstance) == sdaiGetEntity(model, (char*) "IFCTHIRDORDERPOLYNOMIALSPIRAL")) {
		//	QubicTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "QubicTerm", ifcContentInstance, relativeEpsilon);
		//	QuadraticTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "QuadraticTerm", ifcContentInstance, relativeEpsilon);
		//	LinearTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "LinearTerm", ifcContentInstance, relativeEpsilon);
		//	ConstantTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "ConstantTerm", ifcContentInstance, relativeEpsilon);
	}
	else if (sdaiGetInstanceType(mirrorIfcCurveInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCSEVENTHORDERPOLYNOMIALSPIRAL") &&
			 sdaiGetInstanceType(ifcCurveInstance) == sdaiGetEntity(model, (char*) "IFCSEVENTHORDERPOLYNOMIALSPIRAL")) {
		//	SepticTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "SepticTerm", ifcContentInstance, relativeEpsilon);
		//	SexticTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "SexticTerm", ifcContentInstance, relativeEpsilon);
		//	QuinticTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "QuinticTerm", ifcContentInstance, relativeEpsilon);
		//	QuarticTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "QuarticTerm", ifcContentInstance, relativeEpsilon);
		//	QubicTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "QubicTerm", ifcContentInstance, relativeEpsilon);
		//	QuadraticTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "QuadraticTerm", ifcContentInstance, relativeEpsilon);
		//	LinearTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "LinearTerm", ifcContentInstance, relativeEpsilon);
		//	ConstantTerm
		CompareREAL(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "ConstantTerm", ifcContentInstance, relativeEpsilon);
	}
	else if (sdaiGetInstanceType(mirrorIfcCurveInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCPOLYNOMIALCURVE") &&
			 sdaiGetInstanceType(ifcCurveInstance) == sdaiGetEntity(model, (char*) "IFCPOLYNOMIALCURVE")) {
		//	CoefficientsX
		CompareREAL_SET(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "CoefficientsX", ifcContentInstance, relativeEpsilon);
		//	CoefficientsY
		CompareREAL_SET(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "CoefficientsY", ifcContentInstance, relativeEpsilon);
		//	CoefficientsZ
		CompareREAL_SET(mirrorIfcCurveInstance, ifcCurveInstance, (char*) "CoefficientsZ", ifcContentInstance, relativeEpsilon);
	}
	else {
		int64_t	mirrorExpressID = internalGetP21Line(mirrorIfcCurveInstance),
				expressID = internalGetP21Line(ifcCurveInstance);

		int_t	mirrorInstanceEntity = sdaiGetInstanceType(mirrorIfcCurveInstance);
		char	* mirrorEntityName = nullptr;
		engiGetEntityName(mirrorInstanceEntity, sdaiSTRING, &mirrorEntityName);

		int_t	instanceEntity = sdaiGetInstanceType(ifcCurveInstance);
		char	* entityName = nullptr;
		engiGetEntityName(instanceEntity, sdaiSTRING, &entityName);

		assert__error__(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_PARENT_CURVE, ifcCurveInstance, ifcContentInstance, mirrorEntityName);

		int u = 0;
	}
}

void	CompareSegmentCurve(
				int_t	mirrorModel,
				int_t	mirrorIfcCurveSegmentInstance,
				int_t	model,
				int_t	ifcCurveSegmentInstance,
				int_t	ifcAlignmentParameterSegment,
				double	relativeEpsilon
			)
{
	assert(sdaiGetInstanceType(mirrorIfcCurveSegmentInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCCURVESEGMENT"));
	assert(sdaiGetInstanceType(ifcCurveSegmentInstance) == sdaiGetEntity(model, (char*) "IFCCURVESEGMENT"));

	//	Transition
/////////////////////!!!!!!!!!!!!!!!!!!!!!!!????????????????????	CompareEnum(mirrorIfcCurveSegmentInstance, ifcCurveSegmentInstance, "Transition", ifcAlignmentParameterSegment);

	{
		//	Placement
		int_t	mirrorIfcPlacementInstance = 0, ifcPlacementInstance = 0;
		sdaiGetAttrBN(mirrorIfcCurveSegmentInstance, (char*) "Placement", sdaiINSTANCE, &mirrorIfcPlacementInstance);
		sdaiGetAttrBN(ifcCurveSegmentInstance, (char*) "Placement", sdaiINSTANCE, &ifcPlacementInstance);
		if (mirrorIfcPlacementInstance && ifcPlacementInstance &&
			((sdaiGetInstanceType(mirrorIfcPlacementInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCAXIS2PLACEMENT2D") && sdaiGetInstanceType(ifcPlacementInstance) == sdaiGetEntity(model, (char*) "IFCAXIS2PLACEMENT2D")) ||
			 (sdaiGetInstanceType(mirrorIfcPlacementInstance) == sdaiGetEntity(mirrorModel, (char*) "IFCAXIS2PLACEMENT3D") && sdaiGetInstanceType(ifcPlacementInstance) == sdaiGetEntity(model, (char*) "IFCAXIS2PLACEMENT3D")))) {
			ComparePlacement(mirrorModel, mirrorIfcPlacementInstance, model, ifcPlacementInstance, ifcAlignmentParameterSegment, relativeEpsilon);
		}
		else {
			int64_t	mirrorExpressID = internalGetP21Line(mirrorIfcPlacementInstance),
					expressID = internalGetP21Line(ifcPlacementInstance);
			assert__error(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE, ifcCurveSegmentInstance, ifcAlignmentParameterSegment, (char*) "Placement");
		}
	}

	bool	isCircle = false,
			isClothoid = false,
			isLine = false;
	double	factor__P2L = 1.;
	
	{
		int_t	mirrorIfcCurveInstance = 0;
		sdaiGetAttrBN(mirrorIfcCurveSegmentInstance, "ParentCurve", sdaiINSTANCE, &mirrorIfcCurveInstance);

		if (sdaiGetInstanceType(mirrorIfcCurveInstance) == sdaiGetEntity(mirrorModel, "IFCCIRCLE")) {
			isCircle = true;
			double	radius = 123456789.98765;
			sdaiGetAttrBN(mirrorIfcCurveInstance, "Radius", sdaiREAL, &radius);
			factor__P2L = radius;
		}

		if (sdaiGetInstanceType(mirrorIfcCurveInstance) == sdaiGetEntity(mirrorModel, "IFCCLOTHOID")) {
			isClothoid = true;
			double	clothoidConstant = 123456789.98765;
			sdaiGetAttrBN(mirrorIfcCurveInstance, "ClothoidConstant", sdaiREAL, &clothoidConstant);
			factor__P2L = clothoidConstant * sqrt(Pi);
		}
	}

	if (isCircle == false && isLine == false) {
		//	SegmentStart
		CompareADB(mirrorIfcCurveSegmentInstance, ifcCurveSegmentInstance, (char*) "SegmentStart", ifcAlignmentParameterSegment, relativeEpsilon, factor__P2L);
	}

	//	SegmentLength
	CompareADB(mirrorIfcCurveSegmentInstance, ifcCurveSegmentInstance, (char*) "SegmentLength", ifcAlignmentParameterSegment, relativeEpsilon, factor__P2L);

	{
		//	ParentCurve
		int_t	mirrorIfcCurveInstance = 0, ifcCurveInstance = 0;
		sdaiGetAttrBN(mirrorIfcCurveSegmentInstance, (char*) "ParentCurve", sdaiINSTANCE, &mirrorIfcCurveInstance);
		sdaiGetAttrBN(ifcCurveSegmentInstance, (char*) "ParentCurve", sdaiINSTANCE, &ifcCurveInstance);
		if (mirrorIfcCurveInstance && ifcCurveInstance) {
			CompareParentCurve(mirrorModel, mirrorIfcCurveInstance, model, ifcCurveInstance, ifcAlignmentParameterSegment, relativeEpsilon);
		}
		else {
			assert__error(enum_error::ALIGNMENT_SEGMENT_INCONSISTENT_ATTRIBUTE, ifcCurveSegmentInstance, ifcAlignmentParameterSegment, (char*) "ParentCurve");
		}
	}
}

void	CompareMirror(
				int_t	mirrorModel,
				int_t	mirrorIfcAlignmentInstance,
				int_t	model,
				int_t	ifcAlignmentInstance,
				double	relativeEpsilon
			)
{
	int_t	ifcHorizontalAlignmentInstance = ___GetAlignmentHorizontal(model, ifcAlignmentInstance, nullptr),
			ifcVerticalAlignmentInstance = ___GetAlignmentVertical(model, ifcAlignmentInstance, nullptr),
			ifcCantAlignmentInstance = ___GetAlignmentCant(model, ifcAlignmentInstance, nullptr);

	int_t	mirrorIfcHorizontalAlignmentInstance = ___GetAlignmentHorizontal(mirrorModel, mirrorIfcAlignmentInstance, nullptr),
			mirrorIfcVerticalAlignmentInstance = ___GetAlignmentVertical(mirrorModel, mirrorIfcAlignmentInstance, nullptr),
			mirrorIfcCantAlignmentInstance = ___GetAlignmentCant(mirrorModel, mirrorIfcAlignmentInstance, nullptr);

	//
	//	complete alignment curves
	//
	int_t	reprA___ = FindRepresentation__(model, ifcAlignmentInstance, 0),
			reprH_ = FindRepresentation__(model, ifcHorizontalAlignmentInstance, 0),
			reprV_ = FindRepresentation__(model, ifcVerticalAlignmentInstance, 0),
			reprC_ = FindRepresentation__(model, ifcCantAlignmentInstance, 0);

	{
		int_t	reprA_0 = FindRepresentation__(model, ifcAlignmentInstance, 0),
				reprA_1 = FindRepresentation__(model, ifcAlignmentInstance, 1),
				reprA_2 = FindRepresentation__(model, ifcAlignmentInstance, 2);

		if (reprA_2) {
			assert__error(enum_error::ALIGNMENT_SHOULD_NOT_HAVE_2P_GEOMETRY, ifcAlignmentInstance);
		}

		if (sdaiGetInstanceType(reprA_0) == sdaiGetEntity(model, "IFCCOMPOSITECURVE")) {
			reprH_ = reprA_0;
			if (sdaiGetInstanceType(reprA_1) == sdaiGetEntity(model, "IFCGRADIENTCURVE")) {
				reprV_ = reprA_1;
				reprC_ = 0;
			}
			else if (sdaiGetInstanceType(reprA_1) == sdaiGetEntity(model, "IFCSEGMENTEDREFERENCECURVE")) {
				reprC_ = reprA_1;
				int_t	ifcBoundedCurveInstance = 0;
				sdaiGetAttrBN(reprA_1, "BaseCurve", sdaiINSTANCE, &ifcBoundedCurveInstance);
				if (sdaiGetInstanceType(ifcBoundedCurveInstance) == sdaiGetEntity(model, "IFCGRADIENTCURVE")) {
					reprV_ = ifcBoundedCurveInstance;
				}
				else {
					assert(sdaiGetInstanceType(ifcBoundedCurveInstance) == sdaiGetEntity(model, "IFCCOMPOSITECURVE"));
					reprV_ = 0;
				}
			}
			else {
				assert(reprA_1 == 0);
				reprV_ = 0;
				reprC_ = 0;
			}
		}
		else if (sdaiGetInstanceType(reprA_1) == sdaiGetEntity(model, "IFCCOMPOSITECURVE")) {
			reprH_ = reprA_1;
			if (sdaiGetInstanceType(reprA_0) == sdaiGetEntity(model, "IFCGRADIENTCURVE")) {
				reprV_ = reprA_0;
				reprC_ = 0;
			}
			else if (sdaiGetInstanceType(reprA_0) == sdaiGetEntity(model, "IFCSEGMENTEDREFERENCECURVE")) {
				reprC_ = reprA_0;
				int_t	ifcBoundedCurveInstance = 0;
				sdaiGetAttrBN(reprA_0, "BaseCurve", sdaiINSTANCE, &ifcBoundedCurveInstance);
				if (sdaiGetInstanceType(ifcBoundedCurveInstance) == sdaiGetEntity(model, "IFCGRADIENTCURVE")) {
					reprV_ = ifcBoundedCurveInstance;
				}
				else {
					assert(sdaiGetInstanceType(ifcBoundedCurveInstance) == sdaiGetEntity(model, "IFCCOMPOSITECURVE"));
					reprV_ = 0;
				}
			}
			else {
				assert(reprA_1 == 0);
				reprV_ = 0;
				reprC_ = 0;
			}
		}
		else {
			assert__error(enum_error::ALIGNMENT_SHOULD_HAVE_H_GEOMETRY, ifcAlignmentInstance);
		}

		if (FindRepresentation__(model, ifcHorizontalAlignmentInstance, 0)) {
			assert__error(enum_error::ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY, ifcHorizontalAlignmentInstance);
//			assert(false);
		}

		if (FindRepresentation__(model, ifcVerticalAlignmentInstance, 0)) {
			assert__error(enum_error::ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY, ifcVerticalAlignmentInstance);
//			assert(false);
		}

		if (FindRepresentation__(model, ifcCantAlignmentInstance, 0)) {
			assert__error(enum_error::ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY, ifcCantAlignmentInstance);
//			assert(false);
		}
	}

	int_t	mirrorReprA = FindRepresentation__(mirrorModel, mirrorIfcAlignmentInstance, 0),
			mirrorReprH = FindRepresentation__(mirrorModel, mirrorIfcHorizontalAlignmentInstance, 0),
			mirrorReprV = FindRepresentation__(mirrorModel, mirrorIfcVerticalAlignmentInstance, 0),
			mirrorReprC = FindRepresentation__(mirrorModel, mirrorIfcCantAlignmentInstance, 0);

	assert(mirrorReprA && mirrorReprH == 0 && mirrorReprV == 0 && mirrorReprC == 0);

	{
		int_t	mirrorReprA_0 = FindRepresentation__(mirrorModel, mirrorIfcAlignmentInstance, 0),
				mirrorReprA_1 = FindRepresentation__(mirrorModel, mirrorIfcAlignmentInstance, 1),
				mirrorReprA_2 = FindRepresentation__(mirrorModel, mirrorIfcAlignmentInstance, 2);

		assert(mirrorReprA_0 && mirrorReprA_2 == 0);

		if (sdaiGetInstanceType(mirrorReprA_0) == sdaiGetEntity(mirrorModel, "IFCCOMPOSITECURVE")) {
			mirrorReprH = mirrorReprA_0;
			if (sdaiGetInstanceType(mirrorReprA_1) == sdaiGetEntity(mirrorModel, "IFCSEGMENTEDREFERENCECURVE")) {
				mirrorReprC = mirrorReprA_1;

				int_t	ifcBoundedCurveInstance = 0;
				sdaiGetAttrBN(mirrorReprA_1, "BaseCurve", sdaiINSTANCE, &ifcBoundedCurveInstance);
				if (sdaiGetInstanceType(ifcBoundedCurveInstance) == sdaiGetEntity(mirrorModel, "IFCGRADIENTCURVE")) {
					mirrorReprV = ifcBoundedCurveInstance;
				}
				else {
					assert(sdaiGetInstanceType(ifcBoundedCurveInstance) == sdaiGetEntity(mirrorModel, "IFCCOMPOSITECURVE"));
				}
			}
			else if (sdaiGetInstanceType(mirrorReprA_1) == sdaiGetEntity(mirrorModel, "IFCGRADIENTCURVE")) {
				mirrorReprV = mirrorReprA_1;
			}
			else {
				assert(mirrorReprA_1 == 0);
			}
		}
		else {
			assert(sdaiGetInstanceType(mirrorReprA_1) == sdaiGetEntity(mirrorModel, "IFCCOMPOSITECURVE"));
			mirrorReprH = mirrorReprA_1;
			if (sdaiGetInstanceType(mirrorReprA_0) == sdaiGetEntity(mirrorModel, "IFCSEGMENTEDREFERENCECURVE")) {
				mirrorReprC = mirrorReprA_0;

				int_t	ifcBoundedCurveInstance = 0;
				sdaiGetAttrBN(mirrorReprA_0, "BaseCurve", sdaiINSTANCE, &ifcBoundedCurveInstance);
				if (sdaiGetInstanceType(ifcBoundedCurveInstance) == sdaiGetEntity(mirrorModel, "IFCGRADIENTCURVE")) {
					mirrorReprV = ifcBoundedCurveInstance;
				}
				else {
					assert(sdaiGetInstanceType(ifcBoundedCurveInstance) == sdaiGetEntity(mirrorModel, "IFCCOMPOSITECURVE"));
				}
			}
			else {
				assert(sdaiGetInstanceType(mirrorReprA_0) == sdaiGetEntity(mirrorModel, "IFCGRADIENTCURVE"));
				mirrorReprV = mirrorReprA_0;
			}
		}
	}
//	if (reprA && mirrorReprA == 0) {
//		assert__error(enum_error::ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY, ifcAlignmentInstance);
//		reprA = 0;
//	}

//	if (reprH && mirrorReprH == 0) {
//		assert__error(enum_error::HORIZONTAL_ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY, ifcHorizontalAlignmentInstance);
//		reprH = 0;
//	}

//	if (reprV && mirrorReprV == 0) {
//		assert__error(enum_error::VERTICAL_ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY, ifcVerticalAlignmentInstance);
//		reprV = 0;
//	}

//	if (reprC && mirrorReprC == 0) {
//		assert__error(enum_error::CANT_ALIGNMENT_SHOULD_NOT_HAVE_GEOMETRY, ifcCantAlignmentInstance);
//		reprC = 0;
//	}

	assert(mirrorReprH && (mirrorReprV || ifcVerticalAlignmentInstance == 0) && (mirrorReprA || ifcCantAlignmentInstance == 0));

//	if (reprA == 0) {
//		assert__error(enum_error::ALIGNMENT_MISSING_GEOMETRY, ifcAlignmentInstance);
//	}
//	else if (sdaiGetInstanceType(reprA) != sdaiGetEntity(model, (char*) "IFCGRADIENTCURVE")) {
//		assert__error(enum_error::ALIGNMENT_INCORRECT_GEOMETRY_ENTITY, ifcAlignmentInstance, reprA);
//	}

//////////////////////////	assert(mirrorReprA == mirrorReprV);
//	if (reprA != reprV) {
//		assert__error(enum_error::ALIGNMENT_INCORRECT_GEOMETRY_ENTITY, ifcAlignmentInstance, ifcVerticalAlignmentInstance);
//	}

	if (reprH_ == 0) {
		assert__error(enum_error::HORIZONTAL_ALIGNMENT_MISSING_GEOMETRY, ifcAlignmentInstance);
	}
	else if (sdaiGetInstanceType(reprH_) != sdaiGetEntity(model, "IFCCOMPOSITECURVE")) {
		assert__error(enum_error::HORIZONTAL_ALIGNMENT_INCORRECT_GEOMETRY_ENTITY, ifcAlignmentInstance, reprH_);
	}

	if (mirrorReprV) {
		if (reprV_ == 0) {
			assert__error(enum_error::VERTICAL_ALIGNMENT_MISSING_GEOMETRY, ifcAlignmentInstance);
		}
		else if (sdaiGetInstanceType(reprV_) != sdaiGetEntity(model, "IFCGRADIENTCURVE")) {
			assert__error(enum_error::VERTICAL_ALIGNMENT_INCORRECT_GEOMETRY_ENTITY, ifcAlignmentInstance, reprV_);
		}
	}

	if (mirrorReprC) {
		if (reprC_ == 0) {
			assert__error(enum_error::CANT_ALIGNMENT_MISSING_GEOMETRY, ifcAlignmentInstance);
		}
		else if (sdaiGetInstanceType(reprC_) != sdaiGetEntity(model, "IFCSEGMENTEDREFERENCECURVE")) {
			assert__error(enum_error::CANT_ALIGNMENT_INCORRECT_GEOMETRY_ENTITY, ifcAlignmentInstance, reprC_);
		}
	}
	else {
		assert(___GetAlignmentCant(mirrorModel, mirrorIfcAlignmentInstance, nullptr) == 0);
	}

	if (reprV_ == 0) {
		if (reprH_) {
			assert(sdaiGetInstanceType(reprH_) == sdaiGetEntity(model, "IFCCOMPOSITECURVE"));
			int_t	* ifcGradientCurveInstances = sdaiGetEntityExtentBN(model, "IFCGRADIENTCURVE"),
					noIfcGradientCurveInstances = sdaiGetMemberCount(ifcGradientCurveInstances);
			for (int_t i = 0; i < noIfcGradientCurveInstances; i++) {
				int_t	ifcGradientCurveInstance = 0;
				sdaiGetAggrByIndex(ifcGradientCurveInstances, i, sdaiINSTANCE, &ifcGradientCurveInstance);

				int_t	myBaseCurveInstance = 0;
				sdaiGetAttrBN(ifcGradientCurveInstance, (char*) "BaseCurve", sdaiINSTANCE, &myBaseCurveInstance);
				if (myBaseCurveInstance == reprH_) {
					assert(reprV_ == 0);
					reprV_ = ifcGradientCurveInstance;
				}
			}
			assert(reprV_ || ifcVerticalAlignmentInstance == 0);
		}
	}

	if (reprC_ == 0 && mirrorReprC) {
		if (reprV_) {
			assert(sdaiGetInstanceType(reprV_) == sdaiGetEntity(model, "IFCGRADIENTCURVE"));
			int_t	* ifcSegmentedReferenceCurveInstances = sdaiGetEntityExtentBN(model, "IFCSEGMENTEDREFERENCECURVE"),
					noIfcSegmentedReferenceCurveInstances = sdaiGetMemberCount(ifcSegmentedReferenceCurveInstances);
			for (int_t i = 0; i < noIfcSegmentedReferenceCurveInstances; i++) {
				int_t	ifcSegmentedReferenceCurveInstance = 0;
				sdaiGetAggrByIndex(ifcSegmentedReferenceCurveInstances, i, sdaiINSTANCE, &ifcSegmentedReferenceCurveInstance);

				int_t	myBaseCurveInstance = 0;
				sdaiGetAttrBN(ifcSegmentedReferenceCurveInstance, "BaseCurve", sdaiINSTANCE, &myBaseCurveInstance);
				if (myBaseCurveInstance == reprV_) {
					assert(reprC_ == 0);
					reprC_ = ifcSegmentedReferenceCurveInstance;
				}
			}
			assert(reprC_);
		}
	}

	//
	//	alignment segment curves
	//

	if (ifcHorizontalAlignmentInstance || mirrorIfcHorizontalAlignmentInstance) {
		//	Horizontal Alignment
		int_t   noSegmentInstances =
					___GetAlignmentSegments(
							model,
							ifcHorizontalAlignmentInstance,
							nullptr
						),
				mirrorNoSegmentInstances =
					___GetAlignmentSegments(
							mirrorModel,
							mirrorIfcHorizontalAlignmentInstance,
							nullptr
						);

		assert(noSegmentInstances == mirrorNoSegmentInstances);
		if (noSegmentInstances && noSegmentInstances == mirrorNoSegmentInstances) {
			int_t   * segmentInstances = new int_t[noSegmentInstances];
			___GetAlignmentSegments(
					model,
					ifcHorizontalAlignmentInstance,
					segmentInstances
				);

			int_t	* mirrorSegmentInstances = new int_t[mirrorNoSegmentInstances];
			___GetAlignmentSegments(
					mirrorModel,
					mirrorIfcHorizontalAlignmentInstance,
					mirrorSegmentInstances
				);

			int_t	currentIndexCnt = 0;
			for (int_t i = 0; i < noSegmentInstances; i++) {
				int_t   ifcAlignmentSegmentInstance = segmentInstances[i],
						mirrorIfcAlignmentSegmentInstance = mirrorSegmentInstances[i];
				int_t	reprAlignmentSegmentInstanceI = 0, reprAlignmentSegmentInstanceII = 0,
						mirrorReprAlignmentSegmentInstanceI = 0, mirrorReprAlignmentSegmentInstanceII = 0;
				FindRepresentation(model, ifcAlignmentSegmentInstance, &reprAlignmentSegmentInstanceI, &reprAlignmentSegmentInstanceII);
				FindRepresentation(mirrorModel, mirrorIfcAlignmentSegmentInstance, &mirrorReprAlignmentSegmentInstanceI, &mirrorReprAlignmentSegmentInstanceII);

				if (reprAlignmentSegmentInstanceI == 0) {
					assert__error(enum_error::ALIGNMENT_SEGMENT_MISSES_GEOMETRY_HA, ifcAlignmentSegmentInstance);
				}
				else {
					int_t	index = GetIndex(model, reprH_, reprAlignmentSegmentInstanceI),
							mirrorIndex = GetIndex(mirrorModel, mirrorReprH, mirrorReprAlignmentSegmentInstanceI);
					if (index >= 0 && index == mirrorIndex && currentIndexCnt == index) {
						CompareSegmentCurve(mirrorModel, mirrorReprAlignmentSegmentInstanceI, model, reprAlignmentSegmentInstanceI, segmentInstances[i], relativeEpsilon);
						currentIndexCnt++;
						if (mirrorReprAlignmentSegmentInstanceII) {
							CompareSegmentCurve(mirrorModel, mirrorReprAlignmentSegmentInstanceII, model, reprAlignmentSegmentInstanceII, segmentInstances[i], relativeEpsilon);
							assert(reprAlignmentSegmentInstanceII);
							currentIndexCnt++;
						}
						else {
							assert(reprAlignmentSegmentInstanceII == 0);
						}
					}
					else {
						assert__error(enum_error::ALIGNMENT_SEGMENT_INCORRECT_ORDER_BOUNDED_CURVE, ifcAlignmentSegmentInstance, reprAlignmentSegmentInstanceI);
					}
				}
			}
		}
		else {
			assert__error(enum_error::ALIGNMENT_SEGMENT_INCORRECT_SEGMENT_COUNT_BOUNDED_CURVE, ifcHorizontalAlignmentInstance, reprH_);
		}
	}
	else {
		assert(ifcHorizontalAlignmentInstance == 0 && mirrorIfcHorizontalAlignmentInstance == 0);
	}

	if (ifcVerticalAlignmentInstance || mirrorIfcVerticalAlignmentInstance) {
		//	Vertical Alignment
		int_t   noSegmentInstances =
					___GetAlignmentSegments(
							model,
							ifcVerticalAlignmentInstance,
							nullptr
						),
				mirrorNoSegmentInstances =
					___GetAlignmentSegments(
							mirrorModel,
							mirrorIfcVerticalAlignmentInstance,
							nullptr
						);

		assert(noSegmentInstances == mirrorNoSegmentInstances);
		if (noSegmentInstances && noSegmentInstances == mirrorNoSegmentInstances) {
			int_t   * segmentInstances = new int_t[noSegmentInstances];
			___GetAlignmentSegments(
					model,
					ifcVerticalAlignmentInstance,
					segmentInstances
				);

			int_t	* mirrorSegmentInstances = new int_t[mirrorNoSegmentInstances];
			___GetAlignmentSegments(
					mirrorModel,
					mirrorIfcVerticalAlignmentInstance,
					mirrorSegmentInstances
				);

			int_t	currentIndexCnt = 0;
			for (int_t i = 0; i < noSegmentInstances; i++) {
				int_t   ifcAlignmentSegmentInstance = segmentInstances[i],
						mirrorIfcAlignmentSegmentInstance = mirrorSegmentInstances[i];
				int_t	reprAlignmentSegmentInstanceI = 0, reprAlignmentSegmentInstanceII = 0,
						mirrorReprAlignmentSegmentInstanceI = 0, mirrorReprAlignmentSegmentInstanceII = 0;
				FindRepresentation(model, ifcAlignmentSegmentInstance, &reprAlignmentSegmentInstanceI, &reprAlignmentSegmentInstanceII);
				FindRepresentation(mirrorModel, mirrorIfcAlignmentSegmentInstance, &mirrorReprAlignmentSegmentInstanceI, &mirrorReprAlignmentSegmentInstanceII);

				if (reprAlignmentSegmentInstanceI == 0) {
					bool	isLastItemThatCanBeIgnored = false;
					if (i == noSegmentInstances - 1) {
						SdaiInstance	ifcAlignmentVerticalSegmentInstance = 0;
						sdaiGetAttrBN(ifcAlignmentSegmentInstance, "DesignParameters", sdaiINSTANCE, &ifcAlignmentVerticalSegmentInstance);
						if (ifcAlignmentVerticalSegmentInstance && sdaiGetInstanceType(ifcAlignmentVerticalSegmentInstance) == sdaiGetEntity(model, "IfcAlignmentVerticalSegment")) {
							double	horizontalLength = 0.;
							if (sdaiGetAttrBN(ifcAlignmentVerticalSegmentInstance, "HorizontalLength", sdaiREAL, &horizontalLength) && horizontalLength == 0.) {
								isLastItemThatCanBeIgnored = true;
							}
						}
					}

					if (!isLastItemThatCanBeIgnored) {
						assert__error(enum_error::ALIGNMENT_SEGMENT_MISSES_GEOMETRY_VA, ifcAlignmentSegmentInstance);
					}
				}
				else {
					int_t	index = GetIndex(model, reprV_, reprAlignmentSegmentInstanceI),
							mirrorIndex = GetIndex(mirrorModel, mirrorReprV, mirrorReprAlignmentSegmentInstanceI);
					if (index >= 0 && index == mirrorIndex && currentIndexCnt == index) {
						CompareSegmentCurve(mirrorModel, mirrorReprAlignmentSegmentInstanceI, model, reprAlignmentSegmentInstanceI, segmentInstances[i], relativeEpsilon);
						currentIndexCnt++;
						if (mirrorReprAlignmentSegmentInstanceII) {
							CompareSegmentCurve(mirrorModel, mirrorReprAlignmentSegmentInstanceII, model, reprAlignmentSegmentInstanceII, segmentInstances[i], relativeEpsilon);
							assert(reprAlignmentSegmentInstanceII);
							currentIndexCnt++;
						}
						else {
							assert(reprAlignmentSegmentInstanceII == 0);
						}
					}
					else {
						assert__error(enum_error::ALIGNMENT_SEGMENT_INCORRECT_ORDER_BOUNDED_CURVE, ifcAlignmentSegmentInstance, reprAlignmentSegmentInstanceI);
					}
				}
			}
		}
		else {
			assert__error(enum_error::ALIGNMENT_SEGMENT_INCORRECT_SEGMENT_COUNT_BOUNDED_CURVE, ifcVerticalAlignmentInstance, reprH_);
		}
	}
	else {
		assert(ifcVerticalAlignmentInstance == 0 && mirrorIfcVerticalAlignmentInstance == 0);
	}

	if (ifcCantAlignmentInstance || mirrorIfcCantAlignmentInstance) {
		//	Cant Alignment
		int_t   noSegmentInstances =
					___GetAlignmentSegments(
							model,
							ifcCantAlignmentInstance,
							nullptr
						),
				mirrorNoSegmentInstances =
					___GetAlignmentSegments(
							mirrorModel,
							mirrorIfcCantAlignmentInstance,
							nullptr
						);

		assert(noSegmentInstances == mirrorNoSegmentInstances);
		if (noSegmentInstances && noSegmentInstances == mirrorNoSegmentInstances) {
			int_t   * segmentInstances = new int_t[noSegmentInstances];
			___GetAlignmentSegments(
					model,
					ifcCantAlignmentInstance,
					segmentInstances
				);

			int_t	* mirrorSegmentInstances = new int_t[mirrorNoSegmentInstances];
			___GetAlignmentSegments(
					mirrorModel,
					mirrorIfcCantAlignmentInstance,
					mirrorSegmentInstances
				);

			int_t	currentIndexCnt = 0;
			for (int_t i = 0; i < noSegmentInstances; i++) {
				int_t   ifcAlignmentSegmentInstance = segmentInstances[i],
						mirrorIfcAlignmentSegmentInstance = mirrorSegmentInstances[i];
				int_t	reprAlignmentSegmentInstanceI = 0, reprAlignmentSegmentInstanceII = 0,
						mirrorReprAlignmentSegmentInstanceI = 0, mirrorReprAlignmentSegmentInstanceII = 0;
				FindRepresentation(model, ifcAlignmentSegmentInstance, &reprAlignmentSegmentInstanceI, &reprAlignmentSegmentInstanceII);
				FindRepresentation(mirrorModel, mirrorIfcAlignmentSegmentInstance, &mirrorReprAlignmentSegmentInstanceI, &mirrorReprAlignmentSegmentInstanceII);

				if (reprAlignmentSegmentInstanceI == 0) {
					assert__error(enum_error::ALIGNMENT_SEGMENT_MISSES_GEOMETRY_CA, ifcAlignmentSegmentInstance);
				}
				else {
					int_t	index = GetIndex(model, reprC_, reprAlignmentSegmentInstanceI),
							mirrorIndex = GetIndex(mirrorModel, mirrorReprC, mirrorReprAlignmentSegmentInstanceI);
					if (index >= 0 && index == mirrorIndex && currentIndexCnt == index) {
						CompareSegmentCurve(mirrorModel, mirrorReprAlignmentSegmentInstanceI, model, reprAlignmentSegmentInstanceI, segmentInstances[i], relativeEpsilon);
						currentIndexCnt++;
						if (mirrorReprAlignmentSegmentInstanceII) {
							CompareSegmentCurve(mirrorModel, mirrorReprAlignmentSegmentInstanceII, model, reprAlignmentSegmentInstanceII, segmentInstances[i], relativeEpsilon);
							assert(reprAlignmentSegmentInstanceII);
							currentIndexCnt++;
						}
						else {
							assert(reprAlignmentSegmentInstanceII == 0);
						}
					}
					else {
						assert__error(enum_error::ALIGNMENT_SEGMENT_INCORRECT_ORDER_BOUNDED_CURVE, ifcAlignmentSegmentInstance, reprAlignmentSegmentInstanceI);
					}
				}
			}
		}
		else {
			assert__error(enum_error::ALIGNMENT_SEGMENT_INCORRECT_SEGMENT_COUNT_BOUNDED_CURVE, ifcCantAlignmentInstance, reprH_);
		}
	}
	else {
		assert(ifcCantAlignmentInstance == 0 && mirrorIfcCantAlignmentInstance == 0);
	}
}
/*
bool	GetGeometryFromGK__point4D__SEMANTICS(
				OwlModel	owlModel,
				OwlInstance	owlInstancePoint4D,
				___POINT4D	* point4D
			)
{
#ifdef _DEBUG
	SaveInstanceTree(owlInstancePoint4D, "c:\\0\\poinbt4d.bin");
#endif // _DEBUG

	
	bool	asExpected = true;

	OwlClass myClass = GetInstanceClass(owlInstancePoint4D);
	if (myClass == GetClassByName(owlModel, "Point4D")) {
		{
			OwlClass	* values = nullptr;
			int64_t		card = 0;
			GetObjectProperty(owlInstancePoint4D, GetPropertyByName(owlModel, "point"), &values, &card);
			if (card == 1 && values[0]) {
				OwlInstance	owlInstancePoint3D = values[0];
				assert(GetInstanceClass(owlInstancePoint3D) == GetClassByName(owlModel, "Point3D"));

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "x"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->point.x = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->point.x = 0.;
					}
				}

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "y"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->point.y = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->point.y = 0.;
					}
				}

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "z"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->point.z = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->point.z = 0.;
					}
				}
			}
			else {
				assert(false);
				asExpected = false;
			}
		}

		{
			OwlClass	* values = nullptr;
			int64_t		card = 0;
			GetObjectProperty(owlInstancePoint4D, GetPropertyByName(owlModel, "tangent"), &values, &card);
			if (card == 1 && values[0]) {
				OwlInstance	owlInstancePoint3D = values[0];
				assert(GetInstanceClass(owlInstancePoint3D) == GetClassByName(owlModel, "Vector3"));

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "x"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->tangent.x = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->tangent.x = 0.;
					}
				}

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "y"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->tangent.y = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->tangent.y = 0.;
					}
				}

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "z"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->tangent.z = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->tangent.z = 0.;
					}
				}
			}
			else {
				assert(false);
				asExpected = false;
			}
		}

		{
			OwlClass	* values = nullptr;
			int64_t		card = 0;
			GetObjectProperty(owlInstancePoint4D, GetPropertyByName(owlModel, "normal"), &values, &card);
			if (card == 1 && values[0]) {
				OwlInstance	owlInstancePoint3D = values[0];
				assert(GetInstanceClass(owlInstancePoint3D) == GetClassByName(owlModel, "Vector3"));

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "x"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->normal.x = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->normal.x = 0.;
					}
				}

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "y"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->normal.y = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->normal.y = 0.;
					}
				}

				{
					double	* pValues = nullptr;
					GetDatatypeProperty(owlInstancePoint3D, GetPropertyByName(owlModel, "z"), (void**) &pValues, &card);
					if (card == 1) {
						point4D->normal.z = pValues[0];
					}
					else {
						assert(false);
						asExpected = false;
						point4D->normal.z = 0.;
					}
				}
			}
			else {
				assert(false);
				asExpected = false;
			}
		}
	}
	else {
		assert(false);
		asExpected = false;
	}

	return	asExpected;
}

bool	GetGeometryFromGK__SEMANTICS(
				OwlModel	owlModel,
				OwlInstance	owlInstance,
				___POINT4D	* startPoint,
				___POINT4D	* endPoint
			)
{
	bool	asExpected = true;

	OwlClass myClass = GetInstanceClass(owlInstance);
	if (myClass == GetClassByName(owlModel, "PolyLine3D")) {
		OwlClass	* values = nullptr;
		int64_t		card = 0;
		GetObjectProperty(owlInstance, GetPropertyByName(owlModel, "pointReferences"), &values, &card);
		if (card && values[0] && values[card - 1]) {
			if (GetGeometryFromGK__point4D__SEMANTICS(owlModel, values[0], startPoint) == false)
				asExpected = false;
			if (GetGeometryFromGK__point4D__SEMANTICS(owlModel, values[card - 1], endPoint) == false)
				asExpected = false;
		}
		else {
			assert(false);
			asExpected = false;
		}
	}
	else if (myClass == GetClassByName(owlModel, "Point4D")) {
		if (GetGeometryFromGK__point4D__SEMANTICS(owlModel, owlInstance, startPoint) == false)
			asExpected = false;
		if (GetGeometryFromGK__point4D__SEMANTICS(owlModel, owlInstance, endPoint) == false)
			asExpected = false;
	}
	else {
		assert(false);
		asExpected = false;
	}

	return	asExpected;
}	//	*/

#ifdef _DEBUG
int_t	GetGeometryFromGK__GEOMETRY(
				OwlModel	owlModel,
				OwlInstance	owlInstance,
				___VECTOR3	* startVec,
				___VECTOR3	* endVec
			)
{
	int64_t setting = 0,
			mask = GetFormat(0, 0);

    setting += 1 * flagbit2;        //    SINGLE / DOUBLE PRECISION (float / double)
    setting += 0 * flagbit3;        //    32 / 63 BIT INDEX ARRAY (int32_t / int64_t)

    setting += 1 * flagbit4;        //    OFF / ON VECTORS (x, y, z) 
    setting += 1 * flagbit5;        //    OFF / ON NORMALS (Nx, Ny, Nz)

    setting += 0 * flagbit8;        //    OFF / ON TRIANGLES
    setting += 1 * flagbit9;        //    OFF / ON LINES
    setting += 1 * flagbit10;       //    OFF / ON POINTS

    setting += 0 * flagbit12;       //    OFF / ON WIREFRAME FACES
    setting += 0 * flagbit13;       //    OFF / ON WIREFRAME CONCEPTUAL FACES

    int64_t vertexElementSizeInBytes = SetFormat(owlModel, setting, mask);
    assert(vertexElementSizeInBytes == (3 + 3) * sizeof(double));

	int64_t	vertexBufferSize = 0, indexBufferSize = 0;
	CalculateInstance(owlInstance, &vertexBufferSize, &indexBufferSize, nullptr);
	if (vertexBufferSize && indexBufferSize) {
		double	* vertices = new double[6 * (int_t) vertexBufferSize];
		UpdateInstanceVertexBuffer(owlInstance, vertices);
		int32_t	* indices = new int32_t[(int_t) indexBufferSize];
		UpdateInstanceIndexBuffer(owlInstance, indices);

		int64_t	cnt = GetConceptualFaceCnt(owlInstance), lineCnt = 0;
		for (int64_t i = 0; i < cnt; i++) {
			int64_t	startIndexLines = 0, noIndicesLines = 0;
			GetConceptualFace(
					owlInstance, i,
					nullptr, nullptr,
					&startIndexLines, &noIndicesLines,
					nullptr, nullptr
				);

			if (noIndicesLines) {
				if (lineCnt == 0) {
					memcpy(startVec, &vertices[6 * indices[startIndexLines]], 3 * sizeof(double));
				}
				memcpy(endVec, &vertices[6 * indices[startIndexLines + noIndicesLines - 1]], 3 * sizeof(double));
				lineCnt += noIndicesLines;
			}
		}

		if (vertexBufferSize == 1 && indexBufferSize == 1) {
			assert(lineCnt == 0);
			startVec->x = endVec->x = vertices[0];
			startVec->y = endVec->y = vertices[1];
			startVec->z = endVec->z = vertices[2];
			lineCnt = 1;
		}

		delete[] vertices;
		delete[] indices;
		return	(int_t) lineCnt;
	}

	return	0;
}
#endif // _DEBUG

bool	GetGeometryFromIFC(
				int_t		model,
				int_t		ifcAlignmentSegmentInstance,
				int_t		ifcAlignmentInstance,
				___POINT4D	* startPoint,
				___POINT4D	* endPoint
			)
{
	int64_t	owlModel = 0, owlInstance = 0;
	owlGetModel(model, &owlModel);
	owlBuildInstanceInContext(ifcAlignmentSegmentInstance, sdaiGetInstanceType(ifcAlignmentInstance), &owlInstance);

#ifdef _DEBUG
	int64_t	expressID_segment = internalGetP21Line(ifcAlignmentSegmentInstance),
			expressID_boundeccurve = internalGetP21Line(ifcAlignmentInstance);


	sdaiSaveModelBN(model, "C:\\IFCRAIL\\acca2__.ifc");
#endif // _DEBUG

#ifdef _DEBUG
	___VECTOR3	_sVec, _eVec;
	int_t	lineCnt =
				GetGeometryFromGK__GEOMETRY(
							owlModel,
							owlInstance,
							&_sVec,
							&_eVec
						);
#endif // _DEBUG

	bool	asExpected =
				___GetBorderPoints__SEMANTICS(
						owlInstance,
						startPoint,
						endPoint
					);

	assert(___Vec3Distance(&_sVec, &startPoint->point) == 0.);
	assert(___Vec3Distance(&_eVec, &endPoint->point) == 0.);

//	return	lineCnt;
	return	asExpected;
}

void	CheckGeometrySegments(
				int_t			model,
				int_t			ifcAlignmentNestedInstance,
				enum_alignment	enumAlignment
			)
{
    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
						ifcAlignmentNestedInstance,
                        nullptr
                    );

    if (noSegmentInstances) {
        int_t   * segmentInstances = new int_t[noSegmentInstances];
        ___GetAlignmentSegments(
                model,
				ifcAlignmentNestedInstance,
                segmentInstances
            );

		if (enumAlignment == enum_alignment::CANT) {
			int u = 0;
		}

		for (int_t i = 0; i < noSegmentInstances; i++) {
			int_t	ifcCurveSegmentInstance = segmentInstances[i];

			int_t entity = sdaiGetInstanceType(ifcCurveSegmentInstance);
			char	* entityName = nullptr;
			engiGetEntityName(entity, sdaiSTRING, (const char**) &entityName);

			assert(sdaiGetInstanceType(ifcCurveSegmentInstance) == sdaiGetEntity(model, "IFCALIGNMENTSEGMENT"));
//			...

			char	* transitionCode = nullptr;
			sdaiGetAttrBN(ifcCurveSegmentInstance, "Transition", sdaiENUM, &transitionCode);
			

			//	CONTINUOUS
			//	CONTSAMEGRADIENT
			//	CONTSAMEGRADIENTSAMECURVATURE
			//	DISCONTINUOUS

//			....
				int uu = 0;
		}

		___POINT4D	previousEndPnt = { { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0. } };
		for (int_t index = 0; index < noSegmentInstances; index++) {
			___POINT4D	startPnt = { { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0. } },
						endPnt = { { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0. } };
			int_t cnt = GetGeometryFromIFC(model, segmentInstances[index], ifcAlignmentNestedInstance, &startPnt, &endPnt);

int_t myModel = engiGetEntityModel(sdaiGetInstanceType(segmentInstances[index]));
assert(myModel == model);
int_t myEntity = sdaiGetInstanceType(segmentInstances[index]);
const char	* entityName = nullptr;
engiGetEntityName(myEntity, sdaiSTRING, &entityName);

int_t	continuosDegree = 99;
			if (segmentInstances[index]											 &&
				engiGetEntityModel(sdaiGetInstanceType(segmentInstances[index])) &&
				sdaiGetInstanceType(segmentInstances[index]) == sdaiGetEntity(engiGetEntityModel(sdaiGetInstanceType(segmentInstances[index])), "IFCALIGNMENTSEGMENT")) {
				SdaiInstance	ifcAlignmentSegmentInstance = segmentInstances[index],
								ifcProductDefinitionShape = 0;
				sdaiGetAttrBN(ifcAlignmentSegmentInstance, "Representation", sdaiINSTANCE, &ifcProductDefinitionShape);
				SdaiAggr		representationsAGGR = nullptr;
				sdaiGetAttrBN(ifcProductDefinitionShape, "Representations", sdaiAGGR, &representationsAGGR);
				assert(sdaiGetMemberCount(representationsAGGR) == 1);
				SdaiInstance	ifcShapeRepresentation = 0;
				sdaiGetAggrByIndex(representationsAGGR, 0, sdaiINSTANCE, &ifcShapeRepresentation);
				SdaiAggr		itemsAGGR = nullptr;
				sdaiGetAttrBN(ifcShapeRepresentation, "Items", sdaiAGGR, &itemsAGGR);
				assert(sdaiGetMemberCount(itemsAGGR) == 1);
				SdaiInstance	ifcCurveSegment = 0;
				sdaiGetAggrByIndex(itemsAGGR, 0, sdaiINSTANCE, &ifcCurveSegment);
				assert(sdaiGetInstanceType(ifcCurveSegment) == sdaiGetEntity(model, "IFCCURVESEGMENT"));

				const char	* transitionCode = nullptr;
				sdaiGetAttrBN(ifcCurveSegment, "Transition", sdaiENUM, &transitionCode);

				//      CONTINUOUS
				//      CONTSAMEGRADIENT
				//      CONTSAMEGRADIENTSAMECURVATURE
				//      DISCONTINUOUS

				if (___equals(transitionCode, "CONTINUOUS")) {
					continuosDegree = 0;
				}
				else if (___equals(transitionCode, "CONTSAMEGRADIENT")) {
					continuosDegree = 1;
				}
				else if (___equals(transitionCode, "CONTSAMEGRADIENTSAMECURVATURE")) {
					continuosDegree = 2;
				}
				else {
					assert(___equals(transitionCode, "DISCONTINUOUS"));
					continuosDegree = 99;

					assert(index == 0);
				}
			}
			else {
				assert(false);
			}

			if (cnt) {
				if (index) {
					{
						double	distance = ___Vec3Distance(&previousEndPnt.point, &startPnt.point);

						switch (enumAlignment) {
							case  enum_alignment::HORIZONTAL:
								assert__error(
										enum_error::HORIZONTAL_SEGMENT_DISTANCE,
										myMapExpressID[internalGetP21Line(segmentInstances[index - 1])],
										myMapExpressID[internalGetP21Line(segmentInstances[index])],
										distance
									);
								break;
							case  enum_alignment::VERTICAL:
								assert__error(
										enum_error::VERTICAL_SEGMENT_DISTANCE,
										myMapExpressID[internalGetP21Line(segmentInstances[index - 1])],
										myMapExpressID[internalGetP21Line(segmentInstances[index])],
										distance
									);
								break;
							case  enum_alignment::CANT:
if (distance > 0.01) {
	int uu = 0;
}
								assert__error(
										enum_error::CANT_SEGMENT_DISTANCE,
										myMapExpressID[internalGetP21Line(segmentInstances[index - 1])],
										myMapExpressID[internalGetP21Line(segmentInstances[index])],
										distance
									);
								break;
						}
					}

					if (continuosDegree >= 1) {
						double	anglePreviousEndPoint = std::atan2(previousEndPnt.tangent.y, previousEndPnt.tangent.x),
								angleStartPnt = std::atan2(startPnt.tangent.y, startPnt.tangent.x),
								angleDifference = angleStartPnt - anglePreviousEndPoint;

						if (angleDifference >   Pi) { angleDifference -= 2. * Pi; }
						if (angleDifference < - Pi) { angleDifference += 2. * Pi; }

						double	angleDifferenceInDegrees = 360. * angleDifference / (2. * Pi);

						switch (enumAlignment) {
							case  enum_alignment::HORIZONTAL:
								assert__error(
										enum_error::HORIZONTAL_SEGMENT_TANGENT_DEVIATION,
										myMapExpressID[internalGetP21Line(segmentInstances[index - 1])],
										myMapExpressID[internalGetP21Line(segmentInstances[index])],
										std::fabs(angleDifferenceInDegrees)
									);
								break;
							case  enum_alignment::VERTICAL:
								assert__error(
										enum_error::VERTICAL_SEGMENT_TANGENT_DEVIATION,
										myMapExpressID[internalGetP21Line(segmentInstances[index - 1])],
										myMapExpressID[internalGetP21Line(segmentInstances[index])],
										std::fabs(angleDifferenceInDegrees)
									);
								break;
							case  enum_alignment::CANT:
								assert__error(
										enum_error::CANT_SEGMENT_TANGENT_DEVIATION,
										myMapExpressID[internalGetP21Line(segmentInstances[index - 1])],
										myMapExpressID[internalGetP21Line(segmentInstances[index])],
										std::fabs(angleDifferenceInDegrees)
									);
								break;
						}
					}
					else {
						switch (enumAlignment) {
							case  enum_alignment::HORIZONTAL:
								assert__error__(
										enum_error::HORIZONTAL_SEGMENT_TANGENT_DEVIATION,
										myMapExpressID[internalGetP21Line(segmentInstances[index - 1])],
										myMapExpressID[internalGetP21Line(segmentInstances[index])]
									);
								break;
							case  enum_alignment::VERTICAL:
								assert__error__(
										enum_error::VERTICAL_SEGMENT_TANGENT_DEVIATION,
										myMapExpressID[internalGetP21Line(segmentInstances[index - 1])],
										myMapExpressID[internalGetP21Line(segmentInstances[index])]
									);
								break;
							case  enum_alignment::CANT:
								assert__error__(
										enum_error::CANT_SEGMENT_TANGENT_DEVIATION,
										myMapExpressID[internalGetP21Line(segmentInstances[index - 1])],
										myMapExpressID[internalGetP21Line(segmentInstances[index])]
									);
								break;
						}
					}
				}
				previousEndPnt = endPnt;
			}
			else {
				assert(false);
//				assert(i + 1 == noSegmentInstances);
			}

			if (enumAlignment == enum_alignment::HORIZONTAL || enumAlignment == enum_alignment::VERTICAL) {
				assert(startPnt.point.z == 0. && endPnt.point.z == 0.);
			}
			else {
				assert(enumAlignment == enum_alignment::CANT);
			}
		}
	}
	else {
/////		assert(false);
	}
}

void	CheckGeometry(
				int_t	model,
				int_t	ifcAlignmentInstance,
				bool	isMirror,
				int_t	originalIfcAlignmentInstance = 0
			)
{
	{
		int_t	ifcAlignmentHorizontalInstance =
					___GetAlignmentHorizontal(
							model,
							ifcAlignmentInstance,
							nullptr
						);
		if (ifcAlignmentHorizontalInstance) {
			CheckGeometrySegments(model, ifcAlignmentHorizontalInstance, enum_alignment::HORIZONTAL);
		}
		else {
			assert__error(enum_error::ALIGNMENT_HORIZONTAL_MISSING, myMapExpressID[internalGetP21Line(ifcAlignmentInstance)]);
		}
	}

	{
		int_t	ifcAlignmentVerticalInstance =
					___GetAlignmentVertical(
							model,
							ifcAlignmentInstance,
							nullptr
						);
		if (ifcAlignmentVerticalInstance) {
			CheckGeometrySegments(model, ifcAlignmentVerticalInstance, enum_alignment::VERTICAL);
		}
	}

	{
		int_t	ifcAlignmentCantInstance =
					___GetAlignmentCant(
							model,
							ifcAlignmentInstance,
							nullptr
						);
		if (ifcAlignmentCantInstance) {
			CheckGeometrySegments(model, ifcAlignmentCantInstance, enum_alignment::CANT);
		}
	}
}

void	CheckMirrorGeometry(
				int_t	mirrorModel,
				int_t	mirrorIfcAlignmentInstance,
				int_t	originalIfcAlignmentInstance
			)
{
	CheckGeometry(
			mirrorModel,
			mirrorIfcAlignmentInstance,
			true,
			originalIfcAlignmentInstance
		);
}







int_t	CheckConsistencyAlignmentSegment__internal(
				int_t	model,
				int_t	ifcAlignmentInstance
			)
{
//	int_t	ifcCompositeCurveInstance = sdaiCreateInstanceBN(model, "IFCCOMPOSITECURVE"),
//            * aggrCurveSegment = sdaiCreateAggrBN(ifcCompositeCurveInstance, "Segments"),
//        	* aggrSegments = nullptr;

//    char    selfIntersect[2] = "F";
//    sdaiPutAttrBN(ifcCompositeCurveInstance, "SelfIntersect", sdaiENUM, (void*) selfIntersect);

    int_t   noSegmentInstances =
                ___GetAlignmentSegments(
                        model,
                        ifcAlignmentInstance,
                        nullptr
                    );

    int_t   issues = 0;
    if (noSegmentInstances) {
        int_t   * segmentInstances = new int_t[noSegmentInstances];

        ___GetAlignmentSegments(
                model,
                ifcAlignmentInstance,
                segmentInstances
            );

        for (int_t i = 0; i < noSegmentInstances; i++) {
            int_t   ifcAlignmentSegmentInstance = segmentInstances[i];
            assert(sdaiGetInstanceType(ifcAlignmentSegmentInstance) == sdaiGetEntity(model, (char*) "IFCALIGNMENTSEGMENT"));

            int_t   ifcProductRepresentationInstance = 0;
            sdaiGetAttrBN(ifcAlignmentSegmentInstance, (char*) "Representation", sdaiINSTANCE, (void*)&ifcProductRepresentationInstance);

            if (ifcProductRepresentationInstance) {
            }
            else {
//                issues++;
//                AddIssue(nullptr);
            }
        }
    }
    return  issues;
}

int_t	CheckConsistencyAlignment__internal(
				int_t	model,
				int_t	ifcAlignmentInstance
			)
{
    //
    //  Decomposition relation
    //


    int_t   ifcAlignmentHorizontalInstance,
            ifcAlignmentVerticalInstance,
            ifcAlignmentCantInstance;

    {
        bool    hasIssue = false;
        ifcAlignmentHorizontalInstance = ___GetAlignmentHorizontal(model, ifcAlignmentInstance, &hasIssue);
        if (hasIssue) {
            assert__error(
                    enum_error::HORIZONTAL_ALIGNMENT_NESTS,
                    ifcAlignmentInstance,
                    ifcAlignmentHorizontalInstance
                );
        }
    }

    {
        bool    hasIssue = false;
        ifcAlignmentVerticalInstance = ___GetAlignmentVertical(model, ifcAlignmentInstance, &hasIssue);
        if (hasIssue) {
            assert__error(
                    enum_error::VERTICAL_ALIGNMENT_NESTS,
                    ifcAlignmentInstance,
                    ifcAlignmentHorizontalInstance
                );
        }
    }

    {
        bool    hasIssue = false;
        ifcAlignmentCantInstance = ___GetAlignmentCant(model, ifcAlignmentInstance, &hasIssue);
        if (hasIssue) {
            assert__error(
                    enum_error::CANT_ALIGNMENT_NESTS,
                    ifcAlignmentInstance,
                    ifcAlignmentHorizontalInstance
                );
        }
    }

    int_t   issues = 0;
//    if (ifcAlignmentHorizontalInstance) {
        issues += CheckConsistencyAlignmentSegment__internal(model, ifcAlignmentHorizontalInstance);
//    }
    issues += CheckConsistencyAlignmentSegment__internal(model, ifcAlignmentVerticalInstance);
    issues += CheckConsistencyAlignmentSegment__internal(model, ifcAlignmentCantInstance);
    return  issues;
}

int_t	CheckConsistencyAlignment__internal(
				int_t	model,
                double  relativeEpsilon
			)
{
	int64_t	revision = GetRevision();

	if (revision < 1650) {
		assert__error(enum_error::LIBRARY_OUTDATED);
		return  0;
	}

	if (model == 0) {
		assert__error(enum_error::MODEL_ZERO);
		return  0;
	}

	char	* schemaName = nullptr;
	GetSPFFHeaderItem(model, 9, 0, sdaiSTRING, &schemaName);
	if (!equals(schemaName, (char*) "IFC4X3_ADD2") &&
		!equals(schemaName, (char*) "IFC4X3_ADD2")) {
		assert__error(enum_error::UNKNOWN_SCHEMA, schemaName);
	}

    int_t   * ifcAlignmentInstances = sdaiGetEntityExtentBN(model, (char*) "IFCALIGNMENT"),
            noIfcAlignmentInstances = sdaiGetMemberCount(ifcAlignmentInstances);
    if (noIfcAlignmentInstances) {
        int_t   issues = 0;
        for (int_t i = 0; i < noIfcAlignmentInstances; i++) {
            int_t   ifcAlignmentInstance = 0;
            sdaiGetAggrByIndex(ifcAlignmentInstances, i, sdaiINSTANCE, &ifcAlignmentInstance);

            int_t   myMirrorModel = sdaiCreateModelBN(0, nullptr, "IFC4X3");
            setFilter(myMirrorModel, 2, 1 + 2 + 4);

            if (myMirrorModel) {
                myMapExpressID.clear();

                int_t   mirrorIfcAlignmentInstance = CreateMirror(myMirrorModel, model, ifcAlignmentInstance);



                SetSPFFHeaderItem(myMirrorModel, 9, 0, sdaiSTRING, "IFC4x3");

                //sdaiSaveModelBN(myMirrorModel, (char*) "tmp001.ifc");
				int_t	size = 0;
				engiSaveModelByArray(myMirrorModel, nullptr, &size);
				unsigned char	* tmp001_ifc_content = new unsigned char[size + 1];
				engiSaveModelByArray(myMirrorModel, tmp001_ifc_content, &size);
				tmp001_ifc_content[size] = 0;

                sdaiCloseModel(myMirrorModel);

//                myMirrorModel = sdaiOpenModelBN(0, (char*) "tmp001.ifc", "");
                myMirrorModel = engiOpenModelByArray(0, tmp001_ifc_content, size, "");
                setFilter(myMirrorModel, 2, 1 + 2 + 4);

                mirrorIfcAlignmentInstance = 0;
                sdaiGetAggrByIndex(sdaiGetEntityExtentBN(myMirrorModel, "IFCALIGNMENT"), 0, sdaiINSTANCE, &mirrorIfcAlignmentInstance);

sdaiSaveModelBN(myMirrorModel, "mirrorWithoutGeometry.ifc");

{
	int64_t	expressID = internalGetP21Line(mirrorIfcAlignmentInstance);

	int_t	mySubModel = sdaiOpenModelBN(0, "mirrorWithoutGeometry.ifc", "");

	int_t	mySubInstance = internalGetInstanceFromP21Line(mySubModel, expressID);

	inferenceInstance(mySubInstance);

	sdaiSaveModelBN(mySubModel, "mirrorWithoutGeometry__inference.ifc");
	sdaiCloseModel(mySubModel);
}

                EnrichMirror(myMirrorModel, mirrorIfcAlignmentInstance);

                CheckMirrorGeometry(myMirrorModel, mirrorIfcAlignmentInstance, ifcAlignmentInstance);

//#ifdef _DEBUG
sdaiSaveModelBN(myMirrorModel, "mirrorWithGeometry.ifc");
//#endif // _DEBUG

				if (PARSE_GEOMETRY) {
					CompareMirror(myMirrorModel, mirrorIfcAlignmentInstance, model, ifcAlignmentInstance, relativeEpsilon);
				}

                sdaiCloseModel(myMirrorModel);
                myMapExpressID.clear();
            }
            else {
                assert(false);
            }

            issues += CheckConsistencyAlignment__internal(model, ifcAlignmentInstance);
        }

		{
			//...
			ValidationResults   results = validateModel(model);

			for (ValidationIssue issue = validateGetFirstIssue(results); issue; issue = validateGetNextIssue(issue)) {
				SdaiInstance    inst = validateGetInstance(issue);
				const char      * desc = validateGetDescription(issue);
				AddIssue(4, 0, (char*) desc, (int_t) inst);
			}

			validateFreeResults(results);
		}

		return  issues;
    }

	{
		//...
		ValidationResults   results = validateModel(model);

		for (ValidationIssue issue = validateGetFirstIssue(results); issue; issue = validateGetNextIssue(issue)) {
			SdaiInstance    inst = validateGetInstance(issue);
			const char      * desc = validateGetDescription(issue);
			AddIssue(4, 0, (char*) desc, (int_t) inst);
		}

		validateFreeResults(results);
	}

	return	0;
}
