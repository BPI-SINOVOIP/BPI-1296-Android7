/* otf.h -- Header file for libotf (OpenType font library).

Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009
  National Institute of Advanced Industrial Science and Technology (AIST)
  Registration Number H15PRO167

This file is part of libotf.

Libotf is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

Libotf is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library, in a file named COPYING; if not,
write to the Free Software Foundation, Inc., 59 Temple Place, Suite
330, Boston, MA 02111-1307, USA.  */

#ifndef _OTF_H_
#define _OTF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Version name of this library.  */
#define LIBOTF_VERSION "0.9.9"

/* Major version number.  */
#define LIBOTF_MAJOR_VERSION 0
/* Minor version number.  */
#define LIBOTF_MINOR_VERSION 9
/* Release (i.e. patch level) number.  */
#define LIBOTF_RELEASE_NUMBER 9

/***
    Table of contents:

    (1) Structures for OTF Layout tables and OTF itself
    (1-1) Basic types
    (1-2) "head" table
    (1-3) "name" table
    (1-4) "cmap" table
    (1-5) Structures common to GDEF, GSUB, and GPOS
    (1-6) "GDEF" table
    (1-7) Structures for ScriptList, FeatureList, and LookupList
    (1-8) Structures common to GSUB and GPOS
    (1-9) "GSUB" table
    (1-10) "GPOS" table
    (1-11) Structure for OTF

    (2) API for reading OTF
    (2-1) OTF_open(), OTF_open_ft_face()
    (2-2) OTF_close()
    (2-3) OTF_get_table()
    (2-4) OTF_check_table()

    (3) API for driving OTF
    (3-1) Structure for glyph string
    (3-2) OTF_drive_cmap()
    (3-3) OTF_drive_gdef()
    (3-4) OTF_drive_gsub()
    (3-5) OTF_drive_gpos()
    (3-6) OTF_drive_tables()
    (3-7) OTF_get_unicode()
    (3-8) OTF_drive_gsub_alternate()

    (4) API for error handling
    (4-1) Error codes
    (4-2) OTF_perror()

    (5) API miscellaneous

*/


/*** (1) Structures for OTF Layout tables and OTF itself */

/*** (1-1) Basic types */

typedef unsigned OTF_Tag;
typedef unsigned OTF_GlyphID;
typedef unsigned OTF_Offset;

typedef struct
{
  unsigned high;
  unsigned low;
} OTF_Fixed;


/*** (1-2) "head" table */

typedef struct
{
  OTF_Fixed TableVersionNumber;
  OTF_Fixed fontRevision;
  unsigned checkSumAdjustment;
  unsigned magicNumber;
  unsigned flags;
  int unitsPerEm;
} OTF_head;


/*** (1-3) "name" table */

typedef struct
{
  int platformID;
  int encodingID;
  int languageID;
  int nameID;
  int length;
  int offset;

  /* If nonzero, NAME is an ASCII string.  */
  int ascii;
  unsigned char *name;
} OTF_NameRecord;

#define OTF_max_nameID 23

typedef struct
{
  int format;
  int count;
  int stringOffset;
  OTF_NameRecord *nameRecord;
  char *name[OTF_max_nameID + 1];
} OTF_name;


/*** (1-4) "cmap" table */

typedef struct
{
  unsigned char glyphIdArray[256];
} OTF_EncodingSubtable0;

typedef struct
{
  unsigned firstCode;
  unsigned entryCount;
  int idDelta;
  unsigned idRangeOffset;
} OTF_cmapSubHeader;

typedef struct
{
  unsigned short subHeaderKeys[256];
  int subHeaderCount;
  OTF_cmapSubHeader *subHeaders;
  int glyphIndexCount;
  OTF_GlyphID *glyphIndexArray;
} OTF_EncodingSubtable2;

typedef struct
{
  unsigned startCount;
  unsigned endCount;
  int idDelta;
  unsigned idRangeOffset;
} OTF_cmapSegment;

typedef struct
{
  unsigned segCountX2;
  unsigned searchRange;
  unsigned entrySelector;
  unsigned rangeShift;
  OTF_cmapSegment *segments;
  int GlyphCount;
  unsigned *glyphIdArray;
} OTF_EncodingSubtable4;

typedef struct
{
  unsigned firstCode;
  unsigned entryCount;
  unsigned *glyphIdArray;
} OTF_EncodingSubtable6;

typedef struct
{
  unsigned startCharCode;
  unsigned endCharCode;
  unsigned startGlyphID;
} OTF_cmapGroup;

typedef struct
{
  unsigned char is32[8192];
  unsigned nGroups;
  OTF_cmapGroup *Groups;
} OTF_EncodingSubtable8;

typedef struct
{
  unsigned startCharCode;
  unsigned numChars;
  unsigned *glyphs;
} OTF_EncodingSubtable10;

typedef struct
{
  unsigned nGroups;
  OTF_cmapGroup *Groups;
} OTF_EncodingSubtable12;

typedef struct
{
  unsigned unicodeValue;
  unsigned short glyphID;
} OTF_UVSMapping;

typedef struct
{
  unsigned startUnicodeValue;
  unsigned short additionalCount;
} OTF_UnicodeValueRange;

