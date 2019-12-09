#pragma once

#define STRINGVIEWIFY(s) std::string_view(IFY(s))
#define STRINGIFY(s) IFY(s)
#define IFY(s) #s

#include <string>
#include <deque>
#include <type_traits>
#include <string_view>
#include <ratio>

#include "base/vector.h"
#include "base/Ptr.h"
#include "config/command_line.h"
#include "config/ArgManager.h"
#include "hardware/EventDrivenGP.h"
#include "tools/MatchBin.h"
#include "tools/matchbin_utils.h"

#include "ConfigBase.h"
#include "MultiMatchBin.h"

class FrameHardware;

class Config : public ConfigBase {

public:

  static constexpr size_t TAG_WIDTH = 32;

  using TRAIT_TYPE = emp::Ptr<FrameHardware>;
  using chanid_t = uint64_t;

  using DEPO_T = emp::DePoSelector<std::ratio<1,5>>;

  using matchbin_t = MultiMatchBin<
    size_t
#ifdef METRIC
    , std::conditional<STRINGVIEWIFY(METRIC) == "integer",
        emp::SymmetricWrapMetric<TAG_WIDTH>,
      std::conditional<STRINGVIEWIFY(METRIC) == "streak",
        emp::CacheMod<emp::UnifMod<emp::ApproxDualStreakMetric<TAG_WIDTH>>>,
      std::conditional<STRINGVIEWIFY(METRIC) == "simplestreak",
        emp::ExactSingleStreakMetric<TAG_WIDTH>,
      std::conditional<STRINGVIEWIFY(METRIC) == "hash",
        emp::HashMetric<TAG_WIDTH>,
      std::conditional<STRINGVIEWIFY(METRIC) == "hamming",
        emp::HammingCumuMetric<TAG_WIDTH>,
        std::enable_if<false>
      >::type
      >::type
      >::type
      >::type
      >::type
#else
    // default
    , emp::CacheMod<emp::UnifMod<emp::ApproxDualStreakMetric<TAG_WIDTH>>>
#endif
#ifdef SELECTOR
    , std::conditional<STRINGVIEWIFY(SELECTOR) == "roulette",
        emp::RouletteSelector<
          std::ratio<1, 2>,
          std::ratio<1, 500>,
          std::ratio<1, 4>
        >,
      std::conditional<STRINGVIEWIFY(SELECTOR) == "exproulette",
        emp::ExpRouletteSelector<>,
      std::conditional<STRINGVIEWIFY(SELECTOR) == "sieve",
        emp::SieveSelector<>,
      std::conditional<STRINGVIEWIFY(SELECTOR) == "depo",
        DEPO_T,
      std::conditional<STRINGVIEWIFY(SELECTOR) == "ranked",
        emp::RankedSelector<std::ratio<1, 2>>,
        std::enable_if<false>
      >::type
      >::type
      >::type
      >::type
#else
    , DEPO_T
#endif
    // secondary selector
    , emp::SieveSelector<std::ratio<1, 5>>
#ifdef REGULATOR
    , std::conditional<STRINGVIEWIFY(REGULATOR) == "multiplicative",
        emp::MultiplicativeCountdownRegulator<>,
      std::conditional<STRINGVIEWIFY(SELECTOR) == "additive",
        emp::AdditiveCountdownRegulator<>,
        std::enable_if<false>
    >::type
    >::type
#else
    , emp::MultiplicativeCountdownRegulator<>
#endif
  >;

  using hardware_t = emp::EventDrivenGP_AW<
    TAG_WIDTH
    , TRAIT_TYPE
    , matchbin_t
  >;
  using program_t = hardware_t::program_t;
  using inst_lib_t = emp::InstLib<hardware_t>;
  using event_lib_t = emp::EventLib<hardware_t>;
  using event_t = hardware_t::event_t;
  using inbox_t = std::deque<event_t>;
  using tag_t = hardware_t::affinity_t;

  Config();

  void WriteMe(std::ostream & out) const;

};
