#pragma once

#include "base/Ptr.h"

#include "Config.h"
#include "FrameHardware.h"

class LibraryEvent {

public:

  using hardware_t = Config::hardware_t;
  using event_lib_t = Config::event_lib_t;
  using event_t = event_lib_t::event_t;

  static const event_lib_t& Make(const Config &cfg) {

    static event_lib_t el;

    if (el.GetSize() == 0) {

      el.AddEvent(
        "BcstMsg",
        [](hardware_t & hw, const event_t & event){

          if constexpr (
            std::is_same<
              std::decay<decltype(hw.GetMatchBin().GetSelector())>::type,
              Config::DEPO_T
            >::value
          ) {

            const double depo_amt = event.msg.at(
              std::numeric_limits<int>::max()
            );
            hw.GetMatchBin().GetSelector().SetCurDePoAmt(depo_amt);

          }

          hw.SpawnCore(event.affinity, hw.GetMinBindThresh(), event.msg);
        },
        "Send message event."
      );

      el.RegisterDispatchFun(
        "BcstMsg",
        [](hardware_t & hw, const event_t & event) {
          FrameHardware &fh = *hw.GetTrait();
          for (auto & fh_neigh : fh.GetNeighs()) {
            fh_neigh.get().TakeMessage(event);
          }

        }
      );

      el.AddEvent(
        "SendMsg",
        [](hardware_t & hw, const event_t & event){

          if constexpr (
            std::is_same<
              std::decay<decltype(hw.GetMatchBin().GetSelector())>::type,
              Config::DEPO_T
            >::value
          ) {

            const double depo_amt = event.msg.at(
              std::numeric_limits<int>::max()
            );
            hw.GetMatchBin().GetSelector().SetCurDePoAmt(depo_amt);

          }

          hw.SpawnCore(event.affinity, hw.GetMinBindThresh(), event.msg);
        },
        "Send message event."
      );

      el.RegisterDispatchFun(
        "SendMsg",
        [](hardware_t & hw, const event_t & event) {
          FrameHardware &fh = *hw.GetTrait();

          auto & neighs = fh.GetNeighs();

          neighs[
            hw.GetRandom().GetUInt(neighs.size())
          ].get().TakeMessage(event);

        }
      );

      std::cout << "Event Library Size: " << el.GetSize() << std::endl;

    }

    return el;

  }

};
