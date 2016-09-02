#ifndef __treelist_h__
#define __treelist_h__

#include <c_object.h>
#include <rtk_list_entry.h>

CDECL_BEGIN

__class__(ITreeNode)
	struct __tag_ITreeNode * uplink;
	struct __tag_ITreeNode * first_child;
	RTK_LIST_ENTRY	sibling;
__class_end__(ITreeNode)

/*
	this interface manages a child/sibling list to maintain a tree view.
*/
__vtable__(ITreeNode)
	// added __newcomer to a existing tree
	__vmethod__(void, on_mount, (ITreeNode * __newcomer));
	// about to delete __abandoned from containing tree
	__vmethod__(void, before_umount, (ITreeNode * __abandoned));
	__vmethod__(void, after_umount, (ITreeNode * __abandoned));
__vtable_end__(ITreeNode)

/*
	if mount_as_child_or_sibling is true, mount as child
	else mount as sibling
	fails if:
	1) mount as sibling but peer has no parent, since allowing this will
	incur two or more roots.
	2) mount as child but the newcomer has already a parent(you must umount
	it from its previous parent first), i.e., only root can be mounted
*/
__method__(ITreeNode, __bool, mount,
	(ITreeNode * peer, ITreeNode * newcomer, __bool child_or_sibling)
	);

/*
	ITreeNode_umount(ITreeNode * obj)
	detach a branch from its belonging tree,
	Note : the before_umount and after_umount will not get called
	since they are the notifying routines, and we believe the caller 
	is fully aware of the deleted node thus no notification is needed.
*/
__method__(ITreeNode, ITreeNode * , umount, (ITreeNode * obj));

/*
	ITreeNode_umount_children(ITreeNode * root)
	umount all children of obj, RECURSIVELY
	the before_umount and after_umount will get called on each
	child and grandchild, etc.
*/ 
__method__(ITreeNode, __uint, umount_children, (ITreeNode * obj));

#define ITreeNode_cut_branch(obj)\
do{\
	ITreeNode_umount_children(obj);\
	__vcall__(obj, before_umount, (obj));\
	ITreeNode_umount(obj);\
	__vcall__(obj, after_umount, (obj));\
}while(0)

enum tr_TraverseMethod{
	ITreeNode_pre_order = 0,
	ITreeNode_post_order = 1,
	ITreeNode_exclude = 2,
	ITreeNode_both_order = 3,
};

typedef __bool (*ITreeNode_callback)(ITreeNode *nd, __bool firstVisit, __uint context);

__method__(ITreeNode, 
	__bool, 
	traverse,
	(ITreeNode *__this,
	ITreeNode_callback action,
	__bool bRecursive,
	enum tr_TraverseMethod m,
	__uint context
	));

/*
	ITreeNode_next
	returns next sibling in list, or NULL if reaching end.
*/
__method__(ITreeNode, ITreeNode *, next, (ITreeNode *__this));

/*
	ITreeNode_previous
	returns previous sibling in list, or NULL if already head.
*/
__method__(ITreeNode, ITreeNode *, previous, (ITreeNode * __this));

/*
	get NO.n child, or 0 if not that many children
*/
__method__(ITreeNode, ITreeNode *, get_child, (ITreeNode * __this, int n));

CDECL_END

#endif
