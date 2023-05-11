#ifndef CONTROLLER_H
# define CONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mosquitto.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>

char	*join_three(char *day, char *month, char *time, char delimiter);
char	**split(char const *s, char c);
char	**ft_free(char **str);

#endif
