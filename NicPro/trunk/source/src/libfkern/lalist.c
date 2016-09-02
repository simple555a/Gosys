/*
	look-aside list support
*/
#include "precomp.h"

struct lalist{
	void	*used_head;
	void	*free_head;
	f8_uint	free_items;
	f8_uint	used_items;
	f8_uint	item_size;
	f8_byte	items[1];
};

/*
	initialization
*/

/*
	alloc
*/

/*
	free
*/

