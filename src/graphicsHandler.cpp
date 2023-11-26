#include "graphicsHandler.hpp"

GraphicsHandler::GraphicsHandler()
{
    this->activeCamera.offset = Vector2() = {WIDTH / 2.f - 120, 
        HEIGHT / 2.f - 120};
    this->activeCamera.rotation = 0.f;
    this->activeCamera.zoom = 1.f;
    
}

GraphicsHandler::~GraphicsHandler()
{
    for(auto obj : this->objectList)
    {
        delete obj;
    }
    this->objectList.clear();
    this->textureMap.clear();
}

void GraphicsHandler::loadTextureFromImage(const std::string& imgName)
{
    std::string tmp = "..\\assets\\images\\" + imgName;
    Image image = LoadImage(tmp.c_str());
    UnloadImage(image);
    // std::cout << "TEXTURE LOADING HERE" << std::endl;
    this->textureMap[imgName] = LoadTextureFromImage(image);
}

Texture2D* GraphicsHandler::getTexture(const std::string& tName)
{
    return &(this->textureMap[tName]);
}

llint GraphicsHandler::spawnTexture(const std::string& txtr, Vector2& pos)
{
    GameObject* obj = new GameObject();
    obj->texture = &(this->textureMap[txtr]);
    obj->textureName = txtr;
    obj->coordinates = pos;
    obj->objectId = IdCounter::getFreeId();
    obj->collision = false;

    this->objectList.push_back(obj);
    this->objectMap[this->objectList.back()->objectId] = (this->objectList.back());
    return this->objectList.back()->objectId;
}

llint GraphicsHandler::spawnTexture(const std::string& txtr, int p_x, int p_y)
{
    GameObject* obj = new GameObject();
    obj->texture = &(this->textureMap[txtr]);
    obj->textureName = txtr;
    obj->coordinates.x = p_x;
    obj->coordinates.y = p_y;
    obj->objectId = IdCounter::getFreeId();
    obj->collision = false;

    this->objectList.push_back(obj);
    this->objectMap[this->objectList.back()->objectId] = (this->objectList.back());
    // std::cout << this->objectList.back().objectId;
    return this->objectList.back()->objectId;
}



llint GraphicsHandler::spawnEntity(const std::string& txtr, Vector2& pos, double dmg, double hp, Team tm)
{
    Entity* ent = new Entity();
    ent->texture = &(this->textureMap[txtr]);
    ent->textureName = txtr;
    ent->coordinates = pos;
    ent->objectId = IdCounter::getFreeId();
    ent->collision = false;
    ent->baseDamage = dmg;
    ent->baseHP = hp;
    ent->curHP = hp;
    ent->nL = true;
    ent->team = tm;

    this->objectList.push_back(ent);
    this->objectMap[this->objectList.back()->objectId] = (this->objectList.back());
    return this->objectList.back()->objectId;
}

llint GraphicsHandler::spawnEntity(const std::string& txtr, int p_x, int p_y, double dmg, double hp, Team tm)
{
    Entity* ent = new Entity();
    ent->texture = &(this->textureMap[txtr]);
    ent->textureName = txtr;
    ent->coordinates.x = p_x;
    ent->coordinates.y = p_y;
    ent->objectId = IdCounter::getFreeId();
    ent->collision = false;
    ent->baseDamage = dmg;
    ent->baseHP = hp;
    ent->curHP = hp;
    ent->nL = true;
    ent->team = tm;

    this->objectList.push_back(ent);
    this->objectMap[this->objectList.back()->objectId] = (this->objectList.back());
    return this->objectList.back()->objectId;
}



void GraphicsHandler::wallFromObject(const llint object_id)
{
    (this->objectMap[object_id])->isWall = true;
    this->wallIndexList.push_back(object_id);
}



void GraphicsHandler::trapFromEntity(const llint object_id)
{
    if (this->objectMap[object_id]->nL == false)
    {
        throw std::logic_error("Non-entity id was ginven into trapFromEntity func");
    }
    ((Entity*)(this->objectMap[object_id]))->isTrap = true;
}


