/*
	do the convertion needed to produce a 
	controller-executable.

	read the gcc-compiled object file, convert it into a
	format understood by the f8-linker. the linker will
	combine the output of this program together with the block
	header to get the final executable file, which can be
	mounted in a f8-program, or loaded at runtime.
	
	history :	
	2003/11 jhunter@tsinghua.org.cn
*/

#define __LINUX__
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <bfd.h>
#include <strings.h>
#include <f8.h>

#define MAX_BIBI 65536
#define R_386_GOTPC 10
#define R_386_GOTOFF 9

static char text_buf[MAX_BIBI];
static int	text_len;

static asection * code_section;
static asection * data_section;
static asection * bss_section;
static asection * rodata_section;
static asection * rodata8_section;

static int code_offset;
static int data_offset;
static int bss_offset;
static int rodata_offset;
static int rodata8_offset;
static int debug;

/*
merge all .text sections
merge all data sections
merge all bss sections
merge .data + .got + .bss
relocate .text section
*/

static bfd *ibfd;

static asymbol **syms;
static long number_of_symbols;

struct func_t{
	int	name;
	int addr;
};

static func_t funcs[256];
static char str_heap[MAX_BIBI];
static int heap_ptr;

static struct blk_hdr_t m_hdr;
static struct bblk_hdr_t m_bhdr;

#define CANNOT_LOAD_DATA	-1
#define CANNOT_LOAD_TEXT	-2
#define CANNOT_LOAD_BSS		-3
#define RELOC_IN_DATA		-4
#define RELOC_IN_BSS		-5
#define INVALID_SECTION		-6
#define MULTIPLE_SECTION	-7

static void _get_sec_cb(bfd * abfd, asection * sect, PTR obj)
{
	bfd_boolean bret;
	int ret = 0;
	int align, len, offset;
	
	align = 1<<sect->alignment_power;
	
	offset = text_len;
	
	// len = ((sect->rawsize + align - 1)>>sect->alignment_power)<<sect->alignment_power;
	len = sect->size;
	
	if((sect->flags & (SEC_ALLOC | SEC_HAS_CONTENTS))
		==
		(SEC_ALLOC | SEC_HAS_CONTENTS)
		)
	{
		if(debug){
			printf("Getting contents of section %s...\n", sect->name);
		}
		if(sect->flags & SEC_CODE){
			if(m_hdr.u2.n_codes){
				fprintf(
					stderr, 
					"Warning : two or more .text sections detected.\n"
					);
				ret = MULTIPLE_SECTION;
			}else{
				m_hdr.u2.n_codes = len;
			}
		}
		/*
			combine this text section with previous detected ones
			2005/6/22 it's assumed that the .text section goes before
			data sections(including .rodata.cst4/8, etc.)
		*/
		bret = bfd_get_section_contents(
			abfd, 
			sect, 
			text_buf + text_len, 
			0,
			sect->size
			);
		if(!bret){
			ret = CANNOT_LOAD_DATA;
		}
		text_len += len;
	}
	
	if(debug){
		printf("section %s, raw %d, vma %08x, offset %08x, length %08x size %08x\n", 
			sect->name,
			sect->rawsize,
			sect->alignment_power,
			offset,
			len,
			sect->size
			);
	}
	
	if((sect->flags & SEC_ALLOC) && !(sect->flags & SEC_HAS_CONTENTS)){
		/*
			Hmm, we guess this should be a bss section 
		*/
		// BUGBUG: what if there are more than one bss?
		//m_bhdr.n_bss = len;
	}
	if(!strcmp(sect->name, ".data")){
		if(sect->reloc_count){
			ret = RELOC_IN_DATA;
		}
		data_section = sect;
		data_offset = offset;
	}else if(!strcmp(sect->name, ".text")){
		// ok
		code_section = sect;
		code_offset = offset;
	}else if(!strcmp(sect->name, ".bss")){
		// ok
		bss_section = sect;
		bss_offset = offset;
		m_bhdr.n_bss = len;
	}else if(!strcmp(sect->name, ".comment")){
		// ignored
	}else if(!strcmp(sect->name, ".rodata.cst4")){
		// ok
		rodata_section = sect;
		rodata_offset = offset;
	}else if(!strcmp(sect->name, ".rodata.cst8")){
		// ok
		rodata8_section = sect;
		rodata8_offset = offset;
	}else{
		// unknown section
		fprintf(stderr, "Unknown section type %s\n", sect->name);
		ret = INVALID_SECTION;
	}
	
	if(!*((int*)obj)){
		*((int*)obj) = ret;
	}	
}

static int _get_sections()
{
	int	ret = 0;
	bfd_map_over_sections(ibfd, _get_sec_cb, &ret);
	if(debug){
		printf("%d\n", ret);
	}
	m_bhdr.n_data = text_len - m_hdr.u2.n_codes;
	bss_offset = text_len;
	if(debug){
		printf(
			"block header, data=%d, code=%d, bss=%d\n",
			m_bhdr.n_data,
			m_hdr.u2.n_codes,
			m_bhdr.n_bss
			);
	}
	return ret;
}

