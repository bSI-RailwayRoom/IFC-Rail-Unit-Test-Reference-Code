//
//  Author:  Peter Bonsma
//  $Date: 1999-12-31 23:59:59 +0000 (Wed, 31 Jan 1999) $
//  $Revision: 3999 $
//  Project: Geometry Kernel
//
//  Be aware a license fee for use of this library when used commercially is required
//  For more info on commercial use please contact:  peter.bonsma@rdf.bg / contact@rdf.bg
//

#ifndef __RDF_LTD__ENGINE_H
#define __RDF_LTD__ENGINE_H


#include	"engdef.h"

#include	<assert.h>
#include	<string.h>

#if !defined _WINDOWS
#include	<stdint.h>
#endif


#ifdef _WINDOWS
	#ifdef _USRDLL
		#define DECL //__declspec(dllexport)
		//	The use of the Microsoft-specific
		//	__declspec(dllexport)
		//	is needed only when
		//	not using a .def file.
	#else
		#define DECL __declspec(dllimport) 
	#endif
	#define STDC __stdcall
#else
	#define DECL /*nothing*/
	#define STDC /*nothing*/
#endif


typedef		int64_t										RdfsResource;
typedef		RdfsResource								OwlModel;
typedef		RdfsResource								OwlClass;
typedef		RdfsResource								OwlInstance;
typedef		RdfsResource								RdfProperty;
typedef		RdfProperty									OwlDatatypeProperty;
typedef		RdfProperty									OwlObjectProperty;
typedef		int64_t										RdfPropertyType;
typedef		int64_t										ConceptualFace;

#define		OBJECTPROPERTY_TYPE							1
#define		OBJECTTYPEPROPERTY_TYPE						1
#define		DATATYPEPROPERTY_TYPE_BOOLEAN				2
#define		DATATYPEPROPERTY_TYPE_STRING				3		//	DATATYPEPROPERTY_TYPE_CHAR
#define		DATATYPEPROPERTY_TYPE_INTEGER				4
#define		DATATYPEPROPERTY_TYPE_DOUBLE				5
#define		DATATYPEPROPERTY_TYPE_BYTE					6
#define		DATATYPEPROPERTY_TYPE_CHAR_ARRAY			7
#define		DATATYPEPROPERTY_TYPE_WCHAR_T_ARRAY			8

//
// Flags can be used in bitwise combination for settings and masks to SetFormat, GetFormat and other functions working with settings
//

#define		FLAGBIT(n)									((int64_t) (((int64_t) 1L) << (n)))

//control precision / data size
#define		FORMAT_SIZE_VERTEX_DOUBLE					FLAGBIT(2)		//	Vertex items returned as double (8 byte/64 bit) else - as float (4 byte/32 bit)
#define		FORMAT_SIZE_INDEX_INT64						FLAGBIT(3)		//	Index items returned as int64_t (8 byte/64 bit) (only available in 64 bit mode) - else as int32_t (4 byte/32 bit)

//control vertex data
#define		FORMAT_VERTEX_POINT							FLAGBIT(4)		//	Vertex contains 3D point info
#define		FORMAT_VERTEX_NORMAL						FLAGBIT(5)		//	Vertex contains normat info
#define		FORMAT_VERTEX_TEXTURE_UV					FLAGBIT(6)		//	Vertex contains first 2D texture info
#define		FORMAT_VERTEX_TEXTURE_TANGENT				FLAGBIT(28)		//	Vertex does contain tangent vector for first texture
#define		FORMAT_VERTEX_TEXTURE_BINORMAL				FLAGBIT(29)		//	Vertex does contain binormal vector for first texture
#define		FORMAT_VERTEX_TEXTURE2_UV					FLAGBIT(7)		//	Vertex contains second 2D texture info
#define		FORMAT_VERTEX_TEXTURE2_TANGENT				FLAGBIT(30)		//	Vertex does contain tangent vector for second texture (only 64 bit platform)
#define		FORMAT_VERTEX_TEXTURE2_BINORMAL				FLAGBIT(31)		//	Vertex does contain binormal vector for second texture (only 64 bit platform)
#define		FORMAT_VERTEX_COLOR_AMBIENT					FLAGBIT(24)		//	Vertex does contain Ambient color information
#define		FORMAT_VERTEX_COLOR_DIFFUSE					FLAGBIT(25)		//	Vertex does contain Diffuse color information
#define		FORMAT_VERTEX_COLOR_EMISSIVE				FLAGBIT(26)		//	Vertex does contain Emissive color information
#define		FORMAT_VERTEX_COLOR_SPECULAR				FLAGBIT(27)		//	Vertex does contain Specular color information

//control CalculateInstance behaviour
#define		FORMAT_EXPORT_TRIANGLES						FLAGBIT(8)		//	Object form triangles are exported (effective if instance contains faces and/or solids)(triangulated surface representation)
#define		FORMAT_EXPORT_LINES							FLAGBIT(9)		//	Object polygon lines are exported (effective if instance contains line representations)
#define		FORMAT_EXPORT_POINTS						FLAGBIT(10)		//	Object points are exported (effective if instance contains point representations)
#define		FORMAT_EXPORT_FACE_POLYGONS					FLAGBIT(12)		//	Object face polygon lines (dense wireframe) are exported (requires FORMAT_FLAG_CONTAINS_TRIANGLES)
#define		FORMAT_EXPORT_CONCEPTUAL_FACE_POLYGONS		FLAGBIT(13)		//	Object conceptual face polygon lines (wireframe) are exported 
#define		FORMAT_EXPORT_POLYGONS_AS_TUPLES			FLAGBIT(14)		//	Polygon lines (wireframe) exported as tuples (edges) - else as list (loop)
#define		FORMAT_EXPORT_ADVANCED_NORMALS				FLAGBIT(15)		//	Normals are exported to be in line with the original semantic form description (orthogonal to conceprual face, could be non orthogonal to the planar face or triangle) - else all normals of triangles are transformed orthogonal to the palnar face or triangle they belong to
#define		FORMAT_EXPORT_DIRECTX						FLAGBIT(16)		//	Where possible DirectX compatibility is given to exported data. Unsets FORMAT_FLAG_OPENGL, FORMAT_FLAG_VERSION_0001, FORMAT_FLAG_VERSION_0010
#define		FORMAT_EXPORT_OPENGL						FLAGBIT(17)		//	Where possible OpenGL compatibility is given to exported data. Unsets FORMAT_FLAG_DIRECTX. Sets FORMAT_FLAG_VERSION_0001, FORMAT_FLAG_VERSION_0010
#define		FORMAT_EXPORT_DOUBLE_SIDED					FLAGBIT(18)		//	Every face has exactly one opposite face (normally both index and vertex array are doubled in size)
#define		FORMAT_EXPORT_VERSION_0001					FLAGBIT(20)		//	Opposite Triangle Rotation (RHS as expected by OpenGL) - else  Standard Triangle Rotation (LHS as expected by DirectX)
#define		FORMAT_EXPORT_VERSION_0010					FLAGBIT(21)		//	X, Y, Z (nX, nY, nZ) formatted as , i.e. X, -Z, Y (nX, -nZ, nY) considering internal concepts (OpenGL) - else X, Y, Z (nX, nY, nZ) formatted as considering internal concepts
#define		FORMAT_EXPORT_VERSION_0100					FLAGBIT(22)
#define		FORMAT_EXPORT_VERSION_1000					FLAGBIT(23)

#define GetVertexElementsCount(setting) GetVertexDataOffset(0, setting)	// Get number of elements in verctor data array with the format settings

//
//  Color pack/unpack
//

//get color from its components in range 0..255
#define		COLOR_RGBW255(r,g,b,w)						(											\
															((uint32_t) (unsigned char)(r) << 24)	\
														|	((uint32_t) (unsigned char)(g) << 16)	\
														|	((uint32_t) (unsigned char)(b) << 8 )	\
														|	((uint32_t) (unsigned char)(w)      )	\
														)
//get color from its components in range 0..1 
#define		COLOR_RGBW(r,g,b,w)							COLOR_RGBW255(r*255,g*255,b*255,w*255)

#define		COLOR_GET_R255(clr)							((unsigned char) (((uint32_t) (clr) >> 24) &0xFF))	//	get color red component in range 0..255
#define		COLOR_GET_G255(clr)							((unsigned char) (((uint32_t) (clr) >> 16) &0xFF))	//	get color green component in range 0..255
#define		COLOR_GET_B255(clr)							((unsigned char) (((uint32_t) (clr) >> 8 ) &0xFF))	//	get color blue component in range 0..255
#define		COLOR_GET_W255(clr)							((unsigned char) (((uint32_t) (clr)  	 ) &0xFF))	//	get color transparency in range 0..255

#define		COLOR_GET_R(clr)							(COLOR_GET_R255(clr)/255.0)							//	get color red component in range 0..1
#define		COLOR_GET_G(clr)							(COLOR_GET_G255(clr)/255.0)							//	get color green component in range 0..1
#define		COLOR_GET_B(clr)							(COLOR_GET_B255(clr)/255.0)							//	get color blue component in range 0..1
#define		COLOR_GET_W(clr)							(COLOR_GET_W255(clr)/255.0)							//	get color trancparency in range 0..1


#define		COLOR_ARR_RGBW255(r)						COLOR_RGBW255(r[0],r[1],r[2],r[3])					//	get color from array of 4 components in range 0..255
#define		COLOR_ARR_RGBW(r)							COLOR_RGBW(r[0],r[1],r[2],r[3])						//	get color from array of 4 components in range 0..1

//get color comonents in range 0..255 to arry of 4 elements
#define		COLOR_GET_COMPONENTS255(r,clr)				{r[0]=COLOR_GET_R255(clr); r[1]=COLOR_GET_G255(clr); r[2]=COLOR_GET_B255(clr); r[3]=COLOR_GET_W255(clr);}

//get color comonents in range 0..1 to arry of 4 elements
#define		COLOR_GET_COMPONENTS(r,clr)					{r[0]=COLOR_GET_R(clr); r[1]=COLOR_GET_G(clr); r[2]=COLOR_GET_B(clr); r[3]=COLOR_GET_W(clr);}

