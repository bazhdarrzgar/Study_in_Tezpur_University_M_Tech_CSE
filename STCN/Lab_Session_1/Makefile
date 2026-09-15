# Master Makefile for STCN Lab Experiments (Session 1)
# Course: CSMT 502 - Selected Topics in Computer Networks

SUBDIRS = Exp01_Framing \
          Exp02_Error_Detection \
          Exp03_Error_Correction \
          Exp04_TCP_Sockets \
          Exp05_UDP_Sockets \
          Exp06_Routing_Algorithms \
          Exp07_Flow_Control \
          Exp08_Traffic_Shaping \
          Exp09_Cryptography_Ciphers \
          Exp10_RSA_Algorithm

.PHONY: all clean test $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	@echo "============================================="
	@echo "Building $@"
	@echo "============================================="
	$(MAKE) -C $@ all

clean:
	@for dir in $(SUBDIRS); do \
		echo "Cleaning $$dir"; \
		$(MAKE) -C $$dir clean; \
	done
	@rm -f Exp11_NS2_Simulation/out.tr Exp11_NS2_Simulation/out.nam

test: all
	@bash run_all_tests.sh
