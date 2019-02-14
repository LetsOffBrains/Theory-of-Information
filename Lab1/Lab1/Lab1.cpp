// Lab1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

long double fact(int N)
{
	if (N < 0) // если пользователь ввел отрицательное число
		return 0; // возвращаем ноль
	if (N == 0) // если пользователь ввел ноль,
		return 1; // возвращаем факториал от нуля - не удивляетесь, но это 1 =)
	else // Во всех остальных случаях
		return N * fact(N - 1); // делаем рекурсию.
}

void generateFiles(double chance) {
	ofstream f1("F1.txt");
	ofstream f2("F2.txt");

	if (chance <= 1) chance *= 100;

	for (int i = 0; i < 10000; ++i) {
		int r = rand() % 100;

		if (r < 50) f1 << 'a';
		else f1 << 'b';

		if (r < chance) f2 << 'a';
		else f2 << 'b';
	}

	f1.close();
	f2.close();
}

void getEnt(int size, double chance) {
	double e = 0, n = 0, tch;
	map<string, double> m;
	vector<string> v;
	char *c = new char[size];

	ifstream f1("F1.txt");
	for (int i = 0; i < size - 1; ++i) f1 >> c[i];
	for (f1 >> c[size - 1]; !f1.eof(); f1 >> c[size - 1], ++n) {
		string str(c, size);
		if (m.count(str)) m[str] += 1;
		else {
			m[str] = 1;
			v.push_back(str);
		}

		for (int i = 0; i < size - 1; ++i)
			c[i] = c[i + 1];
	}
	f1.close();

	tch = 1.0 / (double)(size * size);
	for (int i = 0; i < size * size; ++i) {
		e += tch * log2(tch);
	}
	e *= -1;
	cout << "F1 Theoretical entropy = " << e << endl;

	e = 0;
	for (auto i : v) {
		//cout << i << " = " << m[i] << endl;
		double p = m[i] / n;
		e += p * log2(p);
	}
	e *= -1;
	cout << "F1 Actual entropy = " << e << endl;

	e = n = 0;
	m.clear();
	v.clear();

	ifstream f2("F2.txt");
	for (int i = 0; i < size - 1; ++i) f2 >> c[i];
	for (f2 >> c[size - 1]; !f2.eof(); f2 >> c[size - 1], ++n) {
		string str(c, size);
		if (m.count(str)) m[str] += 1;
		else {
			m[str] = 1;
			v.push_back(str);
		}

		for (int i = 0; i < size - 1; ++i)
			c[i] = c[i + 1];
	}
	f2.close();

	tch = 1.0 / (double)(size * size);
	for (int i = 0; i < size * size; ++i) {
		e += tch * log2(tch);
	}
	e *= -1;
	cout << "F2 Theoretical entropy = " << e << endl;

	e = 0;
	for (auto i : v) {
		//cout << i << " = " << m[i] << endl;
		double p = m[i] / n;
		e += p * log2(p);
	}
	e *= -1;
	cout << "F2 Actual entropy = " << e << endl;
}

int main(){
	double chance = 0.7;
	int size = 1;
	char c;

	srand(1);

	generateFiles(chance);

	cout << "Size: ";
	cin >> size;

	getEnt(size, chance);
	
	system("pause");
    return 0;
}

