//-----------------------------------------------------------------------------
// HDMVDecoder_GraphicsParserUtils.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __HDMVDECODER_GRAPHICSPARSERUTILS_H__
#define __HDMVDECODER_GRAPHICSPARSERUTILS_H__

namespace BDROM_GraphicsParserUtils
{
	//______________________________________________________________________________________________________________________________
	// common descriptors:
	//

	//-----------------------------------------------------------------------------------
	// PG, IG - segment_descriptor() parsing
	// input pointer should point to the first byte of a segment_descriptor()
	//{
	#define	kg_segment_descriptor_size							3

	#define	g_sgd__segment_type(segDesc)						segDesc[0]
	#define	g_sgd__segment_length(segDesc)						((segDesc[1] << 8) | segDesc[2])
	//}

	//-----------------------------------------------------------------------------------
	// PG, IG - video_descriptor() parsing
	// input pointer should point to the first byte of a video_descriptor()
	//{
	#define	kg_video_descriptor_size							5

	#define	g_vid__video_width(pVidDesc)						((pVidDesc[0] << 8) | pVidDesc[1])
	#define	g_vid__video_height(pVidDesc)						((pVidDesc[2] << 8) | pVidDesc[3])
	#define	g_vid__frame_rate(pVidDesc)							((pVidDesc[4] & 0xF0) >> 4)
	//}

	//-----------------------------------------------------------------------------------
	// PG, IG - composition_descriptor() parsing
	// input pointer should point to the first byte of a composition_descriptor()
	//{
	#define	kg_composition_descriptor_size						3

	#define	g_cpd__composition_number(pCompDesc)				((pCompDesc[0] << 8) | pCompDesc[1])
	#define	g_cpd__composition_state(pCompDesc)					((pCompDesc[2] & 0xC0) >> 6)
	//}

	//-----------------------------------------------------------------------------------
	// PG, IG - sequence_descriptor() parsing
	// input pointer should point to the first byte of a sequence_descriptor()
	//{
	#define	kg_sequence_descriptor_size							1

	#define	g_sqd__first_in_sequence_flag(pSeqDesc)				(( pSeqDesc[0] & 0x80) >> 7)
	#define	g_sqd__is_first_in_sequence(pSeqDesc)				(((pSeqDesc[0] & 0x80) >> 7) == 1)
	#define	g_sqd__last_in_sequence_flag(pSeqDesc)				(( pSeqDesc[0] & 0x40) >> 6)
	#define	g_sqd__is_last_in_sequence(pSeqDesc)				(((pSeqDesc[0] & 0x40) >> 6) == 1)
	//}


	//______________________________________________________________________________________________________________________________
	// common PG and IG composition structures:
	//

	//-----------------------------------------------------------------------------------
	// PG, IG - window() parsing
	// input pointer should point to the first byte of a window()
	//{
	#define	kg_window_size										9

	#define	g_wnd__window_id(pWnd)								  pWnd[0]
	#define	g_wnd__window_horizontal_position(pWnd)				((pWnd[1] << 8) | pWnd[2])
	#define	g_wnd__window_vertical_position(pWnd)				((pWnd[3] << 8) | pWnd[4])
	#define	g_wnd__window_width(pWnd)							((pWnd[5] << 8) | pWnd[6])
	#define	g_wnd__window_height(pWnd)							((pWnd[7] << 8) | pWnd[8])
	//}

	//-----------------------------------------------------------------------------------
	// PG - composition_object() parsing
	// input pointer should point to the first byte of a composition_object()
	//{
	#define	g_composition_object_size(pCompObj)					((((pCompObj[3] & 0x80) >> 7) == 1) ? 16 : 8)

	#define	g_cobj__object_id_ref(pCompObj)						((pCompObj[0]  << 8) | pCompObj[1])
	#define	g_cobj__window_id_ref(pCompObj)						  pCompObj[2]
	#define	g_cobj__object_cropped_flag(pCompObj)				((pCompObj[3]  & 0x80) >> 7)
	#define	g_cobj__forced_on_flag(pCompObj)					((pCompObj[3]  & 0x40) >> 6)
	#define	g_cobj__composition_obj_horizontal_pos(pCompObj)	((pCompObj[4]  << 8) | pCompObj[5])
	#define	g_cobj__composition_obj_vertical_pos(pCompObj)		((pCompObj[6]  << 8) | pCompObj[7])
	#define	g_cobj__obj_cropping_horizontal_pos(pCompObj)		((pCompObj[8]  << 8) | pCompObj[9])
	#define	g_cobj__obj_cropping_vertical_pos(pCompObj)			((pCompObj[10] << 8) | pCompObj[11])
	#define	g_cobj__obj_cropping_width(pCompObj)				((pCompObj[12] << 8) | pCompObj[13])
	#define	g_cobj__obj_cropping_height(pCompObj)				((pCompObj[14] << 8) | pCompObj[15])
	//}


