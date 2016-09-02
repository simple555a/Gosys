#include "precomp.h"

void ITreeNode_on_mount(ITreeNode * __this)
{
	// 
}

void ITreeNode_before_umount(ITreeNode * __this)
{
}

void ITreeNode_after_umount(ITreeNode * __this)
{	
}

__define_vtbl__(ITreeNode)
	ITreeNode_on_mount,
	ITreeNode_before_umount,
	ITreeNode_after_umount
__define_vtbl_end__(ITreeNode)

/*
uplink
|
_____|_______
|			|  
BLink  ---------|	 item	|--------- FLink
|___________|
/			
/			  
/				
first_child

 
  */
__constructor__(ITreeNode)
{
	if(__this){
		void * saved_vtbl = (void *)__this->vtbl;
		memset(__this, 0, sizeof(*__this));
		*((void**)&__this->vtbl) = saved_vtbl;
		RtkInitializeListHead(&__this->sibling);
	}
	return __this;
}

__destructor__(ITreeNode)
{
	return __this;
}

__global_constructor__(ITreeNode)
{
}

/*
if mount_as_child_or_sibling is true, mount as child
else mount as sibling
fails if:
1) mount as sibling but peer has no parent, since allowing this will
incur two or more roots.
2) mount as child but the newcomer has already a parent(you must umount
it from its previous parent first), i.e., only root can be mounted
*/
__bool ITreeNode_mount(ITreeNode * peer, ITreeNode * newcomer, __bool mount_as_child)
{
	__bool ret;
	if(mount_as_child){
		if( newcomer->uplink ){
			ret = __false;
		}else{
			// insert as the last child
			newcomer->uplink = peer;
			if(peer->first_child){
				RtkInsertTailList(&peer->first_child->sibling, &newcomer->sibling);
			}else{
				peer->first_child = newcomer;				
			}
			ret = __true;
		}		
	}else{
		// as sibling
		if( !peer->uplink ){
			ret = __false;
		}else{
			newcomer->uplink = peer->uplink;
			RtkInsertTailList(
				&peer->uplink->first_child->sibling, 
				&newcomer->sibling
				);			
			ret =  __true;
		}
	}
	if(ret){
		__vcall__(newcomer, on_mount, (newcomer));
	}
	return ret;
}

ITreeNode * ITreeNode_umount(ITreeNode * child)
{
	if( child->uplink ){
		if(child == child->uplink->first_child ){
			child->uplink->first_child =ITreeNode_next(child);
		}
		RtkRemoveEntryList(&child->sibling);		
		child->uplink = NULL;
	}
	
	RtkInitializeListHead(&child->sibling);
	
	return child;
}

__uint ITreeNode_umount_children(ITreeNode * __this)
{
	__uint res = 0;
	ITreeNode *nd, *next=0;
	
	nd = __this->first_child;
	if(nd){
		next = ITreeNode_next(nd);
	}
	
	while(nd){
		res += ITreeNode_umount_children(nd);
		__vcall__(nd, before_umount, (nd));
		ITreeNode_umount(nd);
		__vcall__(nd, after_umount, (nd));
		res++;
		nd = next;
		if(nd){
			next = ITreeNode_next(nd);
		}
	}
	
	return res;
}

static __uint ITreeNode_ForEachProc(
									ITreeNode * __this,
									ITreeNode_callback action,
									__bool bRecursive,
									enum tr_TraverseMethod m,
									__bool *bContinue,
									__uint context
									)
{
	__uint res = 0;
	ITreeNode *nd, *next=0;
	
	nd = __this->first_child;
	if((m == ITreeNode_pre_order) || (m==ITreeNode_both_order)){
		*bContinue = action(__this, __true, context);
		res++;
	}
	
	if(nd){
		next = ITreeNode_next(nd);
	}
	
	while(nd && *bContinue){
		if(bRecursive){
			res += ITreeNode_ForEachProc(
				nd,
				action,
				bRecursive,
				m,
				bContinue,
				context
				);
		}else{
			*bContinue = action(nd, __true, context);
			res++;
		}
		nd = next;
		if(nd){
			next = ITreeNode_next(nd);
		}
	}
	
	nd = __this->first_child;
	if( ((m == ITreeNode_post_order) || (m==ITreeNode_both_order && nd)) && bContinue){
		*bContinue = action(__this, __false, context);
		res++;
	}
	
	return res;
}

__bool ITreeNode_traverse(
						  ITreeNode *__this,
						  ITreeNode_callback action,
						  __bool bRecursive,
						  enum tr_TraverseMethod m,
						  __uint context
						  )
{
	__bool bContinue = __true;
	ITreeNode_ForEachProc(__this, action, bRecursive, m, &bContinue, context);
	return bContinue;
}

ITreeNode * ITreeNode_next(ITreeNode *__this)
{
	ITreeNode * next;
	next = SAFE_CONTAINING_RECORD((__this)->sibling.Flink, ITreeNode, sibling);
	if(next == __this->uplink->first_child){
		// circular list has reached its end
		next = NULL;
	}
	return next;
}

ITreeNode * ITreeNode_previous(ITreeNode * __this)
{
	if(__this == __this->uplink->first_child){
		// circular list has reached its head
		return NULL;
	}
	return SAFE_CONTAINING_RECORD((__this)->sibling.Blink, ITreeNode, sibling);
}

ITreeNode * ITreeNode_get_child(ITreeNode * __this, int n)
{
	ITreeNode * c;
	int i;
	c = __this->first_child;
	if( !c ){
		return 0;
	}
	for(i=0; i<n; i++){
		c = ITreeNode_next(c);
		if( !c ){
			return 0;
		}
	}
	return c;
}

