## 1. Issues
### 1.1 How to storage snake ?
A struct whitch include position array, and snake's length.
### 1.2 How to storage snake's food ?
A struct include food's coordinate and scores.

### 1.3 How to add a new cell to snake ?
Just add a new cell to head .

### 1.4 How to generate a snake's food ?
We can caluculate the total legal grids.
And we can generate a random number everytime.
We can let total grids sub snake's length.
Let d equal random number MOD remaining Numbers.
And we count i to d while grid can put food(not snake's body).
Finally, We put food on that grid. 


### 1.5 How to judge snake eat the food ?
Check snake's head's position and food's position equal.

### 1.6 How to let snake move automatical ?
Using non-blocking IO and do ticks.

### 1.7 How to check snake don't hit itself ?
Check snake's head's next position not in current pos(0,length);

