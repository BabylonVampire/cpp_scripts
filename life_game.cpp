#include <iostream>
#include <vector>
#include <array>
#include <windows.h>

using namespace std;

void print(int x, int y, char** field) {
	//Функция для вывода игрового поля
	for (size_t i = 1; i < x - 1; ++i) {
		for (size_t j = 1; j < y - 1; ++j) {
			cout << field[i][j];
		}
		cout << endl;
	}
}

void behaveour(int x, int y, char** field, int &counter) {
	//функуция по подсчету 'живых' клеток по соседству с данной
	for (size_t i = x - 1; i <= x + 1; ++i) {
		for (size_t j = y - 1; j <= y + 1; ++j) {
			if (field[i][j] == '*' && (i != x || j != y)) ++counter;
		}
	}
}

void birth_and_dying(int x, int y, char** field, int &counter_dying, int& counter_birth, vector<array <int, 2>>& births, vector<array <int, 2>>& dyings) {
	//фуинкция, определяющая и заносящая в векторы координаты точек, которые должны родится/умереть
	int counter = 0;
	array<int, 2> dot;
	for (size_t i = 1; i < x - 1; ++i) {
		for (size_t j = 1; j < y - 1; ++j) {
			behaveour(i, j, field, counter);
			if ((counter < 2 || counter > 3) && field[i][j] == '*') {
				dot.at(0) = i;
				dot.at(1) = j;
				dyings.push_back(dot);
				++counter_dying;
			}
			else if (counter == 3 && field[i][j] != '*') {
				dot.at(0) = i;
				dot.at(1) = j;
				births.push_back(dot);
				++counter_birth;
			}
			counter = 0;
		}
	}
}

void killing_and_reprodusing(vector<array <int, 2>>& births, vector<array <int, 2>>& dyings, char** field) {
	//функиця, обновляющая поле, исходя из данных о тех, кто умер/родился
	//вынесена в отдельную по причине того, что программе важно сначала понять, кто умрет/родится, а уже затем использовать эти данные, чтобы все происходило одновременно
	for (size_t i = 0; i < births.size(); ++i) {
		field[births.at(i)[0]][births.at(i)[1]] = '*';
	}
	for (size_t i = 0; i < dyings.size(); ++i) {
		field[dyings.at(i)[0]][dyings.at(i)[1]] = ' ';
	}
}

void statistics(int day, int counter_dying, int counter_birth) {
	cout << "day: " << day << endl;
	cout << "Number of births: " << counter_birth << endl;
	cout << "Number of deaths: " << counter_dying << endl;
}

int main() {
	int x, y;
	cout << "Enter wight and height of field (natural numbers): ";
	cin >> x >> y;
	x += 2;
	y += 2;
	char** field = new char* [x];
	for (size_t i = 0; i < x; ++i) {
		field[i] = new char[y];
		for (int j = 0; j < y; ++j) {
			field[i][j] = ' ';
		}
	}
	cout << "msg: Enter only natural numbers!" << endl;
	cout << "msg: Enter \"-1 -1\" to end" << endl;
	int a, b, counter = 1;
	do {
		cout << "[" << counter << "]: ";
		cin >> a >> b;
		if (a != -1 || b != -1) {
			field[a + 1][b + 1] = '*';
		}
		++counter;
	} while (a != -1 && b != -1);
	int counter_birth = 0, counter_dying = 0, total_births = 0, total_deaths = 0;
	counter = 1;
	vector <array <int, 2>> births;
	vector <array <int, 2>> deaths;
	while (true) {
		system("cls");
		print(x, y, field);
		birth_and_dying(x, y, field, counter_birth, counter_dying, births, deaths);
		killing_and_reprodusing(births, deaths, field);
		total_births += counter_birth;
		total_deaths += counter_dying;
		statistics(counter, total_deaths, total_births);
		births.clear();
		deaths.clear();
		++counter;
		if (counter_birth == 0 && counter_dying == 0) {
			cout << "msg: The game is over!" << endl;
			system("pause");
			for (size_t i = 0; i < x; ++i) delete field[i];
			delete[] field;
			exit(1);
		}
		counter_birth = 0;
		counter_dying = 0;
		system("pause");
	}
}