#pragma once

#include <float.h>
#include <string>

#include "config/config.h"

EMP_BUILD_CONFIG(
  ConfigBase,

  GROUP(CORE, "Core settings for DISHTINY"),
  VALUE(SEED, int, 1, "Random number generator seed."),
  VALUE(GRID_H, size_t, 3, "How many tiles tall should the grid be?"),
  VALUE(GRID_W, size_t, 3, "How many tiles wide should the grid be?"),

  GROUP(PROGRAM_GROUP, "SignalGP program Settings"),
  VALUE(PROGRAM_MAX_FUN_CNT__INIT, size_t, 16, "Used for generating SGP programs. At most, how many functions do we generate?"),
  VALUE(PROGRAM_MIN_FUN_CNT__INIT, size_t, 4, "Used for generating SGP programs. At least, how many functions do we generate?"),
  VALUE(PROGRAM_MAX_FUN_LEN__INIT, size_t, 16, "Used for generating SGP programs. At most, for each function how many instructions do we generate?"),
  VALUE(PROGRAM_MIN_FUN_LEN__INIT, size_t, 1, "Used for generating SGP programs. At least, for each function how many instructions do we generate?"),
  VALUE(PROGRAM_MIN_ARG_VAL__INIT, int, 0, "Minimum argument value for instructions."),
  VALUE(PROGRAM_MAX_ARG_VAL__INIT, int, 4, "Maximum argument value for instructions."),

  GROUP(HARDWARE_GROUP, "SignalGP Hardware Settings"),
  VALUE(HARDWARE_STEPS, size_t, 16, "How many hardware steps to run per update?"),
  VALUE(ENVIRONMENT_SIGNALS, bool, true, "Can environment signals trigger functions?"),
  VALUE(ACTIVE_SENSORS, bool, true, "Do agents have function active sensors?"),
  VALUE(HW_MAX_CORES, size_t, 8, "Max number of hardware cores; i.e., max number of simultaneous threads of execution hardware will support."),
  VALUE(HW_MAX_CALL_DEPTH, size_t, 128, "Max call depth of a hardware unit."),
  VALUE(INBOX_CAPACITY, size_t, 16, "Capacity of a cell's messaging inbox."),
  VALUE(ENV_TRIG_FREQ, size_t, 8, "How often to fire environmental trigger events?"),
  VALUE(COMPUTE_FREQ, size_t, 4, "How often to step the CPUs?"),

  GROUP(MUTATION_GROUP, "SignalGP Mutation Settings"),
  VALUE(MUTATION_RATE, double, 0.01, "What percentage of offspring should experience mutations?"),
  VALUE(PROPAGULE_MUTATION_RATE, double, 0.0, "What percentage of propagule offspring should experience additional mutations?"),
  VALUE(PROGRAM_MIN_ARG_VAL, int, 0, "Minimum argument value for instructions."),
  VALUE(PROGRAM_MAX_ARG_VAL, int, 8, "Maximum argument value for instructions."),
  VALUE(TAG_BIT_FLIP__PER_BIT, double, 0.005, "Per-bit mutation rate of tag bit flips."),
  VALUE(INST_SUB__PER_INST, double, 0.005, "Per-instruction/argument subsitution mutation rate."),
  VALUE(ARG_SUB__PER_ARG, double, 0.005, "Per-instruction/argument subsitution mutation rate."),
  VALUE(INST_INS__PER_INST, double, 0.005, "Per-instruction insertion mutation rate."),
  VALUE(INST_DEL__PER_INST, double, 0.005, "Per-instruction deletion mutation rate."),
  VALUE(SLIP__PER_FUNC, double, 0.05, "Per-function rate of slip mutations."),
  VALUE(FUNC_DUP__PER_FUNC, double, 0.01, "Per-function rate of function duplication mutations."),
  VALUE(FUNC_DEL__PER_FUNC, double, 0.01, "Per-function rate of function deletion mutationss."),
  VALUE(PROGRAM_MAX_FUN_CNT, size_t, 512, "Used for mutating SGP programs. At most, how many functions can we have?"),
  VALUE(PROGRAM_MIN_FUN_CNT, size_t, 1, "Used for mutating SGP programs. At least, how many functions can we have?"),
  VALUE(PROGRAM_MAX_FUN_LEN, size_t, 64, "Used for mutating SGP programs. At most, for each function how many instructions can we have?"),
  VALUE(PROGRAM_MIN_FUN_LEN, size_t, 1, "Used for mutating SGP programs. At least, for each function how many instructions can we have?")

)
