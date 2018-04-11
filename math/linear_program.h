#pragma once

#include "math_common.h"

#include <vector>
#include <cassert>
#include <unordered_map>
#include <iostream>



template <class FT>
class Bound
{
public:
	// bound type for variables and expression
enum BoundType { FIXED, LOWER, UPPER, DOUBLE, FREE };

public:
	Bound()
		: bound_type_(FREE)
		, lower_bound_(-DBL_MAX)
		, upper_bound_(DBL_MAX)
	{
	}

	BoundType bound_type() const { return bound_type_; }

	void set_bounds(BoundType type, FT lb, FT ub) {
		if (bound_type_ != FREE) {
			// Easier life: print a message if you want to change bound(s)
			std::cerr << "Warning: are you sure you want to changed the bound(s)" << std::endl;
		}

		switch (type)
		{
		case FIXED:
			if (std::abs(lb - ub) > 1e-10)
				std::cerr << "lower/upper bounds must be equal for FIXED bounds" << std::endl;
			lower_bound_ = upper_bound_ = lb;	
			break;
		case LOWER:		lower_bound_ = lb;	break;
		case UPPER:		upper_bound_ = ub;	break;
		case DOUBLE:	
			lower_bound_ = lb; 
			upper_bound_ = ub; 
			break;
		case FREE:
		default:
			std::cerr << "no FREE bound(s)" << std::endl;
			break;
		}
		bound_type_ = type;
	}

	// query the single bound according to its bound type
	FT   get_single_bound() const { 
		switch (bound_type_)
		{
		case FIXED:
		case LOWER:
			return lower_bound_;
		case UPPER:
			return upper_bound_;
		case DOUBLE:
			std::cerr << "please use get_double_bounds() to get double bound(s)" << std::endl;
			return lower_bound_;
		case FREE:
		default:
			std::cerr << "no bound specified" << std::endl;
			return lower_bound_;
		}
	}

	void get_double_bounds(FT& lb, FT& ub) const { lb = lower_bound_; ub = upper_bound_; }

private:
	BoundType	bound_type_;
	FT			lower_bound_;
	FT			upper_bound_;	
};


template <class FT>
class Variable : public Bound<FT> {
public:
enum VariableType { CONTINUOUS, INTEGER, BINARY };

public:
	Variable(VariableType type) : variable_type_(type) {
		if (type == BINARY)
			Bound<FT>::set_bounds(Bound<FT>::DOUBLE, 0.0, 1.0);
	}

	VariableType variable_type() const { return variable_type_; }

private:
	VariableType variable_type_;
};


template <class FT>
class LinearExpression {
public:
	LinearExpression() {}

	void add_coefficient(std::size_t var_index, FT coeff) {
		if (coefficients_.find(var_index) == coefficients_.end())
			coefficients_[var_index] = coeff;
		else 
			coefficients_[var_index] += coeff;
	}

	const std::unordered_map<std::size_t, FT>& coefficients() const {
		return coefficients_;
	}

private:
	std::unordered_map<std::size_t, FT>	coefficients_;
};


template <class FT>
class LinearConstraint : public LinearExpression<FT>, public Bound<FT> 
{
public:
	LinearConstraint() {}
};


template <class FT>
class LinearProgram
{
public:
	typedef Variable<FT>			Variable;
	typedef LinearExpression<FT>	Objective;
	typedef LinearConstraint<FT>	Constraint;

	enum Solver { GUROBI, SCIP, LPSOLVE, GLPK };

public:
	LinearProgram() {}
	~LinearProgram() {}

	// num of binary variables
	void add_variable(const Variable& var) { 
		variables_.push_back(var);
	}
	void add_variables(const std::vector<Variable>& vars) {
		variables_.insert(variables_.end(), vars.begin(), vars.end());
	}
	const std::vector<Variable>& variables() const {
		return variables_;
	}

	void set_objective(const Objective& obj) { 
		objective_ = obj; 
	}
	const Objective& objective() const {
		return objective_;
	}

	void add_constraint(const Constraint& cstr) { 
		if (cstr.bound_type() == Constraint::FREE) {
			std::cerr << "incomplete constraint: no bound(s) specified" << std::endl;
			return;
		}
		constraints_.push_back(cstr); 
	}
	void add_constraints(const std::vector<Constraint>& cstrs) {
		constraints_.insert(constraints_.end(), cstrs.begin(), cstrs.end());
	}
	const std::vector<Constraint>& constraints() const {
		return constraints_; 
	}

	std::size_t num_continuous_variables() const {
		std::size_t num_continuous_var = 0;
		for (std::size_t i = 0; i < variables_.size(); ++i) {
			if (variables_[i].variable_type() == Variable::CONTINUOUS)
				++num_continuous_var;
		}
		return num_continuous_var;
	}	
	
	std::size_t num_integer_variables() const {
		std::size_t num_iteger_var = 0;
		for (std::size_t i = 0; i < variables_.size(); ++i) {
			if (variables_[i].variable_type() == Variable::INTEGER)
				++num_iteger_var;
		}
		return num_iteger_var;
	}

	std::size_t num_binary_variables() const {
		std::size_t num_binary_var = 0;
		for (std::size_t i = 0; i < variables_.size(); ++i) {
			if (variables_[i].variable_type() == Variable::BINARY)
				++num_binary_var;
		}
		return num_binary_var;
	}

	// returns true if mixed inter program
	bool is_mix_integer_program() const {
		std::size_t num = num_continuous_variables();
		return (num > 0) && (num < variables_.size());
	}

	// returns true if inter program
	bool is_integer_program() const {
		std::size_t num = num_integer_variables();
		return (num > 0) && (num == variables_.size());
	}

	// returns true if binary program
	bool is_binary_proram() const {
		std::size_t num = num_binary_variables();
		return (num > 0) && (num == variables_.size());
	}

private:
	std::vector<Variable>	variables_;
	std::vector<Constraint>	constraints_;

	Objective	objective_;
	double		objective_value_;
};