void GraphicsHandler::deleteTexture(const llint object_id)
{
    for(auto it = this->objectList.begin(); it != this->objectList.end(); it++)
    {
        if ((*it)->objectId == object_id)
        {
            if ((*it)->isWall == true)
            {
                for (auto jt = this->wallIndexList.begin(); 
                jt != this->wallIndexList.end(); jt++)
                {
                    if ((*jt) == object_id)
                    {
                        this->wallIndexList.erase(jt);
                        break;
                    }
                }
            }
            delete this->objectMap[object_id];
            this->objectMap.erase(object_id);
            this->objectList.erase(it);
        }
    }
}



void GraphicsHandler::animateTexture(const llint t_id, Animation& anim)
{
    // std::cout << "!!HERE:" << &(anim) << std::endl;
    this->objectMap[t_id]->animation = std::make_shared<Animation>(anim);
    this->objectMap[t_id]->texture = *(this->objectMap[t_id]->animation->textures.begin());
    this->objectMap[t_id]->textureName = *(this->objectMap[t_id]->animation->textureNames.begin());
}

void GraphicsHandler::animateEntity(const llint t_id, Animation& attack, Animation& hit)
{
    ((Entity*)(this->objectMap[t_id]))->attackAnimation = std::make_shared<Animation>(attack);
}




void GraphicsHandler::triggerAttack(const llint t_id)
{
    if (((Entity*)this->objectMap[t_id])->attackAnimation != nullptr && 
        ((Entity*)this->objectMap[t_id])->curSt == 0 && 
        ((Entity*)this->objectMap[t_id])->isAttacking == false)
    {
        ((Entity*)this->objectMap[t_id])->isAttacking = true;
        // std::cout << "ROBOT dealt " << ((Entity*)this->objectMap[t_id])->baseDamage
        //     << " DAMAGE" << std::endl;
    }
}



void GraphicsHandler::enemyHostilityTriggerOn()
{
    this->hostilityFlag = true;
}


void GraphicsHandler::enemyHostilityTriggerOff()
{
    this->hostilityFlag = false;
}



const llint GraphicsHandler::getLastId() const
{
    return this->objectList.back()->objectId;
}




void GraphicsHandler::moveTextureDelta(const llint t_id, float dx, float dy)
{
    float w_x = this->objectMap[t_id]->coordinates.x + dx;
    float w_y = this->objectMap[t_id]->coordinates.y + dy;

    for (auto ind : this->wallIndexList)
    {
        if ((w_x < (this->objectMap[ind]->coordinates.x + this->objectMap[ind]->texture->width) 
        && w_x + this->objectMap[t_id]->texture->width > this->objectMap[ind]->coordinates.x 
        && w_y < (this->objectMap[ind]->coordinates.y + this->objectMap[ind]->texture->height) 
        && w_y + this->objectMap[t_id]->texture->height > this->objectMap[ind]->coordinates.y))
        {
            return;
        }
    }

    this->objectMap[t_id]->coordinates.x += dx;
    this->objectMap[t_id]->coordinates.y += dy;
}

void GraphicsHandler::moveTextureDelta(const llint t_id, Vector2& ds)
{
    float w_x = this->objectMap[t_id]->coordinates.x + ds.x;
    float w_y = this->objectMap[t_id]->coordinates.y + ds.y;

    for (auto ind : this->wallIndexList)
    {
        if ((w_x < (this->objectMap[ind]->coordinates.x + this->objectMap[ind]->texture->width) 
        && w_x + this->objectMap[t_id]->texture->width > this->objectMap[ind]->coordinates.x 
        && w_y < (this->objectMap[ind]->coordinates.y + this->objectMap[ind]->texture->height) 
        && w_y + this->objectMap[t_id]->texture->height > this->objectMap[ind]->coordinates.y))
        {
            return;
        }
    }

    this->objectMap[t_id]->coordinates.x += ds.x;
    this->objectMap[t_id]->coordinates.y += ds.y;
}

