//
//  OrageError.h
//  ORAGE
//
//  Created by Vincent Evrard on 17/12/21.
//

#ifndef OrageError_h
#define OrageError_h

#include <exception>
using namespace std;

namespace ORAGE {
    namespace COMMON {
        
        class OrageError : public exception
        {
        public:
            OrageError (int numero=0, string const& phrase="", int niveau=0) throw()
            :m_numero(numero),m_phrase(phrase),m_niveau(niveau)
            {}
            
            virtual const char* what() const throw()
            {
                return m_phrase.c_str();
            }
            
            int getNiveau() const throw()
            {
                return m_niveau;
            }
            
            virtual ~OrageError() throw()
            {}
            
        private:
            int m_numero;               //Numéro de l'erreur
            string m_phrase;            //Description de l'erreur
            int m_niveau;               //Niveau de l'erreur
        };
        
    }
}
#endif /* OrageError_h */
