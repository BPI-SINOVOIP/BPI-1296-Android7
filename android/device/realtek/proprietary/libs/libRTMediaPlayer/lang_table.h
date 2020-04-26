#ifndef _RTMEDIAPLAYER_LANG_TABLE_H_
#define _RTMEDIAPLAYER_LANG_TABLE_H_
typedef struct {
    int code;
    char str[4];
} ISO_639_LANG_TABLE;
static ISO_639_LANG_TABLE lang_code_table[] = 
{
    {  -1, "und"}, // undefined
    {'en', "eng"},
    {'zh', "chi"},
    {'ja', "jpn"},
    {'es', "spa"},
    {'fr', "fre"},
    {'de', "deu"},
    {'it', "ita"},
    {'in', "ind"},
    {'ko', "kor"},
    {'th', "tha"},
    {'pt', "por"},
    {'nl', "dut"},
    {'cs', "cze"},
    {'hu', "hun"},
    {'ru', "rus"},
    {'pl', "pol"},
    {'fi', "fin"},
    {'no', "nor"},
    {'sv', "swe"},
    {'da', "dan"},
    {'el', "gre"},
    {'ga', "gle"},
    {'hi', "hin"},
    {'sl', "slv"},
    {'iw', "heb"},
    {'he', "heb"},
    {'tr', "tuk"},
    {'ro', "ron"},
    {'ar', "ara"},
    {'bg', "bul"},
    {'la', "lat"},
    {'lt', "lit"},
    {'lv', "lav"},
    {'ms', "may"},
    {'my', "bur"},
    {'ne', "nep"},
    {'sk', "slk"},
    {'sq', "sqi"},
    {'sr', "srp"},
    {'uk', "ukr"},
    {'vi', "vie"},
    {'hr', "hrv"},
    {'hy', "hye"},
    {'is', "isl"},
    {'km', "khm"},
    {'te', "tel"},
    {'ta', "tam"},
    {'et', "est"}
};

const static int MAX_LANG_CODE_TABLE_SIZE = sizeof (lang_code_table)/sizeof (ISO_639_LANG_TABLE);

const ISO_639_LANG_TABLE lang_3bytes_code_table[] = 
{ 
	  {  -1, "und"}, // undefined
	  {'eng', "eng"}, 
	  {'chi', "chi"}, 
	  {'zho', "zho"},	
	  {'jpn', "jpn"},
	  {'spa', "spa"},
	  {'fre', "fre"},
	  {'fra', "fra"},  
	  {'ger', "ger"}, 
	  {'deu', "deu"},  
	  {'ita', "ita"}, 
	  {'ind', "ind"},
	  {'kor', "kor"}, 
	  {'tha', "tha"},	   
	  {'por', "por"}, 
	  {'dut', "dut"},
	  {'nld', "nld"},  
	  {'cze', "cze"},
	  {'ces', "ces"},  
	  {'hun', "hun"},  
	  {'rus', "rus"},
	  {'pol', "pol"},
	  {'fin', "fin"},
	  {'nor', "nor"},
	  {'swe', "swe"},
	  {'dan', "dan"},
	  {'grc', "grc"},
	  {'gre', "gre"},
	  {'ell', "ell"},  
	  {'gle', "gle"},
	  {'hin', "hin"},
	  {'slv', "slv"},
	  {'heb', "heb"}, 
	  {'tur', "tur"},
	  {'rum', "rum"},
	  {'ron', "ron"},  
	  {'ara', "ara"},
	  {'bul', "bul"},
	  {'lat', "lat"},
	  {'lit', "lit"},
	  {'lav', "lav"},
	  {'may', "may"},
	  {'msa', "msa"},  
	  {'bur', "bur"},
	  {'mya', "mya"},  
	  {'nep', "nep"},
	  {'slo', "slo"}, 
	  {'slk', "slk"},	
	  {'alb', "alb"}, 
	  {'sqi', "sqi"},	
	  {'srp', "srp"}, 
	  {'ukr', "ukr"},
	  {'vie', "vie"},
	  {'hrv', "hrv"},
	  {'arm', "arm"},  
	  {'hye', "hye"},  
	  {'ice', "ice"},  
	  {'isl', "isl"},	 
	  {'khm', "hkm"},
	  {'cat', "cat"},
	  {'tel', "tel"},
	  {'tam', "tam"},
	  {'est', "est"},
	  {'ENG', "eng"},
	  {'CHI', "chi"},
	  {'ZHO', "zho"},
	  {'JPN', "jpn"},
	  {'SPA', "spa"},
	  {'FRE', "fre"},
	  {'FRA', "fra"},
	  {'GER', "ger"},
	  {'DEU', "deu"},
	  {'ITA', "ita"},
	  {'IND', "ind"},
	  {'KOR', "kor"},
	  {'THA', "tha"},
	  {'POR', "por"}, 
	  {'DUT', "dut"},
	  {'NLD', "nld"},
	  {'CZE', "cze"},
	  {'CES', "ces"},
	  {'HUN', "hun"}, 
	  {'RUS', "rus"},
	  {'POL', "pol"},
	  {'FIN', "fin"},
	  {'NOR', "nor"},
	  {'SWE', "swe"},
	  {'DAN', "dan"},
	  {'GRC', "grc"},
	  {'GRE', "gre"},
	  {'ELL', "ell"},
	  {'GLE', "gle"},
	  {'HIN', "hin"},
	  {'SLV', "slv"},
	  {'HEB', "heb"},
	  {'TUR', "tur"},
	  {'RUM', "rum"},
	  {'RON', "ron"},
	  {'ARA', "ara"},
	  {'BUL', "bul"},
	  {'LAT', "lat"},
	  {'LIT', "lit"},
	  {'LAV', "lav"},
	  {'MAY', "may"},
	  {'MSA', "msa"},
	  {'BUR', "bur"},
	  {'MYA', "mya"},
	  {'NEP', "nep"},
	  {'SLO', "slo"},
	  {'SLK', "slk"},
	  {'ALB', "alb"},
	  {'SQI', "sqi"}, 
	  {'SRP', "srp"},
	  {'UKR', "ukr"},
	  {'VIE', "vie"},
	  {'HRV', "hrv"},
	  {'ARM', "arm"},
	  {'HYE', "hye"},
	  {'ICE', "ice"}, 
	  {'ISL', "isl"},	
	  {'KHM', "hkm"},
	  {'CAT', "cat"},
	  {'TEL', "tel"},
	  {'TAM', "tam"},
	  {'EST', "est"}


};

const static int MAX_LANG_3BYTE_CODE_TABLE_SIZE = sizeof (lang_3bytes_code_table)/sizeof (ISO_639_LANG_TABLE);

const char* langCode_to_str(int code)
{
    if (0 == (code & 0xff0000)) { // 2-byte code
        for (int i = 0; i < MAX_LANG_CODE_TABLE_SIZE; i++) {
            if (lang_code_table[i].code == code)
                return lang_code_table[i].str;
        }
    } else if (code != -1) { // 3-byte code
        for (int i = 0; i < MAX_LANG_3BYTE_CODE_TABLE_SIZE; i++) {
            if (lang_3bytes_code_table[i].code == code)
                return lang_3bytes_code_table[i].str;
        }
		/*
	        static char str[4] = {0};
	        str[0] = (code>>16)&0xff;
	        str[1] = (code>>8)&0xff;
	        str[2] = (code)&0xff;
	        return str;
        	*/
    }
    return "und";
}
#endif // _RTMEDIAPLAYER_LANG_TABLE_H_
