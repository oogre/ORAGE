#ifndef VVGL_GLTexToCPUCopier_hpp
#define VVGL_GLTexToCPUCopier_hpp

#include "VVGL_Defines.hpp"
#include "GLBufferPool.hpp"

#include <mutex>
#include <queue>




//	none of this stuff should be available if we're running ES
#if !defined(VVGL_TARGETENV_GLES) && !defined(VVGL_TARGETENV_GLES3)




namespace VVGL
{




//!	Downloads texture-based GLBuffers (Type_Tex) to CPU memory.
/*!
\ingroup VVGL_BASIC
Offers both immediate download and n-buffered texture downloads for double-/triple-/n-buffering/ping-ponging.  Uses PBOs for async DMA.
*/

class VVGL_EXPORT GLTexToCPUCopier	{
	private:
		std::recursive_mutex			_queueLock;	//	this should be used to serialize access to all member vars
		GLContextRef			_queueCtx = nullptr;	//	this is the context used to perform all GL action
		int						_queueSize = 1;	//	the number of buffers that should be in 'queue' before popping a new buffer off of it (double-buffering is 1)
		std::queue<GLBufferRef>		_cpuQueue;	//	queue of CPU-based images
		std::queue<GLBufferRef>		_pboQueue;	//	queue of PBOs
		std::queue<GLBufferRef>		_texQueue;	//	queue of textures
		std::queue<GLBufferRef>		_fboQueue;	//	queue of FBOs.  the fastest texture download pipeline involves attaching the texture to an FBO so we can use glReadPixels() instead of glGetTexImage().
		
		GLBufferPoolRef			_privatePool = nullptr;	//	by default this is null and the scene will try to use the global buffer pool to create interim resources (temp/persistent buffers).  if non-null, the scene will use this pool to create interim resources.
		
	private:
		//	before calling either of these functions, _queueLock should be locked and a GL context needs to be made current on this thread.
		void _beginProcessing(const GLBufferRef & inCPUBuffer, const GLBufferRef & inPBOBuffer, const GLBufferRef & inTexBuffer, const GLBufferRef & inFBOBuffer);
		void _finishProcessing(const GLBufferRef & inCPUBuffer, const GLBufferRef & inPBOBuffer, const GLBufferRef & inTexBuffer, const GLBufferRef & inFBOBuffer);
	
	public:
		GLTexToCPUCopier();
		GLTexToCPUCopier(const GLContextRef & inCtx) : _queueCtx(inCtx) {};
		~GLTexToCPUCopier();
		
		//!	Clears all the queues, called when freed.
		void clearStream();
		
		//!	Sets the size of the queue used for streaming.  Effectively, this is the number of calls it takes for the texture data to "finish downloading" and get returned as a pointer to non-GPU memory.
		void setQueueSize(const int & inNewQueueSize);
		//!	Returns the size of the queue used for streaming.
		inline int queueSize() { std::lock_guard<std::recursive_mutex> lock(_queueLock); return _queueSize; };
		
		//! Immediately downloads the passed texture into CPU memory- doesn't use the queues.  Less efficient.  Good for quick single-shot texture downloads.
		/*!
		\param inTexBuffer The texture-type GLBuffer that you wish to download to memory.  Must not be null.
		\param inCPUBuffer May be null (null by default).  If null, this function will return a PBO-type GLBuffer that has been mapped- you can access the pixels at its cpuBackingPtr ivar for analysis, encoding, etc.  If you provide a non-null CPU-type GLBuffer for this param, this function will instead return the CPU-type buffer you provided, after populating it with the contents of the texture.
		This function downloads the texture immediately- it doesn't use the queue/doesn't do any double-/triple-/n-buffering.  This function is generally less efficient than streamTexToCPU(), but it's still appropriate if you just want to download a texture immediately and aren't doing any extensive streaming.
		*/
		GLBufferRef downloadTexToCPU(const GLBufferRef & inTexBuffer, const GLBufferRef & inCPUBuffer=nullptr, const bool & createInCurrentContext=false);
		
		//!	Begins downloading the passed texture-based buffer to CPU memory, but stashes it in a queue and will return the CPU-based GLBuffer when this function is called again at a later time (ping-pong/double-/triple-/n-buffering).  Good for streaming texture download.
		/*!
		\param inTexBuffer This must be a texture-based GLBuffer.  This may not be null.
		\param inCPUBuffer May be null (null by default).  If null, this function will eventually return a PBO-type GLBuffer that has been mapped for 'inTexBuffer'.  You can access the pixels of this mapped PBO at its cpuBackingPtr ivar for analysis, encoding, etc.  If you provide a non-null CPU-type GLBuffer for this param, this function will instead eventually return the CPU-type buffer you provided, after populating it with the contents of the texture.
		This function is more efficient than downloadTexToCPU()- CPU use will probably be lower and execution will return to the calling thread more rapidly, though the queue means that there's more latency (it won't start returning buffers until you submit one or two- depending on the size of the queue).
		*/
		GLBufferRef streamTexToCPU(const GLBufferRef & inTexBuffer, const GLBufferRef & inCPUBuffer=nullptr, const bool & createInCurrentContext=false);
		
		//!	Sets the receiver's private buffer pool (which should default to null).  If non-null, this buffer pool will be used to generate any GL resources required by this scene.  Handy if you have a variety of GL contexts that aren't shared and you have to switch between them rapidly on a per-frame basis.
		void setPrivatePool(const GLBufferPoolRef & n) { _privatePool=n; }
		//!	Gets the receiver's private buffer pool- null by default, only non-null if something called setPrivatePool().
		GLBufferPoolRef privatePool() { return _privatePool; }
};




/*!
\relatedalso GLTexToCPUCopier
\brief Creates and returns a GLTexToCPUCopier.  The scene makes a new GL context which shares the context of the global buffer pool.
*/
inline GLTexToCPUCopierRef CreateGLTexToCPUCopierRef() { return std::make_shared<VVGL::GLTexToCPUCopier>(); }
/*!
\relatedalso GLTexToCPUCopier
\brief Creates and returns a GLTexToCPUCopier.  The downloader uses the passed GL context to perform its GL operations.
*/
inline GLTexToCPUCopierRef CreateGLTexToCPUCopierRefUsing(const GLContextRef & inCtx) { return std::make_shared<VVGL::GLTexToCPUCopier>(inCtx); }




}




#endif	//	!defined(VVGL_TARGETENV_GLES) && !defined(VVGL_TARGETENV_GLES3)




#endif /* VVGL_GLTexToCPUCopier_hpp */
