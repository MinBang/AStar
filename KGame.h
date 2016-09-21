#pragma once
class KGame
{
	HWND hWnd;

public:
	KGame(void);
	~KGame(void);

	void Init(HWND hWnd);
	void Draw(HDC hdc);
	void MouseEvent(int param, int xpos, int ypos);
	void Timer(int wParam);
	void KeyEvent(int wParam);
	int initMap(void);
};

