#ifndef GARLAND_SIMPLIFICATION_H
#define GARLAND_SIMPLIFICATION_H

#include <string>
#include <Eigen/Dense>

class GarlandSimplification
{
public:
	GarlandSimplification();

	void inputMesh(std::string meshName);
	bool startSimplification(int finalNumberOfFaces);
	void drawResult();

private:
	const std::string OFF_FILES_DIR = "../data/";

	Eigen::MatrixXd V, U; // V -> original vertices, U -> final vertices
	Eigen::MatrixXi F, G; // F -> original faces, G -> final faces
	Eigen::VectorXi I, J; // I -> birth vertices, J -> birth faces

	bool myQSlim(Eigen::MatrixXd & V, Eigen::MatrixXi & F, int finalNumberOfFaces,
		Eigen::MatrixXd & U, Eigen::MatrixXi & G);
};

#endif // !GARLAND_SIMPLIFICATION_H
