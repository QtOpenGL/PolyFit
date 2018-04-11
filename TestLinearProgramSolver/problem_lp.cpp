#include "../math/linear_program.h"
#include "../math/linear_program_solver.h"


typedef LinearProgram<double>		LinearProblem;

/*
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

LinearProblem lp_problem()
{
	typedef Bound<double>				Bound;
	typedef Variable<double>			Variable;
	typedef LinearExpression<double>	Objective;
	typedef LinearConstraint<double>	Constraint;

	LinearProblem problem;
	Variable x1(Variable::CONTINUOUS);	x1.set_bounds(Variable::DOUBLE, 0.0, 0.5);	problem.add_variable(x1);
	Variable x2(Variable::CONTINUOUS);	x2.set_bounds(Variable::DOUBLE, 0.0, 0.5);	problem.add_variable(x2);
	Variable x3(Variable::CONTINUOUS);	x3.set_bounds(Variable::DOUBLE, 0.0, 0.5);	problem.add_variable(x3);

	Objective obj;
	obj.add_coefficient(0, -0.5);
	obj.add_coefficient(1,  0.5);
	obj.add_coefficient(2, -1.0);
	problem.set_objective(obj);

	Constraint c1;	//  0.0 <= 1.0 * x1 - 0.5 * x2            <= 0.2;
	c1.add_coefficient(0, 1);
	c1.add_coefficient(1, -0.5);
	c1.set_bounds(Variable::DOUBLE, 0.0, 0.2);
	problem.add_constraint(c1);

	Constraint c2;	//   -1.0 * x2 + 1.0 * x3 <= 0.4;
	c2.add_coefficient(1, -1);
	c2.add_coefficient(2, 1);
	c2.set_bounds(Bound::UPPER, 0.0, 0.4);
	problem.add_constraint(c2);


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
	std::cout << "***    Expected results: x1=0.25; x2=0.1; x3=0.5;" << std::endl;
	std::cout << "**************************************************************" << std::endl;

	return problem;
}

