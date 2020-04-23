inherit cmake

DEPENDS = "\
    azure-c-shared-utility \
"

SRC_URI = "file://Fix-cmake-build-issues.patch \
           file://Fix-missing-installed-libs.patch \
           file://0001-Fix-target-link-libraries-for-serializer.patch \
           "

S = "${WORKDIR}/git"
B = "${WORKDIR}/build"

PACKAGECONFIG ??= "amqp mqtt service_client"
PACKAGECONFIG[amqp] = "-Duse_amqp:BOOL=ON, -Duse_amqp:BOOL=OFF, azure-uamqp-c"
PACKAGECONFIG[mqtt] = "-Duse_mqtt:BOOL=ON, -Duse_mqtt:BOOL=OFF, azure-umqtt-c"
PACKAGECONFIG[service_client] = "-Dbuild_service_client:BOOL=ON, -Dbuild_service_client:BOOL=OFF"

EXTRA_OECMAKE = "\
    -Dbuild_as_dynamic:BOOL=ON \
    -Dskip_samples:BOOL=ON \
"

# See solution at https://stackoverflow.com/a/59029698/1446624
do_configure_prepend() {
	cd ${S}
	git submodule update --init --recursive
}
sysroot_stage_all_append () {
    sysroot_stage_dir ${D}${exec_prefix}/cmake ${SYSROOT_DESTDIR}${exec_prefix}/cmake
}

FILES_${PN}-dev += "\
    ${exec_prefix}/cmake \
"

RDEPENDS_${PN} += "\
	azure-uamqp-c \
"

BBCLASSEXTEND = "native nativesdk"
