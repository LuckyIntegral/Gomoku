###############################################################################
######                            PROPERTIES                             ######
###############################################################################

CPP			= c++
RM			= rm -rf
CPPFLAGS	= -MD -MP -g -std=c++20
MAKEFLAGS	= -j$(nproc) --no-print-directory

NAME		= Gomoku
SRCSDIR		= srcs
SRCS		= ${SRCSDIR}/main.cpp

OBJSDIR		= ${SRCSDIR}/objs
DEPS		= $(SRCS:${SRCSDIR}/%.cpp=${OBJSDIR}/%.d)
OBJS		= $(SRCS:${SRCSDIR}/%.cpp=${OBJSDIR}/%.o)

###############################################################################
######                              RULES                                ######
###############################################################################

all			: $(NAME)

$(NAME)		: ${OBJS}
		$(CPP) -o $@ ${OBJS}

${OBJSDIR}/%.o	: ${SRCSDIR}/%.cpp
		@mkdir -p $(dir $@)
		${CPP} ${CPPFLAGS} -c $< -o $@

clean	:
		$(RM) $(OBJSDIR)

fclean	: clean
		$(RM) $(OBJSDIR) $(NAME)

re		:
		$(MAKE) fclean
		$(MAKE) all

run		:
		$(MAKE) all
		clear
		@./$(NAME)

-include $(DEPS)

.PHONY: all clean fclean re run
