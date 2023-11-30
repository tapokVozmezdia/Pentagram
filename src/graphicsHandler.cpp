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
    this->objectMap.clear();
    this->wallIndexList.clear();
    for (auto it = this->textureMap.begin(); it != this->textureMap.end(); ++it)
    {
        UnloadTexture((*it).second);
    }
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

llint GraphicsHandler::spawnTexture(const std::string& txtr, const Vector2& pos)
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
    ent->hitbox.width = ent->texture->width;
    ent->hitbox.height = ent->texture->height;

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
    ent->hitbox.width = ent->texture->width;
    ent->hitbox.height = ent->texture->height;

    this->objectList.push_back(ent);
    this->objectMap[this->objectList.back()->objectId] = (this->objectList.back());
    return this->objectList.back()->objectId;
}


void GraphicsHandler::setHitbox(llint t_id, const Hitbox& hbox)
{
    if (this->objectMap[t_id]->nL == false)
        throw std::logic_error("Trying to set a hitbox for a non-entity object");
    ((Entity*)(this->objectMap[t_id]))->hitbox = {hbox.width, hbox.height};
}

void GraphicsHandler::setHitbox(llint t_id, const int width, const int height)
{
    if (this->objectMap[t_id]->nL == false)
        throw std::logic_error("Trying to set a hitbox for a non-entity object");
    ((Entity*)(this->objectMap[t_id]))->hitbox = {width, height};
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
    // std::cout << "MUST DELETE" << std::endl;
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
            delete (this->objectMap[object_id]);
            this->objectMap.erase(object_id);
            this->objectList.erase(it);
            break;
        }
    }
}



void GraphicsHandler::animateTexture(const llint t_id, const Animation& anim)
{
    // std::cout << "!!HERE:" << &(anim) << std::endl;
    this->objectMap[t_id]->animation = std::make_shared<Animation>(anim);
    this->objectMap[t_id]->texture = *(this->objectMap[t_id]->animation->textures.begin());
    this->objectMap[t_id]->animation->firstTexture = this->objectMap[t_id]->texture;
    this->objectMap[t_id]->textureName = *(this->objectMap[t_id]->animation->textureNames.begin());
}

void GraphicsHandler::animateEntity(const llint t_id, const Animation& attack, const Animation& hit)
{
    if (this->objectMap[t_id]->nL == false)
        throw std::logic_error("trying to animate non-entity");
    ((Entity*)(this->objectMap[t_id]))->attackAnimation = std::make_shared<Animation>(attack);
}