//
//  Meta information API Calls
//

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetRevision                                             (http://rdf.bg/gkdoc/CP64/GetRevision.html)
//				const char				** timeStamp						IN / OUT
//
//				int64_t					returns								OUT
//
//	Returns the revision number.
//	The timeStamp is generated by the SVN system used during development.
//
int64_t			DECL STDC	GetRevision(
									const char				** timeStamp
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	GetRevision(
								char					** timeStamp
							)
{
	return	GetRevision(
					(const char**) timeStamp
				);
}

//
//
static	inline	int64_t	GetRevision(
							)
{
	return	GetRevision(
					(const char**) nullptr				//	timeStamp
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetRevisionW                                            (http://rdf.bg/gkdoc/CP64/GetRevisionW.html)
//				const wchar_t			** timeStamp						IN / OUT
//
//				int64_t					returns								OUT
//
//	Returns the revision number.
//	The timeStamp is generated by the SVN system used during development.
//
int64_t			DECL STDC	GetRevisionW(
									const wchar_t			** timeStamp
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	GetRevisionW(
								wchar_t					** timeStamp
							)
{
	return	GetRevisionW(
					(const wchar_t**) timeStamp
				);
}

//
//
static	inline	int64_t	GetRevisionW(
							)
{
	return	GetRevisionW(
					(const wchar_t**) nullptr			//	timeStamp
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetProtection                                           (http://rdf.bg/gkdoc/CP64/GetProtection.html)
//				int64_t					returns								OUT
//
//	This call is required to be called to enable the DLL to work if protection is active.
//
//	Returns the number of days (incl. this one) that this version is still active or 0 if no protection is embedded.
//	In case no days are left and protection is active this call will return -1.
//
int64_t			DECL STDC	GetProtection(
								);

//
//		GetEnvironment                                          (http://rdf.bg/gkdoc/CP64/GetEnvironment.html)
//				const char				** environmentVariables				IN / OUT
//				const char				** developmentVariables				IN / OUT
//
//				int64_t					returns								OUT
//
//	Returns the revision number similar to the call GetRevision.
//	The environment variables will show known environment variables
//	and if they are set, for example environment variables ABC known
//	and unset and DEF as well as GHI known and set:
//		environmentVariables = "ABC:F;DEF:T;GHI:T"
//	Development variables are depending on the build environment
//	As an example in windows systems where Visual Studio is used:
//		developmentVariables = "...."
//
int64_t			DECL STDC	GetEnvironment(
									const char				** environmentVariables,
									const char				** developmentVariables
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	GetEnvironment(
								char					** environmentVariables,
								char					** developmentVariables
							)
{
	return	GetEnvironment(
					(const char**) environmentVariables,
					(const char**) developmentVariables
				);
}

//
//
static	inline	int64_t	GetEnvironment(
							)
{
	return	GetEnvironment(
					(const char**) nullptr,				//	environmentVariables
					(const char**) nullptr				//	developmentVariables
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetEnvironmentW                                         (http://rdf.bg/gkdoc/CP64/GetEnvironmentW.html)
//				const wchar_t			** environmentVariables				IN / OUT
//				const wchar_t			** developmentVariables				IN / OUT
//
//				int64_t					returns								OUT
//
//	Returns the revision number similar to the call GetRevision[W].
//	The environment variables will show known environment variables
//	and if they are set, for example environment variables ABC known
//	and unset and DEF as well as GHI known and set:
//		environmentVariables = "ABC:F;DEF:T;GHI:T"
//	Development variables are depending on the build environment
//	As an example in windows systems where Visual Studio is used:
//		developmentVariables = "...."
//
int64_t			DECL STDC	GetEnvironmentW(
									const wchar_t			** environmentVariables,
									const wchar_t			** developmentVariables
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	GetEnvironmentW(
								wchar_t					** environmentVariables,
								wchar_t					** developmentVariables
							)
{
	return	GetEnvironmentW(
					(const wchar_t**) environmentVariables,
					(const wchar_t**) developmentVariables
				);
}

//
//
static	inline	int64_t	GetEnvironmentW(
							)
{
	return	GetEnvironmentW(
					(const wchar_t**) nullptr,			//	environmentVariables
					(const wchar_t**) nullptr			//	developmentVariables
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetAssertionFile                                        (http://rdf.bg/gkdoc/CP64/SetAssertionFile.html)
//				const char				* fileName							IN
//
//				void					returns
//
//	This function sets the file location where internal assertions should be written to.
//	If the file name is not set (default) many internal control procedures are not executed
//	and the code will be faster.
//
void			DECL STDC	SetAssertionFile(
									const char				* fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	void	SetAssertionFile(
								char					* fileName
							)
{
	return	SetAssertionFile(
					(const char*) fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetAssertionFileW                                       (http://rdf.bg/gkdoc/CP64/SetAssertionFileW.html)
//				const wchar_t			* fileName							IN
//
//				void					returns
//
//	This function sets the file location where internal assertions should be written to.
//	If the file name is not set (default) many internal control procedures are not executed
//	and the code will be faster.
//
void			DECL STDC	SetAssertionFileW(
									const wchar_t			* fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	void	SetAssertionFileW(
								wchar_t					* fileName
							)
{
	return	SetAssertionFileW(
					(const wchar_t*) fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetAssertionFile                                        (http://rdf.bg/gkdoc/CP64/GetAssertionFile.html)
//				const char				** fileName							IN / OUT
//
//				const char				* returns							OUT
//
//	This function gets the file location as stored/set internally where internal assertions should be written to.
//	It works independent if the file location is set through SetAssertionFile() or SetAssertionFileW().
//
const char		DECL * STDC	GetAssertionFile(
									const char				** fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const char	* GetAssertionFile(
									char					** fileName
								)
{
	return	GetAssertionFile(
					(const char**) fileName
				);
}

//
//
static	inline	const char	* GetAssertionFile(
								)
{
	return	GetAssertionFile(
					(const char**) nullptr				//	fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetAssertionFileW                                       (http://rdf.bg/gkdoc/CP64/GetAssertionFileW.html)
//				const wchar_t			** fileName							IN / OUT
//
//				const wchar_t			* returns							OUT
//
//	This function gets the file location as stored/set internally where internal assertions should be written to.
//	It works independent if the file location is set through SetAssertionFile() or SetAssertionFileW().
//
const wchar_t	DECL * STDC	GetAssertionFileW(
									const wchar_t			** fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const wchar_t	* GetAssertionFileW(
										wchar_t					** fileName
									)
{
	return	GetAssertionFileW(
					(const wchar_t**) fileName
				);
}

//
//
static	inline	const wchar_t	* GetAssertionFileW(
									)
{
	return	GetAssertionFileW(
					(const wchar_t**) nullptr			//	fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetCharacterSerialization                               (http://rdf.bg/gkdoc/CP64/SetCharacterSerialization.html)
//				OwlModel				model								IN
//				int64_t					encoding							IN
//				int64_t					wcharBitSizeOverride				IN
//				bool					ascii								IN
//
//				int64_t					returns								OUT
//
//	This call defines how characters for names, strings will be serializaed and how
//	they are expected to be serialized. An exception are the Open / Import / Save calls,
//	these calls have a fixed way of serialization of path / file names.
//
//	If the encoding value is non-zero the following values are possible (if zero encoding is kept as defined)
//		 32 [default]	encoding ignored
//		 64				encoding Windows 1250
//		 65				encoding Windows 1251
//		 66				encoding Windows 1252
//		 67				encoding Windows 1253
//		 68				encoding Windows 1254
//		 69				encoding Windows 1255
//		 70				encoding Windows 1256
//		 71				encoding Windows 1257
//		 72				encoding Windows 1258
//		128				encoding ISO8859 1
//		129				encoding ISO8859 2
//		130				encoding ISO8859 3
//		131				encoding ISO8859 4
//		132				encoding ISO8859 5
//		133				encoding ISO8859 6
//		134				encoding ISO8859 7
//		135				encoding ISO8859 8
//		136				encoding ISO8859 9
//		137				encoding ISO8859 10
//		138				encoding ISO8859 11
//						encoding ISO8859 12 => does not exist
//		140				encoding ISO8859 13
//		141				encoding ISO8859 14
//		142				encoding ISO8859 15
//		143				encoding ISO8859 16
//		160				encoding MACINTOSH CENTRAL EUROPEAN
//		192				encoding SHIFT JIS X 213
//
//	The wcharBitSizeOverride value overrides the OS based size of wchar_t, the following values can be applied:
//		0			wcharBitSizeOverride is ignored, override is not changed
//		16			wchar_t interpreted as being 2 bytes wide (size of wchar_t in bits)
//		32			wchar_t interpreted as being 4 bytes wide (size of wchar_t in bits)
//		Any other value will reset the override and wchar_t will follow the OS based size of wchar_t
//	Note: this setting is independent from the model, this call can also be called without a model defined.
//
//	The ascii value defines
//		true [default]	8 bit serializatiom (size of char returned in bits)
//		false			16/32 bit serialization (depending on the operating system, i.e. sizeof of wchar_t returned in number of bits)
//	Note: this setting is model-dependent and requires a model present to have any effect.
//
//	The return value is the size of a single character in bits, i.e. 1 byte is 8 bits, the value for a wchar_t can be 16 or 32 depending on settings and operating system
//
int64_t			DECL STDC	SetCharacterSerialization(
									OwlModel				model,
									int64_t					encoding,
									int64_t					wcharBitSizeOverride,
									bool					ascii
								);

//
//		GetCharacterSerialization                               (http://rdf.bg/gkdoc/CP64/GetCharacterSerialization.html)
//				OwlModel				model								IN
//				int64_t					* encoding							IN / OUT
//				bool					* ascii								IN / OUT
//
//				int64_t					returns								OUT
//
//	This call retrieves the values as set by 
//
//	The returns the size of a single character in bits, i.e. 1 byte is 8 bits, this can be 8, 16 or 32 depending on settings and operating system
//
int64_t			DECL STDC	GetCharacterSerialization(
									OwlModel				model,
									int64_t					* encoding,
									bool					* ascii
								);

//
//		SetModellingStyle                                       (http://rdf.bg/gkdoc/CP64/SetModellingStyle.html)
//				OwlModel				model								IN
//				uint64_t				setting								IN
//				uint64_t				mask								IN
//
//				void					returns
//
//	This call sets the modelling style.
//
void			DECL STDC	SetModellingStyle(
									OwlModel				model,
									uint64_t				setting,
									uint64_t				mask
								);

//
//		GetModellingStyle                                       (http://rdf.bg/gkdoc/CP64/GetModellingStyle.html)
//				OwlModel				model								IN
//				uint64_t				mask								IN
//
//				uint64_t				returns								OUT
//
//	This call gets the modelling style.
//
uint64_t		DECL STDC	GetModellingStyle(
									OwlModel				model,
									uint64_t				mask
								);

//
//		AbortModel                                              (http://rdf.bg/gkdoc/CP64/AbortModel.html)
//				OwlModel				model								IN
//				int64_t					setting								IN
//
//				int64_t					returns								OUT
//
//	This function abort running processes for a model. It can be used when a task takes more time than
//	expected / available, or in case the requested results are not relevant anymore.
//
int64_t			DECL STDC	AbortModel(
									OwlModel				model,
									int64_t					setting
								);

//
//		GetSessionMetaInfo                                      (http://rdf.bg/gkdoc/CP64/GetSessionMetaInfo.html)
//				int64_t					* allocatedBlocks					IN / OUT
//				int64_t					* allocatedBytes					IN / OUT
//				int64_t					* nonUsedBlocks						IN / OUT
//				int64_t					* nonUsedBytes						IN / OUT
//
//				int64_t					returns								OUT
//
//	This function is meant for debugging purposes and return statistics during processing.
//	The return value represents the number of active models within the session (or zero if the model was not recognized).
//
int64_t			DECL STDC	GetSessionMetaInfo(
									int64_t					* allocatedBlocks,
									int64_t					* allocatedBytes,
									int64_t					* nonUsedBlocks,
									int64_t					* nonUsedBytes
								);

//
//		GetModelMetaInfo                                        (http://rdf.bg/gkdoc/CP64/GetModelMetaInfo.html)
//				OwlModel				model								IN
//				int64_t					* activeClasses						IN / OUT
//				int64_t					* removedClasses					IN / OUT
//				int64_t					* activeProperties					IN / OUT
//				int64_t					* removedProperties					IN / OUT
//				int64_t					* activeInstances					IN / OUT
//				int64_t					* removedInstances					IN / OUT
//				int64_t					* inactiveInstances					IN / OUT
//
//				int64_t					returns								OUT
//
//	This function is meant for debugging purposes and return statistics during processing.
//	The return value represents the number of active models within the session (or zero if the model was not recognized).
//
int64_t			DECL STDC	GetModelMetaInfo(
									OwlModel				model,
									int64_t					* activeClasses,
									int64_t					* removedClasses,
									int64_t					* activeProperties,
									int64_t					* removedProperties,
									int64_t					* activeInstances,
									int64_t					* removedInstances,
									int64_t					* inactiveInstances
								);

//
//		GetInstanceMetaInfo                                     (http://rdf.bg/gkdoc/CP64/GetInstanceMetaInfo.html)
//				OwlInstance				owlInstance							IN
//				int64_t					* allocatedBlocks					IN / OUT
//				int64_t					* allocatedBytes					IN / OUT
//
//				int64_t					returns								OUT
//
//	This function is meant for debugging purposes and return statistics during processing.
//	The return value represents the number of active instances within the model (or zero if the instance was not recognized).
//
int64_t			DECL STDC	GetInstanceMetaInfo(
									OwlInstance				owlInstance,
									int64_t					* allocatedBlocks,
									int64_t					* allocatedBytes
								);

//
//		GetSmoothness                                           (http://rdf.bg/gkdoc/CP64/GetSmoothness.html)
//				OwlInstance				owlInstance							IN
//				int64_t					* degree							IN / OUT
//
//				int64_t					returns								OUT
//
//	This function returns the smoothness of a line or surface.
//	In case the smoothness can be defined the degree will get assigned either
//		0 - continuous curve / surface (i.e. degree 9)
//		1 - the direction of the curve / surface is gradually changing (i.e. degree 1)
//		2 - the change of direction of the curve / surface is gradually changing (i.e. degree 2)
//	In return value of this function retuns the dimension of the found smoothness:
//		0 - smoothness could not be defined
//		1 - found the smoothness of a curve
//		2 - found the smoothness of a surface
//
int64_t			DECL STDC	GetSmoothness(
									OwlInstance				owlInstance,
									int64_t					* degree
								);

//
//		AddState                                                (http://rdf.bg/gkdoc/CP64/AddState.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//
//				void					returns
//
//	This call will integrate the current state information into the model.
//
//	Model should be non-zero.
//
//	If owlInstance is given the state is only applied on the owlInstance and all its children.
//	If owlInstance is zero the state is applied on all owlInstances within a model.
//
void			DECL STDC	AddState(
									OwlModel				model,
									OwlInstance				owlInstance
								);

//
//		GetModel                                                (http://rdf.bg/gkdoc/CP64/GetModel.html)
//				RdfsResource			rdfsResource						IN
//
//				OwlModel				returns								OUT
//
//	Returns model for any resource, i.e. class, property, instance
//
OwlModel		DECL STDC	GetModel(
									RdfsResource			rdfsResource
								);

//
//		OrderedHandles                                          (http://rdf.bg/gkdoc/CP64/OrderedHandles.html)
//				OwlModel				model								IN
//				int64_t					* classCnt							IN / OUT
//				int64_t					* propertyCnt						IN / OUT
//				int64_t					* instanceCnt						IN / OUT
//				uint64_t				setting								IN
//				uint64_t				mask								IN
//
//				void					returns
//
//	This call can be used in two ways. The optional arguments classCnt,
//	propertyCnt and instanceCnt can be used to get the total amount of active classes,
//	properies and instances available within the model.
//
//	The setting and mask can be used to order the handles given for classes,
//	properties and instances.
//		1 - if set this will number all classes with possible values [1 .. classCnt]
//		2 - if set this will number all classes with possible values [1 .. propertyCnt]
//		4 - if set this will number all classes with possible values [1 .. instanceCnt]
//
//	Note: when enabling ordered handles be aware that classes, properties and instances
//		  can share the same handles, using the correct argument cannot be checked anymore
//		  by the library itself. This could result in crashes in case of incorrect assignments
//		  by the hosting application.
//	Note: internally there is no performance gain / loss. This is purely meant for situations
//		  where the hosting application can benefit performance wise from having an ordered list.
//	Note: use in combination with other libraries is not adviced, i.e. when combined with the
//		  IFC generation from the IFC Engine component for example
//
void			DECL STDC	OrderedHandles(
									OwlModel				model,
									int64_t					* classCnt,
									int64_t					* propertyCnt,
									int64_t					* instanceCnt,
									uint64_t				setting,
									uint64_t				mask
								);

//
//		PeelArray                                               (http://rdf.bg/gkdoc/CP64/PeelArray.html)
//				const void				** inValue							IN
//				const void				** outValue							IN / OUT
//				int64_t					elementSize							IN
//
//				void					returns
//
//	This function introduces functionality that is missing or complicated in some programming languages.
//	The attribute inValue is a reference to an array of references. The attribute outValue is a reference to the same array,
//	however a number of elements earlier or further, i.e. number of elements being attribute elementSize. Be aware that as
//	we are talking about references the offset is depending on 32 bit / 64 bit compilation.
//
void			DECL STDC	PeelArray(
									const void				** inValue,
									const void				** outValue,
									int64_t					elementSize
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	void	PeelArray(
								void					** inValue,
								void					** outValue,
								int64_t					elementSize
							)
{
	return	PeelArray(
					(const void**) inValue,
					(const void**) outValue,
					elementSize
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetInternalCheck                                        (http://rdf.bg/gkdoc/CP64/SetInternalCheck.html)
//				OwlModel				model								IN
//				uint64_t				setting								IN
//				uint64_t				mask								IN
//
//				void					returns
//
//	This function allows to enable or disable several active consistency checks. Enabling the checks can 
//	introduce performance effects; it is helpfull for and meant for debugging on client side.
//	If model is zero the consistency checks are set for all open and to be created models.
//
void			DECL STDC	SetInternalCheck(
									OwlModel				model,
									uint64_t				setting,
									uint64_t				mask
								);

//
//		GetInternalCheck                                        (http://rdf.bg/gkdoc/CP64/GetInternalCheck.html)
//				OwlModel				model								IN
//				uint64_t				mask								IN
//
//				uint64_t				returns								OUT
//
//	This function returns all current enabled active consistency checks given the mask the function is 
//	called for.
//	When leaving mask and settinbg zero it will return all bits that can be set.
//
uint64_t		DECL STDC	GetInternalCheck(
									OwlModel				model,
									uint64_t				mask
								);

//
//		GetInternalCheckIssueCnt                                (http://rdf.bg/gkdoc/CP64/GetInternalCheckIssueCnt.html)
//				OwlModel				model								IN
//
//				int64_t					returns								OUT
//
//	This function returns all issues found and not retrieved by the hosting application through 
//	GetInternalCheckIssue() / GetInternalCheckIssueW().
//
int64_t			DECL STDC	GetInternalCheckIssueCnt(
									OwlModel				model
								);

//
//		GetInternalCheckIssue                                   (http://rdf.bg/gkdoc/CP64/GetInternalCheckIssue.html)
//				OwlModel				model								IN
//				const char				** name								IN / OUT
//				const char				** description						IN / OUT
//				int64_t					* relatedOwlInstance				IN / OUT
//
//				void					returns
//
//	This function returns the oldest issues in the list of issues and reduces the list of issues with 1.
//	The name and description represent the issue as ASCII string, if relevant the relating owlInstance
//	will be returned through relatedOwlInstance.
//	Namer, Description and relatedOwlInstance are optional.
//
void			DECL STDC	GetInternalCheckIssue(
									OwlModel				model,
									const char				** name,
									const char				** description,
									int64_t					* relatedOwlInstance
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	void	GetInternalCheckIssue(
								OwlModel				model,
								char					** name,
								char					** description,
								int64_t					* relatedOwlInstance
							)
{
	return	GetInternalCheckIssue(
					model,
					(const char**) name,
					(const char**) description,
					relatedOwlInstance
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetInternalCheckIssueW                                  (http://rdf.bg/gkdoc/CP64/GetInternalCheckIssueW.html)
//				OwlModel				model								IN
//				const wchar_t			** name								IN / OUT
//				const wchar_t			** description						IN / OUT
//				int64_t					* relatedOwlInstance				IN / OUT
//
//				void					returns
//
//	This function returns the oldest issues in the list of issues and reduces the list of issues with 1.
//	The name and description represent the issue as Unicode string, if relevant the relating owlInstance
//	will be returned through relatedOwlInstance.
//	Namer, Description and relatedOwlInstance are optional.
//
void			DECL STDC	GetInternalCheckIssueW(
									OwlModel				model,
									const wchar_t			** name,
									const wchar_t			** description,
									int64_t					* relatedOwlInstance
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	void	GetInternalCheckIssueW(
								OwlModel				model,
								wchar_t					** name,
								wchar_t					** description,
								int64_t					* relatedOwlInstance
							)
{
	return	GetInternalCheckIssueW(
					model,
					(const wchar_t**) name,
					(const wchar_t**) description,
					relatedOwlInstance
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		CloseSession                                            (http://rdf.bg/gkdoc/CP64/CloseSession.html)
//				int64_t					returns								OUT
//
//	This function closes the session, after this call the geometry kernel cannot be used anymore.
//
int64_t			DECL STDC	CloseSession(
								);

//
//		CleanMemory                                             (http://rdf.bg/gkdoc/CP64/CleanMemory.html)
//				void					returns
//
//		This function ..
//
void			DECL STDC	CleanMemory(
								);

//
//		ClearCache                                              (http://rdf.bg/gkdoc/CP64/ClearCache.html)
//				OwlModel				model								IN
//
//				void					returns
//
//		This function ..
//
void			DECL STDC	ClearCache(
									OwlModel				model
								);

//
//		AllocModelMemory                                        (http://rdf.bg/gkdoc/CP64/AllocModelMemory.html)
//				OwlModel				model								IN
//				int64_t					size								IN
//
//				int64_t					returns								OUT
//
//	Allocates model associated memory.
//	Memory is disposed when model is closed
//
int64_t			DECL STDC	AllocModelMemory(
									OwlModel				model,
									int64_t					size
								);

//
//		SetExternalReferenceData                                (http://rdf.bg/gkdoc/CP64/SetExternalReferenceData.html)
//				RdfsResource			rdfsResource						IN
//				int64_t					identifier							IN
//				void					* data								IN / OUT
//
//				int64_t					returns								OUT
//
//	Sets application data on model, class, property, instance
//	Returns 0 on error, 1 on success
//
int64_t			DECL STDC	SetExternalReferenceData(
									RdfsResource			rdfsResource,
									int64_t					identifier,
									void					* data
								);

//
//		GetExternalReferenceData                                (http://rdf.bg/gkdoc/CP64/GetExternalReferenceData.html)
//				RdfsResource			rdfsResource						IN
//				int64_t					identifier							IN
//
//				int64_t					returns								OUT
//
//	Gets application data from model, class, property, instance that were previosly set by SetExternalReferenceData
//	Returns 0 on error, 1 on success
//
int64_t			DECL STDC	GetExternalReferenceData(
									RdfsResource			rdfsResource,
									int64_t					identifier
								);

//
//		GetExternalReferenceDataId                              (http://rdf.bg/gkdoc/CP64/GetExternalReferenceDataId.html)
//				OwlModel				model								IN
//				const char				* uniqueAppName						IN
//
//				int64_t					returns								OUT
//
//	Returns a key id can be used in calls to Get/SetExternalReferenceData to keep application data on GK entities
//	During model lifetime the id is the same for given string and different for different strings
//	Returns 0 on error
//
int64_t			DECL STDC	GetExternalReferenceDataId(
									OwlModel				model,
									const char				* uniqueAppName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	GetExternalReferenceDataId(
								OwlModel				model,
								char					* uniqueAppName
							)
{
	return	GetExternalReferenceDataId(
					model,
					(const char*) uniqueAppName
				);
}

//
//  File IO / Stream / Copy API Calls
//

#ifdef __cplusplus
	extern "C" {
#endif

//
//		CreateModel                                             (http://rdf.bg/gkdoc/CP64/CreateModel.html)
//				OwlModel				returns								OUT
//
//	This function creates and empty model.
//	References inside to other ontologies will be included.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
OwlModel		DECL STDC	CreateModel(
								);

//
//		OpenModel                                               (http://rdf.bg/gkdoc/CP64/OpenModel.html)
//				const char				* fileName							IN
//
//				OwlModel				returns								OUT
//
//	This function opens the model on location file name.
//	References inside to other ontologies will be included.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
OwlModel		DECL STDC	OpenModel(
									const char				* fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlModel	OpenModel(
									char					* fileName
								)
{
	return	OpenModel(
					(const char*) fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		OpenModelW                                              (http://rdf.bg/gkdoc/CP64/OpenModelW.html)
//				const wchar_t			* fileName							IN
//
//				OwlModel				returns								OUT
//
//	This function opens the model on location file name.
//	References inside to other ontologies will be included.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
OwlModel		DECL STDC	OpenModelW(
									const wchar_t			* fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlModel	OpenModelW(
									wchar_t					* fileName
								)
{
	return	OpenModelW(
					(const wchar_t*) fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		OpenModelS                                              (http://rdf.bg/gkdoc/CP64/OpenModelS.html)
//				const void				* callback							IN
//
//				OwlModel				returns								OUT
//
//	This function opens the model via a stream.
//	References inside to other ontologies will be included.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
OwlModel		DECL STDC	OpenModelS(
									const void				* callback
								);

//
//		OpenModelA                                              (http://rdf.bg/gkdoc/CP64/OpenModelA.html)
//				const unsigned char		* content							IN
//				int64_t					size								IN
//
//				OwlModel				returns								OUT
//
//	This function opens the model via an array.
//	References inside to other ontologies will be included.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
OwlModel		DECL STDC	OpenModelA(
									const unsigned char		* content,
									int64_t					size
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlModel	OpenModelA(
									const unsigned char		* content
								)
{
	return	OpenModelA(
					content,
					strlen((const char*) content)	//	size
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		ImportModel                                             (http://rdf.bg/gkdoc/CP64/ImportModel.html)
//				OwlModel				model								IN
//				const char				* fileName							IN
//
//				OwlInstance				returns								OUT
//
//	This function imports a design tree on location file name.
//	The design tree will be added to the given existing model.
//	The return value contains the first instance not referenced by any other instance or zero 
//	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
//	unique and equal to the instance used within the call SaveInstanceTree().
//
OwlInstance		DECL STDC	ImportModel(
									OwlModel				model,
									const char				* fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlInstance	ImportModel(
									OwlModel				model,
									char					* fileName
								)
{
	return	ImportModel(
					model,
					(const char*) fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		ImportModelW                                            (http://rdf.bg/gkdoc/CP64/ImportModelW.html)
//				OwlModel				model								IN
//				const wchar_t			* fileName							IN
//
//				OwlInstance				returns								OUT
//
//	This function imports a design tree on location file name.
//	The design tree will be added to the given existing model.
//	The return value contains the first instance not referenced by any other instance or zero 
//	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
//	unique and equal to the instance used within the call SaveInstanceTree().
//
OwlInstance		DECL STDC	ImportModelW(
									OwlModel				model,
									const wchar_t			* fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlInstance	ImportModelW(
									OwlModel				model,
									wchar_t					* fileName
								)
{
	return	ImportModelW(
					model,
					(const wchar_t*) fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		ImportModelS                                            (http://rdf.bg/gkdoc/CP64/ImportModelS.html)
//				OwlModel				model								IN
//				const void				* callback							IN
//
//				OwlInstance				returns								OUT
//
//	This function imports a design tree via a stream.
//	The design tree will be added to the given existing model.
//	The return value contains the first instance not referenced by any other instance or zero 
//	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
//	unique and equal to the instance used within the call SaveInstanceTree().
//
OwlInstance		DECL STDC	ImportModelS(
									OwlModel				model,
									const void				* callback
								);

//
//		ImportModelA                                            (http://rdf.bg/gkdoc/CP64/ImportModelA.html)
//				OwlModel				model								IN
//				const unsigned char		* content							IN
//				int64_t					size								IN
//
//				OwlInstance				returns								OUT
//
//	This function imports a design tree via an array.
//	The design tree will be added to the given existing model.
//	The return value contains the first instance not referenced by any other instance or zero 
//	if it does not exist. In case the imported model is created with SaveInstanceTree() this instance is 
//	unique and equal to the instance used within the call SaveInstanceTree().
//
OwlInstance		DECL STDC	ImportModelA(
									OwlModel				model,
									const unsigned char		* content,
									int64_t					size
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlInstance	ImportModelA(
									OwlModel				model,
									const unsigned char		* content
								)
{
	return	ImportModelA(
					model,
					content,
					strlen((const char*) content)	//	size
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SaveInstanceTree                                        (http://rdf.bg/gkdoc/CP64/SaveInstanceTree.html)
//				OwlInstance				owlInstance							IN
//				const char				* fileName							IN
//
//				int64_t					returns								OUT
//
//	This function saves the selected instance and its dependancies on location file name.
//
int64_t			DECL STDC	SaveInstanceTree(
									OwlInstance				owlInstance,
									const char				* fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SaveInstanceTree(
								OwlInstance				owlInstance,
								char					* fileName
							)
{
	return	SaveInstanceTree(
					owlInstance,
					(const char*) fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SaveInstanceTreeW                                       (http://rdf.bg/gkdoc/CP64/SaveInstanceTreeW.html)
//				OwlInstance				owlInstance							IN
//				const wchar_t			* fileName							IN
//
//				int64_t					returns								OUT
//
//	This function saves the selected instance and its dependancies on location file name.
//
int64_t			DECL STDC	SaveInstanceTreeW(
									OwlInstance				owlInstance,
									const wchar_t			* fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SaveInstanceTreeW(
								OwlInstance				owlInstance,
								wchar_t					* fileName
							)
{
	return	SaveInstanceTreeW(
					owlInstance,
					(const wchar_t*) fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SaveInstanceTreeS                                       (http://rdf.bg/gkdoc/CP64/SaveInstanceTreeS.html)
//				OwlInstance				owlInstance							IN
//				const void				* callback							IN
//				int64_t					size								IN
//
//				int64_t					returns								OUT
//
//	This function saves the selected instance and its dependancies in a stream.
//
int64_t			DECL STDC	SaveInstanceTreeS(
									OwlInstance				owlInstance,
									const void				* callback,
									int64_t					size
								);

//
//		SaveInstanceTreeA                                       (http://rdf.bg/gkdoc/CP64/SaveInstanceTreeA.html)
//				OwlInstance				owlInstance							IN
//				unsigned char			* content							IN / OUT
//				int64_t					* size								IN / OUT
//
//				int64_t					returns								OUT
//
//	This function saves the selected instance and its dependancies in an array.
//
int64_t			DECL STDC	SaveInstanceTreeA(
									OwlInstance				owlInstance,
									unsigned char			* content,
									int64_t					* size
								);

//
//		SaveInstanceNetwork                                     (http://rdf.bg/gkdoc/CP64/SaveInstanceNetwork.html)
//				OwlInstance				owlInstance							IN
//				bool					includeInverseRelations				IN
//				const char				* fileName							IN
//
//				int64_t					returns								OUT
//
//	This function saves the selected instance and its dependancies on location file name.
//
int64_t			DECL STDC	SaveInstanceNetwork(
									OwlInstance				owlInstance,
									bool					includeInverseRelations,
									const char				* fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SaveInstanceNetwork(
								OwlInstance				owlInstance,
								bool					includeInverseRelations,
								char					* fileName
							)
{
	return	SaveInstanceNetwork(
					owlInstance,
					includeInverseRelations,
					(const char*) fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SaveInstanceNetworkW                                    (http://rdf.bg/gkdoc/CP64/SaveInstanceNetworkW.html)
//				OwlInstance				owlInstance							IN
//				bool					includeInverseRelations				IN
//				const wchar_t			* fileName							IN
//
//				int64_t					returns								OUT
//
//	This function saves the selected instance and its dependancies on location file name.
//
int64_t			DECL STDC	SaveInstanceNetworkW(
									OwlInstance				owlInstance,
									bool					includeInverseRelations,
									const wchar_t			* fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SaveInstanceNetworkW(
								OwlInstance				owlInstance,
								bool					includeInverseRelations,
								wchar_t					* fileName
							)
{
	return	SaveInstanceNetworkW(
					owlInstance,
					includeInverseRelations,
					(const wchar_t*) fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SaveInstanceNetworkS                                    (http://rdf.bg/gkdoc/CP64/SaveInstanceNetworkS.html)
//				OwlInstance				owlInstance							IN
//				bool					includeInverseRelations				IN
//				const void				* callback							IN
//				int64_t					size								IN
//
//				int64_t					returns								OUT
//
//	This function saves the selected instance and its dependancies in a stream.
//
int64_t			DECL STDC	SaveInstanceNetworkS(
									OwlInstance				owlInstance,
									bool					includeInverseRelations,
									const void				* callback,
									int64_t					size
								);

//
//		SaveInstanceNetworkA                                    (http://rdf.bg/gkdoc/CP64/SaveInstanceNetworkA.html)
//				OwlInstance				owlInstance							IN
//				bool					includeInverseRelations				IN
//				unsigned char			* content							IN / OUT
//				int64_t					* size								IN / OUT
//
//				int64_t					returns								OUT
//
//	This function saves the selected instance and its dependancies in an array.
//
int64_t			DECL STDC	SaveInstanceNetworkA(
									OwlInstance				owlInstance,
									bool					includeInverseRelations,
									unsigned char			* content,
									int64_t					* size
								);

//
//		SaveModel                                               (http://rdf.bg/gkdoc/CP64/SaveModel.html)
//				OwlModel				model								IN
//				const char				* fileName							IN
//
//				int64_t					returns								OUT
//
//	This function saves the current model on location file name.
//
int64_t			DECL STDC	SaveModel(
									OwlModel				model,
									const char				* fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SaveModel(
								OwlModel				model,
								char					* fileName
							)
{
	return	SaveModel(
					model,
					(const char*) fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SaveModelW                                              (http://rdf.bg/gkdoc/CP64/SaveModelW.html)
//				OwlModel				model								IN
//				const wchar_t			* fileName							IN
//
//				int64_t					returns								OUT
//
//	This function saves the current model on location file name.
//
int64_t			DECL STDC	SaveModelW(
									OwlModel				model,
									const wchar_t			* fileName
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SaveModelW(
								OwlModel				model,
								wchar_t					* fileName
							)
{
	return	SaveModelW(
					model,
					(const wchar_t*) fileName
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SaveModelS                                              (http://rdf.bg/gkdoc/CP64/SaveModelS.html)
//				OwlModel				model								IN
//				const void				* callback							IN
//				int64_t					size								IN
//
//				int64_t					returns								OUT
//
//	This function saves the current model in a stream.
//
int64_t			DECL STDC	SaveModelS(
									OwlModel				model,
									const void				* callback,
									int64_t					size
								);

//
//		SaveModelA                                              (http://rdf.bg/gkdoc/CP64/SaveModelA.html)
//				OwlModel				model								IN
//				unsigned char			* content							IN / OUT
//				int64_t					* size								IN / OUT
//
//				int64_t					returns								OUT
//
//	This function saves the current model in an array.
//
int64_t			DECL STDC	SaveModelA(
									OwlModel				model,
									unsigned char			* content,
									int64_t					* size
								);

//
//		SetOverrideFileIO                                       (http://rdf.bg/gkdoc/CP64/SetOverrideFileIO.html)
//				OwlModel				model								IN
//				uint64_t				setting								IN
//				uint64_t				mask								IN
//
//				void					returns
//
//	This function overrides the type of file saved / exported independent of the extension given.
//	By default the extension of the file name will define the type saved / exported:
//		.rdf => generated RDF serialized content
//		.ttl => generated TTL serialized content
//		.bin => generated BIN/X serialized content
//
//	Available formats
//		RDF
//		TTL
//		BIN/L - readible but large BIN format
//		BIN/S - Optimized Binary, only running within given revision 
//		BIN/X - Optimized Binary, running in all revisions supporting BIN/X
//
//	Force file type (overrides extension), works only on save (open selects correct type automatically)
//		bit0	bit1	bit2
//		  0		  0		  0		[default] unset forced file type
//		  0		  0		  1		RESERVED
//		  0		  1		  0		TTL
//		  0		  1		  1		RDF
//		  1		  0		  0		BIN/X
//		  1		  0		  1		BIN/S
//		  1		  1		  0		RESERVED
//		  1		  1		  1		BIN/L
//
//	Force exporting as Base64
//		bit4
//		  0		do not use Base64
//		  1		use Base64 (only works for BIN/S and BIN/X), on other formats no effect
//
void			DECL STDC	SetOverrideFileIO(
									OwlModel				model,
									uint64_t				setting,
									uint64_t				mask
								);

//
//		GetOverrideFileIO                                       (http://rdf.bg/gkdoc/CP64/GetOverrideFileIO.html)
//				OwlModel				model								IN
//				uint64_t				mask								IN
//
//				uint64_t				returns								OUT
//
//	This function get the current overrides for type of file saved / exported independent of the extension given.
//	By default the extension of the file name will define the type saved / exported:
//		.rdf => generated RDF serialized content
//		.ttl => generated TTL serialized content
//		.bin => generated BIN/X serialized content
//
//	Available formats
//		RDF
//		TTL
//		BIN/L - readible but large BIN format
//		BIN/S - Optimized Binary, only running within given revision 
//		BIN/X - Optimized Binary, running in all revisions supporting BIN/X
//
//	Force file type (overrides extension), works only on save (open selects correct type automatically)
//		bit0	bit1	bit2
//		  0		  0		  0		[default] unset forced file type
//		  0		  0		  1		RESERVED
//		  0		  1		  0		TTL
//		  0		  1		  1		RDF
//		  1		  0		  0		BIN/X
//		  1		  0		  1		BIN/S
//		  1		  1		  0		RESERVED
//		  1		  1		  1		BIN/L
//
//	Force exporting as Base64
//		bit4
//		  0		do not use Base64
//		  1		use Base64 (only works for BIN/S and BIN/X), on other formats no effect
//
uint64_t		DECL STDC	GetOverrideFileIO(
									OwlModel				model,
									uint64_t				mask
								);

//
//		CopyInstanceTree                                        (http://rdf.bg/gkdoc/CP64/CopyInstanceTree.html)
//				OwlInstance				owlInstance							IN
//				OwlModel				targetModel							IN
//
//				int64_t					returns								OUT
//
//	This function copies the instance tree towards a new model.
//	In case model is empty a new model will be created (the handle to this new model can be retrieved through
//	the call GetModel() based on the return value of this call).
//	The model can be any opem model, it can be zero (a new model will be created on-the-fly) and it can be
//	the same model as the model owlInstance is defined within, in this case just a perfect copy of the
//	original instance tree.
//
//	The return value is the handle to the copied owlInstance in the model of choice.
//
int64_t			DECL STDC	CopyInstanceTree(
									OwlInstance				owlInstance,
									OwlModel				targetModel
								);

//
//		CopyInstanceNetwork                                     (http://rdf.bg/gkdoc/CP64/CopyInstanceNetwork.html)
//				OwlInstance				owlInstance							IN
//				bool					includeInverseRelations				IN
//				OwlModel				targetModel							IN
//
//				OwlInstance				returns								OUT
//
//	This function copies the instance network towards a new model.
//	An instance network is different from an instance tree in that it can contain 'loops', the performance
//	from this call will be slower in case the tree / network is sparse.
//	In case model is empty a new model will be created (the handle to this new model can be retrieved through
//	the call GetModel() based on the return value of this call).
//	The model can be any open model, it can be zero (a new model will be created on-the-fly) and it can be
//	the same model as the model owlInstance is defined within, in this case just a perfect copy of the
//	original instance tree.
//
//	In case it is known we are talking about a tree (i.e. no internal loops) and inverse relations can be ignored
//	the call CopyInstanceTree is a better choice concerning performance.
//
//	The return value is the handle to the copied owlInstance in the model of choice.
//
OwlInstance		DECL STDC	CopyInstanceNetwork(
									OwlInstance				owlInstance,
									bool					includeInverseRelations,
									OwlModel				targetModel
								);

//
//		EncodeBase64                                            (http://rdf.bg/gkdoc/CP64/EncodeBase64.html)
//				char					* output							IN / OUT
//				const unsigned char		* input								IN
//				int64_t					size								IN
//				bool					terminator							IN
//
//				int64_t					returns								OUT
//
//	Function to encode any data input array into a BASE64 string.
//
//	The output string has to be allocated by the host. The return value defines the length of the string size in bytes.
//
//	Terminator adds a 0 element to the end of the BASE64 generated string, it will NOT increase the length.
//
//	If output is nullptr the length will be calculated but the string itself will not be generated.
//
int64_t			DECL STDC	EncodeBase64(
									char					* output,
									const unsigned char		* input,
									int64_t					size,
									bool					terminator
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	EncodeBase64(
								char					* output,
								const unsigned char		* input,
								int64_t					size
							)
{
	return	EncodeBase64(
					output,
					input,
					size,
					false								//	terminator
				);
}

//
//
static	inline	char	* EncodeBase64(
								const unsigned char		* input,
								int64_t					size
							)
{
	char	* output = new char[(int_t) EncodeBase64(nullptr, input, size) / sizeof(char) + 1];

	EncodeBase64(
			output,
			input,
			size,
			true								//	terminator
		);

	return  output;
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		EncodeBase64W                                           (http://rdf.bg/gkdoc/CP64/EncodeBase64W.html)
//				wchar_t					* output							IN / OUT
//				const unsigned char		* input								IN
//				int64_t					size								IN
//				bool					terminator							IN
//
//				int64_t					returns								OUT
//
//	Function to encode any data input array into a BASE64 string.
//
//	The output string has to be allocated by the host. The return value defines the length of the string size in bytes.
//
//	Terminator adds a 0 element to the end of the BASE64 generated string, it will NOT increase the length.
//
//	If output is nullptr the length will be calculated but the string itself will not be generated.
//
int64_t			DECL STDC	EncodeBase64W(
									wchar_t					* output,
									const unsigned char		* input,
									int64_t					size,
									bool					terminator
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	EncodeBase64W(
								wchar_t					* output,
								const unsigned char		* input,
								int64_t					size
							)
{
	return	EncodeBase64W(
					output,
					input,
					size,
					false								//	terminator
				);
}

//
//
static	inline	wchar_t	* EncodeBase64W(
								const unsigned char		* input,
								int64_t					size
							)
{
	wchar_t * output = new wchar_t[(int_t) EncodeBase64(nullptr, input, size) / sizeof(wchar_t) + 1];

	EncodeBase64W(
			output,
			input,
			size,
			true								//	terminator
		);

	return  output;
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		DecodeBase64                                            (http://rdf.bg/gkdoc/CP64/DecodeBase64.html)
//				unsigned char			* output							IN / OUT
//				const char				* input								IN
//				int64_t					size								IN
//
//				int64_t					returns								OUT
//
//	Function to decode a BASE64 string into any data output array.
//
//	The BASE64 string is measured by the (non-zero) size given or by the terminator.
//
//	If output is nullptr the length will be calculated but the string itself will not be generated.
//
int64_t			DECL STDC	DecodeBase64(
									unsigned char			* output,
									const char				* input,
									int64_t					size
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	DecodeBase64(
								unsigned char			* output,
								char					* input,
								int64_t					size
							)
{
	return	DecodeBase64(
					output,
					(const char*) input,
					size
				);
}

//
//
static	inline	int64_t	DecodeBase64(
								unsigned char			* output,
								const char				* input
							)
{
	return	DecodeBase64(
					output,
					input,
					0									//	size
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		DecodeBase64W                                           (http://rdf.bg/gkdoc/CP64/DecodeBase64W.html)
//				unsigned char			* output							IN / OUT
//				const wchar_t			* input								IN
//				int64_t					size								IN
//
//				int64_t					returns								OUT
//
//	Function to decode a BASE64 string into any data output array.
//
//	The BASE64 string is measured by the (non-zero) size given or by the terminator.
//
//	If output is nullptr the length will be calculated but the string itself will not be generated.
//
int64_t			DECL STDC	DecodeBase64W(
									unsigned char			* output,
									const wchar_t			* input,
									int64_t					size
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	DecodeBase64W(
								unsigned char			* output,
								wchar_t					* input,
								int64_t					size
							)
{
	return	DecodeBase64W(
					output,
					(const wchar_t*) input,
					size
				);
}

//
//
static	inline	int64_t	DecodeBase64W(
								unsigned char			* output,
								const wchar_t			* input
							)
{
	return	DecodeBase64W(
					output,
					input,
					0									//	size
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		CopyModel                                               (http://rdf.bg/gkdoc/CP64/CopyModel.html)
//				OwlModel				sourceModel							IN
//				OwlModel				targetModel							IN
//				int64_t					* values							IN / OUT
//				int64_t					card								IN
//
//				OwlModel				returns								OUT
//
//	This function copies the complete structure of a model towards another or new model.
//	In case the targetModel is empty a new model will be created.
//	The owlInstance array (values) and it's cardinality (card) can be empty, in case they are
//	non-empty the values are expected to contain owlInstance handles referencing in the source model
//	after a successful copy the values will be adjusted into values referencing the copied owl instances
//	in the new model. the list of values does not have to be complete or even unique and can even be empty.
//
//	sourceModel is not allowed to be empty, targetModel however can be empty or even equal to the source model.
//
//	The return value is the targetModel or in case this value was empty the newly created model.
//
OwlModel		DECL STDC	CopyModel(
									OwlModel				sourceModel,
									OwlModel				targetModel,
									int64_t					* values,
									int64_t					card
								);

//
//		CloseModel                                              (http://rdf.bg/gkdoc/CP64/CloseModel.html)
//				OwlModel				model								IN
//
//				int64_t					returns								OUT
//
//	This function closes the model. After this call none of the instances and classes within the model
//	can be used anymore, also garbage collection is not allowed anymore, in default compilation the
//	model itself will be known in the kernel, however known to be disabled. Calls containing the model
//	reference will be protected from crashing when called.
//
int64_t			DECL STDC	CloseModel(
									OwlModel				model
								);

//
//		IsModel                                                 (http://rdf.bg/gkdoc/CP64/IsModel.html)
//				RdfsResource			rdfsResource						IN
//
//				OwlModel				returns								OUT
//
//	Returns OwlModel if the argument rdfsResource is an actual active model. It returns 0 in all other cases,
//	i.e. this could mean the model is already closed or the session is closed.
//	It could also mean it represents a handle to another resource, for example a property, instance or class.
//
OwlModel		DECL STDC	IsModel(
									RdfsResource			rdfsResource
								);

//
//  Design Tree Classes API Calls
//

//
//		CreateClass                                             (http://rdf.bg/gkdoc/CP64/CreateClass.html)
//				OwlModel				model								IN
//				const char				* name								IN
//
//				OwlClass				returns								OUT
//
//	Returns a handle to an on the fly created class, however when
//	a class with this name already exists the handle of existing class will be returned.
//
//	The following reasons will cause a return value of 0:
//		-	when the name is already used for an instance or property;
//		-	if the model input is zero or not a model handle.
//
//	Giving the class a name is optional, if a name is not given it will recieve an automatically generated name,
//	it's automatically generated name can change between sessions.
//
OwlClass		DECL STDC	CreateClass(
									OwlModel				model,
									const char				* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlClass	CreateClass(
									OwlModel				model,
									char					* name
								)
{
	return	CreateClass(
					model,
					(const char*) name
				);
}

//
//
static	inline	OwlClass	CreateClass(
									OwlModel				model
								)
{
	return	CreateClass(
					model,
					(const char*) nullptr				//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		CreateClassW                                            (http://rdf.bg/gkdoc/CP64/CreateClassW.html)
//				OwlModel				model								IN
//				const wchar_t			* name								IN
//
//				OwlClass				returns								OUT
//
//	Returns a handle to an on the fly created class, however when
//	a class with this name already exists the handle of existing class will be returned.
//
//	The following reasons will cause a return value of 0:
//		-	when the name is already used for an instance or property;
//		-	if the model input is zero or not a model handle.
//
//	Giving the class a name is optional, if a name is not given it will recieve an automatically generated name,
//	it's automatically generated name can change between sessions.
//
OwlClass		DECL STDC	CreateClassW(
									OwlModel				model,
									const wchar_t			* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlClass	CreateClassW(
									OwlModel				model,
									wchar_t					* name
								)
{
	return	CreateClassW(
					model,
					(const wchar_t*) name
				);
}

//
//
static	inline	OwlClass	CreateClassW(
									OwlModel				model
								)
{
	return	CreateClassW(
					model,
					(const wchar_t*) nullptr			//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetClassByName                                          (http://rdf.bg/gkdoc/CP64/GetClassByName.html)
//				OwlModel				model								IN
//				const char				* name								IN
//
//				OwlClass				returns								OUT
//
//	Returns a handle to the class as stored inside.
//	When there is no class with such a name the return value is 0 (note that GetModellingStyle(..) can change this behavior).
//
OwlClass		DECL STDC	GetClassByName(
									OwlModel				model,
									const char				* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlClass	GetClassByName(
									OwlModel				model,
									char					* name
								)
{
	return	GetClassByName(
					model,
					(const char*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetClassByNameW                                         (http://rdf.bg/gkdoc/CP64/GetClassByNameW.html)
//				OwlModel				model								IN
//				const wchar_t			* name								IN
//
//				OwlClass				returns								OUT
//
//	Returns a handle to the class as stored inside.
//	When there is no class with such a name the return value is 0 (note that GetModellingStyle(..) can change this behavior).
//
OwlClass		DECL STDC	GetClassByNameW(
									OwlModel				model,
									const wchar_t			* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlClass	GetClassByNameW(
									OwlModel				model,
									wchar_t					* name
								)
{
	return	GetClassByNameW(
					model,
					(const wchar_t*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetClassesByIterator                                    (http://rdf.bg/gkdoc/CP64/GetClassesByIterator.html)
//				OwlModel				model								IN
//				OwlClass				owlClass							IN
//
//				OwlClass				returns								OUT
//
//	Returns a handle to an class.
//	If input class is zero, the handle will point to the first relevant class.
//	If all classes are past (or no relevant classes are found), the function will return 0.
//
OwlClass		DECL STDC	GetClassesByIterator(
									OwlModel				model,
									OwlClass				owlClass
								);

//
//		SetClassParent                                          (http://rdf.bg/gkdoc/CP64/SetClassParent.html)
//				OwlClass				owlClass							IN
//				OwlClass				parentOwlClass						IN
//				int64_t					setting								IN
//
//				int64_t					returns								OUT
//
//	Defines (set/unset) the parent class of a given class. Multiple-inheritence is supported and behavior
//	of parent classes is also inherited as well as cardinality restrictions on datatype properties and
//	object properties (relations).
//
//	When set: it adds parentOwlClass as immediate parent of owlClass if and only if 
//	parentOwlClass is not ancestor of owlClass and owlClass is not ancestor of parentOwlClass.
//	Returns the same value as IsClassAncestor after the call.
//
//	When unset: it removes parentOwlClass from immediate parents and returns 1, 
//	or retunrs 0 if parentOwlClass is not immediate parent
//
int64_t			DECL STDC	SetClassParent(
									OwlClass				owlClass,
									OwlClass				parentOwlClass,
									int64_t					setting
								);

//
//		SetClassParentEx                                        (http://rdf.bg/gkdoc/CP64/SetClassParentEx.html)
//				OwlModel				model								IN
//				OwlClass				owlClass							IN
//				OwlClass				parentOwlClass						IN
//				int64_t					setting								IN
//
//				int64_t					returns								OUT
//
//	Defines (set/unset) the parent class of a given class. Multiple-inheritence is supported and behavior
//	of parent classes is also inherited as well as cardinality restrictions on datatype properties and
//	object properties (relations).
//
//	When set: it adds parentOwlClass as immediate parent of owlClass if and only if 
//	parentOwlClass is not ancestor of owlClass and owlClass is not ancestor of parentOwlClass.
//	Returns the same value as IsClassAncestor after the call.
//
//	When unset: it removes parentOwlClass from immediate parents and returns 1, 
//	or retunrs 0 if parentOwlClass is not immediate parent
//
//	This call has the same behavior as SetClassParent, however needs to be
//	used in case classes are exchanged as a successive series of integers.
//
int64_t			DECL STDC	SetClassParentEx(
									OwlModel				model,
									OwlClass				owlClass,
									OwlClass				parentOwlClass,
									int64_t					setting
								);

//
//		IsClassAncestor                                         (http://rdf.bg/gkdoc/CP64/IsClassAncestor.html)
//				OwlClass				owlClass							IN
//				OwlClass				ancestorOwlClass					IN
//
//				int64_t					returns								OUT
//
//	Checks if the class has given ancestor
//	Returns 0 if not or minimal generation number (1 for direct parent)
//
int64_t			DECL STDC	IsClassAncestor(
									OwlClass				owlClass,
									OwlClass				ancestorOwlClass
								);

//
//		GetClassParentsByIterator                               (http://rdf.bg/gkdoc/CP64/GetClassParentsByIterator.html)
//				OwlClass				owlClass							IN
//				OwlClass				parentOwlClass						IN
//
//				OwlClass				returns								OUT
//
//	Returns the next parent of the class.
//	If input parent is zero, the handle will point to the first relevant parent.
//	If all parent are past (or no relevant parent are found), the function will return 0.
//
OwlClass		DECL STDC	GetClassParentsByIterator(
									OwlClass				owlClass,
									OwlClass				parentOwlClass
								);

//
//		SetNameOfClass                                          (http://rdf.bg/gkdoc/CP64/SetNameOfClass.html)
//				OwlClass				owlClass							IN
//				const char				* name								IN
//
//				int64_t					returns								OUT
//
//	Sets or updates the name of the class, it returns 0 on success.
//
//	Error return codes:
//		0	successful
//		1	argument owlClass is incorrect (not a proper handle to an active class)
//		2	argument name is incorrect (nullptr or zero length name)
//		3	the name of owlClass is locked
//		4	name is already used by another class
//		5	name is already used by a property
//		6	name is already used by an instance
//		7	undefined error
//
int64_t			DECL STDC	SetNameOfClass(
									OwlClass				owlClass,
									const char				* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetNameOfClass(
								OwlClass				owlClass,
								char					* name
							)
{
	return	SetNameOfClass(
					owlClass,
					(const char*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetNameOfClassW                                         (http://rdf.bg/gkdoc/CP64/SetNameOfClassW.html)
//				OwlClass				owlClass							IN
//				const wchar_t			* name								IN
//
//				int64_t					returns								OUT
//
//	Sets or updates the name of the class, it returns 0 on success.
//
//	Error return codes:
//		0	successful
//		1	argument owlClass is incorrect (not a proper handle to an active class)
//		2	argument name is incorrect (nullptr or zero length name)
//		3	the name of owlClass is locked
//		4	name is already used by another class
//		5	name is already used by a property
//		6	name is already used by an instance
//		7	undefined error
//
int64_t			DECL STDC	SetNameOfClassW(
									OwlClass				owlClass,
									const wchar_t			* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetNameOfClassW(
								OwlClass				owlClass,
								wchar_t					* name
							)
{
	return	SetNameOfClassW(
					owlClass,
					(const wchar_t*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetNameOfClassEx                                        (http://rdf.bg/gkdoc/CP64/SetNameOfClassEx.html)
//				OwlModel				model								IN
//				OwlClass				owlClass							IN
//				const char				* name								IN
//
//				int64_t					returns								OUT
//
//	Sets or updates the name of the class, it returns 0 on success.
//
//	Error return codes:
//		0	successful
//		1	argument model or owlClass is incorrect (not a proper handle to an active class)
//		2	argument name is incorrect (nullptr or zero length name)
//		3	the name of owlClass is locked
//		4	name is already used by another class
//		5	name is already used by a property
//		6	name is already used by an instance
//		7	undefined error
//
//	This call has the same behavior as SetNameOfClass, however needs to be
//	used in case classes are exchanged as a successive series of integers.
//
int64_t			DECL STDC	SetNameOfClassEx(
									OwlModel				model,
									OwlClass				owlClass,
									const char				* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetNameOfClassEx(
								OwlModel				model,
								OwlClass				owlClass,
								char					* name
							)
{
	return	SetNameOfClassEx(
					model,
					owlClass,
					(const char*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetNameOfClassWEx                                       (http://rdf.bg/gkdoc/CP64/SetNameOfClassWEx.html)
//				OwlModel				model								IN
//				OwlClass				owlClass							IN
//				const wchar_t			* name								IN
//
//				int64_t					returns								OUT
//
//	Sets or updates the name of the class, it returns 0 on success.
//
//	Error return codes:
//		0	successful
//		1	argument model or owlClass is incorrect (not a proper handle to an active class)
//		2	argument name is incorrect (nullptr or zero length name)
//		3	the name of owlClass is locked
//		4	name is already used by another class
//		5	name is already used by a property
//		6	name is already used by an instance
//		7	undefined error
//
//	This call has the same behavior as SetNameOfClassW, however needs to be
//	used in case classes are exchanged as a successive series of integers.
//
int64_t			DECL STDC	SetNameOfClassWEx(
									OwlModel				model,
									OwlClass				owlClass,
									const wchar_t			* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetNameOfClassWEx(
								OwlModel				model,
								OwlClass				owlClass,
								wchar_t					* name
							)
{
	return	SetNameOfClassWEx(
					model,
					owlClass,
					(const wchar_t*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetNameOfClass                                          (http://rdf.bg/gkdoc/CP64/GetNameOfClass.html)
//				OwlClass				owlClass							IN
//				const char				** name								IN / OUT
//
//				const char				* returns							OUT
//
//	Returns the name of the class, if the class does not exist it returns nullptr.
//
const char		DECL * STDC	GetNameOfClass(
									OwlClass				owlClass,
									const char				** name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const char	* GetNameOfClass(
									OwlClass				owlClass,
									char					** name
								)
{
	return	GetNameOfClass(
					owlClass,
					(const char**) name
				);
}

//
//
static	inline	const char	* GetNameOfClass(
									OwlClass				owlClass
								)
{
	return	GetNameOfClass(
					owlClass,
					(const char**) nullptr				//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetNameOfClassW                                         (http://rdf.bg/gkdoc/CP64/GetNameOfClassW.html)
//				OwlClass				owlClass							IN
//				const wchar_t			** name								IN / OUT
//
//				const wchar_t			* returns							OUT
//
//	Returns the name of the class, if the class does not exist it returns nullptr.
//
const wchar_t	DECL * STDC	GetNameOfClassW(
									OwlClass				owlClass,
									const wchar_t			** name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const wchar_t	* GetNameOfClassW(
										OwlClass				owlClass,
										wchar_t					** name
									)
{
	return	GetNameOfClassW(
					owlClass,
					(const wchar_t**) name
				);
}

//
//
static	inline	const wchar_t	* GetNameOfClassW(
										OwlClass				owlClass
									)
{
	return	GetNameOfClassW(
					owlClass,
					(const wchar_t**) nullptr			//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetNameOfClassEx                                        (http://rdf.bg/gkdoc/CP64/GetNameOfClassEx.html)
//				OwlModel				model								IN
//				OwlClass				owlClass							IN
//				const char				** name								IN / OUT
//
//				const char				* returns							OUT
//
//	Returns the name of the class, if the class does not exist it returns nullptr.
//
//	This call has the same behavior as GetNameOfClass, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
const char		DECL * STDC	GetNameOfClassEx(
									OwlModel				model,
									OwlClass				owlClass,
									const char				** name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const char	* GetNameOfClassEx(
									OwlModel				model,
									OwlClass				owlClass,
									char					** name
								)
{
	return	GetNameOfClassEx(
					model,
					owlClass,
					(const char**) name
				);
}

//
//
static	inline	const char	* GetNameOfClassEx(
									OwlModel				model,
									OwlClass				owlClass
								)
{
	return	GetNameOfClassEx(
					model,
					owlClass,
					(const char**) nullptr				//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetNameOfClassWEx                                       (http://rdf.bg/gkdoc/CP64/GetNameOfClassWEx.html)
//				OwlModel				model								IN
//				OwlClass				owlClass							IN
//				const wchar_t			** name								IN / OUT
//
//				const wchar_t			* returns							OUT
//
//	Returns the name of the class, if the class does not exist it returns nullptr.
//
//	This call has the same behavior as GetNameOfClassW, however needs to be
//	used in case classes are exchanged as a successive series of integers.
//
const wchar_t	DECL * STDC	GetNameOfClassWEx(
									OwlModel				model,
									OwlClass				owlClass,
									const wchar_t			** name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const wchar_t	* GetNameOfClassWEx(
										OwlModel				model,
										OwlClass				owlClass,
										wchar_t					** name
									)
{
	return	GetNameOfClassWEx(
					model,
					owlClass,
					(const wchar_t**) name
				);
}

//
//
static	inline	const wchar_t	* GetNameOfClassWEx(
										OwlModel				model,
										OwlClass				owlClass
									)
{
	return	GetNameOfClassWEx(
					model,
					owlClass,
					(const wchar_t**) nullptr			//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetClassPropertyByIterator                              (http://rdf.bg/gkdoc/CP64/GetClassPropertyByIterator.html)
//				OwlClass				owlClass							IN
//				RdfProperty				rdfProperty							IN
//				int64_t					* minCard							IN / OUT
//				int64_t					* maxCard							IN / OUT
//
//				RdfProperty				returns								OUT
//
//	Returns a handle to a property.
//	If input property is zero, the handle will point to the first property having cardinality restriction to the class.
//	else it will point to next propety with known restriction
//	If all properties are past (or no relevant properties are found), the function will return 0.
//	minCard and maxCard will contain restrictions for returned property
//
//	Note: this function does not return inherited restrictions.
//
RdfProperty		DECL STDC	GetClassPropertyByIterator(
									OwlClass				owlClass,
									RdfProperty				rdfProperty,
									int64_t					* minCard,
									int64_t					* maxCard
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	RdfProperty	GetClassPropertyByIterator(
									OwlClass				owlClass,
									RdfProperty				rdfProperty
								)
{
	return	GetClassPropertyByIterator(
					owlClass,
					rdfProperty,
					nullptr,							//	minCard
					nullptr								//	maxCard
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetClassPropertyByIteratorEx                            (http://rdf.bg/gkdoc/CP64/GetClassPropertyByIteratorEx.html)
//				OwlModel				model								IN
//				OwlClass				owlClass							IN
//				RdfProperty				rdfProperty							IN
//				int64_t					* minCard							IN / OUT
//				int64_t					* maxCard							IN / OUT
//
//				RdfProperty				returns								OUT
//
//	Returns a handle to a property.
//	If input property is zero, the handle will point to the first property having cardinality restriction to the class.
//	else it will point to next propety with known restriction
//	If all properties are past (or no relevant properties are found), the function will return 0.
//	minCard and maxCard will contain restrictions for returned property
//
//	Note: this function does not return inherited restrictions.
//
//	This call has the same behavior as GetClassPropertyByIterator, however needs to be
//	used in case classes and / or properties are exchanged as a successive series of integers.
//
RdfProperty		DECL STDC	GetClassPropertyByIteratorEx(
									OwlModel				model,
									OwlClass				owlClass,
									RdfProperty				rdfProperty,
									int64_t					* minCard,
									int64_t					* maxCard
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	RdfProperty	GetClassPropertyByIteratorEx(
									OwlModel				model,
									OwlClass				owlClass,
									RdfProperty				rdfProperty
								)
{
	return	GetClassPropertyByIteratorEx(
					model,
					owlClass,
					rdfProperty,
					nullptr,							//	minCard
					nullptr								//	maxCard
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetClassPropertyCardinalityRestriction                  (http://rdf.bg/gkdoc/CP64/SetClassPropertyCardinalityRestriction.html)
//				OwlClass				owlClass							IN
//				RdfProperty				rdfProperty							IN
//				int64_t					minCard								IN
//				int64_t					maxCard								IN
//
//				void					returns
//
//	This function sets the minCard and maxCard of a certain property in the context of a class.
//	The cardinality of a property in an instance has to be between minCard and maxCard (as well 
//	as within the cardinality restrictions as given by the property in context of any of its
//	(indirect) parent classes).
//	If undefined minCard and/or maxCard will be of value -1, this means
//	for minCard that it is 0 and for maxCard it means infinity.
//
void			DECL STDC	SetClassPropertyCardinalityRestriction(
									OwlClass				owlClass,
									RdfProperty				rdfProperty,
									int64_t					minCard,
									int64_t					maxCard
								);

//
//		SetClassPropertyCardinalityRestrictionEx                (http://rdf.bg/gkdoc/CP64/SetClassPropertyCardinalityRestrictionEx.html)
//				OwlModel				model								IN
//				OwlClass				owlClass							IN
//				RdfProperty				rdfProperty							IN
//				int64_t					minCard								IN
//				int64_t					maxCard								IN
//
//				void					returns
//
//	This function sets the minCard and maxCard of a certain property in the context of a class.
//	The cardinality of a property in an instance has to be between minCard and maxCard (as well 
//	as within the cardinality restrictions as given by the property in context of any of its
//	(indirect) parent classes).
//	If undefined minCard and/or maxCard will be of value -1, this means
//	for minCard that it is 0 and for maxCard it means infinity.
//
//	This call has the same behavior as SetClassPropertyCardinalityRestriction, however needs to be
//	used in case classes or properties are exchanged as a successive series of integers.
//
void			DECL STDC	SetClassPropertyCardinalityRestrictionEx(
									OwlModel				model,
									OwlClass				owlClass,
									RdfProperty				rdfProperty,
									int64_t					minCard,
									int64_t					maxCard
								);

//
//		GetClassPropertyCardinalityRestriction                  (http://rdf.bg/gkdoc/CP64/GetClassPropertyCardinalityRestriction.html)
//				OwlClass				owlClass							IN
//				RdfProperty				rdfProperty							IN
//				int64_t					* minCard							IN / OUT
//				int64_t					* maxCard							IN / OUT
//
//				void					returns
//
//	This function returns the minCard and maxCard of a certain
//	property in the context of a class. The cardinality of a property in 
//	an instance has to be between minCard and maxCard (as well as within the cardinality restrictions
//	as given by the property in context of any of its (indirect) parent classes).
//	If undefined minCard and/or maxCard will be of value -1, this means
//	for minCard that it is 0 and for maxCard it means infinity.
//
//	Note: this function does not return inherited restrictions. The example shows how to retrieve
//	this knowledge, as it is derived knowledge the call that used to be available is removed.
//
void			DECL STDC	GetClassPropertyCardinalityRestriction(
									OwlClass				owlClass,
									RdfProperty				rdfProperty,
									int64_t					* minCard,
									int64_t					* maxCard
								);

//
//		GetClassPropertyCardinalityRestrictionEx                (http://rdf.bg/gkdoc/CP64/GetClassPropertyCardinalityRestrictionEx.html)
//				OwlModel				model								IN
//				OwlClass				owlClass							IN
//				RdfProperty				rdfProperty							IN
//				int64_t					* minCard							IN / OUT
//				int64_t					* maxCard							IN / OUT
//
//				void					returns
//
//	This function returns the minCard and maxCard of a certain
//	property in the context of a class. The cardinality of a property in 
//	an instance has to be between minCard and maxCard (as well as within the cardinality restrictions
//	as given by the property in context of any of its (indirect) parent classes).
//	If undefined minCard and/or maxCard will be of value -1, this means
//	for minCard that it is 0 and for maxCard it means infinity.
//
//	This call has the same behavior as GetClassPropertyCardinalityRestriction, however needs to be
//	used in case classes or properties are exchanged as a successive series of integers.
//
//	Note: this function does not return inherited restrictions. The example shows how to retrieve
//	this knowledge, as it is derived knowledge the call that used to be available is removed.
//
void			DECL STDC	GetClassPropertyCardinalityRestrictionEx(
									OwlModel				model,
									OwlClass				owlClass,
									RdfProperty				rdfProperty,
									int64_t					* minCard,
									int64_t					* maxCard
								);

//
//		GetClassPropertyAggregatedCardinalityRestriction        (http://rdf.bg/gkdoc/CP64/GetClassPropertyAggregatedCardinalityRestriction.html)
//				OwlClass				owlClass							IN
//				RdfProperty				rdfProperty							IN
//				int64_t					* minCard							IN / OUT
//				int64_t					* maxCard							IN / OUT
//
//				void					returns
//
//	This function returns the minCard and maxCard of a certain
//	property in the context of a class. This function does return inherited restrictions.
//	If undefined minCard and/or maxCard will be of value -1, this means
//	for minCard that it is 0 and for maxCard it means infinity.
//
void			DECL STDC	GetClassPropertyAggregatedCardinalityRestriction(
									OwlClass				owlClass,
									RdfProperty				rdfProperty,
									int64_t					* minCard,
									int64_t					* maxCard
								);

//
//		GetClassPropertyAggregatedCardinalityRestrictionEx      (http://rdf.bg/gkdoc/CP64/GetClassPropertyAggregatedCardinalityRestrictionEx.html)
//				OwlModel				model								IN
//				OwlClass				owlClass							IN
//				RdfProperty				rdfProperty							IN
//				int64_t					* minCard							IN / OUT
//				int64_t					* maxCard							IN / OUT
//
//				void					returns
//
//	This function returns the minCard and maxCard of a certain
//	property in the context of a class. This function does return inherited restrictions.
//	If undefined minCard and/or maxCard will be of value -1, this means
//	for minCard that it is 0 and for maxCard it means infinity.
//
//	This call has the same behavior as GetClassPropertyAggregatedCardinalityRestriction, however needs to be
//	used in case classes or properties are exchanged as a successive series of integers.
//
void			DECL STDC	GetClassPropertyAggregatedCardinalityRestrictionEx(
									OwlModel				model,
									OwlClass				owlClass,
									RdfProperty				rdfProperty,
									int64_t					* minCard,
									int64_t					* maxCard
								);

//
//		GetGeometryClass                                        (http://rdf.bg/gkdoc/CP64/GetGeometryClass.html)
//				OwlClass				owlClass							IN
//
//				OwlClass				returns								OUT
//
//	Returns non-zero if the owlClass is a geometry type. This call will return the input class
//	for all classes initially available. It will return as well non-for all classes created by the
//	user or loaded / imported through a model that (indirectly) inherit one of the
//	original classes available. in this case it returns the original available class
//	it inherits the behavior from.
//
OwlClass		DECL STDC	GetGeometryClass(
									OwlClass				owlClass
								);

//
//		GetGeometryClassEx                                      (http://rdf.bg/gkdoc/CP64/GetGeometryClassEx.html)
//				OwlModel				model								IN
//				OwlClass				owlClass							IN
//
//				OwlClass				returns								OUT
//
//	Returns non-zero if the owlClass is a geometry type. This call will return the input class
//	for all classes initially available. It will return as well non-for all classes created by the
//	user or loaded / imported through a model that (indirectly) inherit one of the
//	original classes available. in this case it returns the original available class
//	it inherits the behavior from.
//
//	This call has the same behavior as GetGeometryClass, however needs to be
//	used in case classes are exchanged as a successive series of integers.
//
OwlClass		DECL STDC	GetGeometryClassEx(
									OwlModel				model,
									OwlClass				owlClass
								);

//
//		IsClass                                                 (http://rdf.bg/gkdoc/CP64/IsClass.html)
//				RdfsResource			rdfsResource						IN
//
//				OwlClass				returns								OUT
//
//	Returns OwlClass if the argument rdfsResource is an actual active class in an active model. It returns 0 in all other cases,
//	i.e. this could mean the model is already closed, the class is inactive or removed or the session is closed.
//	It could also mean it represents a handle to another resource, for example a property, instance or model.
//
OwlClass		DECL STDC	IsClass(
									RdfsResource			rdfsResource
								);

//
//  Design Tree Properties API Calls
//

//
//		CreateProperty                                          (http://rdf.bg/gkdoc/CP64/CreateProperty.html)
//				OwlModel				model								IN
//				int64_t					rdfPropertyType						IN
//				const char				* name								IN
//
//				RdfProperty				returns								OUT
//
//	Returns a handle to an on the fly created property, however when
//	a property with this name already exists the handle of existing property will be returned.
//
//	The following reasons will cause a return value of 0:
//		-	when the name is already used for a class or instance;
//		-	if the model input is zero or not a model handle.
//
//	Giving the property a name is optional, if a name is not given it will recieve an automatically generated name,
//	it's automatically generated name can change between sessions.
//
RdfProperty		DECL STDC	CreateProperty(
									OwlModel				model,
									int64_t					rdfPropertyType,
									const char				* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	RdfProperty	CreateProperty(
									OwlModel				model,
									int64_t					rdfPropertyType,
									char					* name
								)
{
	return	CreateProperty(
					model,
					rdfPropertyType,
					(const char*) name
				);
}

//
//
static	inline	RdfProperty	CreateProperty(
									OwlModel				model,
									int64_t					rdfPropertyType
								)
{
	return	CreateProperty(
					model,
					rdfPropertyType,
					(const char*) nullptr				//	name
				);
}

//
//
static	inline	RdfProperty	CreateProperty(
									OwlModel				model
								)
{
	return	CreateProperty(
					model,
					0,									//	rdfPropertyType
					(const char*) nullptr				//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		CreatePropertyW                                         (http://rdf.bg/gkdoc/CP64/CreatePropertyW.html)
//				OwlModel				model								IN
//				int64_t					rdfPropertyType						IN
//				const wchar_t			* name								IN
//
//				RdfProperty				returns								OUT
//
//	Returns a handle to an on the fly created property, however when
//	a property with this name already exists the handle of existing property will be returned.
//
//	The following reasons will cause a return value of 0:
//		-	when the name is already used for a class or instance;
//		-	if the model input is zero or not a model handle.
//
//	Giving the property a name is optional, if a name is not given it will recieve an automatically generated name,
//	it's automatically generated name can change between sessions.
//
RdfProperty		DECL STDC	CreatePropertyW(
									OwlModel				model,
									int64_t					rdfPropertyType,
									const wchar_t			* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	RdfProperty	CreatePropertyW(
									OwlModel				model,
									int64_t					rdfPropertyType,
									wchar_t					* name
								)
{
	return	CreatePropertyW(
					model,
					rdfPropertyType,
					(const wchar_t*) name
				);
}

//
//
static	inline	RdfProperty	CreatePropertyW(
									OwlModel				model,
									int64_t					rdfPropertyType
								)
{
	return	CreatePropertyW(
					model,
					rdfPropertyType,
					(const wchar_t*) nullptr			//	name
				);
}

//
//
static	inline	RdfProperty	CreatePropertyW(
									OwlModel				model
								)
{
	return	CreatePropertyW(
					model,
					0,									//	rdfPropertyType
					(const wchar_t*) nullptr			//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetPropertyByName                                       (http://rdf.bg/gkdoc/CP64/GetPropertyByName.html)
//				OwlModel				model								IN
//				const char				* name								IN
//
//				RdfProperty				returns								OUT
//
//	Returns a handle to the objectTypeProperty or dataTypeProperty as stored inside.
//	When there is no property with such a name the return value is 0 (note that GetModellingStyle(..) can change this behavior).
//
RdfProperty		DECL STDC	GetPropertyByName(
									OwlModel				model,
									const char				* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	RdfProperty	GetPropertyByName(
									OwlModel				model,
									char					* name
								)
{
	return	GetPropertyByName(
					model,
					(const char*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetPropertyByNameW                                      (http://rdf.bg/gkdoc/CP64/GetPropertyByNameW.html)
//				OwlModel				model								IN
//				const wchar_t			* name								IN
//
//				RdfProperty				returns								OUT
//
//	Returns a handle to the objectTypeProperty or dataTypeProperty as stored inside.
//	When there is no property with such a name the return value is 0 (note that GetModellingStyle(..) can change this behavior).
//
RdfProperty		DECL STDC	GetPropertyByNameW(
									OwlModel				model,
									const wchar_t			* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	RdfProperty	GetPropertyByNameW(
									OwlModel				model,
									wchar_t					* name
								)
{
	return	GetPropertyByNameW(
					model,
					(const wchar_t*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetPropertiesByIterator                                 (http://rdf.bg/gkdoc/CP64/GetPropertiesByIterator.html)
//				OwlModel				model								IN
//				RdfProperty				rdfProperty							IN
//
//				RdfProperty				returns								OUT
//
//	Returns a handle to a property.
//	If input property is zero, the handle will point to the first relevant property.
//	If all properties are past (or no relevant properties are found), the function will return 0.
//
RdfProperty		DECL STDC	GetPropertiesByIterator(
									OwlModel				model,
									RdfProperty				rdfProperty
								);

//
//		SetPropertyRangeRestriction                             (http://rdf.bg/gkdoc/CP64/SetPropertyRangeRestriction.html)
//				RdfProperty				rdfProperty							IN
//				OwlClass				owlClass							IN
//				int64_t					setting								IN
//
//				void					returns
//
//	Sets or unsets a specific owlClass as range restriction to an rdfProperty. The property is expected to
//	be an objectp[roperty, i.e. relation.]
//	If rdfProperty is not an object property this call has no effect.
//
void			DECL STDC	SetPropertyRangeRestriction(
									RdfProperty				rdfProperty,
									OwlClass				owlClass,
									int64_t					setting
								);

//
//		SetPropertyRangeRestrictionEx                           (http://rdf.bg/gkdoc/CP64/SetPropertyRangeRestrictionEx.html)
//				OwlModel				model								IN
//				RdfProperty				rdfProperty							IN
//				OwlClass				owlClass							IN
//				int64_t					setting								IN
//
//				void					returns
//
//	Sets or unsets a specific owlClass as range restriction to an rdfProperty. The property is expected to
//	be an objectp[roperty, i.e. relation.]
//	If rdfProperty is not an object property this call has no effect.
//
void			DECL STDC	SetPropertyRangeRestrictionEx(
									OwlModel				model,
									RdfProperty				rdfProperty,
									OwlClass				owlClass,
									int64_t					setting
								);

//
//		GetRangeRestrictionsByIterator                          (http://rdf.bg/gkdoc/CP64/GetRangeRestrictionsByIterator.html)
//				RdfProperty				rdfProperty							IN
//				OwlClass				owlClass							IN
//
//				int64_t					returns								OUT
//
//	Returns the next class the property is restricted to.
//	If input class is zero, the handle will point to the first relevant class.
//	If all classes are past (or no relevant classes are found), the function will return 0.
//
int64_t			DECL STDC	GetRangeRestrictionsByIterator(
									RdfProperty				rdfProperty,
									OwlClass				owlClass
								);

//
//		GetRangeRestrictionsByIteratorEx                        (http://rdf.bg/gkdoc/CP64/GetRangeRestrictionsByIteratorEx.html)
//				OwlModel				model								IN
//				RdfProperty				rdfProperty							IN
//				OwlClass				owlClass							IN
//
//				int64_t					returns								OUT
//
//	Returns the next class the property is restricted to.
//	If input class is zero, the handle will point to the first relevant class.
//	If all classes are past (or no relevant classes are found), the function will return 0.
//
int64_t			DECL STDC	GetRangeRestrictionsByIteratorEx(
									OwlModel				model,
									RdfProperty				rdfProperty,
									OwlClass				owlClass
								);

//
//		GetPropertyParentsByIterator                            (http://rdf.bg/gkdoc/CP64/GetPropertyParentsByIterator.html)
//				RdfProperty				rdfProperty							IN
//				RdfProperty				parentRdfProperty					IN
//
//				RdfProperty				returns								OUT
//
//	Returns the next parent of the property.
//	If input parent is zero, the handle will point to the first relevant parent.
//	If all parent are past (or no relevant parent are found), the function will return 0.
//
RdfProperty		DECL STDC	GetPropertyParentsByIterator(
									RdfProperty				rdfProperty,
									RdfProperty				parentRdfProperty
								);

//
//		SetNameOfProperty                                       (http://rdf.bg/gkdoc/CP64/SetNameOfProperty.html)
//				RdfProperty				rdfProperty							IN
//				const char				* name								IN
//
//				int64_t					returns								OUT
//
//	Sets or updates the name of the property, it returns 0 on success.
//
//	Error return codes:
//		0	successful
//		1	argument rdfProperty is incorrect (not a proper handle to an active property)
//		2	argument name is incorrect (nullptr or zero length name)
//		3	the name of rdfProperty is locked
//		4	name is already used by another class
//		5	name is already used by a property
//		6	name is already used by an instance
//		7	undefined error
//
int64_t			DECL STDC	SetNameOfProperty(
									RdfProperty				rdfProperty,
									const char				* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetNameOfProperty(
								RdfProperty				rdfProperty,
								char					* name
							)
{
	return	SetNameOfProperty(
					rdfProperty,
					(const char*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetNameOfPropertyW                                      (http://rdf.bg/gkdoc/CP64/SetNameOfPropertyW.html)
//				RdfProperty				rdfProperty							IN
//				const wchar_t			* name								IN
//
//				int64_t					returns								OUT
//
//	Sets or updates the name of the property, it returns 0 on success.
//
//	Error return codes:
//		0	successful
//		1	argument rdfProperty is incorrect (not a proper handle to an active property)
//		2	argument name is incorrect (nullptr or zero length name)
//		3	the name of rdfProperty is locked
//		4	name is already used by another class
//		5	name is already used by a property
//		6	name is already used by an instance
//		7	undefined error
//
int64_t			DECL STDC	SetNameOfPropertyW(
									RdfProperty				rdfProperty,
									const wchar_t			* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetNameOfPropertyW(
								RdfProperty				rdfProperty,
								wchar_t					* name
							)
{
	return	SetNameOfPropertyW(
					rdfProperty,
					(const wchar_t*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetNameOfPropertyEx                                     (http://rdf.bg/gkdoc/CP64/SetNameOfPropertyEx.html)
//				OwlModel				model								IN
//				RdfProperty				rdfProperty							IN
//				const char				* name								IN
//
//				int64_t					returns								OUT
//
//	Sets or updates the name of the property, it returns 0 on success.
//
//	Error return codes:
//		0	successful
//		1	argument model or rdfProperty is incorrect (not a proper handle to an active property)
//		2	argument name is incorrect (nullptr or zero length name)
//		3	the name of rdfProperty is locked
//		4	name is already used by another class
//		5	name is already used by a property
//		6	name is already used by an instance
//		7	undefined error
//
//	This call has the same behavior as SetNameOfProperty, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
int64_t			DECL STDC	SetNameOfPropertyEx(
									OwlModel				model,
									RdfProperty				rdfProperty,
									const char				* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetNameOfPropertyEx(
								OwlModel				model,
								RdfProperty				rdfProperty,
								char					* name
							)
{
	return	SetNameOfPropertyEx(
					model,
					rdfProperty,
					(const char*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetNameOfPropertyWEx                                    (http://rdf.bg/gkdoc/CP64/SetNameOfPropertyWEx.html)
//				OwlModel				model								IN
//				RdfProperty				rdfProperty							IN
//				const wchar_t			* name								IN
//
//				int64_t					returns								OUT
//
//	Sets or updates the name of the property, it returns 0 on success.
//
//	Error return codes:
//		0	successful
//		1	argument model or rdfProperty is incorrect (not a proper handle to an active property)
//		2	argument name is incorrect (nullptr or zero length name)
//		3	the name of rdfProperty is locked
//		4	name is already used by another class
//		5	name is already used by a property
//		6	name is already used by an instance
//		7	undefined error
//
//	This call has the same behavior as SetNameOfPropertyW, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
int64_t			DECL STDC	SetNameOfPropertyWEx(
									OwlModel				model,
									RdfProperty				rdfProperty,
									const wchar_t			* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetNameOfPropertyWEx(
								OwlModel				model,
								RdfProperty				rdfProperty,
								wchar_t					* name
							)
{
	return	SetNameOfPropertyWEx(
					model,
					rdfProperty,
					(const wchar_t*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetNameOfProperty                                       (http://rdf.bg/gkdoc/CP64/GetNameOfProperty.html)
//				RdfProperty				rdfProperty							IN
//				const char				** name								IN / OUT
//
//				const char				* returns							OUT
//
//	Returns the name of the property, if the property does not exist it returns nullptr.
//
const char		DECL * STDC	GetNameOfProperty(
									RdfProperty				rdfProperty,
									const char				** name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const char	* GetNameOfProperty(
									RdfProperty				rdfProperty,
									char					** name
								)
{
	return	GetNameOfProperty(
					rdfProperty,
					(const char**) name
				);
}

//
//
static	inline	const char	* GetNameOfProperty(
									RdfProperty				rdfProperty
								)
{
	return	GetNameOfProperty(
					rdfProperty,
					(const char**) nullptr				//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetNameOfPropertyW                                      (http://rdf.bg/gkdoc/CP64/GetNameOfPropertyW.html)
//				RdfProperty				rdfProperty							IN
//				const wchar_t			** name								IN / OUT
//
//				const wchar_t			* returns							OUT
//
//	Returns the name of the property, if the property does not exist it returns nullptr.
//
const wchar_t	DECL * STDC	GetNameOfPropertyW(
									RdfProperty				rdfProperty,
									const wchar_t			** name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const wchar_t	* GetNameOfPropertyW(
										RdfProperty				rdfProperty,
										wchar_t					** name
									)
{
	return	GetNameOfPropertyW(
					rdfProperty,
					(const wchar_t**) name
				);
}

//
//
static	inline	const wchar_t	* GetNameOfPropertyW(
										RdfProperty				rdfProperty
									)
{
	return	GetNameOfPropertyW(
					rdfProperty,
					(const wchar_t**) nullptr			//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetNameOfPropertyEx                                     (http://rdf.bg/gkdoc/CP64/GetNameOfPropertyEx.html)
//				OwlModel				model								IN
//				RdfProperty				rdfProperty							IN
//				const char				** name								IN / OUT
//
//				const char				* returns							OUT
//
//	Returns the name of the property, if the property does not exist it returns nullptr.
//
const char		DECL * STDC	GetNameOfPropertyEx(
									OwlModel				model,
									RdfProperty				rdfProperty,
									const char				** name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const char	* GetNameOfPropertyEx(
									OwlModel				model,
									RdfProperty				rdfProperty,
									char					** name
								)
{
	return	GetNameOfPropertyEx(
					model,
					rdfProperty,
					(const char**) name
				);
}

//
//
static	inline	const char	* GetNameOfPropertyEx(
									OwlModel				model,
									RdfProperty				rdfProperty
								)
{
	return	GetNameOfPropertyEx(
					model,
					rdfProperty,
					(const char**) nullptr				//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetNameOfPropertyWEx                                    (http://rdf.bg/gkdoc/CP64/GetNameOfPropertyWEx.html)
//				OwlModel				model								IN
//				RdfProperty				rdfProperty							IN
//				const wchar_t			** name								IN / OUT
//
//				const wchar_t			* returns							OUT
//
//	Returns the name of the property, if the property does not exist it returns nullptr.
//
const wchar_t	DECL * STDC	GetNameOfPropertyWEx(
									OwlModel				model,
									RdfProperty				rdfProperty,
									const wchar_t			** name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const wchar_t	* GetNameOfPropertyWEx(
										OwlModel				model,
										RdfProperty				rdfProperty,
										wchar_t					** name
									)
{
	return	GetNameOfPropertyWEx(
					model,
					rdfProperty,
					(const wchar_t**) name
				);
}

//
//
static	inline	const wchar_t	* GetNameOfPropertyWEx(
										OwlModel				model,
										RdfProperty				rdfProperty
									)
{
	return	GetNameOfPropertyWEx(
					model,
					rdfProperty,
					(const wchar_t**) nullptr			//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetPropertyType                                         (http://rdf.bg/gkdoc/CP64/SetPropertyType.html)
//				RdfProperty				rdfProperty							IN
//				RdfPropertyType			propertyType						IN
//
//				RdfPropertyType			returns								OUT
//
//	This function sets the type of the property. This is only allowed
//	if the type of the property was not set before.
//
//	The following values are possible for propertyType:
//			OBJECTPROPERTY_TYPE				The property is an Object Property
//			DATATYPEPROPERTY_TYPE_BOOLEAN	The property is an Datatype Property of type Boolean
//			DATATYPEPROPERTY_TYPE_CHAR		The property is an Datatype Property of type Char
//			DATATYPEPROPERTY_TYPE_INTEGER	The property is an Datatype Property of type Integer
//			DATATYPEPROPERTY_TYPE_DOUBLE	The property is an Datatype Property of type Double
//	The return value of this call is GetPropertyType/Ex applied after applying
//	the type, normally this corresponds with the propertyType requested
//	to be set unless the property already has a different propertyType set before.
//
RdfPropertyType	DECL STDC	SetPropertyType(
									RdfProperty				rdfProperty,
									RdfPropertyType			propertyType
								);

//
//		SetPropertyTypeEx                                       (http://rdf.bg/gkdoc/CP64/SetPropertyTypeEx.html)
//				OwlModel				model								IN
//				RdfProperty				rdfProperty							IN
//				RdfPropertyType			propertyType						IN
//
//				RdfPropertyType			returns								OUT
//
//
RdfPropertyType	DECL STDC	SetPropertyTypeEx(
									OwlModel				model,
									RdfProperty				rdfProperty,
									RdfPropertyType			propertyType
								);

//
//		GetPropertyType                                         (http://rdf.bg/gkdoc/CP64/GetPropertyType.html)
//				RdfProperty				rdfProperty							IN
//
//				RdfPropertyType			returns								OUT
//
//	This function returns the type of the property.
//	The following return values are possible:
//		0								The property is not defined yet
//		OBJECTPROPERTY_TYPE				The property is an Object Property
//		DATATYPEPROPERTY_TYPE_BOOLEAN	The property is an Datatype Property of type Boolean
//		DATATYPEPROPERTY_TYPE_CHAR		The property is an Datatype Property of type Char
//		DATATYPEPROPERTY_TYPE_INTEGER	The property is an Datatype Property of type Integer
//		DATATYPEPROPERTY_TYPE_DOUBLE	The property is an Datatype Property of type Double
//
RdfPropertyType	DECL STDC	GetPropertyType(
									RdfProperty				rdfProperty
								);

//
//		GetPropertyTypeEx                                       (http://rdf.bg/gkdoc/CP64/GetPropertyTypeEx.html)
//				OwlModel				model								IN
//				RdfProperty				rdfProperty							IN
//
//				RdfPropertyType			returns								OUT
//
//	This call has the same behavior as GetPropertyType, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
RdfPropertyType	DECL STDC	GetPropertyTypeEx(
									OwlModel				model,
									RdfProperty				rdfProperty
								);

//
//		RemoveProperty                                          (http://rdf.bg/gkdoc/CP64/RemoveProperty.html)
//				RdfProperty				rdfProperty							IN
//
//				int64_t					returns								OUT
//
//	This call is named remove property instead of a at first sight more logical name delete property as all content depending on this property is not lost per se.
//	Each properties having the removed property as a parent will now inherit ther parents of the removed property.
//	All property values in the context of an instance will become property values of the parent property, or in case no parent property is defined the values are lost.
//	The return value represents a bit set defining findings during the removal, if a clean removal with no side effects was possible the return value is 0. In all other cases 
//	the following bits represent the findings during removal:
//		bit 0:
//			0	Input as expected
//			1	Encountered an issue on input value, i.e. property was not recognized as property
//		bit 1:
//			0	No 'child' properties found
//			1	Properties found that had this property as a parent, they are adjusted by inheriting directly removed properties parents if present
//		bit 2:
//			0	No instances found with value restrictions for this property
//			1	Vertex does contain 3D point info
//		bit 3:
//			0	No instances found with values for this property
//			1	Vertex does contain 3D normal vector info => if set, bit 4 will also be set
//		bit 6:
//			0	Vertex does not contain first 2D texture info
//			1	Vertex does contain first 2D texture info
//		bit 7:
//			0	Vertex does not contain second 2D texture info
//			1	Vertex does contain second 2D texture info => if set, bit 6 will also be set
//
//	Error return codes:
//		0	successful
//		1	argument rdfProperty is incorrect (not a proper handle to an active property)
//		2	another property is dependent on the property to be deleted (for example through an inheritance relation)
//		3	an instance has a non-zero cardinality for the property to be deleted
//		4	undefined error
//
int64_t			DECL STDC	RemoveProperty(
									RdfProperty				rdfProperty
								);

//
//		RemovePropertyEx                                        (http://rdf.bg/gkdoc/CP64/RemovePropertyEx.html)
//				OwlModel				model								IN
//				RdfProperty				rdfProperty							IN
//
//				int64_t					returns								OUT
//
//	This call is named remove property instead of a at first sight more logical name delete property as all content depending on this property is not lost per se.
//	Each properties having the removed property as a parent will now inherit ther parents of the removed property.
//	All property values in the context of an instance will become property values of the parent property, or in case no parent property is defined the values are lost.
//	The return value represents a bit set defining findings during the removal, if a clean removal with no side effects was possible the return value is 0. In all other cases 
//	the following bits represent the findings during removal:
//		bit 0:
//			0	Input as expected
//			1	Encountered an issue on input value, i.e. property was not recognized as property
//		bit 1:
//			0	No 'child' properties found
//			1	Properties found that had this property as a parent, they are adjusted by inheriting directly removed properties parents if present
//		bit 2:
//			0	No instances found with value restrictions for this property
//			1	Vertex does contain 3D point info
//		bit 3:
//			0	No instances found with values for this property
//			1	Vertex does contain 3D normal vector info => if set, bit 4 will also be set
//		bit 6:
//			0	Vertex does not contain first 2D texture info
//			1	Vertex does contain first 2D texture info
//		bit 7:
//			0	Vertex does not contain second 2D texture info
//			1	Vertex does contain second 2D texture info => if set, bit 6 will also be set
//
//	Error return codes:
//		0	successful
//		1	argument model or rdfProperty is incorrect (not a proper handle to an active model)
//		2	another property is dependent on the property to be deleted (for example through an inheritance relation)
//		3	an instance has a non-zero cardinality for the property to be deleted
//		4	undefined error
//
//	This call has the same behavior as RemoveProperty, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
int64_t			DECL STDC	RemovePropertyEx(
									OwlModel				model,
									RdfProperty				rdfProperty
								);

//
//		IsProperty                                              (http://rdf.bg/gkdoc/CP64/IsProperty.html)
//				RdfsResource			rdfsResource						IN
//
//				RdfProperty				returns								OUT
//
//	Returns RdfProperty if the argument rdfsResource is an actual active property in an active model. It returns 0 in all other cases,
//	i.e. this could mean the model is already closed, the property is inactive or removed or the session is closed.
//	It could also mean it represents a handle to another resource, for example a class, instance or model.
//
RdfProperty		DECL STDC	IsProperty(
									RdfsResource			rdfsResource
								);

//
//  Design Tree Instances API Calls
//

//
//		CreateInstance                                          (http://rdf.bg/gkdoc/CP64/CreateInstance.html)
//				OwlClass				owlClass							IN
//				const char				* name								IN
//
//				OwlInstance				returns								OUT
//
//	Returns a handle to an on the fly created instance.
//	If the owlClass input is zero or not a class handle 0 will be returned,
//
OwlInstance		DECL STDC	CreateInstance(
									OwlClass				owlClass,
									const char				* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlInstance	CreateInstance(
									OwlClass				owlClass,
									char					* name
								)
{
	return	CreateInstance(
					owlClass,
					(const char*) name
				);
}

//
//
static	inline	OwlInstance	CreateInstance(
									OwlClass				owlClass
								)
{
	const char	* name = nullptr;
	return	CreateInstance(
					owlClass,
					(const char*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		CreateInstanceW                                         (http://rdf.bg/gkdoc/CP64/CreateInstanceW.html)
//				OwlClass				owlClass							IN
//				const wchar_t			* name								IN
//
//				OwlInstance				returns								OUT
//
//	Returns a handle to an on the fly created instance.
//	If the owlClass input is zero or not a class handle 0 will be returned,
//
OwlInstance		DECL STDC	CreateInstanceW(
									OwlClass				owlClass,
									const wchar_t			* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlInstance	CreateInstanceW(
									OwlClass				owlClass,
									wchar_t					* name
								)
{
	return	CreateInstanceW(
					owlClass,
					(const wchar_t*) name
				);
}

//
//
static	inline	OwlInstance	CreateInstanceW(
									OwlClass				owlClass
								)
{
	const wchar_t	* name = nullptr;
	return	CreateInstanceW(
					owlClass,
					(const wchar_t*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		CreateInstanceEx                                        (http://rdf.bg/gkdoc/CP64/CreateInstanceEx.html)
//				OwlModel				model								IN
//				OwlClass				owlClass							IN
//				const char				* name								IN
//
//				OwlInstance				returns								OUT
//
//	Returns a handle to an on the fly created instance.
//	If the owlClass input is zero or not a class handle 0 will be returned,
//
OwlInstance		DECL STDC	CreateInstanceEx(
									OwlModel				model,
									OwlClass				owlClass,
									const char				* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlInstance	CreateInstanceEx(
									OwlModel				model,
									OwlClass				owlClass,
									char					* name
								)
{
	return	CreateInstanceEx(
					model,
					owlClass,
					(const char*) name
				);
}

//
//
static	inline	OwlInstance	CreateInstanceEx(
									OwlModel				model,
									OwlClass				owlClass
								)
{
	const char	* name = nullptr;
	return	CreateInstanceEx(
					model,
					owlClass,
					(const char*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		CreateInstanceWEx                                       (http://rdf.bg/gkdoc/CP64/CreateInstanceWEx.html)
//				OwlModel				model								IN
//				OwlClass				owlClass							IN
//				const wchar_t			* name								IN
//
//				OwlInstance				returns								OUT
//
//	Returns a handle to an on the fly created instance.
//	If the owlClass input is zero or not a class handle 0 will be returned,
//
OwlInstance		DECL STDC	CreateInstanceWEx(
									OwlModel				model,
									OwlClass				owlClass,
									const wchar_t			* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	OwlInstance	CreateInstanceWEx(
									OwlModel				model,
									OwlClass				owlClass,
									wchar_t					* name
								)
{
	return	CreateInstanceWEx(
					model,
					owlClass,
					(const wchar_t*) name
				);
}

//
//
static	inline	OwlInstance	CreateInstanceWEx(
									OwlModel				model,
									OwlClass				owlClass
								)
{
	const wchar_t	* name = nullptr;
	return	CreateInstanceWEx(
					model,
					owlClass,
					(const wchar_t*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetInstancesByIterator                                  (http://rdf.bg/gkdoc/CP64/GetInstancesByIterator.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//
//				OwlInstance				returns								OUT
//
//	Returns a handle to an instance.
//	If input instance is zero, the handle will point to the first relevant instance.
//	If all instances are past (or no relevant instances are found), the function will return 0.
//
OwlInstance		DECL STDC	GetInstancesByIterator(
									OwlModel				model,
									OwlInstance				owlInstance
								);

//
//		GetInstanceClass                                        (http://rdf.bg/gkdoc/CP64/GetInstanceClass.html)
//				OwlInstance				owlInstance							IN
//
//				OwlClass				returns								OUT
//
//	Returns the handle to the class of which the instance is instantiated. In case the instance 
//	is instantiated on more than one class it will return 0.
//
OwlClass		DECL STDC	GetInstanceClass(
									OwlInstance				owlInstance
								);

//
//		GetInstanceClassEx                                      (http://rdf.bg/gkdoc/CP64/GetInstanceClassEx.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//
//				OwlClass				returns								OUT
//
//	Returns the handle to the class of which the instance is instantiated. In case the instance 
//	is instantiated on more than one class it will return 0.
//
OwlClass		DECL STDC	GetInstanceClassEx(
									OwlModel				model,
									OwlInstance				owlInstance
								);

//
//		GetInstanceClassByIterator                              (http://rdf.bg/gkdoc/CP64/GetInstanceClassByIterator.html)
//				OwlInstance				owlInstance							IN
//				OwlClass				owlClass							IN
//
//				OwlClass				returns								OUT
//
//	Iterates over the classes the instance is instantiated from.
//
OwlClass		DECL STDC	GetInstanceClassByIterator(
									OwlInstance				owlInstance,
									OwlClass				owlClass
								);

//
//		GetInstanceClassByIteratorEx                            (http://rdf.bg/gkdoc/CP64/GetInstanceClassByIteratorEx.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//				OwlClass				owlClass							IN
//
//				OwlClass				returns								OUT
//
//	Iterates over the classes the instance is instantiated from.
//
OwlClass		DECL STDC	GetInstanceClassByIteratorEx(
									OwlModel				model,
									OwlInstance				owlInstance,
									OwlClass				owlClass
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	void	GetInstancePropertyCardinalityRestriction(
								OwlInstance				owlInstance,
								RdfProperty				rdfProperty,
								int64_t					* minCard,
								int64_t					* maxCard
							)
{
	OwlClass	owlClass = GetInstanceClassByIterator(owlInstance, 0);

	GetClassPropertyCardinalityRestriction(
			owlClass,
			rdfProperty,
			minCard,
			maxCard
		);

	while ( (owlClass = GetInstanceClassByIterator(owlInstance, owlClass)) ) {
		int64_t	myMinCard = -1,
				myMaxCard = -1;

		GetClassPropertyCardinalityRestriction(
				owlClass,
				rdfProperty,
				&myMinCard,
				&myMaxCard
			);

		if ((*minCard) < myMinCard)
			(*minCard) = myMinCard;

		if (myMaxCard >= 0 &&
			((*maxCard) == -1 || (*maxCard) > myMaxCard))
			(*maxCard) = myMaxCard;
	}
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetInstanceGeometryClass                                (http://rdf.bg/gkdoc/CP64/GetInstanceGeometryClass.html)
//				OwlInstance				owlInstance							IN
//
//				OwlClass				returns								OUT
//
//	If one of the classes this instance is instantiated from or one of its parents is a geometry class,
//	this class is returned. In all other cases the return value is 0.
//
OwlClass		DECL STDC	GetInstanceGeometryClass(
									OwlInstance				owlInstance
								);

//
//		GetInstanceGeometryClassEx                              (http://rdf.bg/gkdoc/CP64/GetInstanceGeometryClassEx.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//
//				OwlClass				returns								OUT
//
//	If one of the classes this instance is instantiated from or one of its parents is a geometry class,
//	this class is returned. In all other cases the return value is 0.
//
OwlClass		DECL STDC	GetInstanceGeometryClassEx(
									OwlModel				model,
									OwlInstance				owlInstance
								);

//
//		GetInstancePropertyByIterator                           (http://rdf.bg/gkdoc/CP64/GetInstancePropertyByIterator.html)
//				OwlInstance				owlInstance							IN
//				RdfProperty				rdfProperty							IN
//
//				RdfProperty				returns								OUT
//
//	Returns a handle to the objectTypeProperty or dataTypeProperty connected to
//	the instance, this property can also contain a value, but for example also
//	the knowledge about cardinality restrictions in the context of this instance's class
//	and the exact cardinality in context of its instance.
//
RdfProperty		DECL STDC	GetInstancePropertyByIterator(
									OwlInstance				owlInstance,
									RdfProperty				rdfProperty
								);

//
//		GetInstancePropertyByIteratorEx                         (http://rdf.bg/gkdoc/CP64/GetInstancePropertyByIteratorEx.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//				RdfProperty				rdfProperty							IN
//
//				RdfProperty				returns								OUT
//
//
RdfProperty		DECL STDC	GetInstancePropertyByIteratorEx(
									OwlModel				model,
									OwlInstance				owlInstance,
									RdfProperty				rdfProperty
								);

//
//		GetInstanceInverseReferencesByIterator                  (http://rdf.bg/gkdoc/CP64/GetInstanceInverseReferencesByIterator.html)
//				OwlInstance				owlInstance							IN
//				OwlInstance				referencingOwlInstance				IN
//
//				OwlInstance				returns								OUT
//
//	Returns a handle to the owlInstances refering this instance
//
OwlInstance		DECL STDC	GetInstanceInverseReferencesByIterator(
									OwlInstance				owlInstance,
									OwlInstance				referencingOwlInstance
								);

//
//		GetInstanceReferencesByIterator                         (http://rdf.bg/gkdoc/CP64/GetInstanceReferencesByIterator.html)
//				OwlInstance				owlInstance							IN
//				OwlInstance				referencedOwlInstance				IN
//
//				OwlInstance				returns								OUT
//
//	Returns a handle to the owlInstance refered by this instance
//
OwlInstance		DECL STDC	GetInstanceReferencesByIterator(
									OwlInstance				owlInstance,
									OwlInstance				referencedOwlInstance
								);

//
//		SetNameOfInstance                                       (http://rdf.bg/gkdoc/CP64/SetNameOfInstance.html)
//				OwlInstance				owlInstance							IN
//				const char				* name								IN
//
//				int64_t					returns								OUT
//
//	Sets or updates the name of the instance, it returns 0 on success.
//
//	Error return codes:
//		0	successful
//		1	argument owlInstance is incorrect (not a proper handle to an active instance)
//		2	argument name is incorrect (nullptr or zero length name)
//		3	the name of instance is locked
//		4	name is already used by another class
//		5	name is already used by a property
//		6	name is already used by an instance
//		7	undefined error
//
int64_t			DECL STDC	SetNameOfInstance(
									OwlInstance				owlInstance,
									const char				* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetNameOfInstance(
								OwlInstance				owlInstance,
								char					* name
							)
{
	return	SetNameOfInstance(
					owlInstance,
					(const char*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetNameOfInstanceW                                      (http://rdf.bg/gkdoc/CP64/SetNameOfInstanceW.html)
//				OwlInstance				owlInstance							IN
//				const wchar_t			* name								IN
//
//				int64_t					returns								OUT
//
//	Sets or updates the name of the instance, it returns 0 on success.
//
//	Error return codes:
//		0	successful
//		1	argument owlInstance is incorrect (not a proper handle to an active instance)
//		2	argument name is incorrect (nullptr or zero length name)
//		3	the name of instance is locked
//		4	name is already used by another class
//		5	name is already used by a property
//		6	name is already used by an instance
//		7	undefined error
//
int64_t			DECL STDC	SetNameOfInstanceW(
									OwlInstance				owlInstance,
									const wchar_t			* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetNameOfInstanceW(
								OwlInstance				owlInstance,
								wchar_t					* name
							)
{
	return	SetNameOfInstanceW(
					owlInstance,
					(const wchar_t*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetNameOfInstanceEx                                     (http://rdf.bg/gkdoc/CP64/SetNameOfInstanceEx.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//				const char				* name								IN
//
//				int64_t					returns								OUT
//
//	Sets or updates the name of the instance, it returns 0 on success.
//
//	Error return codes:
//		0	successful
//		1	argument model or owlInstance is incorrect (not a proper handle to an active instance)
//		2	argument name is incorrect (nullptr or zero length name)
//		3	the name of instance is locked
//		4	name is already used by another class
//		5	name is already used by a property
//		6	name is already used by an instance
//		7	undefined error
//
//	This call has the same behavior as SetNameOfInstance, however needs to be
//	used in case instance are exchanged as a successive series of integers.
//
int64_t			DECL STDC	SetNameOfInstanceEx(
									OwlModel				model,
									OwlInstance				owlInstance,
									const char				* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetNameOfInstanceEx(
								OwlModel				model,
								OwlInstance				owlInstance,
								char					* name
							)
{
	return	SetNameOfInstanceEx(
					model,
					owlInstance,
					(const char*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetNameOfInstanceWEx                                    (http://rdf.bg/gkdoc/CP64/SetNameOfInstanceWEx.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//				const wchar_t			* name								IN
//
//				int64_t					returns								OUT
//
//	Sets or updates the name of the instance, it returns 0 on success.
//
//	Error return codes:
//		0	successful
//		1	argument model or owlInstance is incorrect (not a proper handle to an active instance)
//		2	argument name is incorrect (nullptr or zero length name)
//		3	the name of instance is locked
//		4	name is already used by another class
//		5	name is already used by a property
//		6	name is already used by an instance
//		7	undefined error
//
//	This call has the same behavior as SetNameOfInstanceW, however needs to be
//	used in case instances are exchanged as a successive series of integers.
//
int64_t			DECL STDC	SetNameOfInstanceWEx(
									OwlModel				model,
									OwlInstance				owlInstance,
									const wchar_t			* name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetNameOfInstanceWEx(
								OwlModel				model,
								OwlInstance				owlInstance,
								wchar_t					* name
							)
{
	return	SetNameOfInstanceWEx(
					model,
					owlInstance,
					(const wchar_t*) name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetNameOfInstance                                       (http://rdf.bg/gkdoc/CP64/GetNameOfInstance.html)
//				OwlInstance				owlInstance							IN
//				const char				** name								IN / OUT
//
//				const char				* returns							OUT
//
//	Returns the name of the instance, if the instance does not exist it returns nullptr.
//
const char		DECL * STDC	GetNameOfInstance(
									OwlInstance				owlInstance,
									const char				** name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const char	* GetNameOfInstance(
									OwlInstance				owlInstance,
									char					** name
								)
{
	return	GetNameOfInstance(
					owlInstance,
					(const char**) name
				);
}

//
//
static	inline	const char	* GetNameOfInstance(
									OwlInstance				owlInstance
								)
{
	return	GetNameOfInstance(
					owlInstance,
					(const char**) nullptr				//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetNameOfInstanceW                                      (http://rdf.bg/gkdoc/CP64/GetNameOfInstanceW.html)
//				OwlInstance				owlInstance							IN
//				const wchar_t			** name								IN / OUT
//
//				const wchar_t			* returns							OUT
//
//	Returns the name of the instance, if the instance does not exist it returns nullptr.
//
const wchar_t	DECL * STDC	GetNameOfInstanceW(
									OwlInstance				owlInstance,
									const wchar_t			** name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const wchar_t	* GetNameOfInstanceW(
										OwlInstance				owlInstance,
										wchar_t					** name
									)
{
	return	GetNameOfInstanceW(
					owlInstance,
					(const wchar_t**) name
				);
}

//
//
static	inline	const wchar_t	* GetNameOfInstanceW(
										OwlInstance				owlInstance
									)
{
	return	GetNameOfInstanceW(
					owlInstance,
					(const wchar_t**) nullptr			//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetNameOfInstanceEx                                     (http://rdf.bg/gkdoc/CP64/GetNameOfInstanceEx.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//				const char				** name								IN / OUT
//
//				const char				* returns							OUT
//
//	Returns the name of the instance, if the instance does not exist it returns nullptr.
//
const char		DECL * STDC	GetNameOfInstanceEx(
									OwlModel				model,
									OwlInstance				owlInstance,
									const char				** name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const char	* GetNameOfInstanceEx(
									OwlModel				model,
									OwlInstance				owlInstance,
									char					** name
								)
{
	return	GetNameOfInstanceEx(
					model,
					owlInstance,
					(const char**) name
				);
}

//
//
static	inline	const char	* GetNameOfInstanceEx(
									OwlModel				model,
									OwlInstance				owlInstance
								)
{
	return	GetNameOfInstanceEx(
					model,
					owlInstance,
					(const char**) nullptr				//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetNameOfInstanceWEx                                    (http://rdf.bg/gkdoc/CP64/GetNameOfInstanceWEx.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//				const wchar_t			** name								IN / OUT
//
//				const wchar_t			* returns							OUT
//
//	Returns the name of the instance, if the instance does not exist it returns nullptr.
//
const wchar_t	DECL * STDC	GetNameOfInstanceWEx(
									OwlModel				model,
									OwlInstance				owlInstance,
									const wchar_t			** name
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	const wchar_t	* GetNameOfInstanceWEx(
										OwlModel				model,
										OwlInstance				owlInstance,
										wchar_t					** name
									)
{
	return	GetNameOfInstanceWEx(
					model,
					owlInstance,
					(const wchar_t**) name
				);
}

//
//
static	inline	const wchar_t	* GetNameOfInstanceWEx(
										OwlModel				model,
										OwlInstance				owlInstance
									)
{
	return	GetNameOfInstanceWEx(
					model,
					owlInstance,
					(const wchar_t**) nullptr			//	name
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetDatatypeProperty                                     (http://rdf.bg/gkdoc/CP64/SetDatatypeProperty.html)
//				OwlInstance				owlInstance							IN
//				OwlDatatypeProperty		owlDatatypeProperty					IN
//				const void				* values							IN
//				int64_t					card								IN
//
//				int64_t					returns								OUT
//
//	This function sets the value(s) of a certain datatypeTypeProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of undefined (void) items is a list of booleans, chars, integers
//	or doubles, this list has a length as givin in the values card. The actual used type
//	is given by the definition of the dataTypeProperty.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
//	Note: the client application needs to make sure the cardinality of
//		  the property is within the boundaries.
//
int64_t			DECL STDC	SetDatatypeProperty(
									OwlInstance				owlInstance,
									OwlDatatypeProperty		owlDatatypeProperty,
									const void				* values,
									int64_t					card
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetDatatypeProperty(
								OwlInstance				owlInstance,
								OwlDatatypeProperty		owlDatatypeProperty,
								bool					value
							)
{
	assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_BOOLEAN);
	const int64_t	card = 1;
	return	SetDatatypeProperty(
					owlInstance,
					owlDatatypeProperty,
					(const void*) &value,
					card
				);
}

//
//
static	inline	int64_t	SetDatatypeProperty(
								OwlInstance				owlInstance,
								OwlDatatypeProperty		owlDatatypeProperty,
								const char				* value
							)
{
	assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_STRING || GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_CHAR_ARRAY);
	const int64_t	card = 1;
	return	SetDatatypeProperty(
					owlInstance,
					owlDatatypeProperty,
					(const void*) &value,
					card
				);
}

//
//
static	inline	int64_t	SetDatatypeProperty(
								OwlInstance				owlInstance,
								OwlDatatypeProperty		owlDatatypeProperty,
								const wchar_t			* value
							)
{
	assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_STRING || GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_WCHAR_T_ARRAY);
	const int64_t	card = 1;
	return	SetDatatypeProperty(
					owlInstance,
					owlDatatypeProperty,
					(const void*) &value,
					card
				);
}

//
//
static	inline	int64_t	SetDatatypeProperty(
								OwlInstance				owlInstance,
								OwlDatatypeProperty		owlDatatypeProperty,
								int64_t					value
							)
{
	assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_INTEGER);
	const int64_t	card = 1;
	return	SetDatatypeProperty(
					owlInstance,
					owlDatatypeProperty,
					(const void*) &value,
					card
				);
}

//
//
static	inline	int64_t	SetDatatypeProperty(
								OwlInstance				owlInstance,
								OwlDatatypeProperty		owlDatatypeProperty,
								double					value
							)
{
	assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_DOUBLE);
	const int64_t	card = 1;
	return	SetDatatypeProperty(
					owlInstance,
					owlDatatypeProperty,
					(const void*) &value,
					card
				);
}

//
//
static	inline	int64_t	SetDatatypeProperty(
								OwlInstance				owlInstance,
								OwlDatatypeProperty		owlDatatypeProperty,
								unsigned char			value
							)
{
	assert(GetPropertyType(owlDatatypeProperty) == DATATYPEPROPERTY_TYPE_BYTE);
	const int64_t	card = 1;
	return	SetDatatypeProperty(
					owlInstance,
					owlDatatypeProperty,
					(const void*) &value,
					card
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetDatatypePropertyEx                                   (http://rdf.bg/gkdoc/CP64/SetDatatypePropertyEx.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//				OwlDatatypeProperty		owlDatatypeProperty					IN
//				const void				* values							IN
//				int64_t					card								IN
//
//				int64_t					returns								OUT
//
//	This function sets the value(s) of a certain datatypeTypeProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of undefined (void) items is a list of booleans, chars, integers
//	or doubles, this list has a length as givin in the values card. The actual used type
//	is given by the definition of the dataTypeProperty.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
//	This call has the same behavior as SetDatatypeProperty, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
//	Note: the client application needs to make sure the cardinality of
//		  the property is within the boundaries.
//
int64_t			DECL STDC	SetDatatypePropertyEx(
									OwlModel				model,
									OwlInstance				owlInstance,
									OwlDatatypeProperty		owlDatatypeProperty,
									const void				* values,
									int64_t					card
								);

//
//		GetDatatypeProperty                                     (http://rdf.bg/gkdoc/CP64/GetDatatypeProperty.html)
//				OwlInstance				owlInstance							IN
//				OwlDatatypeProperty		owlDatatypeProperty					IN
//				const void				** values							IN / OUT
//				int64_t					* card								IN / OUT
//
//				int64_t					returns								OUT
//
//	This function gets the value(s) of a certain datatypeTypeProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of undefined (void) items is a list of booleans, chars, integers
//	or doubles, this list has a length as givin in the value card. The actual used type
//	is given by the definition of the dataTypeProperty.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
int64_t			DECL STDC	GetDatatypeProperty(
									OwlInstance				owlInstance,
									OwlDatatypeProperty		owlDatatypeProperty,
									const void				** values,
									int64_t					* card
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	GetDatatypeProperty(
								OwlInstance				owlInstance,
								OwlDatatypeProperty		owlDatatypeProperty,
								void					** values,
								int64_t					* card
							)
{
	return	GetDatatypeProperty(
					owlInstance,
					owlDatatypeProperty,
					(const void**) values,
					card
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetDatatypePropertyEx                                   (http://rdf.bg/gkdoc/CP64/GetDatatypePropertyEx.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//				OwlDatatypeProperty		owlDatatypeProperty					IN
//				const void				** values							IN / OUT
//				int64_t					* card								IN / OUT
//
//				int64_t					returns								OUT
//
//	This function gets the value(s) of a certain datatypeTypeProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of undefined (void) items is a list of booleans, chars, integers
//	or doubles, this list has a length as givin in the value card. The actual used type
//	is given by the definition of the dataTypeProperty.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
//	This call has the same behavior as GetDatatypeProperty, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
int64_t			DECL STDC	GetDatatypePropertyEx(
									OwlModel				model,
									OwlInstance				owlInstance,
									OwlDatatypeProperty		owlDatatypeProperty,
									const void				** values,
									int64_t					* card
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	GetDatatypePropertyEx(
								OwlModel				model,
								OwlInstance				owlInstance,
								OwlDatatypeProperty		owlDatatypeProperty,
								void					** values,
								int64_t					* card
							)
{
	return	GetDatatypePropertyEx(
					model,
					owlInstance,
					owlDatatypeProperty,
					(const void**) values,
					card
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetObjectProperty                                       (http://rdf.bg/gkdoc/CP64/SetObjectProperty.html)
//				OwlInstance				owlInstance							IN
//				OwlObjectProperty		owlObjectProperty					IN
//				const OwlInstance		* values							IN
//				int64_t					card								IN
//
//				int64_t					returns								OUT
//
//	This function sets the value(s) of a certain objectTypeProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of integers is a list of handles to instances, this list
//	has a length as givin in the values card.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
//	Note: the client application needs to make sure the cardinality of
//		  the property is within the boundaries.
//
int64_t			DECL STDC	SetObjectProperty(
									OwlInstance				owlInstance,
									OwlObjectProperty		owlObjectProperty,
									const OwlInstance		* values,
									int64_t					card
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	SetObjectProperty(
								OwlInstance				owlInstance,
								OwlObjectProperty		owlObjectProperty,
								OwlInstance				value
							)
{
	assert(GetPropertyType(owlObjectProperty) == OBJECTPROPERTY_TYPE);
	const int64_t	card = 1;
	return	SetObjectProperty(
					owlInstance,
					owlObjectProperty,
					(const OwlInstance*) &value,
					card
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetObjectPropertyEx                                     (http://rdf.bg/gkdoc/CP64/SetObjectPropertyEx.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//				OwlObjectProperty		owlObjectProperty					IN
//				const OwlInstance		* values							IN
//				int64_t					card								IN
//
//				int64_t					returns								OUT
//
//	This function sets the value(s) of a certain objectTypeProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of integers is a list of handles to instances, this list
//	has a length as givin in the values card.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
//	This call has the same behavior as SetObjectProperty, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
//	Note: the client application needs to make sure the cardinality of
//		  the property is within the boundaries.
//
int64_t			DECL STDC	SetObjectPropertyEx(
									OwlModel				model,
									OwlInstance				owlInstance,
									OwlObjectProperty		owlObjectProperty,
									const OwlInstance		* values,
									int64_t					card
								);

//
//		GetObjectProperty                                       (http://rdf.bg/gkdoc/CP64/GetObjectProperty.html)
//				OwlInstance				owlInstance							IN
//				OwlObjectProperty		owlObjectProperty					IN
//				const OwlInstance		** values							IN / OUT
//				int64_t					* card								IN / OUT
//
//				int64_t					returns								OUT
//
//	This function gets the value(s) of a certain objectProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of integers is a list of handles to instances, this list
//	has a length as givin in the value card.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
int64_t			DECL STDC	GetObjectProperty(
									OwlInstance				owlInstance,
									OwlObjectProperty		owlObjectProperty,
									const OwlInstance		** values,
									int64_t					* card
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	GetObjectProperty(
								OwlInstance				owlInstance,
								OwlObjectProperty		owlObjectProperty,
								OwlInstance				** values,
								int64_t					* card
							)
{
	return	GetObjectProperty(
					owlInstance,
					owlObjectProperty,
					(const OwlInstance**) values,
					card
				);
}

//
//
static	inline	OwlInstance	GetObjectProperty(
									OwlInstance				owlInstance,
									OwlObjectProperty		owlObjectProperty
								)
{
	OwlInstance	* values = nullptr;
	int64_t		card = 0;

	GetObjectProperty(
			owlInstance,
			owlObjectProperty,
			&values,
			&card
		);

	if (card == 1)
		return	values[0];
	else
		return	0;
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetObjectPropertyEx                                     (http://rdf.bg/gkdoc/CP64/GetObjectPropertyEx.html)
//				OwlModel				model								IN
//				OwlInstance				owlInstance							IN
//				OwlObjectProperty		owlObjectProperty					IN
//				const OwlInstance		** values							IN / OUT
//				int64_t					* card								IN / OUT
//
//				int64_t					returns								OUT
//
//	This function gets the value(s) of a certain objectProperty
//	in the context of an instance.
//	The value of card gives the actual card of the values list.
//	The list values of integers is a list of handles to instances, this list
//	has a length as givin in the values card.
//	The return value always should be 0, if not something is wrong in the way this property is called.
//
//	This call has the same behavior as GetObjectProperty, however needs to be
//	used in case properties are exchanged as a successive series of integers.
//
int64_t			DECL STDC	GetObjectPropertyEx(
									OwlModel				model,
									OwlInstance				owlInstance,
									OwlObjectProperty		owlObjectProperty,
									const OwlInstance		** values,
									int64_t					* card
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	GetObjectPropertyEx(
								OwlModel				model,
								OwlInstance				owlInstance,
								OwlObjectProperty		owlObjectProperty,
								OwlInstance				** values,
								int64_t					* card
							)
{
	return	GetObjectPropertyEx(
					model,
					owlInstance,
					owlObjectProperty,
					(const OwlInstance**) values,
					card
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		CreateInstanceInContextStructure                        (http://rdf.bg/gkdoc/CP64/CreateInstanceInContextStructure.html)
//				OwlInstance				owlInstance							IN
//
//				int64_t					returns								OUT
//
//	InstanceInContext structures give you more detailed information about
//	individual parts of the geometry of a certain instance viualized.
//	It is allowed to have more then 1 InstanceInContext structures per instance.
//	InstanceInContext structures are updated dynamically when the geometry
//	structure is updated.
//
int64_t			DECL STDC	CreateInstanceInContextStructure(
									OwlInstance				owlInstance
								);

//
//		DestroyInstanceInContextStructure                       (http://rdf.bg/gkdoc/CP64/DestroyInstanceInContextStructure.html)
//				OwlInstance				owlInstanceInContext				IN
//
//				void					returns
//
//	InstanceInContext structures are updated dynamically and therfore even while the cost
//	in performance and memory is limited it is advised to destroy structures as soon
//	as they are obsolete.
//
void			DECL STDC	DestroyInstanceInContextStructure(
									OwlInstance				owlInstanceInContext
								);

//
//		InstanceInContextChild                                  (http://rdf.bg/gkdoc/CP64/InstanceInContextChild.html)
//				OwlInstance				owlInstanceInContext				IN
//
//				int64_t					returns								OUT
//
//
int64_t			DECL STDC	InstanceInContextChild(
									OwlInstance				owlInstanceInContext
								);

//
//		InstanceInContextNext                                   (http://rdf.bg/gkdoc/CP64/InstanceInContextNext.html)
//				OwlInstance				owlInstanceInContext				IN
//
//				int64_t					returns								OUT
//
//
int64_t			DECL STDC	InstanceInContextNext(
									OwlInstance				owlInstanceInContext
								);

//
//		InstanceInContextIsUpdated                              (http://rdf.bg/gkdoc/CP64/InstanceInContextIsUpdated.html)
//				OwlInstance				owlInstanceInContext				IN
//
//				int64_t					returns								OUT
//
//
int64_t			DECL STDC	InstanceInContextIsUpdated(
									OwlInstance				owlInstanceInContext
								);

//
//		RemoveInstance                                          (http://rdf.bg/gkdoc/CP64/RemoveInstance.html)
//				OwlInstance				owlInstance							IN
//
//				int64_t					returns								OUT
//
//	This function removes an instance from the internal structure.
//	In case copies are created by the host this function checks if all
//	copies are removed otherwise the instance will stay available.
//	Return value is 0 if everything went ok and positive in case of an error
//
int64_t			DECL STDC	RemoveInstance(
									OwlInstance				owlInstance
								);

//
//		RemoveInstanceRecursively                               (http://rdf.bg/gkdoc/CP64/RemoveInstanceRecursively.html)
//				OwlInstance				owlInstance							IN
//
//				int64_t					returns								OUT
//
//	This function removes an instance recursively from the internal structure.
//	In case checkInverseRelations is non-zero only instances that are not referenced
//	by other existing instances.
//
//	Return value is total number of removed instances
//
int64_t			DECL STDC	RemoveInstanceRecursively(
									OwlInstance				owlInstance
								);

//
//		RemoveInstances                                         (http://rdf.bg/gkdoc/CP64/RemoveInstances.html)
//				OwlModel				model								IN
//
//				int64_t					returns								OUT
//
//	This function removes all available instances for the given model 
//	from the internal structure.
//	Return value is the number of removed instances.
//
int64_t			DECL STDC	RemoveInstances(
									OwlModel				model
								);

//
//		IsInstance                                              (http://rdf.bg/gkdoc/CP64/IsInstance.html)
//				RdfsResource			rdfsResource						IN
//
//				OwlInstance				returns								OUT
//
//	Returns OwlInstance if the argument rdfsResource is an actual active instance in an active model. It returns 0 in all other cases,
//	i.e. this could mean the model is already closed, the instance is inactive or removed or the session is closed.
//	It could also mean it represents a handle to another resource, for example a class, property or model.
//
OwlInstance		DECL STDC	IsInstance(
									RdfsResource			rdfsResource
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	bool	IsKindOfClass(
								OwlClass				myOwlClass,
								OwlClass				owlClass
							)
{
	if (myOwlClass == owlClass)
		return	true;
	int64_t	parentOwlClass = GetClassParentsByIterator(myOwlClass, 0);
	while (parentOwlClass) {
		if (IsKindOfClass(parentOwlClass, owlClass))
			return	true;
		parentOwlClass = GetClassParentsByIterator(myOwlClass, parentOwlClass);
	}
	return	false;
}

//
//
static	inline	bool	IsInstanceOfClass(
								OwlInstance				owlInstance,
								const char				* name
							)
{
	return	IsKindOfClass(GetInstanceClass(owlInstance), GetClassByName(GetModel(owlInstance), name));
}

//
//
static	inline	bool	IsInstanceOfClass(
								OwlInstance				owlInstance,
								char					* name
							)
{
	return	IsInstanceOfClass(
					owlInstance,
					(const char*) name
				);
}

//
//
static	inline	bool	IsInstanceOfClassExact(
								OwlInstance				owlInstance,
								const char				* name
							)
{
	return	GetInstanceClass(owlInstance) == GetClassByName(GetModel(owlInstance), name);
}

//
//
static	inline	bool	IsInstanceOfClassExact(
								OwlInstance				owlInstance,
								char					* name
							)
{
	return	IsInstanceOfClassExact(
					owlInstance,
					(const char*) name
				);
}

//
//  Retrieve Geometry API Calls
//

#ifdef __cplusplus
	extern "C" {
#endif

//
//		CalculateInstance                                       (http://rdf.bg/gkdoc/CP64/CalculateInstance.html)
//				OwlInstance				owlInstance							IN
//				int64_t					* vertexBufferSize					IN / OUT
//				int64_t					* indexBufferSize					IN / OUT
//				int64_t					* transformationBufferSize			IN / OUT
//
//				int64_t					returns								OUT
//
//	This function prepares the content to be ready so the buffers can be filled.
//	It returns the minimum size the buffers should be. This is only the case
//	when the pointer is given, all arguments are allowed to be nullptr.
//
//	Note: This function needs to be called directly before UpdateVertexBuffer(),
//		  UpdateIndexBuffer() and UpdateTransformationBuffer().
//
int64_t			DECL STDC	CalculateInstance(
									OwlInstance				owlInstance,
									int64_t					* vertexBufferSize,
									int64_t					* indexBufferSize,
									int64_t					* transformationBufferSize
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	CalculateInstance(
								OwlInstance				owlInstance,
								int64_t					* vertexBufferSize,
								int64_t					* indexBufferSize
							)
{
	return	CalculateInstance(
					owlInstance,
					vertexBufferSize,
					indexBufferSize,
					nullptr								//	transformationBufferSize
				);
}

//
//
static	inline	int64_t	CalculateInstance(
								OwlInstance				owlInstance
							)
{
	return	CalculateInstance(
					owlInstance,
					nullptr,							//	vertexBufferSize
					nullptr,							//	indexBufferSize
					nullptr								//	transformationBufferSize
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		UpdateInstance                                          (http://rdf.bg/gkdoc/CP64/UpdateInstance.html)
//				OwlInstance				owlInstance							IN
//
//				int64_t					returns								OUT
//
//	This function prepares the content to be ready without filling the buffers
//	as done within CalculateInstance(). CalculateInstance calls this function as a start.
//	This function will also set the 'derived' values for the instance passed as argument.
//	For example the coordinates values of a MultiplicationMatrix will be set if the array is
//	defined.
//
int64_t			DECL STDC	UpdateInstance(
									OwlInstance				owlInstance
								);

//
//		InferenceInstance                                       (http://rdf.bg/gkdoc/CP64/InferenceInstance.html)
//				OwlInstance				owlInstance							IN
//
//				int64_t					returns								OUT
//
//	This function fills in values that are implicitely known but not given by the user. This function
//	can also be used to identify default values of properties if not given.
//
int64_t			DECL STDC	InferenceInstance(
									OwlInstance				owlInstance
								);

//
//		UpdateInstanceVertexBuffer                              (http://rdf.bg/gkdoc/CP64/UpdateInstanceVertexBuffer.html)
//				OwlInstance				owlInstance							IN
//				void					* vertexBuffer						IN / OUT
//
//				int64_t					returns								OUT
//
//	This function should be preceded by the function CalculateInstances(),
//	the only allowed other API functions in between are UpdateIndexBuffer()
//	and UpdateTransformationBuffer().
//	It is expected to be called with a buffer vertexBuffer of at least the size as 
//	given by CalculateInstances().
//	If not called for the first time it will expect to contain the same content as
//	from previous call, even is size is changed. This can be overruled by 
//	the function ClearedExternalBuffers().
//
int64_t			DECL STDC	UpdateInstanceVertexBuffer(
									OwlInstance				owlInstance,
									void					* vertexBuffer
								);

//
//		UpdateInstanceVertexBufferTrimmed                       (http://rdf.bg/gkdoc/CP64/UpdateInstanceVertexBufferTrimmed.html)
//				OwlInstance				owlInstance							IN
//				void					* vertexBuffer						IN / OUT
//				int64_t					offset								IN
//				int64_t					size								IN
//
//				int64_t					returns								OUT
//
//	This function is an alternative for UpdateInstanceVertexBuffer(),
//	in case the vertex buffer should be divided over a set of arrays
//	this function allows to fill part of the vertex buffer given a
//	certain offset and size (both calculated in vertex element count).
//
int64_t			DECL STDC	UpdateInstanceVertexBufferTrimmed(
									OwlInstance				owlInstance,
									void					* vertexBuffer,
									int64_t					offset,
									int64_t					size
								);

//
//		UpdateInstanceIndexBuffer                               (http://rdf.bg/gkdoc/CP64/UpdateInstanceIndexBuffer.html)
//				OwlInstance				owlInstance							IN
//				void					* indexBuffer						IN / OUT
//
//				int64_t					returns								OUT
//
//	This function should be preceded by the function CalculateInstances(),
//	the only allowed other API functions in between are UpdateVertexBuffer()
//	and UpdateTransformationBuffer().
//	It is expected to be called with a buffer indexBuffer of at least the size as 
//	given by CalculateInstances().
//	If not called for the first time it will expect to contain the same content as
//	from previous call, even is size is changed. This can be overruled by 
//	the function ClearedExternalBuffers().
//
int64_t			DECL STDC	UpdateInstanceIndexBuffer(
									OwlInstance				owlInstance,
									void					* indexBuffer
								);

//
//		UpdateInstanceIndexBufferTrimmed                        (http://rdf.bg/gkdoc/CP64/UpdateInstanceIndexBufferTrimmed.html)
//				OwlInstance				owlInstance							IN
//				void					* indexBuffer						IN / OUT
//				int64_t					offset								IN
//				int64_t					size								IN
//
//				int64_t					returns								OUT
//
//	This function is an alternative for UpdateInstanceIndexBuffer(),
//	in case the index buffer should be divided over a set of arrays
//	this function allows to fill part of the index buffer given a
//	certain offset and size.
//
int64_t			DECL STDC	UpdateInstanceIndexBufferTrimmed(
									OwlInstance				owlInstance,
									void					* indexBuffer,
									int64_t					offset,
									int64_t					size
								);

//
//		UpdateInstanceTransformationBuffer                      (http://rdf.bg/gkdoc/CP64/UpdateInstanceTransformationBuffer.html)
//				OwlInstance				owlInstance							IN
//				double					* transformationBuffer				IN / OUT
//
//				int64_t					returns								OUT
//
//	This function should be preceded by the function CalculateInstances(),
//	the only allowed other API functions in between are UpdateVertexBuffer()
//	and UpdateIndexBuffer().
//	It is expected to be called with a buffer vertexBuffer of at least the size as 
//	given by CalculateInstances().
//	If not called for the first time it will expect to contain the same content as
//	from previous call, even is size is changed. This can be overruled by 
//	the function ClearedExternalBuffers().
//
int64_t			DECL STDC	UpdateInstanceTransformationBuffer(
									OwlInstance				owlInstance,
									double					* transformationBuffer
								);

//
//		ClearedInstanceExternalBuffers                          (http://rdf.bg/gkdoc/CP64/ClearedInstanceExternalBuffers.html)
//				OwlInstance				owlInstance							IN
//
//				void					returns
//
//	This function tells the engine that all buffers have no memory of earlier filling 
//	for a certain instance.
//	This means that even when buffer content didn't changed it will be updated when
//	functions UpdateVertexBuffer(), UpdateIndexBuffer() and/or transformationBuffer()
//	are called for this specific instance.
//
void			DECL STDC	ClearedInstanceExternalBuffers(
									OwlInstance				owlInstance
								);

//
//		ClearedExternalBuffers                                  (http://rdf.bg/gkdoc/CP64/ClearedExternalBuffers.html)
//				OwlModel				model								IN
//
//				void					returns
//
//	This function tells the engine that all buffers have no memory of earlier filling.
//	This means that even when buffer content didn't changed it will be updated when
//	functions UpdateVertexBuffer(), UpdateIndexBuffer() and/or transformationBuffer()
//	are called.
//
void			DECL STDC	ClearedExternalBuffers(
									OwlModel				model
								);

//
//		GetConceptualFaceCnt                                    (http://rdf.bg/gkdoc/CP64/GetConceptualFaceCnt.html)
//				OwlInstance				owlInstance							IN
//
//				int64_t					returns								OUT
//
//	This function returns the number of conceptual faces for a certain instance.
//
int64_t			DECL STDC	GetConceptualFaceCnt(
									OwlInstance				owlInstance
								);

//
//		GetConceptualFace                                       (http://rdf.bg/gkdoc/CP64/GetConceptualFace.html)
//				OwlInstance				owlInstance							IN
//				int64_t					index								IN
//				int64_t					* startIndexTriangles				IN / OUT
//				int64_t					* noIndicesTriangles				IN / OUT
//				int64_t					* startIndexLines					IN / OUT
//				int64_t					* noIndicesLines					IN / OUT
//				int64_t					* startIndexPoints					IN / OUT
//				int64_t					* noIndicesPoints					IN / OUT
//				int64_t					* startIndexFacePolygons			IN / OUT
//				int64_t					* noIndicesFacePolygons				IN / OUT
//				int64_t					* startIndexConceptualFacePolygons	IN / OUT
//				int64_t					* noIndicesConceptualFacePolygons	IN / OUT
//
//				ConceptualFace			returns								OUT
//
//	This function returns a handle to the conceptual face. Be aware that different
//	instances can return the same handles (however with possible different startIndices and noIndicesTriangles).
//	Argument index should be at least zero and smaller then return value of GetConceptualFaceCnt().
//
ConceptualFace	DECL STDC	GetConceptualFace(
									OwlInstance				owlInstance,
									int64_t					index,
									int64_t					* startIndexTriangles,
									int64_t					* noIndicesTriangles,
									int64_t					* startIndexLines,
									int64_t					* noIndicesLines,
									int64_t					* startIndexPoints,
									int64_t					* noIndicesPoints,
									int64_t					* startIndexFacePolygons,
									int64_t					* noIndicesFacePolygons,
									int64_t					* startIndexConceptualFacePolygons,
									int64_t					* noIndicesConceptualFacePolygons
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	ConceptualFace	GetConceptualFace(
										OwlInstance				owlInstance,
										int64_t					index,
										int64_t					* startIndexTriangles,
										int64_t					* noIndicesTriangles,
										int64_t					* startIndexLines,
										int64_t					* noIndicesLines,
										int64_t					* startIndexPoints,
										int64_t					* noIndicesPoints
									)
{
	return	GetConceptualFace(
					owlInstance,
					index,
					startIndexTriangles,
					noIndicesTriangles,
					startIndexLines,
					noIndicesLines,
					startIndexPoints,
					noIndicesPoints,
					nullptr,		//	startIndexFacePolygons
					nullptr,		//	noIndicesFacePolygons
					nullptr,		//	startIndexConceptualFacePolygons
					nullptr			//	noIndicesConceptualFacePolygons
				);
}

//
//
static	inline	ConceptualFace	GetConceptualFace(
										OwlInstance				owlInstance,
										int64_t					index
									)
{
	return	GetConceptualFace(
					owlInstance,
					index,
					nullptr,		//	startIndexTriangles
					nullptr,		//	noIndicesTriangles
					nullptr,		//	startIndexLines
					nullptr,		//	noIndicesLines
					nullptr,		//	startIndexPoints
					nullptr			//	noIndicesPoints
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetConceptualFaceMaterial                               (http://rdf.bg/gkdoc/CP64/GetConceptualFaceMaterial.html)
//				ConceptualFace			conceptualFace						IN
//
//				OwlInstance				returns								OUT
//
//	This function returns the material instance relevant for this
//	conceptual face.
//
OwlInstance		DECL STDC	GetConceptualFaceMaterial(
									ConceptualFace			conceptualFace
								);

//
//		GetConceptualFaceOriginCnt                              (http://rdf.bg/gkdoc/CP64/GetConceptualFaceOriginCnt.html)
//				ConceptualFace			conceptualFace						IN
//
//				int64_t					returns								OUT
//
//	This function returns the number of instances that are the source primitive/concept
//	for this conceptual face.
//
int64_t			DECL STDC	GetConceptualFaceOriginCnt(
									ConceptualFace			conceptualFace
								);

//
//		GetConceptualFaceOrigin                                 (http://rdf.bg/gkdoc/CP64/GetConceptualFaceOrigin.html)
//				ConceptualFace			conceptualFace						IN
//				int64_t					index								IN
//
//				OwlInstance				returns								OUT
//
//	This function returns a handle to the instance that is the source primitive/concept
//	for this conceptual face.
//
OwlInstance		DECL STDC	GetConceptualFaceOrigin(
									ConceptualFace			conceptualFace,
									int64_t					index
								);

//
//		GetConceptualFaceOriginEx                               (http://rdf.bg/gkdoc/CP64/GetConceptualFaceOriginEx.html)
//				ConceptualFace			conceptualFace						IN
//				int64_t					index								IN
//				int64_t					* originatingOwlInstance			IN / OUT
//				int64_t					* originatingConceptualFace			IN / OUT
//
//				void					returns
//
//
void			DECL STDC	GetConceptualFaceOriginEx(
									ConceptualFace			conceptualFace,
									int64_t					index,
									int64_t					* originatingOwlInstance,
									int64_t					* originatingConceptualFace
								);

//
//		GetFaceCnt                                              (http://rdf.bg/gkdoc/CP64/GetFaceCnt.html)
//				OwlInstance				owlInstance							IN
//
//				int64_t					returns								OUT
//
//	This function returns the number of faces for a certain instance.
//
int64_t			DECL STDC	GetFaceCnt(
									OwlInstance				owlInstance
								);

//
//		GetFace                                                 (http://rdf.bg/gkdoc/CP64/GetFace.html)
//				OwlInstance				owlInstance							IN
//				int64_t					index								IN
//				int64_t					* startIndex						IN / OUT
//				int64_t					* noOpenings						IN / OUT
//
//				void					returns
//
//	This function gets the individual faces including the meta data, i.e. the number of openings within this specific face.
//	This call is for very dedicated use, it would be more common to iterate over the individual conceptual faces.
//
void			DECL STDC	GetFace(
									OwlInstance				owlInstance,
									int64_t					index,
									int64_t					* startIndex,
									int64_t					* noOpenings
								);

//
//		GetDependingPropertyCnt                                 (http://rdf.bg/gkdoc/CP64/GetDependingPropertyCnt.html)
//				OwlInstance				baseOwlInstance						IN
//				ConceptualFace			conceptualFace						IN
//
//				int64_t					returns								OUT
//
//	This function returns the number of properties that are of influence on the
//	location and form of the conceptualFace.
//
//	Note: BE AWARE, THIS FUNCTION EXPECTS A TREE, NOT A NETWORK, IN CASE OF A NETWORK THIS FUNCTION CAN LOCK THE ENGINE
//
int64_t			DECL STDC	GetDependingPropertyCnt(
									OwlInstance				baseOwlInstance,
									ConceptualFace			conceptualFace
								);

//
//		GetDependingProperty                                    (http://rdf.bg/gkdoc/CP64/GetDependingProperty.html)
//				OwlInstance				baseOwlInstance						IN
//				ConceptualFace			conceptualFace						IN
//				int64_t					index								IN
//				OwlInstance				* owlInstance						IN / OUT
//				OwlDatatypeProperty		* owlDatatypeProperty				IN / OUT
//
//				void					returns
//
//	This function returns a handle to the property that is the 'index'-th property
//	of influence on the form. It also returns the handle to instance this property
//	belongs to.
//
//	Note: the returned property is always a datatypeProperty
//	Note: if input is incorrect (for example index is in wrong domain) _property and
//		  instance will be both zero.
//	Note: BE AWARE, THIS FUNCTION EXPECTS A TREE, NOT A NETWORK, IN CASE OF A NETWORK THIS FUNCTION CAN LOCK THE ENGINE
//
void			DECL STDC	GetDependingProperty(
									OwlInstance				baseOwlInstance,
									ConceptualFace			conceptualFace,
									int64_t					index,
									OwlInstance				* owlInstance,
									OwlDatatypeProperty		* owlDatatypeProperty
								);

//
//		SetFormat                                               (http://rdf.bg/gkdoc/CP64/SetFormat.html)
//				OwlModel				model								IN
//				uint64_t				setting								IN
//				uint64_t				mask								IN
//
//				uint64_t				returns								OUT
//
//	This function sets the type of export format, by setting a mask
//		bit 0 & 1:
//			00	Each vertex is unique (although mostly irrelevant UpdateIndexBuffer() and 
//				UpdateTransformationBuffer() are still returning information)
//			01	Each index is unique => vertices are not necessarily (although mostly
//				irrelevant UpdateTransformationBuffer() is still returning information)
//			10	Single level Transformations are used, most optimal when using DirectX till version 11
//				and OpenGL till version 2
//			11	Nested Transformations are used, most optimal but till 2011 no known support of
//				low level 3D interfaces like DirectX and OpenGL
//		bit 2:	(FORMAT_SIZE_VERTEX_DOUBLE)
//			0	Vertex items returned as float (4 byte/32 bit)
//			1	Vertex items returned as double (8 byte/64 bit)
//		bit 3:	(FORMAT_SIZE_INDEX_INT64)
//			0	Index items returned as int32_t (4 byte/32 bit)
//			1	Index items returned as int64_t (8 byte/64 bit) (only available in 64 bit mode)
//
//		bit 4:	(FORMAT_VERTEX_POINT)
//			0	Vertex does not contain 3D point info
//			1	Vertex does contain 3D point info
//		bit 5:	(FORMAT_VERTEX_NORMAL)
//			0	Vertex does not contain 3D normal vector info
//			1	Vertex does contain 3D normal vector info => if set, bit 4 will also be set
//		bit 6:	(FORMAT_VERTEX_TEXTURE_UV)
//			0	Vertex does not contain first 2D texture info
//			1	Vertex does contain first 2D texture info
//		bit 7:	(FORMAT_VERTEX_TEXTURE2_UV)
//			0	Vertex does not contain second 2D texture info
//			1	Vertex does contain second 2D texture info => if set, bit 6 will also be set
//
//		bit 8:	(FORMAT_EXPORT_TRIANGLES)
//			0	No object form triangles are exported
//			1	Object form triangles are exported (effective if instance contains faces and/or solids)
//		bit 9:	(FORMAT_EXPORT_LINES)
//			0	No object polygon lines are exported
//			1	Object polygon lines are exported (effective if instance contains line representations)
//		bit 10:	(FORMAT_EXPORT_POINTS)
//			0	No object points are exported
//			1	Object points are exported (effective if instance contains point representations)
//
//		bit 11:	Reserved, by default 0
//
//		bit 12:	(FORMAT_EXPORT_FACE_POLYGONS)
//			0	No object face polygon lines are exported
//			1	Object face polygon lines (dense wireframe) are exported => if set, bit 8 will also be set
//		bit 13:	(FORMAT_EXPORT_CONCEPTUAL_FACE_POLYGONS)
//			0	No object conceptual face polygon lines are exported
//			1	Object conceptual face polygon lines (wireframe) are exported => if set, bit 12 will also be set
//		bit 14:	(FORMAT_EXPORT_POLYGONS_AS_TUPLES)
//			0	Polygon lines (wireframe) exported as list, i.e. typical 4 point polygon exported as  0 1 2 3 0 -1
//			1	Polygon lines (wireframe) exported as tuples, i.e. typical 4 point polygon exported as 0 1 1 2 2 3 3 0
//
//		bit 15:	(FORMAT_EXPORT_ADVANCED_NORMALS)
//			0	All normals of triangles are transformed orthogonal to the 2D face they belong to
//			1	Normals are exported to be in line with the original semantic form description (could be non orthogonal to the 2D face) 
//
//		bit 16:	(FORMAT_EXPORT_DIRECTX)
//			0	no specific behavior
//			1	Where possible DirectX compatibility is given to exported data (i.e. order of components in vertex buffer)
//					 => [bit 20, bit 21 both UNSET]
//					 => if set, bit 17 will be unset
//
//		bit 17:	(FORMAT_EXPORT_OPENGL)
//			0	no specific behavior
//			1	Where possible OpenGL compatibility is given to exported data (i.e. order of components in vertex buffer and inverted texture coordinates in Y direction)
//					 => [bit 20, bit 21 both SET]
//					 => if set, bit 16 will be unset
//
//		bit 18:	(FORMAT_EXPORT_DOUBLE_SIDED)
//			0	All faces are defined as calculated
//			1	Every face has exactly one opposite face (normally both index and vertex buffer are doubled in size)
//
//		bit 19:	Reserved, by default 0
//
//		bit 20-23:
//			0000	version 0 (used in case there is different behavior between versions in DirectX or OpenGL)
//			....	...
//			1111	version 15
//
//		bit 20:	(FORMAT_EXPORT_VERSION_0001)
//			0	Standard Triangle Rotation (LHS as expected by DirectX) 
//			1	Opposite Triangle Rotation (RHS as expected by OpenGL)
//		bit 21:	(FORMAT_EXPORT_VERSION_0010)
//			0	X, Y, Z (nX, nY, nZ) formatted as <X Y Z> considering internal concepts
//			1	X, Y, Z (nX, nY, nZ) formatted as <X -Z Y>, i.e. X, -Z, Y (nX, -nZ, nY) considering internal concepts (OpenGL)
//
//		bit 24:	(FORMAT_VERTEX_COLOR_AMBIENT)
//			0	Vertex does not contain Ambient color information
//			1	Vertex does contain Ambient color information
//		bit 25:	(FORMAT_VERTEX_COLOR_DIFFUSE)
//			0	Vertex does not contain Diffuse color information
//			1	Vertex does contain Diffuse color information
//		bit 26:	(FORMAT_VERTEX_COLOR_EMISSIVE)
//			0	Vertex does not contain Emissive color information
//			1	Vertex does contain Emissive color information
//		bit 27:	(FORMAT_VERTEX_COLOR_SPECULAR)
//			0	Vertex does not contain Specular color information
//			1	Vertex does contain Specular color information
//
//		bit 28:	(FORMAT_VERTEX_TEXTURE_TANGENT)
//			0	Vertex does not contain tangent vector for first texture
//			1	Vertex does contain tangent vector for first texture => if set, bit 6 will also be set
//		bit 29:	(FORMAT_VERTEX_TEXTURE_BINORMAL)
//			0	Vertex does not contain binormal vector for first texture
//			1	Vertex does contain binormal vector for first texture => if set, bit 6 will also be set
//		bit 30:	(FORMAT_VERTEX_TEXTURE2_TANGENT)		ONLY WORKS IN 64 BIT MODE
//			0	Vertex does not contain tangent vector for second texture
//			1	Vertex does contain tangent vector for second texture => if set, bit 6 will also be set
//		bit 31:	(FORMAT_VERTEX_TEXTURE2_BINORMAL)		ONLY WORKS IN 64 BIT MODE
//			0	Vertex does not contain binormal vector for second texture
//			1	Vertex does contain binormal vector for second texture => if set, bit 6 will also be set
//
//		bit 26-31:	Reserved, by default 0
//
//		bit 32-63:	Reserved, by default 0
//
//	Note: default setting is 0000 0000 0000 0000   0000 0000 0000 0000  -  0000 0000 0000 0000   1000 0001  0011 0000 = h0000 0000 - 0000 8130 = 33072
//
//
//	Depending on FORMAT_SIZE_VERTEX_DOUBLE each element in the vertex buffer is a double or float number.
//	Number of elements for each vertex depends on format setting. You can get the number by GetVertexElementsCounts. 
//	Each vertex block contains data items in an order according to the table below. The table also specifies when an item is present and number of elements 
//	it occupied. Use GetVertexDataOffset or GetVertexColor to get required item. 
//
//	#	Vertex data item	Included when format setting bit is on					Size (num of elements)
//	Point coordinates		X, Y, X				FORMAT_VERTEX_POINT	(bit 4)					3
//	Normal coordinates		Nx, Ny, Nz			FORMAT_VERTEX_NORMAL (bit 5)				3
//	Texture coordinates		T1u, T1v			FORMAT_VERTEX_TEXTURE_UV (bit 6)			2
//	2nd Texture coordinates	T2u, T2v			FORMAT_VERTEX_TEXTURE2_UV (bit 7)			2
//	Ambient color								FORMAT_VERTEX_COLOR_AMBIENT (bit 24)		1
//	Diffuse color								FORMAT_VERTEX_COLOR_DIFFUSE (bit 25)		1
//	Emissive color								FORMAT_VERTEX_COLOR _EMISSIVE (bit 26)		1
//	Specular color								FORMAT_VERTEX_COLOR _SPECULAR (bit 27)		1
//	Texture tangent			T1Tx, T1Ty, T1Tz	FORMAT_VERTEX_TEXTURE_TANGENT (bit 28)		3
//	Texture binormal		T1BNx,T1BNy,T1BNz	FORMAT_VERTEX_TEXTURE_BINORMAL (bit 29)		3
//	2nd texture tangent		T2Tx, T2Ty, T2Tz	FORMAT_VERTEX_TEXTURE2_TANGENT (bit 30)		3
//	2nd texture binormal	T2BNx,T2BNy,T2BNz	FORMAT_VERTEX_TEXTURE2_BINORMAL (bit 31)	3
//
uint64_t		DECL STDC	SetFormat(
									OwlModel				model,
									uint64_t				setting,
									uint64_t				mask
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	uint64_t	SetFormat(
									OwlModel				model
								)
{
	return	SetFormat(
					model,
					0,									//	setting
					0									//	mask
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetFormat                                               (http://rdf.bg/gkdoc/CP64/GetFormat.html)
//				OwlModel				model								IN
//				uint64_t				mask								IN
//
//				uint64_t				returns								OUT
//
//	Returns the current format given a mask.
//
uint64_t		DECL STDC	GetFormat(
									OwlModel				model,
									uint64_t				mask
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	uint64_t	GetFormat(
									OwlModel				model
								)
{
	return	GetFormat(
					model,
					0									//	mask
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetVertexDataOffset                                     (http://rdf.bg/gkdoc/CP64/GetVertexDataOffset.html)
//				int64_t					requiredData						IN
//				int64_t					setting								IN
//
//				int32_t					returns								OUT
//
//	Returns offset of the required data in a vertex elements array with the specified format settings
//	requiredData is one of the control vertex data bits (FORMAT_VERTEX...) or 0 to get count of all elements in vertex buffer
//	Functions returns -1 if the required data are absent with the settings.
//
//	Ensure your settings are actual. They may be differ you pass to SetFormat (for example because of mask)
//	It's preferable to inquire resulting setting with GetFormat(model, GetFormat(0, 0))
//
//	Note: vertex buffer element is a double or a float number depending on FORMAT_SIZE_VERTEX_DOUBLE flag. 
//	If you need offset in bytes multiply by size of element.
//	Compare to SetFormat that returns size of vertex data in bytes.
//
int32_t			DECL STDC	GetVertexDataOffset(
									int64_t					requiredData,
									int64_t					setting
								);

//
//		SetBehavior                                             (http://rdf.bg/gkdoc/CP64/SetBehavior.html)
//				OwlModel				model								IN
//				uint64_t				setting								IN
//				uint64_t				mask								IN
//
//				void					returns
//
//	This function sets the type of behavior, by setting a mask
//
//		bit 0-7:	Reserved, by default 0
//
//		bit 8:
//			0	Do not optimize
//			1	Vertex items returned as double (8 byte/64 bit)
//
//		bit 9-31:	Reserved, by default 0
//
//		bit 32-63:	Reserved, by default 0
//
//	Note: default setting is 0000 0000 0000 0000   0000 0000 0000 0000  -  0000 0000 0000 0000   0000 0001  0000 0000 = h0000 0000 - 0000 0100 = 256
//
void			DECL STDC	SetBehavior(
									OwlModel				model,
									uint64_t				setting,
									uint64_t				mask
								);

//
//		GetBehavior                                             (http://rdf.bg/gkdoc/CP64/GetBehavior.html)
//				OwlModel				model								IN
//				uint64_t				mask								IN
//
//				uint64_t				returns								OUT
//
//	Returns the current behavior given a mask.
//
uint64_t		DECL STDC	GetBehavior(
									OwlModel				model,
									uint64_t				mask
								);

//
//		SetVertexBufferTransformation                           (http://rdf.bg/gkdoc/CP64/SetVertexBufferTransformation.html)
//				OwlModel				model								IN
//				const double			* matrix							IN
//
//				void					returns
//
//	Sets the transformation for a Vertex Buffer.
//	The transformation will always be calculated in 64 bit, even if the vertex element size is 32 bit.
//	This function can be called just before updating the vertex buffer.
//
void			DECL STDC	SetVertexBufferTransformation(
									OwlModel				model,
									const double			* matrix
								);

//
//		GetVertexBufferTransformation                           (http://rdf.bg/gkdoc/CP64/GetVertexBufferTransformation.html)
//				OwlModel				model								IN
//				double					* matrix							IN / OUT
//
//				void					returns
//
//	Gets the transformation for a Vertex Buffer.
//
void			DECL STDC	GetVertexBufferTransformation(
									OwlModel				model,
									double					* matrix
								);

//
//		SetIndexBufferOffset                                    (http://rdf.bg/gkdoc/CP64/SetIndexBufferOffset.html)
//				OwlModel				model								IN
//				int64_t					offset								IN
//
//				void					returns
//
//	Sets the offset for an Index Buffer.
//	It is important call this function before updating the vertex buffer. 
//
void			DECL STDC	SetIndexBufferOffset(
									OwlModel				model,
									int64_t					offset
								);

//
//		GetIndexBufferOffset                                    (http://rdf.bg/gkdoc/CP64/GetIndexBufferOffset.html)
//				OwlModel				model								IN
//
//				int64_t					returns								OUT
//
//	Gets the current offset for an Index Buffer.
//
int64_t			DECL STDC	GetIndexBufferOffset(
									OwlModel				model
								);

//
//		SetVertexBufferOffset                                   (http://rdf.bg/gkdoc/CP64/SetVertexBufferOffset.html)
//				OwlModel				model								IN
//				double					x									IN
//				double					y									IN
//				double					z									IN
//
//				void					returns
//
//	Sets the offset for a Vertex Buffer.
//	The offset will always be calculated in 64 bit, even if the vertex element size is 32 bit.
//	This function can be called just before updating the vertex buffer.
//
void			DECL STDC	SetVertexBufferOffset(
									OwlModel				model,
									double					x,
									double					y,
									double					z
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	void	SetVertexBufferOffset(
								OwlModel				model,
								const double			* offset
							)
{
	if (offset) {
		return	SetVertexBufferOffset(
						model,
						offset[0],
						offset[1],
						offset[2]
					);
	}
	else {
		return	SetVertexBufferOffset(
						model,
						0.,
						0.,
						0.
					);
	}
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetVertexBufferOffset                                   (http://rdf.bg/gkdoc/CP64/GetVertexBufferOffset.html)
//				OwlModel				model								IN
//				double					* x									IN / OUT
//				double					* y									IN / OUT
//				double					* z									IN / OUT
//
//				void					returns
//
//	Gets the offset for a Vertex Buffer.
//
void			DECL STDC	GetVertexBufferOffset(
									OwlModel				model,
									double					* x,
									double					* y,
									double					* z
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	void	GetVertexBufferOffset(
								OwlModel				model,
								double					* offset
							)
{
	GetVertexBufferOffset(
			model,
			&offset[0],
			&offset[1],
			&offset[2]
		);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetDefaultColor                                         (http://rdf.bg/gkdoc/CP64/SetDefaultColor.html)
//				OwlModel				model								IN
//				uint32_t				ambient								IN
//				uint32_t				diffuse								IN
//				uint32_t				emissive							IN
//				uint32_t				specular							IN
//
//				void					returns
//
//	Set the default values for the colors defined as argument.
//
void			DECL STDC	SetDefaultColor(
									OwlModel				model,
									uint32_t				ambient,
									uint32_t				diffuse,
									uint32_t				emissive,
									uint32_t				specular
								);

//
//		GetDefaultColor                                         (http://rdf.bg/gkdoc/CP64/GetDefaultColor.html)
//				OwlModel				model								IN
//				uint32_t				* ambient							IN / OUT
//				uint32_t				* diffuse							IN / OUT
//				uint32_t				* emissive							IN / OUT
//				uint32_t				* specular							IN / OUT
//
//				void					returns
//
//	Retrieve the default values for the colors defined as argument.
//
void			DECL STDC	GetDefaultColor(
									OwlModel				model,
									uint32_t				* ambient,
									uint32_t				* diffuse,
									uint32_t				* emissive,
									uint32_t				* specular
								);

//
//		CheckConsistency                                        (http://rdf.bg/gkdoc/CP64/CheckConsistency.html)
//				OwlModel				model								IN
//				uint64_t				mask								IN
//
//				uint64_t				returns								OUT
//
//	This function returns information about the consistency of each instance.
//
//	The mask defined what type of information can be retrieved from this call, the mask is a bitwise definition.
//
//		bit 0:	Check Design Tree Consistency
//		bit 1:	Check Consistency for Triangle Output (through API)
//		bit 2:	Check Consistency for Line Output (through API)
//		bit 3:	Check Consistency for Point Output (through API)
//		bit 4:	Check Consistency for Generated Surfaces (through API)
//		bit 5:	Check Consistency for Generated Surfaces (internal)
//		bit 6:	Check Consistency for Generated Solids (through API)
//		bit 7:	Check Consistency for Generated Solids (internal)
//		bit 8:	Check Consistency for BoundingBox's
//		bit 9:	Check Consistency for Triangulation
//		bit 10: Check Consistency for Relations (through API)
//
//		bit 16:	Contains (Closed) Solid(s)
//		bit 18:	Contains (Closed) Infinite Solid(s)
//		bit 20:	Contains Closed Surface(s)
//		bit 21:	Contains Open Surface(s)
//		bit 22:	Contains Closed Infinite Surface(s)
//		bit 23:	Contains Open Infinite Surface(s)
//		bit 24:	Contains Closed Line(s)
//		bit 25:	Contains Open Line(s)
//		bit 26:	Contains Closed Infinite Line(s) [i.e. both ends in infinity]
//		bit 27:	Contains Open Infinite Line(s) [i.e. one end in infinity]
//		bit 28:	Contains (Closed) Point(s)
//
//	If a bit in the mask is set and the result of the check has an issue, the resulting value will have this bit set.
//	i.e. any non-zero return value in Check Consistency is an indication that something is wrong or unexpected; 
//	any non-zero return value in Contains is an indication that this type of geometry is expected in one of the instances; 
//
uint64_t		DECL STDC	CheckConsistency(
									OwlModel				model,
									uint64_t				mask
								);

//
//		CheckInstanceConsistency                                (http://rdf.bg/gkdoc/CP64/CheckInstanceConsistency.html)
//				OwlInstance				owlInstance							IN
//				uint64_t				mask								IN
//
//				uint64_t				returns								OUT
//
//	This function returns information about the consistency of the instance and indirectly referenced instances.
//
//	The mask defined what type of information can be retrieved from this call, the mask is a bitwise definition.
//
//		bit 0:	Check Design Tree Consistency
//		bit 1:	Check Consistency for Triangle Output (through API)
//		bit 2:	Check Consistency for Line Output (through API)
//		bit 3:	Check Consistency for Point Output (through API)
//		bit 4:	Check Consistency for Generated Surfaces (through API)
//		bit 5:	Check Consistency for Generated Surfaces (internal)
//		bit 6:	Check Consistency for Generated Solids (through API)
//		bit 7:	Check Consistency for Generated Solids (internal)
//		bit 8:	Check Consistency for BoundingBox's
//		bit 9:	Check Consistency for Triangulation
//		bit 10: Check Consistency for Relations (through API)
//
//		bit 16:	Contains (Closed) Solid(s)
//		bit 18:	Contains (Closed) Infinite Solid(s)
//		bit 20:	Contains Closed Surface(s)
//		bit 21:	Contains Open Surface(s)
//		bit 22:	Contains Closed Infinite Surface(s)
//		bit 23:	Contains Open Infinite Surface(s)
//		bit 24:	Contains Closed Line(s)
//		bit 25:	Contains Open Line(s)
//		bit 26:	Contains Closed Infinite Line(s) [i.e. both ends in infinity]
//		bit 27:	Contains Open Infinite Line(s) [i.e. one end in infinity]
//		bit 28:	Contains (Closed) Point(s)
//
//	If a bit in the mask is set and the result of the check has an issue, the resulting value will have this bit set.
//	i.e. any non-zero return value in Check Consistency is an indication that something is wrong or unexpected regarding the given instance; 
//	any non-zero return value in Contains is an indication that this type of geometry is expected regarding the given instance; 
//
uint64_t		DECL STDC	CheckInstanceConsistency(
									OwlInstance				owlInstance,
									uint64_t				mask
								);

//
//		IsDuplicate                                             (http://rdf.bg/gkdoc/CP64/IsDuplicate.html)
//				OwlInstance				originalOwlInstance					IN
//				OwlInstance				duplicateOwlInstance				IN
//				double					* duplicateMatrix					IN / OUT
//				double					absoluteEpsilon						IN
//				double					relativeEpsilon						IN
//				bool					checkMaterial						IN
//
//				bool					returns								OUT
//
//	Checks if two geometry representations are (almost) similar except for a transformation matrix and a given epsilon.
//	The parameter duplicateMatrix is optional and can be left to zero.
//
bool			DECL STDC	IsDuplicate(
									OwlInstance				originalOwlInstance,
									OwlInstance				duplicateOwlInstance,
									double					* duplicateMatrix,
									double					absoluteEpsilon,
									double					relativeEpsilon,
									bool					checkMaterial
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	bool	IsDuplicate(
								OwlInstance				originalOwlInstance,
								OwlInstance				duplicateOwlInstance,
								double					* duplicateMatrix,
								double					epsilon,
								bool					checkMaterial
							)
{
	return	IsDuplicate(
					originalOwlInstance,
					duplicateOwlInstance,
					duplicateMatrix,
					epsilon,
					0.,									//	relativeEpsilon
					checkMaterial
				);
}

//
//
static	inline	bool	IsDuplicate(
								OwlInstance				originalOwlInstance,
								OwlInstance				duplicateOwlInstance
							)
{
	return	IsDuplicate(
					originalOwlInstance,
					duplicateOwlInstance,
					nullptr,							//	duplicateMatrix
					0.,									//	absoluteEpsilon
					0.,									//	relativeEpsilon
					false								//	checkMaterial
				);
}

//
//  Derived Geometry API Calls
//

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetPerimeter                                            (http://rdf.bg/gkdoc/CP64/GetPerimeter.html)
//				OwlInstance				owlInstance							IN
//
//				double					returns								OUT
//
//	This function calculates the perimeter of an instance.
//
//	Note: internally the call does not store its results, any optimization based on known
//		  dependancies between instances need to be implemented on the client.
//	Note: due to internal structure using already calculated vertex buffer / index buffer does not
//		  give any performance benefits, in opposite to GetVolume and GetArea
//
double			DECL STDC	GetPerimeter(
									OwlInstance				owlInstance
								);

//
//		GetArea                                                 (http://rdf.bg/gkdoc/CP64/GetArea.html)
//				OwlInstance				owlInstance							IN
//				const void				* vertexBuffer						IN
//				const void				* indexBuffer						IN
//
//				double					returns								OUT
//
//	This function calculates the area of an instance.
//	For perfomance reasons it is benefitial to call it with vertex and index buffer when
//	the arrays are calculated anyway or Volume and Area are needed.
//
//	There are two ways to call GetVolume:
//		vertexBuffer and indexBuffer are both zero: in this case the instance will be
//				recalculated when needed. It is expected the client does not
//				need the arrays itself or there is no performance issue.
//		vertexBuffer and indexBuffer are both given: the call is placed directly after
//				updateBuffer calls and no structural change to depending instances have 
//				been done in between. The transformationMatrix array is not needed,
//				even if it is being used due to not giving any performance gain to this
//				operation.
//
//	Note: internally the call does not store its results, any optimization based on known
//		  dependancies between instances need to be implemented on the client.
//	Note: in case precision is important and vertex buffer is 32 bit it is advised to
//		  set vertexBuffer and indexBuffer to 0 even if arrays are existing.
//
double			DECL STDC	GetArea(
									OwlInstance				owlInstance,
									const void				* vertexBuffer,
									const void				* indexBuffer
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	double	GetArea(
								OwlInstance				owlInstance
							)
{
	const void	* vertexBuffer = nullptr,
				* indexBuffer = nullptr;
	return	GetArea(
					owlInstance,
					vertexBuffer,
					indexBuffer
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetVolume                                               (http://rdf.bg/gkdoc/CP64/GetVolume.html)
//				OwlInstance				owlInstance							IN
//				const void				* vertexBuffer						IN
//				const void				* indexBuffer						IN
//
//				double					returns								OUT
//
//	This function calculates the volume of an instance.
//	For perfomance reasons it is benefitial to call it with vertex and index buffer when
//	the arrays are calculated anyway or Volume and Area are needed.
//
//	There are two ways to call GetVolume:
//		vertexBuffer and indexBuffer are both zero: in this case the instance will be
//				recalculated when needed. It is expected the client does not
//				need the arrays itself or there is no performance issue.
//		vertexBuffer and indexBuffer are both given: the call is placed directly after
//				updateBuffer calls and no structural change to depending instances have 
//				been done in between. The transformationMatrix array is not needed,
//				even if it is being used due to not giving any performance gain to this
//				operation.
//
//	Note: internally the call does not store its results, any optimization based on known
//		  dependancies between instances need to be implemented on the client.
//	Note: in case precision is important and vertex buffer is 32 bit it is advised to
//		  set vertexBuffer and indexBuffer to 0 even if arrays are existing.
//
double			DECL STDC	GetVolume(
									OwlInstance				owlInstance,
									const void				* vertexBuffer,
									const void				* indexBuffer
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	double	GetVolume(
								OwlInstance				owlInstance
							)
{
	const void	* vertexBuffer = nullptr,
				* indexBuffer = nullptr;
	return	GetVolume(
					owlInstance,
					vertexBuffer,
					indexBuffer
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetCenter                                               (http://rdf.bg/gkdoc/CP64/GetCenter.html)
//				OwlInstance				owlInstance							IN
//				const void				* vertexBuffer						IN
//				const void				* indexBuffer						IN
//				double					* center							IN / OUT
//
//				void					returns
//
//	This function calculates the center of an instance.
//	For perfomance reasons it is benefitial to call it with vertex and index buffer when
//	the arrays are calculated anyway or Volume and Area are needed.
//
//	There are two ways to call GetCenter:
//		vertexBuffer and indexBuffer are both zero: in this case the instance will be
//				recalculated when needed. It is expected the client does not
//				need the arrays itself or there is no performance issue.
//		vertexBuffer and indexBuffer are both given: the call is placed directly after
//				updateBuffer calls and no structural change to depending instances have 
//				been done in between. The transformationMatrix array is not needed,
//				even if it is being used due to not giving any performance gain to this
//				operation.
//
//	Note: internally the call does not store its results, any optimization based on known
//		  dependancies between instances need to be implemented on the client.
//	Note: in case precision is important and vertex array is 32 bit it is advised to
//		  set vertexBuffer and indexBuffer to 0 even if arrays are existing.
//
void			DECL STDC	GetCenter(
									OwlInstance				owlInstance,
									const void				* vertexBuffer,
									const void				* indexBuffer,
									double					* center
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	void	GetCenter(
								OwlInstance				owlInstance,
								double					* center
							)
{
	const void	* vertexBuffer = nullptr,
				* indexBuffer = nullptr;
	GetCenter(
			owlInstance,
			vertexBuffer,
			indexBuffer,
			center
		);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetCentroid                                             (http://rdf.bg/gkdoc/CP64/GetCentroid.html)
//				OwlInstance				owlInstance							IN
//				const void				* vertexBuffer						IN
//				const void				* indexBuffer						IN
//				double					* centroid							IN / OUT
//
//				double					returns								OUT
//
//
double			DECL STDC	GetCentroid(
									OwlInstance				owlInstance,
									const void				* vertexBuffer,
									const void				* indexBuffer,
									double					* centroid
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	double	GetCentroid(
								OwlInstance				owlInstance,
								double					* centroid
							)
{
	const void	* vertexBuffer = nullptr,
				* indexBuffer = nullptr;
	return	GetCentroid(
					owlInstance,
					vertexBuffer,
					indexBuffer,
					centroid
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetConceptualFacePerimeter                              (http://rdf.bg/gkdoc/CP64/GetConceptualFacePerimeter.html)
//				ConceptualFace			conceptualFace						IN
//
//				double					returns								OUT
//
//	This function returns the perimeter of a given Conceptual Face.
//
double			DECL STDC	GetConceptualFacePerimeter(
									ConceptualFace			conceptualFace
								);

//
//		GetConceptualFaceArea                                   (http://rdf.bg/gkdoc/CP64/GetConceptualFaceArea.html)
//				ConceptualFace			conceptualFace						IN
//				const void				* vertexBuffer						IN
//				const void				* indexBuffer						IN
//
//				double					returns								OUT
//
//	This function returns the area of a given Conceptual Face. The attributes vertex buffer
//	and index buffer are optional but will improve performance if defined.
//
double			DECL STDC	GetConceptualFaceArea(
									ConceptualFace			conceptualFace,
									const void				* vertexBuffer,
									const void				* indexBuffer
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	double	GetConceptualFaceArea(
								ConceptualFace			conceptualFace
							)
{
	const void	* vertexBuffer = nullptr,
				* indexBuffer = nullptr;
	return	GetConceptualFaceArea(
					conceptualFace,
					vertexBuffer,
					indexBuffer
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetBoundingBoxReference                                 (http://rdf.bg/gkdoc/CP64/SetBoundingBoxReference.html)
//				OwlInstance				owlInstance							IN
//				double					* transformationMatrix				IN / OUT
//				double					* startVector						IN / OUT
//				double					* endVector							IN / OUT
//
//				void					returns
//
//	This function passes addresses from the hosting application. This enables
//	the engine to update these values without extra need for API calls. This is
//	especially of interest because the hosting application is not aware of what
//	instances are updated and 
//	The transformationMatrix has 12 double values: _11, _12, _13, _21, _22, _23, 
//	_31, _32, _33, _41, _42, _43.
//	The startVector is the leftundernear vector and the endVector is the 
//	rightupperfar vector, in all cases values are doubles (64 bit).
//
void			DECL STDC	SetBoundingBoxReference(
									OwlInstance				owlInstance,
									double					* transformationMatrix,
									double					* startVector,
									double					* endVector
								);

//
//		GetBoundingBox                                          (http://rdf.bg/gkdoc/CP64/GetBoundingBox.html)
//				OwlInstance				owlInstance							IN
//				double					* transformationMatrix				IN / OUT
//				double					* startVector						IN / OUT
//				double					* endVector							IN / OUT
//
//				bool					returns								OUT
//
//	When the transformationMatrix is given, it will fill an array of 12 double values.
//	When the transformationMatrix is left empty and both startVector and endVector are
//	given the boundingbox without transformation is calculated and returned.
//
bool			DECL STDC	GetBoundingBox(
									OwlInstance				owlInstance,
									double					* transformationMatrix,
									double					* startVector,
									double					* endVector
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	bool	GetBoundingBox(
								OwlInstance				owlInstance,
								double					* startVector,
								double					* endVector
							)
{
	return	GetBoundingBox(
					owlInstance,
					nullptr,							//	transformationMatrix
					startVector,
					endVector
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetRelativeTransformation                               (http://rdf.bg/gkdoc/CP64/GetRelativeTransformation.html)
//				OwlInstance				owlInstanceHead						IN
//				OwlInstance				owlInstanceTail						IN
//				double					* transformationMatrix				IN / OUT
//
//				void					returns
//
//	This function returns the relative transformation matrix between two instances, i.e. in practise
//	this means the matrices connected to the Transformation instances in the path in between.
//	The matrix is only given when a unique path through inverse relations can be found,
//	otherwise the identity matrix is returned.
//	owlInstanceHead is allowed to be not defined, i.e. zero.
//
void			DECL STDC	GetRelativeTransformation(
									OwlInstance				owlInstanceHead,
									OwlInstance				owlInstanceTail,
									double					* transformationMatrix
								);

//
//		GetDistance                                             (http://rdf.bg/gkdoc/CP64/GetDistance.html)
//				OwlInstance				firstOwlInstance					IN
//				OwlInstance				secondOwlInstance					IN
//				double					* pointFirstInstance				IN / OUT
//				double					* pointSecondInstance				IN / OUT
//				bool					allowCalculateInstance				IN
//
//				double					returns								OUT
//
//	This function returns the shortest distance between two instances.
//
double			DECL STDC	GetDistance(
									OwlInstance				firstOwlInstance,
									OwlInstance				secondOwlInstance,
									double					* pointFirstInstance,
									double					* pointSecondInstance,
									bool					allowCalculateInstance
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	double	GetDistance(
								OwlInstance				firstOwlInstance,
								OwlInstance				secondOwlInstance
							)
{
	return	GetDistance(
					firstOwlInstance,
					secondOwlInstance,
					nullptr,							//	pointFirstInstance
					nullptr,							//	pointSecondInstance
					true								//	allowCalculateInstance
				);
}

//
//
static	inline	double	GetDistance(
								OwlInstance				firstOwlInstance,
								OwlInstance				secondOwlInstance,
								double					* pointFirstInstance,
								double					* pointSecondInstance
							)
{
	return	GetDistance(
					firstOwlInstance,
					secondOwlInstance,
					pointFirstInstance,
					pointSecondInstance,
					true								//	allowCalculateInstance
				);
}

//
//
static	inline	uint32_t	GetColorOfComponent(
									OwlInstance				owlInstanceColorComponent
								)
{
	assert(IsInstanceOfClass(owlInstanceColorComponent, "ColorComponent"));

	int64_t	model = GetModel(owlInstanceColorComponent);

	const char	* rgbwNames[4] = { "R", "G", "B", "W" };
	double		rgbwValues[4] = { 0., 0., 0., 0. };

	for (size_t i = 0; i < 4; i++) {
		double	* values = nullptr;
		int64_t	card = 0;
		GetDatatypeProperty(
				owlInstanceColorComponent,
				GetPropertyByName(
						model,
						rgbwNames[i]
					),
				(const void**) &values,
				&card
			);
		assert(card == 1);
		rgbwValues[i] = (card == 1) ? values[0] : 0.;
	}

	return	COLOR_ARR_RGBW(rgbwValues);
}

//
//
static	inline	void	SetColorOfComponent(
								OwlInstance				owlInstanceColorComponent,
								uint32_t				color
							)
{
	assert(IsInstanceOfClass(owlInstanceColorComponent, "ColorComponent"));

	int64_t	model = GetModel(owlInstanceColorComponent);

	const char	* rgbwNames[4] = { "R", "G", "B", "W" };
	double		rgbwValues[4];

	COLOR_GET_COMPONENTS(rgbwValues, color);

	for (size_t i = 0; i < 4; i++) {
		SetDatatypeProperty(
				owlInstanceColorComponent,
				GetPropertyByName(
						model,
						rgbwNames[i]
					),
				&rgbwValues[i],
				1
			);
	}
}

//
//
static	inline	void	GetColor(
								OwlInstance				owlInstanceColor,
								uint32_t				* ambient,
								uint32_t				* diffuse,
								uint32_t				* emissive,
								uint32_t				* specular
							)
{
	assert(IsInstanceOfClass(owlInstanceColor, "Color"));

	int64_t	model = GetModel(owlInstanceColor);

	GetDefaultColor(
			model,
			ambient,
			diffuse,
			emissive,
			specular
		);

	const char	* componentNames[4] = { "ambient", "diffuse", "emissive", "specular" };
	uint32_t	* componentColors[4] = { ambient, diffuse, emissive, specular };

	for (size_t i = 0; i < 4; i++) {
		if (componentColors[i]) {
			OwlInstance	* values = nullptr;
			int64_t		card = 0;
			GetObjectProperty(
					owlInstanceColor,
					GetPropertyByName(
							model,
							componentNames[i]
						),
					(const OwlInstance**) &values,
					&card
				);

			int64_t owlInstanceColorComponent = (card == 1) ? values[0] : 0;
			if (owlInstanceColorComponent) {
				(*componentColors[i]) = GetColorOfComponent(owlInstanceColorComponent);
			}
		}
	}
}

//
//
static	inline	void	SetColor(
								OwlInstance				owlInstanceColor,
								uint32_t				ambient,
								uint32_t				diffuse,
								uint32_t				emissive,
								uint32_t				specular
							)
{
	assert(IsInstanceOfClass(owlInstanceColor, "Color"));

	{
		OwlInstance	* values = nullptr;
		int64_t		card = 0;
		GetObjectProperty(
				owlInstanceColor,
				GetPropertyByName(
						GetModel(owlInstanceColor),
						"ambient"
					),
				&values,
				&card
			);

		SetColorOfComponent(
				(card == 1) ?
					values[0] :
					CreateInstance(
							GetClassByName(
									GetModel(owlInstanceColor),
									"ColorComponent"
								)
						),
				ambient
			);
	}

	{
		OwlInstance	* values = nullptr;
		int64_t		card = 0;
		GetObjectProperty(
				owlInstanceColor,
				GetPropertyByName(
						GetModel(owlInstanceColor),
						"diffuse"
					),
				&values,
				&card
			);

		SetColorOfComponent(
				(card == 1) ?
					values[0] :
					CreateInstance(
							GetClassByName(
									GetModel(owlInstanceColor),
									"ColorComponent"
								)
						),
				diffuse
			);
	}

	{
		OwlInstance	* values = nullptr;
		int64_t		card = 0;
		GetObjectProperty(
				owlInstanceColor,
				GetPropertyByName(
						GetModel(owlInstanceColor),
						"emissive"
					),
				&values,
				&card
			);

		SetColorOfComponent(
				(card == 1) ?
					values[0] :
					CreateInstance(
							GetClassByName(
									GetModel(owlInstanceColor),
									"ColorComponent"
								)
						),
				emissive
			);
	}

	{
		OwlInstance	* values = nullptr;
		int64_t		card = 0;
		GetObjectProperty(
				owlInstanceColor,
				GetPropertyByName(
						GetModel(owlInstanceColor),
						"specular"
					),
				&values,
				&card
			);

		SetColorOfComponent(
				(card == 1) ?
					values[0] :
					CreateInstance(
							GetClassByName(
									GetModel(owlInstanceColor),
									"ColorComponent"
								)
						),
				specular
			);
	}
}

//
//
static	inline	void	GetMaterialColor(
								OwlInstance				owlInstanceMaterial,
								uint32_t				* ambient,
								uint32_t				* diffuse,
								uint32_t				* emissive,
								uint32_t				* specular
							)
{
	assert(IsInstanceOfClass(owlInstanceMaterial, "Material"));

	OwlInstance	* values = nullptr;
	int64_t		card = 0;

	GetObjectProperty(
			owlInstanceMaterial,
			GetPropertyByName(
					GetModel(owlInstanceMaterial),
					"color"
				),
			&values,
			&card
		);

	OwlInstance	owlInstanceColor = (card == 1) ? values[0] : 0;
	if (owlInstanceColor) {
		GetColor(
				owlInstanceColor,
				ambient,
				diffuse,
				emissive,
				specular
			);
	}
	else {
		GetDefaultColor(
				GetModel(owlInstanceMaterial),
				ambient,
				diffuse,
				emissive,
				specular
			);
	}
}


//
//
static	inline	void	SetMaterialColor(
								OwlInstance				owlInstanceMaterial,
								uint32_t				ambient,
								uint32_t				diffuse,
								uint32_t				emissive,
								uint32_t				specular
							)
{
	assert(IsInstanceOfClass(owlInstanceMaterial, "Material"));

	int64_t	* values = nullptr, card = 0;
	GetObjectProperty(
			owlInstanceMaterial,
			GetPropertyByName(
					GetModel(owlInstanceMaterial),
					"color"
				),
			&values,
			&card
		);

	SetColor(
			(card == 1) ?
				values[0] :
				CreateInstance(
						GetClassByName(
								GetModel(owlInstanceMaterial),
								"color"
							)
					),
			ambient,
			diffuse,
			emissive,
			specular
		);
}

//
//
static	inline	uint32_t	GetMaterialColorAmbient(
									OwlInstance				owlInstanceMaterial
								)
{
	uint32_t	ambient = 0;
	GetMaterialColor(
			owlInstanceMaterial,
			&ambient,
			nullptr,							//	diffuse
			nullptr,							//	emissive
			nullptr								//	specular
		);
	return	ambient;
}

//
//
static	inline	uint32_t	GetMaterialColorDiffuse(
									OwlInstance				owlInstanceMaterial
								)
{
	uint32_t	diffuse = 0;
	GetMaterialColor(
			owlInstanceMaterial,
			nullptr,							//	ambient
			&diffuse,
			nullptr,							//	emissive
			nullptr								//	specular
		);
	return	diffuse;
}

//
//
static	inline	uint32_t	GetMaterialColorEmissive(
									OwlInstance				owlInstanceMaterial
								)
{
	uint32_t	emissive = 0;
	GetMaterialColor(
			owlInstanceMaterial,
			nullptr,							//	ambient
			nullptr,							//	diffuse
			&emissive,
			nullptr								//	specular
		);
	return	emissive;
}

//
//
static	inline	uint32_t	GetMaterialColorSpecular(
									OwlInstance				owlInstanceMaterial
								)
{
	uint32_t	specular = 0;
	GetMaterialColor(
			owlInstanceMaterial,
			nullptr,							//	ambient
			nullptr,							//	diffuse
			nullptr,							//	emissive
			&specular
		);
	return	specular;
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetVertexColor                                          (http://rdf.bg/gkdoc/CP64/GetVertexColor.html)
//				OwlModel				model								IN
//				const void				* vertexBuffer						IN
//				int64_t					vertexIndex							IN
//				int64_t					setting								IN
//				uint32_t				* ambient							IN / OUT
//				uint32_t				* diffuse							IN / OUT
//				uint32_t				* emissive							IN / OUT
//				uint32_t				* specular							IN / OUT
//
//				void					returns
//
//	Returns vertex color
//	requiredColor is one of the control vertex data bits applied to colors (FORMAT_VERTEX_COLOR...) 
//	If vertex format does provide required color, the model default color will be used
//
void			DECL STDC	GetVertexColor(
									OwlModel				model,
									const void				* vertexBuffer,
									int64_t					vertexIndex,
									int64_t					setting,
									uint32_t				* ambient,
									uint32_t				* diffuse,
									uint32_t				* emissive,
									uint32_t				* specular
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	uint32_t	GetVertexColorAmbient(
									OwlModel				model,
									const void				* vertexBuffer,
									int64_t					vertexIndex,
									int64_t					setting
								)
{
	uint32_t	ambient = 0;
	GetVertexColor(
			model,
			vertexBuffer,
			vertexIndex,
			setting,
			&ambient,
			nullptr,							//	diffuse
			nullptr,							//	emissive
			nullptr								//	specular
		);
	return	ambient;
}

//
//
static	inline	uint32_t	GetVertexColorDiffuse(
									OwlModel				model,
									const void				* vertexBuffer,
									int64_t					vertexIndex,
									int64_t					setting
								)
{
	uint32_t	diffuse = 0;
	GetVertexColor(
			model,
			vertexBuffer,
			vertexIndex,
			setting,
			nullptr,							//	ambient
			&diffuse,
			nullptr,							//	emissive
			nullptr								//	specular
		);
	return	diffuse;
}

//
//
static	inline	uint32_t	GetVertexColorEmissive(
									OwlModel				model,
									const void				* vertexBuffer,
									int64_t					vertexIndex,
									int64_t					setting
								)
{
	uint32_t	emissive = 0;
	GetVertexColor(
			model,
			vertexBuffer,
			vertexIndex,
			setting,
			nullptr,							//	ambient
			nullptr,							//	diffuse
			&emissive,
			nullptr								//	specular
		);
	return	emissive;
}

//
//
static	inline	uint32_t	GetVertexColorSpecular(
									OwlModel				model,
									const void				* vertexBuffer,
									int64_t					vertexIndex,
									int64_t					setting
								)
{
	uint32_t	specular = 0;
	GetVertexColor(
			model,
			vertexBuffer,
			vertexIndex,
			setting,
			nullptr,							//	ambient
			nullptr,							//	diffuse
			nullptr,							//	emissive
			&specular
		);
	return	specular;
}

//
//  Deprecated API Calls
//

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetConceptualFaceEx                                     (http://rdf.bg/gkdoc/CP64/GetConceptualFaceEx___.html)
//				OwlInstance				owlInstance							IN
//				int64_t					index								IN
//				int64_t					* startIndexTriangles				IN / OUT
//				int64_t					* noIndicesTriangles				IN / OUT
//				int64_t					* startIndexLines					IN / OUT
//				int64_t					* noIndicesLines					IN / OUT
//				int64_t					* startIndexPoints					IN / OUT
//				int64_t					* noIndicesPoints					IN / OUT
//				int64_t					* startIndexFacePolygons			IN / OUT
//				int64_t					* noIndicesFacePolygons				IN / OUT
//				int64_t					* startIndexConceptualFacePolygons	IN / OUT
//				int64_t					* noIndicesConceptualFacePolygons	IN / OUT
//
//				ConceptualFace			returns								OUT
//
//	Please rename GetConceptualFaceEx into GetConceptualFace.
//
ConceptualFace	DECL STDC	GetConceptualFaceEx(
									OwlInstance				owlInstance,
									int64_t					index,
									int64_t					* startIndexTriangles,
									int64_t					* noIndicesTriangles,
									int64_t					* startIndexLines,
									int64_t					* noIndicesLines,
									int64_t					* startIndexPoints,
									int64_t					* noIndicesPoints,
									int64_t					* startIndexFacePolygons,
									int64_t					* noIndicesFacePolygons,
									int64_t					* startIndexConceptualFacePolygons,
									int64_t					* noIndicesConceptualFacePolygons
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	ConceptualFace	GetConceptualFaceEx(
										OwlInstance				owlInstance,
										int64_t					index,
										int64_t					* startIndexTriangles,
										int64_t					* noIndicesTriangles,
										int64_t					* startIndexLines,
										int64_t					* noIndicesLines,
										int64_t					* startIndexPoints,
										int64_t					* noIndicesPoints
									)
{
	return	GetConceptualFaceEx(
					owlInstance,
					index,
					startIndexTriangles,
					noIndicesTriangles,
					startIndexLines,
					noIndicesLines,
					startIndexPoints,
					noIndicesPoints,
					nullptr,							//	startIndexFacePolygons
					nullptr,							//	noIndicesFacePolygons
					nullptr,							//	startIndexConceptualFacePolygons
					nullptr								//	noIndicesConceptualFacePolygons
				);
}

//
//
static	inline	ConceptualFace	GetConceptualFaceEx(
										OwlInstance				owlInstance,
										int64_t					index
									)
{
	return	GetConceptualFaceEx(
					owlInstance,
					index,
					nullptr,							//	startIndexTriangles
					nullptr,							//	noIndicesTriangles
					nullptr,							//	startIndexLines
					nullptr,							//	noIndicesLines
					nullptr,							//	startIndexPoints
					nullptr								//	noIndicesPoints
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetTriangles                                            (http://rdf.bg/gkdoc/CP64/GetTriangles___.html)
//				OwlInstance				owlInstance							IN
//				int64_t					* startIndex						IN / OUT
//				int64_t					* noTriangles						IN / OUT
//				int64_t					* startVertex						IN / OUT
//				int64_t					* firstNotUsedVertex				IN / OUT
//
//				void					returns
//
//	This call is deprecated as it became trivial and will be removed by end of 2022. The result from CalculateInstance exclusively exists of the relevant triangles when
//	SetFormat() is setting bit 8 and unsetting with bit 9, 10, 12 and 13 
//
void			DECL STDC	GetTriangles(
									OwlInstance				owlInstance,
									int64_t					* startIndex,
									int64_t					* noTriangles,
									int64_t					* startVertex,
									int64_t					* firstNotUsedVertex
								);

//
//		GetLines                                                (http://rdf.bg/gkdoc/CP64/GetLines___.html)
//				OwlInstance				owlInstance							IN
//				int64_t					* startIndex						IN / OUT
//				int64_t					* noLines							IN / OUT
//				int64_t					* startVertex						IN / OUT
//				int64_t					* firstNotUsedVertex				IN / OUT
//
//				void					returns
//
//	This call is deprecated as it became trivial and will be removed by end of 2022. The result from CalculateInstance exclusively exists of the relevant lines when
//	SetFormat() is setting bit 9 and unsetting with bit 8, 10, 12 and 13 
//
void			DECL STDC	GetLines(
									OwlInstance				owlInstance,
									int64_t					* startIndex,
									int64_t					* noLines,
									int64_t					* startVertex,
									int64_t					* firstNotUsedVertex
								);

//
//		GetPoints                                               (http://rdf.bg/gkdoc/CP64/GetPoints___.html)
//				OwlInstance				owlInstance							IN
//				int64_t					* startIndex						IN / OUT
//				int64_t					* noPoints							IN / OUT
//				int64_t					* startVertex						IN / OUT
//				int64_t					* firstNotUsedVertex				IN / OUT
//
//				void					returns
//
//	This call is deprecated as it became trivial and will be removed by end of 2022. The result from CalculateInstance exclusively exists of the relevant points when
//	SetFormat() is setting bit 10 and unsetting with bit 8, 9, 12 and 13 
//
void			DECL STDC	GetPoints(
									OwlInstance				owlInstance,
									int64_t					* startIndex,
									int64_t					* noPoints,
									int64_t					* startVertex,
									int64_t					* firstNotUsedVertex
								);

//
//		GetPropertyRestrictionsConsolidated                     (http://rdf.bg/gkdoc/CP64/GetPropertyRestrictionsConsolidated___.html)
//				OwlClass				owlClass							IN
//				RdfProperty				rdfProperty							IN
//				int64_t					* minCard							IN / OUT
//				int64_t					* maxCard							IN / OUT
//
//				void					returns
//
//	This call is deprecated and will be removed by end of 2022. Please use the call GetClassPropertyAggregatedCardinalityRestriction instead,
//
void			DECL STDC	GetPropertyRestrictionsConsolidated(
									OwlClass				owlClass,
									RdfProperty				rdfProperty,
									int64_t					* minCard,
									int64_t					* maxCard
								);

//
//		IsGeometryType                                          (http://rdf.bg/gkdoc/CP64/IsGeometryType___.html)
//				OwlClass				owlClass							IN
//
//				bool					returns								OUT
//
//	This call is deprecated and will be removed by end of 2022. Please use the call GetGeometryClass instead, rename the function name
//	and interpret non-zero as true and zero as false.
//
bool			DECL STDC	IsGeometryType(
									OwlClass				owlClass
								);

//
//		SetObjectTypeProperty                                   (http://rdf.bg/gkdoc/CP64/SetObjectTypeProperty___.html)
//				OwlInstance				owlInstance							IN
//				OwlObjectProperty		owlObjectProperty					IN
//				const OwlInstance		* values							IN
//				int64_t					card								IN
//
//				int64_t					returns								OUT
//
//	This call is deprecated and will be removed by end of 2022. Please use the call SetObjectProperty instead, just rename the function name.
//
int64_t			DECL STDC	SetObjectTypeProperty(
									OwlInstance				owlInstance,
									OwlObjectProperty		owlObjectProperty,
									const OwlInstance		* values,
									int64_t					card
								);

//
//		GetObjectTypeProperty                                   (http://rdf.bg/gkdoc/CP64/GetObjectTypeProperty___.html)
//				OwlInstance				owlInstance							IN
//				OwlObjectProperty		owlObjectProperty					IN
//				const OwlInstance		** values							IN / OUT
//				int64_t					* card								IN / OUT
//
//				int64_t					returns								OUT
//
//	This call is deprecated and will be removed by end of 2022. Please use the call GetObjectProperty instead, just rename the function name.
//
int64_t			DECL STDC	GetObjectTypeProperty(
									OwlInstance				owlInstance,
									OwlObjectProperty		owlObjectProperty,
									const OwlInstance		** values,
									int64_t					* card
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	GetObjectTypeProperty(
								OwlInstance				owlInstance,
								OwlObjectProperty		owlObjectProperty,
								OwlInstance				** values,
								int64_t					* card
							)
{
	return	GetObjectTypeProperty(
					owlInstance,
					owlObjectProperty,
					(const OwlInstance**) values,
					card
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		SetDataTypeProperty                                     (http://rdf.bg/gkdoc/CP64/SetDataTypeProperty___.html)
//				OwlInstance				owlInstance							IN
//				OwlDatatypeProperty		owlDatatypeProperty					IN
//				const void				* values							IN
//				int64_t					card								IN
//
//				int64_t					returns								OUT
//
//	This call is deprecated and will be removed by end of 2022. Please use the call SetDatatypeProperty instead, just rename the function name.
//
int64_t			DECL STDC	SetDataTypeProperty(
									OwlInstance				owlInstance,
									OwlDatatypeProperty		owlDatatypeProperty,
									const void				* values,
									int64_t					card
								);

//
//		GetDataTypeProperty                                     (http://rdf.bg/gkdoc/CP64/GetDataTypeProperty___.html)
//				OwlInstance				owlInstance							IN
//				OwlDatatypeProperty		owlDatatypeProperty					IN
//				const void				** values							IN / OUT
//				int64_t					* card								IN / OUT
//
//				int64_t					returns								OUT
//
//	This call is deprecated and will be removed by end of 2022. Please use the call GetDatatypeProperty instead, just rename the function name.
//
int64_t			DECL STDC	GetDataTypeProperty(
									OwlInstance				owlInstance,
									OwlDatatypeProperty		owlDatatypeProperty,
									const void				** values,
									int64_t					* card
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	GetDataTypeProperty(
								OwlInstance				owlInstance,
								OwlDatatypeProperty		owlDatatypeProperty,
								void					** values,
								int64_t					* card
							)
{
	return	GetDataTypeProperty(
					owlInstance,
					owlDatatypeProperty,
					(const void**) values,
					card
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		InstanceCopyCreated                                     (http://rdf.bg/gkdoc/CP64/InstanceCopyCreated___.html)
//				OwlInstance				owlInstance							IN
//
//				void					returns
//
//	This call is deprecated as the Copy concept is also deprecated and will be removed by end of 2022.
//
void			DECL STDC	InstanceCopyCreated(
									OwlInstance				owlInstance
								);

//
//		GetPropertyByNameAndType                                (http://rdf.bg/gkdoc/CP64/GetPropertyByNameAndType___.html)
//				OwlModel				model								IN
//				const char				* name								IN
//				int64_t					rdfPropertyType						IN
//
//				int64_t					returns								OUT
//
//	This call is deprecated and will be removed by end of 2022.
//	Please use the call GetPropertyByName(Ex) / GetPropertyByNameW(Ex) + GetPropertyType(Ex) instead, just rename the function name.
//
int64_t			DECL STDC	GetPropertyByNameAndType(
									OwlModel				model,
									const char				* name,
									int64_t					rdfPropertyType
								);

#ifdef __cplusplus
	}
#endif

//
//
static	inline	int64_t	GetPropertyByNameAndType(
								OwlModel				model,
								char					* name,
								int64_t					rdfPropertyType
							)
{
	return	GetPropertyByNameAndType(
					model,
					(const char*) name,
					rdfPropertyType
				);
}

#ifdef __cplusplus
	extern "C" {
#endif

//
//		GetParentsByIterator                                    (http://rdf.bg/gkdoc/CP64/GetParentsByIterator___.html)
//				int64_t					owlClassOrRdfProperty				IN
//				int64_t					parentOwlClassOrRdfProperty			IN
//
//				int64_t					returns								OUT
//
//	Returns the next parent of the class or property.
//	If input parent is zero, the handle will point to the first relevant parent.
//	If all parent are past (or no relevant parent are found), the function will return 0.
//
int64_t			DECL STDC	GetParentsByIterator(
									int64_t					owlClassOrRdfProperty,
									int64_t					parentOwlClassOrRdfProperty
								);


#ifdef __cplusplus
	}
#endif


#undef DECL
#undef STDC
#endif
