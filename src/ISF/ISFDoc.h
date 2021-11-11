//
//  ISFDoc.h
//  ORAGE
//
//  Created by Vincent Evrard on 10/11/21.
//

#ifndef ISFDoc_h
#define ISFDoc_h
#include "ISFBase.h"
#include "ISFAttr.h"
#include "ISFVal.h"
#include "ISFStringUtils.h"
#include "ISFStringUtils_2.h"
#include "ISFErr.h"
#include "ISFRange.h"
#include "ISFConstants.h"
namespace ISF {
    class ISFDoc{
        typedef std::shared_ptr<ISFDoc> ISFDocRef;
        
        std::recursive_mutex        _propLock;
        
        std::string *_path = nullptr; // full path to the loaded file
        std::string *_name = nullptr; // just the file name (including its extension)
        std::string *_description = nullptr; // description of whatever the file does
        std::string *_credit = nullptr; // credit
        std::string *_vsn = nullptr;
        std::vector<std::string> _categories; // array of strings of the category names this doc should be
        
        std::vector<ISFAttrRef> _inputs; // array of ISFAttrRef instances for the various inputs
        std::vector<ISFAttrRef> _imageInputs; // array of ISFAttrRef instances for the image inputs (the image inputs are stored in two arrays).
        std::vector<ISFAttrRef> _imageOutputs; // array of ISFAttrRef instances for the image outputs (the image outputs are stored in two arrays).
        std::vector<ISFAttrRef> _outputs; // array of ISFAttrRef instances for the various inputs
        
        std::vector<ISFAttrRef> _audioInputs; // array of ISFAttrRef instances for the audio inputs
        std::vector<ISFAttrRef> _imageImports; // array of ISFAttrRef instances that describe imported images. attrib's 'attribName' is the name of the sampler, attrib's 'description' is the path to the file.
        
        std::string *_jsonSourceString = nullptr; // the JSON std::string from the source *including the comments and any linebreaks before/after it*
        std::string *_jsonString = nullptr; // the JSON std::string copied from the source- doesn't include any comments before/after it
        std::string *_vertShaderSource = nullptr; // the raw vert shader source before being find-and-replaced
        std::string *_fragShaderSource = nullptr; // the raw frag shader source before being find-and-replaced
    public:
        static ISFDocRef create(const std::string & inPath){
            return ISFDocRef(new ISFDoc(inPath));
        }
        //! Constructs an ISFDoc instance from a passed file on disk.  Consider using CreateISFDocRef() instead.
        /*!
         \param inPath The path to the ISF file you want to load as an ISFDoc.
         Throws an ISFErr if there is a problem of some sort loading the ISF file from disk or parsing the JSON in the ISF file.
         */
        ISFDoc(const std::string & inPath){
            _path = new string(inPath);
            _name = new string(LastPathComponent(inPath));
            ifstream        fin;
            fin.open(inPath);
            if (!fin.is_open())    {
                throw ISFErr(ISFErrType_MissingResource, "cannot create ISFDoc from passed path", inPath);
            }
            string rawFile( static_cast<stringstream const &>(stringstream() << fin.rdbuf()).str() );
            fin.close();
            
            string noExtPath = StringByDeletingExtension(inPath);
            string tmpPath;
            fin.open(noExtPath+".vs");
            //    if the file's not open, try opening a file with a different path
            if (!fin.is_open())    {
                fin.open(noExtPath+".vert");
            }
            //    if the file's still not open then it just didn't exist- i have to use the passthru shader
            if (!fin.is_open())    {
                fin.open(ci::app::getAssetPath("./shaders/default.vs").string());
            }
            //  read it into a string
            if(fin.is_open()){
                _vertShaderSource = new string( static_cast<stringstream const &>(stringstream() << fin.rdbuf()).str() );
                fin.close();
            }
            //    call the init method with the contents of the file we read in
            _initWithRawFragShaderString(rawFile);
            
        }
        virtual ~ISFDoc(){
            lock_guard<recursive_mutex>        lock(_propLock);
            
            if (_path != nullptr)    {
                delete _path;
                _path = nullptr;
            }
            if (_name != nullptr)    {
                delete _name;
                _name = nullptr;
            }
            if (_description != nullptr)    {
                delete _description;
                _description = nullptr;
            }
            if (_credit != nullptr)    {
                delete _credit;
                _credit = nullptr;
            }
            if (_vsn != nullptr)    {
                delete _vsn;
                _vsn = nullptr;
            }
            
            
            if (_jsonSourceString != nullptr)    {
                delete _jsonSourceString;
                _jsonSourceString = nullptr;
            }
            if (_jsonString != nullptr)    {
                delete _jsonString;
                _jsonString = nullptr;
            }
            if (_vertShaderSource != nullptr)    {
                delete _vertShaderSource;
                _vertShaderSource = nullptr;
            }
            if (_fragShaderSource != nullptr)    {
                delete _fragShaderSource;
                _fragShaderSource = nullptr;
            }
        }
        
