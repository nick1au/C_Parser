//============================================================================
// Name        : Part2.cpp
// Description : Course Project Part 2 (Token Classifier)
//============================================================================
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
//
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in Test.cpp if you are submitting that file too.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "<userid1>_<userid2>" (eg. stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit one or two files to courses.cs.sfu.ca
#include <vector>
#include <sstream>
#include <algorithm>
#include "tokenClasses.hpp"
#include "Part1Tokenizer.hpp"

using namespace std;
using namespace ensc251;

void classId(std::string&,std::vector<ensc251::Token*>& );							//id class
void classPunctuator(std::string&,std::vector<ensc251::Token*>& );					//punctuator class
void classTypeSpec(std::string&,std::vector<ensc251::Token*>& );					//type_spec class
void classAssignmentOperator(std::string&,std::vector<ensc251::Token*>& );			//assignment_operator class
void classConditionalOperator(std::string&,std::vector<ensc251::Token*>& );			//conditional_operator class
void classShiftOperator(std::string&,std::vector<ensc251::Token*>& );				//shift_operator class
void classDivOperator(std::string&,std::vector<ensc251::Token*>& );					//div_operator class
void classAdditiveOperator(std::string&,std::vector<ensc251::Token*>& );			//additive_operator class
void classUnaryOperator(std::string&,std::vector<ensc251::Token*>& );				//unary_operator class
void classPostfixOperator(std::string&,std::vector<ensc251::Token*>& );				//postfix_operator class

// Construct token objects categorized into appropriate types
// Input: a stream
// Output: a vector containing the list of Token objects properly categorized
std::vector<ensc251::Token*> tokenClassifier(istream& code)
{

	std::vector<ensc251::Token*> tokenObjectPs;
	Token enscproject;
	std::vector<std::string> classvector = tokenizeCodeStrip(code);

	for(auto i = 0; i < classvector.size(); i++)
	{
		if(classvector.at(i) == "char" || classvector.at(i) == "int" || classvector.at(i) == "float")
		{
			classTypeSpec(classvector.at(i), tokenObjectPs);
			continue;
		}

		if(classvector.at(i) == "&" || classvector.at(i) == "~")
		{
			classUnaryOperator(classvector.at(i), tokenObjectPs);
			continue;
		}

		if(classvector.at(i) == ";" || classvector.at(i) == ":" || classvector.at(i) == "*" || classvector.at(i) == "," || classvector.at(i) == ")" || classvector.at(i) == "(")
		{
			classPunctuator(classvector.at(i), tokenObjectPs);
			continue;
		}

		if(classvector.at(i) == "++" || classvector.at(i) == "--")
		{
			classPostfixOperator(classvector.at(i), tokenObjectPs);
			continue;
		}

		if(classvector.at(i) == "\"")
		{
			std::string temp = classvector.at(i);
			if(temp.find_last_of("\'") != temp.find_first_of("\""))
			{
				auto *type = new ensc251::string(classvector.at(i));
				tokenObjectPs.push_back(type);
				i = temp.find_first_of("\"");
				std::cout << *type <<std::endl;
			}
			else
			{
				auto *type = new incorrect(classvector.at(i));
				tokenObjectPs.push_back(type);
				std::cout << *type <<std::endl;
			}
			continue;
		}

		if(classvector.at(i) == "/" || classvector.at(i) == "%")
		{
			classDivOperator(classvector.at(i), tokenObjectPs);
			continue;
		}

		if(classvector.at(i) == "+" || classvector.at(i) == "-")
		{
			classAdditiveOperator(classvector.at(i), tokenObjectPs);
			continue;
		}

		if(classvector.at(i) == ">>" || classvector.at(i) == "<<")
		{
			classShiftOperator(classvector.at(i), tokenObjectPs);
			continue;
		}

		if(classvector.at(i) == "?")
		{
			classConditionalOperator(classvector.at(i), tokenObjectPs);
			continue;
		}

		if(classvector.at(i) == "=" || classvector.at(i) == "*=" || classvector.at(i) == "/="
				|| classvector.at(i) == "%=" || classvector.at(i) == "+=" || classvector.at(i) == "<<="
						|| classvector.at(i) == ">>=" || classvector.at(i) == "-=" || classvector.at(i) == "&="
								|| classvector.at(i) == "^=" || classvector.at(i) == "|=")
		{
			classAssignmentOperator(classvector.at(i), tokenObjectPs);
			continue;
		}

		std::string test = classvector.at(i);
		if ((int) test.at(0) >= 48 && (int) test.at(0) <= 57)					//integer 48 is 0 and 57 is 9
		{
			if (test.find_first_of(".") != -1 || test.find_first_of("E") != -1 || test.find_first_of("e") != -1)
			{
				float num_float_test = std::stof(test,nullptr);
				auto *type = new numeric_const<float>(num_float_test);
				tokenObjectPs.push_back(type);
				std::cout << *type <<std::endl;
				continue;
			}
			int num_int_test = std::stoi(test,nullptr,10);
			if(num_int_test%10 >= 0 && num_int_test % 10 <= 9)
			{
				auto *type = new numeric_const<int>(num_int_test);
				tokenObjectPs.push_back(type);
				std::cout << *type <<std::endl;
				continue;
			}
		}

		if((int) test.at(0) == 39 && (int) test.at(test.size() - 1) == 39)		//integer 39 in ASCII code is equal to '
		{
			int test_int = (int) test.at(1);
			char test_char = (char) test_int;
			auto *type = new numeric_const<char>(test_char);
			tokenObjectPs.push_back(type);
			std::cout << *type <<std::endl;
			continue;
		}
		if((int) test.at(0) != 34)												//integer 34 in ASCII code is equal to "
		{

			classId(classvector.at(i), tokenObjectPs);
			continue;
		}
		if ((int) test.at(0) == 34 && (int)test.at(test.size() - 1) == 34)
		{
			auto *type = new ensc251::string(test);
			tokenObjectPs.push_back(type);
			std::cout << *type << std::endl;
			continue;
		}

	}
	return tokenObjectPs;
}

void classAssignmentOperator(std::string& astr,std::vector<ensc251::Token*>& tokenObjectPs)
{
	auto *type = new assignment_operator(astr);
	tokenObjectPs.push_back(type);
	std::cout << *type <<std::endl;
}
void classId(std::string& astr,std::vector<ensc251::Token*>& tokenObjectPs)
{
	auto *type = new id(astr);
	tokenObjectPs.push_back(type);
	std::cout << *type <<std::endl;
}
void classPunctuator(std::string& astr,std::vector<ensc251::Token*>& tokenObjectPs )
{
	auto *type = new punctuator(astr);
	tokenObjectPs.push_back(type);
	std::cout << *type <<std::endl;
}
void classTypeSpec(std::string& astr,std::vector<ensc251::Token*>& tokenObjectPs)
{
	auto *type = new type_spec(astr);
	tokenObjectPs.push_back(type);
	std::cout << *type <<std::endl;
}
void classConditionalOperator(std::string& astr,std::vector<ensc251::Token*>& tokenObjectPs)
{
	auto *type = new conditional_operator(astr);
	tokenObjectPs.push_back(type);
	std::cout << *type <<std::endl;
}
void classShiftOperator(std::string& astr,std::vector<ensc251::Token*>& tokenObjectPs)
{
	auto *type = new shift_operator(astr);
	tokenObjectPs.push_back(type);
	std::cout << *type <<std::endl;
}
void classDivOperator(std::string& astr,std::vector<ensc251::Token*>& tokenObjectPs)
{
	auto *type = new div_operator(astr);
	tokenObjectPs.push_back(type);
	std::cout << *type <<std::endl;
}
void classAdditiveOperator(std::string& astr,std::vector<ensc251::Token*>& tokenObjectPs)
{
	auto *type = new additive_operator(astr);
	tokenObjectPs.push_back(type);
	std::cout << *type <<std::endl;
}
void classUnaryOperator(std::string& astr,std::vector<ensc251::Token*>& tokenObjectPs)
{
	auto *type = new unary_operator(astr);
	tokenObjectPs.push_back(type);
	std::cout << *type <<std::endl;
}
void classPostfixOperator(std::string& astr,std::vector<ensc251::Token*>& tokenObjectPs)
{
	auto *type = new postfix_operator(astr);
	tokenObjectPs.push_back(type);
	std::cout << *type <<std::endl;
}
