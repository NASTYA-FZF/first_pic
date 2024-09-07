#pragma once
#include<utility>
#include <vector>
#include <complex>
using namespace std;

typedef pair<int, int> my_point_int;
typedef complex<double> base;

struct gauss
{
	double A;
	my_point_int p0;
	double sigma;

	gauss(double _A, int _x0, int _y0, double _s);
	gauss() { }
};

class my_image
{
	vector<vector<double>> image0, image_shum, image_res;
	double alpha, gamma;
	int w, h;

public:
	my_image() {}
	my_image(vector<gauss> many_gauss, int _w, int _h, double a, double g);
	std::vector<std::vector<double>> generate_shum();
	void generate_pic_with_shum();
	double energy(std::vector<std::vector<double>> image);
	void fft(vector<base>& a, bool invert);
	std::vector<std::vector<base>> fourea_image();
	std::vector<base> get_column(std::vector<std::vector<base>> matr, int num);
	void set_column(std::vector<std::vector<base>>& matr, std::vector<base> vec, int num);
	void filter(std::vector<std::vector<base>>& fourea);
	double find_error();
	std::vector<std::vector<double>> get_ampl_spec(std::vector<std::vector<base>> matr, double& maximum);
	void set_alpha(double a);
	void set_gamma(double g);
	void clear();
};