#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <functional>
#include <string>

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


#define SCALE 0.01

typedef unsigned int ui;

using namespace std;

typedef struct Segment {
	double a;
	double b;
} segment;

typedef struct Example {
	std::function <double(double)> f;
	std::function <double(double)> F;
	std::function <double(double)> f_der2;
	std::function <double(double)> f_der4;
	segment Ex;
} example;

typedef struct Answer {
	double ans;
	double error_analytics;
	double error_runge;
} answer;

std::vector<example> examples;
std::vector<double> examples_ans = {1.29584, 1.32196};


//////////////////Examples//////////////////
double f0(double x) {
	return log(1 + x); 
}

double f0_der2(double x) {
	return -1.0 / ((1 + x)*(1 + x));
}

double f0_der4(double x) {
	return -6.0 / ((1 + x)*(1 + x)*(1 + x)*(1 + x));
}

double F0(double x) {
	return log(1 + x) * (1 + x) - x;
}

double f1(double x) {
	return exp(x)*cos(x);
}

double f1_der2(double x) {
	return -2 * exp(x)*sin(x);
}

double f1_der4(double x) {
	return -4 * exp(x)*cos(x);
}

double F1(double x) {
	return (exp(x)*cos(x) + sin(x)*exp(x))/2;
}


//////////////////File names manipulation//////////////////
std::string add_string_base_to_file(std::string file_name) {
	size_t string_length = file_name.size();
	size_t pos = file_name.rfind(".");
	std::string format_string;
	for (int i = pos; i < string_length; ++i) {
		format_string += file_name[i];
	}
	file_name.erase(pos);
	file_name = file_name + "_" + "base" + format_string;
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
void function_builder(function <double(double)> func, double a, double b, string output_file) {
	output_file = add_string_base_to_file(output_file);
	std::ofstream outFile(output_file);
	outFile << "x,y\n";
	for (double xi = a; xi <= b; xi += SCALE) {
		outFile << xi << "," << func(xi) << "\n";
	}
	outFile.close();
}


//////////////////Integral interoplations//////////////////
answer trapezoidal_interpolation(example ex, int intervals_number, string output_file, string file_number, bool print_errors) {
	double a = ex.Ex.a;
	double b = ex.Ex.b;
	std::function <double(double)> f = ex.f;
	std::function <double(double)> F = ex.F;
	double sum = 0.0;
	double step_size = (b - a) / (intervals_number);

	if (print_errors){
		output_file = add_file_numeration(output_file, file_number);
		std::ofstream outFile(output_file);
		outFile << "x,y\n";
		outFile << a << "," << f(a) << "\n";
		for (int i = 0; i < intervals_number; ++i) {
			sum += (f(a + i*step_size) + f(a + (i + 1) * step_size)) * step_size / 2.0;
			outFile << a + (i + 1) * step_size << "," << f(a + (i + 1) * step_size) << "\n";
		}
		outFile.close();
	}
	else {
		for (int i = 0; i < intervals_number; ++i) {
			sum += (f(a + i * step_size) + f(a + (i + 1) * step_size)) * step_size / 2.0;
		}
	}

	answer Ans, Ans2;
	Ans.ans = sum;
	if (print_errors) {
		//Вычисляем аналитическую погрешность
		double maxi_der2 = ex.f_der2(a);
		for (double xi = a + SCALE; xi <= b; xi += SCALE) {
			if (ex.f_der2(xi) > maxi_der2) {
				maxi_der2 = ex.f_der2(xi);
			}
		}
		Ans.error_analytics = -maxi_der2 * (b - a)*step_size*step_size / 12 ;

		//Вычисляем погрешность Рунге
		Ans2 = trapezoidal_interpolation(ex, intervals_number * 2, output_file, file_number, false);
		Ans.error_runge = (2 * 2 * (Ans2.ans - Ans.ans)) / (2 * 2 - 1);
	}
	
	return Ans;
}

answer trapezoidal_interpolation(example ex, int intervals_number, string output_file, string file_number) {
	return trapezoidal_interpolation(ex, intervals_number, output_file, file_number, true);
}

answer simpson_interpolation(example ex, int intervals_number, string output_file, string file_number, bool print_errors) {
	double a = ex.Ex.a;
	double b = ex.Ex.b;
	std::function <double(double)> f = ex.f;
	std::function <double(double)> F = ex.F;
	double sum = 0.0;
	double step_size = (b - a) / (intervals_number);
	double x0 = a, x1 = a + step_size;

	if (print_errors) {
		output_file = add_file_numeration(output_file, file_number);
		std::ofstream outFile(output_file);
		outFile << "x,y\n";
		outFile << x0 << "," << f(x0) << "\n";
		for (int i = 0; i < intervals_number; ++i) {
			sum += (f(x0) + 4 * f((x0 + x1) / 2) + f(x1)) * step_size / 6.0;
			outFile << (x0 + x1) / 2 << "," << f((x0 + x1) / 2) << "\n";
			outFile << x1 << "," << f(x1) << "\n";
			x0 += step_size;
			x1 += step_size;
		}
		outFile.close();
	}
	else {
		for (int i = 0; i < intervals_number; ++i) {
			sum += (f(x0) + 4 * f((x0 + x1) / 2) + f(x1)) * step_size / 6.0;
			x0 += step_size;
			x1 += step_size;
		}
	}
	

	answer Ans, Ans2;
	Ans.ans = sum;
	if (print_errors) {
		//Вычисляем аналитическую погрешность
		double maxi_der4 = ex.f_der4(a);
		for (double xi = a + SCALE; xi <= b; xi += SCALE) {
			if (ex.f_der4(xi) > maxi_der4) {
				maxi_der4 = ex.f_der4(xi);
			}
		}
		Ans.error_analytics = -maxi_der4 * (b - a)*step_size*step_size*step_size*step_size / 90;

		//Вычисляем погрешность Рунге
		Ans2 = simpson_interpolation(ex, intervals_number * 2, output_file, file_number, false);
		Ans.error_runge = (2 * 2 * 2 * 2 * (Ans2.ans - Ans.ans)) / (2 * 2 * 2 * 2 - 1);
	}

	return Ans;
}

answer simpson_interpolation(example ex, int intervals_number, string output_file, string file_number) {
	return simpson_interpolation(ex, intervals_number, output_file, file_number, true);
}

int main(int argc, char * argv[]) {
	examples.push_back({ f0, F0, f0_der2, f0_der4, {0, 2} });
	examples.push_back({ f1, F1, f1_der2, f1_der4, {0, 2} });
	system("chcp 1251 > nul");
	if (argc != 6) {
		std::cerr << "Usage: " << argv[0] << " <output_file> <test_case> <intervals_number> <iteration_number> <intervals_changes>" << std::endl;
		return 1;
	}
	std::string output_file = argv[1];
	int test_case = std::atoi(argv[2]);
	int intervals_number = std::atoi(argv[3]);
	int iteration_number = std::atoi(argv[4]);
	int intervals_changes = std::atoi(argv[5]);
	function_builder(examples[test_case].f, examples[test_case].Ex.a, examples[test_case].Ex.b, output_file);

	string file_number;
	for (int t = 0; t < iteration_number; ++t) {
		file_number = "trapezoidal" + to_string(t);
		answer answer_trapezoidal = trapezoidal_interpolation(examples[test_case], intervals_number, output_file, file_number);

		file_number = "simpson" + to_string(t);
		answer answer_simpson = simpson_interpolation(examples[test_case], intervals_number, output_file, file_number);

		std::cout << DELIMITER << std::endl;
		std::cout << DARK_ORANGE + "Количество интервалов: " + GREEN << intervals_number << RESET << endl;
		std::cout << TRAPETION << "Реальный результат интегрирования: " + CYAN << examples_ans[test_case] << RESET << endl;
		std::cout << TRAPETION << "Приближённый: " + CYAN << answer_trapezoidal.ans << RESET << std::endl;
		std::cout << TRAPETION << "Погрешность по аналитическим формулам: " + CYAN << answer_trapezoidal.error_analytics << RESET << std::endl;
		std::cout << TRAPETION << "Погрешность по правилу Рунге: " + CYAN << answer_trapezoidal.error_runge << RESET << std::endl;
		std::cout << SYMPSON << "Реальный результат интегрирования: " + CYAN << examples_ans[test_case] << RESET << endl;
		std::cout << SYMPSON << "Приближённый: " + CYAN << answer_simpson.ans << RESET << std::endl;
		std::cout << SYMPSON << "Погрешность по аналитическим формулам: " + CYAN << answer_simpson.error_analytics << RESET << std::endl;
		std::cout << SYMPSON << "Погрешность по правилу Рунге: " + CYAN << answer_simpson.error_runge << RESET << std::endl;
		intervals_number += intervals_changes;
	}

	examples.clear();
	examples_ans.clear();
	return 0;
}
