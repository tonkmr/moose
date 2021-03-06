#include "INSTemperatureNoBCBC.h"

template<>
InputParameters validParams<INSTemperatureNoBCBC>()
{
  InputParameters params = validParams<IntegratedBC>();

  // Required parameters
  params.addRequiredParam<Real>("k", "thermal conductivity");

  return params;
}



INSTemperatureNoBCBC::INSTemperatureNoBCBC(const std::string & name, InputParameters parameters) :
  IntegratedBC(name, parameters),
  // Required parameters
  _k(getParam<Real>("k"))
{
}



Real INSTemperatureNoBCBC::computeQpResidual()
{
  // k * (grad_T.n) * test
  return _k * _grad_u[_qp] * _normals[_qp] * _test[_i][_qp];
}




Real INSTemperatureNoBCBC::computeQpJacobian()
{
  return _k * (_grad_phi[_j][_qp]*_normals[_qp]) * _test[_i][_qp];
}




Real INSTemperatureNoBCBC::computeQpOffDiagJacobian(unsigned /*jvar*/)
{
  // off-diagonal derivatives are all zero.
  return 0.;
}
