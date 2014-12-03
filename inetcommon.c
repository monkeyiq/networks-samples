
void showAddress( char* msg, inaddr_t address )
{
    printf("showAddress() %s: %d.%d.%d.%d\n",
           msg,
           address.sin_addr.s_addr     & 0xFF,
           address.sin_addr.s_addr>>8  & 0xFF,
           address.sin_addr.s_addr>>16 & 0xFF,
           address.sin_addr.s_addr>>24 & 0xFF );
}

void zsys_socket_error( char* msg )
{
    int e = errno;
    printf("error: %d %s %s\n", e, strerror(e), msg );
    exit(1);
}
