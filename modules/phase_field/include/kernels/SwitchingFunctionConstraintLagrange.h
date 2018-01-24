//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef SWITCHINGFUNCTIONCONSTRAINTLAGRANGE_H
#define SWITCHINGFUNCTIONCONSTRAINTLAGRANGE_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"
#include "NonlinearSystem.h"

// Forward Declarations
class SwitchingFunctionConstraintLagrange;

template <>
InputParameters validParams<SwitchingFunctionConstraintLagrange>();

/**
 * SwitchingFunctionConstraintLagrange is a constraint kernel that acts on the
 * lambda lagrange multiplier non-linear variables to
 * enforce \f$ \sum_n h_i(\eta_i) - \epsilon\lambda \equiv 1 \f$.
 */
class SwitchingFunctionConstraintLagrange : public DerivativeMaterialInterface<Kernel>
{
public:
  SwitchingFunctionConstraintLagrange(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int);

  /// Switching function names
  std::vector<MaterialPropertyName> _h_names;
  unsigned int _num_h;

  /// Switching functions and their drivatives
  std::vector<const MaterialProperty<Real> *> _h, _dh;

  /// number of non-linear variables in the problem
  const unsigned int _number_of_nl_variables;

  /// eta index for the j_vars in the jacobian computation
  std::vector<int> _j_eta;

  /// shift factor
  Real _epsilon;
};

#endif // SWITCHINGFUNCTIONCONSTRAINTLAGRANGE_H
