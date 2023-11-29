#include "controls.hpp"

void Controls::moveWASD(const llint t_id, GraphicsHandler* graphics)
{
    float mult = 1.f;
    uint numOfInp = 0;
    uint e = 0;
    bool move[4] = {false, false, false, false};

    std::vector<uint> vec;
    vec.insert(vec.end(),
        {0, 1, 2, 3, 
        1, 0, 3, 2, 
        2, 3, 1, 0, 
        3, 2, 0, 1}
    );
    AbelianGroup G(vec); 
    // Abelian group with this signature is isomorphic to
    // {{1, -1, i, -i}, *, 1} group where i is imaginary unit 

    if (IsKeyDown(KEY_W))
    {
        numOfInp++;
        e = G.product(e, 2);
        move[2] = true;
    }
    if (IsKeyDown(KEY_S))
    {
        numOfInp++;
        e = G.product(e, 3);
        move[3] = true;
    } 
    if (IsKeyDown(KEY_A))
    {
        numOfInp++;
        e = G.product(e, 1);
        move[1] = true;

    } 
    if (IsKeyDown(KEY_D))
    {
        numOfInp++;
        e = G.product(e, 0);
        move[0] = true;

    }


    if (numOfInp == 2 && (e == 2 || e == 3))
    {
        mult *= sqrt(2) / 2;
    }


    if (move[2])
    {
        graphics->moveTextureDelta(t_id, 0, -1 * CHARACTER_SPEED * mult);
    }
    if (move[3])
    {
        graphics->moveTextureDelta(t_id, 0, 1 * CHARACTER_SPEED * mult);
    } 
    if (move[1])
    {
        graphics->moveTextureDelta(t_id, -1 * CHARACTER_SPEED * mult, 0);
    } 
    if (move[0])
    {
        graphics->moveTextureDelta(t_id, 1 * CHARACTER_SPEED * mult, 0);
    }
}

void Controls::attackLMC(const llint t_id, GraphicsHandler* graphics)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        graphics->triggerAttack(t_id);
    }
}

void Controls::attackRMC(const llint t_id, GraphicsHandler* graphics)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        Animation at;

        std::list<Texture2D*> txt1;
        std::list<std::string> nTxt1;
        std::vector<double> tQ1;
        for(int i = 1; i <= 5; ++i) 
        {
            txt1.push_back(graphics->getTexture("rA" + std::to_string(i) + ".png"));
            nTxt1.push_back("rA" + std::to_string(i) + ".png");
            tQ1.push_back(0.05);
        }

        at.textureNames = nTxt1;
        at.textures = txt1;
        at.timeQueues = tQ1;
        graphics->playAnimationOnce({0,0}, at);
        // std::cout << "played" << std::endl;
    }
}

void Controls::vibrate(const llint t_id, GraphicsHandler* graphics, int intencity)
{
    int r = GetRandomValue(0, 3);
    switch (r)
    {
        case 0:
            graphics->moveTextureDelta(t_id, intencity, 0);
            break;
        case 1:
            graphics->moveTextureDelta(t_id, intencity * -1, 0);
            break;
        case 2:
            graphics->moveTextureDelta(t_id, 0, intencity);
            break;
        case 3:
            graphics->moveTextureDelta(t_id, 0, intencity * -1);
            break;
        default:
            break;
    }
}