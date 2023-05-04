/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mqtt_sub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makurz <dumba@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 19:22:07 by makurz            #+#    #+#             */
/*   Updated: 2023/05/04 21:33:23 by makurz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mosquitto.h>

void	on_connect(struct mosquitto *mosq, void *obj, int rc)
{
	printf("ID: %i\n", * (int *) obj);
	if (rc != 0)
	{
		printf("Error with result code: %i\n", rc);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, "sensor/temperature_1", 0);
}

void	on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	(void) mosq;
	(void) obj;
	printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
}

int	main(void)
{
	int					rc;
	int					id;
	struct mosquitto	*mosq;

	mosquitto_lib_init();
	id = 12;
	mosq = mosquitto_new("subscribe-test", true, &id);
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
	return (0);
}
