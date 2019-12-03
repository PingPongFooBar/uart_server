#include "junc.h"

int junc_init(){
	int i;
	char jun_name[15] = "/dev/junction1";
	char oc_cmd[15] = "./oc_protect";
	for(i = 0;i < DEVNUM;i++){
		sprintf(jun_name,"/dev/junction%d",(i + 1));
		sprintf(oc_cmd,"./oc_protect %d &", (i + 1));
		juns[i] = open(jun_name,O_RDWR);
		if(juns[i] < 0){
			printf("Fail to open jun%d device.\n",(i + 1));
			return -1;
		}
		system(oc_cmd);
	}
	return 0;
}
