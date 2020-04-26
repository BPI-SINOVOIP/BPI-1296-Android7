//-----------------------------------------------------------------------------
// HDMVDecoder_Types_Internal.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __HDMVDECODER_TYPES_INTERNAL_H__
#define __HDMVDECODER_TYPES_INTERNAL_H__

#include "BDROM_NavigatorTypes.h"
#include "HDMVDecoder_Types.h"

namespace BDROM_HDMVDecoder
{

// Also see HDMVDecoder_Types.h for more of these

const UInt16	kIGPalette_BaseID					= 0;				// base ID used to make IG Palette IDs unique
const UInt16	kPGPalette_BaseID					= 256;				// base ID used to make PG Palette IDs unique
const UInt16	kTextSTPalette_BaseID				= 264;				// base ID used to make TextST Palette IDs unique

const UInt16	kIGPalette_Max						= 256;				// max active IG palettes allowed
const UInt16	kPGPalette_Max						= 8;				// max active PG palettes allowed
const UInt16	kTextSTPalette_Max					= 2;				// max active TextST palettes allowed (1 from DSS, and 1 from current DPS)

const UInt16	kMaxEntriesInPalette				= 256;				// max palette entries in a hdmv palette

const UInt16	kPGPage_Max							= 255;				// max active IG pages allowed
const UInt8		kPGPage_MaxPageId					= 0xFE;				// largest valid IG Page ID (254)

const UInt16	kIGObject_Max						= 4096;				// max active IG objects allowed
const UInt16	kPGObject_Max						= 64;				// max active PG objects allowed
const UInt16	kTextSTObject_Max					= 2;				// max active TextST objects allowed	(used for sprite based display model)
const UInt16	kTextSTSurface_Max					= 4;				// max active TextST surfaces allowed	(used for frame based display model)

const UInt16	kObjectId_Invalid					= 0xFFFF;			// Invalid Object Id

//				kPageId_Invalid											is defined in HDMVDecoder_Types.h
//				kButtonId_Invalid										is defined in HDMVDecoder_Types.h
//				kMaxButtonsPerPage										is defined in HDMVDecoder_Types.h
//				kButtonNumbericSelectVal_None							is defined in HDMVDecoder_Types.h

const UInt16	kIGButton_MaxNavCommands			= 255;				// max number of navigation commands allowed in a button

const UInt32	kCompSegId_Invalid					= 0xFFFFFFFF;		// Invalid composition segment id

const UInt16	kPageBOGId_Invalid					= 0xFFF0;			// No valid 'selected' BOG in Page

const UInt16	kPageButtonId_InvalidSelect			= 0xFFF0;			// No valid selected Button in Page
const UInt16	kPageButtonId_DefaultSelect			= 0xFFFF;			// Button in Page must be selected by Player
const UInt16	kBOGButtonId_None					= 0xFFFF;			// No Button from BOG to be made valid

// values specific to default_activated_button_id_ref of a Page
const UInt16	kPageDefActBtnId_ActivateSelBtn		= 0xFFFE;			// Currently Selected Button in Page can be auto-activated at Comp Seg Timeout
const UInt16	kPageDefActBtnId_NoActivate			= 0xFFFF;			// No Button in Page can be auto-activated at Comp Seg Timeout

const UInt32	kPageEffects_Min					= 1;				// Min number of in/out effects
const UInt32	kPageEffects_Max					= 128;				// Max number of in/out effects

const UInt32	kSleepDur_None						= 1;				// Almost none
const UInt32	kSleepDur_FrameDur_60Hz				= 16;				// ~ 1/60 of a second
const UInt32	kSleepDur_Infinite					= 0xFFFFFFFF;		// infinite sleep duration

const UInt32	kRegionStylesInDSS_Max				= 60;				// Max number of region_style()s allowed (in a DSS)
const UInt32	kUserStylesInDSS_Max				= 25;				// Max number of user_selectable_style()s allowed (in a DSS)

const UInt32	kRegionsInDPS_Max					= 2;				// Max number of dialog_region() in a DPS of Text Subtitle stream
const UInt32	kSubtitleCharsPerDPS_Max			= 200;				// Max number of subtitle characters allowed in a DPS

const UInt16	kDPSNum_Invalid						= 0;				// Invalid DPS Number

//__________________________________________________________________________________________________________________________________________________
// BDROM HDMV PG, IG and TextST Stream structures
// basic structures present in PG, IG and TextST streams
//

typedef enum tagHDMVSegmentType
{
	kHDMVSeg_Reserved				= 0x00,
	kHDMVSeg_PaletteDefSeg			= 0x14,
	kHDMVSeg_ObjectDefSeg			= 0x15,
	kHDMVSeg_PresentationCompSeg	= 0x16,
	kHDMVSeg_WindowDefSeg			= 0x17,
	kHDMVSeg_InteractiveCompSeg		= 0x18,
	kHDMVSeg_EndOfDisplaySeg		= 0x80,
	kHDMVSeg_DialogStyleSeg			= 0x81,
	kHDMVSeg_DialogPresentationSeg	= 0x82,
	kHDMVSeg_Private_Metadata		= 0xF0				// Private Segment (used for inserting metadata into EB)
} HDMVSegmentType;

typedef enum tagIGDisplayMode
{
	kIGDispMode_PopUp,
	kIGDispMode_AlwaysOn
} IGDisplayMode;

typedef enum tagHDMVCompositionState
{
	kHDMVCompState_NormalCase,
	kHDMVCompState_AcquisitionPoint,
	kHDMVCompState_EpochStart,
	kHDMVCompState_EpochContinue
} HDMVCompositionState;

typedef enum tagIGStreamModel
{
	kIGStreamModel_Muxed,
	kIGStreamModel_NonMuxed,
	kIGStreamModel_Unknown	= 0xFF		// value not defined by spec
} IGStreamModel;

typedef enum tagIGUserInterfaceModel
{
	kIGUIModel_AlwaysOn,
	kIGUIModel_PopUp,
	kIGUIModel_Unknown	= 0xFF			// value not defined by spec
} IGUserInterfaceModel;

typedef enum tagIGButtonState
{
	kIGButtonState_Disabled,		// i.e., not visible
	kIGButtonState_Normal,
	kIGButtonState_Selected,
	kIGButtonState_Activated
} IGButtonState;

struct hdmv_segment_descriptor
{
	HDMVSegmentType			msegment_type;
	UInt16					msegment_length;
};

struct hdmv_video_descriptor
{
	UInt16					mvideo_width;
	UInt16					mvideo_height;
	BDROM_VideoFrameRate	mframe_rate;
};

struct hdmv_composition_descriptor
{
	UInt16					mcomposition_number;
	HDMVCompositionState	mcomposition_state;
};

struct hdmv_sequence_descriptor
{
	UInt8					mfirst_in_sequence_flag;
	UInt8					mlast_in_sequence_flag;
};

struct hdmv_window
{
	UInt8					mwindow_id;
	UInt16					mwindow_horizontal_position;
	UInt16					mwindow_vertical_position;
	UInt16					mwindow_width;
	UInt16					mwindow_height;
};

struct hdmv_cropping_rectangle
{
	UInt16					mobject_cropping_horizontal_position;
	UInt16					mobject_cropping_vertical_position;
	UInt16					mobject_cropping_width;
	UInt16					mobject_cropping_height;
};

struct hdmv_composition_object
{
	UInt16					mobject_id_ref;
	UInt16					mwindow_id_ref;
	UInt8					mobject_cropped_flag;
	UInt8					mforced_on_flag;
	UInt16					mcomposition_object_horizontal_position;
	UInt16					mcomposition_object_vertical_position;
	hdmv_cropping_rectangle	mcropping_rectangle;
};

struct hdmv_palette_entry
{
	UInt8					mpalette_entry_id;
	UInt8					Y_value;
	UInt8					Cr_value;
	UInt8					Cb_value;
	UInt8					T_value;
};

struct hdmv_palette
{
	UInt8					mpalette_id;
	UInt8					mpalette_version_number;
	hdmv_palette_entry		mpalette_entry[kMaxEntriesInPalette];

