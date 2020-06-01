#include<iostream>
#include<cstdlib>
#include<ctime>
#include<cmath>

#define eps 1e-6 //精度

using namespace std;

struct sqrt_st
{
    float result;
    long runtime;
};

/*
 * function:获取系统时间（纳秒级）
 * author:wanglu
 */
long getTimeNs()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME,&ts);

    return ts.tv_sec*1000000000+ts.tv_nsec;
}

/*
 * function:二分法实现sqrt()
 * author:wanglu
 */
float mysqrt_1(float n)
{
    float left = 0, right = n;
    float mid = 0;
    float last;// 保留上一次的结果

    if(n == 1) return 1;// 特判
    if(n < 0) return 0;// 特判

    do{

        if(mid  > n / mid)// 避免溢出
            right = mid;
        else
            left  = mid;

        last = mid;

        mid = left + (right-left) / 2;// 相较(left + right)/2 能避免溢出

    }while(abs(mid-last) > eps);// 相比使用right-left > eps判断，这样更加精确

    return mid;
}

/*
 * function:牛顿迭代法实现sqrt()
 * author:wanglu
 */
float mysqrt_2(float n)
{
    float init_value = n;// 牛顿法需要选择一个初始值，这里使等于n
    float x = init_value;// return value
    float last;// 保留上一次的结果

    do{

        last = x;
        x = (x + n/x)/2;

    }while(abs(x - last) > eps);// 比abs(x-n/x)>eps更精确

    return x;
}

/*
 * function:神奇的算法实现sqrt()
 * author:wanglu
 */
float mysqrt_3(float x)
{

    float xhalf = 0.5f*x;
    int i = *(int*)&x; 
    
    if(!x) return 0;
    
    i = 0x5f375a86- (i>>1); // beautiful number
    x = *(float*)&i; 
    x = x*(1.5f-xhalf*x*x); // 牛顿迭代法，提高精度
    x = x*(1.5f-xhalf*x*x); // 牛顿迭代法，提高精度
    x = x*(1.5f-xhalf*x*x); // 牛顿迭代法，提高精度

    return 1/x;
}

/*
 * function:测试不同的方法
 * author:wanglu
 */
struct sqrt_st test(float (*mysqrt)(float), float n)
{
    struct sqrt_st sqrt_struct;
    
    long start_time = getTimeNs();
    sqrt_struct.result = mysqrt(n);
    long end_time = getTimeNs();

    sqrt_struct.runtime = end_time - start_time;

    return sqrt_struct;
}

int main(int argc, char* argv[])
{
    
    if(argc < 2)
    {
        std::cout<<"Less parameters\n";
        exit(1);
    }
    float n = atof(argv[1]);

    // 测试sqrt库函数
    struct sqrt_st sqrt_sys = test(sqrt,n);
    std::cout<<"sys_sqrt's result:"<<sqrt_sys.result<<std::endl;
    std::cout<<"sys_sqrt's runtime:"<<sqrt_sys.runtime<<std::endl<<std::endl;

    // 测试mysqrt_1
    struct sqrt_st sqrt_st_1 = test(mysqrt_1,n);
    std::cout<<"mtsqrt_1's result:"<<sqrt_st_1.result<<std::endl;
    std::cout<<"mtsqrt_1's runtime:"<<sqrt_st_1.runtime<<std::endl<<std::endl;

    // 测试mysqrt_2
    struct sqrt_st sqrt_st_2 = test(mysqrt_2,n);
    std::cout<<"mtsqrt_2's result:"<<sqrt_st_2.result<<std::endl;
    std::cout<<"mtsqrt_2's runtime:"<<sqrt_st_2.runtime<<std::endl<<std::endl;

    // 测试mysqrt_3
    struct sqrt_st sqrt_st_3 = test(mysqrt_3,n);
    std::cout<<"mtsqrt_3's result:"<<sqrt_st_3.result<<std::endl;
    std::cout<<"mtsqrt_3's runtime:"<<sqrt_st_3.runtime<<std::endl<<std::endl;

    return 0;
}
