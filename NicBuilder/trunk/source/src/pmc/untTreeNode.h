//---------------------------------------------------------------------------

#ifndef untTreeNodeH
#define untTreeNodeH
//---------------------------------------------------------------------------

#include <rtkdefs.h>

class CTreeNode
{
public:
	__uint	m_iData;
public:
	CTreeNode();
	virtual ~CTreeNode(){};

	__uint		GetItems()
	{
		return m_iSubItems;
	}
	CTreeNode * GetParent()
	{
		return m_Parent;
	}
	CTreeNode * GetFirstChild();
	CTreeNode * GetNextChild(CTreeNode *);
	CTreeNode * GetNextSibling();
	CTreeNode * GetChild(__uint index);
	void AddChild(
		CTreeNode *brother,
		CTreeNode *newcomer
		);
	void AddChild(CTreeNode *newcomer)
	{
		return AddChild(0, newcomer);
	}
	CTreeNode * RemoveChild(__uint index);
	CTreeNode * RemoveChild(CTreeNode *brother);
	void RemoveChildren();

	typedef __bool (_cdecl *CAction)(CTreeNode *nd, __uint context);
	enum TraverseMethod{
		pre_order = 0,
		post_order = 1,
		exclude = 2,
	};
	__uint ForEach(
		CAction action,
		__bool bRecursive=__false,
		TraverseMethod m=pre_order,
		__uint context = 0
		);

	virtual void OnAddChild(CTreeNode * NewChild);
	virtual void OnDeleteChild(CTreeNode *node);
	virtual void OnUpdate();
protected:
	CTreeNode * m_Parent;
	CTreeNode * m_FirstChild, *m_LastChild;
	CTreeNode * m_PreviousSibling, * m_NextSibling;
	__uint	m_iSubItems;
private:
	CTreeNode & operator =(const CTreeNode & other);

private:
	__uint _ForEachProc(
		CAction action,
		__bool bRecursive,
		TraverseMethod m,
		__bool &bContinue,
		__uint context
		);
};

#endif
