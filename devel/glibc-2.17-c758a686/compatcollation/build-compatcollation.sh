#!/bin/bash
err_exit() {
  if [ ${1} -gt 0 ]; then
    echo "************* Failed      *************"
    echo "************* Exit Code $? *************"
    exit $1
  fi
}

skip_comment() {
  line="$1"
  if [[ ${line::1} == "#" ]]
  then
    continue
  fi
}

if [ -z "${COMPATPREFIX}" ] ; then
  echo "COMPATPREFIX is undefined"
  exit 1
fi
if [ -z "${BUILDROOT}" ] ; then
  echo "BUILDROOT is undefined"
  exit 1
fi
if [ -z "${BUILDDIR}" ] ; then
  echo "BUILDDIR is undefined"
  exit 1
fi
if [ -z "${GLIBCVERSION}" ] ; then
  echo "GLIBCVERSION is undefined"
  exit 1
fi
if [ -z "${GLIBCRELEASE}" ] ; then
  echo "GLIBCRELEASE is undefined"
  exit 1
fi
if [ -z "${COMPATCOLLATIONVERSION}" ] ; then
  echo "COMPATCOLLATIONVERSION is undefined"
  exit 1
fi

GLIBCRELEASE_STR="'\"${GLIBCRELEASE}\"'"
BUILDOUT="${BUILDROOT}/compatcollation/glibc-rpmbuild.out"
BUILDFILES="${BUILDROOT}/compatcollation/buildfiles.txt"

mkdir -p ${BUILDDIR}/compatcollation

ULL="/usr/lib/locale"
ULLC="${COMPATPREFIX}/lib/locale-compatcollation${COMPATCOLLATIONVERSION}"
USL="/usr/share/locale"
USLC="${COMPATPREFIX}/share/locale-compatcollation${COMPATCOLLATIONVERSION}"
ULG="/usr/lib64/gconv"
ULGC="${COMPATPREFIX}/lib64/gconv-compatcollation${COMPATCOLLATIONVERSION}"
ULeG="/usr/libexec/getconf"
ULeGC="${COMPATPREFIX}/libexec/getconf-compatcollation${COMPATCOLLATIONVERSION}"
GCCSRCVER="7"
GCCTGTVER="7"

if [[ "$1" == "" ]]
then
lastdir=""
while read currpath
do
  skip_comment "${currpath}"
  currdir=$(dirname ${currpath})
  currbase=$(basename ${currpath})
  if [[ ${currdir} != ${lastdir} ]]
  then
    echo "changing directory to ${BUILDROOT}/${currdir}"
    cd ${BUILDROOT}/${currdir}
  fi
  lastdir=${currdir}

  # special cases
  if [[ "${currdir}/${currbase}" == "misc/mremap" || 
        "${currdir}/${currbase}" == "misc/munmap" ||
        "${currdir}/${currbase}" == "posix/sched_yield" ||
        "${currdir}/${currbase}" == "socket/socket" ||
        "${currdir}/${currbase}" == "misc/madvise" ||
        "${currdir}/${currbase}" == "misc/mprotect"
     ]]
  then
    GCCCOMPSTR="$(grep -B6 -Ee '^\) \| gcc \-c.+?'${currdir}/${currbase}.os'\s$' ${BUILDOUT})"

    # decruft the string
    DIN="\; \\\\"
    DOU="\; "
    GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")
  elif [[ "${currdir}/${currbase}" == "misc/syscall_clock_gettime" ]]
  then
    GCCCOMPSTR="$(grep -B5 -Ee '^\) \| gcc \-c.+?'${currdir}/${currbase}.os'\s$' ${BUILDOUT})"

    # decruft the string
    DIN="\; \\\\"
    DOU="\; "
    GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")
  elif [[ "${currdir}/${currbase}" == "misc/sysinfo" ]]
  then
    GCCCOMPSTR="$(grep -B4 -Ee '^\) \| gcc \-c.+?'${currdir}/${currbase}.os'\s$' ${BUILDOUT})"

    # decruft the string
    DIN="\; \\\\"
    DOU="\; "
    GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")
  elif [[ "${currdir}/${currbase}" == "io/open" ]]
  then
    GCCCOMPSTR="$(grep -B13 -Ee '^\) \| gcc \-c.+?'${currdir}/${currbase}.os'\s$' ${BUILDOUT})"

    # decruft the string
    DIN="\; \\\\"
    DOU="\; "
    GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")
  elif [[ "${currdir}/${currbase}" == "resource/getrlimit" ]]
  then
    GCCCOMPSTR="$(grep -B8 -Ee '^\) \| gcc \-c.+?'${currdir}/${currbase}.os'\s$' ${BUILDOUT})"

    # decruft the string
    DIN="\; \\\\"
    DOU="\; "
    GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")
  elif [[ "${currdir}/${currbase}" == "io/read" ||
          "${currdir}/${currbase}" == "io/write" ||
          "${currdir}/${currbase}" == "io/close" ||
          "${currdir}/${currbase}" == "socket/connect"
       ]]
  then
    GCCCOMPSTR="$(grep -B9 -Ee '^\) \| gcc \-c.+?'${currdir}/${currbase}.os'\s$' ${BUILDOUT})"

    # decruft the string
    DIN="\; \\\\"
    DOU="\; "
    GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")
  elif [[ "${currdir}/${currbase}" == "misc/llseek" ]]
  then
    GCCCOMPSTR="$(grep -B19 -Ee '^\) \| gcc \-c.+?'${currdir}/${currbase}.os'\s$' ${BUILDOUT})"

    # decruft the string
    DIN="\; \\\\"
    DOU="\; "
    GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")
  # Now process the usual suspects
  else
    GCCCOMPSTR=$(grep -E "^gcc.+?${currbase}\.(c|S).+?${currdir}/${currbase}.os\s?$" ${BUILDOUT})
    # some source files are *.S for assembly
  fi

  # fix buildtime defined paths
  DIN="DCOMPLOCALEDIR='\"${ULL}\"'"
  DOU="DCOMPLOCALEDIR='\"${ULLC}\"'"
  GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")

  DIN="DLOCALEDIR='\"${ULL}\"'"
  DOU="DLOCALEDIR='\"${ULLC}\"'"
  GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")

  DIN="DLOCALE_ALIAS_PATH='\"${USL}\"'"
  DOU="DLOCALE_ALIAS_PATH='\"${USLC}\"'"
  GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")

  DIN="D'LOCALE_ALIAS_PATH=\"${USL}\"'"
  DOU="D'LOCALE_ALIAS_PATH=\"${USLC}\"'"
  GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")

  DIN="D'LOCALEDIR=\"${USL}\"'"
  DOU="D'LOCALEDIR=\"${USLC}\"'"
  GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")

  DIN="DGCONV_DIR='\"${ULG}\"'"
  DOU="DGCONV_DIR='\"${ULGC}\"'"
  GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")

  DIN="DGCONV_PATH='\"${ULG}\"'"
  DOU="DGCONV_PATH='\"${ULGC}\"'"
  GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")

  DIN="DGETCONF_DIR='\"${ULeG}\"'"
  DOU="DGETCONF_DIR='\"${ULeGC}\"'"
  GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")

  # fix system include path based on gcc version
  DIN="usr/lib/gcc/x86_64-redhat-linux/${GCCSRCVER}/include"
  DOU="usr/lib/gcc/x86_64-redhat-linux/${GCCTGTVER}/include"
  GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")

  # inject -DLIBCOMPATCOLL_MODE and version string suffix
  DIN="\-DMODULE_NAME=libc"
  DOU="\-DMODULE_NAME=libc \-DLIBCOMPATCOLL_MODE \-DGLIBCRELEASE=$(echo ${GLIBCRELEASE_STR})"
  GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")

  # modify the object extension to our own
  DIN="${currdir}/${currbase}.os"
  DOU="${currdir}/${currbase}.oc"
  GCCCOMPSTR=$(sed -e "s:${DIN}:${DOU}:g" <<< "${GCCCOMPSTR}")

  # output for the record
  echo ${GCCCOMPSTR}
  # and run it
  eval ${GCCCOMPSTR}
  err_exit $?
