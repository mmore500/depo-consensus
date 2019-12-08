#pragma once

#include "Config.h"
#include "FrameHardware.h"
#include "GeometryHelper.h"

class Evaluator {

  emp::Random &rand;
  const Config &cfg;
  GeometryHelper geom;

  emp::vector<FrameHardware> frames;

public:

  Evaluator(
    const Config::inst_lib_t &inst_lib,
    const Config::event_lib_t &event_lib,
    emp::Random &rand_,
    const Config &cfg_
  ) : rand(rand_)
  , cfg(cfg_)
  , geom(cfg_)
  {

    frames.reserve(geom.GetLocalSize());
    for (size_t i = 0; i < geom.GetLocalSize(); ++i) {
      frames.emplace_back(cfg, inst_lib, event_lib, rand);
    }

    for (size_t i = 0; i < frames.size(); ++i) {
      emp::vector<std::reference_wrapper<FrameHardware>> res;
      for (const auto & neigh : geom.CalcLocalNeighs(i)) {
        res.push_back(frames[neigh]);
      }
      frames[i].SetNeighs(res);
    }

  }

  void SetProgram(const Config::program_t &program) {
    for (auto & frame : frames) {
      frame.SetProgram(program);
    }
  }

  double EvaluateOnce() {
    const size_t underlying_state = rand.GetUInt(2);
    const double state_probability = (
      underlying_state
      ? 0.75
      : 0.25
    );

    for (auto & frame : frames) {
      frame.SoftReset();
      frame.SetState(rand.P(state_probability));
    }

    for (size_t step = 0; step < 16; ++step) {
      for (auto & frame : frames) {
        frame.Step(step);
      }
    }

    const double correctness_score = std::count_if(
      std::begin(frames),
      std::end(frames),
      [underlying_state](const auto & frame){
        return frame.GetGuess() == underlying_state;
      }
    );

    const double activity = std::count_if(
      std::begin(frames),
      std::end(frames),
      [underlying_state](const auto & frame){
        return (
          frame.GetGuess() != std::numeric_limits<size_t>::max()
          && frame.GetGuess() != frame.GetState()
        );
      }
    );

    return (
      correctness_score + activity / frames.size()
    ) / frames.size();

  }

  double Evaluate(const Config::program_t & org) {

    SetProgram(org);
    return EvaluateOnce();

  }

};
