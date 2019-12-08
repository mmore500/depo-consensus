#pragma once

#include <string>
#include <deque>

#include "base/vector.h"
#include "base/Ptr.h"
#include "config/command_line.h"
#include "config/ArgManager.h"
#include "hardware/EventDrivenGP.h"

#include "Config.h"
#include "ConfigBase.h"

Config::Config() {
  // check conditions on settings
  emp_assert(SEED() > 0);
}

void Config::WriteMe(std::ostream & out) const {
  Write(out);

}
