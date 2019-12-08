//  This file is part of Empirical, https://github.com/devosoft/Empirical
//  Copyright (C) Michigan State University, 2016-2017.
//  Released under the MIT Software license; see doc/LICENSE
//
//
//  This file explores the grid options for emp::World.h

#include <iostream>

#include "Evolve/World.h"
#include "tools/Random.h"

#include "../Config.h"
#include "../Evaluator.h"
#include "../LibraryInstruction.h"
#include "../LibraryEvent.h"
#include "../Mutator.h"

#include "../Config.cpp"
#include "../FrameHardware.cpp"
#include "../LibraryInstruction.cpp"


int main()
{

  Config cfg;

  constexpr size_t POP_SIZE = 400;
  constexpr size_t GENS = 10000;

  emp::Random random(cfg.SEED());

  emp::World<Config::program_t> grid_world(random);
  grid_world.SetPopStruct_Grid(20, 20);

  grid_world.SetFitFun(
    [&](Config::program_t & org){
      const size_t nrep_outer = 32;
      const size_t nrep_inner = 1;

      double res = 0.0;
      const size_t seeder = random.GetUInt();
      const auto & il = LibraryInstruction::Make(cfg);
      const auto & el = LibraryEvent::Make(cfg);

      #pragma omp parallel for reduction(+: res)
      for (size_t out = 0; out < nrep_outer; ++out) {
        emp::Random rand(
          (seeder + out) % 161803398
        );
        Config cfg_;
        Evaluator eval(
          il,
          el,
          rand,
          cfg_
        );
        for (size_t in = 0; in < nrep_inner; ++in) {
          res += eval.Evaluate(org);
        }
      }

      std::cout << "." << std::flush;

      return res / (nrep_outer * nrep_inner);

    }
  );

  Mutator mut(cfg);
  grid_world.SetMutFun(
    [&mut](Config::program_t & org, emp::Random & rand) {
      return mut.ApplyMutations(org, rand);
    }
  );

  for (size_t i = 0; i < POP_SIZE; ++i) {
    grid_world.InjectAt(
      emp::GenRandSignalGPProgram<
        Config::TAG_WIDTH,
        Config::TRAIT_TYPE,
        Config::hardware_t::matchbin_t
      >(
        random,
        LibraryInstruction::Make(cfg),
        cfg.PROGRAM_MIN_FUN_CNT__INIT(), cfg.PROGRAM_MAX_FUN_CNT__INIT(),
        cfg.PROGRAM_MIN_FUN_LEN__INIT(), cfg.PROGRAM_MAX_FUN_LEN__INIT(),
        cfg.PROGRAM_MIN_ARG_VAL__INIT(), cfg.PROGRAM_MAX_ARG_VAL__INIT()
      ),
      i
    );
  }

  grid_world.SetupFitnessFile();

  for (size_t g = 0; g < GENS; ++g) {
    emp::TournamentSelect(grid_world, 5, 10);
    grid_world.Update();
    std::cout << std::endl;
  }

}