	UInt64					mDTS;
	UInt64					mPTS;
};

struct hdmv_palette_definition_segment
{
	UInt32					munique_id;				// unique identifier (not part of spec)

	hdmv_segment_descriptor	msegment_descriptor;
	UInt8					mpalette_id;
	UInt8					mpalette_version_number;
	hdmv_palette *			mpalette;

	UInt64					mDTS;					// Encoded DTS (DTS extracted from PES packet header)
	UInt64					mPTS;					// Encoded PTS (PTS extracted from PES packet header)
	UInt64					mPlatformPTS;			// Platform DTS (Encoded DTS converted to Platform presentation time)
	UInt64					mPlatformDTS;			// Platform PTS (Encoded PTS converted to Platform presentation time)

	hdmv_palette_definition_segment()
	{
		mpalette_id = mpalette_version_number = 0xFF;
		mpalette = 0;
	}
};

struct hdmv_object
{
	UInt16					mobject_id;
	UInt8					mobject_version_number;

	UInt32					mobject_data_length;
	UInt16					mobject_width;
	UInt16					mobject_height;
	UInt8 *					mobject_data;				// decoded object data

	UInt64					mDTS;
	UInt64					mPTS;

	hdmv_object()
	{
		mobject_id = kObjectId_Invalid;
		mobject_version_number = 0xFF;
		mobject_data = 0;
	}
};

struct hdmv_object_definition_segment
{
	UInt32						munique_id;				// unique identifier (not part of spec)

