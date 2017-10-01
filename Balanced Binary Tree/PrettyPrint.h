//Original author of ASCII print: ponnada
//http://web.archive.org/web/20090617110918/http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/
#pragma once
#include <iostream>
#include <stdio.h>

#define MAX_HEIGHT 1000

struct TreeNode;
typedef struct asciinode_struct asciinode;

struct asciinode_struct
{
	asciinode* left, *right;

	//length of the edge from this node to its children
	int edge_length;

	int height;

	int lablen;

	//-1=I am left, 0=I am root, 1=right   
	int parent_dir;

	//max supported unit32 in dec, 10 digits max
	char label[11];
};

class PrettyPrint
{
private:
	int lprofile[MAX_HEIGHT];
	int rprofile[MAX_HEIGHT];

	//adjust gap between left and right nodes
	int gap = 3;

	//used for printing next node in the same level, 
	//this is the x coordinate of the next char printed
	int print_next;

public:
	static PrettyPrint& RetrievePrettyPrint();

	///Functions
	int MIN(int X, int Y);
	int MAX(int X, int Y);
	asciinode* build_ascii_tree_recursive(TreeNode* t);
	asciinode* build_ascii_tree(TreeNode* t);
	void free_ascii_tree(asciinode* node);
	void compute_lprofile(asciinode* node, int x, int y);
	void compute_rprofile(asciinode* node, int x, int y);
	void compute_edge_lengths(asciinode* node);
	void print_level(asciinode* node, int x, int level);
	void prettyPrint(TreeNode* t);
	static void Print(TreeNode* t);
};
