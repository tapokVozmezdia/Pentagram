#include "audioHandler.hpp"

AudioHandler::AudioHandler()
{

}

AudioHandler::~AudioHandler()
{
    for (auto i : this->soundNameMap)
    {
        UnloadSound(i.second);
    }
}

void AudioHandler::connectToGraphics(std::list<GameObject*>* objLst, 
    std::unordered_map<llint, GameObject*>* objMp)
{
    if (objLst == nullptr || objMp == nullptr)
        throw std::logic_error("Trying to link nullptr to audio");

    this->objectList = objLst;
    this->objectMap = objMp;
}

void AudioHandler::loadSoundByName(const std::string& sName)
{
    Sound sound = LoadSound(("..\\assets\\audio\\" + sName).c_str());
    this->soundNameMap[sName] = sound;
}

void AudioHandler::setCombatMusic(const std::string& sName, double vol)
{
    for(auto it : this->combatMusic)
    {
        if (it == sName)
        {
            return;
        }
    }
    this->combatMusic.push_back(sName);
    this->combatVol = vol;
    SetSoundVolume(this->soundNameMap[sName], (float)vol);
}

void AudioHandler::deathFlag()
{
    this->combatTimer = FPS * 1000;
    this->combatFlag = false;
    for (auto it = this->soundNameMap.begin(); it != this->soundNameMap.end(); ++it)
    {
        StopSound((*it).second);
    }
}

void AudioHandler::triggerCombatMusic()
{
    this->combatTimer = 0;
    this->combatFlag = true;
}

void AudioHandler::playAmbient(const std::string& ambient, double vol)
{
    this->currentAmbient = ambient;
    this->ambientVol = vol;
}

void AudioHandler::stopAmbient()
{
    if (IsSoundPlaying(this->soundNameMap[currentAmbient]))
    {
        StopSound(this->soundNameMap[currentAmbient]);
    }
    this->currentAmbient = "null";
    this->ambientVol = 1;
}



void AudioHandler::perform()
{
    this->soundCheck();
}


void AudioHandler::soundCheck()
{
    if (this->objectList != nullptr)
        for (auto it = (this->objectList->begin()); it != (this->objectList->end()); it++)
        {
            if ((*it)->nL == true)
            {
                if (((Entity*)(*it))->curSt == 1 && !(((Entity*)(*it))->animation->soundNames.empty()))
                {
                    // std::cout << "KEK" << std::endl;
                    int r = GetRandomValue(1, ((Entity*)(*it))->animation->soundNames.size());
                    auto jt = ((Entity*)(*it))->animation->soundNames.begin();
                    for (int i = 1; i < r; ++i)
                    {
                        jt++;
                    }
                    //std::cout << *jt << std::endl;
                    this->addToQueue(*jt, true, true);
                    //std::cout << "Tb| 6OT" << std::endl;

                    if (((Entity*)(*it))->didDamage == true && !(((Entity*)(*it))->animation->extraSound.empty()))
                    {
                        // std::cout << "JKHGXK JHSAGDCKJH G" << std::endl;
                        // std::cout << *(((Entity*)(*it))->animation->extraSound.begin());
                        int r1 = GetRandomValue(1, ((Entity*)(*it))->animation->extraSound.size());
                        auto jt = ((Entity*)(*it))->animation->extraSound.begin();
                        for (int i = 1; i < r1; ++i)
                        {
                            jt++;
                        }
                        ((Entity*)(*it))->didDamage = false;
                        this->addToQueue(*jt, true, true);
                    }

                }

                if (((Entity*)(*it))->movingAnimation != nullptr)
                {
                    if (((Entity*)(*it))->movingFlag)
                    {   
                        if (!((Entity*)(*it))->animation->soundNames.empty())
                            this->addToQueue(*(((Entity*)(*it))->animation->soundNames.begin()), false, false);
                    }
                    else
                    {
                        if (!((Entity*)(*it))->movingAnimation->soundNames.empty())
                            this->removeFromQueue(*(((Entity*)(*it))->movingAnimation->soundNames.begin()));
                    }
                }
            }
        }


    if (this->combatFlag == true && this->combatMusicPlaying == false
        && !this->combatMusic.empty())
    {
        int r = GetRandomValue(0, this->combatMusic.size() - 1);
        auto it = this->combatMusic.begin();
        for (int i = 0; i < r; ++i)
        {
            it++;
        }
        this->addToQueue(*it, false, false);
        this->combatMusicPlaying = true;
        this->curCombatTrack = (*it);
    }

    if (this->combatFlag == true)
    {
        bool flg = false;
        this->curCombatVol = 1;
        for (auto it : this->combatMusic)
        {
            if (IsSoundPlaying(this->soundNameMap[it]))
            {
                flg = true;
                break;
            }
        }
        if (!flg)
        {
            int r = GetRandomValue(0, this->combatMusic.size() - 1);
            auto it = this->combatMusic.begin();
            for (int i = 0; i < r; ++i)
            {
                it++;
            }
            this->addToQueue(*it, false, false);
            this->combatMusicPlaying = true;
            this->curCombatTrack = (*it);
        }
    }
    
    if (this->combatFlag == false)
    {
        if (this->combatTimer >= FPS * 3)
        {
            //std::cout << "EXCEEDED" << std::endl;
            for (auto it : this->combatMusic)
            {
                //std::cout << "EXCEEDED" << std::endl;
                this->removeFromQueue(it);
            }
            this->combatTimer = 0;
            this->combatMusicPlaying = false;
        }
        else if (this->combatMusicPlaying == true)
        {
            this->combatTimer++;
            this->curCombatVol -= (1 /((double)FPS * 3.));
            // std::cout << curCombatVol << std::endl;
        }
    }

    this->combatFlag = false;



    if (this->combatMusicPlaying)
    {
        if (IsSoundPlaying(this->soundNameMap[this->curCombatTrack]))
        {
            SetSoundVolume(this->soundNameMap[this->curCombatTrack], this->curCombatVol * this->combatVol);
        }
    }

    this->run();
}


