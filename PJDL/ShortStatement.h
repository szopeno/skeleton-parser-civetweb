#ifndef SHORT_STATEMENT_H
#define SHORT_STATEMENT_H

#include "ContainerStatement.h"

class ShortStatement : public ContainerStatement {
        class JobStatement *_parent;
    public:
        ShortStatement(class JobStatement *p);
        ~ShortStatement();
       
        virtual bool open(LTreeNode *); 
        virtual bool check(LTreeNode *);
};
#endif
