// lab2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <cmath>

using namespace std;

class multidigitNumber
{
private:
	unsigned long int mas[128];
	int len;
public:
	multidigitNumber(unsigned long int a);
	multidigitNumber();
	multidigitNumber(multidigitNumber &B);
	multidigitNumber(const string s);
	void toBin();
	const multidigitNumber& LongAdd(const multidigitNumber& A, const multidigitNumber& B);
	multidigitNumber& LongSub(const multidigitNumber& A, const multidigitNumber& B);
	int LongCmp(const multidigitNumber& B);
	const multidigitNumber& LongMulOneDigit(const multidigitNumber& A, unsigned long int b);
	multidigitNumber& LongMul(const multidigitNumber& A, const multidigitNumber& B);
	multidigitNumber& LongSquare(const multidigitNumber& A);
	multidigitNumber& LongPow(const multidigitNumber& A, const multidigitNumber& B);
	multidigitNumber& LongShiftDigitsToHigh(const multidigitNumber& A, int i);
	multidigitNumber& LongShiftBitsToHigh(const multidigitNumber& A, int i);
	multidigitNumber& LongDiv(const multidigitNumber& A, const multidigitNumber& B);
	multidigitNumber& LongMod(const multidigitNumber& A, const multidigitNumber& B);
	const multidigitNumber& operator=(const multidigitNumber& B);
	friend ostream& operator<<(ostream& s1, const multidigitNumber& A);
	const int BitLength();

	multidigitNumber& gsd(const multidigitNumber& A, const multidigitNumber& B);
	multidigitNumber& lcm(const multidigitNumber& A, const multidigitNumber& B);
	multidigitNumber& LongAddMod(const multidigitNumber& A, const multidigitNumber& B, const multidigitNumber& N);
	multidigitNumber& LongSubMod(const multidigitNumber& A, const multidigitNumber& B, const multidigitNumber& N);
	const int Length();
	multidigitNumber& KillLastDigits(const multidigitNumber& A, int n, int k);
	multidigitNumber& Myu(multidigitNumber& X, multidigitNumber& N);
	multidigitNumber& BarrettReduction(multidigitNumber& X, multidigitNumber& N, multidigitNumber& Myu);
	multidigitNumber& LongModPowerBarrett(multidigitNumber& A, multidigitNumber& B, multidigitNumber& N);
	bool Miller_Rabin_test(const multidigitNumber& P);
};

multidigitNumber::multidigitNumber(const string s)
{
	string hex = "0123456789ABCDEF";

	int n = 0;
	int j = 0;
	char s1[2] = {'0', 0};
	for (int i = 0; i < 128; i++)
		mas[i] = 0;
	for (int i = s.length()-1; i >= 0; i--)
	{
		mas[n] += hex.find(toupper(s[i])) << (4*j);
		j++;
		if (j == 4) 
		{ 
			j=0;
			n++;
		}
	}
	if (j == 0) n--;
}

ostream& operator<<(ostream& s1, const multidigitNumber& A)
{
	int max = 127;
	while ((A.mas[max] == 0)&&(max >= 0)) max--;
    if (max<0) max = 0;

	for(int i = max; i >=0 ; i--)
	{
		s1 << hex << ((A.mas[i] >> 12) & 0xF);
		s1 << hex << ((A.mas[i] >>  8) & 0xF);
		s1 << hex << ((A.mas[i] >>  4) & 0xF);
		s1 << hex << ( A.mas[i]        & 0xF);
	}
	return s1;
}

const int multidigitNumber::BitLength()
{
	int i = 2047;
	while ((((mas[i/16] >> (i%16))&1) == 0) && (i>=0)) i--;
	return i+1;
}

const multidigitNumber& multidigitNumber::operator=(const multidigitNumber& B)
{
	for(int i = 0; i < 128; i++)
		mas[i] = B.mas[i];
	return (*this);
}

multidigitNumber::multidigitNumber(multidigitNumber &B)
{
	for(int i = 0; i < 128; i++)
		mas[i] = B.mas[i];
}

multidigitNumber::multidigitNumber()
{
	for(int i = 0; i < 128; i++)
		mas[i] = 0;	
}

multidigitNumber::multidigitNumber(unsigned long int a)
{
	for(int i = 0; i < 128; i++)
	{
		mas[i] = 0;
	}
	mas[0] = a & 0xFFFF;
	mas[1] = (a >> 16) & 0xFFFF;
}

