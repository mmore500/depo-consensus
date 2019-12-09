#pragma once

#include "LibraryInstruction.h"

using hardware_t = LibraryInstruction::hardware_t;
using inst_lib_t = LibraryInstruction::inst_lib_t;
using inst_t = LibraryInstruction::inst_t;
using state_t = LibraryInstruction::state_t;

void LibraryInstruction::InitDefault(inst_lib_t &il) {

  il.AddInst("Inc", Config::hardware_t::Inst_Inc, 1, "Increment value in local memory Arg1");
  il.AddInst("Dec", Config::hardware_t::Inst_Dec, 1, "Decrement value in local memory Arg1");
  il.AddInst("Not", Config::hardware_t::Inst_Not, 1, "Logically toggle value in local memory Arg1");
  il.AddInst("Add", Config::hardware_t::Inst_Add, 3, "Local memory: Arg3 = Arg1 + Arg2");
  il.AddInst("Sub", Config::hardware_t::Inst_Sub, 3, "Local memory: Arg3 = Arg1 - Arg2");
  il.AddInst("Mult", Config::hardware_t::Inst_Mult, 3, "Local memory: Arg3 = Arg1 * Arg2");
  il.AddInst("Div", Config::hardware_t::Inst_Div, 3, "Local memory: Arg3 = Arg1 / Arg2");
  il.AddInst("Mod", Config::hardware_t::Inst_Mod, 3, "Local memory: Arg3 = Arg1 % Arg2");
  il.AddInst("TestEqu", Config::hardware_t::Inst_TestEqu, 3, "Local memory: Arg3 = (Arg1 == Arg2)");
  il.AddInst("TestNEqu", Config::hardware_t::Inst_TestNEqu, 3, "Local memory: Arg3 = (Arg1 != Arg2)");
  il.AddInst("TestLess", Config::hardware_t::Inst_TestLess, 3, "Local memory: Arg3 = (Arg1 < Arg2)");
  il.AddInst("If", Config::hardware_t::Inst_If, 1, "Local memory: If Arg1 != 0, proceed; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
  il.AddInst("While", Config::hardware_t::Inst_While, 1, "Local memory: If Arg1 != 0, loop; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
  il.AddInst("Countdown", Config::hardware_t::Inst_Countdown, 1, "Local memory: Countdown Arg1 to zero.", emp::ScopeType::BASIC, 0, {"block_def"});
  il.AddInst("Close", Config::hardware_t::Inst_Close, 0, "Close current block if there is a block to close.", emp::ScopeType::BASIC, 0, {"block_close"});
  il.AddInst("Break", Config::hardware_t::Inst_Break, 0, "Break out of current block.");
  il.AddInst("Return", Config::hardware_t::Inst_Return, 0, "Return from current function if possible.");
  il.AddInst("SetMem", Config::hardware_t::Inst_SetMem, 2, "Local memory: Arg1 = numerical value of Arg2");
  il.AddInst("CopyMem", Config::hardware_t::Inst_CopyMem, 2, "Local memory: Arg1 = Arg2");
  il.AddInst("SwapMem", Config::hardware_t::Inst_SwapMem, 2, "Local memory: Swap values of Arg1 and Arg2.");
  il.AddInst("Input", Config::hardware_t::Inst_Input, 2, "Input memory Arg1 => Local memory Arg2.");
  il.AddInst("Output", Config::hardware_t::Inst_Output, 2, "Local memory Arg1 => Output memory Arg2.");
  il.AddInst("Commit", Config::hardware_t::Inst_Commit, 2, "Local memory Arg1 => Shared memory Arg2.");
  il.AddInst("Pull", Config::hardware_t::Inst_Pull, 2, "Shared memory Arg1 => Shared memory Arg2.");
  il.AddInst("Terminate", Config::hardware_t::Inst_Terminate, 0, "Terminate current thread.");
  il.AddInst("Nop", Config::hardware_t::Inst_Nop, 0, "No operation.");
  il.AddInst("Rng", Config::hardware_t::Inst_RngDouble, 1, "Draw from onboard random number generator.");
  il.AddInst("SetOwnRegulator", Config::hardware_t::Inst_SetOwnRegulator, 1, "Sets the regulator the currently executing function.");
  il.AddInst("AdjOwnRegulator", Config::hardware_t::Inst_AdjOwnRegulator, 2, "Adjusts the regulator of the currently executing function towards a target.");
  il.AddInst("SenseOwnRegulator", Config::hardware_t::Inst_SenseOwnRegulator, 1, "Senses the regulator of the currently executing function.");

  il.AddInst(
    emp::to_string("Terminal", 64),
    Config::hardware_t::Inst_Terminal<std::ratio<64>, std::ratio<-64>>,
    1,
    "Writes a genetically-determined value into a register."
  );
  il.AddInst(
    emp::to_string("Terminal", 8),
    Config::hardware_t::Inst_Terminal<std::ratio<8>, std::ratio<-8>>,
    1,
    "Writes a genetically-determined value into a register."
  );
  il.AddInst(
    emp::to_string("Terminal", 1),
    Config::hardware_t::Inst_Terminal<std::ratio<1>, std::ratio<-1>>,
    1,
    "Writes a genetically-determined value into a register."
  );
  il.AddInst(
    emp::to_string("Terminal", 0.125),
    Config::hardware_t::Inst_Terminal<std::ratio<1,8>, std::ratio<-1,8>>,
    1,
    "Writes a genetically-determined value into a register."
  );
}

void LibraryInstruction::InitMatchBinRelated(inst_lib_t &il) {


  il.AddInst(
    "BcstMsgBig",
    [](hardware_t & hw, const inst_t & inst){
      state_t & state = hw.GetCurState();

      const double depo_amt = 1.0 + std::tanh(
         state.GetLocal(inst.args[0])
      );
      state.output_mem[
        std::numeric_limits<int>::max()
      ] = depo_amt;

      hw.TriggerEvent("BcstMsg", inst.affinity, state.output_mem);
    },
    1,
    "Broadcast a message.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "BcstMsgSmall",
    [](hardware_t & hw, const inst_t & inst){
      state_t & state = hw.GetCurState();

      const double depo_amt = 1.0 + std::tanh(
        state.GetLocal(inst.args[0]) - 0.5
      );
      state.output_mem[
        std::numeric_limits<int>::max()
      ] = depo_amt;

      hw.TriggerEvent("BcstMsg", inst.affinity, state.output_mem);
    },
    1,
    "Broadcast a message.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "SendMsgBig",
    [](hardware_t & hw, const inst_t & inst){
      state_t & state = hw.GetCurState();

      const double depo_amt = 1.0 + std::tanh(
         state.GetLocal(inst.args[0])
      );
      state.output_mem[
        std::numeric_limits<int>::max()
      ] = depo_amt;

      hw.TriggerEvent("SendMsg", inst.affinity, state.output_mem);
    },
    1,
    "Send a message to one random neighbor.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "SendMsgSmall",
    [](hardware_t & hw, const inst_t & inst){
      state_t & state = hw.GetCurState();

      const double depo_amt = 1.0 + std::tanh(
        state.GetLocal(inst.args[0]) - 0.5
      );
      state.output_mem[
        std::numeric_limits<int>::max()
      ] = depo_amt;

      hw.TriggerEvent("SendMsg", inst.affinity, state.output_mem);
    },
    1,
    "Send a message to one random neighbor.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "Call",
    [](hardware_t & hw, const inst_t & inst){

      hw.GetMatchBin().SetWhich(false);
      Config::hardware_t::Inst_Call(hw, inst);
      hw.GetMatchBin().SetWhich(true);

    },
    1,
    "Call function that best matches call affinity.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "ForkSmall",
    [](hardware_t & hw, const inst_t & inst){

      if constexpr (
        std::is_same<
          std::decay<decltype(hw.GetMatchBin().GetSelector())>::type,
          Config::DEPO_T
        >::value
      ) {

        const state_t & state = hw.GetCurState();
        const double depo_amt = 1.0 + std::tanh(
          state.GetLocal(inst.args[0]) - 0.5
        );
        hw.GetMatchBin().GetSelector().SetCurDePoAmt(depo_amt);

      }

      Config::hardware_t::Inst_Fork(hw, inst);
    },
    1,
    "Fork a new thread, using tag-based referencing to determine which function to call on the new thread.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "ForkBig",
    [](hardware_t & hw, const inst_t & inst){


      if constexpr (
        std::is_same<
          std::decay<decltype(hw.GetMatchBin().GetSelector())>::type,
          Config::DEPO_T
        >::value
      ) {

        const state_t & state = hw.GetCurState();
        const double depo_amt = 1.0 + std::tanh(
           state.GetLocal(inst.args[0])
        );

        hw.GetMatchBin().GetSelector().SetCurDePoAmt(depo_amt);

      }

      Config::hardware_t::Inst_Fork(hw, inst);
    },
    1,
    "Fork a new thread, using tag-based referencing to determine which function to call on the new thread.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "SetRegulator",
    [](hardware_t & hw, const inst_t & inst){

      hw.GetMatchBin().SetWhich(false);
      Config::hardware_t::Inst_SetRegulator(hw, inst);
      hw.GetMatchBin().SetWhich(true);

    },
    1,
    "Sets the regulator of a tag in the matchbin.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "AdjRegulator",
    [](hardware_t & hw, const inst_t & inst){

      hw.GetMatchBin().SetWhich(false);
      Config::hardware_t::Inst_AdjRegulator(hw, inst);
      hw.GetMatchBin().SetWhich(true);

    },
    1,
    "Adjusts the regulator of a tag in the matchbin.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "ExtRegulator",
    [](hardware_t & hw, const inst_t & inst){

      hw.GetMatchBin().SetWhich(false);
      Config::hardware_t::Inst_ExtRegulator(hw, inst);
      hw.GetMatchBin().SetWhich(true);

    },
    2,
    "extends the decay counter of a regulator of a tag in the matchbin.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

  il.AddInst(
    "SenseRegulator",
    [](hardware_t & hw, const inst_t & inst){

      hw.GetMatchBin().SetWhich(false);
      Config::hardware_t::Inst_SenseRegulator(hw, inst);
      hw.GetMatchBin().SetWhich(true);

    },
    1,
    "Senses the regulator of a tag in the matchbin.",
    emp::ScopeType::BASIC,
    0,
    {"affinity"}
  );

}

const inst_lib_t& LibraryInstruction::Make(const Config &cfg) {

  static inst_lib_t il;

  if (!il.GetSize()) {

    InitDefault(il);

    InitMatchBinRelated(il);

    il.AddInst(
      "Sense",
      [](hardware_t & hw, const inst_t & inst){
        FrameHardware &fh = *hw.GetTrait();
        state_t & state = hw.GetCurState();

        state.SetLocal(
          inst.args[0],
          fh.GetState()
        );

      },
      1,
      "TODO."
    );

    il.AddInst(
      "GuessTrue",
      [](hardware_t & hw, const inst_t & inst){
        FrameHardware &fh = *hw.GetTrait();
        const state_t & state = hw.GetCurState();

        fh.SetGuess(
          state.GetLocal(inst.args[0]) ? 0 : 1
        );

      },
      1,
      "TODO."
    );

    il.AddInst(
      "GuessFalse",
      [](hardware_t & hw, const inst_t & inst){
        FrameHardware &fh = *hw.GetTrait();
        const state_t & state = hw.GetCurState();

        fh.SetGuess(
          state.GetLocal(inst.args[0]) ? 1 : 0
        );

      },
      1,
      "TODO."
    );

    std::cout << "Instruction Library Size: " << il.GetSize() << std::endl;

  }

  emp_assert(il.GetSize());

  return il;

}
