/*!
**  \file Main.cpp
**  \brief Main functions and logic for the progam.
**
**  \author Andrew James
*/

/*!
**  \mainpage ITC363 Assignment 2
**
**  The task was to create a program that would let you create "pipes" made
**   from connected cubes. I have been using the framework as a testbed for
**   some camera related code since handing in the assignment, and I fixed
**   an undiscovered bug.
**
**  See Documentation/Assignment 2.pdf for further details and the detailed
**   user manual. Basic controls are WASD to move the camera, MMB + mouse
**   movement to pan the view, N creates a pipe and 1-6 add boxes to the pipe.
*/

#include <iostream>

#include <cmath>

#include <list>

#include <SDL.h>
#include <SDL_OpenGL.h>

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Box.h"
#include "Camera.h"
#include "DynamicCamera.h"
#include "ShakyCamera.h"
#include "ElasticCamera.h"
#include "ElasticThirdPersonCamera.h"
#include "ElasticShakyThirdPersonCamera.h"
#include "DebugObject.h"

// Prototpes
// Input handling.
void handle_key_down(SDL_keysym* keysym);
void handle_key_up(SDL_keysym* keysym);
void handle_mouse_button_down(Uint8 which, Uint8 button, Uint16 x, Uint16 y);
void handle_mouse_button_up(Uint8 which, Uint8 button, Uint16 x, Uint16 y);
void handle_mouse_motion(Uint8 state, Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel);
void process_events(void);
void quit_func(int code);

// Graphics.
void setup_opengl(int width, int height);
void render(void);

// Application logic.
void update(float dt);
void add_box(boost::shared_ptr<Box> box);
void delete_active(bool ignoreClashes);

// Globals (will be moved to classes after testing.)
//! \todo Remove the globals
boost::weak_ptr<Camera> gCamera;
Vector3 gCameraVelocity;
bool gOrbit = false;
std::vector<Vector3> gObserverPoints;
static const float CAMERATHRESHOLD = 10.0f;

std::list<boost::shared_ptr<Box> > gPipes;          //!< List of all pipes.
std::list<boost::shared_ptr<Box> >::iterator gHead; //!< Pointer to the head of the active pipe (used to save searching for it when changing the active pipe).
boost::weak_ptr<Box> gLast;                         //!< Pointer to the last box created (used when adding a box to a pipe).
boost::weak_ptr<Box> gActive;                       //!< Pointer to the active box (for rotating and translating pipes, and twisting the pipe).
static const float PIPEMOVETHRESHOLD = 50.0f;
static const float PIPEPANTHRESHOLD = 5.0f;

int main(int, char**)
{   // First, initialize SDL's video subsystem.
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {   // Failed, exit.
        std::cerr << "Video initialization failed: " << SDL_GetError() << std::endl;
        quit_func(1);
    }

    // Information about the current video settings.
    const SDL_VideoInfo* info = SDL_GetVideoInfo();

    if(!info)
    {   // This should probably never happen.
        std::cerr << "Video query failed: " << SDL_GetError() << std::endl;
        quit_func(1);
    }

    int width = 640;
    int height = 480;
    int bpp = info->vfmt->BitsPerPixel;

    // Sets a few basic OpenGL attributes, such as colour depth,
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

    //  the resolution of the depth buffer,
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    //  and the fact that we want double buffering.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Tell SDL that we're gonna use OpenGL to control video output.
    int flags = SDL_OPENGL;

    // Set the video mode
    if(SDL_SetVideoMode(width, height, bpp, flags) == 0)
    {   // If we can't set our desired video mode, handle the error like a man!
        //  That is, complain about it then quit.
        std::cerr << "Video mode set failed: " << SDL_GetError() << std::endl;
        quit_func(1);
    }

    // So far, so good. Do the OpenGL initialisation stuff.
    setup_opengl(width, height);

    // Create a camera and move it back a few notches, so that we can see the scene immediatly.
    boost::shared_ptr<Camera> camera(new ElasticShakyThirdPersonCamera(Vector3(0.0f, 0.0f, 5.0f), 1.0f, 2.0f, 15.0f));
    gCamera = camera;

    gObserverPoints.push_back(Vector3(5.0f, 5.0f, 5.0f));
    gObserverPoints.push_back(Vector3(-5.0f, 5.0f, 5.0f));
    gObserverPoints.push_back(Vector3(-5.0f, 5.0f, -5.0f));
    gObserverPoints.push_back(Vector3(5.0f, 5.0f, -5.0f));

    boost::posix_time::ptime previous(boost::posix_time::microsec_clock::local_time());

    // Just a simple application loop.
    while(true)
    {   // Process incoming events.
        process_events();

        boost::posix_time::ptime current(boost::posix_time::microsec_clock::local_time());

        // dt is the time elapsed since the last update in seconds.
        // I've switched to milliseconds instead of nanoseconds because a float
        //  does not have the precision to store time differences on a nanosecond scale.
        //  I was getting jerky movement which I figure were from rounding errors here.
        float dt = static_cast<float>(boost::posix_time::time_period(previous, current).length().total_milliseconds()) / 1000.0f ;

        previous = current;

        // Update the camera position.
        update(dt);

        // Draw the screen.
        render();
    }

    return 0;
}


