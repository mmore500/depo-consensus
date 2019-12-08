#pragma once

#include "Config.h"
#include "FrameHardware.h"
#include "GeometryHelper.h"

class Evaluator {

  emp::Random &rand;
  const Config &cfg;
  GeometryHelper geom;

  emp::vector<emp::Ptr<FrameHardware>> frames;

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
      frames.push_back(
        emp::NewPtr<FrameHardware>(
          cfg, inst_lib, event_lib, rand
        )
      );
    }

    for (size_t i = 0; i < frames.size(); ++i) {
      emp::vector<std::reference_wrapper<FrameHardware>> res;
      for (const auto & neigh : geom.CalcLocalNeighs(i)) {
        res.push_back(*frames[neigh]);
      }
      frames[i]->SetNeighs(res);
    }

    emp_assert(frames.size());

  }

  ~Evaluator() { for (auto & frame : frames) frame.Delete(); }

  void SetProgram(const Config::program_t &program) {
    for (auto & frame : frames) frame->SetProgram(program);
  }

  double EvaluateOnce(const size_t underlying_state) {
    // const double state_probability = (
    //   underlying_state
    //   ? 0.9
    //   : 0.1
    // );
    // emp::vector<size_t> yeps(state_probability *frames.size(), 1);
    // emp::vector<size_t> nopes(frames.size() - yeps.size(), 0);

    emp::vector<size_t> yeps(
      underlying_state ? frames.size() - 1 : 1,
      1
    );
    emp::vector<size_t> nopes(
      !underlying_state ? frames.size() - 1 : 1,
      0
    );

    emp::vector<size_t> shuffler(yeps);
    shuffler.insert(
      std::end(shuffler),
      std::begin(nopes),
      std::end(nopes)
    );

    emp::Shuffle(rand, shuffler);

    emp_assert(shuffler.size() == frames.size());

    for (size_t i = 0; i < frames.size(); ++i) {
      auto & frame = frames[i];
      frame->SoftReset();
      frame->SetState(shuffler[i]);
    }

    for (size_t step = 0; step < 32; ++step) {
      for (auto & frame : frames) {
        frame->Step(step);
      }
    }

    // for (auto & frame : frames) frame->SetGuess(underlying_state);

    const double correctness_score = std::count_if(
      std::begin(frames),
      std::end(frames),
      [underlying_state](const auto & frame){
        return frame->GetGuess() == underlying_state;
      }
    );

    const double guess_score = std::count_if(
      std::begin(frames),
      std::end(frames),
      [](const auto & frame){
        return (
          frame->GetGuess() != std::numeric_limits<size_t>::max()
        );
      }
    );

    const double talking_score = std::count_if(
      std::begin(frames),
      std::end(frames),
      [](const auto & frame){
        return (
          frame->GetGuess() != frame->GetState()
        );
      }
    );

    return (
      correctness_score
        + (
          guess_score + talking_score / frames.size()
        ) / frames.size()
    ) / frames.size();

  }

  double Evaluate(const Config::program_t & org) {

    SetProgram(org);
    return (EvaluateOnce(0) + EvaluateOnce(1)) / 2;

  }

};
