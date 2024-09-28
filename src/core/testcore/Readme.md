# testcore

## 目标功能

### 一、加减乘除

#### python语法及其bytecode翻译

##### 1. 加法
python:
1 + 2

bytecode:
ADD tmp 1 2

##### 2. 减法
python:
2 - 1

bytecode:
SUB tmp 2 1

##### 3. 乘法
python:
2 * 3

bytecode:
MUL tmp 2 3

##### 4. 除法
python:
2 / 3

bytecode:
DIV tmp 2 3

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
CMP tmp 2 1
JZ tmp elif_addr
PRINT "2 > 1"
elif_addr:
JZ tmp else_addr
PRINT "2 < 1"
else_addr:
PRINT "2 = 1"

#### 2. while语句
python:
while 2 > 1:
    print("2 > 1")

bytecode:
CMP tmp 2 1
while_addr:
JZ tmp end_addr
PRINT "2 > 1"
JMP while_addr
end_addr:
NOP

#### 3. for语句
python:
for i in range(3):
    print("i =", i)

bytecode:
for_addr:
MOV i 0
MOV max 3
CMP tmp i max
JZ tmp end_addr
PRINT "i =", i
INC i
JMP for_addr
end_addr:
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

bytecode:
MOV a 2
MOV b "hello"

#### 2. 使用变量
python:
print(a)
print(b)

bytecode:
PRINT a
PRINT b

### 四、函数定义与调用
python:
def add(a, b):
    return a + b

ret = add(2, 3)
print(ret)

bytecode:
func_addr:
FUNC add
ADD tmp a b
RET tmp
main_addr:
CALL ret add 2 3
PRINT ret

### 五、list、tuple、dict

## bytecode

### 一、指令集

#### NOP
作用: 空操作
用法: NOP

#### MOV
作用: 将操作数2的值赋值给操作数1
用法: MOV 操作数1 操作数2

#### INC
作用: 将操作数1的值加1
用法: INC 操作数1

#### DEC
作用: 将操作数1的值减1
用法: DEC 操作数1

#### ADD
作用: 将操作数2加上操作数3，结果赋值给操作数1
用法: ADD 操作数1 操作数2 操作数3

#### SUB
作用: 将操作数2减去操作数3，结果赋值给操作数1
用法: SUB 操作数1 操作数2 操作数3

#### MUL
作用: 将操作数2乘以操作数3，结果赋值给操作数1
用法: MUL 操作数1 操作数2 操作数3

#### DIV
作用: 将操作数2除以操作数3，结果赋值给操作数1
用法: DIV 操作数1 操作数2 操作数3

#### CMP
作用: 比较操作数2和操作数3的大小，若操作数2大于操作数3，则将1赋值给操作数1；否则，将0赋值给操作数1
用法: CMP 操作数1 操作数2 操作数3

#### JZ
作用: 若操作数1为0，则跳转到操作数2指定的指令位置执行指令；否则，继续执行下一条指令
用法: JZ 操作数1 操作数2

#### JNZ
作用: 若操作数1不为0，则跳转到操作数2指定的指令位置执行指令；否则，继续执行下一条指令
用法: JNZ 操作数1 操作数2

#### JMP
作用: 跳转到操作数1指定的指令位置执行指令
用法: JMP 操作数1

#### PRINT
作用: 打印操作数
用法: PRINT 操作数1 操作数2 ... 操作数n

#### EXIT
作用: 退出程序
用法: EXIT

#### CALL
作用: 调用操作数2指定的函数，并将返回值赋值给操作数1，后续操作数为函数参数
用法: CALL 操作数1 操作数2 操作数3 ... 操作数n

#### RET
作用: 返回操作数1指定的值
用法: RET 操作数1

#### FUNC
作用: 定义函数，操作数1为函数名，后续操作数为函数参数
用法: FUNC 操作数1 操作数2 操作数3 ... 操作数n

### 二、执行方式
按照bytecode顺序执行