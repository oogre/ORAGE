//
//  Theme.h
//  Orage
//
//  Created by Vincent Evrard on 26/05/21.
//

#ifndef Theme_h
#define Theme_h

class Theme{
    public :
    static ci::ColorA bgActiveColor;
    static ci::ColorA bgDisactiveColor;
    static ci::ColorA ConnectorActiveColor;
    static ci::ColorA ConnectorDisactiveColor;
};

//////////////////////////////////

using namespace ci;

ColorA Theme::bgActiveColor = { 1.f, 0.f, 1.f, 1.f };
ColorA Theme::bgDisactiveColor = { 1.f, 1.f, 0.f, 1.f };

ColorA Theme::ConnectorActiveColor = { 1.f, 1.f, 1.f, 0.8f };
ColorA Theme::ConnectorDisactiveColor = { 1.f, 1.f, 1.f, 0.5f };

#endif /* Theme_h */
