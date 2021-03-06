#ifndef RANKFOURTENSOR_H
#define RANKFOURTENSOR_H

// Modules includes
#include "RankTwoTensor.h"

// MOOSE includes
#include "PermutationTensor.h"
#include "MooseEnum.h"

// libMesh includes
#include "libmesh/tensor_value.h"
#include "libmesh/libmesh.h"
#include "libmesh/vector_value.h"

// system includes
#include <vector>


/**
 * RankFourTensor is designed to handle any N-dimensional fourth order tensor, C.
 *
 * It is designed to allow for maximum clarity of the mathematics and ease of use.
 * Original class authors: A. M. Jokisaari, O. Heinonen, M.R. Tonks
 *
 * Since N is hard-coded to 3, RankFourTensor holds 81 separate C_ijkl entries.
 * Within the code i = 0, 1, 2, but this object provides methods to extract the entries
 * with i = 1, 2, 3, and some of the documentation is also written in this way.
 */
class RankFourTensor
{
public:

  /// Default constructor; fills to zero
  RankFourTensor();

  /// Copy constructor
  RankFourTensor(const RankFourTensor &a);

  /// Destructor
  ~RankFourTensor() {}

  /// Gets the value for the index specified.  Takes index = 0,1,2
  Real & operator()(unsigned int i, unsigned int j, unsigned int k, unsigned int l);


  /**
   * Gets the value for the index specified.  Takes index = 0,1,2
   * used for const
   */
  Real operator()(unsigned int i, unsigned int j, unsigned int k, unsigned int l) const;

  ///Sets the value for the index specified.  Indices = 0, 1, 2
  void setValue(Real val, unsigned int i, unsigned int j, unsigned int k, unsigned int l);

  /// Gets the value for the index specified.  Takes index = 1,2,3
  Real getValue(unsigned int i, unsigned int j, unsigned int k, unsigned int l) const;

  /// Zeros out the tensor.
  void zero();

  /// copies values from a into this tensor
  RankFourTensor & operator= (const RankFourTensor &a);

  /// C_ijkl*a_kl
  RankTwoTensor operator* (const RankTwoTensor &a) const;

  /// C_ijkl*a_kl
  RealTensorValue operator* (const RealTensorValue &a) const;

  /// C_ijkl*a
  RankFourTensor operator* (const Real &a) const;

  /// C_ijkl *= a
  RankFourTensor & operator*= (const Real &a);

  /// C_ijkl/a
  RankFourTensor operator/ (const Real &a) const;

  /// C_ijkl /= a  for all i, j, k, l
  RankFourTensor & operator/= (const Real &a);

  /// C_ijkl += a_ijkl  for all i, j, k, l
  RankFourTensor & operator+= (const RankFourTensor &a);

  /// C_ijkl + a_ijkl
  RankFourTensor operator+ (const RankFourTensor &a) const;

  /// C_ijkl -= a_ijkl
  RankFourTensor & operator-= (const RankFourTensor &a);

  /// C_ijkl - a_ijkl
  RankFourTensor operator- (const RankFourTensor &a) const;

  /// -C_ijkl
  RankFourTensor operator- () const;

  /// C_ijpq*a_pqkl
  RankFourTensor operator* (const RankFourTensor &a) const;

  /**
   * This returns A_ijkl such that C_ijkl*A_klmn = 0.5*(de_im de_jn + de_in de_jm)
   * This routine assumes that C_ijkl = C_jikl = C_ijlk
   */
  RankFourTensor invSymm() const;

  /**
   * Rotate the tensor using
   * C_ijkl = R_im R_in R_ko R_lp C_mnop
   */
  virtual void rotate(RealTensorValue &R);

  /// Print tensor using nice formatting and Moose::out
  void print() const;

  /**
   * Transpose the tensor by swapping the first pair with the second pair of indices
   * @return C_klji
   */
  RankFourTensor transposeMajor() const;


  /**
   * Fills the tensor entries ignoring the last dimension (ie, C_ijkl=0 if any of i, j, k, or l = 3).
   * Fill method depends on size of input
   * Input size = 2.  Then C_1111 = C_2222 = input[0], and C_1122 = input[1], and C_1212 = (input[0] + input[1])/2,
                      and C_ijkl = C_jikl = C_ijlk = C_klij, and C_1211 = C_1222 = 0.
   * Input size = 9.  Then C_1111 = input[0], C_1112 = input[1], C_1122 = input[3],
                           C_1212 = input[4], C_1222 = input[5], C_1211 = input[6]
                           C_2211 = input[7], C_2212 = input[8], C_2222 = input[9]
                           and C_ijkl = C_jikl = C_ijlk
  */
  virtual void surfaceFillFromInputVector(const std::vector<Real> input);


  /// Static method for use in validParams for getting the "fill_method"
  static MooseEnum fillMethodEnum();

