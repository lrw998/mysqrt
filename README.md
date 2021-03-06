### 实现sqrt函数功能

#### 1 二分法

```c++
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
```

- 耗时

  | n      | runtime(ns) |
  | ------ | ----------- |
  | 1      | 90          |
  | 2      | 404         |
  | 3333   | 570         |
  | 999999 | 710         |

  ​                                       

#### 2 牛顿迭代法

求出根号a的近似值：首先随便猜一个近似值x，然后不断令x等于x和a/x的平均数，迭代个六七次后x的值就已经相当精确了。
例如，我想求根号2等于多少。假如我猜测的结果为4，虽然错的离谱，但你可以看到使用牛顿迭代法后这个值很快就趋近于根号2了：
( 4 + 2/4 ) / 2 = 2.25
( 2.25 + 2/2.25 ) / 2 = 1.56944..
( 1.56944..+ 2/1.56944..) / 2 = 1.42189..
( 1.42189..+ 2/1.42189..) / 2 = 1.41423..
…

这种算法的原理很简单，我们仅仅是不断用(x,f(x))的切线来逼近方程x^2^-a=0的根。根号a实际上就是x^2^-a=0的一个正实根，这个函数的导数是2x。也就是说，函数上任一点(x,f(x))处的切线斜率是2x。那么，x-f(x)/(2x)就是一个比x更接近的近似值。代入 f(x)=x^2^-a得到x-(x^2^-a)/(2x)，也就是(x+a/x)/2。



```c++
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
```

- 耗时

| n      | runtime(ns) |
| ------ | ----------- |
| 1      | 128         |
| 2      | 170         |
| 3333   | 270         |
| 999999 | 300         |

​                                     

#### 3 神奇的方法

算法的原理其实不复杂,就是牛顿迭代法,用x-f(x)/f’(x)来不断的逼近f(x)=a的根。

没错，一般的求平方根都是这么循环迭代算的但是卡马克(quake3作者)真正牛B的地方是他选择了一个神秘的常数0x5f3759df 来计算那个猜测值，就是我们加注释的那一行，那一行算出的值非常接近1/sqrt(n)，这样我们只需要2次牛顿迭代就可以达到我们所需要的精度。好吧如果这个还不算NB,接着看:

普渡大学的数学家Chris Lomont看了以后觉得有趣，决定要研究一下卡马克弄出来的这个猜测值有什么奥秘。Lomont也是个牛人，在精心研究之后从理论上也推导出一个最佳猜测值，和卡马克的数字非常接近, 0x5f37642f。卡马克真牛，他是外星人吗？

传奇并没有在这里结束。Lomont计算出结果以后非常满意，于是拿自己计算出的起始值和卡马克的神秘数字做比赛，看看谁的数字能够更快更精确的求得平方根。结果是卡马克赢了… 谁也不知道卡马克是怎么找到这个数字的。

最后Lomont怒了，采用暴力方法一个数字一个数字试过来，终于找到一个比卡马克数字要好上那么一丁点的数字，虽然实际上这两个数字所产生的结果非常近似，这个暴力得出的数字是0x5f375a86。

```c++
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
```

- 耗时

| n      | runtime(ns) |
| ------ | ----------- |
| 1      | 100         |
| 2      | 100         |
| 3333   | 100         |
| 999999 | 100         |

可以看出这种方法的效率特别高，在实际应用中使用它效果会比其他的好。

​                               

### 本文的project      

Github仓库：https://github.com/lrw998/mysqrt

项目使用cmake管理，源文件为`mysqrt.cpp`

​                     

使用说明：

（1）cd进入`mysqrt`目录

（2）执行`cmake .`命令生成makefile

（3）执行`make`编译工程，在`mysqrt/bin`目录下生成可执行文件

（4） cd进入./bin目录，`./mysqrt`运行可执行文件

​                      

### 引用

https://www.2cto.com/kf/201206/137256.html

