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


#ifdef __cplusplus
	extern "C" {
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

#define		SdaiAccessMode			int_t
#define		SdaiSession				int_t
#define		SdaiRep					int_t
#define		SdaiModel				int_t
#define		SdaiIterator			int_t
#define		SdaiEntity				int_t
#define		SdaiADB					sdaiADB
#define		SdaiErrorCode			int_t
		//#define		sdaiNULL
#define		SdaiAppInstance			int_t
#define		SdaiEntity				int_t
#define		SdaiNPL					int_t

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
typedef	int_t			SdaiInstance;
typedef	int_t			SdaiInteger;
typedef	int_t			SdaiModel;
typedef	int_t			SdaiPrimitiveType;
typedef	int_t			* SdaiSet;
typedef	char			* SdaiString;



//
//  File IO API Calls
//

//
//		sdaiCreateModelBN                           (http://rdf.bg/ifcdoc/CP64/sdaiCreateModelBN.html)
//				int_t				repository							IN
//				const char			* fileName							IN
//				const char			* schemaName						IN
//
//				int_t				returns								OUT
//
//	This function creates and empty model (we expect with a schema file given).
//	Attributes repository and fileName will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
int_t		DECL STDC	sdaiCreateModelBN(
									int_t				repository,
									const char			* fileName,
									const char			* schemaName
								);

//
//		sdaiCreateModelBNUnicode                    (http://rdf.bg/ifcdoc/CP64/sdaiCreateModelBNUnicode.html)
//				int_t				repository							IN
//				const void			* fileName							IN
//				const void			* schemaName						IN
//
//				int_t				returns								OUT
//
//	This function creates and empty model (we expect with a schema file given).
//	Attributes repository and fileName will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
int_t		DECL STDC	sdaiCreateModelBNUnicode(
									int_t				repository,
									const void			* fileName,
									const void			* schemaName
								);

//
//		sdaiOpenModelBN                             (http://rdf.bg/ifcdoc/CP64/sdaiOpenModelBN.html)
//				int_t				repository							IN
//				const char			* fileName							IN
//				const char			* schemaName						IN
//
//				int_t				returns								OUT
//
//	This function opens the model on location fileName.
//	Attribute repository will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
int_t		DECL STDC	sdaiOpenModelBN(
									int_t				repository,
									const char			* fileName,
									const char			* schemaName
								);

//
//		sdaiOpenModelBNUnicode                      (http://rdf.bg/ifcdoc/CP64/sdaiOpenModelBNUnicode.html)
//				int_t				repository							IN
//				const void			* fileName							IN
//				const void			* schemaName						IN
//
//				int_t				returns								OUT
//
//	This function opens the model on location fileName.
//	Attribute repository will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
int_t		DECL STDC	sdaiOpenModelBNUnicode(
									int_t				repository,
									const void			* fileName,
									const void			* schemaName
								);

//
//		engiOpenModelByStream                       (http://rdf.bg/ifcdoc/CP64/engiOpenModelByStream.html)
//				int_t				repository							IN
//				const void			* callback							IN
//				const char			* schemaName						IN
//
//				int_t				returns								OUT
//
//	This function opens the model via a stream.
//	Attribute repository will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
int_t		DECL STDC	engiOpenModelByStream(
									int_t				repository,
									const void			* callback,
									const char			* schemaName
								);

//
//		engiOpenModelByArray                        (http://rdf.bg/ifcdoc/CP64/engiOpenModelByArray.html)
//				int_t				repository							IN
//				const unsigned char	* content							IN
//				int_t				size								IN
//				const char			* schemaName						IN
//
//				int_t				returns								OUT
//
//	This function opens the model via an array.
//	Attribute repository will be ignored, they are their because of backward compatibility.
//	A handle to the model will be returned, or 0 in case something went wrong.
//
int_t		DECL STDC	engiOpenModelByArray(
									int_t				repository,
									const unsigned char	* content,
									int_t				size,
									const char			* schemaName
								);

//
//		sdaiSaveModelBN                             (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelBN.html)
//				int_t				model								IN
//				const char			* fileName							IN
//
//				void				returns
//
//	This function saves the model (char file name).
//
void		DECL STDC	sdaiSaveModelBN(
									int_t				model,
									const char			* fileName
								);

//
//		sdaiSaveModelBNUnicode                      (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelBNUnicode.html)
//				int_t				model								IN
//				const void			* fileName							IN
//
//				void				returns
//
//	This function saves the model (wchar, i.e. Unicode file name).
//
void		DECL STDC	sdaiSaveModelBNUnicode(
									int_t				model,
									const void			* fileName
								);

//
//		engiSaveModelByStream                       (http://rdf.bg/ifcdoc/CP64/engiSaveModelByStream.html)
//				int_t				model								IN
//				const void			* callback							IN
//				int_t				size								IN
//
//				void				returns
//
//	This function saves the model as an array.
//
void		DECL STDC	engiSaveModelByStream(
									int_t				model,
									const void			* callback,
									int_t				size
								);

//
//		engiSaveModelByArray                        (http://rdf.bg/ifcdoc/CP64/engiSaveModelByArray.html)
//				int_t				model								IN
//				unsigned char		* content							IN / OUT
//				int_t				* size								IN / OUT
//
//				void				returns
//
//	This function saves the model as an array.
//
void		DECL STDC	engiSaveModelByArray(
									int_t				model,
									unsigned char		* content,
									int_t				* size
								);

//
//		sdaiSaveModelAsXmlBN                        (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsXmlBN.html)
//				int_t				model								IN
//				const char			* fileName							IN
//
//				void				returns
//
//	This function saves the model as XML according to IFC2x3's way of XML serialization (char file name).
//
void		DECL STDC	sdaiSaveModelAsXmlBN(
									int_t				model,
									const char			* fileName
								);

//
//		sdaiSaveModelAsXmlBNUnicode                 (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsXmlBNUnicode.html)
//				int_t				model								IN
//				const void			* fileName							IN
//
//				void				returns
//
//	This function saves the model as XML according to IFC2x3's way of XML serialization (wchar, i.e. Unicode file name).
//
void		DECL STDC	sdaiSaveModelAsXmlBNUnicode(
									int_t				model,
									const void			* fileName
								);

//
//		sdaiSaveModelAsSimpleXmlBN                  (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsSimpleXmlBN.html)
//				int_t				model								IN
//				const char			* fileName							IN
//
//				void				returns
//
//	This function saves the model as XML according to IFC4's way of XML serialization (char file name).
//
void		DECL STDC	sdaiSaveModelAsSimpleXmlBN(
									int_t				model,
									const char			* fileName
								);

void		DECL STDC	sdaiSaveModelAsJsonBN(
									int_t				model,
									const char			* fileName
								);

//
//		sdaiSaveModelAsSimpleXmlBNUnicode           (http://rdf.bg/ifcdoc/CP64/sdaiSaveModelAsSimpleXmlBNUnicode.html)
//				int_t				model								IN
//				const void			* fileName							IN
//
//				void				returns
//
//	This function saves the model as XML according to IFC4's way of XML serialization (wchar, i.e. Unicode file name).
//
void		DECL STDC	sdaiSaveModelAsSimpleXmlBNUnicode(
									int_t				model,
									const void			* fileName
								);

void		DECL STDC	sdaiSaveModelAsJsonBNUnicode(
									int_t				model,
									const void			* fileName
								);

//
//		sdaiCloseModel                              (http://rdf.bg/ifcdoc/CP64/sdaiCloseModel.html)
//				int_t				model								IN
//
//				void				returns
//
//	This function closes the model. After this call no instance handles will be available including all
//	handles referencing the geometry of this specific file, in default compilation the model itself will
//	be known in the kernel, however known to be disabled. Calls containing the model reference will be
//	protected from crashing when called.
//
void		DECL STDC	sdaiCloseModel(
									int_t				model
								);

//
//		setPrecisionDoubleExport                    (http://rdf.bg/ifcdoc/CP64/setPrecisionDoubleExport.html)
//				int_t				model								IN
//				int_t				precisionCap						IN
//				int_t				precisionRound						IN
//				bool				clean								IN
//
//				void				returns
//
//	...
//
void		DECL STDC	setPrecisionDoubleExport(
									int_t				model,
									int_t				precisionCap,
									int_t				precisionRound,
									bool				clean
								);

//
//  Schema Reading API Calls
//

//
//		sdaiGetEntity                               (http://rdf.bg/ifcdoc/CP64/sdaiGetEntity.html)
//				int_t				model								IN
//				const char			* entityName						IN
//
//				int_t				returns								OUT
//
//	This call retrieves a handle to an entity based on a given entity name.
//
int_t		DECL STDC	sdaiGetEntity(
									int_t				model,
									const char			* entityName
								);

//
//		engiGetEntityArgument                       (http://rdf.bg/ifcdoc/CP64/engiGetEntityArgument.html)
//				int_t				entity								IN
//				const char			* argumentName						IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	engiGetEntityArgument(
									int_t				entity,
									const char			* argumentName
								);