void setup_opengl(int width, int height)
{
    static const float pi = 3.14159f;
    float ratio = static_cast<float>(width) / static_cast<float>(height);

    // Set the near and far view planes, plus the field of view (how wide the viewing angle is).
    float znear = 1.0f;
    float zfar = 1024.0f;
    float fov = 48.0f;

    // Our shading model--Gouraud (smooth).
    glShadeModel(GL_SMOOTH);

    // Culling.
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Set the clear color.
#ifdef _DEBUG
    glClearColor(0, 1, 0, 0);
#else
    glClearColor(0, 0, 0, 0);
#endif

    // Setup our viewport.
    glViewport(0, 0, width, height);

    // Set the projection matrix.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ymax = znear * tanf(fov * 2.0f * pi / 360.0f);
    float xmax = ymax * ratio;
    glFrustum(-xmax, xmax, -ymax, ymax, znear, zfar);

    return;
}

void handle_key_down(SDL_keysym* keysym)
{   // Any actions handled here either will cause any future events to be ignored (because we quit)
    //  or repeated actions are desired.
    switch(keysym->sym)
    {   // Escape is our gtfo key.
    case SDLK_ESCAPE:
        quit_func(0);
        break;


        // Home and End move to the first and last pipe.
    case SDLK_HOME:
        {
            if(boost::shared_ptr<Box> active = gActive.lock())
            {
                active->Active(false);  // Deactivate the active box.
            }

            gHead = gPipes.begin(); // Grad the list head.
            (*gHead)->Active(true); // Make the head the new active element.
            gActive = (*gHead);
        }
        break;

    case SDLK_END:
        {
            if(boost::shared_ptr<Box> active = gActive.lock())
            {
                active->Active(false);  // Deactivate the active box.
            }

            gHead = --(gPipes.rbegin().base()); // Grab the list tail.
            (*gHead)->Active(true);         // Make the tail the new active element.
            gActive = (*gHead);
        }
        break;


        // Page up and Page down move to the previous and next pipe.
    case SDLK_PAGEUP:
        {
            if(boost::shared_ptr<Box> active = gActive.lock())
            {
                if(gHead != gPipes.begin())
                {   // As long as we aren't already at the first pipe,
                    --gHead;    //  move back a step.
                }

                active->Active(false);
                (*gHead)->Active(true);
                gActive = *gHead;
            }
        }
        break;

    case SDLK_PAGEDOWN:
        {
            if(boost::shared_ptr<Box> active = gActive.lock())
            {
                if(++gHead == gPipes.end())
                {   // If moving forward puts us past the end of the list,
                    gHead = --(gPipes.rbegin().base()); //  grab the last element.
                }

                active->Active(false);
                (*gHead)->Active(true);
                gActive = *gHead;
            }
        }
        break;


        // Plus and minus move forward and backward through the current pipe.
    case SDLK_PLUS:     // For some reason SHIFT = does not generate a SDLK_PLUS event, so..
    case SDLK_EQUALS:   // We handle SDLK_EQUALS as well.
    case SDLK_KP_PLUS:
        {   // Moving up in the world.
            if(boost::shared_ptr<Box> active = gActive.lock())
            {   // If the active pointer is valid (it should never not be, but just in case...)
                if(boost::shared_ptr<Box> next = active->Next())
                {   // If we aren't already at the end of the list.
                    active->Active(false);
                    next->Active(true);
                    gActive = next;
                }
            }
        }
        break;

    case SDLK_MINUS:
    case SDLK_KP_MINUS:
        {   // Taking a step backward.
            if(boost::shared_ptr<Box> active = gActive.lock())
            {   // Again check if the active pointer is valid.
                if(boost::shared_ptr<Box> prev = active->Prev())
                {   // And that it's not the head of the list.
                    active->Active(false);
                    prev->Active(true);
                    gActive = prev;
                }
            }
        }
        break;


        // WASD, the arrow keys, and space or ctrl will move the camera around.
        //  If at some point I decide to enable key repeating, this code needs to be modified.
    case SDLK_w:
    case SDLK_UP:
        {   // Move the camera forward.
            gCameraVelocity += Vector3(0.0f, 0.0f, 1.0f);
        }
        break;

    case SDLK_s:
    case SDLK_DOWN:
        {   // Move the camera backward.
            gCameraVelocity -= Vector3(0.0f, 0.0f, 1.0f);
        }
        break;

    case SDLK_d:
    case SDLK_RIGHT:
        {   // Move the camera right.
            gCameraVelocity += Vector3(1.0f, 0.0f, 0.0f);
        }
        break;

    case SDLK_a:
    case SDLK_LEFT:
        {   // Move the camera left.
            gCameraVelocity -= Vector3(1.0f, 0.0f, 0.0f);
        }
        break;

    case SDLK_SPACE:
        {   // Move the camera up.
            gCameraVelocity += Vector3(0.0f, 1.0f, 0.0f);
        }
        break;

    case SDLK_LCTRL:
    case SDLK_RCTRL:
        {   // Move the camera down.
            gCameraVelocity -= Vector3(0.0f, 1.0f, 0.0f);
        }
        break;

    case SDLK_RETURN:
        {
            if(boost::shared_ptr<ShakyCamera> sCamera = boost::shared_dynamic_cast<ShakyCamera>(gCamera.lock()))
            {
                sCamera->Shake(100.0f);
            }
        }
        break;

    default:
        break;
    }

    return;
}

