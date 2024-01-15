# Thinking through handling collisions

## Case 1

Here we need to combine the values at 0 and 4

```
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
 ^
             ^
```

This is kind of a special case, as we are storing the result at the beginning, where the first pointer is.  
We can set the combined result at 0 and swap 4 and 9 and be done with it.

Let's set the start to the combined result

```
[0&4, 1, 2, 3, 4, 5, 6, 7, 8, 9]
  ^
               ^
```

Swap 4 with 9, aka the end

```
[0&4, 1, 2, 3, 9, 5, 6, 7, 8, 4]
  ^                         |
               ^            |
```

Finally, we just need to increment the first pointer and reset the second

```
[0&4, 1, 2, 3, 9, 5, 6, 7, 8, 4]
      ^                     |
         ^                  |
```

## Case 2

Need to combine values at 2 and 4

```
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    ^
             ^
```

Let's place the result at the start and keep a copy of the start

```
temp = 0
[1&3, 1, 2, 3, 4, 5, 6, 7, 8, 9]
      ^
               ^
```

The indexes 1 and 4 are now duds and should be removed
If 1 is set to temp

```
temp = 0
[1&3, 0, 2, 3, 4, 5, 6, 7, 8, 9]
      ^
               ^
```

Then we can swap 3 with the last

```
[1&3, 0, 2, 3, 9, 5, 6, 7, 8, 4]
      ^                     |
               ^            |
```

Increment the first pointer and reset the second  
We do not need to re-handle 0 as it has already been handled

```
[1&3, 0, 2, 3, 9, 5, 6, 7, 8, 4]
         ^                  |
            ^               |
```

## Case 3

Need to combine values at 2 and 4

```
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
       ^
             ^
```

Let's start off the same way by storing a copy and setting the combined result at the start

```
temp = 0
[2&5, 1, 2, 3, 4, 5, 6, 7, 8, 9]
         ^
               ^
```

Indexes 2 and 4 are duds  
Set the first index to temp

```
temp = 0
[2&5, 1, 0, 3, 4, 5, 6, 7, 8, 9]
         ^
               ^
```

Swap 4 with the last

```
[2&5, 1, 0, 3, 9, 5, 6, 7, 8, 4]
         ^                  |
               ^            |
```

Increment the first pointer and reset the second

```
[2&5, 1, 0, 3, 9, 5, 6, 7, 8, 4]
            ^               |
               ^            |
```

I just realised that this is the same as [Case 2](#case-2)