typedef struct
{
  unsigned varSelector;
  unsigned defaultUVSOffset;
  unsigned nonDefaultUVSOffset;
  /* DefaultUVS */
  unsigned numUnicodeValueRanges;
  OTF_UnicodeValueRange *unicodeValueRanges;
  /* NonDefaultUVS */
  unsigned numUVSMappings;
  OTF_UVSMapping *uvsMappings;
} OTF_VariationSelectorRecord;

typedef struct
{
  unsigned nRecords;
  OTF_VariationSelectorRecord *Records;
} OTF_EncodingSubtable14;

typedef struct
{
  unsigned format;
  unsigned length;
  unsigned language;
  union {
    OTF_EncodingSubtable0 *f0;
    OTF_EncodingSubtable2 *f2;
    OTF_EncodingSubtable4 *f4;
    OTF_EncodingSubtable6 *f6;
    OTF_EncodingSubtable8 *f8;
    OTF_EncodingSubtable10 *f10;
    OTF_EncodingSubtable12 *f12;
    OTF_EncodingSubtable14 *f14;
  }f;
} OTF_EncodingSubtable;

typedef struct
{
  unsigned platformID;
  unsigned encodingID;
  unsigned offset;
  OTF_EncodingSubtable subtable;
} OTF_EncodingRecord;

typedef struct
{
  unsigned version;
  unsigned numTables;
  OTF_EncodingRecord *EncodingRecord;
  /* Mapping table: Unicode->GlyphID */
  unsigned short *unicode_table;
  int max_glyph_id;
  /* Mapping table: GlyphID->Unicode */
  unsigned short *decode_table;
} OTF_cmap;


/*** (1-5) Structures common to GDEF, GSUB, GPOS */

typedef struct
{
  OTF_GlyphID Start;
  OTF_GlyphID End;
  unsigned StartCoverageIndex;
} OTF_RangeRecord;

typedef struct
{
  OTF_Offset offset;
  unsigned CoverageFormat;
  unsigned Count;
  union {
    OTF_GlyphID *GlyphArray;
    OTF_RangeRecord *RangeRecord;
  } table;
} OTF_Coverage;

typedef struct
{
  OTF_Offset offset;
  unsigned StartSize;
  unsigned EndSize;
  unsigned DeltaFormat;
  char *DeltaValue;
} OTF_DeviceTable;

typedef struct
{
  OTF_GlyphID Start;
  OTF_GlyphID End;
  unsigned Class;
} OTF_ClassRangeRecord;

typedef struct
{
  OTF_Offset offset;
  unsigned ClassFormat;
  union {
    struct {
      OTF_GlyphID StartGlyph;
      unsigned GlyphCount;
      unsigned *ClassValueArray;
    } f1;
    struct {
      unsigned ClassRangeCount;
      OTF_ClassRangeRecord *ClassRangeRecord;
    } f2;
  } f;
} OTF_ClassDef;


/*** (1-6) "GDEF" table */

typedef struct
{
  OTF_Fixed Version;
  OTF_Offset GlyphClassDef;
  OTF_Offset AttachList;
  OTF_Offset LigCaretList;
  OTF_Offset MarkAttachClassDef;
} OTF_GDEFHeader;

enum OTF_GlyphClassDef
  {
    OTF_GlyphClass0 = 0,
    OTF_GlyphClassBase = 1,
    OTF_GlyphClassLigature = 2,
    OTF_GlyphClassMark = 3,
    OTF_GlyphClassComponent = 4
  };

typedef struct
{
  OTF_Offset offset;
  unsigned PointCount;
  unsigned *PointIndex;
} OTF_AttachPoint;

typedef struct
{
  OTF_Coverage Coverage;
  unsigned GlyphCount;
  OTF_AttachPoint *AttachPoint;
} OTF_AttachList;

typedef struct
{
  OTF_Offset offset;
  unsigned CaretValueFormat;	/* 1, 2, or 3 */
  union {
    union {
      int Coordinate;
    } f1;
    union {
      unsigned CaretValuePoint;
    } f2;
    union {
      int Coordinate;
      OTF_DeviceTable DeviceTable;
    } f3;
  } f;
} OTF_CaretValue;

typedef struct
{
  OTF_Offset offset;
  unsigned CaretCount;
  OTF_CaretValue *CaretValue;
} OTF_LigGlyph;

typedef struct
{
  OTF_Coverage Coverage;
  unsigned LigGlyphCount;
  OTF_LigGlyph *LigGlyph;
} OTF_LigCaretList;

typedef struct
{
  OTF_GDEFHeader header;
  OTF_ClassDef glyph_class_def;
  OTF_AttachList attach_list;
  OTF_LigCaretList lig_caret_list;
  OTF_ClassDef mark_attach_class_def;
} OTF_GDEF;


/*** (1-7) Structures for ScriptList, FeatureList, and LookupList  */

/*** The structure hierarchy

   ScriptList
     ScriptRecord[]
       ScriptTag
     Script[]
       DefaultLangSys
       LangSysRecord[]
         LangSysTag
       LangSys[]
         LookupOrder
	 ReqFeatureIndex
	 FeatureIndex[]

  FeatureList
    FeatureRecored[]
      FeatureTag
    Feature[]
      FeatureParams
      LookupListIndex[]

  LookupList
    LookupOffset[]
    Lookup[]
      LookupType
      LookupFlag
      SubTableOffset[]
      SubTable.gsub[] or SubTable.gpos[]
*/


