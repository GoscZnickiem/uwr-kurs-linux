for d in /usr/share/doc/*; do
    [ -d "$d" ] || continue
    pkg="${d##*/}"
    if ! pacman -Qq "$pkg" &>/dev/null; then
        owner=$(pacman -Qo "$d" 2>/dev/null | awk '{print $5}')
        printf "%s -> %s\n" "$pkg" "${owner:-brak właściciela}"
    fi
done
