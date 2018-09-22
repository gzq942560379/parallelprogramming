//
// Created by gzq on 18-7-13.
//

#ifndef PTHREAD_DEMO_RANDOM_UITLS_H
#define PTHREAD_DEMO_RANDOM_UITLS_H

//左闭右开
int randint(int start, int end);

//返回[0,1)之间的小数
double random_double();

//左闭右开
void random_array(int* arr /* out */, int len, int start, int end);

void random_metrix(int** arr /* out */, int row, int col, int start, int end);

#endif  // PTHREAD_DEMO_RANDOM_UITLS_H
