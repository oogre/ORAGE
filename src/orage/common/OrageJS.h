//
//  OrageJS.h
//  ORAGE
//
//  Created by Vincent Evrard on 17/12/21.
//

#ifndef OrageJS_h
#define OrageJS_h

#include "duk_module_duktape.h"
#include <dukglue/dukglue.h>
#include "OrageFileTools.h"


namespace ORAGE {
    namespace COMMON {
        namespace JS {
            static DukValue push_file_as_string(duk_context *ctx, std::string filename) {
                try{
                    std::string content = ORAGE::COMMON::readFile(filename);
                    return dukglue_peval<DukValue>(ctx, content.c_str());
                }catch(ORAGE::COMMON::OrageError e){
                    std::cerr << "ERREUR : " << e.what() << std::endl;
                    return DukValue();
                }
            }

            static duk_ret_t native_print(duk_context *ctx) {
                duk_push_string(ctx, " ");
                duk_insert(ctx, 0);
                duk_join(ctx, duk_get_top(ctx) - 1);
                printf("%s\n", duk_to_string(ctx, -1));
                return 0;
            }

            static duk_ret_t my_mod_search(duk_context *ctx) {
                /* Nargs was given as 4 and we get the following stack arguments:
                 *   index 0: id
                 *   index 1: require
                 *   index 2: exports
                 *   index 3: module
                 */
                std::string moduleName = duk_to_string(ctx, 0);
                std::string filePath = ci::app::getAssetPath("jsModules/"+moduleName+".js").string();
                try{
                    std::string content = ORAGE::COMMON::readFile(filePath);
                    duk_push_lstring(ctx, content.c_str(), (duk_size_t) content.length());
                }catch(ORAGE::COMMON::OrageError e){
                    std::cerr << "ERREUR : " << e.what() << std::endl;
                    duk_push_undefined(ctx);
                }
                return 1;
            }

            static DukValue init(duk_context *ctx, std::string path){
                duk_module_duktape_init(ctx);
                
                duk_push_c_function(ctx, native_print, DUK_VARARGS);
                duk_put_global_string(ctx, "print");
                
                duk_get_global_string(ctx, "Duktape");
                duk_push_c_function(ctx, my_mod_search, 4);// nargs
                duk_put_prop_string(ctx, -2, "modSearch");
                duk_pop(ctx);
                
                return push_file_as_string(ctx, path);
            }
        }
    }
}

#endif /* OrageJS_h */
