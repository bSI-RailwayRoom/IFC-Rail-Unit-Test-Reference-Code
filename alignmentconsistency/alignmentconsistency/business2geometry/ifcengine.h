//
//  Author:  Peter Bonsma
//  $Date: 1999-12-31 23:59:59 +0000 (Wed, 31 Jan 1999) $
//  $Revision: 3999 $
//  Project: IFC Engine Library
//
//  Be aware of license fee for use of this library when used commercially
//  More info for commercial use:  peter.bonsma@rdf.bg / contact@rdf.bg
//

#ifndef __RDF_LTD__IFCENGINE_H
#define __RDF_LTD__IFCENGINE_H


#include	"engdef.h"

#include	"assert.h"

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


#define		sdaiADB					1
#define		sdaiAGGR				sdaiADB + 1
#define		sdaiBINARY				sdaiAGGR + 1
#define		sdaiBOOLEAN				sdaiBINARY + 1
#define		sdaiENUM				sdaiBOOLEAN + 1
#define		sdaiINSTANCE			sdaiENUM + 1
#define		sdaiINTEGER				sdaiINSTANCE + 1
#define		sdaiLOGICAL				sdaiINTEGER + 1
#define		sdaiREAL				sdaiLOGICAL + 1
#define		sdaiSTRING				sdaiREAL + 1
#define		sdaiUNICODE				sdaiSTRING + 1
#define		sdaiEXPRESSSTRING		sdaiUNICODE + 1
#define		engiGLOBALID			sdaiEXPRESSSTRING + 1
#define		sdaiNUMBER				engiGLOBALID + 1

#define		engiTypeFlagAggr		128
#define		engiTypeFlagAggrOption	32


typedef void(*LOGCB)(const char *);

/*	Note on sdaiSTRING and sdaiUNICODE


	sdaiUNICODE
		this will convert all internal strings from/too unicode, the internal representation and what is written to the IFC file is mapped
			"\" will be converted into "\\" to enable basic parses to still interpret file paths
			"'" will be converted to \X1\hh\X0\ or \X2\00hh\X0\ to prevent basic interpreters to read files with strings containing these characters

	sdaiSTRING
		this will leave all information as is in the IFC file, the rules are that char's ( int ) 32 to 126 (inclusive) will be kept
		all other strings will be converted to \X1\hh\X0\ or \X2\00hh\X0\		*/


#define		sdaiARRAY				1
#define		sdaiLIST				2
#define		sdaiSET					3
#define		sdaiBAG					4

typedef	int_t			* SdaiAggr;
typedef	void			* SdaiAttr;
typedef	int_t			SdaiEntity;
typedef	int_t			SdaiInstance;
typedef	int_t			SdaiInteger;
typedef	int_t			SdaiModel;
typedef	int_t			SdaiPrimitiveType;
typedef	int_t			* SdaiSet;
typedef	char			* SdaiString;
typedef	void			* SdaiADB;
typedef	int_t			SdaiNPL;


enum class enum_express_declaration : unsigned char
{
	__NONE = 0,
	__ENTITY,
	__ENUM,
	__SELECT,
	__DEFINED_TYPE
};


enum class enum_express_attr_type : unsigned char
{
	__NONE = 0, //attribute type is unknown here but it may be defined by referenced domain entity
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


enum class enum_express_aggr : unsigned char
{
	__NONE = 0,
	__ARRAY,
	__BAG,
	__LIST,
	__SET
};

typedef void		* ValidationResults;
typedef void		* ValidationIssue;
typedef int_t		ValidationIssueLevel;

enum class enum_validation_type : uint64_t
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

enum class enum_validation_status : unsigned char
{
	__NONE = 0,
	__COMPLETE_ALL,		//all issues proceed
	__COMPLETE_NOT_ALL, //completed but some issues were excluded by option settings
	__TIME_EXCEED,		//validation was finished because of reach time limit
	__COUNT_EXCEED	    //validation was finished because of reach of issue's numbers limit
};



//
//  File IO API Calls
//

#ifdef __cplusplus
	extern "C" {
#endif

//
//		sdaiCreateModelBN                                       (http://rdf.bg/ifcdoc/CP64/sdaiCreateModelBN.html)
//				int_t					repository							IN
//				const char				* schemaName						IN
//
//				SdaiModel				returns								OUT
//
//	This function creates and empty model (we expect with a schema file given).
//	Attributes repository and fileName will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
SdaiModel		DECL STDC	sdaiCreateModelBN(
									int_t					repository,
									const char				* fileName,
									const char				* schemaName
								);

//
//		sdaiCreateModelBNUnicode                                (http://rdf.bg/ifcdoc/CP64/sdaiCreateModelBNUnicode.html)
//				int_t					repository							IN
//				const wchar_t			* schemaName						IN
//
//				SdaiModel				returns								OUT
//
//	This function creates and empty model (we expect with a schema file given).
//	Attributes repository and fileName will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
SdaiModel		DECL STDC	sdaiCreateModelBNUnicode(
									int_t					repository,
									const wchar_t			* fileName,
									const wchar_t			* schemaName
								);

//
//		sdaiOpenModelBN                                         (http://rdf.bg/ifcdoc/CP64/sdaiOpenModelBN.html)
//				int_t					repository							IN
//				const char				* fileName							IN
//				const char				* schemaName						IN
//
//				SdaiModel				returns								OUT
//
//	This function opens the model on location fileName.
//	Attribute repository will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
SdaiModel		DECL STDC	sdaiOpenModelBN(
									int_t					repository,
									const char				* fileName,
									const char				* schemaName
								);

//
//		sdaiOpenModelBNUnicode                                  (http://rdf.bg/ifcdoc/CP64/sdaiOpenModelBNUnicode.html)
//				int_t					repository							IN
//				const wchar_t			* fileName							IN
//				const wchar_t			* schemaName						IN
//
//				SdaiModel				returns								OUT
//
//	This function opens the model on location fileName.
//	Attribute repository will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
SdaiModel		DECL STDC	sdaiOpenModelBNUnicode(
									int_t					repository,
									const wchar_t			* fileName,
									const wchar_t			* schemaName
								);

//
//		engiOpenModelByStream                                   (http://rdf.bg/ifcdoc/CP64/engiOpenModelByStream.html)
//				int_t					repository							IN
//				const void				* callback							IN
//				const char				* schemaName						IN
//
//				SdaiModel				returns								OUT
//
//	This function opens the model via a stream.
//	Attribute repository will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
SdaiModel		DECL STDC	engiOpenModelByStream(
									int_t					repository,
									const void				* callback,
									const char				* schemaName
								);

//
//		engiOpenModelByArray                                    (http://rdf.bg/ifcdoc/CP64/engiOpenModelByArray.html)
//				int_t					repository							IN
//				const unsigned char		* content							IN
//				int_t					size								IN
//				const char				* schemaName						IN
//
//				SdaiModel				returns								OUT
//
//	This function opens the model via an array.
//	Attribute repository will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
SdaiModel		DECL STDC	engiOpenModelByArray(
									int_t					repository,
									const unsigned char		* content,
									int_t					size,
									const char				* schemaName
								);

//
//		sdaiSaveModelBN                                         (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelBN.html)
//				SdaiModel				model								IN
//				const char				* fileName							IN
//
//				void					returns
//
//	This function saves the model (char file name).
//
void			DECL STDC	sdaiSaveModelBN(
									SdaiModel				model,
									const char				* fileName
								);

//
//		sdaiSaveModelBNUnicode                                  (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelBNUnicode.html)
//				SdaiModel				model								IN
//				const wchar_t			* fileName							IN
//
//				void					returns
//
//	This function saves the model (wchar, i.e. Unicode file name).
//
void			DECL STDC	sdaiSaveModelBNUnicode(
									SdaiModel				model,
									const wchar_t			* fileName
								);

//
//		engiSaveModelByStream                                   (http://rdf.bg/ifcdoc/CP64/engiSaveModelByStream.html)
//				SdaiModel				model								IN
//				const void				* callback							IN
//				int_t					size								IN
//
//				void					returns
//
//	This function saves the model as an array.
//
void			DECL STDC	engiSaveModelByStream(
									SdaiModel				model,
									const void				* callback,
									int_t					size
								);

//
//		engiSaveModelByArray                                    (http://rdf.bg/ifcdoc/CP64/engiSaveModelByArray.html)
//				SdaiModel				model								IN
//				unsigned char			* content							IN / OUT
//				int_t					* size								IN / OUT
//
//				void					returns
//
//	This function saves the model as an array.
//
void			DECL STDC	engiSaveModelByArray(
									SdaiModel				model,
									unsigned char			* content,
									int_t					* size
								);

//
//		sdaiSaveModelAsXmlBN                                    (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsXmlBN.html)
//				SdaiModel				model								IN
//				const char				* fileName							IN
//
//				void					returns
//
//	This function saves the model as XML according to IFC2x3's way of XML serialization (char file name).
//
void			DECL STDC	sdaiSaveModelAsXmlBN(
									SdaiModel				model,
									const char				* fileName
								);

//
//		sdaiSaveModelAsXmlBNUnicode                             (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsXmlBNUnicode.html)
//				SdaiModel				model								IN
//				const wchar_t			* fileName							IN
//
//				void					returns
//
//	This function saves the model as XML according to IFC2x3's way of XML serialization (wchar, i.e. Unicode file name).
//
void			DECL STDC	sdaiSaveModelAsXmlBNUnicode(
									SdaiModel				model,
									const wchar_t			* fileName
								);

//
//		sdaiSaveModelAsSimpleXmlBN                              (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsSimpleXmlBN.html)
//				SdaiModel				model								IN
//				const char				* fileName							IN
//
//				void					returns
//
//	This function saves the model as XML according to IFC4's way of XML serialization (char file name).
//
void			DECL STDC	sdaiSaveModelAsSimpleXmlBN(
									SdaiModel				model,
									const char				* fileName
								);

//
//		sdaiSaveModelAsSimpleXmlBNUnicode                       (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsSimpleXmlBNUnicode.html)
//				SdaiModel				model								IN
//				const wchar_t			* fileName							IN
//
//				void					returns
//
//	This function saves the model as XML according to IFC4's way of XML serialization (wchar, i.e. Unicode file name).
//
void			DECL STDC	sdaiSaveModelAsSimpleXmlBNUnicode(
									SdaiModel				model,
									const wchar_t			* fileName
								);

//
//		sdaiSaveModelAsJsonBN                                   (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsJsonBN.html)
//				SdaiModel				model								IN
//				const char				* fileName							IN
//
//				void					returns
//
//	This function saves the model as JSON according to IFC4's way of JSON serialization (char file name).
//
void			DECL STDC	sdaiSaveModelAsJsonBN(
									SdaiModel				model,
									const char				* fileName
								);

//
//		sdaiSaveModelAsJsonBNUnicode                            (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsJsonBNUnicode.html)
//				SdaiModel				model								IN
//				const wchar_t			* fileName							IN
//
//				void					returns
//
//	This function saves the model as JSON according to IFC4's way of JSON serialization (wchar, i.e. Unicode file name).
//
void			DECL STDC	sdaiSaveModelAsJsonBNUnicode(
									SdaiModel				model,
									const wchar_t			* fileName
								);

//
//		engiSaveSchemaBN                                        (http://rdf.bg/ifcdoc/CP64/engiSaveSchemaBN.html)
//				SdaiModel				model								IN
//				const char				* filePath							IN
//
//				bool					returns								OUT
//
//	This function saves the schema.
//
bool			DECL STDC	engiSaveSchemaBN(
									SdaiModel				model,
									const char				* filePath
								);

//
//		engiSaveSchemaBNUnicode                                 (http://rdf.bg/ifcdoc/CP64/engiSaveSchemaBNUnicode.html)
//				SdaiModel				model								IN
//				const wchar_t			* filePath							IN
//
//				bool					returns								OUT
//
//	This function saves the schema (wchar, i.e. Unicode file name).
//
bool			DECL STDC	engiSaveSchemaBNUnicode(
									SdaiModel				model,
									const wchar_t			* filePath
								);

//
//		sdaiCloseModel                                          (http://rdf.bg/ifcdoc/CP64/sdaiCloseModel.html)
//				SdaiModel				model								IN
//
//				void					returns
//
//	This function closes the model. After this call no instance handles will be available including all
//	handles referencing the geometry of this specific file, in default compilation the model itself will
//	be known in the kernel, however known to be disabled. Calls containing the model reference will be
//	protected from crashing when called.
//
void			DECL STDC	sdaiCloseModel(
									SdaiModel				model
								);

//
//		setPrecisionDoubleExport                                (http://rdf.bg/ifcdoc/CP64/setPrecisionDoubleExport.html)
//				SdaiModel				model								IN
//				int_t					precisionCap						IN
//				int_t					precisionRound						IN
//				bool					clean								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	setPrecisionDoubleExport(
									SdaiModel				model,
									int_t					precisionCap,
									int_t					precisionRound,
									bool					clean
								);

//
//  Schema Reading API Calls
//

//
//		engiGetNextDeclarationIterator                          (http://rdf.bg/ifcdoc/CP64/engiGetNextDeclarationIterator.html)
//				SdaiModel				model								IN
//				int_t					iterator							IN
//
//				int_t					returns								OUT
//
//	This call returns next iterator of EXPRESS schema declarations.
//	If the input iterator is NULL it returns first iterator.
//	If the input iterator is last it returns NULL.
//	Use engiGetDeclarationFromIterator to access EXPRESS declaration data. 
//
int_t			DECL STDC	engiGetNextDeclarationIterator(
									SdaiModel				model,
									int_t					iterator
								);

//
//		engiGetDeclarationFromIterator                          (http://rdf.bg/ifcdoc/CP64/engiGetDeclarationFromIterator.html)
//				SdaiModel				model								IN
//				int_t					iterator							IN
//
//				int_t					returns								OUT
//
//	This call returns handle to the EXPRESS schema declarations from iterator.
//	It may be a handle to entity, or enumeration, select or type definition, use engiGetDeclarationType to recognize
//	Use engiGetNextDeclarationIterator to get iterator.
//
int_t			DECL STDC	engiGetDeclarationFromIterator(
									SdaiModel				model,
									int_t					iterator
								);

//
//		engiGetDeclarationType                                  (http://rdf.bg/ifcdoc/CP64/engiGetDeclarationType.html)
//				int_t					declaration							IN
//
//				enum_express_declaration	returns								OUT
//
//	This call returns a type of the EXPRESS schema declarations from its handle.
//
enum_express_declaration	DECL STDC	engiGetDeclarationType(
									int_t					declaration
								);

//
//		engiGetEnumerationElement                               (http://rdf.bg/ifcdoc/CP64/engiGetEnumerationElement.html)
//				int_t					enumeration							IN
//				int_t					index								IN
//
//				char					* returns							OUT
//
//	This call returns a name of the enumaration element with the given index (zero based)
//	It returns NULL if the index out of range
//
const char		DECL * STDC	engiGetEnumerationElement(
									int_t					enumeration,
									int_t					index
								);

//
//		engiGetSelectElement                                    (http://rdf.bg/ifcdoc/CP64/engiGetSelectElement.html)
//				int_t					select								IN
//				int_t					index								IN
//
//				int_t					returns								OUT
//
//	This call returns a declaration handle of the select element with the given index (zero based)
//	It returns 0 if the index out of range
//
int_t			DECL STDC	engiGetSelectElement(
									int_t					select,
									int_t					index
								);

//
//		engiGetDefinedType                                      (http://rdf.bg/ifcdoc/CP64/engiGetDefinedType.html)
//				int_t					definedType							IN
//				int_t					* referencedDeclaration				IN / OUT
//				int_t					* aggregationDescriptor				IN / OUT
//
//				enum_express_attr_type	returns								OUT
//
//	This call returns a simple type for defined type handle and can inquire referenced type, if any
//
enum_express_attr_type	DECL STDC	engiGetDefinedType(
									int_t					definedType,
									int_t					* referencedDeclaration,
									int_t					* aggregationDescriptor
								);

//
//		sdaiGetEntity                                           (http://rdf.bg/ifcdoc/CP64/sdaiGetEntity.html)
//				SdaiModel				model								IN
//				const char				* entityName						IN
//
//				SdaiEntity				returns								OUT
//
//	This call retrieves a handle to an entity based on a given entity name.
//
SdaiEntity		DECL STDC	sdaiGetEntity(
									SdaiModel				model,
									const char				* entityName
								);

//
//		engiGetEntityModel                                      (http://rdf.bg/ifcdoc/CP64/engiGetEntityModel.html)
//				SdaiEntity				entity								IN
//
//				SdaiModel				returns								OUT
//
//	This call retrieves a model based on a given entity handle.
//
SdaiModel		DECL STDC	engiGetEntityModel(
									SdaiEntity				entity
								);

//
//		engiGetEntityArgument                                   (http://rdf.bg/ifcdoc/CP64/engiGetEntityArgument.html)
//				SdaiEntity				entity								IN
//				const char				* argumentName						IN
//
//				SdaiAttr				returns								OUT
//
//	...
//
SdaiAttr		DECL STDC	engiGetEntityArgument(
									SdaiEntity				entity,
									const char				* argumentName
								);

//
//		engiGetEntityAttributeIndex                             (http://rdf.bg/ifcdoc/CP64/engiGetEntityAttributeIndex.html)
//				SdaiEntity				entity								IN
//				const char				* attributeName						IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	engiGetEntityAttributeIndex(
									SdaiEntity				entity,
									const char				* attributeName
								);

