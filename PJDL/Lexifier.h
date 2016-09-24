#ifndef LEXIFIER_H
#define LEXIFIER_H

#define MAX_WORD_SIZE 256
#define TABSIZE 8

#include <string>
#include <map>
#include <stack>
//#include "SimpleToken.h"

class Lexifier {
    const char * const source; // start of the source
    unsigned const char *fpt; // current character pointer
    unsigned const char *lpt; // start of the line pointer
    char word[MAX_WORD_SIZE]; // current word created
    int fileLine; // number of line in source
    int filePos; // number of characters read so far
    std::stack<int> levels;
    int level; // current indentation level (not exact number!)
    int indent; // exact indent number (number of whitespaces since beginning of line)
    int pIndent; // exact indent number (number of whitespaces since beginning of line)
    bool firstInLine;
    bool finished; 
    bool specialLevel;
    bool specialFirst;
    const char *const fname;
    Lexifier *subLex;
    int tabs;
    
    class SimpleToken *rToken(SimpleToken *);// const;

    inline bool isWhiteSpace(char c) {
        return c==' ' || (c=='\n') || (c=='\t');
    }

    inline void skipTillNewline() {
	skip:
	while ((*fpt) && ((*fpt)!='\n'))
	{
	    fpt++;
	}
    }

    inline void skip_spaces_count_tabs() {
        tabs=0;
	while ( (*fpt) &&
                ((*fpt)==' ' || (*fpt)=='\t' /*|| (*fpt)=='\n' */ )) // \n w komentarzu czy nie?        
        {
            if ( *fpt == '\t' ) tabs++;
	    fpt++;
        }
    }

    inline void skip_spaces() {
        skip:
	while ( (*fpt) &&
                ((*fpt)==' ' || (*fpt)=='\t' /*|| (*fpt)=='\n' */ )) // \n w komentarzu czy nie?
	    fpt++;
    }
    public:
    	Lexifier(const char * const src,const char *const file=0);
    	~Lexifier();

        static const char * const nofname;
        class SimpleToken *getToken(); 

        inline int getIndent() const { return indent;}
        inline int getFileLine() const { return fileLine;}
        inline int getLinePos() const { return fpt-lpt; }
        inline int getFilePos() const { 
	    return filePos;
        }
};

#endif