//
//		engiGetEntityArgumentIndex                  (http://rdf.bg/ifcdoc/CP64/engiGetEntityArgumentIndex.html)
//				int_t				entity								IN
//				const char			* argumentName						IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	engiGetEntityArgumentIndex(
									int_t				entity,
									const char			* argumentName
								);

//
//		engiGetEntityArgumentName                   (http://rdf.bg/ifcdoc/CP64/engiGetEntityArgumentName.html)
//				int_t				entity								IN
//				int_t				index								IN
//				int_t				valueType							IN
//				char				** argumentName						IN / OUT
//
//				void				returns
//
//	This call can be used to retrieve the name of the n-th argument of the given entity. Arguments of parent entities are included in the index. Both direct and inverse arguments are included.
//
void		DECL STDC	engiGetEntityArgumentName(
									int_t				entity,
									int_t				index,
									int_t				valueType,
									char				** argumentName
								);

//
//		engiGetEntityArgumentType                   (http://rdf.bg/ifcdoc/CP64/engiGetEntityArgumentType.html)
//				int_t				entity								IN
//				int_t				index								IN
//				int_t				* argumentType						IN / OUT
//
//				void				returns
//
//	This call can be used to retrieve the type of the n-th argument of the given entity. In case of a select argument no relevant information is given by this call as it depends on the instance. Arguments of parent entities are included in the index. Both direct and inverse arguments are included.
//
void		DECL STDC	engiGetEntityArgumentType(
									int_t				entity,
									int_t				index,
									int_t				* argumentType
								);

//
//		engiGetEntityCount                          (http://rdf.bg/ifcdoc/CP64/engiGetEntityCount.html)
//				int_t				model								IN
//
//				int_t				returns								OUT
//
//	Returns the total number of entities within the loaded schema.
//
int_t		DECL STDC	engiGetEntityCount(
									int_t				model
								);

//
//		engiGetEntityElement                        (http://rdf.bg/ifcdoc/CP64/engiGetEntityElement.html)
//				int_t				model								IN
//				int_t				index								IN
//
//				int_t				returns								OUT
//
//	This call returns a specific entity based on an index, the index needs to be 0 or higher but lower then the number of entities in the loaded schema.
//
int_t		DECL STDC	engiGetEntityElement(
									int_t				model,
									int_t				index
								);

//
//		sdaiGetEntityExtent                         (http://rdf.bg/ifcdoc/CP64/sdaiGetEntityExtent.html)
//				int_t				model								IN
//				int_t				entity								IN
//
//				int_t				* returns							OUT
//
//	This call retrieves an aggregation that contains all instances of the entity given.
//
int_t		DECL * STDC	sdaiGetEntityExtent(
									int_t				model,
									int_t				entity
								);

//
//		sdaiGetEntityExtentBN                       (http://rdf.bg/ifcdoc/CP64/sdaiGetEntityExtentBN.html)
//				int_t				model								IN
//				const char			* entityName						IN
//
//				int_t				* returns							OUT
//
//	This call retrieves an aggregation that contains all instances of the entity given.
//
int_t		DECL * STDC	sdaiGetEntityExtentBN(
									int_t				model,
									const char			* entityName
								);

//
//		engiGetEntityName                           (http://rdf.bg/ifcdoc/CP64/engiGetEntityName.html)
//				int_t				entity								IN
//				int_t				valueType							IN
//				char				** entityName						IN / OUT
//
//				void				returns
//
//	This call can be used to get the name of the given entity.
//
void		DECL STDC	engiGetEntityName(
									int_t				entity,
									int_t				valueType,
									char				** entityName
								);

//
//		engiGetEntityNoArguments                    (http://rdf.bg/ifcdoc/CP64/engiGetEntityNoArguments.html)
//				int_t				entity								IN
//
//				int_t				returns								OUT
//
//	This call returns the number of arguments, this includes the arguments of its (nested) parents and inverse argumnets.
//
int_t		DECL STDC	engiGetEntityNoArguments(
									int_t				entity
								);

//
//		engiGetEntityParent                         (http://rdf.bg/ifcdoc/CP64/engiGetEntityParent.html)
//				int_t				entity								IN
//
//				int_t				returns								OUT
//
//	Returns the direct parent entity, for example the parent of IfcObject is IfcObjectDefinition, of IfcObjectDefinition is IfcRoot and of IfcRoot is 0.
//
int_t		DECL STDC	engiGetEntityParent(
									int_t				entity
								);

//
//		engiGetAttrOptional                         (http://rdf.bg/ifcdoc/CP64/engiGetAttrOptional.html)
//				const void			* attribute							IN
//
//				int_t				returns								OUT
//
//	This call can be used to check if an attribute is optional
//
int_t		DECL STDC	engiGetAttrOptional(
									const void			* attribute
								);

//
//		engiGetAttrOptionalBN                       (http://rdf.bg/ifcdoc/CP64/engiGetAttrOptionalBN.html)
//				int_t				entity								IN
//				const char			* attributeName						IN
//
//				int_t				returns								OUT
//
//	This call can be used to check if an attribute is optional
//
int_t		DECL STDC	engiGetAttrOptionalBN(
									int_t				entity,
									const char			* attributeName
								);

//
//		engiGetAttrInverse                          (http://rdf.bg/ifcdoc/CP64/engiGetAttrInverse.html)
//				const void			* attribute							IN
//
//				int_t				returns								OUT
//
//	This call can be used to check if an attribute is an inverse relation
//
int_t		DECL STDC	engiGetAttrInverse(
									const void			* attribute
								);

//
//		engiGetAttrInverseBN                        (http://rdf.bg/ifcdoc/CP64/engiGetAttrInverseBN.html)
//				int_t				entity								IN
//				const char			* attributeName						IN
//
//				int_t				returns								OUT
//
//	This call can be used to check if an attribute is an inverse relation
//
int_t		DECL STDC	engiGetAttrInverseBN(
									int_t				entity,
									const char			* attributeName
								);

//
//		engiGetEnumerationValue                     (http://rdf.bg/ifcdoc/CP64/engiGetEnumerationValue.html)
//				int_t				attribute							IN
//				int_t				index								IN
//				int_t				valueType							IN
//				char				** enumerationValue					IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	engiGetEnumerationValue(
									int_t				attribute,
									int_t				index,
									int_t				valueType,
									char				** enumerationValue
								);

//
//		engiGetEntityProperty                       (http://rdf.bg/ifcdoc/CP64/engiGetEntityProperty.html)
//				int_t				entity								IN
//				int_t				index								IN
//				char				** propertyName						IN / OUT
//				int_t				* optional							IN / OUT
//				int_t				* type								IN / OUT
//				int_t				* _array							IN / OUT
//				int_t				* set								IN / OUT
//				int_t				* list								IN / OUT
//				int_t				* bag								IN / OUT
//				int_t				* min								IN / OUT
//				int_t				* max								IN / OUT
//				int_t				* referenceEntity					IN / OUT
//				int_t				* inverse							IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	engiGetEntityProperty(
									int_t				entity,
									int_t				index,
									char				** propertyName,
									int_t				* optional,
									int_t				* type,
									int_t				* _array,
									int_t				* set,
									int_t				* list,
									int_t				* bag,
									int_t				* min,
									int_t				* max,
									int_t				* referenceEntity,
									int_t				* inverse
								);

//
//  Instance Header API Calls
//

