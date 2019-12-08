#pragma once

#include <algorithm>

#include "base/vector.h"
#include "base/Ptr.h"

#include "Config.h"

class FrameHardware {

private:

  const Config &cfg;

  size_t guess;
  size_t state;

  emp::vector<std::reference_wrapper<FrameHardware>> neighs;

  Config::hardware_t cpu;

  Config::inbox_t inbox;

  const Config::tag_t pro;

  const Config::tag_t anti;


public:

  FrameHardware() = delete;

  FrameHardware(
      const Config &cfg,
      const Config::inst_lib_t &inst_lib,
      const Config::event_lib_t &event_lib,
      emp::Random &local_rng
    );

  void SoftReset();

  emp::vector<std::reference_wrapper<FrameHardware>>& GetNeighs();

  void SetNeighs(const emp::vector<std::reference_wrapper<FrameHardware>>& set);

  size_t GetState() const;

  void SetState(const size_t set);

  size_t GetGuess() const;

  void SetGuess(const size_t set);

  void SetProgram(const Config::program_t & program);

  void Step(const size_t step);

  void TakeMessage(const Config::event_t & event);


};
