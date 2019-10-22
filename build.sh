set -e
set -x

case $(uname -s) in
	Linux)
		LIBLTO=/usr/lib/gcc/avr/4.9.2/liblto_plugin.so
		./configure --host=avr CXXFLAGS=" -flto" CFLAGS=" -flto"       ARFLAGS="--plugin $LIBLTO" RANLIB="avr-ranlib --plugin $LIBLTO"
		;;
	Darwin)
		LIBLTO=/usr/local//Cellar/avr-gcc/9.1.0/libexec/gcc/avr/9.1.0/liblto_plugin.so
		./configure --host=avr CXXFLAGS=" -flto" CFLAGS=" -flto"       ARFLAGS="--plugin $LIBLTO" RANLIB="avr-ranlib --plugin $LIBLTO"
		;;
	CYGWIN_NT-*)
		LIBLTO=/usr/lib/gcc/x86_64-pc-cygwin/7.4.0/cyglto_plugin.dll
		./configure --host=avr CXXFLAGS=" -flto" CFLAGS=" -flto" AR=ar ARFLAGS="--plugin $LIBLTO" RANLIB="ranlib --plugin $LIBLTO"
		;;
	*)
		echo "Unknown kernel" 1>&2
		exit 1
esac

make
make upload