void GraphicsHandler::moveTextureAbs(const llint t_id, int dx, int dy)
{
    this->objectMap[t_id]->coordinates.x = dx;
    this->objectMap[t_id]->coordinates.y = dy;
}

void GraphicsHandler::moveTextureAbs(const llint t_id, Vector2& ds)
{
    this->objectMap[t_id]->coordinates.x = ds.x;
    this->objectMap[t_id]->coordinates.y = ds.y;
}




void GraphicsHandler::centerCamera(llint t_id)
{
    this->cameraTarget = &(this->objectMap[t_id]->coordinates);
    this->targetId = t_id;
    // std::cout << "HERE " << std::endl;
    // std::cout << this->objectMap[t_id]->coordinates.x << " " <<
    //     this->objectMap[t_id]->coordinates.y << std::endl;
}

void GraphicsHandler::clearAll()
{
    this->objectList.clear();
}

void GraphicsHandler::perform()
{
    this->run();
}




void GraphicsHandler::collisionCheck()
{
    for (auto i = this->objectList.begin(); i != this->objectList.end(); ++i)
    {
        for (auto j = i; j != this->objectList.end(); ++j)
        {
            if (i == j)
                continue;

            // std::cout << "\tCOORD1\t COORD2\n" << 
            //     "\t" << (**j).coordinates.x << "\t" << (**j).coordinates.y <<
            //     "\n\t" << (**i).coordinates.x << "\t" << (**i).coordinates.y << "\n";

            if (((**j).coordinates.x < ((**i).coordinates.x + (**i).texture->width) && 
                (**j).coordinates.x + (**j).texture->width > (**i).coordinates.x &&
                (**j).coordinates.y < ((**i).coordinates.y + (**i).texture->height) && 
                (**j).coordinates.y + (**j).texture->height > (**i).coordinates.y))
            {
                this->objectMap[(**i).objectId]->collision = true;
                this->objectMap[(**j).objectId]->collision = true;

                this->fightCheck(*i, *j);
            } 
        }
    }
}

void GraphicsHandler::resetCollisions()
{
    for (auto i : this->objectList)
    {
        this->objectMap[i->objectId]->collision = false;
    }
}

void GraphicsHandler::animationCheck()
{
    for (auto i = this->objectList.begin(); 
        i != this->objectList.end(); ++i)
    {
        if ((*i)->nL == false) // If not Entity
        {
            if ((*i)->animation == nullptr)
                continue;
            if ((*i)->animation->curTime >= FPS * ((*i)->animation->timeQueues[0]))
            {
                ContainerHandler::shiftListL(&((*i)->animation->textures));
                ContainerHandler::shiftListL(&((*i)->animation->textureNames));
                ContainerHandler::shiftVectorL(&((*i)->animation->timeQueues));
                (*i)->texture = *((*i)->animation->textures.begin());
                (*i)->textureName = *((*i)->animation->textureNames.begin());
                (*i)->animation->curTime = 0;
                continue;
            }
            ((*i)->animation->curTime)++;
            continue;
        }

        //For entity
        if ((*i)->animation == nullptr)
                continue;
        if (((Entity*)(*i))->isAttacking == true &&
            ((Entity*)(*i))->attackAnimation != nullptr)
        {
            std::swap(((Entity*)(*i))->animation, ((Entity*)(*i))->attackAnimation);
            ((Entity*)(*i))->curSt = ((Entity*)(*i))->curSt + 1;
            ((Entity*)(*i))->isAttacking = false;
        }
        if (((Entity*)(*i))->curSt > ((Entity*)(*i))->animation->textures.size()
             && ((Entity*)(*i))->attackAnimation != nullptr)
        {
            ((Entity*)(*i))->curSt = 0;
            std::swap(((Entity*)(*i))->animation, ((Entity*)(*i))->attackAnimation);
        }
        if ((*i)->animation->curTime >= (FPS * ((*i)->animation->timeQueues[0])))
        {
            ContainerHandler::shiftListL(&((*i)->animation->textures));
            ContainerHandler::shiftListL(&((*i)->animation->textureNames));
            ContainerHandler::shiftVectorL(&((*i)->animation->timeQueues));
            (*i)->texture = *((*i)->animation->textures.begin());
            (*i)->textureName = *((*i)->animation->textureNames.begin());
            (*i)->animation->curTime = 0;
            if (((Entity*)(*i))->curSt != 0)
            {
                ((Entity*)(*i))->curSt += 1;
            }
            continue;
        }
        ((*i)->animation->curTime)++;
    }
}




