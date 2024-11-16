
all		: build run

run		:
	python3 main.py

build	:
	pip install -r requirements.txt
	python3 setup.py build_ext --inplace


.PHONY: build run all pyrun
