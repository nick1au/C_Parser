//============================================================================
//
//% Student Name 1: Nicholas Lau
//% Student 1 #: 301290893
//% Student 1 userid (email): nla52 (nla52@sfu.ca)
//
//% Student Name 2: Win Aung
//% Student 2 #: 301291027
//% Student 2 userid (email): raung (raung@sfu.ca)
//
//% Below, edit to list any people who helped you with the code in this file,
//%      or put ‘none’ if nobody helped (the two of) you.
//
// Helpers: _everybody helped us/me with the assignment (list names or put ‘none’)__
//
// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources:  ___________
//
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in tokenClasses.hpp  too.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "P4_<userid1>_<userid2>" (eg. P4_stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit files to courses.cs.sfu.ca
// Name        : tokenClasses.cpp
// Description : Course Project Part 4 (Parser)
// Original portions Copyright (c) 2016 School of Engineering Science, Simon Fraser University
//============================================================================

#ifndef TOKENCLASSES_HPP_
#define TOKENCLASSES_HPP_

#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>

#include "TreeNode.hpp"

namespace ensc251 {

char *demangle(const char *typeName);

class Token;

extern std::vector<Token*> tokenObjectPs; // objects for abstract syntax tree
extern unsigned int tracker; // tracker index

class Token : public TreeNode
{
	// The Base Token Class
public:

	virtual bool has_string_value(const std::string& aString) { return false; };

	void print_ast() { print_traverse(nullptr); OUT << ";" << std::endl;}

	friend std::ostream& operator <<(std::ostream& outs, const Token& token){
		token.print(outs);
		return outs;
	}

	void print(std::ostream& str) const { str << demangle(typeid(*this).name()) << ": "; }

	virtual Token* process_punctuator(const std::string &puncValue) { return nullptr; }
	virtual Token* process_declaration() { return nullptr; }
	virtual Token* process_id() { return nullptr; }
	virtual Token* process_unary() { return nullptr; }
	virtual Token* process_primary_exp() { return nullptr; }
	virtual Token* advance_past_shift_operator() { return nullptr; }
	// ***** you may need to add stuff here *****
	virtual Token* advance_past_assignment_operator() { return nullptr; }
	virtual Token* process_postfix() { return nullptr;}
	virtual bool has_string_value_star(const std::string& aString) { return false; };//Star only

	virtual bool is_postfix_operator() { return false; }

	Token* process_token()
	{
		tracker++;
		if (tracker < tokenObjectPs.size()){
			return this;
		}
		else
			// our grammar must end in ';'. If *this is the end
			//  then it is a premature end which we will report now.
			return nullptr;
	}

	virtual TreeNode* print_action() { return nullptr; };

	virtual void print_traverse(const TreeNode*)
	{
		if(childPVector.size() > 2)
		{
			traverse_children_post_order(
					static_cast<ensc251::traverse_func>(print_traverse),
					static_cast<ensc251::action_func>(print_action));
		}
		else if(childPVector.size() == 2)
		{
			traverse_children_in_order(
					static_cast<ensc251::traverse_func>(print_traverse),
					static_cast<ensc251::action_func>(print_action));
		}
		else
		{
			traverse_children_pre_order(
					static_cast<ensc251::traverse_func>(print_traverse),
					static_cast<ensc251::action_func>(print_action));
		}
	}

	virtual ~Token() {}

};

// the below functions are defined in tokenClasses.cpp
Token* recursive_parser();
Token* assignment_exp();
Token* unary_exp();

class StringBasedToken:public Token {
	// subclasses: id, type_spec, assignment_operator, shift_operator, additive_operator, div_operator, unary_operator, string
protected:
	std::string stringValue;

public:
	StringBasedToken(const std::string &aStringValue): stringValue (aStringValue) { };
	void print(std::ostream& str) const { Token::print(str); str << ": " << stringValue; }
	bool has_string_value(const std::string& aString) { return stringValue == aString; }
	TreeNode* print_action() { OUT << stringValue << " "; return nullptr; };
};

class punctuator:public StringBasedToken {
	// e.g., ';', '*', '(', ')', ':', etc.
public:
	punctuator(const std::string &puncValue): StringBasedToken(puncValue){ };

	void print_traverse(const TreeNode*)
	{
		if (stringValue == "(") {
			traverse_children_pre_order(
					static_cast<ensc251::traverse_func>(print_traverse),
					static_cast<ensc251::action_func>(print_action));

			OUT << ") ";
		}
		else
			traverse_children_in_order(
					static_cast<ensc251::traverse_func>(print_traverse),
					static_cast<ensc251::action_func>(print_action));
	}
	bool has_string_value(const std::string& aString) { return aString == stringValue; };//Star only
	bool has_string_value_star(const std::string& aString) { return aString == "*"; };//Star only

	// NOTE: The following function has just been added to the updated template.
	TreeNode* print_action()
	{
		OUT << stringValue << " ";
		if (stringValue==";") OUT << std::endl;
		return nullptr;
	};

	Token* process_primary_exp()
	{
		if(stringValue == "(" && process_token())
		{
			Token* tObjP = assignment_exp();
			if(tObjP)
			{
				if(tokenObjectPs[tracker]->process_punctuator(")"))
					{
					add_childP(tObjP);
					return this;
					}
			}

		}
		return nullptr;
	}

	Token* process_punctuator(const std::string &puncValue)
	{
		if (puncValue == stringValue)
			return process_token();
		return nullptr;
	}
};

class id:public StringBasedToken
{
	// identifiers -- example:  sum
public:
	id(const std::string &idValue): StringBasedToken(idValue){  };
	Token* process_id()
	{
		return process_token();
	}
	Token* process_primary_exp()
	{
		return process_id(); // return process_token();
	}
};

class type_spec:public StringBasedToken
{
	// 'char' | 'int' | 'float'
public:
	type_spec(const std::string &typeValue): StringBasedToken(typeValue){ };
	// ***** Complete this class
	Token* process_declaration() // Create the type_spec declaration tree
	{
		if (tokenObjectPs[tracker+1]->has_string_value_star("*")) // Create a tree from type_spec pointer (parent star to children type and id)
		{
			Token* startoken = tokenObjectPs[tracker+1];
			Token* typetoken = tokenObjectPs[tracker];
			//startoken->add_childP(typetoken);
			tracker = tracker + 2;
			Token* idtoken = tokenObjectPs[tracker];
			startoken->add_childP(typetoken);
			startoken->add_childP(idtoken);
			tracker++;
			return startoken;
		}
		else // create normal tree type_spec (parent type to id)
		{
			Token* treeP = tokenObjectPs[tracker];
			tracker++;
			Token* subtreeP = tokenObjectPs[tracker];
			treeP->add_childP(subtreeP);
			tracker++;
			return treeP;
		}

	}
};

class assignment_operator:public StringBasedToken
{
	// '=' | '*=' | '/=' | '%=' | '+=' | '-=' | '<<=' | '>>=' | '&=' | '^=' | '|='
public:
	assignment_operator(const std::string &opValue): StringBasedToken(opValue){ };
	Token* advance_past_assignment_operator() {
		//if(tokenObjectPs[tracker]->has_string_value())

		return process_token();
	}
};

class conditional_operator:public StringBasedToken
{
public:
	conditional_operator(const std::string &opValue): StringBasedToken(opValue){ };
	void print_traverse(const TreeNode*)
	{
		static_cast<ensc251::Token*>(this->childPVector[0])->print_traverse(this);
		this->print_action();
		static_cast<ensc251::Token*>(this->childPVector[1])->print_traverse(this);
		OUT << ": " ;
		static_cast<ensc251::Token*>(this->childPVector[2])->print_traverse(this);
	}
};

class shift_operator:public StringBasedToken
{
	// '<<' | '>>'
public:
	shift_operator(const std::string &opValue): StringBasedToken(opValue){ };
	Token* advance_past_shift_operator()
	{
		return process_token();
	}
	Token* process_primary_exp() // create the tree for shift operator (shift parent to operator and operated objects)
	{
		Token* shift_token = tokenObjectPs[tracker+3];
		Token* assign_token = tokenObjectPs[tracker+1];
		Token* id_token1 = tokenObjectPs[tracker];
		Token* num_token = tokenObjectPs[tracker+2];
		Token* id_token2 = tokenObjectPs[tokenObjectPs.size()-2];



		assign_token->add_childP(id_token1);
		assign_token->add_childP(shift_token);
		shift_token->add_childP(num_token);
		shift_token->add_childP(id_token2);

		tracker = tokenObjectPs.size()-1;
		return assign_token;
	}

};

class additive_operator:public StringBasedToken
{
	// '+' | '-'
public:
	additive_operator(const std::string &opValue): StringBasedToken(opValue){ };
	// ***** Complete this class if needed
	Token* process_primary_exp() // Create a tree with additive operations (operator to children 1st token and 2nd)
	{
		Token* plus_token = tokenObjectPs[tracker+3];
		Token* assign_token = tokenObjectPs[tracker+1];
		Token* id_token1 = tokenObjectPs[tracker];
		Token* num_token = tokenObjectPs[tracker+2];
		Token* id_token2 = tokenObjectPs[tokenObjectPs.size()-2];



		assign_token->add_childP(id_token1);
		assign_token->add_childP(plus_token);
		plus_token->add_childP(num_token);
		plus_token->add_childP(id_token2);

		tracker = tokenObjectPs.size()-1;
		return assign_token;
	}

};

class div_operator:public StringBasedToken
{
	// '/' | '%'
public:
	div_operator(const std::string &opValue): StringBasedToken(opValue){ };
	// ***** Complete this class if needed
};

class unary_operator:public StringBasedToken
{
	// '&' | '~'
public:
	unary_operator(const std::string &opValue): StringBasedToken(opValue){ };

	Token* process_unary()
	{
		Token* subTreeP;
		if(process_token() && (subTreeP = unary_exp()))
		{
			add_childP(subTreeP);
			return this;
		}
		return nullptr;
	}
};

class postfix_operator:public StringBasedToken
{
public:
	postfix_operator(const std::string &opValue): StringBasedToken(opValue){ };
	bool is_postfix_operator() { return true; }

	Token* process_postfix() // create postfix operator tree (postfix to object)
	{
		Token* tree = tokenObjectPs[tracker];
		tracker++;
		Token*subtreeP = tokenObjectPs[tracker];
		tree->add_childP(subtreeP);
		tracker++;
		return tree;
	}

	// ***** Complete this class if needed

};

class string:public StringBasedToken
{
	// e.g., "sample string"
public:
	string(const std::string &aStringValue) : StringBasedToken(aStringValue) {};

	Token* process_primary_exp()
	{
		return process_token();
	}

};

class incorrect:public StringBasedToken
{
public:
	incorrect(const std::string &aStringValue) : StringBasedToken(aStringValue) {};
};

//        template<typename T> class numeric_const;

class constant:public Token
{
	// subclasses: numeric_const<T>
public:
	Token* process_primary_exp()
	{
		return process_token();
	}

};

template<typename T>
class numeric_const:public constant
{
private:
	T value;
public:
	numeric_const(const T& constValue): value(constValue) {}
	/*Fill in for additional constructors if desired */

	const T& getValue() const { return value; }
	void print(std::ostream& str) const {Token::print(str); str << ": " << value; }

	TreeNode* print_action() { OUT << value << " "; return nullptr; };
};

} // namespace ensc251

#endif /* TOKENCLASSES_HPP_ */
