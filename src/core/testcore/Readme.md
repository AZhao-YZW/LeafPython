# testcore

## 目标功能

### 一、加减乘除

#### python语法及其bytecode翻译

使用<xxx>代表xxx的`obj_id`，与汇编语言相比较，类似于某个变量的地址

##### 1. 加法
###### 1.1. 两数相加
python:
1 + 2

bytecode:
NEW "anon_1" int
NEW "anon_2" int
NEW "anon_3" int
MOV <anon_1> 1
MOV <anon_2> 2
ADD <anon_3> <anon_1> <anon_2>
(PRINT <anon_3>)
DEL <anon_1>
DEL <anon_2>
DEL <anon_3>

**注**：
1. 在repl模式下，会将结果输出到控制台，故bytecode中会多一个`PRINT`指令，其他案例原理相同，后不赘述
2. 匿名变量的使用后续可以做优化，可以预分配提高性能

###### 1.2. 多数相加
python:
1 + 2 + 3 + ...

bytecode:
NEW "anon_1" int
NEW "anon_2" int
NEW "anon_3" int
MOV <anon_1> 1
MOV <anon_2> 2
ADD <anon_3> <anon_1> <anon_2>
MOV <anon_1> <anon_3>
MOV <anon_2> 3
ADD <anon_3> <anon_1> <anon_2>
...
(PRINT <anon_3>)
DEL <anon_1>
DEL <anon_2>
DEL <anon_3>

##### 2. 减法
###### 1.1. 两数相减
python:
2 - 1

bytecode:
NEW "anon_1" int
NEW "anon_2" int
NEW "anon_3" int
MOV <anon_1> 2
MOV <anon_2> 1
SUB <anon_3> <anon_1> <anon_2>
(PRINT <anon_3>)
DEL <anon_1>
DEL <anon_2>
DEL <anon_3>

###### 1.2. 多数相减
python:
3 - 1 - 2 - ...

bytecode:
NEW "anon_1" int
NEW "anon_2" int
NEW "anon_3" int
MOV <anon_1> 3
MOV <anon_2> 1
SUB <anon_3> <anon_1> <anon_2>
MOV <anon_1> <anon_3>
MOV <anon_2> 2
SUB <anon_3> <anon_1> <anon_2>
...
(PRINT <anon_3>)
DEL <anon_1>
DEL <anon_2>
DEL <anon_3>

###### 1.3. 混合加减法
python:
1 + 2 - 3 + 4 - ...

bytecode:
NEW "anon_1" int
NEW "anon_2" int
NEW "anon_3" int
MOV <anon_1> 1
MOV <anon_2> 2
ADD <anon_3> <anon_1> <anon_2>
MOV <anon_1> <anon_3>
MOV <anon_2> 3
SUB <anon_3> <anon_1> <anon_2>
MOV <anon_1> <anon_3>
MOV <anon_2> 4
ADD <anon_3> <anon_1> <anon_2>
...
(PRINT <anon_3>)
DEL <anon_1>
DEL <anon_2>
DEL <anon_3>

##### 3. 乘法
###### 3.1. 两数相乘
python:
2 * 3

bytecode:
NEW "anon_1" int
NEW "anon_2" int
NEW "anon_3" int
MOV <anon_1> 2
MOV <anon_2> 3
MUL <anon_3> <anon_1> <anon_2>
(PRINT <anon_3>)
DEL <anon_1>
DEL <anon_2>
DEL <anon_3>

###### 3.2. 多数相乘
python:
2 * 3 * 4 * ...

bytecode:
NEW "anon_1" int
NEW "anon_2" int
NEW "anon_3" int
MOV <anon_1> 2
MOV <anon_2> 3
MUL <anon_3> <anon_1> <anon_2>
MOV <anon_1> <anon_3>
MOV <anon_2> 4
MUL <anon_3> <anon_1> <anon_2>
...
(PRINT <anon_3>)
DEL <anon_1>
DEL <anon_2>
DEL <anon_3>

###### 3.3. 混合乘除法
python:
2 * 3 / 4 * 5 / ...

bytecode:
NEW "anon_1" float
NEW "anon_2" float
NEW "anon_3" float
MOV <anon_1> 2
MOV <anon_2> 3
MUL <anon_3> <anon_1> <anon_2>
MOV <anon_1> <anon_3>
MOV <anon_2> 4
DIV <anon_3> <anon_1> <anon_2>
MOV <anon_1> <anon_3>
MOV <anon_2> 5
MUL <anon_3> <anon_1> <anon_2>
...
(PRINT <anon_3>)
DEL <anon_1>
DEL <anon_2>
DEL <anon_3>

##### 4. 除法
###### 4.1. 两数相除
python:
6 / 2

bytecode:
NEW "anon_1" float
NEW "anon_2" float
NEW "anon_3" float
MOV <anon_1> 6
MOV <anon_2> 2
DIV <anon_3> <anon_1> <anon_2>
(PRINT <anon_3>)
DEL <anon_1>
DEL <anon_2>
DEL <anon_3>

