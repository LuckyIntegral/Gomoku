CXX = c++

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

run			:
	make fclean
	BUILD_SANITIZER=0 python3 setup.py build_ext --inplace
	python3 main.py

san				:
	make fclean
	BUILD_SANITIZER=1 python3 setup.py build_ext --inplace
	ASAN_OPTIONS=detect_leaks=0 LD_PRELOAD=$(shell $(CXX) -print-file-name=libasan.so) python3 main.py

test:
	@echo "Building optimized module (only cpp) with sanitizer"
	@if [ -f Gomoku ]; then rm Gomoku; fi
	@echo "Compiling C++ files with sanitizer flags..."
	$(CXX) -O3 -pg -std=c++11 $(shell python3-config --cflags) -o Gomoku $(filter-out bindings.cpp, $(wildcard *.cpp)) $(shell python3-config --ldflags)
#	$(CXX) -O3 -fsanitize=address,leak,undefined -std=c++20 $(shell python3-config --cflags) -o Gomoku $(filter-out bindings.cpp, $(wildcard *.cpp)) $(shell python3-config --ldflags)
#	ASAN_OPTIONS=detect_leaks=0 ./Gomoku

init	:
	pip install -r requirements.txt


.PHONY: all clean fclean re run build
