#ifndef LTREENODE_H
#define LTREENODE_H

#include <string>
#include <list>

#include "SimpleToken.h"

/*
    Each line describes a single node and should have the following format:
        
        keyword NAME .ATTRIB [.ATTRIB2 ... ]
       
    A node can have many children, many names and many attribs
    Each node except the "document" node must have a parent
*/
    
class LTreeNode {
   private:
        LTreeNode *_parent;
        SimpleToken *_token;
        std::string _value;
        float _number;
        bool _isNumber;
        std::list<LTreeNode *> _names;
        std::list<LTreeNode *> _children; /* 
            children of the node */
        std::list<std::string > _attribs; // this node arguments
   public: 
        LTreeNode(SimpleToken *t=0, LTreeNode *parent=0);
        ~LTreeNode();

        inline std::string value() const { return _value;}
        inline float number() const { return _number;}
        inline bool isNumber() const { return _isNumber;}
        void setNumber(float x) { _isNumber = true; _number = x;}
        inline LTreeNode *parent() const { return _parent;}
        inline void parent(LTreeNode *p) { _parent = p;}
        //bool hasName(std::string s) const { return s == _name;}
        bool hasAttrib(std::string) const;
        bool hasChild(std::string) const;
        //inline std::string name() const { return _name;} 
        //inline void name(std::string s) { _name = s;} 

        inline std::list<LTreeNode *> &children() { return _children;}
        inline std::list<std::string > &attribs() { return _attribs;}
        inline std::list<LTreeNode *> &names() { return _names;}
        /*std::list<LTreeNode *> &children(std::string s)  { 
            return _children[ s ];
        }*/

        void addChild(std::string s, LTreeNode *node) { _children.push_back(node);}
        void addName(LTreeNode *node) { _names.push_back(node);}
        void addAttrib(std::string s) { _attribs.push_back(s);}
        inline std::list<std::string> &getAttribs() { return _attribs;}
        class SimpleToken *token() const { return _token;}

        void dump() const;
        bool operator== (std::string &);
};

#endif
