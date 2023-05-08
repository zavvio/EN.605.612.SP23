PROG=		mkfs.hw4fsv3

# XXX consider section 8
MAN=		mkfs.mfs.1

.if !defined(HOSTPROGNAME)
# The default MFS version is v3
SYMLINKS+=	${BINDIR}/${PROG} /usr/sbin/newfs_hw4fs
SYMLINKS+=	${BINDIR}/${PROG} /sbin/mkfs.hw4
MLINKS+=	mkfs.mfs.1 newfs_mfs.1
.endif

.include <../Makefile.mkfs>
