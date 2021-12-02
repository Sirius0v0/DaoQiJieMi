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
	std::cout << "==== 务必切换至半角(英文模式)输入 否则会乱算的！ ====" << std::endl;
	std::cout << "机关个数：" << std::endl;
	std::cin >> this->m_num;
	this->m_init_status.resize(this->m_num);
	this->m_expected_status.resize(this->m_num);
	this->m_jiguan.resize(this->m_num,this->m_num);
	this->m_solution.resize(this->m_num);
	std::cout << "机关的状态数：" << std::endl;
	std::cin >> this->m_status;
	if (this->m_num > 20 || this->m_num < 1 || this->m_status > 10 || this->m_status < 1)
	{
		std::cout << "你这是为难我胖虎，我拒绝！" << std::endl;
		return false;
	}
	std::cout << "===========================" << std::endl;
	std::cout << "将机关及机关状态依次从1开始编号,输入时以空格为分隔符" << std::endl;
	std::cout << "输入机关初始状态向量：" << std::endl;
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
	std::cout << "输入机关末状态向量：" << std::endl;
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
		std::cout << "第" << i + 1 << "个机关联动哪些机关？" << std::endl;
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
		// 矩阵奇异则尝试梯度下降法找局部最优解
		solution = Mod(Mod(this->m_jiguan).colPivHouseholderQr().solve(Mod(this->m_expected_status - this->m_init_status)));
		if (std::isnan(solution(0)))
		{
			std::cout << "===========================" << std::endl;
			std::cout << "该机关无解哦~" << std::endl;
			std::cout << "===========================" << std::endl;
			return 0;
		}
	}
	this->m_solution = solution;
	return true;
}

void JiGuan::ShowSolution() {
	std::cout << "===========================" << std::endl;
	std::cout << "开始解谜：" << std::endl;
	for (int i = 0 ; i < this->m_solution.size() ; i++)
	{
		if (abs(this->m_solution(i)) < 1e-5)
		{
			continue;
		}
		std::cout << "对第" << i+1 << "个机关操作" << (int)(this->m_solution(i)+0.5) << "次" << std::endl;
	}
	std::cout << "解谜完成！" << std::endl;
	std::cout << "===========================" << std::endl;
}

void JiGuan::Run() {
	if (GetArgs())
	{
		if (!Solve())
		{
			std::cout << "===========================" << std::endl;
			std::cout << "该目标无法达成，尝试改变末状态向量" << std::endl;
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