void GraphicsHandler::fightCheck(GameObject* i, GameObject* j)
{
    if (this->objectMap[(*j).objectId]->nL == true 
                    && this->objectMap[(*i).objectId]->nL == true)
    {
        if (((Entity*)(i))->isTrap == true 
            && ((Entity*)(j))->isTrap == false)
        {
            this->triggerAttack(((Entity*)(i))->objectId);
        }
        if (((Entity*)(j))->isTrap == true 
        && ((Entity*)(i))->isTrap == false)
        {
            this->triggerAttack(((Entity*)(j))->objectId);
        }


        if ((((Entity*)(i))->isAttacking == true 
            && ((Entity*)(j))->isTrap == false) && 
            ((((Entity*)(j))->team != ((Entity*)(i))->team) ||
            ((Entity*)(i))->isTrap == true)
            )

        {
            ((Entity*)(j))->curHP -= ((Entity*)(i))->baseDamage;
            std::cout << "OBJECT " << ((Entity*)(j))->objectId <<
                " IS AT " << ((Entity*)(j))->curHP << 
                " HEALTH POINTS" << std::endl;
        }
        if ((((Entity*)(j))->isAttacking == true
            && ((Entity*)(i))->isTrap == false) && 
            ((((Entity*)(j))->team != ((Entity*)(i))->team) 
            || ((Entity*)(j))->isTrap == true)
            )
        {
            ((Entity*)(i))->curHP -= ((Entity*)(j))->baseDamage;
            std::cout << "OBJECT " << ((Entity*)(i))->objectId <<
                " IS AT " << ((Entity*)(i))->curHP << 
                " HEALTH POINTS" << std::endl;
        }
    }
}



