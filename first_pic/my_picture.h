#pragma once
#include<utility>
#include <vector>
#include <complex>
using namespace std;

typedef pair<int, int> my_point_int;
typedef complex<double> base;

//������� �����
struct gauss
{
	//��������� ������
	double A;
	//�������� ������
	my_point_int p0;
	//������ ������
	double sigma;

	//������������
	gauss(double _A, int _x0, int _y0, double _s);
	gauss() { }
};

//����� ��� ������ � ������������
class my_image
{
	//��������, �����������, ��������������� ����������� � ��� ����. ������
	vector<vector<double>> image0, image_shum, image_res, ampl_spec;
	//��������� ���������� � ��������
	double alpha, gamma;
	//������ � ������ �����������
	int w, h;
	int newW, newH;
	//��� ��������� ������ �������� ����
	base need;
	bool interpolation;

public:
	//������������
	my_image() {}
	my_image(vector<gauss> many_gauss, int _w, int _h, double a, double g, bool val); //���� ������� �����
	my_image(vector<vector<double>> matr, double a, double g, bool val); //���� ����������� (���� ����� ��������� ������������� �� ������� 2)
	//������� ��������� ����
	void generate_shum(std::vector<std::vector<double>>& shum);
	//�������, ������� ����������� ���
	void generate_pic_with_shum();
	//������� ������������� ������� �����������
	double energy(std::vector<std::vector<double>> image);
	//���
	void fft(vector<base>& a, bool invert);
	//�������������� ����� �� ������� � ��������
	void fourea_image(std::vector<std::vector<base>>& vec, bool invert);
	//��������� ������� ������� (�� ������ �������, ����� �� ���� �����������)
	void get_column(std::vector<base>& res, std::vector<std::vector<base>> matr, int num);
	//������������ �������
	void set_column(std::vector<std::vector<base>>& matr, std::vector<base> vec, int num);
	//���������� (��������������)
	void filter(std::vector<std::vector<base>>& fourea);
	//����� ������ (������� ����� 2 ���������)
	double find_error(std::vector<std::vector<double>> oneImage, std::vector<std::vector<double>> twoImage);
	double find_PSNR(std::vector<std::vector<double>> oneImage, std::vector<std::vector<double>> twoImage);
	//��������� ����. ������� (��� �����������)
	void get_ampl_spec(std::vector<std::vector<base>> matr);
	//��������� ��������� ����������
	void set_alpha(double a);
	//��������� ��������� ����������
	void set_gamma(double g);
	//������� �������� (�� ��������� ������)
	void clear();
	void clear1();
	//��� ������������������ ��������
	void Process();
	//��������� ����. �������
	void Simmetria(vector<vector<base>>& fourea);
	//��������� ������� �� ��������
	void NewSpectr(vector<vector<base>>& new_vec, vector<vector<base>> fourea, int num, double& energy);
	//�������� �����������
	void ProcessClearImage();

	void AddNull(int w_new, int h_new);
	void DeleteNull(std::vector<std::vector<double>>& matr);
	void GetWHnew();
	void SetInterOrNull(bool value);
	void Norma(std::vector<std::vector<double>>& matr);

	//��������� ����������� �� ������
	vector<vector<double>> GetImageStart();
	vector<vector<double>> GetImageShum();
	vector<vector<double>> GetImageRes();
	vector<vector<double>> GetAmplSpectr();

	double lx;
	double ly;
	double wid;
};

void InterpolationMatr(std::vector<std::vector<double>>& result, std::vector<std::vector<double>> _image, double w, double h);