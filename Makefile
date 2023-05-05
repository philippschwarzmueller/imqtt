VPATH := client_src controller_src

SRC_CLIENT := mqtt_pub.c get_next_line.c get_next_line_utils.c
SRC_CONTROLLER := mqtt_sub.c

ERRLOG	:= $(shell mkdir -p log ; touch log/ERR.log)

OBJ_DIR := ./_obj

OBJ_CLIENT := $(addprefix $(OBJ_DIR)/, $(SRC_CLIENT:%.c=%.o))
OBJ_CONTROLLER := $(addprefix $(OBJ_DIR)/, $(SRC_CONTROLLER:%.c=%.o))

INC := -lmosquitto

all: client controller $(ERRLOG)

client: $(OBJ_CLIENT)
	gcc $(OBJ_CLIENT) -o $@ $(INC) 

controller: $(OBJ_CONTROLLER)
	gcc $(OBJ_CONTROLLER) -o $@ $(INC)

clnt:
	@echo "\033[0;32mstarting client\033[0m"
	@./client 2>>log/ERR.log

ctrl:
	@echo "\033[0;32mstarting controller\033[0m"
	@./controller 2>>log/ERR.log

mosquitto:
	/usr/local/sbin/mosquitto --verbose

mosqlin:
	mosquitto -v

$(OBJ_DIR)/%.o: %.c
	@mkdir -p _obj
	gcc -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f client
	rm -f controller

errclean:
	rm log/ERR.log

logclean:
	rm -rf log/

re: fclean all
