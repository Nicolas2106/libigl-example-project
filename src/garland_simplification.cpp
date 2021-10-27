#include "garland_simplification.h"

#include <igl/readOFF.h>
#include <igl/qslim.h>
#include <igl/opengl/glfw/Viewer.h>

GarlandSimplification::GarlandSimplification() {}

void GarlandSimplification::input(std::string fileName)
{
	// Load a mesh in OFF format
	igl::readOFF(OFF_FILES_DIR + fileName, V, F);
}

void GarlandSimplification::startSimplification(int finalNumberOfFaces)
{
	std::cout << "\n=== BEFORE GARLAND SIMPLIFICATION ===\nVERTICES NUM: " << V.rows() << "\nFACES NUM: " << F.rows() << "\n\n";

	Eigen::VectorXi J, I;
	// Mesh simplification with using quadric error metrics [Garland and Heckbert]
	igl::qslim(V, F, finalNumberOfFaces, U, G, J, I);
	
	// Print mesh info in stdout after simplification
	std::cout << "\n=== AFTER GARLAND SIMPLIFICATION ===\nVERTICES NUM: " << U.rows() << "\nFACES NUM: " << G.rows() << "\n\n";
}

void GarlandSimplification::drawResult()
{
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

	// Set the original mesh (not simplified)
	viewer.data().set_mesh(V, F);

	// Set the simplified mesh
	igl::opengl::ViewerData simplified_mesh;
	simplified_mesh.set_mesh(U, G);
	viewer.append_mesh();
	viewer.data_list[1] = simplified_mesh;

	// Display the meshes in the correct views
	viewer.callback_post_draw = [&](igl::opengl::glfw::Viewer&)
	{
		viewer.data(viewer.data_list[0].id).set_visible(false, right_view);
		viewer.data(viewer.data_list[1].id).set_visible(false, left_view);
		return false;
	};

	viewer.launch();
}
