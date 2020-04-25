#!/bin/bash

df -m | grep "^/dev/" | awk -v nowtime=$(date +"%Y-%m-%d__%H:%M:%S") '
BEGIN{
total=0;
used=0;
available=0;
}
{
    total += $2;
    used += $3;
    available += $4;
    printf("%s 1 %s %s %s %s\n", nowtime, $6, $2, $4, $5);
}
END{
printf("%s 0 disk %d %d %.2f%%\n", nowtime, total, available, used / total * 100);
}'
