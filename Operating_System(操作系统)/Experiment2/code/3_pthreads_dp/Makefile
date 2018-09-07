# Add the dp_asymmetric and dp_waiter targets to all as you implement
# them
#
MY_LAST_NAME = "09015412 马浩宇 & 09015422 陈一鸣 & 09015424 吴嘉诚 & 09015429 宋晓伟"

all: dp # dp_asymmetric dp_waiter

dp: dining_philosophers.c
	gcc -g dining_philosophers.c -lpthread -lm -o dp

dp_asymmetric: dp_asymmetric.c
	gcc -g dp_asymmetric.c -lpthread -lm -o dp_asymmetric

dp_waiter: dp_waiter.c
	gcc -g dp_waiter.c -lpthread -lm -o dp_waiter

# Add the dp_asymmetric_test and dp_waiter_test targets to test as you implement
# them
#
test: dp_test #dp_asymmetric_test dp_waiter_test

dp_test: dp
	./dp

dp_asymmetric_test: dp_asymmetric
	./dp_asymmetric

dp_waiter_test: dp_waiter
	./dp_waiter

clean:
	rm -f dp dp_asymmetric dp_waiter

tar: 
	make clean
	mkdir $(STUDENT_ID)-pthreads_dp-lab
	cp Makefile dining_philosophers.c dp_asymmetric.c dp_waiter.c $(STUDENT_ID)-pthreads_dp-lab/
	tar cvzf $(STUDENT_ID)-pthreads_dp-lab.tar.gz $(STUDENT_ID)-pthreads_dp-lab
	rm -rf $(STUDENT_ID)-pthreads_dp-lab

