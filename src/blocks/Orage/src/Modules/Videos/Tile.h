//
//  Tile.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-18.
//
//

#ifndef Tile_hpp
#define Tile_hpp

#include "ModuleVideo.h"
#include "DataRange.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Tile : public ModuleVideo{
        float _tileSizeX = .0f;
        float _tileSizeY = .0f;
        
        struct DATA {
            RANGE tileSizeX;
            RANGE tileSizeY;
            RANGE tileRotation;
            RANGE tileCenterX;
            RANGE tileCenterY;
            RANGE preRotation;
            RANGE postRotation;
            RANGE rotCenterX;
            RANGE rotCenterY;
            DATA():
                tileSizeX(0.0f, -1.0f, 1.0f),
                tileSizeY(0.0f, -1.0f, 1.0f),
                tileRotation(0.0f, .0f, 1.0f),
                tileCenterX(0.5f, .0f, 1.0f),
                tileCenterY(0.5f, .0f, 1.0f),
                preRotation(0.0f, .0f, 1.0f),
                postRotation(0.0f, .0f, 1.0f),
                rotCenterX(0.5f, .0f, 1.0f),
                rotCenterY(0.5f, .0f, 1.0f)
            {};
            DATA(JsonTree data):
                tileSizeX(data.getChild("tileSizeX")),
                tileSizeY(data.getChild("tileSizeY")),
                tileRotation(data.getChild("tileRotation")),
                tileCenterX(data.getChild("tileCenterX")),
                tileCenterY(data.getChild("tileCenterY")),
                preRotation(data.getChild("preRotation")),
                postRotation(data.getChild("postRotation")),
                rotCenterX(data.getChild("rotCenterX")),
                rotCenterY(data.getChild("rotCenterY"))
            {}
        };
        DATA data;
        
        struct S_DATA {
            float tileSizeX;
            float tileSizeY;
            float tileRotation;
            float tileCenterX;
            float tileCenterY;
            float preRotation;
            float postRotation;
            float rotCenterX;
            float rotCenterY;
            int modifier = 0;
        };
        S_DATA sData;
        
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        
        Tile(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        
        virtual ~Tile(){
            data.~DATA();
            dataUbo.reset();
            mFbo.reset();
            mFbo2.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        typedef std::shared_ptr<class Tile> TileRef;
        gl::Context * mMainWinCtx;
        static TileRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            Tile::COUNT++;
            return TileRef( new Tile( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
        }
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "Tile"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(data.tileSizeX.getData("tileSizeX", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("X Tile Limiter"))));
                sub.addChild(data.tileSizeY.getData("tileSizeY", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Y Tile Limiter"))));
                sub.addChild(data.tileRotation.getData("tileRotation", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Mirror Limiter"))));
                sub.addChild(data.tileCenterX.getData("tileCenterX", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("X center Limiter"))));
                sub.addChild(data.tileCenterY.getData("tileCenterY", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Y center Limiter"))));
                sub.addChild(data.preRotation.getData("preRotation", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Tile Rot Limiter"))));
                sub.addChild(data.postRotation.getData("postRotation", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("All Rot Limiter"))));
                sub.addChild(data.rotCenterX.getData("rotCenterX", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("X Rot Center Limiter"))));
                sub.addChild(data.rotCenterY.getData("rotCenterY", std::dynamic_pointer_cast<Rangef>(mUi->getSubView("Y Rot Center Limiter"))));
                
                obj.pushBack(sub);
                return obj;
            }
        };
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class Tile> TileRef;
}

#endif /* Tile_hpp */
