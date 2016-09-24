#include "SimpleToken.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

SimpleToken::SimpleToken() : indent(-1), _level(0), isInt(false)
{
    value[0] = 0;
    numValue = 0;
}

//const char *const str ?
SimpleToken::SimpleToken(const char str[])
{
    int i;
    //long *dst = (long *)value,*src = (long *) str;
    for (i=0;str[i];i++) 
        value[i]=str[i];
    value[i]=0; 
    /*
    for (i=0;i<(MAX_LITERAL_SIZE/(sizeof(long)));i++) 
    { 
        dst[i]=src[i]; 
    } 
    dst[i]=0;
    */

    numValue = 0;
}

SimpleToken *SimpleToken::createKey(char const val[])
{
    SimpleToken *t = new SimpleToken(val);
    t->basicTokenType = T_KEY;
    return t;
}

SimpleToken *SimpleToken::createString(char const val[])
{
    SimpleToken *t = new SimpleToken(val);
    t->basicTokenType = T_STRING;
    return t;
}

SimpleToken *SimpleToken::createAttrib(char const val[])
{
    SimpleToken *t = new SimpleToken(val);
    t->basicTokenType = T_ATTRIB;
    return t;
}

SimpleToken *SimpleToken::createVar(char const val[])
{
    SimpleToken *t = new SimpleToken(val);
    t->basicTokenType = T_VAR;
    return t;
}

SimpleToken *SimpleToken::createNumber(char const val[], bool isInt)
{
    SimpleToken *t = new SimpleToken();
    t->basicTokenType = T_NUMBER;
    t->numValue = strtold(val,0);
    t->isInt = isInt;
    snprintf(t->value, MAX_LITERAL_SIZE-1, "%.8lf", t->numValue);
    return t;
}

SimpleToken *SimpleToken::createOper(const char val)
{
    SimpleToken *t = new SimpleToken();
    t->value[0] = val;
    t->value[1] = 0;
    t->basicTokenType = T_OPER;
    return t;
}



SimpleToken *SimpleToken::createEOF()
{
    SimpleToken *t = new SimpleToken();
    t->value[0] = 4;
    t->value[1] = 0;
    t->basicTokenType = T_EOF;
    return t;
}

std::string SimpleToken::toString() const
{
    switch ( basicTokenType ) {
        case T_OPER:
            return std::string("operator<")+std::string(value)+std::string(">");
        case T_KEY:
            return std::string("keyword<")+std::string(value)+std::string(">");
        case T_STRING:
            return std::string("string<")+std::string(value)+std::string(">");
        case T_NUMBER:
            return std::string("number<")+std::string(value)+std::string(">");
        case T_ATTRIB:
            return std::string("attrib<")+std::string(value)+std::string(">");
        case T_VAR:
            return std::string("var<")+std::string(value)+std::string(">");
        case T_EOF:
            return std::string("eof<")+std::string(value)+std::string(">");
    }
    return std::string("unknown<")+std::string(value)+std::string(">");
}

std::string SimpleToken::stringValue() const 
{
    return std::string(value);
}

void SimpleToken::setString( std::string val)
{
    strncpy( value, val.c_str(), MAX_LITERAL_SIZE);
}

void SimpleToken::setNumber(double n)
{
    numValue = n;
    snprintf(value, MAX_LITERAL_SIZE-1, "%.8lf", numValue);
}

std::string SimpleToken::fieldsStr() const
{
    char tmpi[32], tmpl[32], tmpf[32],tmpp[32];
    snprintf(tmpi,32,"%d", indent);
    snprintf(tmpl,32,"%d", _level);
    snprintf(tmpf,32,"%d", fileLine);
    snprintf(tmpp,32,"%d", filePos);
    return std::string("[ ") + std::string(source) + std::string(" ") + \
            std::string( tmpf) + std::string( ":" ) + std::string( tmpi ) + \
            std::string( " (") + std::string( tmpp ) + std::string(") ") + \
            std::string( "" ) + std::string( first?"true ":"false ") + \
            std::string( "L: ") + std::string( tmpl ) + std::string(" ]");
}
