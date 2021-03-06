#!/bin/bash
Time=`date +"%F__%T"`

eval $(awk -F: -v sum=0 '{if ($3 >= 1000 && $3 != 65534) {sum+=1; printf("ALL["sum"]=%s;",$1)}}\
        END {printf("UserSum=%d\n", sum)}' /etc/passwd)

MostActiveUser=`last -w | cut -d " " -f 1 | grep -v wtmp | grep -v reboot | grep -v "^$" | sort | 
    uniq -c | sort -k 1 -n -r | awk -v num=3 '{if(num>0) {printf(" %s", $2); num--}}' | cut -c 2-`

eval $(awk -F: '{if($3==1000) printf("User_1000=%s", $1)}' /etc/passwd)
UserWithRoot=${User_1000}
Users=`cat /etc/group | grep sudo | cut -d : -f 4 | tr "," " "`

for i in ${Users};do
    if [[ $i == ${User_1000} ]]; then
        continue
    fi
    UserWithRoot="${UserWithRoot},$i"
done

if [[ -r /etc/sudoers ]]; then
    for i in ${ALL[*]};do
        grep -q -w "^${i}" /etc/sudoers
        if [[ $? -eq 0 ]]; then
            UserWithRoot="${UserWithRoot},$i"
        fi
    done
fi

UserLogedIn=`w -h | awk '{printf(",%s_%s_%s", $1, $3, $2)}' | cut -c 2-`

echo "${Time} ${UserSum} [${MostActiveUser}] [${UserWithRoot}] [${UserLogedIn}]"

