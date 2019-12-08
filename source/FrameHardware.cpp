#pragma once

#include <algorithm>

#include "base/vector.h"
#include "base/Ptr.h"

#include "FrameHardware.h"
#include "Config.h"

FrameHardware::FrameHardware(
    const Config &cfg_,
    const Config::inst_lib_t &inst_lib,
    const Config::event_lib_t &event_lib,
    emp::Random &rand
  ) : cfg(cfg_)
  , cpu(inst_lib, event_lib, &rand)
  , pro()
  , anti(pro.NOT())
  {
    cpu.SetTrait(this);
    SoftReset();
  }

void FrameHardware::SoftReset() {
  guess = std::numeric_limits<size_t>::max();
  cpu.GetMatchBin().GetSelector().Decay();
  inbox.clear();
}

emp::vector<std::reference_wrapper<FrameHardware>>& FrameHardware::GetNeighs() {
  return neighs;
}

void FrameHardware::SetNeighs(
  const emp::vector<std::reference_wrapper<FrameHardware>>& set
) {
  neighs = set;
}

size_t FrameHardware::GetState() const { return state; }

void FrameHardware::SetState(const size_t set) { state = set; }

size_t FrameHardware::GetGuess() const { return guess; };

void FrameHardware::SetGuess(const size_t set) { guess = set; };

void FrameHardware::SetProgram(const Config::program_t & program) {
  cpu.SetProgram(program);
}

void FrameHardware::Step(const size_t step) {

  cpu.GetMatchBin().GetSelector().Decay();

  // const static Config::tag_t neutral(rand);

  cpu.GetMatchBin().GetSelector().SetCurDePoAmt(1.0);
  if (GetState()) {
    cpu.SpawnCore(pro, cpu.GetMinBindThresh());
  } else {
    cpu.SpawnCore(anti, cpu.GetMinBindThresh());
  }

  //hw.SpawnCore(neutral, hw.GetMinBindThresh());

  // enq inbox messages
  while(!inbox.empty()) {
    cpu.QueueEvent(inbox.front());
    inbox.pop_front();
  }

  // compute!
  for (size_t i = 0; i < 16; ++i) {
    cpu.SingleProcess();
  }

}

void FrameHardware::TakeMessage(const Config::event_t & event) {

  // Make room for new message in inbox. Remove oldest first.
  while (
    cfg.INBOX_CAPACITY()
    && inbox.size() >= cfg.INBOX_CAPACITY()
  ) inbox.pop_front();

  if (cfg.INBOX_CAPACITY()) inbox.emplace_back(event);

}
