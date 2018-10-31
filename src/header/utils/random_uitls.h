//
// Created by gzq on 18-7-13.
//

#ifndef PTHREAD_DEMO_RANDOM_UITLS_H
#define PTHREAD_DEMO_RANDOM_UITLS_H

void seed();

//左闭右开
int randint(int start, int end);

//返回[start,end)之间的小数
float random_float(float start, float end);

//左闭右开
void random_float_array(float * arr /* out */, int len, float start,
                         float end);

//返回[start,end)之间的小数
double random_double(double start, double end);

//左闭右开
void random_double_array(double* arr /* out */, int len, double start,
                         double end);

//左闭右开
void random_array(int* arr /* out */, int len, int start, int end);

void random_metrix(int** arr /* out */, int row, int col, int start, int end);

#endif  // PTHREAD_DEMO_RANDOM_UITLS_H
