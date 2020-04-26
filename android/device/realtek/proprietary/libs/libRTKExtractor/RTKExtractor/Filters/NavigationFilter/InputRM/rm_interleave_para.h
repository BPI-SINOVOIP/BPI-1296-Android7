#ifndef __RM_GENR_INTERLEAVE_PARAMETERS_H__
#define __RM_GENR_INTERLEAVE_PARAMETERS_H__

#define RM_COOK_INTERLEAVE_PARA_ENTRY_SIZE 30
#define RM_SIPR_INTERLEAVE_PARA_ENTRY_SIZE 4
#define RM_ATRC_INTERLEAVE_PARA_ENTRY_SIZE 8

typedef struct {

	int frameSize;
	int blockSize;	//interleave block size
	int factor;		//interleave factor
	int framesPerBlock;
}interleave_para_t;

static interleave_para_t gGENRInterleaveParametersForCOOK[RM_COOK_INTERLEAVE_PARA_ENTRY_SIZE] = 
{
	{32, 288, 8, 9},
	{32, 352, 8, 11},
	{47, 564, 8, 12}, 
	{60, 600, 9, 10},
	{93, 651, 14, 7},
	{128, 640, 15, 5},
	{186, 744, 20, 4},
	{93, 558, 16, 6},
	{24, 288, 6, 12},
	{58, 580, 10, 10},
	{94, 564, 14, 6},
	{128, 640, 16, 5},
	{186, 744, 20, 4},
	{278, 834, 30, 3},
	{186, 744, 20, 4},
	{60, 600, 9, 10},
	{93, 651, 14, 7}, 
	{48, 528, 8, 11},
	{60, 600, 10, 10},
	{60, 600, 10, 10},
	{93, 465, 16, 5}, 
	{93, 465, 16, 5},
	{128, 640, 16, 5},        
	{128, 640, 16, 5},    
	{186, 930, 16, 5},
	{280, 1400, 16, 5},
	{36, 396, 8, 11},
	{186, 930, 16, 5},
	{280, 1400, 16, 5},
	{128, 640, 16, 5}
};

static interleave_para_t gGENRInterleaveParametersForSIPR[RM_SIPR_INTERLEAVE_PARA_ENTRY_SIZE] = 
{
	 {14, 232, 6, 16},
	 {19, 304, 6, 16},
	 {18, 296, 6, 16},
	 {20, 320, 6, 16},
};

static interleave_para_t gGENRInterleaveParametersForATRC[RM_ATRC_INTERLEAVE_PARA_ENTRY_SIZE] = 
{
	 {192, 768, 20, 4},
	 {272, 1088, 20, 4},
	 {304, 912, 30, 3},
	 {384, 1152, 30, 3},
	 {424, 1272, 30, 3},
	 {512, 1024, 30, 2},
	 {768, 768, 10, 1},
	 {1024, 1024, 10, 1} 
};
#endif