void AudioHandler::run()
{
    for (auto pr : this->soundListPair.first)
    {

        if (!IsSoundPlaying(this->soundNameMap[pr.first]))
        {
            PlaySound(this->soundNameMap[pr.first]);
            if (pr.second.hasPlayed && pr.second.playOnce)
            {
                this->removeFromQueue(pr.first);
            }
            pr.second.hasPlayed = true;
        }
    }

    for (auto snd : this->soundListPair.second)
    {
        if (IsSoundPlaying(this->soundNameMap[snd]))
        {
            StopSound(this->soundNameMap[snd]);
        }
    }

    this->soundListPair.first.clear();
    this->soundListPair.second.clear();

    if (this->currentAmbient != "null")
    {
        if (!IsSoundPlaying(this->soundNameMap[currentAmbient]))
        {
            PlaySound(this->soundNameMap[currentAmbient]);
            SetSoundVolume(this->soundNameMap[currentAmbient], this->ambientVol);
        }
    }
}

void AudioHandler::addToQueue(const std::string& snd, bool once, bool over)
{
    if (once && over)
    {
        PlaySound(this->soundNameMap[snd]);
        return;
    }
    for (auto it : this->soundListPair.first)
    {
        if (snd == it.first && !over && !once)
        {
            break;
        }
    }
    this->soundListPair.first.push_back(std::pair<std::string, OnceFlags>{snd, {once, false}});
    
    // for (auto it = this->soundListPair.second.begin();
    //     it != this->soundListPair.second.end(); ++it)
    // {
    //     if ((*it) == snd)
    //     {
    //         this->soundListPair.second.erase(it);
    //         break;
    //     } 
    // }
}

void AudioHandler::removeFromQueue(const std::string& snd)
{
    for (auto it : this->soundListPair.second)
    {
        if (snd == it)
        {
            break; 
        }
    }

    this->soundListPair.second.push_back(snd);

    // for (auto it = this->soundListPair.first.begin(); 
    //     it != this->soundListPair.first.end(); ++it)
    // {
    //     if ((*it).first == snd)
    //     {
    //         this->soundListPair.first.erase(it);
    //         break;
    //     }
    // }
}