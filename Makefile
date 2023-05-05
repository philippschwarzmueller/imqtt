VPATH := client_src controller_src

SRC_CLIENT := mqtt_pub.c get_next_line.c get_next_line_utils.c
SRC_CONTROLLER := mqtt_sub.c

OBJ_DIR := ./_obj

OBJ_CLIENT := $(addprefix $(OBJ_DIR)/, $(SRC_CLIENT:%.c=%.o))
OBJ_CONTROLLER := $(addprefix $(OBJ_DIR)/, $(SRC_CONTROLLER:%.c=%.o))

INC := -lmosquitto

all: client controller

client: $(OBJ_CLIENT)
	gcc $(OBJ_CLIENT) -o $@ $(INC) 

controller: $(OBJ_CONTROLLER)
	gcc $(OBJ_CONTROLLER) -o $@ $(INC)

mosquitto:
	/usr/local/sbin/mosquitto --verbose

report:
	cd report_src && npm install
	cd report_src && npm start

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

re: fclean all
