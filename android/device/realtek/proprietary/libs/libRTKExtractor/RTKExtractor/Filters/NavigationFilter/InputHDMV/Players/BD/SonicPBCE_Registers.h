//-----------------------------------------------------------------------------
// SonicPBCE_Registers.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __SONICPBCE_REGISTERS_H__
#define __SONICPBCE_REGISTERS_H__

#include "NotificationEventManager.h"
#include "ref_ptr.h"
#include <string.h>

//-------------------------------------------------------------------------------------------------------
// Registers defines, typedefs, structs, enums
//

#define BDROM_REG_TOTAL_GPR							4096
#define BDROM_REG_TOTAL_PSR							128

// Audio Capability flag mask
//

#define BDROM_ACAP_DOLBYLOSSLESS_MLP_NONE				0x00000000	// Dolby Lossless Capability: MLP Audio - Incapable
#define BDROM_ACAP_DOLBYLOSSLESS_MLP_STEREO				0x00004000	// Dolby Lossless Capability: MLP Audio - Stereo capable
#define BDROM_ACAP_DOLBYLOSSLESS_MLP_SURROUND			0x00008000	// Dolby Lossless Capability: MLP Audio - Surround capable
#define BDROM_ACAP_DOLBYLOSSLESS_DOLBYDIGITAL_STEREO	0x00001000	// Dolby Lossless Capability: Dolby Digital Audio - Stereo capable
#define BDROM_ACAP_DOLBYLOSSLESS_DOLBYDIGITAL_SURROUND	0x00002000	// Dolby Lossless Capability: Dolby Digital Audio - Surround capable

#define BDROM_ACAP_DTSHD_EXTENSION_NONE					0x00000000	// DTS-HD Capability: Extension Substream - Incapable
#define BDROM_ACAP_DTSHD_EXTENSION_STEREO				0x00000400	// DTS-HD Capability: Extension Substream - Stereo capable
#define BDROM_ACAP_DTSHD_EXTENSION_SURROUND				0x00000800	// DTS-HD Capability: Extension Substream - Surround capable
#define BDROM_ACAP_DTSHD_CORE_STEREO					0x00000100	// DTS-HD Capability: Core Substream - Stereo capable
#define BDROM_ACAP_DTSHD_CORE_SURROUND					0x00000200	// DTS-HD Capability: Core Substream - Surround capable

#define BDROM_ACAP_DOLBYDIGITALPLUS_DEP_NONE			0x00000000	// Dolby Digital Plus Capability: Dependent substream - Incapable
#define BDROM_ACAP_DOLBYDIGITALPLUS_DEP_STEREO			0x00000040	// Dolby Digital Plus Capability: Dependent substream - Stereo capable
#define BDROM_ACAP_DOLBYDIGITALPLUS_DEP_SURROUND		0x00000080	// Dolby Digital Plus Capability: Dependent substream - Surround capable
#define BDROM_ACAP_DOLBYDIGITALPLUS_INDEP_STEREO		0x00000010	// Dolby Digital Plus Capability: Independent substream (Dolby Digital) - Stereo capable
#define BDROM_ACAP_DOLBYDIGITALPLUS_INDEP_SURROUND		0x00000020	// Dolby Digital Plus Capability: Independent substream (Dolby Digital) - Surround capable

#define BDROM_ACAP_LPCM_192kHz_NONE						0x00000000	// LPCM Capability: 192 kHz - Incapable
#define BDROM_ACAP_LPCM_192kHz_STEREO					0x00000004	// LPCM Capability: 192 kHz - Stereo capable
#define BDROM_ACAP_LPCM_192kHz_SURROUND					0x00000008	// LPCM Capability: 192 kHz - Surround capable
#define BDROM_ACAP_LPCM_48kHz96kHz_STEREO				0x00000001	// LPCM Capability: 48 kHz and 96 kHz - Stereo capable
#define BDROM_ACAP_LPCM_48kHz96kHz_SURROUND				0x00000002	// LPCM Capability: 48 kHz and 96 kHz - Surround capable

// capability extraction macros
//
#define IS_ACAP_LPCM_48kHz96kHz_STEREO(ac)				((ac & BDROM_ACAP_LPCM_48kHz96kHz_STEREO) > 0)
#define IS_ACAP_LPCM_48kHz96kHz_SURROUND(ac)			((ac & BDROM_ACAP_LPCM_48kHz96kHz_SURROUND) > 0)

#define IS_ACAP_LPCM_192kHz(ac)							((ac & (BDROM_ACAP_LPCM_192kHz_STEREO | BDROM_ACAP_LPCM_192kHz_SURROUND)) > 0)
#define IS_ACAP_LPCM_192kHz_STEREO(ac)					((ac & BDROM_ACAP_LPCM_192kHz_STEREO) > 0)
#define IS_ACAP_LPCM_192kHz_SURROUND(ac)				((ac & BDROM_ACAP_LPCM_192kHz_SURROUND) > 0)

#define IS_ACAP_DOLBYDIGITALPLUS_INDEP_STEREO(ac)		((ac & BDROM_ACAP_DOLBYDIGITALPLUS_INDEP_STEREO) > 0)
#define IS_ACAP_DOLBYDIGITALPLUS_INDEP_SURROUND(ac)		((ac & BDROM_ACAP_DOLBYDIGITALPLUS_INDEP_SURROUND) > 0)

#define IS_ACAP_DOLBYDIGITALPLUS_DEP(ac)				((ac & (BDROM_ACAP_DOLBYDIGITALPLUS_DEP_STEREO | BDROM_ACAP_DOLBYDIGITALPLUS_DEP_SURROUND)) > 0)
#define IS_ACAP_DOLBYDIGITALPLUS_DEP_STEREO(ac)			((ac & BDROM_ACAP_DOLBYDIGITALPLUS_DEP_STEREO) > 0)
#define IS_ACAP_DOLBYDIGITALPLUS_DEP_SURROUND(ac)		((ac & BDROM_ACAP_DOLBYDIGITALPLUS_DEP_SURROUND) > 0)

#define IS_ACAP_DOLBYLOSSLESS_MLP(ac)					((ac & (BDROM_ACAP_DOLBYLOSSLESS_MLP_STEREO | BDROM_ACAP_DOLBYLOSSLESS_MLP_SURROUND)) > 0)
#define IS_ACAP_DOLBYLOSSLESS_MLP_STEREO(ac)			((ac & BDROM_ACAP_DOLBYLOSSLESS_MLP_STEREO) > 0)
#define IS_ACAP_DOLBYLOSSLESS_MLP_SURROUND(ac)			((ac & BDROM_ACAP_DOLBYLOSSLESS_MLP_SURROUND) > 0)

#define IS_ACAP_DOLBYDIGITAL(ac)						((ac & (BDROM_ACAP_DOLBYLOSSLESS_DOLBYDIGITAL_STEREO | BDROM_ACAP_DOLBYLOSSLESS_DOLBYDIGITAL_SURROUND)) > 0)
#define IS_ACAP_DOLBYDIGITAL_STEREO(ac)					((ac & BDROM_ACAP_DOLBYLOSSLESS_DOLBYDIGITAL_STEREO) > 0)
#define IS_ACAP_DOLBYDIGITAL_SURROUND(ac)				((ac & BDROM_ACAP_DOLBYLOSSLESS_DOLBYDIGITAL_SURROUND) > 0)

#define IS_ACAP_DTSHD_CORE_STEREO(ac)					((ac & BDROM_ACAP_DTSHD_CORE_STEREO) > 0)
#define IS_ACAP_DTSHD_CORE_SURROUND(ac)					((ac & BDROM_ACAP_DTSHD_CORE_SURROUND) > 0)

#define IS_ACAP_DTSHD_EXTN(ac)							((ac & (BDROM_ACAP_DTSHD_EXTENSION_STEREO | BDROM_ACAP_DTSHD_EXTENSION_SURROUND)) > 0)
#define IS_ACAP_DTSHD_EXTN_STEREO(ac)					((ac & BDROM_ACAP_DTSHD_EXTENSION_STEREO) > 0)
#define IS_ACAP_DTSHD_EXTN_SURROUND(ac)					((ac & BDROM_ACAP_DTSHD_EXTENSION_SURROUND) > 0)

// Region code values
//
#define BDROM_REGIONCODE_NONE						0x00
#define BDROM_REGIONCODE_A							0x01
#define BDROM_REGIONCODE_B							0x02
#define BDROM_REGIONCODE_C							0x04

// Register Field Defaults
//

#define BDROM_PSR__IG_STRMNUM__DEFAULT				1
#define BDROM_PSR__PRIMARYAUDIO_STRMNUM__NONE		0xFF
#define BDROM_PSR__PGTEXT_STRMNUM__NONE				0xFFF
#define BDROM_PSR__ANGLE__DEFAULT					1
#define BDROM_PSR__TITLE_NUM__NONE_FIRSTPLAY		0xFFFF
#define BDROM_PSR__CHAPTER_NUM__NONE				0xFFFF
#define BDROM_PSR__PLAYLIST_ID__DEFAULT				0
#define BDROM_PSR__PLAYITEM_ID__DEFAULT				0
#define BDROM_PSR__PRESENTATION_TIME__DEFAULT		0
#define BDROM_PSR__NAVTIMER__NONE					0
#define BDROM_PSR__SELECTED_BUTTON__NONE			0xFFFF
#define BDROM_PSR__PAGE_ID__DEFAULT					0
#define BDROM_PSR__USERSTYLE_NUM__NONE				0xFF
#define BDROM_PSR__PARENTAL_LEVEL__DEFAULT			0xFF
#define BDROM_PSR__SECONDARY_STREAM__DEFAULT		0x0000FFFF
#define BDROM_PSR__AUDIO_CAPABILITY_DEFAULT			(BDROM_ACAP_DOLBYLOSSLESS_DOLBYDIGITAL_STEREO | BDROM_ACAP_DTSHD_CORE_STEREO | \
													 BDROM_ACAP_DOLBYDIGITALPLUS_INDEP_STEREO     | BDROM_ACAP_LPCM_48kHz96kHz_STEREO | \
													 BDROM_ACAP_LPCM_192kHz_STEREO)