        //!    Returns the path of the ISF file for the receiver.  This is probably the path to the frag shader.
        std::string path() const { return (_path==nullptr) ? std::string("") : std::string(*_path); }
        //!    Returns the name of the receiver's ISF file (the file name, minus the extension).
        std::string name() const { return (_name==nullptr) ? std::string("") : std::string(*_name); }
        //!    Returns the receiver's "description" std::string, as defined in its JSON blob ("DESCRIPTION").
        std::string description() const { return (_description==nullptr) ? std::string("") : std::string(*_description); }
        //!    Returns the receiver's "credit" std::string, as defined in its JSON blob ("CREDIT").
        std::string credit() const { return (_credit==nullptr) ? std::string("") : std::string(*_credit); }
        //!    Returns the receiver's "vsn" std::string, as defined in its JSON blob ("VSN")
        std::string vsn() const { return (_vsn==nullptr) ? std::string("") : std::string(*_vsn); }
        //!    Returns a std::vector containing strings listing the receiver's categories.
        std::vector<std::string> & categories() { return _categories; }
        
        //!    Returns a std::vector of ISFAttrRef instances describing all of the receiver's inputs.
        std::vector<ISFAttrRef> & inputs() { return _inputs; }
        //!    Returns a std::vector of ISFAttrRef instances describing only the receiver's image inputs.
        std::vector<ISFAttrRef> & imageInputs() { return _imageInputs; }
        //!    Returns a std::vector of ISFAttrRef instances describing only the receiver's image outputs.
        std::vector<ISFAttrRef> & imageOutputs() { return _imageOutputs; }
        //!    Returns a std::vector of ISFAttrRef instances describing only the receiver's audio inputs.
        std::vector<ISFAttrRef> & audioInputs() { return _audioInputs; }
        //!    Returns a std::vector of ISFAttrRef instances describing only the receiver's audioFFT inputs.
        std::vector<ISFAttrRef> & imageImports() { return _imageImports; }
        //!    Returns a std::vector of ISFAttrRef instances describing only the receiver's inputs that match the passed type.
        
