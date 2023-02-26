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
								(char*) "IFC schema validation issues" :
								(char*) "Check if the model has issues against the IFC schema";
				case  0:
					return  isName ?
								(char*) "Mandatory attributes missing" :
								(char*) "Check if the model misses mandatory attributes according to the IFC schema";
				case  1:
					return  isName ?
								(char*) "REAL value boundary issues" :
								(char*) "Check if the model has real value attributes exceeds the boundary defined in the schema";
				case  2:
					return  isName ?
								(char*) "Incorrect ENUMERATION values" :
								(char*) "Check if the model has incorrect ENUMERATION values according to the IFC schema";
				case  3:
					return  isName ?
								(char*) "File schema definition" :
								(char*) "Check if the file schema is not defined correctly";
				default:
					break;
			}
			break;
		case  1:
			switch (subGroup) {
				case  -1:
					return  isName ?
								(char*) "Alignment strucutre issues" :
								(char*) "Check if alignment layouts are structured correctly";
				case  0:
					return  isName ?
								(char*) "Alignment horizontal issues" :
								(char*) "Check if a horizontal alignment has segments other than alignment horizontal segments nested";
				case  1:
					return  isName ?
								(char*) "Alignment vertical issues" :
								(char*) "Check if a vertical alignment has segments other than alignment vertical segments nested";
				case  2:
					return  isName ?
								(char*) "Alignment cant inconsistencies" :
								(char*) "Check if a cant alignment has segments other than alignment cant segments nested";
				default:
					break;
			}
			break;
		case  2:
			switch (subGroup) {
				case  -1:
					return  isName ?
								(char*) "Alignment continuity issues" :
								(char*) "Check if an alignment is continous based on the parameters of alignment segments";
				case  0:
					return  isName ?
								(char*) "Alignment horizontal continuity" :
								(char*) "Check if the deviation between an alignment horizontal segment and the subsequent segment exceeds the defined epsilon";
				case  1:
					return  isName ?
								(char*) "Alignment vertical continuity" :
								(char*) "Check if the deviation between an alignment vertical segment and the subsequent segment exceeds the defined epsilon";
				case  2:
					return  isName ?
								(char*) "Alignment cant continuity" :
								(char*) "Check if the deviation between an alignment cant segment and the subsequent segment exceeds the defined epsilon";
				case  3:
					return  isName ?
								(char*) "Alignment horizontal tangential continuity" :
								(char*) "Check if an alignment horizontal segment is not tangentially continous (exceeds defined epsilon) towards the subsequent segment";
				case  4:
					return  isName ?
								(char*) "Alignment vertical tangential continuity" :
								(char*) "Check if an alignment vertical segment is not tangentially continous (exceeds defined epsilon) towards the subsequent segment";
				case  5:
					return  isName ?
								(char*) "Alignment cant tangential continuity" :
								(char*) "Check if an alignment cant segment is not tangentially continous (exceeds defined epsilon) towards the subsequent segment";
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
