
#include <czmq.h>


int main( int argc, char** argv )
{
    zbeacon_t* client_beacon = 0;
    zctx_t *ctx;
    zloop_t *loop;
    
    ctx = zctx_new ();
    loop = zloop_new ();
    zloop_set_verbose (loop, true);

    client_beacon = zbeacon_new( ctx, 5670 );
    zbeacon_subscribe (client_beacon, NULL, 0);
//    char *msg = zstr_recv (zbeacon_socket (client_beacon));
    /* zframe_t *frame = zframe_recv (zbeacon_socket (client_beacon)); */
    /* char* msg = zframe_data (frame); */

    char *ipaddress = zstr_recv (zbeacon_socket (client_beacon));
    zframe_t *content = zframe_recv (zbeacon_socket (client_beacon));
    char* msg = (char*)zframe_data(content);

    
    printf("server ip: %s\n", ipaddress );
    printf("got a beacon msg: %s\n", msg );

    
    
/*     zsocket_set_rcvtimeo (zbeacon_socket (client_beacon), 100); */
/*     int fd = 0; */
/*     size_t fdsz = sizeof(fd); */
/*     int rc = zmq_getsockopt( zbeacon_socket(client_beacon), ZMQ_FD, &fd, &fdsz ); */
/* //    client_beacon_timerID = BackgroundTimer_new( 1000, zeromq_beacon_timer_callback, 0 ); */


/*     zloop_start( loop ); */

    return 0;
}
