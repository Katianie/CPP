/** Balanced Binary Tree - BinaryTree.cpp
* This is an implementation of a balanced binary tree.
* You can add a bunch of data at once and then balance,
* the whole tree once instead of balancing after each insertion.
*
* Having an unbalanced tree causes the data structure to become
* as inneficent as a regular linked list.
*
* This was uploaded to Katianie.com, Feel free to use this
* code and share it with others, Just give me credit ^_^.
*
* Eddie O'Hagan
* Copyright © 2016 Katianie.com
*/
#include "BinaryTree.h"

using namespace std;

///Constructor
BinaryTree::BinaryTree()
{
	myRoot = NULL;
}

///Destructor
BinaryTree::~BinaryTree()
{
	this->ClearTree();
}

///Functions
void BinaryTree::AddItemToTree(int data)
{
	this->AddItemToTree(myRoot, data);
}

void BinaryTree::AddItemToTree(TreeNode*& root, int data)
{
	if (root == NULL)
	{
		root = new TreeNode(data, NULL, NULL);
	}
	else
	{
		if (data < root->data)
		{
			if (root->left == NULL)
			{
				root->left = new TreeNode(data, NULL, NULL);
			}
			else
			{
				AddItemToTree(root->left, data);
			}
		}
		else
		{
			if (root->right == NULL)
			{
				root->right = new TreeNode(data, NULL, NULL);
			}
			else
			{
				AddItemToTree(root->right, data);
			}
		}
	}
}

//Add all the items then balance the whole tree.
void BinaryTree::AddItemsToTree(int* nums, int numberOfItems)
{
	if (nums != NULL && numberOfItems > 0)
	{
		for (int i = 0; i < numberOfItems; i++)
		{
			this->AddItemToTree(nums[i]);
		}
	}

	this->BalanceTree(true);
}

void BinaryTree::PrintTree(int preOrderInOrderOrPostOrder)
{
	this->PrintTree(myRoot, preOrderInOrderOrPostOrder);
}

void BinaryTree::PrintTree(TreeNode* root, int preOrderInOrderOrPostOrder)
{
	if (root != NULL)
	{
		if (preOrderInOrderOrPostOrder == 0)
		{
			//PreOrder: VLR
			cout << root->data << endl;
			PrintTree(root->left, preOrderInOrderOrPostOrder);
			PrintTree(root->right, preOrderInOrderOrPostOrder);
		}
		else if (preOrderInOrderOrPostOrder == 1)
		{
			//InOrder: LVR
			PrintTree(root->left, preOrderInOrderOrPostOrder);
			cout << root->data << endl;
			PrintTree(root->right, preOrderInOrderOrPostOrder);
		}
		else if (preOrderInOrderOrPostOrder == 2)
		{
			//PostOrder: LRV
			PrintTree(root->left, preOrderInOrderOrPostOrder);
			PrintTree(root->right, preOrderInOrderOrPostOrder);
			cout << root->data << endl;
		}
	}
}

TreeNode* BinaryTree::RotateSubtreeRight(TreeNode* subroot)
{
	TreeNode* leftNode = subroot->left;
	TreeNode* newTree;
	TreeNode* retVal;
	
	if (leftNode != NULL)
	{
		//Create a subtree with the left most data.
		newTree = new TreeNode(subroot->data, leftNode->right, subroot->right);

		//Delete the old subtree thats about to be replaced.
		delete subroot;

		//Create the new root using the left subtree we just created and all right side data.
		retVal = new TreeNode(leftNode->data, leftNode->left, newTree);

		return retVal;
	}

	return NULL;
}

TreeNode* BinaryTree::RotateSubtreeLeft(TreeNode* subroot)
{
	TreeNode* rightNode = subroot->right;
	TreeNode* newTree;
	TreeNode* retVal;

	if (rightNode != NULL)
	{
		//Create a subtree with the left most data.
		newTree = new TreeNode(subroot->data, subroot->left, rightNode->left);

		//Delete the old subtree thats about to be replaced.
		delete subroot;

		//Create the new root using the left subtree we just created and all right side data.
		retVal = new TreeNode(rightNode->data, newTree, rightNode->right);

		return retVal;
	}

	return subroot;
}

TreeNode* BinaryTree::ConvertToList()
{
	return this->ConvertToList(myRoot);
}

TreeNode* BinaryTree::ConvertToList(TreeNode* root)
{
	TreeNode* currRoot = root;
	if (currRoot != NULL)
	{
		//Keep rotating right if this subtree has a left branch.
		while (currRoot->left != NULL)
		{
			currRoot = RotateSubtreeRight(currRoot);
		}

		//Go down to the next subtree and attach it to the current right branch.
		currRoot->right = ConvertToList(currRoot->right);

		return currRoot;
	}

	return NULL;
}

int BinaryTree::CalculateListLength()
{
	return this->CalculateListLength(myRoot);
}

int BinaryTree::CalculateListLength(TreeNode* root)
{
	//Get the length of the list.
	TreeNode* currRoot = root;
	int length = 0;

	//Determine which branch to go down.
	if (currRoot->left != NULL)
	{
		while (currRoot != NULL)
		{
			currRoot = currRoot->left;
			length++;
		}
	}
	else
	{
		while (currRoot != NULL)
		{
			currRoot = currRoot->right;
			length++;
		}
	}

	return length;
}

TreeNode* BinaryTree::ConvertToTree()
{
	return this->ConvertToTree(myRoot);
}

TreeNode* BinaryTree::ConvertToTree(TreeNode* list)
{
	TreeNode* currRoot = list;
	if (currRoot != NULL)
	{
		currRoot = RotateSubtreeLeft(currRoot);

		currRoot->right = ConvertToTree(currRoot->right);

		return currRoot;
	}

	return NULL;
}

void BinaryTree::BalanceTree(TreeNode* root, bool printTrees)
{
	TreeNode* list;
	TreeNode* balancedTree;
	int length;
	int numLeftRotations;

	if (printTrees == true)
	{
		cout << "Initial Tree" << endl;
		PrettyPrint::Print(root);
	}
	
	//First step is to convert the tree into a list by rotating every node right of its parent.
	list = ConvertToList(root);
	length = CalculateListLength(list);

	//The number of left rotations is used so we know where to stop unwinding the list.
	//Find the closest power of two without going over length.
	numLeftRotations = (log2(length)) / (log2(2));
	numLeftRotations = numLeftRotations - 1;

	balancedTree = list;
	for (int i = 0; i < numLeftRotations; i++)
	{
		balancedTree = ConvertToTree(balancedTree);
	}

	if (printTrees == true)
	{
		cout << "Balanced Tree" << endl;
		PrettyPrint::Print(balancedTree);
	}
}

void BinaryTree::BalanceTree(bool printTrees)
{
	this->BalanceTree(myRoot, printTrees);
}

void BinaryTree::ClearTree()
{
	this->ClearTree(myRoot);
}

void BinaryTree::ClearTree(TreeNode* root)
{
	if (root != NULL)
	{
		//Go to the bottom first, then delete while riding the kick back up.
		this->ClearTree(root->left);
		this->ClearTree(root->right);
		delete root;
	}
}

///Getters
TreeNode* BinaryTree::GetRoot()
{
	return myRoot;
}

///Setters
void BinaryTree::SetRoot(TreeNode* root)
{
	myRoot = root;
}