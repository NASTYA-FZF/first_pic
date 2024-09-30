#include "pch.h"
#define _USE_MATH_DEFINES
#define P2(x) ((x)*(x))
#define EXP_G(x, x0, s) exp(-(double)P2(x - x0) / (double)P2(s))
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
	w = _w; h = _h; alpha = a / 100; gamma = g / 100;

	for (int i = 0; i < _gauss.size(); i++)
	{
		if (_gauss[i].p0.first > w || _gauss[i].p0.second > h)
		{
			MessageBox(NULL, L"Некорректно заданы x0 или y0!", L"Предупреждение", NULL);
			break;
		}
	}

	image0 = vector<vector<double>>(h, vector<double>(w));
	double e1, e2;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			image0[i][j] = 0.;
			for (int k = 0; k < _gauss.size(); k++)
			{
				e1 = EXP_G(j, _gauss[k].p0.first, _gauss[k].sigma);
				e2 = EXP_G(i, _gauss[k].p0.second, _gauss[k].sigma);
				image0[i][j] += _gauss[k].A * e1 * e2;
			}
		}
	}
}

my_image::my_image(vector<vector<double>> matr, double a, double g)
{
	clear();
	image0 = matr;
	h = image0.size();
	w = image0[0].size();
	alpha = a / 100;
	gamma = g / 100;
}

void my_image::generate_shum(std::vector<std::vector<double>>& shum)
{
	for(int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			shum[i][j] = (double)(rand()) / RAND_MAX * 2 - 1;
		}
	}

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			for (int k = 0; k < 11; k++)
			{
				shum[i][j] += (double)(rand()) / RAND_MAX * 2 - 1; //по центральной предельной теореме
			}
		}
	}
}

void my_image::generate_pic_with_shum()
{
	clear1();
	vector<vector<double>> sh(h, vector<double>(w)); //вектор шумов
	generate_shum(sh); //генерация шума
	double SumSh = energy(sh); //энергия исходного изображения

	double bet = sqrt(alpha * energy(image0) / SumSh); //коэффициент бетта
	image_shum = vector<vector<double>>(h, vector<double>(w)); //выделяем память
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			image_shum[i][j] = image0[i][j] + bet * sh[i][j]; //накладываем шум
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

void my_image::fourea_image(std::vector<std::vector<base>>& fourea, bool invert)
{
	for (int i = 0; i < h; i++)
		fft(fourea[i], invert);

	vector<base> vec_help;
	for (int j = 0; j < w; j++)
	{
		vec_help = get_column(fourea, j);
		fft(vec_help, invert);
		set_column(fourea, vec_help, j);
	}
	need = fourea[0][0];
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
	double max_A = 0, mmm = 0;
	Simmetria(fourea);
	ampl_spec = get_ampl_spec(fourea, max_A);
	double en = energy(ampl_spec), en1;

	vector<vector<base>> new_vec(fourea.size(), vector<base>(fourea[0].size(), base(0, 0)));
	int num = 0; bool flag;
	do
	{
		num++;
		if (2 * num > w || 2 * num > h) break;
		NewSpectr(new_vec, fourea, num);
		en1 = energy(get_ampl_spec(new_vec, mmm));
	} while (en1 < gamma * en);

	fourea = new_vec;

	ampl_spec = get_ampl_spec(fourea, max_A);
	Simmetria(fourea);
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
	vector<vector<double>> ampl_spectr;
	for (int i = 0; i < h; i++)
	{
		ampl_spectr.push_back(vector<double>());
		for (int j = 0; j < w; j++)
		{
			ampl_spectr[i].push_back(AMPL(matr[i][j]));
			if (maximum < ampl_spectr[i][j])
				maximum = ampl_spectr[i][j];
		}
	}
	return ampl_spectr;
}

void my_image::set_alpha(double a)
{
	alpha = a / 100;
}

void my_image::set_gamma(double g)
{
	gamma = g / 100;
}

void my_image::clear()
{
	image_shum.clear();
	image_res.clear();
	image0.clear();
	ampl_spec.clear();
}

void my_image::clear1()
{
	image_res.clear();
	image_shum.clear();
	ampl_spec.clear();
}

void my_image::Process()
{
	generate_pic_with_shum();
	ProcessClearImage();
}

void my_image::Simmetria(vector<vector<base>>& fourea)
{
	int height = fourea.size() / 2, width = fourea[0].size() / 2;
	base my_pixel;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			my_pixel = fourea[i][j];
			fourea[i][j] = fourea[i + height][j + width];
			fourea[i + height][j + width] = my_pixel;

			my_pixel = fourea[i + height][j];
			fourea[i + height][j] = fourea[i][j + width];
			fourea[i][j + width] = my_pixel;
		}
	}
}

void my_image::NewSpectr(vector<vector<base>>& new_vec, vector<vector<base>> fourea, int num)
{
	int height = fourea.size() / 2, width = fourea[0].size() / 2;
	for (int i = 0; i < 2 * num; i++)
	{
		for (int j = 0; j < 2 * num; j++)
			new_vec[height - 1 + num - i][width - 1 + num - j] = fourea[height - 1 + num - i][width - 1 + num - j];
	}
}

void my_image::ProcessClearImage()
{
	image_res.clear();
	vector<vector<base>> b(h, vector<base>(w));
	for (int i = 0; i < h; i++)
		b[i] = vector<base>(image_shum[i].begin(), image_shum[i].end());
	fourea_image(b, true);
	b[0][0] = base(0, 0);
	filter(b);
	b[0][0] = need;
	fourea_image(b, false);
	for (int i = 0; i < b.size(); i++)
	{
		image_res.push_back(vector<double>());
		for (int j = 0; j < b[0].size(); j++)
		{
			image_res[i].push_back(b[i][j].real());
		}
	}
}

vector<vector<double>> my_image::GetImageShum()
{
	//Norma255(image_shum);
	return image_shum;
}

vector<vector<double>> my_image::GetImageRes()
{
	//Norma255(image_res);
	return image_res;
}

vector<vector<double>> my_image::GetAmplSpectr()
{
	//Norma255(ampl_spec);
	return ampl_spec;
}

vector<vector<double>> my_image::GetImageStart()
{
	//Norma255(image0);
	return image0;
}