typedef struct
{
  OTF_Offset LookupOrder;
  unsigned ReqFeatureIndex;
  unsigned FeatureCount;
  unsigned *FeatureIndex;
} OTF_LangSys;

typedef struct
{
  OTF_Tag LangSysTag;
  OTF_Offset LangSys;
} OTF_LangSysRecord;

typedef struct
{
  OTF_Tag ScriptTag;
  OTF_Offset offset;
  OTF_Offset DefaultLangSysOffset;
  OTF_LangSys DefaultLangSys;
  unsigned LangSysCount;
  OTF_LangSysRecord *LangSysRecord;
  OTF_LangSys *LangSys;
} OTF_Script;

typedef struct
{
  OTF_Offset offset;
  unsigned ScriptCount;
  OTF_Script *Script;
} OTF_ScriptList;

typedef struct
{
  OTF_Tag FeatureTag;
  OTF_Offset offset;
  OTF_Offset FeatureParams;
  unsigned LookupCount;
  unsigned *LookupListIndex;
} OTF_Feature;

typedef struct
{
  OTF_Offset offset;
  unsigned FeatureCount;
  OTF_Feature *Feature;
} OTF_FeatureList;

typedef struct OTF_LookupSubTableGSUB OTF_LookupSubTableGSUB;
typedef struct OTF_LookupSubTableGPOS OTF_LookupSubTableGPOS;

enum OTF_LookupFlagBit
  {
    OTF_RightToLeft = 0x0001,
    OTF_IgnoreBaseGlyphs = 0x0002,
    OTF_IgnoreLigatures = 0x0004,
    OTF_IgnoreMarks = 0x0008,
    OTF_Reserved = 0x00F0,
    OTF_MarkAttachmentType = 0xFF00
  };

#define OTF_LookupFlagIgnoreMask \
  (OTF_IgnoreBaseGlyphs | OTF_IgnoreLigatures | OTF_IgnoreMarks)

typedef struct
{
  OTF_Offset offset;
  unsigned LookupType;
  unsigned LookupFlag;
  unsigned SubTableCount;
  OTF_Offset *SubTableOffset;
  union {
    OTF_LookupSubTableGSUB *gsub;
    OTF_LookupSubTableGPOS *gpos;
  } SubTable;
} OTF_Lookup;

typedef struct
{
  OTF_Offset offset;
  unsigned LookupCount;
  OTF_Lookup *Lookup;
} OTF_LookupList;


/*** (1-8) Structures common to GSUB and GPOS */

/* For SubstLookupRecord (GSUB) and PosLookupRecord (GPOS).  */

typedef struct
{
  unsigned SequenceIndex;
  unsigned LookupListIndex;
} OTF_LookupRecord;

typedef struct
{
  OTF_Offset offset;
  unsigned GlyphCount;
  unsigned LookupCount;
  OTF_GlyphID *Input;		/* [<GlyphCount> - 1] */
  OTF_LookupRecord *LookupRecord; /* [<LookupCount>] */
} OTF_Rule;

typedef struct
{
  OTF_Offset offset;
  unsigned RuleCount;
  OTF_Rule *Rule;		/* [<RuleCount>] */
} OTF_RuleSet;

typedef struct
{
  OTF_Offset offset;
  unsigned GlyphCount;
  unsigned LookupCount;
  unsigned *Class;		/* [<GlyphCount> - 1] */
  OTF_LookupRecord *LookupRecord; /* [<LookupCount>] */
} OTF_ClassRule;

typedef struct
{
  OTF_Offset offset;
  unsigned ClassRuleCnt;
  OTF_ClassRule *ClassRule;	/* [<ClassRuleCnt>] */
} OTF_ClassSet;

typedef struct
{
  OTF_Offset offset;
  unsigned BacktrackGlyphCount;
  OTF_GlyphID *Backtrack;
  unsigned InputGlyphCount;
  OTF_GlyphID *Input;
  unsigned LookaheadGlyphCount;
  OTF_GlyphID *LookAhead;
  unsigned LookupCount;
  OTF_LookupRecord *LookupRecord;
} OTF_ChainRule;

typedef struct
{
  OTF_Offset offset;
  unsigned ChainRuleCount;
  OTF_ChainRule *ChainRule;
} OTF_ChainRuleSet;

typedef struct
{
  OTF_Offset offset;
  unsigned BacktrackGlyphCount;
  unsigned *Backtrack;
  unsigned InputGlyphCount;
  unsigned *Input;
  unsigned LookaheadGlyphCount;
  unsigned *LookAhead;
  unsigned LookupCount;
  OTF_LookupRecord *LookupRecord;
} OTF_ChainClassRule;

typedef struct
{
  OTF_Offset offset;
  unsigned ChainClassRuleCnt;
  OTF_ChainClassRule *ChainClassRule;
} OTF_ChainClassSet;


/* Common to OTF_GSUB/GPOS_Context1/2/3.  */

typedef struct
{
  unsigned RuleSetCount;
  OTF_RuleSet *RuleSet;		/* [<RuleSetCount>] */
} OTF_Context1;

