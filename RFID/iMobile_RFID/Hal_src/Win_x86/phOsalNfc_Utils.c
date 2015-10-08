
 // * $Revision: 1.1 $
 

#include <phNfcStatus.h>
#include <phNfcCompId.h>
#include <phNfcConfig.h>
#include <phOsalNfc.h>


/*The function does a comparison of two strings and returns a non zero value 
if two strings are unequal*/
int phOsalNfc_MemCompare ( void *src, void *dest, unsigned int n )
{
    int8_t *b1  =(int8_t *)src;
    int8_t *b2  =(int8_t *)dest;
    int8_t   diff = 0;
#ifdef VERIFY_MEMORY
    if((NULL != src) && (NULL != dest))
#endif
    {
        for(;((n>0)&&(diff==0));n--,b1++,b2++)
        {
            diff = *b1 - *b2;
        }
    }
    return (int)diff;
}




