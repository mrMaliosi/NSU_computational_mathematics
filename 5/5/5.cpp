#include <iostream>
#include <cmath>
#include <functional>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>

using namespace std;
using namespace std::chrono;

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
const std::string NEWTON = LIGHT_YELLOW + "<Ньютон>: " + RESET;
const std::string SECANT = ORANGE + "<Секущая>: " + RESET;

#define SCALE 0.01

//////////////////File names manipulation//////////////////
std::string add_string_base_to_file(std::string file_name, std::string number = "") {
	size_t string_length = file_name.size();
	size_t pos = file_name.rfind(".");
	std::string format_string;
	for (int i = pos; i < string_length; ++i) {
		format_string += file_name[i];
	}
	file_name.erase(pos);
	file_name = file_name + "_base_" + number + format_string;
	return file_name;
}

std::string add_file_numeration(std::string file_name, string number) {
	size_t string_length = file_name.size();
	size_t pos = file_name.rfind(".");
	std::string format_string;
	for (int i = pos; i < string_length; ++i) {
		format_string += file_name[i];
	}
	file_name.erase(pos);
	file_name = file_name + "_" + number + format_string;
	return file_name;
}

//////////////////Function builders//////////////////
void function_builder(function <double(double, double)> func, double a, double b, double param, string output_file, string number = "") {
	output_file = add_string_base_to_file(output_file, number);
	std::ofstream outFile(output_file);
	outFile << "x,y\n";
	for (double xi = a; xi <= b; xi += SCALE) {
		outFile << xi << "," << func(xi, param) << "\n";
	}
	outFile.close();
}

#define PI 3.14

// Функция f(x) и её производная f'(x)
double f1(double x, double a) {
	return x - exp(x) + a;
}

// Для метода секущих нам нужна функция f(x)
double df1(double x, double a) {
	return 1 - exp(x);
}

double f2(double x, double l) {
	return x - l * cos(x - PI/2);
}

// Для метода секущих нам нужна функция f(x)
double df2(double x, double l) {
	return 1 + l * sin(x - PI / 2);
}

class Examples {
public:

	typedef struct ExampleInfo {
		std::function<double(double, double)> function;
		std::function<double(double, double)> function_der;
	} ExampleInfo;

	std::vector <ExampleInfo> examples;

	// Конструктор
	Examples() {
		ExampleInfo example0 = {
			{f1}, {df1}
		};
		ExampleInfo example1 = {
			{f2}, {df2}
		};

		examples.push_back(example0);
		examples.push_back(example1);
	}

	// Получить f i-ого example
	std::function<double(double, double)> get_example_function(int i) {
		if (i > examples.size()) {
			std::cout << ERROR + "Ну вот, чудесно. Они мне прислали И-ДИ-ОТА! Слушай сюда! Ещё раз ЧИ-ТА-ЕШЬ инструкцию к программе и возвращаешься сюда, чтобы получить вычисление своего примера! СВОБОДЕН!" << std::endl;
			return {};
		}

		return examples[i].function;
	}

	// Получить fd i-ого example
	std::function<double(double, double)> get_example_function_der(int i) {
		if (i > examples.size()) {
			std::cout << ERROR + "И не сметь обуждать приказы начальства!" << std::endl;
			return {};
		}

		return examples[i].function_der;
	}

	~Examples() {
		examples.clear();
	}

};

int file_number = 0;

// Метод Ньютона
std::vector <double> newton_method(std::function<double(double, double)> f, std::function<double(double, double)> df, double x0, double a, string output_file, double eps = 1e-7, int max_iter = 10000) {
	output_file = add_file_numeration(output_file, "newton_" + to_string(file_number));
	std::ofstream outFile(output_file);
	outFile << "x,y\n";
	std::vector <double> ans;
	int iter_count = 0;
	while (1 > 0) {
		double x = x0;
		if (ans.size() > 0) {
			x = ans[ans.size() - 1] - 1;
		}
		int i;
		for (i = 0; i < max_iter; ++i) {
			double fx = f(x, a);
			double dfx = df(x, a);
			
			std::vector <double> deriv_arr (ans.size(), 1);
			double denominator = 1;
			for (int j = 0; j < ans.size(); ++j) {
				dfx *= (x - ans[j]);
				denominator *= (x - ans[j])*(x - ans[j]);
				for (int k = 0; k < ans.size(); ++k) {
					if (k != j) {
						deriv_arr[k] *= (x - ans[j]);
					}
				}
			}

			double der_of_der = 0;
			for (int k = 0; k < ans.size(); ++k) {
				der_of_der += deriv_arr[k];
			}
			dfx = (dfx - fx * der_of_der) / denominator;
			for (int k = 0; k < ans.size(); ++k) {
				fx /= (x - ans[k]);
			}
			
			
			if (fabs(dfx) < 1e-12) {  // Проверка на очень маленькую производную
				cout << NEWTON + "Метод Ньютона завершен за " << iter_count << " итераций." << endl;
				return ans;
			}

			outFile << x << "," << fx << "\n";
			// Обновление x с использованием метода Ньютона
			double x_new = x - fx / dfx;
			iter_count++;

			// Проверка на сходимость
			if (fabs(x_new - x) < eps) {
				ans.push_back(x_new);
				break;
			}

			// Обновляем значение x
			x = x_new;
		}

		if (i == max_iter) {
			cout << NEWTON + "Метод Ньютона завершен за " << iter_count << " итераций." << endl;
			return ans;
		}
	}
	
}

