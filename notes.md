# Notes
## Input Handling
### " and ' in combo with echo
- if only 1 of either in input, we have to go into some kind of heredoc
## echo command
- prints out everything behind it
- stops printing and goes to next command when &&, ||

## leaks
- split.c leaks on line 84, 85 because the var created by ft_substr is not freed