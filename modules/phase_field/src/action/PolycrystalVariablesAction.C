#include "PolycrystalVariablesAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"

#include <sstream>
#include <stdexcept>

// libMesh includes
#include "libmesh/libmesh.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/equation_systems.h"
#include "libmesh/nonlinear_implicit_system.h"
#include "libmesh/explicit_system.h"
#include "libmesh/string_to_enum.h"

const Real PolycrystalVariablesAction::_abs_zero_tol = 1e-12;

template<>
InputParameters validParams<PolycrystalVariablesAction>()
{
  InputParameters params = validParams<Action>();
  params.addParam<std::string>("family", "LAGRANGE", "Specifies the family of FE shape functions to use for this variable");
  params.addParam<std::string>("order", "FIRST", "Specifies the order of the FE shape function to use for this variable");
  params.addParam<Real>("scaling", 1.0, "Specifies a scaling factor to apply to this variable");
  params.addRequiredParam<unsigned int>("crys_num", "specifies the number of order parameters to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");

  return params;
}

PolycrystalVariablesAction::PolycrystalVariablesAction(const std::string & name, InputParameters params) :
    Action(name, params),
    _crys_num(getParam<unsigned int>("crys_num")),
    _var_name_base(getParam<std::string>("var_name_base"))
{}

void
PolycrystalVariablesAction::act()
{
#ifdef DEBUG
  Moose::err << "Inside the PolycrystalVariablesAction Object\n"
             << "VariableBase: " << _var_name_base
             << "\torder: " << getParam<std::string>("order")
             << "\tfamily: " << getParam<std::string>("family") << std::endl;
#endif

  // Loop through the number of order parameters
  for (unsigned int crys = 0; crys < _crys_num; crys++)
  {
    //Create variable names
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << crys;
    var_name.append(out.str());

    _problem->addVariable(var_name,
                          FEType(Utility::string_to_enum<Order>(getParam<std::string>("order")),
                                 Utility::string_to_enum<FEFamily>(getParam<std::string>("family"))),
                          getParam<Real>("scaling"));
  }
}