// Метод секущих
std::vector <double> secant_method(std::function<double(double, double)> f, double x0, double x1, double a, string output_file, double eps = 1e-7, int max_iter = 10000) {\
	output_file = add_file_numeration(output_file, "secant_" + to_string(file_number));
	std::ofstream outFile(output_file);
	outFile << "x_curr,y_curr,x_prev,y_prev\n";
	std::vector <double> ans;
	int iter_count = 0;
	while (1 > 0) {
		double x_prev = x0;
		double x_curr = x1;
		if (ans.size() > 0) {
			x_prev = ans[ans.size() - 1] - 2;
			x_curr = ans[ans.size() - 1] - 1;
		}
		int i;
		for (i = 0; i < max_iter; ++i) {
			double f_prev = f(x_prev, a);
			double f_curr = f(x_curr, a);
			for (int k = 0; k < ans.size(); ++k) {
				f_prev /= (x_prev - ans[k]);
				f_curr /= (x_curr - ans[k]);
			}
			if (std::isnan(f_prev) || std::isnan(f_curr) || fabs(f_prev) > 1e+12 || fabs(f_curr) > 1e+12) {  
				cout << SECANT + "Метод секущих завершен за " << iter_count << " итераций.\n";
				return ans;
			}
			// Формула для нахождения следующего значения
			double x_next = x_curr - f_curr * (x_curr - x_prev) / (f_curr - f_prev);
			iter_count++;
			outFile << x_curr << "," << f_curr << "," << x_prev << "," << f_prev << "\n";
			//cout << "Итерация " << iter_count << ": x = " << x_next << " (интервал: [" << x_curr << ", " << x_next << "])\n";

			if (fabs(x_next - x_curr) < eps) {
				ans.push_back(x_next);
				break;
			}

			// Обновляем значения
			x_prev = x_curr;
			x_curr = x_next;
		}

		if (i == max_iter) {
			cout << SECANT + "Метод секущих завершен за " << iter_count << " итераций.\n";
			return ans;
		}
	}
}

template<typename T>
void vector_print(const vector<T>& q) {
	for (const auto& elem : q) {
		cout << elem << " ";
	}
	cout << endl;
}

int main(int argc, char * argv[]) {
	system("chcp 1251 > nul");
	double a, l;
	std::string output_file = "data.csv";
	if (argc == 1) {
		cout << "Введите a и l: " << endl;
		cin >> a >> l;
	}
	else if (argc != 4) {
		std::cerr << "Usage: " << argv[0] << " <output_file> <parametr_a> <parametr_l>" << std::endl;
		return 1;
	}
	else {
		std::string output_file = argv[1];
		a = stod(argv[2]);
		l = stod(argv[3]);
	}
	Examples examples_storage;
	for (int i = 0; i < 2; ++i) {
		file_number = i;
		std::function<double(double, double)> f = examples_storage.get_example_function(i);
		std::function<double(double, double)> df = examples_storage.get_example_function_der(i);
		std::vector <double> ans;
		if (i == 1) {
			a = l;
		}

		function_builder(f, -50, 50, a, output_file, to_string(i));

		// Вводим начальные приближения
		double x0 = 2.0;
		double x1 = 2.0, x2 = 3.0;  // Начальные приближения для метода секущих

		std::cout << DELIMITER << std::endl;
		std::cout << DARK_ORANGE + "EXAMPLE: " + GREEN << i << RESET << endl;
		auto start_simple_iter_time = high_resolution_clock::now();
		ans = newton_method(f, df, x0, a, output_file);
		auto stop_simple_iter_time = high_resolution_clock::now();
		auto duration_simple = duration_cast<microseconds>(stop_simple_iter_time - start_simple_iter_time);
		if (ans.size() == 0) {
			cout << NEWTON + "Корни: нет." << endl;
		} else {
			cout << NEWTON + "Корни: "; vector_print(ans);
		}
			
		cout << NEWTON + "Время работы: " << duration_simple.count() << " микросекунд.\n";

		auto start_secant = high_resolution_clock::now();
		ans = secant_method(f, x1, x2, a, output_file);
		auto stop_secant = high_resolution_clock::now();
		auto duration_secant = duration_cast<microseconds>(stop_secant - start_secant);
		if (ans.size() == 0) {
			cout << SECANT + "Корни: нет." << endl;
		}
		else {
			cout << SECANT + "Корни: нет."; vector_print(ans);
		}
		cout << SECANT + "Время работы: " << duration_secant.count() << " микросекунд.\n";
	}
	
	return 0;
}
