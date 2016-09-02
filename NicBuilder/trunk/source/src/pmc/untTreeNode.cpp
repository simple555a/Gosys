#include "precomp.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CTreeNode::CTreeNode()
{
	m_Parent = 0;
	m_PreviousSibling = 0;
	m_NextSibling = 0;
	m_FirstChild = 0;
	m_LastChild = 0;
	m_iSubItems = 0;
	m_iData = 0;
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

void CTreeNode::OnAddChild(CTreeNode * NewChild)
{
}

void CTreeNode::OnUpdate()
{
}