//
//		SetSPFFHeader                               (http://rdf.bg/ifcdoc/CP64/SetSPFFHeader.html)
//				int_t				model								IN
//				const char			* description						IN
//				const char			* implementationLevel				IN
//				const char			* name								IN
//				const char			* timeStamp							IN
//				const char			* author							IN
//				const char			* organization						IN
//				const char			* preprocessorVersion				IN
//				const char			* originatingSystem					IN
//				const char			* authorization						IN
//				const char			* fileSchema						IN
//
//				void				returns
//
//	This call is an aggregate of several SetSPFFHeaderItem calls. In several cases the header can be set easily with this call. In case an argument is zero, this argument will not be updated, i.e. it will not be filled with 0.
//
void		DECL STDC	SetSPFFHeader(
									int_t				model,
									const char			* description,
									const char			* implementationLevel,
									const char			* name,
									const char			* timeStamp,
									const char			* author,
									const char			* organization,
									const char			* preprocessorVersion,
									const char			* originatingSystem,
									const char			* authorization,
									const char			* fileSchema
								);

//
//		SetSPFFHeaderItem                           (http://rdf.bg/ifcdoc/CP64/SetSPFFHeaderItem.html)
//				int_t				model								IN
//				int_t				itemIndex							IN
//				int_t				itemSubIndex						IN
//				int_t				valueType							IN
//				const char			* value								IN
//
//				int_t				returns								OUT
//
//	This call can be used to write a specific header item, the source code example is larger to show and explain how this call can be used.
//
int_t		DECL STDC	SetSPFFHeaderItem(
									int_t				model,
									int_t				itemIndex,
									int_t				itemSubIndex,
									int_t				valueType,
									const char			* value
								);

//
//		GetSPFFHeaderItem                           (http://rdf.bg/ifcdoc/CP64/GetSPFFHeaderItem.html)
//				int_t				model								IN
//				int_t				itemIndex							IN
//				int_t				itemSubIndex						IN
//				int_t				valueType							IN
//				char				** value							IN / OUT
//
//				int_t				returns								OUT
//
//	This call can be used to read a specific header item, the source code example is larger to show and explain how this call can be used.
//
int_t		DECL STDC	GetSPFFHeaderItem(
									int_t				model,
									int_t				itemIndex,
									int_t				itemSubIndex,
									int_t				valueType,
									char				** value
								);

//
//		GetSPFFHeaderItemUnicode                    (http://rdf.bg/ifcdoc/CP64/GetSPFFHeaderItemUnicode.html)
//				int_t				model								IN
//				int_t				itemIndex							IN
//				int_t				itemSubIndex						IN
//				unsigned char		* buffer							IN / OUT
//				int_t				bufferLength						IN
//
//				int_t				returns								OUT
//
//	This call can be used to write a specific header item, the source code example is larger to show and explain how this call can be used.
//
int_t		DECL STDC	GetSPFFHeaderItemUnicode(
									int_t				model,
									int_t				itemIndex,
									int_t				itemSubIndex,
									unsigned char		* buffer,
									int_t				bufferLength
								);

//
//  Instance Reading API Calls
//

//
//		sdaiGetADBType                              (http://rdf.bg/ifcdoc/CP64/sdaiGetADBType.html)
//				const void			* ADB								IN
//
//				int_t				returns								OUT
//
//	This call can be used to get the used type within this ADB type.
//
int_t		DECL STDC	sdaiGetADBType(
									const void			* ADB
								);

//
//		sdaiGetADBTypePath                          (http://rdf.bg/ifcdoc/CP64/sdaiGetADBTypePath.html)
//				const void			* ADB								IN
//				int_t				typeNameNumber						IN
//
//				char				* returns							OUT
//
//	This call can be used to get the path of an ADB type.
//
char		DECL * STDC	sdaiGetADBTypePath(
									const void			* ADB,
									int_t				typeNameNumber
								);

//
//		sdaiGetADBTypePathx                         (http://rdf.bg/ifcdoc/CP64/sdaiGetADBTypePathx.html)
//				const void			* ADB								IN
//				int_t				typeNameNumber						IN
//				char				** path								IN / OUT
//
//				void				returns
//
//	This call is the same as sdaiGetADBTypePath, however can be used by porting to languages that have issues with returned char arrays.
//
void		DECL STDC	sdaiGetADBTypePathx(
									const void			* ADB,
									int_t				typeNameNumber,
									char				** path
								);

//
//		sdaiGetADBValue                             (http://rdf.bg/ifcdoc/CP64/sdaiGetADBValue.html)
//				const void			* ADB								IN
//				int_t				valueType							IN
//				void				* value								IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiGetADBValue(
									const void			* ADB,
									int_t				valueType,
									void				* value
								);

//
//		sdaiCreateEmptyADB                          (http://rdf.bg/ifcdoc/CP64/sdaiCreateEmptyADB.html)
//				void				* returns							OUT
//
//	...
//
void		DECL * STDC	sdaiCreateEmptyADB(
								);

//
//		sdaiDeleteADB                               (http://rdf.bg/ifcdoc/CP64/sdaiDeleteADB.html)
//				const void			* ADB								IN
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiDeleteADB(
									const void			* ADB
								);

//
//		engiGetAggrElement                          (http://rdf.bg/ifcdoc/CP64/engiGetAggrElement.html)
//				int_t				* aggregate							IN / OUT
//				int_t				elementIndex						IN
//				int_t				valueType							IN
//				void				* value								IN / OUT
//
//				void				* returns							OUT
//
//	...
//
void		DECL * STDC	engiGetAggrElement(
									int_t				* aggregate,
									int_t				elementIndex,
									int_t				valueType,
									void				* value
								);

//
//		sdaiGetAggrByIndex                          (http://rdf.bg/ifcdoc/CP64/sdaiGetAggrByIndex___.html)
//				int_t				* list								IN / OUT
//				int_t				index								IN
//				int_t				valueType							IN
//				void				* value								IN / OUT
//
//				void				* returns							OUT
//
//	...
//
void		DECL * STDC	sdaiGetAggrByIndex(
									int_t				* list,
									int_t				index,
									int_t				valueType,
									void				* value
								);

//
//		engiGetAggrType                             (http://rdf.bg/ifcdoc/CP64/engiGetAggrType.html)
//				int_t				* aggregate							IN / OUT
//				int_t				* aggragateType						IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	engiGetAggrType(
									int_t				* aggregate,
									int_t				* aggragateType
								);

//
//		engiGetAggrTypex                            (http://rdf.bg/ifcdoc/CP64/engiGetAggrTypex.html)
//				int_t				* aggregate							IN / OUT
//				int_t				* aggragateType						IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	engiGetAggrTypex(
									int_t				* aggregate,
									int_t				* aggragateType
								);

//
//		sdaiGetAttr                                 (http://rdf.bg/ifcdoc/CP64/sdaiGetAttr.html)
//				int_t				instance							IN
//				void				* attribute							IN / OUT
//				int_t				valueType							IN
//				void				* value								IN / OUT
//
//				void				* returns							OUT
//
//	...
//
void		DECL * STDC	sdaiGetAttr(
									int_t				instance,
									void				* attribute,
									int_t				valueType,
									void				* value
								);

//
//		sdaiGetAttrBN                               (http://rdf.bg/ifcdoc/CP64/sdaiGetAttrBN.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//				int_t				valueType							IN
//				void				* value								IN / OUT
//
//				void				* returns							OUT
//
//	...
//
void		DECL * STDC	sdaiGetAttrBN(
									int_t				instance,
									const char			* attributeName,
									int_t				valueType,
									void				* value
								);

//
//		sdaiGetAttrBNUnicode                        (http://rdf.bg/ifcdoc/CP64/sdaiGetAttrBNUnicode.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//				unsigned char		* buffer							IN / OUT
//				int_t				bufferLength						IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiGetAttrBNUnicode(
									int_t				instance,
									const char			* attributeName,
									unsigned char		* buffer,
									int_t				bufferLength
								);

//
//		sdaiGetStringAttrBN                         (http://rdf.bg/ifcdoc/CP64/sdaiGetStringAttrBN.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//
//				char				* returns							OUT
//
//	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiSTRING.
//	This call can be usefull in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
//	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
//
char		DECL * STDC	sdaiGetStringAttrBN(
									int_t				instance,
									const char			* attributeName
								);

