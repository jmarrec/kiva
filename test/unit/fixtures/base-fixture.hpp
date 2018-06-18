/* Copyright (c) 2012-2017 Big Ladder Software LLC. All rights reserved.
* See the LICENSE file for additional terms and conditions. */

#ifndef BASE_FIXTURE_HPP_
#define BASE_FIXTURE_HPP_

#include <gtest/gtest.h>
#include "Ground.hpp"

using namespace Kiva;

class BaseFixture : public testing::Test {
protected:

  void init(){
    ground = std::make_shared<Ground>(fnd,outputMap);
    ground->buildDomain();
    Foundation::NumericalScheme tempNS = fnd.numericalScheme;
    fnd.numericalScheme = Foundation::NS_STEADY_STATE;
    ground->calculate(bcs);
    fnd.numericalScheme = tempNS;
  }

  double calcQ(){
    init();
    ground->calculateSurfaceAverages();
    return ground->getSurfaceAverageValue({Surface::ST_SLAB_CORE,GroundOutput::OT_RATE});
  }

  double calculate() {
    init();
    for (std::size_t i=0; i<200; i++) {
      bcs.outdoorTemp = 273 + dbt[i % 24];
      ground->calculate(bcs, 3600.0);
      ground->calculateSurfaceAverages();
      ground->getSurfaceAverageValue({Surface::ST_SLAB_CORE,GroundOutput::OT_RATE});
    }
    return ground->getSurfaceAverageValue({Surface::ST_SLAB_CORE,GroundOutput::OT_RATE});
  }

  std::shared_ptr<Ground> ground;
  std::map<Surface::SurfaceType, std::vector<GroundOutput::OutputType>> outputMap;
  BoundaryConditions bcs;
  Foundation fnd;
  double dbt[24]{2.8, 2.4, 2.2, 2.2, 2.2, 2.4, 2.8, 3.2, 3.7, 4.2, 4.7, 5.2,
                 5.6, 5.9, 6.1, 6.2, 6.1, 5.9, 5.6, 5.2, 4.7, 4.2, 3.7, 3.2};
};

#endif /* BASE_FIXTURE_HPP_ */