const multidigitNumber& multidigitNumber::LongAdd(const multidigitNumber& A, const multidigitNumber& B)
{
	unsigned long int carry = 0;
	for(int i = 0; i < 128; i++)
	{
		unsigned long int temp = A.mas[i] + B.mas[i] + carry;
		mas[i] = temp & 0xFFFF;
		carry = temp >> 16;
	}
	return (*this);
}


multidigitNumber& multidigitNumber::LongSub(const multidigitNumber& A, const multidigitNumber& B)
{
	unsigned long int borrow = 0;
	for(int i = 0; i < 128; i++)
	{
		long int temp = A.mas[i] - B.mas[i] - borrow;
		if(temp >= 0)
		{
			mas[i] = temp;
			borrow = 0;
		}
		else
		{
			mas[i] = ((unsigned int)1 << 16) + temp;
			borrow = 1;
		}
	}
	return (*this);
}

int multidigitNumber::LongCmp(const multidigitNumber& B)
{
	int i = 127;
	while ((mas[i] == B.mas[i]) && (i>=0))
		i--;
	if(i == -1) return 0;
	else
	{
		if(mas[i] > B.mas[i]) return 1;
		else return -1;
	}
}

const multidigitNumber& multidigitNumber::LongMulOneDigit(const multidigitNumber& A, unsigned long int b)
{
	unsigned long int carry = 0;
	unsigned long int temp = 0;
	for(int i = 0; i < 128; i++)
	{
		temp = A.mas[i]*b + carry;
		mas[i] = temp & 0xFFFF;
		carry = temp >> 16;
	}
	return (*this);
}


multidigitNumber& multidigitNumber::LongShiftDigitsToHigh(const multidigitNumber& A, int i)
{
	for(int k = 0; k < 128; k++)
		mas[k] = 0;
	for(int k = 0; k < 128 - i; k++)
		mas[k+i] = A.mas[k];
	return (*this);
}

multidigitNumber& multidigitNumber::LongShiftBitsToHigh(const multidigitNumber& A, int i)
{
	multidigitNumber T, S;
	S = A;
	if(i < 16)
	{
		mas[0] = (A.mas[0] << i) & 0xFFFF;
		for(int k = 1; k < 128; k++)
			mas[k] = ((A.mas[k] << i)|(A.mas[k-1] >> (16 - i))) & 0xFFFF;
	}
	if(i >= 16)
	{
		T.LongShiftDigitsToHigh(S, i/16);
		S = T;
		mas[0] = (S.mas[0] << (i%16)) & 0xFFFF;
		for(int k = 1; k < 128; k++)
			mas[k] = ((S.mas[k] << (i%16))|(S.mas[k-1] >> (16 - (i%16)))) & 0xFFFF;
	}
	return (*this);
}

multidigitNumber& multidigitNumber::LongMul(const multidigitNumber& A, const multidigitNumber& B)
{
	multidigitNumber C, Temp, D;
	for(int i = 0; i < 128; i++)
	{
		Temp.LongMulOneDigit(A, B.mas[i]);
		D.LongShiftDigitsToHigh(Temp, i);
		C.LongAdd(C, D);
	}
	*this = C;
	return (*this);
}


multidigitNumber& multidigitNumber::LongSquare(const multidigitNumber& A)
{
	multidigitNumber B;
	B.LongMul(A, A);
	*this = B;
	return (*this);
}

multidigitNumber& multidigitNumber::LongDiv(const multidigitNumber& A, const multidigitNumber& B)
{
	multidigitNumber R, Q, C, T, W, Z;
	R = A;
	C = B;
	int k = C.BitLength(); // B.BitLength();
	while(R.LongCmp(B) != -1)
	{
		int t = R.BitLength();
		T.LongShiftBitsToHigh(B, t-k);
		C = T;
		if(R.LongCmp(C) == -1)
		{
			t--;
			W.LongShiftBitsToHigh(B, t-k);
			C = W;
		}
		T.LongSub(R, C);
		R = T;
		Q.mas[(t-k)/16] |= (1 << ((t-k)%16));
	}
	*this = Q;
	return (*this);
}

multidigitNumber& multidigitNumber::LongMod(const multidigitNumber& A, const multidigitNumber& B)
{
	multidigitNumber R, T, W;
	R = A;
	multidigitNumber Q, C;
	C = B;
	int k = C.BitLength(); // B.BitLength();
	while(R.LongCmp(B) != -1)
	{
		int t = R.BitLength();
		T.LongShiftBitsToHigh(B, t-k);
		C = T;
		if(R.LongCmp(C) == -1)
		{
			t--;
			W.LongShiftBitsToHigh(B, t-k);
			C = W;
		}
		T.LongSub(R, C);
		R = T;
		Q.mas[(t-k)/16] |= (1 << ((t-k)%16));
	}
	*this = R;
	return (*this);
}


