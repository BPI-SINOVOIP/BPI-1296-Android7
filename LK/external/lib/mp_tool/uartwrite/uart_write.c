#include <lib/console.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib/sysparam.h>
#include <factorylib.h>
#include <uart_write.h>
#include <malloc.h>
#include <err.h>

#define debug_tag
#define TMP_FACTORY_SN_FILE_APTH "tmp/factory/sn.txt"
#define simple_strtoul strtoul

static 
int do_uart_w( int argc, const cmd_args *argv)
{
  int length =0 , option=0 ,bRes=1 ;
  const char *mac_value = NULL;
  const char *cmd = argv[0].str;
  char *buffer=NULL;
  
	if (strcmp(cmd, "uart_write"))
	{
 	   	printf("Unknown command '%s' - try 'help'\n",cmd);
           return bRes;
	}
	
	printf("argc :%d \n",argc);
	
	if ((argc > 4)||(argc == 1)){
	    printf("argc :%d ,error!\n",argc);
	    cmd_usage();
	  return bRes;
       	}else if (argc == 2){
		mac_value = argv[1].str;
	 option=0;
	}else if (argc == 3){
		buffer = (char *)simple_strtoul(argv[1].str, NULL, 16);
	 	length = (int)simple_strtoul(argv[2].str, NULL, 16);
        	        	printf("length :%d\n",length);
	  #ifdef debug_tag
		printf("buffer :\r\n");				
		hexdump(buffer,length);
	  #endif
	  option=1;
	}else if (argc == 4){
		mac_value = argv[1].str;
		buffer = (char *)simple_strtoul(argv[2].str, NULL, 16);
              	length = (int)simple_strtoul(argv[3].str, NULL, 16);
       		 printf("length :%d\r\n",length);
	        	 printf("buffer :\r\n");
	  #ifdef debug_tag
		printf("buffer :\r\n");				
		hexdump(buffer,length);
	  #endif
	  option=2;
	}

  	      switch(option){
		case 0:
			bRes=do_uart_w_mac(mac_value);
			if(bRes!=0){
				printf("only write mac fail!\n");
			}
		   break;
		case 1:
		  bRes=do_uart_w_sn(buffer,length);
	                  if(bRes!=0){
				printf("only write sn fail!\n");
	                  }
		  break;
		case 2:
                    bRes=do_uart_w_mac(mac_value);
		 	if(bRes!=0){
                        		printf("write mac fail!\n");
                  	}else{
				bRes=do_uart_w_sn(buffer,length);
				if(bRes!=0){
                        			printf("write sn fail!\n");
                  		}
		  	}
		 break;
		default:
			printf("option fail !\n");
		 break;
	     }
	
			if(bRes != 0){
			 	printf("***** FAILFAIL *****\n");
        	         		printf("***** FAILFAIL *****\n");
	                		printf("***** FAILFAIL *****\n");
			}else{
			 	printf("***** OKOK *****\n");
		 		printf("***** OKOK *****\n");
			 	printf("***** OKOK *****\n");
			}
 return bRes;
}

int 
do_uart_w_mac(const char *mac_value){

  int  bRe=1,num=0,len=strlen(mac_value);
  status_t	err;
  uint8_t *check_buf =NULL;
  
	printf("ethaddr:%s\n",mac_value);
	printf("ethaddr len:%d\n",len);

	if(len!=17){
		err=ERR_BAD_LEN;
		goto done;
	}
	
	sysparam_add("ethaddr",mac_value,strlen(mac_value));
	
		check_buf = malloc(len);
	      	if (!check_buf) {
	            err = ERR_NO_MEMORY;
		   goto done;
	        	}else{
	        	   memset(check_buf,0,len);
	        	}
		        len = sysparam_read("ethaddr", check_buf, len);
		        if (len < 0) {
		            err = len;
			   printf("read ethaddr fail !\n");
		            goto done;
		        }
		        err = NO_ERROR;
	 	
		 #ifdef debug_tag
		 	printf("debug mac value:");
		        for (int i = 0; i < len; i++)
		            printf("%c", check_buf[i]);
		        printf("\n");
		#endif

			  if(err==NO_ERROR){
	                		if((strncmp(mac_value,(const char*)check_buf,len))!= 0){
	                	  	 	printf("compare mac fail !\n");
	                		}else{
					bRe=0;
				}
			 }

			for(num=0;num<2;num++){	
				printf("save num:%d\r\n",num);
				err=sysparam_write_factory();
				if(err!=NO_ERROR){
				 	printf("mac envsave fail!\n");
					bRe=1;
				 	break;
				}
			}
done:

   if (err >= NO_ERROR) {
        	printf("OK\r\n");
    } else if (err == ERR_NO_MEMORY) {
        printf("ERROR out of memory\r\n");
    } else if (err == ERR_ALREADY_EXISTS) {
        printf("ERROR already exists\r\n");
    } else if (err == ERR_INVALID_ARGS) {
        printf("ERROR invalid argument\r\n");
    } else if (err == ERR_NOT_FOUND) {
        printf("ERROR not found\r\n");
    } else if (err == ERR_NOT_ALLOWED) {
        printf("ERROR not allowed (locked)\r\n");
    } else if (err == ERR_BAD_LEN){
        printf("ERROR mac valure length errorr\r\n");
    } else{
        printf("ERROR generic error %d\r\n", err);
    }

	if(check_buf)
		free(check_buf);
	check_buf=NULL;	
  return bRe;
}

int 
do_uart_w_sn(char* buf ,int len){

	int bRe=1 , num =0;

		 for(num=0;num<2;num++){
                    bRe = factory_write(TMP_FACTORY_SN_FILE_APTH,buf, len);
                     if (bRe != 0){
                        break;
                     }
                    bRe = factory_save();
                     if (bRe != 0){
                        break;
                     }
                  }
	return bRe;
}

void 
cmd_usage(void){
	printf("for uart mp tool burn mac and sn key\n");
	printf("only write mac : uart_writ [mac]\n");
	printf("only write sn  : uart_writ [load addr] [file lenght(hex)]\n");
	printf("both: uart_writ [mac] [load addr] [file lenght(hex)]\n");
}

STATIC_COMMAND_START
STATIC_COMMAND("uart_write", "for uart mp tool write mac or sn", (console_cmd)&do_uart_w)
STATIC_COMMAND_END(uart_write);