typedef struct
{
  OTF_ClassDef ClassDef;
  unsigned ClassSetCnt;
  OTF_ClassSet *ClassSet;	/* [<ClassSetCnt>] */
} OTF_Context2;

typedef struct
{
  unsigned GlyphCount;
  unsigned LookupCount;
  OTF_Coverage *Coverage;	/* [<GlyphCount>] */
  OTF_LookupRecord *LookupRecord; /* [<LookupCount>] */
} OTF_Context3;


/* Common to OTF_GSUB/GPOS_ChainContext1/2/3.  */

typedef struct
{
  unsigned ChainRuleSetCount;
  OTF_ChainRuleSet *ChainRuleSet;
} OTF_ChainContext1;

typedef struct
{
  OTF_ClassDef BacktrackClassDef;
  OTF_ClassDef InputClassDef;
  OTF_ClassDef LookaheadClassDef;
  unsigned ChainClassSetCnt;
  OTF_ChainClassSet *ChainClassSet;
} OTF_ChainContext2;

typedef struct
{
  unsigned BacktrackGlyphCount;
  OTF_Coverage *Backtrack;
  unsigned InputGlyphCount;
  OTF_Coverage *Input;
  unsigned LookaheadGlyphCount;
  OTF_Coverage *LookAhead;
  unsigned LookupCount;
  OTF_LookupRecord *LookupRecord;
} OTF_ChainContext3;

/* Common to OTF_GSUB/GPOS.  */

typedef struct
{
  OTF_Fixed Version;
  OTF_ScriptList ScriptList;
  OTF_FeatureList FeatureList;
  OTF_LookupList LookupList;
} OTF_GSUB_GPOS;

/*** (1-9) "GSUB" table */

typedef struct
{
  int DeltaGlyphID;
} OTF_GSUB_Single1;

typedef struct
{
  unsigned GlyphCount;
  OTF_GlyphID *Substitute;
} OTF_GSUB_Single2;

typedef struct OTF_Sequence OTF_Sequence;

typedef struct
{
  unsigned SequenceCount;
  OTF_Sequence *Sequence;
} OTF_GSUB_Multiple1;

struct OTF_Sequence
{
  OTF_Offset offset;
  unsigned GlyphCount;
  OTF_GlyphID *Substitute;
};

typedef struct OTF_AlternateSet OTF_AlternateSet;

typedef struct
{
  unsigned AlternateSetCount;
  OTF_AlternateSet *AlternateSet;
} OTF_GSUB_Alternate1;

struct OTF_AlternateSet
{
  OTF_Offset offset;
  unsigned GlyphCount;
  OTF_GlyphID *Alternate;
};

typedef struct OTF_LigatureSet OTF_LigatureSet;
typedef struct OTF_Ligature OTF_Ligature;

typedef struct
{
  unsigned LigSetCount;
  OTF_LigatureSet *LigatureSet;
} OTF_GSUB_Ligature1;

struct OTF_LigatureSet
{
  OTF_Offset offset;
  unsigned LigatureCount;
  OTF_Ligature *Ligature;
};

struct OTF_Ligature
{
  OTF_Offset offset;
  OTF_GlyphID LigGlyph;
  unsigned CompCount;
  OTF_GlyphID *Component;
};

typedef OTF_Context1 OTF_GSUB_Context1;

typedef OTF_Context2 OTF_GSUB_Context2;

typedef OTF_Context3 OTF_GSUB_Context3;

typedef OTF_ChainContext1 OTF_GSUB_ChainContext1;

typedef OTF_ChainContext2 OTF_GSUB_ChainContext2;

typedef OTF_ChainContext3 OTF_GSUB_ChainContext3;

typedef struct
{
  unsigned ExtensionLookupType;
  unsigned ExtensionOffset;
  OTF_LookupSubTableGSUB *ExtensionSubtable;
} OTF_GSUB_Extension1;

typedef struct
{
  unsigned BacktrackGlyphCount;
  OTF_Coverage *Backtrack;
  unsigned LookaheadGlyphCount;
  OTF_Coverage *LookAhead;
  unsigned GlyphCount;
  OTF_GlyphID *Substitute;
} OTF_GSUB_ReverseChain1;

struct OTF_LookupSubTableGSUB
{
  unsigned Format;
  OTF_Coverage Coverage;
  union {
    /* LookupType 1 */
    OTF_GSUB_Single1 single1;
    OTF_GSUB_Single2 single2;
    /* LookupType 2 */
    OTF_GSUB_Multiple1 multiple1;
    /* LookupType 3 */
    OTF_GSUB_Alternate1 alternate1;
    /* LookupType 4 */
    OTF_GSUB_Ligature1 ligature1;
    /* LookupType 5 */
    OTF_GSUB_Context1 context1;
    OTF_GSUB_Context2 context2;
    OTF_GSUB_Context3 context3;
    /* LookupType 6 */
    OTF_GSUB_ChainContext1 chain_context1;
    OTF_GSUB_ChainContext2 chain_context2;
    OTF_GSUB_ChainContext3 chain_context3;
    /* LookupType 7 */
    OTF_GSUB_Extension1 extension1;
    /* LookupType 8 */
    OTF_GSUB_ReverseChain1 reverse_chain1;
  } u;
};

