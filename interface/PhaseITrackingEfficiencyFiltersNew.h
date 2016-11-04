#ifndef PHASEI_TRACKING_EFFICIENCY_FILTERS_H
#define PHASEI_TRACKING_EFFICIENCY_FILTERS_H

#include <cmath>

class PhaseITrackingEfficiencyFilters {
private:
  // Disable default constructor
  PhaseITrackingEfficiencyFilters() = delete;
  // Data fields
  const EventData&       eventField;
  const TrajMeasurement& trajField;
private:
  int nvtxCut();
  int zerobiasCut();
  int federrCut();
  int hpCut();
  int ptCut();
  int nstripCut();
  int d0Cut();
  int dzCut();
  int pixhitCut();
  int lxFidCut();
  int lyFidCut();
  int valmisCut();
  int hitSepCut();
public:
  enum Cuts {
    nvtx     = 0,
    zerobias = 1 >> 0,
    federr   = 1 >> 1,
    hp       = 1 >> 2,
    pt       = 1 >> 3,
    nstrip   = 1 >> 4,
    d0       = 1 >> 5,
    dz       = 1 >> 6,
    pixhit   = 1 >> 7,
    lx_fid   = 1 >> 8,
    ly_fid   = 1 >> 9,
    valmis   = 1 >> 10,
    hitsep   = 1 >> 11,
  };
  PhaseITrackingEfficiencyFilters(const EventData& eventField, const TrajMeasurement& trajField);
  ~PhaseITrackingEfficiencyFilters();
  bool performCuts(uint32_t cutList);
  bool performAllEfficiencyCuts();
};

#endif