void GraphicsHandler::animateEntityMoving(const llint t_id, const Animation& move)
{
    if (this->objectMap[t_id]->nL == false)
        throw std::logic_error("trying to animate non-entity");
    ((Entity*)(this->objectMap[t_id]))->movingAnimation = std::make_shared<Animation>(move);
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


llint GraphicsHandler::fireProjectile(const std::string& txtr, const Vector2& src, 
    const Vector2& trgt, const int dmg, const double speed,
    const Animation& flight, const Animation& hit)
{
    Projectile* pr = new Projectile();

    pr->animation = std::make_shared<Animation>(flight);
    pr->hitAnimation = std::make_shared<Animation>(hit);
    pr->texture = &(this->textureMap[txtr]);
    pr->coordinates.x = src.x;
    pr->coordinates.y = src.y;
    pr->target.x = trgt.x;
    pr->target.y = trgt.y;
    pr->objectId = IdCounter::getFreeId();
    pr->collision = false;
    pr->damage = dmg;
    pr->speedModifier = speed;
    pr->nLP = true;


    this->objectList.push_back(pr);
    this->objectMap[this->objectList.back()->objectId] = (this->objectList.back());
    return this->objectList.back()->objectId;
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
    Vector2 cent = ObjectHandler::getCenter(this->objectMap[t_id]);
    float w_x;
    float w_y;
    if (this->objectMap[t_id]->nL == false)
    {
        w_x = this->objectMap[t_id]->coordinates.x + dx;
        w_y = this->objectMap[t_id]->coordinates.y + dy;
    }
    else
    {
        w_x = cent.x - ((((Entity*)(this->objectMap[t_id]))->hitbox.width) / 2) + dx;
        w_y = cent.y - ((((Entity*)(this->objectMap[t_id]))->hitbox.height) / 2) + dy;
        if ((dx < 0 && !(((Entity*)(this->objectMap[t_id]))->isFlipped))
        ||
        (dx > 0 && (((Entity*)(this->objectMap[t_id]))->isFlipped)))
        {
            this->flipTexture(t_id);
        }
        ((Entity*)(this->objectMap[t_id]))->hasMoved = true;
    }
    // w_y = this->objectMap[t_id]->coordinates.y + dy;

    for (auto ind : this->wallIndexList)
    {
        if ((w_x < (this->objectMap[ind]->coordinates.x + this->objectMap[ind]->texture->width) 
        && w_x + ((Entity*)(this->objectMap[t_id]))->hitbox.width > this->objectMap[ind]->coordinates.x 
        && w_y < (this->objectMap[ind]->coordinates.y + this->objectMap[ind]->texture->height) 
        && w_y + ((Entity*)(this->objectMap[t_id]))->hitbox.height > this->objectMap[ind]->coordinates.y))
        {
            return;
        }
    }

    this->objectMap[t_id]->coordinates.x += dx;
    this->objectMap[t_id]->coordinates.y += dy;
}

void GraphicsHandler::moveTextureDelta(const llint t_id, const Vector2& ds)
{
    this->moveTextureDelta(t_id, ds.x, ds.y);
    // float w_x = this->objectMap[t_id]->coordinates.x + ds.x;
    // float w_y = this->objectMap[t_id]->coordinates.y + ds.y;

    // for (auto ind : this->wallIndexList)
    // {
    //     if ((w_x < (this->objectMap[ind]->coordinates.x + this->objectMap[ind]->texture->width) 
    //     && w_x + this->objectMap[t_id]->texture->width > this->objectMap[ind]->coordinates.x 
    //     && w_y < (this->objectMap[ind]->coordinates.y + this->objectMap[ind]->texture->height) 
    //     && w_y + this->objectMap[t_id]->texture->height > this->objectMap[ind]->coordinates.y))
    //     {
    //         return;
    //     }
    // }

    // this->objectMap[t_id]->coordinates.x += ds.x;
    // this->objectMap[t_id]->coordinates.y += ds.y;
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


bool GraphicsHandler::isFlipped(const llint t_id)
{
    return (this->objectMap[t_id]->isFlipped);
}

void GraphicsHandler::flipTexture(const llint t_id)
{
    this->objectMap[t_id]->isFlipped = !(this->objectMap[t_id]->isFlipped);
}

void GraphicsHandler::playAnimationOnce(const Vector2& pos, const Animation& anim)
{
    llint anim_id = this->spawnTexture("collision.png", pos);
    this->animateTexture(anim_id, anim);
    this->objectMap[anim_id]->mark = true;
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
    for(auto obj : this->objectList)
    {
        delete obj;
    }
    this->objectList.clear();
    this->objectMap.clear();
    this->wallIndexList.clear();

    this->cameraTarget = nullptr;
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

void GraphicsHandler::resetMovement()
{
    for (auto i : this->objectList)
    {
        if (i->nL == true)
        {
            ((Entity*)(i))->hasMoved = false;
        }
    }
}

void GraphicsHandler::animationCheck()
{
    for (auto i = this->objectList.begin(); 
        i != this->objectList.end(); ++i)
    {
        if ((*i)->nL == false && (*i)->nLP == false) // If not Entity/Projectile
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
                if ((*i)->mark == false)
                {
                    continue;
                }
                ((*i)->EXTRA)++;
            }
            ((*i)->animation->curTime)++;
            if ((*i)->mark == false)
                continue;
            // std::cout << (*i)->animation->curTime << " " <<
            // FPS * ContainerHandler::getElementsSum(&((*i)->animation->timeQueues)) << std::endl;
            if ((*i)->EXTRA >= (*i)->animation->timeQueues.size())
            {
                // std::cout << "<N>UST" << std::endl;
                this->deleteTexture((*i)->objectId);
            }
            continue;
        }

        if((*i)->nLP == true) // For projectiles
        {
            // if (((Projectile*)(*i))->exploded)
            // {
            //     llint tmp_id = this->spawnTexture(
            //     *(((Projectile*)(*i))->hitAnimation->textureNames.begin()), 
            //     (*i)->coordinates
            //     );

            //     this->animateTexture(tmp_id, *(((Projectile*)(*i))->hitAnimation));

            //     this->objectMap[tmp_id]->mark = true;

            //     this->deleteTexture((*i)->objectId);
            // }
            // if ((ObjectHandler::measureDistance(
            //     ObjectHandler::getCenter(*i), ((Projectile*)(*i))->target)
            // ) <= ((Projectile*)(*i))->speedModifier * PROJECTILE_SPEED)
            // {
            //     this->moveTextureAbs((*i)->objectId, ((Projectile*)(*i))->target);
            //     ((Projectile*)(*i))->exploded = true;
            // }
            // else
            // {
            //     auto tmp_vec = ObjectHandler::getVectorWithLength(
            //         ObjectHandler::getVectorDiff(
            //             ObjectHandler::getCenter(*i),
            //             ((Projectile*)(*i))->target
            //         ),
            //         ((Projectile*)(*i))->speedModifier * PROJECTILE_SPEED
            //     );
            //     this->moveTextureDelta((*i)->objectId, tmp_vec);
            //     // std::cout << "MUST MOVE ENERGY BALL: " << tmp_vec.x << " " << tmp_vec.y << std::endl;
            // }
            continue;
        }

        //For entity
        if ((*i)->animation == nullptr)
                continue;
        if (((Entity*)(*i))->isAttacking == false && ((Entity*)(*i))->curSt == 0)
        {
            if (((Entity*)(*i))->movingAnimation != nullptr)
            {
                if (((Entity*)(*i))->hasMoved && ((Entity*)(*i))->movingFlag == false)
                {
                    ((Entity*)(*i))->movingFlag = true;
                    std::swap(((Entity*)(*i))->animation, ((Entity*)(*i))->movingAnimation);
                }
                if (((Entity*)(*i))->hasMoved == false && ((Entity*)(*i))->movingFlag == true)
                {
                    ((Entity*)(*i))->movingFlag = false;
                    std::swap(((Entity*)(*i))->animation, ((Entity*)(*i))->movingAnimation);
                    ((Entity*)(*i))->texture = *(((Entity*)(*i))->animation->textures.begin());
                    ((Entity*)(*i))->textureName = *(((Entity*)(*i))->animation->textureNames.begin());
                }
            }
        }
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
            
            (*i)->texture = *((*i)->animation->textures.begin());
            (*i)->textureName = *((*i)->animation->textureNames.begin());

            if (((Entity*)(*i))->hasMoved == false && ((Entity*)(*i))->movingFlag == false)
            {

                while(*(((Entity*)(*i))->animation->textures.begin()) != 
                ((Entity*)(*i))->animation->firstTexture
                )
                {
                    //std::cout << ((Entity*)(*i))->textureName << std::endl;
                    ContainerHandler::shiftListL(&((*i)->animation->textures));
                    ContainerHandler::shiftListL(&((*i)->animation->textureNames));
                    ContainerHandler::shiftVectorL(&((*i)->animation->timeQueues));
                }

                (*i)->texture = *((*i)->animation->textures.begin());
                (*i)->textureName = *((*i)->animation->textureNames.begin());
            }
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
            (((Entity*)(i))->isTrap == true && ((Entity*)(i))->baseDamage >= 0))
            )

        {
            ((Entity*)(j))->curHP -= ((Entity*)(i))->baseDamage;
            if (((Entity*)(j))->team != MAIN)
            std::cout << "OBJECT " << ((Entity*)(j))->objectId <<
                " IS AT " << ((Entity*)(j))->curHP << 
                " HEALTH POINTS" << std::endl;
            if (((Entity*)(i))->baseDamage <= 0)
            {
                this->deleteTexture(((Entity*)(i))->objectId);
            }
        }
        if ((((Entity*)(j))->isAttacking == true
            && ((Entity*)(i))->isTrap == false) && 
            ((((Entity*)(j))->team != ((Entity*)(i))->team) 
            || (((Entity*)(j))->isTrap == true && ((Entity*)(j))->baseDamage >= 0))
            )
        {
            ((Entity*)(i))->curHP -= ((Entity*)(j))->baseDamage;
            if (((Entity*)(i))->team != MAIN)
            std::cout << "OBJECT " << ((Entity*)(i))->objectId <<
                " IS AT " << ((Entity*)(i))->curHP << 
                " HEALTH POINTS" << std::endl;
            if (((Entity*)(j))->baseDamage <= 0)
            {
                this->deleteTexture(((Entity*)(j))->objectId);
            }
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

            bool flipFlag = false;

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
                    this->moveTextureDelta((*j)->objectId, ((Entity*)(*j))->momentum.x, 0);
                    this->moveTextureDelta((*j)->objectId, 0, ((Entity*)(*j))->momentum.y);

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

                    if (ObjectHandler::measureDistance(
                        ObjectHandler::getCenter((*i)),
                        ObjectHandler::getCenter((*j))
                    ) > 3)
                    {
                        this->moveTextureDelta((*j)->objectId, dx, 0);
                        this->moveTextureDelta((*j)->objectId, 0, dy);
                    }
                    
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
                    this->moveTextureDelta((*i)->objectId, ((Entity*)(*i))->momentum.x, 0);
                    this->moveTextureDelta((*i)->objectId, 0, ((Entity*)(*i))->momentum.y);
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
                    
                    if (ObjectHandler::measureDistance(
                        ObjectHandler::getCenter((*i)),
                        ObjectHandler::getCenter((*j))
                    ) > 3)
                    {
                        this->moveTextureDelta((*i)->objectId, dx, 0);
                        this->moveTextureDelta((*i)->objectId, 0, dy);
                    }
                    
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
                if ((it)->objectId == this->targetId)
                {
                    this->startFlag = false;
                    this->cameraTarget = nullptr;
                }    
                to_delete_list.push_back(it->objectId);
            }
        }
    }

    for (auto i : to_delete_list)
    {
        this->deleteTexture(i);
    }
}