typedef OTF_GSUB_GPOS OTF_GSUB;

/*** (1-10) "GPOS" table */

enum OTF_ValueFormat
  {
    OTF_XPlacement = 0x0001,
    OTF_YPlacement = 0x0002,
    OTF_XAdvance = 0x0004,
    OTF_YAdvance = 0x0008,
    OTF_XPlaDevice = 0x0010,
    OTF_YPlaDevice = 0x0020,
    OTF_XAdvDevice = 0x0040,
    OTF_YAdvDevice = 0x0080
  };

typedef struct
{
  int XPlacement;
  int YPlacement;
  int XAdvance;
  int YAdvance;
  OTF_DeviceTable XPlaDevice;
  OTF_DeviceTable YPlaDevice;
  OTF_DeviceTable XAdvDevice;
  OTF_DeviceTable YAdvDevice;
} OTF_ValueRecord;

typedef struct
{
  OTF_Offset offset;
  unsigned AnchorFormat;
  int XCoordinate;
  int YCoordinate;
  union {
    struct {
      unsigned AnchorPoint;
    } f1;
    struct {
      OTF_DeviceTable XDeviceTable;
      OTF_DeviceTable YDeviceTable;
    } f2;
  } f;
} OTF_Anchor;

typedef struct
{
  unsigned Class;
  OTF_Anchor MarkAnchor;
} OTF_MarkRecord;

typedef struct
{
  OTF_Offset offset;
  unsigned MarkCount;
  OTF_MarkRecord *MarkRecord;
} OTF_MarkArray;

typedef struct
{
  unsigned ValueFormat;
  OTF_ValueRecord Value;
} OTF_GPOS_Single1;

typedef struct
{
  unsigned ValueFormat;
  unsigned ValueCount;
  OTF_ValueRecord *Value;  	/* [<ValueCount>] */
} OTF_GPOS_Single2;

typedef struct
{
  OTF_GlyphID SecondGlyph;
  OTF_ValueRecord Value1;
  OTF_ValueRecord Value2;
} OTF_PairValueRecord;

typedef struct
{
  OTF_Offset offset;
  unsigned PairValueCount;
  OTF_PairValueRecord *PairValueRecord;
} OTF_PairSet;

typedef struct
{
  unsigned ValueFormat1;
  unsigned ValueFormat2;
  unsigned PairSetCount;
  OTF_PairSet *PairSet;
} OTF_GPOS_Pair1;

typedef struct
{
  OTF_ValueRecord Value1;
  OTF_ValueRecord Value2;
} OTF_Class2Record;

typedef struct
{
  OTF_Class2Record *Class2Record;
} OTF_Class1Record;

typedef struct
{
  unsigned ValueFormat1;
  unsigned ValueFormat2;
  OTF_ClassDef ClassDef1;
  OTF_ClassDef ClassDef2;
  unsigned Class1Count;
  unsigned Class2Count;
  OTF_Class1Record *Class1Record; /* size: <Class1Count> */
} OTF_GPOS_Pair2;

typedef struct
{
  OTF_Anchor EntryAnchor;
  OTF_Anchor ExitAnchor;
} OTF_EntryExitRecord;

typedef struct
{
  unsigned EntryExitCount;
  OTF_EntryExitRecord *EntryExitRecord;
} OTF_GPOS_Cursive1;

typedef struct
{
  OTF_Anchor *Anchor;
} OTF_AnchorRecord;

typedef struct
{
  OTF_Offset offset;
  unsigned Count;
  OTF_AnchorRecord *AnchorRecord;
} OTF_AnchorArray;

typedef struct
{
  OTF_Coverage BaseCoverage;
  unsigned ClassCount;
  OTF_MarkArray MarkArray;
  OTF_AnchorArray BaseArray;
} OTF_GPOS_MarkBase1;

typedef struct
{
  OTF_Anchor *LigatureAnchor; /* [<ClassCount>] */
} OTF_ComponentRecord;

typedef struct
{
  OTF_Offset offset;
  unsigned ComponentCount;
  OTF_ComponentRecord *ComponentRecord; /* [<ComponentCount>] */
} OTF_LigatureAttach;

typedef struct
{
  OTF_Offset offset;
  unsigned LigatureCount;
  OTF_LigatureAttach *LigatureAttach; /* [<LiagureCount>] */
} OTF_LigatureArray;

typedef struct
{
  OTF_Coverage LigatureCoverage;
  unsigned ClassCount;
  OTF_MarkArray MarkArray;
  OTF_LigatureArray LigatureArray;
} OTF_GPOS_MarkLig1;

typedef struct
{
  OTF_Coverage Mark2Coverage;
  unsigned ClassCount;
  OTF_MarkArray Mark1Array;
  OTF_AnchorArray Mark2Array;
} OTF_GPOS_MarkMark1;

typedef OTF_Context1 OTF_GPOS_Context1;

typedef OTF_Context2 OTF_GPOS_Context2;

typedef OTF_Context3 OTF_GPOS_Context3;

typedef OTF_ChainContext1 OTF_GPOS_ChainContext1;

typedef OTF_ChainContext2 OTF_GPOS_ChainContext2;

typedef OTF_ChainContext3 OTF_GPOS_ChainContext3;

