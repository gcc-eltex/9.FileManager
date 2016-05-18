#include "../headers/shared.h"

//Инициальзирует все переменные и графику
void wininit()	
{
	//Инициализируем все переменные
	dir_print = 0;
	dir_inwin = 0;
	win_active = WIN_L;
	getcwd(dir_patch[WIN_L], 1024);
	getcwd(dir_patch[WIN_R], 1024);
	dir_count[WIN_L] = dir_get(dir_patch[WIN_L], &(dir_content[WIN_L]));
	dir_count[WIN_R] = dir_get(dir_patch[WIN_R], &(dir_content[WIN_R]));

	//Инициализация ncurses
	initscr();						//ncurses mode
	noecho();						//Отключить вывод клвиш
	//cbreak();						//Отключение линейной буферизации
	curs_set(0);
	keypad(stdscr, TRUE);
	signal (SIGWINCH, winresize);

	//Создаем окна
	win[WIN_BL] = newwin((LINES - 2), floor(COLS / 2), 0, 0);
	win[WIN_BR] = newwin((LINES - 2), COLS - floor(COLS / 2), 0, floor(COLS / 2));
	win[WIN_L] = newwin((LINES - 5), floor(COLS / 2 - 2), 2, 1);
	win[WIN_R] = newwin((LINES - 5), COLS - floor(COLS / 2) - 2, 2, floor(COLS / 2) + 1);
	win[WIN_PB] = newwin(2, COLS, LINES - 2, 0);

	//Цвета
	start_color();
	assume_default_colors(COLOR_WHITE,COLOR_BLUE);	//Цвета по умолчанию
	//Цветовые пары
	init_pair(1, COLOR_BLACK, COLOR_CYAN);			//Выбранная директори
	init_pair(2, COLOR_CYAN, COLOR_WHITE);			//Нижняя панель
	init_pair(3, COLOR_YELLOW, COLOR_BLUE);			//Заколовок таблицы
	
	winref_all();
}

//Действие при изменении размера терминала
void winresize()								
{
	struct winsize size;

	//Получаем новые размеры терминала
	ioctl(0, TIOCGWINSZ, &size);
    COLS = size.ws_col;
    LINES = size.ws_row;

    //Очистка экрана
	clear();
	refresh();
	
	//Изменяем размеры окон
	resizeterm(LINES, COLS);
	wresize(win[WIN_BL], (LINES - 2), floor(COLS / 2));
	wresize(win[WIN_BR], (LINES - 2), COLS - floor(COLS / 2));
	wresize(win[WIN_L], (LINES - 5), floor(COLS / 2 - 2));
	wresize(win[WIN_R], (LINES - 5), COLS - floor(COLS / 2) - 2);
	wresize(win[WIN_PB], 2, COLS);

	//Смещаем окна после изменения размера
	mvwin(win[WIN_BR], 0, floor(COLS / 2));
	mvwin(win[WIN_R], 2, floor(COLS / 2) + 1);
	mvwin(win[WIN_PB], LINES - 2, 0);

	//Обновляем 
	winref_all();
}

//Очистка памяти и отключение графики
void windestroy()								
{
	endwin();
}

//Перерисовывает все
void winref_all()
{
	winref_panel(WIN_PB);
	winref_border(WIN_BL);
	winref_border(WIN_BR);
	winref_content(WIN_L);
	winref_content(WIN_R);
}

//Перерисовывает панель
void winref_panel(unsigned index)
{
	wclear(win[index]);
	mvwprintw(win[index], 0, 0, " 9-EXIT");
	refresh();  // ПОЧЕМУ??????
	wrefresh(win[index]);
}

//Перерисовывает окно с содержимым
void winref_content(unsigned index)
{
	int width;						//Ширина окна
	int height; 					//Высота окна
	char pformat[32];				//Формат вывода
	char pdata[256];				//Строка для вывода
	unsigned lname, lsize, ldate;	//Длина каждого параметра

	//Очстка и смещение курсора
	wclear(win[index]);
	wmove(win[index], 0, 0);

	//Определение формата вывода так, чтобы строка занимала все окно
	getmaxyx(win[index], height, width);
	lname = width * 0.5; 
	lsize = width * 0.2 - 2;
	ldate = width - lname - lsize - 5;
	sprintf(pformat, " %%-%d.%ds %%%d.%ds  %%%d.%ds ", lname, lname, lsize, lsize, ldate, ldate);

	//Вывод всего содержимого
	for(int i = 0, j = dir_print; i < height; i++, j++)
	{
		if(i < dir_count[index])	//Если есть что выводить
		{

			sprintf(pdata, pformat, dir_content[index][j].name, dir_content[index][j].size, dir_content[index][j].cdate);
			mvwprintw(win[index], i, 0, pdata);
		}
		//Дорисовка горизонтальных линий
		mvwaddch(win[index], i, lname + 1, ACS_VLINE);
		mvwaddch(win[index], i, lname + lsize + 3, ACS_VLINE);
	}
	wrefresh(win[index]);

	//Выделение выбраной директории если обновляем активное окно
	if(index == win_active)
	{
		wattron(win[index],COLOR_PAIR(1));
		sprintf(pdata, pformat, dir_content[index][dir_inwin + dir_print].name, dir_content[index][dir_inwin + dir_print].size, dir_content[index][dir_inwin + dir_print].cdate);
		mvwprintw(win[index], dir_inwin, 0, pdata);
		//Дорисовка горизонтальных линий
		mvwaddch(win[index], dir_inwin, lname + 1, ACS_VLINE);
		mvwaddch(win[index], dir_inwin, lname + lsize + 3, ACS_VLINE);
		wattroff(win[index],COLOR_PAIR(1));
	}
	//Обновление экрана
	wrefresh(win[index]);
}

//Перерисовывает рамку
void winref_border(unsigned index)
{
	int width;						//Ширина окна
	int height; 					//Высота окна
	char pformat[32];				//Формат вывода
	char pdata[256];				//Строка для вывода
	unsigned lname, lsize, ldate;	//Длина каждого параметра

	//Очстка 
	wclear(win[index]);

	//Определение формата вывода так, чтобы страка занимала все окно
	getmaxyx(win[index], height, width);
	width = width - 2;
	lname = width * 0.5; 
	lsize = width * 0.2 - 2;
	ldate = width - lname - lsize - 7;

	sprintf(pformat, " %%-%d.%ds %%%d.%ds  %%%d.%ds ", lname, lname, lsize, lsize, ldate, ldate);
	sprintf(pdata, pformat, "File name", "Size", "Modify");

	wattron(win[index],COLOR_PAIR(3));
	mvwprintw(win[index], 1, 1, pdata);
	wattroff(win[index],COLOR_PAIR(3));

	refresh();			//ПОЧЕМУ НЕТ РАМКИ БЕЗ ЭТОГО?????????
    box(win[index], 0, 0); //Рисуем рамкy

    //Выводим путь
    lsize = strlen(dir_patch[index - 2]);
    if(lsize > width / 1.5)	//Есди длина пути больше чем есть мета, то урезаем
    	sprintf(pdata, "...%s", dir_patch[index - 2] + lsize - (int)(width / 1.5));
    else
		sprintf(pdata, "%s", dir_patch[index - 2]);
    mvwprintw(win[index], 0, 2, pdata);
    wrefresh(win[index]);
}