multidigitNumber& multidigitNumber::LongPow(const multidigitNumber& A, const multidigitNumber& B)
{
	multidigitNumber D, C(1), Z;
	Z = A;
	for(int i = 0; i < 2048; i++)
	{
		if((B.mas[i/16] >> (i%16) & 1) == 1)
		{
			D.LongMul(C, Z);
			C = D;
		}
		D.LongMul(Z, Z);
		Z = D;
	}
	*this = C;
	return (*this);
}

multidigitNumber& multidigitNumber::gsd(const multidigitNumber& A, const multidigitNumber& B)
{
	multidigitNumber C, D("1"), E, F;
	E = A;
	F = B;

	while((LongMod(E, 2).LongCmp(0) == 0) && (LongMod(F, 2).LongCmp(0) == 0))
	{
		E.LongDiv(E, 2);
		F.LongDiv(F, 2);
		D.LongMulOneDigit(D, 2);
	}
	while(LongMod(E, 2).LongCmp(0) == 0)
		E.LongDiv(E, 2);
	while(F.LongCmp(0) != 0)
	{
		while(LongMod(F, 2).LongCmp(0) == 0)
			F.LongDiv(F, 2);
		if(E.LongCmp(F) == 1) return LongMul(D, gsd(F, E.LongSub(E, F)));
		if(E.LongCmp(F) == -1) return LongMul(D, gsd(E, F.LongSub(F, E)));
		if(E.LongCmp(F) == 0) return LongMul(D, gsd(E, F.LongSub(E, F)));
	}
	D.LongMul(D, E);
	*this = D;
	return(*this);
}

multidigitNumber& multidigitNumber::lcm(const multidigitNumber& A, const multidigitNumber& B)
{
	multidigitNumber E, F, S;
	E.LongMul(A, B);
	S.gsd(A, B);
	F.LongDiv(E, S);
	*this = F;
	return (*this);
}

multidigitNumber& multidigitNumber::LongAddMod(const multidigitNumber& A, const multidigitNumber& B, const multidigitNumber& N)
{
	multidigitNumber C;
	C.LongAdd(A, B);
	C.LongMod(C, N);
	*this = C;
	return (*this);
}

multidigitNumber& multidigitNumber::LongSubMod(const multidigitNumber& A, const multidigitNumber& B, const multidigitNumber& N)
{
	multidigitNumber C, D, E;
	D = A;
	E = B;
	if(D.LongCmp(E) != -1)
	{
		C.LongSub(D, E);
		C.LongMod(C, N);
		*this = C;
		return (*this);
	}
	else cout << "Отрицательное число!" << endl;
}

const int multidigitNumber::Length()
{
	len = 127;
	while((mas[len] == 0) && (len>=0)) len--;
	return len+1;
}

multidigitNumber& multidigitNumber::KillLastDigits(const multidigitNumber& A, int n, int k)
{
	multidigitNumber B;
	B = A;
	for(int i = 0; i < k; i++) B.mas[i] = 0;
	for(int i = 0; i < 128-k; i++)
		B.mas[i] = A.mas[k+i];
	*this = B;
	return (*this);
}

multidigitNumber& multidigitNumber::Myu(multidigitNumber& X, multidigitNumber& N)
{
	multidigitNumber Temp1, Temp2;
	int k = X.Length();
	Temp1.LongPow(65536, X.Length());
	Temp2.LongDiv(Temp1, N);
	cout << Temp2 << endl;
	*this = Temp2;
	return (*this);
}


multidigitNumber& multidigitNumber::BarrettReduction(multidigitNumber& X, multidigitNumber& N, multidigitNumber& Myu)
{
	multidigitNumber Q, R, H;
//	cout << "myu: " << Myu << endl ;
	Q.KillLastDigits(X, X.Length(), N.Length()-1);
	//cout << "X: " << Q << endl;
	Q.LongMul(Q, Myu);
	Q.KillLastDigits(Q, Q.Length(), N.Length()+1);
	H.LongMul(Q, N);
	if(X.LongCmp(H) != -1)
	{
		R.LongSub(X, H);
		while(R.LongCmp(N) != -1) R.LongSub(R, N);
	}
	if(X.LongCmp(H) == -1)
		R.LongMod(X, N);
	*this = R;
	return (*this);
}

