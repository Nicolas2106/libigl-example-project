#ifndef GARLAND_SIMPLIFICATION_H
#define GARLAND_SIMPLIFICATION_H

#include <string>
#include <Eigen/Dense>

class GarlandSimplification
{
public:
	GarlandSimplification();

	void input(std::string fileName);
	void startSimplification(int finalNumberOfFaces);
	void drawResult();

private:
	const std::string OFF_FILES_DIR = "../data/";

	Eigen::MatrixXd V, U;
	Eigen::MatrixXi F, G;
};

#endif // !GARLAND_SIMPLIFICATION_H
