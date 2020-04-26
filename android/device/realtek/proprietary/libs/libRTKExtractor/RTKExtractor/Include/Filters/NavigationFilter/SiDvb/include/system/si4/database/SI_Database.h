/*=======================================================================
 * Copyright c				Realtek Semiconductor Corporation, 2007     *
 * All rights reserved.                                                 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 *
 * Module		: SI4
 *
 * Author		: ss@realtek.com.tw
 * Date			: 2007/04/23
 *
 *=======================================================================*/


/*


ChangeLog:
	2008/02/12
		make si_database_get_logical_num_and_onid() as public. (kjw)


*/

#ifndef _SI_DATABASE_H_
#define _SI_DATABASE_H_

ErrCode SI_Database_Lock
(
	SI_DATABASE							*pDatabase
);

ErrCode SI_Database_Unlock
(
	SI_DATABASE							*pDatabase
);



#endif