void handle_key_up(SDL_keysym* keysym)
{   // I'm handling these functions in SDL_KEYUP as multiple keydown events can be generated
    //  if a key is held down, and I don't really want these actions to be blindly repeated.
    switch(keysym->sym)
    {   // The n key creates a new pipe
    case SDLK_n:
        {   // Create a new pipe.
            boost::shared_ptr<MasterBox> mBox(new MasterBox(Vector3(0.0f)));

            if(boost::shared_ptr<Box> active = gActive.lock())
            {
                active->Active(false);
            }

            mBox->Active(true);

            gHead = gPipes.insert(gPipes.end(), mBox);
            gLast = mBox;
            gActive = mBox;
        }
        break;


        // Keys 1-6 add boxes.
    case SDLK_1:
        {   // Add to the top of the previous box.
            boost::shared_ptr<Box> box(new Box(0.0f, Vector3(0.0f, 1.0f, 0.0f)));

            add_box(box);
        }
        break;

    case SDLK_2:
        {   // Add to the front of the previous box.
            boost::shared_ptr<Box> box(new Box(0.0f, Vector3(0.0f, 0.0f, 1.0f)));

            add_box(box);
        }
        break;

    case SDLK_3:
        {   // Add to the right of the previous box.
            boost::shared_ptr<Box> box(new Box(0.0f, Vector3(1.0f, 0.0f, 0.0f)));

            add_box(box);
        }
        break;

    case SDLK_4:
        {   // Add to the left of the previous box.
            boost::shared_ptr<Box> box(new Box(0.0f, Vector3(-1.0f, 0.0f, 0.0f)));

            add_box(box);
        }
        break;

    case SDLK_5:
        {   // Add to the back of the previous box.
            boost::shared_ptr<Box> box(new Box(0.0f, Vector3(0.0f, 0.0f, -1.0f)));

            add_box(box);
        }
        break;

    case SDLK_6:
        {   // Add to the bottom of the previous box.
            boost::shared_ptr<Box> box(new Box(0.0f, Vector3(0.0f, -1.0f, 0.0f)));

            add_box(box);
        }
        break;


        // Del and Backspace attempt to delete a box.
    case SDLK_DELETE:
    case SDLK_BACKSPACE:
        {   // Ternary statement is there to avoid a compiler warning,
            //  apparently casting an int to a bool causes performance issues,
            //  but actually checking if an unsigned value is not zero,
            //  then passing a value is faster.
            delete_active((keysym->mod & KMOD_SHIFT) > 0 ? true : false);
        }
        break;


        // WASD, the arrow keys, and space or ctrl will move the camera around.
        // When released, we need to negate their effect.
    case SDLK_w:
    case SDLK_UP:
        {   // Stop moving the camera forward.
            gCameraVelocity -= Vector3(0.0f, 0.0f, 1.0f);
        }
        break;

    case SDLK_s:
    case SDLK_DOWN:
        {   // Stop moving the camera backward.
            gCameraVelocity += Vector3(0.0f, 0.0f, 1.0f);
        }
        break;

    case SDLK_d:
    case SDLK_RIGHT:
        {   // Stop moving the camera right.
            gCameraVelocity -= Vector3(1.0f, 0.0f, 0.0f);
        }
        break;

    case SDLK_a:
    case SDLK_LEFT:
        {   // Stop moving the camera left.
            gCameraVelocity += Vector3(1.0f, 0.0f, 0.0f);
        }
        break;

    case SDLK_SPACE:
        {   // Stop moving the camera up.
            gCameraVelocity -= Vector3(0.0f, 1.0f, 0.0f);
        }
        break;

    case SDLK_LCTRL:
    case SDLK_RCTRL:
        {   // Stop moving the camera down.
            gCameraVelocity += Vector3(0.0f, 1.0f, 0.0f);
        }
        break;

    case SDLK_RETURN:
        {
            if(boost::shared_ptr<ShakyCamera> sCamera = boost::shared_dynamic_cast<ShakyCamera>(gCamera.lock()))
            {
                sCamera->Shake(0.0f);
            }
        }
        break;


    case SDLK_TAB:
        {   // Toggles between orbit behaviour and freeform movement.
            gOrbit = !gOrbit;
            if(boost::shared_ptr<Camera> camera = gCamera.lock())
            {
                camera->LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
                if(boost::shared_ptr<ElasticCamera> eCamera = boost::shared_dynamic_cast<ElasticCamera>(camera))
                {
                    eCamera->SetPosition(eCamera->Position());
                }
            }
        }
        break;


    case SDLK_o:
        {
            if(boost::shared_ptr<Camera> camera = gCamera.lock())
            {
                gObserverPoints.push_back(camera->Position());
            }
        }
        break;


    default:
        break;
    }

    return;
}