	//______________________________________________________________________________________________________________________________
	// Palette Definition Segment (IG, PG):
	//

	//-----------------------------------------------------------------------------------
	// PG, IG - palette_definition_segment() parsing
	// input pointer should point to the byte immediately after the segment_descriptor() in a palette_definition_segment()
	//{
	#define	kg_pds__pal_desc_size								2	// size of palette_id and palette_version_number in PDS
	#define	kg_palette_entry_size								5

	#define	g_pds__palette_id(pPds)								pPds[0]
	#define	g_pds__palette_version_number(pPds)					pPds[1]
	//}
	// input pointer should point to the byte of a palette_entry()
	//{
	#define	g_pdse__palette_entry_id(pPdse)						pPdse[0]
	#define	g_pdse__Y_value(pPdse)								pPdse[1]
	#define	g_pdse__Cr_value(pPdse)								pPdse[2]
	#define	g_pdse__Cb_value(pPdse)								pPdse[3]
	#define	g_pdse__T_value(pPdse)								pPdse[4]
	//}


	//______________________________________________________________________________________________________________________________
	// Object Definition Segment (IG, PG):
	//

	//-----------------------------------------------------------------------------------
	// PG, IG - object_definition_segment() parsing
	// input pointer should point to the byte immediately after the segment_descriptor of a object_definition_segment()
	//{
	#define	kg_ods__obj_desc_size								3	// size of object_id and object_version_number in ODS
	#define g_ods__object_id(ods)								((ods[0] << 8) | ods[1])
	#define g_ods__object_version_number(ods)					ods[2]
	//}

	//-----------------------------------------------------------------------------------
	// PG, IG - object_data() parsing
	// input pointer should point to the byte of an object_data()
	//{
	#define	kg_od__obj_data_desc_size							7	// size of object_data_length, object_width and object_height in object_data()
	#define g_od__object_data_length(od)						((od[0] << 16) | (od[1] << 8) | od[2])
	#define g_od__object_width(od)								((od[3] << 8) | od[4])
	#define g_od__object_height(od)								((od[5] << 8) | od[6])
	//}


	//______________________________________________________________________________________________________________________________
	// interactive composition segment (IG):
	//

	//-----------------------------------------------------------------------------------
	// IG - composition_object() peeking
	// input pointer should point to the first byte of a interactive_composition_segment()
	//{
	#define g_ics___composition_state(ics)						((ics[kg_segment_descriptor_size+kg_video_descriptor_size+2] & 0xC0) >> 6)
	//}

	//-----------------------------------------------------------------------------------
	// IG - interactive_composition() parsing
	// input pointer should point to the first byte of an interactive_composition()
	//{
	#define	g_ic__interactive_composition_size(pIC)				(((pIC[0] << 16) | (pIC[1] << 8) | pIC[2]) + 3)

