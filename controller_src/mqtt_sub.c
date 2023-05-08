#include "controller.h"

//global filediscriptor for the log file
int	fd;

char	*get_logname(void)
{
	char	*logname;
	char	**splited;
	time_t	t;
	

	time(&t);
	logname = ctime(&t);
	splited = split(logname, ' ');
	logname = join_three(splited[2], splited[1], splited[3], '_');
	ft_free(splited);
	logname = strcat(logname, ".log");
	return (logname);
}

void	on_connect(struct mosquitto *mosq, void *obj, int rc)
{
	printf("ID: %i\n", * (int *) obj);
	if (rc != 0)
	{
		fprintf(stderr, "Error with result code: %i\n", rc);
		exit(EXIT_FAILURE);
	}
	mosquitto_subscribe(mosq, NULL, "sensor/temperature_1", 0);
}

void	on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	(void) mosq;
	(void) obj;

	// printf("New message with topic %s: %s\n", msg->topic, pay);
	write(fd, (char *)msg->payload, strlen((char *)msg->payload));
	write(fd, "\n", 1);
}

int	main(void)
{
	int					rc;
	int					id;
	struct mosquitto	*mosq;
	char				*logname;

	mosquitto_lib_init();
	id = 12;
	mosq = mosquitto_new("control-client", true, &id);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);
	rc = mosquitto_connect(mosq, "localhost", 1883, 10);
	if (rc != 0)
	{
		fprintf(stderr, "Could not connect to Broker with return code %i\n", rc);
		close(fd);
		return (EXIT_FAILURE);
	}
	logname = get_logname();
	fd = open(logname, O_CREAT | O_APPEND | O_RDWR, 0664);
	mosquitto_loop_start(mosq);
	printf("Press enter to quit...\n");
	getchar();
	mosquitto_loop_stop(mosq, true);
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	close(fd);
	return (EXIT_SUCCESS);
}
