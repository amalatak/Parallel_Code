#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <omp.h>
#include <time.h>
#include <chrono>
#include <stdio.h>

using std::cout;
using std::endl;
using std::cin;
using std::left;
using std::setw;
using namespace std::chrono;

void initialize(float **arr, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            arr[i][j] = 1.*rand()/RAND_MAX;
        }
    }
}

void smooth_static(float **arr_x, float **arr_y, int n, float a, float b, float c, int chunksize) {
    #pragma omp parallel for schedule(static, chunksize)
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < n - 1; j++) {
            arr_y[i][j] = a*(arr_x[i-1][j-1] + arr_x[i-1][j+1] + arr_x[i+1][j-1] + arr_x[i+1][i+1]) +
            b*(arr_x[i-1][j] + arr_x[i+1][j] + arr_x[i][j-1] + arr_x[i][j+1]) + 
            c*arr_x[i][j];
        }
    }    
}

void smooth_dynamic(float **arr_x, float **arr_y, int n, float a, float b, float c, int chunksize) {
    #pragma omp parallel for schedule(dynamic, chunksize)
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
    int nts = 8;

    #ifdef _OPENMP
	    omp_set_num_threads(nts);
        #pragma omp parallel 
        {
            int t;
            #pragma omp for
            for (int i = 0; i < nts; i++) {
                t = omp_get_thread_num();
                printf("This is thread: %d\n",t);
            }
        }
        nts = omp_get_max_threads();
	#endif


    srand(time(NULL)); // randomly seed rand

    // ------ Initialize Variables ------ //
    int n, nbelow_x, nbelow_y;
    float a, b, c, t, frac_below_x, frac_below_y;
    time_point<system_clock> start, end;
    a = .05; b = .1; c = .4; t = .1;
    cin >> n;
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
    int chunks[4] = {100, 1000, 10000, 100000};
    double static_time[4], dynamic_time[4];

    start = system_clock::now();
    initialize(arr_x, n);
    end = system_clock::now(); 
    duration<double> elapsed_seconds_init = end - start;
    duration<double> elapsed_seconds_smooth;

    for (int i = 0; i < 4; i++) {
        start = system_clock::now();
        smooth_static(arr_x, arr_y, n, a, b, c, chunks[i]);
        end = system_clock::now(); 
        elapsed_seconds_smooth = end - start;
        static_time[i] = elapsed_seconds_smooth.count();

        start = system_clock::now();
        smooth_dynamic(arr_x, arr_y, n, a, b, c, chunks[i]);
        end = system_clock::now(); 
        elapsed_seconds_smooth = end - start;
        dynamic_time[i] = elapsed_seconds_smooth.count();
    }
    

    start = system_clock::now();
    count(arr_x, n, t, nbelow_x);
    end = system_clock::now(); 
    duration<double> elapsed_seconds_count_x = end - start;

    start = system_clock::now();
    count(arr_y, n, t, nbelow_y);
    end = system_clock::now(); 
    duration<double> elapsed_seconds_count_y = end - start;

    frac_below_x = (float) nbelow_x / (float)((n-2)*(n-2));
    frac_below_y = (float) nbelow_y / (float)((n-2)*(n-2));

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
    cout << "Number of Threads                     :: " << nts << endl;
    cout << "\n\n" << endl;
    cout << "Action               ::   time (s)" << endl;
    cout << "------" << endl;
    cout << "CPU Alloc-X          :: " << elapsed_seconds_x_alloc.count() << endl;
    cout << "CPU Alloc-Y          :: " << elapsed_seconds_y_alloc.count() << endl;
    cout << "CPU Init-X           :: " << elapsed_seconds_init.count() << endl;
    cout << "CPU Count-X          :: " << elapsed_seconds_count_x.count() << endl;
    cout << "CPU Count-Y          :: " << elapsed_seconds_count_y.count() << "\n" << endl;
    cout << left;
    cout << "Dynamic vs Static scheduling for smoothing times" << endl;
    for (int i = 0; i < 4; i++) {
        cout << "Chunksize: " << setw(8) << chunks[i] << "   " << setw(10) << 
        dynamic_time[i] << "    //    " << setw(10) << static_time[i] << endl;
    }
  

    



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
