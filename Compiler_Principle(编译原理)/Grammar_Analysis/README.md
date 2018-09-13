# C++ Grammar Analysis C++语法分析器

##  Mini  C - BNF 范式     
[reference] (http://blog.csdn.net/pinbodexiaozhu/article/details/25394417)

###（一）文法表达式 

1. program  →  declaration-list  
2. declaration-list → declaration-list  declaration | declaration  
3. declaration → var-declaration | fun-declaration  
4. var-declaration; → type-specifier ID  | type-specifier ID [ NUM ]  ; 
5. type-specifier → int | void | float  
6. fun-declaration → type-specifier ID(params)  compound-stmt 
7. params → params-list | void  
8. param-list → param-list,param | param  
9. param → type-specifier ID
10. compound-stmt → { local-declarations statement-list }  
11. local-declarations → local-declarations var-declaration | empty  
12. statement-list → statement-liststatement | empty  
13. statement → expression-stmt | compound-stmt | selection-stmt  | iteration-stmt | return-stmt  
14. expression-stmt → expression;|;  
15. selection-stmt → if (expression) statement  | if (expression) statement else statement  
16. iteration-stmt → while (expression) statement  
17. return-stmt → return; | return expression;  
18. expression → var = expression | simple-expression  
19. var → ID | ID[expression]  
20. simple-expression → additive-expression relop additive-expression |  additive-expression  
21. relop → <= | < | > | >= | = = | != | && | ||  
22. additive-expression → additive-expression addop term | term  23. addop → + | -  
24. term → term mulopfactor | factor  25. mulop → * | /  
26. factor → (expression) | var | call | NUM  27. call → ID (args)  
28. args → arg-list |empty  
29. arg-list → arg-list,expression | expression 

###（二）文法的解释说明

1.program → declaration-list  
2.declaration-list → declaration-list declaration | declaration  
3.declaration → var-declaration | fun-declaration  
程序由声明的列表(或序列)组成，声明可以是函数或变量声明，顺序是任意的。至少必须有 一个声明。接下来是语义限制(这些在C中不会出现)。所有的变量和函数在使用前必须声明 (这避免了向后backpatching引用)。程序中最后的声明必须是一个函数声明，名字为main。  
4.var-declaration → type-specifier ID ; | type-specifier ID[NUM];  
5.type-specifier → int | void | float | char
变量声明或者声明了简单的整数或浮点类型变量，或者是基类型为整数或浮点的数组变量， 索引范围从0到NUM-1。注意，在C1中仅有的基本类型是整型和空类型。在一个变量声明中，只能使用类型指示符int。void用于函数声明(参见下面)。也要注意，每个声明只能声明一个变量。  
6.fun-declaration → type-specifier ID(params) compound-stmt  
7.params → param-list | void  
8.param-list → param-list , param | param  
9.param → type-specifier ID
函数声明由返回类型指示符(type-specifier)、标识符(ID)以及在圆括号内的用逗号分开的参数列表(params)组成，后面跟着一个复合语句(compound-stmt)，是函数的代码。
如果函数的返回类型是void，那么函数不返回任何值(即是一个过程)。函数的参数可以是void(即没有参数)，或者一列描述函数的参数。参数后面 跟着方括号是数组参数，其大小是可变的。简单的整型参数由值传递。数组参数由引用来传递(也就是指针)，在调用时必须通过数组变量来匹配。注意，类型“函数”没有参数。一个 函数参数的作用域等于函数声明的复合语句，函数的每次请求都有一个独立的参数集。函数 可以是递归的(对于使用声明允许的范围)。  
10.compound-stmt → { local-declarations statement-list }   
复合语句由用花括号围起来的一组声明(local-declarations)和语句(statement-list)组成。复合语句通过用给定的顺序执行语句序列来执行。局部声明的作用域等于复合语句的语句列表，并代替任何全局声明.
11.local-declarations → local-declarations var-declaration | empty  
12.statement-list → statement-list statement | empty  
注意声明和语句列表都可以是空的(非终结符empty表示空字符串，有时写作ε。)  
13.statement → expression-stmt  | compound-stmt  | selection-stmt  | iteration-stmt  | return-stmt  
14.expression-stmt → expression; | ;
表达式语句有一个可选的且后面跟着分号的表达式。这样的表达式通常求出它们一方的结 果。因此，这个语句用于赋值和函数调用。  
15.selection-stmt → if (expression) statement  | if (expression) statement else statement  
if语句有通常的语义：表达式进行计算；非0值引起第一条语句的执行；0值引起第二条语句的执行，如果它存在的话。这个规则导致了典型的悬挂else二义性，可以用一种标准的方法解决：else部分通常作为当前if的一个子结构立即分析(“最近嵌套”非二义性规则)。  
16.iteration-stmt → while (expression) statement  
while语句是C－中唯一的重复语句。它重复执行表达式，并且如果表达式的求值为非0，则 执行语句，当表达式的值为0时结束。  
17.return-stmt → return; | return expression ; 
返回语句可以返回一个值也可无值返回。函数没有说明为void就必须返回一个值。函数声明为void就没有返回值。return引起控制返回调用者(如果它在main中，则程序结束)。  
18.expression → var=expression | simple-expression  
19.var→ ID | ID[expression]  
表达式是一个变量引用，后面跟着赋值符号(等号)和一个表达式，或者就是一个简单的表达 式。赋值有通常的存储语义：找到由var表示的变量的地址，然后由赋值符右边的子表达式 进行求值，子表达式的值存储到给定的地址。这个值也作为整个表达式的值返回。
var是简单的(整型)变量或下标数组变量。负的下标将引起程序停止(与C不同)。然而，不进行下标 越界检查。  
var表示C1比C的进一步限制。在C中赋值的目标必须是左值(l-value)，左值是可以由许多操作获得的地址。在C1中唯一的左值是由var语法给定的，因此这个种类按照句法进行检查， 代替像C中那样的类型检查。故在C1中指针运算是禁止的。  
20.simple-expression → additive-expression relop additive-expression  |
additive-expression  
21.relop → <= | < | > | >= | == | != | && | ||  
简单表达式由无结合的关系操作符组成(即无括号的表达式仅有一个关系操作符)。简单表达 式在它不包含关系操作符时，其值是加法表达式的值，或者如果关系算式求值为ture，其值 为1，求值为false时值为0。  
22.additive-expression → additive-expression addop term | term 
23.addop → + | -  
24.term → term mulop factor | factor  
25.mulop → * | /   
加法表达式和项表示了算术操作符的结合性和优先级。/符号表示整数除；即任何余数都被 截去。  
26.factor → (expression) | var | call | NUM | DECIMAL 
因子是围在括号内的表达式；或一个变量，求出其变量的值；或者一个函数调用，求出函数 的返回值；或者一个NUM，其值由扫描器计算。数组变量必须是下标变量，除非表达式由单个ID组成，并且以数组为参数在函数调用中使用(如下所示)。 
27.call → ID(args)  
28.args → arg-list | empty  
29.arg-list → arg-list , expression | expression  
函数调用的组成是一个ID(函数名)，后面是用括号围起来的参数。参数或者为空，或者由逗 号分割的表达式列表组成，表示在一次调用期间分配的参数的值。函数在调用之前必须声明， 声明中参数的数目必须等于调用中参数的数目。函数声明中的数组参数必须和一个表达式匹 配，这个表达式由一个标识符组成表示一个数组变量。



本文法中，有以下终结符：
标识符 ID
整数数字 NUM
小数数字 DECIMAL
保留字 If else while return 
操作符：+ - * / < > <= >= == != || && 
分隔符 , ; () [] {}


## 构建LL(1)文法

考虑到LR（1）文法项目集太多，不适合手工操作，所以选择了LL（1）文法.
为便于书写，用每个单词的首字母代表整个含义
### 第一步，提取左公因子，消除左递归
#### （一）过程如下：
1. PR → DL
2. DL → DL  D | D 
=>消除左递归   DL →D DL | epsilon
3. D → VD | FD
4. VD → TS ID | TS ID [NUM]
=>提取左公因子			VD → TS ID VD‘ ; 
VD‘ → epsilon 
VD‘ → [NUM]
6. FD → TS ID (Ps) CS 
=>3,4,6一起，合并，并提取左公因子  D → TS ID D‘ 
D‘ → epsilon | [NUM] | (P) CS
5. TS → int | void | float  
7. Ps → PL | epsilon 
8. PL → PL , P | P  
=>消除左递归   PL→P PL‘   
P-L‘ →, P  P-L‘ | epsilon
9. P → T-S ID  
10. C-S → { local-D  S-L }  
11. local-D → local-D  V-D | empty 
=>消除左递归  local-D → V-D  local-D | empty
12. S-L → S-L  S | empty    
=>消除左递归 S-L → S  S-L  | empty
13. S → E-S | C-S | selection-S  | iteration-S | return-S  
14. E-S → E;|;  
15. selection-S → if (E) S  | if (E) S else S  
=>提取左公因子  selection-S → if (E) S selection-S’
selection-S’ → else S | epsilon
16. iteration-S → while (E) S  
17. return-S → return; | return E;  
=>提取左公因子 return-S → return return-S’
return-S’ → ; | E;
18. E → V = E | simple-E   
19. V → ID | ID[E]  
=>提取左公因子  V → ID V’
V’-> [E] | epsilon
20. simple-E → additive-E relop additive-E | additive-E  
=>提取左公因子  simple-E → additive-E simple-E’
simple-E’-> relop additive-E | epsilon
21. relop → <= | < | > | >= | = = | != | && | ||  
22. additive-E → additive-E aop term | term  
=>消除左递归   additive-E-> term additive-E’
additive-E’-> aop term additive-E’ | epsilon
23. aop → + | -  
24. term → term mop F | F 
=>消除左递归   term->F term’
term’ -> mop F term’ | epsilon
25. mop → * | /  
26. F → (E) | V | call | NUM | DECIMAL
27. call → ID (As)  
=>19 26 27 三个式子合并，提取左公因子
F →ID F’ | (E) | NUM | DECIMAL
F’ →[E] | epsilon| (As)
28. As → A-L | empty  
29. A-L → A-L, E | E
=>消除左递归    A-L->E A-L’
A-L’-> , E A-L’ | epsilon

####（二）消除左递归和左公因子后的文法 （这里为节省篇幅，含有 | 的产生式不分开写）
1.	PR → D-L
2.	D-L →D D-L | epsilon
3.	D → T-S ID D‘ 
4.	D‘ → epsilon | [NUM] | (Ps) C-S
5.	T-S → int | void | float  
6.	Ps → P-L | epsilon 
7.	P-L→P P-L‘   
8.	P-L‘ →, P  P-L‘ | epsilon
9.	P → T-S ID  
10.	C-S → { local-D  S-L }  
11.	local-D → V-D  local-D | empty
12.	S-L → S  S-L  | empty
13.	S → E-S | C-S | selection-S  | iteration-S | return-S  
14.	E-S → E;|;  
15.	selection-S → if (E) S selection-S’
16.	selection-S’ → else S | epsilon
17.	iteration-S → while (E) S  
18.	return-S → return return-S’
19.	return-S’ → ; | E;
20.	E → V = E | simple-E   
21.	V → ID V’
22.	V’-> [E] | epsilon
23.	simple-E → additive-E simple-E’
24.	simple-E’-> relop  additive-E | epsilon
25.	relop → <= | < | > | >= | = = | != | && | ||  
26.	additive-E-> term additive-E’
27.	additive-E’-> aop term additive-E’ | epsilon
28.	aop → + | -  
29.	term->F term’
30.	term’ -> mop F term’ | epsilon
31.	mop → * | /  
32.	F →ID F’ | (E) | NUM | DECIMAL
33.	F’ →[E] | epsilon| (As)
34.	As → A-L | empty  
35.	A-L->E A-L’
36.	A-L’-> , E A-L’ | epsilon


### 第二步，求出first 集  follow 集

#### FIRST集
First(PR)={   int   void   float   epsilon } 
First(D-L)= {  int   void   float    epsilon  }
First(D)= {  int   void   float }
FIRST(D’)= {  [   (   epsilon  }
First(V-D)= {  int   void   float }
First(V-D’)={  [   epsilon  } 
First(T-S)= {   int   void   float }
First(P)= {  int   void   float }
FIRST(Ps)= {  int  void  float  epsilon }
FIRST(P-L)= {   int   void   float }
FIRST(P-L’)= {   ,  epsilon }
FIRST(C-S)={   {   }
FIRST(local-D)= {  int  void  float  epsilon  }
FIRST(S-L)={  ;  {   if   while    return   (  ID NUM  DECIMAL epsilon }
FIRST(S)={   ;  {   if   while    return   (  ID NUM  DECIMAL   }
FIRST(E-S)={    ;    (  ID NUM  DECIMAL  }
FIRST(selection-S)={  if  }
FIRST(selection-S’)={  else   epsilon }
FIRST(Iteration-S)={  while }
FIRST(return-S)={  return }
FIRST(return-S’)={ ;  (  ID NUM  DECIMAL }
FIRST(E)= {  (  ID NUM  DECIMAL }
FIRST(V)={  ID }
FIRST(V’)={   [   epsilon }
FIRST(simple-E)= {   (  ID NUM  DECIMAL }
FIRST(simple-E’)={  <   >   <=   >=   ==   !=    &&    ||   epsilon }
FIRST(additive-E)= {  (  ID NUM  DECIMAL }
FIRST(additive-E’)={  +   -    epsilon }
FIRST(term)= {  (  ID NUM  DECIMAL }
FIRST(term’)={ *    /   epsilon }
FIRST(F)=FIRST(term)=FIRST(additive-E)=FIRST(simple-E)= {  (  ID NUM  DECIMAL }
FIRST(F’)={  [   (   EPSILON  }
FIRST(As)={  (  ID NUM  DECIMAL  epsilon }
FIRST(A-L)= {  (  ID NUM  DECIMAL }
FIRST(A-L’)={  ,  epsilon }
FIRST(relop)= {  <   >   <=  >=    ==   !=    &&    ||    }
FIRST(aop)={  +   -   }a
FIRST(mop)={   *    /    }


#### FOLLOW集合
FOLLOW(PR)={  #    }
FOLLOW(D-L)= {  #  }
FOLLOW(D)= {  int   void   float    #  } 
FOLLOW(D’)= {  int   void   float    #  }
FOLLOW(V-D)= { int  void  float ; {  if  while return ( ID NUM  DECIMAL  }
FOLLOW(V-D’)={ int   void  float ; {  if  while return ( ID NUM DECIMAL }
FOLLOW(T-S)= {  ID   }
FOLLOW(Ps)={   )     }
FOLLOW(P)= {   ,   )  }
FOLLOW(P-L)= {    )   }
FOLLOW(P-L’)= {   )  }
FOLLOW(C-S)={ int void float # else }  ;  {  if while  return  (  ID NUM  DECIMAL  }
FOLLOW(local-D)= {  ;  {  if  while  return  (  ID NUM  DECIMAL  }
FOLLOW(S-L)= {    }    }
FOLLOW(S)={ else }  ;  {   if   while   return  (  ID NUM  DECIMAL  }
FOLLOW(E-S)={ else  }  ;  {  if  while  return  (  ID NUM  DECIMAL  }
FOLLOW(selection-S)= { else } ; { if  while  return  (  ID NUM  DECIMAL  }
FOLLOW(selection-S’)= { else } ; {  if while  return  (  ID NUM  DECIMAL  }
FOLLOW(Iteration-S)= { else  } ; { if  while return  (  ID NUM  DECIMAL  }
FOLLOW(return-S)= { else } ;  {  if  while  return  (  ID NUM  DECIMAL  }
FOLLOW(return-S’)= { else }  ; {  if  while  return  (  ID NUM  DECIMAL  }
FOLLOW(E)={  )  ]  }  , else   ;  }
FOLLOW(V)={ = }
FOLLOW(V’)={ = }
FOLLOW(simple-E)={  ;   )  ]  ,   }
FOLLOW(simple-E’)={  ;   )  ]  ,   }
FOLLOW(additive-E)={ ;   )  ]  ,   <   >   <=   >=   ==   !=    &&    ||  }
FOLLOW(additive-E’)={ ;   )  ]  ,   <   >   <=   >=   ==   !=    &&    ||  }
FOLLOW(term)={ + - ;   )  ]  ,   <   >   <=   >=   ==   !=    &&    ||  }
FOLLOW(term’)={ + - ;   )  ]  ,   <   >   <=   >=   ==   !=    &&    ||   }
FOLLOW(F)={ * /  + - ;   )  ]  ,   <   >   <=   >=   ==   !=    &&    ||   }
FOLLOW(F’)={ * /  + - ;   )  ]  ,    <   >   <=   >=     ==   !=    &&    ||  } 
FOLLOW(As)={   )   }
FOLLOW(A-L)={  )  }
FOLLOW(A-L’)={  )   }
FOLLOW(relop)= {  (  ID NUM  DECIMAL  }
FOLLOW(aop)={  (  ID NUM  DECIMAL  }
FOLLOW(mop)={ (  ID NUM  DECIMAL }



## 第三步，构建预测分析表
 
完整版见附录1

***

## 测试结果
（一）测试代码如下：
>  float a;
> int main()
> {
>     int b;
>     int c;
> 
>     b = 1;
>     c = 2;
>     a = 100.5;
>     if(b==c)
> 	a = 100.0;
>     else
>         a = 200.0;
> 
>     while(c>0)
>         c=c-1;
> 
>     return 0;
> }


（二）用第一次试验的词法分析器，分析代码，得到结果。
对第一次词法分析程序做如下改进：
若为语法分析中定义的终结符，则输出终结符，否则输出类型。输出结果如下：
 
int
id
(
)
{
int
id
;
int
id
;
id
=
num
;
id
=
num
;
id
=
decimal
;
if
(
id
==
id
)
id
=
decimal
;
else
id
=
decimal
;
while
(
id
>
num
)
id
=
id
-
num
;
return
num
;
}
 
（三）将此结果作为语法分析器的输入得到产生式输出结果
 
中间省去若干行……
 
（四）构建语法树
在输出分析结果的同时输出dot语言。输出的 dot 语言 如下
(完整版见压缩包文件 Expression.txt)
 
digraph g{
node0[label="PR"]
......
node1207->node1310
node1208[label="SL"]
node1207->node1208
node1407[label="ES"]
node1310->node1407
node4809[label=";"]
node1407->node4809
node6845[label="eps"]
node1208->node6845
} 

采用graphviz作图,语法树如下
 
