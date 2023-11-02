#include "controls.hpp"

void Controls::moveWASD(const llint t_id, GraphicsHandler* graphics)
{
    if (IsKeyDown(KEY_W))
    {
        graphics->moveTextureDelta(t_id, 0, -1 * CHARACTER_SPEED);
    }
    if (IsKeyDown(KEY_S))
    {
        graphics->moveTextureDelta(t_id, 0, 1 * CHARACTER_SPEED);
    } 
    if (IsKeyDown(KEY_A))
    {
        graphics->moveTextureDelta(t_id, -1 * CHARACTER_SPEED, 0);
    } 
    if (IsKeyDown(KEY_D))
    {
        graphics->moveTextureDelta(t_id, 1 * CHARACTER_SPEED, 0);
    } 
}