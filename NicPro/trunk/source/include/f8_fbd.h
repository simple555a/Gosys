/*
	F8 system function block diagram support
	Apr, 2003, panxiaohui@actech-mc.com
*/
#ifndef __f8_fbd_h__
#define __f8_fbd_h__

#include <pshpack1.h>

#if defined(_WIN32) && !defined(FCC_INVOKED)
#ifdef LIBFBD_EXPORTS
#define FBD_API __declspec(dllexport)
#else
#define FBD_API __declspec(dllimport)
#endif
#else
#define FBD_API
#endif

/*++
	SEE $(TOPDIR)/HISTORY for comments
--*/

/*
	maximum number of pin/function/instance data a bblk can have
	(constraint works on each type, not total)
*/
#define MAX_BBLK_ITEMS	256
/*
	maximum byte of code a bblk can have.
*/
#define MAX_CODES		65536

/*
*/
struct blk_pin_t{
// must be EI,EI,DI,DO, etc
	f8_u8	_class;

	// PIN_T_...
	f8_u8	type;

	//
	f8_u8	flags;
	f8_u8	binding;

	// pointer to the containing IBlk, RTO
	struct __tag_IBlk	*blk;

	// to upper level pin(external)
	struct	blk_pin_t	*ulink;

	// link field, RTO
	union{
		// basic block's pins have this part
		TRIPLE_LIST_ENTRY link;

		// composite block's pins have this part
		struct {
			union{
				struct {
					// id of containing subblock in cblk's subblock array, INDEX
					f8_u16	bid;
					// id into containing cblk's pin array, INDEX
					f8_u16	pid;	
				}PACKED id;
				// to lower level pin(internal)
				struct blk_pin_t * llink;
			}PACKED id;
			/*
				real_pin points to the real pin object, but NEVER access
				this field outside the binary library. use IBlk_real_pin
				instead to get the real pin.
			*/
			struct blk_pin_t * real_pin;			
		}PACKED x;
	}u1;

	union{
		f8_ptr	invoke;
		struct mem_addr_t mem_addr;	
	}u2;

	struct dyn_pin_t * mdp;
	struct dyn_pin_t * dp;
}PACKED;	// struct blk_pin_t

/*
	dyn_pin_t is a 8-byte data structure that represent a pin's
	dynamic state, this will contribute to the bulk of the information
	that are transferred on the control network.
*/
struct dyn_pin_t{
	union{
		struct	{
			// timer interval
			f8_int	interval;

			// timer quantum
			f8_int	tid;
		}PACKED e;
		
		union {
			f8_i8		i8;
			f8_i16		i16;
			f8_i32		i32;
			f8_i64		i64;
			f8_u8		ui8;
			f8_u16		ui16;
			f8_u32		ui32;
			f8_u64		ui64;
			f8_r4	flt;
			f8_r8	dbl;
		}PACKED d;
	}u;
};

// functions with this flag will be called upon block initialization
#define FUNC_F_INIT		(0x1 << 0)
// called upon block finalization
#define FUNC_F_FINAL	(0x1 << 1)

struct blk_func_t{
	f8_u16	flags;
	f8_u16	length;
	f8_ptr	address;
}PACKED;

/*
  these magic numbers appear in the binary stream as the
  delimeter between different binary objects(desendants of
  IBlk, see below)
 */
#define BBLK_MAGIC	0x4242	// 'BB' in little-endian
#define CBLK_MAGIC	0x4243	// 'CB' in little-endian
#define BLK_MAGIC	0x424c
#define PIN_MAGIC	0x5049
#define LNK_MAGIC	0x4c4b
#define VAR_MAGIC	0x5641

#define F8_ALIGN_SIZE		16
#define __F8_ALIGN__(n)	\
	((n) = (( (n) + F8_ALIGN_SIZE - 1 ) / F8_ALIGN_SIZE) * F8_ALIGN_SIZE)

#define F8_MAX_PATH	256

#define BLK_F_READONLY	0x0001
#define BLK_F_AUTOSCAN	0x0002
#define BLK_F_INPUT		0x0004
#define BLK_F_OUTPUT	0x0008
#define BLK_F_DISABLED	0x0010
#define BLK_F_SYSTEM     0x0020

#define IS_DEVICE_BLK(b) \
	((b)->h.flags & (BLK_F_INPUT | BLK_F_OUTPUT))

/* 
	compiler stage function block binary representation
*/
struct blk_hdr_t{
	f8_u16	magic;	// BBLK_MAGIC for basic block
					// CBLK_MAGIC for composite block
	f8_u16	n_pins;
	union{
		f8_u16	n_funcs;
		f8_u16	n_subblocks;
	}u1;
	union{
		f8_u16	n_codes;
		f8_u16	n_links;
	}u2;
	f8_u16	flags;
	f8_u16	padding_0;
	// uuid of this class of BBLK
	// NOTE!! it's different from the 'uuid' field in IBlk
	f8_uuid	uuid;
}PACKED;

