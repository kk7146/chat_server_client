NAME			=	chat-client

# flags
NCUR			=	-lncurses

# command
CC				=	gcc
RM				=	rm -rf

# directory
SRC_DIR			=	srcs/
INC_DIR			=	./includes

# srcs
SRCS			=	$(SRC_DIR)main.c $(SRC_DIR)chat.c $(SRC_DIR)login.c $(SRC_DIR)ui_utils.c
OBJS			=	$(SRCS:.cpp=.o)

all:	$(NAME)

.cpp.o: 
	$(CC) -c $< -o $@ -I $(INC_DIR) $(NCUR)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) -I $(INC_DIR) $(NCUR)

clean:
	rm -f $(OBJS) ./srcs/$(NAME)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re