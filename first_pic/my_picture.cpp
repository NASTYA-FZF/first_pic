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

my_image::my_image(vector<gauss> _gauss, int _w, int _h, double a, double g, bool val)
{
	clear();
	interpolation = val;
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

	newW = w;
	newH = h;

	if (interpolation)
	{
		GetWHnew();
		InterpolationMatr(image0, image0, newW, newH);
	}
}

my_image::my_image(vector<vector<double>> matr, double a, double g, bool val)
{
	clear();
	interpolation = val;
	image0 = matr;
	h = image0.size();
	w = image0[0].size();
	alpha = a / 100;
	gamma = g / 100;

	newW = w;
	newH = h;

	if (interpolation)
	{
		GetWHnew();
		InterpolationMatr(image0, image0, newW, newH);
	}
}

void my_image::generate_shum(std::vector<std::vector<double>>& shum)
{
	for(int i = 0; i < newH; i++)
	{
		for (int j = 0; j < newW; j++)
		{
			shum[i][j] = (double)(rand()) / RAND_MAX * 2 - 1;
		}
	}

	for (int i = 0; i < newH; i++)
	{
		for (int j = 0; j < newW; j++)
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
	vector<vector<double>> sh(newH, vector<double>(newW)); //вектор шумов
	generate_shum(sh); //генерация шума
	double SumSh = energy(sh); //энергия исходного изображения

	double bet = sqrt(alpha * energy(image0) / SumSh); //коэффициент бетта
	image_shum = vector<vector<double>>(newH, vector<double>(newW)); //выделяем память
	for (int i = 0; i < newH; i++)
	{
		for (int j = 0; j < newW; j++)
		{
			image_shum[i][j] = image0[i][j] + bet * sh[i][j]; //накладываем шум
		}
	}
}

double my_image::energy(std::vector<std::vector<double>> image)
{
	double res = 0;
	double curH = image.size();
	double curW = image[0].size();
	for (int i = 0; i < curH; i++)
	{
		for (int j = 0; j < curW; j++)
		{
			res += image[i][j] * image[i][j]; //энергия как сумма квадратов отсчетов
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
	for (int i = 0; i < newH; i++)
		fft(fourea[i], invert);

	vector<base> vec_help(newH);
	for (int j = 0; j < newW; j++)
	{
		get_column(vec_help, fourea, j);
		fft(vec_help, invert);
		set_column(fourea, vec_help, j);
	}
	need = fourea[0][0];
}

void my_image::get_column(std::vector<base>& res, std::vector<std::vector<base>> matr, int num)
{
	for (int row = 0; row < newH; row++)
	{
		res[row] = matr[row][num];
	}
}

void my_image::set_column(std::vector<std::vector<base>>& matr, std::vector<base> vec, int num)
{
	for (int row = 0; row < newH; row++)
	{
		matr[row][num] = vec[row];
	}
}

void my_image::filter(std::vector<std::vector<base>>& fourea)
{
	double max_A = 0, mmm = 0;
	Simmetria(fourea);
	ampl_spec = vector<vector<double>>(newH, vector<double>(newW));
	get_ampl_spec(fourea);
	double en = energy(ampl_spec);
	double en1;

	vector<vector<base>> new_vec(fourea.size(), vector<base>(fourea[0].size(), base(0, 0)));
	int num = 0;
	do
	{
		num++;
		if (2 * num >= newW || 2 * num >= newH) break;
		NewSpectr(new_vec, fourea, num, en1);
	} while (en1 < gamma * en); //цикл много времени кушает

	lx = fourea[0].size() / 2 - num;
	ly = fourea.size() / 2 - num;
	wid = 2 * num;
	fourea = new_vec;

	get_ampl_spec(fourea);
	Simmetria(fourea);
}

double my_image::find_error(std::vector<std::vector<double>> oneImage, std::vector<std::vector<double>> twoImage)
{
	if (oneImage.size() != twoImage.size() || oneImage[0].size() != twoImage[0].size())
		return -1;

	double znamen = energy(oneImage);
	double chislit = 0;

	for (int i = 0; i < oneImage.size(); i++)
	{
		for (int j = 0; j < oneImage[0].size(); j++)
		{
			chislit += P2(oneImage[i][j] - twoImage[i][j]);
		}
	}
	double res = chislit / znamen * 100;
	return res;
}

double my_image::find_PSNR(std::vector<std::vector<double>> oneImage, std::vector<std::vector<double>> twoImage)
{
	if (oneImage.size() != twoImage.size() || oneImage[0].size() != twoImage[0].size())
		return -1;

	double znamen = 0;
	double chislit = 0;
	int N = oneImage.size() * oneImage[0].size();

	for (int i = 0; i < oneImage.size(); i++)
	{
		for (int j = 0; j < oneImage[0].size(); j++)
		{
			if (chislit < oneImage[i][j])
				chislit = oneImage[i][j];
		}
	}

	chislit = P2(chislit);

	for (int i = 0; i < oneImage.size(); i++)
	{
		for (int j = 0; j < oneImage[0].size(); j++)
		{
			znamen += P2(oneImage[i][j] - twoImage[i][j]) / N;
		}
	}
	double res = 10 * log10(chislit / znamen);
	return res;
}

void my_image::get_ampl_spec(std::vector<std::vector<base>> matr)
{
	for (int i = 0; i < newH; i++)
	{
		for (int j = 0; j < newW; j++)
		{
			ampl_spec[i][j] = AMPL(matr[i][j]);
		}
	}
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

	if (!interpolation)
	{
		GetWHnew();
		AddNull(newW, newH);
	}

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

void my_image::NewSpectr(vector<vector<base>>& new_vec, vector<vector<base>> fourea, int num, double& energy)
{
	int height = fourea.size() / 2, width = fourea[0].size() / 2;
	energy = 0;
	for (int i = 0; i < 2 * num; i++)
	{
		for (int j = 0; j < 2 * num; j++)
		{
			new_vec[height - 1 + num - i][width - 1 + num - j] = fourea[height - 1 + num - i][width - 1 + num - j];
			energy += P2(AMPL(new_vec[height - 1 + num - i][width - 1 + num - j]));
		}
	}
}

void my_image::ProcessClearImage()
{
	image_res.clear();
	vector<vector<base>> b(newH, vector<base>(newW));
	for (int i = 0; i < newH; i++)
		b[i] = vector<base>(image_shum[i].begin(), image_shum[i].end());
	fourea_image(b, true);
	b[0][0] = base(0, 0);
	filter(b);
	b[0][0] = need;
	fourea_image(b, false);
	image_res = vector<vector<double>>(newH, vector<double>(newW));
	for (int i = 0; i < b.size(); i++)
	{
		for (int j = 0; j < b[0].size(); j++)
		{
			image_res[i][j] = b[i][j].real();
		}
	}
}

void my_image::AddNull(int w_new, int h_new)
{
	image_shum.resize(h_new);
	for (int i = 0; i < h_new; i++)
	{
		image_shum[i].resize(w_new, 0);
	}
}

void my_image::DeleteNull(std::vector<std::vector<double>>& matr)
{
	matr.resize(h);

	for (int i = 0; i < h; i++)
	{
		matr[i].resize(w);
	}
}

void my_image::GetWHnew()
{
	int iter = 2;
	bool readyW = false;
	bool readyH = false;
	newW = 0;
	newH = 0;

	while (!readyW || !readyH)
	{
		if (w <= iter && !readyW)
		{
			newW = iter;
			readyW = true;
		}
		if (h <= iter && !readyH)
		{
			newH = iter;
			readyH = true;
		}
		iter *= 2;
	}
}

void my_image::SetInterOrNull(bool value)
{
	interpolation = value;
}

vector<vector<double>> my_image::GetImageShum()
{
	auto res = image_shum;
	if (!interpolation)
		DeleteNull(res);
	return res;
}

vector<vector<double>> my_image::GetImageRes()
{
	auto res = image_res;
	if (!interpolation)
		DeleteNull(res);
	return res;
}

vector<vector<double>> my_image::GetAmplSpectr()
{
	return ampl_spec;
}

vector<vector<double>> my_image::GetImageStart()
{
	auto res = image0;
	if (!interpolation)
		DeleteNull(res);
	return res;
}
