# Konstructor Code style

### Variables           - snake case
### Functions           - camel case
### Class/Struct name   - pascal case

While and For follows same rule as if statements

### Conditions:
#### each case has only one line of code

```
if(condition)
    oneLineCode;
else
    oneLineCode;
```
#### at least one case has two or more lines of code
```
if(condition)
{
    Code;
    Code;
}
else
{
    Code;
    Code;
}
```
#### more then one condition
```
if(condition1 = value1              &
    condition2 = value2             &
    condition3 = value3             &
    longConditionName = longValue   )
{
    Code;
}
```
#### note: if only two coditions, they can be left on one line depending on their length

### Switch:
```
switch(variable)
{
    case value:
        Code;
        Code;
        break;
    case value:
        Code;
        break;
    default:
        break;
}
```

### Functions:
```
function funcName()
{
    Code;
    Code;
}
```
#### if two or more parameters, give each one new line
##### definition
```
function funcName(parameter1,
                    parameter2,
                    parameter3,
                    parameter4,
                    parameter5)
{
    Code;
}
```
##### declaration
```
function funcName(parameter1,
                    parameter2,
                    parameter3,
                    parameter4,
                    parameter5
                    );

```
##### calling
```
funcName(parameter1,
            parameter2,
            parameter3,
            parameter4,
            parameter5
        );
```
#### note: if only two parameters, they can be left on one line depending on their length


### Pointers:
```
int *variable;
int *function(int *parameter);
int *class::foo(){
}
```

### Variables and functions in class:
If calling member variable, always use keyword 'this'.
Only local variables can be used without it.
Function are always used without keyword 'this'
#### calling many funcions / accessing many members
```
object
    ->fucntion()
    ->function()
    ->variable
    ->function()
    ->variable;
```
