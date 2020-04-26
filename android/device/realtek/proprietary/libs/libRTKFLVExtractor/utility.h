#ifndef FLV_PARSER_UTILITY_FUNCTION
#define FLV_PARSER_UTILITY_FUNCTION
#include <stdlib.h>
#include <string.h>

static unsigned int getbits_sample_rates[12] =
{
96000, 88200, 64000, 48000, 44100, 32000,
24000, 22050, 16000, 12000, 11025, 8000
};

static unsigned char getbits_sr_index(const int samplerate)
{
    if (92017 <= samplerate) return 0;
    if (75132 <= samplerate) return 1;
    if (55426 <= samplerate) return 2;
    if (46009 <= samplerate) return 3;
    if (37566 <= samplerate) return 4;
    if (27713 <= samplerate) return 5;
    if (23004 <= samplerate) return 6;
    if (18783 <= samplerate) return 7;
    if (13856 <= samplerate) return 8;
    if (11502 <= samplerate) return 9;
    if (9391 <= samplerate) return 10;
 
    return 11;
};

static int bit_cnt;

static void getbits_init(void) {
	bit_cnt = 0;
}

static int getbits_get_processed_bits(void) {
	return bit_cnt;
}

static int getbits_get(int num, unsigned char *buffer) {
	unsigned char byte_value;
	int receivedBits = 0;
	int byte_num;
	int bit_start;
	int bit_length;
	int retval = 0;

	// fetch data bit
	while(num> 0) {
		byte_num = bit_cnt / 8;
		bit_start = bit_cnt % 8;
		bit_length = 8 - bit_start;

		if(bit_length > num)
			bit_length = num;

		//byte_value = (buffer[byte_num] & ((0xff) >> ( 7 - (bit_start + bit_length - 1)))) >> bit_start;
		byte_value = ( buffer[byte_num] & ((0xff) >> bit_start) );
		byte_value >>= (8 - bit_start - bit_length);

		//retval |= (byte_value << receivedBits);//1
		retval = retval << bit_length;//2
    retval |= (byte_value);//2
    

		// post
		num -= (bit_length);
		bit_cnt += bit_length;
		receivedBits += bit_length;
	}
	return retval;
}

static int detect_sbr_present_flag(unsigned char *buffer,
				unsigned short bufferLength,
				unsigned char *channelsConfiguration,
				unsigned int *samplingFrequencyIndex,
				int *privateData1,
				int *privateData2) {
	int objectTypeIndex;
	unsigned int samplingFrequency;

	char frameLengthFlag;
	char dependsOnCoreCoder;
	unsigned short int coreCoderDelay;
	char extensionFlag;

	int sbr_present_flag;
	
	getbits_init();

	objectTypeIndex = getbits_get(5, buffer);
	*samplingFrequencyIndex = getbits_get(4, buffer);
	*channelsConfiguration = getbits_get(4, buffer);

	samplingFrequency = getbits_sample_rates[*samplingFrequencyIndex];

	if(objectTypeIndex != 1 //main
					&& objectTypeIndex != 2//LC  
					&& objectTypeIndex != 5) { //HE
        return -1;
  }

  if (samplingFrequency == 0)
    return -1;  

	if(*channelsConfiguration > 7)
		return -1;

	if(*channelsConfiguration == 1)
		*channelsConfiguration = 2;

	sbr_present_flag = 0;

	if(objectTypeIndex == 5) {
		unsigned char tmp;
		sbr_present_flag = 1;
		*samplingFrequencyIndex = getbits_get(4, buffer);
		if(*samplingFrequencyIndex == 15){
			samplingFrequency = getbits_get(24, buffer);
			*samplingFrequencyIndex = getbits_sr_index(samplingFrequency);//sp_rate->index
		}

		objectTypeIndex = getbits_get(5, buffer);
	}

	if(objectTypeIndex == 1 || objectTypeIndex == 2) {
		frameLengthFlag = getbits_get(1, buffer);
		dependsOnCoreCoder = getbits_get(1, buffer);
		if(dependsOnCoreCoder == 1)
			coreCoderDelay = getbits_get(14, buffer);

		extensionFlag = getbits_get(1, buffer);
		
    if (*channelsConfiguration == 0)
      return -1;//implement "program_config_element()" ?
            
	}

	if(objectTypeIndex != 5 && (bufferLength * 8 - getbits_get_processed_bits()) >= 16) {
		short int syncExtensionType;

		syncExtensionType = getbits_get(11, buffer);
		if(syncExtensionType == 0x2b7) {
			objectTypeIndex = getbits_get(5, buffer);

			if(objectTypeIndex == 5) {
				sbr_present_flag = getbits_get(1, buffer);

				if(sbr_present_flag) {
					*samplingFrequencyIndex = getbits_get(4, buffer);

					if(*samplingFrequencyIndex == 15){
						samplingFrequency = getbits_get(24, buffer);
			      *samplingFrequencyIndex = getbits_sr_index(samplingFrequency);//sp_rate->index						
			    }
				}
			}
		}
	}
	
	*privateData1 = objectTypeIndex;
	*privateData2 = sbr_present_flag;
	return 0;
}

#endif
