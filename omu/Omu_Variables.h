/** 
 * @file Omu_Variables.h 
 *   Independent variables for Omuses problem setup.
 *
 * rf, 10/10/01
 */

/*
    Copyright (C) 1997--2014  Ruediger Franke

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; 
    version 2 of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library (file COPYING.LIB);
    if not, write to the Free Software Foundation, Inc.,
    59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef Omu_Variables_H
#define Omu_Variables_H

#include "Omu_Vec.h"

/** Vector of optimization variables, including min, max and initial
    attributes. */
class Omu_VariableVec: public Omu_Vec {
  			// base class holds variable values
 public:
  VECP min;		///< minimal permitted values (default: -Inf)
  VECP max;		///< maximal permitted values (default: +Inf)
  VECP initial;		///< initial values (default: 0.0)
  IVECP integer; 	///< integer variables (default: 0)

  Omu_VariableVec();	///< allocate empty vectors
  virtual ~Omu_VariableVec(); ///< destroy vectors

  /** Allocate vectors of size n_expand for min, max, initial, and this
      (default n_expand = n).
      A derived classes may overload alloc() to restrict 
      the capability of allocations for specific vectors.
  */
  virtual void alloc(int n, int n_expand = -1);

  /** Number of variables treated by the optimizer,
      n <= n_expand = dim. */
  int n() const {return _n;}

 private:
  int _n; ///< number of variables treated by the optimizer

  /// dismiss copy constructor and assignment operator
  //@{
  Omu_VariableVec(const Omu_VariableVec &);
  Omu_VariableVec &operator=(const Omu_VariableVec &);
  //@}
};

/** Vector of state variables, including sensitivity matrices Sx, Su and Sq
    that hold sensitivities wrt initial states, controls and parameters,
    respectively. */
class Omu_StateVec: public Omu_Vec {
public:
  MATP Sx;	///< sensitivity matrix wrt initial states
  MATP Su;	///< sensitivity matrix wrt control parameters
  MATP Sq;	///< sensitivity matrix wrt general parameters

  /// allocate empty sensitivity matrices
  Omu_StateVec() {
    Sx = m_resize(m_get(1, 1), 0, 0);
    Su = m_resize(m_get(1, 1), 0, 0);
    Sq = m_resize(m_get(1, 1), 0, 0);
  }
  /// destroy sensitivity matrices
  ~Omu_StateVec() {
    m_free(Sq);
    m_free(Su);
    m_free(Sx);
  }

 protected:
  /// protect copy constructor and operator= as they should not be used
  //@{
  Omu_StateVec(const Omu_StateVec &sv): Omu_Vec(sv) {
    m_copy(sv.Sx, Sx);
    m_copy(sv.Su, Su);
    m_copy(sv.Sq, Sq);
  }
  Omu_Vec &operator=(const Omu_StateVec &sv) {
    m_copy(sv.Sx, Sx);
    m_copy(sv.Su, Su);
    m_copy(sv.Sq, Sq);
    return *this;
  }
  //@}
};

/** Depreciated name for Omu_VariableVec. */
typedef Omu_VariableVec Omu_Vector;

/** Extend Omu_VariableVec with attributes for optimization criterion. */
class Omu_OptVarVec: public Omu_VariableVec {
public:
  VECP 	weight1;	///< weight for linear objective term (default: 0.0)
  VECP 	weight2;	///< weight for quadratic objective term (default: 0.0)
  VECP 	ref; 		///< reference value for quadratic term (default: 0.0)
  IVECP active; 	///< indicate used variables (default: 0 -- not used)

  Omu_OptVarVec(); 		///< allocate empty vectors
  virtual ~Omu_OptVarVec(); 	///< destroy vectors
  virtual void resize(int n); 	///< resize and initialize vectors
};

#endif
