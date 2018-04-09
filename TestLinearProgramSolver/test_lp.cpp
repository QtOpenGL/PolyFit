// TestLinearProgramSolver.cpp : Defines the entry point for the console application.
//

#include "../math/linear_program.h"
#include "../math/linear_program_solver.h"


/*
* Solves a linear problem.
*
* The problem to solve is
*	var x1, >=0, <= .5;
*	var x2, >=0, <= .5;
*	var x3, >=0, <= .5;
*
* minimize z : 1.0  - 0.5 * x1 + 0.5 * x2 - 1.0 * x3       ;
* s.t. c1    : 0.0 <= 1.0 * x1 - 0.5 * x2            <= 0.2;
* s.t. c2    :                 - 1.0 * x2 + 1.0 * x3 <= 0.4;

Expected results: x1=0.25; x2=0.1; x3=0.5;
*/

int test_lp()
{
	typedef Bound<double>				Bound;
	typedef Variable<double>			Variable;
	typedef LinearExpression<double>	Objective;
	typedef LinearConstraint<double>	Constraint;
	typedef LinearProgram<double>		LinearProgram;

	LinearProgram program;
	Variable x1(Variable::CONTINUOUS);	x1.set_bound(0, 0.5);	program.add_variable(x1);
	Variable x2(Variable::CONTINUOUS);	x2.set_bound(0, 0.5);	program.add_variable(x2);
	Variable x3(Variable::CONTINUOUS);	x3.set_bound(0, 0.5);	program.add_variable(x3);

	Objective obj;
	obj.add_coefficient(0, -0.5);
	obj.add_coefficient(1,  0.5);
	obj.add_coefficient(2, -1.0);
	program.set_objective(obj);

	Constraint c1;	//  0.0 <= 1.0 * x1 - 0.5 * x2            <= 0.2;
	c1.add_coefficient(0, 1);
	c1.add_coefficient(1, -0.5);
	c1.set_bound(0, 0.2);
	program.add_constraint(c1);

	Constraint c2;	//   -1.0 * x2 + 1.0 * x3 <= 0.4;
	c2.add_coefficient(1, -1);
	c2.add_coefficient(2, 1);
	c2.set_bound(Bound::UPPER, 0.4);
	program.add_constraint(c2);

	std::cout << std::endl << "--------------solving LP problem---------------" << std::endl;
	LinearProgramSolver solver;
	LinearProgramSolver::LP_Solver s;

	//////////////////////////////////////////////////////////////////////////

	std::cout << "- using Gurobi solver" << std::endl;
	s = LinearProgramSolver::GUROBI;
	//LinearProgramSolver::LP_Solver s = LinearProgramSolver::LPSOLVE;
	if (solver.solve(&program, s)) {
		std::cout << "		Expected results: x1=0.25; x2=0.1; x3=0.5;" << std::endl;

		const std::vector<double>& result = solver.get_result();
		std::cout << "		Your results:     ";
		for (std::size_t i=0; i<result.size(); ++i) {
			std::cout << "x" << i + 1 << "=" << result[i] << "; ";
		}
		std::cout << std::endl;
	}

	//////////////////////////////////////////////////////////////////////////

	std::cout << "- using LP_SOLVE solver" << std::endl;
	s = LinearProgramSolver::LPSOLVE;
	if (solver.solve(&program, s)) {
		std::cout << "		Expected results: x1=0.25; x2=0.1; x3=0.5;" << std::endl;

		const std::vector<double>& result = solver.get_result();
		std::cout << "		Your results:     ";
		for (std::size_t i = 0; i < result.size(); ++i) {
			std::cout << "x" << i + 1 << "=" << result[i] << "; ";
		}
		std::cout << std::endl;
	}

	std::cout << "- using GLPK solver" << std::endl;
	s = LinearProgramSolver::GLPK;
	if (solver.solve(&program, s)) {
		std::cout << "		Expected results: x1=0.25; x2=0.1; x3=0.5;" << std::endl;

		const std::vector<double>& result = solver.get_result();
		std::cout << "		Your results:     ";
		for (std::size_t i = 0; i < result.size(); ++i) {
			std::cout << "x" << i + 1 << "=" << result[i] << "; ";
		}
		std::cout << std::endl;
	}

	return 0;
}

