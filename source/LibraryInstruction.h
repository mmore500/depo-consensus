#pragma once

#include <functional>
#include <cmath>
#include <limits>
#include <memory>

#include "base/assert.h"
#include "tools/Random.h"
#include "tools/string_utils.h"
#include "hardware/InstLib.h"

#include "FrameHardware.h"
#include "Config.h"
#include "GeometryHelper.h"

class LibraryInstruction {

public:

  using hardware_t = Config::hardware_t;
  using inst_lib_t = Config::inst_lib_t;
  using inst_t = inst_lib_t::inst_t;
  using state_t = hardware_t::State;

public:

  static void InitDefault(inst_lib_t &il);

  static void InitMatchBinRelated(inst_lib_t &il);

  static const inst_lib_t& Make(const Config &cfg);

};
