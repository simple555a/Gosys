/*
	block/links graphical representation management.
	by J.Hunter, 2005/9/13
*/
#include "precomp.h"

/*
	place block <name> <coordinate>
	place link <target-name> <coordinate list>
*/
int on_place(char *v, void *k)
{
	CArgsEx a;
	char * name=0;
	TLinkMetrics m;
	IBlk * b = 0;
	struct blk_pin_t * pin;
	struct cblk_link_t * l;
	int i;
		
	a.parse(v, FS "(),");
	if(a.argc < 3){
		return F8_SYNTAX;
	}
	utils_trim(a.argv[2], '"');
	if(!strcmp(a[1], "block")){
		// place block
		b = IBlk_blk_by_path(sections, a.argv[2]);
		if(!b){
			return F8_OBJECT_NOT_FOUND;
		}
		if(a.argc < 5){
			return F8_SYNTAX;
		}
		set_res_buf(&b->uuid, a[3], strlen(a[3]) + 1, "x");
		set_res_buf(&b->uuid, a[4], strlen(a[4]) + 1, "y");
	}else if(!strcmp(a[1], "link")){
		// place link
		pin = IBlk_pin_by_path(sections, a.argv[2]);
		if(!pin){
			return F8_OBJECT_NOT_FOUND;
		}
		if(pin->_class != PIN_CLASS_DI && pin->_class != PIN_CLASS_EI){
			return F8_INVALID_DATA;
		}
		if(a.argc < 4){
			return F8_SYNTAX;
		}
		m.count = atoi(a.argv[3]);
		if(a.argc < 4 + 2*m.count){
			return F8_SYNTAX;
		}
		l = ICBlk_first_connection(pin);
		if(!l){
			return F8_NO_CONNECTION;
		}
		for(i=0; i<m.count; i++){
			m.coord[i].x = atoi(a.argv[2*i + 4]);
			m.coord[i].y = atoi(a.argv[2*i + 5]);
		}
		set_res_buf(&l->uuid, &m, sizeof(m), "metrics");
	}else{
		return F8_SYNTAX;
	}
	
	return F8_SUCCESS;
}

