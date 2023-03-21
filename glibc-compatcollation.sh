#!/bin/bash

TOPDIR=$(rpm --eval '%{_topdir}')
RPMBUILD=$(which rpmbuild)
BRANCH=2.17-326.el7
BASELINE_TAG=${BRANCH}-BASELINE
LAST_RELEASE_TAG=${BRANCH}-BASELINE
GLIBC_DIR=glibc-2.17-c758a686
DEV_DIR=devel/${GLIBC_DIR}

if [ -z "${RPMBUILD}" ]; then
    echo "rpmbuild command not found"
    exit 1
fi
if [ -z "${TOPDIR}" ]; then
    echo "_topdir not defined"
    exit 1
fi

usage() {
    echo
    echo "$1"
    echo
    echo "Usage: ./glibc-compatcollation.sh <command>"
    echo "command:"
    echo "	build - build binary RPM"
    echo "	extra build options can be passed using env BUILD_EXTRAS"
    echo
    echo "	diff - Show diff from Last RELEASE TAG (including uncommiteed changes)"
    echo "	diff <TAG|COMMIT> - Show diff from RELEASE TAG to given TAG or COMMIT"
    echo "	diff <TAG1|COMMIT1> <TAG2|COMMIT2> - Show diff between TAG1 and  TAG2 or COMMIT1 and COMMIT2"
    echo
    echo "	patch - Generate RPM patches"
    echo
    exit 2
}

build_rpm() {
    for d in SOURCES SPECS
    do
       mkdir -p ${TOPDIR}/${d}/
       rsync -avrP --delete ${d}/ ${TOPDIR}/${d}/
    done
    eval rpmbuild -bb ${BUILD_EXTRAS} ${TOPDIR}/SPECS/glibc.spec
}

rebuild_libs() {
   rsync -avrP ${DEV_DIR}/ ${TOPDIR}/BUILD/${GLIBC_DIR}/
   chmod 755 ${TOPDIR}/BUILD/${GLIBC_DIR}/compatcollation/build-compatcollation.sh
BUILDROOT=${TOPDIR}/BUILD/${GLIBC_DIR} \
BUILDDIR=${TOPDIR}/BUILD/${GLIBC_DIR}/build-x86_64-redhat-linux \
INSTALLROOT=${TOPDIR}/BUILDROOT/${GLIBC_DIR} \
COMPATPREFIX=/usr \
GLIBCVERSION=2.17 \
GLIBCRELEASE=326.el7_9 \
COMPATCOLLATIONVERSION=217326 \
${TOPDIR}/BUILD/${GLIBC_DIR}/compatcollation/build-compatcollation.sh

}

show_diff() {
    if [ -z "$2" ]; then
        git diff ${LAST_RELEASE_TAG} -- devel SPECS
    elif [ -z "$3" ]; then
        git diff ${LAST_RELEASE_TAG}..${2} -- devel SPECS
    elif [ -z "$4" ]; then
        git diff ${2}..${3} -- devel SPECS
    fi
}

rpmpatch() {
	(cd $DEV_DIR; git diff --relative --patch ${BASELINE_TAG}..HEAD -- . :^./compatcollation) >SOURCES/9991-compatcollation-glibc.patch
	(cd $DEV_DIR; git diff --relative --patch ${BASELINE_TAG}..HEAD -- compatcollation) >SOURCES/9992-compatcollation-build.patch
}

case "$1" in
    build)
        build_rpm
      ;;
    rebuild)
        rebuild_libs
      ;;
    diff)
        show_diff
      ;;
    rpmpatch)
        rpmpatch
      ;;
    *)
        usage "ERROR: Invalid command $1"
      ;;
esac

