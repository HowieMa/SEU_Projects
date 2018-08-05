STUDENT_ID=09015412 & 09015422 & 09015424 & 09015429

all: producer_consumer

producer_consumer: producer_consumer.c
	gcc -g -o producer_consumer producer_consumer.c -lpthread -lm 

tar: clean
	make clean
	mkdir $(STUDENT_ID)-pthreads_pc-lab
	cp producer_consumer.c Makefile $(STUDENT_ID)-pthreads_pc-lab/
	tar cvzf $(STUDENT_ID)-pthreads_pc-lab.tar.gz $(STUDENT_ID)-pthreads_pc-lab
	rm -rf $(STUDENT_ID)-pthreads_pc-lab

all-tests: test1 test2 test3 test4

test1: producer_consumer
	@echo "  P-1 C-1   " > narrative1.raw
	@echo "************" >> narrative1.raw
	@echo "    RAW     " >> narrative1.raw
	@echo "************" >> narrative1.raw
	@./producer_consumer 1 1 >> narrative1.raw
	@echo "  P-1 C-1   " > narrative1.sorted
	@echo "************" >> narrative1.sorted
	@echo "  Sorted    " >> narrative1.sorted
	@echo "************" >> narrative1.sorted
	@grep "con 0"  narrative1.raw >> narrative1.sorted
	@echo "    " >> narrative1.sorted
	@grep "prod 0" narrative1.raw >> narrative1.sorted

test2: producer_consumer
	@echo "  P-1 C-5   " > narrative2.raw
	@echo "************" >> narrative2.raw
	@echo "    RAW     " >> narrative2.raw
	@echo "************" >> narrative2.raw
	@./producer_consumer 1 5 >> narrative2.raw
	@echo "  P-1 C-5   " > narrative2.sorted
	@echo "************" >> narrative2.sorted
	@echo "  Sorted    " >> narrative2.sorted
	@echo "************" >> narrative2.sorted
	@grep "con 0"  narrative2.raw >> narrative2.sorted
	@echo "    " >> narrative2.sorted
	@grep "con 1"  narrative2.raw >> narrative2.sorted
	@echo "    " >> narrative2.sorted
	@grep "con 2"  narrative2.raw >> narrative2.sorted
	@echo "    " >> narrative2.sorted
	@grep "con 3"  narrative2.raw >> narrative2.sorted
	@echo "    " >> narrative2.sorted
	@grep "con 4"  narrative2.raw >> narrative2.sorted
	@echo "    " >> narrative2.sorted
	@grep "prod 0" narrative2.raw >> narrative2.sorted

test3: producer_consumer
	@echo "  P-5 C-1   " > narrative3.raw
	@echo "************" >> narrative3.raw
	@echo "    RAW     " >> narrative3.raw
	@echo "************" >> narrative3.raw
	@./producer_consumer 5 1 >> narrative3.raw
	@echo "  P-5 C-1   " > narrative3.sorted
	@echo "************" >> narrative3.sorted
	@echo "  Sorted    " >> narrative3.sorted
	@echo "************" >> narrative3.sorted
	@grep "con 0"  narrative3.raw >> narrative3.sorted
	@echo "    " >> narrative3.sorted
	@grep "prod 0" narrative3.raw >> narrative3.sorted
	@echo "    " >> narrative3.sorted
	@grep "prod 1" narrative3.raw >> narrative3.sorted
	@echo "    " >> narrative3.sorted
	@grep "prod 2" narrative3.raw >> narrative3.sorted
	@echo "    " >> narrative3.sorted
	@grep "prod 3" narrative3.raw >> narrative3.sorted
	@echo "    " >> narrative3.sorted
	@grep "prod 4" narrative3.raw >> narrative3.sorted

test4: producer_consumer
	@echo "  P-5 C-5   " > narrative4.raw
	@echo "************" >> narrative4.raw
	@echo "    RAW     " >> narrative4.raw
	@echo "************" >> narrative4.raw
	@./producer_consumer 5 5 >> narrative4.raw
	@echo "  P-5 C-5   " > narrative4.sorted
	@echo "************" >> narrative4.sorted
	@echo "  Sorted    " >> narrative4.sorted
	@echo "************" >> narrative4.sorted
	@grep "con 0"  narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "con 1"  narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "con 2"  narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "con 3"  narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "con 4"  narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "prod 0" narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "prod 1" narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "prod 2" narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "prod 3" narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "prod 4" narrative4.raw >> narrative4.sorted

clean:
	rm -f *~ *.raw *.sorted producer_consumer

distclean: clean
