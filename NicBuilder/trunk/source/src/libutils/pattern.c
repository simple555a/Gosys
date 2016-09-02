#include "precomp.h"
#pragma hdrstop 
/*
	egfmutil.c source file used in E-Guard project.
	by spAce, sunpan@sunv.com
	Created : Oct.5, 2001
	Lastmodified :Oct.25, 2001
	Version :1.0
	-----------------------------------------
	���ļ����� 
	Ӧ�ú���
	98/NT����
*/
#include <string.h>
#include <stdio.h>

#define IN
#define OUT
#define TRUE  1
#define FALSE 0

/*********************************************************************
int pattern_match_1(IN char * szPattern)

		Pattern�ǲ��ǺͿ�Match

Parameters
		IN char * szPattern			

Return Value
		Yes  return	TRUE
		No   return FALSE

Comments
		it is thread no care.
  
Create by spAce 2001-10-05
Modify by spAce 2001-10-25
*********************************************************************/
int pattern_match_1(IN const char * szPattern)
{
    if( *szPattern && *szPattern != '*' ) 
	{
 
        return FALSE;
    }
	else if(strlen(szPattern)>1)    
	{
		return pattern_match_1(szPattern+1);
	}    
    return TRUE;
}


/*********************************************************************
int pattern_match(IN char * szPattern, IN char * szName )

		���szName�Ƿ����szPattern

Parameters
		IN char * szPattern	ͳ��
		IN char * szName     ����
		
Return Value
		Success  return	�û���ѡ����
		Failed   return	(ULONG)-1 

Comments
		it is thread-safe.

		szPattern ֧�� * ,����Ҫ��д��

Create by spAce 2001-10-05
Modify by spAce 2001-10-25
*********************************************************************/
LIBUTILS_API int UTILS_API pattern_matchA(const char * szPattern, const char * szName )
{
	char matchchar;

    if( !*szPattern ) 
	{
		if(!*szName)
			return TRUE;
		else
			return FALSE;
    }
    //
    // If we hit a wild card, do recursion
    //
    if( *szPattern == '*' ) {

        szPattern++;
        while( *szName && *szPattern ) {

			matchchar = *szName;
			if( matchchar >= 'a' && 
				matchchar <= 'z' &&
				*szPattern>= 'A' &&
				*szPattern<= 'Z') 
			{

				matchchar -= 'a' - 'A';
			}
			else if( matchchar >= 'A' && 
				matchchar <= 'Z' &&
				*szPattern>= 'a' &&
				*szPattern<= 'z') 
			{

				matchchar += 'a' - 'A';
			}


            //
            // See if this substring matches
            //
		    if( *szPattern == matchchar ) {

  		        if( pattern_matchA( szPattern+1, szName+1 )) {

                    return TRUE;
                }
            }

            //
            // Try the next substring
            //
            szName++;
        }

        //
        // See if match condition was met
        //
        return pattern_match_1( szPattern );
    } 

    //
    // Do straight compare until we hit a wild card
    //
    while( *szName && *szPattern != '*' ) {

		matchchar = *szName;
		if( matchchar >= 'a' && 
				matchchar <= 'z' &&
				*szPattern>= 'A' &&
				*szPattern<= 'Z') 
		{

			matchchar -= 'a' - 'A';
		}
		else if( matchchar >= 'A' && 
			matchchar <= 'Z' &&
			*szPattern>= 'a' &&
			*szPattern<= 'z') 
		{

			matchchar += 'a' - 'A';
		}

        if( *szPattern == matchchar ) {
            szPattern++;
            szName++;

        } else {

            return FALSE;
		}
    }

    //
    // If not done, recurse
    //
    if( *szName ) {

        return pattern_matchA( szPattern, szName );
    }

    //
    // Make sure its a match
    //
    return pattern_match_1( szPattern );
}
