#pragma once


static	inline	const char	* GetGroupText(
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
	//		"REAL value derived issues"
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
	//	"Schema validation through IFC Engine"
	//		"Schema validation through IFC Engine"
	//

	switch (group) {
		case  0:
			switch (subGroup) {
				case  -1:
					return  isName ?
								"IFC schema validation issues" :
								"Check if the model has issues against the IFC schema";
				case  0:
					return  isName ?
								"Mandatory attributes missing" :
								"Check if the model misses mandatory attributes according to the IFC schema";
				case  1:
					return  isName ?
								"REAL value derived issues" :
								"Check if the model has real value that is calculated incorrectly";
				case  2:
					return  isName ?
								"REAL value boundary issues" :
								"Check if the model has real value attributes exceeds the boundary defined in the schema";
				case  3:
					return  isName ?
								"Incorrect ENUMERATION values" :
								"Check if the model has incorrect ENUMERATION values according to the IFC schema";
				case  4:
					return  isName ?
								"File schema definition" :
								"Check if the file schema is not defined correctly";
				default:
					break;
			}
			break;
		case  1:
			switch (subGroup) {
				case  -1:
					return  isName ?
								"Alignment strucutre issues" :
								"Check if alignment layouts are structured correctly";
				case  0:
					return  isName ?
								"Alignment horizontal issues" :
								"Check if a horizontal alignment has segments other than alignment horizontal segments nested";
				case  1:
					return  isName ?
								"Alignment vertical issues" :
								"Check if a vertical alignment has segments other than alignment vertical segments nested";
				case  2:
					return  isName ?
								"Alignment cant inconsistencies" :
								"Check if a cant alignment has segments other than alignment cant segments nested";
				default:
					break;
			}
			break;
		case  2:
			switch (subGroup) {
				case  -1:
					return  isName ?
								"Alignment continuity issues" :
								"Check if an alignment is continous based on the parameters of alignment segments";
				case  0:
					return  isName ?
								"Alignment horizontal continuity" :
								"Check if the deviation between an alignment horizontal segment and the subsequent segment exceeds the defined epsilon";
				case  1:
					return  isName ?
								"Alignment vertical continuity" :
								"Check if the deviation between an alignment vertical segment and the subsequent segment exceeds the defined epsilon";
				case  2:
					return  isName ?
								"Alignment cant continuity" :
								"Check if the deviation between an alignment cant segment and the subsequent segment exceeds the defined epsilon";
				case  3:
					return  isName ?
								"Alignment horizontal tangential continuity" :
								"Check if an alignment horizontal segment is not tangentially continous (exceeds defined epsilon) towards the subsequent segment";
				case  4:
					return  isName ?
								"Alignment vertical tangential continuity" :
								"Check if an alignment vertical segment is not tangentially continous (exceeds defined epsilon) towards the subsequent segment";
				case  5:
					return  isName ?
								"Alignment cant tangential continuity" :
								"Check if an alignment cant segment is not tangentially continous (exceeds defined epsilon) towards the subsequent segment";
				default:
					break;
			}
			break;
		case  3:
			if (has_PARSE_GEOMETRY) {
				switch (subGroup) {
					case  -1:
						return  isName ?
									"Geometrical Content Issue" :
									"Description of Geometrical Content Issue";
					case  0:
						return  isName ?
									"Alignment Issues" :
									"Description of Alignment Issues";
					case  1:
						return  isName ?
									"Horizontal Alignment Issues" :
									"Description of Horizontal Alignment Issues";
					case  2:
						return  isName ?
									"Vertical Alignment Issues" :
									"Description of Vertical Alignment Issues";
					case  3:
						return  isName ?
									"Cant Alignment Issues" :
									"Description of Cant Alignment Issues";
					case  4:
						return  isName ?
									"Curve Segment Issues" :
									"Description of Curve Segment Issues";
					default:
						break;
				}
			}
			break;
		case  4:
			switch (subGroup) {
				case  -1:
					return  isName ?
								"Schema validation issues" :
								"Schema validation through IFC Engine";
				case  0:
					return  isName ?
								"Schema validation issues" :
								"Schema validation through IFC Engine";
			}
			break;
		default:
			break;
	}

	return	nullptr;
}
