

#include "sys.h"
#include "stdlib.h"
#include "string.h"
#include "data_map.h"




void data_map_init(void)
{
	memset(DATA_MAP,0,1024);
	memcpy(DATA_MAP->devType,"cjq",4);
	memcpy(DATA_MAP->softVersion,"1.1",4);
	memcpy(DATA_MAP->extType,"cjq",4);
}












