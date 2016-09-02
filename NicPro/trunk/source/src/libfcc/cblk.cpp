#include "precomp.h"

static void print_type(int t)
{
#define dd(t) case t:printf("%s", #t);break;
	switch(t){
	dd(cblk_TOK_CBLOCK);
	dd(cblk_TOK_USES);
	dd(cblk_TOK_LINKS);
	dd(cblk_TOK_END);
#if 0
	dd(cblk_TOK_UUID);
	dd(SYM_BLOCK_DECLARATION);
	dd(SYM_LINK_DECLARATION);
	dd(SYM_PIN);
	dd(cblk_TOK_ID);
	dd(cblk_TOK_FBLOCK_NAME);
	dd(cblk_TOK_EXPORTS);
	dd(SYM_EXPORT_DECLARATION);
#endif
#undef dd
	}
}

static void visit_no_recursive(CSymbol * nd, int k)
{
	for(;k;k--){
		printf("    ");
	}	
	print_type(nd->type);
	if(nd->val.text){
		printf(",%s", nd->val.text);
	}else 	if(nd->type == cblk_TOK_UUID){
		__u16 * u=(__u16*)&nd->val.uuid;
		printf(",%04x-", u[0]);
		printf("%04x-", u[1]);
		printf("%04x-", u[2]);
		printf("%04x", u[3]);
	}else{
		printf(",%d", nd->val.ival);
	}
	printf("\n");
}

static __bool _cdecl visit(CTreeNode *_nd, __uint)
{
	CSymbol *nd = (CSymbol*)_nd;
	CSymbol *rt = nd;
	int level = 0;
	while(rt != g_program){
		level++;
		rt = (CSymbol*)rt->GetParent();
	}
	visit_no_recursive(nd, level);
	return __true;
}

static void dump_tree(CSymbol *root)
{
	root->ForEach(visit, __true);
}

class CCblkCompiler 
{
public:
	__bool generate(CSymbol *root, IF8Stream *s);

protected:
	void clean();
	__bool load_fblocks();
	__bool load_links();
	__bool load_exports();
	__bool load_init_events();
	__bool load_comments();
protected:
	ICBlk	* a;
	IBlk	* b;
	CSymbol * root;
};

static CCblkCompiler gen;

void CCblkCompiler::clean()
{
	a = 0;
	b = 0;
}

__bool CCblkCompiler::generate(CSymbol *root, IF8Stream *s)
{
	clean();

	__new__(ICBlk, a);
	if(!a){
		return __false;
	}
	b = __ucast__(ICBlk, IBlk, a);

	this->root = root;

	CSymbol * sym;
	sym = _get_node_by_type(root, cblk_TOK_UUID);
	b->h.uuid = sym->val.uuid;
	memset(&b->uuid, 0, sizeof(b->uuid));

	b->h.flags = BLK_F_READONLY;

	if(!create_resource(&b->h.uuid)){
		return __false;
	}
	set_res_int(&b->h.uuid, "!readonly", 1);

	set_res_name(&b->h.uuid, root->val.text);

	if(!load_comments()){
		return __false;
	}

	if(!load_fblocks()){
		return __false;
	}

	if(!load_links()){
		return __false;
	}

	if(!load_exports()){
		return __false;
	}
	
	__vcall__(b, save, (b, s));

	save_res_stream(&b->h.uuid, s);	

	__delete__(a);

	return 0;
}

__bool CCblkCompiler::load_comments()
{
	CSymbol * sym;
	CSymbol * cmt;
	CSymbol * val;

	sym = _get_node_by_type(root, cblk_TOK_COMMENTS);
	cmt = (CSymbol*)sym->GetFirstChild();
	while(cmt){
		val = (CSymbol*)cmt->GetFirstChild();
		assert(val);
		set_res_buf_v(
			&b->h.uuid,
			val->val.text,
			strlen(val->val.text) + 1,
			"%s",
			cmt->val.text
			);
		cmt = (CSymbol*)cmt->GetNextSibling();
	}

	return __true;
}

__bool CCblkCompiler::load_fblocks()
{
	CSymbol * sym;
	CSymbol * fb;
	string	file;
	CSymbol *l, *r;
	IBlk * blk;
	__u16	bid = 0;

	sym = _get_node_by_type(root, cblk_TOK_USES);
	if(!sym){
		return __true;
	}

	fb = (CSymbol*)sym->GetFirstChild();
	while(fb){
		l = (CSymbol*)fb->GetFirstChild();
		r = (CSymbol*)fb->GetChild(fb->GetItems() - 1);

		while(l != r){
			blklib_load_by_type(r->val.text, &blk);
			if( !blk ){
				fprintf(stderr, "Error loading %s\n", r->val.text);
				return __false;
			}

			blk->h.flags |= BLK_F_READONLY;
			create_f8_uuid(&blk->uuid);

			// set block name resource
			set_res_buf_v(
				&b->h.uuid,
				l->val.text,
				strlen(l->val.text) + 1,
				"!b%d!name",
				bid
				);		

			if(!ICBlk_add_blk(a, blk)){
				fprintf(stderr, "Error inserting %s\n", l->val.text);
				return __false;
			}		
			
			bid++;		
			l = (CSymbol*)l->GetNextSibling();
		}
		fb = (CSymbol*)fb->GetNextSibling();
	}
	
	return __true; 
}