struct bblk_hdr_t{
	f8_u16		n_data;
	f8_u16		n_bss;	
	// 2003/11/11, IEC-1131 fields(autoscan support)
	f8_int		x_order;
	RTK_LIST_ENTRY	x_link;
}PACKED;

CDECL_BEGIN

__class__(IBlk)
	__inherit__(ITreeNode)
	struct blk_hdr_t h;
	f8_uuid uuid;		/* instance UUID, note it's different from 
						class UUID contained in bblk_hdr_t or
						cblk_hdr_t */
__class_end__(IBlk)

__vtable__(IBlk)
	
	__inherit__(ITreeNode_vtbl)

	__vmethod__(
		__bool, 
		load, 
		(IBlk * _this, IF8Stream *s)
		);

	__vmethod__(
		__bool, 
		save, 
		(IBlk * _this, IF8Stream *s)
		);

	__vmethod__(
		struct blk_pin_t *, 
		pin_by_id, 
		(IBlk * _this, __u16 index)
		);

	__vmethod__(
		__u16,
		id_by_pin,
		(struct blk_pin_t * p)
		);

	__vmethod__(
		struct blk_pin_t *, 
		first_pin, 
		(IBlk * _this)
		);

	__vmethod__(
		struct blk_pin_t *, 
		next_pin, 
		(struct blk_pin_t *)
		);

__vtable_end__(IBlk)

// IBlk routines
FBD_API IBlk * IBlk_from_stream(IF8Stream * s);

#define parent_blk(blk) \
	__dcast__(ITreeNode, IBlk, \
		__ucast__(IBlk, ITreeNode, blk)->uplink\
	)
#define first_subblk(blk) \
	__dcast__(ITreeNode, IBlk, \
		__ucast__(IBlk, ITreeNode, blk)->first_child\
	)
#define next_blk(blk)\
	__dcast__(ITreeNode, IBlk, \
		ITreeNode_next(__ucast__(IBlk, ITreeNode, blk))\
		)
#define previous_blk(blk)\
	__dcast__(ITreeNode, IBlk, \
		ITreeNode_previous(__ucast__(IBlk, ITreeNode, blk))\
		)

#define BLK_IS_READONLY(b) \
	(b->h.magic == BBLK_MAGIC || b->h.flags & BLK_F_READONLY)

FBD_API __uint IBlk_pinsize(struct blk_pin_t * p);

FBD_API IBlk * IBlk_subblk(IBlk * _this, f8_uuid * uuid);

FBD_API IBlk * IBlk_blk_by_id(IBlk * _this, __u16 id);

FBD_API __u16 IBlk_id_by_blk(IBlk * _this);

FBD_API __uint IBlk_level(IBlk * b);

// struct blk_pin_t routines
FBD_API __bool IBlk_linkable(struct blk_pin_t * source, struct blk_pin_t * target);

FBD_API __bool IBlk_exportable(struct blk_pin_t * p);

FBD_API struct blk_pin_t * IBlk_real_pin(struct blk_pin_t *p);

FBD_API struct blk_pin_t * IBlk_peer(struct blk_pin_t *);

FBD_API __bool IBlk_is_output(struct blk_pin_t * t);

FBD_API __bool IBlk_is_multiple(struct blk_pin_t * t);

FBD_API __bool IBlk_connectable(struct blk_pin_t * p);

FBD_API IBlk * delphi_parent(IBlk * b);
FBD_API IBlk * delphi_firstsub(IBlk * b);
FBD_API IBlk * delphi_next(IBlk * b);

/*
	Basic function block
*/
__class__(IBBlk)
	__inherit__(IBlk)

	struct bblk_hdr_t	bh;
	/*
		2003/11/25
		A basic block is composed of 
		1) a code section, 
			code section holds block code and function array.
		2) a data section
			data section includes initialized instance data
			readonly instance data(most are constants)
		3) a bss section
			bss section holds block service table pointer,
			dynamic pin array
			uninitialized instance data

		During block loading, the loader might choose to share all block's 
		code section of the same block class UUID.
	*/

	f8_byte	* data_section;	// 
	f8_byte * code_section;	// block code
	f8_byte * bss_section;
	f8_byte * init_data;	// initialized data

	struct blk_pin_t  * pins;
	struct dyn_pin_t  * dpins;
	struct blk_func_t * funcs;
__class_end__(IBBlk)

__vtable__(IBBlk)
	__inherit__(IBlk_vtbl)
__vtable_end__(IBBlk)