	hdmv_segment_descriptor		msegment_descriptor;
	UInt16						mobject_id;
	UInt8						mobject_version_number;
	hdmv_sequence_descriptor	msequence_descriptor;
	hdmv_object *				mobject;
	UInt64						mDTS;
	UInt64						mPTS;

	hdmv_object_definition_segment()
	{
		munique_id = 0;
		mobject = 0;
		mobject_id = kObjectId_Invalid;
		mobject_version_number = 0xFF;
	}
};

struct hdmv_end_of_display_segment
{
	hdmv_segment_descriptor		msegment_descriptor;
	UInt64						mDTS;
	UInt64						mPTS;
};

//__________________________________________________________________________________________________________________________________________________
// IG Graphics composition - Page, Button, Effects
//

//--------------------------------------------------------------------------
// ig_effect
/// @brief	A single effect (in an In/Out effect sequence)
///
struct ig_effect
{
	// data members defined by the spec
	//
	UInt32						meffect_duration;		// duration of effect in 90KHz units
	UInt8						mpalette_id_ref;
	UInt8						mnumber_of_composition_objects;
	hdmv_composition_object *	mcomposition_object;

	// member functions
	//
	ig_effect();
	~ig_effect();
	void CreateEffCompObject(UInt8 uiNumCompObjs);
	void DestroyEffCompObject(void);
};

//--------------------------------------------------------------------------
// ig_effect_sequence
/// @brief	An In/Out effect sequence for a page
///
struct ig_effect_sequence
{
	// data members defined by the spec
	//
	UInt8						mnumber_of_windows;
	hdmv_window *				mwindow;
	UInt8						mnumber_of_effects;
	ig_effect *					meffect;

	// member functions
	//
	ig_effect_sequence();
	~ig_effect_sequence();
	void CreateEffSeqWindows(UInt8 uiNumWindows);
	void DestroyEffSeqWindows(void);
	void CreateEffects(UInt8 uiNumEffects);
	void DestroyEffects(void);
};

//--------------------------------------------------------------------------
// ig_navigation_command
/// @brief	Stores bytes of a Navigation Command
///
struct ig_navigation_command
{
	unsigned char				mcommand_bytes[12];
};

//--------------------------------------------------------------------------
// ig_button
/// @brief	Represents a button
///
struct ig_button
{
	// data members defined by the spec
	//
	UInt16						mbutton_id;
	UInt16						mbutton_numberic_select_value;
	bool						mauto_action_flag;
	UInt16						mbutton_horizontal_position;
	UInt16						mbutton_vertical_position;

	UInt16						mupper_button_id_ref;
	UInt16						mlower_button_id_ref;
	UInt16						mleft_button_id_ref;
	UInt16						mright_button_id_ref;

	UInt16						mnormal_start_object_id_ref;
	UInt16						mnormal_end_object_id_ref;
	bool						mnormal_repeat_flag;

	UInt8						mselected_state_sound_id_ref;
	UInt16						mselected_start_object_id_ref;
	UInt16						mselected_end_object_id_ref;
	bool						mselected_repeat_flag;

	UInt8						mactivated_state_sound_id_ref;
	UInt16						mactivated_start_object_id_ref;
	UInt16						mactivated_end_object_id_ref;

	UInt16						mnumber_of_navigation_commands;
	ig_navigation_command *		mnavigation_command;

