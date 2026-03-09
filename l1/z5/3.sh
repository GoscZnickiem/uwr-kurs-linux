for pkg in $(pacman -Qq); do                    
	if pacman -Qc "$pkg" 2>/dev/null | grep -q .; then
		echo "$pkg"
	fi
done