void handle_mouse_button_down(Uint8 which, Uint8 button, Uint16 x, Uint16 y)
{
    switch(button)
    {
    case SDL_BUTTON_LEFT:
    case SDL_BUTTON_MIDDLE:
    case SDL_BUTTON_RIGHT:
        {   // If any of the major mouse buttons are pressed we want to hide the cursor and keep it in the window.
            SDL_ShowCursor(SDL_DISABLE);
            SDL_WM_GrabInput(SDL_GRAB_ON);
        }
        break;


    case SDL_BUTTON_WHEELUP:
        {
            if(!(SDL_GetMouseState(NULL, NULL) & (SDL_BUTTON(1) | SDL_BUTTON(3))))
            {   // If we aren't trying to modify the active chain.
                if(boost::shared_ptr<Box> active = gActive.lock())
                {   // Rotate the activebox clockwise (or counter? I don't know) as we scroll up.
                    active->Rotate(1.0f);
                }
                break;
            }

            if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1))
            {   // Trying to translate the active chain.
                if(boost::shared_ptr<Box> active = gActive.lock())
                {   // If there's a chain to translate of course.
                    if(boost::shared_ptr<MasterBox> head = boost::shared_dynamic_cast<MasterBox>(*gHead))
                    {
                        head->Dolly(Vector3(0.0f, 5.0f / PIPEMOVETHRESHOLD, 0.0f));
                    }
                }
            }

            if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3))
            {   // Trying to rotate the active chain.
                if(boost::shared_ptr<Box> active = gActive.lock())
                {   // If there's a chain to rotate of course.
                    if(boost::shared_ptr<MasterBox> head = boost::shared_dynamic_cast<MasterBox>(*gHead))
                    {
                        head->Yaw(Quaternion::DegreesToRadians(5.0f / PIPEPANTHRESHOLD));
                    }
                }
            }
        }
        break;

    case SDL_BUTTON_WHEELDOWN:
        {
            if(!(SDL_GetMouseState(NULL, NULL) & (SDL_BUTTON(1) | SDL_BUTTON(3))))
            {   // If we aren't trying to modify the active chain.
                if(boost::shared_ptr<Box> active = gActive.lock())
                {   // And rotate the other way as we scroll down.
                    active->Rotate(-1.0f);
                }
                break;
            }

            if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1))
            {   // Trying to translate the active chain.
                if(boost::shared_ptr<Box> active = gActive.lock())
                {   // If there's a chain to translate of course.
                    if(boost::shared_ptr<MasterBox> head = boost::shared_dynamic_cast<MasterBox>(*gHead))
                    {
                        head->Dolly(Vector3(0.0f, -5.0f / PIPEMOVETHRESHOLD, 0.0f));
                    }
                }
            }

            if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3))
            {   // Trying to rotate the active chain.
                if(boost::shared_ptr<Box> active = gActive.lock())
                {   // If there's a chain to rotate of course.
                    if(boost::shared_ptr<MasterBox> head = boost::shared_dynamic_cast<MasterBox>(*gHead))
                    {
                        head->Yaw(Quaternion::DegreesToRadians(-5.0f / PIPEPANTHRESHOLD));
                    }
                }
            }
        }
        break;

    default:
        break;
    }

    return;
}

