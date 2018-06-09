//
//  MouseEvent.hpp
//  Orage
//
//  Created by Vincent Evrard on 6/06/18.
//

#pragma once


namespace ogre {
    struct MouseEvent{
        MouseEvent(std::string type, cinder::ivec2 location){
            this->type = type;
            this->position = location;
            this->time = cinder::app::getElapsedSeconds();
        }
        cinder::ivec2 position;
        float time;
        std::string type;
    };
}

