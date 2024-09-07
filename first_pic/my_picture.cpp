#include "pch.h"
#define _USE_MATH_DEFINES
#define P2(x) (x) * (x)
#define EXP_G(x, x0, s) exp(-P2(x - x0) / (s * P2(x)))
#define AMPL(x) sqrt(x.real() * x.real() + x.imag() * x.imag())
#include "my_picture.h"
#include <math.h>

gauss::gauss(double _A, int _x0, int _y0, double _s)
{
	A = _A; p0 = my_point_int(_x0, _y0); sigma = _s;
}

my_image::my_image(vector<gauss> _gauss, int _w, int _h, double a, double g)
{
	clear();
	w = _w; h = _h; alpha = a; gamma = g;

	for (int i = 0; i < _gauss.size(); i++)
	{
		if (_gauss[i].p0.first > w || _gauss[i].p0.second > h)
		{
			MessageBox(NULL, L"Некорректно заданы x0 или y0!", L"Предупреждение", NULL);
			break;
		}
	}

	image0 = vector<vector<double>>(h, vector<double>(w));

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			image0[i][j] = 0.;
			for (int k = 0; k < _gauss.size(); k++)
				image0[i][j] += _gauss[k].A * EXP_G(j, _gauss[k].p0.first, _gauss[k].sigma) * EXP_G(i, _gauss[k].p0.second, _gauss[k].sigma);
		}
	}
}

std::vector<std::vector<double>> my_image::generate_shum()
{
	vector<vector<double>> n0;
	for(int i = 0; i < h; i++)
	{
		n0.push_back(vector<double>());
		for (int j = 0; j < w; j++)
		{
			n0[i].push_back((double)(rand()) / RAND_MAX * 2 - 1);
		}
	}

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			for (int k = 0; k < 11; k++)
			{
				n0[i][j] += (double)(rand()) / RAND_MAX * 2 - 1;
			}
		}
	}

	return n0;
}

void my_image::generate_pic_with_shum()
{
	auto sh = generate_shum();
	double SumSh = energy(sh);

	double bet = sqrt(alpha * energy(image0) / SumSh);

	for (int i = 0; i < h; i++)
	{
		image_shum.push_back(vector<double>());
		for (int j = 0; j < w; j++)
		{
			image_shum[i].push_back(image0[i][j] + bet * sh[i][j]);
		}
	}
}

double my_image::energy(std::vector<std::vector<double>> image)
{
	double res = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			res += image[i][j] * image[i][j];
		}
	}
	return res;
}

void my_image::fft(vector<base>& a, bool invert)
{
	int n = (int)a.size();
	if (n == 1)  return;

	vector<base> a0(n / 2), a1(n / 2);
	for (int i = 0, j = 0; i < n; i += 2, ++j) {
		a0[j] = a[i];
		a1[j] = a[i + 1];
	}
	fft(a0, invert);
	fft(a1, invert);

	double ang = 2 * M_PI / n * (invert ? -1 : 1);
	base w(1), wn(cos(ang), sin(ang));
	for (int i = 0; i < n / 2; ++i) {
		a[i] = a0[i] + w * a1[i];
		a[i + n / 2] = a0[i] - w * a1[i];
		if (invert)
			a[i] /= 2, a[i + n / 2] /= 2;
		w *= wn;
	}
}

std::vector<std::vector<base>> my_image::fourea_image()
{
	vector<vector<base>> fourea(h, vector<base>(w));
	for (int i = 0; i < h; i++)
	{
		fourea[i] = vector<base>(image_shum[i].begin(), image_shum[i].end());
		fft(fourea[i], true);
	}

	vector<base> vec_help;
	for (int j = 0; j < w; j++)
	{
		vec_help = get_column(fourea, j);
		fft(vec_help, true);
		set_column(fourea, vec_help, j);
	}
	fourea[0][0] = base(0, 0);

	return fourea;
}

std::vector<base> my_image::get_column(std::vector<std::vector<base>> matr, int num)
{
	std::vector<base> res;

	for (int row = 0; row < h; row++)
	{
		res.push_back(matr[row][num]);
	}

	return res;
}

void my_image::set_column(std::vector<std::vector<base>>& matr, std::vector<base> vec, int num)
{
	for (int row = 0; row < h; row++)
	{
		matr[row][num] = vec[row];
	}
}

void my_image::filter(std::vector<std::vector<base>>& fourea)
{
	double max_A = 0;
	vector<vector<double>> ampl_spec = get_ampl_spec(fourea, max_A);
	
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (ampl_spec[i][j] < gamma * max_A)
				fourea[i][j] = base(0, 0);
		}
	}
}

double my_image::find_error()
{
	double znamen = energy(image0);
	double chislit = 0;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			chislit += P2(image0[i][j] - image_res[i][j]);
		}
	}
	double res = chislit / znamen * 100;
	return res;
}

std::vector<std::vector<double>> my_image::get_ampl_spec(std::vector<std::vector<base>> matr, double& maximum)
{
	vector<vector<double>> ampl_spec;
	for (int i = 0; i < h; i++)
	{
		ampl_spec.push_back(vector<double>());
		for (int j = 0; j < w; j++)
		{
			ampl_spec[i].push_back(AMPL(matr[i][j]));
			if (maximum < ampl_spec[i][j])
				maximum = ampl_spec[i][j];
		}
	}
	return ampl_spec;
}

void my_image::set_alpha(double a)
{
	alpha = a;
}

void my_image::set_gamma(double g)
{
	gamma = g;
}

void my_image::clear()
{
	image_shum.clear();
	image_res.clear();
	image0.clear();
}