//
//		engiGetEntityAttributeIndexEx                           (http://rdf.bg/ifcdoc/CP64/engiGetEntityAttributeIndexEx.html)
//				SdaiEntity				entity								IN
//				const char				* attributeName						IN
//				bool					countedWithParents					IN
//				bool					countedWithInverse					IN
//
//				int_t					returns								OUT
//
//	..
//
int_t			DECL STDC	engiGetEntityAttributeIndexEx(
									SdaiEntity				entity,
									const char				* attributeName,
									bool					countedWithParents,
									bool					countedWithInverse
								);

//
//		engiGetEntityArgumentIndex                              (http://rdf.bg/ifcdoc/CP64/engiGetEntityArgumentIndex.html)
//				SdaiEntity				entity								IN
//				const char				* argumentName						IN
//
//				int_t					returns								OUT
//
//	DEPRECATED use engiGetEntityAttributeIndex
//
int_t			DECL STDC	engiGetEntityArgumentIndex(
									SdaiEntity				entity,
									const char				* argumentName
								);

//
//		engiGetEntityArgumentName                               (http://rdf.bg/ifcdoc/CP64/engiGetEntityArgumentName.html)
//				SdaiEntity				entity								IN
//				int_t					index								IN
//				int_t					valueType							IN
//				const char				** argumentName						IN / OUT
//
//				void					returns
//
//	This call can be used to retrieve the name of the n-th argument of the given entity. Arguments of parent entities are included in the index. Both direct and inverse arguments are included.
//
void			DECL STDC	engiGetEntityArgumentName(
									SdaiEntity				entity,
									int_t					index,
									int_t					valueType,
									const char				** argumentName
								);

//
//		engiGetEntityArgumentType                               (http://rdf.bg/ifcdoc/CP64/engiGetEntityArgumentType.html)
//				SdaiEntity				entity								IN
//				int_t					index								IN
//				int_t					* argumentType						IN / OUT
//
//				void					returns
//
//	This call can be used to retrieve the type of the n-th argument of the given entity. In case of a select argument no relevant information is given by this call as it depends on the instance. Arguments of parent entities are included in the index. Both direct and inverse arguments are included.
//
void			DECL STDC	engiGetEntityArgumentType(
									SdaiEntity				entity,
									int_t					index,
									int_t					* argumentType
								);

//
//		engiGetEntityCount                                      (http://rdf.bg/ifcdoc/CP64/engiGetEntityCount.html)
//				SdaiModel				model								IN
//
//				SdaiInteger				returns								OUT
//
//	Returns the total number of entities within the loaded schema.
//
SdaiInteger		DECL STDC	engiGetEntityCount(
									SdaiModel				model
								);

//
//		engiGetEntityElement                                    (http://rdf.bg/ifcdoc/CP64/engiGetEntityElement.html)
//				SdaiModel				model								IN
//				SdaiInteger				index								IN
//
//				SdaiEntity				returns								OUT
//
//	This call returns a specific entity based on an index, the index needs to be 0 or higher but lower then the number of entities in the loaded schema.
//
SdaiEntity		DECL STDC	engiGetEntityElement(
									SdaiModel				model,
									SdaiInteger				index
								);

//
//		sdaiGetEntityExtent                                     (http://rdf.bg/ifcdoc/CP64/sdaiGetEntityExtent.html)
//				SdaiModel				model								IN
//				SdaiEntity				entity								IN
//
//				SdaiAggr				returns								OUT
//
//	This call retrieves an aggregation that contains all instances of the entity given.
//
SdaiAggr		DECL STDC	sdaiGetEntityExtent(
									SdaiModel				model,
									SdaiEntity				entity
								);

//
//		sdaiGetEntityExtentBN                                   (http://rdf.bg/ifcdoc/CP64/sdaiGetEntityExtentBN.html)
//				SdaiModel				model								IN
//				const char				* entityName						IN
//
//				SdaiAggr				returns								OUT
//
//	This call retrieves an aggregation that contains all instances of the entity given.
//
SdaiAggr		DECL STDC	sdaiGetEntityExtentBN(
									SdaiModel				model,
									const char				* entityName
								);

//
//		engiGetEntityName                                       (http://rdf.bg/ifcdoc/CP64/engiGetEntityName.html)
//				SdaiEntity				entity								IN
//				int_t					valueType							IN
//				const char				** entityName						IN / OUT
//
//				void					returns
//
//	This call can be used to get the name of the given entity.
//
void			DECL STDC	engiGetEntityName(
									SdaiEntity				entity,
									int_t					valueType,
									const char				** entityName
								);

//
//		engiGetEntityNoAttributes                               (http://rdf.bg/ifcdoc/CP64/engiGetEntityNoAttributes.html)
//				SdaiEntity				entity								IN
//
//				int_t					returns								OUT
//
//	This call returns the number of arguments, this includes the arguments of its (nested) parents and inverse argumnets.
//
int_t			DECL STDC	engiGetEntityNoAttributes(
									SdaiEntity				entity
								);

//
//		engiGetEntityNoAttributesEx                             (http://rdf.bg/ifcdoc/CP64/engiGetEntityNoAttributesEx.html)
//				SdaiEntity				entity								IN
//				bool					includeParent						IN
//				bool					includeInverse						IN
//
//				int_t					returns								OUT
//
//	This call returns the number of attributes, inclusion of parents and inverse depeds on includeParent and includeInverse values
//
int_t			DECL STDC	engiGetEntityNoAttributesEx(
									SdaiEntity				entity,
									bool					includeParent,
									bool					includeInverse
								);

//
//		engiGetEntityNoArguments                                (http://rdf.bg/ifcdoc/CP64/engiGetEntityNoArguments.html)
//				SdaiEntity				entity								IN
//
//				int_t					returns								OUT
//
//	DEPRECATED use engiGetEntityNoAttributes
//
int_t			DECL STDC	engiGetEntityNoArguments(
									SdaiEntity				entity
								);

