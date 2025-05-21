#include <sgg/graphics.h>
#include "gamestate.h"


void draw()
{
    GameState::getInstance()->draw(); // kaloume thn draw apo thn GameState
}

void update(float dt)
{
    GameState::getInstance()->update(dt); // kaloume thn update apo thn GameState
}


int main(int argc, char** argv)
{
    graphics::createWindow(1000, 600, "Wall-E's Quest for the Last Plant"); // ftiaxnei parathyro (platos, upsos, titlos)

    GameState::getInstance()->init();

    graphics::setDrawFunction(draw); // draw
    graphics::setUpdateFunction(update); // update

    graphics::setCanvasSize(GameState::getInstance()->getCanvasWidth(), GameState::getInstance()->getCanvasHeight()); // diastaseis tou camva
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT); // allagh klimakas sto periexomeno tou parathyrou

    graphics::startMessageLoop(); //vrogxos lhpshs mynhmatwn apo thn vivliothiki
	return 0;
}
