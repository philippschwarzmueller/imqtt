#include "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

// Function for creating wrong data 
// to send it to the controller
static char	*create_random_string(void)
{
	const char	charset[] = "0123456789";
	char		*error;
	char		random[4];
	int			i;

	random[2] = ' ';
	i = -1;
	while (++i < 2)
		random[i] = charset[rand() % (sizeof(charset) - 1)];
	random[i + 1] = '\0';
	error = calloc(3 + strlen("Temperature:") + 1, sizeof(char));
	strcpy(error, "Temperature:");
	strcat(error, random);
	return (error);
}

// Get time for sending it together with the
// data from the simulated data set
static char	*get_publish_time(void)
{
	time_t		now;
	struct tm	*local_time;
	char		*timestamp;

	now = time(NULL);
	local_time = localtime(&now);
	timestamp = calloc(50, sizeof(char));
	strftime(timestamp, 50, "%Y-%m-%dT%H:%M:%S", local_time);
	return (timestamp);
}

int	main(void)
{
	char				file_name[] = "./sensordata_src/sensor_simulated_data.txt";
	char				*timestamp;
	char				*line;
	char				*pub_string;
	int					send_error;
	int					rc;
	int					fd;
	struct mosquitto	*mosq;

	srand(time(NULL));
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "Could not open file\n");
		return (EXIT_FAILURE);
	}
	mosquitto_lib_init();
	mosq = mosquitto_new("temperature_sensor", true, NULL);
	// set a last will message for the client
	mosquitto_will_set(mosq, "sensor/temperature_1", 39,\
			"Temperature sensor shut down unintended", 0, false);
	// connect to the MQTT broker
	rc = mosquitto_connect(mosq, "localhost", 1883, 60);
	if (rc != 0)
	{
		fprintf(stderr, "Client could not connect to broker! Error code: %i\n", rc);
		mosquitto_destroy(mosq);
		mosquitto_lib_cleanup();
		return (EXIT_FAILURE);
	}
	printf("Client is now connected to the broker!\n");
	line = get_next_line(fd);
	// Loop for sending the data from the simulated data set
	// to the controller for testing
	while (line != NULL)
	{
		send_error = rand() % 4;
		timestamp = get_publish_time();
		// Randomly send wrong data
		if (send_error == 2)
		{
			free(line);
			line = create_random_string();
		}
		else
			line[strlen(line) - 1] = ' ';
		pub_string = calloc(strlen(timestamp) + strlen(line) + 1,\
				sizeof(char));
		if (pub_string == NULL)
		{
			mosquitto_destroy(mosq);
			mosquitto_lib_cleanup();
			fprintf(stderr, "Malloc failed\n");
			return (EXIT_FAILURE);
		}
		strcpy(pub_string, line);
		strcat(pub_string, timestamp);
		mosquitto_publish(mosq, NULL, "sensor/temperature_1",\
				strlen(pub_string), pub_string, 0, false);
		free(line);
		free(pub_string);
		line = get_next_line(fd);
		sleep(1);
	}
	free(timestamp);
	close(fd);
	// After finish sending the data,
	// disconnect properly from the broker
	// and cleanup
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	return (EXIT_SUCCESS);
}
