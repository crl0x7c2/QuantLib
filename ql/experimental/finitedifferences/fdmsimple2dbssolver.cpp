/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2009 Ralph Schreyer

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*!
 * \file fdmsimple2dbssolver.cpp
*/

#include <ql/experimental/finitedifferences/fdmblackscholesop.hpp>
#include <ql/experimental/finitedifferences/fdm2dimsolver.hpp>
#include <ql/experimental/finitedifferences/fdmsimple2dbssolver.hpp>

namespace QuantLib {

    FdmSimple2dBSSolver::FdmSimple2dBSSolver(
        const Handle<GeneralizedBlackScholesProcess>& process,
        Real strike,
        const FdmSolverDesc& solverDesc,
        const FdmSchemeDesc& schemeDesc)
    : process_(process),
      strike_(strike),
      solverDesc_(solverDesc),
      schemeDesc_(schemeDesc) {

        registerWith(process_);
    }

    void FdmSimple2dBSSolver::performCalculations() const {
        boost::shared_ptr<FdmBlackScholesOp> op(new FdmBlackScholesOp(
                solverDesc_.mesher, process_.currentLink(), strike_));

        solver_ = boost::shared_ptr<Fdm2DimSolver>(
                            new Fdm2DimSolver(solverDesc_, schemeDesc_, op));
    }

    Real FdmSimple2dBSSolver::valueAt(Real s, Real a) const {
        calculate();
        return solver_->interpolateAt(std::log(s), std::log(a));
    }

    Real FdmSimple2dBSSolver::deltaAt(Real s, Real a, Real eps) const {
        return (valueAt(s+eps, a) - valueAt(s-eps, a))/(2*eps);
    }

    Real FdmSimple2dBSSolver::gammaAt(Real s, Real a, Real eps) const {
        return (valueAt(s+eps, a)+valueAt(s-eps, a)-2*valueAt(s,a))/(eps*eps);
    }

    Real FdmSimple2dBSSolver::thetaAt(Real s, Real a) const {
        calculate();
        return solver_->thetaAt(std::log(s), std::log(a));
    }
}