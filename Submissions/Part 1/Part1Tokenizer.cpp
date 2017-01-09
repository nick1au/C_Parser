//============================================================================
//
//% Student Name 1: Nicholas Lau
//% Student 1 #: 301290893
//% Student 1  nla52 (nla52@sfu.ca)
//
//% Student Name 2: Win Aung
//% Student 2 #: 301291027
//% Student 2 raung (raung@sfu.ca)
//
//% Below, edit to list any people who helped you with the code in this file,
//%      or put ‘none’ if nobody helped (the two of) you.
//
// Helpers: _everybody helped us/me with the assignment (list names or put ‘none’)__
//
// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources: https://msdn.microsoft.com/en-us/library/69ze775t.aspx
//			  http://www.cplusplus.com/
//
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in Test.cpp if you are submitting that file too.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "<userid1>_<userid2>" (eg. stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit one or two files to courses.cs.sfu.ca
//
// Name        : Part1.cpp
// Description : Course Project Part 1 (Tokenizer)
//============================================================================

//#include "Part1Tokenizer.h"
// at least some of the following will eventually go in a file "Part1Tokenizer.h":
#include <string>
#include <vector>
#include <iostream>
#define NEW_LINE "\n"
#define QUOTATION "\""

using namespace std;



vector<string> tokenizeCodeStrip(istream& code); // declaration
// end of eventual contents of file "Part1Tokenizer.h"

// Accepts a stream and returns a vector of tokens with any remaining preprocessing directives stripped out
// Input: a stream
// Output: a vector of tokens

//vector<string> test_case_one(string);
//vector<string> test_case_two(string);

string searching_letters(string&, int&);
//Adds the letter characters to the token string and stops at
//the first instance of a non-alphabetical character
//@param Entire string and the location of the index.
//@return One string containing only alphabets.

string searching_numbers(string&, int&);
// Adds numerical characters into the token. If the scientific notation E or e is
// found, it will maintain the token.
//@param Entire string and the location of the index.
//@return One string containing only numbers.

string searching_operators(string&, int&);
// Looks for symbol characters and adds them as a token. If specific combinations
// such as ++ or == are found, they will be tokenized together.
//@param Entire string and the location of the index.
//@return One string containing only opeartions such as +, -, ++, etc.

string searching_quotations(string&, int&);
// Find single quotation and finds the ending quotation. It will preserve
// the contents within the quotation as a string token. If there is no
// end quotation, then a blank token will be entered.
//@param Entire string and the location of the index.
//@return One string containing single quotation.

string searching_double_quotations(string&, int&);
// Find double quotation and finds the ending quotation. It will preserve
// the contents within the quotation as a string token. If there is no
// end quotation, then a blank token will be entered.
//@param Entire string and the location of the index.
//@return One string containing single quotation.

void searching_hashtag(string&,int&);
//Set the index of the string to after #define MYDEFINE.
//@param Entire string and the location of the index.
//@return None.

vector<string> tokenizeCodeStrip(istream& code)
{
	// *** Fill in implementation ***
	string word;
	string next;
	string token;
	vector<string> tokenize_vector;
	getline(code,word);
	getline(code,next);
	word = word + next;
	vector<char> backslash_character = {'\n','\r','\b','\t','\v','\f','\a'};


	int i = 0;

	while(i < word.length())
	{
		for(unsigned int j = 0; j < backslash_character.size(); j++)
		{
			if(word.at(i) == backslash_character.at(j))
				i++;
		}
		if((word.at(i) >= 'a' && word.at(i) <= 'z') || (word.at(i) >= 'A' && word.at(i) <= 'Z'))
		{
			token = "";
			token = token + searching_letters(word,i);
			tokenize_vector.push_back(token);
		}
		else if(word.at(i) >= '0' && word.at(i) <= '9')
			{
				token = "";
				token = searching_numbers(word,i);
				tokenize_vector.push_back(token);
			}
		else if(word.at(i) == '+' || word.at(i) == '-' || word.at(i) == '=' || word.at(i) == '*' || word.at(i) == '/' || word.at(i) == '%')
		{
			token = "";
			token = searching_operators(word,i);
			tokenize_vector.push_back(token);
		}
		else if(word.at(i) == '\'')
		{
			token = "";
			token = searching_quotations(word,i);
			tokenize_vector.push_back(token);
		}
		else if(word.at(i) == '\"')
		{
			token = "";
			token = searching_double_quotations(word,i);
			tokenize_vector.push_back(token);
		}
		else if(word.at(i) == '#')
		{
			token = "";
			searching_hashtag(word,i);
		}
		else if(word.at(i) != ' ')
		{
			token = "";
			token = word.at(i);
			tokenize_vector.push_back(token);
			i++;
		}
		else
		{
			token="";
			i++;
		}

	}


	return tokenize_vector;
}

void searching_hashtag(string& word, int& index)
{
	string token;
	index = word.find(' ',8);
}

string searching_double_quotations(string& word, int& index)
{
	string token;
	int i = word.find_last_of('"');
	if(i != index)
	{
		int j = word.find('"',index+1);
		j = abs(index - j);
		token = word.substr(index,j+1);
		index = word.find_first_of('"',(index+1)) +  1;
	}
	else
	{
		token = "";
		index = word.find_last_of(" ");
	}

	return token;
}
string searching_quotations(string& word, int& index)
{
	string token;
	int i = word.find_last_of("'");
	if(i != index)
	{
		token = word.substr(index,word.find_last_of("'"));
		token = token.substr(0,token.length()-1);
	}
	else
	{
		token = "";
		index = index + 2;
		return token;
	}
	index = word.find_first_of("'",index+1) + 1;
	return token;

}

string searching_operators(string& word, int& i)
{
	string token;
	if(word.at(i+1) == '+' || word.at(i+1) == '-' || word.at(i+1) == '=' || word.at(i+1) == '*' || word.at(i+1) == '/' || (word.at(i+1) == '%'))
	{
		if((word.at(i) == '+' && word.at(i+1) == '+') || (word.at(i) == '-' && word.at(i+1) == '-') || (word.at(i+1) == '='))
		{
			token = token + word.at(i) + word.at(i+1);
			i = i + 2;
		}
		else
			{
				token = word.at(i);
				i++;
			}
	}
	else
	{
		token = word.at(i);
		i++;
	}
	return token;
}

string searching_letters(string& word, int& index)
{
	string token;
	while((word.at(index) >= 'a' && word.at(index) <= 'z') || (word.at(index) >= 'A' && word.at(index) <= 'Z'))
	{
		token = token + word.at(index);
		index++;
	}

	return token;
}

string searching_numbers(string& word,int& index)
{
	string token;
	while(word.at(index) >= '0' && word.at(index) <= '9')
	{
		token = token + word.at(index);
		if(word.at(index+1) == 'E' || word.at(index+1) == 'e')
		{
			token = token + word.at(index+1);
			if(word.at(index+2) == '-')
				{
				token = token + word.at(index+2);
				index = index + 2;
				}
			else
				index++;
		}
		index++;
	}
	return token;
}
/*
vector<string> test_case_two(string word)
{
	string token;
	vector<string> wrong;
	unsigned int i;

	for(i = 0; i < word.length(); i++)
	{
		if(word.at(i) >= 'a' && word.at(i) <= 'z')
			token = token + word.at(i);
		else if(word.at(i) >= '0' && word.at(i) <= '9')
				token = token + word.at(i);
		else
		{
			wrong.push_back(token);
			token = "";
			token = token + word.at(i);
			wrong.push_back(token);
			token = "";
		}
	}
	return wrong;
}
vector<string> test_case_one(string word)
{
	string token;
	vector<string> wrong;
	unsigned int i;
	for(i = 0;i < word.length();i++)
	{
		if(word.at(i) != ' ')
			token = token + word.at(i);
		else
		{

			wrong.push_back(token);
			token = "";
		}

	}
	wrong.push_back(";");
	return wrong;
}
*/
