NAME = APP

build	:
		pip install -r requirements.txt

run		:
		python3 main.py

.PHONY: build run
