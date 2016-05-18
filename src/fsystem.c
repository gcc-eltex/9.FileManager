#include "../headers/shared.h"

//Получает содержание директории
unsigned dir_get(char *name, struct dline **content)
{
	DIR *dir;
	struct stat infodir;
	struct dirent *subdir;
	int i;

//WARING 
	*content = malloc(sizeof(struct dline) * 256);	//УТЕЧКА ПАМЯТИ И ВОЗМОЖНО ПЕРЕПОЛНЕНИЕ. ИСПРАВИТЬ
//WARING

	//Получаем содержимое директории
	dir = opendir(name);
	subdir = readdir(dir);
	for(i = 1; subdir != NULL; subdir = readdir(dir), i++)
	{
		stat(subdir->d_name, &infodir);	//Вытаскивает подробную информацию о файле
		//Если это ссылка на себя, то игнорируем
		if(!strcmp(subdir->d_name,"."))
		{
			i--;
			continue;
		}
		//Если ссылка на родителя, то ставим в начало
		if(!strcmp(subdir->d_name,".."))
		{
			strcpy((*content)[0].name, subdir->d_name);
			strcpy((*content)[0].size, "-UP-");
			strftime((*content)[0].cdate, 32, "%B %d %H:%M", gmtime(&(infodir.st_ctime)));
			(*content)[0].type = subdir->d_type;
		}
		//Иначе попарядку пишем
		else
		{
			strcpy((*content)[i].name, subdir->d_name);
			sprintf((*content)[i].size, "%d", (int)infodir.st_size);
			strftime((*content)[i].cdate, 32, "%B %d %H:%M", gmtime(&(infodir.st_ctime)));
			(*content)[i].type = subdir->d_type;
		}
	}
	printf("\n\n");
	closedir(dir);
	return i;
}
