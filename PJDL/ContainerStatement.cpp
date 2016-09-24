#include "ContainerStatement.h"
#include "LTreeNode.h"

#include <exception>
#include <iostream>
#include <algorithm>
#include "SyntaxError.h"

ContainerStatement::ContainerStatement() : Statement()
{
    _chNo = true;
}

ContainerStatement::~ContainerStatement() 
{
    std::map<std::string, Statement *>::iterator it;

    for (it=_statements.begin();it!=_statements.end();it++) {
        delete (*it).second;//??
    }
    _statements.clear(); //?
}

bool ContainerStatement::addStatement(std::string str, Statement *st,bool required)
{
    if ( _statements.find( str ) != _statements.end() ) {
        return false;
    }
    _statements[str] = st;
    if (required)
	_required.push_back(str);
    return true;
}


bool ContainerStatement::open(LTreeNode *node) 
{
    std::map<std::string, Statement *>::iterator it;

    for (it=_statements.begin();it!=_statements.end();it++) {
        (*it).second->open(node);
    }
}

bool ContainerStatement::checkRequired(LTreeNode *node) //throw
{
    std::list<std::string>::iterator rit = _required.begin();
    std::list<LTreeNode *> cnodes = node->children();
    std::list<LTreeNode *>::iterator cit = cnodes.begin();

    for (;rit!=_required.end();rit++) {
	for (cit=cnodes.begin();cit!=cnodes.end();cit++) {
            if ( (*cit)->token()->stringValue() == (*rit) ) break;
        }

	/*auto cit = std::find_if(cnodes.begin(),cnodes.end(), [rit] (LTreeNode *node) -> bool {
	    if (node->value() == (*rit)) return true; return false;
	} ); */
        if (cit == cnodes.end() ) {
	    throw SyntaxError( node->token(), std::string("No required \"") + (*rit)+"\" children node found");
        }
    }
    return true;
}

bool ContainerStatement::check(LTreeNode *node) 
{

    if (!Statement::check(node)) return false;
    if (!checkRequired(node)) return false;
    if (!checkThis(node)) return false;
    //std::cout<<"kurwa"<< node->value() << std::endl;

    std::list<LTreeNode *> cnodes = node->children();
    //std::list<LTreeNode *>::iterator cit = cnodes.begin();
    auto cit = cnodes.begin();
    std::map<std::string, Statement *>::iterator it,tit;
    std::list<LTreeNode *>::iterator nit;
    for (it=_statements.begin();it!=_statements.end();it++) 
        (*it).second->reset();

    for (;cit!=cnodes.end();cit++) {
        it = _statements.find( (*cit)->value());
	//std::cout<<"cit "<< (*cit)->value() << std::endl;
        if (it == _statements.end() ) {
	    std::cout<<"throw "<< std::endl;
	    std::string stre;
	    if (_statements.size() == 0 ) stre = "no children nodes";
	    else { 
		stre = "one of [ ";
		for (tit = _statements.begin(); tit!=_statements.end();tit++)
		    stre += (*tit).first + " ";
		stre += " ]";
	    }

            throw SyntaxError( (*cit)->token(), stre);
            //return false; //throw error
        }

	//std::cout<<"check "<< (*it).first << std::endl;
	(*it).second->check( (*cit) );

	//std::cout<<"next "<< std::endl;
    }
    return true;
}
