#include<iostream>
#include<random>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<fstream>

#include"Employee.hpp"

int main() {
	// ------------------------ Шаг 1 - Заполняем исходный вектор работниками ===========================
	unsigned int quantity;
	std::cout << "Enter the quantity of employees -> ";
	std::cin >> quantity;
	// Контейнер vector для хранения работников в динамическом массиве для возможности быстрого доступа к данным каждого работника
	std::vector<Employee> Employees(quantity);
	// Чтобы постоянно не перевыделять динамическую память при заполнении вектора, выделяем память под quantity кол-во элементов
	Employees.reserve(quantity);
	// Подключаем генератор случайных чисел
	std::random_device rd; // Источник иистинно случайных чисел
	std::mt19937 provider(rd()); // Генератор псевдослучайных чисел с ядром rd()
	std::uniform_int_distribution<int> distributor(_MIN_SALARY, _MAX_SALARY); // Доставщик рандомной зарплаты	
	srand(time(NULL));
	// Заполняем вектор с работниками данными
	for (int i = 0; i < quantity; ++i) {
		Employees[i].name_ = "Employee " + std::to_string(i + 1);
		Employees[i].number_ = i + 1;
		Employees[i].salary_ = distributor(provider);
		Employees[i].shift_ = rand() % 2;
	}

	// ------------------------ Шаг 2 - Выводим в консоль дневную и ночную смены ===========================
	std::cout << std::endl << "\033[93mDay shift:\033[0m" << std::endl;
	Print(Employees, true); // Выводим в консоль дневную смену
	std::cout << std::endl << "\033[93mNight shift:\033[0m" << std::endl;
	Print(Employees, false); // Выводим в консоль ночную смену	

	// ------------------------ Шаг 3 - Формируем векторы с максимальными зарплатами ===========================
	std::vector<Employee> Day(3); // Вектор для сотрудников дневной смены с максимальным значением зарплаты
	Day.reserve(3); // Чтобы постоянно не перевыделять динамическую память при заполнении вектора, выделяем память под 3-ёх сотрудников
	std::vector<Employee> Night(3); // Вектор для сотрудников ночной смены с максимальным значением зарплаты
	Night.reserve(3);
	std::vector<Employee> All_Max(6); // Вектор сотрудников из всех смен с максимальным значением зарплаты
	All_Max.reserve(6); // Изначально он будет состоять из 6 человек (по 3 со смены)		
	Max_Salary(Employees, Day, true); // Заполняем ветор сотрудниками дневной смены с максимальными зарплатами
	Max_Salary(Employees, Night, false); // Заполняем ветор с сотрудниками ночной смены с максимальными зарплатами	
	Fill_in(All_Max, Day, Night); // Заполняем вектор сотрудников всех смен с максимальныйми зарплатами 
	// Сокращаем вектор до 3-ёх человек с наимбольшими зарплатами
	int salary; // Переменная для хранения зарплаты
	int index; // Переменная для хранения индекса вектора с нужным значением зарплаты
	int it = 0; // Переменная для хранения итератора
	while (All_Max.size() > 3) {
		salary = _MAX_SALARY;
		it = 0;
		for (Employee& el : All_Max) {
			if (el.salary_ < salary) {
				salary = el.salary_;
				index = it;
			}
			++it;
		}
		All_Max.erase(All_Max.begin() + index); // Удаляем из вектора элемент с наименьшей зарплатой
	}
	Sort(All_Max, true); // Сортируем вектор по убыванию зарплат

	// ------------------------ Шаг 4 - Выводим в консоль сотрудников с максимальными зарплатами ===========================
	std::cout << std::endl << "\033[93mDay shift employees with maximum salaries:\033[0m" << std::endl;
	Print(Day); // Дневная смена
	std::cout << std::endl << "\033[93mNight shift employees with maximum salaries:\033[0m" << std::endl;
	Print(Night); // Ночная смена
	std::cout << std::endl << "\033[93mEmployees of all shifts with maximum salaries:\033[0m" << std::endl;
	Print(All_Max); // Все смены

	// ------------------------ Шаг 5 - Запись в файл сотрудников с максимальными зарплатами ===========================
	std::string path = "Result.txt"; // Относительный путь к файлу
	std::ofstream out; // Создаём объект для записи в файл
	std::string str; // Переменная для построчной записи/чтения информации из файла
	out.open(path, std::ios::out); // Открываем файл для записи
	if (out.is_open()) // Если файл открыт для чтения, то
		std::cout << std::endl << "File \033[93m" << path << "\033[0m is open for rewriting." << std::endl;
	else { // Если файл не получилось открыть
		std::cout << "\033[94mError opening file!\033[0m\n";
		system("pause");
		throw - 1; // Кидаем исключение
	}
	// Записываем данные в файл
	out << "Day shift employees with maximum salaries:" << '\n';
	for (Employee& el : Day)
		out << el.number_ << '\t' << el.name_ << '\t' << el.salary_ << " rubles, day shift" << '\n';
	out << '\n' << "Night shift employees with maximum salaries:" << '\n';
	for (Employee& el : Night)
		out << el.number_ << '\t' << el.name_ << '\t' << el.salary_ << " rubles, night shift" << '\n';
	out << '\n' << "Employees of all shifts with maximum salary:" << '\n';
	for (Employee& el : All_Max)
		if (el.shift_)
			out << el.number_ << '\t' << el.name_ << '\t' << el.salary_ << " rubles, day shift" << '\n';
		else
			out << el.number_ << '\t' << el.name_ << '\t' << el.salary_ << " rubles, night shift" << '\n';
	std::cout << "The information has been successfully added into file \033[93m" << path << "\033[0m";
	out.close(); // Закрываем перезаписанный файл
	std::cout << std::endl << "File \033[93m" << path << "\033[0m is closed!" << std::endl;

	// ------------------------ Шаг 6 - Формируем векторы с минимальными зарплатами ===========================
	// Заполняем ветор сотрудниками дневной смены с максимальными зарплатами
	Min_Salary(Employees, Day, true);
	// Заполняем ветор с сотрудниками ночной смены с минимальными зарплатами
	Min_Salary(Employees, Night, false);
	std::vector<Employee> All_Min(6);
	All_Min.reserve(6);
	Fill_in(All_Min, Day, Night);  // Заполняем вектор сотрудников всех смен с минимальныйми зарплатами
	// Сокращаем вектор до 3-ёх человек с наимбольшими зарплатами
	it = 0;
	while (All_Min.size() > 3) {
		salary = _MIN_SALARY;
		it = 0;
		for (Employee& el : All_Min) {
			if (el.salary_ > salary) {
				salary = el.salary_;
				index = it;
			}
			++it;
		}
		All_Min.erase(All_Min.begin() + index); // Удаляем из вектора элемент с наибольшей зарплатой
	}
	Sort(All_Min, false); // Сортируем вектор по возрастанию зарплат

	// ------------------------ Шаг 7 - Выводим в консоль сотрудников с минимальными зарплатами ===========================	
	std::cout << std::endl << "\033[93mDay shift employees with minimal salaries:\033[0m" << std::endl;
	Print(Day); // Дневная смена
	std::cout << std::endl << "\033[93mNight shift employees with minimal salaries:\033[0m" << std::endl;
	Print(Night); // Ночная смена
	std::cout << std::endl << "\033[93mEmployees of all shifts with minimal salaries:\033[0m" << std::endl;
	Print(All_Min); // Все смены

	// ------------------------ Шаг 8 - Дописываем в файл сотрудников с минимальными зарплатами ===========================
	out.open(path, std::ios::app); // Открываем файл для дозаписи
	if (out.is_open()) // Если файл открыт для чтения, то
		std::cout << std::endl << "File \033[93m" << path << "\033[0m is open for adding." << std::endl;
	else { // Если файл не получилось открыть
		std::cout << "\033[94mError opening file!\033[0m\n";
		system("pause");
		throw - 1; // Кидаем исключение
	}
	// Записываем данные в файл
	out << '\n' << "Day shift employees with minimal salaries:" << '\n';
	for (Employee& el : Day)
		out << el.number_ << '\t' << el.name_ << '\t' << el.salary_ << " rubles, day shift" << '\n';
	out << '\n' << "Night shift employees with minimal salaries:" << '\n';
	for (Employee& el : Night)
		out << el.number_ << '\t' << el.name_ << '\t' << el.salary_ << " rubles, night shift" << '\n';
	out << '\n' << "Employees of all shifts with minimal salary:" << '\n';
	for (Employee& el : All_Min)
		if (el.shift_)
			out << el.number_ << '\t' << el.name_ << '\t' << el.salary_ << " rubles, day shift" << '\n';
		else
			out << el.number_ << '\t' << el.name_ << '\t' << el.salary_ << " rubles, night shift" << '\n';
	std::cout << "The information has been successfully added into file \033[93m" << path << "\033[0m";
	out.close(); // Закрываем перезаписанный файл
	std::cout << std::endl << "File \033[93m" << path << "\033[0m is closed!" << std::endl;

	// ------------------------ Шаг 9 - Считываем из файла работников в новую коллекцию ===========================

	std::vector<Employee> Final; // Rоллекция с объектами структуры, считанными из файла
	std::vector<Employee> Unique; // Коллекция для хранения уникальных сотрудников по условию задачи
	std::ifstream in; // Объект для считывания данных из файла
	in.open(path, std::ios::in); // Открываем файл для считывания 
	if (in.is_open()) // Если файл открыт для чтения, то
		std::cout << std::endl << "File \033[93m" << path << "\033[0m is open for reading." << std::endl;
	else { // Если файл не получилось открыть
		std::cout << "\033[94mError opening file!\033[0m\n";
		system("pause");
		throw - 1; // Кидаем исключение
	}
	// Построчно считываем информацию из файла		
	//std::cin.ignore();	
	{ // Локальная область видимость для перменных tmp и pos
		Employee tmp; // Создаём объект структуры Employee для временного хранения считанной из файла информации
		int pos; // Переменная для хранения позиции строки с символом-разделителем ('\t')
		while (std::getline(in, str)) {
			// Если считанная строчка начинается с номера сотрудника, т.е. считанная строчка - это точно сотрудник, то
			// Проверка по ASCII-коду первого символа считанной строки, если число, то считываем из файла
			if (str[0] >= 48 && str[0] <= 57) {
				pos = str.find('\t'); // Находим первое вхождение в строку символа '\t' 
				tmp.number_ = stoi(str.substr(0, pos)); // Считываем во временный объект структуры номер сотрудника			
				str.erase(0, pos + 1); // Удаляем из строки номер сотрудника и символ-разделитель '\t'
				pos = str.find('\t'); // Находим следующее вхождение в обрезанную от номера сотрудника строку символа '\t'
				tmp.name_ = str.substr(0, pos); // Считываем во временный объект структуры имя сотрудника
				str.erase(0, pos); // Удаляем из строки имя сотрудника и символ-разделитель '\t'
				tmp.salary_ = stoi(str); // Считываем во временный объект структуры зарплату сотрудника
				if (str.find("day") != std::string::npos) // Если в строке str есть слово "day"
					tmp.shift_ = true;
				else
					tmp.shift_ = false;
				Final.push_back(tmp); // Увеличиваем вектор на новый объект структуры
			}
		}
	}
	in.close();
	std::cout << std::endl << "File \033[93m" << path << "\033[0m is closed!" << std::endl << std::endl;

	// ------------------------ Шаг 10 - Вывод в консоль уникальной коллекции сотрудников ===========================
	// Циклы для формирования уникальной коллекции
	for (auto& el : Final) {
		bool flag = true;
		for (auto& unique_el : Unique) {
			if (el.number_ == unique_el.number_) {
				flag = false;
				break; // Прерываем вложенный цикл
			}
		}
		if (flag)
			Unique.push_back(el);
	}
	std::cout << "\033[92mUnique collection of employees is:\033[0m" << std::endl;
	// Сортируем вектор по возрастанию уникального номера сотрудника пузырьковой сортировкой
	Employee tmp; // Временный объект структуры Employee
	for (int i = Unique.size() - 1; i > 0; --i)
		for (int j = 0; j < i; ++j)
			if (Unique.at(j).number_ > Unique.at(j + 1).number_) {
				tmp = Unique.at(j);
				Unique.at(j) = Unique.at(j + 1);
				Unique.at(j + 1) = tmp;
			}
	Print(Unique); // Выводим в консоль уникальную коллекцию с работниками

	return 0;
}