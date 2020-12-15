# IFC4.3.RC.2 Production Schema
The repository to maintain IFC4.3.RC.2 production schema, including the EXPRESS schema and HTML documentation, subject for implementation and testing. The shared schema is based on the official IFC4.3.RC.2 published in bSI website: https://standards.buildingsmart.org/IFC/DEV/IFC4_3/RC2/HTML/. Updates are based on change requests and agreements made during discussion in Unit Test technical meetings. 

## Change log:

| Item          | Change        | Description  |
| ------------- |:-------------|:--------------------------------------|
| IfcCurveSegment      |  |  |
|- Placement      | MODIFIED | StartPlacement is changed name to Placement |
|- SegmentStart      | ADDED | New attribute SegmentStart is added to IfcCurveSegment |
| IfcAlignmentHorizontalSegmentTypeEnum      |  |  |
|- CUBICPARABOLA      | ADDED | CUBICPARABOLA is added as a predefined type of IfcAlignmentHorizontalSegment |
| IfcSpatialReferenceSelect      | MODIFIED | IfcSystem is replaced by IfcGroup to be selected |