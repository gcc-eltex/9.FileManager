#include "./headers/shared.h"

int main()
{
	int key = 0;
	setlocale(LC_ALL, "");
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
	for(int i = 0; i < dir_count[win_active]; i++)
		printf("%s\n", dir_content[win_active][i].name);
}