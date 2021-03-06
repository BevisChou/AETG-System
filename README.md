# 基于AETG算法的组合测试用例生成器

## 平台与依赖

- 操作系统：类Unix系统
- C++编译器：支持C++11的g++编译器
- Java JDK：JDK11及以上
- Shell：Bash

## 设计

我们将系统拆分成三个模块，分别是输入模块、抽象用例生成模块与输出模块。

### 输入模块

该模块接受特定格式（将于“使用”部分说明）的文件，统计出测试用例包含的参数种类个数与各参数包含的可选值的数量，输出到抽象用例生成模块。

### 抽象用例生成模块

该模块接收输入模块的输入，生成由数字组成的测试用例组合，输出到输出模块。

该模块使用AETG算法生成测试用例组合，AETG算法的具体内容不在此赘述，请参考相关论文。

### 输出模块

该模块接收抽象用例生成模块的输入，将由数字组成的测试用例组根据输入到输入模块的文件内容生成具体测试用例组合。

### 模块融合

由于Java对Unicode的天然支持，我们采用Java编写输入模块与输出模块；由于C++的性能较好且标准库易于使用，我们采用C++编写抽象用例生成模块。

我们对三个模块进行分别编译，通过bash脚本完成模块间输入输出的处理。

## 使用

将测试参数及各参数的可能取值按以下方式组织，放置于根文件夹下`data`文件夹中。注意各字符串之间使用退格键（`'\t'`）分隔。

```
参数1	取值1-1	取值1-2	...
参数2	取值2-1	取值2-2	...
```

随后，在根文件夹下输入以下命令。其中`文件名1`与`文件名2`均不包含任何路径信息，`覆盖元组大小`为需要涵盖的元组大小，如生成Pairwise的用例组合时，值为2。

```
./generator.sh 文件名1 文件名2 覆盖元组大小
```

输出文件将存于根文件夹下`data`文件夹。

输出文件的第一行的格式如下。后续行为具体的测试用例。注意各字符串之间使用退格键（`'\t'`）分隔。

```
样例ID	参数1	参数2	...	新增的元组数
```

## 注意事项

- 可能需要根据实际情况对`src/Makefile`中的`CPP11`与`OPT`进行修改。

## 例子
在命名行输入以下命令。

```
./generator.sh sample_in_cn.txt sample_out_cn_2.txt 2
```

可以在`data`文件夹下看到`sample_out_cn_2.txt`文件，其中内容如下。

```
样例ID	参数1	参数2	参数3	新增的元组数
1	取值1-1	取值2-1	取值3-1	3
2	取值1-2	取值2-1	取值3-2	3
3	取值1-1	取值2-1	取值3-3	2
4	取值1-2	取值2-1	取值3-1	1
5	取值1-1	取值2-1	取值3-2	1
6	取值1-2	取值2-1	取值3-3	1
```