//
//		sdaiGetInstanceAttrBN                       (http://rdf.bg/ifcdoc/CP64/sdaiGetInstanceAttrBN.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//
//				int_t				returns								OUT
//
//	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiINSTANCE.
//	This call can be usefull in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
//	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
//
int_t		DECL STDC	sdaiGetInstanceAttrBN(
									int_t				instance,
									const char			* attributeName
								);

//
//		sdaiGetAggregationAttrBN                    (http://rdf.bg/ifcdoc/CP64/sdaiGetAggregationAttrBN.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//
//				int_t				returns								OUT
//
//	This function is a specific version of sdaiGetAttrBN(..), where the valueType is sdaiAGGR.
//	This call can be usefull in case of specific programming languages that cannot map towards sdaiGetAttrBN(..) directly,
//	this function is useless for languages as C, C++, C#, JAVA, VB.NET, Delphi and similar as they are able to map sdaiGetAttrBN(..) directly.
//
int_t		DECL STDC	sdaiGetAggregationAttrBN(
									int_t				instance,
									const char			* attributeName
								);

//
//		sdaiGetAttrDefinition                       (http://rdf.bg/ifcdoc/CP64/sdaiGetAttrDefinition.html)
//				int_t				entity								IN
//				const char			* attributeName						IN
//
//				void				* returns							OUT
//
//	...
//
void		DECL * STDC	sdaiGetAttrDefinition(
									int_t				entity,
									const char			* attributeName
								);

//
//		sdaiGetInstanceType                         (http://rdf.bg/ifcdoc/CP64/sdaiGetInstanceType.html)
//				int_t				instance							IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiGetInstanceType(
									int_t				instance
								);

//
//		sdaiGetMemberCount                          (http://rdf.bg/ifcdoc/CP64/sdaiGetMemberCount.html)
//				int_t				* aggregate							IN / OUT
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiGetMemberCount(
									int_t				* aggregate
								);

//
//		sdaiIsKindOf                                (http://rdf.bg/ifcdoc/CP64/sdaiIsKindOf.html)
//				int_t				instance							IN
//				int_t				entity								IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiIsKindOf(
									int_t				instance,
									int_t				entity
								);

//
//		sdaiIsKindOfBN                              (http://rdf.bg/ifcdoc/CP64/sdaiIsKindOfBN.html)
//				int_t				instance							IN
//				const char			* entityName						IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiIsKindOfBN(
									int_t				instance,
									const char			* entityName
								);

//
//		engiGetAttrType                             (http://rdf.bg/ifcdoc/CP64/engiGetAttrType.html)
//				int_t				instance							IN
//				const void			* attribute							IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	engiGetAttrType(
									int_t				instance,
									const void			* attribute
								);

//
//		engiGetAttrTypeBN                           (http://rdf.bg/ifcdoc/CP64/engiGetAttrTypeBN.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	engiGetAttrTypeBN(
									int_t				instance,
									const char			* attributeName
								);

//
//		sdaiIsInstanceOf                            (http://rdf.bg/ifcdoc/CP64/sdaiIsInstanceOf.html)
//				int_t				instance							IN
//				int_t				entity								IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiIsInstanceOf(
									int_t				instance,
									int_t				entity
								);

//
//		sdaiIsInstanceOfBN                          (http://rdf.bg/ifcdoc/CP64/sdaiIsInstanceOfBN.html)
//				int_t				instance							IN
//				const char			* entityName						IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiIsInstanceOfBN(
									int_t				instance,
									const char			* entityName
								);

//
//		sdaiIsEqual                                 (http://rdf.bg/ifcdoc/CP64/sdaiIsEqual.html)
//				int_t				instanceI							IN
//				int_t				instanceII							IN
//
//				bool				returns								OUT
//
//	...
//
bool		DECL STDC	sdaiIsEqual(
									int_t				instanceI,
									int_t				instanceII
								);

//
//		engiValidateAttr                            (http://rdf.bg/ifcdoc/CP64/engiValidateAttr.html)
//				int_t				instance							IN
//				const void			* attribute							IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	engiValidateAttr(
									int_t				instance,
									const void			* attribute
								);

//
//		engiValidateAttrBN                          (http://rdf.bg/ifcdoc/CP64/engiValidateAttrBN.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	engiValidateAttrBN(
									int_t				instance,
									const char			* attributeName
								);

//
//		engiGetInstanceClassInfo                    (http://rdf.bg/ifcdoc/CP64/engiGetInstanceClassInfo.html)
//				int_t				instance							IN
//
//				char				* returns							OUT
//
//	...
//
char		DECL * STDC	engiGetInstanceClassInfo(
									int_t				instance
								);

//
//		engiGetInstanceClassInfoUC                  (http://rdf.bg/ifcdoc/CP64/engiGetInstanceClassInfoUC.html)
//				int_t				instance							IN
//
//				char				* returns							OUT
//
//	...
//
char		DECL * STDC	engiGetInstanceClassInfoUC(
									int_t				instance
								);

//
//		engiGetInstanceClassInfoEx                  (http://rdf.bg/ifcdoc/CP64/engiGetInstanceClassInfoEx.html)
//				int_t				instance							IN
//				void				** classInfo						IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	engiGetInstanceClassInfoEx(
									int_t				instance,
									void				** classInfo
								);

//
//		engiGetInstanceMetaInfo                     (http://rdf.bg/ifcdoc/CP64/engiGetInstanceMetaInfo.html)
//				int_t				* instance							IN / OUT
//				int_t				* localId							IN / OUT
//				char				** entityName						IN / OUT
//				char				** entityNameUC						IN / OUT
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	engiGetInstanceMetaInfo(
									int_t				* instance,
									int_t				* localId,
									char				** entityName,
									char				** entityNameUC
								);

//
//		sdaiFindInstanceUsers                       (http://rdf.bg/ifcdoc/CP64/sdaiFindInstanceUsers.html)
//				int_t				instance							IN
//				int_t				domain								IN
//				int_t				resultList							IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiFindInstanceUsers(
									int_t				instance,
									int_t				domain,
									int_t				resultList
								);

//
//		sdaiFindInstanceUsedInBN                    (http://rdf.bg/ifcdoc/CP64/sdaiFindInstanceUsedInBN___.html)
//				int_t				instance							IN
//				char				** roleName							IN / OUT
//				int_t				domain								IN
//				int_t				resultList							IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiFindInstanceUsedInBN(
									int_t				* instance,
									const char			* roleName,
									int_t				domain,
									int_t				resultList
								);

//
//  Instance Writing API Calls
//

//
//		sdaiPrepend                                 (http://rdf.bg/ifcdoc/CP64/sdaiPrepend.html)
//				int_t				list								IN
//				int_t				valueType							IN
//				void				* value								IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiPrepend(
									int_t				list,
									int_t				valueType,
									void				* value
								);

//
//		sdaiAppend                                  (http://rdf.bg/ifcdoc/CP64/sdaiAppend.html)
//				int_t				list								IN
//				int_t				valueType							IN
//				void				* value								IN
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiAppend(
									int_t				list,
									int_t				valueType,
									void				* value
								);

//
//		engiAppend                                  (http://rdf.bg/ifcdoc/CP64/engiAppend.html)
//				int_t				list								IN
//				int_t				valueType							IN
//				void				** values							IN
//				int_t				card								IN
//
//				void				returns
//
//	...
//
void		DECL STDC	engiAppend(
									int_t				list,
									int_t				valueType,
									void				** values,
									int_t				card
								);

//
//		sdaiAdd                                     (http://rdf.bg/ifcdoc/CP64/sdaiAdd.html)
//				int_t				list								IN
//				int_t				valueType							IN
//				void				* value								IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiAdd(
									int_t				list,
									int_t				valueType,
									void				* value
								);

//
//		sdaiInsertByIndex                           (http://rdf.bg/ifcdoc/CP64/sdaiInsertByIndex.html)
//				int_t				list								IN
//				int_t				index								IN
//				int_t				valueType							IN
//				void				* value								IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiInsertByIndex(
									int_t				list,
									int_t				index,
									int_t				valueType,
									void				* value
								);

//
//		sdaiCreateADB                               (http://rdf.bg/ifcdoc/CP64/sdaiCreateADB.html)
//				int_t				valueType							IN
//				void				* value								IN / OUT
//
//				void				* returns							OUT
//
//	...
//
void		DECL * STDC	sdaiCreateADB(
									int_t				valueType,
									void				* value
								);

