/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mqtt_sub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makurz <dumba@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 19:22:07 by makurz            #+#    #+#             */
/*   Updated: 2023/05/04 16:30:51 by luntiet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mosquitto.h>
// #include </Users/luntiet/.brew/include/mosquitto.h>
#include <fcntl.h>
#include <time.h>

int	fd;

char	*get_logname(void)
{
	char	*logname;
	time_t	t;

	time(&t);
	logname = ctime(&t);
	return (logname);
}

void	on_connect(struct mosquitto *mosq, void *obj, int rc)
{
	printf("ID: %i\n", * (int *) obj);
	if (rc != 0)
	{
		printf("Error with result code: %i\n", rc);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, "test/t1", 0);
}

void	on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	(void) mosq;
	(void) obj;

	printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
	write(fd, msg->payload, strlen(msg->payload));
}

int	main(void)
{
	int					rc;
	int					id;
	struct mosquitto	*mosq;
	char				*logname;

	logname = get_logname();
	printf("%s", logname);
	// fd = open(logname, O_CREAT, 0557);
	mosquitto_lib_init();
	id = 12;
	mosq = mosquitto_new("subscrite-test", true, &id);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);

	rc = mosquitto_connect(mosq, "localhost", 1883, 10);
	if (rc != 0)
	{
		printf("Could not connect to Broker with return code %i\n", rc);
		return (-1);
	}
	mosquitto_loop_start(mosq);
	printf("Press enter to quit...\n");
	getchar();
	mosquitto_loop_stop(mosq, true);
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	close(fd);
	return (0);
}
