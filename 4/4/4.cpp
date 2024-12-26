#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <functional>
#include <string>
#include <set>

//////////////////For some beauty//////////////////
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string PURPLE = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[97m";
const std::string ORANGE = "\033[38;5;214m";
const std::string LIME = "\033[32;5;190m";
const std::string LIGHT_YELLOW = "\033[38;5;190m";
const std::string DARK_ORANGE = "\033[38;5;208m";
const std::string PINK = "\033[38;5;240m";

const std::string ERROR = RED + "<ERROR>: " + RESET;
const std::string SUCCESS = GREEN + "<SUCCESS>:" + RESET;
const std::string DELIMITER = PINK + "------------------------------------------------------------------------------" + RESET;
const std::string TRAPETION = LIGHT_YELLOW + "<Трапеция>: " + RESET;
const std::string SYMPSON = ORANGE + "<Симпсон>: " + RESET;

#define EPS 0.00001

typedef unsigned int ui;

using namespace std;

class Examples {
public:

	typedef struct ExampleInfo {
		std::vector <std::vector <double> > matrix;
		std::vector <double> b;
	} ExampleInfo;

	std::vector <ExampleInfo> examples;

	// Конструктор
	Examples() {
		ExampleInfo example0 = {
			{{1.0, 0.5, 1.0 / 3}, {0.5, 1.0 / 3, 1.0 / 4}, {1.0 / 3, 1.0 / 4, 0.2}}, {1, 1, 1}
		};
		ExampleInfo example1 = {
			{{1.0, 2, 3}, {1, 3, 2}, {2, 7, 4}}, {1, 1, 1}
		};
		ExampleInfo example2 = {
			{{4, 1, 0, 0}, {1, 4, 1, 0}, {0, 1, 4, 1}, {0, 0, 1, 4}}, {5, 5, 5, 5}
		};
		ExampleInfo example3 = {
			{{4, 1, 0, 0}, {1, 4, 1, 0}, {0, 1, 4, 1}, {0, 0, 1, 4}}, {5, 5, 5, 5}
		};

		examples.push_back(example0);
		examples.push_back(example1);
		examples.push_back(example2);
	}

	// Функция для вывода информации о примерах
	void examples_info() {
		std::cout << "Введите номер для выбора примера." << std::endl;
		std::cout << "0. Матрица Гильберта." << std::endl;
		std::cout << "1. Матрица с контрольной." << std::endl;
		std::cout << "2. Трёхдиагональная матрица." << std::endl;
	}

	// Получить matrix i-ого example
	std::vector <std::vector <double> > get_example_matrix(int i) {
		if (i > examples.size()) {
			cout << ERROR + "Ну вот, чудесно. Они мне прислали И-ДИ-ОТА! Слушай сюда! Ещё раз ЧИ-ТА-ЕШЬ инструкцию к программе и возвращаешься сюда, чтобы получить вычисление своего примера! СВОБОДЕН!" << endl;
			return {};
		}
			
		return examples[i].matrix;
	}

	// Получить b i-ого example
	std::vector <double> get_example_b(int i) {
		if (i > examples.size()) {
			cout << ERROR + "И не сметь обуждать приказы начальства!" << endl;
			return {};
		}

		return examples[i].b;
	}

	~Examples() {
		int l = examples.size();
		for (int i = 0; i < l; ++i) {
			int ll = examples[i].matrix.size();
			for (int j = 0; j < ll; ++j) {
				examples[i].matrix[j].clear();
			}
			examples[i].matrix.clear();
		}
		examples.clear();
	}

};


////////////////////////////////////LU////////////////////////////////////
void LU(vector <vector <double>> A, vector <vector <double>> &L, vector <vector <double>> &U, int n)
{
	U = A;

	for (int i = 0; i < n; i++)
		for (int j = i; j < n; j++)
			L[j][i] = U[j][i] / U[i][i];

	for (int k = 1; k < n; k++)
	{
		for (int i = k - 1; i < n; i++)
			for (int j = i; j < n; j++)
				L[j][i] = U[j][i] / U[i][i];
			

		for (int i = k; i < n; i++)
			for (int j = k - 1; j < n; j++)
				U[i][j] = U[i][j] - L[i][k - 1] * U[k - 1][j];
	}
}

void Ly(vector <vector <double>> &L, vector <double> &y, vector <double> &b, int n) {
	for (int i = 0; i < n; ++i) {
		y[i] = b[i];
		for (int j = 0; j < i; ++j) {
			y[i] -= y[j] * L[i][j];
		}
		y[i] = y[i] / L[i][i];
	}
}

void Ux(vector <vector <double>> &U, vector <double> &x, vector <double> &y, int n) {
	for (int i = n - 1; i >= 0; --i) {
		x[i] = y[i];
		for (int j = n - 1; j > i; --j) {
			x[i] -= x[j] * U[i][j];
		}
		x[i] = x[i] / U[i][i];
	}
}

////////////////////////////////////Jacoby////////////////////////////////////
//TO DO добавить перестановку строк для недопущения нулевых элементов на диагонали
void Jacob(vector <vector <double>> &A, vector <double> &x, vector <double> &b, int n) {
	int k = 0;
	set <double> s;
	double start_value = 0.3333;
	vector <double> x1 (n, 0);
	for (int i = 0; i < n; ++i) {
		x[i] = 0;
	}

	double eps;
	do {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				if (j == i) {
					//Ничего не делаем
				} else {
					x1[i] += A[i][j] * x[j];
				}
			}

			x1[i] = (double)1 / A[i][i] * (b[i] - x1[i]);
		}

		//Копируем
		for (int i = 0; i < n; ++i) {
			x[i] = x1[i];
			x1[i] = 0;
		}


		//Вычисляем погрешность
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				x1[i] += A[i][j] * x[j];
			}

			x1[i] -= b[i];
		}
		eps = 0;
		for (int i = 0; i < n; ++i) {
			eps += x1[i] * x1[i];
		}
		eps = sqrt(eps);
	} while (eps > EPS);
}
						//vector <vector <double>> &A, vector <double> &x, vector <double> &b, int n
