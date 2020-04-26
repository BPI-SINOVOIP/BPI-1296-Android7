#ifndef DEBUG_UTILITY_H
#define DEBUG_UTILITY_H

#define SHOW_FILE_LINE(x)	 { printf("[%s %d] ", __FILE__, __LINE__);	printf x ; }
#define INDEX_OP_DEBUG(x)	SHOW_FILE_LINE(x)
#define ARRAY_OP_DEBUG(x)	SHOW_FILE_LINE(x)
#define PARSE_OP_DEBUG(x)	SHOW_FILE_LINE(x)

#endif

