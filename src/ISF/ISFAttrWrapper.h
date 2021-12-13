//
//  ISFAttrWrapper.h
//  ORAGE
//
//  Created by Vincent Evrard on 16/11/21.
//

#ifndef ISFAttrWrapper_h
#define ISFAttrWrapper_h

#include "ISFAttr.h"


namespace ISF {
    class ISFAttrWrapper{
        typedef std::shared_ptr<ISFAttrWrapper> ISFAttrWrapperRef;
        std::recursive_mutex        _propLock;
        
        std::vector<ISFAttrRef> _inputs; // array of ISFAttrRef instances for the various inputs
        std::vector<ISFAttrRef> _imageInputs; // array of ISFAttrRef instances for the image inputs (the image inputs are stored in two arrays).
        std::vector<ISFAttrRef> _outputs; // array of ISFAttrRef instances for the various inputs
        std::vector<ISFAttrRef> _imageOutputs; // array of ISFAttrRef instances for the image outputs (the image outputs are stored in two arrays).
        std::vector<ISFAttrRef> _audioInputs; // array of ISFAttrRef instances for the audio inputs
        std::vector<ISFAttrRef> _imageImports; // array of ISFAttrRef instances that describe imported images. attrib's 'attribName' is the name of the sampler, attrib's 'description' is the path to the file.
        
        
    public :
        ISFAttrWrapper(){
            
        }
        virtual ~ISFAttrWrapper(){
            std::cout<<"~ISFAttrWrapper"<<std::endl;
            clear();
        }
        static ISFAttrWrapperRef create(){
            return std::make_shared<ISFAttrWrapper>();
        }
        
        //!    Returns a std::vector of ISFAttrRef instances describing all of the receiver's inputs.
        std::vector<ISFAttrRef> & inputs() { return _inputs; }
        
        std::vector<ISFAttrRef> & outputs() { return _outputs; }
        
        std::vector<ISFAttrRef> every() {
            std::vector<ISFAttrRef> all = std::vector<ISFAttrRef>();
            all.insert(all.end(), _inputs.begin(), _inputs.end());
            all.insert(all.end(), _outputs.begin(), _outputs.end());
            return all;
        }
        void concat(ISFAttrWrapperRef other){
            lock_guard<recursive_mutex>        lock(_propLock);
            _inputs.insert(_inputs.end(), other->inputs().begin(), other->inputs().end());
            _imageInputs.insert(_imageInputs.end(), other->imageInputs().begin(), other->imageInputs().end());
            _outputs.insert(_outputs.end(), other->outputs().begin(), other->outputs().end());
            _imageOutputs.insert(_imageOutputs.end(), other->imageOutputs().begin(), other->imageOutputs().end());
            _audioInputs.insert(_audioInputs.end(), other->audioInputs().begin(), other->audioInputs().end());
            _imageImports.insert(_imageImports.end(), other->imageImports().begin(), other->imageImports().end());
        }
        void clear(){
            lock_guard<recursive_mutex>        lock(_propLock);
            _inputs.clear();
            _imageInputs.clear();
            _outputs.clear();
            _imageOutputs.clear();
            _audioInputs.clear();
            _imageImports.clear();
        }
        
        
        //!    Returns a std::vector of ISFAttrRef instances describing only the receiver's image inputs.
        std::vector<ISFAttrRef> & imageInputs() { return _imageInputs; }
        //!    Returns a std::vector of ISFAttrRef instances describing only the receiver's image outputs.
        std::vector<ISFAttrRef> & imageOutputs() { return _imageOutputs; }
        //!    Returns a std::vector of ISFAttrRef instances describing only the receiver's audio inputs.
        std::vector<ISFAttrRef> & audioInputs() { return _audioInputs; }
        //!    Returns a std::vector of ISFAttrRef instances describing only the receiver's audioFFT inputs.
        std::vector<ISFAttrRef> & imageImports() { return _imageImports; }
        //!    Returns a std::vector of ISFAttrRef instances describing only the receiver's inputs that match the passed type.
        
        ISFAttrRef addAttr(ISFAttrRef attr){
            lock_guard<recursive_mutex>        lock(_propLock);
            
            ISFAttrRef alreadyRecord = get(attr->name());
            if(!!alreadyRecord)return alreadyRecord;
                 
            if(attr->isInput()){
                _inputs.push_back(attr);
                if(attr->isImage()){
                    _imageInputs.push_back(attr);
                }
            }else{
                _outputs.push_back(attr);
                if(attr->isImage()){
                    _imageOutputs.push_back(attr);
                }
            }
            return attr;
        }
        
        void rmAttr(std::string attrName){
            auto rmFrom = [](std::string name, std::vector<ISFAttrRef> & container){
                for(auto it = container.begin(); it != container.end() ; ){
                    if((*it)->name() == name){
                        (*it)->rmUIref();
                        it = container.erase(it);
                    }else{
                        it++;
                    }
                }
            };
            
            rmFrom(attrName, _inputs);
            rmFrom(attrName, _imageInputs);
            rmFrom(attrName, _outputs);
            rmFrom(attrName, _imageOutputs);
            rmFrom(attrName, _audioInputs);
            rmFrom(attrName, _imageImports);
        }
        ISFAttrRef getInput(const std::string & n){
            lock_guard<recursive_mutex>        lock(_propLock);
            for (const auto & attribRefIt : _inputs)    {
                if (attribRefIt->name() == n)
                    return attribRefIt;
            }
            return nullptr;
        }
        ISFAttrRef getOutput(const std::string & n){
            lock_guard<recursive_mutex>        lock(_propLock);
            for (const auto & attribRefIt : _outputs)    {
                if (attribRefIt->name() == n)
                    return attribRefIt;
            }
            return nullptr;
        }
        
        ISFAttrRef get(const std::string & n){
            lock_guard<recursive_mutex>        lock(_propLock);
            auto res = getInput(n);
            if(!res) res = getOutput(n);
            return res;
        }
        
        bool has(const std::string & n){
            lock_guard<recursive_mutex>        lock(_propLock);
            return !!getInput(n) || !!getOutput(n);
        }
        
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
        
        bool isEmpty(){
            return  !(_inputs.size()>0 || _imageInputs.size()>0 || _outputs.size()>0 || _imageOutputs.size()>0 || _audioInputs.size()>0 || _imageImports.size()>0) ;
        }
        
    };//ISFAttrWrapper
    typedef std::shared_ptr<ISFAttrWrapper> ISFAttrWrapperRef;
}//ISF

#endif /* ISFAttrWrapper_h */
