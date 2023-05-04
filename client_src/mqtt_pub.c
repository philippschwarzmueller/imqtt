/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mqtt_pub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makurz <dumba@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 18:40:06 by makurz            #+#    #+#             */
/*   Updated: 2023/05/04 16:31:04 by luntiet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
// #include </Users/luntiet/.brew/include/mosquitto.h>
#include <unistd.h>

int	main(void)
{
	int					rc;
	struct mosquitto	*mosq;

	mosquitto_lib_init();
	mosq = mosquitto_new("publisher-test", true, NULL);

	rc = mosquitto_connect(mosq, "localhost", 1883, 60);
	if (rc != 0)
	{
		printf("Client could not connect to broker! Error code: %i\n", rc);
		mosquitto_destroy(mosq);
		return (-1);
	}
	printf("We are now connected to the broker!\n");
	int i = 0;
	while (i < 10)
	{
		mosquitto_publish(mosq, NULL, "test/t1", 6, "Hello", 0, false);
		++i;
		sleep(3);
	}

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	return (0);
}
