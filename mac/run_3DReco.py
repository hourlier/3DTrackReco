#
# Author: Adrien Hourlier
#
# date of creation..........: 03/07/2017
# date of last modification.: 03/08/2017
#
# This code runs a 3D track reconstruction using an A* algorithm
#

print "[run_3DReco] IMPORTS"

import os, sys
import ROOT as rt
from ROOT import std
from larcv import larcv
from larlitecv import larlitecv
from larlite import larlite
from math import fabs
import csv


rt.gStyle.SetOptStat(0)

print "[run_3DReco] Read proton CSV file"
evtList=[]
with open('/Users/hourlier/Documents/PostDocMIT/Research/MicroBooNE/DeepLearning/DLwork/DataFiles/passedGBDT_extBNB_AnalysisTrees_cosmic_trained_only_on_mc_score_0.99.csv','r') as csvfile:
    reader = csv.DictReader(csvfile, delimiter=' ', lineterminator='\n')
    for row in reader :
        evtList.append([int(row['Run']), int(row['SubRun']), int(row['Event']), int(row['TrackID'])])



print "[run_3DReco] Setup DataCoordiantor"

dataco = larlitecv.DataCoordinator()
#dataco.add_inputfile("/Users/hourlier/Documents/PostDocMIT/Research/MicroBooNE/DeepLearning/DLwork/DataFiles/larcv_extBNB9131runs_cosmic_trained_only_on_mc_pscore_0.99_1598evts_23aug2016.root","larcv")
dataco.add_inputfile("/Users/hourlier/Documents/PostDocMIT/Research/MicroBooNE/DeepLearning/DLwork/DataFiles/larlite_pandoraNu_20170211_205524_331124.root","larlite")
dataco.configure("config.cfg","StorageManager","IOManager","3DRecoConfigurationFile")
dataco.initialize()

NEntries=dataco.get_nentries("larlite")
print "[run_3DReco] ", NEntries, " available"

Run=int(0)
SubRun=int(0)
Event=int(0)

print "[run_3DReco] Start loop over entries"
ientry = 0
while ientry < NEntries :
    dataco.goto_entry(ientry,"larlite")
    ientry = ientry+1
    Run = dataco.run()
    SubRun=dataco.subrun()
    Event=dataco.event()
    print Run, SubRun, Event

    SelectedTrack=0
    correspondingTrack = False
    for row in evtList :
        if Run == row[0] and SubRun == row[1] and Event == row[2] :
            #print "	\t \t OK"
            correspondingTrack = True
            SelectedTrack = row[3]

    if correspondingTrack == False :
        print "[run_3DReco] ERROR : Event ", Run, SubRun, Event, " => no corresponding track found"
        continue

    larlite_storage = dataco.get_larlite_io()
    track_v    = dataco.get_larlite_data(larlite.data.kTrack,"pandoraNu")
    gaushit_v  = dataco.get_larlite_data(larlite.data.kHit,"gaushit")

#   access association between tracks and hit
    hit_v = []
#ass_info = larlite_storage.find_one_ass(track_v.id(), hit_v, track_v.id().second)
    ass_info = larlite_storage.find_one_assid(track_v.id(),larlite.data.kHit,"pandoraNu");
    print hit_v.size() , "hits associated"

dataco.finalize()
print "[run_3DReco] END"