//
//		engiGetAttributeType                                    (http://rdf.bg/ifcdoc/CP64/engiGetAttributeType.html)
//				SdaiAttr				attribute							IN
//
//				int_t					returns								OUT
//
//	DEPRECATED use engiGetAttrType
//
int_t			DECL STDC	engiGetAttributeType(
									SdaiAttr				attribute
								);

//
//		engiGetArgumentType                                     (http://rdf.bg/ifcdoc/CP64/engiGetArgumentType.html)
//				SdaiAttr				argument							IN
//
//				int_t					returns								OUT
//
//	DEPR4ECATED use engiGetAttributeType
//
int_t			DECL STDC	engiGetArgumentType(
									SdaiAttr				argument
								);

//
//		engiGetEntityParent                                     (http://rdf.bg/ifcdoc/CP64/engiGetEntityParent.html)
//				SdaiEntity				entity								IN
//
//				SdaiEntity				returns								OUT
//
//	Returns the first direct parent entity, for example the parent of IfcObject is IfcObjectDefinition, of IfcObjectDefinition is IfcRoot and of IfcRoot is 0.
//
SdaiEntity		DECL STDC	engiGetEntityParent(
									SdaiEntity				entity
								);

//
//		engiGetEntityNoParents                                  (http://rdf.bg/ifcdoc/CP64/engiGetEntityNoParents.html)
//				SdaiEntity				entity								IN
//
//				int_t					returns								OUT
//
//	Returns number of direct parents entity
//
int_t			DECL STDC	engiGetEntityNoParents(
									SdaiEntity				entity
								);

//
//		engiGetEntityParentEx                                   (http://rdf.bg/ifcdoc/CP64/engiGetEntityParentEx.html)
//				SdaiEntity				entity								IN
//				int_t					index								IN
//
//				SdaiEntity				returns								OUT
//
//	Returns the N-th direct parent of entity or NULL if index exceeds number of parents
//
SdaiEntity		DECL STDC	engiGetEntityParentEx(
									SdaiEntity				entity,
									int_t					index
								);

//
//		engiGetAttrOptional                                     (http://rdf.bg/ifcdoc/CP64/engiGetAttrOptional.html)
//				SdaiAttr				attribute							IN
//
//				int_t					returns								OUT
//
//	This call can be used to check if an attribute is optional
//
int_t			DECL STDC	engiGetAttrOptional(
									SdaiAttr				attribute
								);

//
//		engiGetAttrOptionalBN                                   (http://rdf.bg/ifcdoc/CP64/engiGetAttrOptionalBN.html)
//				SdaiEntity				entity								IN
//				const char				* attributeName						IN
//
//				int_t					returns								OUT
//
//	This call can be used to check if an attribute is optional
//
int_t			DECL STDC	engiGetAttrOptionalBN(
									SdaiEntity				entity,
									const char				* attributeName
								);

//
//		engiGetEntityIsAbstract                                 (http://rdf.bg/ifcdoc/CP64/engiGetEntityIsAbstract.html)
//				SdaiEntity				entity								IN
//
//				int_t					returns								OUT
//
//	This call can be used to check if an entity is abstract
//
int_t			DECL STDC	engiGetEntityIsAbstract(
									SdaiEntity				entity
								);

//
//		engiGetEntityIsAbstractBN                               (http://rdf.bg/ifcdoc/CP64/engiGetEntityIsAbstractBN.html)
//				SdaiModel				model								IN
//				const char				* entityName						IN
//
//				int_t					returns								OUT
//
//	This call can be used to check if an entity is abstract
//
int_t			DECL STDC	engiGetEntityIsAbstractBN(
									SdaiModel				model,
									const char				* entityName
								);

//
//		engiGetAttrInverse                                      (http://rdf.bg/ifcdoc/CP64/engiGetAttrInverse.html)
//				SdaiAttr				attribute							IN
//
//				int_t					returns								OUT
//
//	This call can be used to check if an attribute is an inverse relation
//
int_t			DECL STDC	engiGetAttrInverse(
									SdaiAttr				attribute
								);

//
//		engiGetAttrInverseBN                                    (http://rdf.bg/ifcdoc/CP64/engiGetAttrInverseBN.html)
//				SdaiEntity				entity								IN
//				const char				* attributeName						IN
//
//				int_t					returns								OUT
//
//	This call can be used to check if an attribute is an inverse relation
//
int_t			DECL STDC	engiGetAttrInverseBN(
									SdaiEntity				entity,
									const char				* attributeName
								);

int_t			DECL STDC	engiGetAttrDerived(
									SdaiEntity				entity,
									SdaiAttr				attribute
								);

int_t			DECL STDC	engiGetAttrDerivedBN(
									SdaiEntity				entity,
									const char				* attributeName
								);

//
//		engiGetAttrDomain                                       (http://rdf.bg/ifcdoc/CP64/engiGetAttrDomain.html)
//				SdaiAttr				attribute							IN
//				char					** domainName						IN / OUT
//
//				void					returns
//
//	This call can be used to get the domain of an attribute
//
void			DECL STDC	engiGetAttrDomain(
									SdaiAttr				attribute,
									char					** domainName
								);

//
//		engiGetAttrDomainBN                                     (http://rdf.bg/ifcdoc/CP64/engiGetAttrDomainBN.html)
//				SdaiEntity				entity								IN
//				const char				* attributeName						IN
//				char					** domainName						IN / OUT
//
//				void					returns
//
//	This call can be used to get the domain of an attribute
//
void			DECL STDC	engiGetAttrDomainBN(
									SdaiEntity				entity,
									const char				* attributeName,
									char					** domainName
								);

//
//		engiGetEnumerationValue                                 (http://rdf.bg/ifcdoc/CP64/engiGetEnumerationValue.html)
//				SdaiAttr				attribute							IN
//				int_t					index								IN
//				int_t					valueType							IN
//				char					** enumerationValue					IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetEnumerationValue(
									SdaiAttr				attribute,
									int_t					index,
									int_t					valueType,
									char					** enumerationValue
								);

//
//		engiGetEntityAttributeByIndex                           (http://rdf.bg/ifcdoc/CP64/engiGetEntityAttributeByIndex.html)
//				SdaiEntity				entity								IN
//				int_t					index								IN
//				bool					countedWithParents					IN
//				bool					countedWithInverse					IN
//
//				SdaiAttr				returns								OUT
//
//	Return attribute definition from attribute index
//
SdaiAttr		DECL STDC	engiGetEntityAttributeByIndex(
									SdaiEntity				entity,
									int_t					index,
									bool					countedWithParents,
									bool					countedWithInverse
								);

//
//		engiGetAttributeTraits                                  (http://rdf.bg/ifcdoc/CP64/engiGetAttributeTraits.html)
//				SdaiAttr				attribute							IN
//				const char				** name								IN / OUT
//				SdaiEntity				* definingEntity					IN / OUT
//				bool					* inverse							IN / OUT
//				enum_express_attr_type	* attrType							IN / OUT
//				SdaiEntity				* domainEntity						IN / OUT
//				int_t					* aggregationDescriptor				IN / OUT
//				bool					* optional							IN / OUT
//				bool					* unique							IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetAttributeTraits(
									SdaiAttr				attribute,
									const char				** name,
									SdaiEntity				* definingEntity,
									bool					* inverse,
									enum_express_attr_type	* attrType,
									SdaiEntity				* domainEntity,
									int_t					* aggregationDescriptor,
									bool					* optional,
									bool					* unique
								);

//
//		engiGetAggregation                                      (http://rdf.bg/ifcdoc/CP64/engiGetAggregation.html)
//				int_t					aggregationDescriptor				IN
//				enum_express_aggr		* aggrType							IN / OUT
//				int_t					* cardinalityMin					IN / OUT
//				int_t					* cardinalityMax					IN / OUT
//				int_t					* nextAggregationLevelDescriptor	IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetAggregation(
									int_t					aggregationDescriptor,
									enum_express_aggr		* aggrType,
									int_t					* cardinalityMin,
									int_t					* cardinalityMax,
									int_t					* nextAggregationLevelDescriptor
								);

//
//  Instance Header API Calls
//

//
//		SetSPFFHeader                                           (http://rdf.bg/ifcdoc/CP64/SetSPFFHeader.html)
//				SdaiModel				model								IN
//				const char				* description						IN
//				const char				* implementationLevel				IN
//				const char				* name								IN
//				const char				* timeStamp							IN
//				const char				* author							IN
//				const char				* organization						IN
//				const char				* preprocessorVersion				IN
//				const char				* originatingSystem					IN
//				const char				* authorization						IN
//				const char				* fileSchema						IN
//
//				void					returns
//
//	This call is an aggregate of several SetSPFFHeaderItem calls. In several cases the header can be set easily with this call. In case an argument is zero, this argument will not be updated, i.e. it will not be filled with 0.
//
void			DECL STDC	SetSPFFHeader(
									SdaiModel				model,
									const char				* description,
									const char				* implementationLevel,
									const char				* name,
									const char				* timeStamp,
									const char				* author,
									const char				* organization,
									const char				* preprocessorVersion,
									const char				* originatingSystem,
									const char				* authorization,
									const char				* fileSchema
								);

//
//		SetSPFFHeaderItem                                       (http://rdf.bg/ifcdoc/CP64/SetSPFFHeaderItem.html)
//				SdaiModel				model								IN
//				int_t					itemIndex							IN
//				int_t					itemSubIndex						IN
//				int_t					valueType							IN
//				const char				* value								IN
//
//				int_t					returns								OUT
//
//	This call can be used to write a specific header item, the source code example is larger to show and explain how this call can be used.
//
int_t			DECL STDC	SetSPFFHeaderItem(
									SdaiModel				model,
									int_t					itemIndex,
									int_t					itemSubIndex,
									int_t					valueType,
									const char				* value
								);

//
//		GetSPFFHeaderItem                                       (http://rdf.bg/ifcdoc/CP64/GetSPFFHeaderItem.html)
//				SdaiModel				model								IN
//				int_t					itemIndex							IN
//				int_t					itemSubIndex						IN
//				int_t					valueType							IN
//				char					** value							IN / OUT
//
//				int_t					returns								OUT
//
//	This call can be used to read a specific header item, the source code example is larger to show and explain how this call can be used.
//
int_t			DECL STDC	GetSPFFHeaderItem(
									SdaiModel				model,
									int_t					itemIndex,
									int_t					itemSubIndex,
									int_t					valueType,
									char					** value
								);

//
//		GetSPFFHeaderItemUnicode                                (http://rdf.bg/ifcdoc/CP64/GetSPFFHeaderItemUnicode.html)
//				SdaiModel				model								IN
//				int_t					itemIndex							IN
//				int_t					itemSubIndex						IN
//				unsigned char			* buffer							IN / OUT
//				int_t					bufferLength						IN
//
//				int_t					returns								OUT
//
//	This call can be used to write a specific header item, the source code example is larger to show and explain how this call can be used.
//
int_t			DECL STDC	GetSPFFHeaderItemUnicode(
									SdaiModel				model,
									int_t					itemIndex,
									int_t					itemSubIndex,
									unsigned char			* buffer,
									int_t					bufferLength
								);

//
//  Instance Reading API Calls
//

//
//		sdaiGetADBType                                          (http://rdf.bg/ifcdoc/CP64/sdaiGetADBType.html)
//				SdaiADB					ADB									IN
//
//				int_t					returns								OUT
//
//	This call can be used to get the used type within this ADB type.
//
int_t			DECL STDC	sdaiGetADBType(
									SdaiADB					ADB
								);

//
//		sdaiGetADBTypePath                                      (http://rdf.bg/ifcdoc/CP64/sdaiGetADBTypePath.html)
//				SdaiADB					ADB									IN
//				int_t					typeNameNumber						IN
//
//				char					* returns							OUT
//
//	This call can be used to get the path of an ADB type.
//
char			DECL * STDC	sdaiGetADBTypePath(
									SdaiADB					ADB,
									int_t					typeNameNumber
								);

