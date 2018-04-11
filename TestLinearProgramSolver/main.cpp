
#include "../math/linear_program.h"
#include "../math/linear_program_solver.h"


typedef LinearProgram<double>		LinearProblem;

void solve_using_all_solvers(const LinearProblem& problem)
{
	LinearProgramSolver solver;
	LinearProgramSolver::SolverName s;

	//////////////////////////////////////////////////////////////////////////

	std::cout << "   - using GUROBI" << std::endl;
	s = LinearProgramSolver::GUROBI;
	if (solver.solve(&problem, s)) {
		const std::vector<double>& result = solver.get_result();
		std::cout << "      Objective: " << solver.get_objective_value() << std::endl;
		std::cout << "      Results:     ";
		for (std::size_t i = 0; i < result.size(); ++i) {
			std::cout << "x" << i + 1 << "=" << result[i] << "; ";
		}
		std::cout << std::endl << std::endl;
	}

	//////////////////////////////////////////////////////////////////////////

	std::cout << "   - using SCIP" << std::endl;
	s = LinearProgramSolver::SCIP;
	if (solver.solve(&problem, s)) {
		const std::vector<double>& result = solver.get_result();
		std::cout << "      Objective: " << solver.get_objective_value() << std::endl;
		std::cout << "      Results:     ";
		for (std::size_t i = 0; i < result.size(); ++i) {
			std::cout << "x" << i + 1 << "=" << result[i] << "; ";
		}
		std::cout << std::endl << std::endl;
	}

	//////////////////////////////////////////////////////////////////////////

	std::cout << "   - using GLPK" << std::endl;
	s = LinearProgramSolver::GLPK;
	if (solver.solve(&problem, s)) {
		const std::vector<double>& result = solver.get_result();
		std::cout << "      Objective: " << solver.get_objective_value() << std::endl;
		std::cout << "      Results:     ";
		for (std::size_t i = 0; i < result.size(); ++i) {
			std::cout << "x" << i + 1 << "=" << result[i] << "; ";
		}
		std::cout << std::endl << std::endl;
	}

	//////////////////////////////////////////////////////////////////////////

	std::cout << "   - using LPSOLVE" << std::endl;
	s = LinearProgramSolver::LPSOLVE;
	if (solver.solve(&problem, s)) {
		const std::vector<double>& result = solver.get_result();
		std::cout << "      Objective: " << solver.get_objective_value() << std::endl;
		std::cout << "      Results:     ";
		for (std::size_t i = 0; i < result.size(); ++i) {
			std::cout << "x" << i + 1 << "=" << result[i] << "; ";
		}
		std::cout << std::endl << std::endl;
	}
}




LinearProblem lp_problem();
LinearProblem mip_problem();

int main()
{
	const LinearProblem& lp = lp_problem();
	solve_using_all_solvers(lp);

	std::cout << "\n" << "\n";

	const LinearProblem& mip = mip_problem();
	solve_using_all_solvers(mip);

	return 0;
}