__bool CCblkCompiler::load_links()
{
	CSymbol * sym;
	CSymbol * v;
	CSymbol *l, *r, *ll, *rr;
	CSymbol * pin;
	IBlk *sb, *tb;
	struct blk_pin_t *s,*t, *rs, *rt;
	f8_uuid	uuid;

	// _dump_res(0);

	sym = _get_node_by_type(root, cblk_TOK_LINKS);
	if(!sym){
		return __true;
	}

	v = (CSymbol*)sym->GetFirstChild();
	while(v){
		/*
		*
		*
			now load left side of the link declaration
		*
		*
		*/

		pin = (CSymbol*)v->GetFirstChild();	// source pin
		l = (CSymbol*)pin->GetFirstChild();	// source fblock
		r = (CSymbol*)l->GetNextSibling();	// source pin name

		sb = IBlk_subblk_by_name(b, l->val.text);
		if( !sb ){
			fprintf(
				stderr, 
				"Line %d, %s is not a function block.\n", 
				v->line, l->val.text
				);
			return __false;
		}

		s = IBlk_pin_by_name(sb, r->val.text);
		if( !s ){
			fprintf(
				stderr,
				"Line %d, %s is not in %s's pin-list.\n",
				v->line, r->val.text, l->val.text
				);
			return __false;
		}

		rs = IBlk_real_pin(s);
		switch(rs->_class){
		case PIN_CLASS_EI:
		case PIN_CLASS_DI:
			fprintf(
				stderr,
				"Error C0003: Line %d, %s.%s is not an OUTPUT event or data pin.\n",
				v->line, l->val.text, r->val.text
				);
			return __false;
			break;
		}

		if( !IBlk_connectable(s)){
			fprintf(
				stderr,
				"Error C0003: Line %d, %s.%s is not linkable.\n",
				v->line, l->val.text, r->val.text
				);
			return __false;
		}
		
		/*
		*
		*
			now load right side of the link declaration
		*
		*
		*/
		ll = l, rr = r;
		pin = (CSymbol*)pin->GetNextSibling();

		while(pin){				
			l = (CSymbol*)pin->GetFirstChild();	// dest fblock
			r = (CSymbol*)l->GetNextSibling();	// dest pin name

			tb = IBlk_subblk_by_name(b, l->val.text);
			if( !tb ){
				fprintf(
					stderr, 
					"Line %d, %s is not a function block.\n", 
					v->line, l->val.text
					);
				return __false;
			}

			t = IBlk_pin_by_name(tb, r->val.text);
			if( !t ){
				fprintf(
					stderr,
					"Line %d, %s is not in %s's pin-list.\n",
					v->line, r->val.text, l->val.text
					);
				return __false;
			}
			rt = IBlk_real_pin(t);

			if( !IBlk_connectable(t)){
				fprintf(
					stderr,
					"Error C0003: Line %d, %s.%s is not linkable.\n",
					v->line, l->val.text, r->val.text
					);
				return __false;
			}

			create_f8_uuid(&uuid);

			if(!ICBlk_link(&uuid, s, t)){
				fprintf(
					stderr,
					"Error C0003: Line %d, '%s.%s' and '%s.%s' cannot be connected.\n",
					v->line, 
					ll->val.text, rr->val.text,
					l->val.text, r->val.text
					);
				return __false;
			}

			pin = (CSymbol*)pin->GetNextSibling();
		}
		
		v = (CSymbol*)v->GetNextSibling();
	}

	
	return __true;
}

