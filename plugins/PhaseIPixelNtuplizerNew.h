#ifndef PHASEIPIXELNTUPLIZER_NEW_H
#define PHASEIPIXELNTUPLIZER_NEW_H

// CMSSW code
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
//#include "DataFormats/TrackReco/interface/Track.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"

// Datastructures - Keep all this in one file
// This has to be a versioned file
// It cannot go into separate files included from everywhere
#include "../interface/DataStructures_v1.h" // 2016 Nov 04, CMSSW_8_1_0_pre15

// Private codes
#include "../TrajAnalyzer/interface/TrajAnalyzer.h"
#include "../interface/PhaseITrackingEfficiencyFiltersNew.h"
#include "../interface/Console_colors.h"

// ROOT Libraries
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
// #include <TH1D.h>
// #include <TH2D.h>
#include <TRandom3.h>

// C++
#include <iostream>
#include <string>
#include <vector>
#include <map>

// Compiler directives
#define EDM_ML_LOGDEBUG
#define ML_DEBUG

class PhaseIPixelNtuplizerNew : public edm::EDAnalyzer
{
	private:

	public:
		PhaseIPixelNtuplizerNew(edm::ParameterSet const& iConfig);
		virtual ~PhaseIPixelNtuplizerNew();
		virtual void beginJob();
		virtual void endJob();
		virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
		virtual void beginRun(edm::Run const&, edm::EventSetup const&);
		virtual void endRun(edm::Run const&, edm::EventSetup const&);
		virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
	private:
		edm::ParameterSet iConfig_;
		// Options
		int clusterSaveDownscaling_;
		std::string ntupleOutputFilename_ = "Ntuple.root";
		TFile*      ntupleOutputFile_;
		// Trees
		TTree* eventTree_;
		TTree* lumiTree_;
		TTree* runTree_;
		TTree* digiTree_;
		TTree* clustTree_;
		TTree* trackTree_;
		TTree* trajTree_;
		// Tree field definitions are in the interface directory
		EventData       evt_;
		LumiData        lumi_;
		RunData         run_;
		Digi            digi_;
		Cluster         clu_;
		TrackData       track_;
		TrajMeasurement traj_;
		// Tokens
		edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>> rawDataErrorToken_;
		edm::EDGetTokenT<reco::VertexCollection>                 primaryVerticesToken_;
		edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster>>   clustersToken_;
		edm::EDGetTokenT<TrajTrackAssociationCollection>         trajTrackCollectionToken_;
		// Private methods
		void handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::string msg);
		void handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::vector<std::string> msg);
		void printEvtInfo(const std::string& streamType);
		void getModuleData(ModuleData&, bool, const TrackerGeometry* const, const DetId &, const TrackerTopology* const, const std::map<uint32_t, int> &);
};

#endif