#define BDROM_PSR__VIDEO_CAPABILITY_DEFAULT			0x03 //50&25Hz Video capability & HD 2nd Video capability
#define BDROM_PSR__LANGUAGE__NONE					0xFFFFFF
#define BDROM_PSR__COUNTRY_CODE__NONE				0xFFFFFF
#define BDROM_PSR__REGION_CODE_DEFAULT				BDROM_REGIONCODE_NONE
#define BDROM_PSR__TEXT_SUBTITLE__NO_SUPPORT		0
#define BDROM_PSR__PLAYER__AUDIO_VIDEO__VER_1_0		0x00010200	// Profile 1.1, Spec versions 2.0
#define BDROM_PSR__PLAYER__AUDIO_VIDEO__VER_2_0		0x00030200	// Profile 2.0, Spec versions 2.0
#define BDROM_PSR__CHARACTERISTICTEXT__NO_SUPPORT	0
#define BDROM_PSR__CHARACTERISTICTEXT__SUPPORT_ALL	0xFFFFFFFF
#define BDROM_PSR__CHARACTERISTICTEXT__DEFAULT		BDROM_PSR__CHARACTERISTICTEXT__SUPPORT_ALL

#define BDROM_PSR__IG_MAX_VALID						32
#define BDROM_PSR__PRIMARYAUDIO_MAX_VALID			32
#define BDROM_PSR__PGTEXT_MAX_VALID					255
#define BDROM_PSR__PGTEXT_PIP_MAX_VALID				255
#define	BDROM_PSR__ANGLE_MAX_VALID					9
#define BDROM_PSR__TITLE_NUM_MAX_VALID				999
#define BDROM_PSR__CHAPTER_NUM_MAX_VALID			999
#define BDROM_PSR__PLAYLIST_ID_MAX_VALID			1999
#define BDROM_PSR__PLAYITEM_ID_MAX_VALID			198
#define BDROM_PSR__SECONDARY_VIDEO_MAX_VALID		32
#define BDROM_PSR__SECONDARY_AUDIO_MAX_VALID		32

// Register values and register field values
//

// PSR2 : PG strm num
#define BDROM_PSR__PGTEXTST__DEFAULT				0xFFF
#define BDROM_PSR__PIP_PGTEXTST__DEFAULT			0xFFF

// PSR13 : Parental Level
#define BDROM_PSR__PARENTAL_LEVEL__MAX_RESTRICTION	0			// max restriction
#define BDROM_PSR__PARENTAL_LEVEL__MIN_RESTRICTION	255			// least restriction

// PSR14 : Secondary audio and video
#define BDROM_PSR__SECONDARY_VIDEO__DEFAULT			0xFF
#define BDROM_PSR__SECONDARY_VIDEO__NONE			0xFF
#define BDROM_PSR__SECONDARY_VIDEO__VALID			0xFE
#define BDROM_PSR__SECONDARY_VIDEO_SIZE__SCALED		0x0
#define BDROM_PSR__SECONDARY_VIDEO_SIZE__FULL_SCR	0xF
#define BDROM_PSR__SECONDARY_AUDIO__DEFAULT			0xFF
#define BDROM_PSR__SECONDARY_AUDIO__NONE			0xFF

// PSR31 : profile and version
#define BDROM_PSR__PROFILE_1						0x0
#define BDROM_PSR__PROFILE_1_DOT_1					0x1
#define BDROM_PSR__PROFILE_2						0x3
#define BDROM_PSR__PROFILE_3						0x4
#define BDROM_PSR__VERSION_1_0						0x0100
#define BDROM_PSR__VERSION_2_0						0x0200
#define BDROM_PSR__AVCHD							0x00000200		// Fixed value for PSR31 for AVCHD Discs.


// BDROM_PSR_Index
// PSR Index name
//
typedef enum tag_BDROM_PSR_Index
{
	// Player Status Registers (0 - 12)
	//
	BDROM_PSRidx__IG_StrmNum				=  0,
	BDROM_PSRidx__PrimaryAudio_StrmNum		=  1,
	BDROM_PSRidx__PGText_StrmNum			=  2,
	BDROM_PSRidx__Angle						=  3,
	BDROM_PSRidx__Title_Num					=  4,
	BDROM_PSRidx__Chapter_Num				=  5,
	BDROM_PSRidx__PlayList_Id				=  6,
	BDROM_PSRidx__PlayItem_Id				=  7,
	BDROM_PSRidx__PresentationTime			=  8,
	BDROM_PSRidx__NavTimer					=  9,
	BDROM_PSRidx__SelectedButton			= 10,
	BDROM_PSRidx__MenuPage_Id				= 11,
	BDROM_PSRidx__UserStyle_Num				= 12,

	// Player Settings Registers (13 - 20, 29 - 31)
	//
	BDROM_PSRidx__Parental_Level			= 13,
	BDROM_PSRidx__Secondary_StrmNum			= 14,
	BDROM_PSRidx__Audio_Capability			= 15,
	BDROM_PSRidx__Audio_Language			= 16,
	BDROM_PSRidx__PGText_Language			= 17,
	BDROM_PSRidx__Menu_Language				= 18,
	BDROM_PSRidx__Country_Code				= 19,
	BDROM_PSRidx__Region_Code				= 20,		// Introduced in v 2.0 of the Spec (added for early testing by PPG)
	BDROM_PSRidx__Video_Capability			= 29,
	BDROM_PSRidx__Text_Capability			= 30,
	BDROM_PSRidx__Player_NavType_Ver		= 31,

	// Player Status Registers (36 - 40, 42 - 44)
	//
	BDROM_PSRidx__Title_Num__Backup			= 36,
	BDROM_PSRidx__Chapter_Num__Backup		= 37,
	BDROM_PSRidx__PlayList_Id__Backup		= 38,
	BDROM_PSRidx__PlayItem_Id__Backup		= 39,
	BDROM_PSRidx__PresentationTime__Backup	= 40,
	BDROM_PSRidx__SelectedButton__Backup	= 42,
	BDROM_PSRidx__MenuPage_Id__Backup		= 43,
	BDROM_PSRidx__UserStyle_Num__Backup		= 44,

	// Player Settings Registers (48 - 61)
	//
	BDROM_PSRidx__CharacteristicText_Cap_48	= 48,
	BDROM_PSRidx__CharacteristicText_Cap_49	= 49,
	BDROM_PSRidx__CharacteristicText_Cap_50	= 50,
	BDROM_PSRidx__CharacteristicText_Cap_51	= 51,
	BDROM_PSRidx__CharacteristicText_Cap_52	= 52,
	BDROM_PSRidx__CharacteristicText_Cap_53	= 53,
	BDROM_PSRidx__CharacteristicText_Cap_54	= 54,
	BDROM_PSRidx__CharacteristicText_Cap_55	= 55,
	BDROM_PSRidx__CharacteristicText_Cap_56	= 56,
	BDROM_PSRidx__CharacteristicText_Cap_57	= 57,
	BDROM_PSRidx__CharacteristicText_Cap_58	= 58,
	BDROM_PSRidx__CharacteristicText_Cap_59	= 59,
	BDROM_PSRidx__CharacteristicText_Cap_60	= 60,
	BDROM_PSRidx__CharacteristicText_Cap_61	= 61,

	// AACS Registers (96 - 97)
	//
	BDROM_PSRidx__PlaylistIndicators_1		=  96,
	BDROM_PSRidx__PlaylistIndicators_2		=  97,

	// BD+ Registers (102 - 104)
	//
	BDROM_PSRidx__BDplusMessageFromBDplus	=  102,
	BDROM_PSRidx__BDplusMessageFromNav		=  103,
	BDROM_PSRidx__BDplusStatus				=  104,
} BDROM_RegPSR_Index;


//-------------------------------------------------------------------------------------------------------
// Registers Class
//

// params are expected to be lower case letters
#define MAKE_TEXTCAP_LANGCODE(c1, c2, c3)	((c1 << 16) | (c2 << 8) | c3)


// BDROM_SonicPBCE_Registers
// SonicPlaybackControlEngine's registers
//
class BDROM_SonicPBCE_Registers
{
public:
	BDROM_SonicPBCE_Registers(ref_ptr<NotificationEventManager> pNEManager);
	~BDROM_SonicPBCE_Registers();

public:
	void SetPSR(UInt32 uiPSR, UInt32 uiValue);
	UInt32 GetPSR(UInt32 uiPSR);

	void SetGPR(UInt32 uiGPR, UInt32 uiValue);
	UInt32 GetGPR(UInt32 uiGPR);

	void SetNotificationEventManager(ref_ptr<NotificationEventManager> pNEManager);

