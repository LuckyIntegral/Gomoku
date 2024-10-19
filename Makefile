###############################################################################
######                            PROPERTIES                             ######
###############################################################################

CPP			= gcc
RM			= rm -rf
CPPFLAGS	= -MD -MP -g
MAKEFLAGS	= -j$(nproc) --no-print-directory
LDFLAGS		= -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglfw -lstdc++ -lglut -lm

NAME		= Gomoku
SRCSDIR		= srcs
SRCS		= \
			$(SRCSDIR)/Display.cpp \
			$(SRCSDIR)/Loop.cpp \
			$(SRCSDIR)/Game.cpp \
			$(SRCSDIR)/main.cpp

OBJSDIR		= $(SRCSDIR)/objs
DEPS		= $(SRCS:$(SRCSDIR)/%.cpp=$(OBJSDIR)/%.d)
OBJS		= $(SRCS:$(SRCSDIR)/%.cpp=$(OBJSDIR)/%.o)

###############################################################################
######                              RULES                                ######
###############################################################################

all			: $(NAME)

$(NAME)		: $(OBJS)
		$(CPP) -o $@ $(OBJS) $(LDFLAGS)

$(OBJSDIR)/%.o	: $(SRCSDIR)/%.cpp
		@mkdir -p $(dir $@)
		$(CPP) $(CPPFLAGS) -c $< -o $@

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
