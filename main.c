#include "./headers/shared.h"

int main()
{
	int key = 0;
	//setlocale(LC_ALL, "");
	wininit();
	
	while((key = getch()) != 27)
	{
		switch(key)
		{
			case KEY_RIGHT:
				action_winactive(WIN_R);
			break;
			case KEY_LEFT:
				action_winactive(WIN_L);
			break;
			case KEY_DOWN:
				action_mvdown();
			break;
			case KEY_UP:
				action_mvup();
			break;
			case '\n':
				action_open();
			break;
		}
	}

	windestroy();
}