//
//		sdaiGetADBTypePathx                                     (http://rdf.bg/ifcdoc/CP64/sdaiGetADBTypePathx.html)
//				SdaiADB					ADB									IN
//				int_t					typeNameNumber						IN
//				char					** path								IN / OUT
//
//				void					returns
//
//	This call is the same as sdaiGetADBTypePath, however can be used by porting to languages that have issues with returned char arrays.
//
void			DECL STDC	sdaiGetADBTypePathx(
									SdaiADB					ADB,
									int_t					typeNameNumber,
									char					** path
								);

//
//		sdaiGetADBValue                                         (http://rdf.bg/ifcdoc/CP64/sdaiGetADBValue.html)
//				SdaiADB					ADB									IN
//				int_t					valueType							IN
//				void					* value								IN / OUT
//
//				void					* returns							OUT
//
//	...
//
void			DECL * STDC	sdaiGetADBValue(
									SdaiADB					ADB,
									int_t					valueType,
									void					* value
								);

//
//		sdaiCreateEmptyADB                                      (http://rdf.bg/ifcdoc/CP64/sdaiCreateEmptyADB.html)
//				void					* returns							OUT
//
//	...
//
void			DECL * STDC	sdaiCreateEmptyADB(
								);

//
//		sdaiDeleteADB                                           (http://rdf.bg/ifcdoc/CP64/sdaiDeleteADB.html)
//				SdaiADB					ADB									IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiDeleteADB(
									SdaiADB					ADB
								);

//
//		engiGetAggrElement                                      (http://rdf.bg/ifcdoc/CP64/engiGetAggrElement.html)
//				SdaiAggr				aggregate							IN
//				int_t					index								IN
//				int_t					valueType							IN
//				void					* value								IN / OUT
//
//				void					* returns							OUT
//
//	...
//
void			DECL * STDC	engiGetAggrElement(
									SdaiAggr				aggregate,
									int_t					index,
									int_t					valueType,
									void					* value
								);

//
//		sdaiGetAggrByIndex                                      (http://rdf.bg/ifcdoc/CP64/sdaiGetAggrByIndex.html)
//				SdaiAggr				aggregate							IN
//				int_t					index								IN
//				int_t					valueType							IN
//				void					* value								IN / OUT
//
//				void					* returns							OUT
//
//	...
//
void			DECL * STDC	sdaiGetAggrByIndex(
									SdaiAggr				aggregate,
									int_t					index,
									int_t					valueType,
									void					* value
								);

//
//		engiGetAggrType                                         (http://rdf.bg/ifcdoc/CP64/engiGetAggrType.html)
//				SdaiAggr				aggregate							IN
//				int_t					* aggregateType						IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetAggrType(
									SdaiAggr				aggregate,
									int_t					* aggregateType
								);

//
//		engiGetAggrTypex                                        (http://rdf.bg/ifcdoc/CP64/engiGetAggrTypex.html)
//				SdaiAggr				aggregate							IN
//				int_t					* aggregateType						IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetAggrTypex(
									SdaiAggr				aggregate,
									int_t					* aggregateType
								);

//
//		sdaiGetAttr                                             (http://rdf.bg/ifcdoc/CP64/sdaiGetAttr.html)
//				SdaiInstance			instance							IN
//				SdaiAttr				attribute							IN
//				int_t					valueType							IN
//				void					* value								IN / OUT
//
//				void					* returns							OUT
//
//	...
//
void			DECL * STDC	sdaiGetAttr(
									SdaiInstance			instance,
									SdaiAttr				attribute,
									int_t					valueType,
									void					* value
								);

//
//		sdaiGetAttrBN                                           (http://rdf.bg/ifcdoc/CP64/sdaiGetAttrBN.html)
//				SdaiInstance			instance							IN
//				const char				* attributeName						IN
//				int_t					valueType							IN
//				void					* value								IN / OUT
//
//				void					* returns							OUT
//
//	...
//
void			DECL * STDC	sdaiGetAttrBN(
									SdaiInstance			instance,
									const char				* attributeName,
									int_t					valueType,
									void					* value
								);

//
//		sdaiGetAttrBNUnicode                                    (http://rdf.bg/ifcdoc/CP64/sdaiGetAttrBNUnicode.html)
//				SdaiInstance			instance							IN
//				const char				* attributeName						IN
//				unsigned char			* buffer							IN / OUT
//				int_t					bufferLength						IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiGetAttrBNUnicode(
									SdaiInstance			instance,
									const char				* attributeName,
									unsigned char			* buffer,
									int_t					bufferLength
								);

//
//		sdaiGetStringAttrBN                                     (http://rdf.bg/ifcdoc/CP64/sdaiGetStringAttrBN.html)
//				SdaiInstance			instance							IN
//				const char				* attributeName						IN
//
//				char					* returns							OUT
//
//	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiSTRING.
//	This call can be usefull in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
//	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
//
char			DECL * STDC	sdaiGetStringAttrBN(
									SdaiInstance			instance,
									const char				* attributeName
								);

//
//		sdaiGetInstanceAttrBN                                   (http://rdf.bg/ifcdoc/CP64/sdaiGetInstanceAttrBN.html)
//				SdaiInstance			instance							IN
//				const char				* attributeName						IN
//
//				SdaiInstance			returns								OUT
//
//	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiINSTANCE.
//	This call can be usefull in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
//	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
//
SdaiInstance	DECL STDC	sdaiGetInstanceAttrBN(
									SdaiInstance			instance,
									const char				* attributeName
								);

//
//		sdaiGetAggregationAttrBN                                (http://rdf.bg/ifcdoc/CP64/sdaiGetAggregationAttrBN.html)
//				SdaiInstance			instance							IN
//				const char				* attributeName						IN
//
//				SdaiAggr				returns								OUT
//
//	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiAGGR.
//	This call can be usefull in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
//	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
//
SdaiAggr		DECL STDC	sdaiGetAggregationAttrBN(
									SdaiInstance			instance,
									const char				* attributeName
								);

//
//		sdaiGetAttrDefinition                                   (http://rdf.bg/ifcdoc/CP64/sdaiGetAttrDefinition.html)
//				SdaiEntity				entity								IN
//				const char				* attributeName						IN
//
//				SdaiAttr				returns								OUT
//
//	...
//
SdaiAttr		DECL STDC	sdaiGetAttrDefinition(
									SdaiEntity				entity,
									const char				* attributeName
								);

//
//		sdaiGetInstanceType                                     (http://rdf.bg/ifcdoc/CP64/sdaiGetInstanceType.html)
//				SdaiInstance			instance							IN
//
//				SdaiEntity				returns								OUT
//
//	...
//
SdaiEntity		DECL STDC	sdaiGetInstanceType(
									SdaiInstance			instance
								);

//
//		sdaiGetMemberCount                                      (http://rdf.bg/ifcdoc/CP64/sdaiGetMemberCount.html)
//				SdaiAggr				aggregate							IN
//
//				SdaiInteger				returns								OUT
//
//	...
//
SdaiInteger		DECL STDC	sdaiGetMemberCount(
									SdaiAggr				aggregate
								);

//
//		sdaiIsKindOf                                            (http://rdf.bg/ifcdoc/CP64/sdaiIsKindOf.html)
//				SdaiInstance			instance							IN
//				SdaiEntity				entity								IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiIsKindOf(
									SdaiInstance			instance,
									SdaiEntity				entity
								);

//
//		sdaiIsKindOfBN                                          (http://rdf.bg/ifcdoc/CP64/sdaiIsKindOfBN.html)
//				SdaiInstance			instance							IN
//				const char				* entityName						IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiIsKindOfBN(
									SdaiInstance			instance,
									const char				* entityName
								);

//
//		engiGetAttrType                                         (http://rdf.bg/ifcdoc/CP64/engiGetAttrType.html)
//				SdaiAttr				attribute							IN
//
//				int_t					returns								OUT
//
//	Returns primitive SDAI data type for the attribute according to schema, e.g. sdaiINTEGER
//
//	In case of aggregation if will return base primitive type combined with engiTypeFlagAggr, e.g. sdaiINTEGER|engiTypeFlagAggr
//
//	For SELECT it will return sdaiINSTANCE if all options are instances or aggegation of instances, either sdaiADB
//	In case of SELECT and sdaiINSTANCE, return value will be combined with engiTypeFlagAggrOption if some options are aggregation
//	or engiTypeFlagAggr if all options are aggregations of instances
//
//	It works for direct and inverse attributes
//
int_t			DECL STDC	engiGetAttrType(
									SdaiAttr				attribute
								);

//
//		engiGetAttrTypeBN                                       (http://rdf.bg/ifcdoc/CP64/engiGetAttrTypeBN.html)
//				SdaiEntity				entity								IN
//				const char				* attributeName						IN
//
//				int_t					returns								OUT
//
//	Combines sdaiGetAttrDefinition and engiGetAttrType
//
int_t			DECL STDC	engiGetAttrTypeBN(
									SdaiEntity				entity,
									const char				* attributeName
								);

//
//		engiGetInstanceAttrType                                 (http://rdf.bg/ifcdoc/CP64/engiGetInstanceAttrType.html)
//				SdaiInstance			instance							IN
//				SdaiAttr				attribute							IN
//
//				int_t					returns								OUT
//
//	Returns SDAI type for actual data stored in the instance for the attribute
//	It may be primitive type, sdaiAGGR or sdaiADB
//	Returns 0 for $ and * 
//
int_t			DECL STDC	engiGetInstanceAttrType(
									SdaiInstance			instance,
									SdaiAttr				attribute
								);

//
//		engiGetInstanceAttrTypeBN                               (http://rdf.bg/ifcdoc/CP64/engiGetInstanceAttrTypeBN.html)
//				SdaiInstance			instance							IN
//				const char				* attributeName						IN
//
//				int_t					returns								OUT
//
//	Combines sdaiGetAttrDefinition and engiGetInstanceAttrType
//
int_t			DECL STDC	engiGetInstanceAttrTypeBN(
									SdaiInstance			instance,
									const char				* attributeName
								);

//
//		sdaiIsInstanceOf                                        (http://rdf.bg/ifcdoc/CP64/sdaiIsInstanceOf.html)
//				SdaiInstance			instance							IN
//				SdaiEntity				entity								IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiIsInstanceOf(
									SdaiInstance			instance,
									SdaiEntity				entity
								);

//
//		sdaiIsInstanceOfBN                                      (http://rdf.bg/ifcdoc/CP64/sdaiIsInstanceOfBN.html)
//				SdaiInstance			instance							IN
//				const char				* entityName						IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiIsInstanceOfBN(
									SdaiInstance			instance,
									const char				* entityName
								);

//
//		sdaiIsEqual                                             (http://rdf.bg/ifcdoc/CP64/sdaiIsEqual.html)
//				SdaiInstance			instanceI							IN
//				SdaiInstance			instanceII							IN
//
//				bool					returns								OUT
//
//	...
//
bool			DECL STDC	sdaiIsEqual(
									SdaiInstance			instanceI,
									SdaiInstance			instanceII
								);

//
//		sdaiValidateAttribute                                   (http://rdf.bg/ifcdoc/CP64/sdaiValidateAttribute.html)
//				SdaiInstance			instance							IN
//				SdaiAttr				attribute							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiValidateAttribute(
									SdaiInstance			instance,
									SdaiAttr				attribute
								);

//
//		sdaiValidateAttributeBN                                 (http://rdf.bg/ifcdoc/CP64/sdaiValidateAttributeBN.html)
//				SdaiInstance			instance							IN
//				const char				* attributeName						IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiValidateAttributeBN(
									SdaiInstance			instance,
									const char				* attributeName
								);

