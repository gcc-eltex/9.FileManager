# Файловый менеджер
Позволяет перемещаться по директориям и просматривать их содержимое

Обзор проекта:
file_manager.c - Главный файл

headers/ - Заголовочные файлы
	fmanager.h - Содержит прототипы функций реализованных в данном проекте
	shared.h - Файл с необходимыми includ

src/ - Исходный код:
	action.c - Обработка событий (нажатия клавиш)
	window.c - Обновление и вывод на экран
	fsystem.c - Взаимодействие с файловой системой