void handle_mouse_button_up(Uint8 which, Uint8 button, Uint16 x, Uint16 y)
{
    switch(button)
    {
    case SDL_BUTTON_LEFT:
    case SDL_BUTTON_MIDDLE:
    case SDL_BUTTON_RIGHT:
        {
            if(!(SDL_GetMouseState(NULL, NULL) & (SDL_BUTTON(1) | SDL_BUTTON(2) | SDL_BUTTON(3))))
            {   // All mouse buttons are up, stop grabbing input and show the cursor again.
                SDL_WM_GrabInput(SDL_GRAB_OFF);
                SDL_ShowCursor(SDL_ENABLE);
            }
        }
        break;

    default:
        break;
    }

    

    return;
}

void handle_mouse_motion(Uint8 state, Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel)
{
    if(state & SDL_BUTTON(1))
    {   // If the left mouse button is down we translate the active chain.
        if(boost::shared_ptr<Box> active = gActive.lock())
        {   // If there's a chain to translate of course.
            if(boost::shared_ptr<MasterBox> head = boost::shared_dynamic_cast<MasterBox>(*gHead))
            {
                head->Dolly(Vector3(static_cast<float>(xrel) / PIPEMOVETHRESHOLD, 0.0f, static_cast<float>(yrel) / PIPEMOVETHRESHOLD));
            }
        }
    }

    if(state & SDL_BUTTON(3))
    {   // If the right mouse button is down we rotate the active chain.
        if(boost::shared_ptr<Box> active = gActive.lock())
        {   // If there's a chain to pan of course.
            if(boost::shared_ptr<MasterBox> head = boost::shared_dynamic_cast<MasterBox>(*gHead))
            {
                head->Roll(Quaternion::DegreesToRadians(static_cast<float>(xrel) / PIPEPANTHRESHOLD));
                head->Pitch(Quaternion::DegreesToRadians(static_cast<float>(yrel) / PIPEPANTHRESHOLD));
            }
        }
    }
    if(state & SDL_BUTTON(2))
    {   // If the middle mouse button is down we pan the camera.
        if(boost::shared_ptr<Camera> camera = gCamera.lock())
        {   // If there's a camera to pan of course.
            camera->Pitch(Quaternion::DegreesToRadians(static_cast<float>(yrel) / CAMERATHRESHOLD));
            camera->Pan(Quaternion(Vector3(0.0f, 1.0f, 0.0f), Quaternion::DegreesToRadians(static_cast<float>(xrel) / CAMERATHRESHOLD)));
        }
    }
    return;
}

