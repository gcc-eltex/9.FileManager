#include "../headers/shared.h"

//Делает активным правое окно
void action_winactive(unsigned winindex)
{
	if(winindex == win_active)
		return;
	dir_print = 0;
	dir_inwin = 0;
	win_active = winindex;
	winref_all();
}

//Перемещает курсор активной папки вверх
void action_mvup()
{
	//Если дошли до начала диретории
	if(dir_print + dir_inwin <= 0)
		return;
	//Если дошли до начала окна
	if(dir_inwin <= 0)
		dir_print--;
	else
		dir_inwin--;
	winref_content(win_active);
}

//Перемещает курсор активной папки вниз
void action_mvdown()
{
	int width;						//Ширина окна
	int height; 					//Высота окна

	getmaxyx(win[win_active], height, width);
	//Если дошли до конца диретории
	if(dir_print + dir_inwin >= dir_count[win_active] - 1)
		return;
	//Если дошли до конца окна
	if(dir_inwin >= height - 1)
		dir_print++;
	else
		dir_inwin++;
	winref_content(win_active);
}

//Открывает директорию/файл
void action_open()
{
	struct dline nextdir;
	char path[1024];

	nextdir = dir_content[win_active][dir_print + dir_inwin];
	sprintf(path, "%s/%s", dir_patch[win_active], nextdir.name);
	if(nextdir.type == DT_DIR)	//Если это папка, то переходим в нее
	{
		dir_print = 0;
		dir_inwin = 0;
		chdir(path);
		getcwd(dir_patch[win_active], 1024);
		dir_count[win_active] = dir_get(path, &(dir_content[win_active]));
		winref_border(win_active + 2);
		winref_content(win_active);
	}
}