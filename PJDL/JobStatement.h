#ifndef JOB_STATEMENT_H
#define JOB_STATEMENT_H

#include "ContainerStatement.h"

class JobStatement : public ContainerStatement {
        class ShortStatement *_shortStatement;
    public:
        JobStatement();
        ~JobStatement();

        bool open(class LTreeNode *);
};
#endif