//
//		engiGetInstanceClassInfo                                (http://rdf.bg/ifcdoc/CP64/engiGetInstanceClassInfo.html)
//				SdaiInstance			instance							IN
//
//				char					* returns							OUT
//
//	...
//
char			DECL * STDC	engiGetInstanceClassInfo(
									SdaiInstance			instance
								);

//
//		engiGetInstanceClassInfoUC                              (http://rdf.bg/ifcdoc/CP64/engiGetInstanceClassInfoUC.html)
//				SdaiInstance			instance							IN
//
//				char					* returns							OUT
//
//	...
//
char			DECL * STDC	engiGetInstanceClassInfoUC(
									SdaiInstance			instance
								);

//
//		engiGetInstanceMetaInfo                                 (http://rdf.bg/ifcdoc/CP64/engiGetInstanceMetaInfo.html)
//				SdaiInstance			instance							IN
//				int_t					* localId							IN / OUT
//				char					** entityName						IN / OUT
//				char					** entityNameUC						IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	engiGetInstanceMetaInfo(
									SdaiInstance			instance,
									int_t					* localId,
									char					** entityName,
									char					** entityNameUC
								);

//
//		sdaiFindInstanceUsers                                   (http://rdf.bg/ifcdoc/CP64/sdaiFindInstanceUsers.html)
//				SdaiInstance			instance							IN
//				int_t					domain								IN
//				int_t					resultList							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiFindInstanceUsers(
									SdaiInstance			instance,
									int_t					domain,
									int_t					resultList
								);

//
//		sdaiFindInstanceUsedInBN                                (http://rdf.bg/ifcdoc/CP64/sdaiFindInstanceUsedInBN.html)
//				SdaiInstance			instance							IN
//				const char				* roleName							IN
//				int_t					domain								IN
//				int_t					resultList							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiFindInstanceUsedInBN(
									SdaiInstance			instance,
									const char				* roleName,
									int_t					domain,
									int_t					resultList
								);

//
//  Instance Writing API Calls
//

//
//		sdaiPrepend                                             (http://rdf.bg/ifcdoc/CP64/sdaiPrepend.html)
//				int_t					list								IN
//				int_t					valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiPrepend(
									int_t					list,
									int_t					valueType,
									const void				* value
								);

//
//		sdaiAppend                                              (http://rdf.bg/ifcdoc/CP64/sdaiAppend.html)
//				int_t					list								IN
//				int_t					valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiAppend(
									int_t					list,
									int_t					valueType,
									const void				* value
								);

//
//		sdaiAdd                                                 (http://rdf.bg/ifcdoc/CP64/sdaiAdd.html)
//				int_t					list								IN
//				int_t					valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiAdd(
									int_t					list,
									int_t					valueType,
									const void				* value
								);

//
//		sdaiInsertByIndex                                       (http://rdf.bg/ifcdoc/CP64/sdaiInsertByIndex.html)
//				int_t					list								IN
//				int_t					index								IN
//				int_t					valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiInsertByIndex(
									int_t					list,
									int_t					index,
									int_t					valueType,
									const void				* value
								);

//
//		sdaiCreateADB                                           (http://rdf.bg/ifcdoc/CP64/sdaiCreateADB.html)
//				int_t					valueType							IN
//				const void				* value								IN
//
//				SdaiADB					returns								OUT
//
//	...
//
SdaiADB			DECL STDC	sdaiCreateADB(
									int_t					valueType,
									const void				* value
								);

//
//		sdaiCreateAggr                                          (http://rdf.bg/ifcdoc/CP64/sdaiCreateAggr.html)
//				SdaiInstance			instance							IN
//				SdaiAttr				attribute							IN
//
//				SdaiAggr				returns								OUT
//
//	...
//
SdaiAggr		DECL STDC	sdaiCreateAggr(
									SdaiInstance			instance,
									SdaiAttr				attribute
								);

//
//		sdaiCreateAggrBN                                        (http://rdf.bg/ifcdoc/CP64/sdaiCreateAggrBN.html)
//				SdaiInstance			instance							IN
//				const char				* attributeName						IN
//
//				SdaiAggr				returns								OUT
//
//	...
//
SdaiAggr		DECL STDC	sdaiCreateAggrBN(
									SdaiInstance			instance,
									const char				* attributeName
								);

//
//		sdaiCreateNPL                                           (http://rdf.bg/ifcdoc/CP64/sdaiCreateNPL.html)
//				SdaiNPL					returns								OUT
//
//	...
//
SdaiNPL			DECL STDC	sdaiCreateNPL(
								);

//
//		sdaiDeleteNPL                                           (http://rdf.bg/ifcdoc/CP64/sdaiDeleteNPL.html)
//				SdaiNPL					list								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiDeleteNPL(
									SdaiNPL					list
								);

//
//		sdaiCreateNestedAggr                                    (http://rdf.bg/ifcdoc/CP64/sdaiCreateNestedAggr.html)
//				SdaiAggr				aggregate							IN
//
//				SdaiAggr				returns								OUT
//
//	...
//
SdaiAggr		DECL STDC	sdaiCreateNestedAggr(
									SdaiAggr				aggregate
								);

//
//		sdaiCreateInstance                                      (http://rdf.bg/ifcdoc/CP64/sdaiCreateInstance.html)
//				SdaiModel				model								IN
//				SdaiEntity				entity								IN
//
//				SdaiInstance			returns								OUT
//
//	...
//
SdaiInstance	DECL STDC	sdaiCreateInstance(
									SdaiModel				model,
									SdaiEntity				entity
								);

//
//		sdaiCreateInstanceBN                                    (http://rdf.bg/ifcdoc/CP64/sdaiCreateInstanceBN.html)
//				SdaiModel				model								IN
//				const char				* entityName						IN
//
//				SdaiInstance			returns								OUT
//
//	...
//
SdaiInstance	DECL STDC	sdaiCreateInstanceBN(
									SdaiModel				model,
									const char				* entityName
								);

//
//		sdaiDeleteInstance                                      (http://rdf.bg/ifcdoc/CP64/sdaiDeleteInstance.html)
//				SdaiInstance			instance							IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiDeleteInstance(
									SdaiInstance			instance
								);

//
//		sdaiPutADBTypePath                                      (http://rdf.bg/ifcdoc/CP64/sdaiPutADBTypePath.html)
//				SdaiADB					ADB									IN
//				int_t					pathCount							IN
//				const char				* path								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiPutADBTypePath(
									SdaiADB					ADB,
									int_t					pathCount,
									const char				* path
								);

//
//		sdaiPutAttr                                             (http://rdf.bg/ifcdoc/CP64/sdaiPutAttr.html)
//				SdaiInstance			instance							IN
//				SdaiAttr				attribute							IN
//				int_t					valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiPutAttr(
									SdaiInstance			instance,
									SdaiAttr				attribute,
									int_t					valueType,
									const void				* value
								);

//
//		sdaiPutAttrBN                                           (http://rdf.bg/ifcdoc/CP64/sdaiPutAttrBN.html)
//				SdaiInstance			instance							IN
//				const char				* attributeName						IN
//				int_t					valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiPutAttrBN(
									SdaiInstance			instance,
									const char				* attributeName,
									int_t					valueType,
									const void				* value
								);

//
//		sdaiUnsetAttr                                           (http://rdf.bg/ifcdoc/CP64/sdaiUnsetAttr.html)
//				SdaiInstance			instance							IN
//				SdaiAttr				attribute							IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiUnsetAttr(
									SdaiInstance			instance,
									SdaiAttr				attribute
								);

//
//		sdaiUnsetAttrBN                                         (http://rdf.bg/ifcdoc/CP64/sdaiUnsetAttrBN.html)
//				SdaiInstance			instance							IN
//				const char				* attributeName						IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiUnsetAttrBN(
									SdaiInstance			instance,
									const char				* attributeName
								);

//
//		engiSetComment                                          (http://rdf.bg/ifcdoc/CP64/engiSetComment.html)
//				SdaiInstance			instance							IN
//				const char				* comment							IN
//
//				void					returns
//
//	This call can be used to add a comment to an instance when exporting the content. The comment is available in the exported/saved IFC file.
//
void			DECL STDC	engiSetComment(
									SdaiInstance			instance,
									const char				* comment
								);

//
//		engiGetInstanceLocalId                                  (http://rdf.bg/ifcdoc/CP64/engiGetInstanceLocalId.html)
//				SdaiInstance			instance							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	engiGetInstanceLocalId(
									SdaiInstance			instance
								);

//
//		sdaiTestAttr                                            (http://rdf.bg/ifcdoc/CP64/sdaiTestAttr.html)
//				SdaiInstance			instance							IN
//				SdaiAttr				attribute							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiTestAttr(
									SdaiInstance			instance,
									SdaiAttr				attribute
								);

//
//		sdaiTestAttrBN                                          (http://rdf.bg/ifcdoc/CP64/sdaiTestAttrBN.html)
//				SdaiInstance			instance							IN
//				const char				* attributeName						IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiTestAttrBN(
									SdaiInstance			instance,
									const char				* attributeName
								);

//
//		sdaiCreateInstanceEI                                    (http://rdf.bg/ifcdoc/CP64/sdaiCreateInstanceEI.html)
//				SdaiModel				model								IN
//				SdaiEntity				entity								IN
//				int_t					express_id							IN
//
//				SdaiInstance			returns								OUT
//
//	...
//
SdaiInstance	DECL STDC	sdaiCreateInstanceEI(
									SdaiModel				model,
									SdaiEntity				entity,
									int_t					express_id
								);

//
//		sdaiCreateInstanceBNEI                                  (http://rdf.bg/ifcdoc/CP64/sdaiCreateInstanceBNEI.html)
//				SdaiModel				model								IN
//				const char				* entityName						IN
//				int_t					express_id							IN
//
//				SdaiInstance			returns								OUT
//
//	...
//
SdaiInstance	DECL STDC	sdaiCreateInstanceBNEI(
									SdaiModel				model,
									const char				* entityName,
									int_t					express_id
								);

//
//		setSegmentation                                         (http://rdf.bg/ifcdoc/CP64/setSegmentation.html)
//				SdaiModel				model								IN
//				int_t					segmentationParts					IN
//				double					segmentationLength					IN
//
//				void					returns
//
//	...
//
void			DECL STDC	setSegmentation(
									SdaiModel				model,
									int_t					segmentationParts,
									double					segmentationLength
								);

//
//		getSegmentation                                         (http://rdf.bg/ifcdoc/CP64/getSegmentation.html)
//				SdaiModel				model								IN
//				int_t					* segmentationParts					IN / OUT
//				double					* segmentationLength				IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	getSegmentation(
									SdaiModel				model,
									int_t					* segmentationParts,
									double					* segmentationLength
								);

//
//		setEpsilon                                              (http://rdf.bg/ifcdoc/CP64/setEpsilon.html)
//				SdaiModel				model								IN
//				int_t					mask								IN
//				double					absoluteEpsilon						IN
//				double					relativeEpsilon						IN
//
//				void					returns
//
//	...
//
void			DECL STDC	setEpsilon(
									SdaiModel				model,
									int_t					mask,
									double					absoluteEpsilon,
									double					relativeEpsilon
								);

//
//		getEpsilon                                              (http://rdf.bg/ifcdoc/CP64/getEpsilon.html)
//				SdaiModel				model								IN
//				int_t					mask								IN
//				double					* absoluteEpsilon					IN / OUT
//				double					* relativeEpsilon					IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getEpsilon(
									SdaiModel				model,
									int_t					mask,
									double					* absoluteEpsilon,
									double					* relativeEpsilon
								);

//
//  Controling API Calls
//

//
//		circleSegments                                          (http://rdf.bg/ifcdoc/CP64/circleSegments.html)
//				int_t					circles								IN
//				int_t					smallCircles						IN
//
//				void					returns
//
//	...
//
void			DECL STDC	circleSegments(
									int_t					circles,
									int_t					smallCircles
								);