        //!    Returns the GLBufferRef for the passed key.  Checks all attributes/inputs as well as persistent and temp buffers.
        const ci::gl::TextureRef getBufferForKey(const std::string & n){
            lock_guard<recursive_mutex>        lock(_propLock);
            for (const auto & attribRefIt : _imageInputs)    {
                if (attribRefIt->name() == n)
                    return attribRefIt->currentVal().imageBuffer();
            }
            
            for (const auto & attribRefIt : _imageOutputs)    {
                if (attribRefIt->name() == n)
                    return attribRefIt->currentVal().imageBuffer();
            }
            return nullptr;
        }
        
        
        //! Returns the JSON std::string from the source *including the comments and any linebreaks before/after it*
        std::string * jsonSourceString() const { return _jsonSourceString; }
        //!    Returns the JSON std::string copied from the source- doesn't include any comments before/after it
        std::string * jsonString() const { return _jsonString; }
        //!    Returns the raw vert shader source before being find-and-replaced
        std::string * vertShaderSource() const { return _vertShaderSource; }
        //!    Returns the raw frag shader source before being find-and-replaced
        std::string * fragShaderSource() const { return _fragShaderSource; }
        //!    Populates the passed var with the JSON std::string from the source *including the comments and any linebreaks before/after it*
        void jsonSourceString(std::string & outStr){
            
        }
        /*!
         \brief Returns a true if successful.  Generates GLSL source code, populating the provided vars with strings that are usable for frag/vert shaders
         \param outFragSrc A non-null pre-allocated std::std::string variable which will be populated with the fragment shader source code generated for this ISF file.
         \param outVertSrc A non-null pre-allocated std::std::string variable which will be populated with the vertex shader source code generated for this ISF file.
         \param inGLVers The version of OpenGL that the generated source code must be compatible with.
         \param inVarsAsUBO Defaults to false.  If true, variable declarations for non-image INPUTS will be assembled in a uniform block.  This option was added because a downstream utility requires it.
         */
        bool generateShaderSource(std::string * outFragSrc, std::string * outVertSrc, GLVersion & inGLVers){
            lock_guard<recursive_mutex>        lock(_propLock);
            
            if (outFragSrc==nullptr || outVertSrc==nullptr || _vertShaderSource==nullptr || _fragShaderSource==nullptr)    {
                    throw ISFErr(ISFErrType_ErrorParsingFS, "Preflight failed", __PRETTY_FUNCTION__);
            }
            
            string        vsVarDeclarations = string("");
            string        fsVarDeclarations = string("");
            if (!_assembleShaderSource_VarDeclarations(&vsVarDeclarations, &fsVarDeclarations, inGLVers))    {
                throw ISFErr(ISFErrType_ErrorParsingFS, "Var Dec failed", __PRETTY_FUNCTION__);
            }
            //cout << "vs var declarations:\n*******************\n" << vsVarDeclarations << "*******************\n";
            //cout << "fs var declarations:\n*******************\n" << fsVarDeclarations << "*******************\n";
            /*    stores names of the images/buffers that are accessed via IMG_THIS_PIXEL (which is replaced
             in the frag shader, but the names are then needed to declare vars in the vert shader)        */
            vector<string>        imgThisPixelSamplerNames;
            vector<string>        imgThisNormPixelSamplerNames;
            //    check the source string to see if it requires any of the macro functions, add them if necessary
            //bool            requiresMacroFunctions = false;
            bool            requires2DMacro = false;
            bool            requires2DBiasMacro = false;
            bool            requires2DRectMacro = false;
            bool            requires2DRectBiasMacro = false;
            Range           tmpRange(0,0);
            string          searchString("");
            ci::gl::TextureRef imgBuffer = nullptr;
            string          modSrcString("");
            string          newString("");
            size_t          tmpIndex;
            
            //    put together a new frag shader string from the raw shader source
            string            newFragShaderSrc = string("");
            newFragShaderSrc.reserve( uint32_t(1.5 * (fsVarDeclarations.size()+_fragShaderSource->size())) );
            
            {
                //    remove any lines containing #version tags
                searchString = string("#version");
                tmpRange = Range(newFragShaderSrc.find(searchString), searchString.size());
                do    {
                    if (tmpRange.loc != string::npos)    {
                        tmpIndex = modSrcString.find_first_of("\n\r\f", tmpRange.max());
                        if (tmpIndex != string::npos)    {
                            tmpRange.len = tmpIndex - tmpRange.loc;
                            newFragShaderSrc.erase(tmpRange.loc, tmpRange.len);
                            
                            tmpRange = Range(newFragShaderSrc.find(searchString), searchString.size());
                        }
                    }
                } while (tmpRange.loc != string::npos);
                
                //    add the #version tag for the min version of GLSL supported by this major vsn of openGL
                switch (inGLVers)    {
                    case GLVersion_Unknown:
                    case GLVersion_2:
                        newFragShaderSrc.insert(0, string("#version 110\n"));
                        break;
                    case GLVersion_ES:
                    case GLVersion_ES2:
                        newFragShaderSrc.insert(0, string("#version 100\n"));
                        break;
                    case GLVersion_ES3:
                        newFragShaderSrc.insert(0, string("#version 300 es\n"));
                        break;
                    case GLVersion_33:
                        newFragShaderSrc.insert(0, string("#version 330\n"));
                        break;
                    case GLVersion_4:
                        newFragShaderSrc.insert(0, string("#version 400\n"));
                        break;
                }
                //    add the compatibility define
                switch (inGLVers)    {
                    case GLVersion_Unknown:
                    case GLVersion_2:
                        //    intentionally blank
                        break;
                    case GLVersion_ES:
                    case GLVersion_ES2:
                    case GLVersion_ES3:
                    case GLVersion_33:
                    case GLVersion_4:
                        newFragShaderSrc.append(ISF_ES_Compatibility);
                        break;
                }
                //    copy the variable declarations to the frag shader src
                newFragShaderSrc.append(fsVarDeclarations);
                //    now i have to find-and-replace the shader source for various things- make a copy of the raw source and work from that.
                modSrcString.reserve(newFragShaderSrc.capacity());
                modSrcString.append(*_fragShaderSource);
                
                //    find-and-replace vv_FragNormCoord (v1 of the ISF spec) with isf_FragNormCoord (v2 of the ISF spec)
                searchString = string("vv_FragNormCoord");
                newString = string("isf_FragNormCoord");
                tmpRange = Range(0, searchString.size());
                do    {
                    tmpRange.loc = modSrcString.find(searchString);
                    if (tmpRange.loc != string::npos)
                        modSrcString.replace(tmpRange.loc, tmpRange.len, newString, 0, newString.size());
                } while (tmpRange.loc != string::npos);
                
                //    now find-and-replace IMG_PIXEL
                searchString = string("IMG_PIXEL");
                imgBuffer = nullptr;
                tmpRange = Range(0, searchString.size());
                do    {
                    tmpRange = Range(modSrcString.find(searchString), searchString.size());
                    if (tmpRange.loc != string::npos)    {
                        vector<string> varArray(0);
                        varArray.reserve(5);
                        Range fullFuncRangeToReplace = LexFunctionCall(modSrcString, tmpRange, varArray);
                        size_t varArrayCount = varArray.size();
                        
                        if (varArrayCount!=2 && varArrayCount!=3)    {
                            map<string,string>        tmpErrDict;
                            string                    tmpErrLog = FmtString("ERROR: 0:%d: IMG_PIXEL has wrong number of arguments",NumLines(string(modSrcString, 0, tmpRange.max()))+1);
                            tmpErrDict.insert( pair<string,string>(string("fragErrLog"), tmpErrLog) );
                            tmpErrDict.insert( pair<string,string>(string("fragSrc"), modSrcString) );
                            if (_path != nullptr)
                                throw ISFErr(ISFErrType_ErrorParsingFS, "IMG_PIXEL has wrong number of arguments", *_path, tmpErrDict);
                            else
                                throw ISFErr(ISFErrType_ErrorParsingFS, "IMG_PIXEL has wrong number of args", "", tmpErrDict);
                        }
                        else {
                            string          newFuncString("");
                            string &        samplerName = varArray[0];
                            const char *    samplerNameC = samplerName.c_str();
                            string &        samplerCoord = varArray[1];
                            const char *    samplerCoordC = samplerCoord.c_str();
                            imgBuffer = getBufferForKey(samplerName);
                            
                            newFuncString = FmtString("VVSAMPLER_2DBYPIXEL(%s, _%s_imgRect, _%s_imgSize, _%s_flip, %s)",samplerNameC,samplerNameC,samplerNameC,samplerNameC,samplerCoordC);
                            requires2DMacro = true;
                        
                            modSrcString.replace(fullFuncRangeToReplace.loc, fullFuncRangeToReplace.len, newFuncString, 0, newFuncString.size());
                        }
                    }
                } while (tmpRange.loc != string::npos);
                
                //    now find-and-replace IMG_NORM_PIXEL
                searchString = string("IMG_NORM_PIXEL");
                imgBuffer = nullptr;
                tmpRange = Range(0, searchString.size());
                do    {
                    tmpRange = Range(modSrcString.find(searchString), searchString.size());
                    if (tmpRange.loc != string::npos)    {
                        vector<string> varArray(0);
                        varArray.reserve(5);
                        Range fullFuncRangeToReplace = LexFunctionCall(modSrcString, tmpRange, varArray);
                        size_t varArrayCount = varArray.size();
                        if (varArrayCount!=2 && varArrayCount!=3)    {
                            map<string,string> tmpErrDict;
                            string tmpErrLog = FmtString("ERROR: 0:%d: IMG_NORM_PIXEL has wrong number of arguments",NumLines(string(modSrcString, 0, tmpRange.max()))+1);
                            tmpErrDict.insert( pair<string,string>(string("fragErrLog"), tmpErrLog) );
                            tmpErrDict.insert( pair<string,string>(string("fragSrc"), modSrcString) );
                            if (_path != nullptr)
                                throw ISFErr(ISFErrType_ErrorParsingFS, "IMG_NORM_PIXEL has wrong number of arguments", *_path, tmpErrDict);
                            else
                                throw ISFErr(ISFErrType_ErrorParsingFS, "IMG_NORM_PIXEL has wrong number of arguments", "", tmpErrDict);
                        }
                        else    {
                            string          newFuncString("");
                            string &        samplerName = varArray[0];
                            const char *    samplerNameC = samplerName.c_str();
                            string &        samplerCoord = varArray[1];
                            const char *    samplerCoordC = samplerCoord.c_str();
                            imgBuffer = getBufferForKey(samplerName);
                            
                            newFuncString = FmtString("VVSAMPLER_2DBYNORM(%s, _%s_imgRect, _%s_imgSize, _%s_flip, %s)",samplerNameC,samplerNameC,samplerNameC,samplerNameC,samplerCoordC);
                            requires2DMacro = true;
                            
                            modSrcString.replace(fullFuncRangeToReplace.loc, fullFuncRangeToReplace.len, newFuncString, 0, newFuncString.size());
                        }
                    }
                } while (tmpRange.loc != string::npos);
                
                newFragShaderSrc.append("\n");
                
                //    if the frag shader requires macro functions, add them now that i'm done declaring the variables
                switch (inGLVers)    {
                    case GLVersion_Unknown:
                    case GLVersion_2:
                    case GLVersion_ES:
                    case GLVersion_ES2:
                        if (requires2DMacro)
                            newFragShaderSrc.append(ISFGLMacro2D_GL2);
                        if (requires2DBiasMacro)
                            newFragShaderSrc.append(ISFGLMacro2DBias_GL2);
                        if (requires2DRectMacro)
                            newFragShaderSrc.append(ISFGLMacro2DRect_GL2);
                        if (requires2DRectBiasMacro)
                            newFragShaderSrc.append(ISFGLMacro2DRectBias_GL2);
                        break;
                    case GLVersion_ES3:
                    case GLVersion_33:
                    case GLVersion_4:
                        if (requires2DMacro)
                            newFragShaderSrc.append(ISFGLMacro2D_GL3);
                        if (requires2DBiasMacro)
                            newFragShaderSrc.append(ISFGLMacro2DBias_GL3);
                        if (requires2DRectMacro)
                            newFragShaderSrc.append(ISFGLMacro2DRect_GL3);
                        if (requires2DRectBiasMacro)
                            newFragShaderSrc.append(ISFGLMacro2DRectBias_GL3);
                        break;
                }
                //    add the shader source that has been find-and-replaced
                newFragShaderSrc.append(modSrcString);
//                cout << "newFragShaderSrc is:\n******************\n" << newFragShaderSrc << "\n******************\n";
            }
            
            //    put together a new vert shader string from the raw shader source
            string            newVertShaderSrc = string("");
            newVertShaderSrc.reserve( uint32_t(2.5 * (vsVarDeclarations.size()+_vertShaderSource->size())) );
            {
                modSrcString = string("");
                modSrcString.reserve(newVertShaderSrc.capacity());
                modSrcString.append(*_vertShaderSource);
                newVertShaderSrc.append(modSrcString);
            }
            
            outFragSrc->reserve(newFragShaderSrc.size()+1);
            outFragSrc->append(newFragShaderSrc);
            outVertSrc->reserve(newVertShaderSrc.size()+1);
            outVertSrc->append(newVertShaderSrc);
            return true;
        }
        
