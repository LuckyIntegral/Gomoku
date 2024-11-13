###############################################################################
######                            PROPERTIES                             ######
###############################################################################

CPP			= clang++
RM			= rm -rf
CPPFLAGS	= -MD -MP -g -std=c++20
MAKEFLAGS	= -j$(nproc) --no-print-directory
LDFLAGS		= -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglfw -lstdc++ -lglut -lm

NAME		= Gomoku
SRCSDIR		= srcs
SRCS		= \
			$(SRCSDIR)/Display.cpp \
			$(SRCSDIR)/Loop.cpp \
			$(SRCSDIR)/Game.cpp \
			$(SRCSDIR)/main.cpp \
			$(SRCSDIR)/constants.cpp \
			${SRCSDIR}/AI.cpp

OBJSDIR		= $(SRCSDIR)/objs
DEPS		= $(SRCS:$(SRCSDIR)/%.cpp=$(OBJSDIR)/%.d)
OBJS		= $(SRCS:$(SRCSDIR)/%.cpp=$(OBJSDIR)/%.o)
BUILD 		= build

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
		$(RM) $(BUILD)

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

build	:
	python setup.py build_ext --inplace

pyrun	:
	python main.py

.PHONY: all clean fclean re run
