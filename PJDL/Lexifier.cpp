#include "Lexifier.h"
#include "SimpleToken.h"

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <cstdio>

#include <string>
#include <string.h> // for strdup, temporary


using namespace std;

const char * const Lexifier::nofname = "<stdin>";

Lexifier::Lexifier(const char *src, const char *const file) : source(src), fileLine(1), filePos(1),level(1),indent(1),pIndent(1), firstInLine(true), finished(false), specialLevel(false),specialFirst(false), subLex(0), fname(file)
{
    lpt=fpt=(unsigned const char *) source;
}

Lexifier::~Lexifier()
{
    //delete source;
    if (subLex) delete subLex;
}


SimpleToken *Lexifier::getToken()
{
    sublex:

    if ( subLex ) {
	SimpleToken *t = subLex->getToken();
	if (t==0) {
	    subLex->finished = true;
            delete subLex;
            subLex = 0; 
	} else {
	    return t;
	}
    }

    if (!fpt) { 
        finished=true;
        return 0; // throw error
    }

    char c;
    int i=0;
    int flag=0;
    bool dot, quote;
    firstInLine=false;//needed when skipping whitespaces 

    void *Lptr=0;
    void *Eptr;

    restart:
    i=0;

    if (fpt==lpt) {
        firstInLine = true;
	skip_spaces_count_tabs();
    } else {
        firstInLine = false;
	skip_spaces();
    }
    
    filePos = (int)(fpt-(unsigned const char *)source);
    indent = fpt-lpt+tabs*(TABSIZE-1)+1;

    
    
    unsigned const char *opt = fpt;
    word[i]=0;
    dot = false;

    switch (*fpt) {
        case '#': // single line comment
		while ((*fpt) && ((*fpt)!='\n'))
		{
		    fpt++;
		}
		goto restart;
        case '"' :
            fpt++; 
            Lstringify:
            quote=false;
	    while ((*fpt) && (((*fpt)!='"') || ( quote && *fpt=='"')))
            {
                if (quote)  {
                    if (*fpt=='n') { 
                        word[i++] = '\n';
                    } else if (*fpt=='t') {
                        word[i++] = '\t';
                    } else if (*fpt=='"') {
                        word[i++] = '\"';
                    } else if (*fpt=='\\') {
                        word[i++] = '\\';
                    } else word[i++]=*fpt;
                    quote=false;
                }
                else {
		    if (*fpt=='\\') { quote = true; }
		    else { 
                        quote=false;
			word[i++]=*fpt;
                    }
                }
		fpt++;
            }
	    fpt++;
	    word[i]=0;
            return rToken(SimpleToken::createString(word));
        break;

        case '\\'://ignored
            fpt++;
            skip_spaces();
            if ((*fpt)=='\n') {
		fileLine++; 
		fpt++; 
            }
	    goto restart;
        break;
        case '\n':
	    lpt = fpt+1; 
            fileLine++; 
            fpt++; 
            specialLevel = false; specialFirst=false;
            goto restart;
        break;
        case '0'...'9':
            dot=false;
            Lnumber: 
	    while ((*fpt)&& ((*fpt)>='0') && ((*fpt)<='9')
                  )
            {
                word[i++]=*fpt;
		fpt++;
            }
            if ( *fpt == '.') { 
                if (!dot) { dot=true; word[i++]=*fpt; fpt++; goto Lnumber;
                } else {
                    word[i]=*fpt;
                    word[i+1]=0;
                    cout << "ERROR: in file " << source << " at line " << fileLine <<":"<<filePos<< ": I thought I spot a number, but it has two dots! Analysed string is \""<<word<<"\""<<endl;
                    exit(-1); 
                }//TODO emit error 
            }
            word[i]=0;
            return rToken(SimpleToken::createNumber(word,!dot));
        break;

        case '@':
            fpt++;dot=false;
            Lidentifier_end: 
            while ( 
                    (*fpt) &&
                    (((*fpt>='a') && (*fpt<='z')) ||
                    ((*fpt>='A') && (*fpt<='Z')) ||
                    ((*fpt>='0') && (*fpt<='9')) ||
                    ((*fpt=='_') || (*fpt=='@'))) 
                  ) 
            {
                word[i++]=*fpt;
		fpt++;
                dot = true;
            }
            /*if ((*fpt==':') && (*(fpt+1)=='/') && (*(fpt+2)=='/')) {
                word[i++]=':';word[i++]='/';word[i++]='/'
            }*/
            /*if ((*fpt==':') && (*(fpt+1)!='/') || (*(fpt+2)!='/')) {
               word[i++]=0; fpt++;
	       return rToken(SimpleToken::createLabel(word));
            } */
            word[i]=0;
            /*if (!dot) {
		cout << "ERROR: in file " << source << " at line " << fileLine <<":"<<filePos<< ": I thought I saw <end keyword> construct, but no keyword follows '@'! Analysed string is \""<<word<<"\""<<endl;
            }*/
            return rToken(SimpleToken::createString(word));
        break;

        case ':': // :// means URL, 
            if (( *(fpt+1) == '/') && ( *(fpt+2) == '/')) {
                fpt+=3;
		return rToken(SimpleToken::createOper('u')); // URL
            }
            fpt++;
            return rToken(SimpleToken::createOper(':'));
        break;

        case '$':
            fpt++;
            while ( 
                    (*fpt) &&
                    (
                    ((*fpt>='a') && (*fpt<='z')) ||
                    ((*fpt>='A') && (*fpt<='Z')) ||
                    ((*fpt>='0') && (*fpt<='9')) ||
                    ((*fpt=='_') || (*fpt=='.'))
                    ) 
                  ) 
            {
                word[i++]=*fpt;
		fpt++;
            }
            word[i] = 0;
            return rToken(SimpleToken::createVar(word));
	    

        case '.':
            dot=true;fpt++;
        case '_':
        case 'a'...'z':
        case 'A'...'Z': //# jumps here if not firstInLine
            Lidentifier: 
            while ( 
                    (*fpt) &&
                    (
                    ((*fpt>='a') && (*fpt<='z')) ||
                    ((*fpt>='A') && (*fpt<='Z')) ||
                    ((*fpt>='0') && (*fpt<='9')) ||
                    ((*fpt=='_') || (*fpt=='.'))
                    ) 
                  ) 
            {
                word[i++]=*fpt;
		fpt++;
            }
	    
	    if (*fpt>(char)127) {
	        Lptr = &&Lidentifier; 
	        Eptr = &&Leid;
	        goto Lutf;
	    } 
            
            Leid:
            word[i]=0;
            if (dot) return rToken(SimpleToken::createAttrib(word));
            return rToken(SimpleToken::createKey(word));
        case ',' : 
            fpt++;
            specialFirst = true;
            goto restart;
        break;

        case '/' : 
        case '+' : 
        case '*' : 
        case '~' : //IN
            /* comparisons see below */
	    word[0]=*fpt;
	    word[1]=0;
	    fpt++;
	    return rToken(SimpleToken::createOper(word[0]));
        break; 

        case '-' : 
	    word[0]=*fpt;
	    word[1]=0;
	    fpt++;
            if ( *fpt == '>' ) {
		return rToken(SimpleToken::createOper('S')); // ->
            }
	    return rToken(SimpleToken::createOper(word[0]));
        break;

        case '=' : 
            fpt++; specialLevel = true; specialFirst=true;goto restart;
        break;

        case '<' : 
        case '>' : 
        //case '=' : 
            /* = handled separately due to special case as in name = value */
	    word[0]=*fpt;
	    word[1]=0;
	    fpt++;
            if ( *fpt == '=' ) {
                switch (word[0]) {
                    case '<' :
			return rToken(SimpleToken::createOper('G')); //Greater or equal
                    case '>' :
			return rToken(SimpleToken::createOper('L')); //Lesser or equal
                    case '=' :
			return rToken(SimpleToken::createOper('E')); //equal
                }
            } else if ( (*fpt == '-') && (word[0] == '<')) {
		return rToken(SimpleToken::createOper('R')); // <-
            }
            
	    return rToken(SimpleToken::createOper(word[0]));// '=' means assign
        break;

        default:
            ;
    }
    
    finished = true;
    return 0;

    Lutf:
    /* Polish UTF characters */
    opt=fpt;
    fpt++;
	word[i++]=*opt;
	word[i++]=*fpt;
	word[i]=0;

    goto *Lptr; 

    switch (*opt) {
	case 0xc3:
	    switch ( *(fpt) ) {
		case 0x93 :
		    word[i++]=',';
		    word[i++]='O';
		break;
		case 0xb3 :
		    word[i++]=',';
		    word[i++]='o';
		break;
	    }
	break;

	case 0xc4:
	    switch ( *(fpt+1) ) {
		case 0x81 :
		    word[i++]=',';
		    word[i++]='L';
		break;
		case 0x83 :
		    word[i++]=',';
		    word[i++]='N';
		break;
		case 0x84 :
		    word[i++]=',';
		    word[i++]='A';
		break;
		case 0x85 :
		    word[i++]=',';
		    word[i++]='a';
		break;
		case 0x86 :
		    word[i++]=',';
		    word[i++]='C';
		break;
		case 0x87 :
		    word[i++]=',';
		    word[i++]='c';
		break;
		case 0x98 :
		    word[i++]=',';
		    word[i++]='E';
		break;		
		case 0x99 :
		    word[i++]=',';
		    word[i++]='e';
		break;		
	    }
	break;

	case 0xc5:
	    switch ( *(fpt+1) ) {
		case 0x82 :
		    word[i++]=',';
		    word[i++]='l';
		break;
		case 0x84 :
		    word[i++]=',';
		    word[i++]='n';
		break;
		case 0x9a :
		    word[i++]=',';
		    word[i++]='S';
		break;
		case 0x9b :
		    word[i++]=',';
		    word[i++]='s';
		break;
		case 0xb9 :
		    word[i++]=',';
		    word[i++]='Z';
		break;
		case 0xba :
		    word[i++]=',';
		    word[i++]='z';
		break;
		case 0xbb :
		    word[i++]='.';
		    word[i++]='Z';
		break;
		case 0xbc :
		    word[i++]='.';
		    word[i++]='z';
		break;
	    }
	break;
	}
    fpt++;
    goto *Lptr;
}

SimpleToken *Lexifier::rToken(SimpleToken *_token) //const
{
    if (firstInLine) {
        int prevI;
	//std::cout << "PIND:IND" << pIndent << ":" << indent << std::endl;
        if (indent > pIndent) {
	    level++;
	    levels.push(pIndent);
	    pIndent = indent;
        } else while (indent < pIndent) {
            prevI = levels.top();
            if (indent > prevI) break;
            pIndent = prevI;
            levels.pop();
            level--;
        }
    }
    
    if (specialFirst) { 
        specialFirst=false;
	_token->first = true;
    } else
	_token->first = firstInLine;
    _token->indent = indent;
    _token->startLine = lpt;
    _token->filePos = fpt-(unsigned char const *)source;
    _token->fileLine = fileLine;
    if (specialLevel)
	_token->_level = level+1;
    else
	_token->_level = level;
    //_token->basicTokenType = tokenType;
    if (fname) _token->source = fname;
    else _token->source = Lexifier::nofname;

    return _token;
}

/*
void Lexifier::directive( std::string dir, std::string arg, std::string arg2)
{
    if ( dir == "include" ) {
        subLex = new Lexifier(arg.c_str());
    } else if ( dir == "define" ) {
        defines[arg] = arg2;
    }
}
*/
