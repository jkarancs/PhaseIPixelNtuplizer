#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H
/*
  Creator:         Janos Karancsi, Adam Hunyadi, Viktor Veszpremi

  Description:     Data containers for Phase I Pixel detector

  Comments:        Please, make a new version of this file after each major revision (to be able to track ntuple content)
  e.g:
  git add src/DataStructures_v3.h

  Please, update the README.md file too (With tested working recipe, stating the working CMSSW version, and ntuple version)
  Please, also tag corresponding working PhaseIPixelNtuplizer version
  If you only modify the PhaseIPixelNtuplizer, increase the subversion number in the tag, i.e: v1.1

  How to do it:
  - Inside PhaseIPixelNtuplizer.h
  #include "../interface/DataStructures_v2.h"

  cd DPGAnalysis/PhaseIPixelNtuplizer
  git add interface/DataStructures_v2.h
  git commit -am "Update: v2.0 Phase I ntuple (CMSSW_8_1_0), Description: Switched to using SiPixelCoordinates (new class)"
  git tag -a PhaseI_v2.0_CMSSW_8_1_0 -m "V2 version of Phase I ntuples, tested to work with CMSSW_8_1_0"
  git push origin
  git push origin --tags

  Version history:
  - V1.X - 2016/Nov/04 - First working version, containing most of the required event content for Phase I
  - V2.X - 2016/Dec/16 - Added new geometry variables and implemented them in a new class, SiPixelCoordinates (soon to be added to DQM)
*/


#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

#include <TROOT.h>
#include <TTree.h>

#include <map>
#include <string>
#include <utility>
#include <sstream>
#include <iostream>


class RunData
{
public:
  int fill;
  int run;

  const std::string list = "fill/I:run";

  RunData() { init(); }

  void init() {
    fill = NOVAL_I;
    run  = NOVAL_I;
  }

};

class LumiData
{
public:

  int          fill;
  int          run;
  int          ls;
  unsigned int time; // Unix time - seconds starting from 1970 Jan 01 00:00
  float        pileup;

  const std::string list = 
    "fill/I:run:ls:time/i:pileup/F";

  LumiData() {
    init();
  }

  void init() {
    fill         = NOVAL_I;
    run          = NOVAL_I;
    ls           = NOVAL_I;
    time         = abs(NOVAL_I);
    pileup       = NOVAL_F;
  }

};

class EventData
{
public:

  int          fill;
  int          run;
  int          ls;
  int          orb;
  int          bx;
  int          evt;
  int          nvtx;
  int          trig;
  int          nclu[7]; // [0-3]: layer 1-4, [4-6]: disk 1-3]
  int          npix[7]; // [0-3]: layer 1-4, [4-6]: disk 1-3]
  unsigned int time;
  float        pileup;
  float        vtxndof;
  float        vtxchi2;
  float        vtxD0;
  float        vtxX;
  float        vtxY;
  float        vtxZ;
  int          vtxntrk;
  int          good;
  float        tmuon;
  float        tmuon_err;
  float        tecal;
  float        tecal_raw;
  float        tecal_err;
  float        field;
  int          wbc;
  int          delay;
  int          ntracks;
  int          ntrackFPix[3]; // tracks crossing the pixels
  int          ntrackBPix[4]; // tracks crossing the pixels
  int          ntrackFPixvalid[3]; // tracks crossing the pixels with valid hits
  int          ntrackBPixvalid[4]; // tracks crossing the pixels with valid hits
  float        trackSep;
  int          federrs_size;
  // must be the last variable of the object saved to TTree:
  int          federrs[16][2]; // [error index] [0:Nerror, 1:errorType]

  const std::string list =
    "fill/I:run:ls:orb:bx:evt:nvtx:trig:nclu[7]:npix[7]:time/i:pileup/F:vtxndof:vtxchi2:"
    "vtxD0:vtxX:vtxY:vtxZ:vtxntrk/I:good:tmuon/F:tmuon_err:tecal:tecal_raw:"
    "tecal_err:field:wbc/I:delay:ntracks:ntrackFPix[3]:ntrackBPix[4]:"
    "ntrackFPixvalid[3]:ntrackBPixvalid[4]:trackSep/F:federrs_size/I:"
    "federrs[federrs_size][2]";

  EventData() { init(); }