/*
	perform relocations
	
	NOTe: although the source files are compiled with -fPIC flags on, there
	are still relocations in the .o files, because the relative position of the 
	output sections are not determined yet. For our proprietary .ce file format,
	we have merged all input sections into a flat section(piggy), and we've already
	tracked offset of each input section in piggy section, so relocations are 
	processed on this assumption.

	Another note, although we take no assumption about the precise order
	that each input section will appear in the piggy section, we do expect
	that each one of them appears only once, and typically they will come
	in this order:
	.text, .data, and others
	if this assumption doesn't hold, the loader will be confused.
*/
static int _relocate()
{
	int i;
	void * patched;
	int offset;
	int target;
	int got_position;
	long count;
	asection * ts;
	arelent ** reloc;
	symbol_info symbolinfo;
	asymbol * sym;
	
	got_position = text_len + m_bhdr.n_bss;
	
	if(debug){
		printf("GOT position : %08x\n", got_position);
	}
	ts = bfd_get_section_by_name(ibfd, ".text");
	if(!ts){
		return INVALID_SECTION;
	}
	count = bfd_get_reloc_upper_bound(ibfd, ts);
	if(count < 0){
		return count;
	}
	reloc = (arelent **)malloc(count);
	if(!reloc){
		return -1;
	}
	count = bfd_canonicalize_reloc(ibfd, ts, reloc, syms);
	if(count  < 0){
		return count;
	}
	assert((const int)count == ts->reloc_count);
	for(i=0; i<ts->reloc_count; i++){
		sym = *reloc[i]->sym_ptr_ptr;
		bfd_symbol_info(sym, &symbolinfo);
		if(debug){
			printf("RLC:(T=%d,RS=%d,SZ=%d,PCR=%d,BP=%d,COO=%d,SF=0x%08x,NAME=%s,PI=%d,SM=0x%08x,DM=0x%08x,PO=%d\n",
				reloc[i]->howto->type,
				reloc[i]->howto->rightshift,
				reloc[i]->howto->size,
				reloc[i]->howto->pc_relative,
				reloc[i]->howto->bitpos,
				reloc[i]->howto->complain_on_overflow,
				reloc[i]->howto->special_function,
				reloc[i]->howto->name,
				reloc[i]->howto->partial_inplace,
				reloc[i]->howto->src_mask,
				reloc[i]->howto->dst_mask,
				reloc[i]->howto->pcrel_offset
				);
		}
		if(debug){
			printf(
				"reloc to %s offset %d size %d type %d addend %d value %08x\n", 
				// (*reloc[i]->sym_ptr_ptr)->name,
				symbolinfo.name,
				reloc[i]->address,
				bfd_get_reloc_size(reloc[i]->howto),
				reloc[i]->howto->type,
				reloc[i]->addend,
				symbolinfo.value
				);
		}
		/* find out position in the output section that this relocation
		desingnates */
		if((*reloc[i]->sym_ptr_ptr)->section == data_section){
			if(debug){
				printf("Relocate to data section %08x\n",
					reloc[i]->address
					);
			}
			target = data_offset;
		}else if((*reloc[i]->sym_ptr_ptr)->section == bss_section){
			if(debug){
				printf("Relocate to bss section %08x\n",
					reloc[i]->address
					);
			}
			target = bss_offset;
		}else if((*reloc[i]->sym_ptr_ptr)->section == rodata_section){
			if(debug){
				printf("Relocate to rodata section %08x\n",
					reloc[i]->address
					);
			}
			target = rodata_offset;
		}else if((*reloc[i]->sym_ptr_ptr)->section == rodata8_section){
			if(debug){
				printf("Relocate to rodata-8 section %08x\n",
					reloc[i]->address
					);
			}
			target = rodata8_offset;
		}else{
			if(reloc[i]->howto->type != R_386_GOTPC){
			fprintf(
				stderr,
				"RLC '%s':(T=%d,RS=%d,SZ=%d,PCR=%d,BP=%d,COO=%d,SF=0x%08x,NAME=%s,PI=%d,SM=0x%08x,DM=0x%08x,PO=%d)\n",
				(*reloc[i]->sym_ptr_ptr)->name,
				reloc[i]->howto->type,
				reloc[i]->howto->rightshift,
				reloc[i]->howto->size,
				reloc[i]->howto->pc_relative,
				reloc[i]->howto->bitpos,
				reloc[i]->howto->complain_on_overflow,
				reloc[i]->howto->special_function,
				reloc[i]->howto->name,
				reloc[i]->howto->partial_inplace,
				reloc[i]->howto->src_mask,
				reloc[i]->howto->dst_mask,
				reloc[i]->howto->pcrel_offset
				);
			fprintf(
					stderr, 
					"Relocation into nowhere(%s).\n",
					(*reloc[i]->sym_ptr_ptr)->section->name
					);
				return -1;
			}
		}
		
		target += symbolinfo.value;
		offset = reloc[i]->address + code_offset;
		patched = text_buf + reloc[i]->address;
		
		if(debug){
			printf(
				"Target=%08x, Got=%08x, Offset=%08x\n",
				(char*)target - text_buf,
				got_position,
				offset
				);
		}
		
		switch(reloc[i]->howto->type){
		case R_386_GOTPC:
			{
				char * bb = (char*)patched;
				if(bb[-1] == 0x5){
					// addl $_GLOBAL_OFFSET_TABLE_+[.-.L2], %eax found
					*((int*)patched) = got_position - offset + 2;
				}else{
					// addl $_GLOBAL_OFFSET_TABLE_+[.-.L2], %eax found
					// or
					// addl $_GLOBAL_OFFSET_TABLE_+[.-.L2], %ecx found
					*((int*)patched) = got_position - offset + 3;
				}
			}
			break;
		case R_386_GOTOFF:
			*((int*)patched) = target - got_position + *((int*)patched);
			break;
		default:
			fprintf(
				stderr,
				"Unknown relocation type : %d\n", 
				reloc[i]->howto->type
				);
			return -1;
		}
		if(debug){
			printf("patched with %08x\n", *((int*)patched));
		}
	}
	
	free(reloc);
	
	return 0;
}

