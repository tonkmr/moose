#include "SolidMechZ.h"

template<>
InputParameters validParams<SolidMechZ>()
{
  InputParameters params = validParams<SolidMech>();
  params.addRequiredCoupledVar("x", "Coupled Displacement in the x Direction");
  params.addRequiredCoupledVar("y", "Coupled Displacement in the y Direction");
  return params;
}

SolidMechZ::SolidMechZ(const std::string & name, InputParameters parameters)
  :SolidMech(name, parameters),
    _x_var(coupled("x")),
    _x(coupledValue("x")),
    _grad_x(coupledGradient("x")),
    _y_var(coupled("y")),
    _y(coupledValue("y")),
    _grad_y(coupledGradient("y"))
  {}

Real
SolidMechZ::computeQpResidual()
  {
    if (!_constant_properties)
      recomputeConstants();

    _strain(0,0) = _grad_x[_qp](0);
    _strain(1,1) = _grad_y[_qp](1);
    _strain(2,2) = _grad_u[_qp](2);
    _strain(1,2) = _grad_u[_qp](1)+_grad_y[_qp](2);
    _strain(0,2) = _grad_x[_qp](2)+_grad_u[_qp](0);

    _stress(0) = _c1*_c3*_strain(0,2);
    _stress(1) = _c1*_c3*_strain(1,2);
    _stress(2) = _c1*_c2*_strain(0,0)+_c1*_c2*_strain(1,1)+_c1*_strain(2,2);

    return (_stress*_grad_test[_i][_qp]);

  }

Real
SolidMechZ::computeQpJacobian()
  {
    if (!_constant_properties)
      recomputeConstants();

    Real value = _c1*(_grad_test[_i][_qp]*(_B33*_grad_phi[_j][_qp]));

    return value;
  }

Real
SolidMechZ::computeQpOffDiagJacobian(unsigned int jvar)
  {
    if (!_constant_properties)
      recomputeConstants();

    RealGradient value = 0;

    if (jvar == _x_var)
      value += _B31*_grad_phi[_j][_qp];
    else if (jvar == _y_var)
      value += _B32*_grad_phi[_j][_qp];

    return _c1*(_grad_test[_i][_qp]*value);
  }