  void init() {
    fill = NOVAL_I;
    run  = NOVAL_I;
    ls   = NOVAL_I;
    orb  = NOVAL_I;
    bx   = NOVAL_I;
    evt  = NOVAL_I;
    nvtx = NOVAL_I;
    trig = NOVAL_I;
    std::fill(nclu, nclu + 7, NOVAL_I);
    std::fill(npix, npix + 7, NOVAL_I);
    time         = abs(NOVAL_I);
    pileup       = NOVAL_F;
    vtxndof      = NOVAL_F;
    vtxD0        = NOVAL_F;
    vtxZ         = NOVAL_F;
    vtxX         = NOVAL_F;
    vtxY         = NOVAL_F;
    vtxchi2      = NOVAL_F;
    vtxntrk      = NOVAL_I;
    good         = NOVAL_I;
    tmuon        = NOVAL_F;
    tmuon_err    = NOVAL_F;
    tecal        = NOVAL_F;
    tecal_raw    = NOVAL_F;
    tecal_err    = NOVAL_F;
    field        = NOVAL_F;
    wbc          = NOVAL_I;
    delay        = NOVAL_I;
    ntracks      = NOVAL_I;
    std::fill(ntrackBPix,      ntrackBPix      + 4, NOVAL_I);
    std::fill(ntrackBPixvalid, ntrackBPixvalid + 4, NOVAL_I);
    std::fill(ntrackFPix,      ntrackFPix      + 3, NOVAL_I);
    std::fill(ntrackFPixvalid, ntrackFPixvalid + 3, NOVAL_I);
    trackSep     = NOVAL_F;
    federrs_size = 0;
    for(size_t i = 0; i < 16; i++) {
      std::fill(federrs[i], federrs[i] + 2, NOVAL_I);
    } 
  }

};

class ModuleData
{
public:
  int          det;
  int          shl;
  int          side;
  int          module;

  int          layer;
  int          sec;
  int          ladder;
  int          flipped;
  int          half;

  int          disk;
  int          blade;
  int          panel;
  int          ring;

  int          federr;

  unsigned int rawid;
  unsigned int fedid;
  unsigned int channel;
  unsigned int roc;

  float        module_coord;
  float        ladder_coord;
  float        disk_coord;
  float        disk_ring_coord;
  float        blade_panel_coord;

  const std::string list = "det/I:shl:side:module:layer:sec:ladder:flipped:half:"
    "disk:blade:panel:ring:federr:rawid/i:fedid:channel:roc:"
    "module_coord/F:ladder_coord:disk_coord:disk_ring_coord:blade_panel_coord";

  const std::map<int, std::string> federrortypes = {
    std::make_pair<int, std::string>(25, "invalidROC"),
    std::make_pair<int, std::string>(26, "gap word"),
    std::make_pair<int, std::string>(27, "dummy word"),
    std::make_pair<int, std::string>(28, "FIFO full error"),
    std::make_pair<int, std::string>(29, "timeout error"),
    std::make_pair<int, std::string>(30, "TBM error trailer"),
    std::make_pair<int, std::string>(31, "event number error (TBM and FED event number mismatch)"),
    std::make_pair<int, std::string>(32, "incorrectly formatted Slink Header"),
    std::make_pair<int, std::string>(33, "incorrectly formatted Slink Trailer"),
    std::make_pair<int, std::string>(34, "the event size encoded in the Slink Trailer is" \
				     " different than the size found at raw to digi conversion "),
    std::make_pair<int, std::string>(35, "invalid FED channel number"),
    std::make_pair<int, std::string>(36, "invalid ROC value "),
    std::make_pair<int, std::string>(37, "invalid dcol or pixel value "),
    std::make_pair<int, std::string>(38, "the pixels on a ROC weren't read out from lowest" \
				     " to highest row and dcol value"),
    std::make_pair<int, std::string>(39, "CRC error")
  };

  ModuleData() { init(); }

