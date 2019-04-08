# first input is number of threads, second is array size
# for running on stampede, array_size should be set to 98304+2
# execution of the code should be run with numactl --cpunodebind=0 --preferred=0 ./a.out
# num_threads for the 8 threads file should be set to 8

array_size=10

# part 1 assignment 1
g++ HW1.cpp
numactl --cpunodebind=0 --preferred=0  echo "-1" $array_size | ./a.out > HW1_Serial.txt

g++ -fopenmp HW1.cpp
numactl --cpunodebind=0 --preferred=0  echo "1" $array_size | ./a.out > HW1_1_thread.txt

g++ -fopenmp HW1.cpp
numactl --cpunodebind=0 --preferred=0  echo "4" $array_size | ./a.out > HW1_8_threads.txt

# part 1 assignment 2a
g++ -fopenmp HW1_2a.cpp
numactl --cpunodebind=0 --preferred=0  echo $array_size | ./a.out > HW1_part2a.txt

# part 1 assignment 2b
g++ -fopenmp HW1_2b.cpp
numactl --cpunodebind=0 --preferred=0  echo $array_size | ./a.out > HW1_part2b.txt

#part 1 assignment 2c
g++ -fopenmp HW1.cpp
echo "4" $array_size | ./a.out > HW1_part2c.txt
numactl --cpunodebind=0 --preferred=0  echo "4" $array_size | ./a.out > HW1_part2c.txt
#numactl --cpunodebind=0 --interleave=0,1  echo "4" $array_size | ./a.out > HW1_part2c.txt
#numactl --cpunodebind=0 --preferred=1  echo "4" $array_size | ./a.out > HW1_part2c.txt

