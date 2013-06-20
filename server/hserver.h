//
//  hserver.h
//  vMessageServer
//
//  Created by zhang hailong on 13-6-20.
//  Copyright (c) 2013年 hailong.org. All rights reserved.
//

#ifndef vMessageServer_hserver_h
#define vMessageServer_hserver_h


#ifdef __cplusplus
extern "C" {
#endif
    
    
    typedef struct _SRVServer {
        int listenSocket;
        pthread_mutex_t listenMutex;
    } SRVServer;
    
    int SRVServerAccept(SRVServer * server,double timeout,struct sockaddr * addr,socklen_t * socklen);
    
    struct _SRVProcess;
    
    typedef int (* SRVProcessCreate) (SRVServer * server,struct _SRVProcess * process);
    
    typedef void (* SRVProcessExit) (SRVServer * server,struct _SRVProcess * process);
    
    typedef void (* SRVProcessOpen) (SRVServer * server,struct _SRVProcess * process);
    
    typedef double (* SRVProcessTick) (SRVServer * server,struct _SRVProcess * process);
    
    typedef void (* SRVProcessClose) (SRVServer * server,struct _SRVProcess * process);
    
    typedef struct _SRVProcessClass{
        SRVProcessCreate create;
        SRVProcessCreate exit;
        SRVProcessCreate open;
        SRVProcessCreate tick;
        SRVProcessCreate close;
    } SRVProcessClass;
    
    typedef struct _SRVProcess {
        const SRVProcessClass * clazz;
        pid_t pid;
        int exit;
    } SRVProcess;

#ifdef __cplusplus
}
#endif

#endif
