#include <iostream>
#include "./utils/Debug.h"


#include "./debug/LoggingManager.h"
#include "./input/InputManager.h"
#include "./render/RenderManager.h"
#include "./resources/TextFileManager.h"
#include "./resources/TextureManager.h"
#include "./time/TimeManager.h"
#include "./window/WindowManager.h"
#include "./os/OSManager.h"
#include "./sprites/SpriteManager.h"



void start_up() {
    LoggingManager::get().start_up();
    TextFileManager::get().start_up();
    TextureManager::get().start_up();
    TimeManager::get().start_up();
    WindowManager::get().start_up();
    OSManager::get().start_up();
    RenderManager::get().start_up();
    InputManager::get().start_up();
    SpriteManager::get().start_up();
}

void shut_down() {
    SpriteManager::get().shut_down();
    InputManager::get().shut_down();
    RenderManager::get().shut_down();
    OSManager::get().shut_down();
    WindowManager::get().shut_down();
    TimeManager::get().shut_down();
    TextureManager::get().shut_down();
    TextFileManager::get().shut_down();
    LoggingManager::get().shut_down();
}


inline void tick() {

    TimeManager::get().begin_tick();
    {    
        WindowManager::get().manage_events();
    }
    TimeManager::get().end_tick();

    /*
    @@NOTE
    Use this to know the time needed to update the game (without rendering).
    Bear (ROAR!) in mind that using this will tank framerate since we are basically
    printing a number every frame and maybe writing it to a log file, which takes a ton
    of time we don't want to spend in the middle of a frame. That said, the reading for
    how lonk we took to tick the game is still right, it is just that the framerate will
    be bad and keep getting worse with each printing since we are accumulating stuff to print
    to standard output.
    */
    //LOG("Tick Time: " << TimeManager::get().get_tick_ms());
}

inline void render() {

    WindowManager::get().fill_events();
    RenderManager::get().begin_frame();
    /*
    Everything that renders something goes in the
    next scope
    */
    {
        auto render_target = RenderManager::get().get_main_render_target();

        if (render_target) {

            SpriteManager::get().render_main_sprite(render_target);

        }
    }
    RenderManager::get().end_frame();

}


int main() {

    start_up();

    TimeManager::get().end_frame();

    while (WindowManager::get().is_window_open()) {
        tick();
        render();
    }
    shut_down();

    //press_to_continue();
    return EXIT_SUCCESS;
}