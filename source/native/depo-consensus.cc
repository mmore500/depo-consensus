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


void run(const Config &cfg) {

  emp::Random random(cfg.SEED());

  emp::World<Config::program_t> world(random);

  world.SetupFitnessFile();
  // world.AddSystematics(
  //   emp::NewPtr<emp::Systematics<Config::program_t, Config::program_t>>(
  //     [](Config::program_t & o){return o;},
  //     true,
  //     true,
  //     false
  //   ),
  //   "systematics"
  // );
  // world.SetupSystematicsFile("systematics");

  Evaluator eval(
    LibraryInstruction::Make(cfg),
    LibraryEvent::Make(cfg),
    random,
    cfg
  );
  world.SetFitFun(
    [&eval, &world](Config::program_t & org){
      double res = 0.0;
      const size_t reps = (
        world.GetUpdate() == cfg.GENERATIONS() - 1
        ? 100 // do more evaluations on last update
        : 1
      );
      for (size_t rep = 0; rep < reps; ++rep) {
        res += eval.Evaluate(org);
      }
      return res / reps;
    }
  );

  Mutator mut(cfg);
  world.SetMutFun(
    [&mut](Config::program_t & org, emp::Random & rand) {
      return mut.ApplyMutations(org, rand);
    }
  );
  world.SetAutoMutate();

  for (size_t i = 0; i < cfg.POP_SIZE(); ++i) {
    world.InjectAt(
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

  for (size_t g = 0; g < cfg.GENERATIONS(); ++g) {
    emp::TournamentSelect(world, cfg.TOURNAMENT_SIZE(), cfg.POP_SIZE());
    world.Update();
    std::cout << "." << std::flush;
  }

}

int main(int argc, char* argv[]) {

  std::cout << "PROJECT HASH " << STRINGIFY(PROJECT_HASH_) << std::endl;
  std::cout << "EMPIRICAL HASH " << STRINGIFY(EMPIRICAL_HASH_) << std::endl;
  emp::Random temp(1); // need this to prevent a memory leak
  std::cout << "MATCHBIN "
    << Config::hardware_t(nullptr, nullptr, &temp).GetMatchBin().name()
    << std::endl;

  Config cfg;
  cfg.Read("config.cfg");

  const auto specs = emp::ArgManager::make_builtin_specs(&cfg);
  emp::ArgManager am(argc, argv, specs);
  am.UseCallbacks();
  if (am.HasUnused()) std::exit(EXIT_FAILURE);

  std::cout << "==============================" << std::endl;
  std::cout << "|    How am I configured?    |" << std::endl;
  std::cout << "==============================" << std::endl;
  cfg.WriteMe(std::cout);
  std::cout << "==============================\n" << std::endl;

  run(cfg);
}