//
//		sdaiCreateAggr                              (http://rdf.bg/ifcdoc/CP64/sdaiCreateAggr.html)
//				int_t				instance							IN
//				const void			* attribute							IN
//
//				int_t				* returns							OUT
//
//	...
//
int_t		DECL * STDC	sdaiCreateAggr(
									int_t				instance,
									const void			* attribute
								);

//
//		sdaiCreateAggrBN                            (http://rdf.bg/ifcdoc/CP64/sdaiCreateAggrBN.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//
//				int_t				* returns							OUT
//
//	...
//
int_t		DECL * STDC	sdaiCreateAggrBN(
									int_t				instance,
									const char			* attributeName
								);

//
//		sdaiCreateNPL                               (http://rdf.bg/ifcdoc/CP64/sdaiCreateNPL___.html)
//				int_t				* returns							OUT
//
//	...
//
int_t		DECL STDC	sdaiCreateNPL(
								);

//
//		sdaiDeleteNPL                               (http://rdf.bg/ifcdoc/CP64/sdaiDeleteNPL___.html)
//				int_t				* list								IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiDeleteNPL(
									int_t				list
								);

//
//		sdaiCreateNestedAggr                        (http://rdf.bg/ifcdoc/CP64/sdaiCreateNestedAggr.html)
//				int_t				* aggr								IN / OUT
//
//				int_t				* returns							OUT
//
//	...
//
int_t		DECL * STDC	sdaiCreateNestedAggr(
									int_t				* aggr
								);

//
//		sdaiCreateInstance                          (http://rdf.bg/ifcdoc/CP64/sdaiCreateInstance.html)
//				int_t				model								IN
//				int_t				entity								IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiCreateInstance(
									int_t				model,
									int_t				entity
								);

//
//		sdaiCreateInstanceBN                        (http://rdf.bg/ifcdoc/CP64/sdaiCreateInstanceBN.html)
//				int_t				model								IN
//				const char			* entityName						IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiCreateInstanceBN(
									int_t				model,
									const char			* entityName
								);

//
//		sdaiDeleteInstance                          (http://rdf.bg/ifcdoc/CP64/sdaiDeleteInstance.html)
//				int_t				instance							IN
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiDeleteInstance(
									int_t				instance
								);

//
//		sdaiPutADBTypePath                          (http://rdf.bg/ifcdoc/CP64/sdaiPutADBTypePath.html)
//				const void			* ADB								IN
//				int_t				pathCount							IN
//				const char			* path								IN
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiPutADBTypePath(
									const void			* ADB,
									int_t				pathCount,
									const char			* path
								);

//
//		sdaiPutAttr                                 (http://rdf.bg/ifcdoc/CP64/sdaiPutAttr.html)
//				int_t				instance							IN
//				const void			* attribute							IN
//				int_t				valueType							IN
//				void				* value								IN
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiPutAttr(
									int_t				instance,
									const void			* attribute,
									int_t				valueType,
									void				* value
								);

//
//		sdaiPutAttrBN                               (http://rdf.bg/ifcdoc/CP64/sdaiPutAttrBN.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//				int_t				valueType							IN
//				void				* value								IN
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiPutAttrBN(
									int_t				instance,
									const char			* attributeName,
									int_t				valueType,
									void				* value
								);

//
//		sdaiUnsetAttr                               (http://rdf.bg/ifcdoc/CP64/sdaiUnsetAttr.html)
//				int_t				instance							IN
//				const void			* attribute							IN
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiUnsetAttr(
									int_t				instance,
									const void			* attribute
								);

//
//		sdaiUnsetAttrBN                             (http://rdf.bg/ifcdoc/CP64/sdaiUnsetAttrBN.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiUnsetAttrBN(
									int_t				instance,
									const char			* attributeName
								);

//
//		sdaiGetInstanceAttrBN                       (http://rdf.bg/ifcdoc/CP64/sdaiGetInstanceAttrBN.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiGetInstanceAttrBN(
									int_t				instance,
									const char			* attributeName
								);

//
//		sdaiGetAggregationAttrBN                    (http://rdf.bg/ifcdoc/CP64/sdaiGetAggregationAttrBN.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiGetAggregationAttrBN(
									int_t				instance,
									const char			* attributeName
								);

//
//		engiSetComment                              (http://rdf.bg/ifcdoc/CP64/engiSetComment.html)
//				int_t				instance							IN
//				const char			* comment							IN
//
//				void				returns
//
//	This call can be used to add a comment to an instance when exporting the content. The comment is available in the exported/saved IFC file.
//
void		DECL STDC	engiSetComment(
									int_t				instance,
									const char			* comment
								);

//
//		engiGetInstanceLocalId                      (http://rdf.bg/ifcdoc/CP64/engiGetInstanceLocalId.html)
//				int_t				instance							IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	engiGetInstanceLocalId(
									int_t				instance
								);

//
//		sdaiTestAttr                                (http://rdf.bg/ifcdoc/CP64/sdaiTestAttr.html)
//				int_t				instance							IN
//				const void			* attribute							IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiTestAttr(
									int_t				instance,
									const void			* attribute
								);

//
//		sdaiTestAttrBN                              (http://rdf.bg/ifcdoc/CP64/sdaiTestAttrBN.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiTestAttrBN(
									int_t				instance,
									const char			* attributeName
								);

//
//		sdaiCreateInstanceEI                        (http://rdf.bg/ifcdoc/CP64/sdaiCreateInstanceEI.html)
//				int_t				model								IN
//				int_t				entity								IN
//				int_t				express_id							IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiCreateInstanceEI(
									int_t				model,
									int_t				entity,
									int_t				express_id
								);

//
//		sdaiCreateInstanceBNEI                      (http://rdf.bg/ifcdoc/CP64/sdaiCreateInstanceBNEI.html)
//				int_t				model								IN
//				const char			* entityName						IN
//				int_t				express_id							IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiCreateInstanceBNEI(
									int_t				model,
									const char			* entityName,
									int_t				express_id
								);

//
//  Controling API Calls
//

//
//		getSegmentation                             (http://rdf.bg/ifcdoc/CP64/getSegmentation.html)
//				int_t				model								IN
//				int_t				* segmentationParts					IN / OUT
//				double				* segmentationLength				IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	getSegmentation(
									int_t				model,
									int_t				* segmentationParts,
									double				* segmentationLength
								);

//
//		setSegmentation                             (http://rdf.bg/ifcdoc/CP64/setSegmentation.html)
//				int_t				model								IN
//				int_t				segmentationParts					IN
//				double				segmentationLength					IN
//
//				void				returns
//
//	...
//
void		DECL STDC	setSegmentation(
									int_t				model,
									int_t				segmentationParts,
									double				segmentationLength
								);

//
//		circleSegments                              (http://rdf.bg/ifcdoc/CP64/circleSegments.html)
//				int_t				circles								IN
//				int_t				smallCircles						IN
//
//				void				returns
//
//	...
//
void		DECL STDC	circleSegments(
									int_t				circles,
									int_t				smallCircles
								);

//
//		setMaximumSegmentationLength                (http://rdf.bg/ifcdoc/CP64/setMaximumSegmentationLength.html)
//				int_t				model								IN
//				double				length								IN
//
//				void				returns
//
//	...
//
void		DECL STDC	setMaximumSegmentationLength(
									int_t				model,
									double				length
								);

//
//		getUnitConversionFactor                     (http://rdf.bg/ifcdoc/CP64/getUnitConversionFactor.html)
//				int_t				model								IN
//				const char			* unitType							IN
//				char				** unitPrefix						IN / OUT
//				char				** unitName							IN / OUT
//				char				** SIUnitName						IN / OUT
//
//				double				returns								OUT
//
//	...
//
double		DECL STDC	getUnitConversionFactor(
									int_t				model,
									const char			* unitType,
									char				** unitPrefix,
									char				** unitName,
									char				** SIUnitName
								);

