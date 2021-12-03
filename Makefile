############################################################
# This makefile only works for the simulator target so far #
############################################################

all: main.elf

VBXSIM=./vbx-mxp-simulator_only/repository/lib/vbxsim
VBXAPI=./vbx-mxp-simulator_only/repository/lib/vbxapi
N=4

main.elf: main.c $(VBXAPI)/libvbxapi.a $(VBXSIM)/libvbxsim.a
	gcc -D N=$(N) -Wall -g -std=c99 -DVBX_SIMULATOR -I$(VBXAPI) -o $@ $^


$(VBXAPI)/libvbxapi.a  $(VBXSIM)/libvbxsim.a:
	make -C $(dir $@) SIMULATOR=true

clean:
	rm -rf main.elf
	make -C $(VBXAPI) clean SIMULATOR=true
	make -C $(VBXSIM) clean SIMULATOR=true

.phony: all clean
