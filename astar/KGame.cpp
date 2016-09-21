#include "stdafx.h"
#include "KGame.h"
#include "AStar.h"

#define MOUSE_LDOWN  WM_LBUTTONDOWN
#define MOUSE_RDOWN WM_RBUTTONDOWN

#define KEY_SPACE VK_SPACE
#define KEY_ENTER VK_RETURN

#define MAP_SIZE 50
#define MAP_SOURCE       1
#define MAP_DESTINATION  2
#define MAP_OBSTACLE     3

char map[MAP_SIZE][MAP_SIZE] = {0, };

int size = 40;
int state = 0;

KGame::KGame(void)
{
}


KGame::~KGame(void)
{
}


void KGame::Init(HWND hWnd)
{
	this->hWnd = hWnd;

	SetTimer(hWnd, 1, 16, NULL);
}


void KGame::Draw(HDC hdc)
{
	HBRUSH nBrush = NULL, oBrush = NULL;

	for(int i=0;i<MAP_SIZE;i++){

		for(int j=0;j<MAP_SIZE;j++){

			int sx = j * size;
			int sy = i * size;

			if(map[i][j] == MAP_SOURCE){

				nBrush = CreateSolidBrush(RGB(0, 255, 0));
				oBrush = (HBRUSH)SelectObject(hdc, nBrush);
			}else if(map[i][j] == MAP_DESTINATION){

				nBrush = CreateSolidBrush(RGB(255, 0, 0));
				oBrush = (HBRUSH)SelectObject(hdc, nBrush);
			}else if(map[i][j] == MAP_OBSTACLE){

				nBrush = CreateSolidBrush(RGB(0, 0, 255));
				oBrush = (HBRUSH)SelectObject(hdc, nBrush);
			}else if(map[i][j] == 20){

				nBrush = CreateSolidBrush(RGB(0, 255, 255));
				oBrush = (HBRUSH)SelectObject(hdc, nBrush);
			}

			Rectangle(hdc, sx, sy, sx + size, sy + size);

			if(nBrush != NULL){

				SelectObject(hdc, oBrush);
				DeleteObject(nBrush);
			}
		}
	}
}

void KGame::MouseEvent(int param, int xpos, int ypos)
{
	int x = xpos / size;
	int y = ypos / size;

	switch(param){
	case MOUSE_LDOWN:

		if(map[y][x] != 0){

			if(map[y][x] == MAP_SOURCE){
				state = 0;
			}

			map[y][x] = 0;

			break;
		}

		if(state == 0){
			map[y][x] = MAP_SOURCE;

			state = 1;
		}else{
			map[y][x] = MAP_OBSTACLE;
		}		

		break;

	case MOUSE_RDOWN:

		map[y][x] = MAP_DESTINATION;

		break;

	default:
		break;
	}
}


void KGame::Timer(int wParam)
{
	if(1 == wParam){
		InvalidateRect(hWnd, NULL, FALSE);
	}
}


void KGame::KeyEvent(int wParam)
{
	switch(wParam){

	case KEY_SPACE:
	{
		AStar aStar(MAP_SIZE, MAP_SIZE, map);
		if(aStar.move()){

			MessageBox(hWnd, TEXT("SUC"), TEXT("HI"), 0);
		}

		break;
	}
	case KEY_ENTER:
		
		initMap();

		break;
	default:
		break;
	}
}


int KGame::initMap(void)
{
	for(int i=0;i<MAP_SIZE;i++){

		for(int j=0;j<MAP_SIZE;j++){

			map[i][j] = 0;
		}
	}

	state = 0;

	return 0;
}
