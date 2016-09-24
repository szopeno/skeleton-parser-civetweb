#include "ShortStatement.h"
#include "LTreeNode.h"
#include "SyntaxError.h"
#include <stdio.h>
#include <iostream>

ShortStatement::ShortStatement(JobStatement *p) : ContainerStatement(), _parent(p)
{
    oneName(true);
    once(true);
}

ShortStatement::~ShortStatement()
{
}

bool ShortStatement::open(LTreeNode *)
{
    return true;
}

bool ShortStatement::check(LTreeNode *node)
{
    //std::cout << "SHORT " << std::endl;
    if (!ContainerStatement::check(node)) return false;

    return true;
}