  void init() {
    det     = NOVAL_I;
    shl     = NOVAL_I;
    side    = NOVAL_I;
    module  = NOVAL_I;
    layer   = NOVAL_I;
    sec     = NOVAL_I;
    ladder  = NOVAL_I;
    flipped = NOVAL_I;
    half    = NOVAL_I;
    disk    = NOVAL_I;
    blade   = NOVAL_I;
    panel   = NOVAL_I;
    ring    = NOVAL_I;
    federr  = NOVAL_I;
    rawid   = std::abs(NOVAL_I);
    fedid   = std::abs(NOVAL_I);
    channel = std::abs(NOVAL_I);
    roc     = std::abs(NOVAL_I);
    module_coord      = NOVAL_F;
    ladder_coord      = NOVAL_F;
    disk_ring_coord   = NOVAL_F;
    blade_panel_coord = NOVAL_F;
  }

  std::string federr_name() {
    auto found = federrortypes.find(federr);
    if(found != federrortypes.end()) return found -> second;
    return "FED error not interpreted";
  }

};

class DigiData
{
public:

  int i; // serial num of digi in the given module
  int row;
  int col;
  int adc;

  const std::string list = "i/I:row:col:adc";

  DigiData() { init(); }
  void init() {
    i    = NOVAL_I;
    row  = NOVAL_I;
    col  = NOVAL_I;
    adc  = NOVAL_I;
  }

};

class Digi : public DigiData
{
public:

  ModuleData mod_on; // online module number
  ModuleData mod;    // offline module number

  Digi() { init(); }

  void init() {
    DigiData::init();
    mod_on.init();
    mod.init();
  }

};

class ClustData
{
public:

  // Paired branches (SPLIT mode)
  float x;
  float y;
  float lx;
  float ly;
  float glx;
  float gly;
  float glz;
  int   sizeX;
  int   sizeY;
  int   i; // serial num of cluster in the given module
  int   edge;     // set if there is a valid hit
  int   badpix;   // set if there is a valid hit
  int   tworoc;   // set if there is a valid hit
  int   size;
  float charge;
  // adc must be the last variable of the branch
  float adc[1000];
  float pix[1000][2];

  const std::string list =
    "x/F:y:lx:ly:glx:gly:glz:sizeX/I:sizeY:i:edge:badpix:tworoc:size:charge/F";

  ClustData() { init(); }

  void init() {
    x      = NOVAL_F;
    y      = NOVAL_F;
    sizeX  = NOVAL_I;
    sizeY  = NOVAL_I;
    i      = NOVAL_I;
    edge   = NOVAL_I;
    badpix = NOVAL_I;
    tworoc = NOVAL_I;
    size   = 0;
    charge = NOVAL_F;
    std::fill(adc, adc + 1000, NOVAL_F);
    for(size_t j = 0; j < 1000; j++) {
      std::fill(pix[j], pix[j] + 2, NOVAL_F);
    }
  }

};

class Cluster : public ClustData
{
public:
  ModuleData mod_on; // online module number
  ModuleData mod;    // offline module number

  Cluster() { init(); }

  void init() {
    ClustData::init();
    mod_on.init();
    mod.init();
  }

};

class TrackData
{
public:

  // Non-split mode from here - keep order of variables
  float pt;
  float eta;
  float phi;
  float theta;
  float p;
  float d0;
  float dz;
  int   fromVtxNtrk;
  int   strip; // total valid hits on the strip detector parts
  int   quality;
  int   i;
  int   pix; // total valid hits on the pixel detector parts
  int   pixhit[2]; // 0: top ,1: bottom
  int   validpixhit[2]; // 0: top, 1: bottom
  int   fpix[3]; // recHits in Diks1,2,3
  int   bpix[4]; // recHits in Layer 1,2,3,4
  int   validfpix[3]; // valid recHits in Diks1,2,3
  int   validbpix[4]; // valid recHits in Layer 1,2,3,4
  // muon stuff
  int   muon_global;
  int   muon_intime;
  int   muon_id_soft;
  int   muon_id_loose;
  int   muon_id_medium;
  int   muon_id_tight;
  int   muon_id_mva_soft;
  int   muon_id_mva;
  int   muon_id_mva_lowpt;
  int   muon_id_trigger;
  int   muon_iso_pf;
  int   muon_iso_tk;
  int   muon_iso_mini;
  int   muon_iso_multi;
  int   muon_iso_puppi;
  int   muon_ip_2d;
  //int   muon_ip_3dsig;
  int   muon_time_comb_dir;
  int   muon_time_comb_ndof;
  int   muon_time_rpc_dir;
  int   muon_time_rpc_ndof;
  float muon_time_comb;
  float muon_time_rpc;

