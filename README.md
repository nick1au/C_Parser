# C++ Parser
A C++ based parser designed around the different stages involved.
* The link for this project will be changed to "C_Parser" starting April 15, 2017 (nick1au/C_Parser)

## About

* Course-based Project - ENSC 251 (Software Design and Analysis for Engineers)
* Active during September to December 2016
* Worked with Win Aung
* Professor: Craig Scratchley

## Project Info
* C++ based Parser for C++
* Created on:  
i. Eclipse IDE Mars 2  
ii. MinGW-W64 4.9.3  
iii. BOOST 1.55
* Automated test cases were used with BOOST library and debugging using Eclipse.
* Developed with Object-Oriented Programming

## Project Breakdown

### Part 1
* Tokenize an input into a vector
* Keep float numbers together (eg. 1232456E-3 is a token)
* Must separate string into tokens even if there are no whitespaces between them.

### Part 2
* Classifies the tokens as a class type
* Types include:  
i. ID  
ii. Assignment operator  
iii. numeric_const<T> (where T is a template)  
iv. unary_operator
v. additive_operator  
vi. punctuator  
vii. div_operator  
viii. type_spec  
ix. conditional_operator  
x. shift_operator  
xi. string  
xii. postfix_operator  
xiii. incorrect

### Part 3
* Forms a tree structure that will be used in Part 4
* Implements function pointers (passing itself to other functions)
* Uses the traversal methods given:
i. In-order if = 2 children  
ii. Pre-order if > 2 children  
iii. Post-order if < 2 children  

### Part 4
* Creates an Abstract Syntax Tree (AST) using the tokens based on the Backus-Naur Form (BNF)
i. If int a; a = a + 5; , then the AST is  
ii. ; ~ int ~ a  
      ~ = ~ a  
          ~ + ~ a  
              ~ 5  
             
### Part 5
* Evaluate the tree given using the AST
* Uses the evaluation_map (map class from std library).