	bool IsTextSTLanguageCapFlagSet(BDROM_LanguageCode langCode);

private:
	// enums for Characteristic Text Capability PSR fields
	// ** Note ** This table also lists ISO-639-2/B equivalents of ISO-639-2/T even though the spec only
	//            expects ISO-639-2/T. We're doing this as a defensive measure to handle discs that may
	//            be authored incorrectly. (the B equivalents are the ones with the comments next to them)
	//
	enum TextCapLanguageCode
	{
		// PSR 48
		eTxtCapLC_abk = MAKE_TEXTCAP_LANGCODE('a', 'b', 'k'),
		eTxtCapLC_ace = MAKE_TEXTCAP_LANGCODE('a', 'c', 'e'),
		eTxtCapLC_ach = MAKE_TEXTCAP_LANGCODE('a', 'c', 'h'),
		eTxtCapLC_ada = MAKE_TEXTCAP_LANGCODE('a', 'd', 'a'),
		eTxtCapLC_aar = MAKE_TEXTCAP_LANGCODE('a', 'a', 'r'),
		eTxtCapLC_afh = MAKE_TEXTCAP_LANGCODE('a', 'f', 'h'),
		eTxtCapLC_afr = MAKE_TEXTCAP_LANGCODE('a', 'f', 'r'),
		eTxtCapLC_afa = MAKE_TEXTCAP_LANGCODE('a', 'f', 'a'),

		eTxtCapLC_aka = MAKE_TEXTCAP_LANGCODE('a', 'k', 'a'),
		eTxtCapLC_akk = MAKE_TEXTCAP_LANGCODE('a', 'k', 'k'),
		eTxtCapLC_sqi = MAKE_TEXTCAP_LANGCODE('s', 'q', 'i'),
		eTxtCapLC_alb = MAKE_TEXTCAP_LANGCODE('a', 'l', 'b'), // "alb" same as "sqi"
		eTxtCapLC_ale = MAKE_TEXTCAP_LANGCODE('a', 'l', 'e'),
		eTxtCapLC_alg = MAKE_TEXTCAP_LANGCODE('a', 'l', 'g'),
		eTxtCapLC_tut = MAKE_TEXTCAP_LANGCODE('t', 'u', 't'),
		eTxtCapLC_amh = MAKE_TEXTCAP_LANGCODE('a', 'm', 'h'),
		eTxtCapLC_apa = MAKE_TEXTCAP_LANGCODE('a', 'p', 'a'),

		eTxtCapLC_ara = MAKE_TEXTCAP_LANGCODE('a', 'r', 'a'),
		eTxtCapLC_arc = MAKE_TEXTCAP_LANGCODE('a', 'r', 'c'),
		eTxtCapLC_arp = MAKE_TEXTCAP_LANGCODE('a', 'r', 'p'),
		eTxtCapLC_arn = MAKE_TEXTCAP_LANGCODE('a', 'r', 'n'),
		eTxtCapLC_arw = MAKE_TEXTCAP_LANGCODE('a', 'r', 'w'),
		eTxtCapLC_hye = MAKE_TEXTCAP_LANGCODE('h', 'y', 'e'),
		eTxtCapLC_arm = MAKE_TEXTCAP_LANGCODE('a', 'r', 'm'), // "arm" same as "hye"
		eTxtCapLC_art = MAKE_TEXTCAP_LANGCODE('a', 'r', 't'),
		eTxtCapLC_asm = MAKE_TEXTCAP_LANGCODE('a', 's', 'm'),

		eTxtCapLC_ath = MAKE_TEXTCAP_LANGCODE('a', 't', 'h'),
		eTxtCapLC_aus = MAKE_TEXTCAP_LANGCODE('a', 'u', 's'),
		eTxtCapLC_map = MAKE_TEXTCAP_LANGCODE('m', 'a', 'p'),
		eTxtCapLC_ava = MAKE_TEXTCAP_LANGCODE('a', 'v', 'a'),
		eTxtCapLC_ave = MAKE_TEXTCAP_LANGCODE('a', 'v', 'e'),
		eTxtCapLC_awa = MAKE_TEXTCAP_LANGCODE('a', 'w', 'a'),
		eTxtCapLC_aym = MAKE_TEXTCAP_LANGCODE('a', 'y', 'm'),
		eTxtCapLC_aze = MAKE_TEXTCAP_LANGCODE('a', 'z', 'e'),

		// PSR49
		eTxtCapLC_ban = MAKE_TEXTCAP_LANGCODE('b', 'a', 'n'),
		eTxtCapLC_bat = MAKE_TEXTCAP_LANGCODE('b', 'a', 't'),
		eTxtCapLC_bal = MAKE_TEXTCAP_LANGCODE('b', 'a', 'l'),
		eTxtCapLC_bam = MAKE_TEXTCAP_LANGCODE('b', 'a', 'm'),
		eTxtCapLC_bai = MAKE_TEXTCAP_LANGCODE('b', 'a', 'i'),
		eTxtCapLC_bad = MAKE_TEXTCAP_LANGCODE('b', 'a', 'd'),
		eTxtCapLC_bnt = MAKE_TEXTCAP_LANGCODE('b', 'n', 't'),
		eTxtCapLC_bas = MAKE_TEXTCAP_LANGCODE('b', 'a', 's'),

		eTxtCapLC_bak = MAKE_TEXTCAP_LANGCODE('b', 'a', 'k'),
		eTxtCapLC_eus = MAKE_TEXTCAP_LANGCODE('e', 'u', 's'),
		eTxtCapLC_baq = MAKE_TEXTCAP_LANGCODE('b', 'a', 'q'), // "baq" same as "eus"
		eTxtCapLC_btk = MAKE_TEXTCAP_LANGCODE('b', 't', 'k'),
		eTxtCapLC_bej = MAKE_TEXTCAP_LANGCODE('b', 'e', 'j'),
		eTxtCapLC_bel = MAKE_TEXTCAP_LANGCODE('b', 'e', 'l'),
		eTxtCapLC_bem = MAKE_TEXTCAP_LANGCODE('b', 'e', 'm'),
		eTxtCapLC_ben = MAKE_TEXTCAP_LANGCODE('b', 'e', 'n'),
		eTxtCapLC_ber = MAKE_TEXTCAP_LANGCODE('b', 'e', 'r'),

		eTxtCapLC_bho = MAKE_TEXTCAP_LANGCODE('b', 'h', 'o'),
		eTxtCapLC_bih = MAKE_TEXTCAP_LANGCODE('b', 'i', 'h'),
		eTxtCapLC_bik = MAKE_TEXTCAP_LANGCODE('b', 'i', 'k'),
		eTxtCapLC_bin = MAKE_TEXTCAP_LANGCODE('b', 'i', 'n'),
		eTxtCapLC_bis = MAKE_TEXTCAP_LANGCODE('b', 'i', 's'),
		eTxtCapLC_bra = MAKE_TEXTCAP_LANGCODE('b', 'r', 'a'),
		eTxtCapLC_bre = MAKE_TEXTCAP_LANGCODE('b', 'r', 'e'),
		eTxtCapLC_bug = MAKE_TEXTCAP_LANGCODE('b', 'u', 'g'),

		eTxtCapLC_bul = MAKE_TEXTCAP_LANGCODE('b', 'u', 'l'),
		eTxtCapLC_bua = MAKE_TEXTCAP_LANGCODE('b', 'u', 'a'),
		eTxtCapLC_mya = MAKE_TEXTCAP_LANGCODE('m', 'y', 'a'),
		eTxtCapLC_bur = MAKE_TEXTCAP_LANGCODE('b', 'u', 'r'), // "bur" same as "mya"
		eTxtCapLC_cad = MAKE_TEXTCAP_LANGCODE('c', 'a', 'd'),
		eTxtCapLC_car = MAKE_TEXTCAP_LANGCODE('c', 'a', 'r'),
		eTxtCapLC_cat = MAKE_TEXTCAP_LANGCODE('c', 'a', 't'),
		eTxtCapLC_cau = MAKE_TEXTCAP_LANGCODE('c', 'a', 'u'),
		eTxtCapLC_ceb = MAKE_TEXTCAP_LANGCODE('c', 'e', 'b'),

		// PSR 50
		eTxtCapLC_cel = MAKE_TEXTCAP_LANGCODE('c', 'e', 'l'),
		eTxtCapLC_cai = MAKE_TEXTCAP_LANGCODE('c', 'a', 'i'),
		eTxtCapLC_chg = MAKE_TEXTCAP_LANGCODE('c', 'h', 'g'),
		eTxtCapLC_cmc = MAKE_TEXTCAP_LANGCODE('c', 'm', 'c'),
		eTxtCapLC_cha = MAKE_TEXTCAP_LANGCODE('c', 'h', 'a'),
		eTxtCapLC_che = MAKE_TEXTCAP_LANGCODE('c', 'h', 'e'),
		eTxtCapLC_chr = MAKE_TEXTCAP_LANGCODE('c', 'h', 'r'),
		eTxtCapLC_chy = MAKE_TEXTCAP_LANGCODE('c', 'h', 'y'),

		eTxtCapLC_chb = MAKE_TEXTCAP_LANGCODE('c', 'h', 'b'),
		eTxtCapLC_zho = MAKE_TEXTCAP_LANGCODE('z', 'h', 'o'),
		eTxtCapLC_chi = MAKE_TEXTCAP_LANGCODE('c', 'h', 'i'), // "chi" same as "zho"
		eTxtCapLC_chn = MAKE_TEXTCAP_LANGCODE('c', 'h', 'n'),
		eTxtCapLC_chp = MAKE_TEXTCAP_LANGCODE('c', 'h', 'p'),
		eTxtCapLC_cho = MAKE_TEXTCAP_LANGCODE('c', 'h', 'o'),
		eTxtCapLC_chu = MAKE_TEXTCAP_LANGCODE('c', 'h', 'u'),
		eTxtCapLC_chk = MAKE_TEXTCAP_LANGCODE('c', 'h', 'k'),
		eTxtCapLC_chv = MAKE_TEXTCAP_LANGCODE('c', 'h', 'v'),

