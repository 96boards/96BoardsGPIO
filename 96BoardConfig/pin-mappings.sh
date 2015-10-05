while read line; do
	if [ ${line:0:1} != '#' ] ; then
		parts=($line)
		eval ${parts[2]}=${parts[0]}
	fi
done < /etc/96boards_gpio.conf
