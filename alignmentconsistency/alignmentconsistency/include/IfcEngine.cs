using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Linq;
#if _WIN64
		using int_t = System.Int64;
#else
		using int_t = System.Int32;
#endif

namespace RDF
{
	///	<summary>
	///	ENGINE FORMAT SPECIFICATION
	///	</summary>
	public class FORMAT
	{
		//	control precision / data size
		///	<summary></summary>
		public const Int64 SIZE_VERTEX_DOUBLE = (1<<2);      //	Vertex items returned as double (8 byte/64 bit) else - as float (4 byte/32 bit)
		///	<summary></summary>
		public const Int64 SIZE_INDEX_INT64 = (1<<3);        //	Index items returned as int64_t (8 byte/64 bit) (only available in 64 bit mode) - else as int32_t (4 byte/32 bit)

		//	control vertex data
		///	<summary>Vertex contains 3D point info</summary>
		public const Int64 VERTEX_POINT = (1<<4); 
		///	<summary>Vertex contains normal info</summary>
		public const Int64 VERTEX_NORMAL = (1<<5); 
		///	<summary>Vertex contains first 2D texture info</summary>
		public const Int64 VERTEX_TEXTURE_UV = (1<<6); 
		///	<summary>Vertex does contain tangent vector for first texture</summary>
		public const Int64 VERTEX_TEXTURE_TANGENT = (1<<28); 
		///	<summary>Vertex does contain binormal vector for first texture</summary>
		public const Int64 VERTEX_TEXTURE_BINORMAL = (1<<29); 
		///	<summary>Vertex contains second 2D texture info</summary>
		public const Int64 VERTEX_TEXTURE2_UV = (1<<7); 
		///	<summary>Vertex does contain tangent vector for second texture (only 64 bit platform)</summary>
		public const Int64 VERTEX_TEXTURE2_TANGENT = (1<<30); 
		///	<summary>Vertex does contain binormal vector for second texture (only 64 bit platform)</summary>
		public const Int64 VERTEX_TEXTURE2_BINORMAL = (1<<31); 
		///	<summary>Vertex does contain Ambient color information</summary>
		public const Int64 VERTEX_COLOR_AMBIENT = (1<<24); 
		///	<summary>Vertex does contain Diffuse color information</summary>
		public const Int64 VERTEX_COLOR_DIFFUSE = (1<<25); 
		///	<summary>Vertex does contain Emissive color information</summary>
		public const Int64 VERTEX_COLOR_EMISSIVE = (1<<26); 
		///	<summary>Vertex does contain Specular color information</summary>
		public const Int64 VERTEX_COLOR_SPECULAR = (1<<27); 
		//	control CalculateInstance behaviour
		///	<summary>Object form triangles are exported (effective if instance contains faces and/or solids)(triangulated surface representation)</summary>
		public const Int64 EXPORT_TRIANGLES = (1<<8);
		///	<summary>Object polygon lines are exported (effective if instance contains line representations)</summary>
		public const Int64 EXPORT_LINES = (1<<9);
		///	<summary>Object points are exported (effective if instance contains point representations)</summary>
		public const Int64 EXPORT_POINTS = (1<<10); 
		///	<summary>Object face polygon lines (dense wireframe) are exported (requires FORMAT_FLAG_CONTAINS_TRIANGLES)</summary>
		public const Int64 EXPORT_FACE_POLYGONS = (1<<12); 
		///	<summary>Object conceptual face polygon lines (wireframe) are exported </summary>
		public const Int64 EXPORT_CONCEPTUAL_FACE_POLYGONS = (1<<13); 
		///	<summary>Polygon lines (wireframe) exported as tuples (edges) - else as list (loop)</summary>
		public const Int64 EXPORT_POLYGONS_AS_TUPLES = (1<<14); 
		///	<summary>Normals are exported to be in line with the original semantic form description (orthogonal to conceprual face, could be non orthogonal to the planar face or triangle) - else all normals of triangles are transformed orthogonal to the palnar face or triangle they belong to</summary>
		public const Int64 EXPORT_ADVANCED_NORMALS = (1<<15); 
		///	<summary>Where possible DirectX compatibility is given to exported data. Unsets FORMAT_FLAG_OPENGL, FORMAT_FLAG_VERSION_0001, FORMAT_FLAG_VERSION_0010</summary>
		public const Int64 EXPORT_DIRECTX = (1<<16); 
		///	<summary>Where possible OpenGL compatibility is given to exported data. Unsets FORMAT_FLAG_DIRECTX. Sets FORMAT_FLAG_VERSION_0001, FORMAT_FLAG_VERSION_0010</summary>
		public const Int64 EXPORT_OPENGL = (1<<17); 
		///	<summary>Every face has exactly one opposite face (normally both index and vertex array are doubled in size)</summary>
		public const Int64 EXPORT_DOUBLE_SIDED = (1<<18); 
		///	<summary>Opposite Triangle Rotation (RHS as expected by OpenGL) - else  Standard Triangle Rotation (LHS as expected by DirectX)</summary>
		public const Int64 EXPORT_VERSION_0001 = (1<<20); 
		///	<summary>X, Y, Z (nX, nY, nZ) formatted as , i.e. X, -Z, Y (nX, -nZ, nY) considering internal concepts (OpenGL) - else X, Y, Z (nX, nY, nZ) formatted as considering internal concepts</summary>
		public const Int64 EXPORT_VERSION_0010 = (1<<21); 
		public const Int64 EXPORT_VERSION_0100 = (1<<22); 
		public const Int64 EXPORT_VERSION_1000 = (1<<23); 
	}

	///	<summary>
	///	Color pack/unpack conversion methods
	///	</summary>
	public class COLOR
	{
		///	<summary> get color from its components in range 0..255 </summary>
		public static UInt32 RGBW255(byte r, byte g, byte b, byte w) { return (UInt32)r << 24 | (UInt32)g << 16 | (UInt32)b << 8 | (UInt32)w; }

		///	<summary>get color from its components in range 0..1</summary>
		public static UInt32 RGBW(double r, double g, double b, double w) { return RGBW255((byte)(r * 255), (byte)(g * 255), (byte)(b * 255), (byte)(w * 255)); }

		///	<summary>get color red component in range 0..255</summary>
		public static byte GET_R255(UInt32 clr) { return (byte)((clr >> 24) & 0xFF); }

		///	<summary>get color green component in range 0..255</summary>
		public static byte GET_G255(UInt32 clr) { return (byte)((clr >> 16) & 0xFF); }

		///	<summary>get color blue component in range 0..255</summary>
		public static byte GET_B255(UInt32 clr) { return (byte)((clr >> 8) & 0xFF); }

		///	<summary>get color transparency in range 0..255</summary>
		public static byte GET_W255(UInt32 clr) { return (byte)(clr & 0xFF); }

		///	<summary>get color red component in range 0..1</summary>
		public static double GET_R(UInt32 clr) { return GET_R255(clr) / 255.0; }

		///	<summary>get color green component in range 0..1</summary>
		public static double GET_G(UInt32 clr) { return GET_G255(clr) / 255.0; }

		///	<summary>get color blue component in range 0..1</summary>
		public static double GET_B(UInt32 clr) { return GET_B255(clr) / 255.0; }

		///	<summary>get color trancparency in range 0..1</summary>
		public static double GET_W(UInt32 clr) { return GET_W255(clr) / 255.0; }

		///	<summary>get color from array of 4 components in range 0..255</summary>
		public static UInt32 RGBW255(byte[] r) { return RGBW255(r[0], r[1], r[2], r[3]); }

		///	<summary>get color from array of 4 components in range 0..1</summary>
		public static UInt32 RGBW(double[] r) { return RGBW(r[0], r[1], r[2], r[3]); }

		///	<summary>get color comonents in range 0..255 to arry of 4 elements</summary>
		public static byte[] GET_COMPONENTS255(UInt32 clr)
		{
			var	r = new byte[4]; 
			r[0] = GET_R255(clr); 
			r[1] = GET_G255(clr); 
			r[2] = GET_B255(clr); 
			r[3] = GET_W255(clr); 
			return	r; 
		}

		///	<summary>get color comonents in range 0..1 to arry of 4 elements</summary>
		public static double[] GET_COMPONENTS(UInt32 clr)
		{
			var	r = new double[4]; 
			r[0] = GET_R(clr); 
			r[1] = GET_G(clr); 
			r[2] = GET_B(clr); 
			r[3] = GET_W(clr); 
			return	r;
		}
	}//COLOR

	public enum enum_express_declaration : byte
	{
		__UNDEF = 0,
		__ENTITY,
		__ENUM,
		__SELECT,
		__DEFINED_TYPE
	};

	public enum enum_express_attr_type : byte
	{
		__NONE = 0, //attribute type is defined by reference domain entity
		__BINARY,
		__BINARY_32,
		__BOOLEAN,
		__ENUMERATION,
		__INTEGER,
		__LOGICAL,
		__NUMBER,
		__REAL,
		__SELECT,
		__STRING
	};

	public enum enum_express_aggr : byte
	{
		__NONE = 0,
		__ARRAY,
		__BAG,
		__LIST,
		__SET
	};

	public enum enum_validation_type : System.UInt64
	{
		__NONE						= 0,
		__NO_OF_ARGUMENTS			= 1 << 0,	//number of arguments
		__ARGUMENT_EXPRESS_TYPE		= 1 << 1,	//argument value is correct entity, defined type or enumeration value
		__ARGUMENT_PRIM_TYPE		= 1 << 2,	//argument value has correct primitive type
		__REQUIRED_ARGUMENTS		= 1 << 3,	//non-optional arguments values are provided
		__ARRGEGATION_EXPECTED		= 1 << 4,	//aggregation is provided when expected
		__AGGREGATION_NOT_EXPECTED	= 1 << 5,   //aggregation is not used when not expected
		__AGGREGATION_SIZE			= 1 << 6,   //aggregation size
		__AGGREGATION_UNIQUE		= 1 << 7,	//elements in aggregations are unique when required
		__COMPLEX_INSTANCE			= 1 << 8,	//complex instances contains full parent chains
		__REFERENCE_EXISTS			= 1 << 9,	//referenced instance exists
		__ABSTRACT_ENTITY			= 1 << 10,  //abstract entity should not instantiate
		__WHERE_RULE				= 1 << 11,  //where-rule check
		__UNIQUE_RULE				= 1 << 12,	//unique-rule check
		__STAR_USAGE				= 1 << 13,  //* is used only for derived arguments
		__CALL_ARGUMENT				= 1 << 14,  //validateModel/validateInstance function argument should be model/instance
		__INTERNAL_ERROR			= 1 << 15   //unspecified error
	};

	public enum enum_validation_status : byte
	{
		__NONE = 0,
		__COMPLETE_ALL,		//all issues proceed
		__COMPLETE_NOT_ALL, //completed but some issues were excluded by option settings
		__TIME_EXCEED,		//validation was finished because of reach time limit
		__COUNT_EXCEED	    //validation was finished because of reach of issue's numbers limit
	};

	class ifcengine
	{
		public const int sdaiTYPE = 0; //C++ API generator specific

		public const int_t flagbit0 = 1;           // 2^^0    0000.0000..0000.0001
		public const int_t flagbit1 = 2;           // 2^^1    0000.0000..0000.0010
		public const int_t flagbit2 = 4;           // 2^^2    0000.0000..0000.0100
		public const int_t flagbit3 = 8;           // 2^^3    0000.0000..0000.1000
		public const int_t flagbit4 = 16;          // 2^^4    0000.0000..0001.0000
		public const int_t flagbit5 = 32;          // 2^^5    0000.0000..0010.0000
		public const int_t flagbit6 = 64;          // 2^^6    0000.0000..0100.0000
		public const int_t flagbit7 = 128;         // 2^^7    0000.0000..1000.0000
		public const int_t flagbit8 = 256;         // 2^^8    0000.0001..0000.0000
		public const int_t flagbit9 = 512;         // 2^^9    0000.0010..0000.0000
		public const int_t flagbit10 = 1024;       // 2^^10   0000.0100..0000.0000
		public const int_t flagbit11 = 2048;       // 2^^11   0000.1000..0000.0000
		public const int_t flagbit12 = 4096;       // 2^^12   0001.0000..0000.0000
		public const int_t flagbit13 = 8192;       // 2^^13   0010.0000..0000.0000
		public const int_t flagbit14 = 16384;      // 2^^14   0100.0000..0000.0000
		public const int_t flagbit15 = 32768;      // 2^^15   1000.0000..0000.0000

		public const int_t sdaiADB           = 1;
		public const int_t sdaiAGGR          = sdaiADB + 1;
		public const int_t sdaiBINARY        = sdaiAGGR + 1;
		public const int_t sdaiBOOLEAN       = sdaiBINARY + 1;
		public const int_t sdaiENUM          = sdaiBOOLEAN + 1;
		public const int_t sdaiINSTANCE      = sdaiENUM + 1;
		public const int_t sdaiINTEGER       = sdaiINSTANCE + 1;
		public const int_t sdaiLOGICAL       = sdaiINTEGER + 1;
		public const int_t sdaiREAL          = sdaiLOGICAL + 1;
		public const int_t sdaiSTRING        = sdaiREAL + 1;
		public const int_t sdaiUNICODE       = sdaiSTRING + 1;
		public const int_t sdaiEXPRESSSTRING = sdaiUNICODE + 1;
		public const int_t engiGLOBALID      = sdaiEXPRESSSTRING + 1;

		public const string IFCEngineDLL = @"IFCEngine.dll";

        //
        //  File IO API Calls
        //

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate Int64 ReadCallBackFunction(IntPtr value);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void WriteCallBackFunction(IntPtr value, Int64 size);

		/// <summary>
		///		sdaiCreateModelBN                                       (http://rdf.bg/ifcdoc/CS64/sdaiCreateModelBN.html)
		///
		///	This function creates and empty model (we expect with a schema file given).
		///	Attributes repository and fileName will be ignored, they are their because of backward compatibility.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBN")]
		public static extern int_t sdaiCreateModelBN(int_t repository, string fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBN")]
		public static extern int_t sdaiCreateModelBN(int_t repository, string fileName, byte[] schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBN")]
		public static extern int_t sdaiCreateModelBN(int_t repository, byte[] fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBN")]
		public static extern int_t sdaiCreateModelBN(int_t repository, byte[] fileName, byte[] schemaName);

		/// <summary>
		///		sdaiCreateModelBNUnicode                                (http://rdf.bg/ifcdoc/CS64/sdaiCreateModelBNUnicode.html)
		///
		///	This function creates and empty model (we expect with a schema file given).
		///	Attributes repository and fileName will be ignored, they are their because of backward compatibility.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBNUnicode")]
		public static extern int_t sdaiCreateModelBNUnicode(int_t repository, string fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBNUnicode")]
		public static extern int_t sdaiCreateModelBNUnicode(int_t repository, string fileName, byte[] schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBNUnicode")]
		public static extern int_t sdaiCreateModelBNUnicode(int_t repository, byte[] fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateModelBNUnicode")]
		public static extern int_t sdaiCreateModelBNUnicode(int_t repository, byte[] fileName, byte[] schemaName);

		/// <summary>
		///		sdaiOpenModelBN                                         (http://rdf.bg/ifcdoc/CS64/sdaiOpenModelBN.html)
		///
		///	This function opens the model on location fileName.
		///	Attribute repository will be ignored, they are their because of backward compatibility.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBN")]
		public static extern int_t sdaiOpenModelBN(int_t repository, string fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBN")]
		public static extern int_t sdaiOpenModelBN(int_t repository, string fileName, byte[] schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBN")]
		public static extern int_t sdaiOpenModelBN(int_t repository, byte[] fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBN")]
		public static extern int_t sdaiOpenModelBN(int_t repository, byte[] fileName, byte[] schemaName);

		/// <summary>
		///		sdaiOpenModelBNUnicode                                  (http://rdf.bg/ifcdoc/CS64/sdaiOpenModelBNUnicode.html)
		///
		///	This function opens the model on location fileName.
		///	Attribute repository will be ignored, they are their because of backward compatibility.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBNUnicode")]
		public static extern int_t sdaiOpenModelBNUnicode(int_t repository, string fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBNUnicode")]
		public static extern int_t sdaiOpenModelBNUnicode(int_t repository, string fileName, byte[] schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBNUnicode")]
		public static extern int_t sdaiOpenModelBNUnicode(int_t repository, byte[] fileName, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiOpenModelBNUnicode")]
		public static extern int_t sdaiOpenModelBNUnicode(int_t repository, byte[] fileName, byte[] schemaName);

		/// <summary>
		///		engiOpenModelByStream                                   (http://rdf.bg/ifcdoc/CS64/engiOpenModelByStream.html)
		///
		///	This function opens the model via a stream.
		///	Attribute repository will be ignored, they are their because of backward compatibility.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiOpenModelByStream")]
		public static extern int_t engiOpenModelByStream(int_t repository, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiOpenModelByStream")]
		public static extern int_t engiOpenModelByStream(int_t repository, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, byte[] schemaName);

		/// <summary>
		///		engiOpenModelByArray                                    (http://rdf.bg/ifcdoc/CS64/engiOpenModelByArray.html)
		///
		///	This function opens the model via an array.
		///	Attribute repository will be ignored, they are their because of backward compatibility.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiOpenModelByArray")]
		public static extern int_t engiOpenModelByArray(int_t repository, byte[] content, int_t size, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiOpenModelByArray")]
		public static extern int_t engiOpenModelByArray(int_t repository, byte[] content, int_t size, byte[] schemaName);

		/// <summary>
		///		sdaiSaveModelBN                                         (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelBN.html)
		///
		///	This function saves the model (char file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelBN")]
		public static extern void sdaiSaveModelBN(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelBN")]
		public static extern void sdaiSaveModelBN(int_t model, byte[] fileName);

		/// <summary>
		///		sdaiSaveModelBNUnicode                                  (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelBNUnicode.html)
		///
		///	This function saves the model (wchar, i.e. Unicode file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelBNUnicode")]
		public static extern void sdaiSaveModelBNUnicode(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelBNUnicode")]
		public static extern void sdaiSaveModelBNUnicode(int_t model, byte[] fileName);

		/// <summary>
		///		engiSaveModelByStream                                   (http://rdf.bg/ifcdoc/CS64/engiSaveModelByStream.html)
		///
		///	This function saves the model as a stream.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiSaveModelByStream")]
		public static extern void engiSaveModelByStream(int_t model, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, int_t size);

		/// <summary>
		///		engiSaveModelByArray                                    (http://rdf.bg/ifcdoc/CS64/engiSaveModelByArray.html)
		///
		///	This function saves the model as an array.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiSaveModelByArray")]
		public static extern void engiSaveModelByArray(int_t model, byte[] content, out int_t size);

		/// <summary>
		///		sdaiSaveModelAsXmlBN                                    (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelAsXmlBN.html)
		///
		///	This function saves the model as XML according to IFC2x3's way of XML serialization (char file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsXmlBN")]
		public static extern void sdaiSaveModelAsXmlBN(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsXmlBN")]
		public static extern void sdaiSaveModelAsXmlBN(int_t model, byte[] fileName);

		/// <summary>
		///		sdaiSaveModelAsXmlBNUnicode                             (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelAsXmlBNUnicode.html)
		///
		///	This function saves the model as XML according to IFC2x3's way of XML serialization (wchar, i.e. Unicode file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsXmlBNUnicode")]
		public static extern void sdaiSaveModelAsXmlBNUnicode(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsXmlBNUnicode")]
		public static extern void sdaiSaveModelAsXmlBNUnicode(int_t model, byte[] fileName);

		/// <summary>
		///		sdaiSaveModelAsSimpleXmlBN                              (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelAsSimpleXmlBN.html)
		///
		///	This function saves the model as XML according to IFC4's way of XML serialization (char file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsSimpleXmlBN")]
		public static extern void sdaiSaveModelAsSimpleXmlBN(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsSimpleXmlBN")]
		public static extern void sdaiSaveModelAsSimpleXmlBN(int_t model, byte[] fileName);

		/// <summary>
		///		sdaiSaveModelAsSimpleXmlBNUnicode                       (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelAsSimpleXmlBNUnicode.html)
		///
		///	This function saves the model as XML according to IFC4's way of XML serialization (wchar, i.e. Unicode file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsSimpleXmlBNUnicode")]
		public static extern void sdaiSaveModelAsSimpleXmlBNUnicode(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsSimpleXmlBNUnicode")]
		public static extern void sdaiSaveModelAsSimpleXmlBNUnicode(int_t model, byte[] fileName);

		/// <summary>
		///		sdaiSaveModelAsJsonBN                                   (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelAsJsonBN.html)
		///
		///	This function saves the model as JSON according to IFC4's way of JSON serialization (char file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsJsonBN")]
		public static extern void sdaiSaveModelAsJsonBN(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsJsonBN")]
		public static extern void sdaiSaveModelAsJsonBN(int_t model, byte[] fileName);

		/// <summary>
		///		sdaiSaveModelAsJsonBNUnicode                            (http://rdf.bg/ifcdoc/CS64/sdaiSaveModelAsJsonBNUnicode.html)
		///
		///	This function saves the model as JSON according to IFC4's way of JSON serialization (wchar, i.e. Unicode file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsJsonBNUnicode")]
		public static extern void sdaiSaveModelAsJsonBNUnicode(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiSaveModelAsJsonBNUnicode")]
		public static extern void sdaiSaveModelAsJsonBNUnicode(int_t model, byte[] fileName);

		/// <summary>
		///		engiSaveSchemaBN                                        (http://rdf.bg/ifcdoc/CS64/engiSaveSchemaBN.html)
		///
		///	This function saves the schema.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiSaveSchemaBN")]
		public static extern byte engiSaveSchemaBN(int_t model, string filePath);

		[DllImport(IFCEngineDLL, EntryPoint = "engiSaveSchemaBN")]
		public static extern byte engiSaveSchemaBN(int_t model, byte[] filePath);

		/// <summary>
		///		engiSaveSchemaBNUnicode                                 (http://rdf.bg/ifcdoc/CS64/engiSaveSchemaBNUnicode.html)
		///
		///	This function saves the schema (wchar, i.e. Unicode file name).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiSaveSchemaBNUnicode")]
		public static extern byte engiSaveSchemaBNUnicode(int_t model, string filePath);

		[DllImport(IFCEngineDLL, EntryPoint = "engiSaveSchemaBNUnicode")]
		public static extern byte engiSaveSchemaBNUnicode(int_t model, byte[] filePath);

		/// <summary>
		///		sdaiCloseModel                                          (http://rdf.bg/ifcdoc/CS64/sdaiCloseModel.html)
		///
		///	This function closes the model. After this call no instance handles will be available including all
		///	handles referencing the geometry of this specific file, in default compilation the model itself will
		///	be known in the kernel, however known to be disabled. Calls containing the model reference will be
		///	protected from crashing when called.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCloseModel")]
		public static extern void sdaiCloseModel(int_t model);

		/// <summary>
		///		setPrecisionDoubleExport                                (http://rdf.bg/ifcdoc/CS64/setPrecisionDoubleExport.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setPrecisionDoubleExport")]
		public static extern void setPrecisionDoubleExport(int_t model, int_t precisionCap, int_t precisionRound, byte clean);

        //
        //  Schema Reading API Calls
        //

		/// <summary>
		///		engiGetNextDeclarationIterator                          (http://rdf.bg/ifcdoc/CS64/engiGetNextDeclarationIterator.html)
		///
		///	This call returns next iterator of EXPRESS schema declarations.
		///	If the input iterator is NULL it returns first iterator.
		///	If the input iterator is last it returns NULL.
		///	Use engiGetDeclarationFromIterator to access EXPRESS declaration data. 
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetNextDeclarationIterator")]
		public static extern int_t engiGetNextDeclarationIterator(int_t model, int_t iterator);

		/// <summary>
		///		engiGetDeclarationFromIterator                          (http://rdf.bg/ifcdoc/CS64/engiGetDeclarationFromIterator.html)
		///
		///	This call returns handle to the EXPRESS schema declarations from iterator.
		///	It may be a handle to entity, or enumeration, select or type definition, use engiGetDeclarationType to recognize
		///	Use engiGetNextDeclarationIterator to get iterator.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetDeclarationFromIterator")]
		public static extern int_t engiGetDeclarationFromIterator(int_t model, int_t iterator);

		/// <summary>
		///		engiGetDeclarationType                                  (http://rdf.bg/ifcdoc/CS64/engiGetDeclarationType.html)
		///
		///	This call returns a type of the EXPRESS schema declarations from its handle.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetDeclarationType")]
		public static extern enum_express_declaration engiGetDeclarationType(int_t declaration);

		/// <summary>
		///		engiGetEnumerationElement                               (http://rdf.bg/ifcdoc/CS64/engiGetEnumerationElement.html)
		///
		///	This call returns a name of the enumaration element with the given index (zero based)
		///	It returns NULL if the index out of range
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEnumerationElement")]
		public static extern IntPtr engiGetEnumerationElement(int_t enumeration, int_t index);

		/// <summary>
		///		engiGetSelectElement                                    (http://rdf.bg/ifcdoc/CS64/engiGetSelectElement.html)
		///
		///	This call returns a declaration handle of the select element with the given index (zero based)
		///	It returns 0 if the index out of range
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetSelectElement")]
		public static extern int_t engiGetSelectElement(int_t select, int_t index);

		/// <summary>
		///		engiGetDefinedType                                      (http://rdf.bg/ifcdoc/CS64/engiGetDefinedType.html)
		///
		///	This call returns a simple type for defined type handle and can inquire referenced type, if any
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetDefinedType")]
		public static extern enum_express_attr_type engiGetDefinedType(int_t definedType, out int_t referencedDeclaration, out int_t aggregationDescriptor);

		/// <summary>
		///		sdaiGetEntity                                           (http://rdf.bg/ifcdoc/CS64/sdaiGetEntity.html)
		///
		///	This call retrieves a handle to an entity based on a given entity name.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetEntity")]
		public static extern int_t sdaiGetEntity(int_t model, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetEntity")]
		public static extern int_t sdaiGetEntity(int_t model, byte[] entityName);

		/// <summary>
		///		engiGetEntityModel                                      (http://rdf.bg/ifcdoc/CS64/engiGetEntityModel.html)
		///
		///	This call retrieves a model based on a given entity handle.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityModel")]
		public static extern int_t engiGetEntityModel(int_t entity);

		/// <summary>
		///		engiGetEntityArgument                                   (http://rdf.bg/ifcdoc/CS64/engiGetEntityArgument.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityArgument")]
		public static extern int_t engiGetEntityArgument(int_t entity, string argumentName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityArgument")]
		public static extern int_t engiGetEntityArgument(int_t entity, byte[] argumentName);

		/// <summary>
		///		engiGetEntityAttributeIndex                             (http://rdf.bg/ifcdoc/CS64/engiGetEntityAttributeIndex.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityAttributeIndex")]
		public static extern int_t engiGetEntityAttributeIndex(int_t entity, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityAttributeIndex")]
		public static extern int_t engiGetEntityAttributeIndex(int_t entity, byte[] attributeName);

		/// <summary>
		///		engiGetEntityAttributeIndexEx                           (http://rdf.bg/ifcdoc/CS64/engiGetEntityAttributeIndexEx.html)
		///
		///	..
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityAttributeIndexEx")]
		public static extern int_t engiGetEntityAttributeIndexEx(int_t entity, string attributeName, byte countedWithParents, byte countedWithInverse);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityAttributeIndexEx")]
		public static extern int_t engiGetEntityAttributeIndexEx(int_t entity, byte[] attributeName, byte countedWithParents, byte countedWithInverse);

		/// <summary>
		///		engiGetEntityArgumentIndex                              (http://rdf.bg/ifcdoc/CS64/engiGetEntityArgumentIndex.html)
		///
		///	DEPRECATED use engiGetEntityAttributeIndex
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityArgumentIndex")]
		public static extern int_t engiGetEntityArgumentIndex(int_t entity, string argumentName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityArgumentIndex")]
		public static extern int_t engiGetEntityArgumentIndex(int_t entity, byte[] argumentName);

		/// <summary>
		///		engiGetEntityArgumentName                               (http://rdf.bg/ifcdoc/CS64/engiGetEntityArgumentName.html)
		///
		///	This call can be used to retrieve the name of the n-th argument of the given entity. Arguments of parent entities are included in the index. Both direct and inverse arguments are included.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityArgumentName")]
		public static extern IntPtr engiGetEntityArgumentName(int_t entity, int_t index, int_t valueType, out IntPtr argumentName);

		public static string engiGetEntityArgumentName(int_t entity, int_t index)
		{
			IntPtr argumentName = IntPtr.Zero;
			engiGetEntityArgumentName(entity, index, sdaiSTRING, out argumentName);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(argumentName);
		}

		/// <summary>
		///		engiGetEntityArgumentType                               (http://rdf.bg/ifcdoc/CS64/engiGetEntityArgumentType.html)
		///
		///	This call can be used to retrieve the type of the n-th argument of the given entity. In case of a select argument no relevant information is given by this call as it depends on the instance. Arguments of parent entities are included in the index. Both direct and inverse arguments are included.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityArgumentType")]
		public static extern void engiGetEntityArgumentType(int_t entity, int_t index, out int_t argumentType);

		/// <summary>
		///		engiGetEntityCount                                      (http://rdf.bg/ifcdoc/CS64/engiGetEntityCount.html)
		///
		///	Returns the total number of entities within the loaded schema.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityCount")]
		public static extern int_t engiGetEntityCount(int_t model);

		/// <summary>
		///		engiGetEntityElement                                    (http://rdf.bg/ifcdoc/CS64/engiGetEntityElement.html)
		///
		///	This call returns a specific entity based on an index, the index needs to be 0 or higher but lower then the number of entities in the loaded schema.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityElement")]
		public static extern int_t engiGetEntityElement(int_t model, int_t index);

		/// <summary>
		///		sdaiGetEntityExtent                                     (http://rdf.bg/ifcdoc/CS64/sdaiGetEntityExtent.html)
		///
		///	This call retrieves an aggregation that contains all instances of the entity given.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetEntityExtent")]
		public static extern int_t sdaiGetEntityExtent(int_t model, int_t entity);

		/// <summary>
		///		sdaiGetEntityExtentBN                                   (http://rdf.bg/ifcdoc/CS64/sdaiGetEntityExtentBN.html)
		///
		///	This call retrieves an aggregation that contains all instances of the entity given.
		///
		///	Technically sdaiGetEntityExtentBN will transform into the following call
		///		sdaiGetEntityExtent(
		///				model,
		///				sdaiGetEntity(
		///						model,
		///						entityName
		///					)
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetEntityExtentBN")]
		public static extern int_t sdaiGetEntityExtentBN(int_t model, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetEntityExtentBN")]
		public static extern int_t sdaiGetEntityExtentBN(int_t model, byte[] entityName);

		/// <summary>
		///		engiGetEntityName                                       (http://rdf.bg/ifcdoc/CS64/engiGetEntityName.html)
		///
		///	This call can be used to get the name of the given entity.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityName")]
		public static extern IntPtr engiGetEntityName(int_t entity, int_t valueType, out IntPtr entityName);

		public static string engiGetEntityName(int_t entity)
		{
			IntPtr entityName = IntPtr.Zero;
			engiGetEntityName(entity, sdaiSTRING, out entityName);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(entityName);
		}

		/// <summary>
		///		engiGetEntityNoAttributes                               (http://rdf.bg/ifcdoc/CS64/engiGetEntityNoAttributes.html)
		///
		///	This call returns the number of arguments, this includes the arguments of its (nested) parents and inverse argumnets.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityNoAttributes")]
		public static extern int_t engiGetEntityNoAttributes(int_t entity);

		/// <summary>
		///		engiGetEntityNoAttributesEx                             (http://rdf.bg/ifcdoc/CS64/engiGetEntityNoAttributesEx.html)
		///
		///	This call returns the number of attributes, inclusion of parents and inverse depeds on includeParent and includeInverse values
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityNoAttributesEx")]
		public static extern int_t engiGetEntityNoAttributesEx(int_t entity, byte includeParent, byte includeInverse);

		/// <summary>
		///		engiGetEntityNoArguments                                (http://rdf.bg/ifcdoc/CS64/engiGetEntityNoArguments.html)
		///
		///	DEPRECATED use engiGetEntityNoAttributes
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityNoArguments")]
		public static extern int_t engiGetEntityNoArguments(int_t entity);

		/// <summary>
		///		engiGetAttributeType                                    (http://rdf.bg/ifcdoc/CS64/engiGetAttributeType.html)
		///
		///	DEPRECATED use engiGetAttrType
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttributeType")]
		public static extern int_t engiGetAttributeType(int_t attribute);

		/// <summary>
		///		engiGetArgumentType                                     (http://rdf.bg/ifcdoc/CS64/engiGetArgumentType.html)
		///
		///	DEPR4ECATED use engiGetAttributeType
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetArgumentType")]
		public static extern int_t engiGetArgumentType(int_t attribute);

		/// <summary>
		///		engiGetEntityParent                                     (http://rdf.bg/ifcdoc/CS64/engiGetEntityParent.html)
		///
		///	Returns the first direct parent entity, for example the parent of IfcObject is IfcObjectDefinition, of IfcObjectDefinition is IfcRoot and of IfcRoot is 0.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityParent")]
		public static extern int_t engiGetEntityParent(int_t entity);

		/// <summary>
		///		engiGetEntityNoParents                                  (http://rdf.bg/ifcdoc/CS64/engiGetEntityNoParents.html)
		///
		///	Returns number of direct parents entity
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityNoParents")]
		public static extern int_t engiGetEntityNoParents(int_t entity);

		/// <summary>
		///		engiGetEntityParentEx                                   (http://rdf.bg/ifcdoc/CS64/engiGetEntityParentEx.html)
		///
		///	Returns the N-th direct parent of entity or NULL if index exceeds number of parents
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityParentEx")]
		public static extern int_t engiGetEntityParentEx(int_t entity, int_t index);

		/// <summary>
		///		engiGetAttrOptional                                     (http://rdf.bg/ifcdoc/CS64/engiGetAttrOptional.html)
		///
		///	This call can be used to check if an attribute is optional
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrOptional")]
		public static extern int_t engiGetAttrOptional(int_t attribute);

		/// <summary>
		///		engiGetAttrOptionalBN                                   (http://rdf.bg/ifcdoc/CS64/engiGetAttrOptionalBN.html)
		///
		///	This call can be used to check if an attribute is optional.
		///
		///	Technically engiGetAttrOptionalBN will transform into the following call
		///		engiGetAttrOptional(
		///				sdaiGetAttrDefinition(
		///						entity,
		///						attributeName
		///					)
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrOptionalBN")]
		public static extern int_t engiGetAttrOptionalBN(int_t entity, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrOptionalBN")]
		public static extern int_t engiGetAttrOptionalBN(int_t entity, byte[] attributeName);

		/// <summary>
		///		engiGetAttrDerived                                      (http://rdf.bg/ifcdoc/CS64/engiGetAttrDerived.html)
		///
		///	This call can be used to check if an attribute is defined schema wise in the context of a certain entity
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrDerived")]
		public static extern int_t engiGetAttrDerived(int_t entity, int_t attribute);

		/// <summary>
		///		engiGetAttrDerivedBN                                    (http://rdf.bg/ifcdoc/CS64/engiGetAttrDerivedBN.html)
		///
		///	This call can be used to check if an attribute is defined schema wise in the context of a certain entity.
		///
		///	Technically engiGetAttrDerivedBN will transform into the following call
		///		engiGetAttrDerived(
		///				entity,
		///				sdaiGetAttrDefinition(
		///						entity,
		///						attributeName
		///					)
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrDerivedBN")]
		public static extern int_t engiGetAttrDerivedBN(int_t entity, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrDerivedBN")]
		public static extern int_t engiGetAttrDerivedBN(int_t entity, byte[] attributeName);

		/// <summary>
		///		engiGetAttrInverse                                      (http://rdf.bg/ifcdoc/CS64/engiGetAttrInverse.html)
		///
		///	This call can be used to check if an attribute is an inverse relation
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrInverse")]
		public static extern int_t engiGetAttrInverse(int_t attribute);

		/// <summary>
		///		engiGetAttrInverseBN                                    (http://rdf.bg/ifcdoc/CS64/engiGetAttrInverseBN.html)
		///
		///	This call can be used to check if an attribute is an inverse relation.
		///
		///	Technically engiGetAttrInverseBN will transform into the following call
		///		engiGetAttrInverse(
		///				sdaiGetAttrDefinition(
		///						entity,
		///						attributeName
		///					)
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrInverseBN")]
		public static extern int_t engiGetAttrInverseBN(int_t entity, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrInverseBN")]
		public static extern int_t engiGetAttrInverseBN(int_t entity, byte[] attributeName);

		/// <summary>
		///		engiGetAttrDomain                                       (http://rdf.bg/ifcdoc/CS64/engiGetAttrDomain.html)
		///
		///	This call can be used to get the domain of an attribute
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrDomain")]
		public static extern IntPtr engiGetAttrDomain(int_t attribute, out IntPtr domainName);

		public static string engiGetAttrDomain(int_t attribute)
		{
			IntPtr domainName = IntPtr.Zero;
			engiGetAttrDomain(attribute, out domainName);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(domainName);
		}

		/// <summary>
		///		engiGetAttrDomainBN                                     (http://rdf.bg/ifcdoc/CS64/engiGetAttrDomainBN.html)
		///
		///	This call can be used to get the domain of an attribute.
		///
		///	Technically engiGetAttrDomainBN will transform into the following call
		///		engiGetAttrDomain(
		///				sdaiGetAttrDefinition(
		///						entity,
		///						attributeName
		///					),
		///				domainName
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrDomainBN")]
		public static extern IntPtr engiGetAttrDomainBN(int_t entity, string attributeName, out IntPtr domainName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrDomainBN")]
		public static extern IntPtr engiGetAttrDomainBN(int_t entity, byte[] attributeName, out IntPtr domainName);

		public static string engiGetAttrDomainBN(int_t entity, string attributeName)
		{
			IntPtr domainName = IntPtr.Zero;
			engiGetAttrDomainBN(entity, attributeName, out domainName);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(domainName);
		}

		public static string engiGetAttrDomainBN(int_t entity, byte[] attributeName)
		{
			IntPtr domainName = IntPtr.Zero;
			engiGetAttrDomainBN(entity, attributeName, out domainName);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(domainName);
		}

		/// <summary>
		///		engiGetEntityIsAbstract                                 (http://rdf.bg/ifcdoc/CS64/engiGetEntityIsAbstract.html)
		///
		///	This call can be used to check if an entity is abstract
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityIsAbstract")]
		public static extern int_t engiGetEntityIsAbstract(int_t entity);

		/// <summary>
		///		engiGetEntityIsAbstractBN                               (http://rdf.bg/ifcdoc/CS64/engiGetEntityIsAbstractBN.html)
		///
		///	This call can be used to check if an entity is abstract.
		///
		///	Technically engiGetEntityIsAbstractBN will transform into the following call
		///		engiGetEntityIsAbstract(
		///				sdaiGetEntity(
		///						model,
		///						entityName
		///					)
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityIsAbstractBN")]
		public static extern int_t engiGetEntityIsAbstractBN(int_t model, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityIsAbstractBN")]
		public static extern int_t engiGetEntityIsAbstractBN(int_t model, byte[] entityName);

		/// <summary>
		///		engiGetEnumerationValue                                 (http://rdf.bg/ifcdoc/CS64/engiGetEnumerationValue.html)
		///
		///	Allows to retrieve enumeration values of an attribute by index.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEnumerationValue")]
		public static extern IntPtr engiGetEnumerationValue(int_t attribute, int_t index, int_t valueType, out IntPtr enumerationValue);

		public static string engiGetEnumerationValue(int_t attribute, int_t index)
		{
			IntPtr enumerationValue = IntPtr.Zero;
			engiGetEnumerationValue(attribute, index, sdaiSTRING, out enumerationValue);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(enumerationValue);
		}

		/// <summary>
		///		engiGetEntityAttributeByIndex                           (http://rdf.bg/ifcdoc/CS64/engiGetEntityAttributeByIndex.html)
		///
		///	Return attribute definition from attribute index
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetEntityAttributeByIndex")]
		public static extern int_t engiGetEntityAttributeByIndex(int_t entity, int_t index, bool countedWithParents, bool countedWithInverse);

		/// <summary>
		///		engiGetAttributeTraits                                  (http://rdf.bg/ifcdoc/CS64/engiGetAttributeTraits.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttributeTraits")]
		public static extern void engiGetAttributeTraits(int_t attribute, out IntPtr name, out int_t definingEntity, out byte inverse, out enum_express_attr_type attrType, out int_t domainEntity, out int_t aggregationDescriptor, out byte optional, out byte unique);

		/// <summary>
		///		engiGetAggregation                                      (http://rdf.bg/ifcdoc/CS64/engiGetAggregation.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggregation")]
		public static extern void engiGetAggregation(int_t aggregationDescriptor, out enum_express_aggr aggrType, out int_t cardinalityMin, out int_t cardinalityMax, out int_t nextAggregationLevelDescriptor);

        //
        //  Instance Header API Calls
        //

		/// <summary>
		///		SetSPFFHeader                                           (http://rdf.bg/ifcdoc/CS64/SetSPFFHeader.html)
		///
		///	This call is an aggregate of several SetSPFFHeaderItem calls. In several cases the header can be set easily with this call. In case an argument is zero, this argument will not be updated, i.e. it will not be filled with 0.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "SetSPFFHeader")]
		public static extern void SetSPFFHeader(int_t model, string description, string implementationLevel, string name, string timeStamp, string author, string organization, string preprocessorVersion, string originatingSystem, string authorization, string fileSchema);

		[DllImport(IFCEngineDLL, EntryPoint = "SetSPFFHeader")]
		public static extern void SetSPFFHeader(int_t model, byte[] description, byte[] implementationLevel, byte[] name, byte[] timeStamp, byte[] author, byte[] organization, byte[] preprocessorVersion, byte[] originatingSystem, byte[] authorization, byte[] fileSchema);

		/// <summary>
		///		SetSPFFHeaderItem                                       (http://rdf.bg/ifcdoc/CS64/SetSPFFHeaderItem.html)
		///
		///	This call can be used to write a specific header item, the source code example is larger to show and explain how this call can be used.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "SetSPFFHeaderItem")]
		public static extern int_t SetSPFFHeaderItem(int_t model, int_t itemIndex, int_t itemSubIndex, int_t valueType, string value);

		[DllImport(IFCEngineDLL, EntryPoint = "SetSPFFHeaderItem")]
		public static extern int_t SetSPFFHeaderItem(int_t model, int_t itemIndex, int_t itemSubIndex, int_t valueType, byte[] value);

		/// <summary>
		///		GetSPFFHeaderItem                                       (http://rdf.bg/ifcdoc/CS64/GetSPFFHeaderItem.html)
		///
		///	This call can be used to read a specific header item, the source code example is larger to show and explain how this call can be used.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "GetSPFFHeaderItem")]
		public static extern int_t GetSPFFHeaderItem(int_t model, int_t itemIndex, int_t itemSubIndex, int_t valueType, out IntPtr value);

		/// <summary>
		///		GetSPFFHeaderItemUnicode                                (http://rdf.bg/ifcdoc/CS64/GetSPFFHeaderItemUnicode.html)
		///
		///	This call can be used to write a specific header item, the source code example is larger to show and explain how this call can be used.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "GetSPFFHeaderItemUnicode")]
		public static extern int_t GetSPFFHeaderItemUnicode(int_t model, int_t itemIndex, int_t itemSubIndex, byte[] buffer, int_t bufferLength);

        //
        //  Instance Reading API Calls
        //

		/// <summary>
		///		sdaiGetADBType                                          (http://rdf.bg/ifcdoc/CS64/sdaiGetADBType.html)
		///
		///	This call can be used to get the used type within this ADB type.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBType")]
		public static extern int_t sdaiGetADBType(int_t ADB);

		/// <summary>
		///		sdaiGetADBTypePath                                      (http://rdf.bg/ifcdoc/CS64/sdaiGetADBTypePath.html)
		///
		///	This call can be used to get the path of an ADB type.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBTypePath")]
		public static extern IntPtr sdaiGetADBTypePath(int_t ADB, int_t typeNameNumber);

		/// <summary>
		///		sdaiGetADBTypePathx                                     (http://rdf.bg/ifcdoc/CS64/sdaiGetADBTypePathx.html)
		///
		///	This call is the same as sdaiGetADBTypePath, however can be used by porting to languages that have issues with returned char arrays.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBTypePathx")]
		public static extern IntPtr sdaiGetADBTypePathx(int_t ADB, int_t typeNameNumber, out IntPtr path);

		public static string sdaiGetADBTypePathx(int_t ADB, int_t typeNameNumber)
		{
			IntPtr path = IntPtr.Zero;
			sdaiGetADBTypePathx(ADB, typeNameNumber, out path);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(path);
		}

		/// <summary>
		///		sdaiGetADBValue                                         (http://rdf.bg/ifcdoc/CS64/sdaiGetADBValue.html)
		///
		///	valueType argument to specify what type of data caller wants to get and
		///	value argument where the caller should provide a buffer, and the function will write the result to.
		///	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiGetADBValue, and it works similarly for all get-functions.
		///	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
		///		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
		///
		///	The Table 2 shows what valueType can be fulfilled depending on actual model data.
		///	If a get-function cannot get a value it will return 0, it may happen when model item is unset ($) or incompatible with requested valueType.
		///	To separate these cases you can use engiGetInstanceAttrType(BN), sdaiGetADBType and engiGetAggrType.
		///	On success get-function will return non-zero. More precisely, according to ISO 10303-24-2001 on success they return content of 
		///	value argument (*value) for sdaiADB, sdaiAGGR, or sdaiINSTANCE or value argument itself for other types (it has no useful meaning for C#).
		///
		///
		///	Table 1 – Required value buffer depending on valueType (on the example of sdaiGetADBValue but valid for all get-functions)
		///
		///	valueType				C/C++												C#
		///
		///	sdaiINTEGER				int_t val;											int_t val;
		///							sdaiGetADBValue (ADB, sdaiINTEGER, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiINTEGER, out val);
		///
		///	sdaiREAL or sdaiNUMBER	double val;											double val;
		///							sdaiGetADBValue (ADB, sdaiREAL, &val);				ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiREAL, out val);
		///
		///	sdaiBOOLEAN				bool val;											bool val;
		///							sdaiGetADBValue (ADB, sdaiBOOLEAN, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiBOOLEAN, out val);
		///
		///	sdaiLOGICAL				const TCHAR* val;									string val;
		///							sdaiGetADBValue (ADB, sdaiLOGICAL, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiLOGICAL, out val);
		///
		///	sdaiENUM				const TCHAR* val;									string val;
		///							sdaiGetADBValue (ADB, sdaiENUM, &val);				ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiENUM, out val);
		///
		///	sdaiBINARY				const TCHAR* val;									string val;
		///							sdaiGetADBValue (ADB, sdaiBINARY, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiBINARY, out val);
		///
		///	sdaiSTRING				const char* val;									string val;
		///							sdaiGetADBValue (ADB, sdaiSTRING, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiSTRING, out val);
		///
		///	sdaiUNICODE				const wchar_t* val;									string val;
		///							sdaiGetADBValue (ADB, sdaiUNICODE, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiSTRING, out val);
		///
		///	sdaiEXPRESSSTRING		const char* val;									string val;
		///							sdaiGetADBValue (ADB, sdaiEXPRESSSTRING, &val);		ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiEXPRESSSTRING, out val);
		///
		///	sdaiINSTANCE			SdaiInstance val;									int_t val;
		///							sdaiGetADBValue (ADB, sdaiINSTANCE, &val);			ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiINSTANCE, out val);
		///
		///	sdaiAGGR				SdaiAggr aggr;										int_t aggr;
		///							sdaiGetADBValue (ADB, sdaiAGGR, &aggr);				ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiINSTANCE, out aggr);
		///
		///	sdaiADB					SdaiADB adb = sdaiCreateEmptyADB();					int_t adb = 0;	//	it is important to initialize
		///							sdaiGetADBValue (ADB, sdaiADB, adb);				ifcengine.sdaiGetADBValue (ADB, ifcengine.sdaiADB, out adb);		
		///							sdaiDeleteADB (adb);
		///
		///							SdaiADB	adb = nullptr;	//	it is important to initialize
		///							sdaiGetADBValue (ADB, sdaiADB, &adb);
		///
		///	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
		///	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t
		///	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table
		///
		///
		///	Table 2 - valueType can be requested depending on actual model data.
		///
		///	valueType		Works for following values in the model
		///				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
		///	sdaiINTEGER			Yes			Yes *		 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiREAL			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiNUMBER			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
		///	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
		///	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
		///	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
		///	sdaiSTRING			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
		///	sdaiUNICODE			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
		///	sdaiEXPRESSSTRING	Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
		///	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
		///	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
		///	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
		///	Note: sdaiGetAttr, stdaiGetAttrBN, engiGetElement will success with any model data, except non-set($)
		///		  (Non-standard extensions) sdaiGetADBValue: sdaiADB is allowed and will success when sdaiGetADBTypePath is not NULL, returning ABD value has type path element removed.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBValue")]
		public static extern int_t sdaiGetADBValue(int_t ADB, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBValue")]
		public static extern int_t sdaiGetADBValue(int_t ADB, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBValue")]
		public static extern int_t sdaiGetADBValue(int_t ADB, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetADBValue")]
		public static extern int_t sdaiGetADBValue(int_t ADB, int_t valueType, out IntPtr value);

		public static int_t sdaiGetADBValue(int_t ADB, int_t valueType, out string value)
		{
			value = null;
			valueType = getStringType(valueType);
			if (valueType != 0)
			{
				IntPtr ptr = IntPtr.Zero;
				var ret = sdaiGetADBValue(ADB, valueType, out ptr);
				if (ret != 0 && ptr != IntPtr.Zero)
				{
					value = marshalPtrToString(valueType, ptr);
					return ret;
				}
			}
			return 0;
		}

		/// <summary>
		///		sdaiCreateEmptyADB                                      (http://rdf.bg/ifcdoc/CS64/sdaiCreateEmptyADB.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateEmptyADB")]
		public static extern int_t sdaiCreateEmptyADB();

		/// <summary>
		///		sdaiDeleteADB                                           (http://rdf.bg/ifcdoc/CS64/sdaiDeleteADB.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiDeleteADB")]
		public static extern void sdaiDeleteADB(int_t ADB);

		/// <summary>
		///		sdaiGetAggrByIndex                                      (http://rdf.bg/ifcdoc/CS64/sdaiGetAggrByIndex.html)
		///
		///	valueType argument to specify what type of data caller wants to get and
		///	value argument where the caller should provide a buffer, and the function will write the result to.
		///	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiGetAggrByIndex, and it works similarly for all get-functions.
		///	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
		///		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
		///
		///	The Table 2 shows what valueType can be fulfilled depending on actual model data.
		///	If a get-function cannot get a value it will return 0, it may happen when model item is unset ($) or incompatible with requested valueType.
		///	To separate these cases you can use engiGetInstanceAttrType(BN), sdaiGetADBType and engiGetAggrType.
		///	On success get-function will return non-zero. More precisely, according to ISO 10303-24-2001 on success they return content of
		///	value argument (*value) for sdaiADB, sdaiAGGR, or sdaiINSTANCE or value argument itself for other types (it has no useful meaning for C#).
		///
		///
		///	Table 1 – Required value buffer depending on valueType (on the example of sdaiGetAggrByIndex but valid for all get-functions)
		///
		///	valueType				C/C++															C#
		///
		///	sdaiINTEGER				int_t val;														int_t val;
		///							sdaiGetAggrByIndex (aggr, index, sdaiINTEGER, &val);			ifcengine.sdaiGetAggrByIndex (aggr, index, ifcengine.sdaiINTEGER, out val);
		///
		///	sdaiREAL or sdaiNUMBER	double val;														double val;
		///							sdaiGetAggrByIndex (aggr, index, sdaiREAL, &val);				ifcengine.sdaiGetAggrByIndex (aggr, index, ifcengine.sdaiREAL, out val);
		///
		///	sdaiBOOLEAN				bool val;														bool val;
		///							sdaiGetAggrByIndex (aggr, index, sdaiBOOLEAN, &val);			ifcengine.sdaiGetAggrByIndex (aggr, index, ifcengine.sdaiBOOLEAN, out val);
		///
		///	sdaiLOGICAL				const TCHAR* val;												string val;
		///							sdaiGetAggrByIndex (aggr, index, sdaiLOGICAL, &val);			ifcengine.sdaiGetAggrByIndex (aggr, index, ifcengine.sdaiLOGICAL, out val);
		///
		///	sdaiENUM				const TCHAR* val;												string val;
		///							sdaiGetAggrByIndex (aggr, index, sdaiENUM, &val);				ifcengine.sdaiGetAggrByIndex (aggr, index, ifcengine.sdaiENUM, out val);
		///
		///	sdaiBINARY				const TCHAR* val;												string val;
		///							sdaiGetAggrByIndex (aggr, index, sdaiBINARY, &val);				ifcengine.sdaiGetAggrByIndex (aggr, index, ifcengine.sdaiBINARY, out val);
		///
		///	sdaiSTRING				const char* val;												string val;
		///							sdaiGetAggrByIndex (aggr, index, sdaiSTRING, &val);				ifcengine.sdaiGetAggrByIndex (aggr, index, ifcengine.sdaiSTRING, out val);
		///
		///	sdaiUNICODE				const wchar_t* val;												string val;
		///							sdaiGetAggrByIndex (aggr, index, sdaiUNICODE, &val);			ifcengine.sdaiGetAggrByIndex (aggr, index, ifcengine.sdaiSTRING, out val);
		///
		///	sdaiEXPRESSSTRING		const char* val;												string val;
		///							sdaiGetAggrByIndex (aggr, index, sdaiEXPRESSSTRING, &val);		ifcengine.sdaiGetAggrByIndex (aggr, index, ifcengine.sdaiEXPRESSSTRING, out val);
		///
		///	sdaiINSTANCE			SdaiInstance val;												int_t val;
		///							sdaiGetAggrByIndex (aggr, index, sdaiINSTANCE, &val);			ifcengine.sdaiGetAggrByIndex (aggr, index, ifcengine.sdaiINSTANCE, out val);
		///
		///	sdaiAGGR				SdaiAggr aggr;													int_t aggr;
		///							sdaiGetAggrByIndex (aggr, index, sdaiAGGR, &aggr);				ifcengine.sdaiGetAggrByIndex (aggr, index, ifcengine.sdaiINSTANCE, out aggr);
		///
		///	sdaiADB					SdaiADB adb = sdaiCreateEmptyADB();								int_t adb = 0;	//	it is important to initialize
		///							sdaiGetAggrByIndex (aggr, index, sdaiADB, adb);					ifcengine.sdaiGetAggrByIndex (aggr, index, ifcengine.sdaiADB, out adb);		
		///							sdaiDeleteADB (adb);
		///
		///							SdaiADB	adb = nullptr;	//	it is important to initialize
		///							sdaiGetADBValue (ADB, sdaiADB, &adb);
		///
		///	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
		///	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t
		///	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table
		///
		///
		///	Table 2 - valueType can be requested depending on actual model data.
		///
		///	valueType		Works for following values in the model
		///				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
		///	sdaiINTEGER			Yes			Yes *		 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiREAL			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiNUMBER			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
		///	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
		///	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
		///	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
		///	sdaiSTRING			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
		///	sdaiUNICODE			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
		///	sdaiEXPRESSSTRING	Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
		///	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
		///	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
		///	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
		///	Note: sdaiGetAttr, stdaiGetAttrBN, engiGetElement will success with any model data, except non-set($)
		///		  (Non-standard extensions) sdaiGetADBValue: sdaiADB is allowed and will success when sdaiGetADBTypePath is not NULL, returning ABD value has type path element removed.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIndex")]
		public static extern int_t sdaiGetAggrByIndex(int_t aggregate, int_t index, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIndex")]
		public static extern int_t sdaiGetAggrByIndex(int_t aggregate, int_t index, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIndex")]
		public static extern int_t sdaiGetAggrByIndex(int_t aggregate, int_t index, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIndex")]
		public static extern int_t sdaiGetAggrByIndex(int_t aggregate, int_t index, int_t valueType, out IntPtr value);

		public static int_t sdaiGetAggrByIndex(int_t aggregate, int_t index, int_t valueType, out string value)
		{
			value = null;
			valueType = getStringType(valueType);
			if (valueType != 0)
			{
				IntPtr ptr = IntPtr.Zero;
				var ret = sdaiGetAggrByIndex(aggregate, index, valueType, out ptr);
				if (ret != 0 && ptr != IntPtr.Zero)
				{
					value = marshalPtrToString(valueType, ptr);
					return ret;
				}
			}
			return 0;
		}

		/// <summary>
		///		engiGetAggrType                                         (http://rdf.bg/ifcdoc/CS64/engiGetAggrType.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrType")]
		public static extern void engiGetAggrType(int_t aggregate, out int_t aggregateType);

		/// <summary>
		///		engiGetAggrTypex                                        (http://rdf.bg/ifcdoc/CS64/engiGetAggrTypex.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrTypex")]
		public static extern void engiGetAggrTypex(int_t aggregate, out int_t aggregateType);

		/// <summary>
		///		sdaiGetAttr                                             (http://rdf.bg/ifcdoc/CS64/sdaiGetAttr.html)
		///
		///	valueType argument to specify what type of data caller wants to get and
		///	value argument where the caller should provide a buffer, and the function will write the result to.
		///	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiGetAttr, and it works similarly for all get-functions.
		///	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
		///		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
		///
		///	The Table 2 shows what valueType can be fulfilled depending on actual model data.
		///	If a get-function cannot get a value it will return 0, it may happen when model item is unset ($) or incompatible with requested valueType.
		///	To separate these cases you can use engiGetInstanceAttrType(BN), sdaiGetADBType, and engiGetAggrType.
		///	On success get-function will return non-zero. More precisely, according to ISO 10303-24-2001 on success they return content of
		///	value argument (*value) for sdaiADB, sdaiAGGR, or sdaiINSTANCE or value argument itself for other types (it has no useful meaning for C#).
		///
		///
		///	Table 1 – Required value buffer depending on valueType (on the example of sdaiGetAttr but valid for all get-functions)
		///
		///	valueType				C/C++													C#
		///
		///	sdaiINTEGER				int_t val;												int_t val;
		///							sdaiGetAttr (inst, attr, sdaiINTEGER, &val);			ifcengine.sdaiGetAttr (inst, attr, ifcengine.sdaiINTEGER, out val);
		///
		///	sdaiREAL or sdaiNUMBER	double val;												double val;
		///							sdaiGetAttr (inst, attr, sdaiREAL, &val);				ifcengine.sdaiGetAttr (inst, attr, ifcengine.sdaiREAL, out val);
		///
		///	sdaiBOOLEAN				bool val;												bool val;
		///							sdaiGetAttr (inst, attr, sdaiBOOLEAN, &val);			ifcengine.sdaiGetAttr (inst, attr, ifcengine.sdaiBOOLEAN, out val);
		///
		///	sdaiLOGICAL				const TCHAR* val;										string val;
		///							sdaiGetAttr (inst, attr, sdaiLOGICAL, &val);			ifcengine.sdaiGetAttr (inst, attr, ifcengine.sdaiLOGICAL, out val);
		///
		///	sdaiENUM				const TCHAR* val;										string val;
		///							sdaiGetAttr (inst, attr, sdaiENUM, &val);				ifcengine.sdaiGetAttr (inst, attr, ifcengine.sdaiENUM, out val);
		///
		///	sdaiBINARY				const TCHAR* val;										string val;
		///							sdaiGetAttr (inst, attr, sdaiBINARY, &val);				ifcengine.sdaiGetAttr (inst, attr, ifcengine.sdaiBINARY, out val);
		///
		///	sdaiSTRING				const char* val;										string val;
		///							sdaiGetAttr (inst, attr, sdaiSTRING, &val);				ifcengine.sdaiGetAttr (inst, attr, ifcengine.sdaiSTRING, out val);
		///
		///	sdaiUNICODE				const wchar_t* val;										string val;
		///							sdaiGetAttr (inst, attr, sdaiUNICODE, &val);			ifcengine.sdaiGetAttr (inst, attr, ifcengine.sdaiSTRING, out val);
		///
		///	sdaiEXPRESSSTRING		const char* val;										string val;
		///							sdaiGetAttr (inst, attr, sdaiEXPRESSSTRING, &val);		ifcengine.sdaiGetAttr (inst, attr, ifcengine.sdaiEXPRESSSTRING, out val);
		///
		///	sdaiINSTANCE			SdaiInstance val;										int_t val;
		///							sdaiGetAttr (inst, attr, sdaiINSTANCE, &val);			ifcengine.sdaiGetAttr (inst, attr, ifcengine.sdaiINSTANCE, out val);
		///
		///	sdaiAGGR				SdaiAggr aggr;											int_t aggr;
		///							sdaiGetAttr (inst, attr, sdaiAGGR, &aggr);				ifcengine.sdaiGetAttr (inst, attr, ifcengine.sdaiINSTANCE, out aggr);
		///
		///	sdaiADB					SdaiADB adb = sdaiCreateEmptyADB();						int_t adb = 0;	//	it is important to initialize
		///							sdaiGetAttr (inst, attr, sdaiADB, adb);					ifcengine.sdaiGetAttr (inst, attr, ifcengine.sdaiADB, out adb);		
		///							sdaiDeleteADB (adb);
		///
		///							SdaiADB adb = nullptr;	//	it is important to initialize
		///							sdaiGetAttr (inst, attr, sdaiADB, &adb);
		///
		///	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
		///	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t
		///	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table
		///
		///
		///	Table 2 - valueType can be requested depending on actual model data.
		///
		///	valueType		Works for following values in the model
		///				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
		///	sdaiINTEGER			Yes			Yes *		 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiREAL			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiNUMBER			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
		///	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
		///	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
		///	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
		///	sdaiSTRING			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
		///	sdaiUNICODE			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
		///	sdaiEXPRESSSTRING	Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
		///	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
		///	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
		///	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
		///	Note: sdaiGetAttr, stdaiGetAttrBN, engiGetElement will success with any model data, except non-set($)
		///		  (Non-standard extensions) sdaiGetADBValue: sdaiADB is allowed and will success when sdaiGetADBTypePath is not NULL, returning ABD value has type path element removed.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttr")]
		public static extern int_t sdaiGetAttr(int_t instance, int_t attribute, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttr")]
		public static extern int_t sdaiGetAttr(int_t instance, int_t attribute, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttr")]
		public static extern int_t sdaiGetAttr(int_t instance, int_t attribute, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttr")]
		public static extern int_t sdaiGetAttr(int_t instance, int_t attribute, int_t valueType, out IntPtr value);

		public static int_t sdaiGetAttr(int_t instance, int_t attribute, int_t valueType, out string value)
		{
			value = null;
			valueType = getStringType(valueType);
			if (valueType != 0)
			{
				IntPtr ptr = IntPtr.Zero;
				var ret = sdaiGetAttr(instance, attribute, valueType, out ptr);
				if (ret != 0 && ptr != IntPtr.Zero)
				{
					value = marshalPtrToString(valueType, ptr);
					return ret;
				}
			}
			return 0;
		}

		/// <summary>
		///		sdaiGetAttrBN                                           (http://rdf.bg/ifcdoc/CS64/sdaiGetAttrBN.html)
		///
		///	valueType argument to specify what type of data caller wants to get and
		///	value argument where the caller should provide a buffer, and the function will write the result to.
		///	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiGetAttrBN, and it works similarly for all get-functions.
		///	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
		///		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
		///
		///	The Table 2 shows what valueType can be fulfilled depending on actual model data.
		///	If a get-function cannot get a value it will return 0, it may happen when model item is unset ($) or incompatible with requested valueType.
		///	To separate these cases you can use engiGetInstanceAttrType(BN), sdaiGetADBType and engiGetAggrType.
		///	On success get-function will return non-zero. More precisely, according to ISO 10303-24-2001 on success they return content of
		///	value argument (*value) for sdaiADB, sdaiAGGR, or sdaiINSTANCE or value argument itself for other types (it has no useful meaning for C#).
		///
		///
		///	Table 1 – Required value buffer depending on valueType (on the example of sdaiGetAttrBN but valid for all get-functions)
		///
		///	valueType				C/C++														C#
		///
		///	sdaiINTEGER				int_t val;													int_t val;
		///							sdaiGetAttrBN (inst, "attrName", sdaiINTEGER, &val);		ifcengine.sdaiGetAttrBN (inst, "attrName", ifcengine.sdaiINTEGER, out val);
		///
		///	sdaiREAL or sdaiNUMBER	double val;													double val;
		///							sdaiGetAttrBN (inst, "attrName", sdaiREAL, &val);			ifcengine.sdaiGetAttrBN (inst, "attrName", ifcengine.sdaiREAL, out val);
		///
		///	sdaiBOOLEAN				bool val;													bool val;
		///							sdaiGetAttrBN (inst, "attrName", sdaiBOOLEAN, &val);		ifcengine.sdaiGetAttrBN (inst, "attrName", ifcengine.sdaiBOOLEAN, out val);
		///
		///	sdaiLOGICAL				const TCHAR* val;											string val;
		///							sdaiGetAttrBN (inst, "attrName", sdaiLOGICAL, &val);		ifcengine.sdaiGetAttrBN (inst, "attrName", ifcengine.sdaiLOGICAL, out val);
		///
		///	sdaiENUM				const TCHAR* val;											string val;
		///							sdaiGetAttrBN (inst, "attrName", sdaiENUM, &val);			ifcengine.sdaiGetAttrBN (inst, "attrName", ifcengine.sdaiENUM, out val);
		///
		///	sdaiBINARY				const TCHAR* val;											string val;
		///							sdaiGetAttrBN (inst, "attrName", sdaiBINARY, &val);			ifcengine.sdaiGetAttrBN (inst, "attrName", ifcengine.sdaiBINARY, out val);
		///
		///	sdaiSTRING				const char* val;											string val;
		///							sdaiGetAttrBN (inst, "attrName", sdaiSTRING, &val);			ifcengine.sdaiGetAttrBN (inst, "attrName", ifcengine.sdaiSTRING, out val);
		///
		///	sdaiUNICODE				const wchar_t* val;											string val;
		///							sdaiGetAttrBN (inst, "attrName", sdaiUNICODE, &val);		ifcengine.sdaiGetAttrBN (inst, "attrName", ifcengine.sdaiSTRING, out val);
		///
		///	sdaiEXPRESSSTRING		const char* val;											string val;
		///							sdaiGetAttrBN (inst, "attrName", sdaiEXPRESSSTRING, &val);	ifcengine.sdaiGetAttrBN (inst, "attrName", ifcengine.sdaiEXPRESSSTRING, out val);
		///
		///	sdaiINSTANCE			SdaiInstance val;											int_t val;
		///							sdaiGetAttrBN (inst, "attrName", sdaiINSTANCE, &val);		ifcengine.sdaiGetAttrBN (inst, "attrName", ifcengine.sdaiINSTANCE, out val);
		///
		///	sdaiAGGR				SdaiAggr aggr;												int_t aggr;
		///							sdaiGetAttrBN (inst, "attrName", sdaiAGGR, &aggr);			ifcengine.sdaiGetAttrBN (inst, "attrName", ifcengine.sdaiINSTANCE, out aggr);
		///
		///	sdaiADB					SdaiADB adb = sdaiCreateEmptyADB();							int_t adb = 0;	//	it is important to initialize
		///							sdaiGetAttrBN (inst, "attrName", sdaiADB, adb);				ifcengine.sdaiGetAttrBN (inst, "attrName", ifcengine.sdaiADB, out adb);		
		///							sdaiDeleteADB (adb);
		///
		///							SdaiADB adb = nullptr;	//	it is important to initialize
		///							sdaiGetAttrBN (inst, "attrName", sdaiADB, &adb);
		///
		///	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
		///	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t
		///	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table
		///
		///
		///	Table 2 - valueType can be requested depending on actual model data.
		///
		///	valueType		Works for following values in the model
		///				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
		///	sdaiINTEGER			Yes			Yes *		 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiREAL			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiNUMBER			Yes			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
		///	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
		///	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
		///	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
		///	sdaiSTRING			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
		///	sdaiUNICODE			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
		///	sdaiEXPRESSSTRING	Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .			 .			 .
		///	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
		///	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
		///	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
		///	Note: sdaiGetAttr, stdaiGetAttrBN, engiGetElement will success with any model data, except non-set($)
		///		  (Non-standard extensions) sdaiGetADBValue: sdaiADB is allowed and will success when sdaiGetADBTypePath is not NULL, returning ABD value has type path element removed.
		///
		///	Technically sdaiGetAttrBN will transform into the following call
		///		sdaiGetAttr(
		///				instance,
		///				sdaiGetAttrDefinition(
		///						sdaiGetInstanceType(
		///								instance
		///							),
		///						attributeName
		///					),
		///				valueType,
		///				value
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, string attributeName, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, string attributeName, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, string attributeName, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, string attributeName, int_t valueType, out IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, byte[] attributeName, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, byte[] attributeName, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, byte[] attributeName, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBN")]
		public static extern int_t sdaiGetAttrBN(int_t instance, byte[] attributeName, int_t valueType, out IntPtr value);

		public static int_t sdaiGetAttrBN(int_t instance, string attrName, int_t valueType, out string value)
		{
			value = null;
			valueType = getStringType(valueType);
			if (valueType != 0)
			{
				IntPtr ptr = IntPtr.Zero;
				var ret = sdaiGetAttrBN(instance, attrName, valueType, out ptr);
				if (ret != 0 && ptr != IntPtr.Zero)
				{
					value = marshalPtrToString(valueType, ptr);
					return ret;
				}
			}
			return 0;
		}

		/// <summary>
		///		sdaiGetAttrBNUnicode                                    (http://rdf.bg/ifcdoc/CS64/sdaiGetAttrBNUnicode.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBNUnicode")]
		public static extern int_t sdaiGetAttrBNUnicode(int_t instance, string attributeName, byte[] buffer, int_t bufferLength);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrBNUnicode")]
		public static extern int_t sdaiGetAttrBNUnicode(int_t instance, byte[] attributeName, byte[] buffer, int_t bufferLength);

		/// <summary>
		///		sdaiGetStringAttrBN                                     (http://rdf.bg/ifcdoc/CS64/sdaiGetStringAttrBN.html)
		///
		///	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiSTRING.
		///	This call can be usefull in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
		///	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
		///
		///	Technically sdaiGetStringAttrBN will transform into the following call
		///		char	* rValue = 0;
		///		sdaiGetAttr(
		///				instance,
		///				sdaiGetAttrDefinition(
		///						sdaiGetInstanceType(
		///								instance
		///							),
		///						attributeName
		///					),
		///				sdaiSTRING,
		///				&rValue
		///			);
		///		return	rValue;
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetStringAttrBN")]
		public static extern IntPtr sdaiGetStringAttrBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetStringAttrBN")]
		public static extern IntPtr sdaiGetStringAttrBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		sdaiGetInstanceAttrBN                                   (http://rdf.bg/ifcdoc/CS64/sdaiGetInstanceAttrBN.html)
		///
		///	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiINSTANCE.
		///	This call can be usefull in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
		///	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
		///
		///	Technically sdaiGetInstanceAttrBN will transform into the following call
		///		SdaiInstance	inst = 0;
		///		sdaiGetAttr(
		///				instance,
		///				sdaiGetAttrDefinition(
		///						sdaiGetInstanceType(
		///								instance
		///							),
		///						attributeName
		///					),
		///				sdaiINSTANCE,
		///				&inst
		///			);
		///		return	inst;
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetInstanceAttrBN")]
		public static extern int_t sdaiGetInstanceAttrBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetInstanceAttrBN")]
		public static extern int_t sdaiGetInstanceAttrBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		sdaiGetAggregationAttrBN                                (http://rdf.bg/ifcdoc/CS64/sdaiGetAggregationAttrBN.html)
		///
		///	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiAGGR.
		///	This call can be usefull in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
		///	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
		///
		///	Technically sdaiGetAggregationAttrBN will transform into the following call
		///		SdaiAggr	aggr = 0;
		///		sdaiGetAttr(
		///				instance,
		///				sdaiGetAttrDefinition(
		///						sdaiGetInstanceType(
		///								instance
		///							),
		///						attributeName
		///					),
		///				sdaiAGGR,
		///				&aggr
		///			);
		///		return	aggr;
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggregationAttrBN")]
		public static extern int_t sdaiGetAggregationAttrBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggregationAttrBN")]
		public static extern int_t sdaiGetAggregationAttrBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		sdaiGetAttrDefinition                                   (http://rdf.bg/ifcdoc/CS64/sdaiGetAttrDefinition.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrDefinition")]
		public static extern int_t sdaiGetAttrDefinition(int_t entity, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAttrDefinition")]
		public static extern int_t sdaiGetAttrDefinition(int_t entity, byte[] attributeName);

		/// <summary>
		///		sdaiGetInstanceType                                     (http://rdf.bg/ifcdoc/CS64/sdaiGetInstanceType.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetInstanceType")]
		public static extern int_t sdaiGetInstanceType(int_t instance);

		/// <summary>
		///		sdaiGetMemberCount                                      (http://rdf.bg/ifcdoc/CS64/sdaiGetMemberCount.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetMemberCount")]
		public static extern int_t sdaiGetMemberCount(int_t aggregate);

		/// <summary>
		///		sdaiIsKindOf                                            (http://rdf.bg/ifcdoc/CS64/sdaiIsKindOf.html)
		///
		///	This call checks if an instance is a type of a certain given entity.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsKindOf")]
		public static extern int_t sdaiIsKindOf(int_t instance, int_t entity);

		/// <summary>
		///		sdaiIsKindOfBN                                          (http://rdf.bg/ifcdoc/CS64/sdaiIsKindOfBN.html)
		///
		///	This call checks if an instance is a type of a certain given entity.
		///
		///	Technically sdaiIsKindOfBN will transform into the following call
		///		sdaiIsKindOf(
		///				instance,
		///				sdaiGetEntity(
		///						engiGetEntityModel(
		///								sdaiGetInstanceType(
		///										instance
		///									)
		///							),
		///						entityName
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsKindOfBN")]
		public static extern int_t sdaiIsKindOfBN(int_t instance, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsKindOfBN")]
		public static extern int_t sdaiIsKindOfBN(int_t instance, byte[] entityName);

		/// <summary>
		///		engiGetAttrType                                         (http://rdf.bg/ifcdoc/CS64/engiGetAttrType.html)
		///
		///	Returns primitive SDAI data type for the attribute according to schema, e.g. sdaiINTEGER
		///
		///	In case of aggregation if will return base primitive type combined with engiTypeFlagAggr, e.g. sdaiINTEGER|engiTypeFlagAggr
		///
		///	For SELECT it will return sdaiINSTANCE if all options are instances or aggegation of instances, either sdaiADB
		///	In case of SELECT and sdaiINSTANCE, return value will be combined with engiTypeFlagAggrOption if some options are aggregation
		///	or engiTypeFlagAggr if all options are aggregations of instances
		///
		///	It works for direct and inverse attributes
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrType")]
		public static extern int_t engiGetAttrType(int_t attribute);

		/// <summary>
		///		engiGetAttrTypeBN                                       (http://rdf.bg/ifcdoc/CS64/engiGetAttrTypeBN.html)
		///
		///	Combines sdaiGetAttrDefinition and engiGetAttrType.
		///
		///	Technically engiGetAttrTypeBN will transform into the following call
		///		engiGetAttrType(
		///				sdaiGetAttrDefinition(
		///						entity,
		///						attributeName
		///					)
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrTypeBN")]
		public static extern int_t engiGetAttrTypeBN(int_t entity, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttrTypeBN")]
		public static extern int_t engiGetAttrTypeBN(int_t entity, byte[] attributeName);

		/// <summary>
		///		engiGetInstanceAttrType                                 (http://rdf.bg/ifcdoc/CS64/engiGetInstanceAttrType.html)
		///
		///	Returns SDAI type for actual data stored in the instance for the attribute
		///	It may be primitive type, sdaiAGGR or sdaiADB
		///	Returns 0 for $ and * 
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceAttrType")]
		public static extern int_t engiGetInstanceAttrType(int_t instance, int_t attribute);

		/// <summary>
		///		engiGetInstanceAttrTypeBN                               (http://rdf.bg/ifcdoc/CS64/engiGetInstanceAttrTypeBN.html)
		///
		///	Combines sdaiGetAttrDefinition and engiGetInstanceAttrType.
		///
		///	Technically engiGetInstanceAttrTypeBN will transform into the following call
		///		engiGetInstanceAttrType(
		///				instance,
		///				sdaiGetAttrDefinition(
		///						sdaiGetInstanceType(
		///								instance
		///							),
		///						attributeName
		///					)
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceAttrTypeBN")]
		public static extern int_t engiGetInstanceAttrTypeBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceAttrTypeBN")]
		public static extern int_t engiGetInstanceAttrTypeBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		sdaiIsInstanceOf                                        (http://rdf.bg/ifcdoc/CS64/sdaiIsInstanceOf.html)
		///
		///	This call checks if an instance is an exact instance of a given entity.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsInstanceOf")]
		public static extern int_t sdaiIsInstanceOf(int_t instance, int_t entity);

		/// <summary>
		///		sdaiIsInstanceOfBN                                      (http://rdf.bg/ifcdoc/CS64/sdaiIsInstanceOfBN.html)
		///
		///	This call checks if an instance is an exact instance of a given entity.
		///
		///	Technically sdaiIsInstanceOfBN will transform into the following call
		///		sdaiIsInstanceOf(
		///				instance,
		///				sdaiGetEntity(
		///						engiGetEntityModel(
		///								sdaiGetInstanceType(
		///										instance
		///									)
		///							),
		///						entityName
		///					)
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsInstanceOfBN")]
		public static extern int_t sdaiIsInstanceOfBN(int_t instance, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsInstanceOfBN")]
		public static extern int_t sdaiIsInstanceOfBN(int_t instance, byte[] entityName);

		/// <summary>
		///		sdaiIsEqual                                             (http://rdf.bg/ifcdoc/CS64/sdaiIsEqual.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiIsEqual")]
		public static extern byte sdaiIsEqual(int_t instanceI, int_t instanceII);

		/// <summary>
		///		sdaiValidateAttribute                                   (http://rdf.bg/ifcdoc/CS64/sdaiValidateAttribute.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiValidateAttribute")]
		public static extern int_t sdaiValidateAttribute(int_t instance, int_t attribute);

		/// <summary>
		///		sdaiValidateAttributeBN                                 (http://rdf.bg/ifcdoc/CS64/sdaiValidateAttributeBN.html)
		///
		///	Technically it will transform into the following call
		///		sdaiValidateAttribute(
		///				instance,
		///				sdaiGetAttrDefinition(
		///						sdaiGetInstanceType(
		///								instance
		///							),
		///						attributeName
		///					)
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiValidateAttributeBN")]
		public static extern int_t sdaiValidateAttributeBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiValidateAttributeBN")]
		public static extern int_t sdaiValidateAttributeBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		engiGetInstanceClassInfo                                (http://rdf.bg/ifcdoc/CS64/engiGetInstanceClassInfo.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceClassInfo")]
		public static extern IntPtr engiGetInstanceClassInfo(int_t instance);

		/// <summary>
		///		engiGetInstanceClassInfoUC                              (http://rdf.bg/ifcdoc/CS64/engiGetInstanceClassInfoUC.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceClassInfoUC")]
		public static extern IntPtr engiGetInstanceClassInfoUC(int_t instance);

		/// <summary>
		///		engiGetInstanceMetaInfo                                 (http://rdf.bg/ifcdoc/CS64/engiGetInstanceMetaInfo.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceMetaInfo")]
		public static extern int_t engiGetInstanceMetaInfo(int_t instance, out int_t localId, out IntPtr entityName, out IntPtr entityNameUC);

		/// <summary>
		///		sdaiFindInstanceUsers                                   (http://rdf.bg/ifcdoc/CS64/sdaiFindInstanceUsers.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiFindInstanceUsers")]
		public static extern int_t sdaiFindInstanceUsers(int_t instance, int_t domain, int_t resultList);

		/// <summary>
		///		sdaiFindInstanceUsedInBN                                (http://rdf.bg/ifcdoc/CS64/sdaiFindInstanceUsedInBN.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiFindInstanceUsedInBN")]
		public static extern int_t sdaiFindInstanceUsedInBN(int_t instance, string roleName, int_t domain, int_t resultList);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiFindInstanceUsedInBN")]
		public static extern int_t sdaiFindInstanceUsedInBN(int_t instance, byte[] roleName, int_t domain, int_t resultList);

        //
        //  Instance Writing API Calls
        //

		/// <summary>
		///		sdaiPrepend                                             (http://rdf.bg/ifcdoc/CS64/sdaiPrepend.html)
		///
		///	valueType argument to specify what type of data caller wants to put
		///	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiPrepend, and it works similarly for all put-functions.
		///	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
		///		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
		///
		///
		///	Table 1 – Required value buffer depending on valueType (on the example of sdaiPrepend but valid for all put-functions)
		///
		///	valueType				C/C++														C#
		///
		///	sdaiINTEGER				int_t val = 123;											int_t val = 123;
		///							sdaiPrepend (aggr, sdaiINTEGER, &val);						ifcengine.sdaiPrepend (aggr, ifcengine.sdaiINTEGER, ref val);
		///
		///	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
		///							sdaiPrepend (aggr, sdaiREAL, &val);							ifcengine.sdaiPrepend (aggr, ifcengine.sdaiREAL, ref val);
		///
		///	sdaiBOOLEAN				bool val = true;											bool val = true;
		///							sdaiPrepend (aggr, sdaiBOOLEAN, &val);						ifcengine.sdaiPrepend (aggr, ifcengine.sdaiBOOLEAN, ref val);
		///
		///	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
		///							sdaiPrepend (aggr, sdaiLOGICAL, val);						ifcengine.sdaiPrepend (aggr, ifcengine.sdaiLOGICAL, val);
		///
		///	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
		///							sdaiPrepend (aggr, sdaiENUM, val);							ifcengine.sdaiPrepend (aggr, ifcengine.sdaiENUM, val);
		///
		///	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
		///							sdaiPrepend (aggr, sdaiBINARY, val);						ifcengine.sdaiPrepend (aggr, ifcengine.sdaiBINARY, val);
		///
		///	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
		///							sdaiPrepend (aggr, sdaiSTRING, val);						ifcengine.sdaiPrepend (aggr, ifcengine.sdaiSTRING, val);
		///
		///	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
		///							sdaiPrepend (aggr, sdaiUNICODE, val);						ifcengine.sdaiPrepend (aggr, ifcengine.sdaiSTRING, val);
		///
		///	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
		///							sdaiPrepend (aggr, sdaiEXPRESSSTRING, val);					ifcengine.sdaiPrepend (aggr, ifcengine.sdaiEXPRESSSTRING, val);
		///
		///	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
		///							sdaiPrepend (aggr, sdaiINSTANCE, val);						ifcengine.sdaiPrepend (aggr, ifcengine.sdaiINSTANCE, val);
		///
		///	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
		///							sdaiAppend (val, sdaiINSTANCE, inst);						ifcengine.sdaiAppend (val, ifcengine.sdaiINSTANCE, inst);
		///							sdaiPrepend (aggr, sdaiAGGR, val);							ifcengine.sdaiPrepend (aggr, ifcengine.sdaiAGGR, val);
		///
		///	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
		///							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
		///							sdaiPutADBTypePath (val, 1, "INTEGER");						ifcengine.sdaiPutADBTypePath (val, 1, "INTEGER");
		///							sdaiPrepend (aggr, sdaiADB, val);							ifcengine.sdaiPrepend (aggr, ifcengine.sdaiADB, val);	
		///							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
		///
		///	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
		///	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t
		///	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table
		///
		///
		///	Table 2 - valueType can be requested depending on actual model data.
		///
		///	valueType		Works for following values in the model
		///				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
		///	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
		///	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
		///	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
		///	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
		///	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
		///	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
		///	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrepend")]
		public static extern void sdaiPrepend(int_t aggregate, int_t valueType, ref bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrepend")]
		public static extern void sdaiPrepend(int_t aggregate, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrepend")]
		public static extern void sdaiPrepend(int_t aggregate, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrepend")]
		public static extern void sdaiPrepend(int_t aggregate, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrepend")]
		public static extern void sdaiPrepend(int_t aggregate, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrepend")]
		public static extern void sdaiPrepend(int_t aggregate, int_t valueType, byte[] value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrepend")]
		public static extern void sdaiPrepend(int_t aggregate, int_t valueType, string value);

		/// <summary>
		///		sdaiAppend                                              (http://rdf.bg/ifcdoc/CS64/sdaiAppend.html)
		///
		///	valueType argument to specify what type of data caller wants to put
		///	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiAppend, and it works similarly for all put-functions.
		///	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
		///		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
		///
		///
		///	Table 1 – Required value buffer depending on valueType (on the example of sdaiAppend but valid for all put-functions)
		///
		///	valueType				C/C++														C#
		///
		///	sdaiINTEGER				int_t val = 123;											int_t val = 123;
		///							sdaiAppend (aggr, sdaiINTEGER, &val);						ifcengine.sdaiAppend (aggr, ifcengine.sdaiINTEGER, ref val);
		///
		///	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
		///							sdaiAppend (aggr, sdaiREAL, &val);							ifcengine.sdaiAppend (aggr, ifcengine.sdaiREAL, ref val);
		///
		///	sdaiBOOLEAN				bool val = true;											bool val = true;
		///							sdaiAppend (aggr, sdaiBOOLEAN, &val);						ifcengine.sdaiAppend (aggr, ifcengine.sdaiBOOLEAN, ref val);
		///
		///	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
		///							sdaiAppend (aggr, sdaiLOGICAL, val);						ifcengine.sdaiAppend (aggr, ifcengine.sdaiLOGICAL, val);
		///
		///	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
		///							sdaiAppend (aggr, sdaiENUM, val);							ifcengine.sdaiAppend (aggr, ifcengine.sdaiENUM, val);
		///
		///	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
		///							sdaiAppend (aggr, sdaiBINARY, val);							ifcengine.sdaiAppend (aggr, ifcengine.sdaiBINARY, val);
		///
		///	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
		///							sdaiAppend (aggr, sdaiSTRING, val);							ifcengine.sdaiAppend (aggr, ifcengine.sdaiSTRING, val);
		///
		///	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
		///							sdaiAppend (aggr, sdaiUNICODE, val);						ifcengine.sdaiAppend (aggr, ifcengine.sdaiSTRING, val);
		///
		///	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
		///							sdaiAppend (aggr, sdaiEXPRESSSTRING, val);					ifcengine.sdaiAppend (aggr, ifcengine.sdaiEXPRESSSTRING, val);
		///
		///	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
		///							sdaiAppend (aggr, sdaiINSTANCE, val);						ifcengine.sdaiAppend (aggr, ifcengine.sdaiINSTANCE, val);
		///
		///	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
		///							sdaiAppend (val, sdaiINSTANCE, inst);						ifcengine.sdaiAppend (val, ifcengine.sdaiINSTANCE, inst);
		///							sdaiAppend (aggr, sdaiAGGR, val);							ifcengine.sdaiAppend (aggr, ifcengine.sdaiAGGR, val);
		///
		///	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
		///							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
		///							sdaiPutADBTypePath (val, 1, "INTEGER");						ifcengine.sdaiPutADBTypePath (val, 1, "INTEGER");
		///							sdaiAppend (aggr, sdaiADB, val);							ifcengine.sdaiAppend (aggr, ifcengine.sdaiADB, val);	
		///							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
		///
		///	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
		///	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t
		///	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table
		///
		///
		///	Table 2 - valueType can be requested depending on actual model data.
		///
		///	valueType		Works for following values in the model
		///				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
		///	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
		///	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
		///	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
		///	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
		///	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
		///	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
		///	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t aggregate, int_t valueType, ref bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t aggregate, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t aggregate, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t aggregate, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t aggregate, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t aggregate, int_t valueType, byte[] value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAppend")]
		public static extern void sdaiAppend(int_t aggregate, int_t valueType, string value);

		/// <summary>
		///		sdaiAdd                                                 (http://rdf.bg/ifcdoc/CS64/sdaiAdd.html)
		///
		///	valueType argument to specify what type of data caller wants to put
		///	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiAdd, and it works similarly for all put-functions.
		///	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
		///		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
		///
		///
		///	Table 1 – Required value buffer depending on valueType (on the example of sdaiAdd but valid for all put-functions)
		///
		///	valueType				C/C++														C#
		///
		///	sdaiINTEGER				int_t val = 123;											int_t val = 123;
		///							sdaiAdd (aggr, sdaiINTEGER, &val);							ifcengine.sdaiAdd (aggr, ifcengine.sdaiINTEGER, ref val);
		///
		///	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
		///							sdaiAdd (aggr, sdaiREAL, &val);								ifcengine.sdaiAdd (aggr, ifcengine.sdaiREAL, ref val);
		///
		///	sdaiBOOLEAN				bool val = true;											bool val = true;
		///							sdaiAdd (aggr, sdaiBOOLEAN, &val);							ifcengine.sdaiAdd (aggr, ifcengine.sdaiBOOLEAN, ref val);
		///
		///	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
		///							sdaiAdd (aggr, sdaiLOGICAL, val);							ifcengine.sdaiAdd (aggr, ifcengine.sdaiLOGICAL, val);
		///
		///	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
		///							sdaiAdd (aggr, sdaiENUM, val);								ifcengine.sdaiAdd (aggr, ifcengine.sdaiENUM, val);
		///
		///	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
		///							sdaiAdd (aggr, sdaiBINARY, val);							ifcengine.sdaiAdd (aggr, ifcengine.sdaiBINARY, val);
		///
		///	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
		///							sdaiAdd (aggr, sdaiSTRING, val);							ifcengine.sdaiAdd (aggr, ifcengine.sdaiSTRING, val);
		///
		///	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
		///							sdaiAdd (aggr, sdaiUNICODE, val);							ifcengine.sdaiAdd (aggr, ifcengine.sdaiSTRING, val);
		///
		///	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
		///							sdaiAdd (aggr, sdaiEXPRESSSTRING, val);						ifcengine.sdaiAdd (aggr, ifcengine.sdaiEXPRESSSTRING, val);
		///
		///	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
		///							sdaiAdd (aggr, sdaiINSTANCE, val);							ifcengine.sdaiAdd (aggr, ifcengine.sdaiINSTANCE, val);
		///
		///	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
		///							sdaiAppend (val, sdaiINSTANCE, inst);						ifcengine.sdaiAppend (val, ifcengine.sdaiINSTANCE, inst);
		///							sdaiAdd (aggr, sdaiAGGR, val);								ifcengine.sdaiAdd (aggr, ifcengine.sdaiAGGR, val);
		///
		///	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
		///							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
		///							sdaiPutADBTypePath (val, 1, "INTEGER");						ifcengine.sdaiPutADBTypePath (val, 1, "INTEGER");
		///							sdaiAdd (aggr, sdaiADB, val);								ifcengine.sdaiAdd (aggr, ifcengine.sdaiADB, val);	
		///							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
		///
		///	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
		///	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t
		///	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table
		///
		///
		///	Table 2 - valueType can be requested depending on actual model data.
		///
		///	valueType		Works for following values in the model
		///				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
		///	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
		///	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
		///	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
		///	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
		///	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
		///	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
		///	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAdd")]
		public static extern void sdaiAdd(int_t aggregate, int_t valueType, ref bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAdd")]
		public static extern void sdaiAdd(int_t aggregate, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAdd")]
		public static extern void sdaiAdd(int_t aggregate, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAdd")]
		public static extern void sdaiAdd(int_t aggregate, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAdd")]
		public static extern void sdaiAdd(int_t aggregate, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAdd")]
		public static extern void sdaiAdd(int_t aggregate, int_t valueType, byte[] value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiAdd")]
		public static extern void sdaiAdd(int_t aggregate, int_t valueType, string value);

		/// <summary>
		///		sdaiInsertByIndex                                       (http://rdf.bg/ifcdoc/CS64/sdaiInsertByIndex.html)
		///
		///	valueType argument to specify what type of data caller wants to put
		///	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiInsertByIndex, and it works similarly for all put-functions.
		///	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
		///		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
		///
		///
		///	Table 1 – Required value buffer depending on valueType (on the example of sdaiInsertByIndex but valid for all put-functions)
		///
		///	valueType				C/C++														C#
		///
		///	sdaiINTEGER				int_t val = 123;											int_t val = 123;
		///							sdaiInsertByIndex (aggr, sdaiINTEGER, &val);				ifcengine.sdaiInsertByIndex (aggr, ifcengine.sdaiINTEGER, ref val);
		///
		///	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
		///							sdaiInsertByIndex (aggr, sdaiREAL, &val);					ifcengine.sdaiInsertByIndex (aggr, ifcengine.sdaiREAL, ref val);
		///
		///	sdaiBOOLEAN				bool val = true;											bool val = true;
		///							sdaiInsertByIndex (aggr, sdaiBOOLEAN, &val);				ifcengine.sdaiInsertByIndex (aggr, ifcengine.sdaiBOOLEAN, ref val);
		///
		///	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
		///							sdaiInsertByIndex (aggr, sdaiLOGICAL, val);					ifcengine.sdaiInsertByIndex (aggr, ifcengine.sdaiLOGICAL, val);
		///
		///	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
		///							sdaiInsertByIndex (aggr, sdaiENUM, val);					ifcengine.sdaiInsertByIndex (aggr, ifcengine.sdaiENUM, val);
		///
		///	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
		///							sdaiInsertByIndex (aggr, sdaiBINARY, val);					ifcengine.sdaiInsertByIndex (aggr, ifcengine.sdaiBINARY, val);
		///
		///	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
		///							sdaiInsertByIndex (aggr, sdaiSTRING, val);					ifcengine.sdaiInsertByIndex (aggr, ifcengine.sdaiSTRING, val);
		///
		///	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
		///							sdaiInsertByIndex (aggr, sdaiUNICODE, val);					ifcengine.sdaiInsertByIndex (aggr, ifcengine.sdaiSTRING, val);
		///
		///	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
		///							sdaiInsertByIndex (aggr, sdaiEXPRESSSTRING, val);			ifcengine.sdaiInsertByIndex (aggr, ifcengine.sdaiEXPRESSSTRING, val);
		///
		///	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
		///							sdaiInsertByIndex (aggr, sdaiINSTANCE, val);				ifcengine.sdaiInsertByIndex (aggr, ifcengine.sdaiINSTANCE, val);
		///
		///	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
		///							sdaiAppend (val, sdaiINSTANCE, inst);						ifcengine.sdaiAppend (val, ifcengine.sdaiINSTANCE, inst);
		///							sdaiInsertByIndex (aggr, sdaiAGGR, val);					ifcengine.sdaiInsertByIndex (aggr, ifcengine.sdaiAGGR, val);
		///
		///	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
		///							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
		///							sdaiPutADBTypePath (val, 1, "INTEGER");						ifcengine.sdaiPutADBTypePath (val, 1, "INTEGER");
		///							sdaiInsertByIndex (aggr, sdaiADB, val);						ifcengine.sdaiInsertByIndex (aggr, ifcengine.sdaiADB, val);	
		///							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
		///
		///	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
		///	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t
		///	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table
		///
		///
		///	Table 2 - valueType can be requested depending on actual model data.
		///
		///	valueType		Works for following values in the model
		///				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
		///	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
		///	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
		///	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
		///	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
		///	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
		///	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
		///	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiInsertByIndex")]
		public static extern void sdaiInsertByIndex(int_t aggregate, int_t index, int_t valueType, ref bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiInsertByIndex")]
		public static extern void sdaiInsertByIndex(int_t aggregate, int_t index, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiInsertByIndex")]
		public static extern void sdaiInsertByIndex(int_t aggregate, int_t index, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiInsertByIndex")]
		public static extern void sdaiInsertByIndex(int_t aggregate, int_t index, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiInsertByIndex")]
		public static extern void sdaiInsertByIndex(int_t aggregate, int_t index, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiInsertByIndex")]
		public static extern void sdaiInsertByIndex(int_t aggregate, int_t index, int_t valueType, byte[] value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiInsertByIndex")]
		public static extern void sdaiInsertByIndex(int_t aggregate, int_t index, int_t valueType, string value);

		/// <summary>
		///		sdaiCreateADB                                           (http://rdf.bg/ifcdoc/CS64/sdaiCreateADB.html)
		///
		///	valueType argument to specify what type of data caller wants to put
		///	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiCreateADB, and it works similarly for all put-functions.
		///	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
		///		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
		///
		///
		///	Table 1 – Required value buffer depending on valueType (on the example of sdaiCreateADB but valid for all put-functions)
		///
		///	valueType				C/C++														C#
		///
		///	sdaiINTEGER				int_t val = 123;											int_t val = 123;
		///							SdaiADB adb = sdaiCreateADB (sdaiINTEGER, &val);			int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref val);
		///
		///	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
		///							SdaiADB adb = sdaiCreateADB (sdaiREAL, &val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiREAL, ref val);
		///
		///	sdaiBOOLEAN				bool val = true;											bool val = true;
		///							SdaiADB adb = sdaiCreateADB (sdaiBOOLEAN, &val);			int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiBOOLEAN, ref val);
		///
		///	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
		///							SdaiADB adb = sdaiCreateADB (sdaiLOGICAL, val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiLOGICAL, val);
		///
		///	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
		///							SdaiADB adb = sdaiCreateADB (sdaiENUM, val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiENUM, val);
		///
		///	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
		///							SdaiADB adb = sdaiCreateADB (sdaiBINARY, val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiBINARY, val);
		///
		///	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
		///							SdaiADB adb = sdaiCreateADB (sdaiSTRING, val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiSTRING, val);
		///
		///	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
		///							SdaiADB adb = sdaiCreateADB (sdaiUNICODE, val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiSTRING, val);
		///
		///	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
		///							SdaiADB adb = sdaiCreateADB (sdaiEXPRESSSTRING, val);		int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiEXPRESSSTRING, val);
		///
		///	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
		///							SdaiADB adb = sdaiCreateADB (sdaiINSTANCE, val);			int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiINSTANCE, val);
		///
		///	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
		///							sdaiAppend (val, sdaiINSTANCE, inst);						ifcengine.sdaiAppend (val, ifcengine.sdaiINSTANCE, inst);
		///							SdaiADB adb = sdaiCreateADB (sdaiAGGR, val);				int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiAGGR, val);
		///
		///	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
		///							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
		///							sdaiPutADBTypePath (val, 1, "INTEGER");						ifcengine.sdaiPutADBTypePath (val, 1, "INTEGER");
		///							SdaiADB adb = sdaiCreateADB (sdaiADB, val);					int_t adb = ifcengine.sdaiCreateADB (ifcengine.sdaiADB, val);	
		///							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
		///
		///	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
		///	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t
		///	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table
		///
		///
		///	Table 2 - valueType can be requested depending on actual model data.
		///
		///	valueType		Works for following values in the model
		///				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
		///	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
		///	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
		///	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
		///	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
		///	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
		///	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
		///	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, ref bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, byte[] value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateADB")]
		public static extern int_t sdaiCreateADB(int_t valueType, string value);

		/// <summary>
		///		sdaiCreateAggr                                          (http://rdf.bg/ifcdoc/CS64/sdaiCreateAggr.html)
		///
		///	This call creates an aggregation.
		///	The instance has to be present,
		///	the attribute argument can be empty (0) in case the aggregation is an nested aggregation for this specific instance,
		///	prefered use would be use of sdaiCreateNestedAggr in such a case.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateAggr")]
		public static extern int_t sdaiCreateAggr(int_t instance, int_t attribute);

		/// <summary>
		///		sdaiCreateAggrBN                                        (http://rdf.bg/ifcdoc/CS64/sdaiCreateAggrBN.html)
		///
		///	This call creates an aggregation.
		///	The instance has to be present,
		///	the attributeName argument can be NULL (0) in case the aggregation is an nested aggregation for this specific instance,
		///	prefered use would be use of sdaiCreateNestedAggr in such a case.
		///
		///	Technically sdaiCreateAggrBN will transform into the following call
		///		(attributeName) ?
		///			sdaiCreateAggr(
		///					instance,
		///					sdaiGetAttrDefinition(
		///							sdaiGetInstanceType(
		///									instance
		///								),
		///							attributeName
		///						)
		///				) :
		///			sdaiCreateAggr(
		///					instance,
		///					nullptr
		///				);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateAggrBN")]
		public static extern int_t sdaiCreateAggrBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateAggrBN")]
		public static extern int_t sdaiCreateAggrBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		sdaiCreateNPL                                           (http://rdf.bg/ifcdoc/CS64/sdaiCreateNPL.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateNPL")]
		public static extern int_t sdaiCreateNPL();

		/// <summary>
		///		sdaiDeleteNPL                                           (http://rdf.bg/ifcdoc/CS64/sdaiDeleteNPL.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiDeleteNPL")]
		public static extern void sdaiDeleteNPL(int_t list);

		/// <summary>
		///		sdaiCreateNestedAggr                                    (http://rdf.bg/ifcdoc/CS64/sdaiCreateNestedAggr.html)
		///
		///	This call creates an aggregation within an aggregation.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateNestedAggr")]
		public static extern int_t sdaiCreateNestedAggr(int_t aggregate);

		/// <summary>
		///		sdaiCreateInstance                                      (http://rdf.bg/ifcdoc/CS64/sdaiCreateInstance.html)
		///
		///	This call creates an instance of the given entity
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateInstance")]
		public static extern int_t sdaiCreateInstance(int_t model, int_t entity);

		/// <summary>
		///		sdaiCreateInstanceBN                                    (http://rdf.bg/ifcdoc/CS64/sdaiCreateInstanceBN.html)
		///
		///	This call creates an instance of the given entity.
		///
		///	Technically it will transform into the following call
		///		sdaiCreateInstance(
		///				model,
		///				sdaiGetEntity(
		///						model,
		///						entityName
		///					)
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateInstanceBN")]
		public static extern int_t sdaiCreateInstanceBN(int_t model, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateInstanceBN")]
		public static extern int_t sdaiCreateInstanceBN(int_t model, byte[] entityName);

		/// <summary>
		///		sdaiDeleteInstance                                      (http://rdf.bg/ifcdoc/CS64/sdaiDeleteInstance.html)
		///
		///	This call will delete an existing instance
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiDeleteInstance")]
		public static extern void sdaiDeleteInstance(int_t instance);

		/// <summary>
		///		sdaiPutADBTypePath                                      (http://rdf.bg/ifcdoc/CS64/sdaiPutADBTypePath.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutADBTypePath")]
		public static extern void sdaiPutADBTypePath(int_t ADB, int_t pathCount, string path);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutADBTypePath")]
		public static extern void sdaiPutADBTypePath(int_t ADB, int_t pathCount, byte[] path);

		/// <summary>
		///		sdaiPutAttr                                             (http://rdf.bg/ifcdoc/CS64/sdaiPutAttr.html)
		///
		///	valueType argument to specify what type of data caller wants to put
		///	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiPutAttr, and it works similarly for all put-functions.
		///	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
		///		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
		///
		///
		///	Table 1 – Required value buffer depending on valueType (on the example of sdaiPutAttr but valid for all put-functions)
		///
		///	valueType				C/C++														C#
		///
		///	sdaiINTEGER				int_t val = 123;											int_t val = 123;
		///							sdaiPutAttr (inst, attr, sdaiINTEGER, &val);				ifcengine.sdaiPutAttr (inst, attr, ifcengine.sdaiINTEGER, ref val);
		///
		///	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
		///							sdaiPutAttr (inst, attr, sdaiREAL, &val);					ifcengine.sdaiPutAttr (inst, attr, ifcengine.sdaiREAL, ref val);
		///
		///	sdaiBOOLEAN				bool val = true;											bool val = true;
		///							sdaiPutAttr (inst, attr, sdaiBOOLEAN, &val);				ifcengine.sdaiPutAttr (inst, attr, ifcengine.sdaiBOOLEAN, ref val);
		///
		///	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
		///							sdaiPutAttr (inst, attr, sdaiLOGICAL, val);					ifcengine.sdaiPutAttr (inst, attr, ifcengine.sdaiLOGICAL, val);
		///
		///	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
		///							sdaiPutAttr (inst, attr, sdaiENUM, val);					ifcengine.sdaiPutAttr (inst, attr, ifcengine.sdaiENUM, val);
		///
		///	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
		///							sdaiPutAttr (inst, attr, sdaiBINARY, val);					ifcengine.sdaiPutAttr (inst, attr, ifcengine.sdaiBINARY, val);
		///
		///	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
		///							sdaiPutAttr (inst, attr, sdaiSTRING, val);					ifcengine.sdaiPutAttr (inst, attr, ifcengine.sdaiSTRING, val);
		///
		///	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
		///							sdaiPutAttr (inst, attr, sdaiUNICODE, val);					ifcengine.sdaiPutAttr (inst, attr, ifcengine.sdaiSTRING, val);
		///
		///	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
		///							sdaiPutAttr (inst, attr, sdaiEXPRESSSTRING, val);			ifcengine.sdaiPutAttr (inst, attr, ifcengine.sdaiEXPRESSSTRING, val);
		///
		///	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
		///							sdaiPutAttr (inst, attr, sdaiINSTANCE, val);				ifcengine.sdaiPutAttr (inst, attr, ifcengine.sdaiINSTANCE, val);
		///
		///	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
		///							sdaiAppend (val, sdaiINSTANCE, inst);						ifcengine.sdaiAppend (val, ifcengine.sdaiINSTANCE, inst);
		///							sdaiPutAttr (inst, attr, sdaiAGGR, val);					ifcengine.sdaiPutAttr (inst, attr, ifcengine.sdaiAGGR, val);
		///
		///	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
		///							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
		///							sdaiPutADBTypePath (val, 1, "INTEGER");						ifcengine.sdaiPutADBTypePath (val, 1, "INTEGER");
		///							sdaiPutAttr (inst, attr, sdaiADB, val);						ifcengine.sdaiPutAttr (inst, attr, ifcengine.sdaiADB, val);	
		///							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
		///
		///	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
		///	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t
		///	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table
		///
		///
		///	Table 2 - valueType can be requested depending on actual model data.
		///
		///	valueType		Works for following values in the model
		///				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
		///	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
		///	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
		///	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
		///	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
		///	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
		///	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
		///	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttr")]
		public static extern void sdaiPutAttr(int_t instance, int_t attribute, int_t valueType, ref bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttr")]
		public static extern void sdaiPutAttr(int_t instance, int_t attribute, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttr")]
		public static extern void sdaiPutAttr(int_t instance, int_t attribute, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttr")]
		public static extern void sdaiPutAttr(int_t instance, int_t attribute, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttr")]
		public static extern void sdaiPutAttr(int_t instance, int_t attribute, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttr")]
		public static extern void sdaiPutAttr(int_t instance, int_t attribute, int_t valueType, byte[] value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttr")]
		public static extern void sdaiPutAttr(int_t instance, int_t attribute, int_t valueType, string value);

		/// <summary>
		///		sdaiPutAttrBN                                           (http://rdf.bg/ifcdoc/CS64/sdaiPutAttrBN.html)
		///
		///	valueType argument to specify what type of data caller wants to put
		///	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiPutAttrBN, and it works similarly for all put-functions.
		///	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
		///		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
		///
		///
		///	Table 1 – Required value buffer depending on valueType (on the example of sdaiPutAttrBN but valid for all put-functions)
		///
		///	valueType				C/C++														C#
		///
		///	sdaiINTEGER				int_t val = 123;											int_t val = 123;
		///							sdaiPutAttrBN (inst, "attrName", sdaiINTEGER, &val);		ifcengine.sdaiPutAttrBN (inst, "attrName", ifcengine.sdaiINTEGER, ref val);
		///
		///	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
		///							sdaiPutAttrBN (inst, "attrName", sdaiREAL, &val);			ifcengine.sdaiPutAttrBN (inst, "attrName", ifcengine.sdaiREAL, ref val);
		///
		///	sdaiBOOLEAN				bool val = true;											bool val = true;
		///							sdaiPutAttrBN (inst, "attrName", sdaiBOOLEAN, &val);		ifcengine.sdaiPutAttrBN (inst, "attrName", ifcengine.sdaiBOOLEAN, ref val);
		///
		///	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
		///							sdaiPutAttrBN (inst, "attrName", sdaiLOGICAL, val);			ifcengine.sdaiPutAttrBN (inst, "attrName", ifcengine.sdaiLOGICAL, val);
		///
		///	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
		///							sdaiPutAttrBN (inst, "attrName", sdaiENUM, val);			ifcengine.sdaiPutAttrBN (inst, "attrName", ifcengine.sdaiENUM, val);
		///
		///	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
		///							sdaiPutAttrBN (inst, "attrName", sdaiBINARY, val);			ifcengine.sdaiPutAttrBN (inst, "attrName", ifcengine.sdaiBINARY, val);
		///
		///	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
		///							sdaiPutAttrBN (inst, "attrName", sdaiSTRING, val);			ifcengine.sdaiPutAttrBN (inst, "attrName", ifcengine.sdaiSTRING, val);
		///
		///	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
		///							sdaiPutAttrBN (inst, "attrName", sdaiUNICODE, val);			ifcengine.sdaiPutAttrBN (inst, "attrName", ifcengine.sdaiSTRING, val);
		///
		///	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
		///							sdaiPutAttrBN (inst, "attrName", sdaiEXPRESSSTRING, val);	ifcengine.sdaiPutAttrBN (inst, "attrName", ifcengine.sdaiEXPRESSSTRING, val);
		///
		///	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
		///							sdaiPutAttrBN (inst, "attrName", sdaiINSTANCE, val);		ifcengine.sdaiPutAttrBN (inst, "attrName", ifcengine.sdaiINSTANCE, val);
		///
		///	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
		///							sdaiAppend (val, sdaiINSTANCE, inst);						ifcengine.sdaiAppend (val, ifcengine.sdaiINSTANCE, inst);
		///							sdaiPutAttrBN (inst, "attrName", sdaiAGGR, val);			ifcengine.sdaiPutAttrBN (inst, "attrName", ifcengine.sdaiAGGR, val);
		///
		///	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
		///							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
		///							sdaiPutADBTypePath (val, 1, "INTEGER");						ifcengine.sdaiPutADBTypePath (val, 1, "INTEGER");
		///							sdaiPutAttrBN (inst, "attrName", sdaiADB, val);				ifcengine.sdaiPutAttrBN (inst, "attrName", ifcengine.sdaiADB, val);	
		///							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
		///
		///	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
		///	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t
		///	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table
		///
		///
		///	Table 2 - valueType can be requested depending on actual model data.
		///
		///	valueType		Works for following values in the model
		///				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
		///	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
		///	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
		///	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
		///	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
		///	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
		///	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
		///	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
		///
		///	Technically sdaiPutAttrBN will transform into the following call
		///		sdaiPutAttr(
		///				instance,
		///				sdaiGetAttrDefinition(
		///						sdaiGetInstanceType(
		///								instance
		///							),
		///						attributeName
		///					),
		///				valueType,
		///				value
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, ref bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, byte[] value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, string attributeName, int_t valueType, string value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, ref bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, byte[] value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAttrBN")]
		public static extern void sdaiPutAttrBN(int_t instance, byte[] attributeName, int_t valueType, string value);

		/// <summary>
		///		sdaiUnsetAttr                                           (http://rdf.bg/ifcdoc/CS64/sdaiUnsetAttr.html)
		///
		///	This call removes all data from a specific attribute for the given instance
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiUnsetAttr")]
		public static extern void sdaiUnsetAttr(int_t instance, int_t attribute);

		/// <summary>
		///		sdaiUnsetAttrBN                                         (http://rdf.bg/ifcdoc/CS64/sdaiUnsetAttrBN.html)
		///
		///	This call removes all data from a specific attribute for the given instance
		///
		///	Technically it will transform into the following call
		///		sdaiUnsetAttr(
		///				instance,
		///				sdaiGetAttrDefinition(
		///						sdaiGetInstanceType(
		///								instance
		///							),
		///						attributeName
		///					)
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiUnsetAttrBN")]
		public static extern void sdaiUnsetAttrBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiUnsetAttrBN")]
		public static extern void sdaiUnsetAttrBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		engiSetComment                                          (http://rdf.bg/ifcdoc/CS64/engiSetComment.html)
		///
		///	This call can be used to add a comment to an instance when exporting the content. The comment is available in the exported/saved IFC file.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiSetComment")]
		public static extern void engiSetComment(int_t instance, string comment);

		[DllImport(IFCEngineDLL, EntryPoint = "engiSetComment")]
		public static extern void engiSetComment(int_t instance, byte[] comment);

		/// <summary>
		///		engiGetInstanceLocalId                                  (http://rdf.bg/ifcdoc/CS64/engiGetInstanceLocalId.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetInstanceLocalId")]
		public static extern Int64 engiGetInstanceLocalId(int_t instance);

		/// <summary>
		///		sdaiTestAttr                                            (http://rdf.bg/ifcdoc/CS64/sdaiTestAttr.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiTestAttr")]
		public static extern int_t sdaiTestAttr(int_t instance, int_t attribute);

		/// <summary>
		///		sdaiTestAttrBN                                          (http://rdf.bg/ifcdoc/CS64/sdaiTestAttrBN.html)
		///
		///	Technically it will transform into the following call
		///		sdaiGetAttrDefinition(
		///				sdaiGetInstanceType(
		///						instance
		///					),
		///				attributeName
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiTestAttrBN")]
		public static extern int_t sdaiTestAttrBN(int_t instance, string attributeName);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiTestAttrBN")]
		public static extern int_t sdaiTestAttrBN(int_t instance, byte[] attributeName);

		/// <summary>
		///		sdaiCreateInstanceEI                                    (http://rdf.bg/ifcdoc/CS64/sdaiCreateInstanceEI.html)
		///
		///	This call creates an instance at a specific given express ID, the instance is only created if the express ID was not used yet
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateInstanceEI")]
		public static extern int_t sdaiCreateInstanceEI(int_t model, int_t entity, Int64 expressID);

		/// <summary>
		///		sdaiCreateInstanceBNEI                                  (http://rdf.bg/ifcdoc/CS64/sdaiCreateInstanceBNEI.html)
		///
		///	This call creates an instance at a specific given express ID, the instance is only created if the express ID was not used yet
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateInstanceBNEI")]
		public static extern int_t sdaiCreateInstanceBNEI(int_t model, string entityName, Int64 expressID);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateInstanceBNEI")]
		public static extern int_t sdaiCreateInstanceBNEI(int_t model, byte[] entityName, Int64 expressID);

		/// <summary>
		///		setSegmentation                                         (http://rdf.bg/ifcdoc/CS64/setSegmentation.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setSegmentation")]
		public static extern void setSegmentation(int_t model, int_t segmentationParts, double segmentationLength);

		/// <summary>
		///		getSegmentation                                         (http://rdf.bg/ifcdoc/CS64/getSegmentation.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getSegmentation")]
		public static extern void getSegmentation(int_t model, out int_t segmentationParts, out double segmentationLength);

		/// <summary>
		///		setEpsilon                                              (http://rdf.bg/ifcdoc/CS64/setEpsilon.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setEpsilon")]
		public static extern void setEpsilon(int_t model, int_t mask, double absoluteEpsilon, double relativeEpsilon);

		/// <summary>
		///		getEpsilon                                              (http://rdf.bg/ifcdoc/CS64/getEpsilon.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getEpsilon")]
		public static extern int_t getEpsilon(int_t model, int_t mask, out double absoluteEpsilon, out double relativeEpsilon);

        //
        //  Controling API Calls
        //

		/// <summary>
		///		circleSegments                                          (http://rdf.bg/ifcdoc/CS64/circleSegments.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "circleSegments")]
		public static extern void circleSegments(int_t circles, int_t smallCircles);

		/// <summary>
		///		setMaximumSegmentationLength                            (http://rdf.bg/ifcdoc/CS64/setMaximumSegmentationLength.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setMaximumSegmentationLength")]
		public static extern void setMaximumSegmentationLength(int_t model, double length);

		/// <summary>
		///		getUnitConversionFactor                                 (http://rdf.bg/ifcdoc/CS64/getUnitConversionFactor.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getUnitConversionFactor")]
		public static extern double getUnitConversionFactor(int_t model, string unitType, out IntPtr unitPrefix, out IntPtr unitName, out IntPtr SIUnitName);

		[DllImport(IFCEngineDLL, EntryPoint = "getUnitConversionFactor")]
		public static extern double getUnitConversionFactor(int_t model, byte[] unitType, out IntPtr unitPrefix, out IntPtr unitName, out IntPtr SIUnitName);

		/// <summary>
		///		setBRepProperties                                       (http://rdf.bg/ifcdoc/CS64/setBRepProperties.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setBRepProperties")]
		public static extern void setBRepProperties(int_t model, Int64 consistencyCheck, double fraction, double epsilon, int_t maxVerticesSize);

		/// <summary>
		///		cleanMemory                                             (http://rdf.bg/ifcdoc/CS64/cleanMemory.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "cleanMemory")]
		public static extern void cleanMemory(int_t model, int_t mode);

		/// <summary>
		///		internalGetP21Line                                      (http://rdf.bg/ifcdoc/CS64/internalGetP21Line.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalGetP21Line")]
		public static extern Int64 internalGetP21Line(int_t instance);

		/// <summary>
		///		internalForceInstanceFromP21Line                        (http://rdf.bg/ifcdoc/CS64/internalForceInstanceFromP21Line.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalForceInstanceFromP21Line")]
		public static extern int_t internalForceInstanceFromP21Line(int_t model, Int64 P21Line);

		/// <summary>
		///		internalGetInstanceFromP21Line                          (http://rdf.bg/ifcdoc/CS64/internalGetInstanceFromP21Line.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalGetInstanceFromP21Line")]
		public static extern int_t internalGetInstanceFromP21Line(int_t model, Int64 P21Line);

		/// <summary>
		///		internalGetXMLID                                        (http://rdf.bg/ifcdoc/CS64/internalGetXMLID.html)
		///
		///	In case an XML file is loaded the XML ID values are kept in memory and can be retrieved through this API call
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalGetXMLID")]
		public static extern IntPtr internalGetXMLID(int_t instance, out IntPtr XMLID);

		public static string internalGetXMLID(int_t instance)
		{
			IntPtr XMLID = IntPtr.Zero;
			internalGetXMLID(instance, out XMLID);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(XMLID);
		}

		/// <summary>
		///		setStringUnicode                                        (http://rdf.bg/ifcdoc/CS64/setStringUnicode.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setStringUnicode")]
		public static extern int_t setStringUnicode(int_t unicode);

		/// <summary>
		///		getStringUnicode                                        (http://rdf.bg/ifcdoc/CS64/getStringUnicode.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getStringUnicode")]
		public static extern int_t getStringUnicode();

		/// <summary>
		///		setFilter                                               (http://rdf.bg/ifcdoc/CS64/setFilter.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setFilter")]
		public static extern void setFilter(int_t model, int_t setting, int_t mask);

		/// <summary>
		///		getFilter                                               (http://rdf.bg/ifcdoc/CS64/getFilter.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getFilter")]
		public static extern int_t getFilter(int_t model, int_t mask);

        //
        //  Uncategorized API Calls
        //

		/// <summary>
		///		xxxxGetEntityAndSubTypesExtent                          (http://rdf.bg/ifcdoc/CS64/xxxxGetEntityAndSubTypesExtent.html)
		///
		///	model input parameter is irrelevant, but is required for backwards compatibility
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetEntityAndSubTypesExtent")]
		public static extern int_t xxxxGetEntityAndSubTypesExtent(int_t model, int_t entity);

		/// <summary>
		///		xxxxGetEntityAndSubTypesExtentBN                        (http://rdf.bg/ifcdoc/CS64/xxxxGetEntityAndSubTypesExtentBN.html)
		///
		///	Technically xxxxGetEntityAndSubTypesExtentBN will transform into the following call
		///		xxxxGetEntityAndSubTypesExtent(
		///				model,
		///				sdaiGetEntity(
		///						model,
		///						entityName
		///					)
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetEntityAndSubTypesExtentBN")]
		public static extern int_t xxxxGetEntityAndSubTypesExtentBN(int_t model, string entityName);

		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetEntityAndSubTypesExtentBN")]
		public static extern int_t xxxxGetEntityAndSubTypesExtentBN(int_t model, byte[] entityName);

		/// <summary>
		///		xxxxGetAllInstances                                     (http://rdf.bg/ifcdoc/CS64/xxxxGetAllInstances.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetAllInstances")]
		public static extern int_t xxxxGetAllInstances(int_t model);

		/// <summary>
		///		xxxxGetInstancesUsing                                   (http://rdf.bg/ifcdoc/CS64/xxxxGetInstancesUsing.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetInstancesUsing")]
		public static extern int_t xxxxGetInstancesUsing(int_t instance);

		/// <summary>
		///		xxxxDeleteFromAggregation                               (http://rdf.bg/ifcdoc/CS64/xxxxDeleteFromAggregation.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxDeleteFromAggregation")]
		public static extern int_t xxxxDeleteFromAggregation(int_t instance, int_t aggregate, int_t elementIndex);

		/// <summary>
		///		xxxxGetAttrDefinitionByValue                            (http://rdf.bg/ifcdoc/CS64/xxxxGetAttrDefinitionByValue.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetAttrDefinitionByValue")]
		public static extern int_t xxxxGetAttrDefinitionByValue(int_t instance, out IntPtr value);

		/// <summary>
		///		xxxxGetAttrNameByIndex                                  (http://rdf.bg/ifcdoc/CS64/xxxxGetAttrNameByIndex.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetAttrNameByIndex")]
		public static extern IntPtr xxxxGetAttrNameByIndex(int_t instance, int_t index, out IntPtr name);

		public static string xxxxGetAttrNameByIndex(int_t instance, int_t index)
		{
			IntPtr name = IntPtr.Zero;
			xxxxGetAttrNameByIndex(instance, index, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		iterateOverInstances                                    (http://rdf.bg/ifcdoc/CS64/iterateOverInstances.html)
		///
		///	This function interates over all available instances loaded in memory, it is the fastest way to find all instances.
		///	Argument entity and entityName are both optional and if non-zero are filled with respectively the entity handle and entity name as char array.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "iterateOverInstances")]
		public static extern int_t iterateOverInstances(int_t model, int_t instance, out int_t entity, out IntPtr entityName);

		/// <summary>
		///		iterateOverProperties                                   (http://rdf.bg/ifcdoc/CS64/iterateOverProperties.html)
		///
		///	This function iterated over all available attributes of a specific given entity.
		///	This call is typically used in combination with iterateOverInstances(..).
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "iterateOverProperties")]
		public static extern int_t iterateOverProperties(int_t entity, int_t index);

		/// <summary>
		///		sdaiGetAggrByIterator                                   (http://rdf.bg/ifcdoc/CS64/sdaiGetAggrByIterator.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIterator")]
		public static extern int_t sdaiGetAggrByIterator(int_t iterator, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIterator")]
		public static extern int_t sdaiGetAggrByIterator(int_t iterator, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIterator")]
		public static extern int_t sdaiGetAggrByIterator(int_t iterator, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiGetAggrByIterator")]
		public static extern int_t sdaiGetAggrByIterator(int_t iterator, int_t valueType, out IntPtr value);

		public static int_t sdaiGetAggrByIterator(int_t iterator, int_t valueType, out string value)
		{
			value = null;
			valueType = getStringType(valueType);
			if (valueType != 0)
			{
				IntPtr ptr = IntPtr.Zero;
				var ret = sdaiGetAggrByIterator(iterator, valueType, out ptr);
				if (ret != 0 && ptr != IntPtr.Zero)
				{
					value = marshalPtrToString(valueType, ptr);
					return ret;
				}
			}
			return 0;
		}

		/// <summary>
		///		sdaiPutAggrByIterator                                   (http://rdf.bg/ifcdoc/CS64/sdaiPutAggrByIterator.html)
		///
		///	valueType argument to specify what type of data caller wants to put
		///	Table 1 shows type of buffer the caller should provide depending on the valueType for sdaiPutAggrByIterator, and it works similarly for all put-functions.
		///	Note: with SDAI API it is impossible to check buffer type at compilation or execution time and this is responsibility of a caller to ensure that
		///		  requested valueType is matching with the value argument, a mismatch will lead to unpredictable results.
		///
		///
		///	Table 1 – Required value buffer depending on valueType (on the example of sdaiPutAggrByIterator but valid for all put-functions)
		///
		///	valueType				C/C++														C#
		///
		///	sdaiINTEGER				int_t val = 123;											int_t val = 123;
		///							sdaiPutAggrByIterator (iter, sdaiINTEGER, &val);			ifcengine.sdaiPutAggrByIterator (iter, ifcengine.sdaiINTEGER, ref val);
		///
		///	sdaiREAL or sdaiNUMBER	double val = 123.456;										double val = 123.456;
		///							sdaiPutAggrByIterator (iter, sdaiREAL, &val);				ifcengine.sdaiPutAggrByIterator (iter, ifcengine.sdaiREAL, ref val);
		///
		///	sdaiBOOLEAN				bool val = true;											bool val = true;
		///							sdaiPutAggrByIterator (iter, sdaiBOOLEAN, &val);			ifcengine.sdaiPutAggrByIterator (iter, ifcengine.sdaiBOOLEAN, ref val);
		///
		///	sdaiLOGICAL				const TCHAR* val = "U";										string val = "U";
		///							sdaiPutAggrByIterator (iter, sdaiLOGICAL, val);				ifcengine.sdaiPutAggrByIterator (iter, ifcengine.sdaiLOGICAL, val);
		///
		///	sdaiENUM				const TCHAR* val = "NOTDEFINED";							string val = "NOTDEFINED";
		///							sdaiPutAggrByIterator (iter, sdaiENUM, val);				ifcengine.sdaiPutAggrByIterator (iter, ifcengine.sdaiENUM, val);
		///
		///	sdaiBINARY				const TCHAR* val = "0123456ABC";							string val = "0123456ABC";
		///							sdaiPutAggrByIterator (iter, sdaiBINARY, val);				ifcengine.sdaiPutAggrByIterator (iter, ifcengine.sdaiBINARY, val);
		///
		///	sdaiSTRING				const char* val = "My Simple String";						string val = "My Simple String";
		///							sdaiPutAggrByIterator (iter, sdaiSTRING, val);				ifcengine.sdaiPutAggrByIterator (iter, ifcengine.sdaiSTRING, val);
		///
		///	sdaiUNICODE				const wchar_t* val = L"Any Unicode String";					string val = "Any Unicode String";
		///							sdaiPutAggrByIterator (iter, sdaiUNICODE, val);				ifcengine.sdaiPutAggrByIterator (iter, ifcengine.sdaiSTRING, val);
		///
		///	sdaiEXPRESSSTRING		const char* val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";	string val = "EXPRESS format, i.e. \\X2\\00FC\\X0\\";
		///							sdaiPutAggrByIterator (iter, sdaiEXPRESSSTRING, val);		ifcengine.sdaiPutAggrByIterator (iter, ifcengine.sdaiEXPRESSSTRING, val);
		///
		///	sdaiINSTANCE			SdaiInstance val = sdaiCreateInstanceBN (model, "IFCSITE");	int_t val = ifcengine.sdaiCreateInstanceBN (model, "IFCSITE");
		///							sdaiPutAggrByIterator (iter, sdaiINSTANCE, val);			ifcengine.sdaiPutAggrByIterator (iter, ifcengine.sdaiINSTANCE, val);
		///
		///	sdaiAGGR				SdaiAggr val = sdaiCreateAggr (inst, 0);					int_t val = sdaiCreateAggr (inst, 0);
		///							sdaiAppend (val, sdaiINSTANCE, inst);						ifcengine.sdaiAppend (val, ifcengine.sdaiINSTANCE, inst);
		///							sdaiPutAggrByIterator (iter, sdaiAGGR, val);				ifcengine.sdaiPutAggrByIterator (iter, ifcengine.sdaiAGGR, val);
		///
		///	sdaiADB					int_t integerValue = 123;									int_t integerValue = 123;	
		///							SdaiADB val = sdaiCreateADB (sdaiINTEGER, &integerValue);	int_t val = ifcengine.sdaiCreateADB (ifcengine.sdaiINTEGER, ref integerValue);
		///							sdaiPutADBTypePath (val, 1, "INTEGER");						ifcengine.sdaiPutADBTypePath (val, 1, "INTEGER");
		///							sdaiPutAggrByIterator (iter, sdaiADB, val);					ifcengine.sdaiPutAggrByIterator (iter, ifcengine.sdaiADB, val);	
		///							sdaiDeleteADB (val);										ifcengine.sdaiDeleteADB (val);
		///
		///	TCHAR is “char” or “wchar_t” depending on setStringUnicode.
		///	(Non-standard behavior) sdaiLOGICAL behaves differently from ISO 10303-24-2001: it expects char* while standard declares int_t
		///	(Non-standard extension) sdiADB in C++ has an option to work without sdaiCreateEmptyADB and sdaiDeleteADB as shown in the table
		///
		///
		///	Table 2 - valueType can be requested depending on actual model data.
		///
		///	valueType		Works for following values in the model
		///				 	  integer	   real		.T. or .F.	   .U.		other enum	  binary	  string	 instance	   list		 $ (empty)
		///	sdaiINTEGER			Yes			 .			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiREAL			 .			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiNUMBER			 . 			Yes			 .			 .			 .			 .			 .			 .			 .			 .
		///	sdaiBOOLEAN			 .			 .			Yes			 .			 .			 .			 .			 .			 .			 .
		///	sdaiLOGICAL			 .			 .			Yes			Yes			 .			 .			 .			 .			 .			 .
		///	sdaiENUM			 .			 .			Yes			Yes			Yes			 .			 .			 .			 .			 .
		///	sdaiBINARY			 .			 .			 .			 .			 .			Yes			 .			 .			 .			 .
		///	sdaiSTRING			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiUNICODE			 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiEXPRESSSTRING	 .			 .			 .			 .			 .			 .			Yes			 .			 .			 .
		///	sdaiINSTANCE		 .			 .			 .			 .			 .			 .			 .			Yes			 .			 .
		///	sdaiAGGR			 .			 .			 .			 .			 .			 .			 .			 .			Yes			 .
		///	sdaiADB				Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			Yes			 .
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAggrByIterator")]
		public static extern void sdaiPutAggrByIterator(int_t iterator, int_t valueType, ref bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAggrByIterator")]
		public static extern void sdaiPutAggrByIterator(int_t iterator, int_t valueType, ref int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAggrByIterator")]
		public static extern void sdaiPutAggrByIterator(int_t iterator, int_t valueType, int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAggrByIterator")]
		public static extern void sdaiPutAggrByIterator(int_t iterator, int_t valueType, ref double value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAggrByIterator")]
		public static extern void sdaiPutAggrByIterator(int_t iterator, int_t valueType, ref IntPtr value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAggrByIterator")]
		public static extern void sdaiPutAggrByIterator(int_t iterator, int_t valueType, byte[] value);

		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPutAggrByIterator")]
		public static extern void sdaiPutAggrByIterator(int_t iterator, int_t valueType, string value);

		/// <summary>
		///		internalSetLink                                         (http://rdf.bg/ifcdoc/CS64/internalSetLink.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalSetLink")]
		public static extern void internalSetLink(int_t instance, string attributeName, int_t linked_id);

		[DllImport(IFCEngineDLL, EntryPoint = "internalSetLink")]
		public static extern void internalSetLink(int_t instance, byte[] attributeName, int_t linked_id);

		/// <summary>
		///		internalAddAggrLink                                     (http://rdf.bg/ifcdoc/CS64/internalAddAggrLink.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalAddAggrLink")]
		public static extern void internalAddAggrLink(int_t aggregate, int_t linked_id);

		/// <summary>
		///		engiGetNotReferedAggr                                   (http://rdf.bg/ifcdoc/CS64/engiGetNotReferedAggr.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetNotReferedAggr")]
		public static extern void engiGetNotReferedAggr(int_t model, out int_t value);

		/// <summary>
		///		engiGetAttributeAggr                                    (http://rdf.bg/ifcdoc/CS64/engiGetAttributeAggr.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAttributeAggr")]
		public static extern void engiGetAttributeAggr(int_t instance, out int_t value);

		/// <summary>
		///		engiGetAggrUnknownElement                               (http://rdf.bg/ifcdoc/CS64/engiGetAggrUnknownElement.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrUnknownElement")]
		public static extern void engiGetAggrUnknownElement(int_t aggregate, int_t elementIndex, out int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrUnknownElement")]
		public static extern void engiGetAggrUnknownElement(int_t aggregate, int_t elementIndex, out int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrUnknownElement")]
		public static extern void engiGetAggrUnknownElement(int_t aggregate, int_t elementIndex, out int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrUnknownElement")]
		public static extern void engiGetAggrUnknownElement(int_t aggregate, int_t elementIndex, out int_t valueType, out IntPtr value);


		/// <summary>
		///		sdaiErrorQuery                                          (http://rdf.bg/ifcdoc/CS64/sdaiErrorQuery.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiErrorQuery")]
		public static extern int_t sdaiErrorQuery();

        //
        //  Geometry Kernel related API Calls
        //

		/// <summary>
		///		owlGetModel                                             (http://rdf.bg/ifcdoc/CS64/owlGetModel.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlGetModel")]
		public static extern void owlGetModel(int_t model, out Int64 owlModel);

		/// <summary>
		///		owlGetInstance                                          (http://rdf.bg/ifcdoc/CS64/owlGetInstance.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlGetInstance")]
		public static extern void owlGetInstance(int_t model, int_t instance, out Int64 owlInstance);

		/// <summary>
		///		owlMaterialInstance                                     (http://rdf.bg/ifcdoc/CS64/owlMaterialInstance.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlMaterialInstance")]
		public static extern void owlMaterialInstance(int_t instanceBase, int_t instanceContext, out Int64 owlInstance);

		/// <summary>
		///		owlBuildInstance                                        (http://rdf.bg/ifcdoc/CS64/owlBuildInstance.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlBuildInstance")]
		public static extern void owlBuildInstance(int_t model, int_t instance, out Int64 owlInstance);

		/// <summary>
		///		owlBuildInstanceInContext                               (http://rdf.bg/ifcdoc/CS64/owlBuildInstanceInContext.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlBuildInstanceInContext")]
		public static extern void owlBuildInstanceInContext(int_t instanceBase, int_t instanceContext, out Int64 owlInstance);

		/// <summary>
		///		engiInstanceUsesSegmentation                            (http://rdf.bg/ifcdoc/CS64/engiInstanceUsesSegmentation.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiInstanceUsesSegmentation")]
		public static extern byte engiInstanceUsesSegmentation(int_t instance);

		/// <summary>
		///		owlBuildInstances                                       (http://rdf.bg/ifcdoc/CS64/owlBuildInstances.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlBuildInstances")]
		public static extern void owlBuildInstances(int_t model, int_t instance, out Int64 owlInstanceComplete, out Int64 owlInstanceSolids, out Int64 owlInstanceVoids);

		/// <summary>
		///		owlGetMappedItem                                        (http://rdf.bg/ifcdoc/CS64/owlGetMappedItem.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "owlGetMappedItem")]
		public static extern void owlGetMappedItem(int_t model, int_t instance, out Int64 owlInstance, out double transformationMatrix);

		/// <summary>
		///		getInstanceDerivedPropertiesInModelling                 (http://rdf.bg/ifcdoc/CS64/getInstanceDerivedPropertiesInModelling.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getInstanceDerivedPropertiesInModelling")]
		public static extern int_t getInstanceDerivedPropertiesInModelling(int_t model, int_t instance, out double height, out double width, out double thickness);

		/// <summary>
		///		getInstanceDerivedBoundingBox                           (http://rdf.bg/ifcdoc/CS64/getInstanceDerivedBoundingBox.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getInstanceDerivedBoundingBox")]
		public static extern int_t getInstanceDerivedBoundingBox(int_t model, int_t instance, out double Ox, out double Oy, out double Oz, out double Vx, out double Vy, out double Vz);

		/// <summary>
		///		getInstanceTransformationMatrix                         (http://rdf.bg/ifcdoc/CS64/getInstanceTransformationMatrix.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getInstanceTransformationMatrix")]
		public static extern int_t getInstanceTransformationMatrix(int_t model, int_t instance, out double _11, out double _12, out double _13, out double _14, out double _21, out double _22, out double _23, out double _24, out double _31, out double _32, out double _33, out double _34, out double _41, out double _42, out double _43, out double _44);

		/// <summary>
		///		getInstanceDerivedTransformationMatrix                  (http://rdf.bg/ifcdoc/CS64/getInstanceDerivedTransformationMatrix.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getInstanceDerivedTransformationMatrix")]
		public static extern int_t getInstanceDerivedTransformationMatrix(int_t model, int_t instance, out double _11, out double _12, out double _13, out double _14, out double _21, out double _22, out double _23, out double _24, out double _31, out double _32, out double _33, out double _34, out double _41, out double _42, out double _43, out double _44);

		/// <summary>
		///		internalGetBoundingBox                                  (http://rdf.bg/ifcdoc/CS64/internalGetBoundingBox.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalGetBoundingBox")]
		public static extern int_t internalGetBoundingBox(int_t model, int_t instance);

		/// <summary>
		///		internalGetCenter                                       (http://rdf.bg/ifcdoc/CS64/internalGetCenter.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "internalGetCenter")]
		public static extern int_t internalGetCenter(int_t model, int_t instance);

		/// <summary>
		///		getRootAxis2Placement                                   (http://rdf.bg/ifcdoc/CS64/getRootAxis2Placement.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getRootAxis2Placement")]
		public static extern int_t getRootAxis2Placement(int_t model, byte exclusiveIfHasGeometry);

		/// <summary>
		///		getGlobalPlacement                                      (http://rdf.bg/ifcdoc/CS64/getGlobalPlacement.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getGlobalPlacement")]
		public static extern int_t getGlobalPlacement(int_t model, out double origin);

		/// <summary>
		///		setGlobalPlacement                                      (http://rdf.bg/ifcdoc/CS64/setGlobalPlacement.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setGlobalPlacement")]
		public static extern int_t setGlobalPlacement(int_t model, ref double origin, byte includeRotation);

		/// <summary>
		///		getTimeStamp                                            (http://rdf.bg/ifcdoc/CS64/getTimeStamp.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getTimeStamp")]
		public static extern int_t getTimeStamp(int_t model);

		/// <summary>
		///		setInstanceReference                                    (http://rdf.bg/ifcdoc/CS64/setInstanceReference.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setInstanceReference")]
		public static extern int_t setInstanceReference(int_t instance, int_t value);

		/// <summary>
		///		getInstanceReference                                    (http://rdf.bg/ifcdoc/CS64/getInstanceReference.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getInstanceReference")]
		public static extern int_t getInstanceReference(int_t instance);

		/// <summary>
		///		inferenceInstance                                       (http://rdf.bg/ifcdoc/CS64/inferenceInstance.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "inferenceInstance")]
		public static extern int_t inferenceInstance(int_t instance);

		/// <summary>
		///		sdaiValidateSchemaInstance                              (http://rdf.bg/ifcdoc/CS64/sdaiValidateSchemaInstance.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiValidateSchemaInstance")]
		public static extern int_t sdaiValidateSchemaInstance(int_t instance);

        //
        //  Deprecated API Calls (GENERIC)
        //

		/// <summary>
		///		engiGetAggrElement                                      (http://rdf.bg/ifcdoc/CS64/engiGetAggrElement.html)
		///
		///	This call is deprecated, please use call sdaiGetAggrByIndex instead.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrElement")]
		public static extern int_t engiGetAggrElement(int_t aggregate, int_t index, int_t valueType, out bool value);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrElement")]
		public static extern int_t engiGetAggrElement(int_t aggregate, int_t index, int_t valueType, out int_t value);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrElement")]
		public static extern int_t engiGetAggrElement(int_t aggregate, int_t index, int_t valueType, out double value);

		[DllImport(IFCEngineDLL, EntryPoint = "engiGetAggrElement")]
		public static extern int_t engiGetAggrElement(int_t aggregate, int_t index, int_t valueType, out IntPtr value);

		public static int_t engiGetAggrElement(int_t aggregate, int_t index, int_t valueType, out string value)
		{
			value = null;
			valueType = getStringType(valueType);
			if (valueType != 0)
			{
				IntPtr ptr = IntPtr.Zero;
				var ret = engiGetAggrElement(aggregate, index, valueType, out ptr);
				if (ret != 0 && ptr != IntPtr.Zero)
				{
					value = marshalPtrToString(valueType, ptr);
					return ret;
				}
			}
			return 0;
		}

		/// <summary>
		///		engiAttrIsInverse                                       (http://rdf.bg/ifcdoc/CS64/engiAttrIsInverse.html)
		///
		///	This call is deprecated, please use call engiAttrIsInverse instead.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "engiAttrIsInverse")]
		public static extern int_t engiAttrIsInverse(int_t attribute);

		/// <summary>
		///		xxxxOpenModelByStream                                   (http://rdf.bg/ifcdoc/CS64/xxxxOpenModelByStream.html)
		///
		///	This call is deprecated, please use call engiOpenModelByStream instead.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxOpenModelByStream")]
		public static extern int_t xxxxOpenModelByStream(int_t repository, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, string schemaName);

		[DllImport(IFCEngineDLL, EntryPoint = "xxxxOpenModelByStream")]
		public static extern int_t xxxxOpenModelByStream(int_t repository, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, byte[] schemaName);

		/// <summary>
		///		sdaiCreateIterator                                      (http://rdf.bg/ifcdoc/CS64/sdaiCreateIterator.html)
		///
		///	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..) instead.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiCreateIterator")]
		public static extern int_t sdaiCreateIterator(int_t aggregate);

		/// <summary>
		///		sdaiDeleteIterator                                      (http://rdf.bg/ifcdoc/CS64/sdaiDeleteIterator.html)
		///
		///	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..) instead.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiDeleteIterator")]
		public static extern void sdaiDeleteIterator(int_t iterator);

		/// <summary>
		///		sdaiBeginning                                           (http://rdf.bg/ifcdoc/CS64/sdaiBeginning.html)
		///
		///	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..) instead.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiBeginning")]
		public static extern void sdaiBeginning(int_t iterator);

		/// <summary>
		///		sdaiNext                                                (http://rdf.bg/ifcdoc/CS64/sdaiNext.html)
		///
		///	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..) instead.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiNext")]
		public static extern int_t sdaiNext(int_t iterator);

		/// <summary>
		///		sdaiPrevious                                            (http://rdf.bg/ifcdoc/CS64/sdaiPrevious.html)
		///
		///	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..) instead.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiPrevious")]
		public static extern int_t sdaiPrevious(int_t iterator);

		/// <summary>
		///		sdaiEnd                                                 (http://rdf.bg/ifcdoc/CS64/sdaiEnd.html)
		///
		///	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..) instead.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiEnd")]
		public static extern void sdaiEnd(int_t iterator);

		/// <summary>
		///		sdaiplusGetAggregationType                              (http://rdf.bg/ifcdoc/CS64/sdaiplusGetAggregationType.html)
		///
		///	This call is deprecated, please use call .... instead
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "sdaiplusGetAggregationType")]
		public static extern int_t sdaiplusGetAggregationType(int_t instance, int_t aggregate);

		/// <summary>
		///		xxxxGetAttrType                                         (http://rdf.bg/ifcdoc/CS64/xxxxGetAttrType.html)
		///
		///	...
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetAttrType")]
		public static extern int_t xxxxGetAttrType(int_t instance, int_t attribute, out IntPtr attributeType);

		/// <summary>
		///		xxxxGetAttrTypeBN                                       (http://rdf.bg/ifcdoc/CS64/xxxxGetAttrTypeBN.html)
		///
		///	This call is deprecated, please use calls engiGetAttrTypeBN(..) instead.
		///
		///	Technically it will transform into the following call
		///		xxxxGetAttrType(
		///				instance,
		///				sdaiGetAttrDefinition(
		///						sdaiGetInstanceType(
		///								instance
		///							),
		///						attributeName
		///					),
		///				attributeType
		///			);
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetAttrTypeBN")]
		public static extern int_t xxxxGetAttrTypeBN(int_t instance, string attributeName, out IntPtr attributeType);

		[DllImport(IFCEngineDLL, EntryPoint = "xxxxGetAttrTypeBN")]
		public static extern int_t xxxxGetAttrTypeBN(int_t instance, byte[] attributeName, out IntPtr attributeType);

        //
        //  Validation
        //

		/// <summary>
		///		validateSetOptions                                      (http://rdf.bg/ifcdoc/CS64/validateSetOptions.html)
		///
		///	Allows to set a timne limit in seconds, setting to 0 means no time limite.
		///	Allows to set a count limit, setting to 0 means no count limit.
		///	Allows to hide redundent issues.
		///
		///		bit 0:	(__NO_OF_ARGUMENTS)					number of arguments
		///		bit 1:	(__ARGUMENT_EXPRESS_TYPE)			argument value is correct entity, defined type or enumeration value
		///		bit 2:	(__ARGUMENT_PRIM_TYPE)				argument value has correct primitive type
		///		bit 3:	(__REQUIRED_ARGUMENTS)				non-optional arguments values are provided
		///		bit 4:	(__ARRGEGATION_EXPECTED)			aggregation is provided when expected
		///		bit 5:	(__AGGREGATION_NOT_EXPECTED)		aggregation is not used when not expected
		///		bit 6:	(__AGGREGATION_SIZE)				aggregation size
		///		bit 7:	(__AGGREGATION_UNIQUE)				elements in aggregations are unique when required
		///		bit 8:	(__COMPLEX_INSTANCE)				complex instances contains full parent chains
		///		bit 9:	(__REFERENCE_EXISTS)				referenced instance exists
		///		bit 10:	(__ABSTRACT_ENTITY)					abstract entity should not instantiate
		///		bit 11:	(__WHERE_RULE)						where-rule check
		///		bit 12:	(__UNIQUE_RULE)						unique-rule check
		///		bit 13:	(__STAR_USAGE)						* is used only for derived arguments
		///		bit 14:	(__CALL_ARGUMENT)					validateModel/validateInstance function argument should be model/instance
		///		bit 15:	(__INTERNAL_ERROR)					unspecified error
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateSetOptions")]
		public static extern void validateSetOptions(int_t timeLimitSeconds, int_t issueCntLimit, byte showEachIssueOnce, UInt64 issueTypes, UInt64 mask);

		/// <summary>
		///		validateGetOptions                                      (http://rdf.bg/ifcdoc/CS64/validateGetOptions.html)
		///
		///	Allows to get the timne limit in seconds, value 0 means no time limite, input can be left to NULL if not relevant.
		///	Allows to get the count limit, value 0 means no count limit, input can be left to NULL if not relevant.
		///	Allows to get hide redundent issues, input can be left to NULL if not relevant.
		///	Return value is the issueTypes enabled according to the mask given.
		///
		///		bit 0:	(__NO_OF_ARGUMENTS)					number of arguments
		///		bit 1:	(__ARGUMENT_EXPRESS_TYPE)			argument value is correct entity, defined type or enumeration value
		///		bit 2:	(__ARGUMENT_PRIM_TYPE)				argument value has correct primitive type
		///		bit 3:	(__REQUIRED_ARGUMENTS)				non-optional arguments values are provided
		///		bit 4:	(__ARRGEGATION_EXPECTED)			aggregation is provided when expected
		///		bit 5:	(__AGGREGATION_NOT_EXPECTED)		aggregation is not used when not expected
		///		bit 6:	(__AGGREGATION_SIZE)				aggregation size
		///		bit 7:	(__AGGREGATION_UNIQUE)				elements in aggregations are unique when required
		///		bit 8:	(__COMPLEX_INSTANCE)				complex instances contains full parent chains
		///		bit 9:	(__REFERENCE_EXISTS)				referenced instance exists
		///		bit 10:	(__ABSTRACT_ENTITY)					abstract entity should not instantiate
		///		bit 11:	(__WHERE_RULE)						where-rule check
		///		bit 12:	(__UNIQUE_RULE)						unique-rule check
		///		bit 13:	(__STAR_USAGE)						* is used only for derived arguments
		///		bit 14:	(__CALL_ARGUMENT)					validateModel/validateInstance function argument should be model/instance
		///		bit 15:	(__INTERNAL_ERROR)					unspecified error
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetOptions")]
		public static extern UInt64 validateGetOptions(out int_t timeLimitSeconds, out int_t issueCntLimit, out byte showEachIssueOnce, UInt64 mask);

		/// <summary>
		///		validateModel                                           (http://rdf.bg/ifcdoc/CS64/validateModel.html)
		///
		///	Apply validation of a model
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateModel")]
		public static extern int_t validateModel(int_t model);

		/// <summary>
		///		validateInstance                                        (http://rdf.bg/ifcdoc/CS64/validateInstance.html)
		///
		///	Apply validation of an instance
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateInstance")]
		public static extern int_t validateInstance(int_t instance);

		/// <summary>
		///		validateFreeResults                                     (http://rdf.bg/ifcdoc/CS64/validateFreeResults.html)
		///
		///	Clean validation results
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateFreeResults")]
		public static extern void validateFreeResults(int_t results);

		/// <summary>
		///		validateGetFirstIssue                                   (http://rdf.bg/ifcdoc/CS64/validateGetFirstIssue.html)
		///
		///	Get first issue from validation results.
		///	If no issues inside validation results or validation results is NULL it will return NULL.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetFirstIssue")]
		public static extern int_t validateGetFirstIssue(int_t results);

		/// <summary>
		///		validateGetNextIssue                                    (http://rdf.bg/ifcdoc/CS64/validateGetNextIssue.html)
		///
		///	Get next issue based on a given issue.
		///	If no issues left or validation issue is NULL it will return NULL.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetNextIssue")]
		public static extern int_t validateGetNextIssue(int_t issue);

		/// <summary>
		///		validateGetStatus                                       (http://rdf.bg/ifcdoc/CS64/validateGetStatus.html)
		///
		///	Return value is the issueStatus (enum_validation_status):
		///
		///		value 0:	(__NONE)						no status set
		///		value 1:	(__COMPLETE_ALL)				all issues proceed
		///		value 2:	(__COMPLETE_NOT_ALL)			completed but some issues were excluded by option settings
		///		value 3:	(__TIME_EXCEED)					validation was finished because of reach time limit
		///		value 4:	(__COUNT_EXCEED)				validation was finished because of reach of issue's numbers limit
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetStatus")]
		public static extern enum_validation_status validateGetStatus(int_t results);

		/// <summary>
		///		validateGetIssueType                                    (http://rdf.bg/ifcdoc/CS64/validateGetIssueType.html)
		///
		///	Return value is the issueType (enum_validation_type):
		///
		///		bit 0:	(__NO_OF_ARGUMENTS)					number of arguments
		///		bit 1:	(__ARGUMENT_EXPRESS_TYPE)			argument value is correct entity, defined type or enumeration value
		///		bit 2:	(__ARGUMENT_PRIM_TYPE)				argument value has correct primitive type
		///		bit 3:	(__REQUIRED_ARGUMENTS)				non-optional arguments values are provided
		///		bit 4:	(__ARRGEGATION_EXPECTED)			aggregation is provided when expected
		///		bit 5:	(__AGGREGATION_NOT_EXPECTED)		aggregation is not used when not expected
		///		bit 6:	(__AGGREGATION_SIZE)				aggregation size
		///		bit 7:	(__AGGREGATION_UNIQUE)				elements in aggregations are unique when required
		///		bit 8:	(__COMPLEX_INSTANCE)				complex instances contains full parent chains
		///		bit 9:	(__REFERENCE_EXISTS)				referenced instance exists
		///		bit 10:	(__ABSTRACT_ENTITY)					abstract entity should not instantiate
		///		bit 11:	(__WHERE_RULE)						where-rule check
		///		bit 12:	(__UNIQUE_RULE)						unique-rule check
		///		bit 13:	(__STAR_USAGE)						* is used only for derived arguments
		///		bit 14:	(__CALL_ARGUMENT)					validateModel/validateInstance function argument should be model/instance
		///		bit 15:	(__INTERNAL_ERROR)					unspecified error
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetIssueType")]
		public static extern UInt64 validateGetIssueType(int_t issue);

		/// <summary>
		///		validateGetInstance                                     (http://rdf.bg/ifcdoc/CS64/validateGetInstance.html)
		///
		///	Returns the (first) instance related to the given issue
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetInstance")]
		public static extern int_t validateGetInstance(int_t issue);

		/// <summary>
		///		validateGetInstanceRelated                              (http://rdf.bg/ifcdoc/CS64/validateGetInstanceRelated.html)
		///
		///	Returns the second instance related to the given issue (if relevant)
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetInstanceRelated")]
		public static extern int_t validateGetInstanceRelated(int_t issue);

		/// <summary>
		///		validateGetEntity                                       (http://rdf.bg/ifcdoc/CS64/validateGetEntity.html)
		///
		///	Returns the entity handle related to the given issue (if relevant)
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetEntity")]
		public static extern int_t validateGetEntity(int_t issue);

		/// <summary>
		///		validateGetAttr                                         (http://rdf.bg/ifcdoc/CS64/validateGetAttr.html)
		///
		///	Returns the attribute handle related to the given issue (if relevant)
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetAttr")]
		public static extern int_t validateGetAttr(int_t issue);

		/// <summary>
		///		validateGetAggrLevel                                    (http://rdf.bg/ifcdoc/CS64/validateGetAggrLevel.html)
		///
		///	specifies nesting level of aggregation or 0
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetAggrLevel")]
		public static extern int_t validateGetAggrLevel(int_t issue);

		/// <summary>
		///		validateGetAggrIndArray                                 (http://rdf.bg/ifcdoc/CS64/validateGetAggrIndArray.html)
		///
		///	array of indecies for each aggregation lsize is aggrLevel
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetAggrIndArray")]
		public static extern int_t validateGetAggrIndArray(int_t issue);

		/// <summary>
		///		validateGetIssueLevel                                   (http://rdf.bg/ifcdoc/CS64/validateGetIssueLevel.html)
		///
		///	Returns the issue level (i.e. severaty of the issue) of the issue given as input
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetIssueLevel")]
		public static extern int_t validateGetIssueLevel(int_t issue);

		/// <summary>
		///		validateGetDescription                                  (http://rdf.bg/ifcdoc/CS64/validateGetDescription.html)
		///
		///	Returns the description text of the issue given as input
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "validateGetDescription")]
		public static extern IntPtr validateGetDescription(int_t issue);

        //
        //  Deprecated API Calls (GEOMETRY)
        //

		/// <summary>
		///		initializeModellingInstance                             (http://rdf.bg/ifcdoc/CS64/initializeModellingInstance.html)
		///
		///	This call is deprecated, please use call CalculateInstance().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "initializeModellingInstance")]
		public static extern int_t initializeModellingInstance(int_t model, out int_t noVertices, out int_t noIndices, double scale, int_t instance);

		/// <summary>
		///		finalizeModelling                                       (http://rdf.bg/ifcdoc/CS64/finalizeModelling.html)
		///
		///	This call is deprecated, please use call UpdateInstanceVertexBuffer() and UpdateInstanceIndexBuffer().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "finalizeModelling")]
		public static extern int_t finalizeModelling(int_t model, out float vertices, out int_t indices, int_t FVF);

		/// <summary>
		///		getInstanceInModelling                                  (http://rdf.bg/ifcdoc/CS64/getInstanceInModelling.html)
		///
		///	This call is deprecated, there is no direct / easy replacement although the functionality is present. If you still use this call please contact RDF to find a solution together.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getInstanceInModelling")]
		public static extern int_t getInstanceInModelling(int_t model, int_t instance, int_t mode, out int_t startVertex, out int_t startIndex, out int_t primitiveCount);

		/// <summary>
		///		setVertexOffset                                         (http://rdf.bg/ifcdoc/CS64/setVertexOffset.html)
		///
		///	This call is deprecated, please use call SetVertexBufferOffset().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setVertexOffset")]
		public static extern void setVertexOffset(int_t model, double x, double y, double z);

		/// <summary>
		///		setFormat                                               (http://rdf.bg/ifcdoc/CS64/setFormat.html)
		///
		///	This call is deprecated, please use call SetFormat().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "setFormat")]
		public static extern void setFormat(int_t model, int_t setting, int_t mask);

		/// <summary>
		///		getConceptualFaceCnt                                    (http://rdf.bg/ifcdoc/CS64/getConceptualFaceCnt.html)
		///
		///	This call is deprecated, please use call GetConceptualFaceCnt().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getConceptualFaceCnt")]
		public static extern int_t getConceptualFaceCnt(int_t instance);

		/// <summary>
		///		getConceptualFaceEx                                     (http://rdf.bg/ifcdoc/CS64/getConceptualFaceEx.html)
		///
		///	This call is deprecated, please use call GetConceptualFaceEx().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "getConceptualFaceEx")]
		public static extern int_t getConceptualFaceEx(int_t instance, int_t index, out int_t startIndexTriangles, out int_t noIndicesTriangles, out int_t startIndexLines, out int_t noIndicesLines, out int_t startIndexPoints, out int_t noIndicesPoints, out int_t startIndexFacePolygons, out int_t noIndicesFacePolygons, out int_t startIndexConceptualFacePolygons, out int_t noIndicesConceptualFacePolygons);

		/// <summary>
		///		createGeometryConversion                                (http://rdf.bg/ifcdoc/CS64/createGeometryConversion.html)
		///
		///	This call is deprecated, please use call ... .
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "createGeometryConversion")]
		public static extern void createGeometryConversion(int_t instance, out Int64 owlInstance);

		/// <summary>
		///		convertInstance                                         (http://rdf.bg/ifcdoc/CS64/convertInstance.html)
		///
		///	This call is deprecated, please use call ... .
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "convertInstance")]
		public static extern void convertInstance(int_t instance);

		/// <summary>
		///		initializeModellingInstanceEx                           (http://rdf.bg/ifcdoc/CS64/initializeModellingInstanceEx.html)
		///
		///	This call is deprecated, please use call CalculateInstance().
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "initializeModellingInstanceEx")]
		public static extern int_t initializeModellingInstanceEx(int_t model, out int_t noVertices, out int_t noIndices, double scale, int_t instance, int_t instanceList);

		/// <summary>
		///		exportModellingAsOWL                                    (http://rdf.bg/ifcdoc/CS64/exportModellingAsOWL.html)
		///
		///	This call is deprecated, please contact us if you use this call.
		/// </summary>
		[DllImport(IFCEngineDLL, EntryPoint = "exportModellingAsOWL")]
		public static extern void exportModellingAsOWL(int_t model, string fileName);

		[DllImport(IFCEngineDLL, EntryPoint = "exportModellingAsOWL")]
		public static extern void exportModellingAsOWL(int_t model, byte[] fileName);

		/// <summary>
		/// 
		/// </summary>
		private static int_t getStringType(int_t valueType)
		{
			switch (valueType)
			{
				case sdaiSTRING:
				case sdaiUNICODE:
					return sdaiUNICODE;

				case sdaiEXPRESSSTRING:
				case sdaiENUM:
				case sdaiLOGICAL:
				case sdaiBINARY:
					return valueType;
			}
			return 0;
		}

		/// <summary>
		/// 
		/// </summary>
		private static string marshalPtrToString(int_t valueType, IntPtr ptr)
		{
		    switch (valueType)
		    {
				case sdaiUNICODE:
					return Marshal.PtrToStringUni(ptr);

				case sdaiEXPRESSSTRING:
					return Marshal.PtrToStringAnsi(ptr);

				case sdaiENUM:
				case sdaiLOGICAL:
				case sdaiBINARY:
					{
						var unicode = getStringUnicode();
						if (unicode == 0)
							return Marshal.PtrToStringAnsi(ptr);
						else if (unicode == 1 || unicode == 2)
							return Marshal.PtrToStringUni(ptr);
					}
					break;
		    }
		    return null;
		}
	}

	class engine
	{
		public const Int64 OBJECTPROPERTY_TYPE             = 1;
		public const Int64 DATATYPEPROPERTY_TYPE_BOOLEAN   = 2;
		public const Int64 DATATYPEPROPERTY_TYPE_CHAR      = 3;
		public const Int64 DATATYPEPROPERTY_TYPE_INTEGER   = 4;
		public const Int64 DATATYPEPROPERTY_TYPE_DOUBLE    = 5;
		public const Int64 DATATYPEPROPERTY_TYPE_BYTE      = 6;

		public const UInt64 flagbit0 = 1;               // 2^^0							 0000.0000..0000.0001
		public const UInt64 flagbit1 = 2;               // 2^^1							 0000.0000..0000.0010
		public const UInt64 flagbit2 = 4;               // 2^^2							 0000.0000..0000.0100
		public const UInt64 flagbit3 = 8;               // 2^^3							 0000.0000..0000.1000

		public const UInt64 flagbit4 = 16;              // 2^^4							 0000.0000..0001.0000
		public const UInt64 flagbit5 = 32;              // 2^^5							 0000.0000..0010.0000
		public const UInt64 flagbit6 = 64;              // 2^^6							 0000.0000..0100.0000
		public const UInt64 flagbit7 = 128;             // 2^^7							 0000.0000..1000.0000

		public const UInt64 flagbit8 = 256;             // 2^^8							 0000.0001..0000.0000
		public const UInt64 flagbit9 = 512;             // 2^^9							 0000.0010..0000.0000
		public const UInt64 flagbit10 = 1024;           // 2^^10						 0000.0100..0000.0000
		public const UInt64 flagbit11 = 2048;           // 2^^11						 0000.1000..0000.0000

		public const UInt64 flagbit12 = 4096;           // 2^^12						 0001.0000..0000.0000
		public const UInt64 flagbit13 = 8192;           // 2^^13						 0010.0000..0000.0000
		public const UInt64 flagbit14 = 16384;          // 2^^14						 0100.0000..0000.0000
		public const UInt64 flagbit15 = 32768;          // 2^^15						 1000.0000..0000.0000

		public const UInt64 flagbit16 = 65536;          // 2^^16   0000.0000..0000.0001  0000.0000..0000.0000
		public const UInt64 flagbit17 = 131072;         // 2^^17   0000.0000..0000.0010  0000.0000..0000.0000
		public const UInt64 flagbit18 = 262144;         // 2^^18   0000.0000..0000.0100  0000.0000..0000.0000
		public const UInt64 flagbit19 = 524288;         // 2^^19   0000.0000..0000.1000  0000.0000..0000.0000

		public const UInt64 flagbit20 = 1048576;        // 2^^20   0000.0000..0001.0000  0000.0000..0000.0000
		public const UInt64 flagbit21 = 2097152;        // 2^^21   0000.0000..0010.0000  0000.0000..0000.0000
		public const UInt64 flagbit22 = 4194304;        // 2^^22   0000.0000..0100.0000  0000.0000..0000.0000
		public const UInt64 flagbit23 = 8388608;        // 2^^23   0000.0000..1000.0000  0000.0000..0000.0000

		public const UInt64 flagbit24 = 16777216;       // 2^^24   0000.0001..0000.0000  0000.0000..0000.0000
		public const UInt64 flagbit25 = 33554432;       // 2^^25   0000.0010..0000.0000  0000.0000..0000.0000
		public const UInt64 flagbit26 = 67108864;       // 2^^26   0000.0100..0000.0000  0000.0000..0000.0000
		public const UInt64 flagbit27 = 134217728;      // 2^^27   0000.1000..0000.0000  0000.0000..0000.0000

		public const UInt64 flagbit28 = 268435456;      // 2^^28   0001.0000..0000.0000  0000.0000..0000.0000
		public const UInt64 flagbit29 = 536870912;      // 2^^29   0010.0000..0000.0000  0000.0000..0000.0000
		public const UInt64 flagbit30 = 1073741824;     // 2^^30   0100.0000..0000.0000  0000.0000..0000.0000
		public const UInt64 flagbit31 = 2147483648;		// 2^^31   1000.0000..0000.0000  0000.0000..0000.0000

		public const string enginedll = @"IFCEngine.dll";

        //
        //  Meta information API Calls
        //

		/// <summary>
		///		GetRevision                                             (http://rdf.bg/gkdoc/CS64/GetRevision.html)
		///
		///	Returns the revision number.
		///	The timeStamp is generated by the SVN system used during development.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetRevision")]
		public static extern Int64 GetRevision(out IntPtr timeStamp);

		public static Int64 GetRevision()
		{
			IntPtr timeStamp = IntPtr.Zero;
			return GetRevision(out timeStamp);
		}

		/// <summary>
		///		GetRevisionW                                            (http://rdf.bg/gkdoc/CS64/GetRevisionW.html)
		///
		///	Returns the revision number.
		///	The timeStamp is generated by the SVN system used during development.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetRevisionW")]
		public static extern Int64 GetRevisionW(out IntPtr timeStamp);

		public static Int64 GetRevisionW()
		{
			IntPtr timeStamp = IntPtr.Zero;
			return GetRevisionW(out timeStamp);
		}

		/// <summary>
		///		GetProtection                                           (http://rdf.bg/gkdoc/CS64/GetProtection.html)
		///
		///	This call is required to be called to enable the DLL to work if protection is active.
		///
		///	Returns the number of days (incl. this one) that this version is still active or 0 if no protection is embedded.
		///	In case no days are left and protection is active this call will return -1.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetProtection")]
		public static extern Int64 GetProtection();

		/// <summary>
		///		GetEnvironment                                          (http://rdf.bg/gkdoc/CS64/GetEnvironment.html)
		///
		///	Returns the revision number similar to the call GetRevision.
		///	The environment variables will show known environment variables
		///	and if they are set, for example environment variables ABC known
		///	and unset and DEF as well as GHI known and set:
		///		environmentVariables = "ABC:F;DEF:T;GHI:T"
		///	Development variables are depending on the build environment
		///	As an example in windows systems where Visual Studio is used:
		///		developmentVariables = "...."
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetEnvironment")]
		public static extern Int64 GetEnvironment(out IntPtr environmentVariables, out IntPtr developmentVariables);

		/// <summary>
		///		GetEnvironmentW                                         (http://rdf.bg/gkdoc/CS64/GetEnvironmentW.html)
		///
		///	Returns the revision number similar to the call GetRevision[W].
		///	The environment variables will show known environment variables
		///	and if they are set, for example environment variables ABC known
		///	and unset and DEF as well as GHI known and set:
		///		environmentVariables = "ABC:F;DEF:T;GHI:T"
		///	Development variables are depending on the build environment
		///	As an example in windows systems where Visual Studio is used:
		///		developmentVariables = "...."
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetEnvironmentW")]
		public static extern Int64 GetEnvironmentW(out IntPtr environmentVariables, out IntPtr developmentVariables);

		/// <summary>
		///		SetAssertionFile                                        (http://rdf.bg/gkdoc/CS64/SetAssertionFile.html)
		///
		///	This function sets the file location where internal assertions should be written to.
		///	If the file name is not set (default) many internal control procedures are not executed
		///	and the code will be faster.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetAssertionFile")]
		public static extern void SetAssertionFile(string fileName);

		[DllImport(enginedll, EntryPoint = "SetAssertionFile")]
		public static extern void SetAssertionFile(byte[] fileName);

		/// <summary>
		///		SetAssertionFileW                                       (http://rdf.bg/gkdoc/CS64/SetAssertionFileW.html)
		///
		///	This function sets the file location where internal assertions should be written to.
		///	If the file name is not set (default) many internal control procedures are not executed
		///	and the code will be faster.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetAssertionFileW")]
		public static extern void SetAssertionFileW(string fileName);

		[DllImport(enginedll, EntryPoint = "SetAssertionFileW")]
		public static extern void SetAssertionFileW(byte[] fileName);

		/// <summary>
		///		GetAssertionFile                                        (http://rdf.bg/gkdoc/CS64/GetAssertionFile.html)
		///
		///	This function gets the file location as stored/set internally where internal assertions should be written to.
		///	It works independent if the file location is set through SetAssertionFile() or SetAssertionFileW().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetAssertionFile")]
		public static extern IntPtr GetAssertionFile(out IntPtr fileName);

		public static string GetAssertionFile()
		{
			IntPtr fileName = IntPtr.Zero;
			GetAssertionFile(out fileName);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(fileName);
		}

		/// <summary>
		///		GetAssertionFileW                                       (http://rdf.bg/gkdoc/CS64/GetAssertionFileW.html)
		///
		///	This function gets the file location as stored/set internally where internal assertions should be written to.
		///	It works independent if the file location is set through SetAssertionFile() or SetAssertionFileW().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetAssertionFileW")]
		public static extern IntPtr GetAssertionFileW(out IntPtr fileName);

		public static string GetAssertionFileW()
		{
			IntPtr fileName = IntPtr.Zero;
			GetAssertionFileW(out fileName);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(fileName);
		}

		/// <summary>
		///		SetCharacterSerialization                               (http://rdf.bg/gkdoc/CS64/SetCharacterSerialization.html)
		///
		///	This call defines how characters for names, strings will be serializaed and how
		///	they are expected to be serialized. An exception are the Open / Import / Save calls,
		///	these calls have a fixed way of serialization of path / file names.
		///
		///	If the encoding value is non-zero the following values are possible (if zero encoding is kept as defined)
		///		 32 [default]	encoding ignored
		///		 64				encoding Windows 1250
		///		 65				encoding Windows 1251
		///		 66				encoding Windows 1252
		///		 67				encoding Windows 1253
		///		 68				encoding Windows 1254
		///		 69				encoding Windows 1255
		///		 70				encoding Windows 1256
		///		 71				encoding Windows 1257
		///		 72				encoding Windows 1258
		///		128				encoding ISO8859 1
		///		129				encoding ISO8859 2
		///		130				encoding ISO8859 3
		///		131				encoding ISO8859 4
		///		132				encoding ISO8859 5
		///		133				encoding ISO8859 6
		///		134				encoding ISO8859 7
		///		135				encoding ISO8859 8
		///		136				encoding ISO8859 9
		///		137				encoding ISO8859 10
		///		138				encoding ISO8859 11
		///						encoding ISO8859 12 => does not exist
		///		140				encoding ISO8859 13
		///		141				encoding ISO8859 14
		///		142				encoding ISO8859 15
		///		143				encoding ISO8859 16
		///		160				encoding MACINTOSH CENTRAL EUROPEAN
		///		192				encoding SHIFT JIS X 213
		///
		///	The wcharBitSizeOverride value overrides the OS based size of wchar_t, the following values can be applied:
		///		0			wcharBitSizeOverride is ignored, override is not changed
		///		16			wchar_t interpreted as being 2 bytes wide (size of wchar_t in bits)
		///		32			wchar_t interpreted as being 4 bytes wide (size of wchar_t in bits)
		///		Any other value will reset the override and wchar_t will follow the OS based size of wchar_t
		///	Note: this setting is independent from the model, this call can also be called without a model defined.
		///
		///	The ascii value defines
		///		true [default]	8 bit serializatiom (size of char returned in bits)
		///		false			16/32 bit serialization (depending on the operating system, i.e. sizeof of wchar_t returned in number of bits)
		///	Note: this setting is model-dependent and requires a model present to have any effect.
		///
		///	The return value is the size of a single character in bits, i.e. 1 byte is 8 bits, the value for a wchar_t can be 16 or 32 depending on settings and operating system
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetCharacterSerialization")]
		public static extern Int64 SetCharacterSerialization(Int64 model, Int64 encoding, Int64 wcharBitSizeOverride, byte ascii);

		/// <summary>
		///		GetCharacterSerialization                               (http://rdf.bg/gkdoc/CS64/GetCharacterSerialization.html)
		///
		///	This call retrieves the values as set by 
		///
		///	The returns the size of a single character in bits, i.e. 1 byte is 8 bits, this can be 8, 16 or 32 depending on settings and operating system
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetCharacterSerialization")]
		public static extern Int64 GetCharacterSerialization(Int64 model, out Int64 encoding, out byte ascii);

		/// <summary>
		///		SetModellingStyle                                       (http://rdf.bg/gkdoc/CS64/SetModellingStyle.html)
		///
		///	This call sets the modelling style.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetModellingStyle")]
		public static extern void SetModellingStyle(Int64 model, Int64 setting, Int64 mask);

		/// <summary>
		///		GetModellingStyle                                       (http://rdf.bg/gkdoc/CS64/GetModellingStyle.html)
		///
		///	This call gets the modelling style.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetModellingStyle")]
		public static extern Int64 GetModellingStyle(Int64 model, Int64 mask);

		/// <summary>
		///		AbortModel                                              (http://rdf.bg/gkdoc/CS64/AbortModel.html)
		///
		///	This function abort running processes for a model. It can be used when a task takes more time than
		///	expected / available, or in case the requested results are not relevant anymore.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "AbortModel")]
		public static extern Int64 AbortModel(Int64 model, Int64 setting);

		/// <summary>
		///		GetSessionMetaInfo                                      (http://rdf.bg/gkdoc/CS64/GetSessionMetaInfo.html)
		///
		///	This function is meant for debugging purposes and return statistics during processing.
		///	The return value represents the number of active models within the session (or zero if the model was not recognized).
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetSessionMetaInfo")]
		public static extern Int64 GetSessionMetaInfo(out Int64 allocatedBlocks, out Int64 allocatedBytes, out Int64 nonUsedBlocks, out Int64 nonUsedBytes);

		/// <summary>
		///		GetModelMetaInfo                                        (http://rdf.bg/gkdoc/CS64/GetModelMetaInfo.html)
		///
		///	This function is meant for debugging purposes and return statistics during processing.
		///	The return value represents the number of active models within the session (or zero if the model was not recognized).
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, out Int64 activeClasses, out Int64 removedClasses, out Int64 activeProperties, out Int64 removedProperties, out Int64 activeInstances, out Int64 removedInstances, out Int64 inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, out Int64 activeClasses, out Int64 removedClasses, out Int64 activeProperties, out Int64 removedProperties, IntPtr activeInstances, IntPtr removedInstances, IntPtr inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, out Int64 activeClasses, out Int64 removedClasses, IntPtr activeProperties, IntPtr removedProperties, out Int64 activeInstances, out Int64 removedInstances, out Int64 inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, out Int64 activeClasses, out Int64 removedClasses, IntPtr activeProperties, IntPtr removedProperties, IntPtr activeInstances, IntPtr removedInstances, IntPtr inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, IntPtr activeClasses, IntPtr removedClasses, out Int64 activeProperties, out Int64 removedProperties, out Int64 activeInstances, out Int64 removedInstances, out Int64 inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, IntPtr activeClasses, IntPtr removedClasses, out Int64 activeProperties, out Int64 removedProperties, IntPtr activeInstances, IntPtr removedInstances, IntPtr inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, IntPtr activeClasses, IntPtr removedClasses, IntPtr activeProperties, IntPtr removedProperties, out Int64 activeInstances, out Int64 removedInstances, out Int64 inactiveInstances);

		[DllImport(enginedll, EntryPoint = "GetModelMetaInfo")]
		public static extern Int64 GetModelMetaInfo(Int64 model, IntPtr activeClasses, IntPtr removedClasses, IntPtr activeProperties, IntPtr removedProperties, IntPtr activeInstances, IntPtr removedInstances, IntPtr inactiveInstances);

		/// <summary>
		///		GetInstanceMetaInfo                                     (http://rdf.bg/gkdoc/CS64/GetInstanceMetaInfo.html)
		///
		///	This function is meant for debugging purposes and return statistics during processing.
		///	The return value represents the number of active instances within the model (or zero if the instance was not recognized).
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceMetaInfo")]
		public static extern Int64 GetInstanceMetaInfo(Int64 owlInstance, out Int64 allocatedBlocks, out Int64 allocatedBytes);

		[DllImport(enginedll, EntryPoint = "GetInstanceMetaInfo")]
		public static extern Int64 GetInstanceMetaInfo(Int64 owlInstance, out Int64 allocatedBlocks, IntPtr allocatedBytes);

		[DllImport(enginedll, EntryPoint = "GetInstanceMetaInfo")]
		public static extern Int64 GetInstanceMetaInfo(Int64 owlInstance, IntPtr allocatedBlocks, out Int64 allocatedBytes);

		[DllImport(enginedll, EntryPoint = "GetInstanceMetaInfo")]
		public static extern Int64 GetInstanceMetaInfo(Int64 owlInstance, IntPtr allocatedBlocks, IntPtr allocatedBytes);

		/// <summary>
		///		GetSmoothness                                           (http://rdf.bg/gkdoc/CS64/GetSmoothness.html)
		///
		///	This function returns the smoothness of a line or surface.
		///	In case the smoothness can be defined the degree will get assigned either
		///		0 - continuous curve / surface (i.e. degree 9)
		///		1 - the direction of the curve / surface is gradually changing (i.e. degree 1)
		///		2 - the change of direction of the curve / surface is gradually changing (i.e. degree 2)
		///	In return value of this function retuns the dimension of the found smoothness:
		///		0 - smoothness could not be defined
		///		1 - found the smoothness of a curve
		///		2 - found the smoothness of a surface
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetSmoothness")]
		public static extern Int64 GetSmoothness(Int64 owlInstance, out Int64 degree);

		/// <summary>
		///		AddState                                                (http://rdf.bg/gkdoc/CS64/AddState.html)
		///
		///	This call will integrate the current state information into the model.
		///
		///	Model should be non-zero.
		///
		///	If owlInstance is given the state is only applied on the owlInstance and all its children.
		///	If owlInstance is zero the state is applied on all owlInstances within a model.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "AddState")]
		public static extern void AddState(Int64 model, Int64 owlInstance);

		/// <summary>
		///		GetModel                                                (http://rdf.bg/gkdoc/CS64/GetModel.html)
		///
		///	Returns model for any resource, i.e. class, property, instance
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetModel")]
		public static extern Int64 GetModel(Int64 rdfsResource);

		/// <summary>
		///		OrderedHandles                                          (http://rdf.bg/gkdoc/CS64/OrderedHandles.html)
		///
		///	This call can be used in two ways. The optional arguments classCnt,
		///	propertyCnt and instanceCnt can be used to get the total amount of active classes,
		///	properies and instances available within the model.
		///
		///	The setting and mask can be used to order the handles given for classes,
		///	properties and instances.
		///		1 - if set this will number all classes with possible values [1 .. classCnt]
		///		2 - if set this will number all classes with possible values [1 .. propertyCnt]
		///		4 - if set this will number all classes with possible values [1 .. instanceCnt]
		///
		///	Note: when enabling ordered handles be aware that classes, properties and instances
		///		  can share the same handles, using the correct argument cannot be checked anymore
		///		  by the library itself. This could result in crashes in case of incorrect assignments
		///		  by the hosting application.
		///	Note: internally there is no performance gain / loss. This is purely meant for situations
		///		  where the hosting application can benefit performance wise from having an ordered list.
		///	Note: use in combination with other libraries is not adviced, i.e. when combined with the
		///		  IFC generation from the IFC Engine component for example
		/// </summary>
		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, out Int64 classCnt, out Int64 propertyCnt, out Int64 instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, out Int64 classCnt, out Int64 propertyCnt, IntPtr instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, out Int64 classCnt, IntPtr propertyCnt, out Int64 instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, out Int64 classCnt, IntPtr propertyCnt, IntPtr instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, IntPtr classCnt, out Int64 propertyCnt, out Int64 instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, IntPtr classCnt, out Int64 propertyCnt, IntPtr instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, IntPtr classCnt, IntPtr propertyCnt, out Int64 instanceCnt, Int64 setting, Int64 mask);

		[DllImport(enginedll, EntryPoint = "OrderedHandles")]
		public static extern void OrderedHandles(Int64 model, IntPtr classCnt, IntPtr propertyCnt, IntPtr instanceCnt, Int64 setting, Int64 mask);

		/// <summary>
		///		PeelArray                                               (http://rdf.bg/gkdoc/CS64/PeelArray.html)
		///
		///	This function introduces functionality that is missing or complicated in some programming languages.
		///	The attribute inValue is a reference to an array of references. The attribute outValue is a reference to the same array,
		///	however a number of elements earlier or further, i.e. number of elements being attribute elementSize. Be aware that as
		///	we are talking about references the offset is depending on 32 bit / 64 bit compilation.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "PeelArray")]
		public static extern void PeelArray(ref IntPtr inValue, out IntPtr outValue, Int64 elementSize);

		/// <summary>
		///		SetInternalCheck                                        (http://rdf.bg/gkdoc/CS64/SetInternalCheck.html)
		///
		///	This function allows to enable or disable several active consistency checks. Enabling the checks can 
		///	introduce performance effects; it is helpfull for and meant for debugging on client side.
		///	If model is zero the consistency checks are set for all open and to be created models.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetInternalCheck")]
		public static extern void SetInternalCheck(Int64 model, Int64 setting, Int64 mask);

		/// <summary>
		///		GetInternalCheck                                        (http://rdf.bg/gkdoc/CS64/GetInternalCheck.html)
		///
		///	This function returns all current enabled active consistency checks given the mask the function is 
		///	called for.
		///	When leaving mask and settinbg zero it will return all bits that can be set.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInternalCheck")]
		public static extern Int64 GetInternalCheck(Int64 model, Int64 mask);

		/// <summary>
		///		GetInternalCheckIssueCnt                                (http://rdf.bg/gkdoc/CS64/GetInternalCheckIssueCnt.html)
		///
		///	This function returns all issues found and not retrieved by the hosting application through 
		///	GetInternalCheckIssue() / GetInternalCheckIssueW().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInternalCheckIssueCnt")]
		public static extern Int64 GetInternalCheckIssueCnt(Int64 model);

		/// <summary>
		///		GetInternalCheckIssue                                   (http://rdf.bg/gkdoc/CS64/GetInternalCheckIssue.html)
		///
		///	This function returns the oldest issues in the list of issues and reduces the list of issues with 1.
		///	The name and description represent the issue as ASCII string, if relevant the relating owlInstance
		///	will be returned through relatedOwlInstance.
		///	Namer, Description and relatedOwlInstance are optional.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInternalCheckIssue")]
		public static extern void GetInternalCheckIssue(Int64 model, out IntPtr name, out IntPtr description, out Int64 relatedOwlInstance);

		/// <summary>
		///		GetInternalCheckIssueW                                  (http://rdf.bg/gkdoc/CS64/GetInternalCheckIssueW.html)
		///
		///	This function returns the oldest issues in the list of issues and reduces the list of issues with 1.
		///	The name and description represent the issue as Unicode string, if relevant the relating owlInstance
		///	will be returned through relatedOwlInstance.
		///	Namer, Description and relatedOwlInstance are optional.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInternalCheckIssueW")]
		public static extern void GetInternalCheckIssueW(Int64 model, out IntPtr name, out IntPtr description, out Int64 relatedOwlInstance);

		/// <summary>
		///		CloseSession                                            (http://rdf.bg/gkdoc/CS64/CloseSession.html)
		///
		///	This function closes the session, after this call the geometry kernel cannot be used anymore.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CloseSession")]
		public static extern Int64 CloseSession();

		/// <summary>
		///		CleanMemory                                             (http://rdf.bg/gkdoc/CS64/CleanMemory.html)
		///
		///		This function ..
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CleanMemory")]
		public static extern void CleanMemory();

		/// <summary>
		///		ClearCache                                              (http://rdf.bg/gkdoc/CS64/ClearCache.html)
		///
		///		This function ..
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ClearCache")]
		public static extern void ClearCache(Int64 model);

		/// <summary>
		///		AllocModelMemory                                        (http://rdf.bg/gkdoc/CS64/AllocModelMemory.html)
		///
		///	Allocates model associated memory.
		///	Memory is disposed when model is closed
		/// </summary>
		[DllImport(enginedll, EntryPoint = "AllocModelMemory")]
		public static extern Int64 AllocModelMemory(Int64 model, Int64 size);

		/// <summary>
		///		SetExternalReferenceData                                (http://rdf.bg/gkdoc/CS64/SetExternalReferenceData.html)
		///
		///	Sets application data on model, class, property, instance
		///	Returns 0 on error, 1 on success
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetExternalReferenceData")]
		public static extern Int64 SetExternalReferenceData(Int64 rdfsResource, Int64 identifier, out IntPtr data);

		/// <summary>
		///		GetExternalReferenceData                                (http://rdf.bg/gkdoc/CS64/GetExternalReferenceData.html)
		///
		///	Gets application data from model, class, property, instance that were previosly set by SetExternalReferenceData
		///	Returns 0 on error, 1 on success
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetExternalReferenceData")]
		public static extern Int64 GetExternalReferenceData(Int64 rdfsResource, Int64 identifier);

		/// <summary>
		///		GetExternalReferenceDataId                              (http://rdf.bg/gkdoc/CS64/GetExternalReferenceDataId.html)
		///
		///	Returns a key id can be used in calls to Get/SetExternalReferenceData to keep application data on GK entities
		///	During model lifetime the id is the same for given string and different for different strings
		///	Returns 0 on error
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetExternalReferenceDataId")]
		public static extern Int64 GetExternalReferenceDataId(Int64 model, string uniqueAppName);

		[DllImport(enginedll, EntryPoint = "GetExternalReferenceDataId")]
		public static extern Int64 GetExternalReferenceDataId(Int64 model, byte[] uniqueAppName);

        //
        //  File IO / Stream / Copy API Calls
        //

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate Int64 ReadCallBackFunction(IntPtr value);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void WriteCallBackFunction(IntPtr value, Int64 size);

		/// <summary>
		///		CreateModel                                             (http://rdf.bg/gkdoc/CS64/CreateModel.html)
		///
		///	This function creates and empty model.
		///	References inside to other ontologies will be included.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateModel")]
		public static extern Int64 CreateModel();

		/// <summary>
		///		OpenModel                                               (http://rdf.bg/gkdoc/CS64/OpenModel.html)
		///
		///	This function opens the model on location file name.
		///	References inside to other ontologies will be included.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "OpenModel")]
		public static extern Int64 OpenModel(string fileName);

		[DllImport(enginedll, EntryPoint = "OpenModel")]
		public static extern Int64 OpenModel(byte[] fileName);

		/// <summary>
		///		OpenModelW                                              (http://rdf.bg/gkdoc/CS64/OpenModelW.html)
		///
		///	This function opens the model on location file name.
		///	References inside to other ontologies will be included.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "OpenModelW")]
		public static extern Int64 OpenModelW(string fileName);

		[DllImport(enginedll, EntryPoint = "OpenModelW")]
		public static extern Int64 OpenModelW(byte[] fileName);

		/// <summary>
		///		OpenModelS                                              (http://rdf.bg/gkdoc/CS64/OpenModelS.html)
		///
		///	This function opens the model via a stream.
		///	References inside to other ontologies will be included.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "OpenModelS")]
		public static extern Int64 OpenModelS([MarshalAs(UnmanagedType.FunctionPtr)] ReadCallBackFunction callback);

		/// <summary>
		///		OpenModelA                                              (http://rdf.bg/gkdoc/CS64/OpenModelA.html)
		///
		///	This function opens the model via an array.
		///	References inside to other ontologies will be included.
		///	A handle to the model will be returned, or 0 in case something went wrong.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "OpenModelA")]
		public static extern Int64 OpenModelA(byte[] content, Int64 size);

		/// <summary>
		///		ImportModel                                             (http://rdf.bg/gkdoc/CS64/ImportModel.html)
		///
		///	This function imports a design tree on location file name.
		///	The design tree will be added to the given existing model.
		///	The return value contains the first instance not referenced by any other instance or zero 
		///	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
		///	unique and equal to the instance used within the call SaveInstanceTree().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ImportModel")]
		public static extern Int64 ImportModel(Int64 model, string fileName);

		[DllImport(enginedll, EntryPoint = "ImportModel")]
		public static extern Int64 ImportModel(Int64 model, byte[] fileName);

		/// <summary>
		///		ImportModelW                                            (http://rdf.bg/gkdoc/CS64/ImportModelW.html)
		///
		///	This function imports a design tree on location file name.
		///	The design tree will be added to the given existing model.
		///	The return value contains the first instance not referenced by any other instance or zero 
		///	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
		///	unique and equal to the instance used within the call SaveInstanceTree().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ImportModelW")]
		public static extern Int64 ImportModelW(Int64 model, string fileName);

		[DllImport(enginedll, EntryPoint = "ImportModelW")]
		public static extern Int64 ImportModelW(Int64 model, byte[] fileName);

		/// <summary>
		///		ImportModelS                                            (http://rdf.bg/gkdoc/CS64/ImportModelS.html)
		///
		///	This function imports a design tree via a stream.
		///	The design tree will be added to the given existing model.
		///	The return value contains the first instance not referenced by any other instance or zero 
		///	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
		///	unique and equal to the instance used within the call SaveInstanceTree().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ImportModelS")]
		public static extern Int64 ImportModelS(Int64 model, [MarshalAs(UnmanagedType.FunctionPtr)] ReadCallBackFunction callback);

		/// <summary>
		///		ImportModelA                                            (http://rdf.bg/gkdoc/CS64/ImportModelA.html)
		///
		///	This function imports a design tree via an array.
		///	The design tree will be added to the given existing model.
		///	The return value contains the first instance not referenced by any other instance or zero 
		///	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
		///	unique and equal to the instance used within the call SaveInstanceTree().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ImportModelA")]
		public static extern Int64 ImportModelA(Int64 model, byte[] content, Int64 size);

		/// <summary>
		///		SaveInstanceTree                                        (http://rdf.bg/gkdoc/CS64/SaveInstanceTree.html)
		///
		///	This function saves the selected instance and its dependancies on location file name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceTree")]
		public static extern Int64 SaveInstanceTree(Int64 owlInstance, string fileName);

		[DllImport(enginedll, EntryPoint = "SaveInstanceTree")]
		public static extern Int64 SaveInstanceTree(Int64 owlInstance, byte[] fileName);

		/// <summary>
		///		SaveInstanceTreeW                                       (http://rdf.bg/gkdoc/CS64/SaveInstanceTreeW.html)
		///
		///	This function saves the selected instance and its dependancies on location file name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceTreeW")]
		public static extern Int64 SaveInstanceTreeW(Int64 owlInstance, string fileName);

		[DllImport(enginedll, EntryPoint = "SaveInstanceTreeW")]
		public static extern Int64 SaveInstanceTreeW(Int64 owlInstance, byte[] fileName);

		/// <summary>
		///		SaveInstanceTreeS                                       (http://rdf.bg/gkdoc/CS64/SaveInstanceTreeS.html)
		///
		///	This function saves the selected instance and its dependancies in a stream.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceTreeS")]
		public static extern Int64 SaveInstanceTreeS(Int64 owlInstance, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, Int64 size);

		/// <summary>
		///		SaveInstanceTreeA                                       (http://rdf.bg/gkdoc/CS64/SaveInstanceTreeA.html)
		///
		///	This function saves the selected instance and its dependancies in an array.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceTreeA")]
		public static extern Int64 SaveInstanceTreeA(Int64 owlInstance, byte[] content, out Int64 size);

		/// <summary>
		///		SaveInstanceNetwork                                     (http://rdf.bg/gkdoc/CS64/SaveInstanceNetwork.html)
		///
		///	This function saves the selected instance and its dependancies on location file name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceNetwork")]
		public static extern Int64 SaveInstanceNetwork(Int64 owlInstance, byte includeInverseRelations, string fileName);

		[DllImport(enginedll, EntryPoint = "SaveInstanceNetwork")]
		public static extern Int64 SaveInstanceNetwork(Int64 owlInstance, byte includeInverseRelations, byte[] fileName);

		/// <summary>
		///		SaveInstanceNetworkW                                    (http://rdf.bg/gkdoc/CS64/SaveInstanceNetworkW.html)
		///
		///	This function saves the selected instance and its dependancies on location file name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceNetworkW")]
		public static extern Int64 SaveInstanceNetworkW(Int64 owlInstance, byte includeInverseRelations, string fileName);

		[DllImport(enginedll, EntryPoint = "SaveInstanceNetworkW")]
		public static extern Int64 SaveInstanceNetworkW(Int64 owlInstance, byte includeInverseRelations, byte[] fileName);

		/// <summary>
		///		SaveInstanceNetworkS                                    (http://rdf.bg/gkdoc/CS64/SaveInstanceNetworkS.html)
		///
		///	This function saves the selected instance and its dependancies in a stream.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceNetworkS")]
		public static extern Int64 SaveInstanceNetworkS(Int64 owlInstance, byte includeInverseRelations, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, Int64 size);

		/// <summary>
		///		SaveInstanceNetworkA                                    (http://rdf.bg/gkdoc/CS64/SaveInstanceNetworkA.html)
		///
		///	This function saves the selected instance and its dependancies in an array.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveInstanceNetworkA")]
		public static extern Int64 SaveInstanceNetworkA(Int64 owlInstance, byte includeInverseRelations, byte[] content, out Int64 size);

		/// <summary>
		///		SaveModel                                               (http://rdf.bg/gkdoc/CS64/SaveModel.html)
		///
		///	This function saves the current model on location file name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveModel")]
		public static extern Int64 SaveModel(Int64 model, string fileName);

		[DllImport(enginedll, EntryPoint = "SaveModel")]
		public static extern Int64 SaveModel(Int64 model, byte[] fileName);

		/// <summary>
		///		SaveModelW                                              (http://rdf.bg/gkdoc/CS64/SaveModelW.html)
		///
		///	This function saves the current model on location file name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveModelW")]
		public static extern Int64 SaveModelW(Int64 model, string fileName);

		[DllImport(enginedll, EntryPoint = "SaveModelW")]
		public static extern Int64 SaveModelW(Int64 model, byte[] fileName);

		/// <summary>
		///		SaveModelS                                              (http://rdf.bg/gkdoc/CS64/SaveModelS.html)
		///
		///	This function saves the current model in a stream.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveModelS")]
		public static extern Int64 SaveModelS(Int64 model, [MarshalAs(UnmanagedType.FunctionPtr)] WriteCallBackFunction callback, Int64 size);

		/// <summary>
		///		SaveModelA                                              (http://rdf.bg/gkdoc/CS64/SaveModelA.html)
		///
		///	This function saves the current model in an array.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SaveModelA")]
		public static extern Int64 SaveModelA(Int64 model, byte[] content, out Int64 size);

		/// <summary>
		///		SetOverrideFileIO                                       (http://rdf.bg/gkdoc/CS64/SetOverrideFileIO.html)
		///
		///	This function overrides the type of file saved / exported independent of the extension given.
		///	By default the extension of the file name will define the type saved / exported:
		///		.rdf => generated RDF serialized content
		///		.ttl => generated TTL serialized content
		///		.bin => generated BIN/X serialized content
		///
		///	Available formats
		///		RDF
		///		TTL
		///		BIN/L - readible but large BIN format
		///		BIN/S - Optimized Binary, only running within given revision 
		///		BIN/X - Optimized Binary, running in all revisions supporting BIN/X
		///
		///	Force file type (overrides extension), works only on save (open selects correct type automatically)
		///		bit0	bit1	bit2
		///		  0		  0		  0		[default] unset forced file type
		///		  0		  0		  1		RESERVED
		///		  0		  1		  0		TTL
		///		  0		  1		  1		RDF
		///		  1		  0		  0		BIN/X
		///		  1		  0		  1		BIN/S
		///		  1		  1		  0		RESERVED
		///		  1		  1		  1		BIN/L
		///
		///	Force exporting as Base64
		///		bit4
		///		  0		do not use Base64
		///		  1		use Base64 (only works for BIN/S and BIN/X), on other formats no effect
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetOverrideFileIO")]
		public static extern void SetOverrideFileIO(Int64 model, Int64 setting, Int64 mask);

		/// <summary>
		///		GetOverrideFileIO                                       (http://rdf.bg/gkdoc/CS64/GetOverrideFileIO.html)
		///
		///	This function get the current overrides for type of file saved / exported independent of the extension given.
		///	By default the extension of the file name will define the type saved / exported:
		///		.rdf => generated RDF serialized content
		///		.ttl => generated TTL serialized content
		///		.bin => generated BIN/X serialized content
		///
		///	Available formats
		///		RDF
		///		TTL
		///		BIN/L - readible but large BIN format
		///		BIN/S - Optimized Binary, only running within given revision 
		///		BIN/X - Optimized Binary, running in all revisions supporting BIN/X
		///
		///	Force file type (overrides extension), works only on save (open selects correct type automatically)
		///		bit0	bit1	bit2
		///		  0		  0		  0		[default] unset forced file type
		///		  0		  0		  1		RESERVED
		///		  0		  1		  0		TTL
		///		  0		  1		  1		RDF
		///		  1		  0		  0		BIN/X
		///		  1		  0		  1		BIN/S
		///		  1		  1		  0		RESERVED
		///		  1		  1		  1		BIN/L
		///
		///	Force exporting as Base64
		///		bit4
		///		  0		do not use Base64
		///		  1		use Base64 (only works for BIN/S and BIN/X), on other formats no effect
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetOverrideFileIO")]
		public static extern Int64 GetOverrideFileIO(Int64 model, Int64 mask);

		/// <summary>
		///		CopyInstanceTree                                        (http://rdf.bg/gkdoc/CS64/CopyInstanceTree.html)
		///
		///	This function copies the instance tree towards a new model.
		///	In case model is empty a new model will be created (the handle to this new model can be retrieved through
		///	the call GetModel() based on the return value of this call).
		///	The model can be any opem model, it can be zero (a new model will be created on-the-fly) and it can be
		///	the same model as the model owlInstance is defined within, in this case just a perfect copy of the
		///	original instance tree.
		///
		///	The return value is the handle to the copied owlInstance in the model of choice.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CopyInstanceTree")]
		public static extern Int64 CopyInstanceTree(Int64 owlInstance, Int64 targetModel);

		/// <summary>
		///		CopyInstanceNetwork                                     (http://rdf.bg/gkdoc/CS64/CopyInstanceNetwork.html)
		///
		///	This function copies the instance network towards a new model.
		///	An instance network is different from an instance tree in that it can contain 'loops', the performance
		///	from this call will be slower in case the tree / network is sparse.
		///	In case model is empty a new model will be created (the handle to this new model can be retrieved through
		///	the call GetModel() based on the return value of this call).
		///	The model can be any open model, it can be zero (a new model will be created on-the-fly) and it can be
		///	the same model as the model owlInstance is defined within, in this case just a perfect copy of the
		///	original instance tree.
		///
		///	In case it is known we are talking about a tree (i.e. no internal loops) and inverse relations can be ignored
		///	the call CopyInstanceTree is a better choice concerning performance.
		///
		///	The return value is the handle to the copied owlInstance in the model of choice.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CopyInstanceNetwork")]
		public static extern Int64 CopyInstanceNetwork(Int64 owlInstance, byte includeInverseRelations, Int64 targetModel);

		/// <summary>
		///		CopyModel                                               (http://rdf.bg/gkdoc/CS64/CopyModel.html)
		///
		///	This function copies the complete structure of a model towards another or new model.
		///	In case the targetModel is empty a new model will be created.
		///	The owlInstance array (values) and it's cardinality (card) can be empty, in case they are
		///	non-empty the values are expected to contain owlInstance handles referencing in the source model
		///	after a successful copy the values will be adjusted into values referencing the copied owl instances
		///	in the new model. the list of values does not have to be complete or even unique and can even be empty.
		///
		///	sourceModel is not allowed to be empty, targetModel however can be empty or even equal to the source model.
		///
		///	The return value is the targetModel or in case this value was empty the newly created model.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CopyModel")]
		public static extern Int64 CopyModel(Int64 sourceModel, Int64 targetModel, out Int64 values, Int64 card);

		/// <summary>
		///		CloseModel                                              (http://rdf.bg/gkdoc/CS64/CloseModel.html)
		///
		///	This function closes the model. After this call none of the instances and classes within the model
		///	can be used anymore, also garbage collection is not allowed anymore, in default compilation the
		///	model itself will be known in the kernel, however known to be disabled. Calls containing the model
		///	reference will be protected from crashing when called.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CloseModel")]
		public static extern Int64 CloseModel(Int64 model);

		/// <summary>
		///		IsModel                                                 (http://rdf.bg/gkdoc/CS64/IsModel.html)
		///
		///	Returns OwlModel if the argument rdfsResource is an actual active model. It returns 0 in all other cases,
		///	i.e. this could mean the model is already closed or the session is closed.
		///	It could also mean it represents a handle to another resource, for example a property, instance or class.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "IsModel")]
		public static extern Int64 IsModel(Int64 rdfsResource);

        //
        //  Design Tree Classes API Calls
        //

		/// <summary>
		///		CreateClass                                             (http://rdf.bg/gkdoc/CS64/CreateClass.html)
		///
		///	Returns a handle to an on the fly created class.
		///	If the model input is zero or not a model handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateClass")]
		public static extern Int64 CreateClass(Int64 model, string name);

		[DllImport(enginedll, EntryPoint = "CreateClass")]
		public static extern Int64 CreateClass(Int64 model, byte[] name);

		/// <summary>
		///		CreateClassW                                            (http://rdf.bg/gkdoc/CS64/CreateClassW.html)
		///
		///	Returns a handle to an on the fly created class.
		///	If the model input is zero or not a model handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateClassW")]
		public static extern Int64 CreateClassW(Int64 model, string name);

		[DllImport(enginedll, EntryPoint = "CreateClassW")]
		public static extern Int64 CreateClassW(Int64 model, byte[] name);

		/// <summary>
		///		GetClassByName                                          (http://rdf.bg/gkdoc/CS64/GetClassByName.html)
		///
		///	Returns a handle to the class as stored inside.
		///	When the class does not exist yet and the name is unique
		///	the class will be created on the fly and the handle will be returned.
		///	When the name is not unique and given to an instance, objectTypeProperty
		///	or dataTypeProperty 0 will be returned.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassByName")]
		public static extern Int64 GetClassByName(Int64 model, string name);

		[DllImport(enginedll, EntryPoint = "GetClassByName")]
		public static extern Int64 GetClassByName(Int64 model, byte[] name);

		/// <summary>
		///		GetClassByNameW                                         (http://rdf.bg/gkdoc/CS64/GetClassByNameW.html)
		///
		///	Returns a handle to the class as stored inside.
		///	When the class does not exist yet and the name is unique
		///	the class will be created on the fly and the handle will be returned.
		///	When the name is not unique and given to an instance, objectTypeProperty
		///	or dataTypeProperty 0 will be returned.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassByNameW")]
		public static extern Int64 GetClassByNameW(Int64 model, string name);

		[DllImport(enginedll, EntryPoint = "GetClassByNameW")]
		public static extern Int64 GetClassByNameW(Int64 model, byte[] name);

		/// <summary>
		///		GetClassesByIterator                                    (http://rdf.bg/gkdoc/CS64/GetClassesByIterator.html)
		///
		///	Returns a handle to an class.
		///	If input class is zero, the handle will point to the first relevant class.
		///	If all classes are past (or no relevant classes are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassesByIterator")]
		public static extern Int64 GetClassesByIterator(Int64 model, Int64 owlClass);

		/// <summary>
		///		SetClassParent                                          (http://rdf.bg/gkdoc/CS64/SetClassParent.html)
		///
		///	Defines (set/unset) the parent class of a given class. Multiple-inheritence is supported and behavior
		///	of parent classes is also inherited as well as cardinality restrictions on datatype properties and
		///	object properties (relations).
		///
		///	When set: it adds parentOwlClass as immediate parent of owlClass if and only if 
		///	parentOwlClass is not ancestor of owlClass and owlClass is not ancestor of parentOwlClass.
		///	Returns the same value as IsClassAncestor after the call.
		///
		///	When unset: it removes parentOwlClass from immediate parents and returns 1, 
		///	or retunrs 0 if parentOwlClass is not immediate parent
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetClassParent")]
		public static extern Int64 SetClassParent(Int64 owlClass, Int64 parentOwlClass, Int64 setting);

		/// <summary>
		///		SetClassParentEx                                        (http://rdf.bg/gkdoc/CS64/SetClassParentEx.html)
		///
		///	Defines (set/unset) the parent class of a given class. Multiple-inheritence is supported and behavior
		///	of parent classes is also inherited as well as cardinality restrictions on datatype properties and
		///	object properties (relations).
		///
		///	This call has the same behavior as SetClassParent, however needs to be
		///	used in case classes are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetClassParentEx")]
		public static extern Int64 SetClassParentEx(Int64 model, Int64 owlClass, Int64 parentOwlClass, Int64 setting);

		/// <summary>
		///		IsClassAncestor                                         (http://rdf.bg/gkdoc/CS64/IsClassAncestor.html)
		///
		///	Checks if the class has given ancestor
		///	Returns 0 if not or minimal generation number (1 for direct parent)
		/// </summary>
		[DllImport(enginedll, EntryPoint = "IsClassAncestor")]
		public static extern Int64 IsClassAncestor(Int64 owlClass, Int64 ancestorOwlClass);

		/// <summary>
		///		GetClassParentsByIterator                               (http://rdf.bg/gkdoc/CS64/GetClassParentsByIterator.html)
		///
		///	Returns the next parent of the class.
		///	If input parent is zero, the handle will point to the first relevant parent.
		///	If all parent are past (or no relevant parent are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassParentsByIterator")]
		public static extern Int64 GetClassParentsByIterator(Int64 owlClass, Int64 parentOwlClass);

		/// <summary>
		///		SetNameOfClass                                          (http://rdf.bg/gkdoc/CS64/SetNameOfClass.html)
		///
		///	Sets/updates the name of the class, if no error it returns 0.
		///	In case class does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfClass")]
		public static extern Int64 SetNameOfClass(Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfClass")]
		public static extern Int64 SetNameOfClass(Int64 owlClass, byte[] name);

		/// <summary>
		///		SetNameOfClassW                                         (http://rdf.bg/gkdoc/CS64/SetNameOfClassW.html)
		///
		///	Sets/updates the name of the class, if no error it returns 0.
		///	In case class does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfClassW")]
		public static extern Int64 SetNameOfClassW(Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfClassW")]
		public static extern Int64 SetNameOfClassW(Int64 owlClass, byte[] name);

		/// <summary>
		///		SetNameOfClassEx                                        (http://rdf.bg/gkdoc/CS64/SetNameOfClassEx.html)
		///
		///	Sets/updates the name of the class, if no error it returns 0.
		///	In case class does not exist it returns 1, when name cannot be updated 2.
		///
		///	This call has the same behavior as SetNameOfClass, however needs to be
		///	used in case classes are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfClassEx")]
		public static extern Int64 SetNameOfClassEx(Int64 model, Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfClassEx")]
		public static extern Int64 SetNameOfClassEx(Int64 model, Int64 owlClass, byte[] name);

		/// <summary>
		///		SetNameOfClassWEx                                       (http://rdf.bg/gkdoc/CS64/SetNameOfClassWEx.html)
		///
		///	Sets/updates the name of the class, if no error it returns 0.
		///	In case class does not exist it returns 1, when name cannot be updated 2.
		///
		///	This call has the same behavior as SetNameOfClassW, however needs to be
		///	used in case classes are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfClassWEx")]
		public static extern Int64 SetNameOfClassWEx(Int64 model, Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfClassWEx")]
		public static extern Int64 SetNameOfClassWEx(Int64 model, Int64 owlClass, byte[] name);

		/// <summary>
		///		GetNameOfClass                                          (http://rdf.bg/gkdoc/CS64/GetNameOfClass.html)
		///
		///	Returns the name of the class, if the class does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfClass")]
		public static extern IntPtr GetNameOfClass(Int64 owlClass, out IntPtr name);

		public static string GetNameOfClass(Int64 owlClass)
		{
			IntPtr name = IntPtr.Zero;
			GetNameOfClass(owlClass, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		GetNameOfClassW                                         (http://rdf.bg/gkdoc/CS64/GetNameOfClassW.html)
		///
		///	Returns the name of the class, if the class does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfClassW")]
		public static extern IntPtr GetNameOfClassW(Int64 owlClass, out IntPtr name);

		public static string GetNameOfClassW(Int64 owlClass)
		{
			IntPtr name = IntPtr.Zero;
			GetNameOfClassW(owlClass, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(name);
		}

		/// <summary>
		///		GetNameOfClassEx                                        (http://rdf.bg/gkdoc/CS64/GetNameOfClassEx.html)
		///
		///	Returns the name of the class, if the class does not exist it returns nullptr.
		///
		///	This call has the same behavior as GetNameOfClass, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfClassEx")]
		public static extern IntPtr GetNameOfClassEx(Int64 model, Int64 owlClass, out IntPtr name);

		public static string GetNameOfClassEx(Int64 model, Int64 owlClass)
		{
			IntPtr name = IntPtr.Zero;
			GetNameOfClassEx(model, owlClass, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		GetNameOfClassWEx                                       (http://rdf.bg/gkdoc/CS64/GetNameOfClassWEx.html)
		///
		///	Returns the name of the class, if the class does not exist it returns nullptr.
		///
		///	This call has the same behavior as GetNameOfClassW, however needs to be
		///	used in case classes are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfClassWEx")]
		public static extern IntPtr GetNameOfClassWEx(Int64 model, Int64 owlClass, out IntPtr name);

		public static string GetNameOfClassWEx(Int64 model, Int64 owlClass)
		{
			IntPtr name = IntPtr.Zero;
			GetNameOfClassWEx(model, owlClass, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(name);
		}

		/// <summary>
		///		SetClassPropertyCardinalityRestriction                  (http://rdf.bg/gkdoc/CS64/SetClassPropertyCardinalityRestriction.html)
		///
		///	This function sets the minCard and maxCard of a certain property in the context of a class.
		///	The cardinality of a property in an instance has to be between minCard and maxCard (as well 
		///	as within the cardinality restrictions as given by the property in context of any of its
		///	(indirect) parent classes).
		///	If undefined minCard and/or maxCard will be of value -1, this means
		///	for minCard that it is 0 and for maxCard it means infinity.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetClassPropertyCardinalityRestriction")]
		public static extern void SetClassPropertyCardinalityRestriction(Int64 owlClass, Int64 rdfProperty, Int64 minCard, Int64 maxCard);

		/// <summary>
		///		SetClassPropertyCardinalityRestrictionEx                (http://rdf.bg/gkdoc/CS64/SetClassPropertyCardinalityRestrictionEx.html)
		///
		///	This function sets the minCard and maxCard of a certain property in the context of a class.
		///	The cardinality of a property in an instance has to be between minCard and maxCard (as well 
		///	as within the cardinality restrictions as given by the property in context of any of its
		///	(indirect) parent classes).
		///	If undefined minCard and/or maxCard will be of value -1, this means
		///	for minCard that it is 0 and for maxCard it means infinity.
		///
		///	This call has the same behavior as SetClassPropertyCardinalityRestriction, however needs to be
		///	used in case classes or properties are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetClassPropertyCardinalityRestrictionEx")]
		public static extern void SetClassPropertyCardinalityRestrictionEx(Int64 model, Int64 owlClass, Int64 rdfProperty, Int64 minCard, Int64 maxCard);

		/// <summary>
		///		GetClassPropertyCardinalityRestriction                  (http://rdf.bg/gkdoc/CS64/GetClassPropertyCardinalityRestriction.html)
		///
		///	This function returns the minCard and maxCard of a certain
		///	property in the context of a class. The cardinality of a property in 
		///	an instance has to be between minCard and maxCard (as well as within the cardinality restrictions
		///	as given by the property in context of any of its (indirect) parent classes).
		///	If undefined minCard and/or maxCard will be of value -1, this means
		///	for minCard that it is 0 and for maxCard it means infinity.
		///
		///	Note: this function does not return inherited restrictions. The example shows how to retrieve
		///	this knowledge, as it is derived knowledge the call that used to be available is removed.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassPropertyCardinalityRestriction")]
		public static extern void GetClassPropertyCardinalityRestriction(Int64 owlClass, Int64 rdfProperty, out Int64 minCard, out Int64 maxCard);

		/// <summary>
		///		GetClassPropertyCardinalityRestrictionEx                (http://rdf.bg/gkdoc/CS64/GetClassPropertyCardinalityRestrictionEx.html)
		///
		///	This function returns the minCard and maxCard of a certain
		///	property in the context of a class. The cardinality of a property in 
		///	an instance has to be between minCard and maxCard (as well as within the cardinality restrictions
		///	as given by the property in context of any of its (indirect) parent classes).
		///	If undefined minCard and/or maxCard will be of value -1, this means
		///	for minCard that it is 0 and for maxCard it means infinity.
		///
		///	This call has the same behavior as GetClassPropertyCardinalityRestriction, however needs to be
		///	used in case classes or properties are exchanged as a successive series of integers.
		///
		///	Note: this function does not return inherited restrictions. The example shows how to retrieve
		///	this knowledge, as it is derived knowledge the call that used to be available is removed.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassPropertyCardinalityRestrictionEx")]
		public static extern void GetClassPropertyCardinalityRestrictionEx(Int64 model, Int64 owlClass, Int64 rdfProperty, out Int64 minCard, out Int64 maxCard);

		/// <summary>
		///		GetClassPropertyAggregatedCardinalityRestriction        (http://rdf.bg/gkdoc/CS64/GetClassPropertyAggregatedCardinalityRestriction.html)
		///
		///	This function returns the minCard and maxCard of a certain
		///	property in the context of a class. This function does return inherited restrictions.
		///	If undefined minCard and/or maxCard will be of value -1, this means
		///	for minCard that it is 0 and for maxCard it means infinity.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassPropertyAggregatedCardinalityRestriction")]
		public static extern void GetClassPropertyAggregatedCardinalityRestriction(Int64 owlClass, Int64 rdfProperty, out Int64 minCard, out Int64 maxCard);

		/// <summary>
		///		GetClassPropertyAggregatedCardinalityRestrictionEx      (http://rdf.bg/gkdoc/CS64/GetClassPropertyAggregatedCardinalityRestrictionEx.html)
		///
		///	This function returns the minCard and maxCard of a certain
		///	property in the context of a class. This function does return inherited restrictions.
		///	If undefined minCard and/or maxCard will be of value -1, this means
		///	for minCard that it is 0 and for maxCard it means infinity.
		///
		///	This call has the same behavior as GetClassPropertyAggregatedCardinalityRestriction, however needs to be
		///	used in case classes or properties are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetClassPropertyAggregatedCardinalityRestrictionEx")]
		public static extern void GetClassPropertyAggregatedCardinalityRestrictionEx(Int64 model, Int64 owlClass, Int64 rdfProperty, out Int64 minCard, out Int64 maxCard);

		/// <summary>
		///		GetGeometryClass                                        (http://rdf.bg/gkdoc/CS64/GetGeometryClass.html)
		///
		///	Returns non-zero if the owlClass is a geometry type. This call will return the input class
		///	for all classes initially available. It will return as well non-for all classes created by the
		///	user or loaded / imported through a model that (indirectly) inherit one of the
		///	original classes available. in this case it returns the original available class
		///	it inherits the behavior from.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetGeometryClass")]
		public static extern Int64 GetGeometryClass(Int64 owlClass);

		/// <summary>
		///		GetGeometryClassEx                                      (http://rdf.bg/gkdoc/CS64/GetGeometryClassEx.html)
		///
		///	Returns non-zero if the owlClass is a geometry type. This call will return the input class
		///	for all classes initially available. It will return as well non-for all classes created by the
		///	user or loaded / imported through a model that (indirectly) inherit one of the
		///	original classes available. in this case it returns the original available class
		///	it inherits the behavior from.
		///
		///	This call has the same behavior as GetGeometryClass, however needs to be
		///	used in case classes are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetGeometryClassEx")]
		public static extern Int64 GetGeometryClassEx(Int64 model, Int64 owlClass);

		/// <summary>
		///		IsClass                                                 (http://rdf.bg/gkdoc/CS64/IsClass.html)
		///
		///	Returns OwlClass if the argument rdfsResource is an actual active class in an active model. It returns 0 in all other cases,
		///	i.e. this could mean the model is already closed, the class is inactive or removed or the session is closed.
		///	It could also mean it represents a handle to another resource, for example a property, instance or model.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "IsClass")]
		public static extern Int64 IsClass(Int64 rdfsResource);

        //
        //  Design Tree Properties API Calls
        //

		/// <summary>
		///		CreateProperty                                          (http://rdf.bg/gkdoc/CS64/CreateProperty.html)
		///
		///	Returns a handle to an on the fly created property.
		///	If the model input is zero or not a model handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateProperty")]
		public static extern Int64 CreateProperty(Int64 model, Int64 rdfPropertyType, string name);

		[DllImport(enginedll, EntryPoint = "CreateProperty")]
		public static extern Int64 CreateProperty(Int64 model, Int64 rdfPropertyType, byte[] name);

		/// <summary>
		///		CreatePropertyW                                         (http://rdf.bg/gkdoc/CS64/CreatePropertyW.html)
		///
		///	Returns a handle to an on the fly created property.
		///	If the model input is zero or not a model handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreatePropertyW")]
		public static extern Int64 CreatePropertyW(Int64 model, Int64 rdfPropertyType, string name);

		[DllImport(enginedll, EntryPoint = "CreatePropertyW")]
		public static extern Int64 CreatePropertyW(Int64 model, Int64 rdfPropertyType, byte[] name);

		/// <summary>
		///		GetPropertyByName                                       (http://rdf.bg/gkdoc/CS64/GetPropertyByName.html)
		///
		///	Returns a handle to the objectTypeProperty or dataTypeProperty as stored inside.
		///	When the property does not exist yet and the name is unique
		///	the property will be created on-the-fly and the handle will be returned.
		///	When the name is not unique and given to a class or instance 0 will be returned.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertyByName")]
		public static extern Int64 GetPropertyByName(Int64 model, string name);

		[DllImport(enginedll, EntryPoint = "GetPropertyByName")]
		public static extern Int64 GetPropertyByName(Int64 model, byte[] name);

		/// <summary>
		///		GetPropertyByNameW                                      (http://rdf.bg/gkdoc/CS64/GetPropertyByNameW.html)
		///
		///	Returns a handle to the objectTypeProperty or dataTypeProperty as stored inside.
		///	When the property does not exist yet and the name is unique
		///	the property will be created on-the-fly and the handle will be returned.
		///	When the name is not unique and given to a class or instance 0 will be returned.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertyByNameW")]
		public static extern Int64 GetPropertyByNameW(Int64 model, string name);

		[DllImport(enginedll, EntryPoint = "GetPropertyByNameW")]
		public static extern Int64 GetPropertyByNameW(Int64 model, byte[] name);

		/// <summary>
		///		GetPropertiesByIterator                                 (http://rdf.bg/gkdoc/CS64/GetPropertiesByIterator.html)
		///
		///	Returns a handle to a property.
		///	If input property is zero, the handle will point to the first relevant property.
		///	If all properties are past (or no relevant properties are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertiesByIterator")]
		public static extern Int64 GetPropertiesByIterator(Int64 model, Int64 rdfProperty);

		/// <summary>
		///		SetPropertyRangeRestriction                             (http://rdf.bg/gkdoc/CS64/SetPropertyRangeRestriction.html)
		///
		///	Sets or unsets a specific owlClass as range restriction to an rdfProperty. The property is expected to
		///	be an objectp[roperty, i.e. relation.]
		///	If rdfProperty is not an object property this call has no effect.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetPropertyRangeRestriction")]
		public static extern void SetPropertyRangeRestriction(Int64 rdfProperty, Int64 owlClass, Int64 setting);

		/// <summary>
		///		SetPropertyRangeRestrictionEx                           (http://rdf.bg/gkdoc/CS64/SetPropertyRangeRestrictionEx.html)
		///
		///	Sets or unsets a specific owlClass as range restriction to an rdfProperty. The property is expected to
		///	be an objectp[roperty, i.e. relation.]
		///	If rdfProperty is not an object property this call has no effect.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetPropertyRangeRestrictionEx")]
		public static extern void SetPropertyRangeRestrictionEx(Int64 model, Int64 rdfProperty, Int64 owlClass, Int64 setting);

		/// <summary>
		///		GetRangeRestrictionsByIterator                          (http://rdf.bg/gkdoc/CS64/GetRangeRestrictionsByIterator.html)
		///
		///	Returns the next class the property is restricted to.
		///	If input class is zero, the handle will point to the first relevant class.
		///	If all classes are past (or no relevant classes are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetRangeRestrictionsByIterator")]
		public static extern Int64 GetRangeRestrictionsByIterator(Int64 rdfProperty, Int64 owlClass);

		/// <summary>
		///		GetRangeRestrictionsByIteratorEx                        (http://rdf.bg/gkdoc/CS64/GetRangeRestrictionsByIteratorEx.html)
		///
		///	Returns the next class the property is restricted to.
		///	If input class is zero, the handle will point to the first relevant class.
		///	If all classes are past (or no relevant classes are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetRangeRestrictionsByIteratorEx")]
		public static extern Int64 GetRangeRestrictionsByIteratorEx(Int64 model, Int64 rdfProperty, Int64 owlClass);

		/// <summary>
		///		GetPropertyParentsByIterator                            (http://rdf.bg/gkdoc/CS64/GetPropertyParentsByIterator.html)
		///
		///	Returns the next parent of the property.
		///	If input parent is zero, the handle will point to the first relevant parent.
		///	If all parent are past (or no relevant parent are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertyParentsByIterator")]
		public static extern Int64 GetPropertyParentsByIterator(Int64 rdfProperty, Int64 parentRdfProperty);

		/// <summary>
		///		SetNameOfProperty                                       (http://rdf.bg/gkdoc/CS64/SetNameOfProperty.html)
		///
		///	Sets/updates the name of the property, if no error it returns 0.
		///	In case property does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfProperty")]
		public static extern Int64 SetNameOfProperty(Int64 rdfProperty, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfProperty")]
		public static extern Int64 SetNameOfProperty(Int64 rdfProperty, byte[] name);

		/// <summary>
		///		SetNameOfPropertyW                                      (http://rdf.bg/gkdoc/CS64/SetNameOfPropertyW.html)
		///
		///	Sets/updates the name of the property, if no error it returns 0.
		///	In case property does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfPropertyW")]
		public static extern Int64 SetNameOfPropertyW(Int64 rdfProperty, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfPropertyW")]
		public static extern Int64 SetNameOfPropertyW(Int64 rdfProperty, byte[] name);

		/// <summary>
		///		SetNameOfPropertyEx                                     (http://rdf.bg/gkdoc/CS64/SetNameOfPropertyEx.html)
		///
		///	Sets/updates the name of the property, if no error it returns 0.
		///	In case property does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfPropertyEx")]
		public static extern Int64 SetNameOfPropertyEx(Int64 model, Int64 rdfProperty, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfPropertyEx")]
		public static extern Int64 SetNameOfPropertyEx(Int64 model, Int64 rdfProperty, byte[] name);

		/// <summary>
		///		SetNameOfPropertyWEx                                    (http://rdf.bg/gkdoc/CS64/SetNameOfPropertyWEx.html)
		///
		///	Sets/updates the name of the property, if no error it returns 0.
		///	In case property does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfPropertyWEx")]
		public static extern Int64 SetNameOfPropertyWEx(Int64 model, Int64 rdfProperty, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfPropertyWEx")]
		public static extern Int64 SetNameOfPropertyWEx(Int64 model, Int64 rdfProperty, byte[] name);

		/// <summary>
		///		GetNameOfProperty                                       (http://rdf.bg/gkdoc/CS64/GetNameOfProperty.html)
		///
		///	Returns the name of the property, if the property does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfProperty")]
		public static extern IntPtr GetNameOfProperty(Int64 rdfProperty, out IntPtr name);

		public static string GetNameOfProperty(Int64 rdfProperty)
		{
			IntPtr name = IntPtr.Zero;
			GetNameOfProperty(rdfProperty, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		GetNameOfPropertyW                                      (http://rdf.bg/gkdoc/CS64/GetNameOfPropertyW.html)
		///
		///	Returns the name of the property, if the property does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfPropertyW")]
		public static extern IntPtr GetNameOfPropertyW(Int64 rdfProperty, out IntPtr name);

		public static string GetNameOfPropertyW(Int64 rdfProperty)
		{
			IntPtr name = IntPtr.Zero;
			GetNameOfPropertyW(rdfProperty, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(name);
		}

		/// <summary>
		///		GetNameOfPropertyEx                                     (http://rdf.bg/gkdoc/CS64/GetNameOfPropertyEx.html)
		///
		///	Returns the name of the property, if the property does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfPropertyEx")]
		public static extern IntPtr GetNameOfPropertyEx(Int64 model, Int64 rdfProperty, out IntPtr name);

		public static string GetNameOfPropertyEx(Int64 model, Int64 rdfProperty)
		{
			IntPtr name = IntPtr.Zero;
			GetNameOfPropertyEx(model, rdfProperty, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		GetNameOfPropertyWEx                                    (http://rdf.bg/gkdoc/CS64/GetNameOfPropertyWEx.html)
		///
		///	Returns the name of the property, if the property does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfPropertyWEx")]
		public static extern IntPtr GetNameOfPropertyWEx(Int64 model, Int64 rdfProperty, out IntPtr name);

		public static string GetNameOfPropertyWEx(Int64 model, Int64 rdfProperty)
		{
			IntPtr name = IntPtr.Zero;
			GetNameOfPropertyWEx(model, rdfProperty, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(name);
		}

		/// <summary>
		///		SetPropertyType                                         (http://rdf.bg/gkdoc/CS64/SetPropertyType.html)
		///
		///	This function sets the type of the property. This is only allowed
		///	if the type of the property was not set before.
		///
		///	The following values are possible for propertyType:
		///			OBJECTPROPERTY_TYPE				The property is an Object Property
		///			DATATYPEPROPERTY_TYPE_BOOLEAN	The property is an Datatype Property of type Boolean
		///			DATATYPEPROPERTY_TYPE_CHAR		The property is an Datatype Property of type Char
		///			DATATYPEPROPERTY_TYPE_INTEGER	The property is an Datatype Property of type Integer
		///			DATATYPEPROPERTY_TYPE_DOUBLE	The property is an Datatype Property of type Double
		///	The return value of this call is GetPropertyType/Ex applied after applying
		///	the type, normally this corresponds with the propertyType requested
		///	to be set unless the property already has a different propertyType set before.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetPropertyType")]
		public static extern Int64 SetPropertyType(Int64 rdfProperty, Int64 propertyType);

		/// <summary>
		///		SetPropertyTypeEx                                       (http://rdf.bg/gkdoc/CS64/SetPropertyTypeEx.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetPropertyTypeEx")]
		public static extern Int64 SetPropertyTypeEx(Int64 model, Int64 rdfProperty, Int64 propertyType);

		/// <summary>
		///		GetPropertyType                                         (http://rdf.bg/gkdoc/CS64/GetPropertyType.html)
		///
		///	This function returns the type of the property.
		///	The following return values are possible:
		///		0								The property is not defined yet
		///		OBJECTPROPERTY_TYPE				The property is an Object Property
		///		DATATYPEPROPERTY_TYPE_BOOLEAN	The property is an Datatype Property of type Boolean
		///		DATATYPEPROPERTY_TYPE_CHAR		The property is an Datatype Property of type Char
		///		DATATYPEPROPERTY_TYPE_INTEGER	The property is an Datatype Property of type Integer
		///		DATATYPEPROPERTY_TYPE_DOUBLE	The property is an Datatype Property of type Double
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertyType")]
		public static extern Int64 GetPropertyType(Int64 rdfProperty);

		/// <summary>
		///		GetPropertyTypeEx                                       (http://rdf.bg/gkdoc/CS64/GetPropertyTypeEx.html)
		///
		///	This call has the same behavior as GetPropertyType, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertyTypeEx")]
		public static extern Int64 GetPropertyTypeEx(Int64 model, Int64 rdfProperty);

		/// <summary>
		///		RemoveProperty                                          (http://rdf.bg/gkdoc/CS64/RemoveProperty.html)
		///
		///	This call is named remove property instead of a at first sight more logical name delete property as all content depending on this property is not lost per se.
		///	Each properties having the removed property as a parent will now inherit ther parents of the removed property.
		///	All property values in the context of an instance will become property values of the parent property, or in case no parent property is defined the values are lost.
		///	The return value represents a bit set defining findings during the removal, if a clean removal with no side effects was possible the return value is 0. In all other cases 
		///	the following bits represent the findings during removal:
		///		bit 0:
		///			0	Iunput as expected
		///			1	Encountered an issue on input value, i.e. property was not recognized as property
		///		bit 1:
		///			0	No 'child' properties found
		///			1	Properties found that had this property as a parent, they are adjusted by inheriting directly removed properties parents if present
		///		bit 2:
		///			0	No instances found with value restrictions for this property
		///			1	Vertex does contain 3D point info
		///		bit 3:
		///			0	No instances found with values for this property
		///			1	Vertex does contain 3D normal vector info => if set, bit 4 will also be set
		///		bit 6:
		///			0	Vertex does not contain first 2D texture info
		///			1	Vertex does contain first 2D texture info
		///		bit 7:
		///			0	Vertex does not contain second 2D texture info
		///			1	Vertex does contain second 2D texture info => if set, bit 6 will also be set
		///
		///		0	The property is not defined yet
		///		1	The property is an Object Type Property
		///		2	The property is an Data Type Property of type Boolean
		///		3	The property is an Data Type Property of type Char
		///		4	The property is an Data Type Property of type Integer
		///		5	The property is an Data Type Property of type Double
		/// </summary>
		[DllImport(enginedll, EntryPoint = "RemoveProperty")]
		public static extern Int64 RemoveProperty(Int64 rdfProperty);

		/// <summary>
		///		RemovePropertyEx                                        (http://rdf.bg/gkdoc/CS64/RemovePropertyEx.html)
		///
		///	This call is named remove property instead of a at first sight more logical name delete property as all content depending on this property is not lost per se.
		///	Each properties having the removed property as a parent will now inherit ther parents of the removed property.
		///	All property values in the context of an instance will become property values of the parent property, or in case no parent property is defined the values are lost.
		///	The return value represents a bit set defining findings during the removal, if a clean removal with no side effects was possible the return value is 0. In all other cases 
		///	the following bits represent the findings during removal:
		///		bit 0:
		///			0	Iunput as expected
		///			1	Encountered an issue on input value, i.e. property was not recognized as property
		///		bit 1:
		///			0	No 'child' properties found
		///			1	Properties found that had this property as a parent, they are adjusted by inheriting directly removed properties parents if present
		///		bit 2:
		///			0	No instances found with value restrictions for this property
		///			1	Vertex does contain 3D point info
		///		bit 3:
		///			0	No instances found with values for this property
		///			1	Vertex does contain 3D normal vector info => if set, bit 4 will also be set
		///		bit 6:
		///			0	Vertex does not contain first 2D texture info
		///			1	Vertex does contain first 2D texture info
		///		bit 7:
		///			0	Vertex does not contain second 2D texture info
		///			1	Vertex does contain second 2D texture info => if set, bit 6 will also be set
		///
		///		0	The property is not defined yet
		///		1	The property is an Object Type Property
		///		2	The property is an Data Type Property of type Boolean
		///		3	The property is an Data Type Property of type Char
		///		4	The property is an Data Type Property of type Integer
		///		5	The property is an Data Type Property of type Double
		///
		///
		///	This call has the same behavior as RemoveProperty, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "RemovePropertyEx")]
		public static extern Int64 RemovePropertyEx(Int64 model, Int64 rdfProperty);

		/// <summary>
		///		IsProperty                                              (http://rdf.bg/gkdoc/CS64/IsProperty.html)
		///
		///	Returns RdfProperty if the argument rdfsResource is an actual active property in an active model. It returns 0 in all other cases,
		///	i.e. this could mean the model is already closed, the property is inactive or removed or the session is closed.
		///	It could also mean it represents a handle to another resource, for example a class, instance or model.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "IsProperty")]
		public static extern Int64 IsProperty(Int64 rdfsResource);

        //
        //  Design Tree Instances API Calls
        //

		/// <summary>
		///		CreateInstance                                          (http://rdf.bg/gkdoc/CS64/CreateInstance.html)
		///
		///	Returns a handle to an on the fly created instance.
		///	If the owlClass input is zero or not a class handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateInstance")]
		public static extern Int64 CreateInstance(Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "CreateInstance")]
		public static extern Int64 CreateInstance(Int64 owlClass, byte[] name);

		public static Int64 CreateInstance(Int64 owlClass)
		{
			string name = (string) null;
			return CreateInstance(owlClass, name);
		}

		/// <summary>
		///		CreateInstanceW                                         (http://rdf.bg/gkdoc/CS64/CreateInstanceW.html)
		///
		///	Returns a handle to an on the fly created instance.
		///	If the owlClass input is zero or not a class handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateInstanceW")]
		public static extern Int64 CreateInstanceW(Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "CreateInstanceW")]
		public static extern Int64 CreateInstanceW(Int64 owlClass, byte[] name);

		public static Int64 CreateInstanceW(Int64 owlClass)
		{
			string name = (string) null;
			return CreateInstanceW(owlClass, name);
		}

		/// <summary>
		///		CreateInstanceEx                                        (http://rdf.bg/gkdoc/CS64/CreateInstanceEx.html)
		///
		///	Returns a handle to an on the fly created instance.
		///	If the owlClass input is zero or not a class handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateInstanceEx")]
		public static extern Int64 CreateInstanceEx(Int64 model, Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "CreateInstanceEx")]
		public static extern Int64 CreateInstanceEx(Int64 model, Int64 owlClass, byte[] name);

		public static Int64 CreateInstanceEx(Int64 model, Int64 owlClass)
		{
			string name = (string) null;
			return CreateInstanceEx(model, owlClass, name);
		}

		/// <summary>
		///		CreateInstanceWEx                                       (http://rdf.bg/gkdoc/CS64/CreateInstanceWEx.html)
		///
		///	Returns a handle to an on the fly created instance.
		///	If the owlClass input is zero or not a class handle 0 will be returned,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateInstanceWEx")]
		public static extern Int64 CreateInstanceWEx(Int64 model, Int64 owlClass, string name);

		[DllImport(enginedll, EntryPoint = "CreateInstanceWEx")]
		public static extern Int64 CreateInstanceWEx(Int64 model, Int64 owlClass, byte[] name);

		public static Int64 CreateInstanceWEx(Int64 model, Int64 owlClass)
		{
			string name = (string) null;
			return CreateInstanceWEx(model, owlClass, name);
		}

		/// <summary>
		///		GetInstancesByIterator                                  (http://rdf.bg/gkdoc/CS64/GetInstancesByIterator.html)
		///
		///	Returns a handle to an instance.
		///	If input instance is zero, the handle will point to the first relevant instance.
		///	If all instances are past (or no relevant instances are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstancesByIterator")]
		public static extern Int64 GetInstancesByIterator(Int64 model, Int64 owlInstance);

		/// <summary>
		///		GetInstanceClass                                        (http://rdf.bg/gkdoc/CS64/GetInstanceClass.html)
		///
		///	Returns the handle to the class of which the instance is instantiated. In case the instance 
		///	is instantiated on more than one class it will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceClass")]
		public static extern Int64 GetInstanceClass(Int64 owlInstance);

		/// <summary>
		///		GetInstanceClassEx                                      (http://rdf.bg/gkdoc/CS64/GetInstanceClassEx.html)
		///
		///	Returns the handle to the class of which the instance is instantiated. In case the instance 
		///	is instantiated on more than one class it will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceClassEx")]
		public static extern Int64 GetInstanceClassEx(Int64 model, Int64 owlInstance);

		/// <summary>
		///		GetInstanceClassByIterator                              (http://rdf.bg/gkdoc/CS64/GetInstanceClassByIterator.html)
		///
		///	Iterates over the classes the instance is instantiated from.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceClassByIterator")]
		public static extern Int64 GetInstanceClassByIterator(Int64 owlInstance, Int64 owlClass);

		/// <summary>
		///		GetInstanceClassByIteratorEx                            (http://rdf.bg/gkdoc/CS64/GetInstanceClassByIteratorEx.html)
		///
		///	Iterates over the classes the instance is instantiated from.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceClassByIteratorEx")]
		public static extern Int64 GetInstanceClassByIteratorEx(Int64 model, Int64 owlInstance, Int64 owlClass);

		/// <summary>
		///		GetInstancePropertyCardinalityRestriction               (http://rdf.bg/gkdoc/CS64/GetInstancePropertyCardinalityRestriction.html)
		///
		///	...
		/// </summary>
		public static void GetInstancePropertyCardinalityRestriction(Int64 owlInstance, Int64 rdfProperty, out Int64 minCard, out Int64 maxCard)
		{
			Int64	owlClass = GetInstanceClassByIterator(owlInstance, 0);

			GetClassPropertyCardinalityRestriction(
					owlClass,
					rdfProperty,
					out minCard,
					out maxCard
				);

			owlClass = GetInstanceClassByIterator(owlInstance, owlClass);
			while (owlClass != 0)
			{
				Int64	myMinCard = -1,
						myMaxCard = -1;

				GetClassPropertyCardinalityRestriction(
						owlClass,
						rdfProperty,
						out myMinCard,
						out myMaxCard
					);

				if (minCard < myMinCard)
					minCard = myMinCard;

				if (myMaxCard >= 0 &&
					(maxCard == -1 || maxCard > myMaxCard))
					maxCard = myMaxCard;

				owlClass = GetInstanceClassByIterator(owlInstance, owlClass);
			}
		}

		/// <summary>
		///		GetInstanceGeometryClass                                (http://rdf.bg/gkdoc/CS64/GetInstanceGeometryClass.html)
		///
		///	If one of the classes this instance is instantiated from or one of its parents is a geometry class,
		///	this class is returned. In all other cases the return value is 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceGeometryClass")]
		public static extern Int64 GetInstanceGeometryClass(Int64 owlInstance);

		/// <summary>
		///		GetInstanceGeometryClassEx                              (http://rdf.bg/gkdoc/CS64/GetInstanceGeometryClassEx.html)
		///
		///	If one of the classes this instance is instantiated from or one of its parents is a geometry class,
		///	this class is returned. In all other cases the return value is 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceGeometryClassEx")]
		public static extern Int64 GetInstanceGeometryClassEx(Int64 model, Int64 owlInstance);

		/// <summary>
		///		GetInstancePropertyByIterator                           (http://rdf.bg/gkdoc/CS64/GetInstancePropertyByIterator.html)
		///
		///	Returns a handle to the objectTypeProperty or dataTypeProperty connected to
		///	the instance, this property can also contain a value, but for example also
		///	the knowledge about cardinality restrictions in the context of this instance's class
		///	and the exact cardinality in context of its instance.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstancePropertyByIterator")]
		public static extern Int64 GetInstancePropertyByIterator(Int64 owlInstance, Int64 rdfProperty);

		/// <summary>
		///		GetInstancePropertyByIteratorEx                         (http://rdf.bg/gkdoc/CS64/GetInstancePropertyByIteratorEx.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstancePropertyByIteratorEx")]
		public static extern Int64 GetInstancePropertyByIteratorEx(Int64 model, Int64 owlInstance, Int64 rdfProperty);

		/// <summary>
		///		GetInstanceInverseReferencesByIterator                  (http://rdf.bg/gkdoc/CS64/GetInstanceInverseReferencesByIterator.html)
		///
		///	Returns a handle to the owlInstances refering this instance
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceInverseReferencesByIterator")]
		public static extern Int64 GetInstanceInverseReferencesByIterator(Int64 owlInstance, Int64 referencingOwlInstance);

		/// <summary>
		///		GetInstanceReferencesByIterator                         (http://rdf.bg/gkdoc/CS64/GetInstanceReferencesByIterator.html)
		///
		///	Returns a handle to the owlInstance refered by this instance
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetInstanceReferencesByIterator")]
		public static extern Int64 GetInstanceReferencesByIterator(Int64 owlInstance, Int64 referencedOwlInstance);

		/// <summary>
		///		SetNameOfInstance                                       (http://rdf.bg/gkdoc/CS64/SetNameOfInstance.html)
		///
		///	Sets/updates the name of the instance, if no error it returns 0.
		///	In case instance does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfInstance")]
		public static extern Int64 SetNameOfInstance(Int64 owlInstance, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfInstance")]
		public static extern Int64 SetNameOfInstance(Int64 owlInstance, byte[] name);

		/// <summary>
		///		SetNameOfInstanceW                                      (http://rdf.bg/gkdoc/CS64/SetNameOfInstanceW.html)
		///
		///	Sets/updates the name of the instance, if no error it returns 0.
		///	In case instance does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfInstanceW")]
		public static extern Int64 SetNameOfInstanceW(Int64 owlInstance, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfInstanceW")]
		public static extern Int64 SetNameOfInstanceW(Int64 owlInstance, byte[] name);

		/// <summary>
		///		SetNameOfInstanceEx                                     (http://rdf.bg/gkdoc/CS64/SetNameOfInstanceEx.html)
		///
		///	Sets/updates the name of the instance, if no error it returns 0.
		///	In case instance does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfInstanceEx")]
		public static extern Int64 SetNameOfInstanceEx(Int64 model, Int64 owlInstance, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfInstanceEx")]
		public static extern Int64 SetNameOfInstanceEx(Int64 model, Int64 owlInstance, byte[] name);

		/// <summary>
		///		SetNameOfInstanceWEx                                    (http://rdf.bg/gkdoc/CS64/SetNameOfInstanceWEx.html)
		///
		///	Sets/updates the name of the instance, if no error it returns 0.
		///	In case instance does not exist it returns 1, when name cannot be updated 2.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetNameOfInstanceWEx")]
		public static extern Int64 SetNameOfInstanceWEx(Int64 model, Int64 owlInstance, string name);

		[DllImport(enginedll, EntryPoint = "SetNameOfInstanceWEx")]
		public static extern Int64 SetNameOfInstanceWEx(Int64 model, Int64 owlInstance, byte[] name);

		/// <summary>
		///		GetNameOfInstance                                       (http://rdf.bg/gkdoc/CS64/GetNameOfInstance.html)
		///
		///	Returns the name of the instance, if the instance does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfInstance")]
		public static extern IntPtr GetNameOfInstance(Int64 owlInstance, out IntPtr name);

		public static string GetNameOfInstance(Int64 owlInstance)
		{
			IntPtr name = IntPtr.Zero;
			GetNameOfInstance(owlInstance, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		GetNameOfInstanceW                                      (http://rdf.bg/gkdoc/CS64/GetNameOfInstanceW.html)
		///
		///	Returns the name of the instance, if the instance does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfInstanceW")]
		public static extern IntPtr GetNameOfInstanceW(Int64 owlInstance, out IntPtr name);

		public static string GetNameOfInstanceW(Int64 owlInstance)
		{
			IntPtr name = IntPtr.Zero;
			GetNameOfInstanceW(owlInstance, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(name);
		}

		/// <summary>
		///		GetNameOfInstanceEx                                     (http://rdf.bg/gkdoc/CS64/GetNameOfInstanceEx.html)
		///
		///	Returns the name of the instance, if the instance does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfInstanceEx")]
		public static extern IntPtr GetNameOfInstanceEx(Int64 model, Int64 owlInstance, out IntPtr name);

		public static string GetNameOfInstanceEx(Int64 model, Int64 owlInstance)
		{
			IntPtr name = IntPtr.Zero;
			GetNameOfInstanceEx(model, owlInstance, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
		}

		/// <summary>
		///		GetNameOfInstanceWEx                                    (http://rdf.bg/gkdoc/CS64/GetNameOfInstanceWEx.html)
		///
		///	Returns the name of the instance, if the instance does not exist it returns nullptr.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetNameOfInstanceWEx")]
		public static extern IntPtr GetNameOfInstanceWEx(Int64 model, Int64 owlInstance, out IntPtr name);

		public static string GetNameOfInstanceWEx(Int64 model, Int64 owlInstance)
		{
			IntPtr name = IntPtr.Zero;
			GetNameOfInstanceWEx(model, owlInstance, out name);
			return System.Runtime.InteropServices.Marshal.PtrToStringUni(name);
		}

		/// <summary>
		///		SetDatatypeProperty                                     (http://rdf.bg/gkdoc/CS64/SetDatatypeProperty.html)
		///
		///	This function sets the value(s) of a certain datatypeTypeProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of undefined (void) items is a list of booleans, chars, integers
		///	or doubles, this list has a length as givin in the values card. The actual used type
		///	is given by the definition of the dataTypeProperty.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		///
		///	Note: the client application needs to make sure the cardinality of
		///		  the property is within the boundaries.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, ref byte values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, byte[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, ref Int64 values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, Int64[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, ref double values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, double[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, ref string values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypeProperty")]
		public static extern Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, string[] values, Int64 card);

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, bool value)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_BOOLEAN);
			const Int64 card = 1;
			byte value_inByte = Convert.ToByte(value);
			return SetDatatypeProperty(owlInstance, owlDatatypeProperty, ref value_inByte, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, byte value)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_BYTE);
			const Int64 card = 1;
			return SetDatatypeProperty(owlInstance, owlDatatypeProperty, ref value, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, Int64 value)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_INTEGER);
			const Int64 card = 1;
			return SetDatatypeProperty(owlInstance, owlDatatypeProperty, ref value, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, double value)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_DOUBLE);
			const Int64 card = 1;
			return SetDatatypeProperty(owlInstance, owlDatatypeProperty, ref value, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, string value)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_CHAR);
			const Int64 card = 1;
			return SetDatatypeProperty(owlInstance, owlDatatypeProperty, ref value, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, bool[] values)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_BOOLEAN);
			Int64 card = values.Length;
			byte[] values_inByte = values.Select((v) =>
				{
					return Convert.ToByte(v);
				}).ToArray();
			return SetDatatypeProperty(owlInstance, owlDatatypeProperty, values_inByte, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, byte[] values)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_BYTE);
			Int64 card = values.Length;
			return SetDatatypeProperty(owlInstance, owlDatatypeProperty, values, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, Int64[] values)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_INTEGER);
			Int64 card = values.Length;
			return SetDatatypeProperty(owlInstance, owlDatatypeProperty, values, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, double[] values)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_DOUBLE);
			Int64 card = values.Length;
			return SetDatatypeProperty(owlInstance, owlDatatypeProperty, values, card);
		}

		public static Int64 SetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, string[] values)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_CHAR);
			Int64 card = values.Length;
			return SetDatatypeProperty(owlInstance, owlDatatypeProperty, values, card);
		}

		/// <summary>
		///		SetDatatypePropertyEx                                   (http://rdf.bg/gkdoc/CS64/SetDatatypePropertyEx.html)
		///
		///	This function sets the value(s) of a certain datatypeTypeProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of undefined (void) items is a list of booleans, chars, integers
		///	or doubles, this list has a length as givin in the values card. The actual used type
		///	is given by the definition of the dataTypeProperty.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		///
		///	This call has the same behavior as SetDatatypeProperty, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		///
		///	Note: the client application needs to make sure the cardinality of
		///		  the property is within the boundaries.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 owlDatatypeProperty, ref byte values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 owlDatatypeProperty, byte[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 owlDatatypeProperty, ref Int64 values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 owlDatatypeProperty, Int64[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 owlDatatypeProperty, ref double values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 owlDatatypeProperty, double[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 owlDatatypeProperty, ref string values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDatatypePropertyEx")]
		public static extern Int64 SetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 owlDatatypeProperty, string[] values, Int64 card);

		/// <summary>
		///		GetDatatypeProperty                                     (http://rdf.bg/gkdoc/CS64/GetDatatypeProperty.html)
		///
		///	This function gets the value(s) of a certain datatypeTypeProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of undefined (void) items is a list of booleans, chars, integers
		///	or doubles, this list has a length as givin in the value card. The actual used type
		///	is given by the definition of the dataTypeProperty.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetDatatypeProperty")]
		public static extern Int64 GetDatatypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, out IntPtr values, out Int64 card);

		public static bool[] GetDatatypeProperty_inBool(Int64 owlInstance, Int64 owlDatatypeProperty)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_BOOLEAN);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			GetDatatypeProperty(owlInstance, owlDatatypeProperty, out valuesPtr, out card);

			if (card > 0)
			{
				byte[] values_inByte = new byte[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values_inByte, 0, (int) card);
				bool[] values = values_inByte.Select((v) =>
				{
					return v != 0;
				}).ToArray();
				return values;
			}

			return null;
		}

		public static byte[] GetDatatypeProperty_inByte(Int64 owlInstance, Int64 owlDatatypeProperty)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_BYTE);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			GetDatatypeProperty(owlInstance, owlDatatypeProperty, out valuesPtr, out card);

			if (card > 0)
			{
				byte[] values = new byte[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
				return values;
			}

			return null;
		}

		public static Int64[] GetDatatypeProperty_inInt64(Int64 owlInstance, Int64 owlDatatypeProperty)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_INTEGER);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			GetDatatypeProperty(owlInstance, owlDatatypeProperty, out valuesPtr, out card);

			if (card > 0)
			{
				Int64[] values = new Int64[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
				return values;
			}

			return null;
		}

		public static double[] GetDatatypeProperty_inDouble(Int64 owlInstance, Int64 owlDatatypeProperty)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_DOUBLE);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			GetDatatypeProperty(owlInstance, owlDatatypeProperty, out valuesPtr, out card);

			if (card > 0)
			{
				double[] values = new double[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
				return values;
			}

			return null;
		}

		public static string[] GetDatatypeProperty_inString(Int64 owlInstance, Int64 owlDatatypeProperty)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_CHAR);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			GetDatatypeProperty(owlInstance, owlDatatypeProperty, out valuesPtr, out card);

			if (card > 0)
			{
				IntPtr[] valuesRef = new IntPtr[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, valuesRef, 0, (int) card);

				string[] values = new string[card];
				for (int i = 0; i < (int) card; i++)
				{
					values[i] = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(valuesRef[i]);
				}

				return values;
			}

			return null;
		}

		/// <summary>
		///		GetDatatypePropertyEx                                   (http://rdf.bg/gkdoc/CS64/GetDatatypePropertyEx.html)
		///
		///	This function gets the value(s) of a certain datatypeTypeProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of undefined (void) items is a list of booleans, chars, integers
		///	or doubles, this list has a length as givin in the value card. The actual used type
		///	is given by the definition of the dataTypeProperty.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		///
		///	This call has the same behavior as GetDatatypeProperty, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetDatatypePropertyEx")]
		public static extern Int64 GetDatatypePropertyEx(Int64 model, Int64 owlInstance, Int64 owlDatatypeProperty, out IntPtr values, out Int64 card);

		/// <summary>
		///		SetObjectProperty                                       (http://rdf.bg/gkdoc/CS64/SetObjectProperty.html)
		///
		///	This function sets the value(s) of a certain objectTypeProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of integers is a list of handles to instances, this list
		///	has a length as givin in the values card.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		///
		///	Note: the client application needs to make sure the cardinality of
		///		  the property is within the boundaries.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetObjectProperty")]
		public static extern Int64 SetObjectProperty(Int64 owlInstance, Int64 owlObjectProperty, ref Int64 values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetObjectProperty")]
		public static extern Int64 SetObjectProperty(Int64 owlInstance, Int64 owlObjectProperty, Int64[] values, Int64 card);

		public static Int64 SetObjectProperty(Int64 owlInstance, Int64 owlObjectProperty, Int64 value)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlObjectProperty) == OBJECTPROPERTY_TYPE);
			const Int64	card = 1;
			return SetObjectProperty(owlInstance, owlObjectProperty, ref value, card);
		}

		public static Int64 SetObjectProperty(Int64 owlInstance, Int64 owlObjectProperty, Int64[] values)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlObjectProperty) == OBJECTPROPERTY_TYPE);
			Int64 card = values.Length;
			return SetObjectProperty(owlInstance, owlObjectProperty, values, card);
		}

		/// <summary>
		///		SetObjectPropertyEx                                     (http://rdf.bg/gkdoc/CS64/SetObjectPropertyEx.html)
		///
		///	This function sets the value(s) of a certain objectTypeProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of integers is a list of handles to instances, this list
		///	has a length as givin in the values card.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		///
		///	This call has the same behavior as SetObjectProperty, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		///
		///	Note: the client application needs to make sure the cardinality of
		///		  the property is within the boundaries.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetObjectPropertyEx")]
		public static extern Int64 SetObjectPropertyEx(Int64 model, Int64 owlInstance, Int64 owlObjectProperty, ref Int64 values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetObjectPropertyEx")]
		public static extern Int64 SetObjectPropertyEx(Int64 model, Int64 owlInstance, Int64 owlObjectProperty, Int64[] values, Int64 card);

		/// <summary>
		///		GetObjectProperty                                       (http://rdf.bg/gkdoc/CS64/GetObjectProperty.html)
		///
		///	This function gets the value(s) of a certain objectProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of integers is a list of handles to instances, this list
		///	has a length as givin in the value card.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetObjectProperty")]
		public static extern Int64 GetObjectProperty(Int64 owlInstance, Int64 owlObjectProperty, out IntPtr values, out Int64 card);

		public static Int64[] GetObjectProperty(Int64 owlInstance, Int64 owlObjectProperty)
		{
			System.Diagnostics.Debug.Assert(GetPropertyType(owlObjectProperty) == OBJECTPROPERTY_TYPE);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			GetObjectProperty(owlInstance, owlObjectProperty, out valuesPtr, out card);

			if (card > 0)
			{
				Int64[] values = new Int64[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
				return values;
			}

			return null;
		}

		/// <summary>
		///		GetObjectPropertyEx                                     (http://rdf.bg/gkdoc/CS64/GetObjectPropertyEx.html)
		///
		///	This function gets the value(s) of a certain objectProperty
		///	in the context of an instance.
		///	The value of card gives the actual card of the values list.
		///	The list values of integers is a list of handles to instances, this list
		///	has a length as givin in the values card.
		///	The return value always should be 0, if not something is wrong in the way this property is called.
		///
		///	This call has the same behavior as GetObjectProperty, however needs to be
		///	used in case properties are exchanged as a successive series of integers.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetObjectPropertyEx")]
		public static extern Int64 GetObjectPropertyEx(Int64 model, Int64 owlInstance, Int64 owlObjectProperty, out IntPtr values, out Int64 card);

		/// <summary>
		///		CreateInstanceInContextStructure                        (http://rdf.bg/gkdoc/CS64/CreateInstanceInContextStructure.html)
		///
		///	InstanceInContext structures give you more detailed information about
		///	individual parts of the geometry of a certain instance viualized.
		///	It is allowed to have more then 1 InstanceInContext structures per instance.
		///	InstanceInContext structures are updated dynamically when the geometry
		///	structure is updated.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CreateInstanceInContextStructure")]
		public static extern Int64 CreateInstanceInContextStructure(Int64 owlInstance);

		/// <summary>
		///		DestroyInstanceInContextStructure                       (http://rdf.bg/gkdoc/CS64/DestroyInstanceInContextStructure.html)
		///
		///	InstanceInContext structures are updated dynamically and therfore even while the cost
		///	in performance and memory is limited it is advised to destroy structures as soon
		///	as they are obsolete.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "DestroyInstanceInContextStructure")]
		public static extern void DestroyInstanceInContextStructure(Int64 owlInstanceInContext);

		/// <summary>
		///		InstanceInContextChild                                  (http://rdf.bg/gkdoc/CS64/InstanceInContextChild.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "InstanceInContextChild")]
		public static extern Int64 InstanceInContextChild(Int64 owlInstanceInContext);

		/// <summary>
		///		InstanceInContextNext                                   (http://rdf.bg/gkdoc/CS64/InstanceInContextNext.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "InstanceInContextNext")]
		public static extern Int64 InstanceInContextNext(Int64 owlInstanceInContext);

		/// <summary>
		///		InstanceInContextIsUpdated                              (http://rdf.bg/gkdoc/CS64/InstanceInContextIsUpdated.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "InstanceInContextIsUpdated")]
		public static extern Int64 InstanceInContextIsUpdated(Int64 owlInstanceInContext);

		/// <summary>
		///		RemoveInstance                                          (http://rdf.bg/gkdoc/CS64/RemoveInstance.html)
		///
		///	This function removes an instance from the internal structure.
		///	In case copies are created by the host this function checks if all
		///	copies are removed otherwise the instance will stay available.
		///	Return value is 0 if everything went ok and positive in case of an error
		/// </summary>
		[DllImport(enginedll, EntryPoint = "RemoveInstance")]
		public static extern Int64 RemoveInstance(Int64 owlInstance);

		/// <summary>
		///		RemoveInstanceRecursively                               (http://rdf.bg/gkdoc/CS64/RemoveInstanceRecursively.html)
		///
		///	This function removes an instance recursively from the internal structure.
		///	In case checkInverseRelations is non-zero only instances that are not referenced
		///	by other existing instances.
		///
		///	Return value is total number of removed instances
		/// </summary>
		[DllImport(enginedll, EntryPoint = "RemoveInstanceRecursively")]
		public static extern Int64 RemoveInstanceRecursively(Int64 owlInstance);

		/// <summary>
		///		RemoveInstances                                         (http://rdf.bg/gkdoc/CS64/RemoveInstances.html)
		///
		///	This function removes all available instances for the given model 
		///	from the internal structure.
		///	Return value is the number of removed instances.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "RemoveInstances")]
		public static extern Int64 RemoveInstances(Int64 model);

		/// <summary>
		///		IsInstance                                              (http://rdf.bg/gkdoc/CS64/IsInstance.html)
		///
		///	Returns OwlInstance if the argument rdfsResource is an actual active instance in an active model. It returns 0 in all other cases,
		///	i.e. this could mean the model is already closed, the instance is inactive or removed or the session is closed.
		///	It could also mean it represents a handle to another resource, for example a class, property or model.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "IsInstance")]
		public static extern Int64 IsInstance(Int64 rdfsResource);

		/// <summary>
		///		IsKindOfClass                                           (http://rdf.bg/gkdoc/CS64/IsKindOfClass.html)
		///
		///	...
		/// </summary>
		public static bool IsKindOfClass(Int64 myOwlClass, Int64 owlClass)
		{
			if (myOwlClass == owlClass)
				return	true;
			Int64	parentOwlClass = GetClassParentsByIterator(myOwlClass, 0);
			while (parentOwlClass != 0)
			{
				if (IsKindOfClass(parentOwlClass, owlClass))
					return	true;
				parentOwlClass = GetClassParentsByIterator(myOwlClass, parentOwlClass);
			}
			return	false;
		}

		/// <summary>
		///		IsInstanceOfClass                                       (http://rdf.bg/gkdoc/CS64/IsInstanceOfClass.html)
		///
		///	...
		/// </summary>
		public static bool IsInstanceOfClass(Int64 owlInstance, string name)
		{
			return IsKindOfClass(GetInstanceClass(owlInstance), GetClassByName(GetModel(owlInstance), name));
		}

		public static bool IsInstanceOfClass(Int64 owlInstance, byte[] name)
		{
			return IsKindOfClass(GetInstanceClass(owlInstance), GetClassByName(GetModel(owlInstance), name));
		}

		/// <summary>
		///		IsInstanceOfClassExact                                  (http://rdf.bg/gkdoc/CS64/IsInstanceOfClassExact.html)
		///
		///	...
		/// </summary>
		public static bool IsInstanceOfClassExact(Int64 owlInstance, string name)
		{
			return GetInstanceClass(owlInstance) == GetClassByName(GetModel(owlInstance), name);
		}

		public static bool IsInstanceOfClassExact(Int64 owlInstance, byte[] name)
		{
			return GetInstanceClass(owlInstance) == GetClassByName(GetModel(owlInstance), name);
		}

        //
        //  Retrieve Geometry API Calls
        //

		/// <summary>
		///		CalculateInstance                                       (http://rdf.bg/gkdoc/CS64/CalculateInstance.html)
		///
		///	This function prepares the content to be ready so the buffers can be filled.
		///	It returns the minimum size the buffers should be. This is only the case
		///	when the pointer is given, all arguments are allowed to be nullptr.
		///
		///	Note: This function needs to be called directly before UpdateVertexBuffer(),
		///		  UpdateIndexBuffer() and UpdateTransformationBuffer().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CalculateInstance")]
		public static extern Int64 CalculateInstance(Int64 owlInstance, out Int64 vertexBufferSize, out Int64 indexBufferSize, out Int64 transformationBufferSize);

		[DllImport(enginedll, EntryPoint = "CalculateInstance")]
		public static extern Int64 CalculateInstance(Int64 owlInstance, out Int64 vertexBufferSize, out Int64 indexBufferSize, IntPtr transformationBufferSize);

		[DllImport(enginedll, EntryPoint = "CalculateInstance")]
		public static extern Int64 CalculateInstance(Int64 owlInstance, out Int64 vertexBufferSize, IntPtr indexBufferSize, out Int64 transformationBufferSize);

		[DllImport(enginedll, EntryPoint = "CalculateInstance")]
		public static extern Int64 CalculateInstance(Int64 owlInstance, out Int64 vertexBufferSize, IntPtr indexBufferSize, IntPtr transformationBufferSize);

		[DllImport(enginedll, EntryPoint = "CalculateInstance")]
		public static extern Int64 CalculateInstance(Int64 owlInstance, IntPtr vertexBufferSize, out Int64 indexBufferSize, out Int64 transformationBufferSize);

		[DllImport(enginedll, EntryPoint = "CalculateInstance")]
		public static extern Int64 CalculateInstance(Int64 owlInstance, IntPtr vertexBufferSize, out Int64 indexBufferSize, IntPtr transformationBufferSize);

		[DllImport(enginedll, EntryPoint = "CalculateInstance")]
		public static extern Int64 CalculateInstance(Int64 owlInstance, IntPtr vertexBufferSize, IntPtr indexBufferSize, out Int64 transformationBufferSize);

		[DllImport(enginedll, EntryPoint = "CalculateInstance")]
		public static extern Int64 CalculateInstance(Int64 owlInstance, IntPtr vertexBufferSize, IntPtr indexBufferSize, IntPtr transformationBufferSize);

		public static Int64 CalculateInstance(Int64 owlInstance, out Int64 vertexBufferSize, out Int64 indexBufferSize)
		{
			return CalculateInstance(owlInstance, out vertexBufferSize, out indexBufferSize, IntPtr.Zero);
		}

		/// <summary>
		///		UpdateInstance                                          (http://rdf.bg/gkdoc/CS64/UpdateInstance.html)
		///
		///	This function prepares the content to be ready without filling the buffers
		///	as done within CalculateInstance(). CalculateInstance calls this function as a start.
		///	This function will also set the 'derived' values for the instance passed as argument.
		///	For example the coordinates values of a MultiplicationMatrix will be set if the array is
		///	defined.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "UpdateInstance")]
		public static extern Int64 UpdateInstance(Int64 owlInstance);

		/// <summary>
		///		InferenceInstance                                       (http://rdf.bg/gkdoc/CS64/InferenceInstance.html)
		///
		///	This function fills in values that are implicitely known but not given by the user. This function
		///	can also be used to identify default values of properties if not given.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "InferenceInstance")]
		public static extern Int64 InferenceInstance(Int64 owlInstance);

		/// <summary>
		///		UpdateInstanceVertexBuffer                              (http://rdf.bg/gkdoc/CS64/UpdateInstanceVertexBuffer.html)
		///
		///	This function should be preceded by the function CalculateInstances(),
		///	the only allowed other API functions in between are UpdateIndexBuffer()
		///	and UpdateTransformationBuffer().
		///	It is expected to be called with a buffer vertexBuffer of at least the size as 
		///	given by CalculateInstances().
		///	If not called for the first time it will expect to contain the same content as
		///	from previous call, even is size is changed. This can be overruled by 
		///	the function ClearedExternalBuffers().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBuffer")]
		public static extern Int64 UpdateInstanceVertexBuffer(Int64 owlInstance, out float vertexBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBuffer")]
		public static extern Int64 UpdateInstanceVertexBuffer(Int64 owlInstance, float[] vertexBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBuffer")]
		public static extern Int64 UpdateInstanceVertexBuffer(Int64 owlInstance, out double vertexBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBuffer")]
		public static extern Int64 UpdateInstanceVertexBuffer(Int64 owlInstance, double[] vertexBuffer);

		/// <summary>
		///		UpdateInstanceVertexBufferTrimmed                       (http://rdf.bg/gkdoc/CS64/UpdateInstanceVertexBufferTrimmed.html)
		///
		///	This function is an alternative for UpdateInstanceVertexBuffer(),
		///	in case the vertex buffer should be divided over a set of arrays
		///	this function allows to fill part of the vertex buffer given a
		///	certain offset and size (both calculated in vertex element count).
		/// </summary>
		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBufferTrimmed")]
		public static extern Int64 UpdateInstanceVertexBufferTrimmed(Int64 owlInstance, out float vertexBuffer, Int64 offset, Int64 size);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBufferTrimmed")]
		public static extern Int64 UpdateInstanceVertexBufferTrimmed(Int64 owlInstance, float[] vertexBuffer, Int64 offset, Int64 size);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBufferTrimmed")]
		public static extern Int64 UpdateInstanceVertexBufferTrimmed(Int64 owlInstance, out double vertexBuffer, Int64 offset, Int64 size);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceVertexBufferTrimmed")]
		public static extern Int64 UpdateInstanceVertexBufferTrimmed(Int64 owlInstance, double[] vertexBuffer, Int64 offset, Int64 size);

		/// <summary>
		///		UpdateInstanceIndexBuffer                               (http://rdf.bg/gkdoc/CS64/UpdateInstanceIndexBuffer.html)
		///
		///	This function should be preceded by the function CalculateInstances(),
		///	the only allowed other API functions in between are UpdateVertexBuffer()
		///	and UpdateTransformationBuffer().
		///	It is expected to be called with a buffer indexBuffer of at least the size as 
		///	given by CalculateInstances().
		///	If not called for the first time it will expect to contain the same content as
		///	from previous call, even is size is changed. This can be overruled by 
		///	the function ClearedExternalBuffers().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBuffer")]
		public static extern Int64 UpdateInstanceIndexBuffer(Int64 owlInstance, out Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBuffer")]
		public static extern Int64 UpdateInstanceIndexBuffer(Int64 owlInstance, Int32[] indexBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBuffer")]
		public static extern Int64 UpdateInstanceIndexBuffer(Int64 owlInstance, out Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBuffer")]
		public static extern Int64 UpdateInstanceIndexBuffer(Int64 owlInstance, Int64[] indexBuffer);

		/// <summary>
		///		UpdateInstanceIndexBufferTrimmed                        (http://rdf.bg/gkdoc/CS64/UpdateInstanceIndexBufferTrimmed.html)
		///
		///	This function is an alternative for UpdateInstanceIndexBuffer(),
		///	in case the index buffer should be divided over a set of arrays
		///	this function allows to fill part of the index buffer given a
		///	certain offset and size.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBufferTrimmed")]
		public static extern Int64 UpdateInstanceIndexBufferTrimmed(Int64 owlInstance, out Int32 indexBuffer, Int64 offset, Int64 size);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBufferTrimmed")]
		public static extern Int64 UpdateInstanceIndexBufferTrimmed(Int64 owlInstance, Int32[] indexBuffer, Int64 offset, Int64 size);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBufferTrimmed")]
		public static extern Int64 UpdateInstanceIndexBufferTrimmed(Int64 owlInstance, out Int64 indexBuffer, Int64 offset, Int64 size);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceIndexBufferTrimmed")]
		public static extern Int64 UpdateInstanceIndexBufferTrimmed(Int64 owlInstance, Int64[] indexBuffer, Int64 offset, Int64 size);

		/// <summary>
		///		UpdateInstanceTransformationBuffer                      (http://rdf.bg/gkdoc/CS64/UpdateInstanceTransformationBuffer.html)
		///
		///	This function should be preceded by the function CalculateInstances(),
		///	the only allowed other API functions in between are UpdateVertexBuffer()
		///	and UpdateIndexBuffer().
		///	It is expected to be called with a buffer vertexBuffer of at least the size as 
		///	given by CalculateInstances().
		///	If not called for the first time it will expect to contain the same content as
		///	from previous call, even is size is changed. This can be overruled by 
		///	the function ClearedExternalBuffers().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "UpdateInstanceTransformationBuffer")]
		public static extern Int64 UpdateInstanceTransformationBuffer(Int64 owlInstance, out double transformationBuffer);

		[DllImport(enginedll, EntryPoint = "UpdateInstanceTransformationBuffer")]
		public static extern Int64 UpdateInstanceTransformationBuffer(Int64 owlInstance, double[] transformationBuffer);

		/// <summary>
		///		ClearedInstanceExternalBuffers                          (http://rdf.bg/gkdoc/CS64/ClearedInstanceExternalBuffers.html)
		///
		///	This function tells the engine that all buffers have no memory of earlier filling 
		///	for a certain instance.
		///	This means that even when buffer content didn't changed it will be updated when
		///	functions UpdateVertexBuffer(), UpdateIndexBuffer() and/or transformationBuffer()
		///	are called for this specific instance.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ClearedInstanceExternalBuffers")]
		public static extern void ClearedInstanceExternalBuffers(Int64 owlInstance);

		/// <summary>
		///		ClearedExternalBuffers                                  (http://rdf.bg/gkdoc/CS64/ClearedExternalBuffers.html)
		///
		///	This function tells the engine that all buffers have no memory of earlier filling.
		///	This means that even when buffer content didn't changed it will be updated when
		///	functions UpdateVertexBuffer(), UpdateIndexBuffer() and/or transformationBuffer()
		///	are called.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "ClearedExternalBuffers")]
		public static extern void ClearedExternalBuffers(Int64 model);

		/// <summary>
		///		GetConceptualFaceCnt                                    (http://rdf.bg/gkdoc/CS64/GetConceptualFaceCnt.html)
		///
		///	This function returns the number of conceptual faces for a certain instance.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFaceCnt")]
		public static extern Int64 GetConceptualFaceCnt(Int64 owlInstance);

		/// <summary>
		///		GetConceptualFace                                       (http://rdf.bg/gkdoc/CS64/GetConceptualFace.html)
		///
		///	This function returns a handle to the conceptual face. Be aware that different
		///	instances can return the same handles (however with possible different startIndices and noIndicesTriangles).
		///	Argument index should be at least zero and smaller then return value of GetConceptualFaceCnt().
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFace")]
		public static extern Int64 GetConceptualFace(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		public static Int64 GetConceptualFace(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints)
		{
			return GetConceptualFace(owlInstance, index, out startIndexTriangles, out noIndicesTriangles, out startIndexLines, out noIndicesLines, out startIndexPoints, out noIndicesPoints, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero);
		}

		public static Int64 GetConceptualFace(Int64 owlInstance, Int64 index)
		{
			return GetConceptualFace(owlInstance, index, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero, IntPtr.Zero);
		}

		/// <summary>
		///		GetConceptualFaceMaterial                               (http://rdf.bg/gkdoc/CS64/GetConceptualFaceMaterial.html)
		///
		///	This function returns the material instance relevant for this
		///	conceptual face.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFaceMaterial")]
		public static extern Int64 GetConceptualFaceMaterial(Int64 conceptualFace);

		/// <summary>
		///		GetConceptualFaceOriginCnt                              (http://rdf.bg/gkdoc/CS64/GetConceptualFaceOriginCnt.html)
		///
		///	This function returns the number of instances that are the source primitive/concept
		///	for this conceptual face.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFaceOriginCnt")]
		public static extern Int64 GetConceptualFaceOriginCnt(Int64 conceptualFace);

		/// <summary>
		///		GetConceptualFaceOrigin                                 (http://rdf.bg/gkdoc/CS64/GetConceptualFaceOrigin.html)
		///
		///	This function returns a handle to the instance that is the source primitive/concept
		///	for this conceptual face.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFaceOrigin")]
		public static extern Int64 GetConceptualFaceOrigin(Int64 conceptualFace, Int64 index);

		/// <summary>
		///		GetConceptualFaceOriginEx                               (http://rdf.bg/gkdoc/CS64/GetConceptualFaceOriginEx.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFaceOriginEx")]
		public static extern void GetConceptualFaceOriginEx(Int64 conceptualFace, Int64 index, out Int64 originatingOwlInstance, out Int64 originatingConceptualFace);

		/// <summary>
		///		GetFaceCnt                                              (http://rdf.bg/gkdoc/CS64/GetFaceCnt.html)
		///
		///	This function returns the number of faces for a certain instance.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetFaceCnt")]
		public static extern Int64 GetFaceCnt(Int64 owlInstance);

		/// <summary>
		///		GetFace                                                 (http://rdf.bg/gkdoc/CS64/GetFace.html)
		///
		///	This function gets the individual faces including the meta data, i.e. the number of openings within this specific face.
		///	This call is for very dedicated use, it would be more common to iterate over the individual conceptual faces.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetFace")]
		public static extern void GetFace(Int64 owlInstance, Int64 index, out Int64 startIndex, out Int64 noOpenings);

		/// <summary>
		///		GetDependingPropertyCnt                                 (http://rdf.bg/gkdoc/CS64/GetDependingPropertyCnt.html)
		///
		///	This function returns the number of properties that are of influence on the
		///	location and form of the conceptualFace.
		///
		///	Note: BE AWARE, THIS FUNCTION EXPECTS A TREE, NOT A NETWORK, IN CASE OF A NETWORK THIS FUNCTION CAN LOCK THE ENGINE
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetDependingPropertyCnt")]
		public static extern Int64 GetDependingPropertyCnt(Int64 baseOwlInstance, Int64 conceptualFace);

		/// <summary>
		///		GetDependingProperty                                    (http://rdf.bg/gkdoc/CS64/GetDependingProperty.html)
		///
		///	This function returns a handle to the property that is the 'index'-th property
		///	of influence on the form. It also returns the handle to instance this property
		///	belongs to.
		///
		///	Note: the returned property is always a datatypeProperty
		///	Note: if input is incorrect (for example index is in wrong domain) _property and
		///		  instance will be both zero.
		///	Note: BE AWARE, THIS FUNCTION EXPECTS A TREE, NOT A NETWORK, IN CASE OF A NETWORK THIS FUNCTION CAN LOCK THE ENGINE
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetDependingProperty")]
		public static extern void GetDependingProperty(Int64 baseOwlInstance, Int64 conceptualFace, Int64 index, out Int64 owlInstance, out Int64 owlDatatypeProperty);

		/// <summary>
		///		SetFormat                                               (http://rdf.bg/gkdoc/CS64/SetFormat.html)
		///
		///	This function sets the type of export format, by setting a mask
		///		bit 0 & 1:
		///			00	Each vertex is unique (although mostly irrelevant UpdateIndexBuffer() and 
		///				UpdateTransformationBuffer() are still returning information)
		///			01	Each index is unique => vertices are not necessarily (although mostly
		///				irrelevant UpdateTransformationBuffer() is still returning information)
		///			10	Single level Transformations are used, most optimal when using DirectX till version 11
		///				and OpenGL till version 2
		///			11	Nested Transformations are used, most optimal but till 2011 no known support of
		///				low level 3D interfaces like DirectX and OpenGL
		///		bit 2:	(FORMAT_SIZE_VERTEX_DOUBLE)
		///			0	Vertex items returned as float (4 byte/32 bit)
		///			1	Vertex items returned as double (8 byte/64 bit)
		///		bit 3:	(FORMAT_SIZE_INDEX_INT64)
		///			0	Index items returned as int32_t (4 byte/32 bit)
		///			1	Index items returned as int64_t (8 byte/64 bit) (only available in 64 bit mode)
		///
		///		bit 4:	(FORMAT_VERTEX_POINT)
		///			0	Vertex does not contain 3D point info
		///			1	Vertex does contain 3D point info
		///		bit 5:	(FORMAT_VERTEX_NORMAL)
		///			0	Vertex does not contain 3D normal vector info
		///			1	Vertex does contain 3D normal vector info => if set, bit 4 will also be set
		///		bit 6:	(FORMAT_VERTEX_TEXTURE_UV)
		///			0	Vertex does not contain first 2D texture info
		///			1	Vertex does contain first 2D texture info
		///		bit 7:	(FORMAT_VERTEX_TEXTURE2_UV)
		///			0	Vertex does not contain second 2D texture info
		///			1	Vertex does contain second 2D texture info => if set, bit 6 will also be set
		///
		///		bit 8:	(FORMAT_EXPORT_TRIANGLES)
		///			0	No object form triangles are exported
		///			1	Object form triangles are exported (effective if instance contains faces and/or solids)
		///		bit 9:	(FORMAT_EXPORT_LINES)
		///			0	No object polygon lines are exported
		///			1	Object polygon lines are exported (effective if instance contains line representations)
		///		bit 10:	(FORMAT_EXPORT_POINTS)
		///			0	No object points are exported
		///			1	Object points are exported (effective if instance contains point representations)
		///
		///		bit 11:	Reserved, by default 0
		///
		///		bit 12:	(FORMAT_EXPORT_FACE_POLYGONS)
		///			0	No object face polygon lines are exported
		///			1	Object face polygon lines (dense wireframe) are exported => if set, bit 8 will also be set
		///		bit 13:	(FORMAT_EXPORT_CONCEPTUAL_FACE_POLYGONS)
		///			0	No object conceptual face polygon lines are exported
		///			1	Object conceptual face polygon lines (wireframe) are exported => if set, bit 12 will also be set
		///		bit 14:	(FORMAT_EXPORT_POLYGONS_AS_TUPLES)
		///			0	Polygon lines (wireframe) exported as list, i.e. typical 4 point polygon exported as  0 1 2 3 0 -1
		///			1	Polygon lines (wireframe) exported as tuples, i.e. typical 4 point polygon exported as 0 1 1 2 2 3 3 0
		///
		///		bit 15:	(FORMAT_EXPORT_ADVANCED_NORMALS)
		///			0	All normals of triangles are transformed orthogonal to the 2D face they belong to
		///			1	Normals are exported to be in line with the original semantic form description (could be non orthogonal to the 2D face) 
		///
		///		bit 16:	(FORMAT_EXPORT_DIRECTX)
		///			0	no specific behavior
		///			1	Where possible DirectX compatibility is given to exported data (i.e. order of components in vertex buffer)
		///					 => [bit 20, bit 21 both UNSET]
		///					 => if set, bit 17 will be unset
		///
		///		bit 17:	(FORMAT_EXPORT_OPENGL)
		///			0	no specific behavior
		///			1	Where possible OpenGL compatibility is given to exported data (i.e. order of components in vertex buffer and inverted texture coordinates in Y direction)
		///					 => [bit 20, bit 21 both SET]
		///					 => if set, bit 16 will be unset
		///
		///		bit 18:	(FORMAT_EXPORT_DOUBLE_SIDED)
		///			0	All faces are defined as calculated
		///			1	Every face has exactly one opposite face (normally both index and vertex buffer are doubled in size)
		///
		///		bit 19:	Reserved, by default 0
		///
		///		bit 20-23:
		///			0000	version 0 (used in case there is different behavior between versions in DirectX or OpenGL)
		///			....	...
		///			1111	version 15
		///
		///		bit 20:	(FORMAT_EXPORT_VERSION_0001)
		///			0	Standard Triangle Rotation (LHS as expected by DirectX) 
		///			1	Opposite Triangle Rotation (RHS as expected by OpenGL)
		///		bit 21:	(FORMAT_EXPORT_VERSION_0010)
		///			0	X, Y, Z (nX, nY, nZ) formatted as <X Y Z> considering internal concepts
		///			1	X, Y, Z (nX, nY, nZ) formatted as <X -Z Y>, i.e. X, -Z, Y (nX, -nZ, nY) considering internal concepts (OpenGL)
		///
		///		bit 24:	(FORMAT_VERTEX_COLOR_AMBIENT)
		///			0	Vertex does not contain Ambient color information
		///			1	Vertex does contain Ambient color information
		///		bit 25:	(FORMAT_VERTEX_COLOR_DIFFUSE)
		///			0	Vertex does not contain Diffuse color information
		///			1	Vertex does contain Diffuse color information
		///		bit 26:	(FORMAT_VERTEX_COLOR_EMISSIVE)
		///			0	Vertex does not contain Emissive color information
		///			1	Vertex does contain Emissive color information
		///		bit 27:	(FORMAT_VERTEX_COLOR_SPECULAR)
		///			0	Vertex does not contain Specular color information
		///			1	Vertex does contain Specular color information
		///
		///		bit 28:	(FORMAT_VERTEX_TEXTURE_TANGENT)
		///			0	Vertex does not contain tangent vector for first texture
		///			1	Vertex does contain tangent vector for first texture => if set, bit 6 will also be set
		///		bit 29:	(FORMAT_VERTEX_TEXTURE_BINORMAL)
		///			0	Vertex does not contain binormal vector for first texture
		///			1	Vertex does contain binormal vector for first texture => if set, bit 6 will also be set
		///		bit 30:	(FORMAT_VERTEX_TEXTURE2_TANGENT)		ONLY WORKS IN 64 BIT MODE
		///			0	Vertex does not contain tangent vector for second texture
		///			1	Vertex does contain tangent vector for second texture => if set, bit 6 will also be set
		///		bit 31:	(FORMAT_VERTEX_TEXTURE2_BINORMAL)		ONLY WORKS IN 64 BIT MODE
		///			0	Vertex does not contain binormal vector for second texture
		///			1	Vertex does contain binormal vector for second texture => if set, bit 6 will also be set
		///
		///		bit 26-31:	Reserved, by default 0
		///
		///		bit 32-63:	Reserved, by default 0
		///
		///	Note: default setting is 0000 0000 0000 0000   0000 0000 0000 0000  -  0000 0000 0000 0000   1000 0001  0011 0000 = h0000 0000 - 0000 8130 = 33072
		///
		///
		///	Depending on FORMAT_SIZE_VERTEX_DOUBLE each element in the vertex buffer is a double or float number.
		///	Number of elements for each vertex depends on format setting. You can get the number by GetVertexElementsCounts. 
		///	Each vertex block contains data items in an order according to the table below. The table also specifies when an item is present and number of elements 
		///	it occupied. Use GetVertexDataOffset or GetVertexColor to get required item. 
		///
		///	#	Vertex data item	Included when format setting bit is on					Size (num of elements)
		///	Point coordinates		X, Y, X				FORMAT_VERTEX_POINT	(bit 4)					3
		///	Normal coordinates		Nx, Ny, Nz			FORMAT_VERTEX_NORMAL (bit 5)				3
		///	Texture coordinates		T1u, T1v			FORMAT_VERTEX_TEXTURE_UV (bit 6)			2
		///	2nd Texture coordinates	T2u, T2v			FORMAT_VERTEX_TEXTURE2_UV (bit 7)			2
		///	Ambient color								FORMAT_VERTEX_COLOR_AMBIENT (bit 24)		1
		///	Diffuse color								FORMAT_VERTEX_COLOR_DIFFUSE (bit 25)		1
		///	Emissive color								FORMAT_VERTEX_COLOR _EMISSIVE (bit 26)		1
		///	Specular color								FORMAT_VERTEX_COLOR _SPECULAR (bit 27)		1
		///	Texture tangent			T1Tx, T1Ty, T1Tz	FORMAT_VERTEX_TEXTURE_TANGENT (bit 28)		3
		///	Texture binormal		T1BNx,T1BNy,T1BNz	FORMAT_VERTEX_TEXTURE_BINORMAL (bit 29)		3
		///	2nd texture tangent		T2Tx, T2Ty, T2Tz	FORMAT_VERTEX_TEXTURE2_TANGENT (bit 30)		3
		///	2nd texture binormal	T2BNx,T2BNy,T2BNz	FORMAT_VERTEX_TEXTURE2_BINORMAL (bit 31)	3
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetFormat")]
		public static extern UInt64 SetFormat(Int64 model, UInt64 setting, UInt64 mask);

		public static UInt64 SetFormat(Int64 model)
		{
			return SetFormat(model, 0, 0);
		}

		/// <summary>
		///		GetFormat                                               (http://rdf.bg/gkdoc/CS64/GetFormat.html)
		///
		///	Returns the current format given a mask.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetFormat")]
		public static extern UInt64 GetFormat(Int64 model, UInt64 mask);

		public static UInt64 GetFormat(Int64 model)
		{
			return GetFormat(model, 0);
		}

		/// <summary>
		///		GetVertexDataOffset                                     (http://rdf.bg/gkdoc/CS64/GetVertexDataOffset.html)
		///
		///	Returns offset of the required data in a vertex elements array with the specified format settings
		///	requiredData is one of the control vertex data bits (FORMAT_VERTEX...) or 0 to get count of all elements in vertex buffer
		///	Functions returns -1 if the required data are absent with the settings.
		///
		///	Ensure your settings are actual. They may be differ you pass to SetFormat (for example because of mask)
		///	It's preferable to inquire resulting setting with GetFormat(model, GetFormat(0, 0))
		///
		///	Note: vertex buffer element is a double or a float number depending on FORMAT_SIZE_VERTEX_DOUBLE flag. 
		///	If you need offset in bytes multiply by size of element.
		///	Compare to SetFormat that returns size of vertex data in bytes.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetVertexDataOffset")]
		public static extern Int32 GetVertexDataOffset(Int64 requiredData, Int64 setting);

		/// <summary>
		///		SetBehavior                                             (http://rdf.bg/gkdoc/CS64/SetBehavior.html)
		///
		///	This function sets the type of behavior, by setting a mask
		///
		///		bit 0-7:	Reserved, by default 0
		///
		///		bit 8:
		///			0	Do not optimize
		///			1	Vertex items returned as double (8 byte/64 bit)
		///
		///		bit 9-31:	Reserved, by default 0
		///
		///		bit 32-63:	Reserved, by default 0
		///
		///	Note: default setting is 0000 0000 0000 0000   0000 0000 0000 0000  -  0000 0000 0000 0000   0000 0001  0000 0000 = h0000 0000 - 0000 0100 = 256
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetBehavior")]
		public static extern void SetBehavior(Int64 model, Int64 setting, Int64 mask);

		/// <summary>
		///		GetBehavior                                             (http://rdf.bg/gkdoc/CS64/GetBehavior.html)
		///
		///	Returns the current behavior given a mask.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetBehavior")]
		public static extern Int64 GetBehavior(Int64 model, Int64 mask);

		/// <summary>
		///		SetVertexBufferTransformation                           (http://rdf.bg/gkdoc/CS64/SetVertexBufferTransformation.html)
		///
		///	Sets the transformation for a Vertex Buffer.
		///	The transformation will always be calculated in 64 bit, even if the vertex element size is 32 bit.
		///	This function can be called just before updating the vertex buffer.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetVertexBufferTransformation")]
		public static extern void SetVertexBufferTransformation(Int64 model, ref double matrix);

		[DllImport(enginedll, EntryPoint = "SetVertexBufferTransformation")]
		public static extern void SetVertexBufferTransformation(Int64 model, double[] matrix);

		/// <summary>
		///		GetVertexBufferTransformation                           (http://rdf.bg/gkdoc/CS64/GetVertexBufferTransformation.html)
		///
		///	Gets the transformation for a Vertex Buffer.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetVertexBufferTransformation")]
		public static extern void GetVertexBufferTransformation(Int64 model, out double matrix);

		[DllImport(enginedll, EntryPoint = "GetVertexBufferTransformation")]
		public static extern void GetVertexBufferTransformation(Int64 model, double[] matrix);

		/// <summary>
		///		SetIndexBufferOffset                                    (http://rdf.bg/gkdoc/CS64/SetIndexBufferOffset.html)
		///
		///	Sets the offset for an Index Buffer.
		///	It is important call this function before updating the vertex buffer. 
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetIndexBufferOffset")]
		public static extern void SetIndexBufferOffset(Int64 model, Int64 offset);

		/// <summary>
		///		GetIndexBufferOffset                                    (http://rdf.bg/gkdoc/CS64/GetIndexBufferOffset.html)
		///
		///	Gets the current offset for an Index Buffer.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetIndexBufferOffset")]
		public static extern Int64 GetIndexBufferOffset(Int64 model);

		/// <summary>
		///		SetVertexBufferOffset                                   (http://rdf.bg/gkdoc/CS64/SetVertexBufferOffset.html)
		///
		///	Sets the offset for a Vertex Buffer.
		///	The offset will always be calculated in 64 bit, even if the vertex element size is 32 bit.
		///	This function can be called just before updating the vertex buffer.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetVertexBufferOffset")]
		public static extern void SetVertexBufferOffset(Int64 model, double x, double y, double z);

		public static void SetVertexBufferOffset(Int64 model, ref double[] offset)
		{
			if (offset != null) {
				SetVertexBufferOffset(
						model,
						offset[0],
						offset[1],
						offset[2]
					);
			}
			else {
				SetVertexBufferOffset(
						model,
						0.0,
						0.0,
						0.0
					);
			}
		}

		/// <summary>
		///		GetVertexBufferOffset                                   (http://rdf.bg/gkdoc/CS64/GetVertexBufferOffset.html)
		///
		///	Gets the offset for a Vertex Buffer.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetVertexBufferOffset")]
		public static extern void GetVertexBufferOffset(Int64 model, out double x, out double y, out double z);

		public static void GetVertexBufferOffset(Int64 model, ref double[] offset)
		{
			GetVertexBufferOffset(
					model,
					out offset[0],
					out offset[1],
					out offset[2]
				);
		}

		/// <summary>
		///		SetDefaultColor                                         (http://rdf.bg/gkdoc/CS64/SetDefaultColor.html)
		///
		///	Set the default values for the colors defined as argument.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetDefaultColor")]
		public static extern void SetDefaultColor(Int64 model, UInt32 ambient, UInt32 diffuse, UInt32 emissive, UInt32 specular);

		/// <summary>
		///		GetDefaultColor                                         (http://rdf.bg/gkdoc/CS64/GetDefaultColor.html)
		///
		///	Retrieve the default values for the colors defined as argument.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetDefaultColor")]
		public static extern void GetDefaultColor(Int64 model, out UInt32 ambient, out UInt32 diffuse, out UInt32 emissive, out UInt32 specular);

		/// <summary>
		///		CheckConsistency                                        (http://rdf.bg/gkdoc/CS64/CheckConsistency.html)
		///
		///	This function returns information about the consistency of each instance.
		///
		///	The mask defined what type of information can be retrieved from this call, the mask is a bitwise definition.
		///
		///		bit 0:	Check Design Tree Consistency
		///		bit 1:	Check Consistency for Triangle Output (through API)
		///		bit 2:	Check Consistency for Line Output (through API)
		///		bit 3:	Check Consistency for Point Output (through API)
		///		bit 4:	Check Consistency for Generated Surfaces (through API)
		///		bit 5:	Check Consistency for Generated Surfaces (internal)
		///		bit 6:	Check Consistency for Generated Solids (through API)
		///		bit 7:	Check Consistency for Generated Solids (internal)
		///		bit 8:	Check Consistency for BoundingBox's
		///		bit 9:	Check Consistency for Triangulation
		///		bit 10: Check Consistency for Relations (through API)
		///
		///		bit 16:	Contains (Closed) Solid(s)
		///		bit 18:	Contains (Closed) Infinite Solid(s)
		///		bit 20:	Contains Closed Surface(s)
		///		bit 21:	Contains Open Surface(s)
		///		bit 22:	Contains Closed Infinite Surface(s)
		///		bit 23:	Contains Open Infinite Surface(s)
		///		bit 24:	Contains Closed Line(s)
		///		bit 25:	Contains Open Line(s)
		///		bit 26:	Contains Closed Infinite Line(s) [i.e. both ends in infinity]
		///		bit 27:	Contains Open Infinite Line(s) [i.e. one end in infinity]
		///		bit 28:	Contains (Closed) Point(s)
		///
		///	If a bit in the mask is set and the result of the check has an issue, the resulting value will have this bit set.
		///	i.e. any non-zero return value in Check Consistency is an indication that something is wrong or unexpected; 
		///	any non-zero return value in Contains is an indication that this type of geometry is expected in one of the instances; 
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CheckConsistency")]
		public static extern Int64 CheckConsistency(Int64 model, Int64 mask);

		/// <summary>
		///		CheckInstanceConsistency                                (http://rdf.bg/gkdoc/CS64/CheckInstanceConsistency.html)
		///
		///	This function returns information about the consistency of the instance and indirectly referenced instances.
		///
		///	The mask defined what type of information can be retrieved from this call, the mask is a bitwise definition.
		///
		///		bit 0:	Check Design Tree Consistency
		///		bit 1:	Check Consistency for Triangle Output (through API)
		///		bit 2:	Check Consistency for Line Output (through API)
		///		bit 3:	Check Consistency for Point Output (through API)
		///		bit 4:	Check Consistency for Generated Surfaces (through API)
		///		bit 5:	Check Consistency for Generated Surfaces (internal)
		///		bit 6:	Check Consistency for Generated Solids (through API)
		///		bit 7:	Check Consistency for Generated Solids (internal)
		///		bit 8:	Check Consistency for BoundingBox's
		///		bit 9:	Check Consistency for Triangulation
		///		bit 10: Check Consistency for Relations (through API)
		///
		///		bit 16:	Contains (Closed) Solid(s)
		///		bit 18:	Contains (Closed) Infinite Solid(s)
		///		bit 20:	Contains Closed Surface(s)
		///		bit 21:	Contains Open Surface(s)
		///		bit 22:	Contains Closed Infinite Surface(s)
		///		bit 23:	Contains Open Infinite Surface(s)
		///		bit 24:	Contains Closed Line(s)
		///		bit 25:	Contains Open Line(s)
		///		bit 26:	Contains Closed Infinite Line(s) [i.e. both ends in infinity]
		///		bit 27:	Contains Open Infinite Line(s) [i.e. one end in infinity]
		///		bit 28:	Contains (Closed) Point(s)
		///
		///	If a bit in the mask is set and the result of the check has an issue, the resulting value will have this bit set.
		///	i.e. any non-zero return value in Check Consistency is an indication that something is wrong or unexpected regarding the given instance; 
		///	any non-zero return value in Contains is an indication that this type of geometry is expected regarding the given instance; 
		/// </summary>
		[DllImport(enginedll, EntryPoint = "CheckInstanceConsistency")]
		public static extern Int64 CheckInstanceConsistency(Int64 owlInstance, Int64 mask);

		/// <summary>
		///		IsDuplicate                                             (http://rdf.bg/gkdoc/CS64/IsDuplicate.html)
		///
		///	Checks if two geometry representations are (almost) similar except for a transformation matrix and a given epsilon.
		///	The parameter duplicateMatrix is optional and can be left to zero.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "IsDuplicate")]
		public static extern byte IsDuplicate(Int64 originalOwlInstance, Int64 duplicateOwlInstance, out double duplicateMatrix, double absoluteEpsilon, double relativeEpsilon, byte checkMaterial);

		[DllImport(enginedll, EntryPoint = "IsDuplicate")]
		public static extern byte IsDuplicate(Int64 originalOwlInstance, Int64 duplicateOwlInstance, double[] duplicateMatrix, double absoluteEpsilon, double relativeEpsilon, byte checkMaterial);

        //
        //  Derived Geometry API Calls
        //

		/// <summary>
		///		GetPerimeter                                            (http://rdf.bg/gkdoc/CS64/GetPerimeter.html)
		///
		///	This function calculates the perimeter of an instance.
		///
		///	Note: internally the call does not store its results, any optimization based on known
		///		  dependancies between instances need to be implemented on the client.
		///	Note: due to internal structure using already calculated vertex buffer / index buffer does not
		///		  give any performance benefits, in opposite to GetVolume and GetArea
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPerimeter")]
		public static extern double GetPerimeter(Int64 owlInstance);

		/// <summary>
		///		GetArea                                                 (http://rdf.bg/gkdoc/CS64/GetArea.html)
		///
		///	This function calculates the area of an instance.
		///	For perfomance reasons it is benefitial to call it with vertex and index buffer when
		///	the arrays are calculated anyway or Volume and Area are needed.
		///
		///	There are two ways to call GetVolume:
		///		vertexBuffer and indexBuffer are both zero: in this case the instance will be
		///				recalculated when needed. It is expected the client does not
		///				need the arrays itself or there is no performance issue.
		///		vertexBuffer and indexBuffer are both given: the call is placed directly after
		///				updateBuffer calls and no structural change to depending instances have 
		///				been done in between. The transformationMatrix array is not needed,
		///				even if it is being used due to not giving any performance gain to this
		///				operation.
		///
		///	Note: internally the call does not store its results, any optimization based on known
		///		  dependancies between instances need to be implemented on the client.
		///	Note: in case precision is important and vertex buffer is 32 bit it is advised to
		///		  set vertexBuffer and indexBuffer to 0 even if arrays are existing.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetArea")]
		public static extern double GetArea(Int64 owlInstance, ref float vertexBuffer, ref Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetArea")]
		public static extern double GetArea(Int64 owlInstance, ref float vertexBuffer, ref Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetArea")]
		public static extern double GetArea(Int64 owlInstance, ref double vertexBuffer, ref Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetArea")]
		public static extern double GetArea(Int64 owlInstance, ref double vertexBuffer, ref Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetArea")]
		public static extern double GetArea(Int64 owlInstance, IntPtr vertexBuffer, IntPtr indexBuffer);

		public static double GetArea(Int64 owlInstance)
		{
			return GetArea(owlInstance, IntPtr.Zero, IntPtr.Zero);
		}

		/// <summary>
		///		GetVolume                                               (http://rdf.bg/gkdoc/CS64/GetVolume.html)
		///
		///	This function calculates the volume of an instance.
		///	For perfomance reasons it is benefitial to call it with vertex and index buffer when
		///	the arrays are calculated anyway or Volume and Area are needed.
		///
		///	There are two ways to call GetVolume:
		///		vertexBuffer and indexBuffer are both zero: in this case the instance will be
		///				recalculated when needed. It is expected the client does not
		///				need the arrays itself or there is no performance issue.
		///		vertexBuffer and indexBuffer are both given: the call is placed directly after
		///				updateBuffer calls and no structural change to depending instances have 
		///				been done in between. The transformationMatrix array is not needed,
		///				even if it is being used due to not giving any performance gain to this
		///				operation.
		///
		///	Note: internally the call does not store its results, any optimization based on known
		///		  dependancies between instances need to be implemented on the client.
		///	Note: in case precision is important and vertex buffer is 32 bit it is advised to
		///		  set vertexBuffer and indexBuffer to 0 even if arrays are existing.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetVolume")]
		public static extern double GetVolume(Int64 owlInstance, ref float vertexBuffer, ref Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetVolume")]
		public static extern double GetVolume(Int64 owlInstance, ref float vertexBuffer, ref Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetVolume")]
		public static extern double GetVolume(Int64 owlInstance, ref double vertexBuffer, ref Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetVolume")]
		public static extern double GetVolume(Int64 owlInstance, ref double vertexBuffer, ref Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetVolume")]
		public static extern double GetVolume(Int64 owlInstance, IntPtr vertexBuffer, IntPtr indexBuffer);

		public static double GetVolume(Int64 owlInstance)
		{
			return GetVolume(owlInstance, IntPtr.Zero, IntPtr.Zero);
		}

		/// <summary>
		///		GetCenter                                               (http://rdf.bg/gkdoc/CS64/GetCenter.html)
		///
		///	This function calculates the center of an instance.
		///	For perfomance reasons it is benefitial to call it with vertex and index buffer when
		///	the arrays are calculated anyway or Volume and Area are needed.
		///
		///	There are two ways to call GetCenter:
		///		vertexBuffer and indexBuffer are both zero: in this case the instance will be
		///				recalculated when needed. It is expected the client does not
		///				need the arrays itself or there is no performance issue.
		///		vertexBuffer and indexBuffer are both given: the call is placed directly after
		///				updateBuffer calls and no structural change to depending instances have 
		///				been done in between. The transformationMatrix array is not needed,
		///				even if it is being used due to not giving any performance gain to this
		///				operation.
		///
		///	Note: internally the call does not store its results, any optimization based on known
		///		  dependancies between instances need to be implemented on the client.
		///	Note: in case precision is important and vertex array is 32 bit it is advised to
		///		  set vertexBuffer and indexBuffer to 0 even if arrays are existing.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, ref float vertexBuffer, ref Int32 indexBuffer, out double center);

		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, ref float vertexBuffer, ref Int32 indexBuffer, double[] center);

		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, ref float vertexBuffer, ref Int64 indexBuffer, out double center);

		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, ref float vertexBuffer, ref Int64 indexBuffer, double[] center);

		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, ref double vertexBuffer, ref Int32 indexBuffer, out double center);

		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, ref double vertexBuffer, ref Int32 indexBuffer, double[] center);

		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, ref double vertexBuffer, ref Int64 indexBuffer, out double center);

		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, ref double vertexBuffer, ref Int64 indexBuffer, double[] center);

		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, IntPtr vertexBuffer, IntPtr indexBuffer, out double center);

		[DllImport(enginedll, EntryPoint = "GetCenter")]
		public static extern void GetCenter(Int64 owlInstance, IntPtr vertexBuffer, IntPtr indexBuffer, double[] center);

		public static void GetCenter(Int64 owlInstance, out double center)
		{
			GetCenter(owlInstance, IntPtr.Zero, IntPtr.Zero, out center);
		}

		/// <summary>
		///		GetCentroid                                             (http://rdf.bg/gkdoc/CS64/GetCentroid.html)
		///
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, ref float vertexBuffer, ref Int32 indexBuffer, out double centroid);

		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, ref float vertexBuffer, ref Int32 indexBuffer, double[] centroid);

		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, ref float vertexBuffer, ref Int64 indexBuffer, out double centroid);

		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, ref float vertexBuffer, ref Int64 indexBuffer, double[] centroid);

		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, ref double vertexBuffer, ref Int32 indexBuffer, out double centroid);

		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, ref double vertexBuffer, ref Int32 indexBuffer, double[] centroid);

		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, ref double vertexBuffer, ref Int64 indexBuffer, out double centroid);

		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, ref double vertexBuffer, ref Int64 indexBuffer, double[] centroid);

		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, IntPtr vertexBuffer, IntPtr indexBuffer, out double centroid);

		[DllImport(enginedll, EntryPoint = "GetCentroid")]
		public static extern double GetCentroid(Int64 owlInstance, IntPtr vertexBuffer, IntPtr indexBuffer, double[] centroid);

		public static double GetCentroid(Int64 owlInstance, out double centroid)
		{
			return GetCentroid(owlInstance, IntPtr.Zero, IntPtr.Zero, out centroid);
		}

		/// <summary>
		///		GetConceptualFacePerimeter                              (http://rdf.bg/gkdoc/CS64/GetConceptualFacePerimeter.html)
		///
		///	This function returns the perimeter of a given Conceptual Face.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFacePerimeter")]
		public static extern double GetConceptualFacePerimeter(Int64 conceptualFace);

		/// <summary>
		///		GetConceptualFaceArea                                   (http://rdf.bg/gkdoc/CS64/GetConceptualFaceArea.html)
		///
		///	This function returns the area of a given Conceptual Face. The attributes vertex buffer
		///	and index buffer are optional but will improve performance if defined.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFaceArea")]
		public static extern double GetConceptualFaceArea(Int64 conceptualFace, ref float vertexBuffer, ref Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceArea")]
		public static extern double GetConceptualFaceArea(Int64 conceptualFace, ref float vertexBuffer, ref Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceArea")]
		public static extern double GetConceptualFaceArea(Int64 conceptualFace, ref double vertexBuffer, ref Int32 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceArea")]
		public static extern double GetConceptualFaceArea(Int64 conceptualFace, ref double vertexBuffer, ref Int64 indexBuffer);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceArea")]
		public static extern double GetConceptualFaceArea(Int64 conceptualFace, IntPtr vertexBuffer, IntPtr indexBuffer);

		public static double GetConceptualFaceArea(Int64 conceptualFace)
		{
			return GetConceptualFaceArea(conceptualFace, IntPtr.Zero, IntPtr.Zero);
		}

		/// <summary>
		///		SetBoundingBoxReference                                 (http://rdf.bg/gkdoc/CS64/SetBoundingBoxReference.html)
		///
		///	This function passes addresses from the hosting application. This enables
		///	the engine to update these values without extra need for API calls. This is
		///	especially of interest because the hosting application is not aware of what
		///	instances are updated and 
		///	The transformationMatrix has 12 double values: _11, _12, _13, _21, _22, _23, 
		///	_31, _32, _33, _41, _42, _43.
		///	The startVector is the leftundernear vector and the endVector is the 
		///	rightupperfar vector, in all cases values are doubles (64 bit).
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetBoundingBoxReference")]
		public static extern void SetBoundingBoxReference(Int64 owlInstance, out double transformationMatrix, out double startVector, out double endVector);

		[DllImport(enginedll, EntryPoint = "SetBoundingBoxReference")]
		public static extern void SetBoundingBoxReference(Int64 owlInstance, double[] transformationMatrix, double[] startVector, double[] endVector);

		/// <summary>
		///		GetBoundingBox                                          (http://rdf.bg/gkdoc/CS64/GetBoundingBox.html)
		///
		///	When the transformationMatrix is given, it will fill an array of 12 double values.
		///	When the transformationMatrix is left empty and both startVector and endVector are
		///	given the boundingbox without transformation is calculated and returned.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetBoundingBox")]
		public static extern byte GetBoundingBox(Int64 owlInstance, out double transformationMatrix, out double startVector, out double endVector);

		[DllImport(enginedll, EntryPoint = "GetBoundingBox")]
		public static extern byte GetBoundingBox(Int64 owlInstance, IntPtr transformationMatrix, out double startVector, out double endVector);

		[DllImport(enginedll, EntryPoint = "GetBoundingBox")]
		public static extern byte GetBoundingBox(Int64 owlInstance, double[] transformationMatrix, double[] startVector, double[] endVector);

		[DllImport(enginedll, EntryPoint = "GetBoundingBox")]
		public static extern byte GetBoundingBox(Int64 owlInstance, IntPtr transformationMatrix, double[] startVector, double[] endVector);

		public static byte GetBoundingBox(Int64 owlInstance, out double startVector, out double endVector)
		{
			return GetBoundingBox(owlInstance, IntPtr.Zero, out startVector, out endVector);
		}

		/// <summary>
		///		GetRelativeTransformation                               (http://rdf.bg/gkdoc/CS64/GetRelativeTransformation.html)
		///
		///	This function returns the relative transformation matrix between two instances, i.e. in practise
		///	this means the matrices connected to the Transformation instances in the path in between.
		///	The matrix is only given when a unique path through inverse relations can be found,
		///	otherwise the identity matrix is returned.
		///	owlInstanceHead is allowed to be not defined, i.e. zero.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetRelativeTransformation")]
		public static extern void GetRelativeTransformation(Int64 owlInstanceHead, Int64 owlInstanceTail, out double transformationMatrix);

		[DllImport(enginedll, EntryPoint = "GetRelativeTransformation")]
		public static extern void GetRelativeTransformation(Int64 owlInstanceHead, Int64 owlInstanceTail, double[] transformationMatrix);

		/// <summary>
		///		GetDistance                                             (http://rdf.bg/gkdoc/CS64/GetDistance.html)
		///
		///	This function returns the shortest distance between two instances.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetDistance")]
		public static extern double GetDistance(Int64 firstOwlInstance, Int64 secondOwlInstance, out double pointFirstInstance, out double pointSecondInstance);

		[DllImport(enginedll, EntryPoint = "GetDistance")]
		public static extern double GetDistance(Int64 firstOwlInstance, Int64 secondOwlInstance, IntPtr pointFirstInstance, IntPtr pointSecondInstance);

		public static double GetDistance(Int64 firstOwlInstance, Int64 secondOwlInstance)
		{
			return GetDistance(firstOwlInstance, secondOwlInstance, IntPtr.Zero, IntPtr.Zero);
		}

		/// <summary>
		///		GetColorOfComponent                                     (http://rdf.bg/gkdoc/CS64/GetColorOfComponent.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetColorOfComponent(Int64 owlInstanceColorComponent)
		{
			System.Diagnostics.Debug.Assert(IsInstanceOfClass(owlInstanceColorComponent, "ColorComponent"));
			Int64 model = GetModel(owlInstanceColorComponent);

			string[] rgbwNames = { "R", "G", "B", "W" };
			double[] rgbwValues = { 0.0, 0.0, 0.0, 0.0 };

			for (int i = 0; i < 4; i++)
			{
				Int64 card = 0;
				IntPtr valuesPtr = IntPtr.Zero;
				GetDatatypeProperty(owlInstanceColorComponent, GetPropertyByName(model, rgbwNames[i]), out valuesPtr, out card);
				if (card == 1)
				{
					double[] values = new double[card];
					System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int)card);
					rgbwValues[i] = values[0];
				}
			}

			return RDF.COLOR.RGBW(rgbwValues);
		}

		/// <summary>
		///		SetColorOfComponent                                     (http://rdf.bg/gkdoc/CS64/SetColorOfComponent.html)
		///
		///	...
		/// </summary>
		public static void SetColorOfComponent(Int64 owlInstanceColorComponent, UInt32 color)
		{
			System.Diagnostics.Debug.Assert(IsInstanceOfClass(owlInstanceColorComponent, "ColorComponent"));

			Int64 model = GetModel(owlInstanceColorComponent);

			string[] rgbwNames = { "R", "G", "B", "W" };
			double[] rgbwValues = RDF.COLOR.GET_COMPONENTS(color);

			for (int i = 0; i < 4; i++)
			{
				SetDatatypeProperty(owlInstanceColorComponent, GetPropertyByName(model, rgbwNames[i]), rgbwValues[i]);
			}
		}

		/// <summary>
		///		GetColor                                                (http://rdf.bg/gkdoc/CS64/GetColor.html)
		///
		///	...
		/// </summary>
		public static void GetColor(Int64 owlInstanceColor, out UInt32 ambient, out UInt32 diffuse, out UInt32 emissive, out UInt32 specular)
		{
			System.Diagnostics.Debug.Assert(IsInstanceOfClass(owlInstanceColor, "Color"));

			GetDefaultColor(GetModel(owlInstanceColor), out ambient, out diffuse, out emissive, out specular);

			string[] componentNames = { "ambient", "diffuse", "emissive", "specular" };
			UInt32[] componentColors = { ambient, diffuse, emissive, specular };

			for (int i = 0; i < 4; i++)
			{
				if (componentColors[i] != 0)
				{
					Int64 card = 0;
					IntPtr valuesPtr = IntPtr.Zero;
					GetObjectProperty(owlInstanceColor, GetPropertyByName(GetModel(owlInstanceColor), componentNames[i]), out valuesPtr, out card);

					if (card == 1)
					{
						Int64[] values = new Int64[card];
						System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
						Int64 owlInstanceColorComponent = values[0];
						componentColors[i] = GetColorOfComponent(owlInstanceColorComponent);
					}
				}
			}
		}

		/// <summary>
		///		SetColor                                                (http://rdf.bg/gkdoc/CS64/SetColor.html)
		///
		///	...
		/// </summary>
		public static void SetColor(Int64 owlInstanceColor, UInt32 ambient, UInt32 diffuse, UInt32 emissive, UInt32 specular)
		{
			System.Diagnostics.Debug.Assert(IsInstanceOfClass(owlInstanceColor, "Color"));

			{
				Int64 card = 0;
				IntPtr valuesPtr = IntPtr.Zero;
				GetObjectProperty(owlInstanceColor, GetPropertyByName(GetModel(owlInstanceColor), "ambient"), out valuesPtr, out card);

				if (card == 1)
				{
					Int64[] values = new Int64[card];
					System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
					SetColorOfComponent(values[0], ambient);
				}
				else
				{
					SetColorOfComponent(CreateInstance(GetClassByName(GetModel(owlInstanceColor), "ColorComponent")), ambient);
				}
			}

			{
				Int64 card = 0;
				IntPtr valuesPtr = IntPtr.Zero;
				GetObjectProperty(owlInstanceColor, GetPropertyByName(GetModel(owlInstanceColor), "diffuse"), out valuesPtr, out card);

				if (card == 1)
				{
					Int64[] values = new Int64[card];
					System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
					SetColorOfComponent(values[0], diffuse);
				}
				else
				{
					SetColorOfComponent(CreateInstance(GetClassByName(GetModel(owlInstanceColor), "ColorComponent")), diffuse);
				}
			}

			{
				Int64 card = 0;
				IntPtr valuesPtr = IntPtr.Zero;
				GetObjectProperty(owlInstanceColor, GetPropertyByName(GetModel(owlInstanceColor), "emissive"), out valuesPtr, out card);

				if (card == 1)
				{
					Int64[] values = new Int64[card];
					System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
					SetColorOfComponent(values[0], emissive);
				}
				else
				{
					SetColorOfComponent(CreateInstance(GetClassByName(GetModel(owlInstanceColor), "ColorComponent")), emissive);
				}
			}

			{
				Int64 card = 0;
				IntPtr valuesPtr = IntPtr.Zero;
				GetObjectProperty(owlInstanceColor, GetPropertyByName(GetModel(owlInstanceColor), "specular"), out valuesPtr, out card);

				if (card == 1)
				{
					Int64[] values = new Int64[card];
					System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
					SetColorOfComponent(values[0], specular);
				}
				else
				{
					SetColorOfComponent(CreateInstance(GetClassByName(GetModel(owlInstanceColor), "ColorComponent")), specular);
				}
			}
		}

		/// <summary>
		///		GetMaterialColor                                        (http://rdf.bg/gkdoc/CS64/GetMaterialColor.html)
		///
		///	This function returns the color definition of any material instance. It will return default material
		///	in case the material does not have that specific color component defined.
		/// </summary>
		public static void GetMaterialColor(Int64 owlInstanceMaterial, out UInt32 ambient, out UInt32 diffuse, out UInt32 emissive, out UInt32 specular)
		{
			System.Diagnostics.Debug.Assert(IsInstanceOfClass(owlInstanceMaterial, "Material"));

			GetDefaultColor(GetModel(owlInstanceMaterial), out ambient, out diffuse, out emissive, out specular);

			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			GetObjectProperty(owlInstanceMaterial, GetPropertyByName(GetModel(owlInstanceMaterial), "color"), out valuesPtr, out card);

			if (card == 1)
			{
				Int64[] values = new Int64[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
				if (values[0] != 0)
				{
					GetColor(values[0], out ambient, out diffuse, out emissive, out specular);
				}
			}
		}

		/// <summary>
		///		SetMaterialColor                                        (http://rdf.bg/gkdoc/CS64/SetMaterialColor.html)
		///
		///	This function defines the color definition of any material instance.
		/// </summary>
		public static void SetMaterialColor(Int64 owlInstanceMaterial, UInt32 ambient, UInt32 diffuse, UInt32 emissive, UInt32 specular)
		{
			Int64 card = 0;
			IntPtr valuesPtr = IntPtr.Zero;
			GetObjectProperty(owlInstanceMaterial, GetPropertyByName(GetModel(owlInstanceMaterial), "color"), out valuesPtr, out card);

			if (card == 1)
			{
				Int64[] values = new Int64[card];
				System.Runtime.InteropServices.Marshal.Copy(valuesPtr, values, 0, (int) card);
				SetColor(values[0], ambient, diffuse, emissive, specular);
			}
			else
			{
				SetColor(CreateInstance(GetClassByName(GetModel(owlInstanceMaterial), "color")), ambient, diffuse, emissive, specular);
			}
		}

		/// <summary>
		///		GetMaterialColorAmbient                                 (http://rdf.bg/gkdoc/CS64/GetMaterialColorAmbient.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetMaterialColorAmbient(Int64 owlInstanceMaterial)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			GetMaterialColor(owlInstanceMaterial, out ambient, out diffuse, out emissive, out specular);
			return ambient;
		}

		/// <summary>
		///		GetMaterialColorDiffuse                                 (http://rdf.bg/gkdoc/CS64/GetMaterialColorDiffuse.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetMaterialColorDiffuse(Int64 owlInstanceMaterial)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			GetMaterialColor(owlInstanceMaterial, out ambient, out diffuse, out emissive, out specular);
			return diffuse;
		}

		/// <summary>
		///		GetMaterialColorEmissive                                (http://rdf.bg/gkdoc/CS64/GetMaterialColorEmissive.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetMaterialColorEmissive(Int64 owlInstanceMaterial)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			GetMaterialColor(owlInstanceMaterial, out ambient, out diffuse, out emissive, out specular);
			return emissive;
		}

		/// <summary>
		///		GetMaterialColorSpecular                                (http://rdf.bg/gkdoc/CS64/GetMaterialColorSpecular.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetMaterialColorSpecular(Int64 owlInstanceMaterial)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			GetMaterialColor(owlInstanceMaterial, out ambient, out diffuse, out emissive, out specular);
			return specular;
		}

		/// <summary>
		///		GetVertexColor                                          (http://rdf.bg/gkdoc/CS64/GetVertexColor.html)
		///
		///	Returns vertex color
		///	requiredColor is one of the control vertex data bits applied to colors (FORMAT_VERTEX_COLOR...) 
		///	If vertex format does provide required color, the model default color will be used
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, out UInt32 diffuse, out UInt32 emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, out UInt32 diffuse, out UInt32 emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, out UInt32 diffuse, IntPtr emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, out UInt32 diffuse, IntPtr emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, IntPtr diffuse, out UInt32 emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, IntPtr diffuse, out UInt32 emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, IntPtr diffuse, IntPtr emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, IntPtr diffuse, IntPtr emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, out UInt32 diffuse, out UInt32 emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, out UInt32 diffuse, out UInt32 emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, out UInt32 diffuse, IntPtr emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, out UInt32 diffuse, IntPtr emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, IntPtr diffuse, out UInt32 emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, IntPtr diffuse, out UInt32 emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, IntPtr diffuse, IntPtr emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, IntPtr diffuse, IntPtr emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, out UInt32 diffuse, out UInt32 emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, out UInt32 diffuse, out UInt32 emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, out UInt32 diffuse, IntPtr emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, out UInt32 diffuse, IntPtr emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, IntPtr diffuse, out UInt32 emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, IntPtr diffuse, out UInt32 emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, IntPtr diffuse, IntPtr emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, out UInt32 ambient, IntPtr diffuse, IntPtr emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, out UInt32 diffuse, out UInt32 emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, out UInt32 diffuse, out UInt32 emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, out UInt32 diffuse, IntPtr emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, out UInt32 diffuse, IntPtr emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, IntPtr diffuse, out UInt32 emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, IntPtr diffuse, out UInt32 emissive, IntPtr specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, IntPtr diffuse, IntPtr emissive, out UInt32 specular);

		[DllImport(enginedll, EntryPoint = "GetVertexColor")]
		public static extern void GetVertexColor(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting, IntPtr ambient, IntPtr diffuse, IntPtr emissive, IntPtr specular);

		/// <summary>
		///		GetVertexColorAmbient                                   (http://rdf.bg/gkdoc/CS64/GetVertexColorAmbient.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetVertexColorAmbient(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return ambient;
		}

		public static UInt32 GetVertexColorAmbient(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return ambient;
		}

		/// <summary>
		///		GetVertexColorDiffuse                                   (http://rdf.bg/gkdoc/CS64/GetVertexColorDiffuse.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetVertexColorDiffuse(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return diffuse;
		}

		public static UInt32 GetVertexColorDiffuse(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return diffuse;
		}

		/// <summary>
		///		GetVertexColorEmissive                                  (http://rdf.bg/gkdoc/CS64/GetVertexColorEmissive.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetVertexColorEmissive(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return emissive;
		}

		public static UInt32 GetVertexColorEmissive(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return emissive;
		}

		/// <summary>
		///		GetVertexColorSpecular                                  (http://rdf.bg/gkdoc/CS64/GetVertexColorSpecular.html)
		///
		///	...
		/// </summary>
		public static UInt32 GetVertexColorSpecular(Int64 model, ref float vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return specular;
		}

		public static UInt32 GetVertexColorSpecular(Int64 model, ref double vertexBuffer, Int64 vertexIndex, Int64 setting)
		{
			UInt32 ambient = 0, diffuse = 0, emissive = 0, specular = 0;
			GetVertexColor(model, ref vertexBuffer, vertexIndex, setting, out ambient, out diffuse, out emissive, out specular);
			return specular;
		}

        //
        //  Deprecated API Calls
        //

		/// <summary>
		///		GetConceptualFaceEx                                     (http://rdf.bg/gkdoc/CS64/GetConceptualFaceEx___.html)
		///
		///	Please rename GetConceptualFaceEx into GetConceptualFace.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, out Int64 startIndexTriangles, out Int64 noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, out Int64 startIndexLines, out Int64 noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, out Int64 startIndexPoints, out Int64 noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, out Int64 startIndexFacePolygons, out Int64 noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, out Int64 startIndexConceptualFacePolygons, out Int64 noIndicesConceptualFacePolygons);

		[DllImport(enginedll, EntryPoint = "GetConceptualFaceEx")]
		public static extern Int64 GetConceptualFaceEx(Int64 owlInstance, Int64 index, IntPtr startIndexTriangles, IntPtr noIndicesTriangles, IntPtr startIndexLines, IntPtr noIndicesLines, IntPtr startIndexPoints, IntPtr noIndicesPoints, IntPtr startIndexFacePolygons, IntPtr noIndicesFacePolygons, IntPtr startIndexConceptualFacePolygons, IntPtr noIndicesConceptualFacePolygons);

		/// <summary>
		///		GetTriangles                                            (http://rdf.bg/gkdoc/CS64/GetTriangles___.html)
		///
		///	This call is deprecated as it became trivial and will be removed by end of 2022. The result from CalculateInstance exclusively exists of the relevant triangles when
		///	SetFormat() is setting bit 8 and unsetting with bit 9, 10, 12 and 13 
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetTriangles")]
		public static extern void GetTriangles(Int64 owlInstance, out Int64 startIndex, out Int64 noTriangles, out Int64 startVertex, out Int64 firstNotUsedVertex);

		/// <summary>
		///		GetLines                                                (http://rdf.bg/gkdoc/CS64/GetLines___.html)
		///
		///	This call is deprecated as it became trivial and will be removed by end of 2022. The result from CalculateInstance exclusively exists of the relevant lines when
		///	SetFormat() is setting bit 9 and unsetting with bit 8, 10, 12 and 13 
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetLines")]
		public static extern void GetLines(Int64 owlInstance, out Int64 startIndex, out Int64 noLines, out Int64 startVertex, out Int64 firstNotUsedVertex);

		/// <summary>
		///		GetPoints                                               (http://rdf.bg/gkdoc/CS64/GetPoints___.html)
		///
		///	This call is deprecated as it became trivial and will be removed by end of 2022. The result from CalculateInstance exclusively exists of the relevant points when
		///	SetFormat() is setting bit 10 and unsetting with bit 8, 9, 12 and 13 
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPoints")]
		public static extern void GetPoints(Int64 owlInstance, out Int64 startIndex, out Int64 noPoints, out Int64 startVertex, out Int64 firstNotUsedVertex);

		/// <summary>
		///		GetPropertyRestrictionsConsolidated                     (http://rdf.bg/gkdoc/CS64/GetPropertyRestrictionsConsolidated___.html)
		///
		///	This call is deprecated and will be removed by end of 2022. Please use the call GetClassPropertyAggregatedCardinalityRestriction instead,
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertyRestrictionsConsolidated")]
		public static extern void GetPropertyRestrictionsConsolidated(Int64 owlClass, Int64 rdfProperty, out Int64 minCard, out Int64 maxCard);

		/// <summary>
		///		IsGeometryType                                          (http://rdf.bg/gkdoc/CS64/IsGeometryType___.html)
		///
		///	This call is deprecated and will be removed by end of 2022. Please use the call GetGeometryClass instead, rename the function name
		///	and interpret non-zero as true and zero as false.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "IsGeometryType")]
		public static extern byte IsGeometryType(Int64 owlClass);

		/// <summary>
		///		SetObjectTypeProperty                                   (http://rdf.bg/gkdoc/CS64/SetObjectTypeProperty___.html)
		///
		///	This call is deprecated and will be removed by end of 2022. Please use the call SetObjectProperty instead, just rename the function name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetObjectTypeProperty")]
		public static extern Int64 SetObjectTypeProperty(Int64 owlInstance, Int64 owlObjectProperty, ref Int64 values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetObjectTypeProperty")]
		public static extern Int64 SetObjectTypeProperty(Int64 owlInstance, Int64 owlObjectProperty, Int64[] values, Int64 card);

		/// <summary>
		///		GetObjectTypeProperty                                   (http://rdf.bg/gkdoc/CS64/GetObjectTypeProperty___.html)
		///
		///	This call is deprecated and will be removed by end of 2022. Please use the call GetObjectProperty instead, just rename the function name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetObjectTypeProperty")]
		public static extern Int64 GetObjectTypeProperty(Int64 owlInstance, Int64 owlObjectProperty, out IntPtr values, out Int64 card);

		/// <summary>
		///		SetDataTypeProperty                                     (http://rdf.bg/gkdoc/CS64/SetDataTypeProperty___.html)
		///
		///	This call is deprecated and will be removed by end of 2022. Please use the call SetDatatypeProperty instead, just rename the function name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "SetDataTypeProperty")]
		public static extern Int64 SetDataTypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, ref byte values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDataTypeProperty")]
		public static extern Int64 SetDataTypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, byte[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDataTypeProperty")]
		public static extern Int64 SetDataTypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, ref Int64 values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDataTypeProperty")]
		public static extern Int64 SetDataTypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, Int64[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDataTypeProperty")]
		public static extern Int64 SetDataTypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, ref double values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDataTypeProperty")]
		public static extern Int64 SetDataTypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, double[] values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDataTypeProperty")]
		public static extern Int64 SetDataTypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, ref string values, Int64 card);

		[DllImport(enginedll, EntryPoint = "SetDataTypeProperty")]
		public static extern Int64 SetDataTypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, string[] values, Int64 card);

		/// <summary>
		///		GetDataTypeProperty                                     (http://rdf.bg/gkdoc/CS64/GetDataTypeProperty___.html)
		///
		///	This call is deprecated and will be removed by end of 2022. Please use the call GetDatatypeProperty instead, just rename the function name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetDataTypeProperty")]
		public static extern Int64 GetDataTypeProperty(Int64 owlInstance, Int64 owlDatatypeProperty, out IntPtr values, out Int64 card);

		/// <summary>
		///		InstanceCopyCreated                                     (http://rdf.bg/gkdoc/CS64/InstanceCopyCreated___.html)
		///
		///	This call is deprecated as the Copy concept is also deprecated and will be removed by end of 2022.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "InstanceCopyCreated")]
		public static extern void InstanceCopyCreated(Int64 owlInstance);

		/// <summary>
		///		GetPropertyByNameAndType                                (http://rdf.bg/gkdoc/CS64/GetPropertyByNameAndType___.html)
		///
		///	This call is deprecated and will be removed by end of 2022.
		///	Please use the call GetPropertyByName(Ex) / GetPropertyByNameW(Ex) + GetPropertyType(Ex) instead, just rename the function name.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetPropertyByNameAndType")]
		public static extern Int64 GetPropertyByNameAndType(Int64 model, string name, Int64 rdfPropertyType);

		[DllImport(enginedll, EntryPoint = "GetPropertyByNameAndType")]
		public static extern Int64 GetPropertyByNameAndType(Int64 model, byte[] name, Int64 rdfPropertyType);

		/// <summary>
		///		GetParentsByIterator                                    (http://rdf.bg/gkdoc/CS64/GetParentsByIterator___.html)
		///
		///	Returns the next parent of the class or property.
		///	If input parent is zero, the handle will point to the first relevant parent.
		///	If all parent are past (or no relevant parent are found), the function will return 0.
		/// </summary>
		[DllImport(enginedll, EntryPoint = "GetParentsByIterator")]
		public static extern Int64 GetParentsByIterator(Int64 owlClassOrRdfProperty, Int64 parentOwlClassOrRdfProperty);
	}
}
