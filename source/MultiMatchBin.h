#pragma once

#include "tools/MatchBin.h"

template<
  typename Val,
  typename Metric,
  typename SelectorA,
  typename SelectorB,
  typename Regulator
>
class MultiMatchBin : public emp::BaseMatchBin<
  Val,
  typename Metric::query_t,
  typename Metric::tag_t,
  Regulator
> {

using base_t = emp::BaseMatchBin<
  Val,
  typename Metric::query_t,
  typename Metric::tag_t,
  Regulator
>;

public:
  using query_t = typename base_t::query_t;
  using tag_t = typename base_t::tag_t;
  using uid_t = typename base_t::uid_t;
  using state_t = emp::MatchBinState<Val, tag_t, Regulator>;

protected:
  emp::MatchBin<
    Val,
    Metric,
    SelectorA,
    Regulator
  > mb_a;
  emp::MatchBin<
    Val,
    Metric,
    SelectorB,
    Regulator
  > mb_b;

  bool which = true;

public:

  MultiMatchBin(
    emp::Random & rand_
  ) : mb_a(rand_)
    , mb_b(rand_)
  { ; }

  void SetWhich(const bool set) { which = set; }

  /// Reset the Selector cache for regulated scores.
  void ClearRegulatedCache() {
    mb_a.ClearRegulatedCache();
    mb_b.ClearRegulatedCache();
  }

  /// Reset the Selector cache for raw scores.
  void ClearRawCache() {
    mb_a.ClearRawCache();
    mb_b.ClearRawCache();
  }

  SelectorA & GetSelector() {
    return mb_a.GetSelector();
  }

  /// Compare a query tag to all stored tags using the distance metric
  /// function and return a vector of unique IDs chosen by the selector
  /// function.
  /// Calling with n = 0 means delegate choice for how many values to return
  /// to the Selector.
  emp::vector<uid_t> Match(const query_t & query, size_t n=0) override {
    if (which) return mb_a.Match(query, n);
    else return mb_b.Match(query, n);
  }

  /// Compare a query tag to all stored tags using the distance metric
  /// function and return a vector of unique IDs chosen by the selector
  /// function. Ignore regulators.
  emp::vector<uid_t> MatchRaw(const query_t & query, size_t n=0) override {
    if (which) return mb_a.MatchRaw(query, n);
    else return mb_b.MatchRaw(query, n);
  }

  /// Put an item and associated tag in the container. Returns the uid for
  /// that entry.
  uid_t Put(const Val & v, const tag_t & t) override {
    const auto res = mb_a.Put(v, t);
    mb_b.Set(v, t, res);
    return res;
  }

  /// Put with a manually-chosen UID.
  /// (Caller is responsible for ensuring UID is unique
  /// or calling Delete beforehand.)
  uid_t Set(const Val & v, const tag_t & t, const uid_t uid) override {
    mb_a.Set(v, t, uid);
    mb_b.Set(v, t, uid);
    return uid;
  }


  /// Delete an item and its associated tag.
  void Delete(const uid_t uid) override {
    mb_a.Delete(uid);
    mb_b.Delete(uid);
  }

  /// Clear all items and tags.
  void Clear() override {
    mb_a.Clear();
    mb_b.Clear();
  }

  /// Reset the Selector caches.
  void ClearCache() override {
    mb_a.ClearCache();
    mb_b.ClearCache();
  }

  /// Attempt to activate result caching.
  /// @return true if caching activated
  bool ActivateCaching() override {
    return mb_a.ActivateCaching() || mb_b.ActivateCaching();
  }

  void DeactivateCaching() override {
    mb_a.DeactivateCaching();
    mb_b.DeactivateCaching();
  }

  /// Access a reference single stored value by uid.
  Val & GetVal(const uid_t uid) override {
    return mb_a.GetVal(uid);
  }

  /// Access a const reference to a single stored tag by uid.
  const tag_t & GetTag(const uid_t uid) const override {
    return mb_a.GetTag(uid);
  }

  /// Change the tag at a given uid and clear the cache.
  void SetTag(const uid_t uid, tag_t tag) override {
    mb_a.SetTag(uid, tag);
    mb_b.SetTag(uid, tag);
  }

  /// Generate a vector of values corresponding to a vector of uids.
  emp::vector<Val> GetVals(const emp::vector<uid_t> & uids) override {
    return mb_a.GetVals(uids);
  }

  /// Generate a vector of tags corresponding to a vector of uids.
  emp::vector<tag_t> GetTags(const emp::vector<uid_t> & uids) override {
    return mb_a.GetTags(uids);
  }

  /// Get the number of items stored in the container.
  size_t Size() const override {
    return mb_a.Size();
  }

  /// Adjust an item's regulator value.
  void AdjRegulator(
    const uid_t uid,
    const typename Regulator::adj_t & amt
  ) override {
    mb_a.AdjRegulator(uid, amt);
    mb_b.AdjRegulator(uid, amt);
  }

  /// Set an item's regulator value.
  void SetRegulator(
    const uid_t uid,
    const typename Regulator::set_t & set
  ) override {
    mb_a.SetRegulator(uid, set);
    mb_b.SetRegulator(uid, set);
  }

  /// Set an item's regulator value.
  void SetRegulator(
    const uid_t uid,
    const Regulator & set
  ) override {
    mb_a.SetRegulator(uid, set);
    mb_b.SetRegulator(uid, set);
  }

  /// View an item's regulator value.
  const typename Regulator::view_t & ViewRegulator(
    const uid_t uid
  ) const override {
    return mb_a.ViewRegulator(uid);
  }

  /// Get a regulator.
  const Regulator & GetRegulator(const uid_t uid) override {
    return mb_a.GetRegulator(uid);
  }

  /// Apply decay to a regulator.
  void DecayRegulator(const uid_t uid, const int steps) override {
    mb_a.DecayRegulator(uid, steps);
    mb_b.DecayRegulator(uid, steps);
  }

  /// Apply decay to all regulators.
  void DecayRegulators(const int steps=1) override {
    mb_a.DecayRegulators(steps);
    mb_b.DecayRegulators(steps);
  }

  /// Set up regulators to match target MatchBin
  void ImprintRegulators(
    const emp::BaseMatchBin<Val, query_t, tag_t, Regulator> & target
  ) override {
    mb_a.ImprintRegulators(target);
    mb_b.ImprintRegulators(target);
  }

  /// View UIDs associated with this MatchBin
  const emp::vector<uid_t>& ViewUIDs() const override {
    return mb_a.ViewUIDs();
  }

  /// Get selector, metric name
  std::string name() const override {
    Regulator reg{};
    return emp::to_string(
      "Dual ",
      mb_a.name(),
      " + ",
      mb_b.name()

    );
  }

  /// Extract MatchBin state
  const state_t & GetState() const { return mb_a.GetState(); }

  /// Load MatchBin state
  void SetState(const state_t & state_) {
    mb_a.SetState(state_);
    mb_b.SetState(state_);
  }

};