typedef struct
{
  unsigned ExtensionLookupType;
  unsigned ExtensionOffset;
  OTF_LookupSubTableGPOS *ExtensionSubtable;
} OTF_GPOS_Extension1;


struct OTF_LookupSubTableGPOS
{
  unsigned Format;
  OTF_Coverage Coverage;
  union {
    /* LookupType 1 */
    OTF_GPOS_Single1 single1;
    OTF_GPOS_Single2 single2;
    /* LookupType 2 */
    OTF_GPOS_Pair1 pair1;
    OTF_GPOS_Pair2 pair2;
    /* LookupType 3 */
    OTF_GPOS_Cursive1 cursive1;
    /* LookupType 4 */
    OTF_GPOS_MarkBase1 mark_base1;
    /* LookupType 5 */
    OTF_GPOS_MarkLig1 mark_lig1;
    /* LookupType 6 */
    OTF_GPOS_MarkMark1 mark_mark1;
    /* LookupType 7 */
    OTF_GPOS_Context1 context1;
    OTF_GPOS_Context2 context2;
    OTF_GPOS_Context3 context3;
    /* LookupType 8 */
    OTF_GPOS_ChainContext1 chain_context1;
    OTF_GPOS_ChainContext2 chain_context2;
    OTF_GPOS_ChainContext3 chain_context3;
    /* LookupType 9 */
    OTF_GPOS_Extension1 extension1;
  } u;
};

typedef OTF_GSUB_GPOS OTF_GPOS;

/*** (1-11) Structure for OTF */

typedef struct
{
  OTF_Fixed sfnt_version;
  unsigned numTables;
  unsigned searchRange;
  unsigned enterSelector;
  unsigned rangeShift;
} OTF_OffsetTable;

typedef struct
{
  OTF_Tag tag;
  char name[5];
  unsigned checkSum;
  unsigned offset;
  unsigned length;
} OTF_TableDirectory;

typedef struct OTF_InternalData  OTF_InternalData;

typedef struct
{
  char *filename;
  OTF_OffsetTable offset_table;
  OTF_TableDirectory *table_dirs;
  OTF_head *head;
  OTF_name *name;
  OTF_cmap *cmap;
  OTF_GDEF *gdef;
  OTF_GSUB *gsub;
  OTF_GPOS *gpos;
  /* The following tables are not yet supported.  */
  /* OTF_BASE *base; */
  /* OTF_JSTF *jstf; */
  OTF_InternalData *internal_data;
} OTF;


/*** (2) API for reading OTF */

/*** (2-1) otf_open () */

/***
    Open OpenType font

    The OTF_open() function reads the OpenType font file whose name is
    $NAME, and return a pointer to the structure of type OTF.

    It setups these member of the structure OTF:
	filename, offset_table, table_dirs

    If the file can't be read or the file contains invalid data, NULL
    is returned, and the variable OTF_error is set to one of the
    following values.

	OTF_ERROR_MEMORY
	OTF_ERROR_FILE
	OTF_ERROR_TABLE

    See also OTF_get_table() and OTF_close().  */

extern OTF *OTF_open (const char *name);

#include <ft2build.h>
#include FT_FREETYPE_H

extern OTF *OTF_open_ft_face (FT_Face face);


/*** (2-2) OTF_close () */

/***
    Close OpenType font

    The OTF_close() function closes the OpenType font $OTF which must
    be what the OTF_open() function returned.

    See also OTF_open().  */

extern void OTF_close (OTF *otf);


/*** (2-3) OTF_get_table () */

/***
    Get OpenType font table

    The OTF_get_table() function setups one of the OTF table specified
    by $NAME in the OpenType font $OTF.

    $NAME must be one of "head", "name", "cmap", "GDEF", "GSUB", and
    "GPOS", and a member of the same name is setup.

    If the table is successfully setup, return 0.  Otherwise, return
    -1, and set the variable OTF_error to OTF_ERROR_TABLE.

    See also OTF_open().  */

extern int OTF_get_table (OTF *otf, const char *name);

/*** (2-4) OTF_check_table () */

/***
    Check the existence of OpenType font table

    The OTF_check_table() function checks if the the OTF table
    specified by $NAME exists in OpenType font $OTF.

    If the table exists, return 0, else return -1.

    See also OTF_open().  */

extern int OTF_check_table (OTF *otf, const char *name);

/*** (2-5) OTF_get_scripts () */

/***
    Get supported scripts.

    The OTF_get_scripts() function setups OTF_ScriptList of GSUB (if
    $GSUBP is nonzero) or GPOS (if $GSUBP is zero) table of the
    OpenType font $OTF.

    If the table is successfully setup, return 0.  Otherwise, retrun
    -1, and set the variable OTF_error to OTF_ERROR_TABLE.  */

extern int OTF_get_scripts (OTF *otf, int gsubp);

/*** (2-6) OTF_get_features () */

/***
    Get supported features.

    The OTF_get_features() function setups OTF_FeatureList of GSUB (if
    $GSUBP is nonzero) or GPOS (if $GSUBP is zero) table of the
    OpenType font $OTF.

    If the table is successfully setup, return 0.  Otherwise, retrun
    -1, and set the variable OTF_error to OTF_ERROR_TABLE.  */

