#ifndef SIMPLE_TOKEN_H
#define SIMPLE_TOKEN_H

#define MAX_LITERAL_SIZE 2048

#include <string>

struct SimpleToken {
    enum SimpleToken_t { T_KEY, T_STRING, T_OPER, T_NUMBER, T_ATTRIB, T_VAR, T_EOF };

    enum SimpleToken_t basicTokenType;
   
    int fileLine, filePos;
    bool first;
    unsigned const char * startLine;
    int indent;
    int _level;
    const char *source;
    char value[MAX_LITERAL_SIZE];
    double numValue;
    bool isInt;

    SimpleToken();
    SimpleToken(char const []);

    static SimpleToken *createString(char const []);
    static SimpleToken *createKey(char const []);
    static SimpleToken *createAttrib(char const []);
    static SimpleToken *createNumber(char const [],bool);
    static SimpleToken *createOper(const char);
    static SimpleToken *createVar(const char []);
    static SimpleToken *createEOF();
    inline SimpleToken *set(SimpleToken_t t) {basicTokenType = t; return this;}
    bool isString() const { return basicTokenType == T_STRING;}
    bool isNumber() const { return basicTokenType == T_NUMBER;}
    bool isOper() const { return basicTokenType == T_OPER;}
    bool isVar() const { return basicTokenType == T_VAR;}
    bool isAttrib() const { return basicTokenType == T_ATTRIB;}
    bool isEOF() const { return basicTokenType == T_EOF;}
    bool isFirst() const { return first;}
    bool isKey() const { return basicTokenType == T_KEY;}
    char oper() const { return value[0];}

    std::string toString() const;
    std::string fieldsStr() const;
    std::string stringValue() const;
    void setString(std::string);
    void setNumber(double);
    int level() const { return _level;}
    friend class Lexifier;
};

#endif