void process_events(void)
{
    // Our SDL event placeholder.
    SDL_Event event;

    // Grab all the events off the queue.
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_KEYDOWN:
            // Handle key presses.
            handle_key_down(&event.key.keysym);
            break;

        case SDL_KEYUP:
            // Handle key releases.
            handle_key_up(&event.key.keysym);
            break;

        case SDL_MOUSEBUTTONDOWN:
            // Handle mouse button presses.
            handle_mouse_button_down(event.button.which, event.button.button, event.button.x, event.button.y);
            break;

        case SDL_MOUSEBUTTONUP:
            // Handle mouse button releases.
            handle_mouse_button_up(event.button.which, event.button.button, event.button.x, event.button.y);
            break;

        case SDL_MOUSEMOTION:
            // Handle moving the mouse.
            handle_mouse_motion(event.motion.state, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
            break;

        case SDL_QUIT:
            // Handle quit requests (like Ctrl-c).
            quit_func(0);
            break;

        default:
            break;
        }
    }

    return;
}

void quit_func(int code)
{   // Quit SDL.
    SDL_Quit();

    // And exit.
    exit(code);

    return;
}


void render(void)
{
    static DebugObject axes;

    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // We don't want to modify the projection matrix.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //! \todo Move this code to a scene class.
    if(boost::shared_ptr<Camera> camera = gCamera.lock())
    {
        camera->Render();
    }

    axes.Draw();

    //! \todo Move this code to a BoxManager class.
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT, 0, Box::normals);
    glColorPointer(3, GL_FLOAT, 0, Box::colours);
    glVertexPointer(3, GL_FLOAT, 0, Box::vertices);

    for(std::list<boost::shared_ptr<Box> >::const_iterator it = gPipes.begin(); it != gPipes.end(); ++it)
    {
        (*it)->Draw();
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    SDL_GL_SwapBuffers();

    return;
}


