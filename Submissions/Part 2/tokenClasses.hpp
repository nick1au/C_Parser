//% Student Name 1: Win Aung
//% Student 1 #: 301291027
//% Student 1 userid (email): stu1 (raung@sfu.ca)
//
//% Student Name 2: Nicholas Lau
//% Student 2 #: 301290893
//% Student 2 userid (email): stu2 (nla52@sfu.ca)
//
//% Below, edit to list any people who helped you with the code in this file,
//%      or put ‘none’ if nobody helped (the two of) you.
//
// Helpers: _everybody helped us/me with the assignment (list names or put ‘none’)  TA Shams,
//
// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources: http://www.cplusplus.com/
#ifndef TOKENCLASSES_HPP_
#define TOKENCLASSES_HPP_

#include <string>
#include <iostream>
#include <typeinfo>

namespace ensc251 {

		char *demangle(const char *typeName);

        class Token
        {
        	// The Base Token Class
        public:
         	 // we will fix this up when we learn about virtual functions.
			friend std::ostream& operator <<(std::ostream& outs, const Token& token){
				token.print(outs);
				return outs;
			}

           virtual void print(std::ostream& str) const { str << demangle(typeid(*this).name()) << ": "; }

     	   virtual ~Token() {} // You will learn about virtual soon, but for now, don't delete this line.

		};

        class StringBasedToken:public Token {
			// subclasses: id, type_spec, assignment_operator, shift_operator, additive_operator, mul_operator, unary_operator, string
		private:
        	std::string stringValue;

		public:
			// StringBasedToken() { std::cout << "Warning:  Do not invoke this constructor" << std::endl; }; // make sure this does not get invoked.
			StringBasedToken(const std::string &aStringValue) /* Fill in... */
			{
				stringValue = aStringValue;
			} // constructor
			const std::string& getStringValue( ) const
			{ return stringValue; } // return the stringValue
			void print(std::ostream& str) const { Token::print(str); str << stringValue; }
        };

        class punctuator:public StringBasedToken {
        	// ';', ':', '*', '(', ')'
        public:
        	punctuator(std::string astring): StringBasedToken(astring){
        	}
        };

        class id:public StringBasedToken
        {
			// identifiers -- example:  sum
        public:
        	id(std::string astring): StringBasedToken(astring){
        	}
        };

        class type_spec:public StringBasedToken
		{
			// 'char' | 'int' | 'float'
        public:
        	type_spec(std::string astring): StringBasedToken(astring){
        	}
        	/*Fill in the implementation for constructor  */
		};

        class assignment_operator:public StringBasedToken
		{
			// '=' | '*=' | '/=' | '%=' | '+=' | '-=' | '<<=' | '>>=' | '&=' | '^=' | '|='
        public:
        	assignment_operator(std::string astring): StringBasedToken(astring){
        	}
		};

        class conditional_operator:public StringBasedToken
		{
			// '?'
        public:
        	conditional_operator(std::string astring): StringBasedToken(astring){
        	}
		};

        class shift_operator:public StringBasedToken
		{
			// '<<' | '>>'
        public:
        	shift_operator(std::string astring):StringBasedToken(astring){
        	}
		};

        class additive_operator:public StringBasedToken
		{
			// '+' | '-'
        public:
        	additive_operator(std::string astring): StringBasedToken(astring){
        	}
		};

        class div_operator:public StringBasedToken
		{
			// '/' | '%'
        public:
        	div_operator(std::string astring): StringBasedToken(astring){
        	}
		};
        class unary_operator:public StringBasedToken
		{
			// '&' | '~'
          public:
        	unary_operator(std::string astring): StringBasedToken(astring){
        	}
		};

        class postfix_operator:public StringBasedToken
		{
			// '++' | '--'
        public:
        	postfix_operator(std::string astring): StringBasedToken(astring){
        	}
		};

        class string:public StringBasedToken
        {
        	// e.g., "sample string"
        public:
        	string(std::string astring): StringBasedToken(astring){
        	}
        };

        class incorrect:public StringBasedToken
		{
			// e.g., "sample string     --> This is incorrect ...
        	//		(quotation mark " is missing at the end)
		public:
        	incorrect(std::string astring): StringBasedToken(astring)
        	{
        		std::cout << astring  << " This is incorrect ...quotation mark is missing at the end";
        	}
		};

        template<typename T> class numeric_const;

        class constant:public Token
		{

		};

        template<typename T>
        class numeric_const:public constant
		{
		private:
        	T value;
		public:
        	numeric_const(const T& constValue): value(constValue) {}
        	/*Fill in for additional constructors if desired */

        	const T& getValue() const
        	{return value; }
        	void print(std::ostream& str) const {Token::print(str); str << ": " << value; }
		};

} // namespace ensc251

#endif /* PARSERCLASSES_HPP_ */
