#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <algorithm>

class Recorder
{
    public:

        Recorder();
        void startRecording();
        void stopRecording();

        void saveLastRecord(int lvl,
            int diff);

        void loadStats();
        void deloadStats();

        int* getStats(); // careful, can be altered from outside

        int getLastRecord();

    private:
        std::string _stat_file_path = "stats.txt";
        int stats[9] = {
            2000000000, 2000000000, 2000000000,
            2000000000, 2000000000, 2000000000,
            2000000000, 2000000000, 2000000000};
        std::chrono::time_point<std::chrono::system_clock> t_point;
        std::chrono::seconds lastRecord;
        bool finished = true;
};