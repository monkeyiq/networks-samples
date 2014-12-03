

all: uuid beaconserv beaconclient

uuid: uuid.c
	gcc uuid.c -o uuid `pkg-config --cflags --libs libczmq`

beaconserv: beaconserv.c
	gcc beaconserv.c -o beaconserv  `pkg-config --cflags --libs libczmq`

beaconclient: beaconclient.c
	gcc beaconclient.c -o beaconclient  `pkg-config --cflags --libs libczmq`



udpsend: udpsend.c
	gcc -g udpsend.c -o udpsend

udpsend-osx: udpsend-osx.c
	gcc -g udpsend-osx.c -o udpsend-osx

clean:
	rm -f udpsend beaconclient beaconserv uuid udpsend-osx

synctorsd:
	rsync -av . rsd:~/src/github-monkeyiq/networks-samples
