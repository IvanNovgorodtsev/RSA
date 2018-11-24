#define BITS 32
#define BLOCK BITS/8
#include <iostream>
#include <random>
#include <cstddef>
#include <bitset>
#include "InfInt.h"

using namespace std;

int randomInt(int a, int b) {
	std::random_device rd;
	default_random_engine e(rd());
	uniform_int_distribution<int> d(a, b);
	return d(e);
}

unsigned int cdn(int c, int d, int n)      // c^d mod n
{
	unsigned int value = 1;
	while (d > 0)
	{
		value *= c;
		value %= n;
		d--;
	}
	return value;
}

bool miillerTest(int d, int n)
{
	int a = 2 + rand() % (n - 4);
	int x = cdn(a, d, n);
	if (x == 1 || x == n - 1) return true;
	while (d != n - 1)
	{
		x = (x * x) % n;
		d *= 2;

		if (x == 1)      return false;
		if (x == n - 1)    return true;
	}
	return false;
}
bool isPrime(int n, int k)
{
	if (n <= 1 || n == 4)  return false;
	if (n <= 3) return true;
	int d = n - 1;
	while (d % 2 == 0)
		d /= 2;
	for (int i = 0; i < k; i++)
		if (!miillerTest(d, n))
			return false;
	return true;
}

int nwd(unsigned int a, unsigned int b)
{
	while (a != b)
		if (a < b) b -= a; else a -= b;
	return a;
}


InfInt mod_exp(InfInt n, InfInt e, InfInt mod)
{
	if (e == 1)
	{
		return (n % mod);
	}
	else
	{
		if ((e % 2) == 1)
		{
			InfInt temp = mod_exp(n, (e - 1) / 2, mod);
			return ((n * temp * temp) % mod);
		}
		else
		{
			InfInt temp = mod_exp(n, e / 2, mod);
			return ((temp*temp) % mod);
		}
	}
}


string cipher(string input, InfInt e, InfInt n)
{
	vector<bitset<32>> vector_i;
	InfInt ciphered;
	string output;

	for (size_t i = 0; i < input.size(); i++)
	{
		ciphered = mod_exp((int)input[i], e, n);
		vector_i.push_back(ciphered.toUnsignedLongLong());
	}

	for (size_t i = 0; i < vector_i.size(); i++)
		for (size_t j = 0; j < BLOCK; j++)
		{
			char c1 = static_cast<char>(stoi(vector_i[i].to_string().substr(0 + j * 8, 8), nullptr, 2));
			output += c1;
		}
	return output;
}


string decipher(string ciphered, InfInt d, InfInt n)
{
	vector<bitset<32>> vector_o;
	InfInt deciphered;
	bitset<32> h;
	string block = "";
	string output = "";
	for (size_t i = 0; i < ciphered.size() / 4; i++)
	{
		for (size_t j = 0; j < BLOCK; j++)
		{
			block += (bitset<8>(ciphered.c_str()[j + i * BLOCK])).to_string();
		}
		h = stoi(block, nullptr, 2);
		vector_o.push_back(h);
		block = "";
	}
	for (size_t i = 0; i < vector_o.size(); i++)
	{
		deciphered = mod_exp((int)vector_o[i].to_ulong(), d, n);
		vector_o[i] = deciphered.toUnsignedLongLong();
	}

	for (size_t i = 0; i < vector_o.size(); i++)
	{
		char c1 = static_cast<char>(vector_o[i].to_ulong());
		output += c1;
	}
	return output;
}


int main()
{
	// declarations
	unsigned int p = 0, q = 0;
	int miller_iterations = 4;
	long phi = 0;
	long n = 0;
	long e = 3;
	long d = 0;
	//


	// Generating prime p & q
	while (!(isPrime(p, miller_iterations)))
	{
		p = randomInt(1, 9999);
	}
	while (!(isPrime(q, miller_iterations)))
	{
		q = randomInt(1, 9999);
	}
	// 

	// calculating Euler's function and module
	phi = (p - 1)*(q - 1);
	n = p * q;
	//

	// TESTS

	//phi = 120;
	//n = 143;

	//

	// calculating e
	while (nwd(e, phi) != 1 && 1 < e && e < n) e = e + 2;
	//

	// calculating d
	while ((d*e) % phi != 1) d++;
	//

	// PUBLIC KEY, PRIVATE KEY
	cout << "Public  key (e,n): (" << e << "," << n << ")" << endl;
	cout << "Private key (d,n): (" << d << "," << n << ")" << endl;
	//


	// INPUT DATA

	string input = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
	string ciphered;
	string deciphered;

	ciphered = cipher(input, e, n);
	cout << ciphered << endl;
	deciphered = decipher(ciphered, d, n);
	cout << deciphered << endl;

	system("pause");
}