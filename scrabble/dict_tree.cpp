#include "stdafx.h"

static dict_tree_node * AllocateNode()
{
	dict_tree_node *pNode = (dict_tree_node *)malloc(sizeof(*pNode));
	if (pNode == NULL)
	{
		return NULL;
	}
	pNode->bWordEnd = false;
	pNode->maxElements = CHILD_REALLOC_STEP;
	pNode->usedElements = 0;
	for (int i = 0; i < LETTER_ON_ALPHABET; i++)
	{
		pNode->letters[i] = NULL_LETTER;
	}
	memset(pNode->pChild, 0, sizeof(pNode->pChild));

	return pNode;
}


static int DictTreeAddLetter(dict_tree_node **ppParent, char newLetter, bool WordEnd)
{
	dict_tree_node *pChild;
	if ((*ppParent)->letters[newLetter - 'a'] != NULL_LETTER)
	{
		if (WordEnd)
		{
			(*ppParent)->pChild[(*ppParent)->letters[newLetter - 'a']]->bWordEnd = true;
		}
		return -1;
	}
	pChild = AllocateNode();
	if (pChild == NULL)
	{
		return 0;
	}
	if ((*ppParent)->usedElements == (*ppParent)->maxElements)//if we need to change size of parent
	{
		size_t newSize = sizeof(dict_tree_node) + ((*ppParent)->maxElements - CHILD_REALLOC_STEP + CHILD_REALLOC_STEP) * sizeof(struct dict_tree_node *);//counting size of new parent
		dict_tree_node *pNewParent = (dict_tree_node *)malloc(newSize);
		if (pNewParent == NULL)//check the allocation error
		{
			free(pChild);
			return 0;
		}
		memcpy(pNewParent, *ppParent, sizeof(dict_tree_node) + ((*ppParent)->maxElements - CHILD_REALLOC_STEP) * sizeof(struct dict_tree_node *));//rewrite old parent to new one
		free(*ppParent);//destroy old parent
		*ppParent = pNewParent;//new parent go to positon of old parrent
		(*ppParent)->maxElements += CHILD_REALLOC_STEP;
	}
	(*ppParent)->letters[newLetter - 'a'] = (*ppParent)->usedElements;//write the new leter to the table of offset and create proper pointer
	(*ppParent)->pChild[(*ppParent)->usedElements] = pChild;
	(*ppParent)->usedElements++;
	pChild->bWordEnd = WordEnd;

	return 1;
}

bool DictTreeCreateTree(dict_tree_node **ppRoot)
{
	if (*ppRoot != NULL)
	{
		return false;
	}
	*ppRoot = AllocateNode();
	if (*ppRoot == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool DictTreeAddWord(dict_tree_node **ppRoot,char word[], int counter)
{
	dict_tree_node **ppCurr = ppRoot;
	bool wordEnd = false;
	for (int i = 0; i < counter; i++)
	{
		int status;
		if (i == counter - 1)//if it is the last letter of word, we set proper value
		{
			wordEnd = true;
		}
		status = DictTreeAddLetter(ppCurr, word[i], wordEnd);//we add new letter and get the status
		if (status == 0)//if allocation error, we return false
		{
			return false;
		}
		else
		{
			ppCurr = &((*ppCurr)->pChild[(*ppCurr)->letters[word[i] - 'a']]);
		}
	}
	return true;
}


bool DictTreeFindWord(dict_tree_node *pRoot, char word[], int counter)
{
	dict_tree_node* curr=NULL;
	if ((pRoot->letters[word[0] - 'a']) == NULL_LETTER)//if the first letter don't exist, retuurn false
	{
		return false;
	}
	else
	{
		curr = pRoot->pChild[pRoot->letters[word[0] - 'a']];
	}
	for (int i = 0; i < counter; i++)
	{
		if (word[i]<'a' || word[i]>'z')
		{
			return false;
		}
	}
	for (int i = 0; i < counter-1; i++)
	{
		if (curr == NULL)
		{
			return false;
		}
		if ((curr->letters[word[i+1] - 'a']) == NULL_LETTER)//if any letter don't exist, return false
		{
			return false;
		}
		else
		{
			curr=curr->pChild[curr->letters[word[i+1] - 'a']];
		}
	}
	if (curr == NULL)
	{
		return false;
	}
	if (curr->bWordEnd == true)//if the last letter of word have parameter word end true, we return true
	{
		return true;
	}
	else
	{
		return false;
	}
}


void DictTreeRealease(dict_tree_node *pNode)
{
	for (int i = 0; i < pNode->usedElements; i++)
	{
		if (pNode->pChild[i] != NULL)
		{
			DictTreeRealease(pNode->pChild[i]);
		}
	}
	free(pNode);
}