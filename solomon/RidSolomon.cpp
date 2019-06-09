// RidSolomon.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "rdso.h"
using namespace std;
int * IDFTtest(int *numberArray, int size)
{
	int * resultArray = new int[size] {4, 1, 0, 2, 5, 6};
	/*int* resultArray = new int[size];
	int sum;
	int step = 5;
	int pow = 1;
	for (int i = 0; i < size; i++) //цикл по перебору номера элемента
	{
		sum = 0;
		for (int j = 0; j < size; j++) //цикл суммирования элементов полинома
		{
			pow = 1;
			for (int k = 0; k < i * j; k++) // цикл вычисление степени элемента полинома
			{
				pow *= step;
				if (pow > 7)
				{
					pow = pow % 7;
				}
			}
			sum += numberArray[j] * pow; // суммирование Фурье
			if (sum >= 7)
			{
				sum = sum % 7;
			}
		}
		resultArray[i] = sum;
	}*/
	return resultArray;
}

int* IDFT(int* numberArray, int size)
{
	//int* resultArray = new int[size] {4, 1, 0, 4, 5, 5};
	int* resultArray = new int[size];
	int sum;
	int step = 5;
	int pow = 1;
	for (int i = 0; i < size; i++) //цикл по перебору номера элемента
	{
		sum = 0;
		for (int j = 0; j < size; j++) //цикл суммирования элементов полинома
		{
			pow = 1;
			for (int k = 0; k < i * j; k++) // цикл вычисление степени элемента полинома
			{
				pow *= step;
				if (pow > 7)
				{
					pow = pow % 7;
				}
			}
			sum += numberArray[j] * pow; // суммирование Фурье
			if (sum >= 7)
			{
				sum = sum % 7;
			}
		}
		resultArray[i] = sum;
	}
	return resultArray;
}

int * DFT(int *numberArray, int size)
{
	int * resultArray = new int[size];
	int sum, del, element;
	for (int i = 0; i < size; i++)
	{
		int sum = 0;
		for (int j = 0; j < size; j++)
		{
			int step = 5;
			if (i*j == 0)
			{
				step = 1;
			}
			else
			{
				for (int k = 1; k < size - i; k++)
				{
					step *= 5;
					if (step >= 7)
					{
						step = step % 7;
					}
				}
				int pow = step;
				for (int k = 1; k < j; k++)
				{
					step *= pow;
					if (step >= 7)
					{
						step = step % 7;
					}
				}
			}
			element = numberArray[j];
			sum += (element*step) % 7;
		}
		del = size;
		while (sum%del > 0)
		{
			sum *= 6;
			del *= 6;
			sum = sum % 7;
			del = del % 7;
		}
		resultArray[i] = sum / del;
	}
	return resultArray;
}

int* findPlaceError(int* sindrom)
{
	int tMatrix[3][3] = { {sindrom[2],sindrom[3],0},
							{sindrom[1],sindrom[2],sindrom[3]},    // Тёплицева матрица
							{sindrom[0],sindrom[1],sindrom[2]} };
	int fMatrix[3] = { 1,0,0 }; // Матрица поиска ошибок
	int rMatrix[3] = { 0,0,0 }; // Контрольная матрица
	int p1 = 0; // 2-й элемент матрицы поиска ошибок
	int p2 = 0; // 3-й элемент матрицы поиска ошибок
	for (int i = 0; i < 6; i++)// перебор значений второго элемента матрицы ошибок
	{
		for (int j = 0; j < 6; j++)// перебор значений третьего элемента матрицы ошибок            
		{
			fMatrix[1] = i; 
			fMatrix[2] = j;
			for (int m = 0; m < 3; m++) 
			{                              // вычисление произведения матриц 

				//                |t11 t12 t13|
				// [f1, f2, f3] * |t21 t22 t23| = [r1, r2, r3]
				//                |t31 t32 t33|
				int sum = 0;
				for (int k = 0; k < 3; k++)
				{
					sum += fMatrix[k] * tMatrix[k][m];
				}
				rMatrix[m] = sum;
			}
			if (rMatrix[0] % 7 == 0 && rMatrix[1] % 7 == 0) // Если первые два элемента контрольной матрицы равны нулю, то места ошибок найдены верно
			{
				p1 = i;
				p2 = j;
			}
		}
	}
	int* result = new int[3]{ 1, p1, p2 };
	return result;
}