//
//		setMaximumSegmentationLength                            (http://rdf.bg/ifcdoc/CP64/setMaximumSegmentationLength.html)
//				SdaiModel				model								IN
//				double					length								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	setMaximumSegmentationLength(
									SdaiModel				model,
									double					length
								);

//
//		getUnitConversionFactor                                 (http://rdf.bg/ifcdoc/CP64/getUnitConversionFactor.html)
//				SdaiModel				model								IN
//				const char				* unitType							IN
//				char					** unitPrefix						IN / OUT
//				char					** unitName							IN / OUT
//				char					** SIUnitName						IN / OUT
//
//				double					returns								OUT
//
//	...
//
double			DECL STDC	getUnitConversionFactor(
									SdaiModel				model,
									const char				* unitType,
									char					** unitPrefix,
									char					** unitName,
									char					** SIUnitName
								);

//
//		setBRepProperties                                       (http://rdf.bg/ifcdoc/CP64/setBRepProperties.html)
//				SdaiModel				model								IN
//				int64_t					consistencyCheck					IN
//				double					fraction							IN
//				double					epsilon								IN
//				int_t					maxVerticesSize						IN
//
//				void					returns
//
//	...
//
void			DECL STDC	setBRepProperties(
									SdaiModel				model,
									int64_t					consistencyCheck,
									double					fraction,
									double					epsilon,
									int_t					maxVerticesSize
								);

//
//		cleanMemory                                             (http://rdf.bg/ifcdoc/CP64/cleanMemory.html)
//				SdaiModel				model								IN
//				int_t					mode								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	cleanMemory(
									SdaiModel				model,
									int_t					mode
								);

//
//		internalGetP21Line                                      (http://rdf.bg/ifcdoc/CP64/internalGetP21Line.html)
//				SdaiInstance			instance							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	internalGetP21Line(
									SdaiInstance			instance
								);

//
//		internalForceInstanceFromP21Line                        (http://rdf.bg/ifcdoc/CP64/internalForceInstanceFromP21Line.html)
//				SdaiModel				model								IN
//				int_t					P21Line								IN
//
//				SdaiInstance			returns								OUT
//
//	...
//
SdaiInstance	DECL STDC	internalForceInstanceFromP21Line(
									SdaiModel				model,
									int_t					P21Line
								);

//
//		internalGetInstanceFromP21Line                          (http://rdf.bg/ifcdoc/CP64/internalGetInstanceFromP21Line.html)
//				SdaiModel				model								IN
//				int_t					P21Line								IN
//
//				SdaiInstance			returns								OUT
//
//	...
//
SdaiInstance	DECL STDC	internalGetInstanceFromP21Line(
									SdaiModel				model,
									int_t					P21Line
								);

//
//		internalGetXMLID                                        (http://rdf.bg/ifcdoc/CP64/internalGetXMLID.html)
//				SdaiInstance			instance							IN
//				char					** XMLID							IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	internalGetXMLID(
									SdaiInstance			instance,
									char					** XMLID
								);

//
//		setStringUnicode                                        (http://rdf.bg/ifcdoc/CP64/setStringUnicode.html)
//				int_t					unicode								IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	setStringUnicode(
									int_t					unicode
								);

//
//		setFilter                                               (http://rdf.bg/ifcdoc/CP64/setFilter.html)
//				SdaiModel				model								IN
//				int_t					setting								IN
//				int_t					mask								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	setFilter(
									SdaiModel				model,
									int_t					setting,
									int_t					mask
								);

//
//		getFilter                                               (http://rdf.bg/ifcdoc/CP64/getFilter.html)
//				SdaiModel				model								IN
//				int_t					mask								IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getFilter(
									SdaiModel				model,
									int_t					mask
								);

//
//  Uncategorized API Calls
//

//
//		xxxxGetEntityAndSubTypesExtent                          (http://rdf.bg/ifcdoc/CP64/xxxxGetEntityAndSubTypesExtent.html)
//				SdaiModel				model								IN
//				SdaiEntity				entity								IN
//
//				SdaiAggr				returns								OUT
//
//	model input parameter is irrelevant, but is required for backwards compatibility
//
SdaiAggr		DECL STDC	xxxxGetEntityAndSubTypesExtent(
									SdaiModel				model,		//		this input parameters is irrelevant, but is required for backwards compatibility
									SdaiEntity				entity
								);

//
//		xxxxGetEntityAndSubTypesExtentBN                        (http://rdf.bg/ifcdoc/CP64/xxxxGetEntityAndSubTypesExtentBN.html)
//				SdaiModel				model								IN
//				const char				* entityName						IN
//
//				SdaiAggr				returns								OUT
//
//	...
//
SdaiAggr		DECL STDC	xxxxGetEntityAndSubTypesExtentBN(
									SdaiModel				model,
									const char				* entityName
								);

//
//		xxxxGetAllInstances                                     (http://rdf.bg/ifcdoc/CP64/xxxxGetAllInstances.html)
//				SdaiModel				model								IN
//
//				SdaiAggr				returns								OUT
//
//	...
//
SdaiAggr		DECL STDC	xxxxGetAllInstances(
									SdaiModel				model
								);

//
//		xxxxGetInstancesUsing                                   (http://rdf.bg/ifcdoc/CP64/xxxxGetInstancesUsing.html)
//				SdaiInstance			instance							IN
//
//				SdaiAggr				returns								OUT
//
//	...
//
SdaiAggr		DECL STDC	xxxxGetInstancesUsing(
									SdaiInstance			instance
								);

//
//		xxxxDeleteFromAggregation                               (http://rdf.bg/ifcdoc/CP64/xxxxDeleteFromAggregation.html)
//				SdaiInstance			instance							IN
//				SdaiAggr				aggregate							IN
//				int_t					elementIndex						IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	xxxxDeleteFromAggregation(
									SdaiInstance			instance,
									SdaiAggr				aggregate,
									int_t					elementIndex
								);

//
//		xxxxGetAttrDefinitionByValue                            (http://rdf.bg/ifcdoc/CP64/xxxxGetAttrDefinitionByValue.html)
//				SdaiInstance			instance							IN
//				const void				* value								IN
//
//				SdaiAttr				returns								OUT
//
//	...
//
SdaiAttr		DECL STDC	xxxxGetAttrDefinitionByValue(
									SdaiInstance			instance,
									const void				* value
								);

//
//		xxxxGetAttrNameByIndex                                  (http://rdf.bg/ifcdoc/CP64/xxxxGetAttrNameByIndex.html)
//				SdaiInstance			instance							IN
//				int_t					index								IN
//				const char				** name								IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	xxxxGetAttrNameByIndex(
									SdaiInstance			instance,
									int_t					index,
									const char				** name
								);

//
//		iterateOverInstances                                    (http://rdf.bg/ifcdoc/CP64/iterateOverInstances.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				SdaiEntity				* entity							IN / OUT
//				char					** entityName						IN / OUT
//
//				SdaiInstance			returns								OUT
//
//	This function interates over all available instances loaded in memory, it is the fastest way to find all instances.
//	Argument entity and entityName are both optional and if non-zero are filled with respectively the entity handle and entity name as char array.
//
SdaiInstance	DECL STDC	iterateOverInstances(
									SdaiModel				model,
									SdaiInstance			instance,
									SdaiEntity				* entity,
									char					** entityName
								);

//
//		iterateOverProperties                                   (http://rdf.bg/ifcdoc/CP64/iterateOverProperties.html)
//				SdaiEntity				entity								IN
//				int_t					index								IN
//
//				int_t					returns								OUT
//
//	This function iterated over all available attributes of a specific given entity.
//	This call is typically used in combination with iterateOverInstances(..).
//
int_t			DECL STDC	iterateOverProperties(
									SdaiEntity				entity,
									int_t					index
								);

//
//		sdaiGetAggrByIterator                                   (http://rdf.bg/ifcdoc/CP64/sdaiGetAggrByIterator.html)
//				int_t					iterator							IN
//				int_t					valueType							IN
//				void					* value								IN / OUT
//
//				void					* returns							OUT
//
//	...
//
void			DECL * STDC	sdaiGetAggrByIterator(
									int_t					iterator,
									int_t					valueType,
									void					* value
								);

//
//		sdaiPutAggrByIterator                                   (http://rdf.bg/ifcdoc/CP64/sdaiPutAggrByIterator.html)
//				int_t					iterator							IN
//				int_t					valueType							IN
//				const void				* value								IN
//
//				void					returns
//
//	...
//
void			DECL STDC	sdaiPutAggrByIterator(
									int_t					iterator,
									int_t					valueType,
									const void				* value
								);

//
//		internalSetLink                                         (http://rdf.bg/ifcdoc/CP64/internalSetLink.html)
//				SdaiInstance			instance							IN
//				const char				* attributeName						IN
//				int_t					linked_id							IN
//
//				void					returns
//
//	...
//
void			DECL STDC	internalSetLink(
									SdaiInstance			instance,
									const char				* attributeName,
									int_t					linked_id
								);

//
//		internalAddAggrLink                                     (http://rdf.bg/ifcdoc/CP64/internalAddAggrLink.html)
//				int_t					list								IN
//				int_t					linked_id							IN
//
//				void					returns
//
//	...
//
void			DECL STDC	internalAddAggrLink(
									int_t					list,
									int_t					linked_id
								);

//
//		engiGetNotReferedAggr                                   (http://rdf.bg/ifcdoc/CP64/engiGetNotReferedAggr.html)
//				SdaiModel				model								IN
//				int_t					* value								IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetNotReferedAggr(
									SdaiModel				model,
									int_t					* value
								);

//
//		engiGetAttributeAggr                                    (http://rdf.bg/ifcdoc/CP64/engiGetAttributeAggr.html)
//				SdaiInstance			instance							IN
//				int_t					* value								IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetAttributeAggr(
									SdaiInstance			instance,
									int_t					* value
								);

//
//		engiGetAggrUnknownElement                               (http://rdf.bg/ifcdoc/CP64/engiGetAggrUnknownElement.html)
//				SdaiAggr				aggregate							IN
//				int_t					elementIndex						IN
//				int_t					* valueType							IN / OUT
//				void					* value								IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	engiGetAggrUnknownElement(
									SdaiAggr				aggregate,
									int_t					elementIndex,
									int_t					* valueType,
									void					* value
								);

//
//		sdaiErrorQuery                                          (http://rdf.bg/ifcdoc/CP64/sdaiErrorQuery.html)
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiErrorQuery(
								);

//
//  Geometry Kernel related API Calls
//

//
//		owlGetModel                                             (http://rdf.bg/ifcdoc/CP64/owlGetModel.html)
//				SdaiModel				model								IN
//				int64_t					* owlModel							IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	owlGetModel(
									SdaiModel				model,
									int64_t					* owlModel
								);

//
//		owlGetInstance                                          (http://rdf.bg/ifcdoc/CP64/owlGetInstance.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				int64_t					* owlInstance						IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	owlGetInstance(
									SdaiModel				model,
									SdaiInstance			instance,
									int64_t					* owlInstance
								);

//
//		owlMaterialInstance                                     (http://rdf.bg/ifcdoc/CP64/owlMaterialInstance.html)
//				SdaiInstance			instanceBase						IN
//				SdaiInstance			instanceContext						IN
//				int64_t					* owlInstance						IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	owlMaterialInstance(
									SdaiInstance			instanceBase,
									SdaiInstance			instanceContext,
									int64_t					* owlInstance
								);

//
//		owlBuildInstance                                        (http://rdf.bg/ifcdoc/CP64/owlBuildInstance.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				int64_t					* owlInstance						IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	owlBuildInstance(
									SdaiModel				model,
									SdaiInstance			instance,
									int64_t					* owlInstance
								);

