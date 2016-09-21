#pragma once

typedef struct node{

	int x, y;
	int g;
	int h;
	int f;
	node* pre;
	node* next;
}Node;

class AStar
{
	int mWidth;
	int mHeight;

	//
	char (*map)[50];
	//

	Node start, goal;
	Node open, close;

	Node *openPtr, *closePtr;
	Node *startPtr, *goalPtr;

	Node *current;

public:
	AStar(int mWidth, int mHeight, char map[][50]);
	~AStar(void);

	void init(void);
	int compute_h(Node *s);
	int move(void);
	void initNode(Node *node);
	void initNode(Node *node, int x, int y);
	Node* createChild(int x, int y);
	int setChilds(void);
	bool isSame(Node* aNode, Node* bNode);
	int setOpen(Node* nNode);
	int setCurrent(void);
	int checkClose(Node* cNode);
	int setClose(Node* cNode);
	int setLoad(void);
	int freeAllNode(void);
};

