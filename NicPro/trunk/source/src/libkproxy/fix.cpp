 /*
	This module manages configurations related to blocks.
	by J.Hunter, 2003/9/18
*/
#include "precomp.h"
#pragma hdrstop

struct CAddressConflit{
	f8_uint	magic1, magic2;
	void * o1, * o2;
	int a,b;
};

struct CInvalidAddress{
	f8_u32	magic;
	void * o;
	int a,b,c,d;
};

class CErrorDetector
{
public:
	static __bool _traverse_fixup_pins(ITreeNode * nd, __bool firstVisit, __uint not_used);
	__bool _fixup_pins(ITreeNode * nd);
	
	typedef list<CAddressConflit> CConflictList;
	CConflictList m_addrConflicts;

	typedef list<CInvalidAddress> CInvalidList;
	CInvalidList m_invalidAddress;

	void detect();
	void fix();
	void show();
};

void CErrorDetector::detect()
{
	m_addrConflicts.clear();
	m_invalidAddress.clear();
	_traverse(sections, _traverse_fixup_pins, (__uint)this);
}

void CErrorDetector::fix()
{
	CInvalidList::iterator i1;
	CConflictList::iterator i2;
	char path[F8_MAX_PATH];
	struct blk_pin_t * p;
	struct blk_var_t * v;

	for(i1 = m_invalidAddress.begin(); i1 != m_invalidAddress.end(); i1++){
		if(i1->magic == PIN_MAGIC){
			p = (struct blk_pin_t*)i1->o;
			IBlk_path_by_pin(p, path, sizeof(path));
			utils_error(
				"pin %s, %%%d%05d invalid, type %d\n", 
				path, p->u2.mem_addr.section, p->u2.mem_addr.addr, i1->a
				);
			switch(i1->a){
			case 0:
				/* allocate a register for this PIN */
				break;
			case 1:
				p->binding = PIN_B_CONST;
				break;
			case 2:
				break;
			}
		}else{
			v = (struct blk_var_t*)i1->o;
			IBlk_path_by_blk(v->blk, path, sizeof(path));
			strncat(path, ".", sizeof(path));
			strncat(path, v->name, sizeof(path));
			utils_error(
				"var %s, %%%d%05d invalid\n", 
				path, v->addr.section, v->addr.addr
				);
		}
	}
}

void CErrorDetector::show()
{
	CInvalidList::iterator i1;
	CConflictList::iterator i2;
	char path[F8_MAX_PATH];
	struct blk_pin_t * p;
	struct blk_var_t * v;

	for(i1 = m_invalidAddress.begin(); i1 != m_invalidAddress.end(); i1++){
		if(i1->magic == PIN_MAGIC){
			p = (struct blk_pin_t*)i1->o;
			IBlk_path_by_pin(p, path, sizeof(path));
			utils_error(
				"pin %s, %%%d%05d invalid, type %d\n", 
				path, p->u2.mem_addr.section, p->u2.mem_addr.addr, i1->a
				);
		}else{
			v = (struct blk_var_t*)i1->o;
			IBlk_path_by_blk(v->blk, path, sizeof(path));
			strncat(path, ".", sizeof(path));
			strncat(path, v->name, sizeof(path));
			utils_error(
				"var %s, %%%d%05d invalid\n", 
				path, v->addr.section, v->addr.addr
				);
		}
	}
}
__bool CErrorDetector::_traverse_fixup_pins(ITreeNode * nd, __bool firstVisit, __uint c)
{
	CErrorDetector * ed = (CErrorDetector*)c;
	return ed->_fixup_pins(nd);
}

/*
	errors checked in this procedure
	1) unattached DI with memory binding
	2) DO without memory binding
	3) DO with incorrect memory address
	4) DO with conflicting address
	5) multiple assignments of a variable
*/
__bool CErrorDetector::_fixup_pins(ITreeNode * nd)
{
	IBlk * b;
	struct blk_pin_t * p;
	
	void * o;
	const struct blk_var_t * var;
	struct blk_pin_t * rp;
	int length;

	b = __dcast__(ITreeNode, IBlk, nd);
	for(p = __vcall__(b, first_pin, (b)); p; p = __vcall__(b, next_pin, (p))){
		if(p->ulink){
			continue;
		}

		rp = IBlk_real_pin(p);
		
		if(!(rp->_class == PIN_CLASS_DI || rp->_class == PIN_CLASS_DO)){
			continue;
		}

		if(rp->_class == PIN_CLASS_DO && rp->binding != PIN_B_MEMORY){
			// invalid binding
			CInvalidAddress ia;
			ia.magic = PIN_MAGIC;
			ia.o = rp;
			ia.a = 0; /* DO without MEMORY BINDING */
			m_invalidAddress.insert(m_invalidAddress.end(), ia);
		}

		if(rp->binding != PIN_B_MEMORY){
			continue;
		}

		length = _type_size(rp->type);
		var = (blk_var_t*)query_var_by_pin(p);
		if(!var){
			if(rp->_class == PIN_CLASS_DI){
				if(RtkIsTripleListEmpty(&rp->u1.link)){
					CInvalidAddress ia;
					ia.magic = PIN_MAGIC;
					ia.o = rp;
					ia.a = 1; /* unattached DI with memory binding */
					m_invalidAddress.insert(m_invalidAddress.end(), ia);
				}
			}else{
				if(!is_address_valid(proxy_adapter->kernel, rp, &rp->u2.mem_addr)){
					CInvalidAddress ia;
					ia.magic = PIN_MAGIC;
					ia.o = rp;
					ia.a = 2; /* invalid address */
					m_invalidAddress.insert(m_invalidAddress.end(), ia);
				}
				CAddressConflit ac;
				/* check conflicts */
				o = get_address_owner(&rp->u2.mem_addr, 1, &ac.magic2);
				if(o == rp){
					continue;
				}
				ac.magic1 = PIN_MAGIC;
				ac.o1 = rp;
				ac.o2 = o;
				m_addrConflicts.insert(m_addrConflicts.end(), ac);
			}
		}else{
			// the error is credited to the variable, not check here
		}
	}

	return __true;
}

int on_fix(char *v, void *k)
{
	int	errors = 0;
	if(proxy_adapter->flags & ADAPTER_F_ONLINE){
		return F8_ALREADY_ONLINE;
	}
	CErrorDetector ed;
	ed.detect();
	ed.fix();
	return F8_SUCCESS;
}

int on_check(char *v, void *k)
{
	return check_program();
}

f8_status check_program()
{
	//
	int	errors = 0;
	CErrorDetector ed;
	ed.detect();
	ed.show();
	return F8_SUCCESS;
}


