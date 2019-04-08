# first input is number of threads, second is array size
# for running on stampede, array_size should be set to 98304+2
# execution of the code should be run with numactl --cpunodebind=0 --preferred=0 ./a.out
# num_threads for the 8 threads file should be set to 8

array_size=98306

# part 1 assignment 1
#g++ HW1.cpp -O2 -xcore-avx512
#numactl --cpunodebind=0 --preferred=0  echo "-1" $array_size | ./a.out > HW1_Serial.txt
#g++ HW1.cpp -O2 -xcore-avx512 -fopenmp
#numactl --cpunodebind=0 --preferred=0  echo "1" $array_size | ./a.out > HW1_1_thread.txt
#numactl --cpunodebind=0 --preferred=0  echo "8" $array_size | ./a.out > HW1_8_threads.txt

# part 1 assignment 2a
#g++ HW1_2a.cpp -O2 -xcore-avx512f -fopenmp
#numactl --cpunodebind=0 --preferred=0  echo $array_size | ./a.out > HW1_part2a.txt

# part 1 assignment 2b
#g++ HW1_2b.cpp -O2 -xcore-avx512 -fopenmp
#numactl --cpunodebind=0 --preferred=0  echo $array_size | ./a.out > HW1_part2b.txt

#part 1 assignment 2c
g++ HW1.cpp -O2 -xcore-avx512 -fopenmp
#echo "8" $array_size | ./a.out > HW1_part2c_1.txt
#numactl --cpunodebind=0 --preferred=0  echo "8" $array_size | ./a.out > HW1_part2c_2.txt
#numactl --cpunodebind=0 --interleave=0,1  echo "8" $array_size | ./a.out > HW1_part2c_3.txt
#numactl --cpunodebind=0 --preferred=1  echo "8" $array_size | ./a.out > HW1_part2c_4.txt

