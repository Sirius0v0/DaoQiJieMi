#include <Eigen/Dense>
#include <iostream>
#include <cmath>
#include <vector>
#include "ji_guan.h"

int main()
{
	JiGuan jiguan;
	jiguan.Run();
	/*Eigen::MatrixXd m(5,5);
	m << 0, 1, 0, 0, 0,
		 0, 1, 0, 0, 0,
		 0, 1, 0, 1, 0,
		 0, 0, 0, 1, 0,
		 0, 0, 0, 1, 0;
	Eigen::VectorXd v(5),x(5);
	v << 1, 1, 1, 0, 0;
	x = m.colPivHouseholderQr().solve(v);*/
	std::cout << "按任意键退出" << std::endl;
	std::cin.get();
	return 0;
}