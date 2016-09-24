#include "LTreeNode.h"

#include "SimpleToken.h"

#include <algorithm>
#include <iostream>

LTreeNode::LTreeNode(SimpleToken *t, LTreeNode *parent): _parent(parent), _token(t), _isNumber(false)
{
    if (t){
	_value = t->stringValue();
        if (t->isNumber()) {
            _isNumber = true;
            _number = t->numValue;
        }
        if (parent)
	    parent->addChild(t->stringValue(),this);
    } else { _value = "<nil>";}
    
}

LTreeNode::~LTreeNode() 
{
    std::list<LTreeNode *>::const_iterator itc = _children.begin();

    for (;itc!=_children.end();itc++)
	delete (*itc);
    for (itc=_names.begin();itc!=_names.end();itc++)
	delete (*itc);
        
    
    delete _token;
}

bool LTreeNode::hasAttrib(std::string attr) const
{
    std::list<std::string>::const_iterator it = 
        std::find( _attribs.begin(), _attribs.end(), attr);

    return (it != _attribs.end() );
}

bool LTreeNode::hasChild(std::string name) const
{
    std::list<LTreeNode *>::const_iterator it = _children.begin();
    for (;it!=_children.end();it++)
     //   if ( (*it)->value() ==  name  ) return true;
        if ( *(*it) ==  name  ) return true;

    return false;

    //return (it != _children.end() );
}

void LTreeNode::dump() const
{
    if (!_parent) { 
    //    std::cout<<"ROOT NODE" <<std::endl;
    }

    if (_token) {
    for (int i=0;i<_token->level();i++) std::cout << ' ' ;
    std::cout << _token->stringValue() << " " ;
    }
    
    std::list<LTreeNode *>::const_iterator itc;
    std::cout<<"[names: " ;
    for (itc = _names.begin();itc!=_names.end();itc++)
	std::cout << (*itc)->value() << " " ;
    std::cout<<"]";

    std::list<std::string>::const_iterator att_it;
    std::cout<<"[attribs: "; 
    for (att_it = _attribs.begin();att_it!=_attribs.end();att_it++)
	std::cout << (*att_it) << " ";
    std::cout<<"]";

    std::cout << std::endl;
    for (itc=_children.begin();itc!=_children.end();itc++)
	(*itc)->dump();

    if (_token) {
    for (int i=0;i<_token->level();i++) std::cout << ' ' ;
    std::cout << "CCCCC" << _token->stringValue() << std::endl;
    }
}

bool LTreeNode::operator==(std::string &str)
{
    return _value == str;
}