  const std::string list = "pt/F:eta:phi:theta:p:d0:dz:fromVtxNtrk/I:strip:quality:"
    "i:pix:pixhit[2]:validpixhit[2]:fpix[3]:bpix[4]:validfpix[3]:validbpix[4]:"
    "muon_global:muon_intime:muon_id_soft:muon_id_loose:muon_id_medium:muon_id_tight:"
    "muon_id_mva_soft:muon_id_mva:muon_id_mva_lowpt:muon_id_trigger:muon_iso_pf:"
    "muon_iso_tk:muon_iso_mini:muon_iso_multi:muon_iso_puppi:muon_ip_2d:"//muon_ip_3dsig:"
    "muon_time_comb_dir:muon_time_comb_ndof:muon_time_rpc_dir:muon_time_rpc_ndof:"
    "muon_time_comb/F:muon_time_rpc";

  TrackData() { init(); }

  TrackData(const TrackData& other) {
    pt             = other.pt;
    eta            = other.eta;
    phi            = other.phi;
    theta          = other.theta;
    p              = other.p;
    d0             = other.d0;
    dz             = other.dz;
    fromVtxNtrk    = other.fromVtxNtrk;
    strip          = other.strip;
    quality        = other.quality;
    i              = other.i;
    pix            = other.pix;
    std::copy(other.pixhit,      other.pixhit      + 2, pixhit);
    std::copy(other.validpixhit, other.validpixhit + 2, validpixhit);
    std::copy(other.fpix,        other.fpix        + 3, fpix);
    std::copy(other.bpix,        other.bpix        + 4, bpix);
    std::copy(other.validfpix,   other.validfpix   + 3, validfpix);
    std::copy(other.validbpix,   other.validbpix   + 4, validbpix);
    muon_global 	= other.muon_global;
    muon_intime 	= other.muon_intime;
    muon_id_soft 	= other.muon_id_soft;
    muon_id_loose 	= other.muon_id_loose;
    muon_id_medium 	= other.muon_id_medium;
    muon_id_tight 	= other.muon_id_tight;
    muon_id_mva_soft 	= other.muon_id_mva_soft;
    muon_id_mva 	= other.muon_id_mva;
    muon_id_mva_lowpt 	= other.muon_id_mva_lowpt;
    muon_id_trigger 	= other.muon_id_trigger;
    muon_iso_pf 	= other.muon_iso_pf;
    muon_iso_tk 	= other.muon_iso_tk;
    muon_iso_mini 	= other.muon_iso_mini;
    muon_iso_multi 	= other.muon_iso_multi;
    muon_iso_puppi 	= other.muon_iso_puppi;
    muon_ip_2d 		= other.muon_ip_2d;
    //muon_ip_3dsig 	= other.muon_ip_3dsig;
    muon_time_comb_dir 	= other.muon_time_comb_dir;
    muon_time_comb_ndof = other.muon_time_comb_ndof;
    muon_time_rpc_dir 	= other.muon_time_rpc_dir;
    muon_time_rpc_ndof 	= other.muon_time_rpc_ndof;
    muon_time_comb 	= other.muon_time_comb;
    muon_time_rpc       = other.muon_time_rpc;      
  }

  TrackData(TrackData&& other) {
    swap(*this, other);
  }