//
//		owlBuildInstanceInContext                               (http://rdf.bg/ifcdoc/CP64/owlBuildInstanceInContext.html)
//				SdaiInstance			instanceBase						IN
//				SdaiInstance			instanceContext						IN
//				int64_t					* owlInstance						IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	owlBuildInstanceInContext(
									SdaiInstance			instanceBase,
									SdaiInstance			instanceContext,
									int64_t					* owlInstance
								);

//
//		engiInstanceUsesSegmentation                            (http://rdf.bg/ifcdoc/CP64/engiInstanceUsesSegmentation.html)
//				SdaiInstance			instance							IN
//
//				bool					returns								OUT
//
//	...
//
bool			DECL STDC	engiInstanceUsesSegmentation(
									SdaiInstance			instance
								);

//
//		owlBuildInstances                                       (http://rdf.bg/ifcdoc/CP64/owlBuildInstances.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				int64_t					* owlInstanceComplete				IN / OUT
//				int64_t					* owlInstanceSolids					IN / OUT
//				int64_t					* owlInstanceVoids					IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	owlBuildInstances(
									SdaiModel				model,
									SdaiInstance			instance,
									int64_t					* owlInstanceComplete,
									int64_t					* owlInstanceSolids,
									int64_t					* owlInstanceVoids
								);

//
//		owlGetMappedItem                                        (http://rdf.bg/ifcdoc/CP64/owlGetMappedItem.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				int64_t					* owlInstance						IN / OUT
//				double					* transformationMatrix				IN / OUT
//
//				void					returns
//
//	...
//
void			DECL STDC	owlGetMappedItem(
									SdaiModel				model,
									SdaiInstance			instance,
									int64_t					* owlInstance,
									double					* transformationMatrix
								);

//
//		getInstanceDerivedPropertiesInModelling                 (http://rdf.bg/ifcdoc/CP64/getInstanceDerivedPropertiesInModelling.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				double					* height							IN / OUT
//				double					* width								IN / OUT
//				double					* thickness							IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getInstanceDerivedPropertiesInModelling(
									SdaiModel				model,
									SdaiInstance			instance,
									double					* height,
									double					* width,
									double					* thickness
								);

//
//		getInstanceDerivedBoundingBox                           (http://rdf.bg/ifcdoc/CP64/getInstanceDerivedBoundingBox.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				double					* Ox								IN / OUT
//				double					* Oy								IN / OUT
//				double					* Oz								IN / OUT
//				double					* Vx								IN / OUT
//				double					* Vy								IN / OUT
//				double					* Vz								IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getInstanceDerivedBoundingBox(
									SdaiModel				model,
									SdaiInstance			instance,
									double					* Ox,
									double					* Oy,
									double					* Oz,
									double					* Vx,
									double					* Vy,
									double					* Vz
								);

//
//		getInstanceTransformationMatrix                         (http://rdf.bg/ifcdoc/CP64/getInstanceTransformationMatrix.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				double					* _11								IN / OUT
//				double					* _12								IN / OUT
//				double					* _13								IN / OUT
//				double					* _14								IN / OUT
//				double					* _21								IN / OUT
//				double					* _22								IN / OUT
//				double					* _23								IN / OUT
//				double					* _24								IN / OUT
//				double					* _31								IN / OUT
//				double					* _32								IN / OUT
//				double					* _33								IN / OUT
//				double					* _34								IN / OUT
//				double					* _41								IN / OUT
//				double					* _42								IN / OUT
//				double					* _43								IN / OUT
//				double					* _44								IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getInstanceTransformationMatrix(
									SdaiModel				model,
									SdaiInstance			instance,
									double					* _11,
									double					* _12,
									double					* _13,
									double					* _14,
									double					* _21,
									double					* _22,
									double					* _23,
									double					* _24,
									double					* _31,
									double					* _32,
									double					* _33,
									double					* _34,
									double					* _41,
									double					* _42,
									double					* _43,
									double					* _44
								);

//
//		getInstanceDerivedTransformationMatrix                  (http://rdf.bg/ifcdoc/CP64/getInstanceDerivedTransformationMatrix.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				double					* _11								IN / OUT
//				double					* _12								IN / OUT
//				double					* _13								IN / OUT
//				double					* _14								IN / OUT
//				double					* _21								IN / OUT
//				double					* _22								IN / OUT
//				double					* _23								IN / OUT
//				double					* _24								IN / OUT
//				double					* _31								IN / OUT
//				double					* _32								IN / OUT
//				double					* _33								IN / OUT
//				double					* _34								IN / OUT
//				double					* _41								IN / OUT
//				double					* _42								IN / OUT
//				double					* _43								IN / OUT
//				double					* _44								IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getInstanceDerivedTransformationMatrix(
									SdaiModel				model,
									SdaiInstance			instance,
									double					* _11,
									double					* _12,
									double					* _13,
									double					* _14,
									double					* _21,
									double					* _22,
									double					* _23,
									double					* _24,
									double					* _31,
									double					* _32,
									double					* _33,
									double					* _34,
									double					* _41,
									double					* _42,
									double					* _43,
									double					* _44
								);

//
//		internalGetBoundingBox                                  (http://rdf.bg/ifcdoc/CP64/internalGetBoundingBox.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//
//				void					* returns							OUT
//
//	...
//
void			DECL * STDC	internalGetBoundingBox(
									SdaiModel				model,
									SdaiInstance			instance
								);

//
//		internalGetCenter                                       (http://rdf.bg/ifcdoc/CP64/internalGetCenter.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//
//				void					* returns							OUT
//
//	...
//
void			DECL * STDC	internalGetCenter(
									SdaiModel				model,
									SdaiInstance			instance
								);

//
//		getGlobalPlacement                                      (http://rdf.bg/ifcdoc/CP64/getGlobalPlacement.html)
//				SdaiModel				model								IN
//				double					* origin							IN / OUT
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getGlobalPlacement(
									SdaiModel				model,
									double					* origin
								);

//
//		setGlobalPlacement                                      (http://rdf.bg/ifcdoc/CP64/setGlobalPlacement.html)
//				SdaiModel				model								IN
//				const double			* origin							IN
//				bool					includeRotation						IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	setGlobalPlacement(
									SdaiModel				model,
									const double			* origin,
									bool					includeRotation
								);

//
//		getTimeStamp                                            (http://rdf.bg/ifcdoc/CP64/getTimeStamp.html)
//				SdaiModel				model								IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getTimeStamp(
									SdaiModel				model
								);

//
//		setInstanceReference                                    (http://rdf.bg/ifcdoc/CP64/setInstanceReference.html)
//				SdaiInstance			instance							IN
//				int_t					value								IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	setInstanceReference(
									SdaiInstance			instance,
									int_t					value
								);

//
//		getInstanceReference                                    (http://rdf.bg/ifcdoc/CP64/getInstanceReference.html)
//				SdaiInstance			instance							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	getInstanceReference(
									SdaiInstance			instance
								);

//
//		inferenceInstance                                       (http://rdf.bg/ifcdoc/CP64/inferenceInstance.html)
//				SdaiInstance			instance							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	inferenceInstance(
									SdaiInstance			instance
								);

//
//		sdaiValidateSchemaInstance                              (http://rdf.bg/ifcdoc/CP64/sdaiValidateSchemaInstance.html)
//				SdaiInstance			instance							IN
//
//				int_t					returns								OUT
//
//	...
//
int_t			DECL STDC	sdaiValidateSchemaInstance(
									SdaiInstance			instance
								);

//
//  Deprecated API Calls (GENERIC)
//

//
//		engiAttrIsInverse                                       (http://rdf.bg/ifcdoc/CP64/engiAttrIsInverse.html)
//				SdaiAttr				attribute							IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call engiAttrIsInverse.
//
int_t			DECL STDC	engiAttrIsInverse(
									SdaiAttr				attribute
								);

//
//		xxxxOpenModelByStream                                   (http://rdf.bg/ifcdoc/CP64/xxxxOpenModelByStream.html)
//				int_t					repository							IN
//				const void				* callback							IN
//				const char				* schemaName						IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call engiOpenModelByStream.
//
int_t			DECL STDC	xxxxOpenModelByStream(
									int_t					repository,
									const void				* callback,
									const char				* schemaName
								);

//
//		sdaiCreateIterator                                      (http://rdf.bg/ifcdoc/CP64/sdaiCreateIterator.html)
//				SdaiAggr				aggregate							IN
//
//				void					* returns							OUT
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
//
void			DECL * STDC	sdaiCreateIterator(
									SdaiAggr				aggregate
								);

//
//		sdaiDeleteIterator                                      (http://rdf.bg/ifcdoc/CP64/sdaiDeleteIterator.html)
//				int_t					iterator							IN
//
//				void					returns
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
//
void			DECL STDC	sdaiDeleteIterator(
									int_t					iterator
								);

//
//		sdaiBeginning                                           (http://rdf.bg/ifcdoc/CP64/sdaiBeginning.html)
//				int_t					iterator							IN
//
//				void					returns
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
//
void			DECL STDC	sdaiBeginning(
									int_t					iterator
								);

//
//		sdaiNext                                                (http://rdf.bg/ifcdoc/CP64/sdaiNext.html)
//				int_t					iterator							IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
//
int_t			DECL STDC	sdaiNext(
									int_t					iterator
								);

//
//		sdaiPrevious                                            (http://rdf.bg/ifcdoc/CP64/sdaiPrevious.html)
//				int_t					iterator							IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
//
int_t			DECL STDC	sdaiPrevious(
									int_t					iterator
								);

//
//		sdaiEnd                                                 (http://rdf.bg/ifcdoc/CP64/sdaiEnd.html)
//				int_t					iterator							IN
//
//				void					returns
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
//
void			DECL STDC	sdaiEnd(
									int_t					iterator
								);

//
//		sdaiplusGetAggregationType                              (http://rdf.bg/ifcdoc/CP64/sdaiplusGetAggregationType.html)
//				SdaiInstance			instance							IN
//				SdaiAggr				aggregate							IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call ....
//
int_t			DECL STDC	sdaiplusGetAggregationType(
									SdaiInstance			instance,
									SdaiAggr				aggregate
								);

//
//		xxxxGetAttrTypeBN                                       (http://rdf.bg/ifcdoc/CP64/xxxxGetAttrTypeBN.html)
//				SdaiInstance			instance							IN
//				const char				* attributeName						IN
//				char					** attributeType					IN / OUT
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use calls engiGetAttrTypeBN(..).
//
int_t			DECL STDC	xxxxGetAttrTypeBN(
									SdaiInstance			instance,
									const char				* attributeName,
									char					** attributeType
								);

//
//  Validation
//

//
//void UsageExample(SdaiModel model)
//{
//	//set oprions if you need
//	validateSetOptions(10, 100, 0, enum_validation_type::__WHERE_RULE); //limit the work by 10 secs and first 100 issues,
//																			 //exclude where rules check
//
//	ValidationResults results = validateModel(model);
//
//	for (ValidationIssue issue = validateGetFirstIssue(results); issue; issue = validateGetNextIssue(issue)) {
//		SdaiInstance inst = validateGetInstance(issue);
//		const char* descr = validateGetDescription(issue);
//		...	
//	}
//
//	if (validateGetStatus(results)!=enum_validation_status::__COMPLETE_ALL) {
//		printf("There may be more issues, increase limits\n");
//	}
//
//	validateFreeResults(results);
//}

//
//		validateSetOptions                                      (http://rdf.bg/ifcdoc/CP64/validateSetOptions.html)
//				int_t					timeLimitSeconds					IN
//				int_t					issueCntLimit						IN
//				bool					showEachIssueOnce					IN
//				uint64_t				issueTypes							IN
//				uint64_t				mask								IN
//
//				void					returns
//
//	..
//
void			DECL STDC	validateSetOptions(
									int_t					timeLimitSeconds,
									int_t					issueCntLimit,
									bool					showEachIssueOnce,
									uint64_t				issueTypes,
									uint64_t				mask
								);

