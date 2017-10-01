/** Balanced Binary Tree - BinaryTree.h
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
#pragma once
#include <iostream>
#include <stdio.h>
#include "PrettyPrint.h"

struct TreeNode
{
	int data;
	TreeNode* left = NULL;
	TreeNode* right = NULL;

	TreeNode(int paramData, TreeNode* paramLeft, TreeNode* paramRight)
	{
		data = paramData;
		left = paramLeft;
		right = paramRight;
	}
};

class BinaryTree
{
private:
	TreeNode* myRoot;

	///Private functions
	void AddItemToTree(TreeNode*& root, int data);

public:
	///Constructor
	BinaryTree();

	///Destructor
	virtual ~BinaryTree();

	///Functions
	void AddItemToTree(int data);
	void AddItemsToTree(int* nums, int numberOfItems);
	void PrintTree(int preOrderInOrderOrPostOrder);
	void PrintTree(TreeNode* root, int preOrderInOrderOrPostOrder);
	TreeNode* RotateSubtreeRight(TreeNode* subroot);
	TreeNode* RotateSubtreeLeft(TreeNode* subroot);
	TreeNode* ConvertToList();
	TreeNode* ConvertToList(TreeNode* root);
	int CalculateListLength();
	int CalculateListLength(TreeNode* list);
	TreeNode* ConvertToTree();
	TreeNode* ConvertToTree(TreeNode* list);
	void BalanceTree(bool printTrees);
	void BalanceTree(TreeNode* root, bool printTrees);
	void ClearTree();
	void ClearTree(TreeNode* root);

	///Getters
	TreeNode* GetRoot();

	///Setters
	void SetRoot(TreeNode* root);
};