		eTxtCapLC_cop = MAKE_TEXTCAP_LANGCODE('c', 'o', 'p'),
		eTxtCapLC_cor = MAKE_TEXTCAP_LANGCODE('c', 'o', 'r'),
		eTxtCapLC_cos = MAKE_TEXTCAP_LANGCODE('c', 'o', 's'),
		eTxtCapLC_cre = MAKE_TEXTCAP_LANGCODE('c', 'r', 'e'),
		eTxtCapLC_mus = MAKE_TEXTCAP_LANGCODE('m', 'u', 's'),
		eTxtCapLC_crp = MAKE_TEXTCAP_LANGCODE('c', 'r', 'p'),
		eTxtCapLC_cpe = MAKE_TEXTCAP_LANGCODE('c', 'p', 'e'),
		eTxtCapLC_cpf = MAKE_TEXTCAP_LANGCODE('c', 'p', 'f'),

		eTxtCapLC_cpp = MAKE_TEXTCAP_LANGCODE('c', 'p', 'p'),
		eTxtCapLC_hrv = MAKE_TEXTCAP_LANGCODE('h', 'r', 'v'),
		eTxtCapLC_cus = MAKE_TEXTCAP_LANGCODE('c', 'u', 's'),
		eTxtCapLC_ces = MAKE_TEXTCAP_LANGCODE('c', 'e', 's'),
		eTxtCapLC_cze = MAKE_TEXTCAP_LANGCODE('c', 'z', 'e'), // "cze" same as "ces"
		eTxtCapLC_dak = MAKE_TEXTCAP_LANGCODE('d', 'a', 'k'),
		eTxtCapLC_dan = MAKE_TEXTCAP_LANGCODE('d', 'a', 'n'),
		eTxtCapLC_day = MAKE_TEXTCAP_LANGCODE('d', 'a', 'y'),
		eTxtCapLC_del = MAKE_TEXTCAP_LANGCODE('d', 'e', 'l'),

		// PSR 51
		eTxtCapLC_din = MAKE_TEXTCAP_LANGCODE('d', 'i', 'n'),
		eTxtCapLC_div = MAKE_TEXTCAP_LANGCODE('d', 'i', 'v'),
		eTxtCapLC_doi = MAKE_TEXTCAP_LANGCODE('d', 'o', 'i'),
		eTxtCapLC_dgr = MAKE_TEXTCAP_LANGCODE('d', 'g', 'r'),
		eTxtCapLC_dra = MAKE_TEXTCAP_LANGCODE('d', 'r', 'a'),
		eTxtCapLC_dua = MAKE_TEXTCAP_LANGCODE('d', 'u', 'a'),
		eTxtCapLC_nld = MAKE_TEXTCAP_LANGCODE('n', 'l', 'd'),
		eTxtCapLC_dut = MAKE_TEXTCAP_LANGCODE('d', 'u', 't'), // "dut" same as "nld"
		eTxtCapLC_dum = MAKE_TEXTCAP_LANGCODE('d', 'u', 'm'),

		eTxtCapLC_dyu = MAKE_TEXTCAP_LANGCODE('d', 'y', 'u'),
		eTxtCapLC_dzo = MAKE_TEXTCAP_LANGCODE('d', 'z', 'o'),
		eTxtCapLC_efi = MAKE_TEXTCAP_LANGCODE('e', 'f', 'i'),
		eTxtCapLC_egy = MAKE_TEXTCAP_LANGCODE('e', 'g', 'y'),
		eTxtCapLC_eka = MAKE_TEXTCAP_LANGCODE('e', 'k', 'a'),
		eTxtCapLC_elx = MAKE_TEXTCAP_LANGCODE('e', 'l', 'x'),
		eTxtCapLC_eng = MAKE_TEXTCAP_LANGCODE('e', 'n', 'g'),
		eTxtCapLC_enm = MAKE_TEXTCAP_LANGCODE('e', 'n', 'm'),

		eTxtCapLC_ang = MAKE_TEXTCAP_LANGCODE('a', 'n', 'g'),
		eTxtCapLC_epo = MAKE_TEXTCAP_LANGCODE('e', 'p', 'o'),
		eTxtCapLC_est = MAKE_TEXTCAP_LANGCODE('e', 's', 't'),
		eTxtCapLC_ewe = MAKE_TEXTCAP_LANGCODE('e', 'w', 'e'),
		eTxtCapLC_ewo = MAKE_TEXTCAP_LANGCODE('e', 'w', 'o'),
		eTxtCapLC_fan = MAKE_TEXTCAP_LANGCODE('f', 'a', 'n'),
		eTxtCapLC_fat = MAKE_TEXTCAP_LANGCODE('f', 'a', 't'),
		eTxtCapLC_fao = MAKE_TEXTCAP_LANGCODE('f', 'a', 'o'),

		eTxtCapLC_fij = MAKE_TEXTCAP_LANGCODE('f', 'i', 'j'),
		eTxtCapLC_fin = MAKE_TEXTCAP_LANGCODE('f', 'i', 'n'),
		eTxtCapLC_fiu = MAKE_TEXTCAP_LANGCODE('f', 'i', 'u'),
		eTxtCapLC_fon = MAKE_TEXTCAP_LANGCODE('f', 'o', 'n'),
		eTxtCapLC_fra = MAKE_TEXTCAP_LANGCODE('f', 'r', 'a'),
		eTxtCapLC_fre = MAKE_TEXTCAP_LANGCODE('f', 'r', 'e'), // "fre" same as "fra"
		eTxtCapLC_frm = MAKE_TEXTCAP_LANGCODE('f', 'r', 'm'),
		eTxtCapLC_fro = MAKE_TEXTCAP_LANGCODE('f', 'r', 'o'),
		eTxtCapLC_fry = MAKE_TEXTCAP_LANGCODE('f', 'r', 'y'),

		// PSR 52
		eTxtCapLC_fur = MAKE_TEXTCAP_LANGCODE('f', 'u', 'r'),
		eTxtCapLC_ful = MAKE_TEXTCAP_LANGCODE('f', 'u', 'l'),
		eTxtCapLC_gaa = MAKE_TEXTCAP_LANGCODE('g', 'a', 'a'),
		eTxtCapLC_gla = MAKE_TEXTCAP_LANGCODE('g', 'l', 'a'),
		eTxtCapLC_glg = MAKE_TEXTCAP_LANGCODE('g', 'l', 'g'),
		eTxtCapLC_lug = MAKE_TEXTCAP_LANGCODE('l', 'u', 'g'),
		eTxtCapLC_gay = MAKE_TEXTCAP_LANGCODE('g', 'a', 'y'),
		eTxtCapLC_gba = MAKE_TEXTCAP_LANGCODE('g', 'b', 'a'),

		eTxtCapLC_gez = MAKE_TEXTCAP_LANGCODE('g', 'e', 'z'),
		eTxtCapLC_kat = MAKE_TEXTCAP_LANGCODE('k', 'a', 't'),
		eTxtCapLC_geo = MAKE_TEXTCAP_LANGCODE('g', 'e', 'o'), // "geo" same as "kat"
		eTxtCapLC_deu = MAKE_TEXTCAP_LANGCODE('d', 'e', 'u'),
		eTxtCapLC_ger = MAKE_TEXTCAP_LANGCODE('g', 'e', 'r'), // "ger" same as "deu"
		eTxtCapLC_gmh = MAKE_TEXTCAP_LANGCODE('g', 'm', 'h'),
		eTxtCapLC_goh = MAKE_TEXTCAP_LANGCODE('g', 'o', 'h'),
		eTxtCapLC_gem = MAKE_TEXTCAP_LANGCODE('g', 'e', 'm'),
		eTxtCapLC_gil = MAKE_TEXTCAP_LANGCODE('g', 'i', 'l'),
		eTxtCapLC_gon = MAKE_TEXTCAP_LANGCODE('g', 'o', 'n'),

		eTxtCapLC_gor = MAKE_TEXTCAP_LANGCODE('g', 'o', 'r'),
		eTxtCapLC_got = MAKE_TEXTCAP_LANGCODE('g', 'o', 't'),
		eTxtCapLC_grb = MAKE_TEXTCAP_LANGCODE('g', 'r', 'b'),
		eTxtCapLC_grc = MAKE_TEXTCAP_LANGCODE('g', 'r', 'c'),
		eTxtCapLC_ell = MAKE_TEXTCAP_LANGCODE('e', 'l', 'l'),
		eTxtCapLC_gre = MAKE_TEXTCAP_LANGCODE('g', 'r', 'e'), // "gre" same as "ell"
		eTxtCapLC_grn = MAKE_TEXTCAP_LANGCODE('g', 'r', 'n'),
		eTxtCapLC_guj = MAKE_TEXTCAP_LANGCODE('g', 'u', 'j'),
		eTxtCapLC_gwi = MAKE_TEXTCAP_LANGCODE('g', 'w', 'i'),

