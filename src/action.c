#include "../headers/shared.h"

//Делает активным правое окно
void action_winactive(unsigned index)
{
	//Если окно итак активно
	if(index == win_active)
		return;
	//Смеа директории и сброс значений
	dir_print = 0;
	dir_inwin = 0;
	win_active = index;
	winref_all();
}

//Перемещает курсор активной папки вверх
void action_mvup()
{
	//Если дошли до начала директории
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
	struct dline newdir;
	char path[1024];

	//К текущей директории прибавляем имя директории в которую переходим
	newdir = dir_content[win_active][dir_print + dir_inwin];
	sprintf(path, "%s/%s", dir_patch[win_active], newdir.name);

	//Если это папка, то переходим в нее
	if(newdir.type == DT_DIR)
	{
		dir_print = 0;
		dir_inwin = 0;

		//Изменение рабочей директории
		chdir(path);
		getcwd(dir_patch[win_active], 1024);
		
		//Получение ее содержимого и вывод
		dir_count[win_active] = dir_get(path, &(dir_content[win_active]));
		winref_border(win_active + 2);
		winref_content(win_active);
	}
}