#define BB2BLK(bb) __ucast__(IBBlk, IBlk, (bb))

/*
  a composite function block is composed of two parts:
  1) the function block network, which in turn contains a series
  of function blocks and some links between them;
  2) the pin map, which defines some internal pins to be visible
  from outside world.
  composite function block's internal data are allocated statically,
  because they aren't expected to be changed when in scheduling and
  configuration.
*/
__class__(ICBlk)
	__inherit__(IBlk)
	RTK_LIST_ENTRY	links;
	RTK_LIST_ENTRY	pins;
__class_end__(ICBlk)

__vtable__(ICBlk)
	__inherit__(IBlk_vtbl)
__vtable_end__(ICBlk)

#define CB2BLK(bb) __ucast__(ICBlk, IBlk, (cb))

union blk_pindef_t{
	struct {
		f8_u16	bid;
		f8_u16	pid;
	}PACKED id;
	struct blk_pin_t * pin;
}PACKED;

struct cblk_link_t{
	RTK_LIST_ENTRY			sibling;
	f8_uuid				uuid;
	union blk_pindef_t	s;
	union blk_pindef_t	t;
	ICBlk				*blk;
	__uint				tag[4];
}PACKED;

struct cblk_pin_t{
	RTK_LIST_ENTRY			sibling;
	// uuid
	f8_uuid				uuid;
	struct blk_pin_t	p;
}PACKED;

// ICBlk routines
FBD_API __bool ICBlk_add_blk(ICBlk * _this, IBlk * b);

FBD_API __bool ICBlk_del_blk(IBlk * b);

FBD_API struct cblk_pin_t * ICBlk_add_pin(f8_uuid * uuid, struct blk_pin_t * p);

FBD_API __bool ICBlk_del_pin(struct blk_pin_t * p);

FBD_API struct cblk_link_t * ICBlk_link(f8_uuid * id, struct blk_pin_t * s, struct blk_pin_t * t);

FBD_API struct cblk_link_t * ICBlk_get_link(ICBlk * blk, f8_uuid *id);

FBD_API __bool ICBlk_unlink(struct cblk_link_t *);

FBD_API struct cblk_link_t * ICBlk_first_link(ICBlk * _this);

FBD_API struct cblk_link_t * ICBlk_next_link(struct cblk_link_t * l);

FBD_API struct cblk_link_t * ICBlk_first_connection(struct blk_pin_t *);

FBD_API struct cblk_link_t * ICBlk_next_connection(struct blk_pin_t *, struct cblk_link_t *);

/*
	Note : delete_blk is used to delete a standlone block, freeing
	memory allocated on it, never use it on a block mounted to a 
	block tree, as that will leave un-detached pointers. In that
	case, you should call 'ITreeNode::umount' instead, the overridden
	'before_umount' and 'after_umount' will ensure that all pointers
	are detached and all memory are released(by calling delete_blk
	internally).

	N.B. never initialize a block from outside heap, i.e., block objects
	can only be dynamiclly created, directly calling block objects'
	create method might cause the object un-destroyable.
*/
FBD_API void delete_blk(IBlk * blk);

#ifdef _MSC_VER
#define umount_blk(__blk__) \
do{\
	ITreeNode_cut_branch(__ucast__(IBlk, ITreeNode, __blk__));\
}while(0)
#else
// GCC seems cannot handle too deep macro expansions
FBD_API void umount_blk(IBlk * blk);
#endif

CDECL_END

#include <poppack.h>

CDECL_BEGIN

FBD_API IBlk * IBlk_subblk_by_name(IBlk * me, const char * n);

FBD_API struct blk_pin_t * IBlk_pin_by_path(	IBlk * cur, const char * path);

FBD_API IBlk * IBlk_blk_by_path(IBlk * cur, const char * path);

FBD_API const char * IBlk_pin_name(struct blk_pin_t * p);

FBD_API struct blk_pin_t * IBlk_pin_by_name(IBlk * blk, const char * name);

FBD_API __uint IBlk_path_by_pin(struct blk_pin_t * p, char *buf, __uint len);

FBD_API const char * IBlk_class_name(IBlk * blk);

FBD_API const char * IBlk_name(IBlk * blk);

FBD_API __uint IBlk_path_by_blk(IBlk * blk, char *buf, __uint len);

FBD_API __u16 ICBlk_id_by_link(struct cblk_link_t * lk);

FBD_API struct cblk_link_t * ICBlk_link_by_id(ICBlk *cblk, __u16 id);

FBD_API f8_bool IBlk_set_data	(IBlk * blk, const char * name, void * buf, int len);

FBD_API void * IBlk_get_data(IBlk * blk, const char * name, int * len);

CDECL_END

#endif

