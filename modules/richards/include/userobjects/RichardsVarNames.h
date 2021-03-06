/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

#ifndef RICHARDSVARNAMES_H
#define RICHARDSVARNAMES_H

#include "GeneralUserObject.h"
#include "Coupleable.h"
#include "ZeroInterface.h"

class RichardsVarNames;


template<>
InputParameters validParams<RichardsVarNames>();

/**
 * This holds maps between pressure_var or pressure_var, sat_var
 * used in RichardsMaterial and kernels, etc, and the
 * variable number used internally by MOOSE
 */
class RichardsVarNames :
  public GeneralUserObject,
  public Coupleable,
  public ZeroInterface
{
 public:
  RichardsVarNames(const std::string & name, InputParameters parameters);

  void initialize();
  void execute();
  void finalize();

  /// the number of porepressure variables
  unsigned int num_v() const;

  /**
   * the MOOSE variable number
   * @param richards_var_num the richards variable number
   * eg if richards_vars = 'pwater pgas', and the variables in
   * the simulation are 'temperature pwater pgas displacement'
   * then moose_var_num(0) = 1
   */
  unsigned int moose_var_num(unsigned int richards_var_num) const;

  /**
   * the richards variable number
   * @param moose_var_num the MOOSE variable number
   * eg if richards_vars = 'pwater pgas', and the variables in
   * the simulation are 'temperature pwater pgas displacement'
   * then richards_var_num(2) = 1
   */
  unsigned int richards_var_num(unsigned int moose_var_num) const;

  /**
   * returns true if moose_var_num is not a richards var
   * @param moose_var_num the MOOSE variable number
   * eg if richards_vars = 'pwater pgas', and the variables in
   * the simulation are 'temperature pwater pgas displacement'
   * then not_pressure_var(0) = true, no_pressure_var(1) = false
   */
  bool not_richards_var(unsigned int moose_var_num) const;

  /**
   * a space-separated string of richards variable names
   * eg richards_names() = 'pwater pgas'
   */
  std::string richards_names() const;

  /**
   * a vector of pointers to VariableValues
   * @param richards_var_num the pressure variable number
   * eg if richards_vars = 'pwater pgas', then
   * (*richards_vals(1))[qp] = pgas evaluated at quadpoint qp
   * Also richards_vals(i) = &coupledValue
   */
  VariableValue * richards_vals(unsigned int richards_var_num) const;

  /**
   * a vector of pointers to old VariableValues
   * @param richards_var_num the richards variable number
   * eg if richards_vars = 'pwater pgas', then
   * (*richards_vals_old(1))[qp] = old pgas evaluated at quadpoint qp
   * Also richards_vals_old(i) = &coupledValueOld
   */
  VariableValue * richards_vals_old(unsigned int richards_var_num) const;

  /**
   * a vector of pointers to grad(Variable)
   * @param richards_var_num the richards variable number
   * eg if richards_vars = 'pwater pgas', then
   * (*grad_var(1))[qp] = grad(pgas) evaluated at quadpoint qp
   * Also grad_var(i) = &coupledGradient
   */
  VariableGradient * grad_var(unsigned int richards_var_num) const;

  /**
   * The moose variable for the given richards_var_num
   * This is got using the getVar function.  It allows
   * direct extraction of nodal variable values
   * used in mass lumping.
   * @param richards_var_num the richards variable number
   */
  MooseVariable * raw_var(unsigned int richards_var_num) const;

  /// return the _var_types string
  std::string var_types() const;


 protected:

  /// number of richards variables
  unsigned int _num_v;

  /// space-separated string of names of porepressure variables
  std::string _the_names;

  /// physical meaning of the variables.  Eg 'pppp' means 'all variables are pressure variables'
  MooseEnum _var_types;

  /// _moose_var_num[i] = the moose variable number corresponding to richards variable i
  std::vector<unsigned int> _moose_var_num;

  /// _pressure_var_num[i] = the richards variable corresponding to moose variable i
  std::vector<unsigned int> _ps_var_num;

  /// moose_var_value[i] = values of richards variable i
  std::vector<VariableValue *> _moose_var_value; // this is a vector of pointers to VariableValues

  /// moose_var_value_old[i] = old values of richards variable i
  std::vector<VariableValue *> _moose_var_value_old;

  /// moose_grad_var[i] = gradient values of richards variable i
  std::vector<VariableGradient *> _moose_grad_var;

  /// _moose_raw_var[i] = getVar of richards variable i
  std::vector<MooseVariable *> _moose_raw_var;

};

#endif // RICHARDSVARNAMES_H
