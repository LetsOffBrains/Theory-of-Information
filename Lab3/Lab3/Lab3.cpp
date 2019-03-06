// Lab3.cpp: определяет точку входа для консольного приложения.
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

int Up(int n, double q){
	int j = 1;
	for (int i = n - 1; i >= 2; --i) {
		if (p[i - 1] < q)
			p[i] = p[i - 1];
		else {
			j = i;
			break;
		}
	}
	p[j] = q;
	return j;
}

void Down(int n, int j){
	int s[N + 1];
	int k;

	for (k = 1; k <= N; k++)
		s[k] = code[j][k];
	int L = l[j];

	for (int i = j; i <= n - 2; i++) {
		for (k = 1; k <= N; k++)
			code[i][k] = code[i + 1][k];
		l[i] = l[i + 1];
	}

	for (k = 1; k <= N; k++) {
		code[n - 1][k] = s[k];
		code[n][k] = s[k];
	}
	code[n - 1][L + 1] = start;
	code[n][L + 1] = end;
	l[n - 1] = L + 1;
	l[n] = L + 1;
}

void Huffman(int n){
	if (n <= 2) {
		code[1][1] = start; l[1] = 1;
		code[2][1] = end; l[2] = 1;
		return;
	}
	double q = p[n - 1] + p[n];
	int j = Up(n, q);
	Huffman(n - 1);
	Down(n, j);
}

void encode(string fn1, string fn2) {
	char c;

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

	from >> std::noskipws;

	for (from >> c; !from.eof(); from >> c) {
		int index = ind[Ch_to_Num(c)];
		for (int i = 1; i <= l[index]; i++) {
			to << code[index][i];
		}
	}
	from.close();
	to.close();
}

double getEnt(int size, string filename) {
	double e = 0, n = 0, count = 1.0;
	map<string, double> m;
	vector<string> v;
	char *c = new char[size];

	for (int i = 0; i < size; ++i) count *= 2.0;

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
	double _p[N + 1], aver_len = 0;
	srand(1);

	getProb("text.txt");
	
	SortProb();
	
	for (int i = 1; i <= N; i++) {
		_p[i] = p[i];
		ind[i] = i;
		l[i] = 0;
		for (int j = 1; j <= N; j++)
			code[i][j] = 0;
	}

	Huffman(N);

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

	encode("text.txt", "encode.txt");
	
	cout << "Single simbol entropy: " << getEnt(1, "encode.txt") << endl;
	cout << "Double simbol entropy: " << getEnt(2, "encode.txt") << endl;
	cout << "Triple simbol entropy: " << getEnt(3, "encode.txt") << endl;
	
	system("pause");

	return 0;
}

