NAME = game_module.cpython-310-x86_64-linux-gnu.so

all		: $(NAME)

$(NAME)	:
	python3 setup.py build_ext --inplace

clean	:
	rm -rf build

fclean	: clean
	rm -rf $(NAME)
	rm -rf gomoku/__pycache__

re		:
	make fclean
	make all

run		:
	python3 main.py

init	:
	pip install -r requirements.txt


.PHONY: all clean fclean re run build
