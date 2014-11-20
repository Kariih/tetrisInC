#include "inc/fmod.h"
#include "inc/fmod_common.h"

int main()
{
    FMOD_SYSTEM     *system;
    FMOD_SOUND      *sound;
    FMOD_CHANNEL    *channel = 0;
    FMOD_RESULT       result;
    unsigned int      version;
    void             *extradriverdata = 0;
    
    Common_Init(&extradriverdata);

    /*
        Create a System object and initialize
    */
    result = FMOD_System_Create(&system);

    result = system->getVersion(&version);

    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);

    result = system->createSound(Common_MediaPath("Tetris.wav"), FMOD_LOOP_NORMAL, 0, &sound);
    /*
        Main loop
    */
    while(1)
    {
        Common_Update();

      	result = system->playSound(sound, 0, false, &channel);

        result = system->update();
 
}
    result = sound->release();


    Common_Close();

    return 0;
}