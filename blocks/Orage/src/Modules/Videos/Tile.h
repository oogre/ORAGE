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

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class Tile : public ModuleVideo{
        float _tileSizeX = .0f;
        float _tileSizeY = .0f;

        struct DATA {
            float tileSizeX = .0f;
            float tileSizeY = .0f;
            float tileRotation = .0f;
            float tileCenterX = .5f;
            float tileCenterY = .5f;
            float preRotation = .0f;
            float postRotation = .0f;
            float rotCenterX = .5f;
            float rotCenterY = .5f;
            int modifier = 0;
            DATA(){};
            DATA(JsonTree data):
                tileSizeX(data.getChild("tileSizeX").getValue<float>()),
                tileSizeY(data.getChild("tileSizeY").getValue<float>()),
                tileRotation(data.getChild("tileRotation").getValue<float>()),
                tileCenterX(data.getChild("tileCenterX").getValue<float>()),
                tileCenterY(data.getChild("tileCenterY").getValue<float>()),
                preRotation(data.getChild("preRotation").getValue<float>()),
                postRotation(data.getChild("postRotation").getValue<float>()),
                rotCenterX(data.getChild("rotCenterX").getValue<float>()),
                rotCenterY(data.getChild("rotCenterY").getValue<float>()),
                modifier(data.getChild("modifier").getValue<int>())
            {}
        };
        DATA data;
        
        
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
                sub.addChild(JsonTree("tileSizeX", data.tileSizeX));
                sub.addChild(JsonTree("tileSizeY", data.tileSizeY));
                sub.addChild(JsonTree("tileRotation", data.tileRotation));
                sub.addChild(JsonTree("tileCenterX", data.tileCenterX));
                sub.addChild(JsonTree("tileCenterY", data.tileCenterY));
                sub.addChild(JsonTree("preRotation", data.preRotation));
                sub.addChild(JsonTree("postRotation", data.postRotation));
                sub.addChild(JsonTree("rotCenterX", data.rotCenterX));
                sub.addChild(JsonTree("rotCenterY", data.rotCenterY));
                sub.addChild(JsonTree("modifier", data.modifier));
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
