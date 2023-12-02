#include "penta_mc.hpp"

class AudioHandler
{
    public:

        AudioHandler();
        ~AudioHandler();

        void connectToGraphics(std::list<GameObject*>* objLst, 
            std::unordered_map<llint, GameObject*>* objMp);



        void loadSoundByName(const std::string& sName);

        void setCombatMusic(const std::string& sName, double vol);

        void deathFlag();

        void triggerCombatMusic();

        void playAmbient(const std::string& ambient, double vol);
        void stopAmbient();

        void perform();


    private:

        struct OnceFlags
        {
            bool playOnce = false;
            bool hasPlayed = false;
        };


        void soundCheck();

        void run();

        void addToQueue(const std::string& snd, bool once, bool over);
        void removeFromQueue(const std::string& snd);



        llint combatTimer = 0;
        bool combatFlag = false;
        bool combatMusicPlaying = false;
        double curCombatVol = 1;
        std::string curCombatTrack;

        llint recieverId;
        Vector2* recieverPos;

        double ambientVol = 1;
        std::string currentAmbient = "null";


        std::list<std::string> combatMusic;
        double combatVol = 1;


        // These point to objects stored in graphics handler 
        std::list<GameObject*>* objectList = nullptr;
        std::unordered_map<llint, GameObject*>* objectMap = nullptr;

        std::pair<
            std::list<std::pair<std::string, OnceFlags>>, // Sounds to start playing (if not playing)
            std::list<std::string>  // Sounds to stop playing (if playing)
        > soundListPair;

        std::unordered_map<llint, Sound*> soundPieceMap;
        std::unordered_map<std::string, Sound> soundNameMap;
};