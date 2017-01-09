//============================================================================
//
//% Student Name 1: Wing Aung
//% Student 1 #: 301291027
//% Student 1 userid (email): raung (raung@sfu.ca)
//
//% Student Name 2: Nicholas Lau
//% Student 2 #: 301290893
//% Student 2 userid (email): nla52 (nla52@sfu.ca)
//
//% Below, edit to list any people who helped you with the code in this file,
//%      or put ‘none’ if nobody helped (the two of) you.
//
// Helpers: _everybody helped us/me with the assignment (list names or put ‘none’)__ Professor Craig, Shawn Thai, Stan H.
//
// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources:  cplusplus.com,
//
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in any other files you are also submitting.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "P3_<userid1>_<userid2>" (eg. P3_stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit files to courses.cs.sfu.ca
//
// Name        : Directory.hpp
// Description : Course Project Part 3 (Directory Tree)
//============================================================================


#ifndef SRC_DIRECTORY_HPP_
#define SRC_DIRECTORY_HPP_

#include "TreeNode.hpp"
#include <iomanip>
#include <string>
#include <vector>

class Directory : public ensc251::TreeNode
{
	typedef std::vector<std::string> stringVect;

	private:
		std::string dir_name;
		stringVect file_names;

	public:
		Directory();
		Directory(std::string m_dir_name): dir_name(m_dir_name) {}

		Directory(std::string m_dir_name, stringVect m_file_names): dir_name(m_dir_name), file_names(m_file_names) {}
		// Clone the directory
		//input: none
		//output: the new directory as a copy
		virtual Directory* clone() const
		{
			 return new Directory(*this);
		};

		void set_dir_name(const std::string& m_dir_name){ dir_name = m_dir_name;};

		const std::string & get_dir_name() { return dir_name; }
		//Adds the file to the filename vector
		//input : filename to be added
		//output: new vector by ref with additional filename
		void add_file(const std::string& m_fileName) { file_names.push_back(m_fileName); }

		void add_multiple_files(const stringVect& m_files) { file_names.insert(file_names.end(), m_files.begin(), m_files.end()); }

		const stringVect & get_files() const { return file_names; }

		TreeNode* print_action()
		{
			// Do not modify insertion on OUT in this member function!
			OUT << std::setw(20) << dir_name+"\t|" << " ";
			OUT << this->childPVector.size() << "  ";
			for (auto file : file_names)
			{
				OUT << "  -" << file;
			}
			OUT << std::endl;
		    return nullptr;
		}
		//Recursive function that prints the desired tree using different techniques of going through nodes
		//input : tree
		//output: none
		void print_traverse(const TreeNode* print_tree)
		{
			// ***** this needs work *****
			// ***** encode the rules in the instructions
			if (childPVector.size() < 2) // when less than 2 children, use post-order
				traverse_children_post_order(
						static_cast<ensc251::traverse_func>(print_traverse),
						static_cast<ensc251::action_func>(print_action));
			else if (childPVector.size() == 2) // when 2 children, use in-order
				traverse_children_in_order(static_cast<ensc251::traverse_func>(print_traverse),
						static_cast<ensc251::action_func>(print_action));
			else if (childPVector.size() > 2) // when more than 2 children, use pre-order
					traverse_children_pre_order(static_cast<ensc251::traverse_func>(print_traverse),
						static_cast<ensc251::action_func>(print_action));


		}
};

#endif /* SRC_DIRECTORY_HPP_ */
