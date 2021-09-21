//
//  Wire.hpp
//  ORAGE2
//
//  Created by Vincent Evrard on 2017-04-25.
//
//

#ifndef Wire_hpp
#define Wire_hpp

#include "UI.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
class Wire {
    Path2d path;
    gl::VboMeshRef      mVboMesh;
    
    public :
    bool over = false;
    int id_slave;
    int id_master;
    Wire(ViewRef slaveBtn, ViewRef masterBtn,  int id_slave, int id_master) {
        this->slaveBtn = slaveBtn;
        this->masterBtn = masterBtn;
        this->id_slave = id_slave;
        this->id_master = id_master;
        
        path.clear();
        path.moveTo(slaveBtn->getBounds().getCenter());
        path.lineTo(masterBtn->getBounds().getCenter());
    }
    
    Wire(ViewRef slaveBtn) {
        this->slaveBtn = slaveBtn;
        path.clear();
        path.moveTo(slaveBtn->getBounds().getCenter());
    }
    
    virtual ~Wire(){
        
    }
    
    typedef std::shared_ptr<class Wire> WireRef;
    static WireRef create(ViewRef slaveBtn, ViewRef masterBtn,  int id_slave, int id_master)
    {
        return WireRef(new Wire(slaveBtn, masterBtn, id_slave, id_master));
    }
    
    static WireRef create(ViewRef masterBtn)
    {
        return WireRef(new Wire(masterBtn));
    }
    
    void addMasterBtn(ViewRef masterBtn)
    {
        this->masterBtn = masterBtn;
    }
    
    Color _color = Color(1.0f,1.0f, 1.0f);
    Color _colorHighLigth = Color(1.0f,.0f, .0f);
    ViewRef slaveBtn;
    ViewRef masterBtn;
    static float dist_Point_to_Segment(vec2 P, vec2 P0, vec2 P1){
        vec2 v = P1 - P0;
        vec2 w = P - P0;
        float c1 = dot(w, v);
        if(c1 <= 0 )
            return distance(P, P0);
        float c2 = dot(v, v);
        if(c2 <= c1 )
            return distance(P, P1);
        float b = c1 / c2;
        vec2 Pb = P0 + b * v;
        return distance(Pb, P);
    }
    void draw(ivec2 mMouseLoc){
        
        if(!mVboMesh ||
           masterBtn == nullptr ||
           path.getPoints().at(0) != slaveBtn->getBounds().getCenter() ||
           path.getPoints().at(1) != masterBtn->getBounds().getCenter()
        ){
            path.clear();
            path.moveTo(slaveBtn->getBounds().getCenter());
            if(masterBtn){
                path.lineTo(masterBtn->getBounds().getCenter());
            }else{
                path.lineTo(mMouseLoc);
            }
            mVboMesh.reset();
            // create a new vector that can contain 3D vertices
            std::vector<vec3> vertices;
            
            // to improve performance, make room for the vertices + 2 adjacency vertices
            vertices.reserve( path.getPoints().size() + 2 );
            
            // first, add an adjacency vertex at the beginning
            vertices.push_back( 2.0f * vec3( path.getPoints().at(0), 0 ) - vec3( path.getPoints().at(1), 0 ) );
            
            // next, add all 2D points as 3D vertices
            std::vector<vec2>::iterator itr;
            for( itr = path.getPoints().begin(); itr != path.getPoints().end(); ++itr )
                vertices.push_back( vec3( *itr, 0 ) );
            
            // next, add an adjacency vertex at the end
            unsigned int n = (unsigned int) (path.getPoints().size());
            vertices.push_back( 2.0f * vec3( path.getPoints().at(n - 1), 0 ) - vec3( path.getPoints().at(n - 2), 0 ) );
            
            // now that we have a list of vertices, create the index buffer
            n = (unsigned int) (vertices.size() - 2);
            std::vector<uint16_t> indices;
            indices.reserve( n * 4 );
            
            for( unsigned int i = 1; i < vertices.size() - 2; ++i ) {
                indices.push_back( i - 1 );
                indices.push_back( i );
                indices.push_back( i + 1 );
                indices.push_back( i + 2 );
            }
            
            // finally, create the mesh
            gl::VboMesh::Layout layout;
            layout.attrib( geom::POSITION, 3 );
            
            mVboMesh =  gl::VboMesh::create( (uint32_t) vertices.size(), GL_LINES_ADJACENCY_EXT, { layout }, (uint32_t) indices.size() );
            mVboMesh->bufferAttrib( geom::POSITION, vertices.size() * sizeof( vec3 ), vertices.data() );
            mVboMesh->bufferIndices( indices.size() * sizeof( uint16_t ), indices.data() );
        }
        if(!masterBtn || (slaveBtn->isVisible() && masterBtn && masterBtn->isVisible())){
            if(!masterBtn) over=true;
            else over = 10 >= dist_Point_to_Segment(mMouseLoc, slaveBtn->getBounds().getCenter(), masterBtn->getBounds().getCenter());
            gl::color( over ? _colorHighLigth : _color);
            gl::draw(mVboMesh);
        }
    }
};
typedef std::shared_ptr<class Wire> WireRef;
}
#endif /* Wire_hpp */
