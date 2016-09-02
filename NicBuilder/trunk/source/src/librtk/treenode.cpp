/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\librtk\treenode.cpp
	file path:	E:\vss\pmc2\src\librtk
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Tree node implementation	
	history:
	created:	11:6:2002   13:11
*********************************************************************/
#include "precomp.h"

/*
							  m_Parent
							     |
                            _____|_______
                            |           |  
m_PreviousSibling  ---------|    item   |--------- m_NextSibling
							|___________|
                            /           \
                           /             \
						  /               \
                   m_FirstChild .....     m_LastChild


*/

CTreeNode::CTreeNode()
{
	m_Parent = 0;
	m_PreviousSibling = 0;
	m_NextSibling = 0;
	m_FirstChild = 0;
	m_LastChild = 0;
	m_iSubItems = 0;
	m_iData = 0;
	m_Key = "";
}

CTreeNode * CTreeNode::GetFirstChild()
{
	return m_FirstChild;
}

CTreeNode * CTreeNode::GetNextChild(CTreeNode *child)
{
	return child->m_NextSibling;
}

CTreeNode * CTreeNode::GetNextSibling()
{
	return m_NextSibling;
}

/*
	insert a new child after the specified brother
	if the newcomer has children, they will be my grandchildren.
	if the newcomer has sibling, they will be disonnected from newcomer.
*/
void CTreeNode::AddChild(
	CTreeNode *brother, 
	CTreeNode *newcomer
	)
{
	newcomer->m_Parent = this;
	m_iSubItems++;

	if(brother == 0){
		// insert as the last child
		newcomer->m_NextSibling = 0;
		newcomer->m_PreviousSibling = m_LastChild;
		if(m_LastChild){
			m_LastChild->m_NextSibling = newcomer;
		}
		m_LastChild = newcomer;
		if(m_iSubItems == 1){
			m_FirstChild = newcomer;
		}
        OnAddChild(newcomer);
		return;
	}

	assert(m_iSubItems >= 2);
	if(brother == m_FirstChild){
		newcomer->m_PreviousSibling = m_FirstChild;
		newcomer->m_NextSibling = m_FirstChild->m_NextSibling;
		m_FirstChild->m_NextSibling = newcomer;
		if(newcomer->m_NextSibling){
			newcomer->m_NextSibling->m_PreviousSibling = newcomer;
		}
		if(m_iSubItems == 2){
			m_LastChild = newcomer;
		}
        OnAddChild(newcomer);
		return;
	}

	assert(m_iSubItems >= 3);
	if(brother == m_LastChild){
		newcomer->m_PreviousSibling = m_LastChild;
		newcomer->m_NextSibling = 0;
		m_LastChild->m_NextSibling = newcomer;
	}

    OnAddChild(newcomer);
}

CTreeNode * CTreeNode::GetChild(__uint index)
{
	if(index >= m_iSubItems){
		return 0;
	}
	__uint i;
	CTreeNode *p;
	p = m_FirstChild;
	for( i=0;
	     i<index;
		 i++, p=p->m_NextSibling
		);
	return p;
}

CTreeNode * CTreeNode::RemoveChild(__uint index)
{
	CTreeNode *p;
	p = GetChild(index);
	return RemoveChild(p);
}

CTreeNode* CTreeNode::RemoveChild(CTreeNode *abandoned)
{
	CTreeNode *p, *n;
	if( abandoned->m_Parent != this ){
		return 0;
	}
	p = abandoned->m_PreviousSibling;
	n = abandoned->m_NextSibling;
	if(p){
		p->m_NextSibling = n;
	}else{
        m_FirstChild = n;
    }
	if(n){
		n->m_PreviousSibling = p;
	}else{
        m_LastChild = p;
    }
	abandoned->m_PreviousSibling = 0;
	abandoned->m_NextSibling = 0;
	m_iSubItems--;
    OnDeleteChild(abandoned);
	return n;
}

