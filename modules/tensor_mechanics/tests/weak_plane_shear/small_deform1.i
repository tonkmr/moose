# deformations are applied so that the trial stress is
# shear = 10, normalstress = 2
#
# Cohesion is chosen to be 1, and friction angle = 26.565, so tan(friction_angle) = 1/2
# This means that (shear, normalstress) = (0, 2) is the apex
# of the shear envelope
#
# Poisson's ratio is chosen to be zero, and Lame mu = 1E6,
# so the return must solve
# f = 0
# shear = shear_trial - (1/2)*mu*ga = 10 - 0.5E6*ga
# normalstress = normalstress - mu*tan(dilation)*ga
#
# Finally, tan(dilation) = 2/18 is chosen.
#
# Then the returned value should have
# shear = 1, normalstress = 0
#
# Here shear = sqrt(s_yz^2 + s_xz^2)
[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 1
  nz = 1
  xmin = -0.5
  xmax = 0.5
  ymin = -0.5
  ymax = 0.5
  zmin = -0.5
  zmax = 0.5
[]


[Variables]
  [./x_disp]
  [../]
  [./y_disp]
  [../]
  [./z_disp]
  [../]
[]

[TensorMechanics]
  [./solid]
    disp_x = x_disp
    disp_y = y_disp
    disp_z = z_disp
  [../]
[]


[BCs]
  [./bottomx]
    type = PresetBC
    variable = x_disp
    boundary = back
    value = 0.0
  [../]
  [./bottomy]
    type = PresetBC
    variable = y_disp
    boundary = back
    value = 0.0
  [../]
  [./bottomz]
    type = PresetBC
    variable = z_disp
    boundary = back
    value = 0.0
  [../]

  [./topx]
    type = PresetBC
    variable = x_disp
    boundary = front
    value = 8E-6
  [../]
  [./topy]
    type = PresetBC
    variable = y_disp
    boundary = front
    value = 6E-6
  [../]
  [./topz]
    type = PresetBC
    variable = z_disp
    boundary = front
    value = 1E-6
  [../]
[]

[AuxVariables]
  [./stress_xz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_zx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_yz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_zz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./yield_fcn]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./stress_xz]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_xz
    index_i = 0
    index_j = 2
  [../]
  [./stress_zx]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_zx
    index_i = 2
    index_j = 0
  [../]
  [./stress_yz]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_yz
    index_i = 1
    index_j = 2
  [../]
  [./stress_zz]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_zz
    index_i = 2
    index_j = 2
  [../]
  [./yield_fcn_auxk]
    type = MaterialRealAux
    property = weak_plane_shear_yield_function
    variable = yield_fcn
  [../]
[]

[Postprocessors]
  [./s_xz]
    type = PointValue
    point = '0 0 0'
    variable = stress_xz
  [../]
  [./s_yz]
    type = PointValue
    point = '0 0 0'
    variable = stress_yz
  [../]
  [./s_zz]
    type = PointValue
    point = '0 0 0'
    variable = stress_zz
  [../]
  [./f]
    type = PointValue
    point = '0 0 0'
    variable = yield_fcn
  [../]
[]

[Materials]
  [./mc]
    type = FiniteStrainWeakPlaneShear
    block = 0
    wps_cohesion = 1
    wps_dilation_angle = 6.34
    disp_x = x_disp
    disp_y = y_disp
    disp_z = z_disp
    yield_function_tolerance = 1E-6
    fill_method = symmetric_isotropic
    C_ijkl = '0 1E6'
    wps_friction_angle = 26.565051178
    wps_normal_vector = '0 0 1'
    wps_normal_rotates = false
    direction_tolerance = 1E-5
    internal_constraint_tolerance = 1E-6
    wps_smoother = 0
  [../]
[]


[Executioner]
  end_time = 1
  dt = 1
  type = Transient
[]


[Outputs]
  file_base = small_deform1
  output_initial = true
  exodus = true
  [./console]
    type = Console
    perf_log = true
    linear_residuals = false
  [../]
  [./csv]
    type = CSV
    interval = 1
  [../]
[]