###### 4.2. 多数相除
python:
12 / 2 / 3 / ...

bytecode:
NEW "anon_1" float
NEW "anon_2" float
NEW "anon_3" float
MOV <anon_1> 12
MOV <anon_2> 2
DIV <anon_3> <anon_1> <anon_2>
MOV <anon_1> <anon_3>
MOV <anon_2> 3
DIV <anon_3> <anon_1> <anon_2>
...
(PRINT <anon_3>)
DEL <anon_1>
DEL <anon_2>
DEL <anon_3>

###### 4.3. 混合乘除加减法
python:
2 + 3 * 4 - 6 / 2 + ...

bytecode:
对于任意混合运算表达式，按照以下原则处理：
1. 先计算乘除运算（从左到右）
2. 再计算加减运算（从左到右）
3. 每步运算结果保存在 <anon_3> 中，作为下一步运算的操作数

通用bytecode模式:
NEW "anon_1" float
NEW "anon_2" float
NEW "anon_3" float
NEW "tmp" float
; 乘除运算部分
MOV <anon_1> value1
MOV <anon_2> value2
MUL/DIV <anon_3> <anon_1> <anon_2>
MOV <anon_1> <anon_3>
MOV <anon_2> value3
MUL/DIV <tmp> <anon_1> <anon_2>  ; 需要临时变量保存中间结果
; 加减运算部分
MOV <anon_1> <anon_3>  ; 第一个操作数
MOV <anon_2> <tmp>     ; 第二个操作数
ADD/SUB <anon_3> <anon_1> <anon_2>
...
(PRINT <anon_3>)
DEL <anon_1>
DEL <anon_2>
DEL <anon_3>
DEL <tmp>

### 二、控制语句

#### 1. if语句
python:
if 2 > 1:
    print("2 > 1")
elif 2 < 1:
    print("2 < 1")
else:
    print("2 = 1")

bytecode:
NEW "tmp" int
CMP <tmp> 2 1
JZ <tmp> elif_addr
PRINT "2 > 1"
JMP end_addr
elif_addr:
CMP <tmp> 2 1
JZ <tmp> else_addr
PRINT "2 < 1"
JMP end_addr
else_addr:
PRINT "2 = 1"
end_addr:
DEL <tmp>

#### 2. while语句
python:
while 2 > 1:
    print("2 > 1")

bytecode:
NEW "tmp" int
while_addr:
CMP <tmp> 2 1
JZ <tmp> end_addr
PRINT "2 > 1"
JMP while_addr
end_addr:
DEL <tmp>
NOP

#### 3. for语句
python:
for i in range(3):
    print("i =", i)

bytecode:
NEW "i" int
NEW "max" int
NEW "tmp" int
MOV <i> 0
MOV <max> 3
for_addr:
CMP <tmp> <i> <max>
JZ <tmp> end_addr
PRINT "i =" <i>
INC <i>
JMP for_addr
end_addr:
DEL <i>
DEL <max>
DEL <tmp>
NOP

#### 4. break语句
python:
while True:
    print("break")
    break

bytecode:
while_addr:
PRINT "break"
JMP end_addr
end_addr:
NOP

#### 5. continue语句
python:
while True:
    print("continue")
    continue

bytecode:
while_addr:
PRINT "continue"
JMP while_addr

### 三、变量定义与使用

#### 1. 定义变量
python:
a = 2
b = "hello"
c = 3.14

bytecode:
NEW "a" int
MOV <a> 2
NEW "b" String
MOV <b> "hello"
NEW "c" float
MOV <c> 3.14

#### 2. 使用变量
python:
print(a)
print(b)

bytecode:
PRINT <a>
PRINT <b> <c>

#### 3. 变量运算
python:
d = a + c

bytecode:
NEW "d" float
NEW "anon_1" float
NEW "anon_2" float
MOV <anon_1> <a>
MOV <anon_2> <c>
ADD <d> <anon_1> <anon_2>
DEL <anon_1>
DEL <anon_2>

### 四、函数定义与调用
python:
def add(a, b):
    return a + b

ret = add(2, 3)
print(ret)

bytecode:
add_func:
FUNC "add" "a" "b"
NEW "tmp" int
ADD <tmp> <a> <b>
RET <tmp>
DEL <tmp>

NEW "ret" int
CALL <ret> "add" 2 3
PRINT <ret>
DEL <ret>

### 五、list、tuple、dict
#### list定义与使用
python:
my_list = [1, 2, 3]
print(my_list[0])