		eTxtCapLC_hai = MAKE_TEXTCAP_LANGCODE('h', 'a', 'i'),
		eTxtCapLC_hau = MAKE_TEXTCAP_LANGCODE('h', 'a', 'u'),
		eTxtCapLC_haw = MAKE_TEXTCAP_LANGCODE('h', 'a', 'w'),
		eTxtCapLC_heb = MAKE_TEXTCAP_LANGCODE('h', 'e', 'b'),
		eTxtCapLC_her = MAKE_TEXTCAP_LANGCODE('h', 'e', 'r'),
		eTxtCapLC_hil = MAKE_TEXTCAP_LANGCODE('h', 'i', 'l'),
		eTxtCapLC_him = MAKE_TEXTCAP_LANGCODE('h', 'i', 'm'),
		eTxtCapLC_hin = MAKE_TEXTCAP_LANGCODE('h', 'i', 'n'),

		// PSR 53
		eTxtCapLC_hmo = MAKE_TEXTCAP_LANGCODE('h', 'm', 'o'),
		eTxtCapLC_hit = MAKE_TEXTCAP_LANGCODE('h', 'i', 't'),
		eTxtCapLC_hmn = MAKE_TEXTCAP_LANGCODE('h', 'm', 'n'),
		eTxtCapLC_hun = MAKE_TEXTCAP_LANGCODE('h', 'u', 'n'),
		eTxtCapLC_hup = MAKE_TEXTCAP_LANGCODE('h', 'u', 'p'),
		eTxtCapLC_iba = MAKE_TEXTCAP_LANGCODE('i', 'b', 'a'),
		eTxtCapLC_isl = MAKE_TEXTCAP_LANGCODE('i', 's', 'l'),
		eTxtCapLC_ice = MAKE_TEXTCAP_LANGCODE('i', 'c', 'e'), // "ice" same as "isl"
		eTxtCapLC_ibo = MAKE_TEXTCAP_LANGCODE('i', 'b', 'o'),

		eTxtCapLC_ijo = MAKE_TEXTCAP_LANGCODE('i', 'j', 'o'),
		eTxtCapLC_ilo = MAKE_TEXTCAP_LANGCODE('i', 'l', 'o'),
		eTxtCapLC_inc = MAKE_TEXTCAP_LANGCODE('i', 'n', 'c'),
		eTxtCapLC_ine = MAKE_TEXTCAP_LANGCODE('i', 'n', 'e'),
		eTxtCapLC_ind = MAKE_TEXTCAP_LANGCODE('i', 'n', 'd'),
		eTxtCapLC_ina = MAKE_TEXTCAP_LANGCODE('i', 'n', 'a'),
		eTxtCapLC_ile = MAKE_TEXTCAP_LANGCODE('i', 'l', 'e'),
		eTxtCapLC_iku = MAKE_TEXTCAP_LANGCODE('i', 'k', 'u'),

		eTxtCapLC_ipk = MAKE_TEXTCAP_LANGCODE('i', 'p', 'k'),
		eTxtCapLC_ira = MAKE_TEXTCAP_LANGCODE('i', 'r', 'a'),
		eTxtCapLC_gle = MAKE_TEXTCAP_LANGCODE('g', 'l', 'e'),
		eTxtCapLC_mga = MAKE_TEXTCAP_LANGCODE('m', 'g', 'a'),
		eTxtCapLC_sga = MAKE_TEXTCAP_LANGCODE('s', 'g', 'a'),
		eTxtCapLC_iro = MAKE_TEXTCAP_LANGCODE('i', 'r', 'o'),
		eTxtCapLC_ita = MAKE_TEXTCAP_LANGCODE('i', 't', 'a'),
		eTxtCapLC_jpn = MAKE_TEXTCAP_LANGCODE('j', 'p', 'n'),

		eTxtCapLC_jaw = MAKE_TEXTCAP_LANGCODE('j', 'a', 'w'),
		eTxtCapLC_jav = MAKE_TEXTCAP_LANGCODE('j', 'a', 'v'), // "jav" same as "jaw". "jav" has replaced "jaw" per ISO spec
		eTxtCapLC_jrb = MAKE_TEXTCAP_LANGCODE('j', 'r', 'b'),
		eTxtCapLC_jpr = MAKE_TEXTCAP_LANGCODE('j', 'p', 'r'),
		eTxtCapLC_kab = MAKE_TEXTCAP_LANGCODE('k', 'a', 'b'),
		eTxtCapLC_kac = MAKE_TEXTCAP_LANGCODE('k', 'a', 'c'),
		eTxtCapLC_kal = MAKE_TEXTCAP_LANGCODE('k', 'a', 'l'),
		eTxtCapLC_kam = MAKE_TEXTCAP_LANGCODE('k', 'a', 'm'),
		eTxtCapLC_kan = MAKE_TEXTCAP_LANGCODE('k', 'a', 'n'),

		// PSR 54
		eTxtCapLC_kau = MAKE_TEXTCAP_LANGCODE('k', 'a', 'u'),
		eTxtCapLC_kaa = MAKE_TEXTCAP_LANGCODE('k', 'a', 'a'),
		eTxtCapLC_kar = MAKE_TEXTCAP_LANGCODE('k', 'a', 'r'),
		eTxtCapLC_kas = MAKE_TEXTCAP_LANGCODE('k', 'a', 's'),
		eTxtCapLC_kaw = MAKE_TEXTCAP_LANGCODE('k', 'a', 'w'),
		eTxtCapLC_kaz = MAKE_TEXTCAP_LANGCODE('k', 'a', 'z'),
		eTxtCapLC_kha = MAKE_TEXTCAP_LANGCODE('k', 'h', 'a'),
		eTxtCapLC_khm = MAKE_TEXTCAP_LANGCODE('k', 'h', 'm'),

		eTxtCapLC_khi = MAKE_TEXTCAP_LANGCODE('k', 'h', 'i'),
		eTxtCapLC_kho = MAKE_TEXTCAP_LANGCODE('k', 'h', 'o'),
		eTxtCapLC_kik = MAKE_TEXTCAP_LANGCODE('k', 'i', 'k'),
		eTxtCapLC_kmb = MAKE_TEXTCAP_LANGCODE('k', 'm', 'b'),
		eTxtCapLC_kin = MAKE_TEXTCAP_LANGCODE('k', 'i', 'n'),
		eTxtCapLC_kir = MAKE_TEXTCAP_LANGCODE('k', 'i', 'r'),
		eTxtCapLC_kom = MAKE_TEXTCAP_LANGCODE('k', 'o', 'm'),
		eTxtCapLC_kon = MAKE_TEXTCAP_LANGCODE('k', 'o', 'n'),

		eTxtCapLC_kok = MAKE_TEXTCAP_LANGCODE('k', 'o', 'k'),
		eTxtCapLC_kor = MAKE_TEXTCAP_LANGCODE('k', 'o', 'r'),
		eTxtCapLC_kos = MAKE_TEXTCAP_LANGCODE('k', 'o', 's'),
		eTxtCapLC_kpe = MAKE_TEXTCAP_LANGCODE('k', 'p', 'e'),
		eTxtCapLC_kro = MAKE_TEXTCAP_LANGCODE('k', 'r', 'o'),
		eTxtCapLC_kua = MAKE_TEXTCAP_LANGCODE('k', 'u', 'a'),
		eTxtCapLC_kum = MAKE_TEXTCAP_LANGCODE('k', 'u', 'm'),
		eTxtCapLC_kur = MAKE_TEXTCAP_LANGCODE('k', 'u', 'r'),

		eTxtCapLC_kru = MAKE_TEXTCAP_LANGCODE('k', 'r', 'u'),
		eTxtCapLC_kut = MAKE_TEXTCAP_LANGCODE('k', 'u', 't'),
		eTxtCapLC_lad = MAKE_TEXTCAP_LANGCODE('l', 'a', 'd'),
		eTxtCapLC_lah = MAKE_TEXTCAP_LANGCODE('l', 'a', 'h'),
		eTxtCapLC_lam = MAKE_TEXTCAP_LANGCODE('l', 'a', 'm'),
		eTxtCapLC_lao = MAKE_TEXTCAP_LANGCODE('l', 'a', 'o'),
		eTxtCapLC_lat = MAKE_TEXTCAP_LANGCODE('l', 'a', 't'),
		eTxtCapLC_lav = MAKE_TEXTCAP_LANGCODE('l', 'a', 'v'),

		// PSR 55
		eTxtCapLC_ltz = MAKE_TEXTCAP_LANGCODE('l', 't', 'z'),
		eTxtCapLC_lez = MAKE_TEXTCAP_LANGCODE('l', 'e', 'z'),
		eTxtCapLC_lin = MAKE_TEXTCAP_LANGCODE('l', 'i', 'n'),
		eTxtCapLC_lit = MAKE_TEXTCAP_LANGCODE('l', 'i', 't'),
		eTxtCapLC_loz = MAKE_TEXTCAP_LANGCODE('l', 'o', 'z'),
		eTxtCapLC_lub = MAKE_TEXTCAP_LANGCODE('l', 'u', 'b'),
		eTxtCapLC_lua = MAKE_TEXTCAP_LANGCODE('l', 'u', 'a'),
		eTxtCapLC_lui = MAKE_TEXTCAP_LANGCODE('l', 'u', 'i'),

