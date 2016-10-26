# Dircount assignment
## CS426

1 - prints every full pathname

1 - prints a [f] or a [d] next to the path depending on type

1 - counts the total number of files

1 - counts the total number of directories

1 - Never count the same file twice, even if there are links.

1 - does not go into an infinite loop even if the diretcory structure has loops

1 - prints the total on disk space used. (NOT the sum of all the file sizes)

-1 - Each day after Wed Oct 26th.


There is no way to tell the exact amount of disk space used by a file. However, st_blocks * 512 bytes is a pretty good guess.

The way to detect duplicate entries is to examine the st_ino structure. Each link should print the path name, but should only count once for the totals.

There are hard links and soft links. Soft links are read using readlink. See https://linux.die.net/man/2/readlink.

The article at http://stackoverflow.com/questions/8436841/how-to-recursively-list-directories-in-c-on-linux might be helpful.

You can find the inode number of a file by typing "ls -li filename".

You may assume that no path exceeds 2048 bytes long.

Calling readdir on a link will fail. Use readlink.

+ test1 - has a single, empty file.
+ test2 - has a subdir that has a file within it.
+ test3 -- Has two hard links that are the same file.
+ test4 -- Has a loop for directory soft links.
+ test5 -- Has a soft links that points to a normal file.
+ test6 -- Has a link the loops back, and several files scattered around.
+ test7 -- Has a different link loop.