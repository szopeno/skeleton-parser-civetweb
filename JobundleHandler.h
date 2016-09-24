#ifndef JOBUNDLE_HANDLERH
#define JOBUNDLE_HANDLERH

#include "CivetServer.h"
class JobundleHandler : public CivetHandler {
    private:
        const int _chunksize = 24;
        bool unzip( const char *);
    public:
	bool handlePost(CivetServer *server, struct mg_connection *conn);

};
#endif
