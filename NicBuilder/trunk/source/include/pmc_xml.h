#ifndef __pmcxml_h__
#define __pmcxml_h__

#ifndef LIBPMCXML_EXPORTS
#define LIBPMCXML_API __declspec(dllimport)
#pragma comment(lib, "libpmcxml.lib")
#else
#define LIBPMCXML_API __declspec(dllexport)
#endif

#include <librtk.h>
#include <private/object.h>

enum SYM_TYPE{
	sym_xml,
	sym_id,
	sym_item,
	sym_leaf,
	sym_attribute,
	sym_branch,
	sym_string,
};

/*++
	node objects representing a syntatic unit
--*/
class LIBPMCXML_EXPORTS CXmlSymbol : public CTreeNode{
public:
public:
	int type;
	CXmlSymbol(int t);
	~CXmlSymbol();
	// overload of CTreeNode::AddChild
	void AddChild(CXmlSymbol * child);
	CXmlSymbol * AddSibling(CXmlSymbol * sib);
	CXmlSymbol * sibling;
	char * text;
	int line;	
};

CXmlSymbol * xml_load(const char * file);


#endif
