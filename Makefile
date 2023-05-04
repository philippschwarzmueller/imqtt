all: client controller

client:
	gcc -l mosquitto ./client_src/mqtt_pub.c -o client

controller:
	gcc -l mosquitto ./controller_src/mqtt_sub.c -o controller

clean:
	rm client
	rm controller
