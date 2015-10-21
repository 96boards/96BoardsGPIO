conf="/etc/libsoc_gpio.conf"
[ -f $conf ] || conf="/usr/local$conf"
[ -f $conf ] || echo "ERROR: Unable to find libsoc_gpio.conf"

export $(cat $conf | sed -e '/^#/d' -e 's/ //g')
