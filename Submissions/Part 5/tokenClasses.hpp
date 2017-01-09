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
// Helpers: _everybody helped us/me with the assignment (list names or put ‘none’)
//			Shawn Thai, TA Shams

// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources: http://www.cplusplus.com/reference/type_traits/is_same/
//
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in tokenClasses.cpp  too.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "P5_<userid1>_<userid2>" (eg. P5_stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit files to courses.cs.sfu.ca
//
// Name        : tokenClasses.hpp
// Description : Course Project Part 5 (Interpreting/Evaluation)
// Original portions Copyright (c) 2016 School of Engineering Science, Simon Fraser University
//============================================================================

#ifndef TOKENCLASSES_HPP_
#define TOKENCLASSES_HPP_

#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <map>
#include <memory>
#include <exception>
#include <stdexcept>

#include "TreeNode.hpp"

namespace ensc251 {

char *demangle(const char *typeName);

class Token;
class constant;

typedef std::shared_ptr<constant> shared_ptr_to_constant;

extern std::vector<Token*> tokenObjectPs; // objects for abstract syntax tree
extern unsigned int tracker; // tracker index

extern std::string empty;

void print_evaluation_map();

class Token : public TreeNode
{
	// The Base Token Class
public:

	virtual bool has_string_value(const std::string& aString) { return false; };
	virtual const std::string& get_string_value() { return empty; };

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
	virtual Token* process_postfix_operator() { return nullptr; }
	virtual Token* advance_past_conditional_operator() { return nullptr; }
	virtual Token* advance_past_shift_operator() { return nullptr; }
	virtual Token* advance_past_additive_operator() { return nullptr; }
	virtual Token* advance_past_div_operator() { return nullptr; }
	virtual Token* advance_past_assignment_operator() { return nullptr; }

	virtual shared_ptr_to_constant eval() { return nullptr; }; // should this be pure virtual?

	Token* getChildToken(unsigned childNum) const { return static_cast<Token*>(this->childPVector[childNum]); }
	shared_ptr_to_constant evalChildToken(unsigned childNum) /* const */ { return getChildToken(childNum)->eval(); }

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

#define MULTI_OP_IFACE(RNAME, OP, TYPE) \
		virtual shared_ptr_to_constant RNAME(TYPE v) = 0;

#define NUMERIC_OP_IFACE(NAME, OP) \
		MULTI_OP_IFACE(r##NAME, OP, float) \
		MULTI_OP_IFACE(r##NAME, OP, int) \
		MULTI_OP_IFACE(r##NAME, OP, char) \
		\
		virtual shared_ptr_to_constant NAME(constant& obj) = 0;\
		virtual shared_ptr_to_constant operator OP (constant& obj) = 0;

template<typename U>
class numeric_const;

class constant : public Token
{
	// subclasses: numeric_const<T>
public:

	/* ***** this class needs work including for multiple operations supported on constants such as postfix operations, addition, subtraction, shift. */

	virtual shared_ptr_to_constant putValueIntoInt() = 0;

	virtual shared_ptr_to_constant putValueIntoChar() = 0;
	virtual shared_ptr_to_constant putValueIntoFloat() = 0;
	virtual shared_ptr_to_constant add_postfix() = 0;
	virtual shared_ptr_to_constant subtract_postfix() = 0;

	virtual shared_ptr_to_constant mod(int modInt) = 0;
	virtual shared_ptr_to_constant shift_left(int shiftInt) = 0;
	virtual shared_ptr_to_constant shift_right(int shiftInt) = 0;

	virtual shared_ptr_to_constant TF_conditional(int op1, int op2) = 0;


	virtual std::string getStringValue() = 0;
	virtual bool isNotZero() = 0;

	NUMERIC_OP_IFACE(div, /)

	NUMERIC_OP_IFACE(add, +)
	NUMERIC_OP_IFACE(subtract, -)


	virtual shared_ptr_to_constant complement() = 0;

	// template function.  See beginning of chapter 17.
	template<typename U>
	shared_ptr_to_constant create(U v){ return shared_ptr_to_constant(new numeric_const<U>(v)); }

	Token* process_primary_exp()
	{
		return process_token();
	}
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
	const std::string& get_string_value() { return stringValue; }
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

	TreeNode* print_action()
	{
		StringBasedToken::print_action();
		if (stringValue==";")
			OUT << std::endl;
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

	shared_ptr_to_constant eval()
	{
		this->evalChildToken(0);
		this->evalChildToken(1);
		return shared_ptr_to_constant();
	}
};

class type_spec:public StringBasedToken
{
	// 'char' | 'int' | 'float'
public:
	type_spec(): StringBasedToken("int"){ };
	type_spec(const std::string &typeValue): StringBasedToken(typeValue){ };
	Token* process_declaration()
	{
		Token* subTreeP = process_token();
		Token* tObjP;
		if((tObjP = tokenObjectPs[tracker]->process_punctuator("*"))) {
			tObjP->add_childP(subTreeP);
			subTreeP = tObjP;
		}

		if ((tObjP = tokenObjectPs[tracker]->process_id())) {
			subTreeP->add_childP(tObjP);
			return subTreeP;
		}
		return nullptr;
	}

	shared_ptr_to_constant ofTypeCreateConstant(shared_ptr_to_constant constToCreateFromSP)
	{
		switch (this->stringValue[0]) {
		/* ***** this needs work for other supported data types i.e. char and float */

		case 'i': // int
			return constToCreateFromSP->putValueIntoInt();
		case 'c': // char
			return constToCreateFromSP->putValueIntoChar();
		case 'f': // float
			return constToCreateFromSP->putValueIntoFloat();
		}
		return shared_ptr_to_constant();
	}

	shared_ptr_to_constant eval();  // please see definition of this function in tokenClasses.hpp
};

struct varInfo {
	shared_ptr_to_constant sptc;
	type_spec ts;
};

extern
std::map<std::string, varInfo > evaluation_map;  // This map holds the evaluation results for AST.

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
	shared_ptr_to_constant eval() { return evaluation_map[get_string_value()].sptc; }
};

class assignment_operator:public StringBasedToken
{
	// '=' | '*=' | '/=' | '%=' | '+=' | '-=' | '<<=' | '>>=' | '&=' | '^=' | '|='
public:
	assignment_operator(const std::string &opValue): StringBasedToken(opValue){ };
	Token* advance_past_assignment_operator() {
		return process_token();
	}

	/* ***** BONUS MARKS: Evaluation of string assignments would be considered for the bonus points.
	 * for example 's = "Hello world";'  */

	/*
	 * ***** BONUS MARKS: So far we have only considered '=' in evaluation.
	 * You may try implementing  '/=' | '+=' | '-=' | '<<=' | '>>='  to earn BONUS MARKS.
	 *
	*/
	shared_ptr_to_constant eval()
	{
		return evaluation_map[getChildToken(0)->get_string_value()].sptc = evaluation_map[getChildToken(0)->get_string_value()].ts.ofTypeCreateConstant(evalChildToken(1));
	};
};

class conditional_operator:public StringBasedToken
{
public:
	conditional_operator(const std::string &opValue): StringBasedToken(opValue){ };
	Token* advance_past_conditional_operator() {
		return process_token();
	}
	void print_traverse(const TreeNode*)
	{
		static_cast<ensc251::Token*>(this->childPVector[0])->print_traverse(this);
		this->print_action();
		static_cast<ensc251::Token*>(this->childPVector[1])->print_traverse(this);
		OUT << ": " ;
		static_cast<ensc251::Token*>(this->childPVector[2])->print_traverse(this);
	}

	/* ***** this needs work */

	shared_ptr_to_constant eval()
	{
		auto right_eval = evalChildToken(1);
		auto right_right_eval = evalChildToken(2);
		auto op2 = atoi(right_eval->getStringValue().c_str());
		auto op1 = atoi(right_right_eval->getStringValue().c_str());

		return evalChildToken(0)->TF_conditional(op2, op1);
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

	/* ***** this needs work */
	shared_ptr_to_constant eval()
	{
		auto right_eval = evalChildToken(1);
		auto shiftInt = atoi(right_eval->getStringValue().c_str());
		if (stringValue == "<<")
			return evalChildToken(0)->shift_left(shiftInt);
		if (stringValue == ">>")
			return evalChildToken(0)->shift_right(shiftInt);
		return this->evalChildToken(0);
	}
};

class additive_operator:public StringBasedToken
{
	// '+' | '-'
public:
	additive_operator(const std::string &opValue): StringBasedToken(opValue){ };
	Token* advance_past_additive_operator() {
		return process_token();
	}

	/* ***** this needs work */
	// Similar code as div_op
	shared_ptr_to_constant eval()
	{
		auto right_eval = evalChildToken(1);


		if (this->stringValue == "+")
			return evalChildToken(0)->add(*right_eval);
		else
			return evalChildToken(0)->subtract(*right_eval);
		return evalChildToken(0)->add(*right_eval);



	}
};

class div_operator:public StringBasedToken
{
	// '/' | '%'
public:
	div_operator(const std::string &opValue): StringBasedToken(opValue){ };
	Token* advance_past_div_operator() {
		return process_token();
	}

	shared_ptr_to_constant eval()
	{
		auto right_eval = evalChildToken(1);
		auto modInt = atoi(right_eval->getStringValue().c_str());

		try
		{
			if (!right_eval->isNotZero())
				throw(std::overflow_error("Divide or Mod by zero"));
			else
				/* ***** this needs work */
				if (stringValue[0] == '/')
				{
					return evalChildToken(0)->div(*right_eval);
				}
				if (stringValue[0] == '%')
				{
					return evalChildToken(0)->mod(modInt);
				}
				return this->evalChildToken(0);
		}
		catch (const std::overflow_error& e)
		{
			OUT << e.what() << std::endl;
			return nullptr;
		}

	}
};

class unary_operator:public StringBasedToken
{
	//  '~'
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

	shared_ptr_to_constant eval()
	{
		return this->evalChildToken(0)->complement();
	};
};

class postfix_operator:public StringBasedToken
{
public:
	postfix_operator(const std::string &opValue): StringBasedToken(opValue){ };
	void print_traverse(const TreeNode*)
	{
		traverse_children_post_order(
				static_cast<ensc251::traverse_func>(print_traverse),
				static_cast<ensc251::action_func>(print_action));
	}

	Token* process_postfix_operator() { return process_token(); }

	/* ***** this needs work */
	shared_ptr_to_constant eval()
	{
		if (stringValue == "++")
		{
			return evalChildToken(0)->add_postfix();
		}
		if (stringValue == "--")
		{
			return evalChildToken(0)->subtract_postfix();
		}
		return this->evalChildToken(0);
	}
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

	/* ***** BONUS MARKS: Evaluation of string assignments would be considered for the bonus points.
	 * for example 's = "Hello world";'  */
};

class incorrect:public StringBasedToken
{
public:
	incorrect(const std::string &aStringValue) : StringBasedToken(aStringValue) {};
};

#define MULTI_OP(RNAME, OP, TYPE) \
		shared_ptr_to_constant RNAME(TYPE v) { return create(v OP value); }

#define NUMERIC_OP(NAME, OP) \
		MULTI_OP(r##NAME, OP, float) \
		MULTI_OP(r##NAME, OP, int) \
		MULTI_OP(r##NAME, OP, char) \
		\
		shared_ptr_to_constant NAME(constant& obj) {return obj.r##NAME(value); } \
		shared_ptr_to_constant operator OP (constant& obj) {return obj.r##NAME(value); }

template<typename T>
class numeric_const:public constant
{
private:
	T value;
public:
	/* ***** this class needs work including for multiple operations supported on constants such as postfix operations, addition, subtraction, shift. */

	numeric_const(const T& constValue): value(constValue) {}
	//Fill in for additional constructors if desired

	shared_ptr_to_constant putValueIntoInt() { return shared_ptr_to_constant(new numeric_const<int>(value));}
	shared_ptr_to_constant putValueIntoChar() { return shared_ptr_to_constant(new numeric_const<char>(value));}
	shared_ptr_to_constant putValueIntoFloat() { return shared_ptr_to_constant(new numeric_const<float>(value));}


	std::string getStringValue() { return std::to_string(value); }
	T& getValue() { return value; }
	bool isNotZero() {return value; }

	void print(std::ostream& str) const {Token::print(str); str << ": " << value; }
	TreeNode* print_action() { OUT << value << " "; return nullptr;};

	NUMERIC_OP(div, /)
	NUMERIC_OP(add, +)
	NUMERIC_OP(subtract, -)
	//NUMERIC_OP(Modulus, %)

	shared_ptr_to_constant shift_left(int shiftInt)
	{
		if(!std::is_same<T,float>::value)
			return create((int)value<<shiftInt);
		else {
			OUT << "Cannot do shift left op on float" << std::endl;
			return shared_ptr_to_constant();
		}
	}
	shared_ptr_to_constant shift_right(int shiftInt)
	{
		if(!std::is_same<T,float>::value)
			return create((int)value>>shiftInt);
		else {
			OUT << "Cannot do shift right operation on float" << std::endl;
			return shared_ptr_to_constant();
		}
	}
	shared_ptr_to_constant mod(int modInt)
	{
		if(!std::is_same<T,float>::value)
			return create((int)value%modInt);
		else {
			OUT << "Cannot do modulus on float" << std::endl;
			return shared_ptr_to_constant();
		}
	};

	shared_ptr_to_constant add_postfix() // ++
	{
		return create(value++);
	};
	shared_ptr_to_constant subtract_postfix() // --
	{
		return create(value--);
	};
	// find if same or not
	shared_ptr_to_constant TF_conditional(int op1, int op2)
	{
		return create((int)value?op1:op2);
	};


	shared_ptr_to_constant complement() // Find compliment of value
	{
		if(!std::is_same<T,float>::value)
			return create(~(int)value);
		else {
			OUT << "Cannot do complement on float" << std::endl;
			return shared_ptr_to_constant();
		}
	};

	shared_ptr_to_constant eval()
	{
		return create(value);
	};
};

} // namespace ensc251

#endif /* TOKENCLASSES_HPP_ */
