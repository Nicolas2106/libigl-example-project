#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/decimate.h>

int main(int argc, char* argv[])
{
	const std::string OFF_FILES_DIR = "../data/";

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;

	// Load a closed manifold mesh in OFF format
	igl::readOFF(OFF_FILES_DIR + "bunny.off", V, F);

	// Init the viewer
	igl::opengl::glfw::Viewer viewer;
	
	// Prepare array-based edge data structures and priority queue
	Eigen::VectorXi EMAP, EQ;
	Eigen::MatrixXi E, EF, EI;
	igl::min_heap<std::tuple<double, int, int>> Q;
	// If an edge were collapsed, we'd collapse it to these points:
	Eigen::MatrixXd C;

    igl::edge_flaps(F, E, EMAP, EF, EI);
    C.resize(E.rows(), V.cols());
    Eigen::VectorXd costs(E.rows());
    Q = {};
    EQ = Eigen::VectorXi::Zero(E.rows());
    {
        Eigen::VectorXd costs(E.rows());
        igl::parallel_for(E.rows(), [&](const int e)
            {
                double cost = e;
                Eigen::RowVectorXd p(1, 3);
                igl::shortest_edge_and_midpoint(e, V, F, E, EMAP, EF, EI, cost, p);
                C.row(e) = p;
                costs(e) = cost;
            }, 10000);
        for (int e = 0; e < E.rows(); e++)
        {
            Q.emplace(costs(e), e, 0);
        }
    }

    viewer.data().clear();
    viewer.data().set_mesh(V, F);
    viewer.data().set_face_based(true);

    const auto& pre_draw = [&](igl::opengl::glfw::Viewer& viewer)->bool
    {
        // If animating then collapse 10% of edges
        if (viewer.core().is_animating && !Q.empty())
        {
            bool something_collapsed = false;
            // collapse edge
            const int max_iter = std::ceil(0.01 * Q.size());
            for (int j = 0; j < max_iter; j++)
            {
                if (!igl::collapse_edge(igl::shortest_edge_and_midpoint, V, F, E, EMAP, EF, EI, Q, EQ, C))
                {
                    break;
                }
                something_collapsed = true;
            }

            if (something_collapsed)
            {
                viewer.data().clear();
                viewer.data().set_mesh(V, F);
                viewer.data().set_face_based(true);
            }
        }
        return false;
    };

    viewer.core().is_animating = true;
    viewer.callback_pre_draw = pre_draw;
    return viewer.launch();
}
