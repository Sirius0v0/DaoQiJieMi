#pragma once
#include <Eigen/Dense>

class JiGuan
{
public:
	JiGuan();
	~JiGuan();

private:
	int m_num;
	int m_status;
	Eigen::VectorXd m_init_status;
	Eigen::VectorXd m_expected_status;
	Eigen::MatrixXd m_jiguan;
	Eigen::VectorXd m_solution;

	void ShowSolution();
	bool Solve();
	bool GetArgs();
	Eigen::MatrixXd Mod(Eigen::MatrixXd eMatrix);

public:
	void Run();

};