//
//		setBRepProperties                           (http://rdf.bg/ifcdoc/CP64/setBRepProperties.html)
//				int_t				model								IN
//				int64_t				consistencyCheck					IN
//				double				fraction							IN
//				double				epsilon								IN
//				int_t				maxVerticesSize						IN
//
//				void				returns
//
//	...
//
void		DECL STDC	setBRepProperties(
									int_t				model,
									int64_t				consistencyCheck,
									double				fraction,
									double				epsilon,
									int_t				maxVerticesSize
								);

//
//		cleanMemory                                 (http://rdf.bg/ifcdoc/CP64/cleanMemory.html)
//				int_t				model								IN
//				int_t				mode								IN
//
//				void				returns
//
//	...
//
void		DECL STDC	cleanMemory(
									int_t				model,
									int_t				mode
								);

//
//		internalGetP21Line                          (http://rdf.bg/ifcdoc/CP64/internalGetP21Line.html)
//				int_t				instance							IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	internalGetP21Line(
									int_t				instance
								);

//
//		internalGetInstanceFromP21Line              (http://rdf.bg/ifcdoc/CP64/internalGetInstanceFromP21Line.html)
//				int_t				model								IN
//				int_t				P21Line								IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	internalGetInstanceFromP21Line(
									int_t				model,
									int_t				P21Line
								);

//
//		internalGetXMLID                            (http://rdf.bg/ifcdoc/CP64/internalGetXMLID.html)
//				int_t				instance							IN
//				char				** XMLID							IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	internalGetXMLID(
									int_t				instance,
									char				** XMLID
								);

//
//		setStringUnicode                            (http://rdf.bg/ifcdoc/CP64/setStringUnicode.html)
//				int_t				unicode								IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	setStringUnicode(
									int_t				unicode
								);

//
//		setFilter                                   (http://rdf.bg/ifcdoc/CP64/setFilter.html)
//				int_t				model								IN
//				int_t				setting								IN
//				int_t				mask								IN
//
//				void				returns
//
//	...
//
void		DECL STDC	setFilter(
									int_t				model,
									int_t				setting,
									int_t				mask
								);

//
//		getFilter                                   (http://rdf.bg/ifcdoc/CP64/getFilter.html)
//				int_t				model								IN
//				int_t				mask								IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	getFilter(
									int_t				model,
									int_t				mask
								);

//
//  Uncategorized API Calls
//

//
//		xxxxGetEntityAndSubTypesExtent              (http://rdf.bg/ifcdoc/CP64/xxxxGetEntityAndSubTypesExtent.html)
//				int_t				model								IN
//				int_t				entity								IN
//
//				int_t				* returns							OUT
//
//	...
//
int_t		DECL * STDC	xxxxGetEntityAndSubTypesExtent(
									int_t				model,
									int_t				entity
								);

//
//		xxxxGetEntityAndSubTypesExtentBN            (http://rdf.bg/ifcdoc/CP64/xxxxGetEntityAndSubTypesExtentBN.html)
//				int_t				model								IN
//				const char			* entityName						IN
//
//				int_t				* returns							OUT
//
//	...
//
int_t		DECL * STDC	xxxxGetEntityAndSubTypesExtentBN(
									int_t				model,
									const char			* entityName
								);

//
//		xxxxGetInstancesUsing                       (http://rdf.bg/ifcdoc/CP64/xxxxGetInstancesUsing.html)
//				int_t				instance							IN
//
//				int_t				* returns							OUT
//
//	...
//
int_t		DECL * STDC	xxxxGetInstancesUsing(
									int_t				instance
								);

//
//		xxxxDeleteFromAggregation                   (http://rdf.bg/ifcdoc/CP64/xxxxDeleteFromAggregation.html)
//				int_t				instance							IN
//				int_t				* aggregate							IN
//				int_t				elementIndex						IN
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	xxxxDeleteFromAggregation(
									int_t				instance,
									const int_t			* aggregate,
									int_t				elementIndex
								);

//
//		xxxxGetAttrDefinitionByValue                (http://rdf.bg/ifcdoc/CP64/xxxxGetAttrDefinitionByValue.html)
//				int_t				instance							IN
//				void				* value								IN / OUT
//
//				void				* returns							OUT
//
//	...
//
void		DECL * STDC	xxxxGetAttrDefinitionByValue(
									int_t				instance,
									void				* value
								);

//
//		xxxxGetAttrNameByIndex                      (http://rdf.bg/ifcdoc/CP64/xxxxGetAttrNameByIndex.html)
//				int_t				instance							IN
//				int_t				index								IN
//				char				** name								IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	xxxxGetAttrNameByIndex(
									int_t				instance,
									int_t				index,
									char				** name
								);

//
//		iterateOverInstances                        (http://rdf.bg/ifcdoc/CP64/iterateOverInstances.html)
//				int_t				model								IN
//				int_t				instance							IN
//				int_t				* entity							IN / OUT
//				void				* entityName						IN / OUT
//
//				int_t				returns								OUT
//
//	This function interates over all available instances loaded in memory, it is the fastest way to find all instances.
//	Argument entity and entityName are both optional and if non-zero are filled with respectively the entity handle and entity name as char array.
//
int_t		DECL STDC	iterateOverInstances(
									int_t				model,
									int_t				instance,
									int_t				* entity,
									void				* entityName
								);

//
//		iterateOverProperties                       (http://rdf.bg/ifcdoc/CP64/iterateOverProperties.html)
//				int_t				entity								IN
//				int_t				index								IN
//
//				int_t				returns								OUT
//
//	This function iterated over all available attributes of a specific given entity.
//	This call is typically used in combination with iterateOverInstances(..).
//
int_t		DECL STDC	iterateOverProperties(
									int_t				entity,
									int_t				index
								);

//
//		sdaiGetAggrByIterator                       (http://rdf.bg/ifcdoc/CP64/sdaiGetAggrByIterator.html)
//				int_t				iterator							IN
//				int_t				valueType							IN
//				void				* value								IN / OUT
//
//				void				* returns							OUT
//
//	...
//
void		DECL * STDC	sdaiGetAggrByIterator(
									int_t				iterator,
									int_t				valueType,
									void				* value
								);

//
//		sdaiPutAggrByIterator                       (http://rdf.bg/ifcdoc/CP64/sdaiPutAggrByIterator.html)
//				int_t				iterator							IN
//				int_t				valueType							IN
//				void				* value								IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	sdaiPutAggrByIterator(
									int_t				iterator,
									int_t				valueType,
									void				* value
								);

//
//		internalSetLink                             (http://rdf.bg/ifcdoc/CP64/internalSetLink.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//				int_t				linked_id							IN
//
//				void				returns
//
//	...
//
void		DECL STDC	internalSetLink(
									int_t				instance,
									const char			* attributeName,
									int_t				linked_id
								);

//
//		internalAddAggrLink                         (http://rdf.bg/ifcdoc/CP64/internalAddAggrLink.html)
//				int_t				list								IN
//				int_t				linked_id							IN
//
//				void				returns
//
//	...
//
void		DECL STDC	internalAddAggrLink(
									int_t				list,
									int_t				linked_id
								);

//
//		engiGetNotReferedAggr                       (http://rdf.bg/ifcdoc/CP64/engiGetNotReferedAggr.html)
//				int_t				model								IN
//				int_t				* value								IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	engiGetNotReferedAggr(
									int_t				model,
									int_t				* value
								);

//
//		engiGetAttributeAggr                        (http://rdf.bg/ifcdoc/CP64/engiGetAttributeAggr.html)
//				int_t				instance							IN
//				int_t				* value								IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	engiGetAttributeAggr(
									int_t				instance,
									int_t				* value
								);

//
//		engiGetAggrUnknownElement                   (http://rdf.bg/ifcdoc/CP64/engiGetAggrUnknownElement.html)
//				int_t				* aggregate							IN / OUT
//				int_t				elementIndex						IN
//				int_t				* valueType							IN / OUT
//				void				* value								IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	engiGetAggrUnknownElement(
									int_t				* aggregate,
									int_t				elementIndex,
									int_t				* valueType,
									void				* value
								);

//
//		sdaiErrorQuery                              (http://rdf.bg/ifcdoc/CP64/sdaiErrorQuery.html)
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	sdaiErrorQuery(
								);

//
//  Geometry Kernel related API Calls
//

//
//		owlGetModel                                 (http://rdf.bg/ifcdoc/CP64/owlGetModel.html)
//				int_t				model								IN
//				int64_t				* owlModel							IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	owlGetModel(
									int_t				model,
									int64_t				* owlModel
								);

