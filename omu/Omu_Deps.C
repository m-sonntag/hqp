/*
 * Omu_Deps.C
 *   -- class definition
 *
 * rf, 7/31/01
 */

/*
    Copyright (C) 1997--2007  Ruediger Franke

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

#include "Omu_Deps.h"

//--------------------------------------------------------------------------
Omu_DepVec::Omu_DepVec()
{
  c_setup = false;
  _linear_flags = iv_resize(iv_get(1), 0);
  _linear_vars = iv_resize(iv_get(1), 0);
}

//--------------------------------------------------------------------------
Omu_DepVec::~Omu_DepVec()
{
  iv_free(_linear_vars);
  iv_free(_linear_flags);
}

//--------------------------------------------------------------------------
void Omu_DepVec::size(int dim, int nx, int nu, int ndx, int nxf, int nq)
{
  // should only depend on one of dx or xf
  // (Omu_Dependent::WRT_dx and Omu_Dependent::WRT_xf are equal!)
  assert(ndx == 0 || nxf == 0);
  // should only depend on one of u or q
  // (Omu_Dependent::WRT_u and Omu_Dependent::WRT_q are equal!)
  assert(nu == 0 || nq == 0);

  v_resize(_v, dim);
  iv_resize(_linear_flags, dim);
  Jx.size(dim, nx);
  Ju.size(dim, nu);
  Jdx.size(dim, ndx);
  Jxf.size(dim, nxf);
  Jq.size(dim, nq);
  iv_resize(_linear_vars, nx + nu + ndx + nxf + nq);

  v_zero(_v);
  iv_zero(_linear_flags);
  iv_zero(_linear_vars);
  _required_J = true;
}

//--------------------------------------------------------------------------
void Omu_DepVec::adapt_size(int dim)
{
  assert(dim <= _v->max_dim); // may not re-allocate memory
  assert(dim <= _linear_flags->max_dim);
  _v->dim = dim;
  _linear_flags->dim = dim;
  Jx.adapt_size(dim);
  Ju.adapt_size(dim);
  Jdx.adapt_size(dim);
  Jxf.adapt_size(dim);
  Jq.adapt_size(dim);
}

//--------------------------------------------------------------------------
void Omu_DepVec::analyze_struct()
{
  Jx.analyze_struct(is_linear(Omu_Dependent::WRT_x) || Jx->n == 0);
  Ju.analyze_struct(is_linear(Omu_Dependent::WRT_u) || Ju->n == 0);
  Jdx.analyze_struct(is_linear(Omu_Dependent::WRT_dx) || Jdx->n == 0);
  Jxf.analyze_struct(is_linear(Omu_Dependent::WRT_xf) || Jxf->n == 0);
  Jq.analyze_struct(is_linear(Omu_Dependent::WRT_q) || Jq->n == 0);
}

//==========================================================================

//--------------------------------------------------------------------------
Omu_Dep::Omu_Dep()
{
  c_setup = false;
  _linear_flags = 0;
}

//--------------------------------------------------------------------------
void Omu_Dep::size(int nx, int nu, int nxf)
{
  gx.size(nx);
  gu.size(nu);
  gxf.size(nxf);

  _value = 0.0;
  _linear_flags = 0;
  _required_g = true;
}

//--------------------------------------------------------------------------
void Omu_Dep::analyze_struct()
{
  gx.analyze_struct(is_linear(Omu_Dependent::WRT_x) || gx->dim == 0);
  gu.analyze_struct(is_linear(Omu_Dependent::WRT_u) || gu->dim == 0);
  gxf.analyze_struct(is_linear(Omu_Dependent::WRT_xf) || gxf->dim == 0);
}


//==========================================================================