    protected:
        // used so we can have two constructors without duplicating code
        void _initWithRawFragShaderString(const std::string & inRawFile){
            // isolate the JSON blob that should be at the beginning of the file in a comment, save it as one string- save everything else as the raw shader source string
            auto openCommentIndex = inRawFile.find("/*");
            auto closeCommentIndex = inRawFile.find("*/");
            if (openCommentIndex == string::npos ||
                closeCommentIndex == string::npos ||
                closeCommentIndex < openCommentIndex)
            {
                if (_path != nullptr)
                    throw ISFErr(ISFErrType_MalformedJSON, "ISFDoc missing comment blob", *_path);
                else
                    throw ISFErr(ISFErrType_MalformedJSON, "ISFDoc missing comment blob", "");
            }
            
            // we need to advance 'closeCommentLineEnd' to include both the full "close comment" as well as the next "line break"
            auto tmpIt = inRawFile.begin();
            tmpIt += closeCommentIndex;
            while (*tmpIt!='\n' && *tmpIt!='\r')
                ++tmpIt;
            ++tmpIt;
            auto closeCommentLineEnd = tmpIt - inRawFile.begin();
            
            _jsonSourceString = new string(inRawFile, 0, closeCommentLineEnd);
            _jsonString = new string(inRawFile, openCommentIndex+2, closeCommentIndex - (openCommentIndex+2) );
            _fragShaderSource = new string(inRawFile, closeCommentLineEnd);
            bool            caughtJSONException = false;
            
            try    {
                ci::JsonTree jblob = ci::JsonTree(*_jsonString);
                
                
                _description = new string(
                                          jblob.hasChild("DESCRIPTION") ?
                                          jblob.getChild("DESCRIPTION").getValue<string>()
                                          :
                                          ""
                                          );
                
                _credit = new string(
                                     jblob.hasChild("CREDIT") ?
                                     jblob.getChild("CREDIT").getValue<string>()
                                     :
                                     ""
                                     );
                
                _vsn = new string(
                                  jblob.hasChild("VSN") ?
                                  jblob.getChild("VSN").getValue<string>()
                                  :
                                  ""
                                  );
                
                ci::JsonTree categories = ci::JsonTree::makeArray();
                if(jblob.hasChild("CATEGORIES") &&
                   jblob.getChild("CATEGORIES").getNodeType() == ci::JsonTree::NodeType::NODE_ARRAY)
                {
                    categories = jblob.getChild("CATEGORIES");
                }
                for (auto cat : categories) {
                    _categories.push_back(cat.getValue<string>());
                }
                
                ci::JsonTree inputs = ci::JsonTree::makeArray();
                if(jblob.hasChild("INPUTS") &&
                   jblob.getChild("INPUTS").getNodeType() == ci::JsonTree::NodeType::NODE_ARRAY)
                {
                    inputs = jblob.getChild("INPUTS");
                }
                ci::JsonTree outputs = ci::JsonTree::makeArray();
                if(jblob.hasChild("OUTPUTS") &&
                   jblob.getChild("OUTPUTS").getNodeType() == ci::JsonTree::NodeType::NODE_ARRAY)
                {
                    outputs = jblob.getChild("OUTPUTS");
                }else{
                    outputs = ci::JsonTree("[{\"NAME\" : \"gl_FragColor\", \"TYPE\" : \"image\"}]");
                }
                
                ISFValType newAttribType = ISFValType_None;
                ISFVal minVal = ISFNullVal();
                ISFVal maxVal = ISFNullVal();
                ISFVal defVal = ISFNullVal();
                ISFVal idenVal = ISFNullVal();
                vector<string> labelArray;
                vector<int32_t> valArray;
                
                for (auto input : inputs) {
                    if(input.getNodeType() != ci::JsonTree::NodeType::NODE_OBJECT)
                        continue;
                    if(!input.hasChild("NAME"))
                        continue;
                    if(!input.hasChild("TYPE"))
                        continue;
                    string inputKeyJ = input.getChild("NAME").getValue<string>();
                    string typeStringJ = input.getChild("TYPE").getValue<string>();
                    string descString = input.hasChild("DESCRIPTION") ? jblob.getChild("DESCRIPTION").getValue<string>() : "";
                    string labelString = input.hasChild("LABEL") ? jblob.getChild("LABEL").getValue<string>() : "";
                    ISFAttr_IO io = ISFAttr_IO::IN;
                    bool isImageInput = false;
                    
                    //    clear some state vars
                    newAttribType = ISFValType_None;
                    minVal = ISFNullVal();
                    maxVal = ISFNullVal();
                    defVal = ISFNullVal();
                    idenVal = ISFNullVal();
                    
                    labelArray.clear();
                    valArray.clear();
                    
                    if (typeStringJ == "image") {
                        newAttribType = ISFValType_Image;
                        isImageInput = true;
                    }
                    else if (typeStringJ == "audio") {
                        newAttribType = ISFValType_Audio;
                    }
                    else if (typeStringJ == "audioFFT") {
                        newAttribType = ISFValType_AudioFFT;
                    }
                    else if (typeStringJ == "cube") {
                        newAttribType = ISFValType_Cube;
                    }
                    else if (typeStringJ == "float") {
                        newAttribType = ISFValType_Float;
                        ci::JsonTree tmpObj;
                        ci::JsonTree tmpMinJ = input.hasChild("MIN") ? input.getChild("MIN") : ci::JsonTree();
                        ci::JsonTree tmpMaxJ = input.hasChild("MAX") ? input.getChild("MAX") : ci::JsonTree();
                        ci::JsonTree tmpDefJ = input.hasChild("DEFAULT") ? input.getChild("DEFAULT") : ci::JsonTree();
                        ci::JsonTree tmpIdeJ = input.hasChild("IDENTITY") ? input.getChild("IDENTITY") : ci::JsonTree();
                        minVal = ISFFloatVal(tmpMinJ.getValue<double>());
                        maxVal = ISFFloatVal(tmpMaxJ.getValue<double>());
                        defVal = ISFFloatVal(tmpDefJ.getValue<double>());
                        idenVal= ISFFloatVal(tmpIdeJ.getValue<double>());
                        
                        if ((minVal.isNullVal() && !maxVal.isNullVal()) ||
                            (!minVal.isNullVal() && maxVal.isNullVal())) {
                            minVal = ISFNullVal();
                            maxVal = ISFNullVal();
                        }
                        
                        //    if i don't have a min/max val, default to a normalized range
                        if (minVal.isNullVal() && maxVal.isNullVal()) {
                            minVal = ISFFloatVal(0.);
                            maxVal = ISFFloatVal(1.);
                        }
                        
                        if (defVal.isNullVal()) {
                            defVal = ISFFloatVal((maxVal.getDoubleVal()-minVal.getDoubleVal())/2. + minVal.getDoubleVal());
                        }
                        else {
                            if (defVal.getDoubleVal()<minVal.getDoubleVal())
                                defVal = minVal;
                            else if (defVal.getDoubleVal()>maxVal.getDoubleVal())
                                defVal = maxVal;
                        }
                    }
                    //    else the attribute wasn't recognized- skip it
                    else
                        continue;
                    ISFAttrRef newAttribRef = ISFAttr::create(inputKeyJ, descString, labelString, io, newAttribType, minVal, maxVal, defVal, idenVal, &labelArray, &valArray);
                    _inputs.push_back(newAttribRef);
                    if (isImageInput)
                        _imageInputs.push_back(newAttribRef);
                }
                
                for (auto ouput : outputs) {
                    if(ouput.getNodeType() != ci::JsonTree::NodeType::NODE_OBJECT)
                        continue;
                    if(!ouput.hasChild("NAME"))
                        continue;
                    if(!ouput.hasChild("TYPE"))
                        continue;
                    string inputKeyJ = ouput.getChild("NAME").getValue<string>();
                    string typeStringJ = ouput.getChild("TYPE").getValue<string>();
                    ISFAttr_IO io = ISFAttr_IO::OUT;
                    
                    if (typeStringJ == "image") {
                        ISFAttrRef newAttribRef = ISFAttr::create(inputKeyJ, "", "", io, ISFValType_Image);
                        _outputs.push_back(newAttribRef);
                        _imageOutputs.push_back(newAttribRef);
                    }
                }
            }
            catch (ci::JsonTree::ExcJsonParserError& ex)    {
                caughtJSONException = true;
                map<string,string>        tmpErrDict;
                tmpErrDict.insert( pair<string,string>(string("jsonErrLog"), ex.what()) );
                if (_path != nullptr)
                    throw ISFErr(ISFErrType_MalformedJSON, "the JSON blob in this file is malformed.", *_path, tmpErrDict);
                else
                    throw ISFErr(ISFErrType_MalformedJSON, "the JSON blob in this file is malformed.", "", tmpErrDict);
            }
            catch (...) {
                caughtJSONException = true;
                if (_path != nullptr)
                    throw ISFErr(ISFErrType_MalformedJSON, "the JSON blob in this file is malformed.", *_path);
                else
                    throw ISFErr(ISFErrType_MalformedJSON, "the JSON blob in this file is malformed.", "");
            }
        }
        //    returns a true if successful.  populates a std::string with variable declarations for a frag shader
        bool _assembleShaderSource_VarDeclarations(std::string * outVSString, std::string * outFSString, GLVersion & inGLVers){
            lock_guard<recursive_mutex>        lock(_propLock);
            
            if (outVSString==nullptr || outFSString==nullptr)
                return false;
            
            //    we're going to work this by assembling an array of strings, one for each line- have the vector reserve space for enough strings
            vector<string>        vsDeclarations;
            vector<string>        fsDeclarations;
            
            //    frag shader always needs an output, which we're naming gl_FragColor so shaders written against GL 2.1 will work.  we'll use a #define in the shader source to make the shader precompiler change gl_FragColor to isf_FragColor
            switch (inGLVers)    {
                case GLVersion_Unknown:
                case GLVersion_2:
                case GLVersion_ES:
                case GLVersion_ES2:
                    break;
                case GLVersion_ES3:
                case GLVersion_33:
                case GLVersion_4:
                {
                    for(int i = 0 ; i < _imageOutputs.size() ; i++){
                        fsDeclarations.push_back("layout (location = "+to_string(i)+") out vec4 "+_imageOutputs.at(i)->name()+";\n");
                    }
                    break;
                }
            }
            
            vsDeclarations.push_back("uniform int\t\tPASSINDEX;\n");
            fsDeclarations.push_back("uniform int\t\tPASSINDEX;\n");
            vsDeclarations.push_back("uniform vec2\t\tRENDERSIZE;\n");
            fsDeclarations.push_back("uniform vec2\t\tRENDERSIZE;\n");
            
            switch (inGLVers)    {
                case GLVersion_Unknown:
                case GLVersion_2:
                case GLVersion_ES:
                case GLVersion_ES2:
                    vsDeclarations.push_back("varying vec2\t\tisf_FragNormCoord;\n");
                    fsDeclarations.push_back("varying vec2\t\tisf_FragNormCoord;\n");
                    //vsDeclarations.emplace_back("varying vec3\t\tisf_VertNorm;\n");
                    //fsDeclarations.emplace_back("varying vec3\t\tisf_VertNorm;\n");
                    //vsDeclarations.emplace_back("varying vec3\t\tisf_VertPos;\n");
                    //fsDeclarations.emplace_back("varying vec3\t\tisf_VertPos;\n");
                    break;
                case GLVersion_ES3:
                case GLVersion_33:
                case GLVersion_4:
                    vsDeclarations.push_back("out vec2\t\tisf_FragNormCoord;\n");
                    fsDeclarations.push_back("in vec2\t\tisf_FragNormCoord;\n");
                    //vsDeclarations.emplace_back("out vec3\t\tisf_VertNorm;\n");
                    //fsDeclarations.emplace_back("in vec3\t\tisf_VertNorm;\n");
                    //vsDeclarations.emplace_back("out vec3\t\tisf_VertPos;\n");
                    //fsDeclarations.emplace_back("in vec3\t\tisf_VertPos;\n");
                    break;
            }
            
            vsDeclarations.push_back("uniform float\t\tTIME;\n");
            fsDeclarations.push_back("uniform float\t\tTIME;\n");
            vsDeclarations.push_back("uniform float\t\tTIMEDELTA;\n");
            fsDeclarations.push_back("uniform float\t\tTIMEDELTA;\n");
            vsDeclarations.push_back("uniform vec4\t\tDATE;\n");
            fsDeclarations.push_back("uniform vec4\t\tDATE;\n");
            vsDeclarations.push_back("uniform int\t\tFRAMEINDEX;\n");
            fsDeclarations.push_back("uniform int\t\tFRAMEINDEX;\n");
            
            
            //    this block will be used to add declarations for a provided ISFAttr
            auto attribDecBlock = [&](const ISFAttrRef & inRef)    {
                const string & tmpName = inRef->name();
                const char * nameCStr = tmpName.c_str();
                switch (inRef->type())    {
                    case ISFValType_None:
                        break;
                    case ISFValType_Event:
                    case ISFValType_Bool:
                        vsDeclarations.push_back(FmtString("uniform bool\t\t%s;\n", nameCStr));
                        fsDeclarations.push_back(FmtString("uniform bool\t\t%s;\n", nameCStr));
                        break;
                    case ISFValType_Long:
                        vsDeclarations.push_back(FmtString("uniform int\t\t%s;\n", nameCStr));
                        fsDeclarations.push_back(FmtString("uniform int\t\t%s;\n", nameCStr));
                        break;
                    case ISFValType_Float:
                        vsDeclarations.push_back(FmtString("uniform float\t\t%s;\n", nameCStr));
                        fsDeclarations.push_back(FmtString("uniform float\t\t%s;\n", nameCStr));
                        break;
                    case ISFValType_Point2D:
                        vsDeclarations.push_back(FmtString("uniform vec2\t\t%s;\n", nameCStr));
                        fsDeclarations.push_back(FmtString("uniform vec2\t\t%s;\n", nameCStr));
                        break;
                    case ISFValType_Color:
                        vsDeclarations.push_back(FmtString("uniform vec4\t\t%s;\n", nameCStr));
                        fsDeclarations.push_back(FmtString("uniform vec4\t\t%s;\n", nameCStr));
                        break;
                    case ISFValType_Cube:
                        //    make a sampler for the cubemap texture
                        vsDeclarations.push_back(FmtString("uniform samplerCube\t\t%s;\n", nameCStr));
                        fsDeclarations.push_back(FmtString("uniform samplerCube\t\t%s;\n", nameCStr));
                        //    just pass in the imgSize
                        vsDeclarations.push_back(FmtString("uniform vec2\t\t_%s_imgSize;\n", nameCStr));
                        fsDeclarations.push_back(FmtString("uniform vec2\t\t_%s_imgSize;\n", nameCStr));
                        break;
                    case ISFValType_Image:
                    {
                        ISFVal attribVal = inRef->currentVal();
                        ci::gl::TextureRef attribBuffer = attribVal.imageBuffer();
                        vsDeclarations.emplace_back(FmtString("uniform sampler2D\t\t%s;\n", nameCStr));
                        fsDeclarations.emplace_back(FmtString("uniform sampler2D\t\t%s;\n", nameCStr));
                    
                        vsDeclarations.emplace_back(FmtString("uniform vec4\t\t_%s_imgRect;\n", nameCStr));
                        fsDeclarations.emplace_back(FmtString("uniform vec4\t\t_%s_imgRect;\n", nameCStr));
                    
                        vsDeclarations.emplace_back(FmtString("uniform vec2\t\t_%s_imgSize;\n", nameCStr));
                        fsDeclarations.emplace_back(FmtString("uniform vec2\t\t_%s_imgSize;\n", nameCStr));
                    
                        vsDeclarations.emplace_back(FmtString("uniform bool\t\t_%s_flip;\n", nameCStr));
                        fsDeclarations.emplace_back(FmtString("uniform bool\t\t_%s_flip;\n", nameCStr));
                    
                        vsDeclarations.emplace_back(FmtString("uniform bool\t\t_%s_sample;\n", nameCStr));
                        fsDeclarations.emplace_back(FmtString("uniform bool\t\t_%s_sample;\n", nameCStr));
                        break;
                    }
                    case ISFValType_Audio:
                    case ISFValType_AudioFFT:
                        break;
                }
            };
            //    add the variables for the various inputs
            for (const auto & attrIt : _inputs)
                attribDecBlock(attrIt);
            
            
            //    now calculate the total length of the output string and reserve space for it
            size_t            reserveSize = 0;
            for (const auto & stringIt : fsDeclarations)    {
                reserveSize += stringIt.size();
            }
            
            outVSString->reserve(reserveSize);
            outFSString->reserve(reserveSize);
            for (const auto & stringIt : vsDeclarations)    {
                outVSString->append(stringIt);
            }
            for (const auto & stringIt : fsDeclarations)    {
                outFSString->append(stringIt);
            }
            
            return true;
        }
        //    returns a true if successful.  populates a map with std::string/value pairs that will be used to evaluate variable names in strings
        bool _assembleSubstitutionMap(std::map<std::string,double*> * outMap){
            return false;
        }
        
    };//ISFDoc
    typedef std::shared_ptr<ISFDoc> ISFDocRef;
}//ISF

#endif /* ISFDoc_h */