//
//		owlGetInstance                              (http://rdf.bg/ifcdoc/CP64/owlGetInstance.html)
//				int_t				model								IN
//				int_t				instance							IN
//				int64_t				* owlInstance						IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	owlGetInstance(
									int_t				model,
									int_t				instance,
									int64_t				* owlInstance
								);

//
//		owlBuildInstance                            (http://rdf.bg/ifcdoc/CP64/owlBuildInstance.html)
//				int_t				model								IN
//				int_t				instance							IN
//				int64_t				* owlInstance						IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	owlBuildInstance(
									int_t				model,
									int_t				instance,
									int64_t				* owlInstance
								);

//
//		owlBuildInstances                           (http://rdf.bg/ifcdoc/CP64/owlBuildInstances.html)
//				int_t				model								IN
//				int_t				instance							IN
//				int64_t				* owlInstanceComplete				IN / OUT
//				int64_t				* owlInstanceSolids					IN / OUT
//				int64_t				* owlInstanceVoids					IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	owlBuildInstances(
									int_t				model,
									int_t				instance,
									int64_t				* owlInstanceComplete,
									int64_t				* owlInstanceSolids,
									int64_t				* owlInstanceVoids
								);

//
//		owlGetMappedItem                            (http://rdf.bg/ifcdoc/CP64/owlGetMappedItem.html)
//				int_t				model								IN
//				int_t				instance							IN
//				int64_t				* owlInstance						IN / OUT
//				double				* transformationMatrix				IN / OUT
//
//				void				returns
//
//	...
//
void		DECL STDC	owlGetMappedItem(
									int_t				model,
									int_t				instance,
									int64_t				* owlInstance,
									double				* transformationMatrix
								);

//
//		getInstanceDerivedPropertiesInModelling     (http://rdf.bg/ifcdoc/CP64/getInstanceDerivedPropertiesInModelling.html)
//				int_t				model								IN
//				int_t				instance							IN
//				double				* height							IN / OUT
//				double				* width								IN / OUT
//				double				* thickness							IN / OUT
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	getInstanceDerivedPropertiesInModelling(
									int_t				model,
									int_t				instance,
									double				* height,
									double				* width,
									double				* thickness
								);

//
//		getInstanceDerivedBoundingBox               (http://rdf.bg/ifcdoc/CP64/getInstanceDerivedBoundingBox.html)
//				int_t				model								IN
//				int_t				instance							IN
//				double				* Ox								IN / OUT
//				double				* Oy								IN / OUT
//				double				* Oz								IN / OUT
//				double				* Vx								IN / OUT
//				double				* Vy								IN / OUT
//				double				* Vz								IN / OUT
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	getInstanceDerivedBoundingBox(
									int_t				model,
									int_t				instance,
									double				* Ox,
									double				* Oy,
									double				* Oz,
									double				* Vx,
									double				* Vy,
									double				* Vz
								);

//
//		getInstanceTransformationMatrix             (http://rdf.bg/ifcdoc/CP64/getInstanceTransformationMatrix.html)
//				int_t				model								IN
//				int_t				instance							IN
//				double				* _11								IN / OUT
//				double				* _12								IN / OUT
//				double				* _13								IN / OUT
//				double				* _14								IN / OUT
//				double				* _21								IN / OUT
//				double				* _22								IN / OUT
//				double				* _23								IN / OUT
//				double				* _24								IN / OUT
//				double				* _31								IN / OUT
//				double				* _32								IN / OUT
//				double				* _33								IN / OUT
//				double				* _34								IN / OUT
//				double				* _41								IN / OUT
//				double				* _42								IN / OUT
//				double				* _43								IN / OUT
//				double				* _44								IN / OUT
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	getInstanceTransformationMatrix(
									int_t				model,
									int_t				instance,
									double				* _11,
									double				* _12,
									double				* _13,
									double				* _14,
									double				* _21,
									double				* _22,
									double				* _23,
									double				* _24,
									double				* _31,
									double				* _32,
									double				* _33,
									double				* _34,
									double				* _41,
									double				* _42,
									double				* _43,
									double				* _44
								);

//
//		getInstanceDerivedTransformationMatrix      (http://rdf.bg/ifcdoc/CP64/getInstanceDerivedTransformationMatrix.html)
//				int_t				model								IN
//				int_t				instance							IN
//				double				* _11								IN / OUT
//				double				* _12								IN / OUT
//				double				* _13								IN / OUT
//				double				* _14								IN / OUT
//				double				* _21								IN / OUT
//				double				* _22								IN / OUT
//				double				* _23								IN / OUT
//				double				* _24								IN / OUT
//				double				* _31								IN / OUT
//				double				* _32								IN / OUT
//				double				* _33								IN / OUT
//				double				* _34								IN / OUT
//				double				* _41								IN / OUT
//				double				* _42								IN / OUT
//				double				* _43								IN / OUT
//				double				* _44								IN / OUT
//
//				int_t				returns								OUT
//
//	...
//
int_t		DECL STDC	getInstanceDerivedTransformationMatrix(
									int_t				model,
									int_t				instance,
									double				* _11,
									double				* _12,
									double				* _13,
									double				* _14,
									double				* _21,
									double				* _22,
									double				* _23,
									double				* _24,
									double				* _31,
									double				* _32,
									double				* _33,
									double				* _34,
									double				* _41,
									double				* _42,
									double				* _43,
									double				* _44
								);

//
//		internalGetBoundingBox                      (http://rdf.bg/ifcdoc/CP64/internalGetBoundingBox.html)
//				int_t				model								IN
//				int_t				instance							IN
//
//				void				* returns							OUT
//
//	...
//
void		DECL * STDC	internalGetBoundingBox(
									int_t				model,
									int_t				instance
								);

//
//		internalGetCenter                           (http://rdf.bg/ifcdoc/CP64/internalGetCenter.html)
//				int_t				model								IN
//				int_t				instance							IN
//
//				void				* returns							OUT
//
//	...
//
void		DECL * STDC	internalGetCenter(
									int_t				model,
									int_t				instance
								);

//
//  Deprecated API Calls (GENERIC)
//

//
//		engiAttrIsInverse                           (http://rdf.bg/ifcdoc/CP64/engiAttrIsInverse.html)
//				const void			* attribute							IN
//
//				int_t				returns								OUT
//
//	This call is deprecated, please use call engiAttrIsInverse.
//
int_t		DECL STDC	engiAttrIsInverse(
									const void			* attribute
								);

//
//		xxxxOpenModelByStream                       (http://rdf.bg/ifcdoc/CP64/xxxxOpenModelByStream.html)
//				int_t				repository							IN
//				const void			* callback							IN
//				const char			* schemaName						IN
//
//				int_t				returns								OUT
//
//	This call is deprecated, please use call engiOpenModelByStream.
//
int_t		DECL STDC	xxxxOpenModelByStream(
									int_t				repository,
									const void			* callback,
									const char			* schemaName
								);

//
//		sdaiCreateIterator                          (http://rdf.bg/ifcdoc/CP64/sdaiCreateIterator.html)
//				const void			* aggregate							IN
//
//				void				* returns							OUT
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
//
void		DECL * STDC	sdaiCreateIterator(
									const void			* aggregate
								);

//
//		sdaiDeleteIterator                          (http://rdf.bg/ifcdoc/CP64/sdaiDeleteIterator.html)
//				int_t				iterator							IN
//
//				void				returns
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
//
void		DECL STDC	sdaiDeleteIterator(
									int_t				iterator
								);

//
//		sdaiBeginning                               (http://rdf.bg/ifcdoc/CP64/sdaiBeginning.html)
//				int_t				iterator							IN
//
//				void				returns
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
//
void		DECL STDC	sdaiBeginning(
									int_t				iterator
								);

//
//		sdaiNext                                    (http://rdf.bg/ifcdoc/CP64/sdaiNext.html)
//				int_t				iterator							IN
//
//				int_t				returns								OUT
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
//
int_t		DECL STDC	sdaiNext(
									int_t				iterator
								);