  friend void swap(TrackData& first, TrackData& second) {
    using std::swap;
    swap(first.pt,                  second.pt);
    swap(first.eta,                 second.eta);
    swap(first.phi,                 second.phi);
    swap(first.theta,               second.theta);
    swap(first.p,                   second.p);
    swap(first.d0,                  second.d0);
    swap(first.dz,                  second.dz);
    swap(first.fromVtxNtrk,         second.fromVtxNtrk);
    swap(first.strip,               second.strip);
    swap(first.quality,             second.quality);
    swap(first.i,                   second.i);
    swap(first.pix,                 second.pix);
    swap(first.pixhit,              second.pixhit);
    swap(first.validpixhit,         second.validpixhit);
    swap(first.fpix,                second.fpix);
    swap(first.bpix,                second.bpix);
    swap(first.validfpix,           second.validfpix);
    swap(first.validbpix,           second.validbpix);
    swap(first.muon_global,         second.muon_global);
    swap(first.muon_intime,         second.muon_intime);
    swap(first.muon_id_soft,        second.muon_id_soft);
    swap(first.muon_id_loose,       second.muon_id_loose);
    swap(first.muon_id_medium,      second.muon_id_medium);
    swap(first.muon_id_tight,       second.muon_id_tight);
    swap(first.muon_id_mva_soft,    second.muon_id_mva_soft);
    swap(first.muon_id_mva,         second.muon_id_mva);
    swap(first.muon_id_mva_lowpt,   second.muon_id_mva_lowpt);
    swap(first.muon_id_trigger,     second.muon_id_trigger);
    swap(first.muon_iso_pf,         second.muon_iso_pf);
    swap(first.muon_iso_tk,         second.muon_iso_tk);
    swap(first.muon_iso_mini,       second.muon_iso_mini);
    swap(first.muon_iso_multi,      second.muon_iso_multi);
    swap(first.muon_iso_puppi,      second.muon_iso_puppi);
    swap(first.muon_ip_2d,          second.muon_ip_2d);
    //swap(first.muon_ip_3dsig,       second.muon_ip_3dsig);
    swap(first.muon_time_comb_dir,  second.muon_time_comb_dir);
    swap(first.muon_time_comb_ndof, second.muon_time_comb_ndof);
    swap(first.muon_time_rpc_dir,   second.muon_time_rpc_dir);
    swap(first.muon_time_rpc_ndof,  second.muon_time_rpc_ndof);
    swap(first.muon_time_comb,      second.muon_time_comb);
    swap(first.muon_time_rpc,       second.muon_time_rpc);  
  }

  TrackData& operator =(TrackData other) noexcept {
    swap(*this, other);
    return *this;
  }

  void init() {
    pt             = NOVAL_F;
    eta            = NOVAL_F;
    phi            = NOVAL_F;
    theta          = NOVAL_F;
    p              = NOVAL_F;
    d0             = NOVAL_F;
    dz             = NOVAL_F;
    fromVtxNtrk    = NOVAL_I;
    strip          = NOVAL_I;
    quality        = NOVAL_I;
    i              = NOVAL_I;
    pix            = NOVAL_I;
    std::fill(pixhit,      pixhit      + 2, NOVAL_I);
    std::fill(validpixhit, validpixhit + 2, NOVAL_I);
    std::fill(fpix,        fpix        + 3, NOVAL_I);
    std::fill(bpix,        bpix        + 4, NOVAL_I);
    std::fill(validfpix,   validfpix   + 3, NOVAL_I);
    std::fill(validbpix,   validbpix   + 4, NOVAL_I);
    muon_global         = NOVAL_I;
    muon_intime         = NOVAL_I;
    muon_id_soft        = NOVAL_I;
    muon_id_loose       = NOVAL_I;
    muon_id_medium      = NOVAL_I;
    muon_id_tight       = NOVAL_I;
    muon_id_mva_soft    = NOVAL_I;
    muon_id_mva         = NOVAL_I;
    muon_id_mva_lowpt   = NOVAL_I;
    muon_id_trigger     = NOVAL_I;
    muon_iso_pf         = NOVAL_I;
    muon_iso_tk         = NOVAL_I;
    muon_iso_mini       = NOVAL_I;
    muon_iso_multi      = NOVAL_I;
    muon_iso_puppi      = NOVAL_I;
    muon_ip_2d          = NOVAL_I;
    //muon_ip_3dsig       = NOVAL_I;
    muon_time_comb_dir  = NOVAL_I;
    muon_time_comb_ndof = NOVAL_I;
    muon_time_rpc_dir   = NOVAL_I;
    muon_time_rpc_ndof  = NOVAL_I;
    muon_time_comb      = NOVAL_F;
    muon_time_rpc       = NOVAL_F;
  }

};

class TrajMeasData
{
public:

  // Non-split mode from here - keep order of variables
  int   validhit;
  int   missing;
  int   inactive;
  float lx;
  float ly;
  float lz;
  float glx;
  float gly;
  float glz;
  // float res_dx;
  // float res_dz;
  // float lev;
  int  clust_near;
  int  hit_near;
  int  pass_effcuts;
  // int   nclu_mod;
  // int   nclu_roc;
  // int   npix_mod;
  // int   npix_roc;
  float alpha;
  float beta;
  float norm_charge;
  float d_tr;
  float dx_tr;
  float dy_tr;
  float d_cl;
  float dx_cl;
  float dy_cl;
  float dx_hit;
  float dy_hit;
  float dr_trk;
  float dr_simtrk;
  float dx_simhit;
  float dy_simhit;
  // float lxmatch;
  // float lymatch;
  // int   i; // serial num of trajectory measurement on the (single) track of the event
  int   onedge;
  // int   badhit;
  // int   telescope;
  // int   telescope_valid;
  // int   dmodule; // D(propagated hit, valid hit)
  // int   dladder; // D(propagated hit, valid hit)
  // float glmatch;
  float lx_err;
  float ly_err;
  // float lz_err;
  // float lxymatch;
  // float res_hit;
  // float sig_hit;
  float d_cl2;
  float dx_cl2;
  float dy_cl2;

  const std::string list = 
    "validhit/I:missing:inactive:lx/F:ly:lz:glx:gly:glz:clust_near/I"
    ":hit_near:pass_effcuts:alpha/F"
    ":beta:norm_charge:d_tr:dx_tr:dy_tr:d_cl:dx_cl:dy_cl:dx_hit:dy_hit"
    ":dr_trk:dr_simtrk:dx_simhit:dy_simhit:onedge/I:lx_err/F:ly_err/F"
    ":d_cl2:dx_cl2:dy_cl2";

  TrajMeasData() { init(); }

  void init() {
    validhit        = NOVAL_I;
    missing         = NOVAL_I;
    inactive        = NOVAL_I;
    lx              = NOVAL_F;
    ly              = NOVAL_F;
    lz              = NOVAL_F;
    glx             = NOVAL_F;
    gly             = NOVAL_F;
    glz             = NOVAL_F;
    // res_dx          = NOVAL_F;
    // res_dz          = NOVAL_F;
    // lev             = NOVAL_F;
    clust_near      = NOVAL_I;
    hit_near        = NOVAL_I;
    pass_effcuts    = NOVAL_I;
    // nclu_mod        = NOVAL_I;
    // nclu_roc        = NOVAL_I;
    // npix_mod        = NOVAL_I;
    // npix_roc        = NOVAL_I;
    alpha           = NOVAL_F;
    beta            = NOVAL_F;
    norm_charge     = NOVAL_F;
    d_tr            = NOVAL_F;
    dx_tr           = NOVAL_F;
    dy_tr           = NOVAL_F;
    d_cl            = NOVAL_F;
    dx_cl           = NOVAL_F;
    dy_cl           = NOVAL_F;
    dx_hit          = NOVAL_F;
    dy_hit          = NOVAL_F;
    dx_simhit       = NOVAL_F;
    dy_simhit       = NOVAL_F;
    // lxmatch         = NOVAL_F;
    // lymatch         = NOVAL_F;
    // i               = NOVAL_I;
    onedge          = NOVAL_I;
    // badhit          = NOVAL_I;
    // telescope       = NOVAL_I;
    // telescope_valid = NOVAL_I;
    // dmodule         = NOVAL_I;
    // dladder         = NOVAL_I;
    // glmatch         = NOVAL_F;
    lx_err          = NOVAL_F;
    ly_err          = NOVAL_F;
    // lz_err          = NOVAL_F;
    // lxymatch        = NOVAL_F;
    // res_hit         = NOVAL_F;
    // sig_hit         = NOVAL_F;
    d_cl2           = NOVAL_F;
    dx_cl2          = NOVAL_F;
    dy_cl2          = NOVAL_F;
  }

};

class TrajMeasurement : public TrajMeasData
{
public:

  ModuleData mod_on; // online module number
  ModuleData mod;    // offline module number
  ClustData clu;
  TrackData trk;

  TrajMeasurement() { init(); }

  void init() {
    TrajMeasData::init();
    mod_on.init();
    mod.init();
    clu.init();
    trk.init();
  }

};

class TrajROCEfficiency
{
  public:
    float ROCEfficiency;
    float halfModuleEfficiency;
    float moduleEfficiency;
    const std::string list = "ROCEfficiency/F:halfModuleEfficiency/F:moduleEfficiency/F";
    TrajROCEfficiency() { init(); }
    void init()
    {
      ROCEfficiency        = NOVAL_F;
      halfModuleEfficiency = NOVAL_F;
      moduleEfficiency     = NOVAL_F;
    }
};

#endif
