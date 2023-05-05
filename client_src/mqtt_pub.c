#include "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <unistd.h>
#include <fcntl.h>

int	main(int argc, char **argv)
{
	char				*file_name;
	char				*line;
	int					rc;
	int					fd;
	struct mosquitto	*mosq;

	if (argc != 2)
	{
		printf("Usage: ./mqtt_pub [file_name]\n");
		return (-1);
	}
	file_name = argv[1];
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		printf("Could not open file\n");
		return (-1);
	}
	mosquitto_lib_init();
	mosq = mosquitto_new("publisher-test", true, NULL);

	rc = mosquitto_connect(mosq, "localhost", 1883, 60);
	if (rc != 0)
	{
		printf("Client could not connect to broker! Error code: %i\n", rc);
		mosquitto_destroy(mosq);
		mosquitto_lib_cleanup();
		return (-1);
	}
	printf("We are now connected to the broker!\n");
	line = get_next_line(fd);
	while (line != NULL)
	{
		mosquitto_publish(mosq, NULL, "sensor/temparature_1",\
				strlen(line), line, 0, false);
		free(line);
		line = get_next_line(fd);
		sleep(5);
	}
	close(fd);
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	return (0);
}