done <<< "$(cat ${BUILDFILES})"
cd ${BUILDROOT}
fi #skip if arg passed

arstr=""
while read currpath
do
  skip_comment "${currpath}"
  currdir=$(dirname ${currpath})
  currbase=$(basename ${currpath})

  arstr="${arstr} ${currdir}/${currbase}.oc"
done <<< "$(cat ${BUILDFILES})"
cd ${BUILDROOT}

# create object archive
cd ${BUILDDIR}
echo "ar cruv libcompatcollation_pic.a ${arstr}"
rm -f libcompatcollation_pic.a
ar cruv libcompatcollation_pic.a $(echo ${arstr})
err_exit $?

# create libcompatcollation_pic.os
gcc -nostdlib -nostartfiles -r \
-Wl,-d -Wl,--whole-archive ${BUILDDIR}/libcompatcollation_pic.a \
-o ${BUILDDIR}/libcompatcollation_pic.os
err_exit $?

# build libcompatcollation.so library
cd ${BUILDROOT}

ENABLE_LD_PRELOAD=0
if [[ "${ENABLE_LD_PRELOAD}" == "1" ]]
then
  VERSIONMAP="${BUILDDIR}/libc.map"
else
  VERSIONMAP="${BUILDROOT}/compatcollation/libcompatcollation.map"
fi

# link
GCCCOMPSTR="gcc -shared -Wl,-Bsymbolic \
-Wl,-O1 -Wl,-z,defs -fPIC \
-Wl,--version-script=${VERSIONMAP} \
-Wl,-soname=libcompatcollation.${GLIBCVERSION}-${GLIBCRELEASE}.so -Wl,-z,combreloc \
-Wl,-z,relro -Wl,--hash-style=both -Wl,-z,now \
-L${BUILDDIR} -L${BUILDDIR}/math -L${BUILDDIR}/elf -L${BUILDDIR}/dlfcn \
-L${BUILDDIR}/nss -L${BUILDDIR}/nis -L${BUILDDIR}/rt -L${BUILDDIR}/resolv -L${BUILDDIR}/crypt \
-L${BUILDDIR}/support -L${BUILDDIR}/nptl \
-Wl,-rpath-link=${BUILDDIR}:${BUILDDIR}/math:${BUILDDIR}/elf:${BUILDDIR}/dlfcn:${BUILDDIR}/nss:${BUILDDIR}/nis:${BUILDDIR}/rt:${BUILDDIR}/resolv:${BUILDDIR}/crypt:${BUILDDIR}/support:${BUILDDIR}/nptl \
-o ${BUILDDIR}/libcompatcollation.${GLIBCVERSION}-${GLIBCRELEASE}.so \
${BUILDDIR}/libcompatcollation_pic.os -ldl"

# output for the record
echo ${GCCCOMPSTR}
# and run it
eval ${GCCCOMPSTR}

err_exit $?

echo "Created: ${BUILDDIR}/libcompatcollation.${GLIBCVERSION}-${GLIBCRELEASE}.so"
