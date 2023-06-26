# Colors
_END		= \033[0m
_COMP		= \033[1;33m
_BUILD		= \033[1;32m
_REMOVE		= \033[1;31m

# Messages
COMP		= Compiling
BUILD		= Building
REMOVE		= Removing
MSG_COMP	= ${_COMP}${COMP}:	${_END}
MSG_BUILD	= ${_BUILD}${BUILD}:	${_END}
MSG_RMV		= ${_REMOVE}${REMOVE}:	${_END}

# ============================================================================ #

# Paths
SRCS_PATH	= srcs/
INCS_PATH	= incs/
INCS_SUBDIR	= cmd/ network/ utils/
INCS_PATH	+= $(addprefix incs/, $(INCS_SUBDIR))
OBJS_PATH	= objs/

# Names and files
NAME		= ircserv
FILES		= utils/main.cpp
F_CMD		= JOIN.cpp NICK.cpp NOTICE.cpp PART.cpp PASS.cpp PRIVMSG.cpp \
			QUIT.cpp USER.cpp reply.cpp INVITE.cpp MODE.cpp KICK.cpp
FILES		+= $(addprefix cmd/, $(F_CMD))
F_NETWORK	= Channel.cpp Client.cpp Server.cpp User.cpp
FILES		+= $(addprefix network/, $(F_NETWORK))
F_UTILS		= check.cpp print.cpp
FILES		+= $(addprefix utils/, $(F_UTILS))

OBJS		= $(addprefix $(OBJS_PATH), $(FILES:.cpp=.o))
DEP			= $(OBJS:.o=.d)

# Flags + compilation
INCS		= $(addprefix -I, $(INCS_PATH))
CXXFLAGS 	= -Wall -Wextra -Werror -std=c++98 -MMD
CXX 		= c++

# Optional flags
CXXFLAGS 	+= -O3
# CXXFLAGS 	+= -g3
# CXXFLAGS	+= -fsanitize=address

# Object making rule
$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCS) 
	@echo "${MSG_COMP}$<"

# Compiling command
all: ${NAME}

${NAME}: ${OBJS}
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "${MSG_BUILD}${NAME}${MSG_FINISH}"

-include $(DEP)

# Other rules
clean:
ifneq (,$(wildcard ${OBJS_PATH}))
	@rm -rf ${OBJS_PATH}
	@echo "${MSG_RMV}${OBJS_PATH}"
endif

fclean: clean
ifneq (,$(wildcard ${NAME}))
	@rm -f ${NAME}
	@echo "${MSG_RMV}${NAME}"
endif

re:
	@make fclean -s
	@make all -s

run: re
	@echo -n "\033[1;34m \nRUN => \033[0m"
	./ircserv 6667 password

.PHONY: all clean fclean re run
