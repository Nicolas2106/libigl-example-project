#include "garland_simplification.h"

#include <igl/readOFF.h>
#include <igl/is_edge_manifold.h>
#include <igl/edge_flaps.h>
#include <igl/per_vertex_point_to_plane_quadrics.h>
#include <igl/decimate_callback_types.h>
#include <igl/qslim_optimal_collapse_edge_callbacks.h>
#include <igl/decimate.h>
#include <igl/opengl/glfw/Viewer.h>
#include <Eigen/Core>

GarlandSimplification::GarlandSimplification() {}

void GarlandSimplification::inputMesh(std::string meshName)
{
	igl::readOFF(OFF_FILES_DIR + meshName, V, F); // Load a mesh in OFF format
}

bool GarlandSimplification::startSimplification(int finalNumberOfFaces)
{
	std::cout << "\n=== BEFORE GARLAND SIMPLIFICATION ===\nVERTICES NUM: " << V.rows() << 
		"\nFACES NUM: " << F.rows() << "\n\n";

	// Mesh simplification with using quadric error metrics [Garland and Heckbert]
	if (myQSlim(V, F, finalNumberOfFaces, U, G))
	{
		// Print mesh info in stdout after simplification
		std::cout << "\n=== AFTER GARLAND SIMPLIFICATION ===\nVERTICES NUM: " << U.rows() << 
			"\nFACES NUM: " << G.rows() << "\n\n";
		return true;
	}
	
	std::cout << "\nERROR OCCURED DURING SIMPLIFICATION\n\n";
	return false;
}

bool GarlandSimplification::myQSlim(Eigen::MatrixXd & V, Eigen::MatrixXi & F, int finalNumberOfFaces,
	Eigen::MatrixXd & U, Eigen::MatrixXi & G)
{
	using namespace igl;
	
	if (!is_edge_manifold(F))
	{
		return false;
	}

	Eigen::VectorXi EMAP;
	Eigen::MatrixXi uE, EF, EI;
	edge_flaps(F, uE, EMAP, EF, EI);

	// Compute the Q matrices (quadrics) for all the initial vertices (QSlim in nD)
	typedef std::tuple<Eigen::MatrixXd, Eigen::RowVectorXd, double> Quadric;
	std::vector<Quadric> quadrics;
	per_vertex_point_to_plane_quadrics(V, F, EMAP, EF, EI, quadrics);

	// Prepare the simplification using callbacks
	int v1 = -1, v2 = -1;
	decimate_cost_and_placement_callback cost_and_placement;
	decimate_pre_collapse_callback       pre_collapse;
	decimate_post_collapse_callback      post_collapse;
	qslim_optimal_collapse_edge_callbacks(uE, quadrics, v1, v2, cost_and_placement, pre_collapse, post_collapse);
	
	// Simplification
	int m = F.rows();
	if (!decimate(V, F, cost_and_placement,
		max_faces_stopping_condition(m, (int)F.rows(), finalNumberOfFaces),
		pre_collapse, post_collapse, uE, EMAP, EF, EI, U, G, J, I))
	{
		return false;
	}

	// Remove unreferenced vertices from V, updating F accordingly
	Eigen::VectorXi _I, _J;
	igl::remove_unreferenced(Eigen::MatrixXd(U), Eigen::MatrixXi(G), U, G, _I, _J);

	return true;
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
