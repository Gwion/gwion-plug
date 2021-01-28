LIST=$(for a in $(bash -c "find gwion-soundpipe -type f -name '*.h'" )
do dirname $a
done | sort -u)

for a in $LIST
do printf -- '-I%s ' $a
done
