#ifndef CHARSET_DEF_H
#define CHARSET_DEF_H

#define SBCS						0x00
#define UTF16_LE       				0x01
#define UTF16_BE					0x02
#ifndef UTF8
#define UTF8						0x03
#endif

#define SBCS_LATIN					0x10
#define SBCS_LATIN_GREEK			0x11
#define SBCS_LATIN_TURKISH			0x12
#define SBCS_LATIN_CENTRAL_EU		0x13
#define SBCS_LATIN_ARABIC			0x14
#define SBCS_LATIN_CYRILLIC			0x15
#define SBCS_LATIN_HEBREW			0x16
#define SBCS_LATIN_SOUTH_EASTERN_EU 0x17
#define SBCS_THAI					0x18

#define DBCS_BIG5   				0x20
#define DBCS_GBK					0x21
#define DBCS_JAPANESE				0x22
#define DBCS_KOREAN					0x23

#define UNKNOWN_CHARSET             0xff

#endif
