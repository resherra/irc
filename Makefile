SRC=${wildcard src/*.cpp}
OBJ=$(SRC:.cpp=.o)
HEADERS=${wildcard headers/*.hpp}
NAME=ircserv
FLAGS=-Wall -Wextra -std=c++98  

all: $(NAME)

$(NAME): $(OBJ)
	c++ $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADERS)
	c++ $(FLAGS) -c $< -o $@

run:
	./ircserv 6667 connect

clean:
	rm $(OBJ)

fclean: clean
	    rm $(NAME)

re: fclean all