extern int OTF_get_features (OTF *otf, int gsubp);

/*** (2-7) OTF_check_features  */

/***
    Check supported features.

    The OTF_check_features() function checks if or not the OpenType
    font $OTF has, for $SCRIPT and $LANGUAGE, all features in the
    array $FEATURES.  The array size is $N_FEATURES.  If $LANGUAGE is
    zero or $OTF doesn't have LangSys for $SCRIPT, the default LangSys
    is checked.

    If $OTF has all the features, return 1.  Otherwise, return 0.  If
    an error occurs, return -1, and set the variable OTF_error to
    OTF_ERROR_TABLE.   */

extern int OTF_check_features (OTF *otf, int gsubp,
			       OTF_Tag script, OTF_Tag language,
			       const OTF_Tag *features, int n_features);

/*** (3) API for driving OTF */

/*** (3-1) Structure for glyph string */

/***
    The structure OTF_Glyph contains information about each glyph in
    the structure OTF_GlyphString.  */

typedef struct
{
  /** The first two members must be set by a clinet before calling the
      function OTF_drive_XXX().  **/

  /* Character code of the glyph.  The value less than 32 is treated
     as a place-holder in a glyph string, and OTF_drive_XXX ()
     function just ignore the glyph as if it doesn't exist.  */
  int c;

  /* Glyph ID of the glyph.  If the value is 0, the library gets a
     correct value from the above character code via cmap if such a
     glyph is available in the font.

     The function OTF_drive_gpos2 may insert a glyph whose glyph_id is
     0 but positioning_type is positive.  It is not an actual glyph
     but just contains positioning information that should be
     accumulated to the positioning information of the previous
     glyphs.  */
  OTF_GlyphID glyph_id;

  /* GlyphClass of the glyph.  The value is extracted from the GDEF
     table.  */
  enum OTF_GlyphClassDef GlyphClass;

  /* MarkAttachClassDef of the glyph.  The value is extracted from the
     GDEF table.  */
  unsigned MarkAttachClass;

  /* Positioning format type of the glyph.  The value specifies how
     the glyph positioning information is encoded in the member <f>.
     If the value is N, the union member fN, is used.  If the value is
     zero, the glyph has no positioning information, i.e. it should be
     drawn at the normal position.  */
  int positioning_type;
  union {
    struct {
      int from, to;
    } index;
    struct {
      enum OTF_ValueFormat format;
      OTF_ValueRecord *value;
    } f1;
    struct {
      enum OTF_ValueFormat format;
      OTF_ValueRecord *value;
    } f2;
    struct {
      OTF_Anchor *entry_anchor;
      OTF_Anchor *exit_anchor;
    } f3;
    struct {
      OTF_Anchor *mark_anchor;
      OTF_Anchor *base_anchor;
    } f4;
    struct {
      OTF_Anchor *mark_anchor;
      OTF_Anchor *ligature_anchor;
    } f5;
    struct {
      OTF_Anchor *mark1_anchor;
      OTF_Anchor *mark2_anchor;
    } f6;
  } f;
} OTF_Glyph;

/***
    The structure OTF_GlyphString contains an array of glyphs (type
    OTF_Glyph).  It is used as arguments of otf_drive_XXX().  */

typedef struct
{
  /* How many glyphs are allocated at the memory pointed by the member
      <glyphs>.  */
  int size;
  /* How many glyphs contain valid information.  */
  int used;
  /* Array of glyphs.  It must be allocated by malloc().  The
     functions otf_drive_XXX() may reallocate it and increase the
     members <size> and <used>.  */
  OTF_Glyph *glyphs;
} OTF_GlyphString;


/*** (3-2) OTF_drive_cmap() */

/***
    Process glyph string by Unicode-based cmap table.

    The OTF_drive_cmap() function looks up a Unicode-based cmap table
    of OpenType font $OTF, and setup the member <glyph_id> of all
    glhphs in the glyph string $GSTRING if the value of the member is
    not zero.  */

extern int OTF_drive_cmap (OTF *otf, OTF_GlyphString *gstring);

/***
    Process glyph string by a specific cmap table.

    The OTF_drive_cmap2() function looks up a cmap table (whose
    Platform-ID is $PLATFORM_ID an Encoding-ID is $ENCODING_ID) of
    OpenType font $OTF, and setup the member <glyph_id> of all glhphs
    in the glyph string $GSTRING if the value of the member is not
    zero.  */

extern int OTF_drive_cmap2 (OTF *otf, OTF_GlyphString *gstring,
			    int platform_id, int encoding_id);


/***
    Store variable glyphs of character C in the array CODE.  The array
    size must be 256.  The Nth element of CODE is the glyph corresponding
    to the variation selector (N + 1).  The return value is the number
    of variation glyphs.  */

extern int OTF_get_variation_glyphs (OTF *otf, int c, OTF_GlyphID code[256]);


/*** (3-3) OTF_drive_gdef() */

/***
    Process glyph string by GDEF table.

    The OTF_drive_gdef() function looks up the GDEF table of OpenType
    font $OTF, and setup members <GlyphClass> and <MarkAttachClass> of
    all glhphs in the glyph string $GSTRING.  */

