#
# Author: Adrien Hourlier
#
# date of creation..........: 03/07/2017
# date of last modification.: 03/09/2017
#
# This code runs a 3D track reconstruction using an A* algorithm
#

codename = "\033[97m[run_3DReco]\033[00m"
print codename,"IMPORTS"

import os, sys
import ROOT as rt
from ROOT import std
from larcv import larcv
from larlitecv import larlitecv
from larlite import larlite
from math import fabs
import csv


rt.gStyle.SetOptStat(0)

print  codename,"Read proton CSV file"
evtList=[]
with open('/Users/hourlier/Documents/PostDocMIT/Research/MicroBooNE/DeepLearning/DLwork/DataFiles/passedGBDT_extBNB_AnalysisTrees_cosmic_trained_only_on_mc_score_0.99.csv','r') as csvfile:
    reader = csv.DictReader(csvfile, delimiter=' ', lineterminator='\n')
    for row in reader :
        evtList.append([int(row['Run']), int(row['SubRun']), int(row['Event']), int(row['TrackID'])])



print codename,"Setup DataCoordiantor"

dataco = larlitecv.DataCoordinator()
#dataco.add_inputfile("/Users/hourlier/Documents/PostDocMIT/Research/MicroBooNE/DeepLearning/DLwork/DataFiles/larcv_extBNB9131runs_cosmic_trained_only_on_mc_pscore_0.99_1598evts_23aug2016.root","larcv")
dataco.add_inputfile("/Users/hourlier/Documents/PostDocMIT/Research/MicroBooNE/DeepLearning/DLwork/DataFiles/larlite_pandoraNu_20170211_205524_331124.root","larlite")
dataco.configure("config.cfg","StorageManager","IOManager","3DRecoConfigurationFile")
dataco.initialize()

NEntries=dataco.get_nentries("larlite")
print codename, NEntries, " available"

#NEntries = 10

c3views = rt.TCanvas("c3views","c3views",900,300)
c3views.Divide(3,1)
TrackU = rt.TGraph()
TrackU.SetNameTitle("U-plane","U-plane")
TrackV = rt.TGraph()
TrackV.SetNameTitle("V-plane","V-plane")
TrackY = rt.TGraph()
TrackY.SetNameTitle("Y-plane","Y-plane")
Tracks = []
Tracks.append(TrackU)
Tracks.append(TrackV)
Tracks.append(TrackY)

print codename,"Start loop over entries"
ientry = 0
while ientry < NEntries :
    dataco.goto_entry(ientry,"larlite")
    ientry = ientry+1
    if int(ientry*100.*17/(NEntries))%(10*17) == 0 : print "\t", codename, int(ientry*100./NEntries), "% done"
    Run = dataco.run()
    SubRun=dataco.subrun()
    Event=dataco.event()


    SelectedTrack=0
    correspondingTrack = False
    for row in evtList :
        if Run == row[0] and SubRun == row[1] and Event == row[2] :
            #print "	\t \t OK"
            correspondingTrack = True
            SelectedTrack = row[3]

    if correspondingTrack == False :
        print codename," ERROR : Event ", Run, SubRun, Event, " => no corresponding track found"
        continue

    larlite_storage = dataco.get_larlite_io()
    track_v    = dataco.get_larlite_data(larlite.data.kTrack,"pandoraNu")
    gaushit_v  = dataco.get_larlite_data(larlite.data.kHit,"gaushit")
    hit_v      = dataco.get_larlite_data(larlite.data.kHit, "pandoraCosmicKHitRemoval")


    ass_info = dataco.get_larlite_data(larlite.data.kAssociation,"pandoraNu")
    ass_info_v = ass_info.association(ass_info.find_one_assid(larlite.data.kTrack,larlite.data.kHit))

    for track_index in range (0, ass_info_v.size()) :
        one_track = track_v[track_index]
        hit_index_v = ass_info_v[track_index]
        if one_track.ID() != SelectedTrack : continue

        iHit = [0,0,0]
        Hit_v = [0,0,0]
        for iPlane in range (0,3):
            while Tracks[iPlane].GetN()>0:
                Tracks[iPlane].RemovePoint(0)

        for hit_index in hit_index_v:
            iPlane = hit_v[hit_index].WireID().Plane
            Tracks[iPlane].SetPoint(iHit[iPlane],hit_v[hit_index].WireID().Wire,hit_v[hit_index].PeakTime())
            iHit[iPlane] = iHit[iPlane]+1

        for iPlane in range (0,3):
            c3views.cd(iPlane+1)
            Tracks[iPlane].SetMarkerStyle(20)
            Tracks[iPlane].Draw("APL")

        PictureName = 'plot/Track_{}_{}_{}_{}.pdf'.format(Run,SubRun,Event,one_track.ID())
        c3views.SaveAs(PictureName)




dataco.finalize()
print codename,"END"
