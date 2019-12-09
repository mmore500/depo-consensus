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

  constexpr size_t POP_SIZE = 100;
  constexpr size_t GENS = 10000;

  emp::Random random(cfg.SEED());

  emp::World<Config::program_t> grid_world(random);
  grid_world.SetPopStruct_Grid(10, 10);

  grid_world.SetupFitnessFile();
  grid_world.AddSystematics(
    emp::NewPtr<emp::Systematics<Config::program_t,Config::program_t>>(
      [](Config::program_t & o){return o;},
      true,
      true,
      false
    ),
    "systematics"
  );
  grid_world.SetupSystematicsFile("systematics");

  Evaluator eval(
    LibraryInstruction::Make(cfg),
    LibraryEvent::Make(cfg),
    random,
    cfg
  );
  grid_world.SetFitFun(
    [&eval](Config::program_t & org){
      return eval.Evaluate(org);
    }
  );

  Mutator mut(cfg);
  grid_world.SetMutFun(
    [&mut](Config::program_t & org, emp::Random & rand) {
      return mut.ApplyMutations(org, rand);
    }
  );
  grid_world.SetAutoMutate();

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

  for (size_t g = 0; g < GENS; ++g) {
    emp::TournamentSelect(grid_world, 2, 100);
    grid_world.Update();
    std::cout << "." << std::flush;
  }

}

int main(int argc, char* argv[]) {

  std::cout << "GIT VERSION " << STRINGIFY(GIT_VERSION_) << std::endl;
  std::cout << "EMP HASH " << STRINGIFY(EMPIRICAL_HASH_) << std::endl;
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