//
//		validateGetOptions                                      (http://rdf.bg/ifcdoc/CP64/validateGetOptions.html)
//				int_t					* timeLimitSeconds					IN / OUT
//				int_t					* issueCntLimit						IN / OUT
//				bool					* showEachIssueOnce					IN / OUT
//				uint64_t				mask								IN
//
//				uint64_t				returns								OUT
//
//	..
//
uint64_t		DECL STDC	validateGetOptions(
									int_t					* timeLimitSeconds,
									int_t					* issueCntLimit,
									bool					* showEachIssueOnce,
									uint64_t				mask
								);

//
//		validateModel                                           (http://rdf.bg/ifcdoc/CP64/validateModel.html)
//				SdaiModel				model								IN
//
//				ValidationResults		returns								OUT
//
//	..
//
ValidationResults	DECL STDC	validateModel(
									SdaiModel				model
								);

//
//		validateInstance                                        (http://rdf.bg/ifcdoc/CP64/validateInstance.html)
//				SdaiInstance			instance							IN
//
//				ValidationResults		returns								OUT
//
//	..
//
ValidationResults	DECL STDC	validateInstance(
									SdaiInstance			instance
								);

//
//		validateFreeResults                                     (http://rdf.bg/ifcdoc/CP64/validateFreeResults.html)
//				ValidationResults		results								IN
//
//				void					returns
//
//	..
//
void			DECL STDC	validateFreeResults(
									ValidationResults		results
								);

//
//		validateGetFirstIssue                                   (http://rdf.bg/ifcdoc/CP64/validateGetFirstIssue.html)
//				ValidationResults		results								IN
//
//				ValidationIssue			returns								OUT
//
//	..
//
ValidationIssue	DECL STDC	validateGetFirstIssue(
									ValidationResults		results
								);

//
//		validateGetNextIssue                                    (http://rdf.bg/ifcdoc/CP64/validateGetNextIssue.html)
//				ValidationIssue			issue								IN
//
//				ValidationIssue			returns								OUT
//
//	..
//
ValidationIssue	DECL STDC	validateGetNextIssue(
									ValidationIssue			issue
								);

//
//		validateGetStatus                                       (http://rdf.bg/ifcdoc/CP64/validateGetStatus.html)
//				ValidationResults		results								IN
//
//				enum_validation_status	returns								OUT
//
//	..
//
enum_validation_status	DECL STDC	validateGetStatus(
									ValidationResults		results
								);

//
//		validateGetIssueType                                    (http://rdf.bg/ifcdoc/CP64/validateGetIssueType.html)
//				ValidationIssue			issue								IN
//
//				enum_validation_type	returns								OUT
//
//	..
//
enum_validation_type	DECL STDC	validateGetIssueType(
									ValidationIssue			issue
								);

//
//		validateGetInstance                                     (http://rdf.bg/ifcdoc/CP64/validateGetInstance.html)
//				ValidationIssue			issue								IN
//
//				SdaiInstance			returns								OUT
//
//	..
//
SdaiInstance	DECL STDC	validateGetInstance(
									ValidationIssue			issue		//	step instace where the issue is happend or 0
								);

//
//		validateGetInstanceRelated                              (http://rdf.bg/ifcdoc/CP64/validateGetInstanceRelated.html)
//				ValidationIssue			issue								IN
//
//				SdaiInstance			returns								OUT
//
//	..
//
SdaiInstance	DECL STDC	validateGetInstanceRelated(
									ValidationIssue			issue
								);

//
//		validateGetEntity                                       (http://rdf.bg/ifcdoc/CP64/validateGetEntity.html)
//				ValidationIssue			issue								IN
//
//				SdaiEntity				returns								OUT
//
//	..
//
SdaiEntity		DECL STDC	validateGetEntity(
									ValidationIssue			issue		//	entity or NULL
								);

//
//		validateGetAttr                                         (http://rdf.bg/ifcdoc/CP64/validateGetAttr.html)
//				ValidationIssue			issue								IN
//
//				SdaiAttr				returns								OUT
//
//	..
//
SdaiAttr		DECL STDC	validateGetAttr(
									ValidationIssue			issue		//	attribute or NULL
								);

//
//		validateGetAggrLevel                                    (http://rdf.bg/ifcdoc/CP64/validateGetAggrLevel.html)
//				ValidationIssue			issue								IN
//
//				ValidationIssueLevel	returns								OUT
//
//	..
//
ValidationIssueLevel	DECL STDC	validateGetAggrLevel(
									ValidationIssue			issue		//	specifies nesting level of aggregation or 0
								);

//
//		validateGetAggrIndArray                                 (http://rdf.bg/ifcdoc/CP64/validateGetAggrIndArray.html)
//				ValidationIssue			issue								IN
//
//				int_t					* returns							OUT
//
//	..
//
const int_t		DECL * STDC	validateGetAggrIndArray(
									ValidationIssue			issue		//	array of indecies for each aggregation lsize is aggrLevel
								);

//
//		validateGetIssueLevel                                   (http://rdf.bg/ifcdoc/CP64/validateGetIssueLevel.html)
//				ValidationIssue			issue								IN
//
//				int_t					returns								OUT
//
//	..
//
int_t			DECL STDC	validateGetIssueLevel(
									ValidationIssue			issue
								);

//
//		validateGetDescription                                  (http://rdf.bg/ifcdoc/CP64/validateGetDescription.html)
//				ValidationIssue			issue								IN
//
//				char					* returns							OUT
//
//	..
//
const char		DECL * STDC	validateGetDescription(
									ValidationIssue			issue		//	description text
								);

//
//  Deprecated API Calls (GEOMETRY)
//

//
//		initializeModellingInstance                             (http://rdf.bg/ifcdoc/CP64/initializeModellingInstance.html)
//				SdaiModel				model								IN
//				int_t					* noVertices						IN / OUT
//				int_t					* noIndices							IN / OUT
//				double					scale								IN
//				SdaiInstance			instance							IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call CalculateInstance().
//
int_t			DECL STDC	initializeModellingInstance(
									SdaiModel				model,
									int_t					* noVertices,
									int_t					* noIndices,
									double					scale,
									SdaiInstance			instance
								);

//
//		finalizeModelling                                       (http://rdf.bg/ifcdoc/CP64/finalizeModelling.html)
//				SdaiModel				model								IN
//				float					* vertices							IN / OUT
//				int_t					* indices							IN / OUT
//				int_t					FVF									IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call UpdateInstanceVertexBuffer() and UpdateInstanceIndexBuffer().
//
int_t			DECL STDC	finalizeModelling(
									SdaiModel				model,
									float					* vertices,
									int_t					* indices,
									int_t					FVF
								);

//
//		getInstanceInModelling                                  (http://rdf.bg/ifcdoc/CP64/getInstanceInModelling.html)
//				SdaiModel				model								IN
//				SdaiInstance			instance							IN
//				int_t					mode								IN
//				int_t					* startVertex						IN / OUT
//				int_t					* startIndex						IN / OUT
//				int_t					* primitiveCount					IN / OUT
//
//				int_t					returns								OUT
//
//	This call is deprecated, there is no direct / easy replacement although the functionality is present. If you still use this call please contact RDF to find a solution together.
//
int_t			DECL STDC	getInstanceInModelling(
									SdaiModel				model,
									SdaiInstance			instance,
									int_t					mode,
									int_t					* startVertex,
									int_t					* startIndex,
									int_t					* primitiveCount
								);

//
//		setVertexOffset                                         (http://rdf.bg/ifcdoc/CP64/setVertexOffset.html)
//				SdaiModel				model								IN
//				double					x									IN
//				double					y									IN
//				double					z									IN
//
//				void					returns
//
//	This call is deprecated, please use call SetVertexBufferOffset().
//
void			DECL STDC	setVertexOffset(
									SdaiModel				model,
									double					x,
									double					y,
									double					z
								);

//
//		setFormat                                               (http://rdf.bg/ifcdoc/CP64/setFormat.html)
//				SdaiModel				model								IN
//				int_t					setting								IN
//				int_t					mask								IN
//
//				void					returns
//
//	This call is deprecated, please use call SetFormat().
//
void			DECL STDC	setFormat(
									SdaiModel				model,
									int_t					setting,
									int_t					mask
								);

//
//		getConceptualFaceCnt                                    (http://rdf.bg/ifcdoc/CP64/getConceptualFaceCnt.html)
//				SdaiInstance			instance							IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call GetConceptualFaceCnt().
//
int_t			DECL STDC	getConceptualFaceCnt(
									SdaiInstance			instance
								);

//
//		getConceptualFaceEx                                     (http://rdf.bg/ifcdoc/CP64/getConceptualFaceEx.html)
//				SdaiInstance			instance							IN
//				int_t					index								IN
//				int_t					* startIndexTriangles				IN / OUT
//				int_t					* noIndicesTriangles				IN / OUT
//				int_t					* startIndexLines					IN / OUT
//				int_t					* noIndicesLines					IN / OUT
//				int_t					* startIndexPoints					IN / OUT
//				int_t					* noIndicesPoints					IN / OUT
//				int_t					* startIndexFacePolygons			IN / OUT
//				int_t					* noIndicesFacePolygons				IN / OUT
//				int_t					* startIndexConceptualFacePolygons	IN / OUT
//				int_t					* noIndicesConceptualFacePolygons	IN / OUT
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call GetConceptualFaceEx().
//
int_t			DECL STDC	getConceptualFaceEx(
									SdaiInstance			instance,
									int_t					index,
									int_t					* startIndexTriangles,
									int_t					* noIndicesTriangles,
									int_t					* startIndexLines,
									int_t					* noIndicesLines,
									int_t					* startIndexPoints,
									int_t					* noIndicesPoints,
									int_t					* startIndexFacePolygons,
									int_t					* noIndicesFacePolygons,
									int_t					* startIndexConceptualFacePolygons,
									int_t					* noIndicesConceptualFacePolygons
								);

//
//		createGeometryConversion                                (http://rdf.bg/ifcdoc/CP64/createGeometryConversion.html)
//				SdaiInstance			instance							IN
//				int64_t					* owlInstance						IN / OUT
//
//				void					returns
//
//	This call is deprecated, please use call ... .
//
void			DECL STDC	createGeometryConversion(
									SdaiInstance			instance,
									int64_t					* owlInstance
								);

//
//		convertInstance                                         (http://rdf.bg/ifcdoc/CP64/convertInstance.html)
//				SdaiInstance			instance							IN
//
//				void					returns
//
//	This call is deprecated, please use call ... .
//
void			DECL STDC	convertInstance(
									SdaiInstance			instance
								);

//
//		initializeModellingInstanceEx                           (http://rdf.bg/ifcdoc/CP64/initializeModellingInstanceEx.html)
//				SdaiModel				model								IN
//				int_t					* noVertices						IN / OUT
//				int_t					* noIndices							IN / OUT
//				double					scale								IN
//				SdaiInstance			instance							IN
//				int_t					instanceList						IN
//
//				int_t					returns								OUT
//
//	This call is deprecated, please use call CalculateInstance().
//
int_t			DECL STDC	initializeModellingInstanceEx(
									SdaiModel				model,
									int_t					* noVertices,
									int_t					* noIndices,
									double					scale,
									SdaiInstance			instance,
									int_t					instanceList
								);

//
//		exportModellingAsOWL                                    (http://rdf.bg/ifcdoc/CP64/exportModellingAsOWL.html)
//				SdaiModel				model								IN
//				const char				* fileName							IN
//
//				void					returns
//
//	This call is deprecated, please contact us if you use this call.
//
void			DECL STDC	exportModellingAsOWL(
									SdaiModel				model,
									const char				* fileName
								);


#ifdef __cplusplus
	}
#endif


#undef DECL
#undef STDC
#endif
