SHELL:=/bin/bash

.DEFAULT_GOAL := build 

PROJECT:=lxqt-notificationd
TAG:=latest

.EXPORT_ALL_VARIABLES:
DOCKER_BUILDKIT?=1
DOCKER_CONFIG?=

DOCKERFILE=Dockerfile.debian_bookworm

.PHONY: build
build: clean
	cd .. && sed -i "s|1.4.0|1.2.0|g" CMakeLists.txt 
	cd .. && docker build -f "docker/${DOCKERFILE}" -t ${PROJECT}:${TAG}  .
	git checkout -- ../CMakeLists.txt
	cd .. && docker cp $$(docker create --rm ${PROJECT}:${TAG}):/tmp/lxqt-notificationd/build .
	cd .. && docker cp $$(docker create --rm ${PROJECT}:${TAG}):/usr/lib/x86_64-linux-gnu/liblxqt.so.1 build
	cd .. && docker cp $$(docker create --rm ${PROJECT}:${TAG}):/usr/lib/x86_64-linux-gnu/liblxqt.so.1.2.0 build


.PHONY: clean
clean: 
	cd .. && rm -rf build 
	docker rm $$(docker ps -a -q --filter "ancestor=${PROJECT}:${TAG}") --force 2> /dev/null || true
	docker rmi $$(docker images -q ${PROJECT}:${TAG}) --force 2> /dev/null || true


.PHONY: run
run:
	LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$(shell realpath ../build)" ./../build/src/lxqt-notificationd