	// extra implementation members
	//
	IGButtonState				mState;					// button state information (not in spec)
	bool						mbAnimated;				// indicates if button is animated (in 1 or more states).
	UInt16						mNormalObjectIdRef;		// Animated: object id being used for curr display. Non-animated: same as mnormal_start_object_id_ref
	UInt16						mSelectedObjectIdRef;	// Animated: object id being used for curr display. Non-animated: same as mselected_start_object_id_ref
	UInt16						mActivatedObjectIdRef;	// Animated: object id being used for curr display. Non-animated: same as mactivated_start_object_id_ref
	
	// member functions
	//
	ig_button();
	~ig_button();
	void        GetUserInteractionInfo(bool & bSelectable, bool & bClickable);		// returns info about whether user can select, click on this button
	void		CreateNavigationCommands(UInt16 uiNumNavCmds);
	void		DestroyNavigationCommands(void);
	UInt16		GetCurrentObjectId(void);				// returns object id of current button state
	UInt16		GetNextObjectId(void);					// returns next object id of animated button
	UInt16      GetObjectId(IGButtonState state);       // returns object id of specified state
	void		SetNextObjectId(void);					// updates current state object id for an animated button
	void		ResetObjectId(void);					// resets current object id to the first object id - Resets animation for animated buttons
};

//--------------------------------------------------------------------------
// ig_button_overlap_group
/// @brief	Represents a button overlap group
///
struct ig_button_overlap_group
{
	// data members defined by the spec
	//
	UInt16						mdefault_valid_button_id_ref;
	UInt8						mnumber_of_buttons;
	ig_button *					mbutton;

	// extra implementation members
	//
	UInt16						mValidButtonId;				// id of button that is currently valid in this group

	// members functions
	//
	ig_button_overlap_group();
	~ig_button_overlap_group();
	void		CreateButtons(UInt8 uiNumButtons);
	void		DestroyButtons(void);
	ig_button *	GetValidButton(void);
	ig_button *	GetButton(UInt16 uiBtnId);
	bool        IsButtonDifined(UInt16 uiBtnId);
};

//--------------------------------------------------------------------------
// ig_page
/// @brief	Represents a single Page
///
struct ig_page
{
public:
	// data members defined by the spec
	//
	UInt8						mpage_id;
	UInt8						mpage_version_number;
	UInt8						mUO_mask_table[8];
	ig_effect_sequence			min_effects;
	ig_effect_sequence			mout_effects;
	UInt8						manimation_frame_rate_code;
	UInt16						mdefault_selected_button_id_ref;
	UInt16						mdefault_activated_button_id_ref;
	UInt8						mpalette_id_ref;
	UInt8						mnumber_of_BOGs;
	ig_button_overlap_group *	mbutton_overlap_group;

	// extra implementation members
	//
	UInt16						mSelectedBOGId;			// index of BOG that contains the currently selected button
	UInt16						mSelectedButtonId;		// ID of the currently selected button
	bool						mbButtonActivated;		// indicates that selected button has been activated
	bool						mbAnimated;				// indicates if a page has 1 or more animated buttons
	UInt16						mLastEnabledBtnId;		// ID of button that was last enabled using EnableButton cmd
	UInt16						mLastDisabledBtnId;		// ID of button that was last disabled using DisableButton cmd
	UInt16						mLastActivatedBtnId;	// ID of button that was last activated

	// member functions
	//
	ig_page();
	~ig_page();
	void						CreateBOGs(UInt8 uiNumBOGs);
	void						DestroyBOGs(void);
	ig_button_overlap_group *	GetSelectedButtonGroup(void);
	ig_button_overlap_group *	GetButtonGroup(UInt16 uiBtnId);
	UInt16						GetButtonGroupId(UInt16 uiBtnId);
	ig_button *					GetButton(UInt16 uiBtnId);
	ig_button *					GetSelectedButton(void);
	ig_button *					GetActivatedButton(void);
	UInt16						GetFirstValidDefaultButtonId(void);				// returns ID of first valid default_valid_button_id_ref from its BOGs
	bool						IsButtonIdValidDefaultButtonId(UInt16 uiBtnId);	// returns true if button id is a default_valid_button_id_ref from its BOGs
	bool                        IsButtonIdValid(UInt16 uiBtnId);                // returns true if uiBtnId value have a corresponding button_id in the page
};

struct ig_interactive_composition
{
	UInt32						minteractive_composition_length;
	IGStreamModel				mstream_model;
	IGUserInterfaceModel		muser_interface_model;
	UInt64						mcomposition_time_out_pts;
	UInt64						mselection_time_out_pts;
	UInt32						muser_time_out_duration;
	UInt8						mnumber_of_pages;
	ig_page **					mpage;							// pages must be explicitly deleted by IG Decoder as page lifetime can be greater than ICS lifetime

