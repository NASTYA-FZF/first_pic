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
	//��� ��������� ������ �������� ����
	base need;

public:
	//������������
	my_image() {}
	my_image(vector<gauss> many_gauss, int _w, int _h, double a, double g); //���� ������� �����
	my_image(vector<vector<double>> matr, double a, double g); //���� ����������� (���� ����� ��������� ������������� �� ������� 2)
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
	double find_error();
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

	//��������� ����������� �� ������
	vector<vector<double>> GetImageStart();
	vector<vector<double>> GetImageShum();
	vector<vector<double>> GetImageRes();
	vector<vector<double>> GetAmplSpectr();
};