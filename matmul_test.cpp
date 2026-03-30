#include <iostream>
#include "matmul.h"

using namespace std;

int main(int argc, char **argv)
{
    mat_a in_mat_a[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    mat_b in_mat_b[3][2] = {
        {1, 2},
        {4, 5},
        {7, 8}
    };

    mat_r hw_result[2][2], sw_result[2][2];

#pragma HLS INTERFACE bram port=in_mat_a depth=6
#pragma HLS INTERFACE bram port=in_mat_b depth=6
#pragma HLS INTERFACE bram port=hw_result depth=4

    int err_cnt = 0;

    for(int i = 0; i < a_rows; i++) {
        for(int j = 0; j < b_cols; j++) {
            sw_result[i][j] = 0;
            for(int k = 0; k < b_rows; k++) {
                sw_result[i][j] += in_mat_a[i][k] * in_mat_b[k][j];
            }
        }
    }

#ifdef HW_COSIM
    matmul(in_mat_a, in_mat_b, hw_result);
#endif

    cout << "{" << endl;

    for(int i = 0; i < a_rows; i++) {
        cout << "{";
        for(int j = 0; j < b_cols; j++) {

#ifdef HW_COSIM
            cout << hw_result[i][j];
            if(hw_result[i][j] != sw_result[i][j]) {
                err_cnt++;
                cout << "*";
            }
#else
            cout << sw_result[i][j];
#endif

            if(j == b_cols - 1)
                cout << "}" << endl;
            else
                cout << ", ";
        }
    }

    cout << "}" << endl;

#ifdef HW_COSIM
    if(err_cnt)
        cout << "ERROR: " << err_cnt << " MISMATCHES DETECTED" << endl;
    else
        cout << "TESTS PASSED" << endl;
#endif

    return err_cnt;
}