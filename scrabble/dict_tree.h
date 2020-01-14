#pragma once

#define MIN_CHILD_ALLOC 4
#define CHILD_REALLOC_STEP 4
#define LETTER_ON_ALPHABET 26
#define NULL_LETTER -1

struct dict_tree_node
{
	bool bWordEnd;
	char maxElements;
	char usedElements;
	char letters[LETTER_ON_ALPHABET];
	struct dict_tree_node *pChild[MIN_CHILD_ALLOC];
};

bool DictTreeCreateTree(dict_tree_node **ppRoot);//a root of tree


bool DictTreeFindWord(dict_tree_node *pRoot, char Word[], int counter);//finding wor, if find-true else false


bool DictTreeAddWord(dict_tree_node **ppRoot, char Word[], int counter);//if word already exist or succesful add true, else false


void DictTreeRealease(dict_tree_node *pRoot);//realease memory