	ig_interactive_composition()
	{
		mnumber_of_pages = 0;
		mpage = 0;
	}
};

struct ig_interactive_composition_segment
{
	UInt32						munique_id;				// unique identifier (not part of spec)
	UInt64						mDTS;					// DTS extracted from PES packet header
	UInt64						mPTS;					// PTS extracted from PES packet header

	hdmv_segment_descriptor		msegment_descriptor;
	hdmv_video_descriptor		mvideo_descriptor;
	hdmv_composition_descriptor	mcomposition_descriptor;
	hdmv_sequence_descriptor	msequence_descriptor;
	ig_interactive_composition	minteractive_composition;
};

//__________________________________________________________________________________________________________________________________________________
// PG Graphics composition - Page, Button, Effects
//

struct pg_presentation_composition_segment
{
	hdmv_segment_descriptor		msegment_descriptor;
	hdmv_video_descriptor		mvideo_descriptor;
	hdmv_composition_descriptor	mcomposition_descriptor;

	UInt8						mpalette_update_flag;
	UInt8						mpalette_id_ref;
	UInt8						mnumber_of_composition_objects;
	hdmv_composition_object	*	mcomposition_object;

	UInt32						munique_id;				// unique identifier (not part of spec)

	UInt64						mDTS;					// Encoded DTS (DTS extracted from PES packet header)
	UInt64						mPTS;					// Encoded PTS (PTS extracted from PES packet header)
	UInt64						mPlatformPTS;			// Platform DTS (Encoded DTS converted to Platform presentation time)
	UInt64						mPlatformDTS;			// Platform PTS (Encoded PTS converted to Platform presentation time)
};

struct pg_window_definition_segment
{
	hdmv_segment_descriptor		msegment_descriptor;

	UInt8						mnumber_of_windows;
	hdmv_window **				mwindow;				// windows must be explicitly deleted by PG decoder as window lifetime can be larger than WDS lifetime.

