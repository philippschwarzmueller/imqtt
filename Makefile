all: client controller report

client:
	gcc -l mosquitto ./client_src/mqtt_pub.c ./client_src/get_next_line.c ./client_src/get_next_line_utils.c -o client

controller:
	gcc -l mosquitto ./controller_src/mqtt_sub.c -o controller

mosquitto:
	/usr/local/sbin/mosquitto --verbose

report:
	cd report_src && npm install

clean:
	rm client
	rm controller

re: clean all
