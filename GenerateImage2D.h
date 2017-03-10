#ifndef LARLITE_GENERATEIMAGE2D_H
#define LARLITE_GENERATEIMAGE2D_H

#include <fstream>

#include "DataFormat/track.h"


namespace larlite{

    std::vector< std::vector<int> > ReadCSVFile(std::string FilePath);
    void CheckFileContent(std::vector<std::vector<int> > info);
    bool SelectTrack(const std::vector<int> trackInfo, const std::vector<std::vector<int> > info);
    

}

#endif
