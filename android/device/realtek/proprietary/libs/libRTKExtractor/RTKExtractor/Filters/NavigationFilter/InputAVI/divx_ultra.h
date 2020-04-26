#ifndef __DIVX_ULTRA_HEADER_FILE__
#define __DIVX_ULTRA_HEADER_FILE__

#include <sys/types.h>

typedef struct __attribute__ ((__packed__)) DivxMediaManagerType {
	char 		FourCC[4];
	uint32_t	ChunkSize;
	uint64_t	Offset;
	char		VersionId[3];
	int32_t		MenuVersion;
	int32_t		ObjectID;
	int32_t		StartingMenuID;
	int32_t		DefaultLanguageLen;
	int16_t		*DefaultLanguage; // UTF16
} DivxMediaManager;

typedef struct DivxMediaMenuType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		BackgroundVideoID;
	int32_t		BackgroundAudioID;
	int32_t		StartingMenuItemID;
	int32_t		EnterAction;
	int32_t		ExitAction;
	int32_t		MenuTypeLen;
	int16_t		*MenuType;
	int32_t		MenuNameLen;
	int16_t		*MenuName;
} DivxMediaMenu;

typedef struct ButtonMenuType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		OverlayID;
	int32_t		UpAction;
	int32_t		DownAction;
	int32_t		LeftAction;
	int32_t		RightAction;
	int32_t		SelectAction;
	int32_t		ButtonNameLen;
	int16_t		*ButtonName;
} ButtonMenu;

typedef struct MenuRectangleType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		Left;
	int32_t		Right;
	int32_t		Top;
	int32_t		Height;
} MenuRectangle;

typedef struct ActionType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;		
} Action;

typedef struct SubtitleSelectActionType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		TitleID;
	char		TrackID[4];
} SubtitleSelectAction;

typedef struct AudioSelectActionType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		TitleID;
	char		TrackID[4];
} AudioSelectAction;

typedef struct PlayActionType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		TitleID;
	int32_t		MediaObjectID;
} PlayAction;

typedef struct PlayFromCurrentOffsetType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		TitleID;
	int32_t		MediaObjectID;
} PlayFromCurrentOffset;

typedef struct MenuTransitionActionType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		MenuID;
} MenuTransitionAction;

typedef struct ButtonTransitionActionType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		ButtonMenuID;
} ButtonTransitionAction;

typedef struct LanguageMenusType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		LanguageCodeLen;
	int16_t		*LanguageCode;
	int32_t		StartingMenuID;
	int32_t		RootMenuID;
} LanguageMenus;

typedef struct MediaType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
} Media;

typedef struct MenuTracksType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
} MenuTracks;

typedef struct MediaSourceType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		Reserved;
	uint64_t	RiffOffset;
} MediaSource;

typedef struct MediaTrackType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	uint32_t	MediaSourceID;
	char		TrackID[4];
	int32_t		StartTime;
	int32_t		EndTime;
	int32_t		LanguageCodeLen;
	int16_t		*LanguageCode;
	int32_t		TranslationLookupID;
	int32_t		TypeLen;
	int16_t		*Type;
} MediaTrack;

typedef struct TitleType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		TranslationLookupID;
	int32_t		DefaultAudioLen;
	int16_t		*DefaultAudioTrack;
	int32_t		DefaultSubtitleLen;
	int16_t		*DefaultSubtitleTrack;
} Title;

typedef struct ChapterType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		TranslationLookupID;
} Chapter;

typedef struct TranslationTableType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
} TranslationTable;

typedef struct TranslationLookupType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
} TranslationLookup;

typedef struct TranslationType {
	char		FourCC[4];
	uint32_t	ChunkSize;
	int32_t		ObjectID;
	int32_t		LanguageCodeLen;
	int16_t		*LanguageCode;
	int32_t		ValueLen;
	int16_t		*Value;
} Translation;

#endif
