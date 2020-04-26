// $Header: /Mux_Demux_SDK60/common/ArgumentReader.h 1     2/14/05 11:51a Sbramwell $
//
// Copyright (c) 2001 DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.

#ifndef ARGUMENTREADER_H
#define ARGUMENTREADER_H

/**
 * These macros are used to parse command line arguments. 
 * @version $Revision: 1 $
 */
#define ARGSTRING(ps,switchstring) \
{ \
  int i; \
  for (i=0;i<argc-1;i++)\
  {\
    if (strcmp(argv[i],switchstring)==0)\
    {\
      if (argv[i+1][0]!='-') \
        ps=(argv[i+1]);\
      break;\
    }\
  }\
}
#define ARGINT(n,switchstring) \
{ \
  int i; \
  int j = 0; \
  for (i=0;i<argc-1;i++)\
  {\
    if (strcmp(argv[i],switchstring)==0)\
    {\
      if (argv[i+1][0]!='-') \
        j=atoi(argv[i+1]);\
      break;\
    }\
  }\
  if (j) \
    n = j; \
}

#define END(n)\
{\
  bool pause;\
  ARGSWITCH(pause,"-p");\
  if (pause)\
  { cerr << "Done. Press Enter Key."<<endl;\
    getchar();\
  }\
  return(n);\
}

#endif //ARGUMENTREADER_H