static int _heap_add_string(const char * s)
{
	int ret = heap_ptr;
	int len = strlen(s);
	memcpy(str_heap + heap_ptr, s, len);
	heap_ptr += len + 1;
	return ret;
}

static int _get_functions()
{
	long storage_needed;	
	long i;
	symbol_info symbolinfo ;
	
	storage_needed = bfd_get_symtab_upper_bound (ibfd);
	
	if (storage_needed < 0){
		return -1;
	}
	
	if (storage_needed == 0) {
		return -1;
	}
	syms = (asymbol **) malloc (storage_needed);
	number_of_symbols = bfd_canonicalize_symtab (ibfd, syms);
	
	if (number_of_symbols < 0){
		return -1;
	}
	if(debug){	
		printf("Scanning %i symbols\n", number_of_symbols);
	}
	for(i=0;i<number_of_symbols;i++){
		if(syms[i]->section==NULL) {
			continue;
		}
		bfd_symbol_info(syms[i], &symbolinfo);
		if(debug){
			printf(
				"Symbol \"%s\"	value 0x%x, section %s\n",
				symbolinfo.name, 
				symbolinfo.value,
				syms[i]->section->name
				);
		}
		if(!strcmp(syms[i]->section->name, ".text") && (syms[i]->flags & BSF_FUNCTION)){
			if(debug){
				printf("Got function %s\n", symbolinfo.name);
			}
			funcs[m_hdr.u1.n_funcs].addr = syms[i]->value;
			funcs[m_hdr.u1.n_funcs].name = _heap_add_string(syms[i]->name);
			m_hdr.u1.n_funcs++;
		}
	}
	
	return 0;
}

static int _write_out()
{
	FILE * fp;
	fp = fopen("libfcc-temp.obj", "wb");
	if(!fp){
		fprintf(stderr, "Cannot open libfcc-temp.obj.\n");
		return -1;
	}
	fwrite(&m_hdr, 1, sizeof(m_hdr), fp);
	fwrite(&m_bhdr, 1, sizeof(m_bhdr), fp);
	fwrite(funcs, sizeof(func_t), m_hdr.u1.n_funcs, fp);
	fwrite(&heap_ptr, 1, sizeof(heap_ptr), fp);
	fwrite(str_heap, 1, heap_ptr, fp);
	fwrite(text_buf, 1, text_len, fp);
	fclose(fp);
	return 0;
}

int main(int argc, char *argv[])
{	
	char *filename;
	char **matching;
	int ret;
	
	if (argc<2) {
		exit(-1);
	}
	if(argc >= 3){
		debug = 1;
	}
	filename = argv[1];

	bfd_init();
	
	ibfd = bfd_openr(filename, NULL);
	if (ibfd == NULL){
		fprintf(stderr, "Error opening %s\n", argv[1]);
		return -1;
	}
	
	if (!bfd_check_format_matches(ibfd, bfd_object, &matching)){
		fprintf(stderr, "Error matching.\n");
		return -1;
	}
	
	if(ret = _get_sections()){
		fprintf(stderr, "Warning : error in _get_section : %d\n", ret);
	}
	
	if(ret = _get_functions() ){
		fprintf(stderr, "error in _get_funtions : %d(%08x)\n", ret, ret);
		return ret;
	}
	
	if(ret = _relocate()){
		fprintf(stderr, "error in _relocate : %d(%08x)\n", ret, ret);
		return ret;
	}
	
	if(debug){
		printf(
			"block header, data=%d, code=%d, bss=%d\n",
			m_bhdr.n_data,
			m_hdr.u2.n_codes,
			m_bhdr.n_bss
			);
	}
	
	if(ret = _write_out()){
		fprintf(stderr, "error in _write_out : %d(%08x)\n", ret, ret);
		return ret;
	}
	
	bfd_close(ibfd);
	
	return 0;
}
