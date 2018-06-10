//
//  MouseEvent.hpp
//  Orage
//
//  Created by Vincent Evrard on 6/06/18.
//

#pragma once


namespace ogre {
    struct MouseEvent{
        const static unsigned char ShiftDown = 1<<0;
        const static unsigned char AltDown = 1<<1;
        const static unsigned char ControlDown = 1<<2;
        const static unsigned char MetaDown = 1<<3;
        const static unsigned char AccelDown = 1<<4;
        
        MouseEvent(std::string type, cinder::ivec2 location, unsigned char modifier, float value = 0.0f){
            this->type = type;
            this->position = location;
            this->time = cinder::app::getElapsedSeconds();
            this->value = value;
            this->modifier = modifier;
        }
        cinder::ivec2 position;
        float time;
        float value;
        std::string type;
        unsigned char modifier;
        //! Returns whether the Shift key was pressed during the event.
        bool        isShiftDown() const { return (modifier & ShiftDown) ? true : false; }
        //! Returns whether the Alt (or Option) key was pressed during the event.
        bool        isAltDown() const { return (modifier & AltDown) ? true : false; }
        //! Returns whether the Control key was pressed during the event.
        bool        isControlDown() const { return (modifier & ControlDown) ? true : false; }
        //! Returns whether the meta key was pressed during the event. Maps to the Windows key on Windows and the Command key on Mac OS X.
        bool        isMetaDown() const { return (modifier & MetaDown) ? true : false; }
        //! Returns whether the accelerator key was pressed during the event. Maps to the Control key on Windows and the Command key on Mac OS X.
        bool        isAccelDown() const { return (modifier & AccelDown) ? true : false; }
    };
}

