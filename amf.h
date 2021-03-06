#ifndef __AMF_H__
#define __AMF_H__
/*
 *      Copyright (C) 2005-2008 Team XBMC
 *      http://www.xbmc.org
 *      Copyright (C) 2008-2009 Andrej Stepanchuk
 *      Copyright (C) 2009-2010 Howard Chu
 *
 *  This file is part of librtmp.
 *
 *  librtmp is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1,
 *  or (at your option) any later version.
 *
 *  librtmp is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with librtmp see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 *  http://www.gnu.org/copyleft/lgpl.html
 */

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

#ifdef LIBRTMP_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		AMF_NUMBER = 0, AMF_BOOLEAN, AMF_STRING, AMF_OBJECT,
		AMF_MOVIECLIP,		/* reserved, not used */
		AMF_NULL, AMF_UNDEFINED, AMF_REFERENCE, AMF_ECMA_ARRAY, AMF_OBJECT_END,
		AMF_STRICT_ARRAY, AMF_DATE, AMF_LONG_STRING, AMF_UNSUPPORTED,
		AMF_RECORDSET,		/* reserved, not used */
		AMF_XML_DOC, AMF_TYPED_OBJECT,
		AMF_AVMPLUS,		/* switch to AMF3 */
		AMF_INVALID = 0xff
	}
	AMFDataType;

	typedef enum
	{
		AMF3_UNDEFINED = 0, AMF3_NULL, AMF3_FALSE, AMF3_TRUE,
		AMF3_INTEGER, AMF3_DOUBLE, AMF3_STRING, AMF3_XML_DOC, AMF3_DATE,
		AMF3_ARRAY, AMF3_OBJECT, AMF3_XML, AMF3_BYTE_ARRAY
	} AMF3DataType;

	typedef struct AVal
	{
		char* av_val;
		int av_len;
	} AVal;
#define AVC(str)	{str,sizeof(str)-1}
#define AVMATCH(a1,a2)	((a1)->av_len == (a2)->av_len && !memcmp((a1)->av_val,(a2)->av_val,(a1)->av_len))

	struct AMFObjectProperty;

	typedef struct AMFObject
	{
		int o_num;
		struct AMFObjectProperty* o_props;
	} AMFObject;

	typedef struct AMFObjectProperty
	{
		AVal p_name;
		AMFDataType p_type;
		union
		{
			double p_number;
			AVal p_aval;
			AMFObject p_object;
		} p_vu;
		int16_t p_UTCoffset;
	} AMFObjectProperty;

	DLL_EXPORT char* AMF_EncodeString(char* output, char* outend, const AVal* str);
	DLL_EXPORT char* AMF_EncodeNumber(char* output, char* outend, double dVal);
	DLL_EXPORT char* AMF_EncodeInt16(char* output, char* outend, short nVal);
	DLL_EXPORT  char* AMF_EncodeInt24(char* output, char* outend, int nVal);
	DLL_EXPORT char* AMF_EncodeInt32(char* output, char* outend, int nVal);
	DLL_EXPORT char* AMF_EncodeBoolean(char* output, char* outend, int bVal);

	/* Shortcuts for AMFProp_Encode */
	DLL_EXPORT char* AMF_EncodeNamedString(char* output, char* outend, const AVal* name, const AVal* value);
	DLL_EXPORT char* AMF_EncodeNamedNumber(char* output, char* outend, const AVal* name, double dVal);
	DLL_EXPORT char* AMF_EncodeNamedBoolean(char* output, char* outend, const AVal* name, int bVal);

	DLL_EXPORT unsigned short AMF_DecodeInt16(const char* data);
	DLL_EXPORT unsigned int AMF_DecodeInt24(const char* data);
	DLL_EXPORT unsigned int AMF_DecodeInt32(const char* data);
	DLL_EXPORT void AMF_DecodeString(const char* data, AVal* str);
	DLL_EXPORT void AMF_DecodeLongString(const char* data, AVal* str);
	DLL_EXPORT int AMF_DecodeBoolean(const char* data);
	DLL_EXPORT double AMF_DecodeNumber(const char* data);

	DLL_EXPORT char* AMF_Encode(AMFObject* obj, char* pBuffer, char* pBufEnd);
	DLL_EXPORT char* AMF_EncodeEcmaArray(AMFObject* obj, char* pBuffer, char* pBufEnd);
	DLL_EXPORT char* AMF_EncodeArray(AMFObject* obj, char* pBuffer, char* pBufEnd);

	DLL_EXPORT int AMF_Decode(AMFObject* obj, const char* pBuffer, int nSize, int bDecodeName);
	DLL_EXPORT int AMF_DecodeArray(AMFObject* obj, const char* pBuffer, int nSize, int nArrayLen, int bDecodeName);
	DLL_EXPORT int AMF3_Decode(AMFObject* obj, const char* pBuffer, int nSize, int bDecodeName);
	DLL_EXPORT void AMF_Dump(AMFObject* obj);
	DLL_EXPORT void AMF_Reset(AMFObject* obj);

	DLL_EXPORT void AMF_AddProp(AMFObject* obj, const AMFObjectProperty* prop);
	DLL_EXPORT int AMF_CountProp(AMFObject* obj);
	DLL_EXPORT AMFObjectProperty* AMF_GetProp(AMFObject* obj, const AVal* name, int nIndex);

	DLL_EXPORT AMFDataType AMFProp_GetType(AMFObjectProperty* prop);
	DLL_EXPORT void AMFProp_SetNumber(AMFObjectProperty* prop, double dval);
	DLL_EXPORT void AMFProp_SetBoolean(AMFObjectProperty* prop, int bflag);
	DLL_EXPORT void AMFProp_SetString(AMFObjectProperty* prop, AVal* str);
	DLL_EXPORT void AMFProp_SetObject(AMFObjectProperty* prop, AMFObject* obj);

	DLL_EXPORT void AMFProp_GetName(AMFObjectProperty* prop, AVal* name);
	DLL_EXPORT void AMFProp_SetName(AMFObjectProperty* prop, AVal* name);
	DLL_EXPORT double AMFProp_GetNumber(AMFObjectProperty* prop);
	DLL_EXPORT int AMFProp_GetBoolean(AMFObjectProperty* prop);
	DLL_EXPORT void AMFProp_GetString(AMFObjectProperty* prop, AVal* str);
	DLL_EXPORT void AMFProp_GetObject(AMFObjectProperty* prop, AMFObject* obj);
	DLL_EXPORT int AMFProp_IsValid(AMFObjectProperty* prop);
	DLL_EXPORT char* AMFProp_Encode(AMFObjectProperty* prop, char* pBuffer, char* pBufEnd);
	DLL_EXPORT int AMF3Prop_Decode(AMFObjectProperty* prop, const char* pBuffer, int nSize, int bDecodeName);
	DLL_EXPORT int AMFProp_Decode(AMFObjectProperty* prop, const char* pBuffer, int nSize, int bDecodeName);
	DLL_EXPORT void AMFProp_Dump(AMFObjectProperty* prop);
	DLL_EXPORT void AMFProp_Reset(AMFObjectProperty* prop);

	typedef struct AMF3ClassDef
	{
		AVal cd_name;
		char cd_externalizable;
		char cd_dynamic;
		int cd_num;
		AVal* cd_props;
	} AMF3ClassDef;

	DLL_EXPORT void AMF3CD_AddProp(AMF3ClassDef* cd, AVal* prop);
	DLL_EXPORT AVal* AMF3CD_GetProp(AMF3ClassDef* cd, int idx);

#ifdef __cplusplus
}
#endif

#endif				/* __AMF_H__ */
