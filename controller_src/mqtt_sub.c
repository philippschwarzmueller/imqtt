#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mosquitto.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

int	fd;

char	**split(char const *s, char c);
char	**ft_free(char **str);

void	append_string(char *dest, char *src, int *i)
{
	while (*src)
	{
		dest[*i] = *src;
		src++;
		*i += 1;
	}
}

char	*join_three(char *day, char *month, char *time, char delimiter)
{
	char	*logname;
	int		size;
	int		i;

	i = 0;
	size = strlen(month) + strlen(day) + strlen(time) + 7;
	logname = malloc(size);
	append_string(logname, "log/", &i);
	append_string(logname, day, &i);
	logname[i++] = delimiter;
	append_string(logname, month, &i);
	logname[i++] = delimiter;
	append_string(logname, time, &i);
	logname[size] = '\0';
	return (logname);
}

char	*get_logname(void)
{
	char	*logname;
	char	**splited;
	time_t	t;
	

	time(&t);
	logname = ctime(&t);
	splited = split(logname, ' ');
	logname = join_three(splited[2], splited[1], splited[3], '_');
	logname = strcat(logname, ".log");
	ft_free(splited);
	return (logname);
}

void	on_connect(struct mosquitto *mosq, void *obj, int rc)
{
	printf("ID: %i\n", * (int *) obj);
	if (rc != 0)
	{
		printf("Error with result code: %i\n", rc);
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
	DIR					*dir;
	int					rc;
	int					id;
	struct mosquitto	*mosq;
	char				*logname;

	dir = opendir("log");
	if (dir)
		closedir(dir);
	else if (ENOENT == errno)
		mkdir("log", 0755);
	else
		fprintf(stderr, "Failed to create log directory");
	logname = get_logname();
	fd = open(logname, O_CREAT | O_APPEND | O_RDWR, 0664);
	mosquitto_lib_init();
	id = 12;
	mosq = mosquitto_new("control-client", true, &id);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);
	rc = mosquitto_connect(mosq, "localhost", 1883, 10);
	if (rc != 0)
	{
		printf("Could not connect to Broker with return code %i\n", rc);
		close(fd);
		return (EXIT_FAILURE);
	}
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

static size_t	ft_word_length(const char *s, size_t i, char c)
{
	size_t	count;

	count = 0;
	while (s[i] != c && s[i])
	{
		count++;
		i++;
	}
	return (count);
}

static char	*ft_fillstr(char const *s, size_t i, char c)
{
	char	*stl;
	size_t	k;
	size_t	t;

	k = 0;
	t = ft_word_length(s, i, c);
	stl = calloc(t + 1, sizeof(char));
	if (!stl)
		return (NULL);
	while (s[i] != c && s[i])
	{
		stl[k] = s[i];
		k++;
		i++;
	}
	return (stl);
}

static int	ft_count_words(const char *str, char c)
{
	int	i;
	int	trigger;

	i = 0;
	trigger = 0;
	if (c == 0)
		return (1);
	while (*str)
	{
		if (*str != c && trigger == 0)
		{
			trigger = 1;
			i++;
		}
		else if (*str == c)
			trigger = 0;
		str++;
	}
	return (i);
}

char	**ft_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

char	**split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	char	**str;

	if (!s)
		return (NULL);
	str = calloc(ft_count_words(s, c) + 1, sizeof(char *));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i])
		{
			str[j] = ft_fillstr(s, i, c);
			if (!str[j])
				return (ft_free(str));
			i += strlen(str[j]);
		}
		j++;
	}
	return (str);
}