__bool CCblkCompiler::load_exports()
{
	CSymbol *sym, *v;
	CSymbol *l, *r;
	CSymbol *id, *pin;
	IBlk * blk;
	struct blk_pin_t *p;
	f8_uuid	uuid;

	sym = _get_node_by_type(root, cblk_TOK_EXPORTS);
	if( !sym ){
		return __true;
	}

	v = (CSymbol*)sym->GetFirstChild();
	while(v){
		id = (CSymbol*)v->GetFirstChild();
		pin = (CSymbol*)id->GetNextSibling();
		l = (CSymbol*)pin->GetFirstChild();
		r = (CSymbol*)l->GetNextSibling();

		/*
			syntax check,
			1) id must be at most 8 bytes long
			3) id must not be defined twice in exports and imports
		*/

		p = IBlk_pin_by_name(b, id->val.text);
		if( p ){
			fprintf(
				stderr,
				"Warning : Line %d, export %s re-declared.\n",
				id->line, id->val.text
				);
			continue;
		}

		if( strlen(id->val.text) > 64 ){
			fprintf(
				stderr,
				"Line %d, export name '%s' exceeds %d bytes.\n",
				id->line, id->val.text, 64
				);
			return __false;
		}

		blk = IBlk_subblk_by_name(b, l->val.text);
		if( !blk ){
			fprintf(
				stderr,
				"Line %d, '%s' : block not defined.\n",
				id->line, l->val.text
				);
			return __false;
		}			

		p = IBlk_pin_by_name(blk, r->val.text);
		if( !p ){
			fprintf(
				stderr,
				"Line %d, '%s' is not in pin list of block '%s'\n",
				id->line, r->val.text, l->val.text
				);
			return __false;
		}

		create_f8_uuid(&uuid);
		set_res_buf_v(
			&b->h.uuid, 
			id->val.text, 
			strlen(id->val.text) + 1, 
			"!p%d!name",
			b->h.n_pins
			);
		if( !ICBlk_add_pin(&uuid, p)){
			fprintf(
				stderr,
				"Failed add export '%s.%s'.\n",
				l->val.text, r->val.text
				);
			return __false;
		}


		v = (CSymbol*)v->GetNextSibling();
	}

	return __true;
}

int cblk_compile()
{
	init_blklib();
	
	cblk_in = fopen(input_file, "rt");
	if( !cblk_in ){
		fprintf(stderr, "%s: Can't open.\n", input_file);
		return -1;
	}
	cblk_parse();
	IFileStream * fs;
	IF8Stream *s;
	fs = IFileStream_from_file(output_file, "wb");
	if( !fs ){
		fprintf(stderr, "Error opening %s for writing.\n", output_file);
		return -1;
	}
	s = __ucast__(IFileStream, IF8Stream, fs);
	if(g_program){
		if(b_tree){
			dump_tree(g_program);
		}
		if( gen.generate(g_program, s) ){
			printf("Ok, %d bytes generated.\n", __vcall__(s, tell, (s)));
		}
	}
	__delete__(fs);

	return 0; 
}

int cblk_disassembly(ICBlk * b, f8_bool bRecursively)
{
	IBlk * blk;
	IBlk * sb;
	struct cblk_link_t * lk;
	struct blk_pin_t * p;
	char blkname[F8_MAX_PATH];
	char uuidname[64];

	blk = __ucast__(ICBlk, IBlk, b);

	utils_trace("cblock %s;\n", IBlk_class_name(blk));

	f8_uuid_to_string(&blk->h.uuid, uuidname, sizeof(uuidname));
	utils_trace("class_uuid{%s};\n", uuidname);
	f8_uuid_to_string(&blk->uuid, uuidname, sizeof(uuidname));
	utils_trace("uuid{%s};\n", uuidname);
	
	// uses section
	utils_trace("uses\n");
	sb = first_subblk(blk);
	while(sb){
		utils_trace(
			"\t%s : %s;\n", 
			IBlk_name(sb),
			IBlk_class_name(sb)
			);
		sb = next_blk(sb);
	}
	
	// links section
	utils_trace("links\n");
	lk = ICBlk_first_link(b);
	while(lk){
		utils_trace(
			"\t%s.%s -> %s.%s;\n", 
			IBlk_name(lk->s.pin->blk),
			IBlk_pin_name(lk->s.pin),
			IBlk_name(lk->t.pin->blk),
			IBlk_pin_name(lk->t.pin)
			);
		lk = ICBlk_next_link(lk);
	}

	utils_trace("exports\n");
	p = __vcall__(blk, first_pin, (blk));
	while(p){
		utils_trace(
			"\t%s = %s.%s;\n",
			IBlk_pin_name, (p),
			IBlk_name(p->u1.x.id.llink->blk),
			IBlk_pin_name(p->u1.x.id.llink)
			);
		p = __vcall__(blk, next_pin, (p));
	}
	
	if(bRecursively){
		sb = first_subblk(blk);
		while(sb){
			IBlk_path_by_blk(sb, blkname, sizeof(blkname));
			utils_trace("\n\n##### disassembly of %s ####\n", blkname);		
			blk_disassembly(sb);
			sb = next_blk(sb);
		}
	}

	utils_trace("end.\n");
	
	return 0;
}
