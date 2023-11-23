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



llint GraphicsHandler::spawnEntity(const std::string& txtr, Vector2& pos, double dmg, double hp)
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

    this->objectList.push_back(ent);
    this->objectMap[this->objectList.back()->objectId] = (this->objectList.back());
    return this->objectList.back()->objectId;
}

llint GraphicsHandler::spawnEntity(const std::string& txtr, int p_x, int p_y, double dmg, double hp)
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

    this->objectList.push_back(ent);
    this->objectMap[this->objectList.back()->objectId] = (this->objectList.back());
    return this->objectList.back()->objectId;
}



void GraphicsHandler::deleteTexture(const llint object_id)
{
    for(auto it = this->objectList.begin(); it != this->objectList.end(); it++)
    {
        if ((*it)->objectId == object_id)
        {
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




const llint GraphicsHandler::getLastId() const
{
    return this->objectList.back()->objectId;
}




void GraphicsHandler::moveTextureDelta(const llint t_id, int dx, int dy)
{
    this->objectMap[t_id]->coordinates.x += dx;
    this->objectMap[t_id]->coordinates.y += dy;
}

void GraphicsHandler::moveTextureDelta(const llint t_id, Vector2& ds)
{
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

                if (this->objectMap[(**j).objectId]->nL == true 
                    && this->objectMap[(**i).objectId]->nL == true)
                {
                    if (((Entity*)(*i))->isAttacking == true)
                    {
                        ((Entity*)(*j))->curHP -= ((Entity*)(*i))->baseDamage;
                        std::cout << "OBJECT " << ((Entity*)(*j))->objectId <<
                            " IS AT " << ((Entity*)(*j))->curHP << 
                            " HEALTH POINTS" << std::endl;
                    }
                    if (((Entity*)(*j))->isAttacking == true)
                    {
                        ((Entity*)(*i))->curHP -= ((Entity*)(*j))->baseDamage;
                        std::cout << "OBJECT " << ((Entity*)(*i))->objectId <<
                            " IS AT " << ((Entity*)(*i))->curHP << 
                            " HEALTH POINTS" << std::endl;
                    }
                }

                // if (true
                // && this->objectMap[(**j).objectId]->objectId != this->targetId)
                // {
                //     this->deleteTexture(this->objectMap[(**j).objectId]->objectId);
                // }
                // if (true
                // && this->objectMap[(**i).objectId]->objectId != this->targetId)
                // {
                //     this->deleteTexture(this->objectMap[(**i).objectId]->objectId);
                // }

                // this->objectMap[(**i).objectId]->texture = 
                //     &(this->textureMap["collision.png"]);
                // this->objectMap[(**j).objectId]->texture =
                //     &(this->textureMap["collision.png"]);;

                //std::cout << "Collision!!!" << std::endl;

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



void GraphicsHandler::fightCheck()
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

        DrawText("PROJECT PENTAGRAM", 10, 10, 32, DARKPURPLE);

        if (this->cameraTarget != nullptr)
        {
            this->activeCamera.target = *(this->cameraTarget);
        }


        this->collisionCheck();
        this->animationCheck();
        this->fightCheck();

        BeginMode2D(this->activeCamera);

            for (auto i : objectList)
            {
                // std::cout << "NOT EMPTY " << std::endl;
                // std::cout << "POSITION: " << i.coordinates.x << 
                //     " " << i.coordinates.y << std::endl;

                DrawTexture(*(i->texture), i->coordinates.x, i->coordinates.y, Color{255,255,255,255});
            }

        EndMode2D();

        this->resetCollisions();

    EndDrawing();
}