void update(float dt)
{
    boost::shared_ptr<Camera> camera = gCamera.lock();

    if(!camera)
    {   // No camera? But how do we see!
        return;
    }

    if(gOrbit)
    {
        static unsigned int i = 0;
        static float elapsedTime = 0;
        elapsedTime += dt;
        if(elapsedTime >= 1.0f)
        {
            while(elapsedTime >= 1.0f)
            {
                elapsedTime -= 1.0f;
            }

            if(gObserverPoints.size() > 0)
            {
                ++i %= gObserverPoints.size();

                camera->MoveTo(gObserverPoints[i]);
            }
        }

        if(boost::shared_ptr<DynamicCamera> dCamera = boost::dynamic_pointer_cast<DynamicCamera>(camera))
        {
            dCamera->Update(dt);
        }
    }
    else
    {
        if(gCameraVelocity.x < -0.5f)
        {   // It's just a jump to the left.
            camera->Sway(-dt);
        }
        else if(gCameraVelocity.x > 0.5f)
        {   // Then a step to the right.
            camera->Sway(dt);
        }

        if(gCameraVelocity.y > 0.5f)
        {   // Put your hands on your hips
            camera->Heave(dt);
        }
        else if(gCameraVelocity.y < -0.5f)
        {   // Bring your knees in tight.
            camera->Heave(-dt);
        }

        if(gCameraVelocity.z > 0.5f)
        {   // But it's the pelvic thrust.
            camera->Surge(dt);
        }
        else if(gCameraVelocity.z < -0.5f)
        {   // That really drives you insaaaaaaaaane.
            camera->Surge(-dt);
        }
    }

    // LETS DO THE TIME WARP AGAIIIIIIIIIIIN!
    return;
}

void add_box(boost::shared_ptr<Box> box)
{
    if(boost::shared_ptr<Box> last = gLast.lock())
    {   // If the last pointer is still valid (will only fail if there are no boxes on screen)
        if(last->SetNext(box))
        {   // Try set the next pointer. If it fails it's probably because the next box will
            //  be drawn inside the previous box, and we don't want that.
            box->SetPrev(last);
            gLast = box;
        }
    }

    return;
}

void delete_active(bool ignoreClashes)
{
    if(boost::shared_ptr<Box> active = gActive.lock())
    {
        if(boost::shared_ptr<Box> prev = active->Prev())
        {   // If there is a previous pointer, we're deleting an element of a pipe.
            boost::shared_ptr<Box> next = active->Next();

            bool wasClash = false;  // Tracks whether there actually was a clash so that
                                    //  shift+backspace wont remove the entire tail unnecessarily.

            if(prev->SetNext(next) || (ignoreClashes && (wasClash = true)))
            {   // If there's a problem setting the next pointer
                //  (generally an invalid axis) SetNext returns false,
                //  but if we really wanna delete the box then we do it anyway.

                if(wasClash)
                {   // If there was a clash.
                    next.reset();           // Clear the pointer,
                    prev->SetNext(next);    //  and try again.

                    // This has the effect of dropping the rest of the list from the active element.
                }

                if(!next)
                {   // If there was no next pointer, we must be deleting the tail.
                    if(gLast.lock() == active)
                    {   // If we're deleting the last element, then we want to update the link.
                        gLast = prev;
                    }
                }
                else
                {   // Otherwise, make the reverse link.
                    next->SetPrev(prev);
                }

                prev->Active(true);
                gActive = prev;
            }
        }
        else
        {   // If there's no previous pointer, we must be trying to delete the head of a pipe.
            if(boost::shared_ptr<Box> next = active->Next())
            {   // If this is part of a pipe
                boost::shared_ptr<MasterBox> newHead(new MasterBox(*next));
                newHead->Active(true);
                gActive = newHead;

                if(!next->Next())
                {   // If we're making the last box created the new head we lose the pointer to it
                    //  since we recreate the object, so update the gLast pointer.
                    gLast = newHead;
                }

                std::list<boost::shared_ptr<Box> >::iterator it = gPipes.erase(gHead);
                gHead = gPipes.insert(it, newHead);
            }
            else
            {   // Must be an isolated element, just drop it.
                gHead = gPipes.erase(gHead);

                if(!(gPipes.empty()))
                {
                    if(gHead == gPipes.end())
                    {   // We're deleting the last box created, and it's the head of a list.
                        // Need to find the new last box. Thankfully this isn't hard.
                        gHead = --(gPipes.rbegin().base());
                        gLast = *gHead;
                    }

                    (*gHead)->Active(true);
                    gActive = *gHead;
                }
                else
                {   // We deleted all the pipes :(
                    gActive.reset();
                    gLast.reset();
                }
            }
        }
    }

    return;
}