		eTxtCapLC_lun = MAKE_TEXTCAP_LANGCODE('l', 'u', 'n'),
		eTxtCapLC_luo = MAKE_TEXTCAP_LANGCODE('l', 'u', 'o'),
		eTxtCapLC_lus = MAKE_TEXTCAP_LANGCODE('l', 'u', 's'),
		eTxtCapLC_mkd = MAKE_TEXTCAP_LANGCODE('m', 'k', 'd'),
		eTxtCapLC_mac = MAKE_TEXTCAP_LANGCODE('m', 'a', 'c'), // "mac" same as "mkd"
		eTxtCapLC_mad = MAKE_TEXTCAP_LANGCODE('m', 'a', 'd'),
		eTxtCapLC_mag = MAKE_TEXTCAP_LANGCODE('m', 'a', 'g'),
		eTxtCapLC_mai = MAKE_TEXTCAP_LANGCODE('m', 'a', 'i'),
		eTxtCapLC_mak = MAKE_TEXTCAP_LANGCODE('m', 'a', 'k'),

		eTxtCapLC_mlg = MAKE_TEXTCAP_LANGCODE('m', 'l', 'g'),
		eTxtCapLC_msa = MAKE_TEXTCAP_LANGCODE('m', 's', 'a'),
		eTxtCapLC_may = MAKE_TEXTCAP_LANGCODE('m', 'a', 'y'), // "may" same as "msa"
		eTxtCapLC_mal = MAKE_TEXTCAP_LANGCODE('m', 'a', 'l'),
		eTxtCapLC_mlt = MAKE_TEXTCAP_LANGCODE('m', 'l', 't'),
		eTxtCapLC_mdr = MAKE_TEXTCAP_LANGCODE('m', 'd', 'r'),
		eTxtCapLC_man = MAKE_TEXTCAP_LANGCODE('m', 'a', 'n'),
		eTxtCapLC_mni = MAKE_TEXTCAP_LANGCODE('m', 'n', 'i'),
		eTxtCapLC_mno = MAKE_TEXTCAP_LANGCODE('m', 'n', 'o'),

		eTxtCapLC_glv = MAKE_TEXTCAP_LANGCODE('g', 'l', 'v'),
		eTxtCapLC_mri = MAKE_TEXTCAP_LANGCODE('m', 'r', 'i'),
		eTxtCapLC_mao = MAKE_TEXTCAP_LANGCODE('m', 'a', 'o'), // "mao" same as "mri"
		eTxtCapLC_mar = MAKE_TEXTCAP_LANGCODE('m', 'a', 'r'),
		eTxtCapLC_chm = MAKE_TEXTCAP_LANGCODE('c', 'h', 'm'),
		eTxtCapLC_mah = MAKE_TEXTCAP_LANGCODE('m', 'a', 'h'),
		eTxtCapLC_mwr = MAKE_TEXTCAP_LANGCODE('m', 'w', 'r'),
		eTxtCapLC_mas = MAKE_TEXTCAP_LANGCODE('m', 'a', 's'),
		eTxtCapLC_myn = MAKE_TEXTCAP_LANGCODE('m', 'y', 'n'),

		// PSR 56
		eTxtCapLC_men = MAKE_TEXTCAP_LANGCODE('m', 'e', 'n'),
		eTxtCapLC_mic = MAKE_TEXTCAP_LANGCODE('m', 'i', 'c'),
		eTxtCapLC_min = MAKE_TEXTCAP_LANGCODE('m', 'i', 'n'),
		eTxtCapLC_mis = MAKE_TEXTCAP_LANGCODE('m', 'i', 's'),
		eTxtCapLC_moh = MAKE_TEXTCAP_LANGCODE('m', 'o', 'h'),
		eTxtCapLC_mol = MAKE_TEXTCAP_LANGCODE('m', 'o', 'l'),
		eTxtCapLC_mkh = MAKE_TEXTCAP_LANGCODE('m', 'k', 'h'),
		eTxtCapLC_lol = MAKE_TEXTCAP_LANGCODE('l', 'o', 'l'),

		eTxtCapLC_mon = MAKE_TEXTCAP_LANGCODE('m', 'o', 'n'),
		eTxtCapLC_mos = MAKE_TEXTCAP_LANGCODE('m', 'o', 's'),
		eTxtCapLC_mul = MAKE_TEXTCAP_LANGCODE('m', 'u', 'l'),
		eTxtCapLC_mun = MAKE_TEXTCAP_LANGCODE('m', 'u', 'n'),
		eTxtCapLC_nah = MAKE_TEXTCAP_LANGCODE('n', 'a', 'h'),
		eTxtCapLC_nau = MAKE_TEXTCAP_LANGCODE('n', 'a', 'u'),
		eTxtCapLC_nav = MAKE_TEXTCAP_LANGCODE('n', 'a', 'v'),
		eTxtCapLC_nde = MAKE_TEXTCAP_LANGCODE('n', 'd', 'e'),

		eTxtCapLC_nbl = MAKE_TEXTCAP_LANGCODE('n', 'b', 'l'),
		eTxtCapLC_ndo = MAKE_TEXTCAP_LANGCODE('n', 'd', 'o'),
		eTxtCapLC_nep = MAKE_TEXTCAP_LANGCODE('n', 'e', 'p'),
		eTxtCapLC_new = MAKE_TEXTCAP_LANGCODE('n', 'e', 'w'),
		eTxtCapLC_nia = MAKE_TEXTCAP_LANGCODE('n', 'i', 'a'),
		eTxtCapLC_nic = MAKE_TEXTCAP_LANGCODE('n', 'i', 'c'),
		eTxtCapLC_ssa = MAKE_TEXTCAP_LANGCODE('s', 's', 'a'),
		eTxtCapLC_niu = MAKE_TEXTCAP_LANGCODE('n', 'i', 'u'),

		eTxtCapLC_non = MAKE_TEXTCAP_LANGCODE('n', 'o', 'n'),
		eTxtCapLC_nai = MAKE_TEXTCAP_LANGCODE('n', 'a', 'i'),
		eTxtCapLC_nor = MAKE_TEXTCAP_LANGCODE('n', 'o', 'r'),
		eTxtCapLC_nub = MAKE_TEXTCAP_LANGCODE('n', 'u', 'b'),
		eTxtCapLC_nym = MAKE_TEXTCAP_LANGCODE('n', 'y', 'm'),
		eTxtCapLC_nya = MAKE_TEXTCAP_LANGCODE('n', 'y', 'a'),
		eTxtCapLC_nyn = MAKE_TEXTCAP_LANGCODE('n', 'y', 'n'),
		eTxtCapLC_nyo = MAKE_TEXTCAP_LANGCODE('n', 'y', 'o'),

		// PSR 57
		eTxtCapLC_nzi = MAKE_TEXTCAP_LANGCODE('n', 'z', 'i'),
		eTxtCapLC_oci = MAKE_TEXTCAP_LANGCODE('o', 'c', 'i'),
		eTxtCapLC_oji = MAKE_TEXTCAP_LANGCODE('o', 'j', 'i'),
		eTxtCapLC_ori = MAKE_TEXTCAP_LANGCODE('o', 'r', 'i'),
		eTxtCapLC_orm = MAKE_TEXTCAP_LANGCODE('o', 'r', 'm'),
		eTxtCapLC_osa = MAKE_TEXTCAP_LANGCODE('o', 's', 'a'),
		eTxtCapLC_oss = MAKE_TEXTCAP_LANGCODE('o', 's', 's'),
		eTxtCapLC_oto = MAKE_TEXTCAP_LANGCODE('o', 't', 'o'),

		eTxtCapLC_pal = MAKE_TEXTCAP_LANGCODE('p', 'a', 'l'),
		eTxtCapLC_pau = MAKE_TEXTCAP_LANGCODE('p', 'a', 'u'),
		eTxtCapLC_pli = MAKE_TEXTCAP_LANGCODE('p', 'l', 'i'),
		eTxtCapLC_pam = MAKE_TEXTCAP_LANGCODE('p', 'a', 'm'),
		eTxtCapLC_pag = MAKE_TEXTCAP_LANGCODE('p', 'a', 'g'),
		eTxtCapLC_pan = MAKE_TEXTCAP_LANGCODE('p', 'a', 'n'),
		eTxtCapLC_pap = MAKE_TEXTCAP_LANGCODE('p', 'a', 'p'),
		eTxtCapLC_paa = MAKE_TEXTCAP_LANGCODE('p', 'a', 'a'),

		eTxtCapLC_fas = MAKE_TEXTCAP_LANGCODE('f', 'a', 's'),
		eTxtCapLC_per = MAKE_TEXTCAP_LANGCODE('p', 'e', 'r'), // "per" same as "fas"
		eTxtCapLC_peo = MAKE_TEXTCAP_LANGCODE('p', 'e', 'o'),
		eTxtCapLC_phi = MAKE_TEXTCAP_LANGCODE('p', 'h', 'i'),
		eTxtCapLC_phn = MAKE_TEXTCAP_LANGCODE('p', 'h', 'n'),
		eTxtCapLC_pon = MAKE_TEXTCAP_LANGCODE('p', 'o', 'n'),
		eTxtCapLC_pol = MAKE_TEXTCAP_LANGCODE('p', 'o', 'l'),
		eTxtCapLC_por = MAKE_TEXTCAP_LANGCODE('p', 'o', 'r'),
		eTxtCapLC_pra = MAKE_TEXTCAP_LANGCODE('p', 'r', 'a'),

		eTxtCapLC_pro = MAKE_TEXTCAP_LANGCODE('p', 'r', 'o'),
		eTxtCapLC_pus = MAKE_TEXTCAP_LANGCODE('p', 'u', 's'),
		eTxtCapLC_que = MAKE_TEXTCAP_LANGCODE('q', 'u', 'e'),
		eTxtCapLC_roh = MAKE_TEXTCAP_LANGCODE('r', 'o', 'h'),
		eTxtCapLC_raj = MAKE_TEXTCAP_LANGCODE('r', 'a', 'j'),
		eTxtCapLC_rap = MAKE_TEXTCAP_LANGCODE('r', 'a', 'p'),
		eTxtCapLC_rar = MAKE_TEXTCAP_LANGCODE('r', 'a', 'r'),
		eTxtCapLC_roa = MAKE_TEXTCAP_LANGCODE('r', 'o', 'a'),

