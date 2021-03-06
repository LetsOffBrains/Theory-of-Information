// Lab4.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>

#define start 0
#define end 1

using namespace std;

const int N = 28;
double p[N + 1];
int code[N + 1][N + 1];
int l[N + 1];
int ind[N + 1];

int Ch_to_Num(char c) {
	if (c >= 'a' && c <= 'z')
		return c - 'a' + 1;
	if (c >= 'A' && c <= 'Z')
		return c - 'A' + 1;
	if (c == ' ')
		return 27;
	return 28;
}

char Num_to_ch(int k)
{
	if (k >= 1 && k <= 26)
		return 'a' + k - 1;
	if (k == 27)
		return '_';
	return '!';
}

void getProb(string fileName) {
	double n = 0;
	char c;

	ifstream file(fileName);
	if (!file) {
		cout << "Can't open the file " << fileName << endl;
		return;
	}
	file >> std::noskipws;

	for (file >> c; !file.eof(); file >> c, ++n) {
		++p[Ch_to_Num(c)];
	}
	file.close();

	for (int i = 0; i <= N; ++i) {
		p[i] /= n;
	}
}

void SortProb()
{
	for (int i = 1; i <= N - 1; i++)
		for (int j = 1; j <= N - i; j++)
			if (p[j] < p[j + 1]) {
				double tmp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = tmp;
				int t = ind[j];
				ind[j] = ind[j + 1];
				ind[j + 1] = t;
			}
}

// Находит медиану части массива вероятностей
int Med(int L, int R)
{
	float sl = 0; // Сумма элементов первой части массива
	for (int i = L; i <= R - 1; i++)
		sl += p[i];
	float sr = p[R]; // Сумма элементов второй части массива
	int m = R;
	while (sl >= sr) {
		m--;
		sl -= p[m];
		sr += p[m];
	}
	return m;
}

// Реализует алгоритм Фано
// L - левая граница обрабатываемой части массива вероятностей
// R - правая граница обрабатываемой части массива вероятностей
// k - длина уже построенной части кодовых слов
void Fano(int L, int R, int k)
{
	if (L < R) {
		k++;
		int m = Med(L, R);
		for (int i = L; i <= R; i++) {
			if (i <= m)
				code[i][k] = 0;
			else
				code[i][k] = 1;
			l[i]++;
		}
		Fano(L, m, k);
		Fano(m + 1, R, k);
	}
}

void encode(string fn1, string fn2, string fn3) {
	char c, tmp = 0;
	int size = 0;

	ifstream from(fn1);
	if (!from) {
		cout << "Can't open the file" << fn1 << endl;
		return;
	}

	ofstream to(fn2);
	if (!to) {
		from.close();
		cout << "Can't open the file" << fn2 << endl;
		return;
	}

	ofstream enc(fn3);
	if (!enc) {
		to.close();
		from.close();
		cout << "Can't open the file" << fn3 << endl;
		return;
	}

	from >> std::noskipws;

	for (from >> c; !from.eof(); from >> c) {
		int index = ind[Ch_to_Num(c)];
		for (int i = 1; i <= l[index]; i++) {
			to << code[index][i];
			tmp = (tmp << 1) + code[index][i];
			++size;
			if (size >= 8) {
				enc << tmp;
				tmp = 0;
				size = 0;
			}
		}
	}

	for (int i = size; i <= 8; ++i) {
		tmp = tmp << 1;
	}
	enc << tmp;

	from.close();
	to.close();
	enc.close();
}

double getEnt(int size, string filename) {
	double e = 0, n = 0;
	map<string, double> m;
	vector<string> v;
	char *c = new char[size];

	ifstream f1(filename);
	if (!f1) {
		cout << "Can't open the file" << filename << endl;
		return -1;
	}

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
	for (auto i : v) {
		//cout << i << " = " << m[i] << endl;
		double p = m[i] / n;
		e += p * log2(p);
	}
	e /= -size;
	//cout << "F1 Actual entropy = " << e << endl;

	return e;
}

int main() {
	double _p[N + 1], aver_len = 0, H;
	string f1("text.txt"), f2("bit.txt"), f3("enc.txt");
	srand(1);

	getProb(f1);

	SortProb();

	for (int i = 1; i <= N; i++) {
		_p[i] = p[i];
		ind[i] = i;
		l[i] = 0;
		for (int j = 1; j <= N; j++)
			code[i][j] = 0;
	}

	//Huffman(N);
	Fano(1, N, 0);

	cout << "Simbol \tProbability \tCode \t\tLenght " << endl;
	for (int i = 1; i <= N; ++i) {
		cout << Num_to_ch(ind[i]) << '\t' << setw(12) << left << _p[i] << '\t';

		for (int j = 1; j <= l[i]; ++j) {
			cout << code[i][j];
		}

		if (l[i] < 8) cout << '\t';
		cout << '\t' << l[i] << endl;

		aver_len += _p[i] * l[i];
	}

	cout << "Average lenght: " << aver_len << endl;

	encode(f1, f2, f3);

	H = getEnt(1, f1);
	cout << "Coding redundancy (single char): " << aver_len - H << endl;
	cout << "Input single simbol entropy: " << H << endl;

	cout << endl;

	cout << "Output single simbol entropy: " << getEnt(1, f2) << endl;
	cout << "Output double simbol entropy: " << getEnt(2, f2) << endl;
	cout << "Output triple simbol entropy: " << getEnt(3, f2) << endl;

	system("pause");

	return 0;
}

