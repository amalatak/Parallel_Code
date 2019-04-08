// this program is very similar to "HW1.cpp", but reuses the same 
// initialized array in order to save time for running many different 
// thread count smoothing kernels. This is for PCSE Homework 1 part 1
// Assignment 2a


#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <time.h>
#include <chrono>
#include <stdio.h>

using std::cout;
using std::endl;
using std::cin;
using namespace std::chrono;

void initialize(float **arr, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            arr[i][j] = 1.*rand()/RAND_MAX;
        }
    }
}

void smooth(float **arr_x, float **arr_y, int n, float a, float b, float c) {
    #pragma omp parallel for 
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < n - 1; j++) {
            arr_y[i][j] = a*(arr_x[i-1][j-1] + arr_x[i-1][j+1] + arr_x[i+1][j-1] + arr_x[i+1][i+1]) +
            b*(arr_x[i-1][j] + arr_x[i+1][j] + arr_x[i][j-1] + arr_x[i][j+1]) + 
            c*arr_x[i][j];
        }
    }    
}

int count(float **arr, int n, float t, int & nbelow) {
    nbelow = 0;
    #pragma omp parallel for 
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < n - 1; j++) {
            if (arr[i][j] < t) {
                nbelow++;
            }
        }
    }    
}



int main() {
    // create array of thread counts
    int nts[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
    int nts_len = 9;

    #ifdef _OPENMP
	    omp_set_num_threads(4);
        int thread;
        printf("Testing Prallel Region...\n");
        #pragma omp parallel for
        for (int i = 0; i < 4; i++) {
            thread = omp_get_thread_num();
            printf("This is thread: %d\n", thread);
        }
	#endif


    srand(time(NULL)); // randomly seed rand

    // ------ Initialize Variables ------ //


    int n, nbelow_x, nbelow_y;
    cin >> n;
    float a, b, c, t, frac_below_x, frac_below_y;
    time_point<system_clock> start, end;
    a = .05; b = .1; c = .4; t = .1;
    float **arr_y, **arr_x;



    start = system_clock::now();
    arr_x = new float*[n];
    for(int i = 0; i < n; ++i) {
        arr_x[i] = new float[n];
    }
    end = system_clock::now(); 
    duration<double> elapsed_seconds_x_alloc = end - start;

    start = system_clock::now();
    arr_y = new float*[n];
    for(int i = 0; i < n; ++i) {
        arr_y[i] = new float[n];
    }
    end = system_clock::now(); 
    duration<double> elapsed_seconds_y_alloc = end - start;


    // ---------- perform operations -------- //

    start = system_clock::now();
    initialize(arr_x, n);
    end = system_clock::now(); 
    duration<double> elapsed_seconds_init = end - start;

    double count_time_x_vec[nts_len], count_time_y_vec[nts_len], smoothing_time_vec[nts_len];
    int thread_vec[nts_len];

    for (int j = 0; j < nts_len; j++) {
        omp_set_num_threads(nts[j]);

        start = system_clock::now();
        smooth(arr_x, arr_y, n, a, b, c);
        end = system_clock::now(); 
        duration<double> elapsed_seconds_smooth = end - start;
        smoothing_time_vec[j] = elapsed_seconds_smooth.count();

        start = system_clock::now();
        count(arr_x, n, t, nbelow_x);
        end = system_clock::now(); 
        duration<double> elapsed_seconds_count_x = end - start;
        count_time_x_vec[j] = elapsed_seconds_count_x.count();

        start = system_clock::now();
        count(arr_y, n, t, nbelow_y);
        end = system_clock::now(); 
        duration<double> elapsed_seconds_count_y = end - start;
        count_time_y_vec[j] = elapsed_seconds_count_y.count();

        thread_vec[j] = omp_get_max_threads();

        frac_below_x = (float) nbelow_x / (float)((n-2)*(n-2));
        frac_below_y = (float) nbelow_y / (float)((n-2)*(n-2));
    }

    // ------------ present data ------------ //


    cout << "\n" << endl;
    cout << "Summary" << endl;
    cout << "-------" << endl;
    cout << "Number of elements in a row/col       :: " << n << endl;
    cout << "Number of inner elements in a row/col :: " << (n-2) << endl;
    cout << "Total number of elements              :: " << n*n << endl;
    cout << "Total number of inner elements        :: " << (n-2)*(n-2) << endl;
    cout << "Memory (MB) in each Array             :: " << sizeof(arr_x)*(n)*n/(1000000.) << endl;
    cout << "Threshold                             :: " << t << endl;
    cout << "Smoothing constants (a, b, c)         :: " << a << " " << b << " " << c << endl;
    cout << "Number of elements below threshold (X):: " << nbelow_x << endl;
    cout << "Fraction of elements below threshold  :: " << frac_below_x << endl;
    cout << "Number of elements below threshold (Y):: " << nbelow_y << endl;
    cout << "Fraction of elements below threshold  :: " << frac_below_y << endl;
    cout << "Number of Threads                     :: "; 
    for (int i = 0; i < nts_len; i++) {
        cout << thread_vec[i] << " ";
    } cout << endl;
    cout << "\n\n" << endl;
    cout << "Action               ::   time (s)" << endl;
    cout << "------" << endl;
    cout << "CPU Alloc-X          :: " << elapsed_seconds_x_alloc.count() << endl;
    cout << "CPU Alloc-Y          :: " << elapsed_seconds_y_alloc.count() << endl;
    cout << "CPU Init-X           :: " << elapsed_seconds_init.count() << endl;
    cout << "CPU Smooth           :: ";
    for (int i = 0; i < nts_len; i++) {
        cout << smoothing_time_vec[i] << " ";
    } cout << endl;
    cout << "CPU Count-X          :: ";
    for (int i = 0; i < nts_len; i++) {
        cout << count_time_x_vec[i] << " ";
    } cout << endl;
    cout << "CPU Count-Y          :: ";
    for (int i = 0; i < nts_len; i++) {
        cout << count_time_y_vec[i] << " ";
    } cout << endl;
   



// ---------- terminate arrays ---------- //

    for (int i = 0; i < n; i++) {
        delete[] arr_x[i]; // Delete columns
        }
    delete[] arr_x; // Delete Rows

    for (int i = 0; i < n; i++) {
        delete[] arr_y[i]; // Delete columns
        }
    delete[] arr_y; // Delete Rows
    return 0;
}
