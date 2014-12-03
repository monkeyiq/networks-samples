
#include <czmq.h>


int main( int argc, char** argv )
{
    zbeacon_t *service_beacon = 0;
    zctx_t *ctx;
    zloop_t *loop;
    char* msg = "hi there client, this is the beacon test...";
    
    ctx = zctx_new ();
    loop = zloop_new ();
    zloop_set_verbose (loop, true);

    service_beacon = zbeacon_new( ctx, 5670 );
    zbeacon_set_interval( service_beacon, 300 );
    zbeacon_publish( service_beacon, (byte*)msg, strlen(msg)+1);

    zloop_start( loop );

    return 0;
}
