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
//% * Enter the above information in Directory.hpp too.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "P3_<userid1>_<userid2>" (eg. P3_stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit one or two files to courses.cs.sfu.ca
//
// Name        : TreeNode.hpp
// Description : Course Project Part 3 (Directory Tree)
//============================================================================

#ifndef SRC_TREENODE_HPP_
#define SRC_TREENODE_HPP_

#include <iostream>
#include <vector>

//#define USE_OSS

#ifndef USE_OSS
#define OUT std::cout
#else
#include <sstream>
extern std::ostringstream oss;
#define OUT oss
#endif

namespace ensc251 {

	static unsigned count = 0;

	class TreeNode;

	typedef TreeNode* (TreeNode::*action_func)(void);
	typedef void (TreeNode::*traverse_func)(const TreeNode*);

	class TreeNode
	{
		typedef std::vector<TreeNode*> TreeNodePVect;

		protected:
			TreeNodePVect childPVector;

		public:
			TreeNode() {};
			TreeNode(const TreeNode& nodeToCopy) { 
				// ***** this needs work *****
				//if (nodeToCopy.childPVector.size())
				//	add_childP(nodeToCopy.childPVector.at(0)->clone()); -- This only gets the first child
				for (auto child : nodeToCopy.childPVector) // Full child list
				{
					add_childP(child->clone());
				}

			}

			virtual TreeNode* clone() const { return new TreeNode(*this); };
			//Destructor that removes all children
			//input: none
			//output: none/ removal of all children
			virtual ~TreeNode() {
				// do not modify this insertion on OUT
				OUT << "Destroying TreeNode with " << childPVector.size() << " children."<< std::endl;

				for(auto to_delete:childPVector)
				{
					delete to_delete;
				}


			};
			// Swap the inputed node with the current node/child
			// input: the swapping node by reference
			// output: the nodes swapped by reference
		    void swap(TreeNode & other) // the swap member function (should never fail!)
		    {
		    	//std::swap(childPVector,other.childPVector); -- Same purpose as implementation below

		    	TreeNodePVect temp;
		    	temp = childPVector;
		    	childPVector = other.childPVector;
		    	other.childPVector = temp;
		    	// ***** fill this in if desired *****
		    }
		    // Assignment operator
		    // input: treenode to be assigned
		    // output: the new treenode
		    TreeNode & operator = (TreeNode other) // note: argument passed by value, copy made!
		    {
				// ***** Complete this code *****
		    	//childPVector = other.childPVector;
		    	swap(other); // reassign new value using the swap function

		        return *this; // by convention, always return *this
		    }
		    // Adds a child to the childvector
		    // inputs : child to be inputed
		    // outputs: the newly added child inside vector by direct modification of vector pointer
			void add_childP(TreeNode* child) {childPVector.push_back(child);}

			void add_children(const TreeNodePVect& childPV) { childPVector.insert(childPVector.end(), childPV.begin(), childPV.end()); }

			const TreeNodePVect& get_children() const { return childPVector; }

			bool check_null(const TreeNode* testing)
			{
				return testing == nullptr;
			}
			// Traversal by going to child first before parent
			// input: traversal function (recursive function from Directory.hpp and action function (print_action from Directory.hpp)
			// output: return the print_action/action function
			TreeNode* traverse_children_post_order(traverse_func tf, action_func af)
			{
			    for(auto childP : childPVector) {
			        (childP->*tf)(this); //c traverse child's children using tf
			    }
			    return (this->*af)();
			}
			// Traversal by going to parent, then to child
			// input: traversal function (recursive function from Directory.hpp and action function (print_action from Directory.hpp)
			// output: return of the given node
			TreeNode* traverse_children_pre_order(traverse_func tf, action_func af) //RECURSIVE INPUT, PRINT
			{
				// ***** Complete this member function *****
				(this->*af)(); // Output first before going to child(ren)
				for (auto childP:childPVector)
				{
					(childP->*tf)(this);
				}

				/*
				for (auto childP : childPVector)
				{
					(this->*af)();
					(childP->*tf)(this);
					count_action();
					if(childP->get_count != nullptr)
						count_traverse(childP);

				};*/
				return this;
				/*
				for (auto childP : childPVector)
				{
					if (childP == nullptr)
						return 0;
					else
					{
						(this->*af)();
						(childP->*tf)(this);
						return 0;
					}
				}*/
			}
			// Traversal by going to first child, then parent and finally to child
			// input: traversal function (recursive function from Directory.hpp and action function (print_action from Directory.hpp)
			// output: return of the given node
			TreeNode* traverse_children_in_order(traverse_func tf, action_func af)
			{
				// ***** Complete this member function *****
				(childPVector.at(0)->*tf)(this); //First child
				(this->*af)(); // Parent
				(childPVector.at(1)->*tf)(this); //Second child
				return this;
				/*
				for (auto childP : childPVector)
				{
					if (childP == nullptr)
					{
						return this;
					}
					else
					{
						(childP->*tf)(this);
						return (this->*af)();
					}
				}*/
			}


			TreeNode* count_action()
			{
				count++;
				return nullptr;
			}

			void count_traverse(const TreeNode*)
			{
				traverse_children_post_order(count_traverse, count_action);
			}
			void reset_count() { count = 0; }
			unsigned get_count() const { return count; }



	};
}

#endif /* SRC_TREENODE_HPP_ */