void solveQuadraticEquation(int *place)
{
	int a = place[2];
	int b = place[1]; // множители квадратного уравнения Г(5^n)
	int c = place[0];

	cout << "Koeff quadr equ: (a,b,c)" << endl;
	cout << a << " " << b << " " << c << endl;
	cout << endl;

	cout << "Place errors" << endl;
	for (int i = 0; i < 7; i++)
	{
		int pow = 5;
		if (i == 0)
		{
			pow = 1;
		}
		else
		{
			for (int j = 1; j < i; j++)
			{
				pow *= 5;
				if (pow >= 7)
				{
					pow = pow % 7;
				}
			}
		}

		if ((a*pow*pow + b * pow + c) % 7 == 0)
		{
			cout << i << endl;
		}
	}
	cout << endl;
}

int* fixErrors(int* numberArray, int size, int* place)
{
	int pow = 1;
	for (int k = 0; k < size-1; k++)
	{
		pow *= place[2];
		if (pow >= 7)
		{
			pow = pow % 7;
		}
	}
	int *restylePlace = new int[3]{ (place[0] * pow)%7, (place[1] * pow)%7, 1 };

	cout << " Restyle quadratiq equation " << endl;
	cout << restylePlace[0] << " " << restylePlace[1] <<" "<< restylePlace[2] << endl;
	cout << endl;

	int f1 = - ( restylePlace[0])*numberArray[3] -
		( restylePlace[1])*numberArray[2];
	//cout << restylePlace[0] << " " << numberArray[3] << " " << restylePlace[1] << " " << numberArray[2]<<endl;
	int f0 = - ( restylePlace[0])*numberArray[2] -
		( restylePlace[1])*f1;
	//cout << restylePlace[0] << " " << numberArray[2] << " " << restylePlace[1] << " " << f1 << endl;
	
	f0 = f0 % 7;
	f1 = f1 % 7;

	cout << f0 << " " << f1 << endl;

	int* sindromVSplace = new int[size]{ f0, f1, numberArray[2] ,numberArray[3] ,numberArray[4] ,numberArray[5] };

	cout << "Sindrom + place error" << endl;
	for (int i = 0; i < size; i++)
	{
		cout << sindromVSplace[i] << " ";
	}
	cout << endl;

	cout << endl;
	int* exeption = IDFT(sindromVSplace, size);
	for (int i = 0; i < size; i++)
	{
		cout << exeption[i] << " ";
	}
	cout << endl;
	return exeption;
}


void myfix(int* numberArray, int size, int* place)
{
	char buf[255];
	for (int i = 0; i < 235; i++) buf[i] = i;// заполнили 235 информационных байтов
	int NEr = 10;                   // максимальное количесто ошибок
	c_form(NEr, 255);               // будем исправлять 10 ошибок, в буфере длиной 255 байт 
									// 235 информационых и 20 контрольных
	c_code(buf);                    // Тепрь buf длиной 255 содержит 20 кодовых байт+235 информационных

	for (int i = 0; i < NEr; i++)   // насажаем NEr ошибок
		buf[3 + 4 * i]++;

	int nErr = c_decode(buf);       // Теперь buf длиной 255 содержит 235 байт исходной информации
									// Оставшиеся 20 байт не используются
	printf("Nerr = %d\n", nErr);    // Так как ошибок мы сделали NErr=10, должно быть напечатано 10.
}

/*int main()
{
	const int size = 6;
 	int * array = new int[size]{ 3,1,0,0,0,0 };//mass
	int *result = IDFTtest(array, size);//результат который должен получиться

	cout << "IDFT start array" << endl;
	for (int i = 0; i < size; i++)
	{
		std::cout << result[i] << " ";
	}
	cout << endl << endl;

	array = DFT(result, size);

	cout << "DFT start array" << endl;
	for (int i = 0; i < size; i++)
	{
		std::cout << array[i] << " ";
	}
	cout << endl << endl;

	int* sindrom = new int[4]{ array[2],array[3],array[4],array[5] };

	cout << "Sindrom" << endl;
	cout << sindrom[0] << " " << sindrom[1] << " " << sindrom[2] << " " << sindrom[3] << endl;
	cout << endl;

	int* place = findPlaceError(sindrom);

	cout << "Place errors" << endl;
	for (int i = 0; i < 3; i++)
	{
		cout << place[i] << endl;
	}
	cout << endl;

	solveQuadraticEquation(place);

	int * exeption = fixErrors(array, size, place);
	for (int i = 0; i < size; i++)
	{
		result[i] = result[i] - exeption[i];
	}
	int *arrayResult = DFT(result, size);
	for (int i = 0; i < size; i++)
	{
		std::cout << arrayResult[i] << " ";
	}
	cout << endl;
	system("pause");
}*/

