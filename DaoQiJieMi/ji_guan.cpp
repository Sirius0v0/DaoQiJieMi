#include "ji_guan.h"
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Core>
#include <cmath>

JiGuan::JiGuan() {
	this->m_jiguan = Eigen::MatrixXd::Identity(1,1);
}

JiGuan::~JiGuan() {
}

bool JiGuan::GetArgs() {
	std::vector<double> buff;
	int nRead = 0,i = 0;
	std::cout << "==== ����л������(Ӣ��ģʽ)���� ���������ģ� ====" << std::endl;
	std::cout << "���ظ�����" << std::endl;
	std::cin >> this->m_num;
	this->m_init_status.resize(this->m_num);
	this->m_expected_status.resize(this->m_num);
	this->m_jiguan.resize(this->m_num,this->m_num);
	this->m_solution.resize(this->m_num);
	std::cout << "���ص�״̬����" << std::endl;
	std::cin >> this->m_status;
	if (this->m_num > 20 || this->m_num < 1 || this->m_status > 10 || this->m_status < 1)
	{
		std::cout << "������Ϊ�����ֻ����Ҿܾ���" << std::endl;
		return false;
	}
	std::cout << "===========================" << std::endl;
	std::cout << "�����ؼ�����״̬���δ�1��ʼ���,����ʱ�Կո�Ϊ�ָ���" << std::endl;
	std::cout << "������س�ʼ״̬������" << std::endl;
	std::cin.get();
	while (std::cin >> nRead)
	{
		buff.push_back(nRead);
		if (std::cin.get() == '\n')
		{
			break;
		}		
	}
	this->m_init_status = Eigen::VectorXd::Map(&buff[0], this->m_num);
	std::cout << "�������ĩ״̬������" << std::endl;
	while (std::cin >> nRead)
	{
		buff.push_back(nRead);
		if (std::cin.get() == '\n')
		{
			break;
		}
	}
	this->m_expected_status = Eigen::VectorXd::Map(&buff[0 + this->m_num], this->m_num);
	this->m_jiguan = Eigen::MatrixXd::Zero(this->m_num, this->m_num);

	std::vector<int> unit;
	for (int i = 0; i < this->m_num; i++)
	{
		int ind = 0;
		std::cout << "��" << i + 1 << "������������Щ���أ�" << std::endl;
		while (std::cin >> nRead)
		{
			unit.push_back(nRead);
			if (std::cin.get() == '\n')
			{
				break;
			}
		}

		for (int j = 0; j < unit.size(); j++)
		{
			if (abs(unit[i])<1e-7)
			{
				continue;
			}
			this->m_jiguan(unit[j] - 1, i) = 1;
		}
		unit.clear();
	}
	return true;
}


bool JiGuan::Solve() {
	Eigen::VectorXd solution = Mod((Mod(this->m_jiguan).inverse()) * Mod(this->m_expected_status - this->m_init_status));
	if (abs((int)solution(0)-solution(0))>1e-5)
	{
		return false;
	}
	if (std::isnan(solution(0)))
	{
		// �������������ݶ��½����Ҿֲ����Ž�
		solution = Mod(Mod(this->m_jiguan).colPivHouseholderQr().solve(Mod(this->m_expected_status - this->m_init_status)));
		if (std::isnan(solution(0)))
		{
			std::cout << "===========================" << std::endl;
			std::cout << "�û����޽�Ŷ~" << std::endl;
			std::cout << "===========================" << std::endl;
			return 0;
		}
	}
	this->m_solution = solution;
	return true;
}

void JiGuan::ShowSolution() {
	std::cout << "===========================" << std::endl;
	std::cout << "��ʼ���գ�" << std::endl;
	for (int i = 0 ; i < this->m_solution.size() ; i++)
	{
		if (abs(this->m_solution(i)) < 1e-5)
		{
			continue;
		}
		std::cout << "�Ե�" << i+1 << "�����ز���" << (int)(this->m_solution(i)+0.5) << "��" << std::endl;
	}
	std::cout << "������ɣ�" << std::endl;
	std::cout << "===========================" << std::endl;
}

void JiGuan::Run() {
	if (GetArgs())
	{
		if (!Solve())
		{
			std::cout << "===========================" << std::endl;
			std::cout << "��Ŀ���޷���ɣ����Ըı�ĩ״̬����" << std::endl;
			std::cout << "===========================" << std::endl;
			return;
		}
		ShowSolution();
	}
}

Eigen::MatrixXd JiGuan::Mod(Eigen::MatrixXd eMatrix) {
	for (int i = 0; i < eMatrix.rows(); i++)
	{
		for (int j = 0; j < eMatrix.cols(); j++)
		{
			if (std::isnan(eMatrix(i,j)) || abs((int)eMatrix(i,j)-eMatrix(i,j))>1e-6 )
			{
				continue;
			}
			eMatrix(i, j) = (int)((int)eMatrix(i, j) % this->m_status + this->m_status) % this->m_status;
		}
	}
	return eMatrix;
}