//
//		sdaiPrevious                                (http://rdf.bg/ifcdoc/CP64/sdaiPrevious.html)
//				int_t				iterator							IN
//
//				int_t				returns								OUT
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
//
int_t		DECL STDC	sdaiPrevious(
									int_t				iterator
								);

//
//		sdaiEnd                                     (http://rdf.bg/ifcdoc/CP64/sdaiEnd.html)
//				int_t				iterator							IN
//
//				void				returns
//
//	This call is deprecated, please use calls sdaiGetMemberCount(..) and engiGetEntityElement(..).
//
void		DECL STDC	sdaiEnd(
									int_t				iterator
								);

//
//		sdaiplusGetAggregationType                  (http://rdf.bg/ifcdoc/CP64/sdaiplusGetAggregationType.html)
//				int_t				instance							IN
//				int_t				* aggregation						IN
//
//				int_t				returns								OUT
//
//	This call is deprecated, please use call ....
//
int_t		DECL STDC	sdaiplusGetAggregationType(
									int_t				instance,
									const int_t			* aggregation
								);

//
//		xxxxGetAttrTypeBN                           (http://rdf.bg/ifcdoc/CP64/xxxxGetAttrTypeBN.html)
//				int_t				instance							IN
//				const char			* attributeName						IN
//				char				** attributeType					IN / OUT
//
//				int_t				returns								OUT
//
//	This call is deprecated, please use calls engiGetAttrTypeBN(..).
//
int_t		DECL STDC	xxxxGetAttrTypeBN(
									int_t				instance,
									const char			* attributeName,
									char				** attributeType
								);

//
//  Deprecated API Calls (GEOMETRY)
//

//
//		initializeModellingInstance                 (http://rdf.bg/ifcdoc/CP64/initializeModellingInstance.html)
//				int_t				model								IN
//				int_t				* noVertices						IN / OUT
//				int_t				* noIndices							IN / OUT
//				double				scale								IN
//				int_t				instance							IN
//
//				int_t				returns								OUT
//
//	This call is deprecated, please use call CalculateInstance().
//
int_t		DECL STDC	initializeModellingInstance(
									int_t				model,
									int_t				* noVertices,
									int_t				* noIndices,
									double				scale,
									int_t				instance
								);

//
//		finalizeModelling                           (http://rdf.bg/ifcdoc/CP64/finalizeModelling.html)
//				int_t				model								IN
//				float				* vertices							IN / OUT
//				int_t				* indices							IN / OUT
//				int_t				FVF									IN
//
//				int_t				returns								OUT
//
//	This call is deprecated, please use call UpdateInstanceVertexBuffer() and UpdateInstanceIndexBuffer().
//
int_t		DECL STDC	finalizeModelling(
									int_t				model,
									float				* vertices,
									int_t				* indices,
									int_t				FVF
								);

//
//		getInstanceInModelling                      (http://rdf.bg/ifcdoc/CP64/getInstanceInModelling.html)
//				int_t				model								IN
//				int_t				instance							IN
//				int_t				mode								IN
//				int_t				* startVertex						IN / OUT
//				int_t				* startIndex						IN / OUT
//				int_t				* primitiveCount					IN / OUT
//
//				int_t				returns								OUT
//
//	This call is deprecated, there is no direct / easy replacement although the functionality is present. If you still use this call please contact RDF to find a solution together.
//
int_t		DECL STDC	getInstanceInModelling(
									int_t				model,
									int_t				instance,
									int_t				mode,
									int_t				* startVertex,
									int_t				* startIndex,
									int_t				* primitiveCount
								);

//
//		setVertexOffset                             (http://rdf.bg/ifcdoc/CP64/setVertexOffset.html)
//				int_t				model								IN
//				double				x									IN
//				double				y									IN
//				double				z									IN
//
//				void				returns
//
//	This call is deprecated, please use call SetVertexBufferOffset().
//
void		DECL STDC	setVertexOffset(
									int_t				model,
									double				x,
									double				y,
									double				z
								);

//
//		setFormat                                   (http://rdf.bg/ifcdoc/CP64/setFormat.html)
//				int_t				model								IN
//				int_t				setting								IN
//				int_t				mask								IN
//
//				void				returns
//
//	This call is deprecated, please use call SetFormat().
//
void		DECL STDC	setFormat(
									int_t				model,
									int_t				setting,
									int_t				mask
								);

//
//		getConceptualFaceCnt                        (http://rdf.bg/ifcdoc/CP64/getConceptualFaceCnt.html)
//				int_t				instance							IN
//
//				int_t				returns								OUT
//
//	This call is deprecated, please use call GetConceptualFaceCnt().
//
int_t		DECL STDC	getConceptualFaceCnt(
									int_t				instance
								);

//
//		getConceptualFaceEx                         (http://rdf.bg/ifcdoc/CP64/getConceptualFaceEx.html)
//				int_t				instance							IN
//				int_t				index								IN
//				int_t				* startIndexTriangles				IN / OUT
//				int_t				* noIndicesTriangles				IN / OUT
//				int_t				* startIndexLines					IN / OUT
//				int_t				* noIndicesLines					IN / OUT
//				int_t				* startIndexPoints					IN / OUT
//				int_t				* noIndicesPoints					IN / OUT
//				int_t				* startIndexFacesPolygons			IN / OUT
//				int_t				* noIndicesFacesPolygons			IN / OUT
//				int_t				* startIndexConceptualFacePolygons	IN / OUT
//				int_t				* noIndicesConceptualFacePolygons	IN / OUT
//
//				int_t				returns								OUT
//
//	This call is deprecated, please use call GetConceptualFaceEx().
//
int_t		DECL STDC	getConceptualFaceEx(
									int_t				instance,
									int_t				index,
									int_t				* startIndexTriangles,
									int_t				* noIndicesTriangles,
									int_t				* startIndexLines,
									int_t				* noIndicesLines,
									int_t				* startIndexPoints,
									int_t				* noIndicesPoints,
									int_t				* startIndexFacesPolygons,
									int_t				* noIndicesFacesPolygons,
									int_t				* startIndexConceptualFacePolygons,
									int_t				* noIndicesConceptualFacePolygons
								);

//
//		createGeometryConversion                    (http://rdf.bg/ifcdoc/CP64/createGeometryConversion.html)
//				int_t				instance							IN
//				int64_t				* owlInstance						IN / OUT
//
//				void				returns
//
//	This call is deprecated, please use call ... .
//
void		DECL STDC	createGeometryConversion(
									int_t				instance,
									int64_t				* owlInstance
								);

//
//		convertInstance                             (http://rdf.bg/ifcdoc/CP64/convertInstance.html)
//				int_t				instance							IN
//
//				void				returns
//
//	This call is deprecated, please use call ... .
//
void		DECL STDC	convertInstance(
									int_t				instance
								);

//
//		initializeModellingInstanceEx               (http://rdf.bg/ifcdoc/CP64/initializeModellingInstanceEx.html)
//				int_t				model								IN
//				int_t				* noVertices						IN / OUT
//				int_t				* noIndices							IN / OUT
//				double				scale								IN
//				int_t				instance							IN
//				int_t				instanceList						IN
//
//				int_t				returns								OUT
//
//	This call is deprecated, please use call CalculateInstance().
//
int_t		DECL STDC	initializeModellingInstanceEx(
									int_t				model,
									int_t				* noVertices,
									int_t				* noIndices,
									double				scale,
									int_t				instance,
									int_t				instanceList
								);

//
//		exportModellingAsOWL                        (http://rdf.bg/ifcdoc/CP64/exportModellingAsOWL.html)
//				int_t				model								IN
//				const char			* fileName							IN
//
//				void				returns
//
//	This call is deprecated, please contact us if you use this call.
//
void		DECL STDC	exportModellingAsOWL(
									int_t				model,
									const char			* fileName
								);

int_t		DECL STDC	getGlobalPlacement(
									int_t				model,
									double				* origin
								);

int_t		DECL STDC	setGlobalPlacement(
									int_t				model,
									double				* origin,
									bool				includeRotation
								);

int_t		DECL STDC	getTimeStamp(
									int_t				model
								);

int_t		DECL STDC	setInstanceReference(
									int_t				instance,
									int_t				value
								);

int_t		DECL STDC	getInstanceReference(
									int_t				instance
								);


#ifdef __cplusplus
	}
#endif


#undef DECL
#undef STDC
#endif
