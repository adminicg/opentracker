
/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** header file for class UbitrackClient.
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#ifndef UBITRACKCLIENT_HH
#define UBITRACKCLIENT_HH

#include "../dllinclude.h"

#ifndef SWIG
#include <string>
#endif

namespace ot 
{

    class Context;
    class ThreadContext;

    enum ContextType {
        NORMAL = 0,
        THREAD = 1
    };

    class OPENTRACKER_API UbitrackClient {
    public:
        typedef void (*ModuleInitFunc) (Context *, void * data);
	 
    protected:
        struct MIFunctor{
            ModuleInitFunc function;
            void * data;
            //MIFunctor(ModuleInitFunc f, void * d):function(f), data(d){};
        };
    public:
        typedef std::map<std::string, MIFunctor> Registry;
    protected:
	static Registry initFunctions;
	static UbitrackClient * self;
        Context * ctx;
	bool isDefaultContext;

	static void doInitialization(Context & newctx);
      
	UbitrackClient(int ctx_type = NORMAL);

    public:
	

	static UbitrackClient * instance(int ctx_type = NORMAL);
	virtual ~UbitrackClient();

	Context * getContext();
	void sendUTQL(std::string utqlString);
	void sendUTQLFile(std::string utqlFileName);

	static void addModuleInit(const char * name, ModuleInitFunc func, void * data);
	static void loadModule(Context & newctx, const char * module);
	friend OPENTRACKER_API void initializeContext(Context *, void *);
    };

}


#endif //UBITRACKCLIENT_HH



/* 
 * ------------------------------------------------------------
 *   End of UbitrackClient.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
