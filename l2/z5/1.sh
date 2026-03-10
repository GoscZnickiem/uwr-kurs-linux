pacman -Qq | awk '{                             
    cmd="pacman -Ql " $1 " | grep -q \"^" $1 " /usr/share/doc/\""
    if (system(cmd) != 0) print $1
}'