void GraphicsHandler::drawButtons()
{
    uint counter = 0;
    bool tmp_flag = false;
    for(auto it = this->buttonList.begin(); it != this->buttonList.end(); it++)
    {
        if ((*it)->visible == true)
        {
            if (this->buttonFlag == false)
            {
                ClearBackground(BLACK);
                this->buttonFlag = true;
                tmp_flag = true;
            }
            DrawRectangle(WIDTH / 2 - 125, 20 + 70 * counter, 250, 50, DARKBLUE);
            DrawText(((*it)->text).c_str(), WIDTH / 2 - 125, 20 + 70 * counter, 24, RED);
            (*it)->position = {(float)(WIDTH / 2 - 125), (float)(20 + 70 * counter)};
            counter++;
        }
    }
    if (tmp_flag == false)
    {
        this->buttonFlag = false;
    }
}



void GraphicsHandler::linkButtons(std::list<Button>* _buttons)
{
    for (auto it = _buttons->begin(); it != _buttons->end(); ++it)
    {
        this->buttonList.push_back(&(*it));
    }
}


void GraphicsHandler::buttonClickCheck()
{
    int mx = GetMouseX();
    int my = GetMouseY();
    for (auto it = this->buttonList.begin(); it != this->buttonList.end(); it++)
    {
        if ((*it)->visible == false)
        {
            (*it)->is_clicked = false;
            continue;
        }
        if (mx < (*it)->position.x + (*it)->width && 
        mx > (*it)->position.x &&
        my < (*it)->position.y + (*it)->height && 
        my > (*it)->position.y
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            (*it)->is_clicked = true;

            for (auto ik = this->buttonList.begin(); ik != this->buttonList.end(); ik++)
            {
                if ((*ik)->button_id != (*it)->button_id)
                {
                    (*ik)->is_clicked = false;
                }
            }
            
            std::cout << (*it)->text + " BUTTON PRESSED!" << std::endl;

            this->buttonManage(*it);

            break;
        }
        (*it)->is_clicked = false;
    }
}


