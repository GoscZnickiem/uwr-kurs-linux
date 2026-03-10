for pkg in $(pacman -Qq); do
    log=$(pacman -Qc "$pkg" 2>/dev/null)
    [ -z "$log" ] && continue

    if [ "$(printf "%s\n" "$log" | grep -Ec '^[0-9]{4}-[0-9]{2}-[0-9]{2}')" -eq 1 ]; then
        echo "$pkg"
    fi
done