		// PSR 58
		eTxtCapLC_ron = MAKE_TEXTCAP_LANGCODE('r', 'o', 'n'),
		eTxtCapLC_rum = MAKE_TEXTCAP_LANGCODE('r', 'u', 'm'), // "rum" same as "ron"
		eTxtCapLC_rom = MAKE_TEXTCAP_LANGCODE('r', 'o', 'm'),
		eTxtCapLC_run = MAKE_TEXTCAP_LANGCODE('r', 'u', 'n'),
		eTxtCapLC_rus = MAKE_TEXTCAP_LANGCODE('r', 'u', 's'),
		eTxtCapLC_sal = MAKE_TEXTCAP_LANGCODE('s', 'a', 'l'),
		eTxtCapLC_sam = MAKE_TEXTCAP_LANGCODE('s', 'a', 'm'),
		eTxtCapLC_smi = MAKE_TEXTCAP_LANGCODE('s', 'm', 'i'),
		eTxtCapLC_smo = MAKE_TEXTCAP_LANGCODE('s', 'm', 'o'),

		eTxtCapLC_sad = MAKE_TEXTCAP_LANGCODE('s', 'a', 'd'),
		eTxtCapLC_sag = MAKE_TEXTCAP_LANGCODE('s', 'a', 'g'),
		eTxtCapLC_san = MAKE_TEXTCAP_LANGCODE('s', 'a', 'n'),
		eTxtCapLC_sat = MAKE_TEXTCAP_LANGCODE('s', 'a', 't'),
		eTxtCapLC_srd = MAKE_TEXTCAP_LANGCODE('s', 'r', 'd'),
		eTxtCapLC_sas = MAKE_TEXTCAP_LANGCODE('s', 'a', 's'),
		eTxtCapLC_sco = MAKE_TEXTCAP_LANGCODE('s', 'c', 'o'),
		eTxtCapLC_sel = MAKE_TEXTCAP_LANGCODE('s', 'e', 'l'),

		eTxtCapLC_sem = MAKE_TEXTCAP_LANGCODE('s', 'e', 'm'),
		eTxtCapLC_srp = MAKE_TEXTCAP_LANGCODE('s', 'r', 'p'),
		eTxtCapLC_srr = MAKE_TEXTCAP_LANGCODE('s', 'r', 'r'),
		eTxtCapLC_shn = MAKE_TEXTCAP_LANGCODE('s', 'h', 'n'),
		eTxtCapLC_sna = MAKE_TEXTCAP_LANGCODE('s', 'n', 'a'),
		eTxtCapLC_sid = MAKE_TEXTCAP_LANGCODE('s', 'i', 'd'),
		eTxtCapLC_bla = MAKE_TEXTCAP_LANGCODE('b', 'l', 'a'),
		eTxtCapLC_snd = MAKE_TEXTCAP_LANGCODE('s', 'n', 'd'),

		eTxtCapLC_sin = MAKE_TEXTCAP_LANGCODE('s', 'i', 'n'),
		eTxtCapLC_sit = MAKE_TEXTCAP_LANGCODE('s', 'i', 't'),
		eTxtCapLC_sio = MAKE_TEXTCAP_LANGCODE('s', 'i', 'o'),
		eTxtCapLC_den = MAKE_TEXTCAP_LANGCODE('d', 'e', 'n'),
		eTxtCapLC_sla = MAKE_TEXTCAP_LANGCODE('s', 'l', 'a'),
		eTxtCapLC_slk = MAKE_TEXTCAP_LANGCODE('s', 'l', 'k'),
		eTxtCapLC_slo = MAKE_TEXTCAP_LANGCODE('s', 'l', 'o'), // "slo" same as "slk"
		eTxtCapLC_slv = MAKE_TEXTCAP_LANGCODE('s', 'l', 'v'),
		eTxtCapLC_sog = MAKE_TEXTCAP_LANGCODE('s', 'o', 'g'),

		// PSR 59
		eTxtCapLC_som = MAKE_TEXTCAP_LANGCODE('s', 'o', 'm'),
		eTxtCapLC_son = MAKE_TEXTCAP_LANGCODE('s', 'o', 'n'),
		eTxtCapLC_snk = MAKE_TEXTCAP_LANGCODE('s', 'n', 'k'),
		eTxtCapLC_wen = MAKE_TEXTCAP_LANGCODE('w', 'e', 'n'),
		eTxtCapLC_nso = MAKE_TEXTCAP_LANGCODE('n', 's', 'o'),
		eTxtCapLC_sot = MAKE_TEXTCAP_LANGCODE('s', 'o', 't'),
		eTxtCapLC_sai = MAKE_TEXTCAP_LANGCODE('s', 'a', 'i'),
		eTxtCapLC_spa = MAKE_TEXTCAP_LANGCODE('s', 'p', 'a'),

		eTxtCapLC_suk = MAKE_TEXTCAP_LANGCODE('s', 'u', 'k'),
		eTxtCapLC_sux = MAKE_TEXTCAP_LANGCODE('s', 'u', 'x'),
		eTxtCapLC_sun = MAKE_TEXTCAP_LANGCODE('s', 'u', 'n'),
		eTxtCapLC_sus = MAKE_TEXTCAP_LANGCODE('s', 'u', 's'),
		eTxtCapLC_swa = MAKE_TEXTCAP_LANGCODE('s', 'w', 'a'),
		eTxtCapLC_ssw = MAKE_TEXTCAP_LANGCODE('s', 's', 'w'),
		eTxtCapLC_swe = MAKE_TEXTCAP_LANGCODE('s', 'w', 'e'),
		eTxtCapLC_syr = MAKE_TEXTCAP_LANGCODE('s', 'y', 'r'),

		eTxtCapLC_tgl = MAKE_TEXTCAP_LANGCODE('t', 'g', 'l'),
		eTxtCapLC_tah = MAKE_TEXTCAP_LANGCODE('t', 'a', 'h'),
		eTxtCapLC_tai = MAKE_TEXTCAP_LANGCODE('t', 'a', 'i'),
		eTxtCapLC_tgk = MAKE_TEXTCAP_LANGCODE('t', 'g', 'k'),
		eTxtCapLC_tmh = MAKE_TEXTCAP_LANGCODE('t', 'm', 'h'),
		eTxtCapLC_tam = MAKE_TEXTCAP_LANGCODE('t', 'a', 'm'),
		eTxtCapLC_tat = MAKE_TEXTCAP_LANGCODE('t', 'a', 't'),
		eTxtCapLC_tel = MAKE_TEXTCAP_LANGCODE('t', 'e', 'l'),

		eTxtCapLC_ter = MAKE_TEXTCAP_LANGCODE('t', 'e', 'r'),
		eTxtCapLC_tet = MAKE_TEXTCAP_LANGCODE('t', 'e', 't'),
		eTxtCapLC_tha = MAKE_TEXTCAP_LANGCODE('t', 'h', 'a'),
		eTxtCapLC_bod = MAKE_TEXTCAP_LANGCODE('b', 'o', 'd'),
		eTxtCapLC_tib = MAKE_TEXTCAP_LANGCODE('t', 'i', 'b'), // "tib" same as "bod"
		eTxtCapLC_tig = MAKE_TEXTCAP_LANGCODE('t', 'i', 'g'),
		eTxtCapLC_tir = MAKE_TEXTCAP_LANGCODE('t', 'i', 'r'),
		eTxtCapLC_tem = MAKE_TEXTCAP_LANGCODE('t', 'e', 'm'),
		eTxtCapLC_tiv = MAKE_TEXTCAP_LANGCODE('t', 'i', 'v'),

		// PSR 60
		eTxtCapLC_tli = MAKE_TEXTCAP_LANGCODE('t', 'l', 'i'),
		eTxtCapLC_tpi = MAKE_TEXTCAP_LANGCODE('t', 'p', 'i'),
		eTxtCapLC_tkl = MAKE_TEXTCAP_LANGCODE('t', 'k', 'l'),
		eTxtCapLC_tog = MAKE_TEXTCAP_LANGCODE('t', 'o', 'g'),
		eTxtCapLC_ton = MAKE_TEXTCAP_LANGCODE('t', 'o', 'n'),
		eTxtCapLC_tsi = MAKE_TEXTCAP_LANGCODE('t', 's', 'i'),
		eTxtCapLC_tso = MAKE_TEXTCAP_LANGCODE('t', 's', 'o'),
		eTxtCapLC_tsn = MAKE_TEXTCAP_LANGCODE('t', 's', 'n'),

		eTxtCapLC_tum = MAKE_TEXTCAP_LANGCODE('t', 'u', 'm'),
		eTxtCapLC_tur = MAKE_TEXTCAP_LANGCODE('t', 'u', 'r'),
		eTxtCapLC_ota = MAKE_TEXTCAP_LANGCODE('o', 't', 'a'),
		eTxtCapLC_tuk = MAKE_TEXTCAP_LANGCODE('t', 'u', 'k'),
		eTxtCapLC_tvl = MAKE_TEXTCAP_LANGCODE('t', 'v', 'l'),
		eTxtCapLC_tyv = MAKE_TEXTCAP_LANGCODE('t', 'y', 'v'),
		eTxtCapLC_twi = MAKE_TEXTCAP_LANGCODE('t', 'w', 'i'),
		eTxtCapLC_uga = MAKE_TEXTCAP_LANGCODE('u', 'g', 'a'),

