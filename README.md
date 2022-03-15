# directoryManager
Removes oldest created files from linux directory if directory total usage size crosses predefined threshold

Initial values:
1. Maximum capacity of required directory
2. Threshold memory(i.e. max directory size allowed) 
3. Directory path

Working of program:
1.First it will check if the directory memory size is above threshold
2.If current directory memory is above thresold it will remove oldest created file from the directory 
3.And deletion of files will continue untill current memory is reduced to below threshold
