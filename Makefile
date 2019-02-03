## -*- Makefile -*-
##
## User: jerasonbanes
## Time: Feb 3, 2019 2:01:18 AM
## Makefile created by Oracle Developer Studio.
##
## This file is generated automatically.
##


dingux-2600.ipk: src
	$(MAKE) -C src
	./package

clean: 
	$(MAKE) -C src clean