		eTxtCapLC_uig = MAKE_TEXTCAP_LANGCODE('u', 'i', 'g'),
		eTxtCapLC_ukr = MAKE_TEXTCAP_LANGCODE('u', 'k', 'r'),
		eTxtCapLC_umb = MAKE_TEXTCAP_LANGCODE('u', 'm', 'b'),
		eTxtCapLC_und = MAKE_TEXTCAP_LANGCODE('u', 'n', 'd'),
		eTxtCapLC_urd = MAKE_TEXTCAP_LANGCODE('u', 'r', 'd'),
		eTxtCapLC_uzb = MAKE_TEXTCAP_LANGCODE('u', 'z', 'b'),
		eTxtCapLC_vai = MAKE_TEXTCAP_LANGCODE('v', 'a', 'i'),
		eTxtCapLC_ven = MAKE_TEXTCAP_LANGCODE('v', 'e', 'n'),

		eTxtCapLC_vie = MAKE_TEXTCAP_LANGCODE('v', 'i', 'e'),
		eTxtCapLC_vol = MAKE_TEXTCAP_LANGCODE('v', 'o', 'l'),
		eTxtCapLC_vot = MAKE_TEXTCAP_LANGCODE('v', 'o', 't'),
		eTxtCapLC_wak = MAKE_TEXTCAP_LANGCODE('w', 'a', 'k'),
		eTxtCapLC_wal = MAKE_TEXTCAP_LANGCODE('w', 'a', 'l'),
		eTxtCapLC_war = MAKE_TEXTCAP_LANGCODE('w', 'a', 'r'),
		eTxtCapLC_was = MAKE_TEXTCAP_LANGCODE('w', 'a', 's'),
		eTxtCapLC_cym = MAKE_TEXTCAP_LANGCODE('c', 'y', 'm'),
		eTxtCapLC_wel = MAKE_TEXTCAP_LANGCODE('w', 'e', 'l'),	// "wel" same as "cym"

		// PSR 61
		eTxtCapLC_wol = MAKE_TEXTCAP_LANGCODE('w', 'o', 'l'),
		eTxtCapLC_xho = MAKE_TEXTCAP_LANGCODE('x', 'h', 'o'),
		eTxtCapLC_sah = MAKE_TEXTCAP_LANGCODE('s', 'a', 'h'),
		eTxtCapLC_yao = MAKE_TEXTCAP_LANGCODE('y', 'a', 'o'),
		eTxtCapLC_yap = MAKE_TEXTCAP_LANGCODE('y', 'a', 'p'),
		eTxtCapLC_yid = MAKE_TEXTCAP_LANGCODE('y', 'i', 'd'),
		eTxtCapLC_yor = MAKE_TEXTCAP_LANGCODE('y', 'o', 'r'),
		eTxtCapLC_ypk = MAKE_TEXTCAP_LANGCODE('y', 'p', 'k'),

		eTxtCapLC_znd = MAKE_TEXTCAP_LANGCODE('z', 'n', 'd'),
		eTxtCapLC_zap = MAKE_TEXTCAP_LANGCODE('z', 'a', 'p'),
		eTxtCapLC_zen = MAKE_TEXTCAP_LANGCODE('z', 'e', 'n'),
		eTxtCapLC_zha = MAKE_TEXTCAP_LANGCODE('z', 'h', 'a'),
		eTxtCapLC_zul = MAKE_TEXTCAP_LANGCODE('z', 'u', 'l'),
		eTxtCapLC_zun = MAKE_TEXTCAP_LANGCODE('z', 'u', 'n'),
	};

private:
	void Reset(void);
	void ResetSettingPSRs(void);
	void ResetStatusPSRs(void);
	void ResetGPRs(void);

	// PSR Event notification
	void SendPSREvent(UInt32 uiPSR, UInt32 uiValue);

	// GPR Event notification
	void SendGPREvent(UInt32 uiGPR, UInt32 uiValue);

private:
	UInt32		m_GPRs[BDROM_REG_TOTAL_GPR];
	UInt32		m_PSRs[BDROM_REG_TOTAL_PSR];

	ref_ptr<NotificationEventManager>				m_pNEManager;
};


//-------------------------------------------------------------------------------------------------------
// Structs with bitfields for using with PSRs that have multiple fields within them.
//
namespace BDROM
{

// PSR_PGTextStream
// Use for PSR2 (PGTextST Stream Number)
//
struct PSR_PGTextStream
{
	// PSR fields
	unsigned	primary			: 12;
	unsigned	reserved1		: 4;
	unsigned	pip				: 12;
	unsigned	reserved2		: 2;
	unsigned	pip_valid_flag	: 1;
	unsigned	disp_s_flag		: 1;

	// constructor, converters
	PSR_PGTextStream(UInt32 num = 0x0FFF0FFF)
	{
		memcpy(this, &num, sizeof(UInt32));
	}
	PSR_PGTextStream& operator=(UInt32 num)
	{
		memcpy(this, &num, sizeof(UInt32));
		return *this;
	}
	operator UInt32()
	{
		UInt32 num;
		memcpy(&num, this, sizeof(UInt32));
		return num;
	}
};

// PSR_SecondaryAudioVideo
// Use for PSR14 (Secondary Audio Stream Number and Secondary Video Stream Number)
//
struct PSR_SecondaryAudioVideo
{
	// PSR fields
	unsigned	audio			: 8;
	unsigned	video			: 8;
	unsigned	reserved		: 8;
	unsigned	videoSize		: 4;
	unsigned	padding			: 2;
	unsigned	disp_a_flag		: 1;
	unsigned	disp_v_flag		: 1;

	// constructor, converters
	PSR_SecondaryAudioVideo()
	{
		audio = video = 0xFF;
		reserved = videoSize = padding = disp_a_flag = disp_v_flag = 0;
	}
	PSR_SecondaryAudioVideo(UInt32 num)
	{
		memcpy(this, &num, sizeof(UInt32));
	}
	PSR_SecondaryAudioVideo& operator=(UInt32 num)
	{
		memcpy(this, &num, sizeof(UInt32));
		return *this;
	}
	operator UInt32()
	{
		UInt32 i;
		memcpy(&i, this, sizeof(UInt32));
		return i;
	}
};

// PSR_LangCodePGTextST
// Use for PSR17 (Language Code for PG/TextST)
//
struct PSR_LangCodePGTextST
{
	// PSR fields
	unsigned	langCode3		: 8;
	unsigned	langCode2		: 8;
	unsigned	langCode1		: 8;
	unsigned	reserved		: 8;

	// constructor, converters
	PSR_LangCodePGTextST()
	{
		langCode3 = langCode2 = langCode1 = reserved = 0xFF;
	}
	PSR_LangCodePGTextST(UInt32 num)
	{
		memcpy(this, &num, sizeof(UInt32));
	}
	PSR_LangCodePGTextST& operator=(UInt32 num)
	{
		memcpy(this, &num, sizeof(UInt32));
		return *this;
	}
	operator UInt32()
	{
		UInt32 i;
		memcpy(&i, this, sizeof(UInt32));
		return i;
	}
};

// PSR_PlayerCapTextST
// Use for PSR30 (Player Capability for Text Subtitle)
//
struct PSR_PlayerCapTextST
{
	// PSR fields
	unsigned	reserved1						: 3;
	unsigned	cap_Shift_JIS					: 1;
	unsigned	cap_EUC_KR						: 1;
	unsigned	cap_GB18030_2000				: 1;
	unsigned	cap_CN_GB						: 1;
	unsigned	cap_BIG5						: 1;
	unsigned	reserved2						: 4;
	unsigned	cap_Inline_Style				: 1;
	unsigned	cap_Outline_Border				: 1;
	unsigned	cap_Txt_Flow_Align				: 1;
	unsigned	cap_User_Changeable_StyleSet	: 1;
	unsigned	cap_Unsupported_Language		: 1;
	unsigned	reserved3						: 15;

	// constructor, converters
	PSR_PlayerCapTextST()
	{
		reserved1						= 0;
		cap_Shift_JIS					= 0;
		cap_EUC_KR						= 0;
		cap_GB18030_2000				= 0;
		cap_CN_GB						= 0;
		cap_BIG5						= 0;
		reserved2						= 0;
		cap_Inline_Style				= 0;
		cap_Outline_Border				= 0;
		cap_Txt_Flow_Align				= 0;
		cap_User_Changeable_StyleSet	= 0;
		cap_Unsupported_Language		= 0;
		reserved3						= 0;
	}
	PSR_PlayerCapTextST(UInt32 num)
	{
		memcpy(this, &num, sizeof(UInt32));
	}
	operator UInt32()
	{
		UInt32 i;
		memcpy(&i, this, sizeof(UInt32));
		return i;
	}
};

// PSR_Profile
// Use for PSR31 (Player Profile / Player Version)
//
struct PSR_Profile
{
	// PSR fields
	unsigned	version			: 16;
	unsigned	profile			: 4;
	unsigned	reserved		: 12;

	// constructor, converters
	PSR_Profile()
	{
		version = profile = reserved = 0;
	}
	PSR_Profile(UInt32 num)
	{
		memcpy(this, &num, sizeof(UInt32));
	}
	operator UInt32()
	{
		UInt32 i;
		memcpy(&i, this, sizeof(UInt32));
		return i;
	}
};

};

#endif
