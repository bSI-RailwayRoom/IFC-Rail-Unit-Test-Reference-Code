#pragma once


static	inline	char	* GetGroupText(
								int		group,
								int		subGroup,
								bool	isName,
								bool	has_PARSE_GEOMETRY
							)
{
	//
	//	"IFC Schema Validation Issues (inconsistency against the latest IFC4x3 schema)"
	//		"Mandatory Attribute Missing"
	//		"REAL value boundary issues"
	//		"Incorrect ENUMERATION values"
	// 	    "File schema definition"
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
	// 
	//	"Geometrical Content Issue"
	//		"Alignment Issues"
	//		"Horizontal Alignment Issues"
	//		"Vertical Alignment Issues"
	//		"Cant Alignment Issues"
	//		"Curve Segment Issues"
	//

	switch (group) {
		case  0:
			switch (subGroup) {
				case  -1:
					return  isName ?
								(char*) "IFC Schema Validation Issues" :
								(char*) "inconsistency against the latest IFC4x3 schema";
				case  0:
					return  isName ?
								(char*) "Mandatory Attribute Missing" :
								(char*) "Description of Mandatory Attribute Missing";
				case  1:
					return  isName ?
								(char*) "REAL value boundary issues" :
								(char*) "Description of REAL value boundary issues";
				case  2:
					return  isName ?
								(char*) "Incorrect ENUMERATION values" :
								(char*) "Description of Incorrect ENUMERATION values";
				case  3:
					return  isName ?
								(char*) "File schema definition" :
								(char*) "Description of File schema definition";
				default:
					break;
			}
			break;
		case  1:
			switch (subGroup) {
				case  -1:
					return  isName ?
								(char*) "Relation Inconsistencies" :
								(char*) "valid against the schema, however inconsistent according to agreements";
				case  0:
					return  isName ?
								(char*) "Horizontal Alignment inconsistencies" :
								(char*) "Description of Horizontal Alignment inconsistencies";
				case  1:
					return  isName ?
								(char*) "Vertical Alignment inconsistencies" :
								(char*) "Description of Vertical Alignment inconsistencies";
				case  2:
					return  isName ?
								(char*) "Cant Alignment inconsistencies" :
								(char*) "Description of Cant Alignment inconsistencies";
				default:
					break;
			}
			break;
		case  2:
			switch (subGroup) {
				case  -1:
					return  isName ?
								(char*) "Geometrical Distances" :
								(char*) "based on Business Logic Knowledge";
				case  0:
					return  isName ?
								(char*) "Horizontal Alignment distances / deviations" :
								(char*) "Description of Horizontal Alignment distances / deviations";
				case  1:
					return  isName ?
								(char*) "Vertical Alignment distances / deviations" :
								(char*) "Description of Vertical Alignment distances / deviations";
				case  2:
					return  isName ?
								(char*) "Cant Alignment distances / deviations" :
								(char*) "Description of Cant Alignment distances / deviations";
				case  3:
					return  isName ?
								(char*) "Horizontal Alignment Angle differences" :
								(char*) "Description of Horizontal Alignment Angle differences";
				case  4:
					return  isName ?
								(char*) "Vertical Alignment Angle differences" :
								(char*) "Description of Vertical Alignment Angle differences";
				default:
					break;
			}
			break;
		case  3:
			if (has_PARSE_GEOMETRY) {
				switch (subGroup) {
					case  -1:
						return  isName ?
									(char*) "Geometrical Content Issue" :
									(char*) "Description of Geometrical Content Issue";
					case  0:
						return  isName ?
									(char*) "Alignment Issues" :
									(char*) "Description of Alignment Issues";
					case  1:
						return  isName ?
									(char*) "Horizontal Alignment Issues" :
									(char*) "Description of Horizontal Alignment Issues";
					case  2:
						return  isName ?
									(char*) "Vertical Alignment Issues" :
									(char*) "Description of Vertical Alignment Issues";
					case  3:
						return  isName ?
									(char*) "Cant Alignment Issues" :
									(char*) "Description of Cant Alignment Issues";
					case  4:
						return  isName ?
									(char*) "Curve Segment Issues" :
									(char*) "Description of Curve Segment Issues";
					default:
						break;
				}
			}
			break;
		default:
			break;
	}

	return	nullptr;
}
