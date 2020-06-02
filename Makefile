
.PHONY: build
build:
	sudo docker build -t open_close_toggle_test .

.PHONY: run
run:
	mkdir -p ./logs && \
	xhost + local:root
	sudo docker run -it \
   	--network="host" \
	--privileged \
	--mount type=bind,src=/dev,dst=/dev,readonly \
	--mount type=bind,src=`pwd`/logs,dst=/app/logs\
	 open_close_toggle_test /bin/bash -c "./run.sh"

.PHONY: clean_log
clean_log:
	sudo rm -rf ./logs
