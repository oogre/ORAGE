//
//  GLVersion.h
//  ORAGE
//
//  Created by Vincent Evrard on 10/11/21.
//

#ifndef ISFBasic_h
#define ISFBasic_h
namespace ISF {
    /*!
     \brief    This enum is used to describe the GL environment of a GL context, which is determined at runtime.
     */
    enum GLVersion {
        GLVersion_Unknown,
        GLVersion_2,
        GLVersion_ES,
        GLVersion_ES2,
        GLVersion_ES3,
        GLVersion_33,
        GLVersion_4
    };
}//ISF
#endif /* ISFBase_h */
