#include <iostream>
#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/ViewerData.h>
#include <igl/decimate.h>

int main(int argc, char* argv[])
{
	const std::string OFF_FILES_DIR = "../data/";
	const float DECIMATION_RATE = 0.2;
	
	Eigen::MatrixXd V, dV;
	Eigen::MatrixXi F, dF;

	// Load a mesh in OFF format
	igl::readOFF(OFF_FILES_DIR + "bunny.off", V, F);

	// Print mesh info in stdout before decimation
	std::cout << "\n======== BEFORE DECIMATION ========\nVERTICES: " << V.rows() << "\nFACES: " << F.rows() << "\n\n";

	// Init Viewer with two views
	igl::opengl::glfw::Viewer viewer;
	unsigned int left_view = 0, right_view = 1;
	viewer.callback_init = [&](igl::opengl::glfw::Viewer&)
	{
		viewer.core().viewport = Eigen::Vector4f(0, 0, 640, 800);
		left_view = viewer.core_list[0].id;
		right_view = viewer.append_core(Eigen::Vector4f(640, 0, 640, 800));
		return false;
	};

	// Set the original mesh (not decimated)
	viewer.data().set_mesh(V, F);

	// Decimate according the DECIMATION_RATE
	Eigen::VectorXi J;
	igl::decimate(V, F, DECIMATION_RATE * (double)F.rows(), dV, dF, J);

	// Print mesh info in stdout after decimation
	std::cout << "\n======== AFTER DECIMATION ========\nVERTICES: " << dV.rows() << "\nFACES: " << dF.rows() << "\n\n";

	// Set the decimated mesh
	igl::opengl::ViewerData decimated_mesh;
	decimated_mesh.set_mesh(dV, dF);
	viewer.append_mesh();
	viewer.data_list[1] = decimated_mesh;

	// Display the meshes in the correct views
	viewer.callback_post_draw = [&](igl::opengl::glfw::Viewer&)
	{
		viewer.data(viewer.data_list[0].id).set_visible(false, right_view);
		viewer.data(viewer.data_list[1].id).set_visible(false, left_view);
		return false;
	};

	viewer.launch();
}