void GraphicsHandler::buttonManage(Button* button)
{
    if (button->text == "PLAY")
    {
        int i = 1;
        for (auto it = this->buttonList.begin(); it != this->buttonList.end(); it++)
        {
            if (i > 3 && i < 6)
            {
                (*it)->visible = true;
            }
            else
            {
                (*it)->visible = false;
            }
            i++;
        }
    }
    if (button->text == "LEVELS")
    {
        int i = 1;
        for (auto it = this->buttonList.begin(); it != this->buttonList.end(); it++)
        {
            if (i > 5 && i < 10)
            {
                (*it)->visible = true;
            }
            else
            {
                (*it)->visible = false;
            }
            i++;
        }
    }
    if (button->text == "LEVEL 1")
    {
        this->buttonFlag = false;
        for (auto it = this->buttonList.begin(); it != this->buttonList.end(); it++)
        {
            (*it)->visible = false;
        }
    }
    
    if (button->text == "QUIT")
    {
        exit(0);
    }
}

bool GraphicsHandler::checkPulse()
{
    return this->startFlag;
}

void GraphicsHandler::flagTheStart()
{
    this->startFlag = true;
}


void GraphicsHandler::run()
{
    BeginDrawing();


        ClearBackground(BLACK);

        this->drawButtons();

        this->buttonClickCheck();

        if (this->buttonFlag == false && this->startFlag == true)
        {
            for (auto ik = this->buttonList.begin(); ik != this->buttonList.end(); ik++)
            {
                (*ik)->is_clicked = false;
            }


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

            if (this->cameraTarget != nullptr)
            {
                BeginMode2D(this->activeCamera);

                    for (auto i : objectList)
                    {
                        // std::cout << "NOT EMPTY " << std::endl;
                        // std::cout << "POSITION: " << i.coordinates.x << 
                        //     " " << i.coordinates.y << std::endl;

                        // DrawTexture(*(i->texture), i->coordinates.x, i->coordinates.y, Color{255,255,255,255});
                        int flipped = 1;
                        if (i->isFlipped)
                            flipped *= -1;
                        DrawTextureRec(*(i->texture), {0,0,(float)flipped*(i->texture)->width,(float)(i->texture)->height}, {i->coordinates.x, i->coordinates.y}, Color{255,255,255,255});
                    }

                EndMode2D();

                this->resetCollisions();
                this->resetMovement();

                DrawText(("HP: " + std::to_string(((Entity*)(this->objectMap[this->targetId]))->curHP)).c_str(), 10, 42, 32, RED);

                // For level-design purposes
                int xc, yc;
                int boost_x = 0, boost_y = 0;
                if (this->activeCamera.target.x < 0)
                {
                    boost_x++;
                }
                if (this->activeCamera.target.y < 0)
                {
                    boost_y++;
                }

                DrawText(("BLOCK X: " + std::to_string(
                    (int)(this->activeCamera.target.x / 200) - boost_x
                )).c_str(), 10, 74, 32, GREEN);

                DrawText(("BLOCK Y: " + std::to_string(
                    (int)(this->activeCamera.target.y / 200) - boost_y
                )).c_str(), 10, 106, 32, GREEN);
            }

        }

        DrawText("PROJECT PENTAGRAM", 10, 10, 32, DARKPURPLE);


    EndDrawing();
}
