#pragma once
#include<utility>
#include <vector>
#include <complex>
using namespace std;

typedef pair<int, int> my_point_int;
typedef complex<double> base;

//гауссов купол
struct gauss
{
	//амплитуда купола
	double A;
	//максимум купола
	my_point_int p0;
	//ширина купола
	double sigma;

	//конструкторы
	gauss(double _A, int _x0, int _y0, double _s);
	gauss() { }
};

//класс для работы с изображением
class my_image
{
	//исходное, зашумленное, отфильтрованное изображения и его ампл. спектр
	vector<vector<double>> image0, image_shum, image_res, ampl_spec;
	//параметры зашумления и очищения
	double alpha, gamma;
	//ширина и высота изображения
	int w, h;
	//для зануления левого верхнего угла
	base need;

public:
	//конструкторы
	my_image() {}
	my_image(vector<gauss> many_gauss, int _w, int _h, double a, double g); //если гауссов купол
	my_image(vector<vector<double>> matr, double a, double g); //если изображение (сюда можно перенести доопределение до степени 2)
	//функция генерации шума
	void generate_shum(std::vector<std::vector<double>>& shum);
	//функция, которая накладывает шум
	void generate_pic_with_shum();
	//функция высчитывающая энергию изображения
	double energy(std::vector<std::vector<double>> image);
	//БПФ
	void fft(vector<base>& a, bool invert);
	//преобразование Фурье по строкам и столбцам
	void fourea_image(std::vector<std::vector<base>>& vec, bool invert);
	//получение колонки матрицы (по ссылке сделать, чтобы не было копирования)
	void get_column(std::vector<base>& res, std::vector<std::vector<base>> matr, int num);
	//установление колонки
	void set_column(std::vector<std::vector<base>>& matr, std::vector<base> vec, int num);
	//фильтрация (оптимизировать)
	void filter(std::vector<std::vector<base>>& fourea);
	//поиск ошибки (сделать через 2 параметра)
	double find_error();
	//получение ампл. спектра (без копирования)
	void get_ampl_spec(std::vector<std::vector<base>> matr);
	//установка параметра зашумления
	void set_alpha(double a);
	//установка параметра фильтрации
	void set_gamma(double g);
	//функции очищения (во избежании ошибок)
	void clear();
	void clear1();
	//вся последовательность действий
	void Process();
	//Симметрия ампл. спектра
	void Simmetria(vector<vector<base>>& fourea);
	//получение спектра из середины
	void NewSpectr(vector<vector<base>>& new_vec, vector<vector<base>> fourea, int num, double& energy);
	//очищение изображения
	void ProcessClearImage();

	//получение изображений на выходе
	vector<vector<double>> GetImageStart();
	vector<vector<double>> GetImageShum();
	vector<vector<double>> GetImageRes();
	vector<vector<double>> GetAmplSpectr();
};