// Lab1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

double fact(int N)
{
	if (N < 0) // если пользователь ввел отрицательное число
		return 0; // возвращаем ноль
	if (N == 0) // если пользователь ввел ноль,
		return 1; // возвращаем факториал от нуля - не удивляетесь, но это 1 =)
	else // Во всех остальных случаях
		return N * fact(N - 1); // делаем рекурсию.
}

double comb(int n, int k) {
	return fact(n) / (fact(n - k) * fact(k));
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
	double e = 0, n = 0, count = 1.0;
	map<string, double> m;
	vector<string> v;
	char *c = new char[size];

	for (int i = 0; i < size; ++i) count *= 2.0;

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

	e = 0;
	for (int i = 0; i < count; ++i) {
		e += (1.0 / count) * log2(1.0 / count);
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

	// #######################################################################

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

	e += chance * log2(chance);
	e += (1 - chance) * log2(1 - chance);
	for (int i = 1; i < size; ++i) {
		//cout << "[" << size << ";" << i << "]";
		int p = comb(size , i);
		//cout << "P: " << p << endl;
		e += p * (chance * log2(chance)) + (size - p) * ((1 - chance) * log2(1 - chance));
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

	srand(1);

	cout << "Incorrect data input leads to the end." << endl;

	do {
		cout << "Enter chance of 'a' to appear (0 <= chance <= 1): ";
		cin >> chance;

		if (chance < 0 || chance > 1) break;

		generateFiles(chance);

		cout << "Size of pairs (size >= 1): ";
		cin >> size;

		if (size <= 0) break;

		cout << endl;

		getEnt(size, chance);

		system("pause");
		cout << endl << endl;
	}
	while (chance >= 0 && chance <= 1 && size > 0);
    return 0;
}