	UInt32						munique_id;				// unique identifier (not part of spec)
	UInt64						mDTS;					// DTS extracted from PES packet header
	UInt64						mPTS;					// PTS extracted from PES packet header
};


//__________________________________________________________________________________________________________________________________________________
// TextST composition - Dialog Style segments, Dialog Presentation segments
//

typedef enum tagTextST_PlayerStyle
{
	kTextST_PlayerStyle__Prohibited					= 0,		// content provider prohibits BDROM player from using its own style
	kTextST_PlayerStyle__Allowed					= 1			// content provider permits BDROM player to use its own style
} TextST_PlayerStyle;

typedef enum tagTextST_Font_Style
{
	kTextST_FontStyle__Normal						= 0x00,
	kTextST_FontStyle__Bold							= 0x01,
	kTextST_FontStyle__Italic						= 0x02,
	kTextST_FontStyle__BoldAndItalic				= 0x03,
	kTextST_FontStyle__OutlineBordered				= 0x04,
	kTextST_FontStyle__BoldAndOutlineBordered		= 0x05,
	kTextST_FontStyle__ItalicAndOutlineBordered		= 0x06,
	kTextST_FontStyle__BoldItalicAndOutlineBordered	= 0x07
} TextST_Font_Style;

typedef enum tagTextST_Font_OutlineThickness
{
	kTextST_FontOutline__None						= 0x00,
	kTextST_FontOutline__ThinBorder					= 0x01,
	kTextST_FontOutline__MediumBorder				= 0x02,
	kTextST_FontOutline__ThickBorder				= 0x03
} TextST_Font_OutlineThickness;

typedef enum tagTextST_Horizontal_Alignment
{
	kTextST_HorzAlign__Reserved						= 0x00,
	kTextST_HorzAlign__Left							= 0x01,
	kTextST_HorzAlign__Middle						= 0x02,
	kTextST_HerzAlign__Right						= 0x03
} TextST_Horizontal_Alignment;

typedef enum tagTextST_Vertical_Alignment
{
	kTextST_VertAlign__Reserved						= 0x00,
	kTextST_VertAlign__Top							= 0x01,
	kTextST_VertAlign__Middle						= 0x02,
	kTextST_VertAlign__Bottom						= 0x03
} TextST_Vertical_Alignment;

typedef enum tagTextST_Flow
{
	kTextST_Flow__Reserved							= 0x00,
	kTextST_Flow__L_R_char__T_B_line				= 0x01,		// Left-to-Right character progression, Top-to-Bottom line progression
	kTextST_Flow__R_L_char__T_B_line				= 0x02,		// Right-to-Left character progression, Top-to-Bottom line progression
	kTextST_Flow__T_B_char__R_L_line				= 0x03		// Top-to-Bottom character progression, Right-to-Left line progression
} TextST_Flow;

typedef enum tagTextST_Subtitle_Data_Type
{
	kTextST_STDataType__Reserved_00					= 0x00,

	kTextST_STDataType__TextStringStart				= 0x01,
	kTextST_STDataType__ChangeFontSet				= 0x02,
	kTextST_STDataType__ChangeFontStyle				= 0x03,
	kTextST_STDataType__ChangeFontSize				= 0x04,
	kTextST_STDataType__ChangeFontColor				= 0x05,

	kTextST_STDataType__Reserved_06					= 0x06,
	kTextST_STDataType__Reserved_07					= 0x07,
	kTextST_STDataType__Reserved_08					= 0x08,
	kTextST_STDataType__Reserved_09					= 0x09,

	kTextST_STDataType__LineBreak					= 0x0A,
	kTextST_STDataType__EndOfInlineStyle			= 0x0B,
} TextST_Subtitle_Data_Type;

struct txt_region_info
{
	UInt16						mregion_horizontal_pos;
	UInt16						mregion_vertical_pos;
	UInt16						mregion_width;
	UInt16						mregion_height;
	UInt8						mregion_bg_palette_entry_id_ref;
};

struct txt_region_style
{
	UInt8						mregion_style_id;
	txt_region_info				mregion_info;
	UInt16						mtext_box_horizontal_pos;
	UInt16						mtext_box_vertical_pos;
	UInt16						mtext_box_width;
	UInt16						mtext_box_height;
	UInt8						mtext_flow;
	UInt8						mtext_horizontal_alignment;
	UInt8						mtext_vertical_alignment;
	UInt8						mline_space;
	UInt8						mfont_id_ref;
	UInt8						mfont_style;
	UInt8						mfont_size;
	UInt8						mfont_palette_entry_id_ref;
	UInt8						mfont_outline_palette_entry_id_ref;
	UInt8						mfont_outline_thickness;
};

struct txt_user_control_style
{
	UInt8						muser_style_id;
	UInt8						mregion_horizontal_position_direction;
	UInt16						mregion_horizontal_position_delta;
	UInt8						mregion_vertical_position_direction;
	UInt16						mregion_vertical_position_delta;
	UInt8						mfont_size_inc_dec;
	UInt16						mfont_size_delta;
	UInt8						mtext_box_horizontal_position_direction;
	UInt16						mtext_box_horizontal_position_delta;
	UInt8						mtext_box_vertical_position_direction;
	UInt16						mtext_box_vertical_position_delta;
	UInt8						mtext_box_width_inc_dec;
	UInt16						mtext_box_width_delta;
	UInt8						mtext_box_height_inc_dec;
	UInt16						mtext_box_height_delta;
	UInt8						mline_space_inc_dec;
	UInt16						mline_space_delta;
};

struct txt_region_style_with_user_control
{
	txt_region_style			mregion_style;
	txt_user_control_style *	muser_styles;

public:
	txt_region_style_with_user_control();
	~txt_region_style_with_user_control();
};

struct txt_dialog_style_set
{
	UInt8						mplayer_style_flag;
	UInt8						mnumber_of_region_styles;
	UInt8						mnumber_of_user_styles;

	txt_region_style_with_user_control *		mregion_user_styles;

	hdmv_palette				mpalette;		// though only the palette entries in the palette are used. all other fields are ignored.

public:
	txt_dialog_style_set();
	~txt_dialog_style_set();
};

struct txt_region_font_style
{
	TextST_Font_Style			mfont_style;
	UInt8						moutline_palette_entry_id_ref;
	UInt8						mfont_outline_thickness;
};

struct txt_dialog_style_segment
{
	hdmv_segment_descriptor		msegment_descriptor;
	txt_dialog_style_set		mstyle_set;

	UInt16						mnumber_of_dialog_presentation_segments;
};

// single element of info in raw region_subtitle() data
struct txt_region_subtitle_elem
{
	TextST_Subtitle_Data_Type	mdata_type;
	UInt8						mdata_length;