multidigitNumber& multidigitNumber::LongModPowerBarrett(multidigitNumber& A, multidigitNumber& B, multidigitNumber& N)
{
	multidigitNumber Myu, C("1"), S;
	S = A;
	Myu.LongShiftDigitsToHigh(1, 2*N.Length());
	Myu.LongDiv(Myu, N);
	cout << "Myu: " << Myu << endl;
	for(int i = 0; i < B.BitLength(); i++)
	{
		if((B.mas[i/16] >> (i%16) & 1) == 1)
		{
			cout << LongMul(C, S) << endl;
			C.BarrettReduction(LongMul(C, S), N, Myu);
		}
		S.BarrettReduction(LongSquare(S), N, Myu);
	}
	*this = C;
	return (*this);
}
/*
bool multidigitNumber::Miller_Rabin_test(const multidigitNumber& P)
{
	multidigitNumber A, B, C, D, E, S, F, P1;
	P1 = P;
	bool temp1, temp2;
	multidigitNumber Count;
	B.LongSub(P, 1);
	C = B;
	while(LongMod(B, 2).LongCmp(0) == 0)
	{
		C.LongDiv(B, 2);
		Count.LongAdd(Count, 1);
	}
	int i = 0;
	while(A.LongCmp(B) == -1)
	{
		long int number;
		number = 2*rand();//65535
		A.mas[i] = number;
		i++;
	}
	if(A.LongCmp(B) == 1)
		A.LongSub(A, 0xffff);
	cout << A;
	if(LongSub(D.LongModPowerBarrett(A, C, P1), 1).LongCmp(0) == 0) temp1 = 1;
	i = 0;
	while(S.LongCmp(Count) == -1)
	{
		long int number;
		number = 2*rand();
		S.mas[i] = number;
		i++;
	}
	F.LongModPowerBarrett(2, S, P);
	if(LongAdd(LongModPowerBarrett(A, LongMul(F, C), P1), 1).LongCmp(0) == 0) temp2 = 1;
	if((temp1 == 1) && (temp2 == 1)) return 1;
	else return 0;
}*/

int main()
{
	setlocale(LC_ALL, "Rus");
	multidigitNumber A("F170FD5E52889828898BBE528898289828898B11");
	multidigitNumber B("FDE3E5133E513333E554EADE3EE554EAD8713A11");
	multidigitNumber N("12345678");
	multidigitNumber C, D, E, F, G, H;
//	C.KillLastDigits(A, A.Length(), 4);
//	cout << C << endl;
	C.gsd(A, B);
	D.lcm(A, B);
	cout << "Число А = " << A << endl;
	cout << "Число В = " << B << endl;
	cout << "НОД А и В = " << C << endl;
	cout << "НОК А и В = " << D << endl;
//	E.LongAddMod(A, B, N);
//	cout << "(A+B)modN = " << E << endl;
//	F.LongSubMod(A, B, N);
//	cout << "(A-B)modN = " << F << endl;
	/*
	multidigitNumber C, D, E, F, G, H;
	multidigitNumber P("5A0AF453BA5");
	multidigitNumber R("EE837B0");
	multidigitNumber N1("1B5FDD46");
	cout << R.BitLength() << endl;
	G.LongModPowerBarrett(P, R, N1);
	cout << G << endl;
	cout << "RAND_MAX = " << RAND_MAX << endl;
	for(int i = 0; i < 20; i++)
	{
		cout << "test: ";
		H.Miller_Rabin_test(N1);
		cout << endl;
	}*/
/*	multidigitNumber A("1170FD528898B7B831350C4CAD511D45566C6DE9AA1C2CD4BA0F519E0DC7FC57");
	multidigitNumber B("2222222222");
	multidigitNumber N("10000");
	multidigitNumber C, D, E, F, G, H;
	cout << G.Myu(B, N);
	multidigitNumber A1("477e68a5f9bd6adb4c7750c79fef0f0e39de645c5ea3dba8278c1511094277e2");
	multidigitNumber A2("490a4e064270b5a15ad9095cc5fd20365b3c3d420c958e47a8db7a4832a9334f");
	C.LongMul(A1, A2);
	cout << endl <<C;
	H.BarrettReduction(B, N, G.Myu(B, N));
	cout << H << endl;
	E.LongMod(B, N);
	cout << E << endl;*/
	cin.get();
	cin.get();
	return 0;
}