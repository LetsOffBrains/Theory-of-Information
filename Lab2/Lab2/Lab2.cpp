// Lab2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

void getEnt(int size) {
	double e = 0, n = 0, count = 1.0;
	map<string, double> m;
	vector<string> v;
	char *c = new char[size];

	for (int i = 0; i < size; ++i) count *= 28.0;

	ifstream file("text.txt");
	if (!file) {
		cout << "Can't open the file" << endl;
		return;
	}
	file >> std::noskipws;

	for (int i = 0; i < size - 1; ++i) {
		file >> c[i];
		if (c[i] >= 'a' && c[i] <= 'z') c[i] -= 'a' - 'A';
		else if (c[i] != ' ' && (c[i] < 'A' || c[i] > 'Z')) c[i] = '!';
	}

	for (file >> c[size - 1]; !file.eof(); file >> c[size - 1], ++n) {
		if (c[size - 1] >= 'a' && c[size - 1] <= 'z') c[size - 1] -= 'a' - 'A';
		else if (c[size - 1] != ' ' && (c[size - 1] < 'A' || c[size - 1] > 'Z')) c[size - 1] = '!';

		string str(c, size);
		if (m.count(str)) m[str] += 1;
		else {
			m[str] = 1;
			v.push_back(str);
		}

		for (int i = 0; i < size - 1; ++i)
			c[i] = c[i + 1];
	}
	file.close();

	cout << "Alphabet size: " << m.size() << ";" << v.size() << endl;

	e = 0;
	for (int i = 0; i < count; ++i) {
		e += (1.0 / count) * log2(1.0 / count);
	}
	e *= -1;
	cout << "F1 Theoretical entropy = " << e << endl;

	sort(v.begin(), v.end());

	int tmp = 0;

	e = 0;
	for (auto i : v) {
		cout << i << " = " << m[i] << endl;
		tmp += m[i];
		double p = m[i] / n;
		e += p * log2(p);
	}
	e *= -1;
	cout << "F1 Actual entropy = " << e << endl;

	cout << tmp << " = " << n << endl;
}

int main() {
	int size = 1;

	srand(1);

	cout << "Incorrect data input leads to the end." << endl;

	do {
		cout << "Size of pairs (size >= 1): ";
		cin >> size;

		if (size <= 0) break;

		cout << endl;

		getEnt(size);

		system("pause");
		cout << endl << endl;
	} while (size > 0);
	return 0;
}


