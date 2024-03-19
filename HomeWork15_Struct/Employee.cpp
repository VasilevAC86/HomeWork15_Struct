#include<iostream>
#include<fstream>
#include<string>
#include "Employee.hpp"

void Print(std::vector<Employee>& const obj, bool key) {
	for (Employee& el : obj)
		if (el.shift_ == key)
			std::cout << el.number_ << '\t' << el.name_ << '\t' << el.salary_ << " rubles" << std::endl;
}

void Print(std::vector<Employee>& const obj) {
	for (Employee& el : obj)
		if (el.shift_)
			std::cout << el.number_ << '\t' << el.name_ << '\t' << el.salary_ << " rubles, day shift" << std::endl;
		else
			std::cout << el.number_ << '\t' << el.name_ << '\t' << el.salary_ << " rubles, night shift" << std::endl;
}

void Max_Salary(std::vector<Employee> obj_source, std::vector<Employee>& obj_new, bool key) {
	int counter = 0; // Счётчик кол-ва работников смены
	int salary; // Переменная для хранения максимальной зарплаты
	int index; // Переменная для хранения индекса вектора с максимальным значением зарплаты	
	while (counter < 3) {
		int it = 0; // Переменная для хранения итератора
		salary = _MIN_SALARY;
		for (Employee& el : obj_source) {
			if (el.salary_ > salary && el.shift_ == key) {
				salary = el.salary_;
				index = it;
			}
			++it;
		}
		obj_new.at(counter) = obj_source.at(index); // Добавляем в новый вектор сотрудника с максимальной зарплатой		
		obj_source.erase(obj_source.begin() + index); // Удаляем из исходного вектора сотрудника с максимальной зарплатой
		++counter;
	}
}

void Min_Salary(std::vector<Employee> obj_source, std::vector<Employee>& obj_new, bool key) {
	int counter = 0; // Счётчик кол-ва работников смены
	int salary; // Переменная для хранения максимальной зарплаты
	int index; // Переменная для хранения индекса вектора с максимальным значением зарплаты	
	int it; // Переменная для хранения итератора
	while (counter < 3) {
		it = 0;
		salary = _MAX_SALARY;
		for (Employee& el : obj_source) {
			if (el.salary_ < salary && el.shift_ == key) {
				salary = el.salary_;
				index = it;
			}
			++it;
		}
		obj_new.at(counter) = obj_source.at(index); // Добавляем в новый вектор сотрудника с максимальной зарплатой		
		obj_source.erase(obj_source.begin() + index); // Удаляем из исходного вектора сотрудника с максимальной зарплатой
		++counter;
	}
}

void Fill_in(std::vector<Employee>& All, std::vector<Employee>& const Day, std::vector<Employee>& const Night) {
	int it = 0; // Переменная для хранения итератора
	for (Employee& el : Day) {
		All.at(it) = el;
		++it;
	}
	for (Employee& el : Night) {
		All.at(it) = el;
		++it;
	}
}

void Sort(std::vector<Employee>& All, bool key) {
	// Сортируем вектор по убыванию зарплаты пузырьковой сортировкой
	Employee tmp; // Временный объект структуры Employee
	for (int i = All.size() - 1; i > 0; --i)
		for (int j = 0; j < i; ++j) {
			if (key)
				if (All.at(j).salary_ < All.at(j + 1).salary_) {
					tmp = All.at(j);
					All.at(j) = All.at(j + 1);
					All.at(j + 1) = tmp;
				}
			if (!key)
				if (All.at(j).salary_ > All.at(j + 1).salary_) {
					tmp = All.at(j);
					All.at(j) = All.at(j + 1);
					All.at(j + 1) = tmp;
				}
		}
}