#include "PVCPparse.h"
#include "SimpleToken.h"
#include "Lexifier.h"
#include "LTreeNode.h"
#include "JobStatement.h"

#include "SyntaxError.h"

#include <stdio.h>
#include <iostream>
#include <stack>

void PVCP_parse(char *infile)
{
    printf("OK!\n");
    Lexifier *lex = new Lexifier(infile);
       
    int plevel = 0;
    bool hasName=false;
    bool isUrl=false;
    SimpleToken *t;
    LTreeNode *node = new LTreeNode(0,0);
    LTreeNode *pnode,*tnode = 0;
    std::stack<LTreeNode *> nodes;
    nodes.push(node);

    pnode=tnode=node;
    while (t = lex->getToken()){
        
        if (t->isFirst() ) {
            //std::cout<< "PREV" << t->stringValue() << ":" << t->level() << ":" << plevel << std::endl;
            if (!t->isKey()) {
                //throw error
                ;//std::cout << "NOT A KEY!" << std::endl;
            }
            hasName=false;
            if ( t->level() == plevel ) {
		pnode = nodes.top();
		tnode = new LTreeNode(t,pnode);
            } else if ( t->level() > plevel ) {
                plevel = t->level();
                nodes.push(pnode);
		tnode = new LTreeNode(t,pnode);
                pnode = tnode;
            } else {
		while (t->level() < plevel) {
		    plevel--;
		    pnode=nodes.top();
		    nodes.pop();
		}
		tnode = new LTreeNode(t,pnode);
                pnode = tnode;
            }
            std::cout<< t->stringValue() << ":" << t->level() << ":" << plevel << std::endl;
        } else {
             
            if ( t->isAttrib() ) {
              tnode->addAttrib( t->stringValue() ); 
            } else {
	    if (t->isKey()) {
                ; //throw error
		//    std::cout << "A KEY! Maybe you forgot \"?" << std::endl;
	    }
              if (hasName) ; //throw error
              tnode->addName( new LTreeNode(t) );
              hasName=true;
            }
        }

        //std::cerr << t->toString() << std::string(" ");
        //std::cerr << t->fieldsStr() << std::endl;
    }; 
    JobStatement *jobStatement = new JobStatement;

    std::list<LTreeNode*> jobNodes = node->children();
    std::list<LTreeNode*>::iterator jit =  jobNodes.begin();
    LTreeNode *jobNode;
    try {
	for (;jit!=jobNodes.end();jit++) {
	    jobStatement->check( *jit );
        }

    } catch (SyntaxError e){
        std::cerr << "Syntax error: file " << e._source << "\", line " << e._line << "(character in position "<< e._pos << ", indent " << e._indent << ").\n\t Offending token is of value shown in line below:\n\n\t\t" << e._word << "\n\n\tI expected " << e._expected << ".\n" << std::endl;
        const int MAX_LINE_LENGTH = 80;
        unsigned char currentLine[MAX_LINE_LENGTH]={0};
        unsigned char const *lpt = e._currLine;
        int i;
        for (i=0;i<MAX_LINE_LENGTH-4;i++){
            currentLine[i] = *(lpt+i);
            if (currentLine[i]=='\n') {
                currentLine[i]= 0;
                break;
            }
        }
        if (i==MAX_LINE_LENGTH-4) {
            currentLine[i]='.';
            currentLine[i+1]='.';
            currentLine[i+2]='.';
        }
        std::cerr << "The whole line is:" << std::endl;
        std::cerr << "-------------------------------------------------------------\n";
        std::cerr << currentLine << "\n";
        std::cerr << "-------------------------------------------------------------\n";
        std::cerr << std::endl;
        return;
    }
    node->dump();
    printf("DONE!\n");

    delete node;
    delete lex;
}