	#define	g_ic__interactive_composition_length(pIC)			(( pIC[0] << 16) | (pIC[1] << 8) | pIC[2])
	#define	g_ic__stream_model(pIC)								(( pIC[3] & 0x80) >> 7)
	#define	g_ic__is_stream_model_muxed(pIC)					(((pIC[3] & 0x80) >> 7) == 0)
	#define	g_ic__user_interface_model(pIC)						(( pIC[3] & 0x40) >> 6)
	#define	g_ic__composition_time_out_pts(pIC)					(g_ic__is_stream_model_muxed(pIC) ?																\
																	(((((UInt64)pIC[4]) & 0x01) << 32) | (pIC[5] << 24) |	(pIC[6] << 16) | (pIC[7] <<  8) | pIC[8]) :		\
																	(UInt64)0x1FFFFFFFFLL)
	#define	g_ic__selection_time_out_pts(pIC)					(g_ic__is_stream_model_muxed(pIC) ?																\
																	(((((UInt64)pIC[9]) & 0x01) << 32) | (pIC[10] << 24) | (pIC[11] << 16) | (pIC[12] <<  8) | pIC[13]) :	\
																	(UInt64)0x1FFFFFFFFLL)
	#define	g_ic__user_time_out_duration(pIC)					(g_ic__is_stream_model_muxed(pIC) ?																\
																	((pIC[14] << 16) | (pIC[15] << 8) | pIC[16]) :												\
																	((pIC[4]  << 16) | (pIC[5]  << 8) | pIC[6] ))
	#define	g_ic__number_of_pages(pIC)							(g_ic__is_stream_model_muxed(pIC) ? pIC[17] : pIC[7])

	#define	g_ic__interactive_composition_desc_size(pIC)		(g_ic__is_stream_model_muxed(pIC) ? 18 : 8)	// size of interactive_composition() minus the pages
	//}


	//-----------------------------------------------------------------------------------
	// IG - page() parsing
	// input pointer should point to the first byte of a page()
	//{
	#define kg_pg__page_desc_size_1								10		// size of page_id, page_version_number, UO_mask_table
	#define g_pg__page_id(pg)									pg[0]
	#define g_pg__page_version_number(pg)						pg[1]
	#define g_pg__UO_mask_table(pg, msk)						{for (int uoi = 0; uoi < 8; uoi++)	\
																	msk[uoi] = pg[2+uoi];			}
	//}
	// input pointer should point to the first byte beyond the in_effects and out_effects in a page(), i.e. at animation_frame_rate_code
	//{
	#define kg_pg__page_desc_size_2								7		// size of animation_frame_rate_code to number_of_BOGs
	#define g_pg__animation_frame_rate_code(pg)					pg[0]
	#define g_pg__default_selected_button_id_ref(pg)			((pg[1] << 8) | pg[2])
	#define g_pg__default_activated_button_id_ref(pg)			((pg[3] << 8) | pg[4])
	#define g_pg__palette_id_ref(pg)							pg[5]
	#define g_pg__number_of_BOGs(pg)							pg[6]
	//}

	//-----------------------------------------------------------------------------------
	// IG - page - effect_sequence()
	// input pointer should point to the first byte of an effect_sequence()
	//{
	#define kg_efsq__effectseq_desc_size_1						1		// size of number_of_windows
	#define g_efsq__number_of_windows(effseq)					effseq[0]
	//}
	// input pointer should point to the first byte after all the windows() in an effect_sequence()
	//{
	#define kg_efsq__effectseq_desc_size_2						1		// size of number_of_effects
	#define g_efsq__number_of_effects(effseq)					effseq[0]
	//}

	//-----------------------------------------------------------------------------------
	// IG - effect()
	// input pointer should point to the first byte of an effect()
	//{
	#define kg_eff__effect_desc_size							5		// size of effect_duration, palette_id_ref, number_of_composition_objects
	#define g_eff__effect_duration(eff)							((eff[0] << 16) | (eff[1] << 8) | eff[2])
	#define g_eff__palette_id_ref(eff)							eff[3]
	#define g_eff__number_of_composition_objects(eff)			eff[4]
	//}

	//-----------------------------------------------------------------------------------
	// IG - button_overlap_group()
	// input pointer should point to the first byte of a button_overlap_group()
	//{
	#define kg_bog__BOG_desc_size								3		// size of default_valid_button_id_ref and number_of_buttons
	#define g_bog__default_valid_button_id_ref(bog)				((bog[0] << 8) | bog[1])
	#define g_bog__number_of_buttons(bog)						bog[2]
	//}

	//-----------------------------------------------------------------------------------
	// IG - navigation_command()
	// input pointer should point to the first byte of a navigation_command()
	//{
	#define kg_btn__button_nav_command_size						12
	#define kg_nc__command_group_branch							0x00
	#define kg_nc__command_group_compare						0x01
	#define kg_nc__command_group_set							0x02
	#define g_btn__navigation_command(btn, nc)					{for (int nci = 0; nci < kg_btn__button_nav_command_size; nci++)	\
																	nc[nci] = btn[nci];												}
	#define g_nc__operand_count(nc)								( (nc[0] & 0xE0) >> 5)
	#define g_nc__command_group(nc)								( (nc[0] & 0x18) >> 3)
	#define g_nc__command_subgroup(nc)							(  nc[0] & 0x03)
	#define g_nc__I_flag_for_operand_1(nc)						( (nc[1] & 0x80) >> 7)
	#define g_nc__I_flag_for_operand_2(nc)						( (nc[1] & 0x40) >> 6)
	#define g_nc__branch_option(nc)								(  nc[1] & 0x0F)
	#define g_nc__compare_option(nc)							(  nc[2] & 0x0F)
	#define g_nc__set_option(nc)								(  nc[3] & 0x1F)
	#define g_nc__operand_1(nc)									( (nc[4] << 24) | (nc[5] << 16) | (nc[6] << 8) | nc[7])
	#define g_nc__operand_2(nc)									( (nc[8] << 24) | (nc[9] << 16) | (nc[10] << 8) | nc[11])
	//}

	//-----------------------------------------------------------------------------------
	// IG - button()
	// input pointer should point to the first byte of a button()
	//{
	#define kg_btn__button_desc_size							35		// size of button info before the button commands
	#define kg_btn__button_size									(kg_btn__button_desc_size + (kg_btn__number_of_navigation_commands * kg_btn__button_nav_command_size))

	#define g_btn__button_id(btn)								((btn[0] << 8) | btn[1])
	#define g_btn__button_numberic_select_value(btn)			((btn[2] << 8) | btn[3])
	#define g_btn__auto_action_flag(btn)						((btn[4] & 0x80) >> 7)
	#define g_btn__is_auto_action(btn)							(((btn[4] & 0x80) >> 7) == 1)
	#define g_btn__button_horizontal_position(btn)				((btn[5] << 8) | btn[6])
	#define g_btn__button_vertical_position(btn)				((btn[7] << 8) | btn[8])
	#define g_btn__upper_button_id_ref(btn)						((btn[9] << 8) | btn[10])
	#define g_btn__lower_button_id_ref(btn)						((btn[11] << 8) | btn[12])
	#define g_btn__left_button_id_ref(btn)						((btn[13] << 8) | btn[14])
	#define g_btn__right_button_id_ref(btn)						((btn[15] << 8) | btn[16])
	#define g_btn__normal_start_object_id_ref(btn)				((btn[17] << 8) | btn[18])
	#define g_btn__normal_end_object_id_ref(btn)				((btn[19] << 8) | btn[20])
	#define g_btn__normal_repeat_flag(btn)						((btn[21] & 0x80) >> 7)
	#define g_btn__is_normal_repeat(btn)						(((btn[21] & 0x80) >> 7) == 1)
	#define g_btn__selected_state_sound_id_ref(btn)				btn[22]
	#define g_btn__selected_start_object_id_ref(btn)			((btn[23] << 8) | btn[24])
	#define g_btn__selected_end_object_id_ref(btn)				((btn[25] << 8) | btn[26])
	#define g_btn__selected_repeat_flag(btn)					((btn[27] & 0x80) >> 7)
	#define g_btn__is_selected_repeat(btn)						(((btn[27] & 0x80) >> 7) == 1)
	#define g_btn__activated_state_sound_id_ref(btn)			btn[28]
	#define g_btn__activated_start_object_id_ref(btn)			((btn[29] << 8) | btn[30])
	#define g_btn__activated_end_object_id_ref(btn)				((btn[31] << 8) | btn[32])
	#define g_btn__number_of_navigation_commands(btn)			((btn[33] << 8) | btn[34])
	//}


	//______________________________________________________________________________________________________________________________
	// Presentation Graphics (PG):
	//

	//-----------------------------------------------------------------------------------
	// PG - composition_object() peeking
	// input pointer should point to the first byte of a presentation_composition_segment()
	//{
	#define g_pcs___composition_state(pcs)						((pcs[kg_segment_descriptor_size+kg_video_descriptor_size+2] & 0xC0) >> 6)
	//}

	//-----------------------------------------------------------------------------------
	// PG - presentation_composition_segment() parsing
	// input pointer should point to the first byte palette_update_flag of an presentation_composition()
	//{
	#define kg_pcs__pal_flag_id_num_comp_obj_size				3		// size of palette_update_flag to number_of_composition_objects fields (both inclusive)

	#define g_pcs__palette_update_flag(pcs)						((pcs[0] & 0x80) >> 7)
	#define g_pcs__is_palette_update(pcs)						((pcs[0] & 0x80) == 0x80)
	#define g_pcs__palette_id_ref(pcs)							pcs[1]
	#define g_pcs__number_of_composition_objects(pcs)			pcs[2]
	//}

	//-----------------------------------------------------------------------------------
	// PG - window_definition_segment() parsing
	// input pointer should point to the first byte number_of_windows field of an presentation_composition()
	//{
	#define kg_wds__num_wnds_size								1		// size of number_of_windows field

	#define g_wds__number_of_windows(wds)						wds[0]
	//}


	//______________________________________________________________________________________________________________________________
	// Text Subtitle (TextST):
	//

	//-----------------------------------------------------------------------------------
	// TextST - dialog_style_set() parsing (aka dialog_style_segment)
	// input pointer should point to the first byte of a dialog_style_set()
	//{
	#define kg_dss__hdr_fields_size								4		// size of fields before the region_styles() loop

	#define g_dss__player_style_flag(dss)						((dss[0] & 0x80) >> 7)
	// reserved: 15 bits
	#define g_dss__number_of_region_styles(dss)					dss[2]
	#define g_dss__number_of_user_styles(dss)					dss[3]
	//}

	//-----------------------------------------------------------------------------------
	// TextST - number of dialog presentation segments. found between DSS and first DPS
	// input pointer should point to the first byte of number_of_dialog_presentation_segments field
	//{
	#define g_dss__number_of_dialog_presentation_segments(dss)	((dss[0] << 8) | dss[1])
	//}

	//-----------------------------------------------------------------------------------
	// TextST - region_style() (in dialog_style_set()) parsing
	// input pointer should point to the first byte of a region_style()
	//{
	#define kg_rgs__region_style_size							29		// size of region_style()

	#define g_rgs__region_style_id(rgs)							rgs[0]
	#define g_rgs__region_horizontal_pos(rgs)					((rgs[1] << 8) | rgs[2])
	#define g_rgs__region_vertical_pos(rgs)						((rgs[3] << 8) | rgs[4])
	#define g_rgs__region_width(rgs)							((rgs[5] << 8) | rgs[6])
	#define g_rgs__region_height(rgs)							((rgs[7] << 8) | rgs[8])
	#define g_rgs__region_bg_palette_entry_id_ref(rgs)			rgs[9]
	// reserved: 8 bits
	#define g_rgs__text_box_horizontal_pos(rgs)					((rgs[11] << 8) | rgs[12])
	#define g_rgs__text_box_vertical_pos(rgs)					((rgs[13] << 8) | rgs[14])
	#define g_rgs__text_box_width(rgs)							((rgs[15] << 8) | rgs[16])
	#define g_rgs__text_box_height(rgs)							((rgs[17] << 8) | rgs[18])
	#define g_rgs__text_flow(rgs)								rgs[19]
	#define g_rgs__text_horizontal_alignment(rgs)				rgs[20]
	#define g_rgs__text_vertical_alignment(rgs)					rgs[21]
	#define g_rgs__line_space(rgs)								rgs[22]
	#define g_rgs__font_id_ref(rgs)								rgs[23]
	#define g_rgs__font_style(rgs)								rgs[24]
	#define g_rgs__font_size(rgs)								rgs[25]
	#define g_rgs__font_palette_entry_id_ref(rgs)				rgs[26]
	#define g_rgs__font_outline_palette_entry_id_ref(rgs)		rgs[27]
	#define g_rgs__font_outline_thickness(rgs)					rgs[28]
	//}

	//-----------------------------------------------------------------------------------
	// TextST - user_control_style() (in dialog_style_set()) parsing
	// input pointer should point to the first byte of a user_control_style()
	//{
	#define kg_ucs__user_control_style_size						15		// size of user_control_style()

	#define g_ucs__user_style_id(ucs)							ucs[0]
	#define g_ucs__region_horizontal_position_direction(ucs)	((ucs[1] & 0x80) >> 7)
	#define g_ucs__region_horizontal_position_delta(ucs)		(((ucs[1] & 0x7F) << 8) | ucs[2])
	#define g_ucs__region_vertical_position_direction(ucs)		((ucs[3] & 0x80) >> 7)
	#define g_ucs__region_vertical_position_delta(ucs)			(((ucs[3] & 0x7F) << 8) | ucs[4])
	#define g_ucs__font_size_inc_dec(ucs)						((ucs[5] & 0x80) >> 7)
	#define g_ucs__font_size_delta(ucs)							(ucs[5] & 0x7F)

	#define g_ucs__text_box_horizontal_position_direction(ucs)	((ucs[6] & 0x80) >> 7)
	#define g_ucs__text_box_horizontal_position_delta(ucs)		(((ucs[6] & 0x7F) << 8) | ucs[7])
	#define g_ucs__text_box_vertical_position_direction(ucs)	((ucs[8] & 0x80) >> 7)
	#define g_ucs__text_box_vertical_position_delta(ucs)		(((ucs[8] & 0x7F) << 8) | ucs[9])
	#define g_ucs__text_box_width_inc_dec(ucs)					((ucs[10] & 0x80) >> 7)
	#define g_ucs__text_box_width_delta(ucs)					(((ucs[10] & 0x7F) << 8) | ucs[11])
	#define g_ucs__text_box_height_inc_dec(ucs)					((ucs[12] & 0x80) >> 7)
	#define g_ucs__text_box_height_delta(ucs)					(((ucs[12] & 0x7F) << 8) | ucs[13])
	#define g_ucs__line_space_inc_dec(ucs)						((ucs[14] & 0x80) >> 7)
	#define g_ucs__line_space_delta(ucs)						(ucs[14] & 0x7F)
	//}

	//-----------------------------------------------------------------------------------
	// TextST - dialog_presentation_segment() parsing
	// input pointer should point to the first byte of a dialog_presentation_segment() after the segment_descriptor()
	//{

	// 33-bit 90 KHz and 32-bit 45 KHz PTS extaction.
	#define kg_dps__PTSs__palette_update_flag_size				11		// size of fields PTS fields and palette_update_flag before palette()
	#define	g_dps__dialog_start_PTS(dps)						( (((UInt64)dps[0]) & 0x01) << 32) |											\
																  ((UInt64)((dps[1] << 24) | (dps[2] << 16) | (dps[3] <<  8) | dps[4]))
	#define	g_dps__dialog_end_PTS(dps)							( ((((UInt64)dps[5]) & 0x01) << 32) |											\
																  ((UInt64)((dps[6] << 24) | (dps[7] << 16) | (dps[8] <<  8) | dps[9])) )
	#define	g_dps__dialog_start_PTS_32bit(dps)					( (((dps[0]) & 0x01) << 31) |													\
																  (((dps[1] << 24) | (dps[2] << 16) | (dps[3] <<  8) | dps[4]) >> 1) )
	#define	g_dps__dialog_end_PTS_32bit(dps)					( (((dps[5]) & 0x01) << 31) |													\
																  (((dps[6] << 24) | (dps[7] << 16) | (dps[8] <<  8) | dps[9]) >> 1) )
	#define g_dps__palette_update_flag(dps)						((dps[10] & 0x80) >> 7)

	// region info extraction
	#define kg_dps__number_of_regions_size						1		// size of number_of_regions field (that is between palette() and dialog_region() loop)
	#define g_dps__number_of_regions(dps)						dps[0]
	//}

	//-----------------------------------------------------------------------------------
	// TextST - dialog_region() (in dialog_presentation_segment()) parsing
	// input pointer should point to the first byte of a dialog_region()
	//{
	#define kg_drg__dialog_region_hdr_size						2										// size of fields before region_subtitle()
	#define g_drg__dialog_region_size(drg)						(2 + (2 + ((drg[2] << 8) | drg[3]))) 	// size of dialog_region()

	#define g_drg__continuous_presentation_flag(drg)			((drg[0] & 0x80) >> 7)
	#define g_drg__forced_on_flag(drg)							((drg[0] & 0x40) >> 6)
	#define g_drg__region_style_id_ref(drg)						drg[1]
	//}

	//-----------------------------------------------------------------------------------
	// TextST - region_subtitle() (in dialog_region()) parsing
	// input pointer should point to the first byte of a region_subtitle()
	//{
	#define g_rst__region_subtitle_length(rst)					((rst[0] << 8) | rst[1])
	#define g_rst__region_subtitle_size(rst)					(2 + g_rst__region_subtitle_length(rst)) 	// size of region_subtitle()
	#define g_rst__region_subtitle_byte(rst, i)					(rst[i+2]) 									// byte from region_subtitle()'s subtitle data
	//}

	//-----------------------------------------------------------------------------------
	// TextST - palette() (in DPS and DSS) parsing
	// input pointer should point to the first byte of a palette() in a DPS/DSS
	//{
	#define g_txt__palette_length(pal)							((pal[0] << 8) | pal[1])
	//}
};

#endif // __HDMVDECODER_GRAPHICSPARSERUTILS_H__
