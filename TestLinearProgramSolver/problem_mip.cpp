// TestLinearProgramSolver.cpp : Defines the entry point for the console application.
//

#include "../math/linear_program.h"
#include "../math/linear_program_solver.h"

typedef LinearProgram<double>		LinearProblem;

/*
Maximize
obj: x1 + 2 x2 + 3 x3 + x4
Subject To
	c1: - x1 +   x2 + x3 + 10 x4 <= 20
	c2:   x1 - 3 x2 + x3         <= 30
	c3:   x2            - 3.5 x4 = 0
Bounds
	0 <= x1 <= 40
	2 <= x4 <= 3
General
	x4 is integer

	Expected results: x0=40; x1=10.5; x2=19.5; x3=3
*/

LinearProblem mip_problem()
{
	typedef Bound<double>				Bound;
	typedef Variable<double>			Variable;
	typedef LinearExpression<double>	Objective;
	typedef LinearConstraint<double>	Constraint;

	LinearProblem problem;
	Variable x1(Variable::CONTINUOUS);	x1.set_bounds(Variable::DOUBLE, 0, 40);	problem.add_variable(x1);
	Variable x2(Variable::CONTINUOUS);											problem.add_variable(x2);
	Variable x3(Variable::CONTINUOUS);											problem.add_variable(x3);
	Variable x4(Variable::INTEGER);		x4.set_bounds(Variable::DOUBLE, 2, 3);	problem.add_variable(x4);

	double coeff = -1; // for maximize
	Objective obj;
	obj.add_coefficient(0, coeff * 1.0);
	obj.add_coefficient(1, coeff * 2.0);
	obj.add_coefficient(2, coeff * 3.0);
	obj.add_coefficient(3, coeff * 1.0);
	problem.set_objective(obj);

	Constraint c1;	//  - x1 +   x2 + x3 + 10 x4 <= 20
	c1.add_coefficient(0, -1);
	c1.add_coefficient(1, 1);
	c1.add_coefficient(2, 1);
	c1.add_coefficient(3, 10);
	c1.set_bounds(Bound::UPPER, 0, 20);
	problem.add_constraint(c1);

	Constraint c2;	//   x1 - 3 x2 + x3         <= 30
	c2.add_coefficient(0, 1);
	c2.add_coefficient(1, -3);
	c2.add_coefficient(2, 1);
	c2.set_bounds(Bound::UPPER, 0, 30);
	problem.add_constraint(c2);

	Constraint c3;	//      x2            - 3.5 x4 = 0
	c3.add_coefficient(1, 1);
	c3.add_coefficient(3, -3.5);
	c3.set_bounds(Bound::FIXED, 0, 0);
	problem.add_constraint(c3);

	std::cout << "**************************************************************" << std::endl;
	if (problem.is_mix_integer_program())
		std::cout << "***  Mixed Integer Program" << std::endl;
	else if (problem.is_integer_program())
		std::cout << "***  Integer Program" << std::endl;
	else if (problem.is_binary_proram())
		std::cout << "***  Binary Program" << std::endl;
	else
		std::cout << "***  Continuous Program" << std::endl;
	std::cout << "***    # variables: " << problem.num_continuous_variables() + problem.num_integer_variables() + problem.num_binary_variables() << std::endl;
	std::cout << "***      - Continuous variables: " << problem.num_continuous_variables() << std::endl;
	std::cout << "***      - Integer variables: " << problem.num_integer_variables() << std::endl;
	std::cout << "***      - Binary variables: " << problem.num_binary_variables() << std::endl;
	std::cout << "***  Expected results: x1=40; x2=10.5; x3=19.5; x4=3;" << std::endl;
	std::cout << "**************************************************************" << std::endl;

	return problem;
}

