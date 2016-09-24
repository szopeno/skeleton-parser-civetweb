#ifndef CONTAINER_STATEMENT_H
#define CONTAINER_STATEMENT_H

#include <map>
#include <string>
#include <list>

#include "Statement.h"

class ContainerStatement : public Statement {
    private:
        std::map<std::string,Statement *> _statements;
        std::list<std::string> _required;
    public:
        ContainerStatement();
       ~ContainerStatement(); 

        bool addStatement(std::string, Statement *, bool required=false);
        virtual bool open(LTreeNode *);
        virtual bool check(LTreeNode *);
        virtual bool checkRequired(LTreeNode *);
        virtual bool checkThis(LTreeNode *) { return true;}
};
#endif
