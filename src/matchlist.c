#include "mem_ops.h"
#include "utils.h"
#include "BSD/strsec.h"
#include "match_algorithms.h"

/* read lines of file matchlist.txt and test if match strings

If match context with request return array of void pointer with:
if true returns matched string

*/


char *matchlist(char *input,int input_len, short option_algorithm)
{
	FILE * arq;
	bool at_list=false;
	int line_len=0;

	if(option_algorithm==4)	
		arq = fopen("config/regex_rules.txt", "r"); // if user choice regex need to use regex list
	else
		arq = fopen("config/match_list_request.txt", "r");

	if( arq == NULL )
	{

		DEBUG("error to open() file"); 	 
		exit(0);
		return NULL;
	}

	char line[1024];

	

	while(at_list==false)
	{
		burn_mem(line,0,1023);
		if (fgets(line,sizeof(line),arq) == NULL)
			break;

		line_len=strnlen(line,1023);		
		line[line_len+1]='\0';	

// remove \n\0 etc... sub -2 at line_len
		if(line_len>4)		
			switch(option_algorithm)
			{
				case 1:
				at_list=DFA_Search(line,line_len-2, input, input_len);
				break;

				case 2:
				at_list=horspool_search(input,input_len, line, line_len-2);
				break;
			
				case 3:
				at_list=Rabin_Karp_search(input, input_len, line, line_len-2);
				break;
 	
				case 4:
				at_list=pcre_regex_search(input, input_len, line);
				break; 
			}		
	}

	if( fclose(arq) == EOF )
	{
		DEBUG("Error in close() file config/matchlist_ip.txt ");
		exit(0);
		return NULL;
	}
		
	arq=NULL;

	if(at_list==true) 
	{
		char *tmp=strdup(line);
		return tmp;
	} else
		return NULL;
}