void thomas_algorithm(vector<vector <double>>& A, vector<double>& b, vector<double>& x, int n) {
	// Прямой ход
	vector<double> csi(n + 1), teta(n + 1);
	csi[0] = 0;
	teta[0] = 0;
	csi[1] = - A[0][1] / A[0][0];
	teta[1] = b[0] / A[0][0];

	for (int i = 1; i < n; ++i) {
		if (i + 1 < n) {
			csi[i + 1] = -A[i][i + 1] / (A[i][i - 1] * csi[i] + A[i][i]);
		}
		teta[i + 1] = (b[i] - A[i][i - 1] * teta[i]) / (A[i][i - 1] * csi[i] + A[i][i]);
	}

	// Обратный ход
	x[n - 1] = teta[n];
	for (int i = n - 2; i >= 0; --i) {
		x[i] = csi[i + 1] * x[i + 1] + teta[i + 1];
	}
}

template<typename T>
void vector_print(const vector<T>& q) {
	for (const auto& elem : q) {
		cout << elem << " ";
	}
	cout << endl;
}

void A_normalize()

typedef struct Answer {
	double ans;
	double error;
} answer;

std::vector<double> examples_ans = { 1.29584, 1.32196 };


std::vector <std::vector <double> > A;
std::vector <std::vector <double> > L;
std::vector <std::vector <double> > U;
std::vector <double> b;
std::vector <double> y;
std::vector <double> x;

int main(int argc, char * argv[]) {
	system("chcp 1251 > null");
	int user_input;
	int n, m;
	cout << "Введите 1, если хотите ручной ввод или 0, если хотите выбрать пример: ";
	cin >> user_input;
	if (user_input == 1) {
		cout << "Введите количество неизвестных: ";
		cin >> n;
		A.resize(n, vector <double>(n));
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				cin >> A[i][j];
			}
		}

		b.resize(n);
		cout << "Введите значения вектора b: ";
		for (int i = 0; i < n; ++i) {
			cin >> b[i];
		}
	}
	else {
		Examples examples_storage;
		examples_storage.examples_info();
		int exampleNumber;
		cin >> exampleNumber;
		A = examples_storage.get_example_matrix(exampleNumber);
		b = examples_storage.get_example_b(exampleNumber);
		while (A.size() == 0) {
			A = examples_storage.get_example_matrix(exampleNumber);
			b = examples_storage.get_example_b(exampleNumber);
		}
	}

	//////////////////LU//////////////////
	n = A.size();
	L.resize(n, vector <double>(n));
	U.resize(n, vector <double>(n));
	x.resize(n);
	y.resize(n);
	LU(A, L, U, n);
	Ly(L, y, b, n);
	Ux(U, x, y, n);
	cout << PURPLE + "[LU]" + RESET << endl;
	cout << "Vector x is: ";
	vector_print(x);
	cout << endl;


	//////////////////Jacob//////////////////
	Jacob(A, x, b, n);
	cout << ORANGE + "[Jacob]" + RESET << endl;
	cout << "Vector x is: ";
	vector_print(x);
	cout << endl;


	//////////////////Progonka//////////////////
	thomas_algorithm(A, b, x, n);
	cout << PURPLE + "[Progonka]" + RESET << endl;
	cout << "Vector x is: ";
	vector_print(x);
	cout << endl;


	examples_ans.clear();
	return 0;
}