bytecode:
NEW "my_list" List
NEW "anon_1" int
MOV <anon_1> 1
NEW "anon_2" int
MOV <anon_2> 2
NEW "anon_3" int
MOV <anon_3> 3
; 假设有一个LIST_ADD指令用于向列表添加元素
LIST_ADD <my_list> <anon_1>
LIST_ADD <my_list> <anon_2>
LIST_ADD <my_list> <anon_3>
DEL <anon_1>
DEL <anon_2>
DEL <anon_3>
; 访问列表元素
NEW "tmp" int
LIST_GET <tmp> <my_list> 0
PRINT <tmp>
DEL <tmp>
DEL <my_list>

#### tuple定义与使用
python:
my_tuple = (1, "hello")
print(my_tuple[1])

bytecode:
NEW "my_tuple" Tuple
NEW "anon_1" int
MOV <anon_1> 1
NEW "anon_2" String
MOV <anon_2> "hello"
TUPLE_SET <my_tuple> 0 <anon_1>
TUPLE_SET <my_tuple> 1 <anon_2>
DEL <anon_1>
DEL <anon_2>
; 访问tuple元素
NEW "tmp" String
TUPLE_GET <tmp> <my_tuple> 1
PRINT <tmp>
DEL <tmp>
DEL <my_tuple>

#### dict定义与使用
python:
my_dict = {"key1": "value1", "key2": 2}
print(my_dict["key1"])

bytecode:
NEW "my_dict" Dict
NEW "key1" String
MOV <key1> "key1"
NEW "value1" String
MOV <value1> "value1"
NEW "key2" String
MOV <key2> "key2"
NEW "value2" int
MOV <value2> 2
DICT_SET <my_dict> <key1> <value1>
DICT_SET <my_dict> <key2> <value2>
DEL <key1>
DEL <value1>
DEL <key2>
DEL <value2>
; 访问dict元素
NEW "tmp" String
NEW "search_key" String
MOV <search_key> "key1"
DICT_GET <tmp> <my_dict> <search_key>
PRINT <tmp>
DEL <tmp>
DEL <search_key>
DEL <my_dict>

## bytecode

### 一、指令集

#### NOP
作用: 空操作
用法: NOP

#### NEW
作用：创建对象
用法: NEW <obj_name> <type>  [ret: obj_id]

#### DEL
作用：删除对象
用法: DEL <obj_id>

#### MOV
作用: 值赋给对象 or 对象的值赋给对象
用法: 
1. MOV <obj_id> <value>
2. MOV <obj_id1> <obj_id2>

#### INC
作用: 对象的值加1
用法: INC <obj_id>

#### DEC
作用: 对象的值减1
用法: DEC <obj_id>

#### ADD
作用: 值相加并赋值给对象
用法:
1. ADD <obj_id1> <obj_id2> <obj_id3>
2. ADD <obj_id1> <obj_id2> <value>
3. ADD <obj_id1> <value> <value>

#### SUB
作用: 值相减并赋值给对象
用法:
1. SUB <obj_id1> <obj_id2> <obj_id3>
2. SUB <obj_id1> <obj_id2> <value>
3. SUB <obj_id1> <value> <value>

#### MUL
作用: 值相乘并赋值给对象
用法:
1. MUL <obj_id1> <obj_id2> <obj_id3>
2. MUL <obj_id1> <obj_id2> <value>
3. MUL <obj_id1> <value> <value>

#### DIV
作用: 值相除并赋值给对象
用法:
1. DIV <obj_id1> <obj_id2> <obj_id3>
2. DIV <obj_id1> <obj_id2> <value>
3. DIV <obj_id1> <value> <value>

#### CMP
作用: 比较操作数2和操作数3的大小，将比较结果（大于返回1，等于返回0，小于返回-1）存储到操作数1中
用法:
1. CMP <obj_id1> <obj_id2> <obj_id3>
2. CMP <obj_id1> <obj_id2> <value>
3. CMP <obj_id1> <value> <value>

#### JZ
作用: 比较操作数1与0，若相等则跳转到操作数2指定的指令位置执行指令；否则继续执行下一条指令
用法: JZ <obj_id1> <obj_id2>

#### JNZ
作用: 比较操作数1与0，若不相等则跳转到操作数2指定的指令位置执行指令；否则继续执行下一条指令
用法: JNZ <obj_id1> <obj_id2>

#### JMP
作用: 无条件跳转到操作数1指定的指令位置执行指令
用法: JMP <obj_id1>

#### PRINT
作用: 打印指定的操作数
用法: PRINT <obj_id1> <obj_id2> ... <obj_idn>

#### EXIT
作用: 退出程序执行
用法: EXIT

#### CALL
作用: 调用操作数2指定的函数，将返回值赋值给操作数1，后续操作数为函数参数
用法: CALL <obj_id1> <obj_id2> <obj_id3> ... <obj_idn>

#### RET
作用: 返回操作数1指定的值作为函数执行结果
用法: RET <obj_id1>

#### FUNC
作用: 定义函数，操作数1为函数名，后续操作数为函数参数
用法: FUNC <obj_id1> <obj_id2> <obj_id3> ... <obj_idn>

### 二、执行方式
按照bytecode顺序执行