__uint CTreeNode::_ForEachProc(
	CAction action,
	__bool bRecursive,
	TraverseMethod m,
	__bool &bContinue,
    __uint context
	)
{
	__uint res = 0;

	if(m == pre_order){
		bContinue = action(this, context);
		res++;
	}

	CTreeNode *nd, *next=0;
	nd = GetFirstChild();
    if(nd){
        next = nd->m_NextSibling;
    }
	while(nd && bContinue){
		if(bRecursive){
			res += nd->_ForEachProc(
                action,
                bRecursive,
                /*
                    注意递归时必须使用后序遍历，这样才能保证DeleteTree等客户
                    不出错，后序遍历时执行action(this)时可以保证this已经没有
                    孩子乐。同时因为下面的代码要使用nd->m_NextSibling，而执行
                    到该行代码时nd可能已经不可访问(action中可能已经把它删掉乐
                    )所以必须事先将m_NextSibling保存起来。
                */
                m==exclude? post_order : m,
                bContinue,
                context
                );
		}else{
			bContinue = action(nd, context);
			res++;
		}
		nd = next;
        if(nd){
            next = nd->m_NextSibling;
        }
	}

	if( (m == post_order) && bContinue){
		bContinue = action(this, context);
		res++;
	}

	return res;
}

__uint CTreeNode::ForEach(
	CAction action,
	__bool bRecursive,
	TraverseMethod m,
    __uint context
	)
{
	__bool bContinue = __true;
	return _ForEachProc(action, bRecursive, m, bContinue, context);
}

void CTreeNode::OnDeleteChild(CTreeNode *node)
{
}

static __bool _RemoveChild(CTreeNode *node, __uint)
{
    node->GetParent()->RemoveChild(node);
    return __true;
}

void CTreeNode::RemoveChildren()
{
    ForEach(_RemoveChild, __true, exclude);
}

void CTreeNode::OnAddChild(CTreeNode *)
{
}

void CTreeNode::OnUpdate()
{
}

struct CSearchContext{
    CTreeNode * Result;
    char * Key;
};

static __bool _SearchAction(CTreeNode * node, __uint context)
{
    CTreeNode *it;
    CSearchContext * ctx = (CSearchContext*)context;
    it = dynamic_cast<CTreeNode*>(node);
    assert(it);
    if(it->Key() == ctx->Key){
        ctx->Result = it;
        return false;
    }
    return true;
}

CTreeNode * CTreeNode::Search(const string & RelativeUrl)
{
    CArgs args;
    CSearchContext ctx;
    parse_arg_ex(RelativeUrl.c_str(), &args, ".");
    if(!args.argc){
        return 0;
    }
    CTreeNode *currentParent;
    currentParent = this;
    for(int i=0; i<args.argc; i++){
        ctx.Result = 0;
        ctx.Key = args.argv[i];
        currentParent->ForEach(_SearchAction, false, exclude, (__uint)&ctx);
        if(!ctx.Result){
            break;
        }
        currentParent = ctx.Result;
    }
    return ctx.Result;
}

#if 0
class CNamedNode : public CTreeNode
{
public:
	int level;
	char * name;
};

__bool visit(CTreeNode *_nd, __uint)
{
	CNamedNode *nd = (CNamedNode*)_nd;
	for(int i=0;i<nd->level;i++){
		printf(" ");
	}
	printf("%s\n", nd->name);
	return __true;
}

static void dump_tree(CNamedNode *root)
{
	/*
	static int indent;
	int i;
	for(i=0;i<indent;i++){
		printf(" ");
	}
	printf("%s\n", root->name);
	indent += strlen("├─");
	CNamedNode *c;
	CTreeNode *p;
	p=root->GetFirstChild();
	c=dynamic_cast<CNamedNode*>(p);
	while(c){
		dump_tree(c);
		c = dynamic_cast<CNamedNode*>(root->GetNextChild(c));
	}
	indent -= strlen("├─");
	*/
	root->ForEach(visit, __true);
}

extern "C" _declspec(dllexport) RTK_API TestTree(int,int,int,int)
{
	setup_console_for_log();
	CNamedNode root, *cur[32];
	int level;
	level=1;	
	ZeroMemory(cur, sizeof(cur));
	cur[0] = &root;	
	root.name = "Root";
	root.level = 0;
	char buf[256];
	while(!feof(stdin)){
		gets(buf);
		if(!buf[0]) break;
		int i;
		for(i=0; i<strlen(buf); i++){
			if(buf[i] != ' ')
				break;
		}
		i++;
		if(i>level+1){
			printf("Invalid.\n");
			continue;
		}
		CNamedNode *nd;
		nd = new CNamedNode;
		nd->name = strdup(buf + i - 1);
		nd->level = i;
		DEBUG_PRINTF((_T("Attaching %s to %s\n"), cur[i-1]->name, nd->name));
		cur[i - 1]->AddChild(nd);
		cur[i] = nd;
		level=i;
		for(i=level+1;i<32;i++){
			cur[i] = 0;
		}
	}
	printf("Dump of tree............\n");
	dump_tree(&root);
	getchar();
	return 1;
}
#endif