	union
	{
		UInt8 *					mtext_string_start;		// if mdata_type=0x01. pointer into memory that contains raw region_subtitle() bytes
		UInt8					mfont_id;				// if mdata_type=0x02. font_id in associated clip file
		txt_region_font_style	mfont_style;			// if mdata_type=0x03. font_style, font_outline_palette_entry_id_ref, and font_outline_thickness
		UInt8					mfont_size;				// if mdata_type=0x04. font size form 8 to 144
		UInt8					mfont_color;			// if mdata_type=0x05. palette entry id
	};
};

// parsed info in region_subtitle() data
struct txt_region_subtitle
{
	std::vector<txt_region_subtitle_elem>	msubtitleElems;

	UInt32 NumElems(void)							{ return msubtitleElems.size(); }
	void AddElem(txt_region_subtitle_elem & elem)	{ msubtitleElems.push_back(elem); }
	txt_region_subtitle_elem GetElem(UInt32 index)	{ return msubtitleElems.at(index); }

	txt_region_subtitle()		{ msubtitleElems.clear(); }
	~txt_region_subtitle()		{ msubtitleElems.clear(); }
};

struct txt_region_subtitle_data
{
	UInt8						mregion_subtitle_length;
	UInt8 *						mregion_subtitle_bytes;

	txt_region_subtitle			msubtitle;

public:
	txt_region_subtitle_data();
	~txt_region_subtitle_data();
};

struct txt_dialog_region
{
	UInt8						mcontinuous_presentation_flag;
	UInt8						mforced_on_flag;
	UInt8						mregion_style_id_ref;

	txt_region_subtitle_data	mregion_subtitle_data;		// container for raw bytes of region_subtitle

	UInt16						mObjectId;					// id of object that has the bitmap of the decoded subtitle

public:
	txt_dialog_region() : mObjectId(kObjectId_Invalid)	{}
	~txt_dialog_region()								{}
};

struct txt_dialog_presentation_segment
{
	UInt32						mdps_num;					// used internally to locate DPS. DPS num starts at 1

	hdmv_segment_descriptor		msegment_descriptor;

	UInt64						mdialog_start_PTS;			// dialog_start_PTS in DPS (90 KHz value)
	UInt64						mdialog_end_PTS;			// dialog_end_PTS in DPS (90 KHz value)
	UInt64						mdialog_start_PlatformPTS;	// dialog_start_PTS + any sync offset converted to Platform PTS
	UInt64						mdialog_end_PlatformPTS;	// dialog_end_PTS + any sync offset converted to Platform PTS

	UInt8						mpalette_update_flag;
	hdmv_palette				mpalette;		// though only the palette entries in the palette are used. all other fields are ignored.

	UInt8						mnumber_of_regions;			// min 1, max 2
	txt_dialog_region			mdialog_region[kRegionsInDPS_Max];

	bool						mClearAtEndPts;				// if true, TextST Plane should be cleared at mdialog_end_PTS (presumably
															// because there is a presentation gap between this DPS and next DPS.
};

//__________________________________________________________________________________________________________________________________________________
// Metadata
//

typedef enum tagHDMVPrivateMetadataType
{
	kHDMVPrvMeta_Reserved		= 0x00,
	kHDMVPrvMeta_StreamExtent	= 0x01				// HDMV Stream Extent information
} HDMVPrivateMetadataType;


// HDMVPrivateMetadata
//	Stores metadata.
//	Metadata is stored in the HDMVSeg_EBSegment that corresponds to the segment (in the EB buffer) that this metadata applies to.
//
struct HDMVPrivateMetadata
{
public:
	HDMVPrivateMetadata() : mmetadataType(kHDMVPrvMeta_Reserved), mplaylistPts90k(0) {}
	~HDMVPrivateMetadata() {}

public:
	HDMVPrivateMetadataType		mmetadataType;		// type of metadata
	TimeValue					mplaylistPts90k;	// 90 KHz playlist PTS at which this metadata becomes valid. 0 for immediate

	// Mutually exclusive metadata (can't use a union because 1 or more of these have no default constructor
	HDMVStreamExtent			mextent;			// HDMV Stream extent
};

typedef std::vector<HDMVPrivateMetadata>		HDMVPrivateMetadataList;

};

#endif // __HDMVDECODER_TYPES_INTERNAL_H__
