//
//  Range.h
//  ORAGE
//
//  Created by Vincent Evrard on 11/01/23.
//

#ifndef DataRange_h
#define DataRange_h

#include <iostream>
#include <map>
#include <string>


using namespace std;

namespace ogre {
    struct RANGE {
        float value;
        float low;
        float high;
        float min;
        float max;
        RANGE():
            value(0.0f),
            min(-1.0f),
            max(1.0f),
            low(-1.0f),
            high(1.0f)
        {}
        RANGE(float a, float b, float c):
            RANGE(a, b, c, b, c)
        {}
        RANGE(float a, float b, float c, float d, float e):
            value(a),
            min(b),
            max(c),
            low(d),
            high(e)
        {}
        RANGE(JsonTree data):
            value(data.getChild("value").getValue<float>()),
            min(data.getChild("min").getValue<float>()),
            max(data.getChild("max").getValue<float>()),
            low(data.getChild("low").getValue<float>()),
            high(data.getChild("high").getValue<float>())
        {
        }
        JsonTree getData(string name, RangefRef limiter){
            JsonTree sub = JsonTree::makeObject(name);
            sub.addChild(JsonTree("value", value));
            sub.addChild(JsonTree("min", limiter->getMin()));
            sub.addChild(JsonTree("max", limiter->getMax()));
            sub.addChild(JsonTree("low", limiter->getValueLow()));
            sub.addChild(JsonTree("high", limiter->getValueHigh()));
            return sub;
        }
        void print(){
            cout << value << " " << min << " " << max << " " << low << " " << high << endl;
        }
    };
    
}
#endif /* DataRange_h */
