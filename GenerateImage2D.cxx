#ifndef LARLITE_GENERATEIMAGE2D_CXX
#define LARLITE_GENERATEIMAGE2D_CXX

#include "GenerateImage2D.h"

namespace larlite{

    std::vector< std::vector<int> > ReadCSVFile(std::string FilePath){
        //std::cout << "FilePath : " << FilePath << std::endl;
        std::vector< std::vector<int> > info;
        std::vector<int> eventInfo;

        std::ifstream  file(FilePath);
        if(!file){std::cout << "ERROR, could not open file"<<std::endl;return info;}
        int Run, SubRun, Event, TrackID, WireUMin, TimeUMin, WireUMax, TimeUMax, WireVMin, TimeVMin, WireVMax, TimeVMax, WireYMin, TimeYMin, WireYMax, TimeYMax;
        //char coma;
        double protonBDTscore;
        bool GoOn = true;
        std::string line;
        getline(file, line);
        //std::cout << line << std::endl;
        while(GoOn){
            file >> Run >> SubRun >> Event >> TrackID >> WireUMin >> TimeUMin >> WireUMax >> TimeUMax >>  WireVMin >> TimeVMin >> WireVMax >> TimeVMax >> WireYMin >> TimeYMin >> WireYMax >> TimeYMax >> protonBDTscore;

            if(file.eof())GoOn = false;
            if(!GoOn){break;}

            //std::cout << Run << "\t" << SubRun << "\t" << Event << "\t" << TrackID << "\t" << WireUMin << "\t" << TimeUMin << "\t" << WireUMax << "\t" << TimeUMax << "\t" << WireVMin << "\t" << TimeVMin << "\t" << WireVMax << "\t" << TimeVMax << "\t" << WireYMin << "\t" << TimeYMin << "\t" << WireYMax << "\t" << TimeYMax << "\t" << protonBDTscore << std::endl;

            eventInfo.clear();
            eventInfo.push_back(Run);
            eventInfo.push_back(SubRun);
            eventInfo.push_back(Event);
            eventInfo.push_back(TrackID);
            eventInfo.push_back(WireUMin);
            eventInfo.push_back(TimeUMin);
            eventInfo.push_back(WireUMax);
            eventInfo.push_back(TimeUMax);
            eventInfo.push_back(WireVMin);
            eventInfo.push_back(TimeVMin);
            eventInfo.push_back(WireVMax);
            eventInfo.push_back(TimeVMax);
            eventInfo.push_back(WireYMin);
            eventInfo.push_back(TimeYMin);
            eventInfo.push_back(WireYMax);
            eventInfo.push_back(TimeYMax);

            info.push_back(eventInfo);
        }

        return info;
    }
    /*******************************************************************************/
    void CheckFileContent(const std::vector<std::vector<int> > info){
        for(size_t i = 0; i<info.size();i++){
            for(size_t j =0;j<info[i].size();j++){
                std::cout << info[i][j] << "\t";
            }
            std::cout << std::endl;
        }
        std::cout << info.size() << " events found" << std::endl;
    }
    /*******************************************************************************/
    bool SelectTrack(const std::vector<int> trackInfo, const std::vector<std::vector<int> > info){
        for(size_t iTrack = 0;iTrack < info.size();iTrack++){
            if(trackInfo[0] == info[iTrack][0] && trackInfo[1] == info[iTrack][1] && trackInfo[2] == info[iTrack][2] && trackInfo[3] == info[iTrack][3]){
                return true;
            }
        }
        return false;

    }
    /*******************************************************************************/
    /*******************************************************************************/
    /*******************************************************************************/
    /*******************************************************************************/
}

#endif
