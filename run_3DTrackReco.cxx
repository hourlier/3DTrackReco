//
// Author: Adrien Hourlier
//
// date of creation..........: 03/07/2017
// date of last modification.: 03/07/2017
//
// This code runs a 3D track reconstruction using an A* algorithm
//

#include <iostream>
#include <string>
#include <vector>

// config/storage: from LArCV
#include "Base/PSet.h"
#include "Base/LArCVBaseUtilFunc.h"
#include "Base/DataCoordinator.h"

// larlite data
#include "Base/DataFormatConstants.h"
#include "DataFormat/opflash.h" // example of data product

// larcv data
#include "DataFormat/EventImage2D.h"
#include "DataFormat/EventROI.h"
#include "DataFormat/EventPixel2D.h"

// ROOT includes
#include "TH2D.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TBox.h"

#include "GenerateImage2D.h"



int main( int nargs, char** argv ) {

    std::string codename = "\t \033[97m[run_3DTrackReco]\033[00m   ";

    std::cout << codename << std::endl;
    std::string FilePath = "/Users/hourlier/Documents/PostDocMIT/Research/MicroBooNE/DeepLearning/DLwork/DataFiles/passedGBDT_extBNB_AnalysisTrees_cosmic_trained_only_on_mc_score_0.99.csv";
    std::vector<std::vector<int> > info = larlite::ReadCSVFile(FilePath);
    //larlite::CheckFileContent(info);

    // configuration
    //larcv::PSet cfg = larcv::CreatePSetFromFile("config.cfg");
    //larcv::PSet PFconfig = cfg.get<larcv::PSet>("3DRecoConfigurationFile");

    // configure Data coordinator
    larlitecv::DataCoordinator dataco;
    std::string PATH = "/Users/hourlier/Documents/PostDocMIT/Research/MicroBooNE/DeepLearning/DLwork/DataFiles";
    dataco.add_inputfile(Form("%s/larlite_pandoraNu_20170211_205524_331124.root",PATH.c_str()), "larlite");
    //dataco.add_inputfile(Form("%s/larcv_extBNB9131runs_cosmic_trained_only_on_mc_pscore_0.99_1598evts_23aug2016.root",PATH.c_str()),"larcv");
    //dataco.add_inputfile(Form("%s/roi_extBNB9131runs_cosmic_trained_only_on_mc_pscore_0.99_1598evts_23aug2016.root",PATH.c_str()),"larcv");
    dataco.configure( "config.cfg", "StorageManager", "IOManager", "3DRecoConfigurationFile" );

    // initialize
    dataco.initialize();

    int nEntries = dataco.get_nentries("larlite");
    int Run, SubRun,Event,iTrack;
    std::cout << codename << nEntries << " entries available" << std::endl;

    for(int iEntry = 0;iEntry < nEntries ;iEntry++){
        dataco.goto_entry(iEntry,"larlite");
        if((int)(iEntry*100.*17/(nEntries))%(10*17) == 0){std::cout << codename << (int)(iEntry*100./nEntries) << " % done" << std::endl;}
        Run = dataco.run();
        SubRun=dataco.subrun();
        Event=dataco.event();

        auto const& larlite_storage = dataco.get_larlite_io();
        auto const& track_v    = dataco.get_larlite_data(larlite::data::kTrack,"pandoraNu");
        auto const& gaushit_v  = dataco.get_larlite_data(larlite::data::kHit,"gaushit");
        /*auto const& hit_v      = dataco.get_larlite_data(larlite::data::kHit, "pandoraCosmicKHitRemoval");
        auto const& ass_info   = dataco.get_larlite_data(larlite::data::kAssociation,"pandoraNu");

        auto const& ass_info_v = ass_info->association(ass_info.find_one_assid(larlite::data::kTrack,larlite::data::kHit));

        auto track_v   = dataco.get_larlite_data<larlite::event_track>("pandoraNu");
        auto gaushit_v = dataco.get_larlite_data<larlite::event_hit>("gaushit");*/

        larlite::event_hit* hit_v = nullptr; // B
        auto const& ass_info = larlite_storage.find_one_ass(track_v->id(),  hit_v, track_v->id().second);

        /*for(size_t track_index = 0;track_index < ass_info_v.size();track_index++){
            larlite::track one_track = track_v[track_index];
            std::vector<int> hit_index_v = ass_info_v[track_index];
            std::vector<int> track_info = {Run, SubRun,Event, one_track.ID()};
            if(!larlite::SelectTrack(track_info, info)){continue;}// only keeps proton tracks

            for(size_t ihit = 0; ihit < hit_index_v.size();ihit++){
                int hit_index = hit_index_v[ihit];
                int iPlane = hit_v[hit_index].WireID().Plane;
                // here, get the vector if selected hits ()
            }

        }*/

    }

    // finalize
    dataco.finalize();
    return 0;
}