  /**
   * To fill up the 81 entries in the 4th-order tensor, fillFromInputVector
   * is called with one of the following fill_methods.
   * See the fill*FromInputVector functions for more details
   */
  enum FillMethod
  {
    antisymmetric,
    symmetric9,
    symmetric21,
    general_isotropic,
    symmetric_isotropic,
    antisymmetric_isotropic,
    general
  };


  /**
   * fillFromInputVector takes some number of inputs to fill
   * the Rank-4 tensor.
   * @param input the numbers that will be placed in the tensor
   * @param fill_method this can be:
   *             antisymmetric (use fillAntisymmetricFromInputVector)
   *             symmetric9 (use fillSymmetricFromInputVector with all=false)
   *             symmetric21 (use fillSymmetricFromInputVector with all=true)
   *             general_isotropic (use fillGeneralIsotropicFrominputVector)
   *             symmetric_isotropic (use fillSymmetricIsotropicFromInputVector)
   *             antisymmetric_isotropic (use fillAntisymmetricIsotropicFromInputVector)
   *             general (use fillGeneralFromInputVector)
   */
  void fillFromInputVector(const std::vector<Real> input, FillMethod fill_method);

  // Deprecated method, remove after existing Tensor Mechanics applications have been updated
  void fillFromInputVector(const std::vector<Real> input, bool all_21);

protected:

  /// Dimensionality of rank-four tensor
  static const unsigned int N = 3;

  /// The values of the rank-four tensor
  Real _vals[N][N][N][N];

  /**
   * Inverts the dense matrix A using LAPACK routines
   * @param A upon input this is a row vector representing an n-by-n matrix.  Upon output it is the inverse (as a row-vector)
   * @param n size of A
   * @return if zero then inversion was successful.  Otherwise A contained illegal entries or was singular
   */
  int MatrixInversion(double *A, int n) const;

   /**
  * fillSymmetricFromInputVector takes either 21 (all=true) or 9 (all=false) inputs to fill in
  * the Rank-4 tensor with the appropriate crystal symmetries maintained. I.e., C_ijkl = C_klij,
  * C_ijkl = C_ijlk, C_ijkl = C_jikl
  * @param input If all==true then this is
                   C1111 C1122 C1133 C2222 C2233 C3333 C2323 C1313 C1212
                   In the isotropic case this is (la is first Lame constant, mu is second (shear) Lame constant)
                   la+2mu la la la+2mu la la+2mu mu mu mu
                 If all==false then this is
                   C1111 C1122 C1133 C1123 C1113 C1112 C2222 C2233 C2223 C2213 C2212 C3333 C3323 C3313 C3312 C2323 C2313 C2312 C1313 C1312 C1212
  */
  void fillSymmetricFromInputVector(const std::vector<Real> input, bool all);

  /**
   * fillAntisymmetricFromInputVector takes 6 inputs to fill the
   * the antisymmetric Rank-4 tensor with the appropriate symmetries maintained.
   * I.e., B_ijkl = -B_jikl = -B_ijlk = B_klij
   * @param input this is B1212, B1213, B1223, B1313, B1323, B2323.
   */
  void fillAntisymmetricFromInputVector(const std::vector<Real> input);

  /**
   * fillGeneralIsotropicFromInputVector takes 3 inputs to fill the
   * Rank-4 tensor with symmetries C_ijkl = C_klij, and isotropy, ie
   * C_ijkl = la*de_ij*de_kl + mu*(de_ik*de_jl + de_il*de_jk) + a*ep_ijm*ep_klm
   * where la is the first Lame modulus, mu is the second (shear) Lame modulus,
   * and a is the antisymmetric shear modulus, and ep is the permutation tensor
   * @param input this is la, mu, a in the above formula
   */
  void fillGeneralIsotropicFromInputVector(const std::vector<Real> input);

  /**
   * fillAntisymmetricIsotropicFromInputVector takes 1 inputs to fill the
   * the antisymmetric Rank-4 tensor with the appropriate symmetries maintained.
   * I.e., C_ijkl = a * ep_ijm * ep_klm, where epsilon is the permutation tensor (and sum on m)
   * @param input this is a in the above formula
   */
  void fillAntisymmetricIsotropicFromInputVector(const std::vector<Real> input);

  /**
   * fillSymmetricIsotropicFromInputVector takes 2 inputs to fill the
   * the symmetric Rank-4 tensor with the appropriate symmetries maintained.
   * C_ijkl = la*de_ij*de_kl + mu*(de_ik*de_jl + de_il*de_jk)
   * where la is the first Lame modulus, mu is the second (shear) Lame modulus,
   * @param input this is la and mu in the above formula
   */
  void fillSymmetricIsotropicFromInputVector(const std::vector<Real> input);

  /**
   * fillGeneralFromInputVector takes 81 inputs to fill the Rank-4 tensor
   * No symmetries are explicitly maintained
   * @param input  C[i][j][k][l] = input[i*N*N*N + j*N*N + k*N + l]
   */
  void fillGeneralFromInputVector(const std::vector<Real> input);


private:

};

#endif //RANKFOURTENSOR_H