void GraphicsHandler::enemyHostile()
{
    for (auto i = this->objectList.begin(); i != this->objectList.end(); ++i)
    {
        for (auto j = i; j != this->objectList.end(); ++j)
        {
            if ((*i)->nL == false || (*j)->nL == false)
            {
                continue;
            }

            if (((Entity*)(*i))->team == ((Entity*)(*j))->team ||
                ((Entity*)(*i))->team == NEUTRAL || ((Entity*)(*j))->team == NEUTRAL
                || ((Entity*)(*i))->isTrap == true || ((Entity*)(*j))->isTrap == true)
            {
                continue;
            }

            bool makeNoise = false;
            double nse = GetRandomValue(0, 10000);
            nse /= 10000;

            if (nse < 1  * NOISE_FREQUENCY / (double(FPS * 5.)))
            {
                makeNoise = true;
            }

            if ((((Entity*)(*i))->team == MAIN || ((Entity*)(*i))->team == ALLY) 
            && (((Entity*)(*j))->team == HOSTILE))
            {
                Vector2 c_i = ObjectHandler::getCenter(*i);
                Vector2 c_j = ObjectHandler::getCenter(*j);

                if(makeNoise && ((Entity*)(*j))->noise == 0)
                {
                    (((Entity*)(*j))->noise) += 1;
                    ((Entity*)(*j))->momentum = {
                        (float)GetRandomValue(-100, 100) / 100,
                        (float)GetRandomValue(-100, 100) / 100
                    };
                    ((Entity*)(*j))->noise_length = (float)GetRandomValue(0, 200) / 100;
                }

                if(((Entity*)(*j))->noise > NOISE_LENGTH * (float)FPS * ((Entity*)(*j))->noise_length)
                {
                    ((Entity*)(*j))->noise = 0;
                }

                if(((Entity*)(*j))->noise != 0)
                {
                    this->moveTextureDelta((*j)->objectId, ((Entity*)(*j))->momentum.x, ((Entity*)(*j))->momentum.y);
                    ((Entity*)(*j))->noise += 1;
                }

                if (sqrt(((c_i.x - c_j.x) * (c_i.x - c_j.x))
                + ((c_i.y - c_j.y) * (c_i.y - c_j.y))) <= ENEMY_VISIBILITY)
                {
                    double dx = c_i.x - c_j.x;
                    double dy = c_i.y - c_j.y;
                    double l = sqrt(dx * dx + dy * dy);
                    dx = dx / l * ENEMY_SPEED;
                    dy = dy / l * ENEMY_SPEED;
                    this->moveTextureDelta((*j)->objectId, dx, dy);
                    if (ObjectHandler::collided(*i, *j) && makeNoise)
                    {
                        triggerAttack((*j)->objectId);
                    }
                }
            }

            if ((((Entity*)(*j))->team == MAIN || ((Entity*)(*j))->team == ALLY) 
            && (((Entity*)(*i))->team == HOSTILE))
            {
                Vector2 c_i = ObjectHandler::getCenter(*i);
                Vector2 c_j = ObjectHandler::getCenter(*j);

                if(makeNoise && ((Entity*)(*i))->noise == 0)
                {
                    (((Entity*)(*i))->noise) += 1;
                    ((Entity*)(*i))->momentum = {
                        (float)GetRandomValue(-100, 100) / 100,
                        (float)GetRandomValue(-100, 100) / 100
                    };
                    ((Entity*)(*i))->noise_length = (float)GetRandomValue(0, 200) / 100;
                }

                if(((Entity*)(*i))->noise > NOISE_LENGTH * (float)FPS * ((Entity*)(*i))->noise_length)
                {
                    ((Entity*)(*i))->noise = 0;
                }

                if(((Entity*)(*i))->noise != 0)
                {
                    this->moveTextureDelta((*i)->objectId, ((Entity*)(*i))->momentum.x, ((Entity*)(*i))->momentum.y);
                    ((Entity*)(*i))->noise += 1;
                }

                if (sqrt(((c_j.x - c_i.x) * (c_j.x - c_i.x))
                + ((c_j.y - c_i.y) * (c_j.y - c_i.y))) <= ENEMY_VISIBILITY)
                {
                    double dx = c_j.x - c_i.x;
                    double dy = c_j.y - c_i.y;
                    double l = sqrt(dx * dx + dy * dy);
                    dx = dx / l * ENEMY_SPEED;
                    dy = dy / l * ENEMY_SPEED;
                    this->moveTextureDelta((*i)->objectId, dx, dy);
                    if (ObjectHandler::collided(*i, *j) && makeNoise)
                    {
                        triggerAttack((*i)->objectId);
                    }
                }
            }
        }
    }
}



void GraphicsHandler::afterFightCheck()
{
    std::list<llint> to_delete_list;

    for (auto it : objectList)
    {
        if (it->nL == true)
        {
            if (((Entity*)(it))->curHP <= 0)
            {
                to_delete_list.push_back(it->objectId);
            }
        }
    }

    for (auto i : to_delete_list)
    {
        this->deleteTexture(i);
    }
}



void GraphicsHandler::run()
{
    BeginDrawing();

        ClearBackground(BLACK);

        if (this->cameraTarget != nullptr)
        {
            this->activeCamera.target = *(this->cameraTarget);
        }


        if (this->hostilityFlag)
        {
            this->enemyHostile();
        }


        this->collisionCheck();
        this->animationCheck();
        this->afterFightCheck();

        BeginMode2D(this->activeCamera);

            for (auto i : objectList)
            {
                // std::cout << "NOT EMPTY " << std::endl;
                // std::cout << "POSITION: " << i.coordinates.x << 
                //     " " << i.coordinates.y << std::endl;

                DrawTexture(*(i->texture), i->coordinates.x, i->coordinates.y, Color{255,255,255,255});
            }

        EndMode2D();

        DrawText("PROJECT PENTAGRAM", 10, 10, 32, DARKPURPLE);

        this->resetCollisions();

    EndDrawing();
}