extern int OTF_drive_gdef (OTF *otf, OTF_GlyphString *gstring);


/*** (3-4) OTF_drive_gsub() */

/***
    Process glyph string by GSUB table.

    The OTF_drive_gsub() function looks up the GSUB table of OpenType
    font $OTF, and by using features the font has for script $SCRIPT
    and language system $LANGSYS, update member <glyphs> of the glyph
    string $GSTRING.  It may substitute, delete, insert glyphs in that
    array.  $FEATURES is a list of features to apply.  This doesn't
    perform a lookup of type 3 (Alternate Substitution).  For that,
    use OTF_drive_gsub_alternate().  */

extern int OTF_drive_gsub (OTF *otf, OTF_GlyphString *gstring,
			   const char *script, const char *language,
			   const char *features);

/*** (3-5) OTF_drive_gpos() */

/***
    Process glyph string by GPOS table.

    The OTF_drive_gpos() function is deprecated.  Use
    OTF_drive_gpos2() instread.  */

extern int OTF_drive_gpos (OTF *otf, OTF_GlyphString *gstring,
			   const char *script, const char *language,
			   const char *features);

/***
    Process glyph string by GPOS table.

    The OTF_drive_gpos2() function looks up the GPOS table of $OTF of
    OpenType font $OTF, and by using features the font has for script
    $SCRIPT and language system $LANGSYS, setup members
    <positioning_type> and <f> of all glhphs in the glyph string
    $GSTRING.  $FEATURES is a list of features to apply.  */

extern int OTF_drive_gpos2 (OTF *otf, OTF_GlyphString *gstring,
			    const char *script, const char *language,
			    const char *features);

/*** (3-6) OTF_drive_tables() */

/***
    Process glyph string by cmap, GDEF, GSUB, and GPOS tables.

    The OTF_drive_tables() function calls OTF_drive_cmap(),
    OTF_drive_gdef(), OTF_drive_gsub(), and OTF_drive_gpos() in this
    order, and update the glyphs string GSTRING.  */

extern int OTF_drive_tables (OTF *otf, OTF_GlyphString *gstring,
			     const char *script, const char *language,
			     const char *gsub_features,
			     const char *gpos_features);


/*** (3-7) OTF_get_unicode()  */

/***
    Return Unicode code point corresponding to the glyph-id CODE.
  */

extern int OTF_get_unicode (OTF *otf, OTF_GlyphID code);

/*** (3-8) OTF_drive_gsub_alternate() */

/***
    Find alternate glyphs.

    This is like OTF_drive_gsub(), but perform only a lookup of type 3
    (Alternate Substituion).  */

extern int OTF_drive_gsub_alternate (OTF *otf, OTF_GlyphString *gstring,
				     const char *script, const char *language,
				     const char *features);

/*** (4) API for error handling ***/

/*** (4-1) Error codes ***/

/***
    Global variable holding an error code.

    The variable OTF_error is set to one of OTF_ERROR_XXX macros when
    an error is detected in the OTF library.  */
extern int OTF_error;

/***
    Memory allocation error

    This error indicates that the library couldn't allocate
    memory.  */
#define OTF_ERROR_MEMORY	1

/***
    File error

    This error indicates that the library fails in opening, reading,
    or seeking an OTF file.  */
#define OTF_ERROR_FILE		2

/***
    Invalid table contents

    This error indicates that an OTF file contains invalid data.  */
#define OTF_ERROR_TABLE		3

/***
    CMAP driving error

    See the function otf_drive_cmap() for more detail.  */
#define OTF_ERROR_CMAP_DRIVE	4

/***
    GDEF driving error

    See the function OTF_drive_gdef() for more detail.  */
#define OTF_ERROR_GDEF_DRIVE	5

/***
    GSUB driving error

    See the function OTF_drive_gsub() for more detail.  */
#define OTF_ERROR_GSUB_DRIVE	6

/***
    GPOS driving error

    See the function OTF_drive_gpos() for more detail.  */
#define OTF_ERROR_GPOS_DRIVE	7

/***
    FT_Face access error.

    This error indicates that the library fails in accessing Sfnt
    tables via FT_Face.  */
#define OTF_ERROR_FT_FACE	8


/*** (4-2) OTF_perror() */

/***
    Print an OTF error message

    The OTF_perror() function produces a message on the standard error
    output, describing the last error encountered during a call to the
    OTF library function.  If $PREFIX is not NULL, it is printed
    first, followed by a colon and a blank.  Then the message and a
    newline.  */

extern void OTF_perror (const char *prefix);


/*** (5) API miscellaneous ***/

/***
    Return OTF tag of a specified name string.

    The OTF_tag() function returns OTF tag of name $NAME.  If $NAME is
    NULL, return 0.  Otherwise, $NAME must be at least 4-byte length.
    Only the first 4 characters are took into an account.  */

extern OTF_Tag OTF_tag (const char *name);

/***
    Convert OTF tag to name string.

    The OTF_tag_name() function converts OTF tag $TAG to a 5-byte name
    string (including the terminating NULL), and store it in $NAME.
    At least 5-byte space must be at $NAME.  */

extern void OTF_tag_name (OTF_Tag tag, char *name);

#ifdef __cplusplus
}
#endif

#endif /* not _OTF_H_ */
