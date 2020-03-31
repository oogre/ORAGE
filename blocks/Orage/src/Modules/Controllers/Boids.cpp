//
//  Boids.cpp
//  PianoProjete
//
//  Created by Vincent Evrard on 9/12/17.
//
//

#include "Boids.hpp"
//
//  Lfos.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-22.
//
//

using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Boids::COUNT = 0;
    Boids::Boids( std::string name, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleCommon(name+" "+ tools.to_roman(Boids::COUNT), origin, size){
        
        this->mMainWinCtx = mMainWinCtx;
        for(int i = 0 ; i < num ; i ++){
            outputs.push_back(vec3(Rand::randFloat(0.0f, 1.f) * size.x, Rand::randFloat(0.0f, 1.f) * size.y, 0.0f));
            forces.push_back(vec3(0.0f, 0.0f, 0.0f));
            attractor_forces.push_back(vec3(0.0f, 0.0f, 0.0f));
            repulsor_forces.push_back(vec3(0.0f, 0.0f, 0.0f));
            imitator_forces.push_back(vec3(0.0f, 0.0f, 0.0f));
        }
    }
    
    void Boids::setup(){
        ModuleCommon::setup();
        setupUI();
    }
    
    void Boids::update(){
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleCommon::update();
        
        for(int i = 0 ; i < num ; i ++){
            vec3 * pos = &(outputs.at(i));
            vec3 * force = &(forces.at(i));
            
            vec3 * attractor = &(attractor_forces.at(i));
            *attractor *= boidDatas.attractor_inertie;

            vec3 * repulsor = &(repulsor_forces.at(i));
            *repulsor *= boidDatas.repulsor_inertie;

            vec3 * imitator = &(imitator_forces.at(i));
            *imitator *= boidDatas.imitator_inertie;
            
            int countAttrctor = 0 ;
            int countRepulsor = 0 ;
            int countImitator = 0 ;
            
            for(int j = 0 ; j < num ; j ++){
                if(i == j)continue;
                vec3 * other_pos = &(outputs.at(j));
                vec3 * other_force = &(forces.at(j));

                float d = (other_pos->x - pos->x) * (other_pos->y + pos->y);
                if (d < boidDatas.attractor_dist) {
                    countAttrctor++;
                    *attractor += *other_pos;
                }
                if (d < boidDatas.repulsor_dist) {
                    *repulsor -= (*other_pos - *pos);
                }
                if (d < boidDatas.imitator_dist) {
                    countImitator++;
                    *imitator += *other_force;
                }
            }
            
            if (countAttrctor != 0) {
                *attractor /= countAttrctor;
            }
            *attractor -= *pos;
            
            if (countAttrctor != 0) {
                *imitator /= countImitator;
            }
            *imitator -= *force;

            *force +=   (*attractor / boidDatas.attractor_mult) +
                        (*repulsor / boidDatas.repulsor_mult) +
                        (*imitator / boidDatas.imitator_mult);
            *pos += *force;

            if (pos->x > size.x) {
                pos->x = 0 ;
            } else if (pos->x < 0) {
                pos->x = size.x;
            }
            if (pos->y > size.y) {
                pos->y = 0;
            } else if (pos->y < 0) {
                pos->y = size.y;
            }
            
        }
    }
    
    void Boids::setupUI(){
        ModuleCommon::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .2f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.1f, .9f, 1.f, 1.f)));
        
        string id = toString(COUNT);
        
        
        //CanvasRef v = Canvas::create("BOID"+id, ci::app::getWindow());
        
        ToggleRef on = Toggle::create("ON "+id, &(boidDatas.active), Button::Format().label(true));

        mUi->addSubViewDown(on);
        mUi->addSubViewToHeader(on);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "attractor_mult "+id, this->id, &(boidDatas.attractor_mult), 1, 1000, 0, true);
        tools.addSlider(mUi, "attractor_dist "+id, this->id, &(boidDatas.attractor_dist), 1, 1000, 0, true);
        tools.addSlider(mUi, "attractor_inertie  "+id, this->id, &(boidDatas.attractor_inertie ), 0, 1, 0, true);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "repulsor_mult "+id, this->id, &(boidDatas.repulsor_mult), 1, 1000, 0, true);
        tools.addSlider(mUi, "repulsor_dist "+id, this->id, &(boidDatas.repulsor_dist), 1, 1000, 0, true);
        tools.addSlider(mUi, "repulsor_inertie  "+id, this->id, &(boidDatas.repulsor_inertie ), 0, 1, 0, true);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "imitator_mult "+id, this->id, &(boidDatas.imitator_mult), 1, 1000, 0, true);
        tools.addSlider(mUi, "imitator_dist "+id, this->id, &(boidDatas.imitator_dist), 1, 1000, 0, true);
        tools.addSlider(mUi, "imitator_inertie  "+id, this->id, &(boidDatas.imitator_inertie ), 0, 1, 0, true);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        for(int i = 0 ; i < num ; i ++){
            tools.addSlider(mUi, "output "+id+" "+toString(i)+" x", this->id, &(outputs.at(i).x), 0, size.x, 0, true);
            tools.addSlider(mUi, "output "+id+" "+toString(i)+" y", this->id, &(outputs.at(i).y), 0, size.y, 0, true);
        }
        
        mUi->setMinified(false);
    }
}
