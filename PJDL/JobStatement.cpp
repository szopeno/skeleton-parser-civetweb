#include "JobStatement.h"
#include "LTreeNode.h"
#include "ShortStatement.h"

JobStatement::JobStatement() : ContainerStatement()
{
    _shortStatement = new ShortStatement(this);
    oneName(true);
    addStatement("short", _shortStatement, true );
}

JobStatement::~JobStatement() 
{
}

bool JobStatement::open(LTreeNode *node)
{
    if (!ContainerStatement::open(node)) return false;
    return true;
}

