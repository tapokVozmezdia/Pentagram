#include "statRecorder.hpp"
#include <sstream>
#include <string>


Recorder::Recorder(){}

void Recorder::startRecording()
{
    this->t_point = std::chrono::system_clock::now();
    this->finished = false;
}

void Recorder::stopRecording()
{
    if (this->finished)
        return;
    
    std::chrono::seconds record = 
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - this->t_point);
    this->finished = true;
    this->lastRecord = record;
    // std::cout << "LEVEL TOOK " << this->lastRecord.count() << " SECONDS!" << std::endl;
}

int Recorder::getLastRecord()
{
    return this->lastRecord.count();
}

int* Recorder::getStats()
{
    return this->stats;
}

void Recorder::saveLastRecord(int lvl,
    int diff)
{
    std::ofstream _file;
    _file.open(this->_stat_file_path, std::ios_base::app);
    std::string r = std::to_string(lvl) + "," + std::to_string(diff) + "," + 
        std::to_string(this->getLastRecord()) + "\n";
    _file << r;
    _file.close();
}

void Recorder::loadStats()
{
    std::ifstream _file;
    _file.open(this->_stat_file_path);
    std::string tmp;
    while (getline(_file, tmp))
    {
        std::stringstream s(tmp);
        std::string nlvl;
        std::string ndif;
        std::string nsec;
        getline(s, nlvl, ',');
        getline(s, ndif, ',');
        getline(s, nsec, ',');
        // std::cout << nlvl << " " << ndif << " " << nsec << std::endl;
        std::cout << stats[(stoi(nlvl) - 1) * 3 + (stoi(ndif) + 1)] << std::endl;
        if (this->stats[(stoi(nlvl) - 1) * 3 + (stoi(ndif) + 1)] > stoi(nsec))
            this->stats[(stoi(nlvl) - 1) * 3 + (stoi(ndif) + 1)] = stoi(nsec);
    }
    // for (int i = 0; i < 9; ++i)
    //     std::cout << this->stats[i] << std::endl;
}