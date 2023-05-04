/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mqtt_pub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makurz <dumba@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 18:40:06 by makurz            #+#    #+#             */
/*   Updated: 2023/05/05 00:18:23 by makurz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

char	*get_publish_time(void)
{
	time_t		now;
	struct tm	*local_time;
	char		*timestamp;

	now = time(NULL);
	local_time = localtime(&now);
	timestamp = calloc(50, sizeof(char));
	strftime(timestamp, 50, "%Y-%m-%dT%H-%M-%S", local_time);
	return (timestamp);
}

int	main(int argc, char **argv)
{
	char				*file_name;
	char				*timestamp;
	char				*line;
	char				*pub_string;
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
	printf("Client is now connected to the broker!\n");
	line = get_next_line(fd);
	while (line != NULL)
	{
		timestamp = get_publish_time();
		line[strlen(line) - 1] = ' ';
		pub_string = calloc(strlen(timestamp) + strlen(line) + 1,\
				sizeof(char));
		printf("##%s##\n", timestamp);
		if (pub_string == NULL)
		{
			mosquitto_destroy(mosq);
			mosquitto_lib_cleanup();
			printf("Malloc failed\n");
			return (-1);
		}
		strcpy(pub_string, line);
		strcat(pub_string, timestamp);
		mosquitto_publish(mosq, NULL, "sensor/temperature_1",\
				strlen(pub_string), pub_string, 0, false);
		free(line);
		free(pub_string);
		line = get_next_line(fd);
		sleep(5);
	}
	free(timestamp);
	close(fd);
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	return (0);
}
