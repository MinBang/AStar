#include "stdafx.h"
#include "AStar.h"

#define MAP_SIZE 20
#define MAP_SOURCE       1
#define MAP_DESTINATION  2
#define MAP_OBSTACLE     3

int comX[8] = {-1, 0, 1, 1, 1, 0, -1, -1}; 
int comY[8] = {-1, -1, -1, 0, 1, 1, 1, 0}; 

AStar::AStar(int mWidth, int mHeight, char map[][50])
: mWidth(0)
, mHeight(0)
{
	this->mWidth = mWidth;
	this->mHeight = mHeight;
	this->map = map;

	init();
}

AStar::~AStar(void)
{
}

void AStar::init(void)
{
	openPtr = &open;
	closePtr = &close;
	startPtr = &start;
	goalPtr = &goal;

	current = startPtr;

	for(int i=0;i<mHeight;i++){
		for(int j=0;j<mWidth;j++){

			if(map[i][j] == MAP_SOURCE){

				startPtr->x = j;
				startPtr->y = i;
			}else if(map[i][j] == MAP_DESTINATION){

				goalPtr->x = j;
				goalPtr->y = i;
			}
		}
	}

	initNode(&open);
	initNode(&close);
	initNode(&start);
	initNode(&goal);
}

int AStar::compute_h(Node* s)
{
	int cx = goalPtr->x - s->x;
	int cy = goalPtr->y - s->y;

	if(cx < 0){

		cx = -cx;
	}

	if(cy < 0){

		cy = -cy;
	}

	return cx + cy;
}

int AStar::move(void)
{
	/*if(map[0][0] != 0){
		return 1;
	}*/

	int tNum = 0;

	//child 생성
	setOpen(&start);

	while(openPtr->next != NULL){

		setCurrent();

		if(isSame(current, goalPtr)){

			break;
		}

		setChilds();		
	}

	setLoad();
	
	freeAllNode();

	return 1;
}


void AStar::initNode(Node* node)
{
	node->pre = NULL;
	node->next = NULL;
	node->g = 0;
	node->h = compute_h(node);
	node->f = node->g + node->h;
}

void AStar::initNode(Node* node, int x, int y)
{
	node->pre = current;
	node->next = NULL;
	node->x = x;
	node->y = y;
	node->g = current->g + 1;
	node->h = compute_h(node);
	node->f = node->g + node->h;
}

Node* AStar::createChild(int x, int y)
{
	if(x < 0 || y < 0){
		return NULL;
	}

	//갈수 있으면 길을 만든다.
	Node* tNode = NULL;

	if(map[y][x] == 0 || map[y][x] == MAP_DESTINATION){

		tNode = (Node*)malloc(sizeof(Node));
		initNode(tNode, x, y);
	}
	
	return tNode;
}

int AStar::setChilds(void)
{
	int childNum = 0;
	
	for(int i=0;i<8;i++){
		
		Node* nNode = createChild(current->x + comX[i], current->y + comY[i]);

		if(checkClose(nNode)){

			continue;
		}

		childNum += setOpen(nNode);
	}

	return childNum;
}

bool AStar::isSame(Node* aNode, Node* bNode)
{
	if(aNode->x == bNode->x && aNode->y == bNode->y){
		
		return true;
	}

	return false;
}

int AStar::setOpen(Node* nNode)
{
	if(nNode == NULL){
		
		return 0;
	}

	int re = 1;
	Node* sNode = openPtr;

	//open에 넣는다.
	while(sNode->next != NULL)
	{
		if(isSame(sNode->next, nNode))
		{
			if(nNode->f < sNode->next->f){

				Node* dNode = sNode->next;
				nNode->next = dNode->next;
				sNode->next = nNode;

				free(dNode);
			}

			re = 0;

			break;
		}else if(nNode->f < sNode->next->f){

			Node* tNode = sNode->next;
			nNode->next = tNode;
			sNode->next = nNode;

			break;
		}

		sNode = sNode->next;		
	}

	if(sNode->next == NULL){

		sNode->next = nNode;
	}

	return re;
}

int AStar::checkClose(Node* cNode)
{
	if(cNode == NULL){
		
		return 0;
	}

	Node* sNode = closePtr;
	cNode->next = NULL;
	int re = 0;

	//close에 넣는다.
	while(sNode->next != NULL)
	{
		if(isSame(sNode->next, cNode))
		{
			if(cNode->f < sNode->next->f){

				Node* dNode = sNode->next;
				cNode->next = dNode->next;
				sNode->next = cNode;

				free(dNode);
			}

			re++;

			break;
		}

		sNode = sNode->next;		
	}

	return re;
}

int AStar::setCurrent(void)
{
	if(current != startPtr){

		setClose(current);
	}

	current = openPtr->next;
	openPtr->next = openPtr->next->next;

	return 0;
}

int AStar::setClose(Node* cNode)
{
	if(cNode == NULL){
		
		return 0;
	}

	Node* tNode = closePtr;

	while(tNode->next != NULL){

		if(cNode->f < tNode->next->f){

			break;
		}

		tNode = tNode->next;
	}

	cNode->next = tNode->next;
	tNode->next = cNode;

	return 0;
}

int AStar::setLoad(void)
{
	if(openPtr->next == NULL){

		return 0;
	}

	Node* tNode = current->pre;

	while(tNode->pre != NULL){

		int x = tNode->x;
		int y = tNode->y;

		map[y][x] = 20;

		tNode = tNode->pre;
	}

	return 0;
}


int AStar::freeAllNode(void)
{
	Node* dNode = NULL;
	Node* tNode = NULL;

	dNode = openPtr->next;
	
	while(dNode != NULL){

		tNode = dNode;
		dNode = dNode->next;
		free(tNode);
	}

	dNode = closePtr->next;

	while(dNode != NULL){

		tNode = dNode;
		dNode = dNode->next;
		free(tNode);
	}

	return 0;
}
